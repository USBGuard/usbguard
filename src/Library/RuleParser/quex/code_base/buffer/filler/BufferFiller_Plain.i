/*  -*- C++ -*-  vim: set syntax=cpp: */
/* (C) 2008 Frank-Rene Schaefer*/
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_PLAIN_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_PLAIN_I
/**/

#if ! defined (__QUEX_OPTION_PLAIN_C)
#   include <iostream> 
#   include <cerrno>
#   include <stdexcept>
#endif
#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/MemoryManager>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void
QUEX_NAME(BufferFiller_Plain_construct)(QUEX_NAME(BufferFiller_Plain)*, QUEX_NAME(ByteLoader)* byte_loader);

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Plain_stomach_byte_n)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Plain_stomach_clear)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE void   
QUEX_NAME(BufferFiller_Plain_destruct_self)(QUEX_NAME(BufferFiller)* alter_ego);

QUEX_INLINE size_t 
QUEX_NAME(BufferFiller_Plain_load_characters)(QUEX_NAME(BufferFiller)* alter_ego,
                                                   QUEX_TYPE_CHARACTER*     RegionBeginP, 
                                                   const size_t             N,
                                                   bool*                    end_of_stream_f);

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Plain_fill_prepare)(QUEX_NAME(BufferFiller)*   alter_ego,
                                           QUEX_TYPE_CHARACTER*       RegionBeginP,
                                           QUEX_TYPE_CHARACTER*       RegionEndP,
                                           void**                     begin_p,
                                           const void**               end_p);

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Plain_fill_finish)(QUEX_NAME(BufferFiller)*   alter_ego,
                                          QUEX_TYPE_CHARACTER*       insertion_p,
                                          const QUEX_TYPE_CHARACTER* BufferEnd,
                                          const void*                ContentEnd);

QUEX_INLINE QUEX_NAME(BufferFiller)*
QUEX_NAME(BufferFiller_Plain_new)(QUEX_NAME(ByteLoader)* byte_loader)
{
    QUEX_NAME(BufferFiller_Plain)*  me = \
         (QUEX_NAME(BufferFiller_Plain)*) \
          QUEXED(MemoryManager_allocate)(sizeof(QUEX_NAME(BufferFiller_Plain)),
                                         E_MemoryObjectType_BUFFER_FILLER);
    __quex_assert(me);
    /* __quex_assert(byte_loader); not for manual filling. */

    QUEX_NAME(BufferFiller_Plain_construct)(me, byte_loader);

    return &me->base;
}

QUEX_INLINE void
QUEX_NAME(BufferFiller_Plain_construct)(QUEX_NAME(BufferFiller_Plain)* me, 
                                        QUEX_NAME(ByteLoader)*         byte_loader)
{
    /* A linear relationship between stream position and character index 
     * requires that the input stream is in 'binary mode'. That is, the 
     * stream position is proportional to the number of bytes that lie 
     * behind.                                                               */
    ptrdiff_t   byte_n_per_character = byte_loader && byte_loader->binary_mode_f ? 
                                       (ptrdiff_t)sizeof(QUEX_TYPE_CHARACTER) : -1;
    QUEX_NAME(BufferFiller_setup)(&me->base,
                                  QUEX_NAME(BufferFiller_Plain_load_characters),
                                  QUEX_NAME(BufferFiller_Plain_stomach_byte_n),
                                  QUEX_NAME(BufferFiller_Plain_stomach_clear),
                                  QUEX_NAME(BufferFiller_Plain_destruct_self), 
                                  QUEX_NAME(BufferFiller_Plain_fill_prepare), 
                                  QUEX_NAME(BufferFiller_Plain_fill_finish), 
                                  byte_loader,
                                  byte_n_per_character);
}

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Plain_stomach_byte_n)(QUEX_NAME(BufferFiller)* alter_ego) 
{
    (void)alter_ego;
    return (ptrdiff_t)0;
}

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Plain_stomach_clear)(QUEX_NAME(BufferFiller)* alter_ego) 
{
    (void)alter_ego;
}

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Plain_destruct_self)(QUEX_NAME(BufferFiller)* alter_ego) 
/* destruct_self: Free resources occupied by 'me' BUT NOT 'myself'.
 * delete_self:   Free resources occupied by 'me' AND 'myself'.              */
{
    (void)alter_ego;
    /* There is nothing allocated by a BufferFiller_Plain. 
     * Nothing to do.                                                        */
}

