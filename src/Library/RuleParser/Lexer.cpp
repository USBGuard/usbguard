
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

    /* (806 from BEFORE_ENTRY)  */
    input = *(me->buffer._read_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._read_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(806);
if     ( input >= 0xB )  goto _4;
else if( input == 0xA )  goto _2;
else if( input == 0x9 )  goto _3;
else                     goto _4;


    __quex_assert_no_passage();
_8:
    /* (806 from 810)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 808)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 807)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 809)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (808 from 806)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(808);
goto _6;


    __quex_assert_no_passage();
_3:
    /* (809 from 806)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(809);
goto _7;


    __quex_assert_no_passage();
_4:
    /* (807 from 806)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(807);
goto _5;

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
    /* (776 from RELOAD_FORWARD) (776 from BEFORE_ENTRY)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(776);
switch( input ) {
case 0x0: goto _21;
case 0x1: 
case 0xA: goto _12;
case 0x29: goto _6;
case 0x5C: goto _4;
default: goto _5;
}


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 777) (DROP_OUT from 778)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 784)  */
    goto _34;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 781)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 780)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 779)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 776)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 783)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 782)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (777 from 776)  */
    ++(me->buffer._read_p);

_39:
    input = *(me->buffer._read_p);


    __quex_debug_state(777);
switch( input ) {
case 0x0: goto _23;
case 0x28: goto _7;
case 0x5C: goto _8;
case 0x78: goto _9;
default: goto _13;
}


    __quex_assert_no_passage();
_22:
    /* (777 from RELOAD_FORWARD)  */
    goto _39;


    __quex_assert_no_passage();
_5:
    /* (778 from 776)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(778);
goto _13;


    __quex_assert_no_passage();
_6:
    /* (779 from 776)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(779);
goto _14;


    __quex_assert_no_passage();
_7:
    /* (780 from 777)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(780);
goto _15;


    __quex_assert_no_passage();
_8:
    /* (781 from 777)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(781);
goto _16;


    __quex_assert_no_passage();
_24:
    /* (782 from RELOAD_FORWARD)  */
_45:
    input = *(me->buffer._read_p);


    __quex_debug_state(782);
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
    /* (782 from 777)  */
    ++(me->buffer._read_p);

    goto _45;


    __quex_assert_no_passage();
_10:
    /* (783 from 782)  */
    ++(me->buffer._read_p);

_47:
    input = *(me->buffer._read_p);


    __quex_debug_state(783);
switch( input ) {
case 0x0: goto _27;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _11;
default: goto _18;
}


    __quex_assert_no_passage();
_26:
    /* (783 from RELOAD_FORWARD)  */
    goto _47;


    __quex_assert_no_passage();
_11:
    /* (784 from 783)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(784);
goto _19;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_20:
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
_36:
    __quex_debug("* TERMINAL \"\\\\(\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 130 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 800 "Lexer.cpp"

}
goto _0;
_35:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 133 "Lexer.qx"
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

#   line 136 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 830 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \")\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 141 "Lexer.qx"
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

#   line 143 "Lexer.qx"
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
    /* (RELOAD_FORWARD from 782)  */
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
    /* (RELOAD_FORWARD from 776)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(20);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 783)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(18);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 777)  */
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
#   define QUEX_LABEL_STATE_ROUTER _912
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
_911:
    me->buffer._lexeme_start_p = me->buffer._read_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);
_3:
    /* (1407 from BEFORE_ENTRY) (1407 from RELOAD_FORWARD)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(1407);
switch( input ) {
case 0x0: goto _327;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _74;
case 0x21: goto _67;
case 0x22: goto _69;
case 0x28: goto _71;
case 0x2A: goto _66;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _75;
case 0x61: goto _76;
case 0x62: goto _59;
case 0x63: goto _75;
case 0x64: goto _61;
case 0x65: goto _72;
case 0x66: goto _78;
case 0x68: goto _79;
case 0x69: goto _77;
case 0x6C: goto _70;
case 0x6D: goto _63;
case 0x6E: goto _80;
case 0x6F: goto _58;
case 0x72: goto _62;
case 0x73: goto _73;
case 0x74: goto _57;
case 0x76: goto _60;
case 0x77: goto _68;
case 0x7B: goto _64;
case 0x7D: goto _65;
default: goto _185;
}


    __quex_assert_no_passage();
_185:
    /* (DROP_OUT from 1447) (DROP_OUT from 1429) (DROP_OUT from 1526) (DROP_OUT from 1444) (DROP_OUT from 1426) (DROP_OUT from 1523) (DROP_OUT from 1441) (DROP_OUT from 1439) (DROP_OUT from 1520) (DROP_OUT from 1436) (DROP_OUT from 1533) (DROP_OUT from 1451) (DROP_OUT from 1433) (DROP_OUT from 1573) (DROP_OUT from 1555) (DROP_OUT from 1448) (DROP_OUT from 1414) (DROP_OUT from 1570) (DROP_OUT from 1552) (DROP_OUT from 1511) (DROP_OUT from 1493) (DROP_OUT from 1411) (DROP_OUT from 1583) (DROP_OUT from 1565) (DROP_OUT from 1508) (DROP_OUT from 1490) (DROP_OUT from 1408) (DROP_OUT from 1562) (DROP_OUT from 1421) (DROP_OUT from 1577) (DROP_OUT from 1543) (DROP_OUT from 1518) (DROP_OUT from 1540) (DROP_OUT from 1497) (DROP_OUT from 1537) (DROP_OUT from 1512) (DROP_OUT from 1460) (DROP_OUT from 1550) (DROP_OUT from 1457) (DROP_OUT from 1547) (DROP_OUT from 1472) (DROP_OUT from 1470) (DROP_OUT from 1544) (DROP_OUT from 1464) (DROP_OUT from 1430) (DROP_OUT from 1586) (DROP_OUT from 1527) (DROP_OUT from 1445) (DROP_OUT from 1427) (DROP_OUT from 1524) (DROP_OUT from 1442) (DROP_OUT from 1424) (DROP_OUT from 1521) (DROP_OUT from 1455) (DROP_OUT from 1437) (DROP_OUT from 1559) (DROP_OUT from 1452) (DROP_OUT from 1434) (DROP_OUT from 1574) (DROP_OUT from 1556) (DROP_OUT from 1531) (DROP_OUT from 1449) (DROP_OUT from 1571) (DROP_OUT from 1553) (DROP_OUT from 1412) (DROP_OUT from 1568) (DROP_OUT from 1566) (DROP_OUT from 1509) (DROP_OUT from 1491) (DROP_OUT from 1409) (DROP_OUT from 1581) (DROP_OUT from 1563) (DROP_OUT from 1488) (DROP_OUT from 1578) (DROP_OUT from 1560) (DROP_OUT from 1519) (DROP_OUT from 1419) (DROP_OUT from 1516) (DROP_OUT from 1498) (DROP_OUT from 1538) (DROP_OUT from 1513) (DROP_OUT from 1551) (DROP_OUT from 1458) (DROP_OUT from 1548) (DROP_OUT from 1471) (DROP_OUT from 1486) (DROP_OUT from 1468) (DROP_OUT from 1465) (DROP_OUT from 1431) (DROP_OUT from 1587) (DROP_OUT from 1446) (DROP_OUT from 1428) (DROP_OUT from 1584) (DROP_OUT from 1525) (DROP_OUT from 1522) (DROP_OUT from 1535) (DROP_OUT from 1453) (DROP_OUT from 1435) (DROP_OUT from 1575) (DROP_OUT from 1557) (DROP_OUT from 1532) (DROP_OUT from 1432) (DROP_OUT from 1572) (DROP_OUT from 1554) (DROP_OUT from 1529) (DROP_OUT from 1495) (DROP_OUT from 1413) (DROP_OUT from 1567) (DROP_OUT from 1510) (DROP_OUT from 1492) (DROP_OUT from 1410) (DROP_OUT from 1582) (DROP_OUT from 1423) (DROP_OUT from 1579) (DROP_OUT from 1561) (DROP_OUT from 1542) (DROP_OUT from 1517) (DROP_OUT from 1417) (DROP_OUT from 1539) (DROP_OUT from 1514) (DROP_OUT from 1496) (DROP_OUT from 1462) (DROP_OUT from 1536) (DROP_OUT from 1549) (DROP_OUT from 1456) (DROP_OUT from 1546) (DROP_OUT from 1487) (DROP_OUT from 1469) (DROP_OUT from 1484) (DROP_OUT from 1407)  */

        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _540;
_538:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_212:
    /* (DROP_OUT from 1483)  */
    goto _541;


    __quex_assert_no_passage();
_232:
    /* (DROP_OUT from 1564)  */
    goto _542;


    __quex_assert_no_passage();
_199:
    /* (DROP_OUT from 1463)  */
    goto _544;


    __quex_assert_no_passage();
_192:
    /* (DROP_OUT from 1438)  */
    goto _545;


    __quex_assert_no_passage();
_234:
    /* (DROP_OUT from 1576)  */
    goto _546;


    __quex_assert_no_passage();
_228:
    /* (DROP_OUT from 1534)  */
    goto _547;


    __quex_assert_no_passage();
_201:
    /* (DROP_OUT from 1467)  */
    goto _548;


    __quex_assert_no_passage();
_195:
    /* (DROP_OUT from 1450)  */
    goto _549;


    __quex_assert_no_passage();
_188:
    /* (DROP_OUT from 1418)  */
    goto _551;


    __quex_assert_no_passage();
_213:
    /* (DROP_OUT from 1485)  */
    goto _552;


    __quex_assert_no_passage();
_200:
    /* (DROP_OUT from 1466)  */
    goto _553;


    __quex_assert_no_passage();
_235:
    /* (DROP_OUT from 1580)  */
    goto _554;


    __quex_assert_no_passage();
_229:
    /* (DROP_OUT from 1541)  */
    goto _555;


    __quex_assert_no_passage();
_202:
    /* (DROP_OUT from 1473)  */
    goto _543;


    __quex_assert_no_passage();
_196:
    /* (DROP_OUT from 1454)  */
    goto _556;


    __quex_assert_no_passage();
_189:
    /* (DROP_OUT from 1420)  */
    goto _557;


    __quex_assert_no_passage();
_225:
    /* (DROP_OUT from 1515)  */
    goto _558;


    __quex_assert_no_passage();
_214:
    /* (DROP_OUT from 1489)  */
    goto _559;


    __quex_assert_no_passage();
_236:
    /* (DROP_OUT from 1585)  */
    goto _560;


    __quex_assert_no_passage();
_230:
    /* (DROP_OUT from 1545)  */
    goto _561;


    __quex_assert_no_passage();
_197:
    /* (DROP_OUT from 1459)  */
    goto _562;


    __quex_assert_no_passage();
_190:
    /* (DROP_OUT from 1422)  */
    goto _563;


    __quex_assert_no_passage();
_226:
    /* (DROP_OUT from 1528)  */
    goto _564;


    __quex_assert_no_passage();
_215:
    /* (DROP_OUT from 1494)  */
    goto _565;


    __quex_assert_no_passage();
_193:
    /* (DROP_OUT from 1440)  */
    goto _566;


    __quex_assert_no_passage();
_186:
    /* (DROP_OUT from 1415)  */
    goto _567;


    __quex_assert_no_passage();
_237:
    /* (DROP_OUT from 1588)  */
    goto _568;


    __quex_assert_no_passage();
_231:
    /* (DROP_OUT from 1558)  */
    goto _569;


    __quex_assert_no_passage();
_198:
    /* (DROP_OUT from 1461)  */
    goto _570;


    __quex_assert_no_passage();
_224:
    /* (DROP_OUT from 1507)  */
    goto _571;


    __quex_assert_no_passage();
_191:
    /* (DROP_OUT from 1425)  */
    goto _572;


    __quex_assert_no_passage();
_233:
    /* (DROP_OUT from 1569)  */
    goto _573;


    __quex_assert_no_passage();
_227:
    /* (DROP_OUT from 1530)  */
    goto _574;


    __quex_assert_no_passage();
_216:
    /* (DROP_OUT from 1499)  */
    goto _550;


    __quex_assert_no_passage();
_194:
    /* (DROP_OUT from 1443)  */
    goto _575;


    __quex_assert_no_passage();
_187:
    /* (DROP_OUT from 1416)  */
    goto _576;


    __quex_assert_no_passage();
_205:
    /* (DROP_OUT from 1476)  */
        me->buffer._read_p -= 3;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_221:
    /* (DROP_OUT from 1504)  */
        me->buffer._read_p -= 5;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_217:
    /* (DROP_OUT from 1500)  */
        me->buffer._read_p -= 1;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_206:
    /* (DROP_OUT from 1477)  */
        me->buffer._read_p -= 4;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_209:
    /* (DROP_OUT from 1480)  */
        me->buffer._read_p -= 7;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_222:
    /* (DROP_OUT from 1505)  */
        me->buffer._read_p -= 6;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_218:
    /* (DROP_OUT from 1501)  */
        me->buffer._read_p -= 2;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_207:
    /* (DROP_OUT from 1478)  */
        me->buffer._read_p -= 5;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_210:
    /* (DROP_OUT from 1481)  */
        me->buffer._read_p -= 8;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_203:
    /* (DROP_OUT from 1474)  */
        me->buffer._read_p -= 1;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_223:
    /* (DROP_OUT from 1506)  */
        me->buffer._read_p -= 7;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_219:
    /* (DROP_OUT from 1502)  */
        me->buffer._read_p -= 3;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_208:
    /* (DROP_OUT from 1479)  */
        me->buffer._read_p -= 6;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_211:
    /* (DROP_OUT from 1482)  */
        me->buffer._read_p -= 9;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_204:
    /* (DROP_OUT from 1475)  */
        me->buffer._read_p -= 2;
goto _543;
    goto _538;


    __quex_assert_no_passage();
_220:
    /* (DROP_OUT from 1503)  */
        me->buffer._read_p -= 4;
goto _550;
    goto _538;


    __quex_assert_no_passage();
_238:
    /* (1536 from RELOAD_FORWARD)  */
_578:
    input = *(me->buffer._read_p);


    __quex_debug_state(1536);
if     ( input == 0x6A )  goto _10;
else if( input == 0x0 )   goto _239;
else                      goto _185;


    __quex_assert_no_passage();
_4:
    /* (1536 from 1413)  */
    ++(me->buffer._read_p);

    goto _578;


    __quex_assert_no_passage();
_5:
    /* (1537 from 1413)  */
    ++(me->buffer._read_p);

_580:
    input = *(me->buffer._read_p);


    __quex_debug_state(1537);
if     ( input == 0x6E )  goto _6;
else if( input == 0x0 )   goto _241;
else                      goto _185;


    __quex_assert_no_passage();
_240:
    /* (1537 from RELOAD_FORWARD)  */
    goto _580;


    __quex_assert_no_passage();
_6:
    /* (1538 from 1537)  */
    ++(me->buffer._read_p);

