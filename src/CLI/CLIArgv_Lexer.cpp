
#include "CLIArgv_Lexer.hpp"

#include <quex/code_base/analyzer/C-adaptions.h>
/* The file 'multi.i' contains implementations which are the same for all 
 * possibly generated analyzers. If QUEX_OPTION_MULTI is defined, it is
 * NOT supposed to be included here. If not--in which case we have a single
 * analzer--then it is included.                                             */
#include <quex/code_base/single.i>

QUEX_NAMESPACE_MAIN_OPEN
/* Global */QUEX_NAME(Mode)  QUEX_NAME(CLI);
#ifndef __QUEX_INDICATOR_DUMPED_TOKEN_ID_DEFINED
    static QUEX_TYPE_TOKEN_ID    QUEX_NAME_TOKEN(DumpedTokenIdObject);
#endif
#define self  (*(QUEX_TYPE_DERIVED_ANALYZER*)me)
#define __self_result_token_id    QUEX_NAME_TOKEN(DumpedTokenIdObject)

void
QUEX_NAME(CLI_on_entry)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* FromMode) {
    (void)me;
    (void)FromMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(CLI).has_entry_from(FromMode);
#   endif

}

void
QUEX_NAME(CLI_on_exit)(QUEX_TYPE_ANALYZER* me, const QUEX_NAME(Mode)* ToMode)  {
    (void)me;
    (void)ToMode;
#   ifdef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
    QUEX_NAME(CLI).has_exit_to(ToMode);
#   endif

}

#if defined(QUEX_OPTION_INDENTATION_TRIGGER) 
void
QUEX_NAME(CLI_on_indentation)(QUEX_TYPE_ANALYZER*    me, 
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
QUEX_NAME(CLI_has_base)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;
    return false;
}
bool
QUEX_NAME(CLI_has_entry_from)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_CLI): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(CLI)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): CLI, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(CLI_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
    (void)Mode;

    switch( Mode->id ) {
    case QUEX_NAME(ModeID_CLI): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(CLI)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): CLI, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
#endif    
#undef self
#undef __self_result_token_id
QUEX_NAMESPACE_MAIN_CLOSE

/* #include "CLIArgv_Lexer.hpp"*/
QUEX_NAMESPACE_MAIN_OPEN
QUEX_TYPE_CHARACTER  QUEX_LEXEME_NULL_IN_ITS_NAMESPACE = (QUEX_TYPE_CHARACTER)0;
#ifdef      __QUEX_COUNT_VOID
#   undef   __QUEX_COUNT_VOID
#endif
#ifdef      __QUEX_OPTION_COUNTER
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) \
            do {                              \
                QUEX_NAME(CLI_counter)((ME), (BEGIN), (END));     \
                __quex_debug_counter();       \
            } while(0)
#else
#    define __QUEX_COUNT_VOID(ME, BEGIN, END) /* empty */
#endif
#ifdef __QUEX_OPTION_COUNTER
static void
QUEX_NAME(CLI_counter)(QUEX_TYPE_ANALYZER* me, QUEX_TYPE_CHARACTER* LexemeBegin, QUEX_TYPE_CHARACTER* LexemeEnd)
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

    /* (680 from BEFORE_ENTRY)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(680);
if     ( input >= 0xB )  goto _2;
else if( input == 0xA )  goto _3;
else if( input == 0x9 )  goto _4;
else                     goto _2;


    __quex_assert_no_passage();
_8:
    /* (680 from 684)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 682)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 681)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 683)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (681 from 680)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(681);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (682 from 680)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(682);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (683 from 680)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(683);
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
QUEX_NAME(CLI_analyzer_function)(QUEX_TYPE_ANALYZER* me) 
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
#   define QUEX_LABEL_STATE_ROUTER _381
#   define CLI    (QUEX_NAME(CLI))

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
_380:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (562 from BEFORE_ENTRY)  */
_192:
    input = *(me->buffer._input_p);


    __quex_debug("Init State\n");
    __quex_debug_state(562);
switch( input ) {
case 0x0: goto _113;
case 0x1: 
case 0xA: goto _86;
case 0x2D: goto _12;
case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _10;
case 0x61: goto _9;
case 0x64: goto _15;
case 0x66: goto _14;
case 0x68: goto _11;
case 0x6C: goto _16;
case 0x72: goto _18;
case 0x73: goto _17;
case 0x74: goto _13;
default: goto _19;
}


    __quex_assert_no_passage();
_111:
    /* (562 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    goto _192;


    __quex_assert_no_passage();
_87:
    /* (DROP_OUT from 610) (DROP_OUT from 585) (DROP_OUT from 592) (DROP_OUT from 579) (DROP_OUT from 623) (DROP_OUT from 630) (DROP_OUT from 605) (DROP_OUT from 576) (DROP_OUT from 574) (DROP_OUT from 627) (DROP_OUT from 602) (DROP_OUT from 614) (DROP_OUT from 589) (DROP_OUT from 596) (DROP_OUT from 571) (DROP_OUT from 642) (DROP_OUT from 617) (DROP_OUT from 624) (DROP_OUT from 583) (DROP_OUT from 565) (DROP_OUT from 586) (DROP_OUT from 568) (DROP_OUT from 631) (DROP_OUT from 606) (DROP_OUT from 634) (DROP_OUT from 577) (DROP_OUT from 575) (DROP_OUT from 621) (DROP_OUT from 628) (DROP_OUT from 603) (DROP_OUT from 615) (DROP_OUT from 590) (DROP_OUT from 572) (DROP_OUT from 643) (DROP_OUT from 618) (DROP_OUT from 625) (DROP_OUT from 600) (DROP_OUT from 566) (DROP_OUT from 612) (DROP_OUT from 587) (DROP_OUT from 594) (DROP_OUT from 569) (DROP_OUT from 640) (DROP_OUT from 581) (DROP_OUT from 563) (DROP_OUT from 609) (DROP_OUT from 607) (DROP_OUT from 635) (DROP_OUT from 578) (DROP_OUT from 622) (DROP_OUT from 632) (DROP_OUT from 591) (DROP_OUT from 598) (DROP_OUT from 573) (DROP_OUT from 619) (DROP_OUT from 626) (DROP_OUT from 601) (DROP_OUT from 567) (DROP_OUT from 613) (DROP_OUT from 588) (DROP_OUT from 595) (DROP_OUT from 570) (DROP_OUT from 641) (DROP_OUT from 616)  */

    goto _195;
