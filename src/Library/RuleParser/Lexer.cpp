
#include "Lexer.hpp"

#include <quex/code_base/analyzer/C-adaptions.h>
/* The file 'multi.i' contains implementations which are the same for all 
 * possibly generated analyzers. If QUEX_OPTION_MULTI is defined, it is
 * NOT supposed to be included here. If not--in which case we have a single
 * analzer--then it is included.                                             */
#include <quex/code_base/single.i>

QUEX_NAMESPACE_MAIN_OPEN
QUEX_NAME(Mode) QUEX_NAME(PQ_STRING_READER) = {
    /* id                */ QUEX_NAME(ModeID_PQ_STRING_READER),
    /* name              */ "PQ_STRING_READER",
#   if      defined(QUEX_OPTION_INDENTATION_TRIGGER) \
       && ! defined(QUEX_OPTION_INDENTATION_DEFAULT_HANDLER)
    /* on_indentation    */ QUEX_NAME(Mode_on_indentation_null_function),
#   endif
    /* on_entry          */ QUEX_NAME(PQ_STRING_READER_on_entry),
    /* on_exit           */ QUEX_NAME(PQ_STRING_READER_on_exit),
#   if      defined(QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK)
    /* has_base          */ QUEX_NAME(PQ_STRING_READER_has_base),
    /* has_entry_from    */ QUEX_NAME(PQ_STRING_READER_has_entry_from),
    /* has_exit_to       */ QUEX_NAME(PQ_STRING_READER_has_exit_to),
#   endif
    /* analyzer_function */ QUEX_NAME(PQ_STRING_READER_analyzer_function)
};
QUEX_NAME(Mode) QUEX_NAME(RULE) = {
    /* id                */ QUEX_NAME(ModeID_RULE),
    /* name              */ "RULE",
#   if      defined(QUEX_OPTION_INDENTATION_TRIGGER) \
       && ! defined(QUEX_OPTION_INDENTATION_DEFAULT_HANDLER)
    /* on_indentation    */ QUEX_NAME(Mode_on_indentation_null_function),
#   endif
    /* on_entry          */ QUEX_NAME(Mode_on_entry_exit_null_function),
    /* on_exit           */ QUEX_NAME(Mode_on_entry_exit_null_function),
#   if      defined(QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK)
    /* has_base          */ QUEX_NAME(RULE_has_base),
    /* has_entry_from    */ QUEX_NAME(RULE_has_entry_from),
    /* has_exit_to       */ QUEX_NAME(RULE_has_exit_to),
#   endif
    /* analyzer_function */ QUEX_NAME(RULE_analyzer_function)
};
QUEX_NAME(Mode) QUEX_NAME(DQ_STRING_READER) = {
    /* id                */ QUEX_NAME(ModeID_DQ_STRING_READER),
    /* name              */ "DQ_STRING_READER",
#   if      defined(QUEX_OPTION_INDENTATION_TRIGGER) \
       && ! defined(QUEX_OPTION_INDENTATION_DEFAULT_HANDLER)
    /* on_indentation    */ QUEX_NAME(Mode_on_indentation_null_function),
#   endif
    /* on_entry          */ QUEX_NAME(DQ_STRING_READER_on_entry),
    /* on_exit           */ QUEX_NAME(DQ_STRING_READER_on_exit),
#   if      defined(QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK)
    /* has_base          */ QUEX_NAME(DQ_STRING_READER_has_base),
    /* has_entry_from    */ QUEX_NAME(DQ_STRING_READER_has_entry_from),
    /* has_exit_to       */ QUEX_NAME(DQ_STRING_READER_has_exit_to),
#   endif
    /* analyzer_function */ QUEX_NAME(DQ_STRING_READER_analyzer_function)
};

QUEX_NAME(Mode)* (QUEX_NAME(mode_db)[__QUEX_SETTING_MAX_MODE_CLASS_N]) = {
    &QUEX_NAME(PQ_STRING_READER),
    &QUEX_NAME(RULE),
    &QUEX_NAME(DQ_STRING_READER)
};
#ifndef __QUEX_INDICATOR_DUMPED_TOKEN_ID_DEFINED
    static QUEX_TYPE_TOKEN_ID    QUEX_NAME_TOKEN(DumpedTokenIdObject);
#endif
#define self  (*(QUEX_TYPE_DERIVED_ANALYZER*)me)
#define __self_result_token_id    QUEX_NAME_TOKEN(DumpedTokenIdObject)

void
QUEX_NAME(PQ_STRING_READER_on_entry)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* FromMode) {
    (void)me;
    (void)FromMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(PQ_STRING_READER).has_entry_from(FromMode);
#   endif
 self_accumulator_clear(); 
}

void
QUEX_NAME(PQ_STRING_READER_on_exit)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* ToMode)  {
    (void)me;
    (void)ToMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(PQ_STRING_READER).has_exit_to(ToMode);
#   endif

		self_accumulator_flush(RULE_TOKEN_PQ_STRING);
		self_send(RULE_TOKEN_PQ_STRING_END);
	
}

#if defined(QUEX_OPTION_INDENTATION_TRIGGER) 
void
QUEX_NAME(PQ_STRING_READER_on_indentation)(QUEX_TYPE_ANALYZER*    me, 
                                        QUEX_TYPE_INDENTATION  Indentation, 
                                        QUEX_TYPE_CHARACTER*   Begin) {
    (void)me;
    (void)Indentation;
    (void)Begin;
    return;
}
#endif

#ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
bool
QUEX_NAME(PQ_STRING_READER_has_base)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;
    return false;
}
bool
QUEX_NAME(PQ_STRING_READER_has_entry_from)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_RULE): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(RULE)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): RULE, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(PQ_STRING_READER_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_RULE): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(RULE)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): RULE, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
#endif    

void
QUEX_NAME(RULE_on_entry)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* FromMode) {
    (void)me;
    (void)FromMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(RULE).has_entry_from(FromMode);
#   endif

}

void
QUEX_NAME(RULE_on_exit)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* ToMode)  {
    (void)me;
    (void)ToMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(RULE).has_exit_to(ToMode);
#   endif

}

#if defined(QUEX_OPTION_INDENTATION_TRIGGER) 
void
QUEX_NAME(RULE_on_indentation)(QUEX_TYPE_ANALYZER*    me, 
                                        QUEX_TYPE_INDENTATION  Indentation, 
                                        QUEX_TYPE_CHARACTER*   Begin) {
    (void)me;
    (void)Indentation;
    (void)Begin;
    return;
}
#endif

#ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
bool
QUEX_NAME(RULE_has_base)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;
    return false;
}
bool
QUEX_NAME(RULE_has_entry_from)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_DQ_STRING_READER): return true;
    case QUEX_NAME(ModeID_PQ_STRING_READER): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(DQ_STRING_READER)) ) return true;
        if( Mode->has_base(&QUEX_NAME(PQ_STRING_READER)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): DQ_STRING_READER, PQ_STRING_READER, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(RULE_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_DQ_STRING_READER): return true;
    case QUEX_NAME(ModeID_PQ_STRING_READER): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(DQ_STRING_READER)) ) return true;
        if( Mode->has_base(&QUEX_NAME(PQ_STRING_READER)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): DQ_STRING_READER, PQ_STRING_READER, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
#endif    

void
QUEX_NAME(DQ_STRING_READER_on_entry)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* FromMode) {
    (void)me;
    (void)FromMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(DQ_STRING_READER).has_entry_from(FromMode);
#   endif
 self_accumulator_clear(); 
}

void
QUEX_NAME(DQ_STRING_READER_on_exit)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* ToMode)  {
    (void)me;
    (void)ToMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(DQ_STRING_READER).has_exit_to(ToMode);
#   endif

		self_accumulator_flush(RULE_TOKEN_DQ_STRING);
		self_send(RULE_TOKEN_DQ_STRING_END);
	
}

#if defined(QUEX_OPTION_INDENTATION_TRIGGER) 
void
QUEX_NAME(DQ_STRING_READER_on_indentation)(QUEX_TYPE_ANALYZER*    me, 
                                        QUEX_TYPE_INDENTATION  Indentation, 
                                        QUEX_TYPE_CHARACTER*   Begin) {
    (void)me;
    (void)Indentation;
    (void)Begin;
    return;
}
#endif

#ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
bool
QUEX_NAME(DQ_STRING_READER_has_base)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;
    return false;
}
bool
QUEX_NAME(DQ_STRING_READER_has_entry_from)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_RULE): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(RULE)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): RULE, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(DQ_STRING_READER_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_RULE): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(RULE)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): RULE, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
#endif    
#undef self
#undef __self_result_token_id
QUEX_NAMESPACE_MAIN_CLOSE

/* #include "Lexer.hpp"*/
QUEX_NAMESPACE_MAIN_OPEN
QUEX_TYPE_CHARACTER  QUEX_LEXEME_NULL_IN_ITS_NAMESPACE = (QUEX_TYPE_CHARACTER)0;
#ifdef      __QUEX_COUNT_VOID
#   undef   __QUEX_COUNT_VOID
#endif
#ifdef      __QUEX_OPTION_COUNTER
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) \
            do {                              \
                QUEX_NAME(PQ_STRING_READER_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif
#ifdef __QUEX_OPTION_COUNTER
static void
QUEX_NAME(PQ_STRING_READER_counter)(QUEX_TYPE_ANALYZER* me, QUEX_TYPE_CHARACTER* LexemeBegin, QUEX_TYPE_CHARACTER* LexemeEnd)
{
#   define self (*me)
/*  'QUEX_GOTO_STATE' requires 'QUEX_LABEL_STATE_ROUTER' */
#   define QUEX_LABEL_STATE_ROUTER _18
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
#   ifdef QUEX_OPTION_COLUMN_NUMBER_COUNTING
    QUEX_TYPE_CHARACTER*           reference_p                    = (QUEX_TYPE_CHARACTER*)0x0;
#   endif /* QUEX_OPTION_COLUMN_NUMBER_COUNTING */
    (void)me;
    __QUEX_IF_COUNT_SHIFT_VALUES();
    /* Allow LexemeBegin == LexemeEnd (e.g. END_OF_STREAM)
     * => Caller does not need to check
     * BUT, if so quit immediately after 'shift values'. */
    __quex_assert(LexemeBegin <= LexemeEnd);
    if(LexemeBegin == LexemeEnd) return;
    me->buffer._read_p = LexemeBegin;

    /* (504 from BEFORE_ENTRY)  */
    input = *(me->buffer._read_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._read_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(504);
if     ( input >= 0xB )  goto _2;
else if( input == 0xA )  goto _3;
else if( input == 0x9 )  goto _4;
else                     goto _2;


    __quex_assert_no_passage();
_8:
    /* (504 from 508)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 506)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 505)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 507)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (505 from 504)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(505);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (506 from 504)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(506);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (507 from 504)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(507);
goto _7;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_11:
    __quex_debug("* TERMINAL LINE\n");
__QUEX_IF_COUNT_LINES_ADD((size_t)1);

    __QUEX_IF_COUNT_COLUMNS((me->counter._column_number_at_end) = (size_t)1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._read_p));

if( me->buffer._read_p != LexemeEnd ) goto _8;

goto _0;

_13:
    __quex_debug("* TERMINAL GRID\n");
__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._read_p) - reference_p - 1)));

__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end -= 1);
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end &= ~ ((size_t)0x3));
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end += 4 + 1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._read_p));

if( me->buffer._read_p != LexemeEnd ) goto _8;

goto _0;

_12:
    __quex_debug("* TERMINAL COLUMN\n");
if( me->buffer._read_p != LexemeEnd ) goto _8;

__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._read_p) - reference_p)));

goto _0;

_17:
    __quex_debug("* TERMINAL <BEYOND>\n");
    --(me->buffer._read_p);

__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._read_p) - reference_p)));

goto _0;

_19: /* TERMINAL: FAILURE */
goto _17;
_0:
     __quex_assert(me->buffer._read_p == LexemeEnd); /* Otherwise, lexeme violates codec character boundaries. */
    return;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_18:
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
#    if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
     goto _18; /* in QUEX_GOTO_STATE       */
#    endif
    /* Avoid compiler warning: Unused label for 'TERMINAL <BEYOND>' */
    goto _17;
    goto _19;
    (void)target_state_index;
    (void)target_state_else_index;
}
#endif /* __QUEX_OPTION_COUNTER */

#include <quex/code_base/analyzer/struct/basic>
#include <quex/code_base/buffer/Buffer>
#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
#   include <quex/code_base/token/TokenQueue>
#endif

#ifdef    CONTINUE
#   undef CONTINUE
#endif
#define   CONTINUE do { goto _0; } while(0)

#ifdef    RETURN
#   undef RETURN
#endif
#define   RETURN   do { goto _1; } while(0)

__QUEX_TYPE_ANALYZER_RETURN_VALUE  
QUEX_NAME(PQ_STRING_READER_analyzer_function)(QUEX_TYPE_ANALYZER* me) 
{
    /* NOTE: Different modes correspond to different analyzer functions. The 
     *       analyzer functions are all located inside the main class as static
     *       functions. That means, they are something like 'globals'. They 
     *       receive a pointer to the lexical analyzer, since static members do
     *       not have access to the 'this' pointer.                          */
#   if defined(QUEX_OPTION_TOKEN_POLICY_SINGLE)
    register QUEX_TYPE_TOKEN_ID __self_result_token_id 
           = (QUEX_TYPE_TOKEN_ID)__QUEX_SETTING_TOKEN_ID_UNINITIALIZED;
#   endif
#   ifdef     self
#       undef self
#   endif
#   define self (*((QUEX_TYPE_ANALYZER*)me))
/*  'QUEX_GOTO_STATE' requires 'QUEX_LABEL_STATE_ROUTER' */
#   define QUEX_LABEL_STATE_ROUTER _51
#   define PQ_STRING_READER    (QUEX_NAME(PQ_STRING_READER))
#   define RULE                (QUEX_NAME(RULE))
#   define DQ_STRING_READER    (QUEX_NAME(DQ_STRING_READER))

    /* Lexeme setup: 
     *
     * There is a temporary zero stored at the end of each lexeme, if the action 
     * references to the 'Lexeme'. 'LexemeNull' provides a reference to an empty
     * zero terminated string.                                                    */
#if defined(QUEX_OPTION_ASSERTS)
#   define Lexeme       QUEX_NAME(access_Lexeme)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeBegin  QUEX_NAME(access_LexemeBegin)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeL      QUEX_NAME(access_LexemeL)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeEnd    QUEX_NAME(access_LexemeEnd)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#else
#   define Lexeme       (me->buffer._lexeme_start_p)
#   define LexemeBegin  Lexeme
#   define LexemeL      ((size_t)(me->buffer._read_p - me->buffer._lexeme_start_p))
#   define LexemeEnd    me->buffer._read_p
#endif

#define LexemeNull      (&QUEX_LEXEME_NULL)
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_CHARACTER**          position                       = 0x0;

    /* Post context positions do not have to be reset or initialized. If a state
     * is reached which is associated with 'end of post context' it is clear what
     * post context is meant. This results from the ways the state machine is 
     * constructed. Post context position's live cycle:
     *
     * (1)   unitialized (don't care)
     * (1.b) on buffer reload it may, or may not be adapted (don't care)
     * (2)   when a post context begin state is passed, then it is **SET** (now: take care)
     * (2.b) on buffer reload it **is adapted**.
     * (3)   when a terminal state of the post context is reached (which can only be reached
     *       for that particular post context), then the post context position is used
     *       to reset the input position.                                              */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE) \
       || defined(QUEX_OPTION_ASSERTS)
    me->DEBUG_analyzer_function_at_entry = me->current_analyzer_function;
#   endif
_50:
    me->buffer._lexeme_start_p = me->buffer._read_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);
_3:
    /* (474 from BEFORE_ENTRY) (474 from RELOAD_FORWARD)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(474);
switch( input ) {
case 0x0: goto _23;
case 0x1: 
case 0xA: goto _12;
case 0x29: goto _8;
case 0x5C: goto _9;
default: goto _7;
}


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 475) (DROP_OUT from 477)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 482)  */
    goto _34;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 480)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 478)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 476)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 474)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 481)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 479)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (480 from 477)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(480);
