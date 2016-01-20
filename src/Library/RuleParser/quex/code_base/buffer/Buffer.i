/* vim:set ft=c: -*- C++ -*- */
#ifndef __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_I
#define __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_I

#include <quex/code_base/asserts>
#include <quex/code_base/buffer/asserts>
#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/buffer/Buffer_debug.i>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/MemoryManager>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE ptrdiff_t QUEX_NAME(Buffer_move_forward)(QUEX_NAME(Buffer)*  me, 
                                                     ptrdiff_t           move_distance);
QUEX_INLINE void      QUEX_NAME(Buffer_move_forward_undo)(QUEX_NAME(Buffer)* me,
                                                          intmax_t           move_distance,
                                                          ptrdiff_t          move_size);
QUEX_INLINE ptrdiff_t QUEX_NAME(Buffer_move_backward)(QUEX_NAME(Buffer)* me, 
                                                      ptrdiff_t          move_distance);

QUEX_INLINE void*     QUEX_NAME(Buffer_fill)(QUEX_NAME(Buffer)*  me, 
                                             const void*         ContentBegin,
                                             const void*         ContentEnd);
QUEX_INLINE void      QUEX_NAME(Buffer_fill_prepare)(QUEX_NAME(Buffer)*  me, 
                                                     void**              begin_p, 
                                                     const void**        end_p);
QUEX_INLINE void      QUEX_NAME(Buffer_fill_finish)(QUEX_NAME(Buffer)* me,
                                                    const void*        FilledEndP);
QUEX_INLINE bool      QUEX_NAME(Buffer_is_end_of_stream_inside)(QUEX_NAME(Buffer)* me);
QUEX_INLINE void      QUEX_NAME(Buffer_init_content)(QUEX_NAME(Buffer)* me, 
                                                     QUEX_TYPE_CHARACTER* EndOfFileP);
QUEX_INLINE void      QUEX_NAME(Buffer_init_analyzis)(QUEX_NAME(Buffer)*   me);

QUEX_INLINE void      QUEX_NAME(Buffer_on_overflow_DEFAULT)(QUEX_NAME(Buffer)*, bool ForwardF);
QUEX_INLINE void      QUEX_NAME(Buffer_on_content_change_DEFAULT)(const QUEX_TYPE_CHARACTER*, 
                                                                  const QUEX_TYPE_CHARACTER*);

QUEX_INLINE void
QUEX_NAME(Buffer_construct)(QUEX_NAME(Buffer)*        me, 
                            QUEX_NAME(BufferFiller)*  filler,
                            QUEX_TYPE_CHARACTER*      memory,
                            const size_t              MemorySize,
                            QUEX_TYPE_CHARACTER*      EndOfFileP,
                            E_Ownership               Ownership)
{
    /* Ownership of InputMemory is passed to 'me->_memory'.                  */
    QUEX_NAME(BufferMemory_construct)(&me->_memory, memory, MemorySize, 
                                      Ownership); 
    
    /* By setting begin and end to zero, we indicate to the loader that      
     * this is the very first load procedure.                                */
    me->filler = filler;
    me->fill         = QUEX_NAME(Buffer_fill);
    me->fill_prepare = QUEX_NAME(Buffer_fill_prepare);
    me->fill_finish  = QUEX_NAME(Buffer_fill_finish);

    /* Event handlers.                                                       */
    me->on_content_change = QUEX_NAME(Buffer_on_content_change_DEFAULT);
    me->on_overflow       = QUEX_NAME(Buffer_on_overflow_DEFAULT);

    /* Initialize.                                                           */
    QUEX_NAME(Buffer_init_content)(me, EndOfFileP);
    QUEX_NAME(Buffer_init_analyzis)(me);

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
}

QUEX_INLINE void
QUEX_NAME(Buffer_destruct)(QUEX_NAME(Buffer)* me)
{
    if( me->filler && me->filler->ownership == E_Ownership_LEXICAL_ANALYZER ) {
        me->filler->delete_self(me->filler); 
    }
    QUEX_NAME(BufferMemory_destruct)(&me->_memory);
}

QUEX_INLINE void
QUEX_NAME(Buffer_init_analyzis)(QUEX_NAME(Buffer)*   me) 
/* Initialize:  _read_p                          
 *              _lexeme_start_p                 
 *              _character_at_lexeme_start     
 *              _character_before_lexeme_start                               */
{
    QUEX_TYPE_CHARACTER*      BeginP           = &me->_memory._front[1];

    if( ! me->_memory._front ) {
        /* No memory => Analyzer is put into a non-functional state.         */
        me->_read_p                             = (QUEX_TYPE_CHARACTER*)0;
        me->_lexeme_start_p                     = (QUEX_TYPE_CHARACTER*)0;
        me->_character_at_lexeme_start          = (QUEX_TYPE_CHARACTER)0;                                   
#       ifdef  __QUEX_OPTION_SUPPORT_BEGIN_OF_LINE_PRE_CONDITION                 
        me->_character_before_lexeme_start      = (QUEX_TYPE_CHARACTER)0;
#       endif
    }
    else {
        /* The first state in the state machine does not increment. 
         * => input pointer is set to the first position, not before.        */
        me->_read_p                             = BeginP;                                
        me->_lexeme_start_p                     = BeginP;                                
        me->_character_at_lexeme_start          = '\0';  /* Nothing covered. */
#       ifdef  __QUEX_OPTION_SUPPORT_BEGIN_OF_LINE_PRE_CONDITION                     
        /* When the buffer is initialized, a line begins. Set 'newline'.     */
        me->_character_before_lexeme_start      = QUEX_SETTING_CHARACTER_NEWLINE_IN_ENGINE_CODEC;
#       endif
    }
}

