
#include "Lexer.hpp"

#include <quex/code_base/analyzer/C-adaptions.h>
/* The file 'multi.i' contains implementations which are the same for all 
 * possibly generated analyzers. If QUEX_OPTION_MULTI is defined, it is
 * NOT supposed to be included here. If not--in which case we have a single
 * analzer--then it is included.                                             */
#include <quex/code_base/single.i>

QUEX_NAMESPACE_MAIN_OPEN
/* Global */QUEX_NAME(Mode)  QUEX_NAME(RULE);
/* Global */QUEX_NAME(Mode)  QUEX_NAME(DQ_STRING_READER);
#ifndef __QUEX_INDICATOR_DUMPED_TOKEN_ID_DEFINED
    static QUEX_TYPE_TOKEN_ID    QUEX_NAME_TOKEN(DumpedTokenIdObject);
#endif
#define self  (*(QUEX_TYPE_DERIVED_ANALYZER*)me)
#define __self_result_token_id    QUEX_NAME_TOKEN(DumpedTokenIdObject)

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
    default:
        if( Mode->has_base(&QUEX_NAME(DQ_STRING_READER)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): DQ_STRING_READER, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(RULE_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_DQ_STRING_READER): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(DQ_STRING_READER)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): DQ_STRING_READER, \n", Mode->name);
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
                QUEX_NAME(RULE_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif
#ifdef __QUEX_OPTION_COUNTER
static void
QUEX_NAME(RULE_counter)(QUEX_TYPE_ANALYZER* me, QUEX_TYPE_CHARACTER* LexemeBegin, QUEX_TYPE_CHARACTER* LexemeEnd)
{
#   define self (*me)
/*  'QUEX_GOTO_STATE' requires 'QUEX_LABEL_STATE_ROUTER' */
#   define QUEX_LABEL_STATE_ROUTER _18
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
#   ifdef QUEX_OPTION_COLUMN_NUMBER_COUNTING
    QUEX_TYPE_CHARACTER_POSITION   reference_p                    = (QUEX_TYPE_CHARACTER_POSITION)0x0;
#   endif /* QUEX_OPTION_COLUMN_NUMBER_COUNTING */
    (void)me;
    __QUEX_IF_COUNT_SHIFT_VALUES();
    /* Allow LexemeBegin == LexemeEnd (e.g. END_OF_STREAM)
     * => Caller does not need to check
     * BUT, if so quit immediately after 'shift values'. */
    __quex_assert(LexemeBegin <= LexemeEnd);
    if(LexemeBegin == LexemeEnd) return;
    me->buffer._input_p = LexemeBegin;

    /* (373 from NONE)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

    __quex_debug("Init State\n");
    __quex_debug_state(373);
_9:
    __quex_debug("Init State\n");
    __quex_debug_state(373);
if     ( input >= 0xB )  goto _2;
else if( input == 0xA )  goto _3;
else if( input == 0x9 )  goto _4;
else                     goto _2;


    __quex_assert_no_passage();
_8:
    /* (373 from 377)  */
    __quex_debug_state(373);
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 375)  */
    goto _11;
    __quex_debug("Drop-Out Catcher\n");

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 374)  */
    goto _12;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 376)  */
    goto _13;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_2:
    /* (376 from 373)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(376);

    __quex_debug_state(376);
goto _7;


    __quex_assert_no_passage();
_3:
    /* (374 from 373)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(374);

    __quex_debug_state(374);
goto _5;


    __quex_assert_no_passage();
_4:
    /* (375 from 373)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(375);

    __quex_debug_state(375);
goto _6;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_12:
    __quex_debug("* TERMINAL LINE\n");
__QUEX_IF_COUNT_LINES_ADD((size_t)1);

    __QUEX_IF_COUNT_COLUMNS((me->counter._column_number_at_end) = (size_t)1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_11:
    __quex_debug("* TERMINAL GRID\n");
__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._input_p) - reference_p - 1)));

__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end -= 1);
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end &= ~ ((size_t)0x3));
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end += 4 + 1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_13:
    __quex_debug("* TERMINAL COLUMN\n");
if( me->buffer._input_p != LexemeEnd ) goto _8;

__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._input_p) - reference_p)));

goto _0;

_17:
    __quex_debug("* TERMINAL <BEYOND>\n");
    --(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._input_p) - reference_p)));

goto _0;

_19: /* TERMINAL: FAILURE */
goto _17;
_0:
     __quex_assert(me->buffer._input_p == LexemeEnd); /* Otherwise, lexeme violates codec character boundaries. */
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