goto _17;


    __quex_assert_no_passage();
_5:
    /* (481 from 479)  */
    ++(me->buffer._read_p);

_40:
    input = *(me->buffer._read_p);


    __quex_debug_state(481);
switch( input ) {
case 0x0: goto _21;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _6;
default: goto _18;
}


    __quex_assert_no_passage();
_20:
    /* (481 from RELOAD_FORWARD)  */
    goto _40;


    __quex_assert_no_passage();
_6:
    /* (482 from 481)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(482);
goto _19;


    __quex_assert_no_passage();
_7:
    /* (475 from 474)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(475);
goto _13;


    __quex_assert_no_passage();
_8:
    /* (476 from 474)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(476);
goto _14;


    __quex_assert_no_passage();
_24:
    /* (477 from RELOAD_FORWARD)  */
_45:
    input = *(me->buffer._read_p);


    __quex_debug_state(477);
switch( input ) {
case 0x0: goto _25;
case 0x28: goto _4;
case 0x5C: goto _10;
case 0x78: goto _11;
default: goto _13;
}


    __quex_assert_no_passage();
_9:
    /* (477 from 474)  */
    ++(me->buffer._read_p);

    goto _45;


    __quex_assert_no_passage();
_10:
    /* (478 from 477)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(478);
goto _15;


    __quex_assert_no_passage();
_11:
    /* (479 from 477)  */
    ++(me->buffer._read_p);

_48:
    input = *(me->buffer._read_p);


    __quex_debug_state(479);
switch( input ) {
case 0x0: goto _27;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _5;
default: goto _16;
}


    __quex_assert_no_passage();
_26:
    /* (479 from RELOAD_FORWARD)  */
    goto _48;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_22:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 771 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _1;
_33:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
    throw std::runtime_error("Invalid parameter string");