_582:
    input = *(me->buffer._read_p);


    __quex_debug_state(1538);
if     ( input == 0x64 )  goto _7;
else if( input == 0x0 )   goto _243;
else                      goto _185;


    __quex_assert_no_passage();
_242:
    /* (1538 from RELOAD_FORWARD)  */
    goto _582;


    __quex_assert_no_passage();
_7:
    /* (1539 from 1538)  */
    ++(me->buffer._read_p);

_584:
    input = *(me->buffer._read_p);


    __quex_debug_state(1539);
if     ( input == 0x6F )  goto _8;
else if( input == 0x0 )   goto _245;
else                      goto _185;


    __quex_assert_no_passage();
_244:
    /* (1539 from RELOAD_FORWARD)  */
    goto _584;


    __quex_assert_no_passage();
_246:
    /* (1540 from RELOAD_FORWARD)  */
_586:
    input = *(me->buffer._read_p);


    __quex_debug_state(1540);
if     ( input == 0x6D )  goto _9;
else if( input == 0x0 )   goto _247;
else                      goto _185;


    __quex_assert_no_passage();
_8:
    /* (1540 from 1539)  */
    ++(me->buffer._read_p);

    goto _586;


    __quex_assert_no_passage();
_9:
    /* (1541 from 1540)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1541);
goto _229;


    __quex_assert_no_passage();
_248:
    /* (1542 from RELOAD_FORWARD)  */
_589:
    input = *(me->buffer._read_p);


    __quex_debug_state(1542);
if     ( input == 0x65 )  goto _11;
else if( input == 0x0 )   goto _249;
else                      goto _185;


    __quex_assert_no_passage();
_10:
    /* (1542 from 1536)  */
    ++(me->buffer._read_p);

    goto _589;


    __quex_assert_no_passage();
_11:
    /* (1543 from 1542)  */
    ++(me->buffer._read_p);

_591:
    input = *(me->buffer._read_p);


    __quex_debug_state(1543);
if     ( input == 0x63 )  goto _12;
else if( input == 0x0 )   goto _251;
else                      goto _185;


    __quex_assert_no_passage();
_250:
    /* (1543 from RELOAD_FORWARD)  */
    goto _591;


    __quex_assert_no_passage();
_252:
    /* (1544 from RELOAD_FORWARD)  */
_593:
    input = *(me->buffer._read_p);


    __quex_debug_state(1544);
if     ( input == 0x74 )  goto _13;
else if( input == 0x0 )   goto _253;
else                      goto _185;


    __quex_assert_no_passage();
_12:
    /* (1544 from 1543)  */
    ++(me->buffer._read_p);

    goto _593;


    __quex_assert_no_passage();
_13:
    /* (1545 from 1544)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1545);
goto _230;


    __quex_assert_no_passage();
_14:
    /* (1546 from 1535)  */
    ++(me->buffer._read_p);

_596:
    input = *(me->buffer._read_p);


    __quex_debug_state(1546);
if     ( input == 0x65 )  goto _15;
else if( input == 0x0 )   goto _255;
else                      goto _185;


    __quex_assert_no_passage();
_254:
    /* (1546 from RELOAD_FORWARD)  */
    goto _596;


    __quex_assert_no_passage();
_15:
    /* (1547 from 1546)  */
    ++(me->buffer._read_p);

_598:
    input = *(me->buffer._read_p);


    __quex_debug_state(1547);
if     ( input == 0x2D )  goto _16;
else if( input == 0x0 )   goto _257;
else                      goto _185;


    __quex_assert_no_passage();
_256:
    /* (1547 from RELOAD_FORWARD)  */
    goto _598;


    __quex_assert_no_passage();
_258:
    /* (1548 from RELOAD_FORWARD)  */
_600:
    input = *(me->buffer._read_p);


    __quex_debug_state(1548);
switch( input ) {
case 0x0: goto _259;
case 0x61: goto _17;
case 0x65: goto _18;
default: goto _185;
}


    __quex_assert_no_passage();
_16:
    /* (1548 from 1547)  */
    ++(me->buffer._read_p);

    goto _600;


    __quex_assert_no_passage();
_260:
    /* (1549 from RELOAD_FORWARD)  */
_602:
    input = *(me->buffer._read_p);


    __quex_debug_state(1549);
if     ( input == 0x70 )  goto _27;
else if( input == 0x0 )   goto _261;
else                      goto _185;


    __quex_assert_no_passage();
_17:
    /* (1549 from 1548)  */
    ++(me->buffer._read_p);

    goto _602;


    __quex_assert_no_passage();
_262:
    /* (1550 from RELOAD_FORWARD)  */
_604:
    input = *(me->buffer._read_p);


    __quex_debug_state(1550);
if     ( input == 0x76 )  goto _19;
else if( input == 0x0 )   goto _263;
else                      goto _185;


    __quex_assert_no_passage();
_18:
    /* (1550 from 1548)  */
    ++(me->buffer._read_p);

    goto _604;


    __quex_assert_no_passage();
_19:
    /* (1551 from 1550)  */
    ++(me->buffer._read_p);

_606:
    input = *(me->buffer._read_p);


    __quex_debug_state(1551);
if     ( input == 0x61 )  goto _20;
else if( input == 0x0 )   goto _265;
else                      goto _185;


    __quex_assert_no_passage();
_264:
    /* (1551 from RELOAD_FORWARD)  */
    goto _606;


    __quex_assert_no_passage();
_266:
    /* (1552 from RELOAD_FORWARD)  */
_608:
    input = *(me->buffer._read_p);


    __quex_debug_state(1552);
if     ( input == 0x6C )  goto _21;
else if( input == 0x0 )   goto _267;
else                      goto _185;


    __quex_assert_no_passage();
_20:
    /* (1552 from 1551)  */
    ++(me->buffer._read_p);

    goto _608;


    __quex_assert_no_passage();
_21:
    /* (1553 from 1552)  */
    ++(me->buffer._read_p);

_610:
    input = *(me->buffer._read_p);


    __quex_debug_state(1553);
if     ( input == 0x75 )  goto _22;
else if( input == 0x0 )   goto _269;
else                      goto _185;


    __quex_assert_no_passage();
_268:
    /* (1553 from RELOAD_FORWARD)  */
    goto _610;


    __quex_assert_no_passage();
_22:
    /* (1554 from 1553)  */
    ++(me->buffer._read_p);

_612:
    input = *(me->buffer._read_p);


    __quex_debug_state(1554);
if     ( input == 0x61 )  goto _23;
else if( input == 0x0 )   goto _271;
else                      goto _185;


    __quex_assert_no_passage();
_270:
    /* (1554 from RELOAD_FORWARD)  */
    goto _612;


    __quex_assert_no_passage();
_23:
    /* (1555 from 1554)  */
    ++(me->buffer._read_p);

_614:
    input = *(me->buffer._read_p);


    __quex_debug_state(1555);
if     ( input == 0x74 )  goto _24;
else if( input == 0x0 )   goto _273;
else                      goto _185;


    __quex_assert_no_passage();
_272:
    /* (1555 from RELOAD_FORWARD)  */
    goto _614;


    __quex_assert_no_passage();
_274:
    /* (1556 from RELOAD_FORWARD)  */
_616:
    input = *(me->buffer._read_p);


    __quex_debug_state(1556);
if     ( input == 0x65 )  goto _25;
else if( input == 0x0 )   goto _275;
else                      goto _185;


    __quex_assert_no_passage();
_24:
    /* (1556 from 1555)  */
    ++(me->buffer._read_p);

    goto _616;


    __quex_assert_no_passage();
_276:
    /* (1557 from RELOAD_FORWARD)  */
_618:
    input = *(me->buffer._read_p);


    __quex_debug_state(1557);
if     ( input == 0x64 )  goto _26;
else if( input == 0x0 )   goto _277;
else                      goto _185;


    __quex_assert_no_passage();
_25:
    /* (1557 from 1556)  */
    ++(me->buffer._read_p);

    goto _618;


    __quex_assert_no_passage();
_26:
    /* (1558 from 1557)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1558);
goto _231;


    __quex_assert_no_passage();
_27:
    /* (1559 from 1549)  */
    ++(me->buffer._read_p);

_621:
    input = *(me->buffer._read_p);


    __quex_debug_state(1559);
if     ( input == 0x70 )  goto _28;
else if( input == 0x0 )   goto _279;
else                      goto _185;


    __quex_assert_no_passage();
_278:
    /* (1559 from RELOAD_FORWARD)  */
    goto _621;


    __quex_assert_no_passage();
_280:
    /* (1560 from RELOAD_FORWARD)  */
_623:
    input = *(me->buffer._read_p);


    __quex_debug_state(1560);
if     ( input == 0x6C )  goto _29;
else if( input == 0x0 )   goto _281;
else                      goto _185;


    __quex_assert_no_passage();
_28:
    /* (1560 from 1559)  */
    ++(me->buffer._read_p);

    goto _623;


    __quex_assert_no_passage();
_29:
    /* (1561 from 1560)  */
    ++(me->buffer._read_p);

_625:
    input = *(me->buffer._read_p);


    __quex_debug_state(1561);
if     ( input == 0x69 )  goto _30;
else if( input == 0x0 )   goto _283;
else                      goto _185;


    __quex_assert_no_passage();
_282:
    /* (1561 from RELOAD_FORWARD)  */
    goto _625;


    __quex_assert_no_passage();
_30:
    /* (1562 from 1561)  */
    ++(me->buffer._read_p);

_627:
    input = *(me->buffer._read_p);


    __quex_debug_state(1562);
if     ( input == 0x65 )  goto _31;
else if( input == 0x0 )   goto _285;
else                      goto _185;


    __quex_assert_no_passage();
_284:
    /* (1562 from RELOAD_FORWARD)  */
    goto _627;


    __quex_assert_no_passage();
_31:
    /* (1563 from 1562)  */
    ++(me->buffer._read_p);

_629:
    input = *(me->buffer._read_p);


    __quex_debug_state(1563);
if     ( input == 0x64 )  goto _32;
else if( input == 0x0 )   goto _287;
else                      goto _185;


    __quex_assert_no_passage();
_286:
    /* (1563 from RELOAD_FORWARD)  */
    goto _629;


    __quex_assert_no_passage();
_32:
    /* (1564 from 1563)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1564);
goto _232;


    __quex_assert_no_passage();
_288:
    /* (1565 from RELOAD_FORWARD)  */
_632:
    input = *(me->buffer._read_p);


    __quex_debug_state(1565);
switch( input ) {
case 0x0: goto _289;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _96;
case 0x3A: goto _95;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _96;
case 0x76: goto _34;
default: goto _185;
}


    __quex_assert_no_passage();
_33:
    /* (1565 from 1412)  */
    ++(me->buffer._read_p);

    goto _632;


    __quex_assert_no_passage();
_290:
    /* (1566 from RELOAD_FORWARD)  */
_634:
    input = *(me->buffer._read_p);


    __quex_debug_state(1566);
if     ( input == 0x69 )  goto _35;
else if( input == 0x0 )   goto _291;
else                      goto _185;


    __quex_assert_no_passage();
_34:
    /* (1566 from 1565)  */
    ++(me->buffer._read_p);

    goto _634;


    __quex_assert_no_passage();
_35:
    /* (1567 from 1566)  */
    ++(me->buffer._read_p);

_636:
    input = *(me->buffer._read_p);


    __quex_debug_state(1567);
if     ( input == 0x63 )  goto _36;
else if( input == 0x0 )   goto _293;
else                      goto _185;


    __quex_assert_no_passage();
_292:
    /* (1567 from RELOAD_FORWARD)  */
    goto _636;


    __quex_assert_no_passage();
_294:
    /* (1568 from RELOAD_FORWARD)  */
_638:
    input = *(me->buffer._read_p);


    __quex_debug_state(1568);
if     ( input == 0x65 )  goto _37;
else if( input == 0x0 )   goto _295;
else                      goto _185;


    __quex_assert_no_passage();
_36:
    /* (1568 from 1567)  */
    ++(me->buffer._read_p);

    goto _638;


    __quex_assert_no_passage();
_37:
    /* (1569 from 1568)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1569);
goto _233;


    __quex_assert_no_passage();
_38:
    /* (1570 from 1411)  */
    ++(me->buffer._read_p);

_641:
    input = *(me->buffer._read_p);


    __quex_debug_state(1570);
if     ( input == 0x61 )  goto _39;
else if( input == 0x0 )   goto _297;
else                      goto _185;


    __quex_assert_no_passage();
_296:
    /* (1570 from RELOAD_FORWARD)  */
    goto _641;


    __quex_assert_no_passage();
_39:
    /* (1571 from 1570)  */
    ++(me->buffer._read_p);

_643:
    input = *(me->buffer._read_p);


    __quex_debug_state(1571);
if     ( input == 0x2D )  goto _40;
else if( input == 0x0 )   goto _299;
else                      goto _185;


    __quex_assert_no_passage();
_298:
    /* (1571 from RELOAD_FORWARD)  */
    goto _643;


    __quex_assert_no_passage();
_300:
    /* (1572 from RELOAD_FORWARD)  */
_645:
    input = *(me->buffer._read_p);


    __quex_debug_state(1572);
if     ( input == 0x70 )  goto _41;
else if( input == 0x0 )   goto _301;
else                      goto _185;


    __quex_assert_no_passage();
_40:
    /* (1572 from 1571)  */
    ++(me->buffer._read_p);

    goto _645;


    __quex_assert_no_passage();
_302:
    /* (1573 from RELOAD_FORWARD)  */
_647:
    input = *(me->buffer._read_p);


    __quex_debug_state(1573);
if     ( input == 0x6F )  goto _42;
else if( input == 0x0 )   goto _303;
else                      goto _185;


    __quex_assert_no_passage();
_41:
    /* (1573 from 1572)  */
    ++(me->buffer._read_p);

    goto _647;


    __quex_assert_no_passage();
_304:
    /* (1574 from RELOAD_FORWARD)  */
_649:
    input = *(me->buffer._read_p);


    __quex_debug_state(1574);
if     ( input == 0x72 )  goto _43;
else if( input == 0x0 )   goto _305;
else                      goto _185;


    __quex_assert_no_passage();
_42:
    /* (1574 from 1573)  */
    ++(me->buffer._read_p);

    goto _649;


    __quex_assert_no_passage();
_43:
    /* (1575 from 1574)  */
    ++(me->buffer._read_p);

_651:
    input = *(me->buffer._read_p);


    __quex_debug_state(1575);
if     ( input == 0x74 )  goto _44;
else if( input == 0x0 )   goto _307;
else                      goto _185;


    __quex_assert_no_passage();
_306:
    /* (1575 from RELOAD_FORWARD)  */
    goto _651;


    __quex_assert_no_passage();
_44:
    /* (1576 from 1575)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1576);
goto _234;


    __quex_assert_no_passage();
_45:
    /* (1577 from 1410)  */
    ++(me->buffer._read_p);

_654:
    input = *(me->buffer._read_p);


    __quex_debug_state(1577);