#include <quex/code_base/analyzer/member/basic>
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
#include <quex/code_base/temporary_macros_on>

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
#   define QUEX_LABEL_STATE_ROUTER _189
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
#   define LexemeL      ((size_t)(me->buffer._input_p - me->buffer._lexeme_start_p))
#   define LexemeEnd    me->buffer._input_p
#endif

#define LexemeNull      (&QUEX_LEXEME_NULL)
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_CHARACTER_POSITION*  position                       = 0x0;

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
_188:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (299 from NONE)  */
    __quex_debug("Init State\n");
    __quex_debug_state(299);
_96:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(299);

    __quex_debug("Init State\n");
    __quex_debug_state(299);
switch( input ) {
case 0x0: goto _64;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
case 0x22: goto _8;
case 0x2A: goto _15;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _5;
case 0x3A: goto _16;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _5;
case 0x61: goto _11;
case 0x62: goto _12;
case 0x63: case 0x64: case 0x65: case 0x66: goto _5;
case 0x68: goto _10;
case 0x6E: goto _7;
case 0x70: goto _9;
case 0x72: goto _14;
case 0x7B: goto _4;
case 0x7D: goto _6;
default: goto _47;
}


    __quex_assert_no_passage();
_62:
    /* (299 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(299);
    goto _96;


    __quex_assert_no_passage();
_47:
    /* (DROP_OUT from 323) (DROP_OUT from 305) (DROP_OUT from 320) (DROP_OUT from 318) (DROP_OUT from 340) (DROP_OUT from 315) (DROP_OUT from 327) (DROP_OUT from 330) (DROP_OUT from 337) (DROP_OUT from 299) (DROP_OUT from 306) (DROP_OUT from 319) (DROP_OUT from 334) (DROP_OUT from 341) (DROP_OUT from 316) (DROP_OUT from 303) (DROP_OUT from 310) (DROP_OUT from 331) (DROP_OUT from 338) (DROP_OUT from 313) (DROP_OUT from 325) (DROP_OUT from 307) (DROP_OUT from 328) (DROP_OUT from 322) (DROP_OUT from 335) (DROP_OUT from 332) (DROP_OUT from 314) (DROP_OUT from 326) (DROP_OUT from 301) (DROP_OUT from 308)  */
    __quex_debug("Drop-Out Catcher\n");

        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _99;
    __quex_debug("Drop-Out Catcher\n");

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_48:
    /* (DROP_OUT from 300)  */
    goto _100;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_61:
    /* (DROP_OUT from 342)  */
    goto _101;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_60:
    /* (DROP_OUT from 339)  */
    goto _102;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_54:
    /* (DROP_OUT from 317)  */
    goto _103;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_59:
    /* (DROP_OUT from 336)  */
    goto _104;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_53:
    /* (DROP_OUT from 312)  */
    goto _105;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_58:
    /* (DROP_OUT from 333)  */
    goto _106;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_52:
    /* (DROP_OUT from 311)  */
    goto _107;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_57:
    /* (DROP_OUT from 329)  */
    goto _108;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_51:
    /* (DROP_OUT from 309)  */
    goto _109;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_56:
    /* (DROP_OUT from 324)  */
    goto _110;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_50:
    /* (DROP_OUT from 304)  */
    goto _111;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_55:
    /* (DROP_OUT from 321)  */
    goto _112;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_49:
    /* (DROP_OUT from 302)  */
    goto _113;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_4:
    /* (300 from 299)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(300);

    __quex_debug_state(300);
goto _48;


    __quex_assert_no_passage();
_5:
    /* (301 from RELOAD_FORWARD) (301 from 299)  */
    __quex_debug_state(301);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(301);

    __quex_debug_state(301);
switch( input ) {
case 0x0: goto _65;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _23;
default: goto _47;
}


    __quex_assert_no_passage();