#   line 785 "Lexer.cpp"

}
goto _2;
_35:
    __quex_debug("* TERMINAL \"\\\\(\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 125 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 800 "Lexer.cpp"

}
goto _0;
_36:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 128 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 815 "Lexer.cpp"

}
goto _0;
_34:
    __quex_debug("* TERMINAL \"\\\\x\"[0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 131 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 830 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \")\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 136 "Lexer.qx"
    self << RULE;
    RETURN;


#   line 845 "Lexer.cpp"

}
goto _0;
_32:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 138 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 859 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_51:
    switch( target_state_index ) {
        case 3: { goto _3; }
        case 13: { goto _13; }
        case 16: { goto _16; }
        case 18: { goto _18; }
        case 20: { goto _20; }
        case 22: { goto _22; }
        case 24: { goto _24; }
        case 26: { goto _26; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_25:
    /* (RELOAD_FORWARD from 477)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(13);

_49:
    __quex_debug3("RELOAD_FORWARD: success->%i; failure->%i", (int)target_state_index, (int)target_state_else_index);
    __quex_assert(*(me->buffer._read_p) == QUEX_SETTING_BUFFER_LIMIT_CODE);
    /* Detect whether the buffer limit code appeared at non-border.          */
    if( me->buffer._read_p != me->buffer.input.end_p ) {
        __quex_assert(false); /* Later: on codec error! */
    }
    __quex_debug_reload_before();                 /* Report source position. */
    if( QUEX_NAME(Buffer_load_forward)(&me->buffer, (QUEX_TYPE_CHARACTER**)position, PositionRegisterN) ) {
        __quex_debug_reload_after();
        QUEX_GOTO_STATE(target_state_index);      /* may use 'computed goto' */
    }
    __quex_debug("reload impossible\n");
    QUEX_GOTO_STATE(target_state_else_index);     /* may use 'computed goto' */


    __quex_assert_no_passage();
_21:
    /* (RELOAD_FORWARD from 481)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(18);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 479)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(16);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 474)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(22);

    goto _49;

_1:
/* RETURN -- after executing 'on_after_match' code. */
    __QUEX_PURE_RETURN;


_0:
/* CONTINUE -- after executing 'on_after_match' code. */

_2:
/* CONTINUE -- without executing 'on_after_match' (e.g. on FAILURE). */


#   ifndef __QUEX_OPTION_PLAIN_ANALYZER_OBJECT
#   ifdef  QUEX_OPTION_TOKEN_POLICY_QUEUE
    if( QUEX_NAME(TokenQueue_is_full)(&self._token_queue) ) {
        return;
    }
#   else
    if( self_token_get_id() != __QUEX_SETTING_TOKEN_ID_UNINITIALIZED) {
        return __self_result_token_id;
    }
#   endif
#   endif


    /*  If a mode change happened, then the function must first return and
     *  indicate that another mode function is to be called. At this point, 
     *  we to force a 'return' on a mode change. 
     *
     *  Pseudo Code: if( previous_mode != current_mode ) {
     *                   return 0;
     *               }
     *
     *  When the analyzer returns, the caller function has to watch if a mode 
     *  change occurred. If not it can call this function again.             */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)        || defined(QUEX_OPTION_ASSERTS)
    if( me->DEBUG_analyzer_function_at_entry != me->current_analyzer_function ) 
#   endif
    { 
#       if defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)
        self_token_set_id(__QUEX_SETTING_TOKEN_ID_UNINITIALIZED);
        __QUEX_PURE_RETURN;
#       elif defined(QUEX_OPTION_ASSERTS)
        QUEX_ERROR_EXIT("Mode change without immediate return from the lexical analyzer.");
#       endif
    }


goto _50;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _1; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _2; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _51; /* in QUEX_GOTO_STATE       */
#   endif

    /* Prevent compiler warning 'unused variable'.                           */
    (void)QUEX_LEXEME_NULL;
    (void)QUEX_NAME_TOKEN(DumpedTokenIdObject);
    /* target_state_index and target_state_else_index appear when 
     * QUEX_GOTO_STATE is used without computed goto-s.                      */
    (void)target_state_index;
    (void)target_state_else_index;

#   undef Lexeme
#   undef LexemeBegin
#   undef LexemeEnd
#   undef LexemeNull
#   undef LexemeL
#   undef PQ_STRING_READER
#   undef RULE
#   undef DQ_STRING_READER
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
}
#ifdef      __QUEX_COUNT_VOID
#   undef   __QUEX_COUNT_VOID
#endif
#ifdef      __QUEX_OPTION_COUNTER
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) \
            do {                              \
                QUEX_NAME(PQ_STRING_READER_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif

#include <quex/code_base/analyzer/struct/basic>
#include <quex/code_base/buffer/Buffer>
#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
#   include <quex/code_base/token/TokenQueue>
#endif

#ifdef    CONTINUE
#   undef CONTINUE
#endif
#define   CONTINUE do { goto _0; } while(0)

#ifdef    RETURN
#   undef RETURN
#endif
#define   RETURN   do { goto _2; } while(0)

__QUEX_TYPE_ANALYZER_RETURN_VALUE  
QUEX_NAME(RULE_analyzer_function)(QUEX_TYPE_ANALYZER* me) 
{
    /* NOTE: Different modes correspond to different analyzer functions. The 
     *       analyzer functions are all located inside the main class as static
     *       functions. That means, they are something like 'globals'. They 
     *       receive a pointer to the lexical analyzer, since static members do
     *       not have access to the 'this' pointer.                          */
#   if defined(QUEX_OPTION_TOKEN_POLICY_SINGLE)
    register QUEX_TYPE_TOKEN_ID __self_result_token_id 
           = (QUEX_TYPE_TOKEN_ID)__QUEX_SETTING_TOKEN_ID_UNINITIALIZED;
#   endif
#   ifdef     self
#       undef self
#   endif
#   define self (*((QUEX_TYPE_ANALYZER*)me))
/*  'QUEX_GOTO_STATE' requires 'QUEX_LABEL_STATE_ROUTER' */
#   define QUEX_LABEL_STATE_ROUTER _613
#   define PQ_STRING_READER    (QUEX_NAME(PQ_STRING_READER))
#   define RULE                (QUEX_NAME(RULE))
#   define DQ_STRING_READER    (QUEX_NAME(DQ_STRING_READER))

    /* Lexeme setup: 
     *
     * There is a temporary zero stored at the end of each lexeme, if the action 
     * references to the 'Lexeme'. 'LexemeNull' provides a reference to an empty
     * zero terminated string.                                                    */
#if defined(QUEX_OPTION_ASSERTS)
#   define Lexeme       QUEX_NAME(access_Lexeme)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeBegin  QUEX_NAME(access_LexemeBegin)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeL      QUEX_NAME(access_LexemeL)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeEnd    QUEX_NAME(access_LexemeEnd)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#else
#   define Lexeme       (me->buffer._lexeme_start_p)
#   define LexemeBegin  Lexeme
#   define LexemeL      ((size_t)(me->buffer._read_p - me->buffer._lexeme_start_p))
#   define LexemeEnd    me->buffer._read_p
#endif

#define LexemeNull      (&QUEX_LEXEME_NULL)
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_CHARACTER**          position                       = 0x0;

    /* Post context positions do not have to be reset or initialized. If a state
     * is reached which is associated with 'end of post context' it is clear what
     * post context is meant. This results from the ways the state machine is 
     * constructed. Post context position's live cycle:
     *
     * (1)   unitialized (don't care)
     * (1.b) on buffer reload it may, or may not be adapted (don't care)
     * (2)   when a post context begin state is passed, then it is **SET** (now: take care)
     * (2.b) on buffer reload it **is adapted**.
     * (3)   when a terminal state of the post context is reached (which can only be reached
     *       for that particular post context), then the post context position is used
     *       to reset the input position.                                              */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE) \
       || defined(QUEX_OPTION_ASSERTS)
    me->DEBUG_analyzer_function_at_entry = me->current_analyzer_function;
#   endif
_612:
    me->buffer._lexeme_start_p = me->buffer._read_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);
_3:
    /* (909 from BEFORE_ENTRY) (909 from RELOAD_FORWARD)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(909);
switch( input ) {
case 0x0: goto _178;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
case 0x21: goto _22;
case 0x22: goto _10;
case 0x28: goto _28;
case 0x2A: goto _25;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _11;
case 0x3A: goto _9;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _11;
case 0x61: goto _18;
case 0x62: goto _17;
case 0x63: goto _11;
case 0x64: goto _30;
case 0x65: goto _20;
case 0x66: goto _11;
case 0x68: goto _14;
case 0x69: goto _19;
case 0x6D: goto _27;
case 0x6E: goto _15;
case 0x6F: goto _23;
case 0x72: goto _21;
case 0x73: goto _16;
case 0x76: goto _24;
case 0x77: goto _12;
case 0x7B: goto _26;
case 0x7D: goto _29;
default: goto _123;
}


    __quex_assert_no_passage();
_123:
    /* (DROP_OUT from 969) (DROP_OUT from 917) (DROP_OUT from 1014) (DROP_OUT from 1011) (DROP_OUT from 1008) (DROP_OUT from 924) (DROP_OUT from 1021) (DROP_OUT from 921) (DROP_OUT from 1018) (DROP_OUT from 999) (DROP_OUT from 996) (DROP_OUT from 993) (DROP_OUT from 991) (DROP_OUT from 909) (DROP_OUT from 1006) (DROP_OUT from 985) (DROP_OUT from 1025) (DROP_OUT from 1000) (DROP_OUT from 966) (DROP_OUT from 948) (DROP_OUT from 960) (DROP_OUT from 955) (DROP_OUT from 970) (DROP_OUT from 952) (DROP_OUT from 918) (DROP_OUT from 915) (DROP_OUT from 912) (DROP_OUT from 1009) (DROP_OUT from 925) (DROP_OUT from 1022) (DROP_OUT from 922) (DROP_OUT from 1019) (DROP_OUT from 1016) (DROP_OUT from 997) (DROP_OUT from 989) (DROP_OUT from 1004) (DROP_OUT from 986) (DROP_OUT from 1026) (DROP_OUT from 1001) (DROP_OUT from 967) (DROP_OUT from 949) (DROP_OUT from 964) (DROP_OUT from 961) (DROP_OUT from 959) (DROP_OUT from 956) (DROP_OUT from 971) (DROP_OUT from 953) (DROP_OUT from 919) (DROP_OUT from 916) (DROP_OUT from 1013) (DROP_OUT from 931) (DROP_OUT from 913) (DROP_OUT from 1010) (DROP_OUT from 928) (DROP_OUT from 1023) (DROP_OUT from 1020) (DROP_OUT from 920) (DROP_OUT from 1017) (DROP_OUT from 983) (DROP_OUT from 995) (DROP_OUT from 992) (DROP_OUT from 1005) (DROP_OUT from 987) (DROP_OUT from 1027) (DROP_OUT from 1002) (DROP_OUT from 984) (DROP_OUT from 950) (DROP_OUT from 1024) (DROP_OUT from 965) (DROP_OUT from 962) (DROP_OUT from 957) (DROP_OUT from 972) (DROP_OUT from 954)  */

        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _365;
_363:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_150:
    /* (DROP_OUT from 973)  */
    goto _366;


    __quex_assert_no_passage();
_137:
    /* (DROP_OUT from 939)  */
    goto _367;


    __quex_assert_no_passage();
_130:
    /* (DROP_OUT from 929)  */
    goto _368;


    __quex_assert_no_passage();
_166:
    /* (DROP_OUT from 1012)  */
    goto _369;


    __quex_assert_no_passage();
_159:
    /* (DROP_OUT from 982)  */
    goto _371;


    __quex_assert_no_passage();
_126:
    /* (DROP_OUT from 914)  */
    goto _372;


    __quex_assert_no_passage();
_147:
    /* (DROP_OUT from 958)  */
    goto _373;


    __quex_assert_no_passage();
_167:
    /* (DROP_OUT from 1015)  */
    goto _374;


    __quex_assert_no_passage();
_134:
    /* (DROP_OUT from 936)  */
    goto _375;


    __quex_assert_no_passage();
_160:
    /* (DROP_OUT from 988)  */
    goto _376;


    __quex_assert_no_passage();
_127:
    /* (DROP_OUT from 923)  */
    goto _377;


    __quex_assert_no_passage();
_163:
    /* (DROP_OUT from 998)  */
    goto _378;


    __quex_assert_no_passage();
_145:
    /* (DROP_OUT from 947)  */
    goto _379;


    __quex_assert_no_passage();
_148:
    /* (DROP_OUT from 963)  */
    goto _380;


    __quex_assert_no_passage();
_168:
    /* (DROP_OUT from 1028)  */
    goto _381;


    __quex_assert_no_passage();
_161:
    /* (DROP_OUT from 990)  */
    goto _382;


    __quex_assert_no_passage();
_128:
    /* (DROP_OUT from 926)  */
    goto _383;


    __quex_assert_no_passage();
_164:
    /* (DROP_OUT from 1003)  */
    goto _384;


    __quex_assert_no_passage();
_131:
    /* (DROP_OUT from 930)  */
    goto _385;


    __quex_assert_no_passage();
_146:
    /* (DROP_OUT from 951)  */
    goto _386;


    __quex_assert_no_passage();
_124:
    /* (DROP_OUT from 910)  */
    goto _387;


    __quex_assert_no_passage();
_149:
    /* (DROP_OUT from 968)  */
    goto _388;


    __quex_assert_no_passage();
_162:
    /* (DROP_OUT from 994)  */
    goto _389;


    __quex_assert_no_passage();
_129:
    /* (DROP_OUT from 927)  */
    goto _390;


    __quex_assert_no_passage();
_165:
    /* (DROP_OUT from 1007)  */
    goto _391;


    __quex_assert_no_passage();
_132:
    /* (DROP_OUT from 932) (DROP_OUT from 933)  */
    goto _370;


    __quex_assert_no_passage();
_158:
    /* (DROP_OUT from 981)  */
    goto _392;


    __quex_assert_no_passage();
_125:
    /* (DROP_OUT from 911)  */
    goto _393;


    __quex_assert_no_passage();
_143:
    /* (DROP_OUT from 945)  */
        me->buffer._read_p -= 6;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_139:
    /* (DROP_OUT from 941)  */
        me->buffer._read_p -= 2;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_133:
    /* (DROP_OUT from 935) (DROP_OUT from 934)  */
        me->buffer._read_p -= 1;
goto _370;
    goto _363;


    __quex_assert_no_passage();
_151:
    /* (DROP_OUT from 974)  */
        me->buffer._read_p -= 1;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_155:
    /* (DROP_OUT from 978)  */
        me->buffer._read_p -= 5;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_144:
    /* (DROP_OUT from 946)  */
        me->buffer._read_p -= 7;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_138:
    /* (DROP_OUT from 940)  */
        me->buffer._read_p -= 1;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_140:
    /* (DROP_OUT from 942)  */
        me->buffer._read_p -= 3;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_152:
    /* (DROP_OUT from 975)  */
        me->buffer._read_p -= 2;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_156:
    /* (DROP_OUT from 979)  */
        me->buffer._read_p -= 6;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_141:
    /* (DROP_OUT from 943)  */
        me->buffer._read_p -= 4;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_135:
    /* (DROP_OUT from 937)  */
        me->buffer._read_p -= 2;
goto _370;
    goto _363;


    __quex_assert_no_passage();
_153:
    /* (DROP_OUT from 976)  */
        me->buffer._read_p -= 3;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_157:
    /* (DROP_OUT from 980)  */
        me->buffer._read_p -= 7;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_142:
    /* (DROP_OUT from 944)  */
        me->buffer._read_p -= 5;
goto _367;
    goto _363;


    __quex_assert_no_passage();
_136:
    /* (DROP_OUT from 938)  */
        me->buffer._read_p -= 3;
goto _370;
    goto _363;


    __quex_assert_no_passage();
_154:
    /* (DROP_OUT from 977)  */
        me->buffer._read_p -= 4;
goto _366;
    goto _363;


    __quex_assert_no_passage();
_169:
    /* (1024 from RELOAD_FORWARD)  */
_395:
    input = *(me->buffer._read_p);


    __quex_debug_state(1024);
if     ( input == 0x66 )  goto _5;
else if( input == 0x0 )   goto _170;
else                      goto _123;


    __quex_assert_no_passage();
_4:
    /* (1024 from 1023)  */
    ++(me->buffer._read_p);

    goto _395;


    __quex_assert_no_passage();
_5:
    /* (1025 from 1024)  */
    ++(me->buffer._read_p);

_397:
    input = *(me->buffer._read_p);


    __quex_debug_state(1025);
if     ( input == 0x61 )  goto _6;
else if( input == 0x0 )   goto _172;
else                      goto _123;


    __quex_assert_no_passage();
_171:
    /* (1025 from RELOAD_FORWARD)  */
    goto _397;


    __quex_assert_no_passage();
_6:
    /* (1026 from 1025)  */
    ++(me->buffer._read_p);

_399:
    input = *(me->buffer._read_p);


    __quex_debug_state(1026);
if     ( input == 0x63 )  goto _7;
else if( input == 0x0 )   goto _174;
else                      goto _123;


    __quex_assert_no_passage();
_173:
    /* (1026 from RELOAD_FORWARD)  */
    goto _399;


    __quex_assert_no_passage();
_7:
    /* (1027 from 1026)  */
    ++(me->buffer._read_p);

_401:
    input = *(me->buffer._read_p);


    __quex_debug_state(1027);
if     ( input == 0x65 )  goto _8;
else if( input == 0x0 )   goto _176;
else                      goto _123;


    __quex_assert_no_passage();
_175:
    /* (1027 from RELOAD_FORWARD)  */
    goto _401;


    __quex_assert_no_passage();
_8:
    /* (1028 from 1027)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1028);
goto _168;


    __quex_assert_no_passage();
_9:
    /* (910 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(910);
goto _124;


    __quex_assert_no_passage();
_10:
    /* (911 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(911);
goto _125;


    __quex_assert_no_passage();
_179:
    /* (912 from RELOAD_FORWARD)  */
_406:
    input = *(me->buffer._read_p);


    __quex_debug_state(912);
switch( input ) {
case 0x0: goto _180;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _31;
default: goto _123;
}


    __quex_assert_no_passage();
_11:
    /* (912 from 909)  */
    ++(me->buffer._read_p);

    goto _406;


    __quex_assert_no_passage();
_12:
    /* (913 from 909)  */
    ++(me->buffer._read_p);

_408:
    input = *(me->buffer._read_p);


    __quex_debug_state(913);
if     ( input == 0x69 )  goto _115;
else if( input == 0x0 )   goto _182;
else                      goto _123;


    __quex_assert_no_passage();
_181:
    /* (913 from RELOAD_FORWARD)  */
    goto _408;


    __quex_assert_no_passage();
_13:
    /* (914 from 909) (914 from 914)  */

    ++(me->buffer._read_p);

_411:
    input = *(me->buffer._read_p);


    __quex_debug_state(914);
switch( input ) {
case 0x0: goto _184;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
default: goto _126;
}


    __quex_assert_no_passage();
_183:
    /* (914 from RELOAD_FORWARD)  */
    goto _411;


    __quex_assert_no_passage();
_14:
    /* (915 from 909)  */
    ++(me->buffer._read_p);

_413:
    input = *(me->buffer._read_p);


    __quex_debug_state(915);
if     ( input == 0x61 )  goto _112;
else if( input == 0x0 )   goto _186;
else                      goto _123;


    __quex_assert_no_passage();
_185:
    /* (915 from RELOAD_FORWARD)  */
    goto _413;


    __quex_assert_no_passage();
_187:
    /* (916 from RELOAD_FORWARD)  */
_415:
    input = *(me->buffer._read_p);


    __quex_debug_state(916);
switch( input ) {
case 0x0: goto _188;
case 0x61: goto _104;
case 0x6F: goto _103;
default: goto _123;
}


    __quex_assert_no_passage();
_15:
    /* (916 from 909)  */
    ++(me->buffer._read_p);

    goto _415;


    __quex_assert_no_passage();
_189:
    /* (917 from RELOAD_FORWARD)  */
_417:
    input = *(me->buffer._read_p);


    __quex_debug_state(917);
if     ( input == 0x65 )  goto _98;
else if( input == 0x0 )   goto _190;
else                      goto _123;


    __quex_assert_no_passage();
_16:
    /* (917 from 909)  */
    ++(me->buffer._read_p);

    goto _417;


    __quex_assert_no_passage();
_191:
    /* (918 from RELOAD_FORWARD)  */
_419:
    input = *(me->buffer._read_p);


    __quex_debug_state(918);
switch( input ) {
case 0x0: goto _192;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _31;
case 0x6C: goto _94;
default: goto _123;
}


    __quex_assert_no_passage();
_17:
    /* (918 from 909)  */
    ++(me->buffer._read_p);

    goto _419;


    __quex_assert_no_passage();
_18:
    /* (919 from 909)  */
    ++(me->buffer._read_p);

_421:
    input = *(me->buffer._read_p);


    __quex_debug_state(919);
switch( input ) {
case 0x0: goto _194;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _31;
case 0x6C: goto _83;
case 0x6E: goto _82;
default: goto _123;
}


    __quex_assert_no_passage();
_193:
    /* (919 from RELOAD_FORWARD)  */
    goto _421;


    __quex_assert_no_passage();
_195:
    /* (920 from RELOAD_FORWARD)  */
_423:
    input = *(me->buffer._read_p);


    __quex_debug_state(920);
if     ( input == 0x66 )  goto _81;
else if( input == 0x0 )   goto _196;
else                      goto _123;


    __quex_assert_no_passage();
_19:
    /* (920 from 909)  */
    ++(me->buffer._read_p);

    goto _423;


    __quex_assert_no_passage();
_20:
    /* (921 from 909)  */
    ++(me->buffer._read_p);

_425:
    input = *(me->buffer._read_p);


    __quex_debug_state(921);
switch( input ) {
case 0x0: goto _198;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _31;
case 0x71: goto _68;
default: goto _123;
}


    __quex_assert_no_passage();
_197:
    /* (921 from RELOAD_FORWARD)  */
    goto _425;


    __quex_assert_no_passage();
_21:
    /* (922 from 909)  */
    ++(me->buffer._read_p);

_427:
    input = *(me->buffer._read_p);


    __quex_debug_state(922);
if     ( input == 0x65 )  goto _63;
else if( input == 0x0 )   goto _200;
else                      goto _123;


    __quex_assert_no_passage();
_199:
    /* (922 from RELOAD_FORWARD)  */
    goto _427;


    __quex_assert_no_passage();
_22:
    /* (923 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(923);
goto _127;


    __quex_assert_no_passage();
_201:
    /* (924 from RELOAD_FORWARD)  */
_430:
    input = *(me->buffer._read_p);


    __quex_debug_state(924);
if     ( input == 0x6E )  goto _58;
else if( input == 0x0 )   goto _202;
else                      goto _123;


    __quex_assert_no_passage();
_23:
    /* (924 from 909)  */
    ++(me->buffer._read_p);

    goto _430;


    __quex_assert_no_passage();
_203:
    /* (925 from RELOAD_FORWARD)  */
_432:
    input = *(me->buffer._read_p);


    __quex_debug_state(925);
if     ( input == 0x69 )  goto _51;
else if( input == 0x0 )   goto _204;
else                      goto _123;


    __quex_assert_no_passage();
_24:
    /* (925 from 909)  */
    ++(me->buffer._read_p);

    goto _432;


    __quex_assert_no_passage();
_25:
    /* (926 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(926);
goto _128;


    __quex_assert_no_passage();
_26:
    /* (927 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(927);
goto _129;


    __quex_assert_no_passage();
_205:
    /* (928 from RELOAD_FORWARD)  */
_436:
    input = *(me->buffer._read_p);


    __quex_debug_state(928);
if     ( input == 0x61 )  goto _47;
else if( input == 0x0 )   goto _206;
else                      goto _123;


    __quex_assert_no_passage();
_27:
    /* (928 from 909)  */
    ++(me->buffer._read_p);

    goto _436;


    __quex_assert_no_passage();
_28:
    /* (929 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(929);
goto _130;


    __quex_assert_no_passage();
_29:
    /* (930 from 909)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(930);
goto _131;


    __quex_assert_no_passage();
_30:
    /* (931 from 909)  */
    ++(me->buffer._read_p);

_440:
    input = *(me->buffer._read_p);


    __quex_debug_state(931);
switch( input ) {
case 0x0: goto _208;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: goto _31;
case 0x65: goto _32;
case 0x66: goto _31;
default: goto _123;
}


    __quex_assert_no_passage();
_207:
    /* (931 from RELOAD_FORWARD)  */
    goto _440;


    __quex_assert_no_passage();
_31:
    /* (932 from 918) (932 from 919) (932 from 921) (932 from 912) (932 from 931)  */

    ++(me->buffer._read_p);

_443:
    input = *(me->buffer._read_p);


    __quex_debug_state(932);
switch( input ) {
case 0x0: goto _210;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
default: goto _132;
}


    __quex_assert_no_passage();
_209:
    /* (932 from RELOAD_FORWARD)  */
    goto _443;


    __quex_assert_no_passage();
_211:
    /* (933 from RELOAD_FORWARD)  */
_445:
    input = *(me->buffer._read_p);


    __quex_debug_state(933);
switch( input ) {
case 0x0: goto _212;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
case 0x76: goto _33;
default: goto _132;
}


    __quex_assert_no_passage();
_32:
    /* (933 from 931)  */
    ++(me->buffer._read_p);

    goto _445;


    __quex_assert_no_passage();
_213:
    /* (934 from RELOAD_FORWARD)  */
_447:
    input = *(me->buffer._read_p);


    __quex_debug_state(934);
if     ( input == 0x69 )  goto _36;
else if( input == 0x0 )   goto _214;
else                      goto _133;


    __quex_assert_no_passage();
_33:
    /* (934 from 933)  */
    ++(me->buffer._read_p);

    goto _447;


    __quex_assert_no_passage();
_34:
    /* (935 from 932) (935 from 933)  */

    ++(me->buffer._read_p);

_450:
    input = *(me->buffer._read_p);


    __quex_debug_state(935);
switch( input ) {
case 0x0: goto _216;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _35;
default: goto _133;
}


    __quex_assert_no_passage();
_215:
    /* (935 from RELOAD_FORWARD)  */
    goto _450;


    __quex_assert_no_passage();
_35:
    /* (936 from 935)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(936);
goto _134;


    __quex_assert_no_passage();
_36:
    /* (937 from 934)  */
    ++(me->buffer._read_p);

_453:
    input = *(me->buffer._read_p);


    __quex_debug_state(937);
if     ( input == 0x63 )  goto _37;
else if( input == 0x0 )   goto _218;
else                      goto _135;


    __quex_assert_no_passage();
_217:
    /* (937 from RELOAD_FORWARD)  */
    goto _453;


    __quex_assert_no_passage();
_37:
    /* (938 from 937)  */
    ++(me->buffer._read_p);

_455:
    input = *(me->buffer._read_p);


    __quex_debug_state(938);
if     ( input == 0x65 )  goto _38;
else if( input == 0x0 )   goto _220;
else                      goto _136;


    __quex_assert_no_passage();
_219:
    /* (938 from RELOAD_FORWARD)  */
    goto _455;


    __quex_assert_no_passage();
_38:
    /* (939 from 938)  */
    ++(me->buffer._read_p);

_457:
    input = *(me->buffer._read_p);


    __quex_debug_state(939);
if     ( input == 0x2D )  goto _39;
else if( input == 0x0 )   goto _222;
else                      goto _137;


    __quex_assert_no_passage();
_221:
    /* (939 from RELOAD_FORWARD)  */
    goto _457;


    __quex_assert_no_passage();
_223:
    /* (940 from RELOAD_FORWARD)  */
_459:
    input = *(me->buffer._read_p);


    __quex_debug_state(940);
if     ( input == 0x6D )  goto _40;
else if( input == 0x0 )   goto _224;
else                      goto _138;


    __quex_assert_no_passage();
_39:
    /* (940 from 939)  */
    ++(me->buffer._read_p);

    goto _459;


    __quex_assert_no_passage();
_225:
    /* (941 from RELOAD_FORWARD)  */
_461:
    input = *(me->buffer._read_p);


    __quex_debug_state(941);
if     ( input == 0x61 )  goto _41;
else if( input == 0x0 )   goto _226;
else                      goto _139;


    __quex_assert_no_passage();
_40:
    /* (941 from 940)  */
    ++(me->buffer._read_p);

    goto _461;


    __quex_assert_no_passage();
_227:
    /* (942 from RELOAD_FORWARD)  */
_463:
    input = *(me->buffer._read_p);


    __quex_debug_state(942);
if     ( input == 0x74 )  goto _42;
else if( input == 0x0 )   goto _228;
else                      goto _140;


    __quex_assert_no_passage();
_41:
    /* (942 from 941)  */
    ++(me->buffer._read_p);

    goto _463;


    __quex_assert_no_passage();
_42:
    /* (943 from 942)  */
    ++(me->buffer._read_p);

_465:
    input = *(me->buffer._read_p);


    __quex_debug_state(943);
if     ( input == 0x63 )  goto _43;
else if( input == 0x0 )   goto _230;
else                      goto _141;


    __quex_assert_no_passage();
_229:
    /* (943 from RELOAD_FORWARD)  */
    goto _465;


    __quex_assert_no_passage();
_231:
    /* (944 from RELOAD_FORWARD)  */
_467:
    input = *(me->buffer._read_p);


    __quex_debug_state(944);
if     ( input == 0x68 )  goto _44;
else if( input == 0x0 )   goto _232;
else                      goto _142;


    __quex_assert_no_passage();
_43:
    /* (944 from 943)  */
    ++(me->buffer._read_p);

    goto _467;


    __quex_assert_no_passage();
_44:
    /* (945 from 944)  */
    ++(me->buffer._read_p);

_469:
    input = *(me->buffer._read_p);


    __quex_debug_state(945);
if     ( input == 0x65 )  goto _45;
else if( input == 0x0 )   goto _234;
else                      goto _143;


    __quex_assert_no_passage();
_233:
    /* (945 from RELOAD_FORWARD)  */
    goto _469;


    __quex_assert_no_passage();
_45:
    /* (946 from 945)  */
    ++(me->buffer._read_p);

_471:
    input = *(me->buffer._read_p);


    __quex_debug_state(946);
if     ( input == 0x73 )  goto _46;
else if( input == 0x0 )   goto _236;
else                      goto _144;


    __quex_assert_no_passage();
_235:
    /* (946 from RELOAD_FORWARD)  */
    goto _471;


    __quex_assert_no_passage();
_46:
    /* (947 from 946)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(947);
goto _145;


    __quex_assert_no_passage();
_237:
    /* (948 from RELOAD_FORWARD)  */
_474:
    input = *(me->buffer._read_p);


    __quex_debug_state(948);
if     ( input == 0x74 )  goto _48;
else if( input == 0x0 )   goto _238;
else                      goto _123;


    __quex_assert_no_passage();
_47:
    /* (948 from 928)  */
    ++(me->buffer._read_p);

    goto _474;


    __quex_assert_no_passage();
_239:
    /* (949 from RELOAD_FORWARD)  */
_476:
    input = *(me->buffer._read_p);


    __quex_debug_state(949);
if     ( input == 0x63 )  goto _49;
else if( input == 0x0 )   goto _240;
else                      goto _123;


    __quex_assert_no_passage();
_48:
    /* (949 from 948)  */
    ++(me->buffer._read_p);

    goto _476;


    __quex_assert_no_passage();
_241:
    /* (950 from RELOAD_FORWARD)  */
_478:
    input = *(me->buffer._read_p);


    __quex_debug_state(950);
if     ( input == 0x68 )  goto _50;
else if( input == 0x0 )   goto _242;
else                      goto _123;


    __quex_assert_no_passage();
_49:
    /* (950 from 949)  */
    ++(me->buffer._read_p);

    goto _478;


    __quex_assert_no_passage();
_50:
    /* (951 from 950)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(951);
goto _146;


    __quex_assert_no_passage();
_243:
    /* (952 from RELOAD_FORWARD)  */
_481:
    input = *(me->buffer._read_p);


    __quex_debug_state(952);
if     ( input == 0x61 )  goto _52;
else if( input == 0x0 )   goto _244;
else                      goto _123;


    __quex_assert_no_passage();
_51:
    /* (952 from 925)  */
    ++(me->buffer._read_p);

    goto _481;


    __quex_assert_no_passage();
_52:
    /* (953 from 952)  */
    ++(me->buffer._read_p);

_483:
    input = *(me->buffer._read_p);


    __quex_debug_state(953);
if     ( input == 0x2D )  goto _53;
else if( input == 0x0 )   goto _246;
else                      goto _123;


    __quex_assert_no_passage();
_245:
    /* (953 from RELOAD_FORWARD)  */
    goto _483;


    __quex_assert_no_passage();
_53:
    /* (954 from 953)  */
    ++(me->buffer._read_p);

_485:
    input = *(me->buffer._read_p);


    __quex_debug_state(954);
if     ( input == 0x70 )  goto _54;
else if( input == 0x0 )   goto _248;
else                      goto _123;


    __quex_assert_no_passage();
_247:
    /* (954 from RELOAD_FORWARD)  */
    goto _485;


    __quex_assert_no_passage();
_54:
    /* (955 from 954)  */
    ++(me->buffer._read_p);

_487:
    input = *(me->buffer._read_p);


    __quex_debug_state(955);
if     ( input == 0x6F )  goto _55;
else if( input == 0x0 )   goto _250;
else                      goto _123;


    __quex_assert_no_passage();
_249:
    /* (955 from RELOAD_FORWARD)  */
    goto _487;


    __quex_assert_no_passage();
_251:
    /* (956 from RELOAD_FORWARD)  */
_489:
    input = *(me->buffer._read_p);


    __quex_debug_state(956);
if     ( input == 0x72 )  goto _56;
else if( input == 0x0 )   goto _252;
else                      goto _123;


    __quex_assert_no_passage();
_55:
    /* (956 from 955)  */
    ++(me->buffer._read_p);

    goto _489;


    __quex_assert_no_passage();
_253:
    /* (957 from RELOAD_FORWARD)  */
_491:
    input = *(me->buffer._read_p);


    __quex_debug_state(957);
if     ( input == 0x74 )  goto _57;
else if( input == 0x0 )   goto _254;
else                      goto _123;


    __quex_assert_no_passage();
_56:
    /* (957 from 956)  */
    ++(me->buffer._read_p);

    goto _491;


    __quex_assert_no_passage();
_57:
    /* (958 from 957)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(958);
goto _147;


    __quex_assert_no_passage();
_58:
    /* (959 from 924)  */
    ++(me->buffer._read_p);

_494:
    input = *(me->buffer._read_p);


    __quex_debug_state(959);
if     ( input == 0x65 )  goto _59;
else if( input == 0x0 )   goto _256;
else                      goto _123;


    __quex_assert_no_passage();
_255:
    /* (959 from RELOAD_FORWARD)  */
    goto _494;


    __quex_assert_no_passage();
_257:
    /* (960 from RELOAD_FORWARD)  */
_496:
    input = *(me->buffer._read_p);


    __quex_debug_state(960);
if     ( input == 0x2D )  goto _60;
else if( input == 0x0 )   goto _258;
else                      goto _123;


    __quex_assert_no_passage();
_59:
    /* (960 from 959)  */
    ++(me->buffer._read_p);

    goto _496;


    __quex_assert_no_passage();
_60:
    /* (961 from 960)  */
    ++(me->buffer._read_p);

_498:
    input = *(me->buffer._read_p);


    __quex_debug_state(961);
if     ( input == 0x6F )  goto _61;
else if( input == 0x0 )   goto _260;
else                      goto _123;


    __quex_assert_no_passage();
_259:
    /* (961 from RELOAD_FORWARD)  */
    goto _498;


    __quex_assert_no_passage();
_61:
    /* (962 from 961)  */
    ++(me->buffer._read_p);

_500:
    input = *(me->buffer._read_p);


    __quex_debug_state(962);
if     ( input == 0x66 )  goto _62;
else if( input == 0x0 )   goto _262;
else                      goto _123;


    __quex_assert_no_passage();
_261:
    /* (962 from RELOAD_FORWARD)  */
    goto _500;


    __quex_assert_no_passage();
_62:
    /* (963 from 962)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(963);
goto _148;


    __quex_assert_no_passage();
_263:
    /* (964 from RELOAD_FORWARD)  */
_503:
    input = *(me->buffer._read_p);


    __quex_debug_state(964);
if     ( input == 0x6A )  goto _64;
else if( input == 0x0 )   goto _264;
else                      goto _123;


    __quex_assert_no_passage();
_63:
    /* (964 from 922)  */
    ++(me->buffer._read_p);

    goto _503;


    __quex_assert_no_passage();
_265:
    /* (965 from RELOAD_FORWARD)  */
_505:
    input = *(me->buffer._read_p);


    __quex_debug_state(965);
if     ( input == 0x65 )  goto _65;
else if( input == 0x0 )   goto _266;
else                      goto _123;


    __quex_assert_no_passage();
_64:
    /* (965 from 964)  */
    ++(me->buffer._read_p);

    goto _505;


    __quex_assert_no_passage();
_267:
    /* (966 from RELOAD_FORWARD)  */
_507:
    input = *(me->buffer._read_p);


    __quex_debug_state(966);
if     ( input == 0x63 )  goto _66;
else if( input == 0x0 )   goto _268;
else                      goto _123;


    __quex_assert_no_passage();
_65:
    /* (966 from 965)  */
    ++(me->buffer._read_p);

    goto _507;


    __quex_assert_no_passage();
_66:
    /* (967 from 966)  */
    ++(me->buffer._read_p);

_509:
    input = *(me->buffer._read_p);


    __quex_debug_state(967);
if     ( input == 0x74 )  goto _67;
else if( input == 0x0 )   goto _270;
else                      goto _123;


    __quex_assert_no_passage();
_269:
    /* (967 from RELOAD_FORWARD)  */
    goto _509;


    __quex_assert_no_passage();
_67:
    /* (968 from 967)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(968);
goto _149;


    __quex_assert_no_passage();
_68:
    /* (969 from 921)  */
    ++(me->buffer._read_p);

_512:
    input = *(me->buffer._read_p);


    __quex_debug_state(969);
if     ( input == 0x75 )  goto _69;
else if( input == 0x0 )   goto _272;
else                      goto _123;


    __quex_assert_no_passage();
_271:
    /* (969 from RELOAD_FORWARD)  */
    goto _512;


    __quex_assert_no_passage();
_69:
    /* (970 from 969)  */
    ++(me->buffer._read_p);

_514:
    input = *(me->buffer._read_p);


    __quex_debug_state(970);
if     ( input == 0x61 )  goto _70;
else if( input == 0x0 )   goto _274;
else                      goto _123;


    __quex_assert_no_passage();
_273:
    /* (970 from RELOAD_FORWARD)  */
    goto _514;


    __quex_assert_no_passage();
_70:
    /* (971 from 970)  */
    ++(me->buffer._read_p);

_516:
    input = *(me->buffer._read_p);


    __quex_debug_state(971);
if     ( input == 0x6C )  goto _71;
else if( input == 0x0 )   goto _276;
else                      goto _123;


    __quex_assert_no_passage();
_275:
    /* (971 from RELOAD_FORWARD)  */
    goto _516;


    __quex_assert_no_passage();
_277:
    /* (972 from RELOAD_FORWARD)  */
_518:
    input = *(me->buffer._read_p);


    __quex_debug_state(972);
if     ( input == 0x73 )  goto _72;
else if( input == 0x0 )   goto _278;
else                      goto _123;


    __quex_assert_no_passage();
_71:
    /* (972 from 971)  */
    ++(me->buffer._read_p);

    goto _518;


    __quex_assert_no_passage();
_279:
    /* (973 from RELOAD_FORWARD)  */
_520:
    input = *(me->buffer._read_p);


    __quex_debug_state(973);
if     ( input == 0x2D )  goto _73;
else if( input == 0x0 )   goto _280;
else                      goto _150;


    __quex_assert_no_passage();
_72:
    /* (973 from 972)  */
    ++(me->buffer._read_p);

    goto _520;


    __quex_assert_no_passage();
_281:
    /* (974 from RELOAD_FORWARD)  */
_522:
    input = *(me->buffer._read_p);


    __quex_debug_state(974);
if     ( input == 0x6F )  goto _74;
else if( input == 0x0 )   goto _282;
else                      goto _151;


    __quex_assert_no_passage();
_73:
    /* (974 from 973)  */
    ++(me->buffer._read_p);

    goto _522;


    __quex_assert_no_passage();
_74:
    /* (975 from 974)  */
    ++(me->buffer._read_p);

_524:
    input = *(me->buffer._read_p);


    __quex_debug_state(975);
if     ( input == 0x72 )  goto _75;
else if( input == 0x0 )   goto _284;
else                      goto _152;


    __quex_assert_no_passage();
_283:
    /* (975 from RELOAD_FORWARD)  */
    goto _524;


    __quex_assert_no_passage();
_285:
    /* (976 from RELOAD_FORWARD)  */
_526:
    input = *(me->buffer._read_p);


    __quex_debug_state(976);
if     ( input == 0x64 )  goto _76;
else if( input == 0x0 )   goto _286;
else                      goto _153;


    __quex_assert_no_passage();
_75:
    /* (976 from 975)  */
    ++(me->buffer._read_p);

    goto _526;


    __quex_assert_no_passage();
_76:
    /* (977 from 976)  */
    ++(me->buffer._read_p);

_528:
    input = *(me->buffer._read_p);


    __quex_debug_state(977);
if     ( input == 0x65 )  goto _77;
else if( input == 0x0 )   goto _288;
else                      goto _154;


    __quex_assert_no_passage();
_287:
    /* (977 from RELOAD_FORWARD)  */
    goto _528;


    __quex_assert_no_passage();
_77:
    /* (978 from 977)  */
    ++(me->buffer._read_p);

_530:
    input = *(me->buffer._read_p);


    __quex_debug_state(978);
if     ( input == 0x72 )  goto _78;
else if( input == 0x0 )   goto _290;
else                      goto _155;


    __quex_assert_no_passage();
_289:
    /* (978 from RELOAD_FORWARD)  */
    goto _530;


    __quex_assert_no_passage();
_78:
    /* (979 from 978)  */
    ++(me->buffer._read_p);

_532:
    input = *(me->buffer._read_p);


    __quex_debug_state(979);
if     ( input == 0x65 )  goto _79;
else if( input == 0x0 )   goto _292;
else                      goto _156;


    __quex_assert_no_passage();
_291:
    /* (979 from RELOAD_FORWARD)  */
    goto _532;


    __quex_assert_no_passage();
_293:
    /* (980 from RELOAD_FORWARD)  */
_534:
    input = *(me->buffer._read_p);


    __quex_debug_state(980);
if     ( input == 0x64 )  goto _80;
else if( input == 0x0 )   goto _294;
else                      goto _157;


    __quex_assert_no_passage();
_79:
    /* (980 from 979)  */
    ++(me->buffer._read_p);

    goto _534;


    __quex_assert_no_passage();
_80:
    /* (981 from 980)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(981);
goto _158;


    __quex_assert_no_passage();
_81:
    /* (982 from 920)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(982);
goto _159;


    __quex_assert_no_passage();
_82:
    /* (983 from 919)  */
    ++(me->buffer._read_p);

_538:
    input = *(me->buffer._read_p);


    __quex_debug_state(983);
if     ( input == 0x79 )  goto _90;
else if( input == 0x0 )   goto _296;
else                      goto _123;


    __quex_assert_no_passage();
_295:
    /* (983 from RELOAD_FORWARD)  */
    goto _538;


    __quex_assert_no_passage();
_297:
    /* (984 from RELOAD_FORWARD)  */
_540:
    input = *(me->buffer._read_p);


    __quex_debug_state(984);
if     ( input == 0x6C )  goto _84;
else if( input == 0x0 )   goto _298;
else                      goto _123;


    __quex_assert_no_passage();
_83:
    /* (984 from 919)  */
    ++(me->buffer._read_p);

    goto _540;


    __quex_assert_no_passage();
_84:
    /* (985 from 984)  */
    ++(me->buffer._read_p);

_542:
    input = *(me->buffer._read_p);


    __quex_debug_state(985);
switch( input ) {
case 0x0: goto _300;
case 0x2D: goto _85;
case 0x6F: goto _86;
default: goto _123;
}


    __quex_assert_no_passage();
_299:
    /* (985 from RELOAD_FORWARD)  */
    goto _542;


    __quex_assert_no_passage();
_85:
    /* (986 from 985)  */
    ++(me->buffer._read_p);

_544:
    input = *(me->buffer._read_p);


    __quex_debug_state(986);
if     ( input == 0x6F )  goto _88;
else if( input == 0x0 )   goto _302;
else                      goto _123;


    __quex_assert_no_passage();
_301:
    /* (986 from RELOAD_FORWARD)  */
    goto _544;


    __quex_assert_no_passage();
_86:
    /* (987 from 985)  */
    ++(me->buffer._read_p);

_546:
    input = *(me->buffer._read_p);


    __quex_debug_state(987);
if     ( input == 0x77 )  goto _87;
else if( input == 0x0 )   goto _304;
else                      goto _123;


    __quex_assert_no_passage();
_303:
    /* (987 from RELOAD_FORWARD)  */
    goto _546;


    __quex_assert_no_passage();
_87:
    /* (988 from 987)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(988);
goto _160;


    __quex_assert_no_passage();
_305:
    /* (989 from RELOAD_FORWARD)  */
_549:
    input = *(me->buffer._read_p);


    __quex_debug_state(989);
if     ( input == 0x66 )  goto _89;
else if( input == 0x0 )   goto _306;
else                      goto _123;


    __quex_assert_no_passage();
_88:
    /* (989 from 986)  */
    ++(me->buffer._read_p);

    goto _549;


    __quex_assert_no_passage();
_89:
    /* (990 from 989)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(990);
goto _161;


    __quex_assert_no_passage();
_90:
    /* (991 from 983)  */
    ++(me->buffer._read_p);

_552:
    input = *(me->buffer._read_p);


    __quex_debug_state(991);
if     ( input == 0x2D )  goto _91;
else if( input == 0x0 )   goto _308;
else                      goto _123;


    __quex_assert_no_passage();
_307:
    /* (991 from RELOAD_FORWARD)  */
    goto _552;


    __quex_assert_no_passage();
_309:
    /* (992 from RELOAD_FORWARD)  */
_554:
    input = *(me->buffer._read_p);


    __quex_debug_state(992);
if     ( input == 0x6F )  goto _92;
else if( input == 0x0 )   goto _310;
else                      goto _123;


    __quex_assert_no_passage();
_91:
    /* (992 from 991)  */
    ++(me->buffer._read_p);

    goto _554;


    __quex_assert_no_passage();
_92:
    /* (993 from 992)  */
    ++(me->buffer._read_p);

_556:
    input = *(me->buffer._read_p);


    __quex_debug_state(993);
if     ( input == 0x66 )  goto _93;
else if( input == 0x0 )   goto _312;
else                      goto _123;


    __quex_assert_no_passage();
_311:
    /* (993 from RELOAD_FORWARD)  */
    goto _556;


    __quex_assert_no_passage();
_93:
    /* (994 from 993)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(994);
goto _162;


    __quex_assert_no_passage();
_94:
    /* (995 from 918)  */
    ++(me->buffer._read_p);

_559:
    input = *(me->buffer._read_p);


    __quex_debug_state(995);
if     ( input == 0x6F )  goto _95;
else if( input == 0x0 )   goto _314;
else                      goto _123;


    __quex_assert_no_passage();
_313:
    /* (995 from RELOAD_FORWARD)  */
    goto _559;


    __quex_assert_no_passage();
_315:
    /* (996 from RELOAD_FORWARD)  */
_561:
    input = *(me->buffer._read_p);


    __quex_debug_state(996);
if     ( input == 0x63 )  goto _96;
else if( input == 0x0 )   goto _316;
else                      goto _123;


    __quex_assert_no_passage();
_95:
    /* (996 from 995)  */
    ++(me->buffer._read_p);

    goto _561;


    __quex_assert_no_passage();
_317:
    /* (997 from RELOAD_FORWARD)  */
_563:
    input = *(me->buffer._read_p);


    __quex_debug_state(997);
if     ( input == 0x6B )  goto _97;
else if( input == 0x0 )   goto _318;
else                      goto _123;


    __quex_assert_no_passage();
_96:
    /* (997 from 996)  */
    ++(me->buffer._read_p);

    goto _563;


    __quex_assert_no_passage();
_97:
    /* (998 from 997)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(998);
goto _163;


    __quex_assert_no_passage();
_98:
    /* (999 from 917)  */
    ++(me->buffer._read_p);

_566:
    input = *(me->buffer._read_p);


    __quex_debug_state(999);
if     ( input == 0x72 )  goto _99;
else if( input == 0x0 )   goto _320;
else                      goto _123;


    __quex_assert_no_passage();
_319:
    /* (999 from RELOAD_FORWARD)  */
    goto _566;


    __quex_assert_no_passage();
_321:
    /* (1000 from RELOAD_FORWARD)  */
_568:
    input = *(me->buffer._read_p);


    __quex_debug_state(1000);
if     ( input == 0x69 )  goto _100;
else if( input == 0x0 )   goto _322;
else                      goto _123;


    __quex_assert_no_passage();
_99:
    /* (1000 from 999)  */
    ++(me->buffer._read_p);

    goto _568;


    __quex_assert_no_passage();
_100:
    /* (1001 from 1000)  */
    ++(me->buffer._read_p);

_570:
    input = *(me->buffer._read_p);


    __quex_debug_state(1001);
if     ( input == 0x61 )  goto _101;
else if( input == 0x0 )   goto _324;
else                      goto _123;


    __quex_assert_no_passage();
_323:
    /* (1001 from RELOAD_FORWARD)  */
    goto _570;


    __quex_assert_no_passage();
_101:
    /* (1002 from 1001)  */
    ++(me->buffer._read_p);

_572:
    input = *(me->buffer._read_p);


    __quex_debug_state(1002);
if     ( input == 0x6C )  goto _102;
else if( input == 0x0 )   goto _326;
else                      goto _123;


    __quex_assert_no_passage();
_325:
    /* (1002 from RELOAD_FORWARD)  */
    goto _572;


    __quex_assert_no_passage();
_102:
    /* (1003 from 1002)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1003);
goto _164;


    __quex_assert_no_passage();
_327:
    /* (1004 from RELOAD_FORWARD)  */
_575:
    input = *(me->buffer._read_p);


    __quex_debug_state(1004);
if     ( input == 0x6E )  goto _107;
else if( input == 0x0 )   goto _328;
else                      goto _123;


    __quex_assert_no_passage();
_103:
    /* (1004 from 916)  */
    ++(me->buffer._read_p);

    goto _575;


    __quex_assert_no_passage();
_329:
    /* (1005 from RELOAD_FORWARD)  */
_577:
    input = *(me->buffer._read_p);


    __quex_debug_state(1005);
if     ( input == 0x6D )  goto _105;
else if( input == 0x0 )   goto _330;
else                      goto _123;


    __quex_assert_no_passage();
_104:
    /* (1005 from 916)  */
    ++(me->buffer._read_p);

    goto _577;


    __quex_assert_no_passage();
_331:
    /* (1006 from RELOAD_FORWARD)  */
_579:
    input = *(me->buffer._read_p);


    __quex_debug_state(1006);
if     ( input == 0x65 )  goto _106;
else if( input == 0x0 )   goto _332;
else                      goto _123;


    __quex_assert_no_passage();
_105:
    /* (1006 from 1005)  */
    ++(me->buffer._read_p);

    goto _579;


    __quex_assert_no_passage();
_106:
    /* (1007 from 1006)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1007);
goto _165;


    __quex_assert_no_passage();
_333:
    /* (1008 from RELOAD_FORWARD)  */
_582:
    input = *(me->buffer._read_p);


    __quex_debug_state(1008);
if     ( input == 0x65 )  goto _108;
else if( input == 0x0 )   goto _334;
else                      goto _123;


    __quex_assert_no_passage();
_107:
    /* (1008 from 1004)  */
    ++(me->buffer._read_p);

    goto _582;


    __quex_assert_no_passage();
_108:
    /* (1009 from 1008)  */
    ++(me->buffer._read_p);

_584:
    input = *(me->buffer._read_p);


    __quex_debug_state(1009);
if     ( input == 0x2D )  goto _109;
else if( input == 0x0 )   goto _336;
else                      goto _123;


    __quex_assert_no_passage();
_335:
    /* (1009 from RELOAD_FORWARD)  */
    goto _584;


    __quex_assert_no_passage();
_109:
    /* (1010 from 1009)  */
    ++(me->buffer._read_p);

_586:
    input = *(me->buffer._read_p);


    __quex_debug_state(1010);
if     ( input == 0x6F )  goto _110;
else if( input == 0x0 )   goto _338;
else                      goto _123;


    __quex_assert_no_passage();
_337:
    /* (1010 from RELOAD_FORWARD)  */
    goto _586;


    __quex_assert_no_passage();
_110:
    /* (1011 from 1010)  */
    ++(me->buffer._read_p);

_588:
    input = *(me->buffer._read_p);


    __quex_debug_state(1011);
if     ( input == 0x66 )  goto _111;
else if( input == 0x0 )   goto _340;
else                      goto _123;


    __quex_assert_no_passage();
_339:
    /* (1011 from RELOAD_FORWARD)  */
    goto _588;


    __quex_assert_no_passage();
_111:
    /* (1012 from 1011)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1012);
goto _166;


    __quex_assert_no_passage();
_341:
    /* (1013 from RELOAD_FORWARD)  */
_591:
    input = *(me->buffer._read_p);


    __quex_debug_state(1013);
if     ( input == 0x73 )  goto _113;
else if( input == 0x0 )   goto _342;
else                      goto _123;


    __quex_assert_no_passage();
_112:
    /* (1013 from 915)  */
    ++(me->buffer._read_p);

    goto _591;


    __quex_assert_no_passage();
_343:
    /* (1014 from RELOAD_FORWARD)  */
_593:
    input = *(me->buffer._read_p);


    __quex_debug_state(1014);
if     ( input == 0x68 )  goto _114;
else if( input == 0x0 )   goto _344;
else                      goto _123;


    __quex_assert_no_passage();
_113:
    /* (1014 from 1013)  */
    ++(me->buffer._read_p);

    goto _593;


    __quex_assert_no_passage();
_114:
    /* (1015 from 1014)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1015);
goto _167;


    __quex_assert_no_passage();
_345:
    /* (1016 from RELOAD_FORWARD)  */
_596:
    input = *(me->buffer._read_p);


    __quex_debug_state(1016);
if     ( input == 0x74 )  goto _116;
else if( input == 0x0 )   goto _346;
else                      goto _123;


    __quex_assert_no_passage();
_115:
    /* (1016 from 913)  */
    ++(me->buffer._read_p);

    goto _596;


    __quex_assert_no_passage();
_116:
    /* (1017 from 1016)  */
    ++(me->buffer._read_p);

_598:
    input = *(me->buffer._read_p);


    __quex_debug_state(1017);
if     ( input == 0x68 )  goto _117;
else if( input == 0x0 )   goto _348;
else                      goto _123;


    __quex_assert_no_passage();
_347:
    /* (1017 from RELOAD_FORWARD)  */
    goto _598;


    __quex_assert_no_passage();
_117:
    /* (1018 from 1017)  */
    ++(me->buffer._read_p);

_600:
    input = *(me->buffer._read_p);


    __quex_debug_state(1018);
if     ( input == 0x2D )  goto _118;
else if( input == 0x0 )   goto _350;
else                      goto _123;


    __quex_assert_no_passage();
_349:
    /* (1018 from RELOAD_FORWARD)  */
    goto _600;


    __quex_assert_no_passage();
_118:
    /* (1019 from 1018)  */
    ++(me->buffer._read_p);

_602:
    input = *(me->buffer._read_p);


    __quex_debug_state(1019);
if     ( input == 0x69 )  goto _119;
else if( input == 0x0 )   goto _352;
else                      goto _123;


    __quex_assert_no_passage();
_351:
    /* (1019 from RELOAD_FORWARD)  */
    goto _602;


    __quex_assert_no_passage();
_353:
    /* (1020 from RELOAD_FORWARD)  */
_604:
    input = *(me->buffer._read_p);


    __quex_debug_state(1020);
if     ( input == 0x6E )  goto _120;
else if( input == 0x0 )   goto _354;
else                      goto _123;


    __quex_assert_no_passage();
_119:
    /* (1020 from 1019)  */
    ++(me->buffer._read_p);

    goto _604;


    __quex_assert_no_passage();
_355:
    /* (1021 from RELOAD_FORWARD)  */
_606:
    input = *(me->buffer._read_p);


    __quex_debug_state(1021);
if     ( input == 0x74 )  goto _121;
else if( input == 0x0 )   goto _356;
else                      goto _123;


    __quex_assert_no_passage();
_120:
    /* (1021 from 1020)  */
    ++(me->buffer._read_p);

    goto _606;


    __quex_assert_no_passage();
_357:
    /* (1022 from RELOAD_FORWARD)  */
_608:
    input = *(me->buffer._read_p);


    __quex_debug_state(1022);
if     ( input == 0x65 )  goto _122;
else if( input == 0x0 )   goto _358;
else                      goto _123;


    __quex_assert_no_passage();
_121:
    /* (1022 from 1021)  */
    ++(me->buffer._read_p);

    goto _608;


    __quex_assert_no_passage();
_122:
    /* (1023 from 1022)  */
    ++(me->buffer._read_p);

_610:
    input = *(me->buffer._read_p);


    __quex_debug_state(1023);
if     ( input == 0x72 )  goto _4;
else if( input == 0x0 )   goto _360;
else                      goto _123;


    __quex_assert_no_passage();
_359:
    /* (1023 from RELOAD_FORWARD)  */
    goto _610;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_177:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 3810 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_365:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
    std::cerr << "Failed lexeme: " << Lexeme << std::endl;


#   line 3825 "Lexer.cpp"

}
goto _1;
_372:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{

#   line 37 "Lexer.qx"


#   line 3837 "Lexer.cpp"

}
goto _0;
_383:
    __quex_debug("* TERMINAL \"*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 39 "Lexer.qx"
self_send(RULE_TOKEN_ASTERISK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3852 "Lexer.cpp"

}
goto _0;
_390:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 40 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3867 "Lexer.cpp"

}
goto _0;
_385:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 41 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3882 "Lexer.cpp"

}
goto _0;
_387:
    __quex_debug("* TERMINAL \":\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 42 "Lexer.qx"
self_send(RULE_TOKEN_COLON);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3897 "Lexer.cpp"

}
goto _0;
_377:
    __quex_debug("* TERMINAL \"!\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 43 "Lexer.qx"
self_send(RULE_TOKEN_NEGATION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3912 "Lexer.cpp"

}
goto _0;
_376:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 45 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3927 "Lexer.cpp"

}
goto _0;
_378:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 46 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3942 "Lexer.cpp"

}
goto _0;
_388:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 47 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3957 "Lexer.cpp"

}
goto _0;
_386:
    __quex_debug("* TERMINAL \"match\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 48 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_MATCH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3972 "Lexer.cpp"

}
goto _0;
_367:
    __quex_debug("* TERMINAL \"device\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 49 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_DEVICE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 3987 "Lexer.cpp"

}
goto _0;
_391:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 51 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4002 "Lexer.cpp"

}
goto _0;
_374:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 52 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4017 "Lexer.cpp"

}
goto _0;
_373:
    __quex_debug("* TERMINAL \"via-port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{

#   line 53 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_VIAPORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4032 "Lexer.cpp"

}
goto _0;
_381:
    __quex_debug("* TERMINAL \"with-interface\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 54 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_WITHINTERFACE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4047 "Lexer.cpp"

}
goto _0;
_384:
    __quex_debug("* TERMINAL \"serial\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 55 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_SERIAL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4062 "Lexer.cpp"

}
goto _0;
_371:
    __quex_debug("* TERMINAL \"if\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{

#   line 56 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_IF);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4077 "Lexer.cpp"

}
goto _0;
_382:
    __quex_debug("* TERMINAL \"all-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 58 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4093 "Lexer.cpp"

}
goto _0;
_389:
    __quex_debug("* TERMINAL \"any-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 59 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4109 "Lexer.cpp"

}
goto _0;
_380:
    __quex_debug("* TERMINAL \"one-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 60 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4125 "Lexer.cpp"

}
goto _0;
_369:
    __quex_debug("* TERMINAL \"none-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{

#   line 61 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4141 "Lexer.cpp"

}
goto _0;
_366:
    __quex_debug("* TERMINAL \"equals\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 62 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4157 "Lexer.cpp"

}
goto _0;
_392:
    __quex_debug("* TERMINAL \"equals-ordered\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 63 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4173 "Lexer.cpp"

}
goto _0;
_379:
    __quex_debug("* TERMINAL \"device-matches\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 65 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4189 "Lexer.cpp"

}
goto _0;
_375:
    __quex_debug("* TERMINAL [0-9a-fA-F]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 67 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_HEXCHAR4);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4205 "Lexer.cpp"

}
goto _0;
_370:
    __quex_debug("* TERMINAL [0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{

#   line 68 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_HEXCHAR2);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4221 "Lexer.cpp"

}
goto _0;
_393:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 74 "Lexer.qx"
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 4237 "Lexer.cpp"

}
goto _0;
_368:
    __quex_debug("* TERMINAL \"(\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 80 "Lexer.qx"
    self_send(RULE_TOKEN_PQ_STRING_BEGIN);
    self << PQ_STRING_READER;
    RETURN;


#   line 4253 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_613:
    switch( target_state_index ) {
        case 3: { goto _3; }
        case 123: { goto _123; }
        case 126: { goto _126; }
        case 132: { goto _132; }
        case 133: { goto _133; }
        case 135: { goto _135; }
        case 136: { goto _136; }
        case 137: { goto _137; }
        case 138: { goto _138; }
        case 139: { goto _139; }
        case 140: { goto _140; }
        case 141: { goto _141; }
        case 142: { goto _142; }
        case 143: { goto _143; }
        case 144: { goto _144; }
        case 150: { goto _150; }
        case 151: { goto _151; }
        case 152: { goto _152; }
        case 153: { goto _153; }
        case 154: { goto _154; }
        case 155: { goto _155; }
        case 156: { goto _156; }
        case 157: { goto _157; }
        case 169: { goto _169; }
        case 171: { goto _171; }
        case 173: { goto _173; }
        case 175: { goto _175; }
        case 177: { goto _177; }
        case 179: { goto _179; }
        case 181: { goto _181; }
        case 183: { goto _183; }
        case 185: { goto _185; }
        case 187: { goto _187; }
        case 189: { goto _189; }
        case 191: { goto _191; }
        case 193: { goto _193; }
        case 195: { goto _195; }
        case 197: { goto _197; }
        case 199: { goto _199; }
        case 201: { goto _201; }
        case 203: { goto _203; }
        case 205: { goto _205; }
        case 207: { goto _207; }
        case 209: { goto _209; }
        case 211: { goto _211; }
        case 213: { goto _213; }
        case 215: { goto _215; }
        case 217: { goto _217; }
        case 219: { goto _219; }
        case 221: { goto _221; }
        case 223: { goto _223; }
        case 225: { goto _225; }
        case 227: { goto _227; }
        case 229: { goto _229; }
        case 231: { goto _231; }
        case 233: { goto _233; }
        case 235: { goto _235; }
        case 237: { goto _237; }
        case 239: { goto _239; }
        case 241: { goto _241; }
        case 243: { goto _243; }
        case 245: { goto _245; }
        case 247: { goto _247; }
        case 249: { goto _249; }
        case 251: { goto _251; }
        case 253: { goto _253; }
        case 255: { goto _255; }
        case 257: { goto _257; }
        case 259: { goto _259; }
        case 261: { goto _261; }
        case 263: { goto _263; }
        case 265: { goto _265; }
        case 267: { goto _267; }
        case 269: { goto _269; }
        case 271: { goto _271; }
        case 273: { goto _273; }
        case 275: { goto _275; }
        case 277: { goto _277; }
        case 279: { goto _279; }
        case 281: { goto _281; }
        case 283: { goto _283; }
        case 285: { goto _285; }
        case 287: { goto _287; }
        case 289: { goto _289; }
        case 291: { goto _291; }
        case 293: { goto _293; }
        case 295: { goto _295; }
        case 297: { goto _297; }
        case 299: { goto _299; }
        case 301: { goto _301; }
        case 303: { goto _303; }
        case 305: { goto _305; }
        case 307: { goto _307; }
        case 309: { goto _309; }
        case 311: { goto _311; }
        case 313: { goto _313; }
        case 315: { goto _315; }
        case 317: { goto _317; }
        case 319: { goto _319; }
        case 321: { goto _321; }
        case 323: { goto _323; }
        case 325: { goto _325; }
        case 327: { goto _327; }
        case 329: { goto _329; }
        case 331: { goto _331; }
        case 333: { goto _333; }
        case 335: { goto _335; }
        case 337: { goto _337; }
        case 339: { goto _339; }
        case 341: { goto _341; }
        case 343: { goto _343; }
        case 345: { goto _345; }
        case 347: { goto _347; }
        case 349: { goto _349; }
        case 351: { goto _351; }
        case 353: { goto _353; }
        case 355: { goto _355; }
        case 357: { goto _357; }
        case 359: { goto _359; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_238:
    /* (RELOAD_FORWARD from 948)  */
    target_state_index = QUEX_LABEL(237); target_state_else_index = QUEX_LABEL(123);

_611:
    __quex_debug3("RELOAD_FORWARD: success->%i; failure->%i", (int)target_state_index, (int)target_state_else_index);
    __quex_assert(*(me->buffer._read_p) == QUEX_SETTING_BUFFER_LIMIT_CODE);
    /* Detect whether the buffer limit code appeared at non-border.          */
    if( me->buffer._read_p != me->buffer.input.end_p ) {
        __quex_assert(false); /* Later: on codec error! */
    }
    __quex_debug_reload_before();                 /* Report source position. */
    if( QUEX_NAME(Buffer_load_forward)(&me->buffer, (QUEX_TYPE_CHARACTER**)position, PositionRegisterN) ) {
        __quex_debug_reload_after();
        QUEX_GOTO_STATE(target_state_index);      /* may use 'computed goto' */
    }
    __quex_debug("reload impossible\n");
    QUEX_GOTO_STATE(target_state_else_index);     /* may use 'computed goto' */


    __quex_assert_no_passage();
_204:
    /* (RELOAD_FORWARD from 925)  */
    target_state_index = QUEX_LABEL(203); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_336:
    /* (RELOAD_FORWARD from 1009)  */
    target_state_index = QUEX_LABEL(335); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_182:
    /* (RELOAD_FORWARD from 913)  */
    target_state_index = QUEX_LABEL(181); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_298:
    /* (RELOAD_FORWARD from 984)  */
    target_state_index = QUEX_LABEL(297); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_192:
    /* (RELOAD_FORWARD from 918)  */
    target_state_index = QUEX_LABEL(191); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_282:
    /* (RELOAD_FORWARD from 974)  */
    target_state_index = QUEX_LABEL(281); target_state_else_index = QUEX_LABEL(151);

    goto _611;


    __quex_assert_no_passage();
_260:
    /* (RELOAD_FORWARD from 961)  */
    target_state_index = QUEX_LABEL(259); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_226:
    /* (RELOAD_FORWARD from 941)  */
    target_state_index = QUEX_LABEL(225); target_state_else_index = QUEX_LABEL(139);

    goto _611;


    __quex_assert_no_passage();
_358:
    /* (RELOAD_FORWARD from 1022)  */
    target_state_index = QUEX_LABEL(357); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_172:
    /* (RELOAD_FORWARD from 1025)  */
    target_state_index = QUEX_LABEL(171); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_304:
    /* (RELOAD_FORWARD from 987)  */
    target_state_index = QUEX_LABEL(303); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_278:
    /* (RELOAD_FORWARD from 972)  */
    target_state_index = QUEX_LABEL(277); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_288:
    /* (RELOAD_FORWARD from 977)  */
    target_state_index = QUEX_LABEL(287); target_state_else_index = QUEX_LABEL(154);

    goto _611;


    __quex_assert_no_passage();
_250:
    /* (RELOAD_FORWARD from 955)  */
    target_state_index = QUEX_LABEL(249); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_338:
    /* (RELOAD_FORWARD from 1010)  */
    target_state_index = QUEX_LABEL(337); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_232:
    /* (RELOAD_FORWARD from 944)  */
    target_state_index = QUEX_LABEL(231); target_state_else_index = QUEX_LABEL(142);

    goto _611;


    __quex_assert_no_passage();
_348:
    /* (RELOAD_FORWARD from 1017)  */
    target_state_index = QUEX_LABEL(347); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_194:
    /* (RELOAD_FORWARD from 919)  */
    target_state_index = QUEX_LABEL(193); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_326:
    /* (RELOAD_FORWARD from 1002)  */
    target_state_index = QUEX_LABEL(325); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_268:
    /* (RELOAD_FORWARD from 966)  */
    target_state_index = QUEX_LABEL(267); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_246:
    /* (RELOAD_FORWARD from 953)  */
    target_state_index = QUEX_LABEL(245); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_256:
    /* (RELOAD_FORWARD from 959)  */
    target_state_index = QUEX_LABEL(255); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_212:
    /* (RELOAD_FORWARD from 933)  */
    target_state_index = QUEX_LABEL(211); target_state_else_index = QUEX_LABEL(132);

    goto _611;


    __quex_assert_no_passage();
_344:
    /* (RELOAD_FORWARD from 1014)  */
    target_state_index = QUEX_LABEL(343); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_222:
    /* (RELOAD_FORWARD from 939)  */
    target_state_index = QUEX_LABEL(221); target_state_else_index = QUEX_LABEL(137);

    goto _611;


    __quex_assert_no_passage();
_306:
    /* (RELOAD_FORWARD from 989)  */
    target_state_index = QUEX_LABEL(305); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_200:
    /* (RELOAD_FORWARD from 922)  */
    target_state_index = QUEX_LABEL(199); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_316:
    /* (RELOAD_FORWARD from 996)  */
    target_state_index = QUEX_LABEL(315); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_290:
    /* (RELOAD_FORWARD from 978)  */
    target_state_index = QUEX_LABEL(289); target_state_else_index = QUEX_LABEL(155);

    goto _611;


    __quex_assert_no_passage();
_236:
    /* (RELOAD_FORWARD from 946)  */
    target_state_index = QUEX_LABEL(235); target_state_else_index = QUEX_LABEL(144);

    goto _611;


    __quex_assert_no_passage();
_202:
    /* (RELOAD_FORWARD from 924)  */
    target_state_index = QUEX_LABEL(201); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_334:
    /* (RELOAD_FORWARD from 1008)  */
    target_state_index = QUEX_LABEL(333); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_180:
    /* (RELOAD_FORWARD from 912)  */
    target_state_index = QUEX_LABEL(179); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_312:
    /* (RELOAD_FORWARD from 993)  */
    target_state_index = QUEX_LABEL(311); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_190:
    /* (RELOAD_FORWARD from 917)  */
    target_state_index = QUEX_LABEL(189); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_296:
    /* (RELOAD_FORWARD from 983)  */
    target_state_index = QUEX_LABEL(295); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_258:
    /* (RELOAD_FORWARD from 960)  */
    target_state_index = QUEX_LABEL(257); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_208:
    /* (RELOAD_FORWARD from 931)  */
    target_state_index = QUEX_LABEL(207); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_356:
    /* (RELOAD_FORWARD from 1021)  */
    target_state_index = QUEX_LABEL(355); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_170:
    /* (RELOAD_FORWARD from 1024)  */
    target_state_index = QUEX_LABEL(169); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_302:
    /* (RELOAD_FORWARD from 986)  */
    target_state_index = QUEX_LABEL(301); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_276:
    /* (RELOAD_FORWARD from 971)  */
    target_state_index = QUEX_LABEL(275); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_286:
    /* (RELOAD_FORWARD from 976)  */
    target_state_index = QUEX_LABEL(285); target_state_else_index = QUEX_LABEL(153);

    goto _611;


    __quex_assert_no_passage();
_264:
    /* (RELOAD_FORWARD from 964)  */
    target_state_index = QUEX_LABEL(263); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_230:
    /* (RELOAD_FORWARD from 943)  */
    target_state_index = QUEX_LABEL(229); target_state_else_index = QUEX_LABEL(141);

    goto _611;


    __quex_assert_no_passage();
_346:
    /* (RELOAD_FORWARD from 1016)  */
    target_state_index = QUEX_LABEL(345); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_176:
    /* (RELOAD_FORWARD from 1027)  */
    target_state_index = QUEX_LABEL(175); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_324:
    /* (RELOAD_FORWARD from 1001)  */
    target_state_index = QUEX_LABEL(323); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_266:
    /* (RELOAD_FORWARD from 965)  */
    target_state_index = QUEX_LABEL(265); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_244:
    /* (RELOAD_FORWARD from 952)  */
    target_state_index = QUEX_LABEL(243); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_254:
    /* (RELOAD_FORWARD from 957)  */
    target_state_index = QUEX_LABEL(253); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_210:
    /* (RELOAD_FORWARD from 932)  */
    target_state_index = QUEX_LABEL(209); target_state_else_index = QUEX_LABEL(132);

    goto _611;


    __quex_assert_no_passage();
_342:
    /* (RELOAD_FORWARD from 1013)  */
    target_state_index = QUEX_LABEL(341); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_220:
    /* (RELOAD_FORWARD from 938)  */
    target_state_index = QUEX_LABEL(219); target_state_else_index = QUEX_LABEL(136);

    goto _611;


    __quex_assert_no_passage();
_352:
    /* (RELOAD_FORWARD from 1019)  */
    target_state_index = QUEX_LABEL(351); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_198:
    /* (RELOAD_FORWARD from 921)  */
    target_state_index = QUEX_LABEL(197); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_314:
    /* (RELOAD_FORWARD from 995)  */
    target_state_index = QUEX_LABEL(313); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_272:
    /* (RELOAD_FORWARD from 969)  */
    target_state_index = QUEX_LABEL(271); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_234:
    /* (RELOAD_FORWARD from 945)  */
    target_state_index = QUEX_LABEL(233); target_state_else_index = QUEX_LABEL(143);

    goto _611;


    __quex_assert_no_passage();
_216:
    /* (RELOAD_FORWARD from 935)  */
    target_state_index = QUEX_LABEL(215); target_state_else_index = QUEX_LABEL(133);

    goto _611;


    __quex_assert_no_passage();
_332:
    /* (RELOAD_FORWARD from 1006)  */
    target_state_index = QUEX_LABEL(331); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_178:
    /* (RELOAD_FORWARD from 909)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(177);

    goto _611;


    __quex_assert_no_passage();
_310:
    /* (RELOAD_FORWARD from 992)  */
    target_state_index = QUEX_LABEL(309); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_188:
    /* (RELOAD_FORWARD from 916)  */
    target_state_index = QUEX_LABEL(187); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_320:
    /* (RELOAD_FORWARD from 999)  */
    target_state_index = QUEX_LABEL(319); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_294:
    /* (RELOAD_FORWARD from 980)  */
    target_state_index = QUEX_LABEL(293); target_state_else_index = QUEX_LABEL(157);

    goto _611;


    __quex_assert_no_passage();
_240:
    /* (RELOAD_FORWARD from 949)  */
    target_state_index = QUEX_LABEL(239); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_206:
    /* (RELOAD_FORWARD from 928)  */
    target_state_index = QUEX_LABEL(205); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_354:
    /* (RELOAD_FORWARD from 1020)  */
    target_state_index = QUEX_LABEL(353); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_184:
    /* (RELOAD_FORWARD from 914)  */
    target_state_index = QUEX_LABEL(183); target_state_else_index = QUEX_LABEL(126);

    goto _611;


    __quex_assert_no_passage();
_300:
    /* (RELOAD_FORWARD from 985)  */
    target_state_index = QUEX_LABEL(299); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_274:
    /* (RELOAD_FORWARD from 970)  */
    target_state_index = QUEX_LABEL(273); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_284:
    /* (RELOAD_FORWARD from 975)  */
    target_state_index = QUEX_LABEL(283); target_state_else_index = QUEX_LABEL(152);

    goto _611;


    __quex_assert_no_passage();
_262:
    /* (RELOAD_FORWARD from 962)  */
    target_state_index = QUEX_LABEL(261); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_228:
    /* (RELOAD_FORWARD from 942)  */
    target_state_index = QUEX_LABEL(227); target_state_else_index = QUEX_LABEL(140);

    goto _611;


    __quex_assert_no_passage();
_360:
    /* (RELOAD_FORWARD from 1023)  */
    target_state_index = QUEX_LABEL(359); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_174:
    /* (RELOAD_FORWARD from 1026)  */
    target_state_index = QUEX_LABEL(173); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_322:
    /* (RELOAD_FORWARD from 1000)  */
    target_state_index = QUEX_LABEL(321); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_280:
    /* (RELOAD_FORWARD from 973)  */
    target_state_index = QUEX_LABEL(279); target_state_else_index = QUEX_LABEL(150);

    goto _611;


    __quex_assert_no_passage();
_242:
    /* (RELOAD_FORWARD from 950)  */
    target_state_index = QUEX_LABEL(241); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_252:
    /* (RELOAD_FORWARD from 956)  */
    target_state_index = QUEX_LABEL(251); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_340:
    /* (RELOAD_FORWARD from 1011)  */
    target_state_index = QUEX_LABEL(339); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_218:
    /* (RELOAD_FORWARD from 937)  */
    target_state_index = QUEX_LABEL(217); target_state_else_index = QUEX_LABEL(135);

    goto _611;


    __quex_assert_no_passage();
_350:
    /* (RELOAD_FORWARD from 1018)  */
    target_state_index = QUEX_LABEL(349); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_196:
    /* (RELOAD_FORWARD from 920)  */
    target_state_index = QUEX_LABEL(195); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_328:
    /* (RELOAD_FORWARD from 1004)  */
    target_state_index = QUEX_LABEL(327); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_270:
    /* (RELOAD_FORWARD from 967)  */
    target_state_index = QUEX_LABEL(269); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_248:
    /* (RELOAD_FORWARD from 954)  */
    target_state_index = QUEX_LABEL(247); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_214:
    /* (RELOAD_FORWARD from 934)  */
    target_state_index = QUEX_LABEL(213); target_state_else_index = QUEX_LABEL(133);

    goto _611;


    __quex_assert_no_passage();
_330:
    /* (RELOAD_FORWARD from 1005)  */
    target_state_index = QUEX_LABEL(329); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_224:
    /* (RELOAD_FORWARD from 940)  */
    target_state_index = QUEX_LABEL(223); target_state_else_index = QUEX_LABEL(138);

    goto _611;


    __quex_assert_no_passage();
_308:
    /* (RELOAD_FORWARD from 991)  */
    target_state_index = QUEX_LABEL(307); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_186:
    /* (RELOAD_FORWARD from 915)  */
    target_state_index = QUEX_LABEL(185); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_318:
    /* (RELOAD_FORWARD from 997)  */
    target_state_index = QUEX_LABEL(317); target_state_else_index = QUEX_LABEL(123);

    goto _611;


    __quex_assert_no_passage();
_292:
    /* (RELOAD_FORWARD from 979)  */
    target_state_index = QUEX_LABEL(291); target_state_else_index = QUEX_LABEL(156);

    goto _611;

_2:
/* RETURN -- after executing 'on_after_match' code. */
    __QUEX_PURE_RETURN;


_0:
/* CONTINUE -- after executing 'on_after_match' code. */

_1:
/* CONTINUE -- without executing 'on_after_match' (e.g. on FAILURE). */


#   ifndef __QUEX_OPTION_PLAIN_ANALYZER_OBJECT
#   ifdef  QUEX_OPTION_TOKEN_POLICY_QUEUE
    if( QUEX_NAME(TokenQueue_is_full)(&self._token_queue) ) {
        return;
    }
#   else
    if( self_token_get_id() != __QUEX_SETTING_TOKEN_ID_UNINITIALIZED) {
        return __self_result_token_id;
    }
#   endif
#   endif


    /*  If a mode change happened, then the function must first return and
     *  indicate that another mode function is to be called. At this point, 
     *  we to force a 'return' on a mode change. 
     *
     *  Pseudo Code: if( previous_mode != current_mode ) {
     *                   return 0;
     *               }
     *
     *  When the analyzer returns, the caller function has to watch if a mode 
     *  change occurred. If not it can call this function again.             */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)        || defined(QUEX_OPTION_ASSERTS)
    if( me->DEBUG_analyzer_function_at_entry != me->current_analyzer_function ) 
#   endif
    { 
#       if defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)
        self_token_set_id(__QUEX_SETTING_TOKEN_ID_UNINITIALIZED);
        __QUEX_PURE_RETURN;
#       elif defined(QUEX_OPTION_ASSERTS)
        QUEX_ERROR_EXIT("Mode change without immediate return from the lexical analyzer.");
#       endif
    }


goto _612;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _613; /* in QUEX_GOTO_STATE       */
#   endif

    /* Prevent compiler warning 'unused variable'.                           */
    (void)QUEX_LEXEME_NULL;
    (void)QUEX_NAME_TOKEN(DumpedTokenIdObject);
    /* target_state_index and target_state_else_index appear when 
     * QUEX_GOTO_STATE is used without computed goto-s.                      */
    (void)target_state_index;
    (void)target_state_else_index;

#   undef Lexeme
#   undef LexemeBegin
#   undef LexemeEnd
#   undef LexemeNull
#   undef LexemeL
#   undef PQ_STRING_READER
#   undef RULE
#   undef DQ_STRING_READER
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
}
#ifdef      __QUEX_COUNT_VOID
#   undef   __QUEX_COUNT_VOID
#endif
#ifdef      __QUEX_OPTION_COUNTER
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) \
            do {                              \
                QUEX_NAME(PQ_STRING_READER_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif

#include <quex/code_base/analyzer/struct/basic>
#include <quex/code_base/buffer/Buffer>
#ifdef QUEX_OPTION_TOKEN_POLICY_QUEUE
#   include <quex/code_base/token/TokenQueue>
#endif

#ifdef    CONTINUE
#   undef CONTINUE
#endif
#define   CONTINUE do { goto _0; } while(0)

#ifdef    RETURN
#   undef RETURN
#endif
#define   RETURN   do { goto _1; } while(0)

__QUEX_TYPE_ANALYZER_RETURN_VALUE  
QUEX_NAME(DQ_STRING_READER_analyzer_function)(QUEX_TYPE_ANALYZER* me) 
{
    /* NOTE: Different modes correspond to different analyzer functions. The 
     *       analyzer functions are all located inside the main class as static
     *       functions. That means, they are something like 'globals'. They 
     *       receive a pointer to the lexical analyzer, since static members do
     *       not have access to the 'this' pointer.                          */
#   if defined(QUEX_OPTION_TOKEN_POLICY_SINGLE)
    register QUEX_TYPE_TOKEN_ID __self_result_token_id 
           = (QUEX_TYPE_TOKEN_ID)__QUEX_SETTING_TOKEN_ID_UNINITIALIZED;
#   endif
#   ifdef     self
#       undef self
#   endif
#   define self (*((QUEX_TYPE_ANALYZER*)me))
/*  'QUEX_GOTO_STATE' requires 'QUEX_LABEL_STATE_ROUTER' */
#   define QUEX_LABEL_STATE_ROUTER _51
#   define PQ_STRING_READER    (QUEX_NAME(PQ_STRING_READER))
#   define RULE                (QUEX_NAME(RULE))
#   define DQ_STRING_READER    (QUEX_NAME(DQ_STRING_READER))

    /* Lexeme setup: 
     *
     * There is a temporary zero stored at the end of each lexeme, if the action 
     * references to the 'Lexeme'. 'LexemeNull' provides a reference to an empty
     * zero terminated string.                                                    */
#if defined(QUEX_OPTION_ASSERTS)
#   define Lexeme       QUEX_NAME(access_Lexeme)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeBegin  QUEX_NAME(access_LexemeBegin)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeL      QUEX_NAME(access_LexemeL)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#   define LexemeEnd    QUEX_NAME(access_LexemeEnd)((const char*)__FILE__, (size_t)__LINE__, &me->buffer)
#else
#   define Lexeme       (me->buffer._lexeme_start_p)
#   define LexemeBegin  Lexeme
#   define LexemeL      ((size_t)(me->buffer._read_p - me->buffer._lexeme_start_p))
#   define LexemeEnd    me->buffer._read_p
#endif

#define LexemeNull      (&QUEX_LEXEME_NULL)
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_CHARACTER**          position                       = 0x0;

    /* Post context positions do not have to be reset or initialized. If a state
     * is reached which is associated with 'end of post context' it is clear what
     * post context is meant. This results from the ways the state machine is 
     * constructed. Post context position's live cycle:
     *
     * (1)   unitialized (don't care)
     * (1.b) on buffer reload it may, or may not be adapted (don't care)
     * (2)   when a post context begin state is passed, then it is **SET** (now: take care)
     * (2.b) on buffer reload it **is adapted**.
     * (3)   when a terminal state of the post context is reached (which can only be reached
     *       for that particular post context), then the post context position is used
     *       to reset the input position.                                              */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE) \
       || defined(QUEX_OPTION_ASSERTS)
    me->DEBUG_analyzer_function_at_entry = me->current_analyzer_function;
#   endif
_50:
    me->buffer._lexeme_start_p = me->buffer._read_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);
_3:
    /* (1102 from RELOAD_FORWARD) (1102 from BEFORE_ENTRY)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(1102);
switch( input ) {
case 0x0: goto _21;
case 0x1: 
case 0xA: goto _12;
case 0x22: goto _5;
case 0x5C: goto _4;
default: goto _6;
}


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 1105) (DROP_OUT from 1103)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 1110)  */
    goto _34;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 1107)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 1106)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 1104)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 1102)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 1109)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 1108)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (1103 from 1102)  */
    ++(me->buffer._read_p);

