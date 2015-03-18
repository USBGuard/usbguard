
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

    /* (400 from NONE)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

    __quex_debug("Init State\n");
    __quex_debug_state(400);
_9:
    __quex_debug("Init State\n");
    __quex_debug_state(400);
if     ( input >= 0xB )  goto _2;
else if( input == 0xA )  goto _3;
else if( input == 0x9 )  goto _4;
else                     goto _2;


    __quex_assert_no_passage();
_8:
    /* (400 from 404)  */
    __quex_debug_state(400);
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 402)  */
    goto _11;
    __quex_debug("Drop-Out Catcher\n");

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 401)  */
    goto _12;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 403)  */
    goto _13;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_2:
    /* (401 from 400)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(401);

    __quex_debug_state(401);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (402 from 400)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(402);

    __quex_debug_state(402);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (403 from 400)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(403);

    __quex_debug_state(403);
goto _7;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_11:
    __quex_debug("* TERMINAL LINE\n");
__QUEX_IF_COUNT_LINES_ADD((size_t)1);

    __QUEX_IF_COUNT_COLUMNS((me->counter._column_number_at_end) = (size_t)1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_13:
    __quex_debug("* TERMINAL GRID\n");
__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._input_p) - reference_p - 1)));

__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end -= 1);
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end &= ~ ((size_t)0x3));
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end += 4 + 1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_12:
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
#   define QUEX_LABEL_STATE_ROUTER _205
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
_204:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (321 from NONE)  */
    __quex_debug("Init State\n");
    __quex_debug_state(321);
_104:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(321);

    __quex_debug("Init State\n");
    __quex_debug_state(321);
switch( input ) {
case 0x0: goto _69;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _8;
case 0x22: goto _12;
case 0x2A: goto _11;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _15;
case 0x3A: goto _13;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _15;
case 0x61: goto _4;
case 0x62: goto _15;
case 0x63: goto _17;
case 0x64: goto _16;
case 0x65: case 0x66: goto _15;
case 0x68: goto _5;
case 0x6E: goto _10;
case 0x70: goto _7;
case 0x72: goto _9;
case 0x7B: goto _14;
case 0x7D: goto _6;
default: goto _51;
}


    __quex_assert_no_passage();
_67:
    /* (321 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(321);
    goto _104;


    __quex_assert_no_passage();
_51:
    /* (DROP_OUT from 354) (DROP_OUT from 336) (DROP_OUT from 323) (DROP_OUT from 367) (DROP_OUT from 349) (DROP_OUT from 343) (DROP_OUT from 364) (DROP_OUT from 346) (DROP_OUT from 358) (DROP_OUT from 333) (DROP_OUT from 361) (DROP_OUT from 327) (DROP_OUT from 355) (DROP_OUT from 337) (DROP_OUT from 352) (DROP_OUT from 350) (DROP_OUT from 321) (DROP_OUT from 347) (DROP_OUT from 334) (DROP_OUT from 341) (DROP_OUT from 362) (DROP_OUT from 344) (DROP_OUT from 338) (DROP_OUT from 325) (DROP_OUT from 328) (DROP_OUT from 351) (DROP_OUT from 322) (DROP_OUT from 366) (DROP_OUT from 335) (DROP_OUT from 363) (DROP_OUT from 357) (DROP_OUT from 339) (DROP_OUT from 360)  */
    __quex_debug("Drop-Out Catcher\n");

        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _107;
    __quex_debug("Drop-Out Catcher\n");

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_52:
    /* (DROP_OUT from 324)  */
    goto _108;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_66:
    /* (DROP_OUT from 368)  */
    goto _109;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_65:
    /* (DROP_OUT from 365)  */
    goto _110;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_64:
    /* (DROP_OUT from 359)  */
    goto _111;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_58:
    /* (DROP_OUT from 340)  */
    goto _112;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_63:
    /* (DROP_OUT from 356)  */
    goto _113;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_57:
    /* (DROP_OUT from 332)  */
    goto _114;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_62:
    /* (DROP_OUT from 353)  */
    goto _115;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_56:
    /* (DROP_OUT from 331)  */
    goto _116;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_61:
    /* (DROP_OUT from 348)  */
    goto _117;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_55:
    /* (DROP_OUT from 330)  */
    goto _118;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_60:
    /* (DROP_OUT from 345)  */
    goto _119;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_54:
    /* (DROP_OUT from 329)  */
    goto _120;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_59:
    /* (DROP_OUT from 342)  */
    goto _121;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_53:
    /* (DROP_OUT from 326)  */
    goto _122;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_4:
    /* (322 from RELOAD_FORWARD) (322 from 321)  */
    __quex_debug_state(322);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(322);

    __quex_debug_state(322);
