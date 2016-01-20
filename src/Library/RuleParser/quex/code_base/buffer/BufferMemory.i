/* vim:set ft=c: -*- C++ -*- */
#ifndef __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_MEMORY_I
#define __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_MEMORY_I

#include <quex/code_base/asserts>
#include <quex/code_base/buffer/asserts>
#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/buffer/Buffer_debug.i>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/MemoryManager>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void 
QUEX_NAME(BufferMemory_construct)(QUEX_NAME(BufferMemory)*  me, 
                                  QUEX_TYPE_CHARACTER*      Memory, 
                                  const size_t              Size,
                                  E_Ownership               Ownership) 
{
    if( Memory ) {
        /* "Memory size > QUEX_SETTING_BUFFER_MIN_FALLBACK_N + 2" is reqired.
         * Maybe, define '-DQUEX_SETTING_BUFFER_MIN_FALLBACK_N=0' for 
         * compilation (assumed no pre-contexts.)                                */
        __quex_assert(Size > QUEX_SETTING_BUFFER_MIN_FALLBACK_N + 2);

        me->_front    = Memory;
        me->_back     = &Memory[Size-1];
        me->ownership = Ownership;
        *(me->_front) = QUEX_SETTING_BUFFER_LIMIT_CODE;
        *(me->_back)  = QUEX_SETTING_BUFFER_LIMIT_CODE;
    } else {
        me->_front    = (QUEX_TYPE_CHARACTER*)0;
        me->_back     = me->_front;
        me->ownership = Ownership;
    }
}

QUEX_INLINE void 
QUEX_NAME(BufferMemory_destruct)(QUEX_NAME(BufferMemory)* me) 
/* Does not set 'me->_front' to zero, if it is not deleted. Thus, the user
 * may detect wether it needs to be deleted or not.                          */
{
    if( me->_front && me->ownership == E_Ownership_LEXICAL_ANALYZER ) {
        QUEXED(MemoryManager_free)((void*)me->_front, 
                                   E_MemoryObjectType_BUFFER_MEMORY);
        /* Protect against double-destruction.                               */
        me->_front = me->_back = (QUEX_TYPE_CHARACTER*)0x0;
    }
}

QUEX_INLINE size_t          
QUEX_NAME(BufferMemory_size)(QUEX_NAME(BufferMemory)* me)
{ return (size_t)(me->_back - me->_front + 1); }

QUEX_NAMESPACE_MAIN_CLOSE

#endif /*  __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_MEMORY_I */