_39:
    input = *(me->buffer._read_p);


    __quex_debug_state(1103);
switch( input ) {
case 0x0: goto _23;
case 0x22: goto _7;
case 0x5C: goto _8;
case 0x78: goto _9;
default: goto _13;
}


    __quex_assert_no_passage();
_22:
    /* (1103 from RELOAD_FORWARD)  */
    goto _39;


    __quex_assert_no_passage();
_5:
    /* (1104 from 1102)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1104);
goto _14;


    __quex_assert_no_passage();
_6:
    /* (1105 from 1102)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1105);
goto _13;


    __quex_assert_no_passage();
_7:
    /* (1106 from 1103)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1106);
goto _15;


    __quex_assert_no_passage();
_8:
    /* (1107 from 1103)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1107);
goto _16;


    __quex_assert_no_passage();
_24:
    /* (1108 from RELOAD_FORWARD)  */
_45:
    input = *(me->buffer._read_p);


    __quex_debug_state(1108);
switch( input ) {
case 0x0: goto _25;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _10;
default: goto _17;
}


    __quex_assert_no_passage();
_9:
    /* (1108 from 1103)  */
    ++(me->buffer._read_p);

    goto _45;


    __quex_assert_no_passage();
_26:
    /* (1109 from RELOAD_FORWARD)  */