if     ( input == 0x6F )  goto _46;
else if( input == 0x0 )   goto _309;
else                      goto _185;


    __quex_assert_no_passage();
_308:
    /* (1577 from RELOAD_FORWARD)  */
    goto _654;


    __quex_assert_no_passage();
_46:
    /* (1578 from 1577)  */
    ++(me->buffer._read_p);

_656:
    input = *(me->buffer._read_p);


    __quex_debug_state(1578);
if     ( input == 0x63 )  goto _47;
else if( input == 0x0 )   goto _311;
else                      goto _185;


    __quex_assert_no_passage();
_310:
    /* (1578 from RELOAD_FORWARD)  */
    goto _656;


    __quex_assert_no_passage();
_47:
    /* (1579 from 1578)  */
    ++(me->buffer._read_p);

_658:
    input = *(me->buffer._read_p);


    __quex_debug_state(1579);
if     ( input == 0x6B )  goto _48;
else if( input == 0x0 )   goto _313;
else                      goto _185;


    __quex_assert_no_passage();
_312:
    /* (1579 from RELOAD_FORWARD)  */
    goto _658;


    __quex_assert_no_passage();
_48:
    /* (1580 from 1579)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1580);
goto _235;


    __quex_assert_no_passage();
_314:
    /* (1581 from RELOAD_FORWARD)  */
_661:
    input = *(me->buffer._read_p);


    __quex_debug_state(1581);
if     ( input == 0x65 )  goto _50;
else if( input == 0x0 )   goto _315;
else                      goto _185;


    __quex_assert_no_passage();
_49:
    /* (1581 from 1409)  */
    ++(me->buffer._read_p);

    goto _661;


    __quex_assert_no_passage();
_316:
    /* (1582 from RELOAD_FORWARD)  */
_663:
    input = *(me->buffer._read_p);


    __quex_debug_state(1582);
if     ( input == 0x2D )  goto _51;
else if( input == 0x0 )   goto _317;
else                      goto _185;


    __quex_assert_no_passage();
_50:
    /* (1582 from 1581)  */
    ++(me->buffer._read_p);

    goto _663;


    __quex_assert_no_passage();
_51:
    /* (1583 from 1582)  */
    ++(me->buffer._read_p);

_665:
    input = *(me->buffer._read_p);


    __quex_debug_state(1583);
if     ( input == 0x6F )  goto _52;
else if( input == 0x0 )   goto _319;
else                      goto _185;


    __quex_assert_no_passage();
_318:
    /* (1583 from RELOAD_FORWARD)  */
    goto _665;


    __quex_assert_no_passage();
_320:
    /* (1584 from RELOAD_FORWARD)  */
_667:
    input = *(me->buffer._read_p);


    __quex_debug_state(1584);
if     ( input == 0x66 )  goto _53;
else if( input == 0x0 )   goto _321;
else                      goto _185;


    __quex_assert_no_passage();
_52:
    /* (1584 from 1583)  */
    ++(me->buffer._read_p);

    goto _667;


    __quex_assert_no_passage();
_53:
    /* (1585 from 1584)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1585);
goto _236;


    __quex_assert_no_passage();
_54:
    /* (1586 from 1408)  */
    ++(me->buffer._read_p);

_670:
    input = *(me->buffer._read_p);


    __quex_debug_state(1586);
if     ( input == 0x75 )  goto _55;
else if( input == 0x0 )   goto _323;
else                      goto _185;


    __quex_assert_no_passage();
_322:
    /* (1586 from RELOAD_FORWARD)  */
    goto _670;


    __quex_assert_no_passage();
_55:
    /* (1587 from 1586)  */
    ++(me->buffer._read_p);

_672:
    input = *(me->buffer._read_p);


    __quex_debug_state(1587);
if     ( input == 0x65 )  goto _56;
else if( input == 0x0 )   goto _325;
else                      goto _185;


    __quex_assert_no_passage();
_324:
    /* (1587 from RELOAD_FORWARD)  */
    goto _672;


    __quex_assert_no_passage();
_56:
    /* (1588 from 1587)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1588);
goto _237;


    __quex_assert_no_passage();
_328:
    /* (1408 from RELOAD_FORWARD)  */
_675:
    input = *(me->buffer._read_p);


    __quex_debug_state(1408);
if     ( input == 0x72 )  goto _54;
else if( input == 0x0 )   goto _329;
else                      goto _185;


    __quex_assert_no_passage();
_57:
    /* (1408 from 1407)  */
    ++(me->buffer._read_p);

    goto _675;


    __quex_assert_no_passage();
_58:
    /* (1409 from 1407)  */
    ++(me->buffer._read_p);

_677:
    input = *(me->buffer._read_p);


    __quex_debug_state(1409);
if     ( input == 0x6E )  goto _49;
else if( input == 0x0 )   goto _331;
else                      goto _185;


    __quex_assert_no_passage();
_330:
    /* (1409 from RELOAD_FORWARD)  */
    goto _677;


    __quex_assert_no_passage();
_59:
    /* (1410 from 1407)  */
    ++(me->buffer._read_p);

_679:
    input = *(me->buffer._read_p);


    __quex_debug_state(1410);
switch( input ) {
case 0x0: goto _333;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _94;
case 0x6C: goto _45;
default: goto _185;
}


    __quex_assert_no_passage();
_332:
    /* (1410 from RELOAD_FORWARD)  */
    goto _679;


    __quex_assert_no_passage();
_60:
    /* (1411 from 1407)  */
    ++(me->buffer._read_p);

_681:
    input = *(me->buffer._read_p);


    __quex_debug_state(1411);
if     ( input == 0x69 )  goto _38;
else if( input == 0x0 )   goto _335;
else                      goto _185;


    __quex_assert_no_passage();
_334:
    /* (1411 from RELOAD_FORWARD)  */
    goto _681;


    __quex_assert_no_passage();
_336:
    /* (1412 from RELOAD_FORWARD)  */
_683:
    input = *(me->buffer._read_p);


    __quex_debug_state(1412);
switch( input ) {
case 0x0: goto _337;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: goto _94;
case 0x65: goto _33;
case 0x66: goto _94;
default: goto _185;
}


    __quex_assert_no_passage();
_61:
    /* (1412 from 1407)  */
    ++(me->buffer._read_p);

    goto _683;


    __quex_assert_no_passage();
_338:
    /* (1413 from RELOAD_FORWARD)  */
_685:
    input = *(me->buffer._read_p);


    __quex_debug_state(1413);
switch( input ) {
case 0x0: goto _339;
case 0x61: goto _5;
case 0x65: goto _4;
case 0x75: goto _184;
default: goto _185;
}


    __quex_assert_no_passage();
_62:
    /* (1413 from 1407)  */
    ++(me->buffer._read_p);

    goto _685;


    __quex_assert_no_passage();
_340:
    /* (1414 from RELOAD_FORWARD)  */
_687:
    input = *(me->buffer._read_p);


    __quex_debug_state(1414);
if     ( input == 0x61 )  goto _180;
else if( input == 0x0 )   goto _341;
else                      goto _185;


    __quex_assert_no_passage();
_63:
    /* (1414 from 1407)  */
    ++(me->buffer._read_p);

    goto _687;


    __quex_assert_no_passage();
_64:
    /* (1415 from 1407)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1415);
goto _186;


    __quex_assert_no_passage();
_65:
    /* (1416 from 1407)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1416);
goto _187;


    __quex_assert_no_passage();
_66:
    /* (1417 from 1407)  */
    ++(me->buffer._read_p);

_691:
    input = *(me->buffer._read_p);


    __quex_debug_state(1417);
if     ( input == 0x3A )  goto _178;
else if( input == 0x0 )   goto _343;
else                      goto _185;


    __quex_assert_no_passage();
_342:
    /* (1417 from RELOAD_FORWARD)  */
    goto _691;


    __quex_assert_no_passage();
_67:
    /* (1418 from 1407)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1418);
goto _188;


    __quex_assert_no_passage();
_68:
    /* (1419 from 1407)  */
    ++(me->buffer._read_p);

_694:
    input = *(me->buffer._read_p);


    __quex_debug_state(1419);
if     ( input == 0x69 )  goto _165;
else if( input == 0x0 )   goto _345;
else                      goto _185;


    __quex_assert_no_passage();
_344:
    /* (1419 from RELOAD_FORWARD)  */
    goto _694;


    __quex_assert_no_passage();
_69:
    /* (1420 from 1407)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1420);
goto _189;


    __quex_assert_no_passage();
_346:
    /* (1421 from RELOAD_FORWARD)  */
_697:
    input = *(me->buffer._read_p);


    __quex_debug_state(1421);
if     ( input == 0x6F )  goto _157;
else if( input == 0x0 )   goto _347;
else                      goto _185;


    __quex_assert_no_passage();
_70:
    /* (1421 from 1407)  */
    ++(me->buffer._read_p);

    goto _697;


    __quex_assert_no_passage();
_71:
    /* (1422 from 1407)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1422);
goto _190;


    __quex_assert_no_passage();
_72:
    /* (1423 from 1407)  */
    ++(me->buffer._read_p);

_700:
    input = *(me->buffer._read_p);


    __quex_debug_state(1423);
switch( input ) {
case 0x0: goto _349;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _94;
case 0x71: goto _144;
default: goto _185;
}


    __quex_assert_no_passage();
_348:
    /* (1423 from RELOAD_FORWARD)  */
    goto _700;


    __quex_assert_no_passage();
_350:
    /* (1424 from RELOAD_FORWARD)  */
_702:
    input = *(me->buffer._read_p);


    __quex_debug_state(1424);
if     ( input == 0x65 )  goto _139;
else if( input == 0x0 )   goto _351;
else                      goto _185;


    __quex_assert_no_passage();
_73:
    /* (1424 from 1407)  */
    ++(me->buffer._read_p);

    goto _702;


    __quex_assert_no_passage();
_74:
    /* (1425 from 1425) (1425 from 1407)  */

    ++(me->buffer._read_p);

_705:
    input = *(me->buffer._read_p);


    __quex_debug_state(1425);
switch( input ) {
case 0x0: goto _353;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _74;
default: goto _191;
}


    __quex_assert_no_passage();
_352:
    /* (1425 from RELOAD_FORWARD)  */
    goto _705;


    __quex_assert_no_passage();
_75:
    /* (1426 from 1407)  */
    ++(me->buffer._read_p);

_707:
    input = *(me->buffer._read_p);


    __quex_debug_state(1426);
switch( input ) {
case 0x0: goto _355;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _94;
default: goto _185;
}


    __quex_assert_no_passage();
_354:
    /* (1426 from RELOAD_FORWARD)  */
    goto _707;


    __quex_assert_no_passage();
_76:
    /* (1427 from 1407)  */
    ++(me->buffer._read_p);

_709:
    input = *(me->buffer._read_p);


    __quex_debug_state(1427);
switch( input ) {
case 0x0: goto _357;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _94;
case 0x6C: goto _118;
case 0x6E: goto _117;
default: goto _185;
}


    __quex_assert_no_passage();
_356:
    /* (1427 from RELOAD_FORWARD)  */
    goto _709;


    __quex_assert_no_passage();
_358:
    /* (1428 from RELOAD_FORWARD)  */
_711:
    input = *(me->buffer._read_p);


    __quex_debug_state(1428);
if     ( input == 0x66 )  goto _116;
else if( input == 0x0 )   goto _359;
else                      goto _185;


    __quex_assert_no_passage();
_77:
    /* (1428 from 1407)  */
    ++(me->buffer._read_p);

    goto _711;


    __quex_assert_no_passage();
_360:
    /* (1429 from RELOAD_FORWARD)  */
_713:
    input = *(me->buffer._read_p);


    __quex_debug_state(1429);
switch( input ) {
case 0x0: goto _361;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _94;
case 0x61: goto _93;
case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _94;
default: goto _185;
}


    __quex_assert_no_passage();
_78:
    /* (1429 from 1407)  */
    ++(me->buffer._read_p);

    goto _713;


    __quex_assert_no_passage();
_362:
    /* (1430 from RELOAD_FORWARD)  */
_715:
    input = *(me->buffer._read_p);


    __quex_debug_state(1430);
if     ( input == 0x61 )  goto _90;
else if( input == 0x0 )   goto _363;
else                      goto _185;


    __quex_assert_no_passage();
_79:
    /* (1430 from 1407)  */
    ++(me->buffer._read_p);

    goto _715;


    __quex_assert_no_passage();
_80:
    /* (1431 from 1407)  */
    ++(me->buffer._read_p);

_717:
    input = *(me->buffer._read_p);


    __quex_debug_state(1431);
switch( input ) {
case 0x0: goto _365;
case 0x61: goto _81;
case 0x6F: goto _82;
default: goto _185;
}


    __quex_assert_no_passage();
_364:
    /* (1431 from RELOAD_FORWARD)  */
    goto _717;


    __quex_assert_no_passage();
_366:
    /* (1432 from RELOAD_FORWARD)  */
_719:
    input = *(me->buffer._read_p);


    __quex_debug_state(1432);
if     ( input == 0x6D )  goto _88;
else if( input == 0x0 )   goto _367;
else                      goto _185;


    __quex_assert_no_passage();
_81:
    /* (1432 from 1431)  */
    ++(me->buffer._read_p);

    goto _719;


    __quex_assert_no_passage();
_82:
    /* (1433 from 1431)  */
    ++(me->buffer._read_p);

_721:
    input = *(me->buffer._read_p);


    __quex_debug_state(1433);
if     ( input == 0x6E )  goto _83;
else if( input == 0x0 )   goto _369;
else                      goto _185;


    __quex_assert_no_passage();
_368:
    /* (1433 from RELOAD_FORWARD)  */
    goto _721;


    __quex_assert_no_passage();
_83:
    /* (1434 from 1433)  */
    ++(me->buffer._read_p);

_723:
    input = *(me->buffer._read_p);


    __quex_debug_state(1434);
if     ( input == 0x65 )  goto _84;
else if( input == 0x0 )   goto _371;
else                      goto _185;


    __quex_assert_no_passage();
_370:
    /* (1434 from RELOAD_FORWARD)  */
    goto _723;


    __quex_assert_no_passage();
_84:
    /* (1435 from 1434)  */
    ++(me->buffer._read_p);

_725:
    input = *(me->buffer._read_p);


    __quex_debug_state(1435);
if     ( input == 0x2D )  goto _85;
else if( input == 0x0 )   goto _373;
else                      goto _185;


    __quex_assert_no_passage();
_372:
    /* (1435 from RELOAD_FORWARD)  */
    goto _725;


    __quex_assert_no_passage();
_374:
    /* (1436 from RELOAD_FORWARD)  */
_727:
    input = *(me->buffer._read_p);


    __quex_debug_state(1436);
if     ( input == 0x6F )  goto _86;
else if( input == 0x0 )   goto _375;
else                      goto _185;


    __quex_assert_no_passage();
_85:
    /* (1436 from 1435)  */
    ++(me->buffer._read_p);

    goto _727;


    __quex_assert_no_passage();