QUEX_INLINE void
QUEX_NAME(Buffer_init_content)(QUEX_NAME(Buffer)* me, QUEX_TYPE_CHARACTER* EndOfFileP)
/*  Initialize: input.character_index_begin
 *              input.character_index_end_of_stream                         
 *              input.end_p                                                  */
{
    QUEX_TYPE_CHARACTER*      BeginP           = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER*      EndP             = me->_memory._back;
    const ptrdiff_t           ContentSize      = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);
    ptrdiff_t                 loaded_n;
    bool                      end_of_stream_f  = false;
    QUEX_TYPE_STREAM_POSITION ci_begin         = (QUEX_TYPE_STREAM_POSITION)0;
    QUEX_TYPE_STREAM_POSITION ci_end_of_stream = (QUEX_TYPE_STREAM_POSITION)-1;
    QUEX_TYPE_CHARACTER*      end_p            = (QUEX_TYPE_CHARACTER*)0;
    (void)EndP;

    if( ! me->_memory._front ) {
        ci_end_of_stream = (QUEX_TYPE_STREAM_POSITION)-1;
        end_p            = (QUEX_TYPE_CHARACTER*)0;
        ci_begin         = (QUEX_TYPE_STREAM_POSITION)-1;
    }
    else if( me->filler && me->filler->byte_loader ) {
        __quex_assert(! EndOfFileP);

        loaded_n         = QUEX_NAME(BufferFiller_load)(me->filler, BeginP, ContentSize,
                                                        0, &end_of_stream_f);
        ci_end_of_stream = ((! loaded_n) || end_of_stream_f) ? loaded_n 
                                                             : (QUEX_TYPE_STREAM_POSITION)-1;
        end_p            = &BeginP[loaded_n];
    } 
    else {
        __quex_assert(me->_memory._front);           /* See first condition. */
        __quex_assert(! EndOfFileP || (EndOfFileP >= BeginP && EndOfFileP <= EndP));

        if( EndOfFileP ) {
            ci_end_of_stream = EndOfFileP - BeginP;
            end_p            = EndOfFileP;   
        }
        else {
            ci_end_of_stream = (QUEX_TYPE_STREAM_POSITION)-1;
            end_p            = BeginP;   
        }
    }
    me->input.character_index_begin         = ci_begin;
    me->input.character_index_end_of_stream = ci_end_of_stream;
    me->input.end_p                         = end_p;
    if( me->input.end_p ) {
        *(me->input.end_p)                  = QUEX_SETTING_BUFFER_LIMIT_CODE;
    }

    QUEX_IF_ASSERTS_poison(&me->input.end_p[1], me->_memory._back);
}

QUEX_INLINE void
QUEX_NAME(Buffer_register_content)(QUEX_NAME(Buffer)*        me,
                                   QUEX_TYPE_CHARACTER*      EndOfInputP,
                                   QUEX_TYPE_STREAM_POSITION CharacterIndexBegin)
/* Registers information about the stream that fills the buffer and its
 * relation to the buffer. 
 *  
 *  EndOfInputP --> Position behind the last character in the buffer that has
 *                  been streamed.
 *          '0' --> No change.
 *  
 *  CharacterIndexBegin --> Character index of the first character in the 
 *                          buffer.
 *                 '-1' --> No change.                                       */
{
    if( EndOfInputP ) {
        __quex_assert(EndOfInputP <= me->_memory._back);
        __quex_assert(EndOfInputP >  me->_memory._front);

        me->input.end_p    = EndOfInputP;
        *(me->input.end_p) = QUEX_SETTING_BUFFER_LIMIT_CODE;
    }

    if( CharacterIndexBegin != (QUEX_TYPE_STREAM_POSITION)-1 ) {
        me->input.character_index_begin = CharacterIndexBegin;
    }

    QUEX_IF_ASSERTS_poison(&me->input.end_p[1], me->_memory._back);
    /* NOT: assert(QUEX_NAME(Buffer_input_character_index_begin)(me) >= 0);
     * This function may be called before content is setup/loaded propperly. */ 
}

QUEX_INLINE void       
QUEX_NAME(Buffer_register_eos)(QUEX_NAME(Buffer)*        me,
                               QUEX_TYPE_STREAM_POSITION CharacterIndexEndOfStream)
{
    me->input.character_index_end_of_stream = CharacterIndexEndOfStream;
}