_47:
    input = *(me->buffer._read_p);


    __quex_debug_state(1109);
switch( input ) {
case 0x0: goto _27;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _11;
default: goto _18;
}


    __quex_assert_no_passage();
_10:
    /* (1109 from 1108)  */
    ++(me->buffer._read_p);

    goto _47;


    __quex_assert_no_passage();
_11:
    /* (1110 from 1109)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1110);
goto _19;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_20:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 5564 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _1;
_33:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
    throw std::runtime_error("Invalid double-quoted string");


#   line 5578 "Lexer.cpp"

}
goto _2;
_36:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 94 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 5593 "Lexer.cpp"

}
goto _0;
_35:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 97 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 5608 "Lexer.cpp"

}
goto _0;
_34:
    __quex_debug("* TERMINAL \"\\\\x\"[0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 100 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 5623 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 105 "Lexer.qx"
    self << RULE;
    RETURN;


#   line 5638 "Lexer.cpp"

}
goto _0;
_32:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 107 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 5652 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_51:
    switch( target_state_index ) {
        case 3: { goto _3; }
        case 13: { goto _13; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 20: { goto _20; }
        case 22: { goto _22; }
        case 24: { goto _24; }
        case 26: { goto _26; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_25:
    /* (RELOAD_FORWARD from 1108)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(17);

_49:
    __quex_debug3("RELOAD_FORWARD: success->%i; failure->%i", (int)target_state_index, (int)target_state_else_index);
    __quex_assert(*(me->buffer._read_p) == QUEX_SETTING_BUFFER_LIMIT_CODE);
    /* Detect whether the buffer limit code appeared at non-border.          */
    if( me->buffer._read_p != me->buffer.input.end_p ) {
        __quex_assert(false); /* Later: on codec error! */
    }
    __quex_debug_reload_before();                 /* Report source position. */
    if( QUEX_NAME(Buffer_load_forward)(&me->buffer, (QUEX_TYPE_CHARACTER**)position, PositionRegisterN) ) {
        __quex_debug_reload_after();
        QUEX_GOTO_STATE(target_state_index);      /* may use 'computed goto' */
    }
    __quex_debug("reload impossible\n");
    QUEX_GOTO_STATE(target_state_else_index);     /* may use 'computed goto' */


    __quex_assert_no_passage();
