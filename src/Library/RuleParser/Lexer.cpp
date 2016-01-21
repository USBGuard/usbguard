
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

    /* (750 from BEFORE_ENTRY)  */
    input = *(me->buffer._read_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._read_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(750);
if     ( input >= 0xB )  goto _4;
else if( input == 0xA )  goto _2;
else if( input == 0x9 )  goto _3;
else                     goto _4;


    __quex_assert_no_passage();
_8:
    /* (750 from 754)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 752)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 751)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 753)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (752 from 750)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(752);
goto _6;


    __quex_assert_no_passage();
_3:
    /* (753 from 750)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(753);
goto _7;


    __quex_assert_no_passage();
_4:
    /* (751 from 750)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(751);
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
    /* (720 from RELOAD_FORWARD) (720 from BEFORE_ENTRY)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(720);
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
    /* (DROP_OUT from 721) (DROP_OUT from 722)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 728)  */
    goto _34;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 725)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 724)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 723)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 720)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 727)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 726)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (721 from 720)  */
    ++(me->buffer._read_p);

_39:
    input = *(me->buffer._read_p);


    __quex_debug_state(721);
switch( input ) {
case 0x0: goto _23;
case 0x28: goto _7;
case 0x5C: goto _8;
case 0x78: goto _9;
default: goto _13;
}


    __quex_assert_no_passage();
_22:
    /* (721 from RELOAD_FORWARD)  */
    goto _39;


    __quex_assert_no_passage();
_5:
    /* (722 from 720)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(722);
goto _13;


    __quex_assert_no_passage();
_6:
    /* (723 from 720)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(723);
goto _14;


    __quex_assert_no_passage();
_7:
    /* (724 from 721)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(724);
goto _15;


    __quex_assert_no_passage();
_8:
    /* (725 from 721)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(725);
goto _16;


    __quex_assert_no_passage();
_24:
    /* (726 from RELOAD_FORWARD)  */
_45:
    input = *(me->buffer._read_p);


    __quex_debug_state(726);
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
    /* (726 from 721)  */
    ++(me->buffer._read_p);

    goto _45;


    __quex_assert_no_passage();
_10:
    /* (727 from 726)  */
    ++(me->buffer._read_p);

_47:
    input = *(me->buffer._read_p);


    __quex_debug_state(727);
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
    /* (727 from RELOAD_FORWARD)  */
    goto _47;


    __quex_assert_no_passage();
_11:
    /* (728 from 727)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(728);
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

#   line 128 "Lexer.qx"
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

#   line 131 "Lexer.qx"
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

#   line 134 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 830 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \")\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 139 "Lexer.qx"
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

#   line 141 "Lexer.qx"
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
    /* (RELOAD_FORWARD from 726)  */
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
    /* (RELOAD_FORWARD from 720)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(20);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 727)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(18);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 721)  */
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
#   define QUEX_LABEL_STATE_ROUTER _814
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
_813:
    me->buffer._lexeme_start_p = me->buffer._read_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);
_3:
    /* (1291 from RELOAD_FORWARD) (1291 from BEFORE_ENTRY)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(1291);
switch( input ) {
case 0x0: goto _217;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _6;
case 0x21: goto _14;
case 0x22: goto _13;
case 0x28: goto _5;
case 0x2A: goto _25;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _24;
case 0x61: goto _18;
case 0x62: goto _16;
case 0x63: goto _24;
case 0x64: goto _23;
case 0x65: goto _17;
case 0x66: goto _12;
case 0x68: goto _27;
case 0x69: goto _26;
case 0x6C: goto _20;
case 0x6D: goto _8;
case 0x6E: goto _15;
case 0x6F: goto _21;
case 0x72: goto _9;
case 0x73: goto _22;
case 0x74: goto _4;
case 0x76: goto _7;
case 0x77: goto _19;
case 0x7B: goto _10;
case 0x7D: goto _11;
default: goto _165;
}


    __quex_assert_no_passage();
_165:
    /* (DROP_OUT from 1307) (DROP_OUT from 1447) (DROP_OUT from 1429) (DROP_OUT from 1322) (DROP_OUT from 1304) (DROP_OUT from 1444) (DROP_OUT from 1426) (DROP_OUT from 1401) (DROP_OUT from 1367) (DROP_OUT from 1441) (DROP_OUT from 1439) (DROP_OUT from 1364) (DROP_OUT from 1436) (DROP_OUT from 1379) (DROP_OUT from 1361) (DROP_OUT from 1295) (DROP_OUT from 1451) (DROP_OUT from 1433) (DROP_OUT from 1374) (DROP_OUT from 1292) (DROP_OUT from 1448) (DROP_OUT from 1414) (DROP_OUT from 1371) (DROP_OUT from 1368) (DROP_OUT from 1334) (DROP_OUT from 1349) (DROP_OUT from 1421) (DROP_OUT from 1346) (DROP_OUT from 1418) (DROP_OUT from 1359) (DROP_OUT from 1341) (DROP_OUT from 1356) (DROP_OUT from 1338) (DROP_OUT from 1353) (DROP_OUT from 1316) (DROP_OUT from 1395) (DROP_OUT from 1313) (DROP_OUT from 1311) (DROP_OUT from 1326) (DROP_OUT from 1308) (DROP_OUT from 1430) (DROP_OUT from 1323) (DROP_OUT from 1305) (DROP_OUT from 1445) (DROP_OUT from 1427) (DROP_OUT from 1402) (DROP_OUT from 1320) (DROP_OUT from 1365) (DROP_OUT from 1437) (DROP_OUT from 1380) (DROP_OUT from 1362) (DROP_OUT from 1377) (DROP_OUT from 1375) (DROP_OUT from 1372) (DROP_OUT from 1412) (DROP_OUT from 1369) (DROP_OUT from 1332) (DROP_OUT from 1347) (DROP_OUT from 1329) (DROP_OUT from 1419) (DROP_OUT from 1344) (DROP_OUT from 1342) (DROP_OUT from 1416) (DROP_OUT from 1357) (DROP_OUT from 1354) (DROP_OUT from 1336) (DROP_OUT from 1399) (DROP_OUT from 1317) (DROP_OUT from 1396) (DROP_OUT from 1314) (DROP_OUT from 1296) (DROP_OUT from 1393) (DROP_OUT from 1327) (DROP_OUT from 1309) (DROP_OUT from 1431) (DROP_OUT from 1324) (DROP_OUT from 1306) (DROP_OUT from 1446) (DROP_OUT from 1443) (DROP_OUT from 1425) (DROP_OUT from 1400) (DROP_OUT from 1366) (DROP_OUT from 1440) (DROP_OUT from 1381) (DROP_OUT from 1435) (DROP_OUT from 1378) (DROP_OUT from 1360) (DROP_OUT from 1450) (DROP_OUT from 1432) (DROP_OUT from 1373) (DROP_OUT from 1291) (DROP_OUT from 1413) (DROP_OUT from 1370) (DROP_OUT from 1351) (DROP_OUT from 1333) (DROP_OUT from 1423) (DROP_OUT from 1348) (DROP_OUT from 1330) (DROP_OUT from 1420) (DROP_OUT from 1343) (DROP_OUT from 1417) (DROP_OUT from 1358) (DROP_OUT from 1337) (DROP_OUT from 1303) (DROP_OUT from 1352) (DROP_OUT from 1300) (DROP_OUT from 1397) (DROP_OUT from 1315) (DROP_OUT from 1297) (DROP_OUT from 1312) (DROP_OUT from 1310) (DROP_OUT from 1325)  */

        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _482;
_480:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_212:
    /* (DROP_OUT from 1438)  */
    goto _484;


    __quex_assert_no_passage();
_185:
    /* (DROP_OUT from 1382)  */
    goto _483;


    __quex_assert_no_passage();
_179:
    /* (DROP_OUT from 1340)  */
    goto _485;


    __quex_assert_no_passage();
_172:
    /* (DROP_OUT from 1318)  */
    goto _486;


    __quex_assert_no_passage();
_214:
    /* (DROP_OUT from 1449)  */
    goto _487;


    __quex_assert_no_passage();
_208:
    /* (DROP_OUT from 1422)  */
    goto _488;


    __quex_assert_no_passage();
_181:
    /* (DROP_OUT from 1350)  */
    goto _489;


    __quex_assert_no_passage();
_175:
    /* (DROP_OUT from 1328)  */
    goto _490;


    __quex_assert_no_passage();
_168:
    /* (DROP_OUT from 1298)  */
    goto _492;


    __quex_assert_no_passage();
_197:
    /* (DROP_OUT from 1398)  */
    goto _493;


    __quex_assert_no_passage();
_180:
    /* (DROP_OUT from 1345)  */
    goto _494;


    __quex_assert_no_passage();
_215:
    /* (DROP_OUT from 1452)  */
    goto _495;


    __quex_assert_no_passage();
_209:
    /* (DROP_OUT from 1424)  */
    goto _496;


    __quex_assert_no_passage();
_182:
    /* (DROP_OUT from 1355)  */
    goto _497;


    __quex_assert_no_passage();
_176:
    /* (DROP_OUT from 1331)  */
    goto _498;


    __quex_assert_no_passage();
_169:
    /* (DROP_OUT from 1299)  */
    goto _499;


    __quex_assert_no_passage();
_198:
    /* (DROP_OUT from 1403)  */
    goto _491;


    __quex_assert_no_passage();
_210:
    /* (DROP_OUT from 1428)  */
    goto _500;


    __quex_assert_no_passage();
_183:
    /* (DROP_OUT from 1363)  */
    goto _501;


    __quex_assert_no_passage();
_177:
    /* (DROP_OUT from 1335)  */
    goto _502;


    __quex_assert_no_passage();
_170:
    /* (DROP_OUT from 1301)  */
    goto _503;


    __quex_assert_no_passage();
_206:
    /* (DROP_OUT from 1411)  */
    goto _504;


    __quex_assert_no_passage();
_195:
    /* (DROP_OUT from 1392)  */
    goto _505;


    __quex_assert_no_passage();
_173:
    /* (DROP_OUT from 1319)  */
    goto _506;


    __quex_assert_no_passage();
_166:
    /* (DROP_OUT from 1293)  */
    goto _507;


    __quex_assert_no_passage();
_211:
    /* (DROP_OUT from 1434)  */
    goto _508;


    __quex_assert_no_passage();
_184:
    /* (DROP_OUT from 1376)  */
    goto _509;


    __quex_assert_no_passage();
_178:
    /* (DROP_OUT from 1339)  */
    goto _510;


    __quex_assert_no_passage();
_171:
    /* (DROP_OUT from 1302)  */
    goto _511;


    __quex_assert_no_passage();
_213:
    /* (DROP_OUT from 1442)  */
    goto _512;


    __quex_assert_no_passage();
_207:
    /* (DROP_OUT from 1415)  */
    goto _513;


    __quex_assert_no_passage();
_196:
    /* (DROP_OUT from 1394)  */
    goto _514;


    __quex_assert_no_passage();
_174:
    /* (DROP_OUT from 1321)  */
    goto _515;


    __quex_assert_no_passage();
_167:
    /* (DROP_OUT from 1294)  */
    goto _516;


    __quex_assert_no_passage();
_192:
    /* (DROP_OUT from 1389)  */
        me->buffer._read_p -= 7;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_201:
    /* (DROP_OUT from 1406)  */
        me->buffer._read_p -= 3;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_193:
    /* (DROP_OUT from 1390)  */
        me->buffer._read_p -= 8;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_186:
    /* (DROP_OUT from 1383)  */
        me->buffer._read_p -= 1;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_189:
    /* (DROP_OUT from 1386)  */
        me->buffer._read_p -= 4;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_202:
    /* (DROP_OUT from 1407)  */
        me->buffer._read_p -= 4;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_205:
    /* (DROP_OUT from 1410)  */
        me->buffer._read_p -= 7;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_194:
    /* (DROP_OUT from 1391)  */
        me->buffer._read_p -= 9;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_187:
    /* (DROP_OUT from 1384)  */
        me->buffer._read_p -= 2;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_190:
    /* (DROP_OUT from 1387)  */
        me->buffer._read_p -= 5;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_203:
    /* (DROP_OUT from 1408)  */
        me->buffer._read_p -= 5;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_199:
    /* (DROP_OUT from 1404)  */
        me->buffer._read_p -= 1;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_188:
    /* (DROP_OUT from 1385)  */
        me->buffer._read_p -= 3;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_191:
    /* (DROP_OUT from 1388)  */
        me->buffer._read_p -= 6;