QUEX_INLINE bool
QUEX_NAME(Buffer_is_empty)(QUEX_NAME(Buffer)* me)
/* RETURNS: true, if buffer does not contain anything.
 *          false, else.                                                     */
{ 
    return    me->input.end_p == &me->_memory._front[1] 
           && me->input.character_index_begin == 0; 
}

QUEX_INLINE QUEX_TYPE_STREAM_POSITION  
QUEX_NAME(Buffer_input_character_index_end)(QUEX_NAME(Buffer)* me)
/* RETURNS: Character index of the character to which '.input.end_p' points.
 *                                                                           */
{
    __quex_assert(me->input.character_index_begin >= 0);
    QUEX_BUFFER_ASSERT_pointers_in_range(me);

    return   me->input.character_index_begin 
           + (me->input.end_p - &me->_memory._front[1]);
}

QUEX_INLINE void
QUEX_NAME(Buffer_read_p_add_offset)(QUEX_NAME(Buffer)* buffer, const size_t Offset)
/* Add offset to '._read_p'. No check applies whether this is admissible.
 *                                                                           */
{ 
    QUEX_BUFFER_ASSERT_pointers_in_range(buffer);
    buffer->_read_p += Offset; 
    QUEX_BUFFER_ASSERT_pointers_in_range(buffer);
}

QUEX_INLINE size_t
QUEX_NAME(Buffer_content_size)(QUEX_NAME(Buffer)* me)
{
    return QUEX_NAME(BufferMemory_size)(&(me->_memory)) - 2;
}

QUEX_INLINE bool 
QUEX_NAME(Buffer_is_end_of_file)(QUEX_NAME(Buffer)* me)
{ 
    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
    if     ( me->_read_p != me->input.end_p )                return false;
    else if( me->input.character_index_end_of_stream == -1 ) return false;

    return    QUEX_NAME(Buffer_input_character_index_end)(me) 
           == me->input.character_index_end_of_stream;
}

QUEX_INLINE bool 
QUEX_NAME(Buffer_is_end_of_stream_inside)(QUEX_NAME(Buffer)* me)
{ 
    const ptrdiff_t ContentSize = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);

    if( me->input.character_index_end_of_stream == -1 ) return false;
    else if( me->input.character_index_end_of_stream < me->input.character_index_begin ) return false;
    
    return me->input.character_index_end_of_stream - me->input.character_index_begin < ContentSize;
}

QUEX_INLINE bool                  
QUEX_NAME(Buffer_is_begin_of_file)(QUEX_NAME(Buffer)* buffer)
{ 
    QUEX_BUFFER_ASSERT_CONSISTENCY(buffer);
    if     ( buffer->_read_p != buffer->_memory._front )                  return false;
    else if( QUEX_NAME(Buffer_input_character_index_begin)(buffer) != 0 ) return false;
    else                                                                  return true;
}

QUEX_INLINE bool
QUEX_NAME(Buffer_move_and_load_forward)(QUEX_NAME(Buffer)*        me, 
                                        QUEX_TYPE_STREAM_POSITION NewCharacterIndexBegin,
                                        QUEX_TYPE_STREAM_POSITION MinCharacterIndexInBuffer)
/* RETURNS:  true -- if the the buffer could be filled start from 
 *                   NewCharacterIndexBegin.
 *           false, else.
 *
 * In case, that the loading fails, the buffer is setup as it was BEFORE the call
 * to this function.
 *
 * EXPLANATION:
 *
 * Before:    .-------------------------------------- prev character_index_begin             
 *            :                 
 *            | . . . . . . . . .x.x.x.x.x.x.x.x.x.x.x| 
 *                              |<---- move size ---->|
 * After:     |<- move distance |
 *            .-------------------------------------- new character_index_begin
 *            :                     .---------------- prev character index begin
 *            :                     :  
 *            |x.x.x.x.x.x.x.x.x.x.x|N.N.N.N.N.N.N. . | 
 *            |- move_size -------->|- loaded_n ->|
 *                                                             
 * Moves the region of size 'Size' from the end of the buffer to the beginning
 * of the buffer and tries to load as many characters as possible behind it. */
{
    QUEX_TYPE_CHARACTER*       BeginP      = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER*       EndP        = me->_memory._back;
    const ptrdiff_t            ContentSize = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);
    QUEX_TYPE_STREAM_POSITION  load_character_index;
    ptrdiff_t                  load_request_n;
    QUEX_TYPE_CHARACTER*       load_p;
    ptrdiff_t                  loaded_n;
    intmax_t                   move_distance;
    ptrdiff_t                  move_size;
    bool                       end_of_stream_f = false;

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
    __quex_assert(me->input.character_index_begin      <= NewCharacterIndexBegin);
    __quex_assert(NewCharacterIndexBegin               <= MinCharacterIndexInBuffer);
    __quex_assert(NewCharacterIndexBegin + ContentSize >= MinCharacterIndexInBuffer );

    if(    me->input.character_index_end_of_stream != -1
        && MinCharacterIndexInBuffer >= me->input.character_index_end_of_stream ) {
        /* If the end of the stream is INSIDE the buffer already, then there
         * is no need, no chance, of loading more content.                   */
        return false;
    }

    /* (1) Move existing content in the buffer to appropriate position.      */
    move_distance        = NewCharacterIndexBegin - me->input.character_index_begin;
    move_size            = QUEX_NAME(Buffer_move_forward)(me, (ptrdiff_t)move_distance);
    load_character_index = NewCharacterIndexBegin + move_size;
    load_request_n       = ContentSize - move_size; 
    load_p               = &BeginP[move_size];

    __quex_assert(load_character_index == NewCharacterIndexBegin + (load_p - BeginP));
    __quex_assert(load_p >= BeginP);
    __quex_assert(&load_p[load_request_n] <= EndP);
    (void)EndP;
    loaded_n = QUEX_NAME(BufferFiller_load)(me->filler, load_p, load_request_n,
                                            load_character_index,
                                            &end_of_stream_f);

    if( (! loaded_n) || end_of_stream_f ) { /* End of stream detected.       */
        QUEX_NAME(Buffer_register_eos)(me, load_character_index + loaded_n);
    }

    /* (3) In case of failure, restore previous buffer content.              */
    if( MinCharacterIndexInBuffer >= load_character_index + loaded_n ) {
        QUEX_NAME(Buffer_move_forward_undo)(me, move_distance, move_size);
        return false;
    }

    QUEX_NAME(Buffer_register_content)(me, &load_p[loaded_n], NewCharacterIndexBegin);
    return true;
}