_21:
    /* (RELOAD_FORWARD from 1102)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(20);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 1109)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(18);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 1103)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(13);

    goto _49;

_1:
/* RETURN -- after executing 'on_after_match' code. */
    __QUEX_PURE_RETURN;


_0:
/* CONTINUE -- after executing 'on_after_match' code. */

_2:
/* CONTINUE -- without executing 'on_after_match' (e.g. on FAILURE). */


#   ifndef __QUEX_OPTION_PLAIN_ANALYZER_OBJECT
#   ifdef  QUEX_OPTION_TOKEN_POLICY_QUEUE
    if( QUEX_NAME(TokenQueue_is_full)(&self._token_queue) ) {
        return;
    }
#   else
    if( self_token_get_id() != __QUEX_SETTING_TOKEN_ID_UNINITIALIZED) {
        return __self_result_token_id;
    }
#   endif
#   endif


    /*  If a mode change happened, then the function must first return and
     *  indicate that another mode function is to be called. At this point, 
     *  we to force a 'return' on a mode change. 
     *
     *  Pseudo Code: if( previous_mode != current_mode ) {
     *                   return 0;
     *               }
     *
     *  When the analyzer returns, the caller function has to watch if a mode 
     *  change occurred. If not it can call this function again.             */
#   if    defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)        || defined(QUEX_OPTION_ASSERTS)
    if( me->DEBUG_analyzer_function_at_entry != me->current_analyzer_function ) 