_6:
    /* (302 from 299)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(302);

    __quex_debug_state(302);
goto _49;


    __quex_assert_no_passage();
_7:
    /* (303 from 299) (303 from RELOAD_FORWARD)  */
    __quex_debug_state(303);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(303);

    __quex_debug_state(303);
if     ( input == 0x61 )  goto _44;
else if( input == 0x0 )   goto _66;
else                      goto _47;


    __quex_assert_no_passage();
_8:
    /* (304 from 299)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(304);

    __quex_debug_state(304);
goto _50;


    __quex_assert_no_passage();
_9:
    /* (305 from RELOAD_FORWARD) (305 from 299)  */
    __quex_debug_state(305);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(305);

    __quex_debug_state(305);
if     ( input == 0x6F )  goto _41;
else if( input == 0x0 )   goto _67;
else                      goto _47;


    __quex_assert_no_passage();
_10:
    /* (306 from RELOAD_FORWARD) (306 from 299)  */
    __quex_debug_state(306);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(306);

    __quex_debug_state(306);
if     ( input == 0x61 )  goto _38;
else if( input == 0x0 )   goto _68;
else                      goto _47;


    __quex_assert_no_passage();
_11:
    /* (307 from 299) (307 from RELOAD_FORWARD)  */
    __quex_debug_state(307);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(307);

    __quex_debug_state(307);
switch( input ) {
case 0x0: goto _69;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: goto _23;
case 0x63: goto _29;
case 0x64: case 0x65: case 0x66: goto _23;
case 0x6C: goto _30;
default: goto _47;
}


    __quex_assert_no_passage();
_12:
    /* (308 from 299) (308 from RELOAD_FORWARD)  */
    __quex_debug_state(308);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(308);

    __quex_debug_state(308);
switch( input ) {
case 0x0: goto _70;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _23;
case 0x6C: goto _22;
default: goto _47;
}


    __quex_assert_no_passage();
_13:
    /* (309 from 309) (309 from RELOAD_FORWARD) (309 from 299)  */
    __quex_debug_state(309);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(309);

    __quex_debug_state(309);
switch( input ) {
case 0x0: goto _71;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
default: goto _51;
}


    __quex_assert_no_passage();
_14:
    /* (310 from RELOAD_FORWARD) (310 from 299)  */
    __quex_debug_state(310);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(310);

    __quex_debug_state(310);
if     ( input == 0x65 )  goto _17;
else if( input == 0x0 )   goto _72;
else                      goto _47;


    __quex_assert_no_passage();
_15:
    /* (311 from 299)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(311);

    __quex_debug_state(311);
goto _52;


    __quex_assert_no_passage();
_16:
    /* (312 from 299)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(312);

    __quex_debug_state(312);
goto _53;


    __quex_assert_no_passage();
_17:
    /* (313 from RELOAD_FORWARD) (313 from 310)  */
    __quex_debug_state(313);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(313);

    __quex_debug_state(313);
if     ( input == 0x6A )  goto _18;
else if( input == 0x0 )   goto _73;
else                      goto _47;


    __quex_assert_no_passage();
_18:
    /* (314 from RELOAD_FORWARD) (314 from 313)  */
    __quex_debug_state(314);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(314);

    __quex_debug_state(314);
if     ( input == 0x65 )  goto _19;
else if( input == 0x0 )   goto _74;
else                      goto _47;


    __quex_assert_no_passage();
_19:
    /* (315 from 314) (315 from RELOAD_FORWARD)  */
    __quex_debug_state(315);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(315);

    __quex_debug_state(315);
if     ( input == 0x63 )  goto _20;
else if( input == 0x0 )   goto _75;
else                      goto _47;


    __quex_assert_no_passage();
_20:
    /* (316 from 315) (316 from RELOAD_FORWARD)  */
    __quex_debug_state(316);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(316);

    __quex_debug_state(316);
if     ( input == 0x74 )  goto _21;
else if( input == 0x0 )   goto _76;
else                      goto _47;


    __quex_assert_no_passage();