QUEX_INLINE bool
QUEX_NAME(Buffer_move_and_load_backward)(QUEX_NAME(Buffer)*        me, 
                                         QUEX_TYPE_STREAM_POSITION NewCharacterIndexBegin)
/* Before:                     
 *            .------------------------------------- prev character index begin
 *            :
 *            |x.x.x.x.x.x.x.x.x.x. . . . . . . . . . . . . |
 *            |<--- move size---->|                         
 * After:                                             
 *            .------------------------------------- new character index begin
 *            :                     .--------------- prev character index begin
 *            :                     :
 *            :--- move distance--->|                 
 *            |N.N.N.N.N.N.N.N.N.N.N.x.x.x.x.x.x.x.x.x.x. . | 
 *                               
 * Moves the region of size 'Size' from the beginning of the buffer to the end
 * and tries to load as many characters as possible behind it. If the try fails
 * something is seriously wrong.                                             */
{
    QUEX_TYPE_CHARACTER*       BeginP   = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER*       EndP     = me->_memory._back;
    QUEX_TYPE_STREAM_POSITION  ci_begin = QUEX_NAME(Buffer_input_character_index_begin)(me);
    ptrdiff_t                  load_request_n;
    ptrdiff_t                  loaded_n;
    ptrdiff_t                  move_distance;
    QUEX_TYPE_CHARACTER*       end_p;
    bool                       end_of_stream_f = false;

    __quex_assert(NewCharacterIndexBegin >= 0);
    __quex_assert(ci_begin  >= NewCharacterIndexBegin);

    /* (1) Move away content, so that previous content can be reloaded.      */
    move_distance  = (ptrdiff_t)(ci_begin - NewCharacterIndexBegin);
    load_request_n = QUEX_NAME(Buffer_move_backward)(me, (ptrdiff_t)move_distance);

    __quex_assert(&BeginP[load_request_n] <= EndP);

    /* (2) Move away content, so that previous content can be reloaded.      */
    loaded_n = QUEX_NAME(BufferFiller_load)(me->filler, BeginP, load_request_n,
                                            NewCharacterIndexBegin,
                                            &end_of_stream_f);

    /* (3) In case of error, the stream must have been corrupted. Previously
     *     present content is not longer available. Continuation impossible. */
    if( loaded_n != load_request_n ) {
        QUEX_ERROR_EXIT("Buffer filler failed to load content that has been loaded before.!");
        return false;
    }

    end_p = EndP - me->input.end_p < move_distance ? 
            EndP : &me->input.end_p[move_distance];

    QUEX_NAME(Buffer_register_content)(me, end_p, NewCharacterIndexBegin);
    return true;
}
   
QUEX_INLINE ptrdiff_t
QUEX_NAME(Buffer_move_away_passed_content)(QUEX_NAME(Buffer)*    me,
                                           QUEX_TYPE_CHARACTER** position_register,
                                           const size_t          PositionRegisterN)