#   endif
    { 
#       if defined(QUEX_OPTION_AUTOMATIC_ANALYSIS_CONTINUATION_ON_MODE_CHANGE)
        self_token_set_id(__QUEX_SETTING_TOKEN_ID_UNINITIALIZED);
        __QUEX_PURE_RETURN;
#       elif defined(QUEX_OPTION_ASSERTS)
        QUEX_ERROR_EXIT("Mode change without immediate return from the lexical analyzer.");
#       endif
    }


goto _50;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _1; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _2; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _51; /* in QUEX_GOTO_STATE       */
#   endif

    /* Prevent compiler warning 'unused variable'.                           */
    (void)QUEX_LEXEME_NULL;
    (void)QUEX_NAME_TOKEN(DumpedTokenIdObject);
    /* target_state_index and target_state_else_index appear when 
     * QUEX_GOTO_STATE is used without computed goto-s.                      */
    (void)target_state_index;
    (void)target_state_else_index;

#   undef Lexeme
#   undef LexemeBegin
#   undef LexemeEnd
#   undef LexemeNull
#   undef LexemeL
#   undef PQ_STRING_READER
#   undef RULE
#   undef DQ_STRING_READER
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
}
QUEX_NAMESPACE_MAIN_CLOSE


QUEX_NAMESPACE_TOKEN_OPEN