QUEX_INLINE size_t   
QUEX_NAME(BufferFiller_Plain_load_characters)(QUEX_NAME(BufferFiller)*  alter_ego,
                                                   QUEX_TYPE_CHARACTER* RegionBeginP, 
                                                   const size_t         N,
                                                   bool*                end_of_stream_f)  
/* Loads content into a region of memory. Does NOT effect any of the buffer's
 * variables. 
 *
 * RETURNS: Number of loaded characters into the given region.               */
{ 
    QUEX_NAME(BufferFiller_Plain)* me = (QUEX_NAME(BufferFiller_Plain)*)alter_ego;
    size_t                         loaded_byte_n = (size_t)-1;
    size_t                         loaded_n;

    __quex_assert(alter_ego); 
    __quex_assert(RegionBeginP); 
    __quex_assert(me->base.byte_loader); 
    /* NOT: QUEX_IF_ASSERTS_poison(RegionBeginP, &RegionBeginP[N]);
     * The buffer must remain intact, in case that not all is loaded.        */
    loaded_byte_n = me->base.byte_loader->load(me->base.byte_loader, 
                                               RegionBeginP, 
                                               N * sizeof(QUEX_TYPE_CHARACTER),
                                               end_of_stream_f);

    if( loaded_byte_n % sizeof(QUEX_TYPE_CHARACTER) ) {
        __QUEX_STD_printf("Error: End of file cuts in the middle a multi-byte character.");
    }
    loaded_n = loaded_byte_n / sizeof(QUEX_TYPE_CHARACTER);

    return loaded_n;
}

QUEX_INLINE void 
QUEX_NAME(BufferFiller_Plain_fill_prepare)(QUEX_NAME(BufferFiller)*   alter_ego,
                                           QUEX_TYPE_CHARACTER*       RegionBeginP,
                                           QUEX_TYPE_CHARACTER*       RegionEndP,
                                           void**                     begin_p,
                                           const void**               end_p)
{
    (void)alter_ego;

    /* Move-away of passed content in engine's buffer is done by caller. */
    *begin_p = (void*)RegionBeginP;
    *end_p   = (void*)RegionEndP; 
}

QUEX_INLINE ptrdiff_t 
QUEX_NAME(BufferFiller_Plain_fill_finish)(QUEX_NAME(BufferFiller)*   alter_ego,
                                          QUEX_TYPE_CHARACTER*       insertion_p,
                                          const QUEX_TYPE_CHARACTER* BufferEnd,
                                          const void*                FilledEndP_raw)
{
    const QUEX_TYPE_CHARACTER*  FilledEndP = (const QUEX_TYPE_CHARACTER*)FilledEndP_raw;
    (void)alter_ego;
    (void)BufferEnd;

    __quex_assert(FilledEndP >= insertion_p);
    __quex_assert(FilledEndP <= BufferEnd);
    /* Assert triggers => FilledEndP points WRONGLY BEHIND terminating zero. 
     * (FilledEndP, may point to it, at max.)                                */
    __quex_assert(   FilledEndP     <= insertion_p 
                  || FilledEndP[-1] != QUEX_SETTING_BUFFER_LIMIT_CODE);

    /* Copying of content is done, already, by caller.                       */
    /* Inserted number of characters = End - Begin.                          */
    return (ptrdiff_t)(FilledEndP - insertion_p);
}

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/buffer/filler/BufferFiller.i>

#endif /* __INCLUDE_GUARD__QUEX_BUFFER_INPUT_STRATEGY_PLAIN_I__ */