/* Free some space AHEAD so that new content can be loaded. Content that 
 * is still used, or expected to be used shall remain inside the buffer.
 * Following things need to be respected:
 *
 *    _lexeme_start_p  --> points to the lexeme that is currently treated.
 *                         MUST BE INSIDE BUFFER!
 *    _read_p          --> points to the character that is currently used
 *                         for triggering. MUST BE INSIDE BUFFER!
 *    fall back region --> A used defined buffer backwards from the lexeme
 *                         start. Shall help to avoid extensive backward
 *                         loading.
 *
 * RETURNS: Pointer to the end of the maintained content.                    */
{ 
    QUEX_TYPE_CHARACTER*        BeginP = &me->_memory._front[1];
    const QUEX_TYPE_CHARACTER*  EndP   = me->_memory._back;
    const ptrdiff_t             ContentSize = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);
    QUEX_TYPE_CHARACTER*        move_begin_p;
    ptrdiff_t                   move_size;
    ptrdiff_t                   move_distance;
    const ptrdiff_t             FallBackN = (ptrdiff_t)QUEX_SETTING_BUFFER_MIN_FALLBACK_N;
    QUEX_TYPE_CHARACTER**       pr_it     = 0x0;

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);

    if( me->_read_p - me->_lexeme_start_p >= ContentSize - FallBackN ) { 
        /* OVERFLOW: If stretch from _read_p to _lexeme_start_p 
         * spans the whole buffer, then nothing can be loaded.               */
        me->on_overflow(me, /* Forward */ true);
        return 0;
    }
    else if( QUEX_NAME(Buffer_is_end_of_stream_inside)(me) ) {
        /* Refuse the move, if the end of stream is inside buffer.           */
        return 0;
    }

    /* Determine from where the region-to-be-moved BEGINS, what its size is
     * and how far it is to be moved.                                        */
    move_begin_p  = me->_read_p;
    move_begin_p  = me->_lexeme_start_p ? QUEX_MIN(move_begin_p, me->_lexeme_start_p)
                                        : move_begin_p;
    /* Plain math: move_begin_p = max(BeginP, move_begin_p - FallBackN); 
     * BUT: Consider case where 'move_begin_p - FallBackN < 0'! CAREFUL!     */
    move_begin_p  = move_begin_p - BeginP < FallBackN ? BeginP 
                                                      : &move_begin_p[- FallBackN];
    move_distance = move_begin_p    - BeginP;

    if( ! move_distance ) {
        return 0;
    }

    move_size = QUEX_NAME(Buffer_move_forward)(me, move_distance);

    /* Pointer Adaption: _read_p, _lexeme_start_p, position registers.
     *                   input.end_p, input.end_character_index              */
    me->_read_p -= move_distance;
    if( me->_lexeme_start_p ) me->_lexeme_start_p -= move_distance;
   
    if( position_register ) {
        /* All position registers MUST point behind '_lexeme_start_p'.       */
        for(pr_it = position_register; pr_it != &position_register[PositionRegisterN]; ++pr_it) {
            if( ! *pr_it ) continue;
            *pr_it = (*pr_it - BeginP) >= move_distance ? *pr_it - move_distance : 0;
        }
    }

    /* input.end_p/end_character_index: End character index remains the SAME, 
     * since no new content has been loaded into the buffer.                 */
    __quex_assert(me->input.end_p - BeginP >= move_distance);

    QUEX_NAME(Buffer_register_content)(me, &me->input.end_p[- move_distance], 
                                       me->input.character_index_begin + move_distance);

    /*_______________________________________________________________________*/
    __quex_assert(me->input.end_p == &move_begin_p[move_size - move_distance]);
    (void)move_size;
    QUEX_IF_ASSERTS_poison(&EndP[- move_distance + 1], EndP);
    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
    (void)EndP;

    return move_distance;
}

QUEX_INLINE bool
QUEX_NAME(Buffer_load_forward)(QUEX_NAME(Buffer)*    me,
                               QUEX_TYPE_CHARACTER** position_register,
                               const size_t          PositionRegisterN)
/* Load as much new content into the buffer as possible--from what lies ahead
 * in the input stream. Maintains '_read_p', '_lexeme_start_p' inside the
 * buffer (if possible also fallback region). The 'input.end_p' pointer and
 * 'input.end_character_index' are adapted according to the newly loaded
 * content, i.e. the point to exactly the same character as before the load.
 *
 * BEHAVIOR: BLOCKING wait for incoming stream content. 
 *           No return without content--except at end of stream.
 *
 *           Buffer and pointers are adapted are adapted IN ANY CASE!
 *
 *           (i) Content present:
 *               => return 'true'.
 *
 *           (ii) No content:
 *               => pointers are 'disabled' because 'end_p = _read_p'.
 *                  return 'false'.
 *
 * RETURNS: true  => load successful: analysis MAY CONTINUE.
 *          false => nothing loaded:  analysis MUST STOP!                    
 *
 * The case of 'end-of-stream' may be true in both cases. When 'end-of-stream' 
 * is detected, the character index of the 'end-of-stream' is registered. This 
 * prevents future attemps to load beyond that index. Again, even if 
 * 'end-of-stream' has been detected, there might be characters for the lexer 
 * to chew on.                                                               */
{
    QUEX_TYPE_CHARACTER*        BeginP      = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER*        EndP        = me->_memory._back;
    const ptrdiff_t             ContentSize = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);
    QUEX_TYPE_STREAM_POSITION   ci_begin    = QUEX_NAME(Buffer_input_character_index_begin)(me);
    QUEX_TYPE_STREAM_POSITION   ci_load_begin;
    ptrdiff_t                   move_distance;
    ptrdiff_t                   load_request_n;
    ptrdiff_t                   loaded_n;
    bool                        end_of_stream_f = false;

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);

    if( ! me->filler || ! me->filler->byte_loader ) {
        QUEX_NAME(Buffer_register_eos)(me, ci_begin + (me->input.end_p - BeginP));
        return false;       /* No filler, no loader => no loading!           */
    }

    /* Move remaining content.
     * Maintain lexeme and fallback.                 
     * Adapt pointers.                                                       */
    move_distance = QUEX_NAME(Buffer_move_away_passed_content)(me, position_register, 
                                                               PositionRegisterN);
    if( ! move_distance && me->input.end_p == EndP ) {
        return false;        /* Cannot free space for loading => no loading! */
    }

    /* Load new content.                                                     */
    ci_load_begin  = me->input.character_index_begin + (me->input.end_p - BeginP);
    load_request_n = ContentSize                     - (me->input.end_p - BeginP);
    loaded_n       = QUEX_NAME(BufferFiller_load)(me->filler, 
                                                  me->input.end_p, load_request_n,
                                                  ci_load_begin, &end_of_stream_f);
    QUEX_NAME(Buffer_register_content)(me, &me->input.end_p[loaded_n], -1);

    if( ! loaded_n ) {
        /* Filler blocks until either some characters are filled, or
         * returns fills characters indicating 'end-of-stream'.              */
        end_of_stream_f = true; 
    }
    if( end_of_stream_f ) {
        QUEX_NAME(Buffer_register_eos)(me, ci_load_begin + loaded_n); 
    }

    __quex_debug_buffer_load(me, "LOAD FORWARD(exit)\n");
    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
    return loaded_n ? true : false;
}