_193:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_101:
    /* (DROP_OUT from 636)  */
    goto _197;


    __quex_assert_no_passage();
_100:
    /* (DROP_OUT from 633)  */
    goto _198;


    __quex_assert_no_passage();
_105:
    /* (DROP_OUT from 644)  */
    goto _199;


    __quex_assert_no_passage();
_99:
    /* (DROP_OUT from 629)  */
    goto _200;


    __quex_assert_no_passage();
_104:
    /* (DROP_OUT from 639)  */
    goto _201;


    __quex_assert_no_passage();
_93:
    /* (DROP_OUT from 597)  */
    goto _202;


    __quex_assert_no_passage();
_98:
    /* (DROP_OUT from 620)  */
    goto _203;


    __quex_assert_no_passage();
_103:
    /* (DROP_OUT from 638)  */
    goto _204;


    __quex_assert_no_passage();
_92:
    /* (DROP_OUT from 593)  */
    goto _205;


    __quex_assert_no_passage();
_97:
    /* (DROP_OUT from 611)  */
    goto _206;


    __quex_assert_no_passage();
_102:
    /* (DROP_OUT from 637)  */
    goto _207;


    __quex_assert_no_passage();
_91:
    /* (DROP_OUT from 584)  */
    goto _208;


    __quex_assert_no_passage();
_96:
    /* (DROP_OUT from 608)  */
    goto _209;


    __quex_assert_no_passage();
_90:
    /* (DROP_OUT from 582)  */
    goto _210;


    __quex_assert_no_passage();
_95:
    /* (DROP_OUT from 604)  */
    goto _211;


    __quex_assert_no_passage();
_89:
    /* (DROP_OUT from 580)  */
    goto _212;


    __quex_assert_no_passage();
_94:
    /* (DROP_OUT from 599)  */
    goto _213;


    __quex_assert_no_passage();
_88:
    /* (DROP_OUT from 564)  */
    goto _214;


    __quex_assert_no_passage();
