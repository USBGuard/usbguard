/* -*- C++ -*- vim: set syntax=cpp: */
#ifndef __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_FILL_I
#define __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_FILL_I

#include <quex/code_base/buffer/Buffer>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void*
QUEX_NAME(Buffer_fill)(QUEX_NAME(Buffer)*  me, 
                       const void*         ContentBegin,
                       const void*         ContentEnd)
{
    ptrdiff_t      copy_n;
    void*          begin_p;
    const void*    end_p;

    /* Prepare the buffer for the reception of new input an acquire the
     * border pointers of where new content can be filled.                   */
    me->fill_prepare(me, &begin_p, &end_p);

    /* Copy as much as possible of the new content into the designated
     * region in memory. This may be the engine's buffer or a 'raw' buffer
     * whose content still needs to be converted.                            */
    copy_n = (ptrdiff_t)QUEXED(MemoryManager_insert)((uint8_t*)begin_p,  
                                                     (uint8_t*)end_p,
                                                     (uint8_t*)ContentBegin, 
                                                     (uint8_t*)ContentEnd);

    /* Flush into buffer what has been filled from &begin[0] to 
     * &begin[inserted_byte_n].                                              */
    me->fill_finish(me, &((uint8_t*)begin_p)[copy_n]);

    /* Report a pointer to the first content element that has not yet 
     * been treated (== ContentEnd if all complete).                         */
    return (void*)&((uint8_t*)ContentBegin)[copy_n];
}

QUEX_INLINE void
QUEX_NAME(Buffer_fill_prepare)(QUEX_NAME(Buffer)*  me, 
                               void**              begin_p, 
                               const void**        end_p)
/* SETS: *begin_p: position where the next content needs to be filled. 
 *       *end_p:   address directly behind the last byte that can be filled.
 *
 * The content may be filled into the engine's buffer or an intermediate 
 * 'raw' buffer which still needs to be converted.                          */
{
    (void)QUEX_NAME(Buffer_move_away_passed_content)(me, (QUEX_TYPE_CHARACTER**)0, 0);

    /* Get the pointers for the border where to fill content.               */
    me->filler->derived.fill_prepare(me->filler, 
                                     me->input.end_p, me->_memory._back, 
                                     begin_p, end_p);

    __quex_assert(*end_p >= *begin_p);
}

QUEX_INLINE void
QUEX_NAME(Buffer_fill_finish)(QUEX_NAME(Buffer)* me,
                              const void*        FilledEndP)
/* Uses the content that has been inserted until 'FilledEndP' to fill the
 * engine's character buffer (if it is not already done). A fille of type
 * 'BufferFiller_Converter' takes the content of the raw buffer and converts
 * it into the engine's buffer from 'me->input.end_p' to 'me->_memory._back'.
 *                                                                           */
{
    QUEX_TYPE_CHARACTER*   BeginP = &me->_memory._front[1];

    /* Place new content in the engine's buffer.                             */
    ptrdiff_t inserted_character_n = me->filler->derived.fill_finish(me->filler, 
                                                                     me->input.end_p,
                                                                     me->_memory._back, 
                                                                     FilledEndP);

    /* Assume: content from 'input.end_p' to 'input.end_p[CharN]'
     * has been filled with data.                                            */
    if( me->filler->_byte_order_reversion_active_f ) {
        QUEX_NAME(BufferFiller_reverse_byte_order)(me->input.end_p, 
                                                   &me->input.end_p[inserted_character_n]);
    }

    /* -- Manual buffer filling requires the end-of-stream pointer always
     *    to be set. 
     * -- The 'character_index_begin' has been set in 'fill_prepare()'.
     *    '-1' => no change.
     * -- The 'character_index_end_of_stream' can now be set, since it is
     *    known how many characters have been inserted.
     *                                                                       */
    QUEX_NAME(Buffer_register_content)(me, &me->input.end_p[inserted_character_n], -1);
    QUEX_NAME(Buffer_register_eos)(me,   me->input.character_index_begin
                                       + (me->input.end_p - BeginP));

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
}

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_FILL_I */