QUEX_INLINE ptrdiff_t        
QUEX_NAME(Buffer_move_away_upfront_content)(QUEX_NAME(Buffer)* me)
/* Free some space in the REAR so that previous content can be re-loaded. Some 
 * content is to be left in front, so that no immediate reload is necessary
 * once the analysis goes forward again. Following things need to be respected:
 *
 *    _lexeme_start_p  --> points to the lexeme that is currently treated.
 *                         MUST BE INSIDE BUFFER!
 *    _read_p          --> points to the character that is currently used
 *                         for triggering. MUST BE INSIDE BUFFER!
 *
 * RETURNS: Distance the the buffer content has been freed to be filled.     */
{
    const QUEX_TYPE_CHARACTER*  BeginP      = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER*        EndP        = me->_memory._back;
    const ptrdiff_t             ContentSize = EndP - BeginP;
    ptrdiff_t                   move_distance;
    QUEX_TYPE_CHARACTER*        end_p;

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);

    if( me->input.character_index_begin == 0 && BeginP == me->_read_p ) {
        return 0;                        /* Begin of stream.                 */
    }
    else if( me->_lexeme_start_p >= &EndP[-1] ) { 
        /* If _lexeme_start_p at back, then no new content can be loaded.    */
        me->on_overflow(me, /* Forward */ false);
        return 0;
    }

    /* Determine where the region-to-be-moved ENDS, what its size is and how
     * far it is to be moved.                                                */
    move_distance = EndP - me->input.end_p;
    move_distance = QUEX_MAX(move_distance, (ptrdiff_t)(ContentSize/3));
    move_distance = (ptrdiff_t)QUEX_MIN((QUEX_TYPE_STREAM_POSITION)move_distance, 
                                        me->input.character_index_begin);
    move_distance = QUEX_MIN(move_distance, &EndP[-1] - me->_read_p);
    if( me->_lexeme_start_p ) {
        move_distance = QUEX_MIN(move_distance, &EndP[-1] - me->_lexeme_start_p);
    }

    if( ! move_distance ) {
        return 0;
    }

    (void)QUEX_NAME(Buffer_move_backward)(me, move_distance);

    /* Pointer Adaption: _read_p, _lexeme_start_p.                           */
    me->_read_p += move_distance;
    if( me->_lexeme_start_p ) me->_lexeme_start_p += move_distance;

    /* Adapt and of content pointer and new character index at begin.        */
    end_p = EndP - me->input.end_p < move_distance ? EndP
                                                   : &me->input.end_p[move_distance];

    QUEX_NAME(Buffer_register_content)(me, end_p, 
                                       me->input.character_index_begin - move_distance);

    /*_______________________________________________________________________*/
    QUEX_IF_ASSERTS_poison(BeginP, &BeginP[move_distance]); 
    QUEX_BUFFER_ASSERT_CONSISTENCY(me);

    return move_distance;
}