_86:
    /* (DROP_OUT from 562)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _196;
    goto _193;


    __quex_assert_no_passage();
_9:
    /* (563 from 562) (563 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(563);
switch( input ) {
case 0x0: goto _114;
case 0x1: 
case 0xA: goto _87;
case 0x70: goto _4;
default: goto _19;
}


    __quex_assert_no_passage();
_10:
    /* (564 from 564) (564 from 562) (564 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(564);
switch( input ) {
case 0x0: goto _115;
case 0x1: 
case 0xA: goto _88;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _10;
case 0x64: goto _85;
case 0x68: goto _84;
case 0x6D: goto _83;
default: goto _19;
}


    __quex_assert_no_passage();
_11:
    /* (565 from RELOAD_FORWARD) (565 from 562)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(565);
switch( input ) {
case 0x0: goto _116;
case 0x1: 
case 0xA: goto _87;
case 0x61: goto _80;
default: goto _19;
}


    __quex_assert_no_passage();
_12:
    /* (566 from RELOAD_FORWARD) (566 from 562)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(566);
switch( input ) {
case 0x0: goto _117;
case 0x1: 
case 0xA: goto _87;
case 0x2D: goto _58;
default: goto _19;
}


    __quex_assert_no_passage();
_13:
    /* (567 from 562) (567 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(567);
switch( input ) {
case 0x0: goto _118;
case 0x1: 
case 0xA: goto _87;
case 0x72: goto _55;
default: goto _19;
}


    __quex_assert_no_passage();
_14:
    /* (568 from 562) (568 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(568);
switch( input ) {
case 0x0: goto _119;
case 0x1: 
case 0xA: goto _87;
case 0x61: goto _51;
default: goto _19;
}


    __quex_assert_no_passage();
_15:
    /* (569 from RELOAD_FORWARD) (569 from 562)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(569);
switch( input ) {
case 0x0: goto _120;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _46;
default: goto _19;
}


    __quex_assert_no_passage();
_16:
    /* (570 from RELOAD_FORWARD) (570 from 562)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(570);
switch( input ) {
case 0x0: goto _121;
case 0x1: 
case 0xA: goto _87;
case 0x61: goto _40;
case 0x69: goto _41;
default: goto _19;
}


    __quex_assert_no_passage();
_17:
    /* (571 from 562) (571 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(571);
switch( input ) {
case 0x0: goto _122;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _31;
default: goto _19;
}


    __quex_assert_no_passage();
_18:
    /* (572 from 562) (572 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(572);
switch( input ) {
case 0x0: goto _123;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _22;
case 0x6F: goto _21;
case 0x75: goto _20;
default: goto _19;
}


    __quex_assert_no_passage();
_19:
    /* (573 from 636) (573 from 567) (573 from 576) (573 from 617) (573 from 626) (573 from 589) (573 from 598) (573 from 639) (573 from 570) (573 from 579) (573 from 620) (573 from 629) (573 from 601) (573 from 610) (573 from 573) (573 from 582) (573 from 623) (573 from 632) (573 from 641) (573 from 563) (573 from 604) (573 from 613) (573 from 585) (573 from 594) (573 from 635) (573 from 644) (573 from 566) (573 from 607) (573 from 616) (573 from 625) (573 from RELOAD_FORWARD) (573 from 588) (573 from 597) (573 from 638) (573 from 569) (573 from 578) (573 from 619) (573 from 628) (573 from 591) (573 from 600) (573 from 609) (573 from 572) (573 from 581) (573 from 622) (573 from 631) (573 from 640) (573 from 562) (573 from 603) (573 from 612) (573 from 575) (573 from 584) (573 from 593) (573 from 634) (573 from 643) (573 from 565) (573 from 606) (573 from 615) (573 from 624) (573 from 587) (573 from 596) (573 from 637) (573 from 568) (573 from 577) (573 from 618) (573 from 627) (573 from 590) (573 from 599) (573 from 608) (573 from 571) (573 from 580) (573 from 621) (573 from 630) (573 from 602) (573 from 611) (573 from 574) (573 from 583) (573 from 592) (573 from 633) (573 from 642) (573 from 564) (573 from 605) (573 from 614) (573 from 586) (573 from 595)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(573);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _87;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _87;
else                     goto _124;


    __quex_assert_no_passage();
_20:
    /* (574 from RELOAD_FORWARD) (574 from 572)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(574);
switch( input ) {
case 0x0: goto _125;
case 0x1: 
case 0xA: goto _87;
case 0x6C: goto _29;
default: goto _19;
}


    __quex_assert_no_passage();
_21:
    /* (575 from 572) (575 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(575);
switch( input ) {
case 0x0: goto _126;
case 0x1: 
case 0xA: goto _87;
case 0x6F: goto _27;
default: goto _19;
}


    __quex_assert_no_passage();
_22:
    /* (576 from RELOAD_FORWARD) (576 from 572)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(576);
switch( input ) {
case 0x0: goto _127;
case 0x1: 
case 0xA: goto _87;
case 0x6D: goto _23;
default: goto _19;
}


    __quex_assert_no_passage();
_23:
    /* (577 from 576) (577 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(577);
switch( input ) {
case 0x0: goto _128;
case 0x1: 
case 0xA: goto _87;
case 0x6F: goto _24;
default: goto _19;
}


    __quex_assert_no_passage();
_24:
    /* (578 from RELOAD_FORWARD) (578 from 577)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(578);
switch( input ) {
case 0x0: goto _129;
case 0x1: 
case 0xA: goto _87;
case 0x76: goto _25;
default: goto _19;
}


    __quex_assert_no_passage();
_25:
    /* (579 from 578) (579 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(579);
switch( input ) {
case 0x0: goto _130;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _26;
default: goto _19;
}


    __quex_assert_no_passage();
_26:
    /* (580 from 579) (580 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(580);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _89;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _89;
else                     goto _131;


    __quex_assert_no_passage();
_27:
    /* (581 from 575) (581 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(581);
switch( input ) {
case 0x0: goto _132;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _28;
default: goto _19;
}


    __quex_assert_no_passage();
_28:
    /* (582 from RELOAD_FORWARD) (582 from 581)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(582);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _90;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _90;
else                     goto _133;


    __quex_assert_no_passage();
_29:
    /* (583 from 574) (583 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(583);
switch( input ) {
case 0x0: goto _134;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _30;
default: goto _19;
}


    __quex_assert_no_passage();
_30:
    /* (584 from RELOAD_FORWARD) (584 from 583)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(584);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _91;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _91;
else                     goto _135;


    __quex_assert_no_passage();
_31:
    /* (585 from RELOAD_FORWARD) (585 from 571)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(585);
switch( input ) {
case 0x0: goto _136;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _32;
default: goto _19;
}


    __quex_assert_no_passage();
_32:
    /* (586 from RELOAD_FORWARD) (586 from 585)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(586);
switch( input ) {
case 0x0: goto _137;
case 0x1: 
case 0xA: goto _87;
case 0x2D: goto _33;
default: goto _19;
}


    __quex_assert_no_passage();
_33:
    /* (587 from 586) (587 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(587);
switch( input ) {
case 0x0: goto _138;
case 0x1: 
case 0xA: goto _87;
case 0x70: goto _34;
default: goto _19;
}


    __quex_assert_no_passage();
_34:
    /* (588 from 587) (588 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(588);
switch( input ) {
case 0x0: goto _139;
case 0x1: 
case 0xA: goto _87;
case 0x6F: goto _35;
default: goto _19;
}


    __quex_assert_no_passage();
_35:
    /* (589 from 588) (589 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(589);
switch( input ) {
case 0x0: goto _140;
case 0x1: 
case 0xA: goto _87;
case 0x6C: goto _36;
default: goto _19;
}


    __quex_assert_no_passage();
_36:
    /* (590 from RELOAD_FORWARD) (590 from 589)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(590);
switch( input ) {
case 0x0: goto _141;
case 0x1: 
case 0xA: goto _87;
case 0x69: goto _37;
default: goto _19;
}


    __quex_assert_no_passage();
_37:
    /* (591 from 590) (591 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(591);
switch( input ) {
case 0x0: goto _142;
case 0x1: 
case 0xA: goto _87;
case 0x63: goto _38;
default: goto _19;
}


    __quex_assert_no_passage();
_38:
    /* (592 from RELOAD_FORWARD) (592 from 591)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(592);
switch( input ) {
case 0x0: goto _143;
case 0x1: 
case 0xA: goto _87;
case 0x79: goto _39;
default: goto _19;
}


    __quex_assert_no_passage();
_39:
    /* (593 from 592) (593 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(593);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _92;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _92;
else                     goto _144;


    __quex_assert_no_passage();
_40:
    /* (594 from RELOAD_FORWARD) (594 from 570)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(594);
switch( input ) {
case 0x0: goto _145;
case 0x1: 
case 0xA: goto _87;
case 0x73: goto _44;
default: goto _19;
}


    __quex_assert_no_passage();
_41:
    /* (595 from 570) (595 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(595);
switch( input ) {
case 0x0: goto _146;
case 0x1: 
case 0xA: goto _87;
case 0x73: goto _42;
default: goto _19;
}


    __quex_assert_no_passage();
_42:
    /* (596 from 595) (596 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(596);
switch( input ) {
case 0x0: goto _147;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _43;
default: goto _19;
}


    __quex_assert_no_passage();
_43:
    /* (597 from 596) (597 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(597);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _93;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _93;
else                     goto _148;


    __quex_assert_no_passage();
_44:
    /* (598 from RELOAD_FORWARD) (598 from 594)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(598);
switch( input ) {
case 0x0: goto _149;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _45;
default: goto _19;
}


    __quex_assert_no_passage();
_45:
    /* (599 from 598) (599 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(599);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _94;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _94;
else                     goto _150;


    __quex_assert_no_passage();
_46:
    /* (600 from 569) (600 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(600);
switch( input ) {
case 0x0: goto _151;
case 0x1: 
case 0xA: goto _87;
case 0x76: goto _47;
default: goto _19;
}


    __quex_assert_no_passage();
_47:
    /* (601 from 600) (601 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(601);
switch( input ) {
case 0x0: goto _152;
case 0x1: 
case 0xA: goto _87;
case 0x69: goto _48;
default: goto _19;
}


    __quex_assert_no_passage();
_48:
    /* (602 from RELOAD_FORWARD) (602 from 601)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(602);
switch( input ) {
case 0x0: goto _153;
case 0x1: 
case 0xA: goto _87;
case 0x63: goto _49;
default: goto _19;
}


    __quex_assert_no_passage();
_49:
    /* (603 from 602) (603 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(603);
switch( input ) {
case 0x0: goto _154;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _50;
default: goto _19;
}


    __quex_assert_no_passage();
_50:
    /* (604 from 603) (604 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(604);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _95;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _95;
else                     goto _155;


    __quex_assert_no_passage();
_51:
    /* (605 from 568) (605 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(605);
switch( input ) {
case 0x0: goto _156;
case 0x1: 
case 0xA: goto _87;
case 0x6C: goto _52;
default: goto _19;
}


    __quex_assert_no_passage();
_52:
    /* (606 from RELOAD_FORWARD) (606 from 605)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(606);
switch( input ) {
case 0x0: goto _157;
case 0x1: 
case 0xA: goto _87;
case 0x73: goto _53;
default: goto _19;
}


    __quex_assert_no_passage();
_53:
    /* (607 from 606) (607 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(607);
switch( input ) {
case 0x0: goto _158;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _54;
default: goto _19;
}


    __quex_assert_no_passage();
_54:
    /* (608 from RELOAD_FORWARD) (608 from 607)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(608);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _96;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _96;
else                     goto _159;


    __quex_assert_no_passage();
_55:
    /* (609 from RELOAD_FORWARD) (609 from 567)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(609);
switch( input ) {
case 0x0: goto _160;
case 0x1: 
case 0xA: goto _87;
case 0x75: goto _56;
default: goto _19;
}


    __quex_assert_no_passage();
_56:
    /* (610 from RELOAD_FORWARD) (610 from 609)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(610);
switch( input ) {
case 0x0: goto _161;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _57;
default: goto _19;
}


    __quex_assert_no_passage();
_57:
    /* (611 from 610) (611 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(611);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _97;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _97;
else                     goto _162;


    __quex_assert_no_passage();
_58:
    /* (612 from 566) (612 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(612);
switch( input ) {
case 0x0: goto _163;
case 0x1: 
case 0xA: goto _87;
case 0x70: goto _59;
case 0x74: goto _60;
default: goto _19;
}


    __quex_assert_no_passage();
_59:
    /* (613 from 612) (613 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(613);
switch( input ) {
case 0x0: goto _164;
case 0x1: 
case 0xA: goto _87;
case 0x61: goto _67;
case 0x65: goto _68;
default: goto _19;
}


    __quex_assert_no_passage();
_60:
    /* (614 from RELOAD_FORWARD) (614 from 612)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(614);
switch( input ) {
case 0x0: goto _165;
case 0x1: 
case 0xA: goto _87;
case 0x69: goto _61;
default: goto _19;
}


    __quex_assert_no_passage();
_61:
    /* (615 from 614) (615 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(615);
switch( input ) {
case 0x0: goto _166;
case 0x1: 
case 0xA: goto _87;
case 0x6D: goto _62;
default: goto _19;
}


    __quex_assert_no_passage();
_62:
    /* (616 from RELOAD_FORWARD) (616 from 615)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(616);
switch( input ) {
case 0x0: goto _167;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _63;
default: goto _19;
}


    __quex_assert_no_passage();
_63:
    /* (617 from 616) (617 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(617);
switch( input ) {
case 0x0: goto _168;
case 0x1: 
case 0xA: goto _87;
case 0x6F: goto _64;
default: goto _19;
}


    __quex_assert_no_passage();
_64:
    /* (618 from RELOAD_FORWARD) (618 from 617)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(618);
switch( input ) {
case 0x0: goto _169;
case 0x1: 
case 0xA: goto _87;
case 0x75: goto _65;
default: goto _19;
}


    __quex_assert_no_passage();
_65:
    /* (619 from 618) (619 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(619);
switch( input ) {
case 0x0: goto _170;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _66;
default: goto _19;
}


    __quex_assert_no_passage();
_66:
    /* (620 from 619) (620 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(620);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _98;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _98;
else                     goto _171;


    __quex_assert_no_passage();
_67:
    /* (621 from 613) (621 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(621);
switch( input ) {
case 0x0: goto _172;
case 0x1: 
case 0xA: goto _87;
case 0x72: goto _76;
default: goto _19;
}


    __quex_assert_no_passage();
_68:
    /* (622 from RELOAD_FORWARD) (622 from 613)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(622);
switch( input ) {
case 0x0: goto _173;
case 0x1: 
case 0xA: goto _87;
case 0x72: goto _69;
default: goto _19;
}


    __quex_assert_no_passage();
_69:
    /* (623 from 622) (623 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(623);
switch( input ) {
case 0x0: goto _174;
case 0x1: 
case 0xA: goto _87;
case 0x6D: goto _70;
default: goto _19;
}


    __quex_assert_no_passage();
_70:
    /* (624 from RELOAD_FORWARD) (624 from 623)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(624);
switch( input ) {
case 0x0: goto _175;
case 0x1: 
case 0xA: goto _87;
case 0x61: goto _71;
default: goto _19;
}


    __quex_assert_no_passage();
_71:
    /* (625 from 624) (625 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(625);
switch( input ) {
case 0x0: goto _176;
case 0x1: 
case 0xA: goto _87;
case 0x6E: goto _72;
default: goto _19;
}


    __quex_assert_no_passage();
_72:
    /* (626 from RELOAD_FORWARD) (626 from 625)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(626);
switch( input ) {
case 0x0: goto _177;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _73;
default: goto _19;
}


    __quex_assert_no_passage();
_73:
    /* (627 from 626) (627 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(627);
switch( input ) {
case 0x0: goto _178;
case 0x1: 
case 0xA: goto _87;
case 0x6E: goto _74;
default: goto _19;
}


    __quex_assert_no_passage();
_74:
    /* (628 from 627) (628 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(628);
switch( input ) {
case 0x0: goto _179;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _75;
default: goto _19;
}


    __quex_assert_no_passage();
_75:
    /* (629 from 628) (629 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(629);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _99;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _99;
else                     goto _180;


    __quex_assert_no_passage();
_76:
    /* (630 from RELOAD_FORWARD) (630 from 621)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(630);
switch( input ) {
case 0x0: goto _181;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _77;
default: goto _19;
}


    __quex_assert_no_passage();
_77:
    /* (631 from 630) (631 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(631);
switch( input ) {
case 0x0: goto _182;
case 0x1: 
case 0xA: goto _87;
case 0x6E: goto _78;
default: goto _19;
}


    __quex_assert_no_passage();
_78:
    /* (632 from RELOAD_FORWARD) (632 from 631)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(632);
switch( input ) {
case 0x0: goto _183;
case 0x1: 
case 0xA: goto _87;
case 0x74: goto _79;
default: goto _19;
}


    __quex_assert_no_passage();
_79:
    /* (633 from 632) (633 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(633);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _100;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _100;
else                     goto _184;


    __quex_assert_no_passage();
_80:
    /* (634 from 565) (634 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(634);
switch( input ) {
case 0x0: goto _185;
case 0x1: 
case 0xA: goto _87;
case 0x73: goto _81;
default: goto _19;
}


    __quex_assert_no_passage();
_81:
    /* (635 from 634) (635 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(635);
switch( input ) {
case 0x0: goto _186;
case 0x1: 
case 0xA: goto _87;
case 0x68: goto _82;
default: goto _19;
}


    __quex_assert_no_passage();
_82:
    /* (636 from 635) (636 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(636);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _101;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _101;
else                     goto _187;


    __quex_assert_no_passage();
_83:
    /* (637 from 564) (637 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(637);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _102;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _102;
else                     goto _188;


    __quex_assert_no_passage();
_84:
    /* (638 from RELOAD_FORWARD) (638 from 564)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(638);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _103;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _103;
else                     goto _189;


    __quex_assert_no_passage();
_85:
    /* (639 from 564) (639 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(639);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _104;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _104;
else                     goto _190;


    __quex_assert_no_passage();
_4:
    /* (640 from 563) (640 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(640);
switch( input ) {
case 0x0: goto _106;
case 0x1: 
case 0xA: goto _87;
case 0x70: goto _5;
default: goto _19;
}


    __quex_assert_no_passage();
_5:
    /* (641 from 640) (641 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(641);
switch( input ) {
case 0x0: goto _107;
case 0x1: 
case 0xA: goto _87;
case 0x65: goto _6;
default: goto _19;
}


    __quex_assert_no_passage();
_6:
    /* (642 from RELOAD_FORWARD) (642 from 641)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(642);
switch( input ) {
case 0x0: goto _108;
case 0x1: 
case 0xA: goto _87;
case 0x6E: goto _7;
default: goto _19;
}


    __quex_assert_no_passage();
_7:
    /* (643 from 642) (643 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(643);
switch( input ) {
case 0x0: goto _109;
case 0x1: 
case 0xA: goto _87;
case 0x64: goto _8;
default: goto _19;
}


    __quex_assert_no_passage();
_8:
    /* (644 from 643) (644 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(644);
if     ( input >= 0xB )  goto _19;
else if( input == 0xA )  goto _105;
else if( input >= 0x2 )  goto _19;
else if( input == 0x1 )  goto _105;
else                     goto _110;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_112:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(CLI_TOKEN_TERMINATION);


#   line 2050 "CLIArgv_Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_196:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'CLI'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 2066 "CLIArgv_Lexer.cpp"

}
goto _1;
_208:
    __quex_debug("* TERMINAL \"rule\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_RULE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2079 "CLIArgv_Lexer.cpp"

}
goto _0;
_211:
    __quex_debug("* TERMINAL \"device\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_DEVICE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2092 "CLIArgv_Lexer.cpp"

}
goto _0;
_199:
    __quex_debug("* TERMINAL \"append\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_APPEND);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2105 "CLIArgv_Lexer.cpp"

}
goto _0;
_212:
    __quex_debug("* TERMINAL \"remove\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_REMOVE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2118 "CLIArgv_Lexer.cpp"

}
goto _0;
_202:
    __quex_debug("* TERMINAL \"list\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_LIST);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2131 "CLIArgv_Lexer.cpp"

}
goto _0;
_197:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2144 "CLIArgv_Lexer.cpp"

}
goto _0;
_205:
    __quex_debug("* TERMINAL \"set-policy\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(10);
{
self_send(CLI_TOKEN_KEYWORD_SET_POLICY);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2157 "CLIArgv_Lexer.cpp"

}
goto _0;
_213:
    __quex_debug("* TERMINAL \"last\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_LAST);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2170 "CLIArgv_Lexer.cpp"

}
goto _0;
_210:
    __quex_debug("* TERMINAL \"root\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_ROOT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2183 "CLIArgv_Lexer.cpp"

}
goto _0;
_206:
    __quex_debug("* TERMINAL \"true\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_TRUE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2196 "CLIArgv_Lexer.cpp"

}
goto _0;
_209:
    __quex_debug("* TERMINAL \"false\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(CLI_TOKEN_KEYWORD_FALSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2209 "CLIArgv_Lexer.cpp"

}
goto _0;
_198:
    __quex_debug("* TERMINAL \"--parent\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{
self_send(CLI_TOKEN_OPTION_PARENT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2222 "CLIArgv_Lexer.cpp"

}
goto _0;
_203:
    __quex_debug("* TERMINAL \"--timeout\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{
self_send(CLI_TOKEN_OPTION_TIMEOUT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2235 "CLIArgv_Lexer.cpp"

}
goto _0;
_200:
    __quex_debug("* TERMINAL \"--permanent\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(11);
{
self_send(CLI_TOKEN_OPTION_PERMANENT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2248 "CLIArgv_Lexer.cpp"

}
goto _0;
_214:
    __quex_debug("* TERMINAL [1-9][0-9]*\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd));
self_send(CLI_TOKEN_NUMBER_UNSIGNED_INTEGER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2263 "CLIArgv_Lexer.cpp"

}
goto _0;
_204:
    __quex_debug("* TERMINAL [1-9][0-9]*\"h\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_HOURS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2278 "CLIArgv_Lexer.cpp"

}
goto _0;
_207:
    __quex_debug("* TERMINAL [1-9][0-9]*\"m\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_MINUTES);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2293 "CLIArgv_Lexer.cpp"

}
goto _0;
_201:
    __quex_debug("* TERMINAL [1-9][0-9]*\"d\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_DAYS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2308 "CLIArgv_Lexer.cpp"

}
goto _0;
_195:
    __quex_debug("* TERMINAL .+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd));
self_send(CLI_TOKEN_STRING);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2322 "CLIArgv_Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_381:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 5: { goto _5; }
        case 6: { goto _6; }
        case 7: { goto _7; }
        case 8: { goto _8; }
        case 9: { goto _9; }
        case 10: { goto _10; }
        case 11: { goto _11; }
        case 12: { goto _12; }
        case 13: { goto _13; }
        case 14: { goto _14; }
        case 15: { goto _15; }
        case 16: { goto _16; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 19: { goto _19; }
        case 20: { goto _20; }
        case 21: { goto _21; }
        case 22: { goto _22; }
        case 23: { goto _23; }
        case 24: { goto _24; }
        case 25: { goto _25; }
        case 26: { goto _26; }
        case 27: { goto _27; }
        case 28: { goto _28; }
        case 29: { goto _29; }
        case 30: { goto _30; }
        case 31: { goto _31; }
        case 32: { goto _32; }
        case 33: { goto _33; }
        case 34: { goto _34; }
        case 35: { goto _35; }
        case 36: { goto _36; }
        case 37: { goto _37; }
        case 38: { goto _38; }
        case 39: { goto _39; }
        case 40: { goto _40; }
        case 41: { goto _41; }
        case 42: { goto _42; }
        case 43: { goto _43; }
        case 44: { goto _44; }
        case 45: { goto _45; }
        case 46: { goto _46; }
        case 47: { goto _47; }
        case 48: { goto _48; }
        case 49: { goto _49; }
        case 50: { goto _50; }
        case 51: { goto _51; }
        case 52: { goto _52; }
        case 53: { goto _53; }
        case 54: { goto _54; }
        case 55: { goto _55; }
        case 56: { goto _56; }
        case 57: { goto _57; }
        case 58: { goto _58; }
        case 59: { goto _59; }
        case 60: { goto _60; }
        case 61: { goto _61; }
        case 62: { goto _62; }
        case 63: { goto _63; }
        case 64: { goto _64; }
        case 65: { goto _65; }
        case 66: { goto _66; }
        case 67: { goto _67; }
        case 68: { goto _68; }
        case 69: { goto _69; }
        case 70: { goto _70; }
        case 71: { goto _71; }
        case 72: { goto _72; }
        case 73: { goto _73; }
        case 74: { goto _74; }
        case 75: { goto _75; }
        case 76: { goto _76; }
        case 77: { goto _77; }
        case 78: { goto _78; }
        case 79: { goto _79; }
        case 80: { goto _80; }
        case 81: { goto _81; }
        case 82: { goto _82; }
        case 83: { goto _83; }
        case 84: { goto _84; }
        case 85: { goto _85; }
        case 87: { goto _87; }
        case 88: { goto _88; }
        case 89: { goto _89; }
        case 90: { goto _90; }
        case 91: { goto _91; }
        case 92: { goto _92; }
        case 93: { goto _93; }
        case 94: { goto _94; }
        case 95: { goto _95; }
        case 96: { goto _96; }
        case 97: { goto _97; }
        case 98: { goto _98; }
        case 99: { goto _99; }
        case 100: { goto _100; }
        case 101: { goto _101; }
        case 102: { goto _102; }
        case 103: { goto _103; }
        case 104: { goto _104; }
        case 105: { goto _105; }
        case 111: { goto _111; }
        case 112: { goto _112; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_142:
    /* (RELOAD_FORWARD from 591)  */
    target_state_index = QUEX_LABEL(37); target_state_else_index = QUEX_LABEL(87);