switch( input ) {
case 0x0: goto _70;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: goto _18;
case 0x63: goto _43;
case 0x64: case 0x65: case 0x66: goto _18;
case 0x6C: goto _42;
default: goto _51;
}


    __quex_assert_no_passage();
_5:
    /* (323 from RELOAD_FORWARD) (323 from 321)  */
    __quex_debug_state(323);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(323);

    __quex_debug_state(323);
if     ( input == 0x61 )  goto _39;
else if( input == 0x0 )   goto _71;
else                      goto _51;


    __quex_assert_no_passage();
_6:
    /* (324 from 321)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(324);

    __quex_debug_state(324);
goto _52;


    __quex_assert_no_passage();
_7:
    /* (325 from 321) (325 from RELOAD_FORWARD)  */
    __quex_debug_state(325);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(325);

    __quex_debug_state(325);
if     ( input == 0x6F )  goto _36;
else if( input == 0x0 )   goto _72;
else                      goto _51;


    __quex_assert_no_passage();
_8:
    /* (326 from 321) (326 from RELOAD_FORWARD) (326 from 326)  */
    __quex_debug_state(326);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(326);

    __quex_debug_state(326);
switch( input ) {
case 0x0: goto _73;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _8;
default: goto _53;
}


    __quex_assert_no_passage();
_9:
    /* (327 from RELOAD_FORWARD) (327 from 321)  */
    __quex_debug_state(327);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(327);

    __quex_debug_state(327);
if     ( input == 0x65 )  goto _31;
else if( input == 0x0 )   goto _74;
else                      goto _51;


    __quex_assert_no_passage();
_10:
    /* (328 from 321) (328 from RELOAD_FORWARD)  */
    __quex_debug_state(328);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(328);

    __quex_debug_state(328);
if     ( input == 0x61 )  goto _28;
else if( input == 0x0 )   goto _75;
else                      goto _51;


    __quex_assert_no_passage();
_11:
    /* (329 from 321)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(329);

    __quex_debug_state(329);
goto _54;


    __quex_assert_no_passage();
_12:
    /* (330 from 321)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(330);

    __quex_debug_state(330);
goto _55;


    __quex_assert_no_passage();
_13:
    /* (331 from 321)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(331);

    __quex_debug_state(331);
goto _56;


    __quex_assert_no_passage();
_14:
    /* (332 from 321)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(332);

    __quex_debug_state(332);
goto _57;


    __quex_assert_no_passage();
_15:
    /* (333 from 321) (333 from RELOAD_FORWARD)  */
    __quex_debug_state(333);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(333);

    __quex_debug_state(333);
switch( input ) {
case 0x0: goto _76;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _18;
default: goto _51;
}


    __quex_assert_no_passage();
_16:
    /* (334 from 321) (334 from RELOAD_FORWARD)  */
    __quex_debug_state(334);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(334);

    __quex_debug_state(334);
switch( input ) {
case 0x0: goto _77;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: goto _18;
case 0x65: goto _25;
case 0x66: goto _18;
default: goto _51;
}


    __quex_assert_no_passage();
_17:
    /* (335 from RELOAD_FORWARD) (335 from 321)  */
    __quex_debug_state(335);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(335);

    __quex_debug_state(335);
switch( input ) {
case 0x0: goto _78;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _18;
case 0x6C: goto _19;
default: goto _51;
}


    __quex_assert_no_passage();
_18:
    /* (336 from 322) (336 from RELOAD_FORWARD) (336 from 335) (336 from 334) (336 from 333)  */
    __quex_debug_state(336);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(336);

    __quex_debug_state(336);
switch( input ) {
case 0x0: goto _79;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _23;
default: goto _51;
}


    __quex_assert_no_passage();
_19:
    /* (337 from RELOAD_FORWARD) (337 from 335)  */
    __quex_debug_state(337);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(337);

    __quex_debug_state(337);
if     ( input == 0x61 )  goto _20;
else if( input == 0x0 )   goto _80;
else                      goto _51;


    __quex_assert_no_passage();
_20:
    /* (338 from RELOAD_FORWARD) (338 from 337)  */
    __quex_debug_state(338);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(338);

    __quex_debug_state(338);
