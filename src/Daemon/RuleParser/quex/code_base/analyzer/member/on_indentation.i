/* (C) 2010 Frank-Rene Schaefer 
 * ABSOLUTELY NO WARRANTY         */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__ON_INDENTATION_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__ON_INDENTATION_I

QUEX_NAMESPACE_MAIN_OPEN

#include <quex/code_base/analyzer/member/token-sending>
#include <quex/code_base/analyzer/Counter>

QUEX_INLINE __QUEX_TYPE_ANALYZER_RETURN_VALUE  
QUEX_NAME(on_indentation)(QUEX_TYPE_ANALYZER*    me, 
                          QUEX_TYPE_INDENTATION  Indentation, 
                          QUEX_TYPE_CHARACTER*   Begin)
{
#   define self  (*me)
    QUEX_NAME(IndentationStack)*  stack = &me->counter._indentation_stack;
#   if defined(QUEX_OPTION_TOKEN_POLICY_SINGLE)
    register QUEX_TYPE_TOKEN_ID __self_result_token_id 
           = (QUEX_TYPE_TOKEN_ID)__QUEX_SETTING_TOKEN_ID_UNINITIALIZED;
#   endif
    __QUEX_IF_TOKEN_REPETITION_SUPPORT(QUEX_TYPE_INDENTATION* start = 0);
    (void)Begin;

    if( Indentation > *(stack->back) ) {
        self_send(__QUEX_SETTING_TOKEN_ID_INDENT);
        ++(stack->back);
        if( stack->back == stack->memory_end ) QUEX_ERROR_EXIT("Indentation stack overflow.");
        *(stack->back) = Indentation;
        __QUEX_RETURN;
    }
    else if( Indentation == *(stack->back) ) {
        self_send(__QUEX_SETTING_TOKEN_ID_NODENT);
    }
    else {
        __QUEX_IF_TOKEN_REPETITION_SUPPORT(start = stack->back);
        /* From Above: (NOT Indentation > *back) and (NOT Indentation == *back),
         * Thus:       Indentation < *back.                                      */
        do {
            __QUEX_IF_TOKEN_REPETITION_SUPPORT_DISABLED(self_send(__QUEX_SETTING_TOKEN_ID_DEDENT));
            --(stack->back);
        } while( Indentation < *(stack->back) ); 

        if( Indentation == *(stack->back) ) { /* 'Landing' must happen on indentation border. */
            __QUEX_IF_TOKEN_REPETITION_SUPPORT(self_send_n((size_t)(start - stack->back), 
                                                           __QUEX_SETTING_TOKEN_ID_DEDENT));     
        } else {
            QUEX_ERROR_EXIT("Indentation Error");
        }
    }
    __QUEX_RETURN;
#   undef self 
}
QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__MEMBER__ON_INDENTATION_I */