_376:
    /* (1437 from RELOAD_FORWARD)  */
_729:
    input = *(me->buffer._read_p);


    __quex_debug_state(1437);
if     ( input == 0x66 )  goto _87;
else if( input == 0x0 )   goto _377;
else                      goto _185;


    __quex_assert_no_passage();
_86:
    /* (1437 from 1436)  */
    ++(me->buffer._read_p);

    goto _729;


    __quex_assert_no_passage();
_87:
    /* (1438 from 1437)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1438);
goto _192;


    __quex_assert_no_passage();
_88:
    /* (1439 from 1432)  */
    ++(me->buffer._read_p);

_732:
    input = *(me->buffer._read_p);


    __quex_debug_state(1439);
if     ( input == 0x65 )  goto _89;
else if( input == 0x0 )   goto _379;
else                      goto _185;


    __quex_assert_no_passage();
_378:
    /* (1439 from RELOAD_FORWARD)  */
    goto _732;


    __quex_assert_no_passage();
_89:
    /* (1440 from 1439)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1440);
goto _193;


    __quex_assert_no_passage();
_90:
    /* (1441 from 1430)  */
    ++(me->buffer._read_p);

_735:
    input = *(me->buffer._read_p);


    __quex_debug_state(1441);
if     ( input == 0x73 )  goto _91;
else if( input == 0x0 )   goto _381;
else                      goto _185;


    __quex_assert_no_passage();
_380:
    /* (1441 from RELOAD_FORWARD)  */
    goto _735;


    __quex_assert_no_passage();
_91:
    /* (1442 from 1441)  */
    ++(me->buffer._read_p);

_737:
    input = *(me->buffer._read_p);


    __quex_debug_state(1442);
if     ( input == 0x68 )  goto _92;
else if( input == 0x0 )   goto _383;
else                      goto _185;


    __quex_assert_no_passage();
_382:
    /* (1442 from RELOAD_FORWARD)  */
    goto _737;


    __quex_assert_no_passage();
_92:
    /* (1443 from 1442)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1443);
goto _194;


    __quex_assert_no_passage();
_384:
    /* (1444 from RELOAD_FORWARD)  */
_740:
    input = *(me->buffer._read_p);


    __quex_debug_state(1444);
switch( input ) {
case 0x0: goto _385;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _96;
case 0x3A: goto _95;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _96;
case 0x6C: goto _113;
default: goto _185;
}


    __quex_assert_no_passage();
_93:
    /* (1444 from 1429)  */
    ++(me->buffer._read_p);

    goto _740;


    __quex_assert_no_passage();
_94:
    /* (1445 from 1412) (1445 from 1423) (1445 from 1429) (1445 from 1410) (1445 from 1427) (1445 from 1426)  */

    ++(me->buffer._read_p);

_743:
    input = *(me->buffer._read_p);


    __quex_debug_state(1445);
switch( input ) {
case 0x0: goto _387;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _96;
case 0x3A: goto _95;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _96;
default: goto _185;
}


    __quex_assert_no_passage();
_386:
    /* (1445 from RELOAD_FORWARD)  */
    goto _743;


    __quex_assert_no_passage();
_95:
    /* (1446 from 1445) (1446 from 1565) (1446 from 1444)  */

    ++(me->buffer._read_p);

_746:
    input = *(me->buffer._read_p);


    __quex_debug_state(1446);
switch( input ) {
case 0x0: goto _389;
case 0x2A: goto _104;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _105;
default: goto _185;
}


    __quex_assert_no_passage();
_388:
    /* (1446 from RELOAD_FORWARD)  */
    goto _746;


    __quex_assert_no_passage();
_96:
    /* (1447 from 1444) (1447 from 1565) (1447 from 1445)  */

    ++(me->buffer._read_p);

_749:
    input = *(me->buffer._read_p);


    __quex_debug_state(1447);
switch( input ) {
case 0x0: goto _391;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _97;
default: goto _185;
}


    __quex_assert_no_passage();
_390:
    /* (1447 from RELOAD_FORWARD)  */
    goto _749;


    __quex_assert_no_passage();
_392:
    /* (1448 from RELOAD_FORWARD)  */
_751:
    input = *(me->buffer._read_p);


    __quex_debug_state(1448);
if     ( input == 0x3A )  goto _98;
else if( input == 0x0 )   goto _393;
else                      goto _185;


    __quex_assert_no_passage();
_97:
    /* (1448 from 1447)  */
    ++(me->buffer._read_p);

    goto _751;


    __quex_assert_no_passage();
_98:
    /* (1449 from 1448)  */
    ++(me->buffer._read_p);

_753:
    input = *(me->buffer._read_p);


    __quex_debug_state(1449);
switch( input ) {
case 0x0: goto _395;
case 0x2A: goto _99;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _100;
default: goto _185;
}


    __quex_assert_no_passage();
_394:
    /* (1449 from RELOAD_FORWARD)  */
    goto _753;


    __quex_assert_no_passage();
_99:
    /* (1450 from 1449)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1450);
goto _195;


    __quex_assert_no_passage();
_100:
    /* (1451 from 1449)  */
    ++(me->buffer._read_p);

_756:
    input = *(me->buffer._read_p);


    __quex_debug_state(1451);
switch( input ) {
case 0x0: goto _397;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _101;
default: goto _185;
}


    __quex_assert_no_passage();
_396:
    /* (1451 from RELOAD_FORWARD)  */
    goto _756;


    __quex_assert_no_passage();
_398:
    /* (1452 from RELOAD_FORWARD)  */
_758:
    input = *(me->buffer._read_p);


    __quex_debug_state(1452);
switch( input ) {
case 0x0: goto _399;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _102;
default: goto _185;
}


    __quex_assert_no_passage();
_101:
    /* (1452 from 1451)  */
    ++(me->buffer._read_p);

    goto _758;


    __quex_assert_no_passage();
_400:
    /* (1453 from RELOAD_FORWARD)  */
_760:
    input = *(me->buffer._read_p);


    __quex_debug_state(1453);
switch( input ) {
case 0x0: goto _401;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _103;
default: goto _185;
}


    __quex_assert_no_passage();
_102:
    /* (1453 from 1452)  */
    ++(me->buffer._read_p);

    goto _760;


    __quex_assert_no_passage();
_103:
    /* (1454 from 1453)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1454);
goto _196;


    __quex_assert_no_passage();
_104:
    /* (1455 from 1446)  */
    ++(me->buffer._read_p);

_763:
    input = *(me->buffer._read_p);


    __quex_debug_state(1455);
if     ( input == 0x3A )  goto _111;
else if( input == 0x0 )   goto _403;
else                      goto _185;


    __quex_assert_no_passage();
_402:
    /* (1455 from RELOAD_FORWARD)  */
    goto _763;


    __quex_assert_no_passage();
_404:
    /* (1456 from RELOAD_FORWARD)  */
_765:
    input = *(me->buffer._read_p);


    __quex_debug_state(1456);
switch( input ) {
case 0x0: goto _405;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _106;
default: goto _185;
}


    __quex_assert_no_passage();
_105:
    /* (1456 from 1446)  */
    ++(me->buffer._read_p);

    goto _765;


    __quex_assert_no_passage();
_106:
    /* (1457 from 1456)  */
    ++(me->buffer._read_p);

_767:
    input = *(me->buffer._read_p);


    __quex_debug_state(1457);
if     ( input == 0x3A )  goto _107;
else if( input == 0x0 )   goto _407;
else                      goto _185;


    __quex_assert_no_passage();
_406:
    /* (1457 from RELOAD_FORWARD)  */
    goto _767;


    __quex_assert_no_passage();
_107:
    /* (1458 from 1457)  */
    ++(me->buffer._read_p);

_769:
    input = *(me->buffer._read_p);


    __quex_debug_state(1458);
switch( input ) {
case 0x0: goto _409;
case 0x2A: goto _108;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _109;
default: goto _185;
}


    __quex_assert_no_passage();
_408:
    /* (1458 from RELOAD_FORWARD)  */
    goto _769;


    __quex_assert_no_passage();
_108:
    /* (1459 from 1458)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1459);
goto _197;


    __quex_assert_no_passage();
_410:
    /* (1460 from RELOAD_FORWARD)  */
_772:
    input = *(me->buffer._read_p);


    __quex_debug_state(1460);
switch( input ) {
case 0x0: goto _411;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _110;
default: goto _185;
}


    __quex_assert_no_passage();
_109:
    /* (1460 from 1458)  */
    ++(me->buffer._read_p);

    goto _772;


    __quex_assert_no_passage();
_110:
    /* (1461 from 1460)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1461);
goto _198;


    __quex_assert_no_passage();
_412:
    /* (1462 from RELOAD_FORWARD)  */
_775:
    input = *(me->buffer._read_p);


    __quex_debug_state(1462);
if     ( input == 0x2A )  goto _112;
else if( input == 0x0 )   goto _413;
else                      goto _185;


    __quex_assert_no_passage();
_111:
    /* (1462 from 1455)  */
    ++(me->buffer._read_p);

    goto _775;


    __quex_assert_no_passage();
_112:
    /* (1463 from 1462)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1463);
goto _199;


    __quex_assert_no_passage();
_414:
    /* (1464 from RELOAD_FORWARD)  */
_778:
    input = *(me->buffer._read_p);


    __quex_debug_state(1464);
if     ( input == 0x73 )  goto _114;
else if( input == 0x0 )   goto _415;
else                      goto _185;


    __quex_assert_no_passage();
_113:
    /* (1464 from 1444)  */
    ++(me->buffer._read_p);

    goto _778;


    __quex_assert_no_passage();
_114:
    /* (1465 from 1464)  */
    ++(me->buffer._read_p);

_780:
    input = *(me->buffer._read_p);


    __quex_debug_state(1465);
if     ( input == 0x65 )  goto _115;
else if( input == 0x0 )   goto _417;
else                      goto _185;


    __quex_assert_no_passage();
_416:
    /* (1465 from RELOAD_FORWARD)  */
    goto _780;


    __quex_assert_no_passage();
_115:
    /* (1466 from 1465)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1466);
goto _200;


    __quex_assert_no_passage();
_116:
    /* (1467 from 1428)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1467);
goto _201;


    __quex_assert_no_passage();
_418:
    /* (1468 from RELOAD_FORWARD)  */
_784:
    input = *(me->buffer._read_p);


    __quex_debug_state(1468);
if     ( input == 0x79 )  goto _135;
else if( input == 0x0 )   goto _419;
else                      goto _185;


    __quex_assert_no_passage();
_117:
    /* (1468 from 1427)  */
    ++(me->buffer._read_p);

    goto _784;


    __quex_assert_no_passage();
_420:
    /* (1469 from RELOAD_FORWARD)  */
_786:
    input = *(me->buffer._read_p);


    __quex_debug_state(1469);
if     ( input == 0x6C )  goto _119;
else if( input == 0x0 )   goto _421;
else                      goto _185;


    __quex_assert_no_passage();
_118:
    /* (1469 from 1427)  */
    ++(me->buffer._read_p);

    goto _786;


    __quex_assert_no_passage();
_422:
    /* (1470 from RELOAD_FORWARD)  */
_788:
    input = *(me->buffer._read_p);


    __quex_debug_state(1470);
switch( input ) {
case 0x0: goto _423;
case 0x2D: goto _120;
case 0x6F: goto _121;
default: goto _185;
}


    __quex_assert_no_passage();
_119:
    /* (1470 from 1469)  */
    ++(me->buffer._read_p);

    goto _788;


    __quex_assert_no_passage();
_120:
    /* (1471 from 1470)  */
    ++(me->buffer._read_p);

_790:
    input = *(me->buffer._read_p);


    __quex_debug_state(1471);
if     ( input == 0x6F )  goto _133;
else if( input == 0x0 )   goto _425;
else                      goto _185;


    __quex_assert_no_passage();
_424:
    /* (1471 from RELOAD_FORWARD)  */
    goto _790;


    __quex_assert_no_passage();
_426:
    /* (1472 from RELOAD_FORWARD)  */
_792:
    input = *(me->buffer._read_p);


    __quex_debug_state(1472);
if     ( input == 0x77 )  goto _122;
else if( input == 0x0 )   goto _427;
else                      goto _185;


    __quex_assert_no_passage();
_121:
    /* (1472 from 1470)  */
    ++(me->buffer._read_p);

    goto _792;


    __quex_assert_no_passage();
_122:
    /* (1473 from 1472)  */
    ++(me->buffer._read_p);

_794:
    input = *(me->buffer._read_p);


    __quex_debug_state(1473);
if     ( input == 0x65 )  goto _123;
else if( input == 0x0 )   goto _429;
else                      goto _202;


    __quex_assert_no_passage();
_428:
    /* (1473 from RELOAD_FORWARD)  */
    goto _794;


    __quex_assert_no_passage();
_123:
    /* (1474 from 1473)  */
    ++(me->buffer._read_p);

_796:
    input = *(me->buffer._read_p);


    __quex_debug_state(1474);
if     ( input == 0x64 )  goto _124;
else if( input == 0x0 )   goto _431;
else                      goto _203;


    __quex_assert_no_passage();
_430:
    /* (1474 from RELOAD_FORWARD)  */
    goto _796;


    __quex_assert_no_passage();
_124:
    /* (1475 from 1474)  */
    ++(me->buffer._read_p);

_798:
    input = *(me->buffer._read_p);


    __quex_debug_state(1475);
if     ( input == 0x2D )  goto _125;
else if( input == 0x0 )   goto _433;
else                      goto _204;


    __quex_assert_no_passage();
_432:
    /* (1475 from RELOAD_FORWARD)  */
    goto _798;


    __quex_assert_no_passage();
_434:
    /* (1476 from RELOAD_FORWARD)  */
_800:
    input = *(me->buffer._read_p);


    __quex_debug_state(1476);
if     ( input == 0x6D )  goto _126;
else if( input == 0x0 )   goto _435;
else                      goto _205;


    __quex_assert_no_passage();
_125:
    /* (1476 from 1475)  */
    ++(me->buffer._read_p);

    goto _800;


    __quex_assert_no_passage();
_436:
    /* (1477 from RELOAD_FORWARD)  */
_802:
    input = *(me->buffer._read_p);


    __quex_debug_state(1477);
if     ( input == 0x61 )  goto _127;
else if( input == 0x0 )   goto _437;
else                      goto _206;


    __quex_assert_no_passage();
_126:
    /* (1477 from 1476)  */
    ++(me->buffer._read_p);

    goto _802;


    __quex_assert_no_passage();
_438:
    /* (1478 from RELOAD_FORWARD)  */
_804:
    input = *(me->buffer._read_p);


    __quex_debug_state(1478);
if     ( input == 0x74 )  goto _128;
else if( input == 0x0 )   goto _439;
else                      goto _207;


    __quex_assert_no_passage();
_127:
    /* (1478 from 1477)  */
    ++(me->buffer._read_p);

    goto _804;


    __quex_assert_no_passage();