if     ( input == 0x73 )  goto _21;
else if( input == 0x0 )   goto _81;
else                      goto _51;


    __quex_assert_no_passage();
_21:
    /* (339 from 338) (339 from RELOAD_FORWARD)  */
    __quex_debug_state(339);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(339);

    __quex_debug_state(339);
if     ( input == 0x73 )  goto _22;
else if( input == 0x0 )   goto _82;
else                      goto _51;


    __quex_assert_no_passage();
_22:
    /* (340 from 339)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(340);

    __quex_debug_state(340);
goto _58;


    __quex_assert_no_passage();
_23:
    /* (341 from 343) (341 from 336) (341 from 361) (341 from RELOAD_FORWARD)  */
    __quex_debug_state(341);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(341);

    __quex_debug_state(341);
switch( input ) {
case 0x0: goto _83;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _24;
default: goto _51;
}


    __quex_assert_no_passage();
_24:
    /* (342 from 341)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(342);

    __quex_debug_state(342);
goto _59;


    __quex_assert_no_passage();
_25:
    /* (343 from 334) (343 from RELOAD_FORWARD)  */
    __quex_debug_state(343);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(343);

    __quex_debug_state(343);
switch( input ) {
case 0x0: goto _84;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _23;
case 0x6E: goto _26;
default: goto _51;
}


    __quex_assert_no_passage();
_26:
    /* (344 from RELOAD_FORWARD) (344 from 343)  */
    __quex_debug_state(344);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(344);

    __quex_debug_state(344);
if     ( input == 0x79 )  goto _27;
else if( input == 0x0 )   goto _85;
else                      goto _51;


    __quex_assert_no_passage();
_27:
    /* (345 from 344)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(345);

    __quex_debug_state(345);
goto _60;


    __quex_assert_no_passage();
_28:
    /* (346 from RELOAD_FORWARD) (346 from 328)  */
    __quex_debug_state(346);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(346);

    __quex_debug_state(346);
if     ( input == 0x6D )  goto _29;
else if( input == 0x0 )   goto _86;
else                      goto _51;


    __quex_assert_no_passage();
_29:
    /* (347 from 346) (347 from RELOAD_FORWARD)  */
    __quex_debug_state(347);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(347);

    __quex_debug_state(347);
if     ( input == 0x65 )  goto _30;
else if( input == 0x0 )   goto _87;
else                      goto _51;


    __quex_assert_no_passage();
_30:
    /* (348 from 347)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(348);

    __quex_debug_state(348);
goto _61;


    __quex_assert_no_passage();
_31:
    /* (349 from 327) (349 from RELOAD_FORWARD)  */
    __quex_debug_state(349);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(349);

    __quex_debug_state(349);
if     ( input == 0x6A )  goto _32;
else if( input == 0x0 )   goto _88;
else                      goto _51;


    __quex_assert_no_passage();
_32:
    /* (350 from RELOAD_FORWARD) (350 from 349)  */
    __quex_debug_state(350);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(350);

    __quex_debug_state(350);
if     ( input == 0x65 )  goto _33;
else if( input == 0x0 )   goto _89;
else                      goto _51;


    __quex_assert_no_passage();
_33:
    /* (351 from 350) (351 from RELOAD_FORWARD)  */
    __quex_debug_state(351);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(351);

    __quex_debug_state(351);
if     ( input == 0x63 )  goto _34;
else if( input == 0x0 )   goto _90;
else                      goto _51;


    __quex_assert_no_passage();
_34:
    /* (352 from RELOAD_FORWARD) (352 from 351)  */
    __quex_debug_state(352);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(352);

    __quex_debug_state(352);
if     ( input == 0x74 )  goto _35;
else if( input == 0x0 )   goto _91;
else                      goto _51;


    __quex_assert_no_passage();
_35:
    /* (353 from 352)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(353);

    __quex_debug_state(353);
goto _62;


    __quex_assert_no_passage();
_36:
    /* (354 from 325) (354 from RELOAD_FORWARD)  */
    __quex_debug_state(354);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(354);

    __quex_debug_state(354);
if     ( input == 0x72 )  goto _37;
else if( input == 0x0 )   goto _92;
else                      goto _51;


    __quex_assert_no_passage();
_37:
    /* (355 from 354) (355 from RELOAD_FORWARD)  */
    __quex_debug_state(355);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(355);

    __quex_debug_state(355);
