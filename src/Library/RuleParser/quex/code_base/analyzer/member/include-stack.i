/* -*- C++ -*- vim:set syntax=cpp:
 *
 * (C) 2004-2009 Frank-Rene Schaefer
 *
 * __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__INCLUDE_STACK_I may be undefined in case
 *    that multiple lexical analyzers are used. Then, the name of the
 *    QUEX_NAME(Accumulator) must be different.                             */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__INCLUDE_STACK_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__INCLUDE_STACK_I

#ifndef   QUEX_TYPE_ANALYZER
#   error "Macro QUEX_TYPE_ANALYZER must be defined before inclusion of this file."
#endif


#include <quex/code_base/temporary_macros_on>

QUEX_NAMESPACE_MAIN_OPEN

    TEMPLATE_IN(InputHandleT) void    
    QUEX_NAME(include_push)(QUEX_TYPE_ANALYZER*    me,
                            InputHandleT*          Optional_InputHandle,
                            QUEX_TYPE_CHARACTER*   Optional_InputName,
                            const QUEX_NAME(Mode)* Mode, 
                            const char*            CharacterCodecName /* = 0x0 */)
    {
        /* (A) Freezing and Copying away:
         *
         *     memento_pack(...): Store the lexical analyzer's to the state before including   */
        InputHandleT*        input_handle = Optional_InputHandle;
#       ifndef __QUEX_OPTION_PLAIN_C
        QUEX_NAME(Memento)*  m            = QUEX_NAME(memento_pack)<InputHandleT>(me, Optional_InputName, &input_handle);
#       else
        QUEX_NAME(Memento)*  m            = QUEX_NAME(memento_pack)(me, Optional_InputName, &input_handle);
#       endif

        if( input_handle == 0x0 ) {
            QUEX_ERROR_EXIT("Segment 'memento_pack' segment did not set the input_handle.");
        }

        /* (B) Initializations on Inclusion:
         *  
         *    (1) A new buffer is required for the new content.                           */
        QUEX_NAME(Buffer_construct)(&me->buffer, input_handle, 
                                    0x0, QUEX_SETTING_BUFFER_SIZE, 0x0,
                                    CharacterCodecName, QUEX_SETTING_TRANSLATION_BUFFER_SIZE,
                                    me->buffer._byte_order_reversion_active_f);

        /*    (2) If requested: transition to a specific mode for new file.               */
        if( Mode != 0x0 ) QUEX_NAME(set_mode_brutally)(me, (QUEX_NAME(Mode)*)Mode);
        /*        now leave alone:
         *               __current_mode_p 
         *               current_analyzer_function                                       
         *               DEBUG_analyzer_function_at_entry                                 */

        /*    (*) Mode stack is not subject to include handling.                          */
        /*    (*) Tokens and token queues are not subject to include handling.            */

        /*    (3) Initialize line, column, and indentation counters.                      */
        __QUEX_IF_COUNT( QUEX_NAME(Counter_construct)(&me->counter); )

        /*    (4) Initialize accumulator content.                                         */
#       ifdef QUEX_OPTION_STRING_ACCUMULATOR
        QUEX_NAME(Accumulator_construct)(&me->accumulator, me);
#       endif
        /*    (*) Post categorizer is not subject to include handling.                    */
        /*    (5) File handle allocated by **initial constructor** == 0x0 to prevent 
         *        deletion by pre-mature destructor call.                                 */
        me->__file_handle_allocated_by_constructor = 0x0;

        /*    (6) Keep track of 'who's your daddy?'                                       */
        me->_parent_memento = m;
    }   

    TEMPLATE_IN(InputHandleT) void    
    QUEX_NAME(include_push_input_name)(QUEX_TYPE_ANALYZER*      me,
                                       QUEX_TYPE_CHARACTER*     InputName,
                                       const QUEX_NAME(Mode)*   mode, 
                                       const char*              CharacterCodecName /* = 0x0 */)
    {
        /* The Optional_InputHandle = 0x0, which indicates that InputName tells how to 
         * open the input stream.                                                       */
#       ifndef __QUEX_OPTION_PLAIN_C
        QUEX_NAME(include_push)<InputHandleT>(me, 0x0, InputName, mode, CharacterCodecName);
#       else
        QUEX_NAME(include_push)(me, 0x0, InputName, mode, CharacterCodecName);
#       endif
    }

    TEMPLATE_IN(InputHandleT) void    
    QUEX_NAME(include_push_input_handle)(QUEX_TYPE_ANALYZER*      me,
                                         InputHandleT*            sh,
                                         const QUEX_NAME(Mode)*   mode, 
                                         const char*              CharacterCodecName /* = 0x0 */)
    {
        /* The Optional_InputName = 0x0, which indicates that the InputName is defined. */
#       ifndef __QUEX_OPTION_PLAIN_C
        QUEX_NAME(include_push)<InputHandleT>(me, sh, 0x0, mode, CharacterCodecName);
#       else
        QUEX_NAME(include_push)(me, sh, 0x0, mode, CharacterCodecName);
#       endif
    }

    QUEX_INLINE bool
    QUEX_NAME(include_pop)(QUEX_TYPE_ANALYZER* me) 
    {
        /* Not included? return 'false' to indicate we're on the top level       */
        if( me->_parent_memento == 0x0 ) return false; 

        /* (A) Uninitializing of unused objects:                                 
         *    (1) A new buffer is required for the new content.                           */
        QUEX_NAME(Buffer_destruct)(&me->buffer);

        /*    (2) Current mode will be determined by unfreeze/copy back.                  */
        /*        now leave alone:
         *               __current_mode_p 
         *               current_analyzer_function                                       
         *               DEBUG_analyzer_function_at_entry                                 */

        /*    (*) Mode stack is not subject to include handling.                          */
        /*    (*) Tokens and token queues are not subject to include handling.            */

        /*    (3) Counters will be determined by unfreeze/copy back.                      */
        /*    (4) Allocated accumulator content needs to be freed, previous content is      
         *        restored by unfreeze/copy back.                                         */
#       ifdef QUEX_OPTION_STRING_ACCUMULATOR
        QUEX_NAME(Accumulator_destruct)(&me->accumulator);
#       endif
        /*    (*) Post categorizer is not subject to include handling.                    */
        /*    (5) File handle allocated by **initial constructor** reset by unfreeze.     */

        /*    (6) Keep track of 'who's your daddy?' handled by unfreeze/copy back.        */

        /* (B) Unfreezing / Copy Back of content that was stored upon inclusion.          */
        QUEX_NAME(memento_unpack)(me, me->_parent_memento);

        /* Return to including file succesful */
        return true;
    }

    QUEX_INLINE void
    QUEX_NAME(include_stack_delete)(QUEX_TYPE_ANALYZER* me) 
    {
        while( me->_parent_memento != 0x0 ) {
            if( QUEX_NAME(include_pop)(me) == false ) {
                QUEX_ERROR_EXIT("Error during deletion of include stack.");
            }
        }
    }