_379:
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
_129:
    /* (RELOAD_FORWARD from 578)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_109:
    /* (RELOAD_FORWARD from 643)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_165:
    /* (RELOAD_FORWARD from 614)  */
    target_state_index = QUEX_LABEL(60); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_134:
    /* (RELOAD_FORWARD from 583)  */
    target_state_index = QUEX_LABEL(29); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_185:
    /* (RELOAD_FORWARD from 634)  */
    target_state_index = QUEX_LABEL(80); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_156:
    /* (RELOAD_FORWARD from 605)  */
    target_state_index = QUEX_LABEL(51); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_154:
    /* (RELOAD_FORWARD from 603)  */
    target_state_index = QUEX_LABEL(49); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_125:
    /* (RELOAD_FORWARD from 574)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_176:
    /* (RELOAD_FORWARD from 625)  */
    target_state_index = QUEX_LABEL(71); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_123:
    /* (RELOAD_FORWARD from 572)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_145:
    /* (RELOAD_FORWARD from 594)  */
    target_state_index = QUEX_LABEL(40); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_114:
    /* (RELOAD_FORWARD from 563)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_181:
    /* (RELOAD_FORWARD from 630)  */
    target_state_index = QUEX_LABEL(76); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_168:
    /* (RELOAD_FORWARD from 617)  */
    target_state_index = QUEX_LABEL(63); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_150:
    /* (RELOAD_FORWARD from 599)  */
    target_state_index = QUEX_LABEL(45); target_state_else_index = QUEX_LABEL(94);

    goto _379;


    __quex_assert_no_passage();
