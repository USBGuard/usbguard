/*  -*- C++ -*-  vim: set syntax=cpp: */
/* (C) 2008 Frank-Rene Schaefer*/
#ifndef __QUEX_INCLUDE_GUARD__BUFFER__PLAIN__BUFFER_FILLER_PLAIN_I
#define __QUEX_INCLUDE_GUARD__BUFFER__PLAIN__BUFFER_FILLER_PLAIN_I
/**/

#if ! defined (__QUEX_OPTION_PLAIN_C)
#   include <iostream> 
#   include <cerrno>
#   include <stdexcept>
#endif
#include <quex/code_base/definitions>
#include <quex/code_base/buffer/InputPolicy>
#include <quex/code_base/buffer/Buffer>
#include <quex/code_base/buffer/BufferFiller>
#include <quex/code_base/MemoryManager>

#include <quex/code_base/temporary_macros_on>

QUEX_NAMESPACE_MAIN_OPEN

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(BufferFiller_Plain_construct)(TEMPLATED(BufferFiller_Plain)*, InputHandleT*    input_handle);

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(BufferFiller_Plain_init)(TEMPLATED(BufferFiller_Plain)* me, InputHandleT*    input_handle);

    TEMPLATE_IN(InputHandleT) ptrdiff_t QUEX_NAME(__BufferFiller_Plain_tell_character_index)(QUEX_NAME(BufferFiller)* alter_ego);
    TEMPLATE_IN(InputHandleT) void   QUEX_NAME(__BufferFiller_Plain_seek_character_index)(
                                           QUEX_NAME(BufferFiller)*  alter_ego, 
                                           const ptrdiff_t           CharacterIndex); 
    TEMPLATE_IN(InputHandleT) size_t QUEX_NAME(__BufferFiller_Plain_read_characters)(QUEX_NAME(BufferFiller)*    alter_ego,
                                                                          QUEX_TYPE_CHARACTER* start_of_buffer, 
                                                                          const size_t         N);
    TEMPLATE_IN(InputHandleT) void   QUEX_NAME(__BufferFiller_Plain_delete_self)(QUEX_NAME(BufferFiller)* alter_ego);


    TEMPLATE_IN(InputHandleT) TEMPLATED(BufferFiller_Plain)*
    QUEX_NAME(BufferFiller_Plain_new)(InputHandleT*    input_handle)
    {
        TEMPLATED(BufferFiller_Plain)*  me = \
             (TEMPLATED(BufferFiller_Plain)*) \
              QUEXED(MemoryManager_allocate)(sizeof(TEMPLATED(BufferFiller_Plain)),
                                                QUEXED(MemoryObjectType_BUFFER_FILLER));
        __quex_assert(me != 0x0);
        __quex_assert(input_handle != 0x0);

        QUEX_NAME(BufferFiller_Plain_construct)(me, input_handle);

        return me;
    }

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(BufferFiller_Plain_construct)(TEMPLATED(BufferFiller_Plain)* me, InputHandleT*    input_handle)
    {
        QUEX_NAME(BufferFiller_setup_functions)(&me->base,
                                   TEMPLATED(__BufferFiller_Plain_tell_character_index),
                                   TEMPLATED(__BufferFiller_Plain_seek_character_index), 
                                   TEMPLATED(__BufferFiller_Plain_read_characters),
                                   TEMPLATED(__BufferFiller_Plain_delete_self));

        QUEX_NAME(BufferFiller_Plain_init)(me, input_handle);
    }

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(BufferFiller_Plain_init)(TEMPLATED(BufferFiller_Plain)* me, InputHandleT*    input_handle)
    {
        me->ih             = input_handle;
        me->start_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
#       ifdef QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION
        me->_character_index = 0;
#       endif
        me->_last_stream_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
    }

    TEMPLATE_IN(InputHandleT) void 
    QUEX_NAME(__BufferFiller_Plain_delete_self)(QUEX_NAME(BufferFiller)* alter_ego) 
    {
        TEMPLATED(BufferFiller_Plain)* me = (TEMPLATED(BufferFiller_Plain)*)alter_ego;
        QUEXED(MemoryManager_free)((void*)me, QUEXED(MemoryObjectType_BUFFER_FILLER));

    }

    TEMPLATE_IN(InputHandleT) ptrdiff_t 
    QUEX_NAME(__BufferFiller_Plain_tell_character_index)(QUEX_NAME(BufferFiller)* alter_ego) 
    { 
       /* The type cast is necessary, since the function signature needs to 
        * work with the first argument being of base class type. */
       TEMPLATED(BufferFiller_Plain)* me = (TEMPLATED(BufferFiller_Plain)*)alter_ego;

       __quex_assert(alter_ego != 0x0); 
       __quex_assert(me->ih != 0x0); 
       /* Ensure, that the stream position is only influenced by
        *    __read_characters(...) 
        *    __seek_character_index(...)                                             */
       __quex_assert(me->_last_stream_position == QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT)); 
#      ifdef QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION
       return me->_character_index;