QUEX_INLINE bool   
QUEX_NAME(Buffer_load_backward)(QUEX_NAME(Buffer)* me)
/* Load *previous* content into the buffer so that the analyzer can continue
 * seeminglessly (in backward direction).
 *
 * BEHAVIOR: BLOCKING wait for incoming stream content. 
 *           No return without content--except at end of stream.
 *
 *           Buffer and pointers are adapted are adapted IN ANY CASE!
 *
 * RETURNS: true  => load successful:           backward analysis MAY CONTINUE.
 *          false => begin of stream or ERROR:  backward analysis MUST STOP!
 *
 *  __________________________________________________________________________
 * ! In the false case, the range from 'Begin' to '_lexeme_start_p' may       !
 * ! have ARBITRARY CONTENT. Then the '_read_p' MUST be reset IMMEDIATELY and !
 * ! only forward analysis may work.                                          !
 * '--------------------------------------------------------------------------'
 *
 *_____________________________________________________________________________
 * NO ADAPTIONS OF POST-CONTEXT POSITIONS. Reason: Backward analysis appears
 * only in the following two cases.
 *  
 *  (1) When checking for a pre-condition. This does not involve pre-contexts.
 * 
 *  (2) When tracing back along a 'pseudo-ambigous post context'. However,
 *      the stretch from 'end-of-core' pattern to 'end-of-post context' lies
 *      completely in between 'lexeme start' to 'read '. Thus, one never has
 *      to go farther back then the buffer's begin.                        */
{
    QUEX_TYPE_CHARACTER*       BeginP = &me->_memory._front[1];
    ptrdiff_t                  move_distance;
    ptrdiff_t                  loaded_n;
    bool                       end_of_stream_f = false;

    QUEX_BUFFER_ASSERT_CONSISTENCY(me);

    __quex_debug_buffer_load(me, "BACKWARD(entry)\n");

    /* REFUSE CASES:                                                         */
    if( ! me->filler || ! me->filler->byte_loader ) {
        return false;                    /* Buffer based analysis.           */
    }
    else if( ! QUEX_NAME(ByteLoader_seek_is_enabled)(me->filler->byte_loader) ) {
        return false;                    /* Stream cannot go backwards.      */
    }

    move_distance = QUEX_NAME(Buffer_move_away_upfront_content)(me);

    if( ! move_distance ) {
        return false;
    }

    /* Load new content.                                                     */
    loaded_n      = QUEX_NAME(BufferFiller_load)(me->filler, 
                                                 BeginP, move_distance,
                                                 me->input.character_index_begin, 
                                                 &end_of_stream_f);

    if( loaded_n  != move_distance ) {
        /* Serious: previously loaded content could not be loaded again!     
         * => Buffer has now hole: 
         *    from BeginP[loaded_n] to Begin[move_distance]                 
         * The analysis can continue in forward direction, but not backwards.*/
        return false;
    }

    __quex_debug_buffer_load(me, "BACKWARD(exit)\n");
    QUEX_BUFFER_ASSERT_CONSISTENCY(me);
    return true;
}

QUEX_INLINE ptrdiff_t
QUEX_NAME(Buffer_move_forward)(QUEX_NAME(Buffer)* me, 
                               ptrdiff_t          move_distance)
/* Moves the entire (meaningful) content of the buffer by 'move_distance'
 * forward. It does NOT MODIFY any pointers about the buffer content!
 *
 *                                                               EndP
 *                                         |<---- move size ---->|
 *     Before:   | . . . . . . . . . . . . .x.x.x.x.x.x.x.x.x.x.x| 
 *               |<---- move distance -----|                     |
 *                  .----------------------'                     |
 *               .-'                            .----------------'
 *               |                     .-------'
 *     After:    |x.x.x.x.x.x.x.x.x.x.x| . . . . . . . . . . . . | 
 *               |<---- move_size ---->|
 *
 * The callback 'on_content_change()' informs the user that any pointer into
 * the buffer must now copy its pointed objects, because the pointers will 
 * become invalid.
 *
 * RETURNS: Number of characters that have been moved.                       */
{
    QUEX_TYPE_CHARACTER* BeginP       = &me->_memory._front[1];
    const ptrdiff_t      FilledSize   = me->input.end_p - BeginP;
    ptrdiff_t            move_size;

    if( move_distance >= FilledSize ) {
        return 0;
    }

    move_size = FilledSize - move_distance;

    if( move_distance && move_size ) {
        me->on_content_change(BeginP, me->input.end_p);

        __QUEX_STD_memmove((void*)BeginP, (void*)&BeginP[move_distance],
                           (size_t)move_size * sizeof(QUEX_TYPE_CHARACTER));
    }
    return move_size;
}

QUEX_INLINE ptrdiff_t
QUEX_NAME(Buffer_move_backward)(QUEX_NAME(Buffer)* me, 
                                ptrdiff_t          move_distance)
/* Moves content so that previous content may be filled into the buffer.
 *
 *          BeginP
 *            |<--- move size---->|                         
 *            |x.x.x.x.x.x.x.x.x.x| . . . . . . . . . . . . . . |
 *            |                   '--------------.               
 *            '------------.                      '-------------.
 *                          '---------------.                   |
 *            :------- move distance------->|                   |
 *            | . . . . . . . . . . . . . . |x.x.x.x.x.x.x.x.x.x| 
 *                               
 *
 * RETURNS: Number of character that need to be filled into the gap.
 *                                                                           */
{
    QUEX_TYPE_CHARACTER*  BeginP      = &me->_memory._front[1];
    const ptrdiff_t       ContentSize = (ptrdiff_t)QUEX_NAME(Buffer_content_size)(me);
    ptrdiff_t             move_size;

    if( move_distance > ContentSize ) {
        return ContentSize;
    }

    move_size = ContentSize - move_distance;

    if( move_distance && move_size ) {
        me->on_content_change(BeginP, me->input.end_p);

        __QUEX_STD_memmove((void*)&BeginP[move_distance], BeginP, 
                           (size_t)move_size * sizeof(QUEX_TYPE_CHARACTER));
    }
    return (ptrdiff_t)move_distance;
}