goto _483;
    goto _480;


    __quex_assert_no_passage();
_204:
    /* (DROP_OUT from 1409)  */
        me->buffer._read_p -= 6;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_200:
    /* (DROP_OUT from 1405)  */
        me->buffer._read_p -= 2;
goto _491;
    goto _480;


    __quex_assert_no_passage();
_218:
    /* (1292 from RELOAD_FORWARD)  */
_518:
    input = *(me->buffer._read_p);


    __quex_debug_state(1292);
if     ( input == 0x72 )  goto _162;
else if( input == 0x0 )   goto _219;
else                      goto _165;


    __quex_assert_no_passage();
_4:
    /* (1292 from 1291)  */
    ++(me->buffer._read_p);

    goto _518;


    __quex_assert_no_passage();
_5:
    /* (1293 from 1291)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1293);
goto _166;


    __quex_assert_no_passage();
_6:
    /* (1294 from 1291) (1294 from 1294)  */

    ++(me->buffer._read_p);

_522:
    input = *(me->buffer._read_p);


    __quex_debug_state(1294);
switch( input ) {
case 0x0: goto _221;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _6;
default: goto _167;
}


    __quex_assert_no_passage();
_220:
    /* (1294 from RELOAD_FORWARD)  */
    goto _522;


    __quex_assert_no_passage();
_7:
    /* (1295 from 1291)  */
    ++(me->buffer._read_p);

_524:
    input = *(me->buffer._read_p);


    __quex_debug_state(1295);
if     ( input == 0x69 )  goto _155;
else if( input == 0x0 )   goto _223;
else                      goto _165;


    __quex_assert_no_passage();
_222:
    /* (1295 from RELOAD_FORWARD)  */
    goto _524;


    __quex_assert_no_passage();
_224:
    /* (1296 from RELOAD_FORWARD)  */
_526:
    input = *(me->buffer._read_p);


    __quex_debug_state(1296);
if     ( input == 0x61 )  goto _151;
else if( input == 0x0 )   goto _225;
else                      goto _165;


    __quex_assert_no_passage();
_8:
    /* (1296 from 1291)  */
    ++(me->buffer._read_p);

    goto _526;


    __quex_assert_no_passage();
_9:
    /* (1297 from 1291)  */
    ++(me->buffer._read_p);

_528:
    input = *(me->buffer._read_p);


    __quex_debug_state(1297);
switch( input ) {
case 0x0: goto _227;
case 0x61: goto _142;
case 0x65: goto _141;
default: goto _165;
}


    __quex_assert_no_passage();
_226:
    /* (1297 from RELOAD_FORWARD)  */
    goto _528;


    __quex_assert_no_passage();
_10:
    /* (1298 from 1291)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1298);
goto _168;


    __quex_assert_no_passage();
_11:
    /* (1299 from 1291)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1299);
goto _169;


    __quex_assert_no_passage();
_228:
    /* (1300 from RELOAD_FORWARD)  */
_532:
    input = *(me->buffer._read_p);


    __quex_debug_state(1300);
switch( input ) {
case 0x0: goto _229;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _34;
case 0x61: goto _137;
case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
default: goto _165;
}


    __quex_assert_no_passage();
_12:
    /* (1300 from 1291)  */
    ++(me->buffer._read_p);

    goto _532;


    __quex_assert_no_passage();
_13:
    /* (1301 from 1291)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1301);
goto _170;


    __quex_assert_no_passage();
_14:
    /* (1302 from 1291)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1302);
goto _171;


    __quex_assert_no_passage();
_15:
    /* (1303 from 1291)  */
    ++(me->buffer._read_p);

_536:
    input = *(me->buffer._read_p);


    __quex_debug_state(1303);
switch( input ) {
case 0x0: goto _231;
case 0x61: goto _128;
case 0x6F: goto _129;
default: goto _165;
}


    __quex_assert_no_passage();
_230:
    /* (1303 from RELOAD_FORWARD)  */
    goto _536;


    __quex_assert_no_passage();
_232:
    /* (1304 from RELOAD_FORWARD)  */
_538:
    input = *(me->buffer._read_p);


    __quex_debug_state(1304);
switch( input ) {
case 0x0: goto _233;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
case 0x6C: goto _124;
default: goto _165;
}


    __quex_assert_no_passage();
_16:
    /* (1304 from 1291)  */
    ++(me->buffer._read_p);

    goto _538;


    __quex_assert_no_passage();
_17:
    /* (1305 from 1291)  */
    ++(me->buffer._read_p);

_540:
    input = *(me->buffer._read_p);


    __quex_debug_state(1305);
switch( input ) {
case 0x0: goto _235;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
case 0x71: goto _111;
default: goto _165;
}


    __quex_assert_no_passage();
_234:
    /* (1305 from RELOAD_FORWARD)  */
    goto _540;


    __quex_assert_no_passage();
_18:
    /* (1306 from 1291)  */
    ++(me->buffer._read_p);

_542:
    input = *(me->buffer._read_p);


    __quex_debug_state(1306);
switch( input ) {
case 0x0: goto _237;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
case 0x6C: goto _90;
case 0x6E: goto _89;
default: goto _165;
}


    __quex_assert_no_passage();
_236:
    /* (1306 from RELOAD_FORWARD)  */
    goto _542;


    __quex_assert_no_passage();
_19:
    /* (1307 from 1291)  */
    ++(me->buffer._read_p);

_544:
    input = *(me->buffer._read_p);


    __quex_debug_state(1307);
if     ( input == 0x69 )  goto _76;
else if( input == 0x0 )   goto _239;
else                      goto _165;


    __quex_assert_no_passage();
_238:
    /* (1307 from RELOAD_FORWARD)  */
    goto _544;


    __quex_assert_no_passage();
_240:
    /* (1308 from RELOAD_FORWARD)  */
_546:
    input = *(me->buffer._read_p);


    __quex_debug_state(1308);
if     ( input == 0x6F )  goto _68;
else if( input == 0x0 )   goto _241;
else                      goto _165;


    __quex_assert_no_passage();
_20:
    /* (1308 from 1291)  */
    ++(me->buffer._read_p);

    goto _546;


    __quex_assert_no_passage();
_242:
    /* (1309 from RELOAD_FORWARD)  */
_548:
    input = *(me->buffer._read_p);


    __quex_debug_state(1309);
if     ( input == 0x6E )  goto _63;
else if( input == 0x0 )   goto _243;
else                      goto _165;


    __quex_assert_no_passage();
_21:
    /* (1309 from 1291)  */
    ++(me->buffer._read_p);

    goto _548;


    __quex_assert_no_passage();
_244:
    /* (1310 from RELOAD_FORWARD)  */
_550:
    input = *(me->buffer._read_p);


    __quex_debug_state(1310);
if     ( input == 0x65 )  goto _58;
else if( input == 0x0 )   goto _245;
else                      goto _165;


    __quex_assert_no_passage();
_22:
    /* (1310 from 1291)  */
    ++(me->buffer._read_p);

    goto _550;


    __quex_assert_no_passage();
_23:
    /* (1311 from 1291)  */
    ++(me->buffer._read_p);

_552:
    input = *(me->buffer._read_p);


    __quex_debug_state(1311);
switch( input ) {
case 0x0: goto _247;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: goto _34;
case 0x65: goto _53;
case 0x66: goto _34;
default: goto _165;
}


    __quex_assert_no_passage();
_246:
    /* (1311 from RELOAD_FORWARD)  */
    goto _552;


    __quex_assert_no_passage();
_248:
    /* (1312 from RELOAD_FORWARD)  */
_554:
    input = *(me->buffer._read_p);


    __quex_debug_state(1312);
switch( input ) {
case 0x0: goto _249;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
default: goto _165;
}


    __quex_assert_no_passage();
_24:
    /* (1312 from 1291)  */
    ++(me->buffer._read_p);

    goto _554;


    __quex_assert_no_passage();
_25:
    /* (1313 from 1291)  */
    ++(me->buffer._read_p);

_556:
    input = *(me->buffer._read_p);


    __quex_debug_state(1313);
if     ( input == 0x3A )  goto _32;
else if( input == 0x0 )   goto _251;
else                      goto _165;


    __quex_assert_no_passage();
_250:
    /* (1313 from RELOAD_FORWARD)  */
    goto _556;


    __quex_assert_no_passage();
_26:
    /* (1314 from 1291)  */
    ++(me->buffer._read_p);

_558:
    input = *(me->buffer._read_p);


    __quex_debug_state(1314);
if     ( input == 0x66 )  goto _31;
else if( input == 0x0 )   goto _253;
else                      goto _165;


    __quex_assert_no_passage();
_252:
    /* (1314 from RELOAD_FORWARD)  */
    goto _558;


    __quex_assert_no_passage();
_27:
    /* (1315 from 1291)  */
    ++(me->buffer._read_p);

_560:
    input = *(me->buffer._read_p);


    __quex_debug_state(1315);
if     ( input == 0x61 )  goto _28;
else if( input == 0x0 )   goto _255;
else                      goto _165;


    __quex_assert_no_passage();
_254:
    /* (1315 from RELOAD_FORWARD)  */
    goto _560;


    __quex_assert_no_passage();
_256:
    /* (1316 from RELOAD_FORWARD)  */
_562:
    input = *(me->buffer._read_p);


    __quex_debug_state(1316);
if     ( input == 0x73 )  goto _29;
else if( input == 0x0 )   goto _257;
else                      goto _165;


    __quex_assert_no_passage();
_28:
    /* (1316 from 1315)  */
    ++(me->buffer._read_p);

    goto _562;


    __quex_assert_no_passage();
_258:
    /* (1317 from RELOAD_FORWARD)  */
_564:
    input = *(me->buffer._read_p);


    __quex_debug_state(1317);
if     ( input == 0x68 )  goto _30;
else if( input == 0x0 )   goto _259;
else                      goto _165;


    __quex_assert_no_passage();
_29:
    /* (1317 from 1316)  */
    ++(me->buffer._read_p);

    goto _564;


    __quex_assert_no_passage();
_30:
    /* (1318 from 1317)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1318);
goto _172;


    __quex_assert_no_passage();
_31:
    /* (1319 from 1314)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1319);
goto _173;


    __quex_assert_no_passage();
_260:
    /* (1320 from RELOAD_FORWARD)  */
_568:
    input = *(me->buffer._read_p);


    __quex_debug_state(1320);
if     ( input == 0x2A )  goto _33;
else if( input == 0x0 )   goto _261;
else                      goto _165;


    __quex_assert_no_passage();
_32:
    /* (1320 from 1313)  */
    ++(me->buffer._read_p);

    goto _568;


    __quex_assert_no_passage();
_33:
    /* (1321 from 1320)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1321);
goto _174;


    __quex_assert_no_passage();
_34:
    /* (1322 from 1305) (1322 from 1300) (1322 from 1306) (1322 from 1312) (1322 from 1311) (1322 from 1304)  */

    ++(me->buffer._read_p);

_572:
    input = *(me->buffer._read_p);


    __quex_debug_state(1322);
switch( input ) {
case 0x0: goto _263;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _36;
case 0x3A: goto _35;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _36;
default: goto _165;
}


    __quex_assert_no_passage();
_262:
    /* (1322 from RELOAD_FORWARD)  */
    goto _572;


    __quex_assert_no_passage();
_35:
    /* (1323 from 1322) (1323 from 1425) (1323 from 1341)  */

    ++(me->buffer._read_p);

_575:
    input = *(me->buffer._read_p);


    __quex_debug_state(1323);
switch( input ) {
case 0x0: goto _265;
case 0x2A: goto _45;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _44;
default: goto _165;
}


    __quex_assert_no_passage();
_264:
    /* (1323 from RELOAD_FORWARD)  */
    goto _575;


    __quex_assert_no_passage();
_36:
    /* (1324 from 1341) (1324 from 1322) (1324 from 1425)  */

    ++(me->buffer._read_p);

_578:
    input = *(me->buffer._read_p);


    __quex_debug_state(1324);
switch( input ) {
case 0x0: goto _267;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _37;
default: goto _165;
}


    __quex_assert_no_passage();
_266:
    /* (1324 from RELOAD_FORWARD)  */
    goto _578;


    __quex_assert_no_passage();
_268:
    /* (1325 from RELOAD_FORWARD)  */
_580:
    input = *(me->buffer._read_p);


    __quex_debug_state(1325);
if     ( input == 0x3A )  goto _38;
else if( input == 0x0 )   goto _269;
else                      goto _165;


    __quex_assert_no_passage();