_137:
    /* (RELOAD_FORWARD from 586)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_172:
    /* (RELOAD_FORWARD from 621)  */
    target_state_index = QUEX_LABEL(67); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_119:
    /* (RELOAD_FORWARD from 568)  */
    target_state_index = QUEX_LABEL(14); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_141:
    /* (RELOAD_FORWARD from 590)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_128:
    /* (RELOAD_FORWARD from 577)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_108:
    /* (RELOAD_FORWARD from 642)  */
    target_state_index = QUEX_LABEL(6); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_164:
    /* (RELOAD_FORWARD from 613)  */
    target_state_index = QUEX_LABEL(59); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_133:
    /* (RELOAD_FORWARD from 582)  */
    target_state_index = QUEX_LABEL(28); target_state_else_index = QUEX_LABEL(90);

    goto _379;


    __quex_assert_no_passage();
_184:
    /* (RELOAD_FORWARD from 633)  */
    target_state_index = QUEX_LABEL(79); target_state_else_index = QUEX_LABEL(100);

    goto _379;


    __quex_assert_no_passage();
_163:
    /* (RELOAD_FORWARD from 612)  */
    target_state_index = QUEX_LABEL(58); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_153:
    /* (RELOAD_FORWARD from 602)  */
    target_state_index = QUEX_LABEL(48); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_124:
    /* (RELOAD_FORWARD from 573)  */
    target_state_index = QUEX_LABEL(19); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_122:
    /* (RELOAD_FORWARD from 571)  */
    target_state_index = QUEX_LABEL(17); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_144:
    /* (RELOAD_FORWARD from 593)  */
    target_state_index = QUEX_LABEL(39); target_state_else_index = QUEX_LABEL(92);

    goto _379;


    __quex_assert_no_passage();
