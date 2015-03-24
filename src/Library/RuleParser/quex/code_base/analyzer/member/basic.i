/* -*- C++ -*- vim: set syntax=cpp: */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__BASIC_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__BASIC_I

#include    <quex/code_base/definitions>
#include    <quex/code_base/buffer/Buffer>
#include    <quex/code_base/buffer/BufferFiller>
#include    <quex/code_base/buffer/asserts>
#ifdef      QUEX_OPTION_INCLUDE_STACK
#   include <quex/code_base/analyzer/member/include-stack>
#endif

#include <quex/code_base/temporary_macros_on>

QUEX_NAMESPACE_MAIN_OPEN
    struct QUEX_NAME(Mode_tag);

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(construct_basic)(QUEX_TYPE_ANALYZER*     me,
                               InputHandleT*           input_handle,
                               QUEX_TYPE_CHARACTER*    BufferMemory,
                               const size_t            BufferMemorySize,
                               QUEX_TYPE_CHARACTER*    EndOfFileP,
                               const char*             CharacterEncodingName, 
                               const size_t            TranslationBufferMemorySize,
                               bool                    ByteOrderReversionF)
    /* Initialization of Components of the Lexical Analyzer Engine ________________________
     *
     * input_handle == 0x0 means that there is no stream/file to read from. Instead, the 
     *                     user intends to perform the lexical analysis directly on plain
     *                     memory. In this case, the user needs to call the following function
     *                     by hand in order to setup the memory:
     *
     *                     QuexBufferMemory_construct(analyse->buffer._memory, 
     *                                                (uint8_t*)MyMemoryP, MyMemorySize); 
     */
    {
#       if      defined(QUEX_OPTION_ASSERTS) \
           && ! defined(QUEX_OPTION_ASSERTS_WARNING_MESSAGE_DISABLED)
        __QUEX_STD_printf(__QUEX_MESSAGE_ASSERTS_INFO);
#       endif
       
#       if      defined(QUEX_OPTION_ASSERTS) 
        if( QUEX_SETTING_BUFFER_LIMIT_CODE == QUEX_SETTING_PATH_TERMINATION_CODE ) {
            QUEX_ERROR_EXIT("Path termination code (PTC) and buffer limit code (BLC) must be different.\n");
        }
#       endif

#       if      ! defined(__QUEX_OPTION_CONVERTER) \
             ||   defined(__QUEX_OPTION_ENGINE_RUNNING_ON_CODEC)
        if( CharacterEncodingName != (void*)0 ) {
            __QUEX_STD_printf(__QUEX_MESSAGE_CHARACTER_ENCODING_SPECIFIED_WITHOUT_CONVERTER, CharacterEncodingName);
        }
#       endif

#       if defined(QUEX_OPTION_TOKEN_POLICY_QUEUE)
#           if defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
            /* Assume that the user will pass us a constructed token queue */
            QUEX_NAME(TokenQueue_init)(&me->_token_queue, 0, 0x0);
#           else
            QUEX_NAME(TokenQueue_construct)(&me->_token_queue, 
                                            (QUEX_TYPE_TOKEN*)&me->__memory_token_queue,
                                            QUEX_SETTING_TOKEN_QUEUE_SIZE);
#           endif
#       else
#           if defined(QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY)
            /* Assume that the user will pass us a constructed token */
            me->token = 0x0;     
#           else
            me->token = &me->__memory_token;     
#           ifdef __QUEX_OPTION_PLAIN_C
            QUEX_NAME_TOKEN(construct)(me->token);
#           endif
#           endif
#       endif
       
        __QUEX_IF_COUNT( QUEX_NAME(Counter_construct)(&me->counter); )

#       ifdef QUEX_OPTION_STRING_ACCUMULATOR
        QUEX_NAME(Accumulator_construct)(&me->accumulator, (QUEX_TYPE_ANALYZER*)me);
#       endif
       
#       ifdef  QUEX_OPTION_INCLUDE_STACK
        me->_parent_memento = 0x0;
#       endif

#       ifdef QUEX_OPTION_POST_CATEGORIZER
        QUEX_NAME(PostCategorizer_construct)(&me->post_categorizer);
#       endif

        me->_mode_stack.end        = me->_mode_stack.begin;
        me->_mode_stack.memory_end = me->_mode_stack.begin + QUEX_SETTING_MODE_STACK_SIZE;

#       ifdef QUEX_OPTION_ASSERTS
        /* Initialize everything to 0xFF which is most probably causing an error
         * if a member variable is not initialized before it is used.             */
        __QUEX_STD_memset((uint8_t*)&me->buffer, 0xFF, sizeof(me->buffer));
#       endif
        
        QUEX_NAME(Buffer_construct)(&me->buffer, input_handle, 
                                    BufferMemory, BufferMemorySize, EndOfFileP,
                                    CharacterEncodingName, TranslationBufferMemorySize,
                                    ByteOrderReversionF);

        if( input_handle == 0x0 ) {
            /* TWO CASES:
             * (1) The user provides a buffer memory: --> assume it is filled to the end.
             * (2) The user does not provide memory:  --> the memory IS empty.             */
            if( BufferMemory == 0x0 ) {
                /* 'buffer._memory._front' has been set at this point in time.             */
                QUEX_NAME(Buffer_end_of_file_set)(&me->buffer, me->buffer._memory._front + 1);
            }
            /* When working on plain memory, the '_end_of_file_p' must be set to indicate
             * the end of the content.                                                     */
            __quex_assert(me->buffer._memory._end_of_file_p >  me->buffer._memory._front);
            __quex_assert(me->buffer._memory._end_of_file_p <= me->buffer._memory._back);
        }

        me->__file_handle_allocated_by_constructor = 0x0;
    }

    TEMPLATE_IN(InputHandleT) void
    QUEX_NAME(reset_basic)(QUEX_TYPE_ANALYZER*  me,
                           InputHandleT*        input_handle, 
                           const char*          CharacterEncodingName, 
                           const size_t         TranslationBufferMemorySize)
        /* Reset of Components of the Lexical Analyzer Engine ____________________________*/
    {
        __QUEX_IF_COUNT( QUEX_NAME(Counter_reset)(&me->counter); )

#       ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
        QUEX_NAME(TokenQueue_reset)(&me->_token_queue);
#       endif

#       ifdef QUEX_OPTION_INCLUDE_STACK
        QUEX_NAME(include_stack_delete)((QUEX_TYPE_ANALYZER*)me);
#       endif

        /* IMPORTANT: THE ACCUMULATOR CAN ONLY BE DESTRUCTED AFTER THE INCLUDE *
         *            STACK HAS BEEN DELETED. OTHERWISE, THERE MIGHT BE LEAKS. */
#       ifdef QUEX_OPTION_STRING_ACCUMULATOR
        QUEX_NAME(Accumulator_clear)(&me->accumulator);
#       endif

#       ifdef QUEX_OPTION_POST_CATEGORIZER
        QUEX_NAME(PostCategorizer_clear)(&me->post_categorizer);
#       endif

        me->_mode_stack.end        = me->_mode_stack.begin;
        me->_mode_stack.memory_end = me->_mode_stack.begin + QUEX_SETTING_MODE_STACK_SIZE;

        QUEX_NAME(Buffer_reset)(&me->buffer, input_handle, CharacterEncodingName, TranslationBufferMemorySize);
    }


    QUEX_INLINE void
    QUEX_NAME(destruct_basic)(QUEX_TYPE_ANALYZER* me)
    {
#       ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE 
        QUEX_NAME(TokenQueue_destruct)(&me->_token_queue);
#       else
#           ifdef __QUEX_OPTION_PLAIN_C
            QUEX_NAME_TOKEN(destruct)(me->token);
#           endif
#       endif

#       ifdef QUEX_OPTION_INCLUDE_STACK
        QUEX_NAME(include_stack_delete)((QUEX_TYPE_ANALYZER*)me);
#       endif

        /* IMPORTANT: THE ACCUMULATOR CAN ONLY BE DESTRUCTED AFTER THE INCLUDE *
         *            STACK HAS BEEN DELETED. OTHERWISE, THERE MIGHT BE LEAKS. */
#       ifdef QUEX_OPTION_STRING_ACCUMULATOR
        QUEX_NAME(Accumulator_destruct)(&me->accumulator);
#       endif
       
#       ifdef QUEX_OPTION_POST_CATEGORIZER
        QUEX_NAME(PostCategorizer_destruct)(&me->post_categorizer);
#       endif

        QUEX_NAME(Buffer_destruct)(&me->buffer);

        if( me->__file_handle_allocated_by_constructor != 0x0 ) {
            __QUEX_STD_fclose(me->__file_handle_allocated_by_constructor); 
        }
    }

    /* NOTE: 'reload_forward()' needs to be implemented for each mode, because
     *       addresses related to acceptance positions need to be adapted. This
     *       is not the case for 'reload_backward()'. In no case of backward
     *       reloading, there are important addresses to keep track.            */
    QUEX_INLINE void 
    QUEX_NAME(buffer_reload_backward)(QUEX_NAME(Buffer)* buffer)
    {
        __quex_assert(buffer != 0x0);
        __quex_assert(buffer->filler != 0x0);

        if( buffer->on_buffer_content_change != 0x0 ) {
            /* In contrast to 'reload forward', a reload backward is very well 
             * conceivable, even if end of file pointer != 0x0.                          */
            buffer->on_buffer_content_change(buffer->_memory._front, 
                                             QUEX_NAME(Buffer_text_end)(buffer));
        }

        (void)QUEX_NAME(BufferFiller_load_backward)(buffer);
        
        /* Backward lexing happens in two cases:
         *
         *  (1) When checking for a pre-condition. In this case, no dedicated acceptance
         *      is involved. No acceptance positions are considered.
         *  (2) When tracing back to get the end of a core pattern in pseudo-ambigous
         *      post conditions. Then, no acceptance positions are involved, because
         *      the start of the lexeme shall not drop before the begin of the buffer 
         *      and the end of the core pattern, is of course, after the start of the 
         *      lexeme. => there will be no reload backwards.                            */
    }

    QUEX_INLINE size_t 
    QUEX_NAME(__buffer_reload_forward_core)(QUEX_NAME(Buffer)*  buffer) 
    {
        size_t loaded_character_n = (size_t)-1;

        __quex_assert(buffer != 0x0);
        __quex_assert(buffer->filler != 0x0);
        __quex_assert(buffer->_memory._end_of_file_p == 0x0);

        if( buffer->_memory._end_of_file_p != 0x0 ) {
            return 0;
        }

        if( buffer->on_buffer_content_change != 0x0 ) {
            /* If the end of file pointer is set, the reload will not be initiated,
             * and the buffer remains as is. No reload happens, see above. 
             * => HERE: end of content = end of buffer.                             */
            buffer->on_buffer_content_change(buffer->_memory._front, 
                                             buffer->_memory._back);
        }

        loaded_character_n = QUEX_NAME(BufferFiller_load_forward)(buffer);
        return loaded_character_n;
    }

    QUEX_INLINE void 
    QUEX_NAME(buffer_reload_forward)(QUEX_NAME(Buffer)*            buffer, 
                                     QUEX_TYPE_CHARACTER_POSITION* position_register,
                                     const size_t                  PositionRegisterN)
    {
        QUEX_TYPE_CHARACTER_POSITION*  iterator           = 0x0;
        QUEX_TYPE_CHARACTER_POSITION*  End                = position_register + (ptrdiff_t)PositionRegisterN;
        size_t                         loaded_character_n = (size_t)-1;    

        loaded_character_n = QUEX_NAME(__buffer_reload_forward_core)(buffer);

        for(iterator = position_register; iterator != End; ++iterator) {
            /* NOTE: When the post_context_start_position is still undefined the following operation may
             *       underflow. But, do not care, once it is **assigned** to a meaningful value, it won't */
            *iterator -= (ptrdiff_t)loaded_character_n;
        }
    }


QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/temporary_macros_off>

#include <quex/code_base/buffer/Buffer.i>

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__BASIC_I */
