/* -*- C++ -*- vim: set syntax=cpp: */

/* This file contains an implementation which can potentially be shared between
 * multiple different lexical analyzers. See 'multi.i' for further info.     */

#ifndef __QUEX_INCLUDE_GUARD__MEMORY_MANAGER_I
#define __QUEX_INCLUDE_GUARD__MEMORY_MANAGER_I

#include <quex/code_base/definitions>
#include <quex/code_base/MemoryManager>

QUEX_NAMESPACE_QUEX_OPEN

uint8_t*
QUEXED_DEF(MemoryManager_allocate)(const size_t                 ByteN, 
                                   QUEXED_DEF(MemoryObjectType) Type)
{
    uint8_t*  memory = (uint8_t*)__QUEX_STD_malloc((size_t)ByteN);
    (void)Type;
#   ifdef QUEX_OPTION_ASSERTS
    __QUEX_STD_memset((void*)memory, 0xFF, ByteN);
#   endif
    return memory;
}
       
void 
QUEXED_DEF(MemoryManager_free)(void*                        memory, 
                               QUEXED_DEF(MemoryObjectType) Type)  
{ 
    (void)Type;
    /* The de-allocator shall never be called for LexemeNull object.     */
    if( memory != (void*)0 ) {
        __QUEX_STD_free(memory); 
    }
}

size_t
QUEXED_DEF(MemoryManager_insert)(uint8_t* drain_begin_p,  uint8_t* drain_end_p,
                                 uint8_t* source_begin_p, uint8_t* source_end_p)
    /* Inserts as many bytes as possible into the array from 'drain_begin_p'
     * to 'drain_end_p'. The source of bytes starts at 'source_begin_p' and
     * ends at 'source_end_p'.
     *
     * RETURNS: Number of bytes that have been copied.                      */
{
    /* Determine the insertion size. */
    const size_t DrainSize = (size_t)(drain_end_p  - drain_begin_p);
    size_t       size      = (size_t)(source_end_p - source_begin_p);
    __quex_assert(drain_end_p  >= drain_begin_p);
    __quex_assert(source_end_p >= source_begin_p);

    if( DrainSize < size ) size = DrainSize;

    /* memcpy() might fail if the source and drain domain overlap! */
#   ifdef QUEX_OPTION_ASSERTS 
    if( drain_begin_p > source_begin_p ) __quex_assert(drain_begin_p >= source_begin_p + size);
    else                                 __quex_assert(drain_begin_p <= source_begin_p - size);
#   endif
    __QUEX_STD_memcpy(drain_begin_p, source_begin_p, size);

    return size;
}

QUEX_NAMESPACE_QUEX_CLOSE
 
#endif /*  __QUEX_INCLUDE_GUARD__MEMORY_MANAGER_I */