if     ( input == 0x74 )  goto _38;
else if( input == 0x0 )   goto _93;
else                      goto _51;


    __quex_assert_no_passage();
_38:
    /* (356 from 355)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(356);

    __quex_debug_state(356);
goto _63;


    __quex_assert_no_passage();
_39:
    /* (357 from RELOAD_FORWARD) (357 from 323)  */
    __quex_debug_state(357);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(357);

    __quex_debug_state(357);
if     ( input == 0x73 )  goto _40;
else if( input == 0x0 )   goto _94;
else                      goto _51;


    __quex_assert_no_passage();
_40:
    /* (358 from RELOAD_FORWARD) (358 from 357)  */
    __quex_debug_state(358);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(358);

    __quex_debug_state(358);
if     ( input == 0x68 )  goto _41;
else if( input == 0x0 )   goto _95;
else                      goto _51;


    __quex_assert_no_passage();
_41:
    /* (359 from 358)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(359);

    __quex_debug_state(359);
goto _64;


    __quex_assert_no_passage();
_42:
    /* (360 from 322) (360 from RELOAD_FORWARD)  */
    __quex_debug_state(360);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(360);

    __quex_debug_state(360);
if     ( input == 0x6C )  goto _48;
else if( input == 0x0 )   goto _96;
else                      goto _51;


    __quex_assert_no_passage();
_43:
    /* (361 from RELOAD_FORWARD) (361 from 322)  */
    __quex_debug_state(361);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(361);

    __quex_debug_state(361);
switch( input ) {
case 0x0: goto _97;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _23;
case 0x74: goto _44;
default: goto _51;
}


    __quex_assert_no_passage();
_44:
    /* (362 from RELOAD_FORWARD) (362 from 361)  */
    __quex_debug_state(362);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(362);

    __quex_debug_state(362);
if     ( input == 0x69 )  goto _45;
else if( input == 0x0 )   goto _98;
else                      goto _51;


    __quex_assert_no_passage();
_45:
    /* (363 from 362) (363 from RELOAD_FORWARD)  */
    __quex_debug_state(363);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(363);

    __quex_debug_state(363);
if     ( input == 0x6F )  goto _46;
else if( input == 0x0 )   goto _99;
else                      goto _51;


    __quex_assert_no_passage();
_46:
    /* (364 from 363) (364 from RELOAD_FORWARD)  */
    __quex_debug_state(364);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(364);

    __quex_debug_state(364);
if     ( input == 0x6E )  goto _47;
else if( input == 0x0 )   goto _100;
else                      goto _51;


    __quex_assert_no_passage();
_47:
    /* (365 from 364)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(365);

    __quex_debug_state(365);
goto _65;


    __quex_assert_no_passage();
_48:
    /* (366 from RELOAD_FORWARD) (366 from 360)  */
    __quex_debug_state(366);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(366);

    __quex_debug_state(366);
if     ( input == 0x6F )  goto _49;
else if( input == 0x0 )   goto _101;
else                      goto _51;


    __quex_assert_no_passage();
_49:
    /* (367 from 366) (367 from RELOAD_FORWARD)  */
    __quex_debug_state(367);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(367);

    __quex_debug_state(367);
if     ( input == 0x77 )  goto _50;
else if( input == 0x0 )   goto _102;
else                      goto _51;


    __quex_assert_no_passage();