_128:
    /* (1479 from 1478)  */
    ++(me->buffer._read_p);

_806:
    input = *(me->buffer._read_p);


    __quex_debug_state(1479);
if     ( input == 0x63 )  goto _129;
else if( input == 0x0 )   goto _441;
else                      goto _208;


    __quex_assert_no_passage();
_440:
    /* (1479 from RELOAD_FORWARD)  */
    goto _806;


    __quex_assert_no_passage();
_442:
    /* (1480 from RELOAD_FORWARD)  */
_808:
    input = *(me->buffer._read_p);


    __quex_debug_state(1480);
if     ( input == 0x68 )  goto _130;
else if( input == 0x0 )   goto _443;
else                      goto _209;


    __quex_assert_no_passage();
_129:
    /* (1480 from 1479)  */
    ++(me->buffer._read_p);

    goto _808;


    __quex_assert_no_passage();
_130:
    /* (1481 from 1480)  */
    ++(me->buffer._read_p);

_810:
    input = *(me->buffer._read_p);


    __quex_debug_state(1481);
if     ( input == 0x65 )  goto _131;
else if( input == 0x0 )   goto _445;
else                      goto _210;


    __quex_assert_no_passage();
_444:
    /* (1481 from RELOAD_FORWARD)  */
    goto _810;


    __quex_assert_no_passage();
_131:
    /* (1482 from 1481)  */
    ++(me->buffer._read_p);

_812:
    input = *(me->buffer._read_p);


    __quex_debug_state(1482);
if     ( input == 0x73 )  goto _132;
else if( input == 0x0 )   goto _447;
else                      goto _211;


    __quex_assert_no_passage();
_446:
    /* (1482 from RELOAD_FORWARD)  */
    goto _812;


    __quex_assert_no_passage();
_132:
    /* (1483 from 1482)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1483);
goto _212;


    __quex_assert_no_passage();
_448:
    /* (1484 from RELOAD_FORWARD)  */
_815:
    input = *(me->buffer._read_p);


    __quex_debug_state(1484);
if     ( input == 0x66 )  goto _134;
else if( input == 0x0 )   goto _449;
else                      goto _185;


    __quex_assert_no_passage();
_133:
    /* (1484 from 1471)  */
    ++(me->buffer._read_p);

    goto _815;


    __quex_assert_no_passage();
_134:
    /* (1485 from 1484)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1485);
goto _213;


    __quex_assert_no_passage();
_450:
    /* (1486 from RELOAD_FORWARD)  */
_818:
    input = *(me->buffer._read_p);


    __quex_debug_state(1486);
if     ( input == 0x2D )  goto _136;
else if( input == 0x0 )   goto _451;
else                      goto _185;


    __quex_assert_no_passage();
_135:
    /* (1486 from 1468)  */
    ++(me->buffer._read_p);

    goto _818;


    __quex_assert_no_passage();
_136:
    /* (1487 from 1486)  */
    ++(me->buffer._read_p);

_820:
    input = *(me->buffer._read_p);


    __quex_debug_state(1487);
if     ( input == 0x6F )  goto _137;
else if( input == 0x0 )   goto _453;
else                      goto _185;


    __quex_assert_no_passage();
_452:
    /* (1487 from RELOAD_FORWARD)  */
    goto _820;


    __quex_assert_no_passage();
_454:
    /* (1488 from RELOAD_FORWARD)  */
_822:
    input = *(me->buffer._read_p);


    __quex_debug_state(1488);
if     ( input == 0x66 )  goto _138;
else if( input == 0x0 )   goto _455;
else                      goto _185;


    __quex_assert_no_passage();
_137:
    /* (1488 from 1487)  */
    ++(me->buffer._read_p);

    goto _822;


    __quex_assert_no_passage();
_138:
    /* (1489 from 1488)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1489);
goto _214;


    __quex_assert_no_passage();
_139:
    /* (1490 from 1424)  */
    ++(me->buffer._read_p);

_825:
    input = *(me->buffer._read_p);


    __quex_debug_state(1490);
if     ( input == 0x72 )  goto _140;
else if( input == 0x0 )   goto _457;
else                      goto _185;


    __quex_assert_no_passage();
_456:
    /* (1490 from RELOAD_FORWARD)  */
    goto _825;


    __quex_assert_no_passage();
_140:
    /* (1491 from 1490)  */
    ++(me->buffer._read_p);

_827:
    input = *(me->buffer._read_p);


    __quex_debug_state(1491);
if     ( input == 0x69 )  goto _141;
else if( input == 0x0 )   goto _459;
else                      goto _185;


    __quex_assert_no_passage();
_458:
    /* (1491 from RELOAD_FORWARD)  */
    goto _827;


    __quex_assert_no_passage();
_460:
    /* (1492 from RELOAD_FORWARD)  */
_829:
    input = *(me->buffer._read_p);


    __quex_debug_state(1492);
if     ( input == 0x61 )  goto _142;
else if( input == 0x0 )   goto _461;
else                      goto _185;


    __quex_assert_no_passage();
_141:
    /* (1492 from 1491)  */
    ++(me->buffer._read_p);

    goto _829;


    __quex_assert_no_passage();
_462:
    /* (1493 from RELOAD_FORWARD)  */
_831:
    input = *(me->buffer._read_p);


    __quex_debug_state(1493);
if     ( input == 0x6C )  goto _143;
else if( input == 0x0 )   goto _463;
else                      goto _185;


    __quex_assert_no_passage();
_142:
    /* (1493 from 1492)  */
    ++(me->buffer._read_p);

    goto _831;


    __quex_assert_no_passage();
_143:
    /* (1494 from 1493)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1494);
goto _215;


    __quex_assert_no_passage();
_144:
    /* (1495 from 1423)  */
    ++(me->buffer._read_p);

_834:
    input = *(me->buffer._read_p);


    __quex_debug_state(1495);
if     ( input == 0x75 )  goto _145;
else if( input == 0x0 )   goto _465;
else                      goto _185;


    __quex_assert_no_passage();
_464:
    /* (1495 from RELOAD_FORWARD)  */
    goto _834;


    __quex_assert_no_passage();
_466:
    /* (1496 from RELOAD_FORWARD)  */
_836:
    input = *(me->buffer._read_p);


    __quex_debug_state(1496);
if     ( input == 0x61 )  goto _146;
else if( input == 0x0 )   goto _467;
else                      goto _185;


    __quex_assert_no_passage();
_145:
    /* (1496 from 1495)  */
    ++(me->buffer._read_p);

    goto _836;


    __quex_assert_no_passage();
_146:
    /* (1497 from 1496)  */
    ++(me->buffer._read_p);

_838:
    input = *(me->buffer._read_p);


    __quex_debug_state(1497);
if     ( input == 0x6C )  goto _147;
else if( input == 0x0 )   goto _469;
else                      goto _185;


    __quex_assert_no_passage();
_468:
    /* (1497 from RELOAD_FORWARD)  */
    goto _838;


    __quex_assert_no_passage();
_147:
    /* (1498 from 1497)  */
    ++(me->buffer._read_p);

_840:
    input = *(me->buffer._read_p);


    __quex_debug_state(1498);
if     ( input == 0x73 )  goto _148;
else if( input == 0x0 )   goto _471;
else                      goto _185;


    __quex_assert_no_passage();
_470:
    /* (1498 from RELOAD_FORWARD)  */
    goto _840;


    __quex_assert_no_passage();
_148:
    /* (1499 from 1498)  */
    ++(me->buffer._read_p);

_842:
    input = *(me->buffer._read_p);


    __quex_debug_state(1499);
if     ( input == 0x2D )  goto _149;
else if( input == 0x0 )   goto _473;
else                      goto _216;


    __quex_assert_no_passage();
_472:
    /* (1499 from RELOAD_FORWARD)  */
    goto _842;


    __quex_assert_no_passage();
_474:
    /* (1500 from RELOAD_FORWARD)  */
_844:
    input = *(me->buffer._read_p);


    __quex_debug_state(1500);
if     ( input == 0x6F )  goto _150;
else if( input == 0x0 )   goto _475;
else                      goto _217;


    __quex_assert_no_passage();
_149:
    /* (1500 from 1499)  */
    ++(me->buffer._read_p);

    goto _844;


    __quex_assert_no_passage();
_476:
    /* (1501 from RELOAD_FORWARD)  */
_846:
    input = *(me->buffer._read_p);


    __quex_debug_state(1501);
if     ( input == 0x72 )  goto _151;
else if( input == 0x0 )   goto _477;
else                      goto _218;


    __quex_assert_no_passage();
_150:
    /* (1501 from 1500)  */
    ++(me->buffer._read_p);

    goto _846;


    __quex_assert_no_passage();
_478:
    /* (1502 from RELOAD_FORWARD)  */
_848:
    input = *(me->buffer._read_p);


    __quex_debug_state(1502);
if     ( input == 0x64 )  goto _152;
else if( input == 0x0 )   goto _479;
else                      goto _219;


    __quex_assert_no_passage();
_151:
    /* (1502 from 1501)  */
    ++(me->buffer._read_p);

    goto _848;


    __quex_assert_no_passage();
_152:
    /* (1503 from 1502)  */
    ++(me->buffer._read_p);

_850:
    input = *(me->buffer._read_p);


    __quex_debug_state(1503);
if     ( input == 0x65 )  goto _153;
else if( input == 0x0 )   goto _481;
else                      goto _220;


    __quex_assert_no_passage();
_480:
    /* (1503 from RELOAD_FORWARD)  */
    goto _850;


    __quex_assert_no_passage();
_482:
    /* (1504 from RELOAD_FORWARD)  */
_852:
    input = *(me->buffer._read_p);


    __quex_debug_state(1504);
if     ( input == 0x72 )  goto _154;
else if( input == 0x0 )   goto _483;
else                      goto _221;


    __quex_assert_no_passage();
_153:
    /* (1504 from 1503)  */
    ++(me->buffer._read_p);

    goto _852;


    __quex_assert_no_passage();
_154:
    /* (1505 from 1504)  */
    ++(me->buffer._read_p);

_854:
    input = *(me->buffer._read_p);


    __quex_debug_state(1505);
if     ( input == 0x65 )  goto _155;
else if( input == 0x0 )   goto _485;
else                      goto _222;


    __quex_assert_no_passage();
_484:
    /* (1505 from RELOAD_FORWARD)  */
    goto _854;


    __quex_assert_no_passage();
_155:
    /* (1506 from 1505)  */
    ++(me->buffer._read_p);

_856:
    input = *(me->buffer._read_p);


    __quex_debug_state(1506);
if     ( input == 0x64 )  goto _156;
else if( input == 0x0 )   goto _487;
else                      goto _223;


    __quex_assert_no_passage();
_486:
    /* (1506 from RELOAD_FORWARD)  */
    goto _856;


    __quex_assert_no_passage();
_156:
    /* (1507 from 1506)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1507);
goto _224;


    __quex_assert_no_passage();
_488:
    /* (1508 from RELOAD_FORWARD)  */
_859:
    input = *(me->buffer._read_p);


    __quex_debug_state(1508);
if     ( input == 0x63 )  goto _158;
else if( input == 0x0 )   goto _489;
else                      goto _185;


    __quex_assert_no_passage();
_157:
    /* (1508 from 1421)  */
    ++(me->buffer._read_p);

    goto _859;


    __quex_assert_no_passage();
_490:
    /* (1509 from RELOAD_FORWARD)  */
_861:
    input = *(me->buffer._read_p);


    __quex_debug_state(1509);
if     ( input == 0x61 )  goto _159;
else if( input == 0x0 )   goto _491;
else                      goto _185;


    __quex_assert_no_passage();
_158:
    /* (1509 from 1508)  */
    ++(me->buffer._read_p);

    goto _861;


    __quex_assert_no_passage();
_492:
    /* (1510 from RELOAD_FORWARD)  */
_863:
    input = *(me->buffer._read_p);


    __quex_debug_state(1510);
if     ( input == 0x6C )  goto _160;
else if( input == 0x0 )   goto _493;
else                      goto _185;


    __quex_assert_no_passage();
_159:
    /* (1510 from 1509)  */
    ++(me->buffer._read_p);

    goto _863;


    __quex_assert_no_passage();
_160:
    /* (1511 from 1510)  */
    ++(me->buffer._read_p);

_865:
    input = *(me->buffer._read_p);


    __quex_debug_state(1511);
if     ( input == 0x74 )  goto _161;
else if( input == 0x0 )   goto _495;
else                      goto _185;


    __quex_assert_no_passage();
_494:
    /* (1511 from RELOAD_FORWARD)  */
    goto _865;


    __quex_assert_no_passage();
_496:
    /* (1512 from RELOAD_FORWARD)  */
_867:
    input = *(me->buffer._read_p);


    __quex_debug_state(1512);
if     ( input == 0x69 )  goto _162;
else if( input == 0x0 )   goto _497;
else                      goto _185;


    __quex_assert_no_passage();
_161:
    /* (1512 from 1511)  */
    ++(me->buffer._read_p);

    goto _867;


    __quex_assert_no_passage();
_162:
    /* (1513 from 1512)  */
    ++(me->buffer._read_p);

_869:
    input = *(me->buffer._read_p);


    __quex_debug_state(1513);
if     ( input == 0x6D )  goto _163;
else if( input == 0x0 )   goto _499;
else                      goto _185;


    __quex_assert_no_passage();
_498:
    /* (1513 from RELOAD_FORWARD)  */
    goto _869;


    __quex_assert_no_passage();
_163:
    /* (1514 from 1513)  */
    ++(me->buffer._read_p);

_871:
    input = *(me->buffer._read_p);


    __quex_debug_state(1514);
if     ( input == 0x65 )  goto _164;
else if( input == 0x0 )   goto _501;
else                      goto _185;


    __quex_assert_no_passage();
_500:
    /* (1514 from RELOAD_FORWARD)  */
    goto _871;


    __quex_assert_no_passage();
_164:
    /* (1515 from 1514)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1515);
goto _225;


    __quex_assert_no_passage();
_502:
    /* (1516 from RELOAD_FORWARD)  */
_874:
    input = *(me->buffer._read_p);


    __quex_debug_state(1516);
if     ( input == 0x74 )  goto _166;
else if( input == 0x0 )   goto _503;
else                      goto _185;


    __quex_assert_no_passage();
_165:
    /* (1516 from 1419)  */
    ++(me->buffer._read_p);

    goto _874;


    __quex_assert_no_passage();
_504:
    /* (1517 from RELOAD_FORWARD)  */
_876:
    input = *(me->buffer._read_p);


    __quex_debug_state(1517);
if     ( input == 0x68 )  goto _167;
else if( input == 0x0 )   goto _505;
else                      goto _185;


    __quex_assert_no_passage();
_166:
    /* (1517 from 1516)  */
    ++(me->buffer._read_p);

    goto _876;


    __quex_assert_no_passage();
_506:
    /* (1518 from RELOAD_FORWARD)  */