_21:
    /* (317 from 316)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(317);

    __quex_debug_state(317);
goto _54;


    __quex_assert_no_passage();
_22:
    /* (318 from RELOAD_FORWARD) (318 from 308)  */
    __quex_debug_state(318);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(318);

    __quex_debug_state(318);
if     ( input == 0x6F )  goto _26;
else if( input == 0x0 )   goto _77;
else                      goto _47;


    __quex_assert_no_passage();
_23:
    /* (319 from 308) (319 from 301) (319 from 307) (319 from RELOAD_FORWARD)  */
    __quex_debug_state(319);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(319);

    __quex_debug_state(319);
switch( input ) {
case 0x0: goto _78;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _24;
default: goto _47;
}


    __quex_assert_no_passage();
_24:
    /* (320 from RELOAD_FORWARD) (320 from 319) (320 from 325)  */
    __quex_debug_state(320);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(320);

    __quex_debug_state(320);
switch( input ) {
case 0x0: goto _79;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _25;
default: goto _47;
}


    __quex_assert_no_passage();
_25:
    /* (321 from 320)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(321);

    __quex_debug_state(321);
goto _55;


    __quex_assert_no_passage();
_26:
    /* (322 from 318) (322 from RELOAD_FORWARD)  */
    __quex_debug_state(322);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(322);

    __quex_debug_state(322);
if     ( input == 0x63 )  goto _27;
else if( input == 0x0 )   goto _80;
else                      goto _47;


    __quex_assert_no_passage();
_27:
    /* (323 from 322) (323 from RELOAD_FORWARD)  */
    __quex_debug_state(323);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(323);

    __quex_debug_state(323);
if     ( input == 0x6B )  goto _28;
else if( input == 0x0 )   goto _81;
else                      goto _47;


    __quex_assert_no_passage();
_28:
    /* (324 from 323)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(324);

    __quex_debug_state(324);
goto _56;


    __quex_assert_no_passage();
_29:
    /* (325 from RELOAD_FORWARD) (325 from 307)  */
    __quex_debug_state(325);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(325);

    __quex_debug_state(325);
switch( input ) {
case 0x0: goto _82;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _24;
case 0x74: goto _34;
default: goto _47;
}


    __quex_assert_no_passage();
_30:
    /* (326 from RELOAD_FORWARD) (326 from 307)  */
    __quex_debug_state(326);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(326);

    __quex_debug_state(326);
if     ( input == 0x6C )  goto _31;
else if( input == 0x0 )   goto _83;
else                      goto _47;


    __quex_assert_no_passage();
_31:
    /* (327 from 326) (327 from RELOAD_FORWARD)  */
    __quex_debug_state(327);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(327);

    __quex_debug_state(327);
if     ( input == 0x6F )  goto _32;
else if( input == 0x0 )   goto _84;
else                      goto _47;


    __quex_assert_no_passage();
_32:
    /* (328 from RELOAD_FORWARD) (328 from 327)  */
    __quex_debug_state(328);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(328);

    __quex_debug_state(328);
if     ( input == 0x77 )  goto _33;
else if( input == 0x0 )   goto _85;
else                      goto _47;


    __quex_assert_no_passage();
_33:
    /* (329 from 328)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(329);

    __quex_debug_state(329);
goto _57;


    __quex_assert_no_passage();
_34:
    /* (330 from 325) (330 from RELOAD_FORWARD)  */
    __quex_debug_state(330);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(330);

    __quex_debug_state(330);
if     ( input == 0x69 )  goto _35;
else if( input == 0x0 )   goto _86;
else                      goto _47;


    __quex_assert_no_passage();
_35:
    /* (331 from 330) (331 from RELOAD_FORWARD)  */
    __quex_debug_state(331);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(331);

    __quex_debug_state(331);
if     ( input == 0x6F )  goto _36;
else if( input == 0x0 )   goto _87;
else                      goto _47;


    __quex_assert_no_passage();
_36:
    /* (332 from 331) (332 from RELOAD_FORWARD)  */
    __quex_debug_state(332);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(332);

    __quex_debug_state(332);
if     ( input == 0x6E )  goto _37;
else if( input == 0x0 )   goto _88;
else                      goto _47;


    __quex_assert_no_passage();