_50:
    /* (368 from 367)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(368);

    __quex_debug_state(368);
goto _66;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_68:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 1400 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_107:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 1416 "Lexer.cpp"

}
goto _1;
_122:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{


#   line 1426 "Lexer.cpp"

}
goto _0;
_120:
    __quex_debug("* TERMINAL \"*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_ASTERISK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1439 "Lexer.cpp"

}
goto _0;
_114:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1452 "Lexer.cpp"

}
goto _0;
_108:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1465 "Lexer.cpp"

}
goto _0;
_116:
    __quex_debug("* TERMINAL \":\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_COLON);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1478 "Lexer.cpp"

}
goto _0;
_121:
    __quex_debug("* TERMINAL [0-9a-fA-F]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR4);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1492 "Lexer.cpp"

}
goto _0;
_109:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1505 "Lexer.cpp"

}
goto _0;
_119:
    __quex_debug("* TERMINAL \"deny\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_DENY);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1518 "Lexer.cpp"

}
goto _0;
_115:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1531 "Lexer.cpp"

}
goto _0;
_112:
    __quex_debug("* TERMINAL \"class\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_CLASS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1544 "Lexer.cpp"

}
goto _0;
_117:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1557 "Lexer.cpp"

}
goto _0;
_110:
    __quex_debug("* TERMINAL \"action\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_ACTION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1570 "Lexer.cpp"

}
goto _0;
_111:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1583 "Lexer.cpp"

}
goto _0;
_113:
    __quex_debug("* TERMINAL \"port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_PORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1596 "Lexer.cpp"

}
goto _0;
_118:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 1610 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_205:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 5: { goto _5; }
        case 7: { goto _7; }
        case 8: { goto _8; }
        case 9: { goto _9; }
        case 10: { goto _10; }
        case 15: { goto _15; }
        case 16: { goto _16; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 19: { goto _19; }
        case 20: { goto _20; }
        case 21: { goto _21; }
        case 23: { goto _23; }
        case 25: { goto _25; }
        case 26: { goto _26; }
        case 28: { goto _28; }
        case 29: { goto _29; }
        case 31: { goto _31; }
        case 32: { goto _32; }
        case 33: { goto _33; }
        case 34: { goto _34; }
        case 36: { goto _36; }
        case 37: { goto _37; }
        case 39: { goto _39; }
        case 40: { goto _40; }
        case 42: { goto _42; }
        case 43: { goto _43; }
        case 44: { goto _44; }
        case 45: { goto _45; }
        case 46: { goto _46; }
        case 48: { goto _48; }
        case 49: { goto _49; }
        case 51: { goto _51; }
        case 53: { goto _53; }
        case 67: { goto _67; }
        case 68: { goto _68; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_90:
    /* (RELOAD_FORWARD from 351)  */
    target_state_index = QUEX_LABEL(33); target_state_else_index = QUEX_LABEL(51);

_203:
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
_72:
    /* (RELOAD_FORWARD from 325)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_95:
    /* (RELOAD_FORWARD from 358)  */
    target_state_index = QUEX_LABEL(40); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_86:
    /* (RELOAD_FORWARD from 346)  */
    target_state_index = QUEX_LABEL(28); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_84:
    /* (RELOAD_FORWARD from 343)  */
    target_state_index = QUEX_LABEL(25); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_75:
    /* (RELOAD_FORWARD from 328)  */
    target_state_index = QUEX_LABEL(10); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_98:
    /* (RELOAD_FORWARD from 362)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_80:
    /* (RELOAD_FORWARD from 337)  */
    target_state_index = QUEX_LABEL(19); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_102:
    /* (RELOAD_FORWARD from 367)  */
    target_state_index = QUEX_LABEL(49); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_89:
    /* (RELOAD_FORWARD from 350)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_71:
    /* (RELOAD_FORWARD from 323)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_94:
    /* (RELOAD_FORWARD from 357)  */
    target_state_index = QUEX_LABEL(39); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_85:
    /* (RELOAD_FORWARD from 344)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_83:
    /* (RELOAD_FORWARD from 341)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_74:
    /* (RELOAD_FORWARD from 327)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_97:
    /* (RELOAD_FORWARD from 361)  */
    target_state_index = QUEX_LABEL(43); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_79:
    /* (RELOAD_FORWARD from 336)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_101:
    /* (RELOAD_FORWARD from 366)  */
    target_state_index = QUEX_LABEL(48); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_70:
    /* (RELOAD_FORWARD from 322)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_93:
    /* (RELOAD_FORWARD from 355)  */
    target_state_index = QUEX_LABEL(37); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_92:
    /* (RELOAD_FORWARD from 354)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_82:
    /* (RELOAD_FORWARD from 339)  */
    target_state_index = QUEX_LABEL(21); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_73:
    /* (RELOAD_FORWARD from 326)  */
    target_state_index = QUEX_LABEL(8); target_state_else_index = QUEX_LABEL(53);

    goto _203;


    __quex_assert_no_passage();
_88:
    /* (RELOAD_FORWARD from 349)  */
    target_state_index = QUEX_LABEL(31); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_78:
    /* (RELOAD_FORWARD from 335)  */
    target_state_index = QUEX_LABEL(17); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_69:
    /* (RELOAD_FORWARD from 321)  */
    target_state_index = QUEX_LABEL(67); target_state_else_index = QUEX_LABEL(68);

    goto _203;


    __quex_assert_no_passage();