_190:
    /* (RELOAD_FORWARD from 639)  */
    target_state_index = QUEX_LABEL(85); target_state_else_index = QUEX_LABEL(104);

    goto _379;


    __quex_assert_no_passage();
_113:
    /* (RELOAD_FORWARD from 562)  */
    target_state_index = QUEX_LABEL(111); target_state_else_index = QUEX_LABEL(112);

    goto _379;


    __quex_assert_no_passage();
_180:
    /* (RELOAD_FORWARD from 629)  */
    target_state_index = QUEX_LABEL(75); target_state_else_index = QUEX_LABEL(99);

    goto _379;


    __quex_assert_no_passage();
_159:
    /* (RELOAD_FORWARD from 608)  */
    target_state_index = QUEX_LABEL(54); target_state_else_index = QUEX_LABEL(96);

    goto _379;


    __quex_assert_no_passage();
_149:
    /* (RELOAD_FORWARD from 598)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_136:
    /* (RELOAD_FORWARD from 585)  */
    target_state_index = QUEX_LABEL(31); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_179:
    /* (RELOAD_FORWARD from 628)  */
    target_state_index = QUEX_LABEL(74); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_118:
    /* (RELOAD_FORWARD from 567)  */
    target_state_index = QUEX_LABEL(13); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_140:
    /* (RELOAD_FORWARD from 589)  */
    target_state_index = QUEX_LABEL(35); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_107:
    /* (RELOAD_FORWARD from 641)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_171:
    /* (RELOAD_FORWARD from 620)  */
    target_state_index = QUEX_LABEL(66); target_state_else_index = QUEX_LABEL(98);

    goto _379;


    __quex_assert_no_passage();