_878:
    input = *(me->buffer._read_p);


    __quex_debug_state(1518);
if     ( input == 0x2D )  goto _168;
else if( input == 0x0 )   goto _507;
else                      goto _185;


    __quex_assert_no_passage();
_167:
    /* (1518 from 1517)  */
    ++(me->buffer._read_p);

    goto _878;


    __quex_assert_no_passage();
_168:
    /* (1519 from 1518)  */
    ++(me->buffer._read_p);

_880:
    input = *(me->buffer._read_p);


    __quex_debug_state(1519);
if     ( input == 0x69 )  goto _169;
else if( input == 0x0 )   goto _509;
else                      goto _185;


    __quex_assert_no_passage();
_508:
    /* (1519 from RELOAD_FORWARD)  */
    goto _880;


    __quex_assert_no_passage();
_510:
    /* (1520 from RELOAD_FORWARD)  */
_882:
    input = *(me->buffer._read_p);


    __quex_debug_state(1520);
if     ( input == 0x6E )  goto _170;
else if( input == 0x0 )   goto _511;
else                      goto _185;


    __quex_assert_no_passage();
_169:
    /* (1520 from 1519)  */
    ++(me->buffer._read_p);

    goto _882;


    __quex_assert_no_passage();
_170:
    /* (1521 from 1520)  */
    ++(me->buffer._read_p);

_884:
    input = *(me->buffer._read_p);


    __quex_debug_state(1521);
if     ( input == 0x74 )  goto _171;
else if( input == 0x0 )   goto _513;
else                      goto _185;


    __quex_assert_no_passage();
_512:
    /* (1521 from RELOAD_FORWARD)  */
    goto _884;


    __quex_assert_no_passage();
_171:
    /* (1522 from 1521)  */
    ++(me->buffer._read_p);

_886:
    input = *(me->buffer._read_p);


    __quex_debug_state(1522);
if     ( input == 0x65 )  goto _172;
else if( input == 0x0 )   goto _515;
else                      goto _185;


    __quex_assert_no_passage();
_514:
    /* (1522 from RELOAD_FORWARD)  */
    goto _886;


    __quex_assert_no_passage();
_172:
    /* (1523 from 1522)  */
    ++(me->buffer._read_p);

_888:
    input = *(me->buffer._read_p);


    __quex_debug_state(1523);
if     ( input == 0x72 )  goto _173;
else if( input == 0x0 )   goto _517;
else                      goto _185;


    __quex_assert_no_passage();
_516:
    /* (1523 from RELOAD_FORWARD)  */
    goto _888;


    __quex_assert_no_passage();
_518:
    /* (1524 from RELOAD_FORWARD)  */
_890:
    input = *(me->buffer._read_p);


    __quex_debug_state(1524);
if     ( input == 0x66 )  goto _174;
else if( input == 0x0 )   goto _519;
else                      goto _185;


    __quex_assert_no_passage();
_173:
    /* (1524 from 1523)  */
    ++(me->buffer._read_p);

    goto _890;


    __quex_assert_no_passage();
_520:
    /* (1525 from RELOAD_FORWARD)  */
_892:
    input = *(me->buffer._read_p);


    __quex_debug_state(1525);
if     ( input == 0x61 )  goto _175;
else if( input == 0x0 )   goto _521;
else                      goto _185;


    __quex_assert_no_passage();
_174:
    /* (1525 from 1524)  */
    ++(me->buffer._read_p);

    goto _892;


    __quex_assert_no_passage();
_522:
    /* (1526 from RELOAD_FORWARD)  */
_894:
    input = *(me->buffer._read_p);


    __quex_debug_state(1526);
if     ( input == 0x63 )  goto _176;
else if( input == 0x0 )   goto _523;
else                      goto _185;


    __quex_assert_no_passage();
_175:
    /* (1526 from 1525)  */
    ++(me->buffer._read_p);

    goto _894;


    __quex_assert_no_passage();
_176:
    /* (1527 from 1526)  */
    ++(me->buffer._read_p);

_896:
    input = *(me->buffer._read_p);


    __quex_debug_state(1527);
if     ( input == 0x65 )  goto _177;
else if( input == 0x0 )   goto _525;
else                      goto _185;


    __quex_assert_no_passage();
_524:
    /* (1527 from RELOAD_FORWARD)  */
    goto _896;


    __quex_assert_no_passage();
_177:
    /* (1528 from 1527)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1528);
goto _226;


    __quex_assert_no_passage();
_178:
    /* (1529 from 1417)  */
    ++(me->buffer._read_p);

_899:
    input = *(me->buffer._read_p);


    __quex_debug_state(1529);
if     ( input == 0x2A )  goto _179;
else if( input == 0x0 )   goto _527;
else                      goto _185;


    __quex_assert_no_passage();
_526:
    /* (1529 from RELOAD_FORWARD)  */
    goto _899;


    __quex_assert_no_passage();
_179:
    /* (1530 from 1529)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1530);
goto _227;


    __quex_assert_no_passage();
_180:
    /* (1531 from 1414)  */
    ++(me->buffer._read_p);

_902:
    input = *(me->buffer._read_p);


    __quex_debug_state(1531);
if     ( input == 0x74 )  goto _181;
else if( input == 0x0 )   goto _529;
else                      goto _185;


    __quex_assert_no_passage();
_528:
    /* (1531 from RELOAD_FORWARD)  */
    goto _902;


    __quex_assert_no_passage();
_530:
    /* (1532 from RELOAD_FORWARD)  */
_904:
    input = *(me->buffer._read_p);


    __quex_debug_state(1532);
if     ( input == 0x63 )  goto _182;
else if( input == 0x0 )   goto _531;
else                      goto _185;


    __quex_assert_no_passage();
_181:
    /* (1532 from 1531)  */
    ++(me->buffer._read_p);

    goto _904;


    __quex_assert_no_passage();
_532:
    /* (1533 from RELOAD_FORWARD)  */
_906:
    input = *(me->buffer._read_p);


    __quex_debug_state(1533);
if     ( input == 0x68 )  goto _183;
else if( input == 0x0 )   goto _533;
else                      goto _185;


    __quex_assert_no_passage();
_182:
    /* (1533 from 1532)  */
    ++(me->buffer._read_p);

    goto _906;


    __quex_assert_no_passage();
_183:
    /* (1534 from 1533)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1534);
goto _228;


    __quex_assert_no_passage();
_184:
    /* (1535 from 1413)  */
    ++(me->buffer._read_p);

_909:
    input = *(me->buffer._read_p);


    __quex_debug_state(1535);
if     ( input == 0x6C )  goto _14;
else if( input == 0x0 )   goto _535;
else                      goto _185;


    __quex_assert_no_passage();