_100:
    /* (RELOAD_FORWARD from 364)  */
    target_state_index = QUEX_LABEL(46); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_91:
    /* (RELOAD_FORWARD from 352)  */
    target_state_index = QUEX_LABEL(34); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_81:
    /* (RELOAD_FORWARD from 338)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_96:
    /* (RELOAD_FORWARD from 360)  */
    target_state_index = QUEX_LABEL(42); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_87:
    /* (RELOAD_FORWARD from 347)  */
    target_state_index = QUEX_LABEL(29); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_77:
    /* (RELOAD_FORWARD from 334)  */
    target_state_index = QUEX_LABEL(16); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_76:
    /* (RELOAD_FORWARD from 333)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(51);

    goto _203;


    __quex_assert_no_passage();
_99:
    /* (RELOAD_FORWARD from 363)  */
    target_state_index = QUEX_LABEL(45); target_state_else_index = QUEX_LABEL(51);

    goto _203;

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


goto _204;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _205; /* in QUEX_GOTO_STATE       */
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

    /* (445 from NONE)  */
    __quex_debug("Init State\n");
    __quex_debug_state(445);
_19:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(445);

    __quex_debug("Init State\n");
    __quex_debug_state(445);
switch( input ) {
case 0x0: goto _17;
case 0x1: 
case 0xA: goto _9;
case 0x22: goto _7;
case 0x5C: goto _4;
default: goto _8;
}


    __quex_assert_no_passage();
_15:
    /* (445 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(445);
    goto _19;


    __quex_assert_no_passage();
_11:
    /* (DROP_OUT from 448) (DROP_OUT from 447)  */
    __quex_debug("Drop-Out Catcher\n");

    goto _22;
    __quex_debug("Drop-Out Catcher\n");
_20:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_10:
    /* (DROP_OUT from 446)  */
    goto _23;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 449)  */
    goto _24;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 450)  */
    goto _26;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_9:
    /* (DROP_OUT from 445)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _25;
    __quex_debug("Drop-Out Catcher\n");
    goto _20;


    __quex_assert_no_passage();
_4:
    /* (448 from RELOAD_FORWARD) (448 from 445)  */
    __quex_debug_state(448);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(448);

    __quex_debug_state(448);
switch( input ) {
case 0x0: goto _14;
case 0x22: goto _6;
case 0x5C: goto _5;
default: goto _11;
}


    __quex_assert_no_passage();
_5:
    /* (449 from 448)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(449);

    __quex_debug_state(449);
goto _12;


    __quex_assert_no_passage();
_6:
    /* (450 from 448)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(450);

    __quex_debug_state(450);
goto _13;


    __quex_assert_no_passage();
_7:
    /* (446 from 445)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(446);

    __quex_debug_state(446);
goto _10;


    __quex_assert_no_passage();
_8:
    /* (447 from 445)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(447);

    __quex_debug_state(447);
goto _11;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_16:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 2274 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _1;
_25:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
    self_accumulator_add(Lexeme, LexemeEnd);


#   line 2288 "Lexer.cpp"

}
goto _2;
_26:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2301 "Lexer.cpp"

}
goto _0;
_24:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2314 "Lexer.cpp"

}
goto _0;
_23:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self << RULE;
    RETURN;


#   line 2327 "Lexer.cpp"

}
goto _0;
_22:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 2339 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_35:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 11: { goto _11; }
        case 15: { goto _15; }
        case 16: { goto _16; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_14:
    /* (RELOAD_FORWARD from 448)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(11);

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
    /* (RELOAD_FORWARD from 445)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(16);

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
   static const char  token_id_str_KEYWORD_CLASS[]    = "KEYWORD_CLASS";
   static const char  token_id_str_KEYWORD_DENY[]     = "KEYWORD_DENY";
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
   case RULE_TOKEN_KEYWORD_CLASS:    return token_id_str_KEYWORD_CLASS;
   case RULE_TOKEN_KEYWORD_DENY:     return token_id_str_KEYWORD_DENY;
   case RULE_TOKEN_KEYWORD_HASH:     return token_id_str_KEYWORD_HASH;
   case RULE_TOKEN_KEYWORD_NAME:     return token_id_str_KEYWORD_NAME;
   case RULE_TOKEN_KEYWORD_PORT:     return token_id_str_KEYWORD_PORT;
   case RULE_TOKEN_KEYWORD_REJECT:   return token_id_str_KEYWORD_REJECT;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