_37:
    /* (1325 from 1324)  */
    ++(me->buffer._read_p);

    goto _580;


    __quex_assert_no_passage();
_270:
    /* (1326 from RELOAD_FORWARD)  */
_582:
    input = *(me->buffer._read_p);


    __quex_debug_state(1326);
switch( input ) {
case 0x0: goto _271;
case 0x2A: goto _40;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _39;
default: goto _165;
}


    __quex_assert_no_passage();
_38:
    /* (1326 from 1325)  */
    ++(me->buffer._read_p);

    goto _582;


    __quex_assert_no_passage();
_39:
    /* (1327 from 1326)  */
    ++(me->buffer._read_p);

_584:
    input = *(me->buffer._read_p);


    __quex_debug_state(1327);
switch( input ) {
case 0x0: goto _273;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _41;
default: goto _165;
}


    __quex_assert_no_passage();
_272:
    /* (1327 from RELOAD_FORWARD)  */
    goto _584;


    __quex_assert_no_passage();
_40:
    /* (1328 from 1326)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1328);
goto _175;


    __quex_assert_no_passage();
_41:
    /* (1329 from 1327)  */
    ++(me->buffer._read_p);

_587:
    input = *(me->buffer._read_p);


    __quex_debug_state(1329);
switch( input ) {
case 0x0: goto _275;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _42;
default: goto _165;
}


    __quex_assert_no_passage();
_274:
    /* (1329 from RELOAD_FORWARD)  */
    goto _587;


    __quex_assert_no_passage();
_42:
    /* (1330 from 1329)  */
    ++(me->buffer._read_p);

_589:
    input = *(me->buffer._read_p);


    __quex_debug_state(1330);
switch( input ) {
case 0x0: goto _277;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _43;
default: goto _165;
}


    __quex_assert_no_passage();
_276:
    /* (1330 from RELOAD_FORWARD)  */
    goto _589;


    __quex_assert_no_passage();
_43:
    /* (1331 from 1330)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1331);
goto _176;


    __quex_assert_no_passage();
_278:
    /* (1332 from RELOAD_FORWARD)  */
_592:
    input = *(me->buffer._read_p);


    __quex_debug_state(1332);
switch( input ) {
case 0x0: goto _279;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _48;
default: goto _165;
}


    __quex_assert_no_passage();
_44:
    /* (1332 from 1323)  */
    ++(me->buffer._read_p);

    goto _592;


    __quex_assert_no_passage();
_280:
    /* (1333 from RELOAD_FORWARD)  */
_594:
    input = *(me->buffer._read_p);


    __quex_debug_state(1333);
if     ( input == 0x3A )  goto _46;
else if( input == 0x0 )   goto _281;
else                      goto _165;


    __quex_assert_no_passage();
_45:
    /* (1333 from 1323)  */
    ++(me->buffer._read_p);

    goto _594;


    __quex_assert_no_passage();
_282:
    /* (1334 from RELOAD_FORWARD)  */
_596:
    input = *(me->buffer._read_p);


    __quex_debug_state(1334);
if     ( input == 0x2A )  goto _47;
else if( input == 0x0 )   goto _283;
else                      goto _165;


    __quex_assert_no_passage();
_46:
    /* (1334 from 1333)  */
    ++(me->buffer._read_p);

    goto _596;


    __quex_assert_no_passage();
_47:
    /* (1335 from 1334)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1335);
goto _177;


    __quex_assert_no_passage();
_284:
    /* (1336 from RELOAD_FORWARD)  */
_599:
    input = *(me->buffer._read_p);


    __quex_debug_state(1336);
if     ( input == 0x3A )  goto _49;
else if( input == 0x0 )   goto _285;
else                      goto _165;


    __quex_assert_no_passage();
_48:
    /* (1336 from 1332)  */
    ++(me->buffer._read_p);

    goto _599;


    __quex_assert_no_passage();
_49:
    /* (1337 from 1336)  */
    ++(me->buffer._read_p);

_601:
    input = *(me->buffer._read_p);


    __quex_debug_state(1337);
switch( input ) {
case 0x0: goto _287;
case 0x2A: goto _51;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _50;
default: goto _165;
}


    __quex_assert_no_passage();
_286:
    /* (1337 from RELOAD_FORWARD)  */
    goto _601;


    __quex_assert_no_passage();
_50:
    /* (1338 from 1337)  */
    ++(me->buffer._read_p);

_603:
    input = *(me->buffer._read_p);


    __quex_debug_state(1338);
switch( input ) {
case 0x0: goto _289;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _52;
default: goto _165;
}


    __quex_assert_no_passage();
_288:
    /* (1338 from RELOAD_FORWARD)  */
    goto _603;


    __quex_assert_no_passage();
_51:
    /* (1339 from 1337)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1339);
goto _178;


    __quex_assert_no_passage();
_52:
    /* (1340 from 1338)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1340);
goto _179;


    __quex_assert_no_passage();
_290:
    /* (1341 from RELOAD_FORWARD)  */
_607:
    input = *(me->buffer._read_p);


    __quex_debug_state(1341);
switch( input ) {
case 0x0: goto _291;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _36;
case 0x3A: goto _35;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _36;
case 0x76: goto _54;
default: goto _165;
}


    __quex_assert_no_passage();
_53:
    /* (1341 from 1311)  */
    ++(me->buffer._read_p);

    goto _607;


    __quex_assert_no_passage();
_292:
    /* (1342 from RELOAD_FORWARD)  */
_609:
    input = *(me->buffer._read_p);


    __quex_debug_state(1342);
if     ( input == 0x69 )  goto _55;
else if( input == 0x0 )   goto _293;
else                      goto _165;


    __quex_assert_no_passage();
_54:
    /* (1342 from 1341)  */
    ++(me->buffer._read_p);

    goto _609;


    __quex_assert_no_passage();
_55:
    /* (1343 from 1342)  */
    ++(me->buffer._read_p);

_611:
    input = *(me->buffer._read_p);


    __quex_debug_state(1343);
if     ( input == 0x63 )  goto _56;
else if( input == 0x0 )   goto _295;
else                      goto _165;


    __quex_assert_no_passage();
_294:
    /* (1343 from RELOAD_FORWARD)  */
    goto _611;


    __quex_assert_no_passage();
_296:
    /* (1344 from RELOAD_FORWARD)  */
_613:
    input = *(me->buffer._read_p);


    __quex_debug_state(1344);
if     ( input == 0x65 )  goto _57;
else if( input == 0x0 )   goto _297;
else                      goto _165;


    __quex_assert_no_passage();
_56:
    /* (1344 from 1343)  */
    ++(me->buffer._read_p);

    goto _613;


    __quex_assert_no_passage();
_57:
    /* (1345 from 1344)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1345);
goto _180;


    __quex_assert_no_passage();
_58:
    /* (1346 from 1310)  */
    ++(me->buffer._read_p);

_616:
    input = *(me->buffer._read_p);


    __quex_debug_state(1346);
if     ( input == 0x72 )  goto _59;
else if( input == 0x0 )   goto _299;
else                      goto _165;


    __quex_assert_no_passage();
_298:
    /* (1346 from RELOAD_FORWARD)  */
    goto _616;


    __quex_assert_no_passage();
_59:
    /* (1347 from 1346)  */
    ++(me->buffer._read_p);

_618:
    input = *(me->buffer._read_p);


    __quex_debug_state(1347);
if     ( input == 0x69 )  goto _60;
else if( input == 0x0 )   goto _301;
else                      goto _165;


    __quex_assert_no_passage();
_300:
    /* (1347 from RELOAD_FORWARD)  */
    goto _618;


    __quex_assert_no_passage();
_302:
    /* (1348 from RELOAD_FORWARD)  */
_620:
    input = *(me->buffer._read_p);


    __quex_debug_state(1348);
if     ( input == 0x61 )  goto _61;
else if( input == 0x0 )   goto _303;
else                      goto _165;


    __quex_assert_no_passage();
_60:
    /* (1348 from 1347)  */
    ++(me->buffer._read_p);

    goto _620;


    __quex_assert_no_passage();
_304:
    /* (1349 from RELOAD_FORWARD)  */
_622:
    input = *(me->buffer._read_p);


    __quex_debug_state(1349);
if     ( input == 0x6C )  goto _62;
else if( input == 0x0 )   goto _305;
else                      goto _165;


    __quex_assert_no_passage();
_61:
    /* (1349 from 1348)  */
    ++(me->buffer._read_p);

    goto _622;


    __quex_assert_no_passage();
_62:
    /* (1350 from 1349)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1350);
goto _181;


    __quex_assert_no_passage();
_63:
    /* (1351 from 1309)  */
    ++(me->buffer._read_p);

_625:
    input = *(me->buffer._read_p);


    __quex_debug_state(1351);
if     ( input == 0x65 )  goto _64;
else if( input == 0x0 )   goto _307;
else                      goto _165;


    __quex_assert_no_passage();
_306:
    /* (1351 from RELOAD_FORWARD)  */
    goto _625;


    __quex_assert_no_passage();
_308:
    /* (1352 from RELOAD_FORWARD)  */
_627:
    input = *(me->buffer._read_p);


    __quex_debug_state(1352);
if     ( input == 0x2D )  goto _65;
else if( input == 0x0 )   goto _309;
else                      goto _165;


    __quex_assert_no_passage();
_64:
    /* (1352 from 1351)  */
    ++(me->buffer._read_p);

    goto _627;


    __quex_assert_no_passage();
_65:
    /* (1353 from 1352)  */
    ++(me->buffer._read_p);

_629:
    input = *(me->buffer._read_p);


    __quex_debug_state(1353);
if     ( input == 0x6F )  goto _66;
else if( input == 0x0 )   goto _311;
else                      goto _165;


    __quex_assert_no_passage();
_310:
    /* (1353 from RELOAD_FORWARD)  */
    goto _629;


    __quex_assert_no_passage();
_66:
    /* (1354 from 1353)  */
    ++(me->buffer._read_p);

_631:
    input = *(me->buffer._read_p);


    __quex_debug_state(1354);
if     ( input == 0x66 )  goto _67;
else if( input == 0x0 )   goto _313;
else                      goto _165;


    __quex_assert_no_passage();
_312:
    /* (1354 from RELOAD_FORWARD)  */
    goto _631;


    __quex_assert_no_passage();
_67:
    /* (1355 from 1354)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1355);
goto _182;


    __quex_assert_no_passage();
_314:
    /* (1356 from RELOAD_FORWARD)  */
_634:
    input = *(me->buffer._read_p);


    __quex_debug_state(1356);
if     ( input == 0x63 )  goto _69;
else if( input == 0x0 )   goto _315;
else                      goto _165;


    __quex_assert_no_passage();
_68:
    /* (1356 from 1308)  */
    ++(me->buffer._read_p);

    goto _634;


    __quex_assert_no_passage();
_316:
    /* (1357 from RELOAD_FORWARD)  */
_636:
    input = *(me->buffer._read_p);


    __quex_debug_state(1357);
if     ( input == 0x61 )  goto _70;
else if( input == 0x0 )   goto _317;
else                      goto _165;


    __quex_assert_no_passage();
_69:
    /* (1357 from 1356)  */
    ++(me->buffer._read_p);

    goto _636;


    __quex_assert_no_passage();
_318:
    /* (1358 from RELOAD_FORWARD)  */
_638:
    input = *(me->buffer._read_p);


    __quex_debug_state(1358);
if     ( input == 0x6C )  goto _71;
else if( input == 0x0 )   goto _319;
else                      goto _165;


    __quex_assert_no_passage();
_70:
    /* (1358 from 1357)  */
    ++(me->buffer._read_p);

    goto _638;


    __quex_assert_no_passage();
_71:
    /* (1359 from 1358)  */
    ++(me->buffer._read_p);

_640:
    input = *(me->buffer._read_p);


    __quex_debug_state(1359);
if     ( input == 0x74 )  goto _72;
else if( input == 0x0 )   goto _321;
else                      goto _165;


    __quex_assert_no_passage();
_320:
    /* (1359 from RELOAD_FORWARD)  */
    goto _640;


    __quex_assert_no_passage();
_322:
    /* (1360 from RELOAD_FORWARD)  */
_642:
    input = *(me->buffer._read_p);


    __quex_debug_state(1360);
if     ( input == 0x69 )  goto _73;
else if( input == 0x0 )   goto _323;
else                      goto _165;


    __quex_assert_no_passage();