_132:
    /* (RELOAD_FORWARD from 581)  */
    target_state_index = QUEX_LABEL(27); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_175:
    /* (RELOAD_FORWARD from 624)  */
    target_state_index = QUEX_LABEL(70); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_162:
    /* (RELOAD_FORWARD from 611)  */
    target_state_index = QUEX_LABEL(57); target_state_else_index = QUEX_LABEL(97);

    goto _379;


    __quex_assert_no_passage();
_152:
    /* (RELOAD_FORWARD from 601)  */
    target_state_index = QUEX_LABEL(47); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_131:
    /* (RELOAD_FORWARD from 580)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(89);

    goto _379;


    __quex_assert_no_passage();
_121:
    /* (RELOAD_FORWARD from 570)  */
    target_state_index = QUEX_LABEL(16); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_167:
    /* (RELOAD_FORWARD from 616)  */
    target_state_index = QUEX_LABEL(62); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_189:
    /* (RELOAD_FORWARD from 638)  */
    target_state_index = QUEX_LABEL(84); target_state_else_index = QUEX_LABEL(103);

    goto _379;


    __quex_assert_no_passage();
_110:
    /* (RELOAD_FORWARD from 644)  */
    target_state_index = QUEX_LABEL(8); target_state_else_index = QUEX_LABEL(105);

    goto _379;


    __quex_assert_no_passage();
_187:
    /* (RELOAD_FORWARD from 636)  */
    target_state_index = QUEX_LABEL(82); target_state_else_index = QUEX_LABEL(101);

    goto _379;


    __quex_assert_no_passage();
_158:
    /* (RELOAD_FORWARD from 607)  */
    target_state_index = QUEX_LABEL(53); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_148:
    /* (RELOAD_FORWARD from 597)  */
    target_state_index = QUEX_LABEL(43); target_state_else_index = QUEX_LABEL(93);

    goto _379;


    __quex_assert_no_passage();
_127:
    /* (RELOAD_FORWARD from 576)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_178:
    /* (RELOAD_FORWARD from 627)  */
    target_state_index = QUEX_LABEL(73); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_117:
    /* (RELOAD_FORWARD from 566)  */
    target_state_index = QUEX_LABEL(12); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_147:
    /* (RELOAD_FORWARD from 596)  */
    target_state_index = QUEX_LABEL(42); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_106:
    /* (RELOAD_FORWARD from 640)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_183:
    /* (RELOAD_FORWARD from 632)  */
    target_state_index = QUEX_LABEL(78); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_170:
    /* (RELOAD_FORWARD from 619)  */
    target_state_index = QUEX_LABEL(65); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_139:
    /* (RELOAD_FORWARD from 588)  */
    target_state_index = QUEX_LABEL(34); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_174:
    /* (RELOAD_FORWARD from 623)  */
    target_state_index = QUEX_LABEL(69); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_161:
    /* (RELOAD_FORWARD from 610)  */
    target_state_index = QUEX_LABEL(56); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_143:
    /* (RELOAD_FORWARD from 592)  */
    target_state_index = QUEX_LABEL(38); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_130:
    /* (RELOAD_FORWARD from 579)  */
    target_state_index = QUEX_LABEL(25); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_120:
    /* (RELOAD_FORWARD from 569)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_166:
    /* (RELOAD_FORWARD from 615)  */
    target_state_index = QUEX_LABEL(61); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_188:
    /* (RELOAD_FORWARD from 637)  */
    target_state_index = QUEX_LABEL(83); target_state_else_index = QUEX_LABEL(102);

    goto _379;


    __quex_assert_no_passage();
_135:
    /* (RELOAD_FORWARD from 584)  */
    target_state_index = QUEX_LABEL(30); target_state_else_index = QUEX_LABEL(91);

    goto _379;


    __quex_assert_no_passage();
_186:
    /* (RELOAD_FORWARD from 635)  */
    target_state_index = QUEX_LABEL(81); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_157:
    /* (RELOAD_FORWARD from 606)  */
    target_state_index = QUEX_LABEL(52); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_155:
    /* (RELOAD_FORWARD from 604)  */
    target_state_index = QUEX_LABEL(50); target_state_else_index = QUEX_LABEL(95);

    goto _379;


    __quex_assert_no_passage();
_126:
    /* (RELOAD_FORWARD from 575)  */
    target_state_index = QUEX_LABEL(21); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_177:
    /* (RELOAD_FORWARD from 626)  */
    target_state_index = QUEX_LABEL(72); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_116:
    /* (RELOAD_FORWARD from 565)  */
    target_state_index = QUEX_LABEL(11); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_146:
    /* (RELOAD_FORWARD from 595)  */
    target_state_index = QUEX_LABEL(41); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_115:
    /* (RELOAD_FORWARD from 564)  */
    target_state_index = QUEX_LABEL(10); target_state_else_index = QUEX_LABEL(88);

    goto _379;


    __quex_assert_no_passage();