#      else
       /* The stream position type is most likely >= size_t >= ptrdiff_t so let the 
        * computation happen with that type, then cast to what needs to be returned. */
       return (ptrdiff_t)(  (me->_last_stream_position - me->start_position) 
                          / (STREAM_POSITION_TYPE(InputHandleT))sizeof(QUEX_TYPE_CHARACTER));
#      endif
    }

#   if ! defined(QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION)
    /* NOTE: This differs from QuexBuffer_seek(...) in the sense, that it only sets the
     *       stream to a particular position given by a character index. QuexBuffer_seek(..)
     *       sets the _input_p to a particular position.                                      */
    TEMPLATE_IN(InputHandleT) void 
    QUEX_NAME(__BufferFiller_Plain_seek_character_index)(QUEX_NAME(BufferFiller)* alter_ego, 
                                                         const ptrdiff_t          CharacterIndex) 
    { 
        TEMPLATED(BufferFiller_Plain)* me = (TEMPLATED(BufferFiller_Plain)*)alter_ego;
        long                           avoid_tmp_arg = -1;

        __quex_assert(alter_ego != 0x0); 
        /* The type cast is necessary, since the function signature needs to 
         * work with the first argument being of base class type. */
        __quex_assert(me->ih != 0x0); 

        avoid_tmp_arg =   (long)( ((size_t)CharacterIndex) * sizeof(QUEX_TYPE_CHARACTER)) \
                        + (long)(me->start_position); 

        QUEX_INPUT_POLICY_SEEK(me->ih, InputHandleT, avoid_tmp_arg);
        me->_last_stream_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
    }
#   else
    /* Implementation for 'strange streams', i.e. streams where the input position increase is not
     * necessarily proportional to the amount of read-in characters. Note, that the seek function is
     * the only function that is significantly different for this case.                           */
    TEMPLATE_IN(InputHandleT) void 
    QUEX_NAME(__BufferFiller_Plain_seek_character_index)(QUEX_NAME(BufferFiller)* alter_ego, 
                                                         const ptrdiff_t          CharacterIndex) 
    { 
        __quex_assert(alter_ego != 0x0); 
        TEMPLATED(BufferFiller_Plain)* me = (TEMPLATED(BufferFiller_Plain)*)alter_ego;
        __quex_assert(me->ih != 0x0); 

        if     ( me->_character_index == CharacterIndex ) {
            return;
        }
        else if( me->_character_index < CharacterIndex ) {
            QUEX_NAME(BufferFiller_step_forward_n_characters)(alter_ego, CharacterIndex - me->_character_index);
        }
        else { /* me->_character_index > CharacterIndex */
            QUEX_INPUT_POLICY_SEEK(me->ih, InputHandleT, me->start_position);
#           ifdef QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION
            me->_last_stream_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
#           endif
            QUEX_NAME(BufferFiller_step_forward_n_characters)(alter_ego, CharacterIndex);
        }
        me->_last_stream_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
    }
#   endif

    TEMPLATE_IN(InputHandleT) size_t   
    QUEX_NAME(__BufferFiller_Plain_read_characters)(QUEX_NAME(BufferFiller)*  alter_ego,
                                                    QUEX_TYPE_CHARACTER*      buffer_memory, 
                                                    const size_t              N)  
    { 
        TEMPLATED(BufferFiller_Plain)* me         = (TEMPLATED(BufferFiller_Plain)*)alter_ego;
        size_t                         ByteN      = (size_t)-1;
        size_t                         CharacterN = (size_t)-1;

        __quex_assert(alter_ego != 0x0); 
        __quex_assert(buffer_memory != 0x0); 
        /* The type cast is necessary, since the function signature needs to 
         * work with the first argument being of base class type. */
#       ifdef QUEX_OPTION_ASSERTS
        __QUEX_STD_memset((uint8_t*)buffer_memory, 0xFF, N * sizeof(QUEX_TYPE_CHARACTER));
#       endif

        __quex_assert(me->ih != 0x0); 
        ByteN = QUEX_INPUT_POLICY_LOAD_BYTES(me->ih, InputHandleT, 
                                             buffer_memory, N * sizeof(QUEX_TYPE_CHARACTER));

        if( ByteN % sizeof(QUEX_TYPE_CHARACTER) != 0 ) 
            QUEX_ERROR_EXIT("Error: End of file cuts in the middle a multi-byte character.");

        CharacterN = ByteN / sizeof(QUEX_TYPE_CHARACTER); 

#       ifdef QUEX_OPTION_STRANGE_ISTREAM_IMPLEMENTATION
        me->_character_index += (ptrdiff_t)CharacterN;
#       endif

        me->_last_stream_position = QUEX_INPUT_POLICY_TELL(me->ih, InputHandleT);
        return CharacterN;
    }

#   undef TEMPLATED_CLASS

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/temporary_macros_off>

#include <quex/code_base/buffer/BufferFiller.i>

#endif /* __INCLUDE_GUARD__QUEX_BUFFER_INPUT_STRATEGY_PLAIN_I__ */