_72:
    /* (1360 from 1359)  */
    ++(me->buffer._read_p);

    goto _642;


    __quex_assert_no_passage();
_73:
    /* (1361 from 1360)  */
    ++(me->buffer._read_p);

_644:
    input = *(me->buffer._read_p);


    __quex_debug_state(1361);
if     ( input == 0x6D )  goto _74;
else if( input == 0x0 )   goto _325;
else                      goto _165;


    __quex_assert_no_passage();
_324:
    /* (1361 from RELOAD_FORWARD)  */
    goto _644;


    __quex_assert_no_passage();
_74:
    /* (1362 from 1361)  */
    ++(me->buffer._read_p);

_646:
    input = *(me->buffer._read_p);


    __quex_debug_state(1362);
if     ( input == 0x65 )  goto _75;
else if( input == 0x0 )   goto _327;
else                      goto _165;


    __quex_assert_no_passage();
_326:
    /* (1362 from RELOAD_FORWARD)  */
    goto _646;


    __quex_assert_no_passage();
_75:
    /* (1363 from 1362)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1363);
goto _183;


    __quex_assert_no_passage();
_328:
    /* (1364 from RELOAD_FORWARD)  */
_649:
    input = *(me->buffer._read_p);


    __quex_debug_state(1364);
if     ( input == 0x74 )  goto _77;
else if( input == 0x0 )   goto _329;
else                      goto _165;


    __quex_assert_no_passage();
_76:
    /* (1364 from 1307)  */
    ++(me->buffer._read_p);

    goto _649;


    __quex_assert_no_passage();
_330:
    /* (1365 from RELOAD_FORWARD)  */
_651:
    input = *(me->buffer._read_p);


    __quex_debug_state(1365);
if     ( input == 0x68 )  goto _78;
else if( input == 0x0 )   goto _331;
else                      goto _165;


    __quex_assert_no_passage();
_77:
    /* (1365 from 1364)  */
    ++(me->buffer._read_p);

    goto _651;


    __quex_assert_no_passage();
_332:
    /* (1366 from RELOAD_FORWARD)  */
_653:
    input = *(me->buffer._read_p);


    __quex_debug_state(1366);
if     ( input == 0x2D )  goto _79;
else if( input == 0x0 )   goto _333;
else                      goto _165;


    __quex_assert_no_passage();
_78:
    /* (1366 from 1365)  */
    ++(me->buffer._read_p);

    goto _653;


    __quex_assert_no_passage();
_79:
    /* (1367 from 1366)  */
    ++(me->buffer._read_p);

_655:
    input = *(me->buffer._read_p);


    __quex_debug_state(1367);
if     ( input == 0x69 )  goto _80;
else if( input == 0x0 )   goto _335;
else                      goto _165;


    __quex_assert_no_passage();
_334:
    /* (1367 from RELOAD_FORWARD)  */
    goto _655;


    __quex_assert_no_passage();
_336:
    /* (1368 from RELOAD_FORWARD)  */
_657:
    input = *(me->buffer._read_p);


    __quex_debug_state(1368);
if     ( input == 0x6E )  goto _81;
else if( input == 0x0 )   goto _337;
else                      goto _165;


    __quex_assert_no_passage();
_80:
    /* (1368 from 1367)  */
    ++(me->buffer._read_p);

    goto _657;


    __quex_assert_no_passage();
_81:
    /* (1369 from 1368)  */
    ++(me->buffer._read_p);

_659:
    input = *(me->buffer._read_p);


    __quex_debug_state(1369);
if     ( input == 0x74 )  goto _82;
else if( input == 0x0 )   goto _339;
else                      goto _165;


    __quex_assert_no_passage();
_338:
    /* (1369 from RELOAD_FORWARD)  */
    goto _659;


    __quex_assert_no_passage();
_82:
    /* (1370 from 1369)  */
    ++(me->buffer._read_p);

_661:
    input = *(me->buffer._read_p);


    __quex_debug_state(1370);
if     ( input == 0x65 )  goto _83;
else if( input == 0x0 )   goto _341;
else                      goto _165;


    __quex_assert_no_passage();
_340:
    /* (1370 from RELOAD_FORWARD)  */
    goto _661;


    __quex_assert_no_passage();
_83:
    /* (1371 from 1370)  */
    ++(me->buffer._read_p);

_663:
    input = *(me->buffer._read_p);


    __quex_debug_state(1371);
if     ( input == 0x72 )  goto _84;
else if( input == 0x0 )   goto _343;
else                      goto _165;


    __quex_assert_no_passage();
_342:
    /* (1371 from RELOAD_FORWARD)  */
    goto _663;


    __quex_assert_no_passage();
_344:
    /* (1372 from RELOAD_FORWARD)  */
_665:
    input = *(me->buffer._read_p);


    __quex_debug_state(1372);
if     ( input == 0x66 )  goto _85;
else if( input == 0x0 )   goto _345;
else                      goto _165;


    __quex_assert_no_passage();
_84:
    /* (1372 from 1371)  */
    ++(me->buffer._read_p);

    goto _665;


    __quex_assert_no_passage();
_346:
    /* (1373 from RELOAD_FORWARD)  */
_667:
    input = *(me->buffer._read_p);


    __quex_debug_state(1373);
if     ( input == 0x61 )  goto _86;
else if( input == 0x0 )   goto _347;
else                      goto _165;


    __quex_assert_no_passage();
_85:
    /* (1373 from 1372)  */
    ++(me->buffer._read_p);

    goto _667;


    __quex_assert_no_passage();
_348:
    /* (1374 from RELOAD_FORWARD)  */
_669:
    input = *(me->buffer._read_p);


    __quex_debug_state(1374);
if     ( input == 0x63 )  goto _87;
else if( input == 0x0 )   goto _349;
else                      goto _165;


    __quex_assert_no_passage();
_86:
    /* (1374 from 1373)  */
    ++(me->buffer._read_p);

    goto _669;


    __quex_assert_no_passage();
_87:
    /* (1375 from 1374)  */
    ++(me->buffer._read_p);

_671:
    input = *(me->buffer._read_p);


    __quex_debug_state(1375);
if     ( input == 0x65 )  goto _88;
else if( input == 0x0 )   goto _351;
else                      goto _165;


    __quex_assert_no_passage();
_350:
    /* (1375 from RELOAD_FORWARD)  */
    goto _671;


    __quex_assert_no_passage();
_88:
    /* (1376 from 1375)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1376);
goto _184;


    __quex_assert_no_passage();
_89:
    /* (1377 from 1306)  */
    ++(me->buffer._read_p);

_674:
    input = *(me->buffer._read_p);


    __quex_debug_state(1377);
if     ( input == 0x79 )  goto _107;
else if( input == 0x0 )   goto _353;
else                      goto _165;


    __quex_assert_no_passage();
_352:
    /* (1377 from RELOAD_FORWARD)  */
    goto _674;


    __quex_assert_no_passage();
_90:
    /* (1378 from 1306)  */
    ++(me->buffer._read_p);

_676:
    input = *(me->buffer._read_p);


    __quex_debug_state(1378);
if     ( input == 0x6C )  goto _91;
else if( input == 0x0 )   goto _355;
else                      goto _165;


    __quex_assert_no_passage();
_354:
    /* (1378 from RELOAD_FORWARD)  */
    goto _676;


    __quex_assert_no_passage();
_91:
    /* (1379 from 1378)  */
    ++(me->buffer._read_p);

_678:
    input = *(me->buffer._read_p);


    __quex_debug_state(1379);
switch( input ) {
case 0x0: goto _357;
case 0x2D: goto _92;
case 0x6F: goto _93;
default: goto _165;
}


    __quex_assert_no_passage();
_356:
    /* (1379 from RELOAD_FORWARD)  */
    goto _678;


    __quex_assert_no_passage();
_358:
    /* (1380 from RELOAD_FORWARD)  */
_680:
    input = *(me->buffer._read_p);


    __quex_debug_state(1380);
if     ( input == 0x6F )  goto _105;
else if( input == 0x0 )   goto _359;
else                      goto _165;


    __quex_assert_no_passage();
_92:
    /* (1380 from 1379)  */
    ++(me->buffer._read_p);

    goto _680;


    __quex_assert_no_passage();
_360:
    /* (1381 from RELOAD_FORWARD)  */
_682:
    input = *(me->buffer._read_p);


    __quex_debug_state(1381);
if     ( input == 0x77 )  goto _94;
else if( input == 0x0 )   goto _361;
else                      goto _165;


    __quex_assert_no_passage();
_93:
    /* (1381 from 1379)  */
    ++(me->buffer._read_p);

    goto _682;


    __quex_assert_no_passage();
_362:
    /* (1382 from RELOAD_FORWARD)  */
_684:
    input = *(me->buffer._read_p);


    __quex_debug_state(1382);
if     ( input == 0x65 )  goto _95;
else if( input == 0x0 )   goto _363;
else                      goto _185;


    __quex_assert_no_passage();
_94:
    /* (1382 from 1381)  */
    ++(me->buffer._read_p);

    goto _684;


    __quex_assert_no_passage();
_95:
    /* (1383 from 1382)  */
    ++(me->buffer._read_p);

_686:
    input = *(me->buffer._read_p);


    __quex_debug_state(1383);
if     ( input == 0x64 )  goto _96;
else if( input == 0x0 )   goto _365;
else                      goto _186;


    __quex_assert_no_passage();
_364:
    /* (1383 from RELOAD_FORWARD)  */
    goto _686;


    __quex_assert_no_passage();
_366:
    /* (1384 from RELOAD_FORWARD)  */
_688:
    input = *(me->buffer._read_p);


    __quex_debug_state(1384);
if     ( input == 0x2D )  goto _97;
else if( input == 0x0 )   goto _367;
else                      goto _187;


    __quex_assert_no_passage();
_96:
    /* (1384 from 1383)  */
    ++(me->buffer._read_p);

    goto _688;


    __quex_assert_no_passage();
_97:
    /* (1385 from 1384)  */
    ++(me->buffer._read_p);

_690:
    input = *(me->buffer._read_p);


    __quex_debug_state(1385);
if     ( input == 0x6D )  goto _98;
else if( input == 0x0 )   goto _369;
else                      goto _188;


    __quex_assert_no_passage();
_368:
    /* (1385 from RELOAD_FORWARD)  */
    goto _690;


    __quex_assert_no_passage();
_98:
    /* (1386 from 1385)  */
    ++(me->buffer._read_p);

_692:
    input = *(me->buffer._read_p);


    __quex_debug_state(1386);
if     ( input == 0x61 )  goto _99;
else if( input == 0x0 )   goto _371;
else                      goto _189;


    __quex_assert_no_passage();
_370:
    /* (1386 from RELOAD_FORWARD)  */
    goto _692;


    __quex_assert_no_passage();
_99:
    /* (1387 from 1386)  */
    ++(me->buffer._read_p);

_694:
    input = *(me->buffer._read_p);


    __quex_debug_state(1387);
if     ( input == 0x74 )  goto _100;
else if( input == 0x0 )   goto _373;
else                      goto _190;


    __quex_assert_no_passage();
_372:
    /* (1387 from RELOAD_FORWARD)  */
    goto _694;


    __quex_assert_no_passage();
_374:
    /* (1388 from RELOAD_FORWARD)  */
_696:
    input = *(me->buffer._read_p);


    __quex_debug_state(1388);
if     ( input == 0x63 )  goto _101;
else if( input == 0x0 )   goto _375;
else                      goto _191;


    __quex_assert_no_passage();
_100:
    /* (1388 from 1387)  */
    ++(me->buffer._read_p);

    goto _696;


    __quex_assert_no_passage();
_376:
    /* (1389 from RELOAD_FORWARD)  */
_698:
    input = *(me->buffer._read_p);


    __quex_debug_state(1389);
if     ( input == 0x68 )  goto _102;
else if( input == 0x0 )   goto _377;
else                      goto _192;


    __quex_assert_no_passage();
_101:
    /* (1389 from 1388)  */
    ++(me->buffer._read_p);

    goto _698;


    __quex_assert_no_passage();
_378:
    /* (1390 from RELOAD_FORWARD)  */
_700:
    input = *(me->buffer._read_p);


    __quex_debug_state(1390);
if     ( input == 0x65 )  goto _103;
else if( input == 0x0 )   goto _379;
else                      goto _193;


    __quex_assert_no_passage();
_102:
    /* (1390 from 1389)  */
    ++(me->buffer._read_p);

    goto _700;


    __quex_assert_no_passage();