_37:
    /* (333 from 332)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(333);

    __quex_debug_state(333);
goto _58;


    __quex_assert_no_passage();
_38:
    /* (334 from RELOAD_FORWARD) (334 from 306)  */
    __quex_debug_state(334);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(334);

    __quex_debug_state(334);
if     ( input == 0x73 )  goto _39;
else if( input == 0x0 )   goto _89;
else                      goto _47;


    __quex_assert_no_passage();
_39:
    /* (335 from 334) (335 from RELOAD_FORWARD)  */
    __quex_debug_state(335);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(335);

    __quex_debug_state(335);
if     ( input == 0x68 )  goto _40;
else if( input == 0x0 )   goto _90;
else                      goto _47;


    __quex_assert_no_passage();
_40:
    /* (336 from 335)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(336);

    __quex_debug_state(336);
goto _59;


    __quex_assert_no_passage();
_41:
    /* (337 from 305) (337 from RELOAD_FORWARD)  */
    __quex_debug_state(337);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(337);

    __quex_debug_state(337);
if     ( input == 0x72 )  goto _42;
else if( input == 0x0 )   goto _91;
else                      goto _47;


    __quex_assert_no_passage();
_42:
    /* (338 from RELOAD_FORWARD) (338 from 337)  */
    __quex_debug_state(338);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(338);

    __quex_debug_state(338);
if     ( input == 0x74 )  goto _43;
else if( input == 0x0 )   goto _92;
else                      goto _47;


    __quex_assert_no_passage();
_43:
    /* (339 from 338)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(339);

    __quex_debug_state(339);
goto _60;


    __quex_assert_no_passage();
_44:
    /* (340 from 303) (340 from RELOAD_FORWARD)  */
    __quex_debug_state(340);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(340);

    __quex_debug_state(340);
if     ( input == 0x6D )  goto _45;
else if( input == 0x0 )   goto _93;
else                      goto _47;


    __quex_assert_no_passage();
_45:
    /* (341 from 340) (341 from RELOAD_FORWARD)  */
    __quex_debug_state(341);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(341);

    __quex_debug_state(341);
if     ( input == 0x65 )  goto _46;
else if( input == 0x0 )   goto _94;
else                      goto _47;


    __quex_assert_no_passage();
_46:
    /* (342 from 341)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(342);

    __quex_debug_state(342);
goto _61;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_63:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 1314 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_99:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 1330 "Lexer.cpp"

}
goto _1;
_109:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{


#   line 1340 "Lexer.cpp"

}
goto _0;
_107:
    __quex_debug("* TERMINAL \"*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_ASTERISK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1353 "Lexer.cpp"

}
goto _0;
_100:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1366 "Lexer.cpp"

}
goto _0;
_113:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1379 "Lexer.cpp"

}
goto _0;
_105:
    __quex_debug("* TERMINAL \":\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_COLON);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1392 "Lexer.cpp"

}
goto _0;
_112:
    __quex_debug("* TERMINAL [0-9a-fA-F]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR4);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1406 "Lexer.cpp"

}
goto _0;
_108:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1419 "Lexer.cpp"

}
goto _0;
_110:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1432 "Lexer.cpp"

}
goto _0;
_103:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1445 "Lexer.cpp"

}
goto _0;
_101:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1458 "Lexer.cpp"

}
goto _0;
_106:
    __quex_debug("* TERMINAL \"action\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_ACTION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1471 "Lexer.cpp"

}
goto _0;
_104:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1484 "Lexer.cpp"

}
goto _0;
_102:
    __quex_debug("* TERMINAL \"port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_PORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1497 "Lexer.cpp"

}
goto _0;
_111:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 1511 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_189:
    switch( target_state_index ) {
        case 5: { goto _5; }
        case 7: { goto _7; }
        case 9: { goto _9; }
        case 10: { goto _10; }
        case 11: { goto _11; }
        case 12: { goto _12; }
        case 13: { goto _13; }
        case 14: { goto _14; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 19: { goto _19; }
        case 20: { goto _20; }
        case 22: { goto _22; }
        case 23: { goto _23; }
        case 24: { goto _24; }
        case 26: { goto _26; }
        case 27: { goto _27; }
        case 29: { goto _29; }
        case 30: { goto _30; }
        case 31: { goto _31; }
        case 32: { goto _32; }
        case 34: { goto _34; }
        case 35: { goto _35; }
        case 36: { goto _36; }
        case 38: { goto _38; }
        case 39: { goto _39; }
        case 41: { goto _41; }
        case 42: { goto _42; }
        case 44: { goto _44; }
        case 45: { goto _45; }
        case 47: { goto _47; }
        case 51: { goto _51; }
        case 62: { goto _62; }
        case 63: { goto _63; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_85:
    /* (RELOAD_FORWARD from 328)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(47);

_187:
    __quex_debug3("RELOAD_FORWARD: success->%i; failure->%i", (int)target_state_index, (int)target_state_else_index);
    __quex_assert(*(me->buffer._input_p) == QUEX_SETTING_BUFFER_LIMIT_CODE);
    if( me->buffer._memory._end_of_file_p == 0x0 ) {

        __quex_debug_reload_before();          /* Report source position. */
        QUEX_NAME(buffer_reload_forward)(&me->buffer, (QUEX_TYPE_CHARACTER_POSITION*)position, PositionRegisterN);

        __quex_debug_reload_after();
        QUEX_GOTO_STATE(target_state_index);   /* may use 'computed goto' */
    }
    __quex_debug("reload impossible\n");
    QUEX_GOTO_STATE(target_state_else_index);  /* may use 'computed goto' */


    __quex_assert_no_passage();