_534:
    /* (1535 from RELOAD_FORWARD)  */
    goto _909;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_326:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 5138 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_540:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 5155 "Lexer.cpp"

}
goto _1;
_572:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{

#   line 33 "Lexer.qx"


#   line 5167 "Lexer.cpp"

}
goto _0;
_567:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 35 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5182 "Lexer.cpp"

}
goto _0;
_576:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 36 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5197 "Lexer.cpp"

}
goto _0;
_551:
    __quex_debug("* TERMINAL \"!\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 37 "Lexer.qx"
self_send(RULE_TOKEN_NEGATION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5212 "Lexer.cpp"

}
goto _0;
_543:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 39 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5227 "Lexer.cpp"

}
goto _0;
_554:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 40 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5242 "Lexer.cpp"

}
goto _0;
_561:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 41 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5257 "Lexer.cpp"

}
goto _0;
_547:
    __quex_debug("* TERMINAL \"match\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 42 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_MATCH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5272 "Lexer.cpp"

}
goto _0;
_573:
    __quex_debug("* TERMINAL \"device\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 43 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_DEVICE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5287 "Lexer.cpp"

}
goto _0;
_566:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 45 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5302 "Lexer.cpp"

}
goto _0;
_575:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 46 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5317 "Lexer.cpp"

}
goto _0;
_546:
    __quex_debug("* TERMINAL \"via-port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{

#   line 47 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_VIAPORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5332 "Lexer.cpp"

}
goto _0;
_564:
    __quex_debug("* TERMINAL \"with-interface\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 48 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_WITHINTERFACE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5347 "Lexer.cpp"

}
goto _0;
_565:
    __quex_debug("* TERMINAL \"serial\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 49 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_SERIAL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5362 "Lexer.cpp"

}
goto _0;
_548:
    __quex_debug("* TERMINAL \"if\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{

#   line 50 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_IF);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5377 "Lexer.cpp"

}
goto _0;
_552:
    __quex_debug("* TERMINAL \"all-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 52 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5393 "Lexer.cpp"

}
goto _0;
_559:
    __quex_debug("* TERMINAL \"any-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 53 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5409 "Lexer.cpp"

}
goto _0;
_560:
    __quex_debug("* TERMINAL \"one-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 54 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5425 "Lexer.cpp"

}
goto _0;
_545:
    __quex_debug("* TERMINAL \"none-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{

#   line 55 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5441 "Lexer.cpp"

}
goto _0;
_550:
    __quex_debug("* TERMINAL \"equals\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 56 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5457 "Lexer.cpp"

}
goto _0;
_571:
    __quex_debug("* TERMINAL \"equals-ordered\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 57 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5473 "Lexer.cpp"

}
goto _0;
_541:
    __quex_debug("* TERMINAL \"allowed-matches\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(15);
{

#   line 59 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5489 "Lexer.cpp"

}
goto _0;
_558:
    __quex_debug("* TERMINAL \"localtime\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{

#   line 60 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5505 "Lexer.cpp"

}
goto _0;
_568:
    __quex_debug("* TERMINAL \"true\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 61 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5521 "Lexer.cpp"

}
goto _0;
_553:
    __quex_debug("* TERMINAL \"false\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 62 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5537 "Lexer.cpp"

}
goto _0;
_555:
    __quex_debug("* TERMINAL \"random\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 63 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5553 "Lexer.cpp"

}
goto _0;
_542:
    __quex_debug("* TERMINAL \"rule-applied\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(12);
{

#   line 64 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5569 "Lexer.cpp"

}
goto _0;
_569:
    __quex_debug("* TERMINAL \"rule-evaluated\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 65 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5585 "Lexer.cpp"

}
goto _0;
_556:
    __quex_debug("* TERMINAL [:xdigit:]{4}\":\"[:xdigit:]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{

#   line 67 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_DEVICE_VPID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5601 "Lexer.cpp"

}
goto _0;
_549:
    __quex_debug("* TERMINAL [:xdigit:]{4}\":*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 68 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_DEVICE_VID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5617 "Lexer.cpp"

}
goto _0;
_574:
    __quex_debug("* TERMINAL \"*:*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(3);
{

#   line 69 "Lexer.qx"
self_send(RULE_TOKEN_DEVICE_ANYID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5632 "Lexer.cpp"

}
goto _0;
_570:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":\"[:xdigit:]{2}\":\"[:xdigit:]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{

#   line 71 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_FULL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5648 "Lexer.cpp"

}
goto _0;
_562:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":\"[:xdigit:]{2}\":*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{

#   line 72 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_SUBCLASS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5664 "Lexer.cpp"

}
goto _0;
_544:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":*:*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 73 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_CLASS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5680 "Lexer.cpp"

}
goto _0;
_557:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 79 "Lexer.qx"
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 5696 "Lexer.cpp"

}
goto _0;
_563:
    __quex_debug("* TERMINAL \"(\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 85 "Lexer.qx"
    self_send(RULE_TOKEN_PQ_STRING_BEGIN);
    self << PQ_STRING_READER;
    RETURN;


#   line 5712 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_912:
    switch( target_state_index ) {
        case 3: { goto _3; }
        case 185: { goto _185; }
        case 191: { goto _191; }
        case 202: { goto _202; }
        case 203: { goto _203; }
        case 204: { goto _204; }
        case 205: { goto _205; }
        case 206: { goto _206; }
        case 207: { goto _207; }
        case 208: { goto _208; }
        case 209: { goto _209; }
        case 210: { goto _210; }
        case 211: { goto _211; }
        case 216: { goto _216; }
        case 217: { goto _217; }
        case 218: { goto _218; }
        case 219: { goto _219; }
        case 220: { goto _220; }
        case 221: { goto _221; }
        case 222: { goto _222; }
        case 223: { goto _223; }
        case 238: { goto _238; }
        case 240: { goto _240; }
        case 242: { goto _242; }
        case 244: { goto _244; }
        case 246: { goto _246; }
        case 248: { goto _248; }
        case 250: { goto _250; }
        case 252: { goto _252; }
        case 254: { goto _254; }
        case 256: { goto _256; }
        case 258: { goto _258; }
        case 260: { goto _260; }
        case 262: { goto _262; }
        case 264: { goto _264; }
        case 266: { goto _266; }
        case 268: { goto _268; }
        case 270: { goto _270; }
        case 272: { goto _272; }
        case 274: { goto _274; }
        case 276: { goto _276; }
        case 278: { goto _278; }
        case 280: { goto _280; }
        case 282: { goto _282; }
        case 284: { goto _284; }
        case 286: { goto _286; }
        case 288: { goto _288; }
        case 290: { goto _290; }
        case 292: { goto _292; }
        case 294: { goto _294; }
        case 296: { goto _296; }
        case 298: { goto _298; }
        case 300: { goto _300; }
        case 302: { goto _302; }
        case 304: { goto _304; }
        case 306: { goto _306; }
        case 308: { goto _308; }
        case 310: { goto _310; }
        case 312: { goto _312; }
        case 314: { goto _314; }
        case 316: { goto _316; }
        case 318: { goto _318; }
        case 320: { goto _320; }
        case 322: { goto _322; }
        case 324: { goto _324; }
        case 326: { goto _326; }
        case 328: { goto _328; }
        case 330: { goto _330; }
        case 332: { goto _332; }
        case 334: { goto _334; }
        case 336: { goto _336; }
        case 338: { goto _338; }
        case 340: { goto _340; }
        case 342: { goto _342; }
        case 344: { goto _344; }
        case 346: { goto _346; }
        case 348: { goto _348; }
        case 350: { goto _350; }
        case 352: { goto _352; }
        case 354: { goto _354; }
        case 356: { goto _356; }
        case 358: { goto _358; }
        case 360: { goto _360; }
        case 362: { goto _362; }
        case 364: { goto _364; }
        case 366: { goto _366; }
        case 368: { goto _368; }
        case 370: { goto _370; }
        case 372: { goto _372; }
        case 374: { goto _374; }
        case 376: { goto _376; }
        case 378: { goto _378; }
        case 380: { goto _380; }
        case 382: { goto _382; }
        case 384: { goto _384; }
        case 386: { goto _386; }
        case 388: { goto _388; }
        case 390: { goto _390; }
        case 392: { goto _392; }
        case 394: { goto _394; }
        case 396: { goto _396; }
        case 398: { goto _398; }
        case 400: { goto _400; }
        case 402: { goto _402; }
        case 404: { goto _404; }
        case 406: { goto _406; }
        case 408: { goto _408; }
        case 410: { goto _410; }
        case 412: { goto _412; }
        case 414: { goto _414; }
        case 416: { goto _416; }
        case 418: { goto _418; }
        case 420: { goto _420; }
        case 422: { goto _422; }
        case 424: { goto _424; }
        case 426: { goto _426; }
        case 428: { goto _428; }
        case 430: { goto _430; }
        case 432: { goto _432; }
        case 434: { goto _434; }
        case 436: { goto _436; }
        case 438: { goto _438; }
        case 440: { goto _440; }
        case 442: { goto _442; }
        case 444: { goto _444; }
        case 446: { goto _446; }
        case 448: { goto _448; }
        case 450: { goto _450; }
        case 452: { goto _452; }
        case 454: { goto _454; }
        case 456: { goto _456; }
        case 458: { goto _458; }
        case 460: { goto _460; }
        case 462: { goto _462; }
        case 464: { goto _464; }
        case 466: { goto _466; }
        case 468: { goto _468; }
        case 470: { goto _470; }
        case 472: { goto _472; }
        case 474: { goto _474; }
        case 476: { goto _476; }
        case 478: { goto _478; }
        case 480: { goto _480; }
        case 482: { goto _482; }
        case 484: { goto _484; }
        case 486: { goto _486; }
        case 488: { goto _488; }
        case 490: { goto _490; }
        case 492: { goto _492; }
        case 494: { goto _494; }
        case 496: { goto _496; }
        case 498: { goto _498; }
        case 500: { goto _500; }
        case 502: { goto _502; }
        case 504: { goto _504; }
        case 506: { goto _506; }
        case 508: { goto _508; }
        case 510: { goto _510; }
        case 512: { goto _512; }
        case 514: { goto _514; }
        case 516: { goto _516; }
        case 518: { goto _518; }
        case 520: { goto _520; }
        case 522: { goto _522; }
        case 524: { goto _524; }
        case 526: { goto _526; }
        case 528: { goto _528; }
        case 530: { goto _530; }
        case 532: { goto _532; }
        case 534: { goto _534; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_307:
    /* (RELOAD_FORWARD from 1575)  */
    target_state_index = QUEX_LABEL(306); target_state_else_index = QUEX_LABEL(185);

_910:
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
_455:
    /* (RELOAD_FORWARD from 1488)  */
    target_state_index = QUEX_LABEL(454); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_273:
    /* (RELOAD_FORWARD from 1555)  */
    target_state_index = QUEX_LABEL(272); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_525:
    /* (RELOAD_FORWARD from 1527)  */
    target_state_index = QUEX_LABEL(524); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_405:
    /* (RELOAD_FORWARD from 1456)  */
    target_state_index = QUEX_LABEL(404); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_251:
    /* (RELOAD_FORWARD from 1543)  */
    target_state_index = QUEX_LABEL(250); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_367:
    /* (RELOAD_FORWARD from 1432)  */
    target_state_index = QUEX_LABEL(366); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_261:
    /* (RELOAD_FORWARD from 1549)  */
    target_state_index = QUEX_LABEL(260); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_473:
    /* (RELOAD_FORWARD from 1499)  */
    target_state_index = QUEX_LABEL(472); target_state_else_index = QUEX_LABEL(216);

    goto _910;


    __quex_assert_no_passage();
_351:
    /* (RELOAD_FORWARD from 1424)  */
    target_state_index = QUEX_LABEL(350); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_483:
    /* (RELOAD_FORWARD from 1504)  */
    target_state_index = QUEX_LABEL(482); target_state_else_index = QUEX_LABEL(221);

    goto _910;


    __quex_assert_no_passage();
_329:
    /* (RELOAD_FORWARD from 1408)  */
    target_state_index = QUEX_LABEL(328); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_527:
    /* (RELOAD_FORWARD from 1529)  */
    target_state_index = QUEX_LABEL(526); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_461:
    /* (RELOAD_FORWARD from 1492)  */
    target_state_index = QUEX_LABEL(460); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_505:
    /* (RELOAD_FORWARD from 1517)  */
    target_state_index = QUEX_LABEL(504); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_295:
    /* (RELOAD_FORWARD from 1568)  */
    target_state_index = QUEX_LABEL(294); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_515:
    /* (RELOAD_FORWARD from 1522)  */
    target_state_index = QUEX_LABEL(514); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_427:
    /* (RELOAD_FORWARD from 1472)  */
    target_state_index = QUEX_LABEL(426); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_241:
    /* (RELOAD_FORWARD from 1537)  */
    target_state_index = QUEX_LABEL(240); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_373:
    /* (RELOAD_FORWARD from 1435)  */
    target_state_index = QUEX_LABEL(372); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_347:
    /* (RELOAD_FORWARD from 1421)  */
    target_state_index = QUEX_LABEL(346); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_463:
    /* (RELOAD_FORWARD from 1493)  */
    target_state_index = QUEX_LABEL(462); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_357:
    /* (RELOAD_FORWARD from 1427)  */
    target_state_index = QUEX_LABEL(356); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_441:
    /* (RELOAD_FORWARD from 1479)  */
    target_state_index = QUEX_LABEL(440); target_state_else_index = QUEX_LABEL(208);

    goto _910;


    __quex_assert_no_passage();
_319:
    /* (RELOAD_FORWARD from 1583)  */
    target_state_index = QUEX_LABEL(318); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_533:
    /* (RELOAD_FORWARD from 1533)  */
    target_state_index = QUEX_LABEL(532); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_451:
    /* (RELOAD_FORWARD from 1486)  */
    target_state_index = QUEX_LABEL(450); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_495:
    /* (RELOAD_FORWARD from 1511)  */
    target_state_index = QUEX_LABEL(494); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_407:
    /* (RELOAD_FORWARD from 1457)  */
    target_state_index = QUEX_LABEL(406); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_301:
    /* (RELOAD_FORWARD from 1572)  */
    target_state_index = QUEX_LABEL(300); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_417:
    /* (RELOAD_FORWARD from 1465)  */
    target_state_index = QUEX_LABEL(416); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_263:
    /* (RELOAD_FORWARD from 1550)  */
    target_state_index = QUEX_LABEL(262); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_395:
    /* (RELOAD_FORWARD from 1449)  */
    target_state_index = QUEX_LABEL(394); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_337:
    /* (RELOAD_FORWARD from 1412)  */
    target_state_index = QUEX_LABEL(336); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_469:
    /* (RELOAD_FORWARD from 1497)  */
    target_state_index = QUEX_LABEL(468); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_315:
    /* (RELOAD_FORWARD from 1581)  */
    target_state_index = QUEX_LABEL(314); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_431:
    /* (RELOAD_FORWARD from 1474)  */
    target_state_index = QUEX_LABEL(430); target_state_else_index = QUEX_LABEL(203);

    goto _910;


    __quex_assert_no_passage();
_325:
    /* (RELOAD_FORWARD from 1587)  */
    target_state_index = QUEX_LABEL(324); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_281:
    /* (RELOAD_FORWARD from 1560)  */
    target_state_index = QUEX_LABEL(280); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_501:
    /* (RELOAD_FORWARD from 1514)  */
    target_state_index = QUEX_LABEL(500); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_413:
    /* (RELOAD_FORWARD from 1462)  */
    target_state_index = QUEX_LABEL(412); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_291:
    /* (RELOAD_FORWARD from 1566)  */
    target_state_index = QUEX_LABEL(290); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_375:
    /* (RELOAD_FORWARD from 1436)  */
    target_state_index = QUEX_LABEL(374); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_269:
    /* (RELOAD_FORWARD from 1553)  */
    target_state_index = QUEX_LABEL(268); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_385:
    /* (RELOAD_FORWARD from 1444)  */
    target_state_index = QUEX_LABEL(384); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_359:
    /* (RELOAD_FORWARD from 1428)  */
    target_state_index = QUEX_LABEL(358); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_491:
    /* (RELOAD_FORWARD from 1509)  */
    target_state_index = QUEX_LABEL(490); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_305:
    /* (RELOAD_FORWARD from 1574)  */
    target_state_index = QUEX_LABEL(304); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_535:
    /* (RELOAD_FORWARD from 1535)  */
    target_state_index = QUEX_LABEL(534); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_437:
    /* (RELOAD_FORWARD from 1477)  */
    target_state_index = QUEX_LABEL(436); target_state_else_index = QUEX_LABEL(206);

    goto _910;


    __quex_assert_no_passage();
_271:
    /* (RELOAD_FORWARD from 1554)  */
    target_state_index = QUEX_LABEL(270); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_523:
    /* (RELOAD_FORWARD from 1526)  */
    target_state_index = QUEX_LABEL(522); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_403:
    /* (RELOAD_FORWARD from 1455)  */
    target_state_index = QUEX_LABEL(402); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_249:
    /* (RELOAD_FORWARD from 1542)  */
    target_state_index = QUEX_LABEL(248); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_381:
    /* (RELOAD_FORWARD from 1441)  */
    target_state_index = QUEX_LABEL(380); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_259:
    /* (RELOAD_FORWARD from 1548)  */
    target_state_index = QUEX_LABEL(258); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_471:
    /* (RELOAD_FORWARD from 1498)  */
    target_state_index = QUEX_LABEL(470); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_365:
    /* (RELOAD_FORWARD from 1431)  */
    target_state_index = QUEX_LABEL(364); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_481:
    /* (RELOAD_FORWARD from 1503)  */
    target_state_index = QUEX_LABEL(480); target_state_else_index = QUEX_LABEL(220);

    goto _910;


    __quex_assert_no_passage();
_327:
    /* (RELOAD_FORWARD from 1407)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(326);

    goto _910;


    __quex_assert_no_passage();
_459:
    /* (RELOAD_FORWARD from 1491)  */
    target_state_index = QUEX_LABEL(458); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_503:
    /* (RELOAD_FORWARD from 1516)  */
    target_state_index = QUEX_LABEL(502); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_277:
    /* (RELOAD_FORWARD from 1557)  */
    target_state_index = QUEX_LABEL(276); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_513:
    /* (RELOAD_FORWARD from 1521)  */
    target_state_index = QUEX_LABEL(512); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_425:
    /* (RELOAD_FORWARD from 1471)  */
    target_state_index = QUEX_LABEL(424); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_239:
    /* (RELOAD_FORWARD from 1536)  */
    target_state_index = QUEX_LABEL(238); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_371:
    /* (RELOAD_FORWARD from 1434)  */
    target_state_index = QUEX_LABEL(370); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_345:
    /* (RELOAD_FORWARD from 1419)  */
    target_state_index = QUEX_LABEL(344); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_477:
    /* (RELOAD_FORWARD from 1501)  */
    target_state_index = QUEX_LABEL(476); target_state_else_index = QUEX_LABEL(218);

    goto _910;


    __quex_assert_no_passage();
_355:
    /* (RELOAD_FORWARD from 1426)  */
    target_state_index = QUEX_LABEL(354); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_439:
    /* (RELOAD_FORWARD from 1478)  */
    target_state_index = QUEX_LABEL(438); target_state_else_index = QUEX_LABEL(207);

    goto _910;


    __quex_assert_no_passage();
_333:
    /* (RELOAD_FORWARD from 1410)  */
    target_state_index = QUEX_LABEL(332); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_531:
    /* (RELOAD_FORWARD from 1532)  */
    target_state_index = QUEX_LABEL(530); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_449:
    /* (RELOAD_FORWARD from 1484)  */
    target_state_index = QUEX_LABEL(448); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_509:
    /* (RELOAD_FORWARD from 1519)  */
    target_state_index = QUEX_LABEL(508); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_299:
    /* (RELOAD_FORWARD from 1571)  */
    target_state_index = QUEX_LABEL(298); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_415:
    /* (RELOAD_FORWARD from 1464)  */
    target_state_index = QUEX_LABEL(414); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_245:
    /* (RELOAD_FORWARD from 1539)  */
    target_state_index = QUEX_LABEL(244); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_393:
    /* (RELOAD_FORWARD from 1448)  */
    target_state_index = QUEX_LABEL(392); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_335:
    /* (RELOAD_FORWARD from 1411)  */
    target_state_index = QUEX_LABEL(334); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_467:
    /* (RELOAD_FORWARD from 1496)  */
    target_state_index = QUEX_LABEL(466); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_313:
    /* (RELOAD_FORWARD from 1579)  */
    target_state_index = QUEX_LABEL(312); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_445:
    /* (RELOAD_FORWARD from 1481)  */
    target_state_index = QUEX_LABEL(444); target_state_else_index = QUEX_LABEL(210);

    goto _910;


    __quex_assert_no_passage();
_323:
    /* (RELOAD_FORWARD from 1586)  */
    target_state_index = QUEX_LABEL(322); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_279:
    /* (RELOAD_FORWARD from 1559)  */
    target_state_index = QUEX_LABEL(278); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_499:
    /* (RELOAD_FORWARD from 1513)  */
    target_state_index = QUEX_LABEL(498); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_411:
    /* (RELOAD_FORWARD from 1460)  */
    target_state_index = QUEX_LABEL(410); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_289:
    /* (RELOAD_FORWARD from 1565)  */
    target_state_index = QUEX_LABEL(288); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_421:
    /* (RELOAD_FORWARD from 1469)  */
    target_state_index = QUEX_LABEL(420); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_267:
    /* (RELOAD_FORWARD from 1552)  */
    target_state_index = QUEX_LABEL(266); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_383:
    /* (RELOAD_FORWARD from 1442)  */
    target_state_index = QUEX_LABEL(382); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_341:
    /* (RELOAD_FORWARD from 1414)  */
    target_state_index = QUEX_LABEL(340); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_489:
    /* (RELOAD_FORWARD from 1508)  */
    target_state_index = QUEX_LABEL(488); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_303:
    /* (RELOAD_FORWARD from 1573)  */
    target_state_index = QUEX_LABEL(302); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_435:
    /* (RELOAD_FORWARD from 1476)  */
    target_state_index = QUEX_LABEL(434); target_state_else_index = QUEX_LABEL(205);

    goto _910;


    __quex_assert_no_passage();
_285:
    /* (RELOAD_FORWARD from 1562)  */
    target_state_index = QUEX_LABEL(284); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_521:
    /* (RELOAD_FORWARD from 1525)  */
    target_state_index = QUEX_LABEL(520); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_401:
    /* (RELOAD_FORWARD from 1453)  */
    target_state_index = QUEX_LABEL(400); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_247:
    /* (RELOAD_FORWARD from 1540)  */
    target_state_index = QUEX_LABEL(246); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_379:
    /* (RELOAD_FORWARD from 1439)  */
    target_state_index = QUEX_LABEL(378); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_257:
    /* (RELOAD_FORWARD from 1547)  */
    target_state_index = QUEX_LABEL(256); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_389:
    /* (RELOAD_FORWARD from 1446)  */
    target_state_index = QUEX_LABEL(388); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_363:
    /* (RELOAD_FORWARD from 1430)  */
    target_state_index = QUEX_LABEL(362); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_479:
    /* (RELOAD_FORWARD from 1502)  */
    target_state_index = QUEX_LABEL(478); target_state_else_index = QUEX_LABEL(219);

    goto _910;


    __quex_assert_no_passage();
_309:
    /* (RELOAD_FORWARD from 1577)  */
    target_state_index = QUEX_LABEL(308); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_457:
    /* (RELOAD_FORWARD from 1490)  */
    target_state_index = QUEX_LABEL(456); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_275:
    /* (RELOAD_FORWARD from 1556)  */
    target_state_index = QUEX_LABEL(274); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_511:
    /* (RELOAD_FORWARD from 1520)  */
    target_state_index = QUEX_LABEL(510); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_423:
    /* (RELOAD_FORWARD from 1470)  */
    target_state_index = QUEX_LABEL(422); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_253:
    /* (RELOAD_FORWARD from 1544)  */
    target_state_index = QUEX_LABEL(252); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_369:
    /* (RELOAD_FORWARD from 1433)  */
    target_state_index = QUEX_LABEL(368); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_343:
    /* (RELOAD_FORWARD from 1417)  */
    target_state_index = QUEX_LABEL(342); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_475:
    /* (RELOAD_FORWARD from 1500)  */
    target_state_index = QUEX_LABEL(474); target_state_else_index = QUEX_LABEL(217);

    goto _910;


    __quex_assert_no_passage();
_353:
    /* (RELOAD_FORWARD from 1425)  */
    target_state_index = QUEX_LABEL(352); target_state_else_index = QUEX_LABEL(191);

    goto _910;


    __quex_assert_no_passage();
_485:
    /* (RELOAD_FORWARD from 1505)  */
    target_state_index = QUEX_LABEL(484); target_state_else_index = QUEX_LABEL(222);

    goto _910;


    __quex_assert_no_passage();
_331:
    /* (RELOAD_FORWARD from 1409)  */
    target_state_index = QUEX_LABEL(330); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_529:
    /* (RELOAD_FORWARD from 1531)  */
    target_state_index = QUEX_LABEL(528); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_447:
    /* (RELOAD_FORWARD from 1482)  */
    target_state_index = QUEX_LABEL(446); target_state_else_index = QUEX_LABEL(211);

    goto _910;


    __quex_assert_no_passage();
_507:
    /* (RELOAD_FORWARD from 1518)  */
    target_state_index = QUEX_LABEL(506); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_297:
    /* (RELOAD_FORWARD from 1570)  */
    target_state_index = QUEX_LABEL(296); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_517:
    /* (RELOAD_FORWARD from 1523)  */
    target_state_index = QUEX_LABEL(516); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_429:
    /* (RELOAD_FORWARD from 1473)  */
    target_state_index = QUEX_LABEL(428); target_state_else_index = QUEX_LABEL(202);

    goto _910;


    __quex_assert_no_passage();
_243:
    /* (RELOAD_FORWARD from 1538)  */
    target_state_index = QUEX_LABEL(242); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_391:
    /* (RELOAD_FORWARD from 1447)  */
    target_state_index = QUEX_LABEL(390); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_349:
    /* (RELOAD_FORWARD from 1423)  */
    target_state_index = QUEX_LABEL(348); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_465:
    /* (RELOAD_FORWARD from 1495)  */
    target_state_index = QUEX_LABEL(464); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_311:
    /* (RELOAD_FORWARD from 1578)  */
    target_state_index = QUEX_LABEL(310); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_443:
    /* (RELOAD_FORWARD from 1480)  */
    target_state_index = QUEX_LABEL(442); target_state_else_index = QUEX_LABEL(209);

    goto _910;


    __quex_assert_no_passage();
_321:
    /* (RELOAD_FORWARD from 1584)  */
    target_state_index = QUEX_LABEL(320); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_453:
    /* (RELOAD_FORWARD from 1487)  */
    target_state_index = QUEX_LABEL(452); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_497:
    /* (RELOAD_FORWARD from 1512)  */
    target_state_index = QUEX_LABEL(496); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_409:
    /* (RELOAD_FORWARD from 1458)  */
    target_state_index = QUEX_LABEL(408); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_287:
    /* (RELOAD_FORWARD from 1563)  */
    target_state_index = QUEX_LABEL(286); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_419:
    /* (RELOAD_FORWARD from 1468)  */
    target_state_index = QUEX_LABEL(418); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_265:
    /* (RELOAD_FORWARD from 1551)  */
    target_state_index = QUEX_LABEL(264); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_397:
    /* (RELOAD_FORWARD from 1451)  */
    target_state_index = QUEX_LABEL(396); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_339:
    /* (RELOAD_FORWARD from 1413)  */
    target_state_index = QUEX_LABEL(338); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_487:
    /* (RELOAD_FORWARD from 1506)  */
    target_state_index = QUEX_LABEL(486); target_state_else_index = QUEX_LABEL(223);

    goto _910;


    __quex_assert_no_passage();
_317:
    /* (RELOAD_FORWARD from 1582)  */
    target_state_index = QUEX_LABEL(316); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_433:
    /* (RELOAD_FORWARD from 1475)  */
    target_state_index = QUEX_LABEL(432); target_state_else_index = QUEX_LABEL(204);

    goto _910;


    __quex_assert_no_passage();
_283:
    /* (RELOAD_FORWARD from 1561)  */
    target_state_index = QUEX_LABEL(282); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_519:
    /* (RELOAD_FORWARD from 1524)  */
    target_state_index = QUEX_LABEL(518); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_399:
    /* (RELOAD_FORWARD from 1452)  */
    target_state_index = QUEX_LABEL(398); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_293:
    /* (RELOAD_FORWARD from 1567)  */
    target_state_index = QUEX_LABEL(292); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_377:
    /* (RELOAD_FORWARD from 1437)  */
    target_state_index = QUEX_LABEL(376); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_255:
    /* (RELOAD_FORWARD from 1546)  */
    target_state_index = QUEX_LABEL(254); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_387:
    /* (RELOAD_FORWARD from 1445)  */
    target_state_index = QUEX_LABEL(386); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_361:
    /* (RELOAD_FORWARD from 1429)  */
    target_state_index = QUEX_LABEL(360); target_state_else_index = QUEX_LABEL(185);

    goto _910;


    __quex_assert_no_passage();
_493:
    /* (RELOAD_FORWARD from 1510)  */
    target_state_index = QUEX_LABEL(492); target_state_else_index = QUEX_LABEL(185);

    goto _910;

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


goto _911;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _912; /* in QUEX_GOTO_STATE       */
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
    /* (1670 from RELOAD_FORWARD) (1670 from BEFORE_ENTRY)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(1670);
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
    /* (DROP_OUT from 1673) (DROP_OUT from 1671)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 1678)  */
    goto _34;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 1675)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 1674)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 1672)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 1670)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 1677)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 1676)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (1671 from 1670)  */
    ++(me->buffer._read_p);