_103:
    /* (1391 from 1390)  */
    ++(me->buffer._read_p);

_702:
    input = *(me->buffer._read_p);


    __quex_debug_state(1391);
if     ( input == 0x73 )  goto _104;
else if( input == 0x0 )   goto _381;
else                      goto _194;


    __quex_assert_no_passage();
_380:
    /* (1391 from RELOAD_FORWARD)  */
    goto _702;


    __quex_assert_no_passage();
_104:
    /* (1392 from 1391)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1392);
goto _195;


    __quex_assert_no_passage();
_105:
    /* (1393 from 1380)  */
    ++(me->buffer._read_p);

_705:
    input = *(me->buffer._read_p);


    __quex_debug_state(1393);
if     ( input == 0x66 )  goto _106;
else if( input == 0x0 )   goto _383;
else                      goto _165;


    __quex_assert_no_passage();
_382:
    /* (1393 from RELOAD_FORWARD)  */
    goto _705;


    __quex_assert_no_passage();
_106:
    /* (1394 from 1393)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1394);
goto _196;


    __quex_assert_no_passage();
_107:
    /* (1395 from 1377)  */
    ++(me->buffer._read_p);

_708:
    input = *(me->buffer._read_p);


    __quex_debug_state(1395);
if     ( input == 0x2D )  goto _108;
else if( input == 0x0 )   goto _385;
else                      goto _165;


    __quex_assert_no_passage();
_384:
    /* (1395 from RELOAD_FORWARD)  */
    goto _708;


    __quex_assert_no_passage();
_386:
    /* (1396 from RELOAD_FORWARD)  */
_710:
    input = *(me->buffer._read_p);


    __quex_debug_state(1396);
if     ( input == 0x6F )  goto _109;
else if( input == 0x0 )   goto _387;
else                      goto _165;


    __quex_assert_no_passage();
_108:
    /* (1396 from 1395)  */
    ++(me->buffer._read_p);

    goto _710;


    __quex_assert_no_passage();
_388:
    /* (1397 from RELOAD_FORWARD)  */
_712:
    input = *(me->buffer._read_p);


    __quex_debug_state(1397);
if     ( input == 0x66 )  goto _110;
else if( input == 0x0 )   goto _389;
else                      goto _165;


    __quex_assert_no_passage();
_109:
    /* (1397 from 1396)  */
    ++(me->buffer._read_p);

    goto _712;


    __quex_assert_no_passage();
_110:
    /* (1398 from 1397)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1398);
goto _197;


    __quex_assert_no_passage();
_111:
    /* (1399 from 1305)  */
    ++(me->buffer._read_p);

_715:
    input = *(me->buffer._read_p);


    __quex_debug_state(1399);
if     ( input == 0x75 )  goto _112;
else if( input == 0x0 )   goto _391;
else                      goto _165;


    __quex_assert_no_passage();
_390:
    /* (1399 from RELOAD_FORWARD)  */
    goto _715;


    __quex_assert_no_passage();
_392:
    /* (1400 from RELOAD_FORWARD)  */
_717:
    input = *(me->buffer._read_p);


    __quex_debug_state(1400);
if     ( input == 0x61 )  goto _113;
else if( input == 0x0 )   goto _393;
else                      goto _165;


    __quex_assert_no_passage();
_112:
    /* (1400 from 1399)  */
    ++(me->buffer._read_p);

    goto _717;


    __quex_assert_no_passage();
_113:
    /* (1401 from 1400)  */
    ++(me->buffer._read_p);

_719:
    input = *(me->buffer._read_p);


    __quex_debug_state(1401);
if     ( input == 0x6C )  goto _114;
else if( input == 0x0 )   goto _395;
else                      goto _165;


    __quex_assert_no_passage();
_394:
    /* (1401 from RELOAD_FORWARD)  */
    goto _719;


    __quex_assert_no_passage();
_114:
    /* (1402 from 1401)  */
    ++(me->buffer._read_p);

_721:
    input = *(me->buffer._read_p);


    __quex_debug_state(1402);
if     ( input == 0x73 )  goto _115;
else if( input == 0x0 )   goto _397;
else                      goto _165;


    __quex_assert_no_passage();
_396:
    /* (1402 from RELOAD_FORWARD)  */
    goto _721;


    __quex_assert_no_passage();
_115:
    /* (1403 from 1402)  */
    ++(me->buffer._read_p);

_723:
    input = *(me->buffer._read_p);


    __quex_debug_state(1403);
if     ( input == 0x2D )  goto _116;
else if( input == 0x0 )   goto _399;
else                      goto _198;


    __quex_assert_no_passage();
_398:
    /* (1403 from RELOAD_FORWARD)  */
    goto _723;


    __quex_assert_no_passage();
_400:
    /* (1404 from RELOAD_FORWARD)  */
_725:
    input = *(me->buffer._read_p);


    __quex_debug_state(1404);
if     ( input == 0x6F )  goto _117;
else if( input == 0x0 )   goto _401;
else                      goto _199;


    __quex_assert_no_passage();
_116:
    /* (1404 from 1403)  */
    ++(me->buffer._read_p);

    goto _725;


    __quex_assert_no_passage();
_402:
    /* (1405 from RELOAD_FORWARD)  */
_727:
    input = *(me->buffer._read_p);


    __quex_debug_state(1405);
if     ( input == 0x72 )  goto _118;
else if( input == 0x0 )   goto _403;
else                      goto _200;


    __quex_assert_no_passage();
_117:
    /* (1405 from 1404)  */
    ++(me->buffer._read_p);

    goto _727;


    __quex_assert_no_passage();
_404:
    /* (1406 from RELOAD_FORWARD)  */
_729:
    input = *(me->buffer._read_p);


    __quex_debug_state(1406);
if     ( input == 0x64 )  goto _119;
else if( input == 0x0 )   goto _405;
else                      goto _201;


    __quex_assert_no_passage();
_118:
    /* (1406 from 1405)  */
    ++(me->buffer._read_p);

    goto _729;


    __quex_assert_no_passage();
_119:
    /* (1407 from 1406)  */
    ++(me->buffer._read_p);

_731:
    input = *(me->buffer._read_p);


    __quex_debug_state(1407);
if     ( input == 0x65 )  goto _120;
else if( input == 0x0 )   goto _407;
else                      goto _202;


    __quex_assert_no_passage();
_406:
    /* (1407 from RELOAD_FORWARD)  */
    goto _731;


    __quex_assert_no_passage();
_408:
    /* (1408 from RELOAD_FORWARD)  */
_733:
    input = *(me->buffer._read_p);


    __quex_debug_state(1408);
if     ( input == 0x72 )  goto _121;
else if( input == 0x0 )   goto _409;
else                      goto _203;


    __quex_assert_no_passage();
_120:
    /* (1408 from 1407)  */
    ++(me->buffer._read_p);

    goto _733;


    __quex_assert_no_passage();
_121:
    /* (1409 from 1408)  */
    ++(me->buffer._read_p);

_735:
    input = *(me->buffer._read_p);


    __quex_debug_state(1409);
if     ( input == 0x65 )  goto _122;
else if( input == 0x0 )   goto _411;
else                      goto _204;


    __quex_assert_no_passage();
_410:
    /* (1409 from RELOAD_FORWARD)  */
    goto _735;


    __quex_assert_no_passage();
_122:
    /* (1410 from 1409)  */
    ++(me->buffer._read_p);

_737:
    input = *(me->buffer._read_p);


    __quex_debug_state(1410);
if     ( input == 0x64 )  goto _123;
else if( input == 0x0 )   goto _413;
else                      goto _205;


    __quex_assert_no_passage();
_412:
    /* (1410 from RELOAD_FORWARD)  */
    goto _737;


    __quex_assert_no_passage();
_123:
    /* (1411 from 1410)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1411);
goto _206;


    __quex_assert_no_passage();
_414:
    /* (1412 from RELOAD_FORWARD)  */
_740:
    input = *(me->buffer._read_p);


    __quex_debug_state(1412);
if     ( input == 0x6F )  goto _125;
else if( input == 0x0 )   goto _415;
else                      goto _165;


    __quex_assert_no_passage();
_124:
    /* (1412 from 1304)  */
    ++(me->buffer._read_p);

    goto _740;


    __quex_assert_no_passage();
_416:
    /* (1413 from RELOAD_FORWARD)  */
_742:
    input = *(me->buffer._read_p);


    __quex_debug_state(1413);
if     ( input == 0x63 )  goto _126;
else if( input == 0x0 )   goto _417;
else                      goto _165;


    __quex_assert_no_passage();
_125:
    /* (1413 from 1412)  */
    ++(me->buffer._read_p);

    goto _742;


    __quex_assert_no_passage();
_418:
    /* (1414 from RELOAD_FORWARD)  */
_744:
    input = *(me->buffer._read_p);


    __quex_debug_state(1414);
if     ( input == 0x6B )  goto _127;
else if( input == 0x0 )   goto _419;
else                      goto _165;


    __quex_assert_no_passage();
_126:
    /* (1414 from 1413)  */
    ++(me->buffer._read_p);

    goto _744;


    __quex_assert_no_passage();
_127:
    /* (1415 from 1414)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1415);
goto _207;


    __quex_assert_no_passage();
_420:
    /* (1416 from RELOAD_FORWARD)  */
_747:
    input = *(me->buffer._read_p);


    __quex_debug_state(1416);
if     ( input == 0x6D )  goto _135;
else if( input == 0x0 )   goto _421;
else                      goto _165;


    __quex_assert_no_passage();
_128:
    /* (1416 from 1303)  */
    ++(me->buffer._read_p);

    goto _747;


    __quex_assert_no_passage();
_129:
    /* (1417 from 1303)  */
    ++(me->buffer._read_p);

_749:
    input = *(me->buffer._read_p);


    __quex_debug_state(1417);
if     ( input == 0x6E )  goto _130;
else if( input == 0x0 )   goto _423;
else                      goto _165;


    __quex_assert_no_passage();
_422:
    /* (1417 from RELOAD_FORWARD)  */
    goto _749;


    __quex_assert_no_passage();
_130:
    /* (1418 from 1417)  */
    ++(me->buffer._read_p);

_751:
    input = *(me->buffer._read_p);


    __quex_debug_state(1418);
if     ( input == 0x65 )  goto _131;
else if( input == 0x0 )   goto _425;
else                      goto _165;


    __quex_assert_no_passage();
_424:
    /* (1418 from RELOAD_FORWARD)  */
    goto _751;


    __quex_assert_no_passage();
_131:
    /* (1419 from 1418)  */
    ++(me->buffer._read_p);

_753:
    input = *(me->buffer._read_p);


    __quex_debug_state(1419);
if     ( input == 0x2D )  goto _132;
else if( input == 0x0 )   goto _427;
else                      goto _165;


    __quex_assert_no_passage();
_426:
    /* (1419 from RELOAD_FORWARD)  */
    goto _753;


    __quex_assert_no_passage();
_428:
    /* (1420 from RELOAD_FORWARD)  */
_755:
    input = *(me->buffer._read_p);


    __quex_debug_state(1420);
if     ( input == 0x6F )  goto _133;
else if( input == 0x0 )   goto _429;
else                      goto _165;


    __quex_assert_no_passage();
_132:
    /* (1420 from 1419)  */
    ++(me->buffer._read_p);

    goto _755;


    __quex_assert_no_passage();
_430:
    /* (1421 from RELOAD_FORWARD)  */
_757:
    input = *(me->buffer._read_p);


    __quex_debug_state(1421);
if     ( input == 0x66 )  goto _134;
else if( input == 0x0 )   goto _431;
else                      goto _165;


    __quex_assert_no_passage();
_133:
    /* (1421 from 1420)  */
    ++(me->buffer._read_p);

    goto _757;


    __quex_assert_no_passage();
_134:
    /* (1422 from 1421)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1422);
goto _208;


    __quex_assert_no_passage();
_135:
    /* (1423 from 1416)  */
    ++(me->buffer._read_p);

_760:
    input = *(me->buffer._read_p);


    __quex_debug_state(1423);
if     ( input == 0x65 )  goto _136;
else if( input == 0x0 )   goto _433;
else                      goto _165;


    __quex_assert_no_passage();
_432:
    /* (1423 from RELOAD_FORWARD)  */
    goto _760;


    __quex_assert_no_passage();
_136:
    /* (1424 from 1423)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1424);
goto _209;


    __quex_assert_no_passage();
_137:
    /* (1425 from 1300)  */
    ++(me->buffer._read_p);

