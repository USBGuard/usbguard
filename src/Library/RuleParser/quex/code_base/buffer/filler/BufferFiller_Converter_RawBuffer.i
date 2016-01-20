/* -*- C++ -*-  vim: set syntax=cpp:
 * (C) 2007-2015 Frank-Rene Schaefer  */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_RAW_BUFFER_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_RAW_BUFFER_I

#include <quex/code_base/MemoryManager>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/buffer/filler/BufferFiller_Converter>
#include <quex/code_base/compatibility/iconv-argument-types.h>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void   
QUEX_NAME(RawBuffer_init)(QUEX_NAME(RawBuffer)* me, 
                          uint8_t*              Begin, 
                          size_t                SizeInBytes)
/* Initialize raw buffer. 
 * (1) Begin != 0 => Assign memory. 
 * (2) Begin == 0 => Only reset pointers, so buffer is 'empty'.              */
{
    if( Begin ) {
        me->begin      = Begin;
        me->memory_end = &Begin[(ptrdiff_t)SizeInBytes];
    }
    me->fill_end_p        = &me->begin[0];
    me->next_to_convert_p = &me->begin[0];            /* --> trigger reload. */

    QUEX_IF_ASSERTS_poison(me->begin, me->memory_end);
}

QUEX_INLINE void 
QUEX_NAME(RawBuffer_move_away_passed_content)(QUEX_NAME(RawBuffer)*  me)
/* Consider any content in the raw buffer from begin to 'next_to_convert_p' as
 * passed and useless. Thus, move what comes behind to the beginning of the 
 * buffer. Adapt:
 *
 *     -- '.fill_end_p'
 *     -- '.next_to_convert_p'
 *
 * The relation of '.next_to_convert_p' and '.next_to_convert_character_index' 
 * remains unaffected. The pointer still points to the same character index. */
{
    uint8_t*   move_begin_p;
    ptrdiff_t  move_size;
    ptrdiff_t  move_distance;
   
    __quex_assert(me->next_to_convert_p <= me->fill_end_p);
    QUEX_ASSERT_RAW_BUFFER(me);

    move_begin_p  = me->next_to_convert_p;
    move_size     = me->fill_end_p - me->next_to_convert_p;
    move_distance = me->next_to_convert_p - me->begin;

    if( ! move_distance ) {
        return;
    }
    else if( move_size ) {
        __QUEX_STD_memmove((void*)me->begin, (void*)move_begin_p, (size_t)move_size);
    }

    me->next_to_convert_p  = me->begin; 
    me->fill_end_p        -= move_distance;

    QUEX_IF_ASSERTS_poison(me->fill_end_p, me->memory_end);
    QUEX_ASSERT_RAW_BUFFER(me);
}

QUEX_INLINE bool 
QUEX_NAME(RawBuffer_load)(QUEX_NAME(RawBuffer)*  me,
                          QUEX_NAME(ByteLoader)* byte_loader, 
                          bool*                  end_of_stream_f)  
/* Try to fill the me buffer to its limits with data from the file.  The
 * filling starts from its current position, thus the remaining bytes to be
 * translated are exactly the number of bytes in the buffer.                 */
{
    uint8_t*  fill_begin_p;
    size_t    load_request_n;
    size_t    loaded_byte_n;

    QUEX_ASSERT_RAW_BUFFER(me);

    /* Move content that has not yet been converted to the buffer's begin.   */
    QUEX_NAME(RawBuffer_move_away_passed_content)(me);

    fill_begin_p    = me->fill_end_p;
    load_request_n  = (size_t)(me->memory_end - fill_begin_p);
    /* load(): Blocks until either bytes are received, or the end-of-stream
     *         occurs. In the latter case zero bytes are received. The end-
     *         of-stream, may also be detected in other cases--as hint.      */
    loaded_byte_n   = byte_loader->load(byte_loader, fill_begin_p, load_request_n, 
                                        end_of_stream_f);
    me->fill_end_p  = &fill_begin_p[loaded_byte_n];

    if( ! loaded_byte_n ) {
        *end_of_stream_f = true;
    }

    QUEX_ASSERT_RAW_BUFFER(me);
    return loaded_byte_n == load_request_n;
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_CONVERTER_RAW_BUFFER_I */