QUEX_INLINE void
QUEX_NAME(Buffer_move_forward_undo)(QUEX_NAME(Buffer)* me,
                                    intmax_t           move_distance,
                                    ptrdiff_t          move_size)
/* Restore the buffer's raw memory to what it was before in the 'FORWARD' case. 
 * It is assumed that the buffer's parameters in
 *
 *                         me->input
 *
 * remained UNTOUCHED during the moving and loading of the caller function.
 * That is, they indicate the situation to be restored.                      */
{
    QUEX_TYPE_CHARACTER* BeginP      = &me->_memory._front[1];
    QUEX_TYPE_CHARACTER* EndP        = me->_memory._back;
    ptrdiff_t            load_request_n;
    ptrdiff_t            loaded_n;
    bool                 end_of_stream_f = false;

    /* Character with character index 'MinCharacterIndexInBuffer' has
     * not been loaded. => Buffer must be setup as before.                   */
    if( move_size ) {
        __QUEX_STD_memmove((void*)&BeginP[move_distance], (void*)BeginP, 
                           (size_t)move_size * sizeof(QUEX_TYPE_CHARACTER));
        load_request_n = (ptrdiff_t)move_distance;
    }
    else {
        load_request_n = (me->input.end_p - BeginP);
    }
    __quex_assert(&BeginP[load_request_n] <= EndP);
    (void)EndP;
    loaded_n = QUEX_NAME(BufferFiller_load)(me->filler, BeginP, load_request_n,
                                            me->input.character_index_begin,
                                            &end_of_stream_f);

    if( loaded_n != load_request_n ) {
        QUEX_ERROR_EXIT("Buffer filler failed to load content that has been loaded before.!");
    }
    else {
        /* Ensure, that the buffer limit code is restored.                   */
        *(me->input.end_p) = (QUEX_TYPE_CHARACTER)QUEX_SETTING_BUFFER_LIMIT_CODE;
    }
}

QUEX_INLINE void      
QUEX_NAME(Buffer_on_content_change_DEFAULT)(const QUEX_TYPE_CHARACTER* BeginP, 
                                            const QUEX_TYPE_CHARACTER* EndP)
{ (void)BeginP; (void)EndP; }

QUEX_INLINE void
QUEX_NAME(Buffer_on_overflow_DEFAULT)(QUEX_NAME(Buffer)* me, bool ForwardF)
{
    (void)me; (void)ForwardF;
#   ifdef QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE
    uint8_t                     utf8_encoded_str[512]; 
    char                        message[1024];
    const size_t                MessageSize = (size_t)1024;
    uint8_t*                    WEnd        = 0x0;
    uint8_t*                    witerator   = 0x0; 
    QUEX_TYPE_CHARACTER*        End         = 0x0; 
    const QUEX_TYPE_CHARACTER*  iterator    = 0x0; 

    /* Print out the lexeme start, so that the user has a hint. */
    WEnd        = utf8_encoded_str + 512 - 7;
    witerator   = utf8_encoded_str; 
    End         = me->_memory._back; 
    iterator    = me->_lexeme_start_p; 

    QUEX_CONVERTER_STRING(QUEX_SETTING_CHARACTER_CODEC, utf8)(&iterator, End, &witerator, WEnd);

    message[0] = '\0';
    /* No use of 'snprintf()' because not all systems seem to support it propperly. */
    __QUEX_STD_strncat(message, 
                       "Distance between lexeme start and current pointer exceeds buffer size.\n"
                       "(tried to load buffer",
                       MessageSize);
    __QUEX_STD_strncat(message, ForwardF ? "forward)" : "backward)",                   MessageSize);
    __QUEX_STD_strncat(message, "As a hint consider the beginning of the lexeme:\n[[", MessageSize);
    __QUEX_STD_strncat(message, (char*)utf8_encoded_str,                               MessageSize);
    __QUEX_STD_strncat(message, "]]\n",                                                MessageSize);

    QUEX_ERROR_EXIT(message);
#   else
    QUEX_ERROR_EXIT("Distance between lexeme start and current pointer exceeds buffer size.\n"
                    "(tried to load buffer forward). Please, compile with option\n\n"
                    "    QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE\n\n"
                    "in order to get a more informative output. Most likely, one of your patterns\n"
                    "eats more than you intended. Alternatively you might want to set the buffer\n"
                    "size to a greater value or use skippers (<skip: [ \\n\\t]> for example).\n");
#   endif /* QUEX_OPTION_INFORMATIVE_BUFFER_OVERFLOW_MESSAGE */
}

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/buffer/filler/BufferFiller.i>
#include <quex/code_base/buffer/Buffer_debug.i>
#include <quex/code_base/buffer/Buffer_navigation.i>
#include <quex/code_base/buffer/Buffer_fill.i>
#include <quex/code_base/buffer/BufferMemory.i>

#endif /* __QUEX_INCLUDE_GUARD__BUFFER__BUFFER_I */