_763:
    input = *(me->buffer._read_p);


    __quex_debug_state(1425);
switch( input ) {
case 0x0: goto _435;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _36;
case 0x3A: goto _35;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _36;
case 0x6C: goto _138;
default: goto _165;
}


    __quex_assert_no_passage();
_434:
    /* (1425 from RELOAD_FORWARD)  */
    goto _763;


    __quex_assert_no_passage();
_138:
    /* (1426 from 1425)  */
    ++(me->buffer._read_p);

_765:
    input = *(me->buffer._read_p);


    __quex_debug_state(1426);
if     ( input == 0x73 )  goto _139;
else if( input == 0x0 )   goto _437;
else                      goto _165;


    __quex_assert_no_passage();
_436:
    /* (1426 from RELOAD_FORWARD)  */
    goto _765;


    __quex_assert_no_passage();
_139:
    /* (1427 from 1426)  */
    ++(me->buffer._read_p);

_767:
    input = *(me->buffer._read_p);


    __quex_debug_state(1427);
if     ( input == 0x65 )  goto _140;
else if( input == 0x0 )   goto _439;
else                      goto _165;


    __quex_assert_no_passage();
_438:
    /* (1427 from RELOAD_FORWARD)  */
    goto _767;


    __quex_assert_no_passage();
_140:
    /* (1428 from 1427)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1428);
goto _210;


    __quex_assert_no_passage();
_440:
    /* (1429 from RELOAD_FORWARD)  */
_770:
    input = *(me->buffer._read_p);


    __quex_debug_state(1429);
if     ( input == 0x6A )  goto _147;
else if( input == 0x0 )   goto _441;
else                      goto _165;


    __quex_assert_no_passage();
_141:
    /* (1429 from 1297)  */
    ++(me->buffer._read_p);

    goto _770;


    __quex_assert_no_passage();
_442:
    /* (1430 from RELOAD_FORWARD)  */
_772:
    input = *(me->buffer._read_p);


    __quex_debug_state(1430);
if     ( input == 0x6E )  goto _143;
else if( input == 0x0 )   goto _443;
else                      goto _165;


    __quex_assert_no_passage();
_142:
    /* (1430 from 1297)  */
    ++(me->buffer._read_p);

    goto _772;


    __quex_assert_no_passage();
_143:
    /* (1431 from 1430)  */
    ++(me->buffer._read_p);

_774:
    input = *(me->buffer._read_p);


    __quex_debug_state(1431);
if     ( input == 0x64 )  goto _144;
else if( input == 0x0 )   goto _445;
else                      goto _165;


    __quex_assert_no_passage();
_444:
    /* (1431 from RELOAD_FORWARD)  */
    goto _774;


    __quex_assert_no_passage();
_446:
    /* (1432 from RELOAD_FORWARD)  */
_776:
    input = *(me->buffer._read_p);


    __quex_debug_state(1432);
if     ( input == 0x6F )  goto _145;
else if( input == 0x0 )   goto _447;
else                      goto _165;


    __quex_assert_no_passage();
_144:
    /* (1432 from 1431)  */
    ++(me->buffer._read_p);

    goto _776;


    __quex_assert_no_passage();
_145:
    /* (1433 from 1432)  */
    ++(me->buffer._read_p);

_778:
    input = *(me->buffer._read_p);


    __quex_debug_state(1433);
if     ( input == 0x6D )  goto _146;
else if( input == 0x0 )   goto _449;
else                      goto _165;


    __quex_assert_no_passage();
_448:
    /* (1433 from RELOAD_FORWARD)  */
    goto _778;


    __quex_assert_no_passage();
_146:
    /* (1434 from 1433)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1434);
goto _211;


    __quex_assert_no_passage();
_147:
    /* (1435 from 1429)  */
    ++(me->buffer._read_p);

_781:
    input = *(me->buffer._read_p);


    __quex_debug_state(1435);
if     ( input == 0x65 )  goto _148;
else if( input == 0x0 )   goto _451;
else                      goto _165;


    __quex_assert_no_passage();
_450:
    /* (1435 from RELOAD_FORWARD)  */
    goto _781;


    __quex_assert_no_passage();
_452:
    /* (1436 from RELOAD_FORWARD)  */
_783:
    input = *(me->buffer._read_p);


    __quex_debug_state(1436);
if     ( input == 0x63 )  goto _149;
else if( input == 0x0 )   goto _453;
else                      goto _165;


    __quex_assert_no_passage();
_148:
    /* (1436 from 1435)  */
    ++(me->buffer._read_p);

    goto _783;


    __quex_assert_no_passage();
_454:
    /* (1437 from RELOAD_FORWARD)  */
_785:
    input = *(me->buffer._read_p);


    __quex_debug_state(1437);
if     ( input == 0x74 )  goto _150;
else if( input == 0x0 )   goto _455;
else                      goto _165;


    __quex_assert_no_passage();
_149:
    /* (1437 from 1436)  */
    ++(me->buffer._read_p);

    goto _785;


    __quex_assert_no_passage();
_150:
    /* (1438 from 1437)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1438);
goto _212;


    __quex_assert_no_passage();
_151:
    /* (1439 from 1296)  */
    ++(me->buffer._read_p);

_788:
    input = *(me->buffer._read_p);


    __quex_debug_state(1439);
if     ( input == 0x74 )  goto _152;
else if( input == 0x0 )   goto _457;
else                      goto _165;


    __quex_assert_no_passage();
_456:
    /* (1439 from RELOAD_FORWARD)  */
    goto _788;


    __quex_assert_no_passage();
_458:
    /* (1440 from RELOAD_FORWARD)  */
_790:
    input = *(me->buffer._read_p);


    __quex_debug_state(1440);
if     ( input == 0x63 )  goto _153;
else if( input == 0x0 )   goto _459;
else                      goto _165;


    __quex_assert_no_passage();
_152:
    /* (1440 from 1439)  */
    ++(me->buffer._read_p);

    goto _790;


    __quex_assert_no_passage();
_153:
    /* (1441 from 1440)  */
    ++(me->buffer._read_p);

_792:
    input = *(me->buffer._read_p);


    __quex_debug_state(1441);
if     ( input == 0x68 )  goto _154;
else if( input == 0x0 )   goto _461;
else                      goto _165;


    __quex_assert_no_passage();
_460:
    /* (1441 from RELOAD_FORWARD)  */
    goto _792;


    __quex_assert_no_passage();
_154:
    /* (1442 from 1441)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1442);
goto _213;


    __quex_assert_no_passage();
_155:
    /* (1443 from 1295)  */
    ++(me->buffer._read_p);

_795:
    input = *(me->buffer._read_p);


    __quex_debug_state(1443);
if     ( input == 0x61 )  goto _156;
else if( input == 0x0 )   goto _463;
else                      goto _165;


    __quex_assert_no_passage();
_462:
    /* (1443 from RELOAD_FORWARD)  */
    goto _795;


    __quex_assert_no_passage();
_464:
    /* (1444 from RELOAD_FORWARD)  */
_797:
    input = *(me->buffer._read_p);


    __quex_debug_state(1444);
if     ( input == 0x2D )  goto _157;
else if( input == 0x0 )   goto _465;
else                      goto _165;


    __quex_assert_no_passage();
_156:
    /* (1444 from 1443)  */
    ++(me->buffer._read_p);

    goto _797;


    __quex_assert_no_passage();
_466:
    /* (1445 from RELOAD_FORWARD)  */
_799:
    input = *(me->buffer._read_p);


    __quex_debug_state(1445);
if     ( input == 0x70 )  goto _158;
else if( input == 0x0 )   goto _467;
else                      goto _165;


    __quex_assert_no_passage();
_157:
    /* (1445 from 1444)  */
    ++(me->buffer._read_p);

    goto _799;


    __quex_assert_no_passage();
_468:
    /* (1446 from RELOAD_FORWARD)  */
_801:
    input = *(me->buffer._read_p);


    __quex_debug_state(1446);
if     ( input == 0x6F )  goto _159;
else if( input == 0x0 )   goto _469;
else                      goto _165;


    __quex_assert_no_passage();
_158:
    /* (1446 from 1445)  */
    ++(me->buffer._read_p);

    goto _801;


    __quex_assert_no_passage();
_159:
    /* (1447 from 1446)  */
    ++(me->buffer._read_p);

_803:
    input = *(me->buffer._read_p);


    __quex_debug_state(1447);
if     ( input == 0x72 )  goto _160;
else if( input == 0x0 )   goto _471;
else                      goto _165;


    __quex_assert_no_passage();
_470:
    /* (1447 from RELOAD_FORWARD)  */
    goto _803;


    __quex_assert_no_passage();
_472:
    /* (1448 from RELOAD_FORWARD)  */
_805:
    input = *(me->buffer._read_p);


    __quex_debug_state(1448);
if     ( input == 0x74 )  goto _161;
else if( input == 0x0 )   goto _473;
else                      goto _165;


    __quex_assert_no_passage();
_160:
    /* (1448 from 1447)  */
    ++(me->buffer._read_p);

    goto _805;


    __quex_assert_no_passage();
_161:
    /* (1449 from 1448)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1449);
goto _214;


    __quex_assert_no_passage();
_162:
    /* (1450 from 1292)  */
    ++(me->buffer._read_p);

_808:
    input = *(me->buffer._read_p);


    __quex_debug_state(1450);
if     ( input == 0x75 )  goto _163;
else if( input == 0x0 )   goto _475;
else                      goto _165;


    __quex_assert_no_passage();
_474:
    /* (1450 from RELOAD_FORWARD)  */
    goto _808;


    __quex_assert_no_passage();
_163:
    /* (1451 from 1450)  */
    ++(me->buffer._read_p);

_810:
    input = *(me->buffer._read_p);


    __quex_debug_state(1451);
if     ( input == 0x65 )  goto _164;
else if( input == 0x0 )   goto _477;
else                      goto _165;


    __quex_assert_no_passage();
_476:
    /* (1451 from RELOAD_FORWARD)  */
    goto _810;


    __quex_assert_no_passage();
