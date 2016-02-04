/* -*- C++ -*- vim: set syntax=cpp: */
#ifndef  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_NAVIGATION_I
#define  __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_NAVIGATION_I
/* PURPOSE: BufferFiller's seek: 
 *
 *    .------------------------------------------------------------------.
 *    |  Setting the character index of the next character to be filled  |
 *    |                         into the buffer.                         |
 *    '------------------------------------------------------------------'
 *
 * NOT TO CONFUSE with two other forms of seek:
 *
 *    -- Buffer's seek sets the '_read_p' of the analyzer to a character
 *       that is to be treated next.
 *    -- QUEX_NAME(ByteLoader)'s seek sets the position in the low level input
 *       stream.
 * 
 * A BufferFiller's seek has NO direct influence on the content or the pointers
 * of the Buffer object that it fills.
 *
 * The difference between the QUEX_NAME(ByteLoader)'s seeking (stream seeking) and the
 * BufferFiller's seeking is that the QUEX_NAME(ByteLoader )understands the position 
 * independently of its meaning. The BufferFiller already interprets the stream
 * as 'characters'. A QUEX_NAME(ByteLoader )sets the stream to a particular byte position.
 * A BufferFiller prepare the input of a character.
 *
 * (C) Frank-Rene Schaefer                                                   */

#include <quex/code_base/definitions>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/MemoryManager>
#include <quex/code_base/buffer/filler/BufferFiller>
#include <quex/code_base/buffer/Buffer_debug>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE QUEX_TYPE_STREAM_POSITION 
QUEX_NAME(BufferFiller_character_index_tell)(QUEX_NAME(BufferFiller)* me)
{
    return me->character_index_next_to_fill;
}


QUEX_INLINE bool 
QUEX_NAME(BufferFiller_character_index_seek)(QUEX_NAME(BufferFiller)*         me, 
                                             const QUEX_TYPE_STREAM_POSITION  CharacterIndex) 
/* BufferFiller's seek sets the input position for the next character load in
 * the stream. That is, it adapts:
 *
 *     'character_index_next_to_fill = CharacterIndex' 
 *
 * and the byte loader is brought into a position so that this will happen.  
 *
 * RETURNS: true upon success, false else.                                   */
{ 
    ptrdiff_t                      backup_stomach_byte_n; 
    QUEX_TYPE_STREAM_POSITION      backup_byte_loader_position; 
    QUEX_TYPE_STREAM_POSITION      backup_character_index_next_to_fill;
    QUEX_TYPE_STREAM_POSITION      target_byte_pos;

    if( me->character_index_next_to_fill == CharacterIndex ) return true;

    /* Converter reports '-1' => unable to determine the number of bytes in
     *                           the stomach. 
     * => When backup position is to setup, start stepping from beginning.
     * (This is currently only an issue with ICU; IConv behaves well)        */
    backup_stomach_byte_n               = me->stomach_byte_n(me);
    backup_byte_loader_position         = me->byte_loader->tell(me->byte_loader);
    backup_character_index_next_to_fill = me->character_index_next_to_fill;

    if( me->byte_n_per_character != -1 ) {
        /* LINEAR RELATION between character index and stream position       
         * (It is not safe to assume that it can be computed)                */
        target_byte_pos =   CharacterIndex * me->byte_n_per_character
                          + me->byte_loader->initial_position;

        me->byte_loader->seek(me->byte_loader, target_byte_pos);
        if( me->byte_loader->tell(me->byte_loader) != target_byte_pos ) {
            me->byte_loader->seek(me->byte_loader, backup_byte_loader_position);
            return false;
        }
        me->character_index_next_to_fill = CharacterIndex;
        me->stomach_clear(me);
        return true;
    }

    /* STEPPING (WITHOUT COMPUTING) to the input position.                   */
    if( CharacterIndex < me->character_index_next_to_fill ) {
        /* Character index lies backward, so stepping needs to start from 
         * the initial position.                                             */
        QUEX_NAME(BufferFiller_character_index_reset)(me);
    }

    /* step_forward_n_characters() calls derived.load_characters() 
     * which increments 'character_index_next_to_fill'.                      */
    if( ! QUEX_NAME(BufferFiller_character_index_step_to)(me, (ptrdiff_t)CharacterIndex) ) {
        QUEX_NAME(BufferFiller_character_index_reset_backup)(me, 
                                                             backup_character_index_next_to_fill, 
                                                             backup_stomach_byte_n, 
                                                             backup_byte_loader_position);
        return false;
    }
    __quex_assert(me->character_index_next_to_fill == CharacterIndex);
    return true;
}