const char*
QUEX_NAME_TOKEN(map_id_to_name)(const QUEX_TYPE_TOKEN_ID TokenID)
{
   static char  error_string[64];
   static const char  uninitialized_string[] = "<UNINITIALIZED>";
   static const char  termination_string[]   = "<TERMINATION>";
#  if defined(QUEX_OPTION_INDENTATION_TRIGGER)
   static const char  indent_string[]        = "<INDENT>";
   static const char  dedent_string[]        = "<DEDENT>";
   static const char  nodent_string[]        = "<NODENT>";
#  endif
   static const char  token_id_str_ASTERISK[]              = "ASTERISK";
   static const char  token_id_str_COLON[]                 = "COLON";
   static const char  token_id_str_CONDITION_IDENTIFIER[]  = "CONDITION_IDENTIFIER";
   static const char  token_id_str_CURLYBRACE_CLOSE[]      = "CURLYBRACE_CLOSE";
   static const char  token_id_str_CURLYBRACE_OPEN[]       = "CURLYBRACE_OPEN";
   static const char  token_id_str_DQ_STRING[]             = "DQ_STRING";
   static const char  token_id_str_DQ_STRING_BEGIN[]       = "DQ_STRING_BEGIN";
   static const char  token_id_str_DQ_STRING_END[]         = "DQ_STRING_END";
   static const char  token_id_str_HEXCHAR2[]              = "HEXCHAR2";
   static const char  token_id_str_HEXCHAR4[]              = "HEXCHAR4";
   static const char  token_id_str_KEYWORD_ALLOW[]         = "KEYWORD_ALLOW";
   static const char  token_id_str_KEYWORD_BLOCK[]         = "KEYWORD_BLOCK";
   static const char  token_id_str_KEYWORD_DEVICE[]        = "KEYWORD_DEVICE";
   static const char  token_id_str_KEYWORD_HASH[]          = "KEYWORD_HASH";
   static const char  token_id_str_KEYWORD_IF[]            = "KEYWORD_IF";
   static const char  token_id_str_KEYWORD_MATCH[]         = "KEYWORD_MATCH";
   static const char  token_id_str_KEYWORD_NAME[]          = "KEYWORD_NAME";
   static const char  token_id_str_KEYWORD_REJECT[]        = "KEYWORD_REJECT";
   static const char  token_id_str_KEYWORD_SERIAL[]        = "KEYWORD_SERIAL";
   static const char  token_id_str_KEYWORD_VIAPORT[]       = "KEYWORD_VIAPORT";
   static const char  token_id_str_KEYWORD_WITHINTERFACE[] = "KEYWORD_WITHINTERFACE";
   static const char  token_id_str_NEGATION[]              = "NEGATION";
   static const char  token_id_str_PQ_STRING[]             = "PQ_STRING";
   static const char  token_id_str_PQ_STRING_BEGIN[]       = "PQ_STRING_BEGIN";
   static const char  token_id_str_PQ_STRING_END[]         = "PQ_STRING_END";
   static const char  token_id_str_SET_OPERATOR[]          = "SET_OPERATOR";
       

   /* NOTE: This implementation works only for token id types that are 
    *       some type of integer or enum. In case an alien type is to
    *       used, this function needs to be redefined.                  */
   switch( TokenID ) {
   default: {
       __QUEX_STD_sprintf(error_string, "<UNKNOWN TOKEN-ID: %i>", (int)TokenID);
       return error_string;
   }
   case RULE_TOKEN_TERMINATION:    return termination_string;
   case RULE_TOKEN_UNINITIALIZED:  return uninitialized_string;
#  if defined(QUEX_OPTION_INDENTATION_TRIGGER)
   case RULE_TOKEN_INDENT:         return indent_string;
   case RULE_TOKEN_DEDENT:         return dedent_string;
   case RULE_TOKEN_NODENT:         return nodent_string;
#  endif
   case RULE_TOKEN_ASTERISK:              return token_id_str_ASTERISK;
   case RULE_TOKEN_COLON:                 return token_id_str_COLON;
   case RULE_TOKEN_CONDITION_IDENTIFIER:  return token_id_str_CONDITION_IDENTIFIER;
   case RULE_TOKEN_CURLYBRACE_CLOSE:      return token_id_str_CURLYBRACE_CLOSE;
   case RULE_TOKEN_CURLYBRACE_OPEN:       return token_id_str_CURLYBRACE_OPEN;
   case RULE_TOKEN_DQ_STRING:             return token_id_str_DQ_STRING;
   case RULE_TOKEN_DQ_STRING_BEGIN:       return token_id_str_DQ_STRING_BEGIN;
   case RULE_TOKEN_DQ_STRING_END:         return token_id_str_DQ_STRING_END;
   case RULE_TOKEN_HEXCHAR2:              return token_id_str_HEXCHAR2;
   case RULE_TOKEN_HEXCHAR4:              return token_id_str_HEXCHAR4;
   case RULE_TOKEN_KEYWORD_ALLOW:         return token_id_str_KEYWORD_ALLOW;
   case RULE_TOKEN_KEYWORD_BLOCK:         return token_id_str_KEYWORD_BLOCK;
   case RULE_TOKEN_KEYWORD_DEVICE:        return token_id_str_KEYWORD_DEVICE;
   case RULE_TOKEN_KEYWORD_HASH:          return token_id_str_KEYWORD_HASH;
   case RULE_TOKEN_KEYWORD_IF:            return token_id_str_KEYWORD_IF;
   case RULE_TOKEN_KEYWORD_MATCH:         return token_id_str_KEYWORD_MATCH;
   case RULE_TOKEN_KEYWORD_NAME:          return token_id_str_KEYWORD_NAME;
   case RULE_TOKEN_KEYWORD_REJECT:        return token_id_str_KEYWORD_REJECT;
   case RULE_TOKEN_KEYWORD_SERIAL:        return token_id_str_KEYWORD_SERIAL;
   case RULE_TOKEN_KEYWORD_VIAPORT:       return token_id_str_KEYWORD_VIAPORT;
   case RULE_TOKEN_KEYWORD_WITHINTERFACE: return token_id_str_KEYWORD_WITHINTERFACE;
   case RULE_TOKEN_NEGATION:              return token_id_str_NEGATION;
   case RULE_TOKEN_PQ_STRING:             return token_id_str_PQ_STRING;
   case RULE_TOKEN_PQ_STRING_BEGIN:       return token_id_str_PQ_STRING_BEGIN;
   case RULE_TOKEN_PQ_STRING_END:         return token_id_str_PQ_STRING_END;
   case RULE_TOKEN_SET_OPERATOR:          return token_id_str_SET_OPERATOR;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

