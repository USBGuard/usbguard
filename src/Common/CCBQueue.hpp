//
// Copyright (C) 2015 Red Hat, Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors: Daniel Kopecek <dkopecek@redhat.com>
//
#ifndef CCBQUEUE_HPP
#define CCBQUEUE_HPP

#include <stdint.h>
#include <stddef.h>
#include <time.h>

#ifndef CCBQUEUE_STATE_BITS
# define CCBQUEUE_STATE_BITS 32
#endif

namespace CCBQueueOpt
{
  enum BlockingMethod {
    NoBlocking,
    Spinlock,
    Yield,
    ConstSleep,
    AdaptiveSleep
  };
}

template<typename T,
	 CCBQueueOpt::BlockingMethod = CCBQueueOpt::NoBlocking,
	 const struct timespec * sleep_ts = static_cast<const struct timespec *>(nullptr)>
class CCBQueue
{
public:
#if CCBQUEUE_STATE_BITS == 64
  typedef uint32_t index_type;
  typedef uint32_t count_type;
  typedef uint64_t state_type;
  const index_type invalid_index = UINT32_MAX;
  const count_type count_type_max = UINT32_MAX;
#elif CCBQUEUE_STATE_BITS == 32
  typedef uint16_t index_type;
  typedef uint16_t count_type;
  typedef uint32_t state_type;
  const index_type invalid_index = UINT16_MAX;
  const count_type count_type_max = UINT16_MAX;
#elif CCBQUEUE_STATE_BITS == 16
  typedef uint8_t index_type;
  typedef uint8_t count_type;
  typedef uint16_t state_type;
  const index_type invalid_index = UINT8_MAX;
  const count_type count_type_max = UINT8_MAX;
#else
# error "Invalid CCBQUEUE_STATE_BITS value. Must be one of: 64, 32, 16."
#endif
private:
  union State {
    struct {
      index_type begin;
      count_type count;
    };
    state_type unified;
  };

  uint8_t* _queue_mem;
  const size_t _item_size;
  const count_type _capacity;

  State _r_state; // readability state
  State _w_state; // writability state

protected:
  T* indexToPointer(index_type index) const __attribute__((/*returns_nonnull,*/ nothrow, hot))
  {
    return (T *)(((uintptr_t)_queue_mem) + (_item_size * index));
  }

  index_type pointerToIndex(const T* pointer) const __attribute__((nonnull, nothrow, hot))
  {
    return (index_type)((((uintptr_t)pointer) - ((uintptr_t)_queue_mem)) / _item_size);
  }

  // Modifies write state (++count)
  index_type acquireFreeIndex() __attribute__((nothrow))
  {
    State w_state_old, w_state_new;
    do {
      // Read the current state
      w_state_old.unified = _w_state.unified;
      // Check whether there's scape for a new item
      if (w_state_old.count >= _capacity) {
	// No free index
	return invalid_index;
      }
      // Create a new state
      w_state_new.unified = w_state_old.unified;
      ++w_state_new.count;
      //
      // Try to atomically update the state, repeat the
      // whole process if it fails
      //
    } while(!__sync_bool_compare_and_swap(&_w_state.unified,
					  w_state_old.unified,
					  w_state_new.unified));

    return ((w_state_old.begin + w_state_old.count) % _capacity);
  }

  // Modifies read state (++begin, --count)
  index_type acquireReadIndex() __attribute__((nothrow))
  {
    State r_state_old, r_state_new;
    do {
      // Read the current state
      r_state_old.unified = _r_state.unified;
      // Check whether there's something to read
      if (r_state_old.count == 0) {
	return invalid_index;
      }
      // Create a new state
      r_state_new.unified = r_state_old.unified;
      --r_state_new.count;
      // Wrap the .begin index if needed
      if (++r_state_new.begin == _capacity) {
	r_state_new.begin = 0;
      }
      //
      // Try to atomically update the state, repeat the
      // while process if it fails
      //
    } while(!__sync_bool_compare_and_swap(&_r_state.unified,
					  r_state_old.unified,
					  r_state_new.unified));
    return r_state_old.begin;
  }

  index_type nextEnqueueIndex() __attribute__((nothrow))
  {
    State r_state;
    r_state.unified = __sync_add_and_fetch(&_r_state.unified, 0);
    return ((r_state.begin + r_state.count) % _capacity);
  }

  index_type nextReleaseIndex() __attribute__((nothrow))
  {
    State w_state;
    w_state.unified = __sync_add_and_fetch(&_w_state.unified, 0);
    return w_state.begin;
  }

public:
  CCBQueue(uint32_t max_memory_MiB)
    : _item_size(sizeof(T)),
      _capacity(((max_memory_MiB*1<<20)/sizeof(T))>count_type_max?
		count_type_max:(max_memory_MiB*1<<20)/sizeof(T))
  {
    _queue_mem = new uint8_t[_capacity * _item_size];
    _w_state.unified = 0;
    _r_state.unified = 0;
  }

  ~CCBQueue()
  {
    delete [] _queue_mem;
  }

  T* acquire() __attribute__((nothrow))
  {
    const index_type free_index = acquireFreeIndex();
    if (free_index == invalid_index) {
      /* No free space */
      return nullptr;
    }
    else {
      return indexToPointer(free_index);
    }
  }

  // Modifies read state (++count)
  bool enqueue(const T* item) __attribute__((nonnull, nothrow))
  {
    // Check that we can enqueue the item
    if (pointerToIndex(item) != nextEnqueueIndex()) {
      return false;
    }
    // Enqueue the item / Update readability state
    State r_state_old, r_state_new;

    do {
      r_state_old.unified = _r_state.unified;
      r_state_new.unified = r_state_old.unified;
      ++r_state_new.count;
    } while(!__sync_bool_compare_and_swap(&_r_state.unified,
					  r_state_old.unified,
					  r_state_new.unified));
    return true;
  }

  const T* dequeue() __attribute__((nothrow))
  {
    const index_type index = acquireReadIndex();
    if (index == invalid_index) {
      return nullptr;
    }
    else {
      return indexToPointer(index);
    }
  }

  // Modifies write state (++begin, --count)
  bool release(const T* item) __attribute__((nonnull, nothrow))
  {
    // Check that we can release the item
    if (pointerToIndex(item) != nextReleaseIndex()) {
      return false;
    }

    State w_state_old, w_state_new;

    do {
      // Read the current state
      w_state_old.unified = _w_state.unified;
      // Create an updated state
      w_state_new.unified = w_state_old.unified;
      --w_state_new.count;
      if (++w_state_new.begin == _capacity) {
	w_state_new.begin = 0;
      }
      // Atomically update the state
    } while(!__sync_bool_compare_and_swap(&_w_state.unified,
					  w_state_old.unified,
					  w_state_new.unified));
    return true;
  }

  // Returns number of queued items
  count_type count() const
  {
    return _r_state.count;
  }

};

#endif /* CCBQUEUE_HPP */