#if ! defined( __QUEX_OPTION_PLAIN_C )
    TEMPLATE_IN(InputHandleT) void    
    QUEX_MEMBER(include_push)(InputHandleT*          sh,
                              const QUEX_NAME(Mode)* Mode, 
                              const char*            CharacterCodecName /* = 0x0 */)
    { QUEX_NAME(include_push_input_handle)<InputHandleT>(this, sh, Mode, CharacterCodecName); }

    TEMPLATE_IN(InputHandleT) void    
    QUEX_MEMBER(include_push)(QUEX_TYPE_CHARACTER*   InputName,
                              const QUEX_NAME(Mode)* Mode, 
                              const char*            CharacterCodecName /* = 0x0 */)
    { QUEX_NAME(include_push_input_name)<InputHandleT>(this, InputName, Mode, CharacterCodecName); }

    QUEX_INLINE bool
    QUEX_MEMBER(include_pop)() 
    { return QUEX_NAME(include_pop)(this); }

    QUEX_INLINE void
    QUEX_MEMBER(include_stack_delete)() 
    { QUEX_NAME(include_stack_delete)(this); }
#endif

QUEX_NAMESPACE_MAIN_CLOSE

#include <quex/code_base/temporary_macros_off>

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__INCLUDE_STACK_I */