_67:
    /* (RELOAD_FORWARD from 305)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_90:
    /* (RELOAD_FORWARD from 335)  */
    target_state_index = QUEX_LABEL(39); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_81:
    /* (RELOAD_FORWARD from 323)  */
    target_state_index = QUEX_LABEL(27); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_79:
    /* (RELOAD_FORWARD from 320)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_70:
    /* (RELOAD_FORWARD from 308)  */
    target_state_index = QUEX_LABEL(12); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_93:
    /* (RELOAD_FORWARD from 340)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_75:
    /* (RELOAD_FORWARD from 315)  */
    target_state_index = QUEX_LABEL(19); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_84:
    /* (RELOAD_FORWARD from 327)  */
    target_state_index = QUEX_LABEL(31); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_66:
    /* (RELOAD_FORWARD from 303)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_89:
    /* (RELOAD_FORWARD from 334)  */
    target_state_index = QUEX_LABEL(38); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_80:
    /* (RELOAD_FORWARD from 322)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_78:
    /* (RELOAD_FORWARD from 319)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_69:
    /* (RELOAD_FORWARD from 307)  */
    target_state_index = QUEX_LABEL(11); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_92:
    /* (RELOAD_FORWARD from 338)  */
    target_state_index = QUEX_LABEL(42); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_74:
    /* (RELOAD_FORWARD from 314)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_65:
    /* (RELOAD_FORWARD from 301)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_88:
    /* (RELOAD_FORWARD from 332)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_87:
    /* (RELOAD_FORWARD from 331)  */
    target_state_index = QUEX_LABEL(35); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_77:
    /* (RELOAD_FORWARD from 318)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_68:
    /* (RELOAD_FORWARD from 306)  */
    target_state_index = QUEX_LABEL(10); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_83:
    /* (RELOAD_FORWARD from 326)  */
    target_state_index = QUEX_LABEL(30); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_73:
    /* (RELOAD_FORWARD from 313)  */
    target_state_index = QUEX_LABEL(17); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_64:
    /* (RELOAD_FORWARD from 299)  */
    target_state_index = QUEX_LABEL(62); target_state_else_index = QUEX_LABEL(63);

    goto _187;


    __quex_assert_no_passage();
_86:
    /* (RELOAD_FORWARD from 330)  */
    target_state_index = QUEX_LABEL(34); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_76:
    /* (RELOAD_FORWARD from 316)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_91:
    /* (RELOAD_FORWARD from 337)  */
    target_state_index = QUEX_LABEL(41); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_82:
    /* (RELOAD_FORWARD from 325)  */
    target_state_index = QUEX_LABEL(29); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_72:
    /* (RELOAD_FORWARD from 310)  */
    target_state_index = QUEX_LABEL(14); target_state_else_index = QUEX_LABEL(47);

    goto _187;


    __quex_assert_no_passage();