_182:
    /* (RELOAD_FORWARD from 631)  */
    target_state_index = QUEX_LABEL(77); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_169:
    /* (RELOAD_FORWARD from 618)  */
    target_state_index = QUEX_LABEL(64); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_151:
    /* (RELOAD_FORWARD from 600)  */
    target_state_index = QUEX_LABEL(46); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_138:
    /* (RELOAD_FORWARD from 587)  */
    target_state_index = QUEX_LABEL(33); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_173:
    /* (RELOAD_FORWARD from 622)  */
    target_state_index = QUEX_LABEL(68); target_state_else_index = QUEX_LABEL(87);

    goto _379;


    __quex_assert_no_passage();
_160:
    /* (RELOAD_FORWARD from 609)  */
    target_state_index = QUEX_LABEL(55); target_state_else_index = QUEX_LABEL(87);

    goto _379;

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


goto _380;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _381; /* in QUEX_GOTO_STATE       */
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
#   undef CLI
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
   static const char  token_id_str_DQ_STRING_BEGIN[]         = "DQ_STRING_BEGIN";
   static const char  token_id_str_DQ_STRING_END[]           = "DQ_STRING_END";
   static const char  token_id_str_KEYWORD_APPEND[]          = "KEYWORD_APPEND";
   static const char  token_id_str_KEYWORD_DEVICE[]          = "KEYWORD_DEVICE";
   static const char  token_id_str_KEYWORD_FALSE[]           = "KEYWORD_FALSE";
   static const char  token_id_str_KEYWORD_HASH[]            = "KEYWORD_HASH";
   static const char  token_id_str_KEYWORD_LAST[]            = "KEYWORD_LAST";
   static const char  token_id_str_KEYWORD_LIST[]            = "KEYWORD_LIST";
   static const char  token_id_str_KEYWORD_REMOVE[]          = "KEYWORD_REMOVE";
   static const char  token_id_str_KEYWORD_ROOT[]            = "KEYWORD_ROOT";
   static const char  token_id_str_KEYWORD_RULE[]            = "KEYWORD_RULE";
   static const char  token_id_str_KEYWORD_SET_POLICY[]      = "KEYWORD_SET_POLICY";
   static const char  token_id_str_KEYWORD_TRUE[]            = "KEYWORD_TRUE";
   static const char  token_id_str_NUMBER_DAYS[]             = "NUMBER_DAYS";
   static const char  token_id_str_NUMBER_HOURS[]            = "NUMBER_HOURS";
   static const char  token_id_str_NUMBER_MINUTES[]          = "NUMBER_MINUTES";
   static const char  token_id_str_NUMBER_UNSIGNED_INTEGER[] = "NUMBER_UNSIGNED_INTEGER";
   static const char  token_id_str_OPTION_PARENT[]           = "OPTION_PARENT";
   static const char  token_id_str_OPTION_PERMANENT[]        = "OPTION_PERMANENT";
   static const char  token_id_str_OPTION_TIMEOUT[]          = "OPTION_TIMEOUT";
   static const char  token_id_str_STRING[]                  = "STRING";
       

   /* NOTE: This implementation works only for token id types that are 
    *       some type of integer or enum. In case an alien type is to
    *       used, this function needs to be redefined.                  */
   switch( TokenID ) {
   default: {
       __QUEX_STD_sprintf(error_string, "<UNKNOWN TOKEN-ID: %i>", (int)TokenID);
       return error_string;
   }
   case CLI_TOKEN_TERMINATION:    return termination_string;
   case CLI_TOKEN_UNINITIALIZED:  return uninitialized_string;
#  if defined(QUEX_OPTION_INDENTATION_TRIGGER)
   case CLI_TOKEN_INDENT:         return indent_string;
   case CLI_TOKEN_DEDENT:         return dedent_string;
   case CLI_TOKEN_NODENT:         return nodent_string;
#  endif
   case CLI_TOKEN_DQ_STRING_BEGIN:         return token_id_str_DQ_STRING_BEGIN;
   case CLI_TOKEN_DQ_STRING_END:           return token_id_str_DQ_STRING_END;
   case CLI_TOKEN_KEYWORD_APPEND:          return token_id_str_KEYWORD_APPEND;
   case CLI_TOKEN_KEYWORD_DEVICE:          return token_id_str_KEYWORD_DEVICE;
   case CLI_TOKEN_KEYWORD_FALSE:           return token_id_str_KEYWORD_FALSE;
   case CLI_TOKEN_KEYWORD_HASH:            return token_id_str_KEYWORD_HASH;
   case CLI_TOKEN_KEYWORD_LAST:            return token_id_str_KEYWORD_LAST;
   case CLI_TOKEN_KEYWORD_LIST:            return token_id_str_KEYWORD_LIST;
   case CLI_TOKEN_KEYWORD_REMOVE:          return token_id_str_KEYWORD_REMOVE;
   case CLI_TOKEN_KEYWORD_ROOT:            return token_id_str_KEYWORD_ROOT;
   case CLI_TOKEN_KEYWORD_RULE:            return token_id_str_KEYWORD_RULE;
   case CLI_TOKEN_KEYWORD_SET_POLICY:      return token_id_str_KEYWORD_SET_POLICY;
   case CLI_TOKEN_KEYWORD_TRUE:            return token_id_str_KEYWORD_TRUE;
   case CLI_TOKEN_NUMBER_DAYS:             return token_id_str_NUMBER_DAYS;
   case CLI_TOKEN_NUMBER_HOURS:            return token_id_str_NUMBER_HOURS;
   case CLI_TOKEN_NUMBER_MINUTES:          return token_id_str_NUMBER_MINUTES;
   case CLI_TOKEN_NUMBER_UNSIGNED_INTEGER: return token_id_str_NUMBER_UNSIGNED_INTEGER;
   case CLI_TOKEN_OPTION_PARENT:           return token_id_str_OPTION_PARENT;
   case CLI_TOKEN_OPTION_PERMANENT:        return token_id_str_OPTION_PERMANENT;
   case CLI_TOKEN_OPTION_TIMEOUT:          return token_id_str_OPTION_TIMEOUT;
   case CLI_TOKEN_STRING:                  return token_id_str_STRING;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