QUEX_INLINE void
QUEX_NAME(BufferFiller_character_index_reset)(QUEX_NAME(BufferFiller)* me)
/* Set the character index position to '0' and the byte loader to the initial
 * position. The 'stomach' of derived buffer fillers is cleared, so that 
 * filling may start from the beginning.                                     */
{
    if( me->byte_loader ) {
        me->byte_loader->seek(me->byte_loader, me->byte_loader->initial_position);
        if( me->byte_loader->tell(me->byte_loader) != me->byte_loader->initial_position ) {
            QUEX_ERROR_EXIT("QUEX_NAME(ByteLoader )failed to seek to initial position.\n");
        }
    }
    me->character_index_next_to_fill = 0;
    me->stomach_clear(me);
}

QUEX_INLINE void
QUEX_NAME(BufferFiller_character_index_reset_backup)(QUEX_NAME(BufferFiller)* me, 
                                                     QUEX_TYPE_STREAM_POSITION Backup_character_index_next_to_fill, 
                                                     ptrdiff_t                 BackupStomachByteN, 
                                                     QUEX_TYPE_STREAM_POSITION BackupByteLoaderPosition)
/* Reset a previous state of the BufferFiller and its QUEX_NAME(ByteLoader).            */
{
    QUEX_TYPE_STREAM_POSITION backup_byte_pos;

    if( BackupStomachByteN == -1 ) {
        /* Since it was not possible to determine the number of bytes in
         * the converter's stomach, the backup position must be reached 
         * by starting from the begining.                                    */
        QUEX_NAME(BufferFiller_character_index_reset)(me);
        if( ! QUEX_NAME(BufferFiller_character_index_step_to)(me, (ptrdiff_t)Backup_character_index_next_to_fill) ) {
            QUEX_ERROR_EXIT("BufferFiller failed to seek previously reached character.\n");
        }
        return;
    }

    backup_byte_pos = BackupByteLoaderPosition - BackupStomachByteN;
    me->byte_loader->seek(me->byte_loader, backup_byte_pos);
    me->stomach_clear(me);
    me->character_index_next_to_fill = Backup_character_index_next_to_fill;
}

QUEX_INLINE bool 
QUEX_NAME(BufferFiller_character_index_step_to)(QUEX_NAME(BufferFiller)*        me,
                                                const QUEX_TYPE_STREAM_POSITION TargetCI)
/* From the given 'character_index_next_to_fill' (i.e. the return value of 
 * 'input_character_tell()') step forward to character index 'TargetCI'. This 
 * function is used to reach a target input position in cases where computing 
 * is impossible.
 *
 * THIS FUNCTION DOES NOT BACKUP OR RE-INIT ANYTHING.
 * => Must be done upon failure in the caller's function.
 *
 * RETURNS: true - success; false - else.                                    */
{ 
    const QUEX_TYPE_STREAM_POSITION ChunkSize = QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE;
    QUEX_TYPE_CHARACTER             chunk[QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE];
    QUEX_TYPE_STREAM_POSITION       remaining_n = TargetCI - me->character_index_next_to_fill;
    bool                            end_of_stream_f = false;
    size_t                          loaded_n;

    __quex_assert(QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE >= 1);

    for(; remaining_n > ChunkSize; remaining_n -= ChunkSize ) {
        loaded_n = me->derived.load_characters(me, &chunk[0], (size_t)ChunkSize, &end_of_stream_f);
        me->character_index_next_to_fill += loaded_n;
        if( ChunkSize > loaded_n ) {
            return false;
        }
    }
    if( remaining_n ) {
        loaded_n = me->derived.load_characters(me, &chunk[0], (size_t)remaining_n, &end_of_stream_f);
        me->character_index_next_to_fill += loaded_n;
        if( remaining_n > loaded_n ) {
            return false;
        }
    }
   
    __quex_assert(me->character_index_next_to_fill == TargetCI);
    return true;
}


QUEX_NAMESPACE_MAIN_CLOSE

#endif /*   __QUEX_INCLUDE_GUARD__BUFFER__FILLER__BUFFER_FILLER_NAVIGATION_I */