_71:
    /* (RELOAD_FORWARD from 309)  */
    target_state_index = QUEX_LABEL(13); target_state_else_index = QUEX_LABEL(51);

    goto _187;


    __quex_assert_no_passage();
_94:
    /* (RELOAD_FORWARD from 341)  */
    target_state_index = QUEX_LABEL(45); target_state_else_index = QUEX_LABEL(47);

    goto _187;

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


goto _188;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _189; /* in QUEX_GOTO_STATE       */
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
#   undef RULE
#   undef DQ_STRING_READER
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
}
#include <quex/code_base/temporary_macros_off>
#ifdef      __QUEX_COUNT_VOID
#   undef   __QUEX_COUNT_VOID
#endif
#ifdef      __QUEX_OPTION_COUNTER
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) \
            do {                              \
                QUEX_NAME(RULE_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif

#include <quex/code_base/analyzer/member/basic>
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
#include <quex/code_base/temporary_macros_on>

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
#   define QUEX_LABEL_STATE_ROUTER _35
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
#   define LexemeL      ((size_t)(me->buffer._input_p - me->buffer._lexeme_start_p))
#   define LexemeEnd    me->buffer._input_p
#endif

#define LexemeNull      (&QUEX_LEXEME_NULL)
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;
    QUEX_TYPE_CHARACTER_POSITION*  position                       = 0x0;

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
_34:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (418 from NONE)  */
    __quex_debug("Init State\n");
    __quex_debug_state(418);
_19:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(418);

    __quex_debug("Init State\n");
    __quex_debug_state(418);
switch( input ) {
case 0x0: goto _16;
case 0x1: 
case 0xA: goto _9;
case 0x22: goto _6;
case 0x5C: goto _4;
default: goto _5;
}


    __quex_assert_no_passage();
_14:
    /* (418 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(418);
    goto _19;


    __quex_assert_no_passage();
_10:
    /* (DROP_OUT from 419) (DROP_OUT from 420)  */
    __quex_debug("Drop-Out Catcher\n");

    goto _22;
    __quex_debug("Drop-Out Catcher\n");
_20:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 422)  */
    goto _23;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_11:
    /* (DROP_OUT from 421)  */
    goto _25;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 423)  */
    goto _26;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_9:
    /* (DROP_OUT from 418)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _24;
    __quex_debug("Drop-Out Catcher\n");
    goto _20;


    __quex_assert_no_passage();
_4:
    /* (419 from 418) (419 from RELOAD_FORWARD)  */
    __quex_debug_state(419);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(419);

    __quex_debug_state(419);
switch( input ) {
case 0x0: goto _17;
case 0x22: goto _7;
case 0x5C: goto _8;
default: goto _10;
}


    __quex_assert_no_passage();
_5:
    /* (420 from 418)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(420);

    __quex_debug_state(420);
goto _10;


    __quex_assert_no_passage();
_6:
    /* (421 from 418)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(421);

    __quex_debug_state(421);
goto _11;


    __quex_assert_no_passage();
_7:
    /* (422 from 419)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(422);

    __quex_debug_state(422);
goto _12;


    __quex_assert_no_passage();
_8:
    /* (423 from 419)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(423);

    __quex_debug_state(423);
goto _13;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_15:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 2148 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _1;
_24:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
    self_accumulator_add(Lexeme, LexemeEnd);


#   line 2162 "Lexer.cpp"

}
goto _2;
_23:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2175 "Lexer.cpp"

}
goto _0;
_26:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2188 "Lexer.cpp"

}
goto _0;
_25:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self << RULE;
    RETURN;


#   line 2201 "Lexer.cpp"

}
goto _0;
_22:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2213 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_35:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 10: { goto _10; }
        case 14: { goto _14; }
        case 15: { goto _15; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_16:
    /* (RELOAD_FORWARD from 418)  */
    target_state_index = QUEX_LABEL(14); target_state_else_index = QUEX_LABEL(15);