_164:
    /* (1452 from 1451)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1452);
goto _215;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_216:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 4720 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_482:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 4737 "Lexer.cpp"

}
goto _1;
_516:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{

#   line 33 "Lexer.qx"


#   line 4749 "Lexer.cpp"

}
goto _0;
_492:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 35 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4764 "Lexer.cpp"

}
goto _0;
_499:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 36 "Lexer.qx"
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4779 "Lexer.cpp"

}
goto _0;
_511:
    __quex_debug("* TERMINAL \"!\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 37 "Lexer.qx"
self_send(RULE_TOKEN_NEGATION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4794 "Lexer.cpp"

}
goto _0;
_483:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 39 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4809 "Lexer.cpp"

}
goto _0;
_513:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 40 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4824 "Lexer.cpp"

}
goto _0;
_484:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 41 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4839 "Lexer.cpp"

}
goto _0;
_512:
    __quex_debug("* TERMINAL \"match\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 42 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_MATCH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4854 "Lexer.cpp"

}
goto _0;
_494:
    __quex_debug("* TERMINAL \"device\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 43 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_DEVICE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4869 "Lexer.cpp"

}
goto _0;
_496:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 45 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4884 "Lexer.cpp"

}
goto _0;
_486:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 46 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4899 "Lexer.cpp"

}
goto _0;
_487:
    __quex_debug("* TERMINAL \"via-port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{

#   line 47 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_VIAPORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4914 "Lexer.cpp"

}
goto _0;
_509:
    __quex_debug("* TERMINAL \"with-interface\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 48 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_WITHINTERFACE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4929 "Lexer.cpp"

}
goto _0;
_489:
    __quex_debug("* TERMINAL \"serial\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 49 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_SERIAL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4944 "Lexer.cpp"

}
goto _0;
_506:
    __quex_debug("* TERMINAL \"if\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{

#   line 50 "Lexer.qx"
self_send(RULE_TOKEN_KEYWORD_IF);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4959 "Lexer.cpp"

}
goto _0;
_514:
    __quex_debug("* TERMINAL \"all-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 52 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4975 "Lexer.cpp"

}
goto _0;
_493:
    __quex_debug("* TERMINAL \"any-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 53 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 4991 "Lexer.cpp"

}
goto _0;
_497:
    __quex_debug("* TERMINAL \"one-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 54 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5007 "Lexer.cpp"

}
goto _0;
_488:
    __quex_debug("* TERMINAL \"none-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{

#   line 55 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5023 "Lexer.cpp"

}
goto _0;
_491:
    __quex_debug("* TERMINAL \"equals\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 56 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5039 "Lexer.cpp"

}
goto _0;
_504:
    __quex_debug("* TERMINAL \"equals-ordered\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{

#   line 57 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5055 "Lexer.cpp"

}
goto _0;
_505:
    __quex_debug("* TERMINAL \"allowed-matches\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(15);
{

#   line 59 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5071 "Lexer.cpp"

}
goto _0;
_501:
    __quex_debug("* TERMINAL \"localtime\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{

#   line 60 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5087 "Lexer.cpp"

}
goto _0;
_495:
    __quex_debug("* TERMINAL \"true\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{

#   line 61 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5103 "Lexer.cpp"

}
goto _0;
_500:
    __quex_debug("* TERMINAL \"false\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{

#   line 62 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5119 "Lexer.cpp"

}
goto _0;
_508:
    __quex_debug("* TERMINAL \"random\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 63 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_CONDITION_IDENTIFIER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5135 "Lexer.cpp"

}
goto _0;
_498:
    __quex_debug("* TERMINAL [:xdigit:]{4}\":\"[:xdigit:]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{

#   line 65 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_DEVICE_VPID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5151 "Lexer.cpp"

}
goto _0;
_490:
    __quex_debug("* TERMINAL [:xdigit:]{4}\":*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 66 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_DEVICE_VID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5167 "Lexer.cpp"

}
goto _0;
_515:
    __quex_debug("* TERMINAL \"*:*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(3);
{

#   line 67 "Lexer.qx"
self_send(RULE_TOKEN_DEVICE_ANYID);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5182 "Lexer.cpp"

}
goto _0;
_485:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":\"[:xdigit:]{2}\":\"[:xdigit:]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{

#   line 69 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_FULL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5198 "Lexer.cpp"

}
goto _0;
_510:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":\"[:xdigit:]{2}\":*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{

#   line 70 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_SUBCLASS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5214 "Lexer.cpp"

}
goto _0;
_502:
    __quex_debug("* TERMINAL [:xdigit:]{2}\":*:*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{

#   line 71 "Lexer.qx"
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._read_p);
self_send(RULE_TOKEN_INTERFACE_TYPE_CLASS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 5230 "Lexer.cpp"

}
goto _0;
_503:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 77 "Lexer.qx"
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 5246 "Lexer.cpp"

}
goto _0;
_507:
    __quex_debug("* TERMINAL \"(\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 83 "Lexer.qx"
    self_send(RULE_TOKEN_PQ_STRING_BEGIN);
    self << PQ_STRING_READER;
    RETURN;


#   line 5262 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_814:
    switch( target_state_index ) {
        case 3: { goto _3; }
        case 165: { goto _165; }
        case 167: { goto _167; }
        case 185: { goto _185; }
        case 186: { goto _186; }
        case 187: { goto _187; }
        case 188: { goto _188; }
        case 189: { goto _189; }
        case 190: { goto _190; }
        case 191: { goto _191; }
        case 192: { goto _192; }
        case 193: { goto _193; }
        case 194: { goto _194; }
        case 198: { goto _198; }
        case 199: { goto _199; }
        case 200: { goto _200; }
        case 201: { goto _201; }
        case 202: { goto _202; }
        case 203: { goto _203; }
        case 204: { goto _204; }
        case 205: { goto _205; }
        case 216: { goto _216; }
        case 218: { goto _218; }
        case 220: { goto _220; }
        case 222: { goto _222; }
        case 224: { goto _224; }
        case 226: { goto _226; }
        case 228: { goto _228; }
        case 230: { goto _230; }
        case 232: { goto _232; }
        case 234: { goto _234; }
        case 236: { goto _236; }
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

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_285:
    /* (RELOAD_FORWARD from 1336)  */
    target_state_index = QUEX_LABEL(284); target_state_else_index = QUEX_LABEL(165);

_812:
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
_433:
    /* (RELOAD_FORWARD from 1423)  */
    target_state_index = QUEX_LABEL(432); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_251:
    /* (RELOAD_FORWARD from 1313)  */
    target_state_index = QUEX_LABEL(250); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_383:
    /* (RELOAD_FORWARD from 1393)  */
    target_state_index = QUEX_LABEL(382); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_229:
    /* (RELOAD_FORWARD from 1300)  */
    target_state_index = QUEX_LABEL(228); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_345:
    /* (RELOAD_FORWARD from 1372)  */
    target_state_index = QUEX_LABEL(344); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_239:
    /* (RELOAD_FORWARD from 1307)  */
    target_state_index = QUEX_LABEL(238); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_451:
    /* (RELOAD_FORWARD from 1435)  */
    target_state_index = QUEX_LABEL(450); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_329:
    /* (RELOAD_FORWARD from 1364)  */
    target_state_index = QUEX_LABEL(328); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_461:
    /* (RELOAD_FORWARD from 1441)  */
    target_state_index = QUEX_LABEL(460); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_307:
    /* (RELOAD_FORWARD from 1351)  */
    target_state_index = QUEX_LABEL(306); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_439:
    /* (RELOAD_FORWARD from 1427)  */
    target_state_index = QUEX_LABEL(438); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_273:
    /* (RELOAD_FORWARD from 1327)  */
    target_state_index = QUEX_LABEL(272); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_405:
    /* (RELOAD_FORWARD from 1406)  */
    target_state_index = QUEX_LABEL(404); target_state_else_index = QUEX_LABEL(201);

    goto _812;


    __quex_assert_no_passage();
_219:
    /* (RELOAD_FORWARD from 1292)  */
    target_state_index = QUEX_LABEL(218); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_351:
    /* (RELOAD_FORWARD from 1375)  */
    target_state_index = QUEX_LABEL(350); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_325:
    /* (RELOAD_FORWARD from 1361)  */
    target_state_index = QUEX_LABEL(324); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_441:
    /* (RELOAD_FORWARD from 1429)  */
    target_state_index = QUEX_LABEL(440); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_335:
    /* (RELOAD_FORWARD from 1367)  */
    target_state_index = QUEX_LABEL(334); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_419:
    /* (RELOAD_FORWARD from 1414)  */
    target_state_index = QUEX_LABEL(418); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_297:
    /* (RELOAD_FORWARD from 1344)  */
    target_state_index = QUEX_LABEL(296); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_429:
    /* (RELOAD_FORWARD from 1420)  */
    target_state_index = QUEX_LABEL(428); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_473:
    /* (RELOAD_FORWARD from 1448)  */
    target_state_index = QUEX_LABEL(472); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_385:
    /* (RELOAD_FORWARD from 1395)  */
    target_state_index = QUEX_LABEL(384); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_279:
    /* (RELOAD_FORWARD from 1332)  */
    target_state_index = QUEX_LABEL(278); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_395:
    /* (RELOAD_FORWARD from 1401)  */
    target_state_index = QUEX_LABEL(394); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_241:
    /* (RELOAD_FORWARD from 1308)  */
    target_state_index = QUEX_LABEL(240); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_373:
    /* (RELOAD_FORWARD from 1387)  */
    target_state_index = QUEX_LABEL(372); target_state_else_index = QUEX_LABEL(190);

    goto _812;


    __quex_assert_no_passage();
_315:
    /* (RELOAD_FORWARD from 1356)  */
    target_state_index = QUEX_LABEL(314); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_447:
    /* (RELOAD_FORWARD from 1432)  */
    target_state_index = QUEX_LABEL(446); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_293:
    /* (RELOAD_FORWARD from 1342)  */
    target_state_index = QUEX_LABEL(292); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_409:
    /* (RELOAD_FORWARD from 1408)  */
    target_state_index = QUEX_LABEL(408); target_state_else_index = QUEX_LABEL(203);

    goto _812;


    __quex_assert_no_passage();
_303:
    /* (RELOAD_FORWARD from 1348)  */
    target_state_index = QUEX_LABEL(302); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_259:
    /* (RELOAD_FORWARD from 1317)  */
    target_state_index = QUEX_LABEL(258); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_391:
    /* (RELOAD_FORWARD from 1399)  */
    target_state_index = QUEX_LABEL(390); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_269:
    /* (RELOAD_FORWARD from 1325)  */
    target_state_index = QUEX_LABEL(268); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_353:
    /* (RELOAD_FORWARD from 1377)  */
    target_state_index = QUEX_LABEL(352); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_247:
    /* (RELOAD_FORWARD from 1311)  */
    target_state_index = QUEX_LABEL(246); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_363:
    /* (RELOAD_FORWARD from 1382)  */
    target_state_index = QUEX_LABEL(362); target_state_else_index = QUEX_LABEL(185);

    goto _812;


    __quex_assert_no_passage();
_337:
    /* (RELOAD_FORWARD from 1368)  */
    target_state_index = QUEX_LABEL(336); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_469:
    /* (RELOAD_FORWARD from 1446)  */
    target_state_index = QUEX_LABEL(468); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_283:
    /* (RELOAD_FORWARD from 1334)  */
    target_state_index = QUEX_LABEL(282); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_415:
    /* (RELOAD_FORWARD from 1412)  */
    target_state_index = QUEX_LABEL(414); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_249:
    /* (RELOAD_FORWARD from 1312)  */
    target_state_index = QUEX_LABEL(248); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_381:
    /* (RELOAD_FORWARD from 1391)  */
    target_state_index = QUEX_LABEL(380); target_state_else_index = QUEX_LABEL(194);

    goto _812;


    __quex_assert_no_passage();
_227:
    /* (RELOAD_FORWARD from 1297)  */
    target_state_index = QUEX_LABEL(226); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_359:
    /* (RELOAD_FORWARD from 1380)  */
    target_state_index = QUEX_LABEL(358); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_237:
    /* (RELOAD_FORWARD from 1306)  */
    target_state_index = QUEX_LABEL(236); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_449:
    /* (RELOAD_FORWARD from 1433)  */
    target_state_index = QUEX_LABEL(448); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_343:
    /* (RELOAD_FORWARD from 1371)  */
    target_state_index = QUEX_LABEL(342); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_459:
    /* (RELOAD_FORWARD from 1440)  */
    target_state_index = QUEX_LABEL(458); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_305:
    /* (RELOAD_FORWARD from 1349)  */
    target_state_index = QUEX_LABEL(304); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_437:
    /* (RELOAD_FORWARD from 1426)  */
    target_state_index = QUEX_LABEL(436); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_255:
    /* (RELOAD_FORWARD from 1315)  */
    target_state_index = QUEX_LABEL(254); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_403:
    /* (RELOAD_FORWARD from 1405)  */
    target_state_index = QUEX_LABEL(402); target_state_else_index = QUEX_LABEL(200);

    goto _812;


    __quex_assert_no_passage();
_217:
    /* (RELOAD_FORWARD from 1291)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(216);

    goto _812;


    __quex_assert_no_passage();
_349:
    /* (RELOAD_FORWARD from 1374)  */
    target_state_index = QUEX_LABEL(348); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_323:
    /* (RELOAD_FORWARD from 1360)  */
    target_state_index = QUEX_LABEL(322); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_455:
    /* (RELOAD_FORWARD from 1437)  */
    target_state_index = QUEX_LABEL(454); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_333:
    /* (RELOAD_FORWARD from 1366)  */
    target_state_index = QUEX_LABEL(332); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_417:
    /* (RELOAD_FORWARD from 1413)  */
    target_state_index = QUEX_LABEL(416); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_311:
    /* (RELOAD_FORWARD from 1353)  */
    target_state_index = QUEX_LABEL(310); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_427:
    /* (RELOAD_FORWARD from 1419)  */
    target_state_index = QUEX_LABEL(426); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_277:
    /* (RELOAD_FORWARD from 1330)  */
    target_state_index = QUEX_LABEL(276); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_393:
    /* (RELOAD_FORWARD from 1400)  */
    target_state_index = QUEX_LABEL(392); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_223:
    /* (RELOAD_FORWARD from 1295)  */
    target_state_index = QUEX_LABEL(222); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_371:
    /* (RELOAD_FORWARD from 1386)  */
    target_state_index = QUEX_LABEL(370); target_state_else_index = QUEX_LABEL(189);

    goto _812;


    __quex_assert_no_passage();