_39:
    input = *(me->buffer._read_p);


    __quex_debug_state(1671);
switch( input ) {
case 0x0: goto _23;
case 0x22: goto _7;
case 0x5C: goto _8;
case 0x78: goto _9;
default: goto _13;
}


    __quex_assert_no_passage();
_22:
    /* (1671 from RELOAD_FORWARD)  */
    goto _39;


    __quex_assert_no_passage();
_5:
    /* (1672 from 1670)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1672);
goto _14;


    __quex_assert_no_passage();
_6:
    /* (1673 from 1670)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1673);
goto _13;


    __quex_assert_no_passage();
_7:
    /* (1674 from 1671)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1674);
goto _15;


    __quex_assert_no_passage();
_8:
    /* (1675 from 1671)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1675);
goto _16;


    __quex_assert_no_passage();
_24:
    /* (1676 from RELOAD_FORWARD)  */
_45:
    input = *(me->buffer._read_p);


    __quex_debug_state(1676);
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
    /* (1676 from 1671)  */
    ++(me->buffer._read_p);

    goto _45;


    __quex_assert_no_passage();
_26:
    /* (1677 from RELOAD_FORWARD)  */
_47:
    input = *(me->buffer._read_p);


    __quex_debug_state(1677);
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
    /* (1677 from 1676)  */
    ++(me->buffer._read_p);

    goto _47;


    __quex_assert_no_passage();
_11:
    /* (1678 from 1677)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1678);
goto _19;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_20:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 7498 "Lexer.cpp"

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


#   line 7512 "Lexer.cpp"

}
goto _2;
_36:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 99 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 7527 "Lexer.cpp"

}
goto _0;
_35:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 102 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 7542 "Lexer.cpp"

}
goto _0;
_34:
    __quex_debug("* TERMINAL \"\\\\x\"[0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 105 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 7557 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 110 "Lexer.qx"
    self << RULE;
    RETURN;


#   line 7572 "Lexer.cpp"

}
goto _0;
_32:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 112 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 7586 "Lexer.cpp"

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
    /* (RELOAD_FORWARD from 1676)  */
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
    /* (RELOAD_FORWARD from 1670)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(20);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 1677)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(18);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 1671)  */
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
   static const char  token_id_str_CONDITION_IDENTIFIER[]    = "CONDITION_IDENTIFIER";
   static const char  token_id_str_CURLYBRACE_CLOSE[]        = "CURLYBRACE_CLOSE";
   static const char  token_id_str_CURLYBRACE_OPEN[]         = "CURLYBRACE_OPEN";
   static const char  token_id_str_DEVICE_ANYID[]            = "DEVICE_ANYID";
   static const char  token_id_str_DEVICE_VID[]              = "DEVICE_VID";
   static const char  token_id_str_DEVICE_VPID[]             = "DEVICE_VPID";
   static const char  token_id_str_DQ_STRING[]               = "DQ_STRING";
   static const char  token_id_str_DQ_STRING_BEGIN[]         = "DQ_STRING_BEGIN";
   static const char  token_id_str_DQ_STRING_END[]           = "DQ_STRING_END";
   static const char  token_id_str_INTERFACE_TYPE_CLASS[]    = "INTERFACE_TYPE_CLASS";
   static const char  token_id_str_INTERFACE_TYPE_FULL[]     = "INTERFACE_TYPE_FULL";
   static const char  token_id_str_INTERFACE_TYPE_SUBCLASS[] = "INTERFACE_TYPE_SUBCLASS";
   static const char  token_id_str_KEYWORD_ALLOW[]           = "KEYWORD_ALLOW";
   static const char  token_id_str_KEYWORD_BLOCK[]           = "KEYWORD_BLOCK";
   static const char  token_id_str_KEYWORD_DEVICE[]          = "KEYWORD_DEVICE";
   static const char  token_id_str_KEYWORD_HASH[]            = "KEYWORD_HASH";
   static const char  token_id_str_KEYWORD_IF[]              = "KEYWORD_IF";
   static const char  token_id_str_KEYWORD_MATCH[]           = "KEYWORD_MATCH";
   static const char  token_id_str_KEYWORD_NAME[]            = "KEYWORD_NAME";
   static const char  token_id_str_KEYWORD_REJECT[]          = "KEYWORD_REJECT";
   static const char  token_id_str_KEYWORD_SERIAL[]          = "KEYWORD_SERIAL";
   static const char  token_id_str_KEYWORD_VIAPORT[]         = "KEYWORD_VIAPORT";
   static const char  token_id_str_KEYWORD_WITHINTERFACE[]   = "KEYWORD_WITHINTERFACE";
   static const char  token_id_str_NEGATION[]                = "NEGATION";
   static const char  token_id_str_PQ_STRING[]               = "PQ_STRING";
   static const char  token_id_str_PQ_STRING_BEGIN[]         = "PQ_STRING_BEGIN";
   static const char  token_id_str_PQ_STRING_END[]           = "PQ_STRING_END";
   static const char  token_id_str_SET_OPERATOR[]            = "SET_OPERATOR";
       

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
   case RULE_TOKEN_CONDITION_IDENTIFIER:    return token_id_str_CONDITION_IDENTIFIER;
   case RULE_TOKEN_CURLYBRACE_CLOSE:        return token_id_str_CURLYBRACE_CLOSE;
   case RULE_TOKEN_CURLYBRACE_OPEN:         return token_id_str_CURLYBRACE_OPEN;
   case RULE_TOKEN_DEVICE_ANYID:            return token_id_str_DEVICE_ANYID;
   case RULE_TOKEN_DEVICE_VID:              return token_id_str_DEVICE_VID;
   case RULE_TOKEN_DEVICE_VPID:             return token_id_str_DEVICE_VPID;
   case RULE_TOKEN_DQ_STRING:               return token_id_str_DQ_STRING;
   case RULE_TOKEN_DQ_STRING_BEGIN:         return token_id_str_DQ_STRING_BEGIN;
   case RULE_TOKEN_DQ_STRING_END:           return token_id_str_DQ_STRING_END;
   case RULE_TOKEN_INTERFACE_TYPE_CLASS:    return token_id_str_INTERFACE_TYPE_CLASS;
   case RULE_TOKEN_INTERFACE_TYPE_FULL:     return token_id_str_INTERFACE_TYPE_FULL;
   case RULE_TOKEN_INTERFACE_TYPE_SUBCLASS: return token_id_str_INTERFACE_TYPE_SUBCLASS;
   case RULE_TOKEN_KEYWORD_ALLOW:           return token_id_str_KEYWORD_ALLOW;
   case RULE_TOKEN_KEYWORD_BLOCK:           return token_id_str_KEYWORD_BLOCK;
   case RULE_TOKEN_KEYWORD_DEVICE:          return token_id_str_KEYWORD_DEVICE;
   case RULE_TOKEN_KEYWORD_HASH:            return token_id_str_KEYWORD_HASH;
   case RULE_TOKEN_KEYWORD_IF:              return token_id_str_KEYWORD_IF;
   case RULE_TOKEN_KEYWORD_MATCH:           return token_id_str_KEYWORD_MATCH;
   case RULE_TOKEN_KEYWORD_NAME:            return token_id_str_KEYWORD_NAME;
   case RULE_TOKEN_KEYWORD_REJECT:          return token_id_str_KEYWORD_REJECT;
   case RULE_TOKEN_KEYWORD_SERIAL:          return token_id_str_KEYWORD_SERIAL;
   case RULE_TOKEN_KEYWORD_VIAPORT:         return token_id_str_KEYWORD_VIAPORT;
   case RULE_TOKEN_KEYWORD_WITHINTERFACE:   return token_id_str_KEYWORD_WITHINTERFACE;
   case RULE_TOKEN_NEGATION:                return token_id_str_NEGATION;
   case RULE_TOKEN_PQ_STRING:               return token_id_str_PQ_STRING;
   case RULE_TOKEN_PQ_STRING_BEGIN:         return token_id_str_PQ_STRING_BEGIN;
   case RULE_TOKEN_PQ_STRING_END:           return token_id_str_PQ_STRING_END;
   case RULE_TOKEN_SET_OPERATOR:            return token_id_str_SET_OPERATOR;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