_33:
    __quex_debug3("RELOAD_FORWARD: success->%i; failure->%i", (int)target_state_index, (int)target_state_else_index);
    __quex_assert(*(me->buffer._input_p) == QUEX_SETTING_BUFFER_LIMIT_CODE);
    if( me->buffer._memory._end_of_file_p == 0x0 ) {

        __quex_debug_reload_before();          /* Report source position. */
        QUEX_NAME(buffer_reload_forward)(&me->buffer, (QUEX_TYPE_CHARACTER_POSITION*)position, PositionRegisterN);

        __quex_debug_reload_after();
        QUEX_GOTO_STATE(target_state_index);   /* may use 'computed goto' */
    }
    __quex_debug("reload impossible\n");
    QUEX_GOTO_STATE(target_state_else_index);  /* may use 'computed goto' */


    __quex_assert_no_passage();
_17:
    /* (RELOAD_FORWARD from 419)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(10);

    goto _33;

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


goto _34;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _1; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _2; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _35; /* in QUEX_GOTO_STATE       */
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
#   undef RULE
#   undef DQ_STRING_READER
#   undef self
#   undef QUEX_LABEL_STATE_ROUTER
}
#include <quex/code_base/temporary_macros_off>
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
   static const char  token_id_str_ASTERISK[]         = "ASTERISK";
   static const char  token_id_str_COLON[]            = "COLON";
   static const char  token_id_str_CURLYBRACE_CLOSE[] = "CURLYBRACE_CLOSE";
   static const char  token_id_str_CURLYBRACE_OPEN[]  = "CURLYBRACE_OPEN";
   static const char  token_id_str_DQ_STRING[]        = "DQ_STRING";
   static const char  token_id_str_DQ_STRING_BEGIN[]  = "DQ_STRING_BEGIN";
   static const char  token_id_str_DQ_STRING_END[]    = "DQ_STRING_END";
   static const char  token_id_str_HEXCHAR4[]         = "HEXCHAR4";
   static const char  token_id_str_KEYWORD_ACTION[]   = "KEYWORD_ACTION";
   static const char  token_id_str_KEYWORD_ALLOW[]    = "KEYWORD_ALLOW";
   static const char  token_id_str_KEYWORD_BLOCK[]    = "KEYWORD_BLOCK";
   static const char  token_id_str_KEYWORD_HASH[]     = "KEYWORD_HASH";
   static const char  token_id_str_KEYWORD_NAME[]     = "KEYWORD_NAME";
   static const char  token_id_str_KEYWORD_PORT[]     = "KEYWORD_PORT";
   static const char  token_id_str_KEYWORD_REJECT[]   = "KEYWORD_REJECT";
       

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
   case RULE_TOKEN_ASTERISK:         return token_id_str_ASTERISK;
   case RULE_TOKEN_COLON:            return token_id_str_COLON;
   case RULE_TOKEN_CURLYBRACE_CLOSE: return token_id_str_CURLYBRACE_CLOSE;
   case RULE_TOKEN_CURLYBRACE_OPEN:  return token_id_str_CURLYBRACE_OPEN;
   case RULE_TOKEN_DQ_STRING:        return token_id_str_DQ_STRING;
   case RULE_TOKEN_DQ_STRING_BEGIN:  return token_id_str_DQ_STRING_BEGIN;
   case RULE_TOKEN_DQ_STRING_END:    return token_id_str_DQ_STRING_END;
   case RULE_TOKEN_HEXCHAR4:         return token_id_str_HEXCHAR4;
   case RULE_TOKEN_KEYWORD_ACTION:   return token_id_str_KEYWORD_ACTION;
   case RULE_TOKEN_KEYWORD_ALLOW:    return token_id_str_KEYWORD_ALLOW;
   case RULE_TOKEN_KEYWORD_BLOCK:    return token_id_str_KEYWORD_BLOCK;
   case RULE_TOKEN_KEYWORD_HASH:     return token_id_str_KEYWORD_HASH;
   case RULE_TOKEN_KEYWORD_NAME:     return token_id_str_KEYWORD_NAME;
   case RULE_TOKEN_KEYWORD_PORT:     return token_id_str_KEYWORD_PORT;
   case RULE_TOKEN_KEYWORD_REJECT:   return token_id_str_KEYWORD_REJECT;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