_313:
    /* (RELOAD_FORWARD from 1354)  */
    target_state_index = QUEX_LABEL(312); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_445:
    /* (RELOAD_FORWARD from 1431)  */
    target_state_index = QUEX_LABEL(444); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_291:
    /* (RELOAD_FORWARD from 1341)  */
    target_state_index = QUEX_LABEL(290); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_423:
    /* (RELOAD_FORWARD from 1417)  */
    target_state_index = QUEX_LABEL(422); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_301:
    /* (RELOAD_FORWARD from 1347)  */
    target_state_index = QUEX_LABEL(300); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_257:
    /* (RELOAD_FORWARD from 1316)  */
    target_state_index = QUEX_LABEL(256); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_477:
    /* (RELOAD_FORWARD from 1451)  */
    target_state_index = QUEX_LABEL(476); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_389:
    /* (RELOAD_FORWARD from 1397)  */
    target_state_index = QUEX_LABEL(388); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_267:
    /* (RELOAD_FORWARD from 1324)  */
    target_state_index = QUEX_LABEL(266); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_399:
    /* (RELOAD_FORWARD from 1403)  */
    target_state_index = QUEX_LABEL(398); target_state_else_index = QUEX_LABEL(198);

    goto _812;


    __quex_assert_no_passage();
_245:
    /* (RELOAD_FORWARD from 1310)  */
    target_state_index = QUEX_LABEL(244); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_361:
    /* (RELOAD_FORWARD from 1381)  */
    target_state_index = QUEX_LABEL(360); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_319:
    /* (RELOAD_FORWARD from 1358)  */
    target_state_index = QUEX_LABEL(318); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_467:
    /* (RELOAD_FORWARD from 1445)  */
    target_state_index = QUEX_LABEL(466); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_281:
    /* (RELOAD_FORWARD from 1333)  */
    target_state_index = QUEX_LABEL(280); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_413:
    /* (RELOAD_FORWARD from 1410)  */
    target_state_index = QUEX_LABEL(412); target_state_else_index = QUEX_LABEL(205);

    goto _812;


    __quex_assert_no_passage();
_263:
    /* (RELOAD_FORWARD from 1322)  */
    target_state_index = QUEX_LABEL(262); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_379:
    /* (RELOAD_FORWARD from 1390)  */
    target_state_index = QUEX_LABEL(378); target_state_else_index = QUEX_LABEL(193);

    goto _812;


    __quex_assert_no_passage();
_225:
    /* (RELOAD_FORWARD from 1296)  */
    target_state_index = QUEX_LABEL(224); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_357:
    /* (RELOAD_FORWARD from 1379)  */
    target_state_index = QUEX_LABEL(356); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_235:
    /* (RELOAD_FORWARD from 1305)  */
    target_state_index = QUEX_LABEL(234); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_367:
    /* (RELOAD_FORWARD from 1384)  */
    target_state_index = QUEX_LABEL(366); target_state_else_index = QUEX_LABEL(187);

    goto _812;


    __quex_assert_no_passage();
_341:
    /* (RELOAD_FORWARD from 1370)  */
    target_state_index = QUEX_LABEL(340); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_457:
    /* (RELOAD_FORWARD from 1439)  */
    target_state_index = QUEX_LABEL(456); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_287:
    /* (RELOAD_FORWARD from 1337)  */
    target_state_index = QUEX_LABEL(286); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_435:
    /* (RELOAD_FORWARD from 1425)  */
    target_state_index = QUEX_LABEL(434); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_253:
    /* (RELOAD_FORWARD from 1314)  */
    target_state_index = QUEX_LABEL(252); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_401:
    /* (RELOAD_FORWARD from 1404)  */
    target_state_index = QUEX_LABEL(400); target_state_else_index = QUEX_LABEL(199);

    goto _812;


    __quex_assert_no_passage();
_231:
    /* (RELOAD_FORWARD from 1303)  */
    target_state_index = QUEX_LABEL(230); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_347:
    /* (RELOAD_FORWARD from 1373)  */
    target_state_index = QUEX_LABEL(346); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_321:
    /* (RELOAD_FORWARD from 1359)  */
    target_state_index = QUEX_LABEL(320); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_453:
    /* (RELOAD_FORWARD from 1436)  */
    target_state_index = QUEX_LABEL(452); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_331:
    /* (RELOAD_FORWARD from 1365)  */
    target_state_index = QUEX_LABEL(330); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_463:
    /* (RELOAD_FORWARD from 1443)  */
    target_state_index = QUEX_LABEL(462); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_309:
    /* (RELOAD_FORWARD from 1352)  */
    target_state_index = QUEX_LABEL(308); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_425:
    /* (RELOAD_FORWARD from 1418)  */
    target_state_index = QUEX_LABEL(424); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_275:
    /* (RELOAD_FORWARD from 1329)  */
    target_state_index = QUEX_LABEL(274); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_407:
    /* (RELOAD_FORWARD from 1407)  */
    target_state_index = QUEX_LABEL(406); target_state_else_index = QUEX_LABEL(202);

    goto _812;


    __quex_assert_no_passage();
_221:
    /* (RELOAD_FORWARD from 1294)  */
    target_state_index = QUEX_LABEL(220); target_state_else_index = QUEX_LABEL(167);

    goto _812;


    __quex_assert_no_passage();
_369:
    /* (RELOAD_FORWARD from 1385)  */
    target_state_index = QUEX_LABEL(368); target_state_else_index = QUEX_LABEL(188);

    goto _812;


    __quex_assert_no_passage();
_327:
    /* (RELOAD_FORWARD from 1362)  */
    target_state_index = QUEX_LABEL(326); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_443:
    /* (RELOAD_FORWARD from 1430)  */
    target_state_index = QUEX_LABEL(442); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_289:
    /* (RELOAD_FORWARD from 1338)  */
    target_state_index = QUEX_LABEL(288); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_421:
    /* (RELOAD_FORWARD from 1416)  */
    target_state_index = QUEX_LABEL(420); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_299:
    /* (RELOAD_FORWARD from 1346)  */
    target_state_index = QUEX_LABEL(298); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_431:
    /* (RELOAD_FORWARD from 1421)  */
    target_state_index = QUEX_LABEL(430); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_475:
    /* (RELOAD_FORWARD from 1450)  */
    target_state_index = QUEX_LABEL(474); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_387:
    /* (RELOAD_FORWARD from 1396)  */
    target_state_index = QUEX_LABEL(386); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_265:
    /* (RELOAD_FORWARD from 1323)  */
    target_state_index = QUEX_LABEL(264); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_397:
    /* (RELOAD_FORWARD from 1402)  */
    target_state_index = QUEX_LABEL(396); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_243:
    /* (RELOAD_FORWARD from 1309)  */
    target_state_index = QUEX_LABEL(242); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_375:
    /* (RELOAD_FORWARD from 1388)  */
    target_state_index = QUEX_LABEL(374); target_state_else_index = QUEX_LABEL(191);

    goto _812;


    __quex_assert_no_passage();
_317:
    /* (RELOAD_FORWARD from 1357)  */
    target_state_index = QUEX_LABEL(316); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_465:
    /* (RELOAD_FORWARD from 1444)  */
    target_state_index = QUEX_LABEL(464); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_295:
    /* (RELOAD_FORWARD from 1343)  */
    target_state_index = QUEX_LABEL(294); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_411:
    /* (RELOAD_FORWARD from 1409)  */
    target_state_index = QUEX_LABEL(410); target_state_else_index = QUEX_LABEL(204);

    goto _812;


    __quex_assert_no_passage();
_261:
    /* (RELOAD_FORWARD from 1320)  */
    target_state_index = QUEX_LABEL(260); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_377:
    /* (RELOAD_FORWARD from 1389)  */
    target_state_index = QUEX_LABEL(376); target_state_else_index = QUEX_LABEL(192);

    goto _812;


    __quex_assert_no_passage();
_271:
    /* (RELOAD_FORWARD from 1326)  */
    target_state_index = QUEX_LABEL(270); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_355:
    /* (RELOAD_FORWARD from 1378)  */
    target_state_index = QUEX_LABEL(354); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_233:
    /* (RELOAD_FORWARD from 1304)  */
    target_state_index = QUEX_LABEL(232); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_365:
    /* (RELOAD_FORWARD from 1383)  */
    target_state_index = QUEX_LABEL(364); target_state_else_index = QUEX_LABEL(186);

    goto _812;


    __quex_assert_no_passage();
_339:
    /* (RELOAD_FORWARD from 1369)  */
    target_state_index = QUEX_LABEL(338); target_state_else_index = QUEX_LABEL(165);

    goto _812;


    __quex_assert_no_passage();
_471:
    /* (RELOAD_FORWARD from 1447)  */
    target_state_index = QUEX_LABEL(470); target_state_else_index = QUEX_LABEL(165);

    goto _812;

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


goto _813;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _814; /* in QUEX_GOTO_STATE       */
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
    /* (1532 from BEFORE_ENTRY) (1532 from RELOAD_FORWARD)  */

    input = *(me->buffer._read_p);


    __quex_debug("Init State\n");
    __quex_debug_state(1532);
switch( input ) {
case 0x0: goto _25;
case 0x1: 
case 0xA: goto _12;
case 0x22: goto _10;
case 0x5C: goto _11;
default: goto _9;
}


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 1533) (DROP_OUT from 1535)  */

    goto _32;
_30:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 1540)  */
    goto _34;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 1537)  */
    goto _35;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 1536)  */
    goto _36;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 1534)  */
    goto _37;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 1532)  */
        me->buffer._read_p = me->buffer._lexeme_start_p + 1;
goto _33;
    goto _30;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 1539)  */
        me->buffer._read_p -= 2;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 1538)  */
        me->buffer._read_p -= 1;
goto _32;
    goto _30;


    __quex_assert_no_passage();
_4:
    /* (1536 from 1535)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1536);
goto _15;


    __quex_assert_no_passage();
_5:
    /* (1537 from 1535)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1537);
goto _16;


    __quex_assert_no_passage();
_6:
    /* (1538 from 1535)  */
    ++(me->buffer._read_p);

_41:
    input = *(me->buffer._read_p);


    __quex_debug_state(1538);
switch( input ) {
case 0x0: goto _21;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _7;
default: goto _17;
}


    __quex_assert_no_passage();
_20:
    /* (1538 from RELOAD_FORWARD)  */
    goto _41;


    __quex_assert_no_passage();
_7:
    /* (1539 from 1538)  */
    ++(me->buffer._read_p);

_43:
    input = *(me->buffer._read_p);


    __quex_debug_state(1539);
switch( input ) {
case 0x0: goto _23;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _8;
default: goto _18;
}


    __quex_assert_no_passage();
_22:
    /* (1539 from RELOAD_FORWARD)  */
    goto _43;


    __quex_assert_no_passage();
_8:
    /* (1540 from 1539)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1540);
goto _19;


    __quex_assert_no_passage();
_9:
    /* (1533 from 1532)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1533);
goto _13;


    __quex_assert_no_passage();
_10:
    /* (1534 from 1532)  */
    ++(me->buffer._read_p);

    input = *(me->buffer._read_p);


    __quex_debug_state(1534);
goto _14;


    __quex_assert_no_passage();
_11:
    /* (1535 from 1532)  */
    ++(me->buffer._read_p);

_48:
    input = *(me->buffer._read_p);


    __quex_debug_state(1535);
switch( input ) {
case 0x0: goto _27;
case 0x22: goto _4;
case 0x5C: goto _5;
case 0x78: goto _6;
default: goto _13;
}


    __quex_assert_no_passage();
_26:
    /* (1535 from RELOAD_FORWARD)  */
    goto _48;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_24:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 6886 "Lexer.cpp"

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


#   line 6900 "Lexer.cpp"

}
goto _2;
_36:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 97 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 6915 "Lexer.cpp"

}
goto _0;
_35:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 100 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 6930 "Lexer.cpp"

}
goto _0;
_34:
    __quex_debug("* TERMINAL \"\\\\x\"[0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 103 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 6945 "Lexer.cpp"

}
goto _0;
_37:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{

#   line 108 "Lexer.qx"
    self << RULE;
    RETURN;


#   line 6960 "Lexer.cpp"

}
goto _0;
_32:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{

#   line 110 "Lexer.qx"
self_accumulator_add(Lexeme, LexemeEnd);


#   line 6974 "Lexer.cpp"

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
    /* (RELOAD_FORWARD from 1532)  */
    target_state_index = QUEX_LABEL(3); target_state_else_index = QUEX_LABEL(24);

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
    /* (RELOAD_FORWARD from 1538)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(17);

    goto _49;


    __quex_assert_no_passage();
_27:
    /* (RELOAD_FORWARD from 1535)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(13);

    goto _49;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 1539)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(18);

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

