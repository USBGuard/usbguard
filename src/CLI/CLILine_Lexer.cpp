
#include "CLILine_Lexer.hpp"

#include <quex/code_base/analyzer/C-adaptions.h>
/* The file 'multi.i' contains implementations which are the same for all 
 * possibly generated analyzers. If QUEX_OPTION_MULTI is defined, it is
 * NOT supposed to be included here. If not--in which case we have a single
 * analzer--then it is included.                                             */
#include <quex/code_base/single.i>

QUEX_NAMESPACE_MAIN_OPEN
/* Global */QUEX_NAME(Mode)  QUEX_NAME(CLI);
/* Global */QUEX_NAME(Mode)  QUEX_NAME(DQ_STRING_READER);
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
    case QUEX_NAME(ModeID_DQ_STRING_READER): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(DQ_STRING_READER)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): DQ_STRING_READER, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(CLI_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
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

		self_accumulator_flush(CLI_TOKEN_STRING);
		self_send(CLI_TOKEN_DQ_STRING_END);
	
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
    case QUEX_NAME(ModeID_CLI): return true;
    default:
        if( Mode->has_base(&QUEX_NAME(CLI)) ) return true;
    }
    __QUEX_STD_fprintf(stderr, "mode '%s' is not one of (and not a derived mode of): CLI, \n", Mode->name);
    __quex_assert(false);
    return false;
    
}
bool
QUEX_NAME(DQ_STRING_READER_has_exit_to)(const QUEX_NAME(Mode)* Mode) {
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

/* #include "CLILine_Lexer.hpp"*/
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

    /* (712 from BEFORE_ENTRY)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(712);
if     ( input >= 0xB )  goto _2;
else if( input == 0xA )  goto _3;
else if( input == 0x9 )  goto _4;
else                     goto _2;


    __quex_assert_no_passage();
_8:
    /* (712 from 716)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 714)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 713)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 715)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (713 from 712)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(713);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (714 from 712)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(714);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (715 from 712)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(715);
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
#   define QUEX_LABEL_STATE_ROUTER _351
#   define CLI                 (QUEX_NAME(CLI))
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
_350:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (592 from BEFORE_ENTRY)  */
_177:
    input = *(me->buffer._input_p);


    __quex_debug("Init State\n");
    __quex_debug_state(592);
switch( input ) {
case 0x0: goto _135;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _44;
case 0x22: goto _50;
case 0x2D: goto _43;
case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x61: goto _48;
case 0x64: goto _47;
case 0x66: goto _45;
case 0x68: goto _46;
case 0x6C: goto _40;
case 0x72: goto _41;
case 0x73: goto _49;
case 0x74: goto _51;
default: goto _87;
}


    __quex_assert_no_passage();
_133:
    /* (592 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    goto _177;


    __quex_assert_no_passage();
_87:
    /* (DROP_OUT from 610) (DROP_OUT from 592) (DROP_OUT from 654) (DROP_OUT from 661) (DROP_OUT from 636) (DROP_OUT from 664) (DROP_OUT from 623) (DROP_OUT from 630) (DROP_OUT from 605) (DROP_OUT from 651) (DROP_OUT from 599) (DROP_OUT from 620) (DROP_OUT from 627) (DROP_OUT from 602) (DROP_OUT from 648) (DROP_OUT from 671) (DROP_OUT from 614) (DROP_OUT from 596) (DROP_OUT from 642) (DROP_OUT from 617) (DROP_OUT from 624) (DROP_OUT from 668) (DROP_OUT from 611) (DROP_OUT from 593) (DROP_OUT from 662) (DROP_OUT from 637) (DROP_OUT from 608) (DROP_OUT from 631) (DROP_OUT from 606) (DROP_OUT from 652) (DROP_OUT from 659) (DROP_OUT from 634) (DROP_OUT from 646) (DROP_OUT from 628) (DROP_OUT from 674) (DROP_OUT from 615) (DROP_OUT from 643) (DROP_OUT from 618) (DROP_OUT from 625) (DROP_OUT from 600) (DROP_OUT from 612) (DROP_OUT from 594) (DROP_OUT from 640) (DROP_OUT from 663) (DROP_OUT from 638) (DROP_OUT from 666) (DROP_OUT from 609) (DROP_OUT from 653) (DROP_OUT from 660) (DROP_OUT from 635) (DROP_OUT from 647) (DROP_OUT from 622) (DROP_OUT from 604) (DROP_OUT from 650) (DROP_OUT from 632) (DROP_OUT from 598) (DROP_OUT from 644) (DROP_OUT from 619) (DROP_OUT from 601) (DROP_OUT from 672) (DROP_OUT from 670) (DROP_OUT from 613) (DROP_OUT from 641) (DROP_OUT from 639)  */

        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _180;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_88:
    /* (DROP_OUT from 595)  */
    goto _181;


    __quex_assert_no_passage();
_102:
    /* (DROP_OUT from 658)  */
    goto _182;


    __quex_assert_no_passage();
_107:
    /* (DROP_OUT from 675)  */
    goto _183;


    __quex_assert_no_passage();
_101:
    /* (DROP_OUT from 657)  */
    goto _184;


    __quex_assert_no_passage();
_106:
    /* (DROP_OUT from 673)  */
    goto _185;


    __quex_assert_no_passage();
_100:
    /* (DROP_OUT from 656)  */
    goto _186;


    __quex_assert_no_passage();
_105:
    /* (DROP_OUT from 669)  */
    goto _187;


    __quex_assert_no_passage();
_94:
    /* (DROP_OUT from 626)  */
    goto _188;


    __quex_assert_no_passage();
_99:
    /* (DROP_OUT from 655)  */
    goto _189;


    __quex_assert_no_passage();
_104:
    /* (DROP_OUT from 667)  */
    goto _190;


    __quex_assert_no_passage();
_93:
    /* (DROP_OUT from 621)  */
    goto _191;


    __quex_assert_no_passage();
_98:
    /* (DROP_OUT from 649)  */
    goto _192;


    __quex_assert_no_passage();
_103:
    /* (DROP_OUT from 665)  */
    goto _193;


    __quex_assert_no_passage();
_92:
    /* (DROP_OUT from 616)  */
    goto _194;


    __quex_assert_no_passage();
_97:
    /* (DROP_OUT from 645)  */
    goto _195;


    __quex_assert_no_passage();
_91:
    /* (DROP_OUT from 607)  */
    goto _196;


    __quex_assert_no_passage();
_96:
    /* (DROP_OUT from 633)  */
    goto _197;


    __quex_assert_no_passage();
_90:
    /* (DROP_OUT from 603)  */
    goto _198;


    __quex_assert_no_passage();
_95:
    /* (DROP_OUT from 629)  */
    goto _199;


    __quex_assert_no_passage();
_89:
    /* (DROP_OUT from 597)  */
    goto _200;


    __quex_assert_no_passage();
_40:
    /* (593 from 592) (593 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(593);
switch( input ) {
case 0x0: goto _136;
case 0x61: goto _34;
case 0x69: goto _35;
default: goto _87;
}


    __quex_assert_no_passage();
_41:
    /* (594 from RELOAD_FORWARD) (594 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(594);
switch( input ) {
case 0x0: goto _137;
case 0x65: goto _25;
case 0x6F: goto _23;
case 0x75: goto _24;
default: goto _87;
}


    __quex_assert_no_passage();
_42:
    /* (595 from 595) (595 from 592) (595 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(595);
switch( input ) {
case 0x0: goto _138;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x64: goto _22;
case 0x68: goto _21;
case 0x6D: goto _20;
default: goto _88;
}


    __quex_assert_no_passage();
_43:
    /* (596 from RELOAD_FORWARD) (596 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(596);
if     ( input == 0x2D )  goto _81;
else if( input == 0x0 )   goto _139;
else                      goto _87;


    __quex_assert_no_passage();
_44:
    /* (597 from 597) (597 from 592) (597 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(597);
switch( input ) {
case 0x0: goto _140;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _44;
default: goto _89;
}


    __quex_assert_no_passage();
_45:
    /* (598 from RELOAD_FORWARD) (598 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(598);
if     ( input == 0x61 )  goto _77;
else if( input == 0x0 )   goto _141;
else                      goto _87;


    __quex_assert_no_passage();
_46:
    /* (599 from 592) (599 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(599);
if     ( input == 0x61 )  goto _74;
else if( input == 0x0 )   goto _142;
else                      goto _87;


    __quex_assert_no_passage();
_47:
    /* (600 from RELOAD_FORWARD) (600 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(600);
if     ( input == 0x65 )  goto _69;
else if( input == 0x0 )   goto _143;
else                      goto _87;


    __quex_assert_no_passage();
_48:
    /* (601 from 592) (601 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(601);
if     ( input == 0x70 )  goto _64;
else if( input == 0x0 )   goto _144;
else                      goto _87;


    __quex_assert_no_passage();
_49:
    /* (602 from RELOAD_FORWARD) (602 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(602);
if     ( input == 0x65 )  goto _55;
else if( input == 0x0 )   goto _145;
else                      goto _87;


    __quex_assert_no_passage();
_50:
    /* (603 from 592)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(603);
goto _90;


    __quex_assert_no_passage();
_51:
    /* (604 from RELOAD_FORWARD) (604 from 592)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(604);
if     ( input == 0x72 )  goto _52;
else if( input == 0x0 )   goto _146;
else                      goto _87;


    __quex_assert_no_passage();
_52:
    /* (605 from 604) (605 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(605);
if     ( input == 0x75 )  goto _53;
else if( input == 0x0 )   goto _147;
else                      goto _87;


    __quex_assert_no_passage();
_53:
    /* (606 from RELOAD_FORWARD) (606 from 605)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(606);
if     ( input == 0x65 )  goto _54;
else if( input == 0x0 )   goto _148;
else                      goto _87;


    __quex_assert_no_passage();
_54:
    /* (607 from 606)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(607);
goto _91;


    __quex_assert_no_passage();
_55:
    /* (608 from 602) (608 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(608);
if     ( input == 0x74 )  goto _56;
else if( input == 0x0 )   goto _149;
else                      goto _87;


    __quex_assert_no_passage();
_56:
    /* (609 from 608) (609 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(609);
if     ( input == 0x2D )  goto _57;
else if( input == 0x0 )   goto _150;
else                      goto _87;


    __quex_assert_no_passage();
_57:
    /* (610 from RELOAD_FORWARD) (610 from 609)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(610);
if     ( input == 0x70 )  goto _58;
else if( input == 0x0 )   goto _151;
else                      goto _87;


    __quex_assert_no_passage();
_58:
    /* (611 from 610) (611 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(611);
if     ( input == 0x6F )  goto _59;
else if( input == 0x0 )   goto _152;
else                      goto _87;


    __quex_assert_no_passage();
_59:
    /* (612 from 611) (612 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(612);
if     ( input == 0x6C )  goto _60;
else if( input == 0x0 )   goto _153;
else                      goto _87;


    __quex_assert_no_passage();
_60:
    /* (613 from 612) (613 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(613);
if     ( input == 0x69 )  goto _61;
else if( input == 0x0 )   goto _154;
else                      goto _87;


    __quex_assert_no_passage();
_61:
    /* (614 from RELOAD_FORWARD) (614 from 613)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(614);
if     ( input == 0x63 )  goto _62;
else if( input == 0x0 )   goto _155;
else                      goto _87;


    __quex_assert_no_passage();
_62:
    /* (615 from 614) (615 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(615);
if     ( input == 0x79 )  goto _63;
else if( input == 0x0 )   goto _156;
else                      goto _87;


    __quex_assert_no_passage();
_63:
    /* (616 from 615)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(616);
goto _92;


    __quex_assert_no_passage();
_64:
    /* (617 from 601) (617 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(617);
if     ( input == 0x70 )  goto _65;
else if( input == 0x0 )   goto _157;
else                      goto _87;


    __quex_assert_no_passage();
_65:
    /* (618 from RELOAD_FORWARD) (618 from 617)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(618);
if     ( input == 0x65 )  goto _66;
else if( input == 0x0 )   goto _158;
else                      goto _87;


    __quex_assert_no_passage();
_66:
    /* (619 from 618) (619 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(619);
if     ( input == 0x6E )  goto _67;
else if( input == 0x0 )   goto _159;
else                      goto _87;


    __quex_assert_no_passage();
_67:
    /* (620 from 619) (620 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(620);
if     ( input == 0x64 )  goto _68;
else if( input == 0x0 )   goto _160;
else                      goto _87;


    __quex_assert_no_passage();
_68:
    /* (621 from 620)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(621);
goto _93;


    __quex_assert_no_passage();
_69:
    /* (622 from RELOAD_FORWARD) (622 from 600)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(622);
if     ( input == 0x76 )  goto _70;
else if( input == 0x0 )   goto _161;
else                      goto _87;


    __quex_assert_no_passage();
_70:
    /* (623 from 622) (623 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(623);
if     ( input == 0x69 )  goto _71;
else if( input == 0x0 )   goto _162;
else                      goto _87;


    __quex_assert_no_passage();
_71:
    /* (624 from RELOAD_FORWARD) (624 from 623)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(624);
if     ( input == 0x63 )  goto _72;
else if( input == 0x0 )   goto _163;
else                      goto _87;


    __quex_assert_no_passage();
_72:
    /* (625 from 624) (625 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(625);
if     ( input == 0x65 )  goto _73;
else if( input == 0x0 )   goto _164;
else                      goto _87;


    __quex_assert_no_passage();
_73:
    /* (626 from 625)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(626);
goto _94;


    __quex_assert_no_passage();
_74:
    /* (627 from RELOAD_FORWARD) (627 from 599)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(627);
if     ( input == 0x73 )  goto _75;
else if( input == 0x0 )   goto _165;
else                      goto _87;


    __quex_assert_no_passage();
_75:
    /* (628 from 627) (628 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(628);
if     ( input == 0x68 )  goto _76;
else if( input == 0x0 )   goto _166;
else                      goto _87;


    __quex_assert_no_passage();
_76:
    /* (629 from 628)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(629);
goto _95;


    __quex_assert_no_passage();
_77:
    /* (630 from RELOAD_FORWARD) (630 from 598)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(630);
if     ( input == 0x6C )  goto _78;
else if( input == 0x0 )   goto _167;
else                      goto _87;


    __quex_assert_no_passage();
_78:
    /* (631 from 630) (631 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(631);
if     ( input == 0x73 )  goto _79;
else if( input == 0x0 )   goto _168;
else                      goto _87;


    __quex_assert_no_passage();
_79:
    /* (632 from RELOAD_FORWARD) (632 from 631)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(632);
if     ( input == 0x65 )  goto _80;
else if( input == 0x0 )   goto _169;
else                      goto _87;


    __quex_assert_no_passage();
_80:
    /* (633 from 632)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(633);
goto _96;


    __quex_assert_no_passage();
_81:
    /* (634 from 596) (634 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(634);
switch( input ) {
case 0x0: goto _170;
case 0x70: goto _83;
case 0x74: goto _82;
default: goto _87;
}


    __quex_assert_no_passage();
_82:
    /* (635 from 634) (635 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(635);
if     ( input == 0x69 )  goto _14;
else if( input == 0x0 )   goto _171;
else                      goto _87;


    __quex_assert_no_passage();
_83:
    /* (636 from 634) (636 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(636);
switch( input ) {
case 0x0: goto _172;
case 0x61: goto _84;
case 0x65: goto _85;
default: goto _87;
}


    __quex_assert_no_passage();
_84:
    /* (637 from 636) (637 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(637);
if     ( input == 0x72 )  goto _10;
else if( input == 0x0 )   goto _173;
else                      goto _87;


    __quex_assert_no_passage();
_85:
    /* (638 from RELOAD_FORWARD) (638 from 636)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(638);
if     ( input == 0x72 )  goto _86;
else if( input == 0x0 )   goto _174;
else                      goto _87;


    __quex_assert_no_passage();
_86:
    /* (639 from 638) (639 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(639);
if     ( input == 0x6D )  goto _4;
else if( input == 0x0 )   goto _175;
else                      goto _87;


    __quex_assert_no_passage();
_4:
    /* (640 from RELOAD_FORWARD) (640 from 639)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(640);
if     ( input == 0x61 )  goto _5;
else if( input == 0x0 )   goto _108;
else                      goto _87;


    __quex_assert_no_passage();
_5:
    /* (641 from 640) (641 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(641);
if     ( input == 0x6E )  goto _6;
else if( input == 0x0 )   goto _109;
else                      goto _87;


    __quex_assert_no_passage();
_6:
    /* (642 from RELOAD_FORWARD) (642 from 641)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(642);
if     ( input == 0x65 )  goto _7;
else if( input == 0x0 )   goto _110;
else                      goto _87;


    __quex_assert_no_passage();
_7:
    /* (643 from 642) (643 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(643);
if     ( input == 0x6E )  goto _8;
else if( input == 0x0 )   goto _111;
else                      goto _87;


    __quex_assert_no_passage();
_8:
    /* (644 from 643) (644 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(644);
if     ( input == 0x74 )  goto _9;
else if( input == 0x0 )   goto _112;
else                      goto _87;


    __quex_assert_no_passage();
_9:
    /* (645 from 644)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(645);
goto _97;


    __quex_assert_no_passage();
_10:
    /* (646 from RELOAD_FORWARD) (646 from 637)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(646);
if     ( input == 0x65 )  goto _11;
else if( input == 0x0 )   goto _113;
else                      goto _87;


    __quex_assert_no_passage();
_11:
    /* (647 from 646) (647 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(647);
if     ( input == 0x6E )  goto _12;
else if( input == 0x0 )   goto _114;
else                      goto _87;


    __quex_assert_no_passage();
_12:
    /* (648 from RELOAD_FORWARD) (648 from 647)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(648);
if     ( input == 0x74 )  goto _13;
else if( input == 0x0 )   goto _115;
else                      goto _87;


    __quex_assert_no_passage();
_13:
    /* (649 from 648)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(649);
goto _98;


    __quex_assert_no_passage();
_14:
    /* (650 from RELOAD_FORWARD) (650 from 635)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(650);
if     ( input == 0x6D )  goto _15;
else if( input == 0x0 )   goto _116;
else                      goto _87;


    __quex_assert_no_passage();
_15:
    /* (651 from 650) (651 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(651);
if     ( input == 0x65 )  goto _16;
else if( input == 0x0 )   goto _117;
else                      goto _87;


    __quex_assert_no_passage();
_16:
    /* (652 from 651) (652 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(652);
if     ( input == 0x6F )  goto _17;
else if( input == 0x0 )   goto _118;
else                      goto _87;


    __quex_assert_no_passage();
_17:
    /* (653 from 652) (653 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(653);
if     ( input == 0x75 )  goto _18;
else if( input == 0x0 )   goto _119;
else                      goto _87;


    __quex_assert_no_passage();
_18:
    /* (654 from RELOAD_FORWARD) (654 from 653)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(654);
if     ( input == 0x74 )  goto _19;
else if( input == 0x0 )   goto _120;
else                      goto _87;


    __quex_assert_no_passage();
_19:
    /* (655 from 654)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(655);
goto _99;


    __quex_assert_no_passage();
_20:
    /* (656 from 595)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(656);
goto _100;


    __quex_assert_no_passage();
_21:
    /* (657 from 595)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(657);
goto _101;


    __quex_assert_no_passage();
_22:
    /* (658 from 595)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(658);
goto _102;


    __quex_assert_no_passage();
_23:
    /* (659 from 594) (659 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(659);
if     ( input == 0x6F )  goto _32;
else if( input == 0x0 )   goto _121;
else                      goto _87;


    __quex_assert_no_passage();
_24:
    /* (660 from 594) (660 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(660);
if     ( input == 0x6C )  goto _30;
else if( input == 0x0 )   goto _122;
else                      goto _87;


    __quex_assert_no_passage();
_25:
    /* (661 from RELOAD_FORWARD) (661 from 594)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(661);
if     ( input == 0x6D )  goto _26;
else if( input == 0x0 )   goto _123;
else                      goto _87;


    __quex_assert_no_passage();
_26:
    /* (662 from RELOAD_FORWARD) (662 from 661)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(662);
if     ( input == 0x6F )  goto _27;
else if( input == 0x0 )   goto _124;
else                      goto _87;


    __quex_assert_no_passage();
_27:
    /* (663 from 662) (663 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(663);
if     ( input == 0x76 )  goto _28;
else if( input == 0x0 )   goto _125;
else                      goto _87;


    __quex_assert_no_passage();
_28:
    /* (664 from RELOAD_FORWARD) (664 from 663)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(664);
if     ( input == 0x65 )  goto _29;
else if( input == 0x0 )   goto _126;
else                      goto _87;


    __quex_assert_no_passage();
_29:
    /* (665 from 664)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(665);
goto _103;


    __quex_assert_no_passage();
_30:
    /* (666 from 660) (666 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(666);
if     ( input == 0x65 )  goto _31;
else if( input == 0x0 )   goto _127;
else                      goto _87;


    __quex_assert_no_passage();
_31:
    /* (667 from 666)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(667);
goto _104;


    __quex_assert_no_passage();
_32:
    /* (668 from 659) (668 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(668);
if     ( input == 0x74 )  goto _33;
else if( input == 0x0 )   goto _128;
else                      goto _87;


    __quex_assert_no_passage();
_33:
    /* (669 from 668)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(669);
goto _105;


    __quex_assert_no_passage();
_34:
    /* (670 from 593) (670 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(670);
if     ( input == 0x73 )  goto _38;
else if( input == 0x0 )   goto _129;
else                      goto _87;


    __quex_assert_no_passage();
_35:
    /* (671 from RELOAD_FORWARD) (671 from 593)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(671);
if     ( input == 0x73 )  goto _36;
else if( input == 0x0 )   goto _130;
else                      goto _87;


    __quex_assert_no_passage();
_36:
    /* (672 from RELOAD_FORWARD) (672 from 671)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(672);
if     ( input == 0x74 )  goto _37;
else if( input == 0x0 )   goto _131;
else                      goto _87;


    __quex_assert_no_passage();
_37:
    /* (673 from 672)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(673);
goto _106;


    __quex_assert_no_passage();
_38:
    /* (674 from 670) (674 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(674);
if     ( input == 0x74 )  goto _39;
else if( input == 0x0 )   goto _132;
else                      goto _87;


    __quex_assert_no_passage();
_39:
    /* (675 from 674)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(675);
goto _107;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_134:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(CLI_TOKEN_TERMINATION);


#   line 1813 "CLILine_Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_180:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'CLI'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 1829 "CLILine_Lexer.cpp"

}
goto _1;
_200:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{


#   line 1839 "CLILine_Lexer.cpp"

}
goto _0;
_190:
    __quex_debug("* TERMINAL \"rule\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_RULE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1852 "CLILine_Lexer.cpp"

}
goto _0;
_188:
    __quex_debug("* TERMINAL \"device\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_DEVICE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1865 "CLILine_Lexer.cpp"

}
goto _0;
_191:
    __quex_debug("* TERMINAL \"append\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_APPEND);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1878 "CLILine_Lexer.cpp"

}
goto _0;
_193:
    __quex_debug("* TERMINAL \"remove\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(CLI_TOKEN_KEYWORD_REMOVE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1891 "CLILine_Lexer.cpp"

}
goto _0;
_185:
    __quex_debug("* TERMINAL \"list\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_LIST);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1904 "CLILine_Lexer.cpp"

}
goto _0;
_199:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1917 "CLILine_Lexer.cpp"

}
goto _0;
_194:
    __quex_debug("* TERMINAL \"set-policy\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(10);
{
self_send(CLI_TOKEN_KEYWORD_SET_POLICY);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1930 "CLILine_Lexer.cpp"

}
goto _0;
_183:
    __quex_debug("* TERMINAL \"last\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_LAST);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1943 "CLILine_Lexer.cpp"

}
goto _0;
_187:
    __quex_debug("* TERMINAL \"root\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_ROOT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1956 "CLILine_Lexer.cpp"

}
goto _0;
_196:
    __quex_debug("* TERMINAL \"true\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(CLI_TOKEN_KEYWORD_TRUE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1969 "CLILine_Lexer.cpp"

}
goto _0;
_197:
    __quex_debug("* TERMINAL \"false\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(CLI_TOKEN_KEYWORD_FALSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1982 "CLILine_Lexer.cpp"

}
goto _0;
_192:
    __quex_debug("* TERMINAL \"--parent\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{
self_send(CLI_TOKEN_OPTION_PARENT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 1995 "CLILine_Lexer.cpp"

}
goto _0;
_189:
    __quex_debug("* TERMINAL \"--timeout\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(9);
{
self_send(CLI_TOKEN_OPTION_TIMEOUT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2008 "CLILine_Lexer.cpp"

}
goto _0;
_195:
    __quex_debug("* TERMINAL \"--permanent\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(11);
{
self_send(CLI_TOKEN_OPTION_PERMANENT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2021 "CLILine_Lexer.cpp"

}
goto _0;
_181:
    __quex_debug("* TERMINAL [1-9][0-9]*\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd));
self_send(CLI_TOKEN_NUMBER_UNSIGNED_INTEGER);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2036 "CLILine_Lexer.cpp"

}
goto _0;
_184:
    __quex_debug("* TERMINAL [1-9][0-9]*\"h\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_HOURS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2051 "CLILine_Lexer.cpp"

}
goto _0;
_186:
    __quex_debug("* TERMINAL [1-9][0-9]*\"m\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_MINUTES);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2066 "CLILine_Lexer.cpp"

}
goto _0;
_182:
    __quex_debug("* TERMINAL [1-9][0-9]*\"d\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(LexemeL);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, (Lexeme), (LexemeEnd - 1));
self_send(CLI_TOKEN_NUMBER_DAYS);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2081 "CLILine_Lexer.cpp"

}
goto _0;
_198:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self_send(CLI_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 2095 "CLILine_Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_351:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 5: { goto _5; }
        case 6: { goto _6; }
        case 7: { goto _7; }
        case 8: { goto _8; }
        case 10: { goto _10; }
        case 11: { goto _11; }
        case 12: { goto _12; }
        case 14: { goto _14; }
        case 15: { goto _15; }
        case 16: { goto _16; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 23: { goto _23; }
        case 24: { goto _24; }
        case 25: { goto _25; }
        case 26: { goto _26; }
        case 27: { goto _27; }
        case 28: { goto _28; }
        case 30: { goto _30; }
        case 32: { goto _32; }
        case 34: { goto _34; }
        case 35: { goto _35; }
        case 36: { goto _36; }
        case 38: { goto _38; }
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
        case 51: { goto _51; }
        case 52: { goto _52; }
        case 53: { goto _53; }
        case 55: { goto _55; }
        case 56: { goto _56; }
        case 57: { goto _57; }
        case 58: { goto _58; }
        case 59: { goto _59; }
        case 60: { goto _60; }
        case 61: { goto _61; }
        case 62: { goto _62; }
        case 64: { goto _64; }
        case 65: { goto _65; }
        case 66: { goto _66; }
        case 67: { goto _67; }
        case 69: { goto _69; }
        case 70: { goto _70; }
        case 71: { goto _71; }
        case 72: { goto _72; }
        case 74: { goto _74; }
        case 75: { goto _75; }
        case 77: { goto _77; }
        case 78: { goto _78; }
        case 79: { goto _79; }
        case 81: { goto _81; }
        case 82: { goto _82; }
        case 83: { goto _83; }
        case 84: { goto _84; }
        case 85: { goto _85; }
        case 86: { goto _86; }
        case 87: { goto _87; }
        case 88: { goto _88; }
        case 89: { goto _89; }
        case 133: { goto _133; }
        case 134: { goto _134; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_144:
    /* (RELOAD_FORWARD from 601)  */
    target_state_index = QUEX_LABEL(48); target_state_else_index = QUEX_LABEL(87);

_349:
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
    /* (RELOAD_FORWARD from 670)  */
    target_state_index = QUEX_LABEL(34); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_111:
    /* (RELOAD_FORWARD from 643)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_167:
    /* (RELOAD_FORWARD from 630)  */
    target_state_index = QUEX_LABEL(77); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_136:
    /* (RELOAD_FORWARD from 593)  */
    target_state_index = QUEX_LABEL(40); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_158:
    /* (RELOAD_FORWARD from 618)  */
    target_state_index = QUEX_LABEL(65); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_156:
    /* (RELOAD_FORWARD from 615)  */
    target_state_index = QUEX_LABEL(62); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_125:
    /* (RELOAD_FORWARD from 663)  */
    target_state_index = QUEX_LABEL(27); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_123:
    /* (RELOAD_FORWARD from 661)  */
    target_state_index = QUEX_LABEL(25); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_147:
    /* (RELOAD_FORWARD from 605)  */
    target_state_index = QUEX_LABEL(52); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_114:
    /* (RELOAD_FORWARD from 647)  */
    target_state_index = QUEX_LABEL(11); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_170:
    /* (RELOAD_FORWARD from 634)  */
    target_state_index = QUEX_LABEL(81); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_152:
    /* (RELOAD_FORWARD from 611)  */
    target_state_index = QUEX_LABEL(58); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_139:
    /* (RELOAD_FORWARD from 596)  */
    target_state_index = QUEX_LABEL(43); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_174:
    /* (RELOAD_FORWARD from 638)  */
    target_state_index = QUEX_LABEL(85); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_119:
    /* (RELOAD_FORWARD from 653)  */
    target_state_index = QUEX_LABEL(17); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_143:
    /* (RELOAD_FORWARD from 600)  */
    target_state_index = QUEX_LABEL(47); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_128:
    /* (RELOAD_FORWARD from 668)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_110:
    /* (RELOAD_FORWARD from 642)  */
    target_state_index = QUEX_LABEL(6); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_166:
    /* (RELOAD_FORWARD from 628)  */
    target_state_index = QUEX_LABEL(75); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_135:
    /* (RELOAD_FORWARD from 592)  */
    target_state_index = QUEX_LABEL(133); target_state_else_index = QUEX_LABEL(134);

    goto _349;


    __quex_assert_no_passage();
_165:
    /* (RELOAD_FORWARD from 627)  */
    target_state_index = QUEX_LABEL(74); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_155:
    /* (RELOAD_FORWARD from 614)  */
    target_state_index = QUEX_LABEL(61); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_124:
    /* (RELOAD_FORWARD from 662)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_122:
    /* (RELOAD_FORWARD from 660)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_146:
    /* (RELOAD_FORWARD from 604)  */
    target_state_index = QUEX_LABEL(51); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_113:
    /* (RELOAD_FORWARD from 646)  */
    target_state_index = QUEX_LABEL(10); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_161:
    /* (RELOAD_FORWARD from 622)  */
    target_state_index = QUEX_LABEL(69); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_151:
    /* (RELOAD_FORWARD from 610)  */
    target_state_index = QUEX_LABEL(57); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_138:
    /* (RELOAD_FORWARD from 595)  */
    target_state_index = QUEX_LABEL(42); target_state_else_index = QUEX_LABEL(88);

    goto _349;


    __quex_assert_no_passage();
_118:
    /* (RELOAD_FORWARD from 652)  */
    target_state_index = QUEX_LABEL(16); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_142:
    /* (RELOAD_FORWARD from 599)  */
    target_state_index = QUEX_LABEL(46); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_109:
    /* (RELOAD_FORWARD from 641)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_173:
    /* (RELOAD_FORWARD from 637)  */
    target_state_index = QUEX_LABEL(84); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_132:
    /* (RELOAD_FORWARD from 674)  */
    target_state_index = QUEX_LABEL(38); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_164:
    /* (RELOAD_FORWARD from 625)  */
    target_state_index = QUEX_LABEL(72); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_154:
    /* (RELOAD_FORWARD from 613)  */
    target_state_index = QUEX_LABEL(60); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_131:
    /* (RELOAD_FORWARD from 672)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_121:
    /* (RELOAD_FORWARD from 659)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_169:
    /* (RELOAD_FORWARD from 632)  */
    target_state_index = QUEX_LABEL(79); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_112:
    /* (RELOAD_FORWARD from 644)  */
    target_state_index = QUEX_LABEL(8); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_160:
    /* (RELOAD_FORWARD from 620)  */
    target_state_index = QUEX_LABEL(67); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_150:
    /* (RELOAD_FORWARD from 609)  */
    target_state_index = QUEX_LABEL(56); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_127:
    /* (RELOAD_FORWARD from 666)  */
    target_state_index = QUEX_LABEL(30); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_117:
    /* (RELOAD_FORWARD from 651)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_149:
    /* (RELOAD_FORWARD from 608)  */
    target_state_index = QUEX_LABEL(55); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_108:
    /* (RELOAD_FORWARD from 640)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_172:
    /* (RELOAD_FORWARD from 636)  */
    target_state_index = QUEX_LABEL(83); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_141:
    /* (RELOAD_FORWARD from 598)  */
    target_state_index = QUEX_LABEL(45); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_163:
    /* (RELOAD_FORWARD from 624)  */
    target_state_index = QUEX_LABEL(71); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_145:
    /* (RELOAD_FORWARD from 602)  */
    target_state_index = QUEX_LABEL(49); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_130:
    /* (RELOAD_FORWARD from 671)  */
    target_state_index = QUEX_LABEL(35); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_120:
    /* (RELOAD_FORWARD from 654)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_168:
    /* (RELOAD_FORWARD from 631)  */
    target_state_index = QUEX_LABEL(78); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_137:
    /* (RELOAD_FORWARD from 594)  */
    target_state_index = QUEX_LABEL(41); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_159:
    /* (RELOAD_FORWARD from 619)  */
    target_state_index = QUEX_LABEL(66); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_157:
    /* (RELOAD_FORWARD from 617)  */
    target_state_index = QUEX_LABEL(64); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_126:
    /* (RELOAD_FORWARD from 664)  */
    target_state_index = QUEX_LABEL(28); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_116:
    /* (RELOAD_FORWARD from 650)  */
    target_state_index = QUEX_LABEL(14); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_148:
    /* (RELOAD_FORWARD from 606)  */
    target_state_index = QUEX_LABEL(53); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_115:
    /* (RELOAD_FORWARD from 648)  */
    target_state_index = QUEX_LABEL(12); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_171:
    /* (RELOAD_FORWARD from 635)  */
    target_state_index = QUEX_LABEL(82); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_153:
    /* (RELOAD_FORWARD from 612)  */
    target_state_index = QUEX_LABEL(59); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_140:
    /* (RELOAD_FORWARD from 597)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(89);

    goto _349;


    __quex_assert_no_passage();
_175:
    /* (RELOAD_FORWARD from 639)  */
    target_state_index = QUEX_LABEL(86); target_state_else_index = QUEX_LABEL(87);

    goto _349;


    __quex_assert_no_passage();
_162:
    /* (RELOAD_FORWARD from 623)  */
    target_state_index = QUEX_LABEL(70); target_state_else_index = QUEX_LABEL(87);

    goto _349;

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


goto _350;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _351; /* in QUEX_GOTO_STATE       */
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
                QUEX_NAME(CLI_counter)((ME), (BEGIN), (END));     \
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
#   define CLI                 (QUEX_NAME(CLI))
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

    /* (757 from BEFORE_ENTRY)  */
_19:
    input = *(me->buffer._input_p);


    __quex_debug("Init State\n");
    __quex_debug_state(757);
switch( input ) {
case 0x0: goto _16;
case 0x1: 
case 0xA: goto _9;
case 0x22: goto _4;
case 0x5C: goto _6;
default: goto _5;
}


    __quex_assert_no_passage();
_14:
    /* (757 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    goto _19;


    __quex_assert_no_passage();
_11:
    /* (DROP_OUT from 760) (DROP_OUT from 759)  */

    goto _22;
_20:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_10:
    /* (DROP_OUT from 758)  */
    goto _23;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 761)  */
    goto _24;


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 762)  */
    goto _26;


    __quex_assert_no_passage();
_9:
    /* (DROP_OUT from 757)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _25;
    goto _20;


    __quex_assert_no_passage();
_4:
    /* (758 from 757)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(758);
goto _10;


    __quex_assert_no_passage();
_5:
    /* (759 from 757)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(759);
goto _11;


    __quex_assert_no_passage();
_6:
    /* (760 from RELOAD_FORWARD) (760 from 757)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(760);
switch( input ) {
case 0x0: goto _17;
case 0x22: goto _8;
case 0x5C: goto _7;
default: goto _11;
}


    __quex_assert_no_passage();
_7:
    /* (761 from 760)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(761);
goto _12;


    __quex_assert_no_passage();
_8:
    /* (762 from 760)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(762);
goto _13;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_15:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 3031 "CLILine_Lexer.cpp"

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


#   line 3045 "CLILine_Lexer.cpp"

}
goto _2;
_26:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3058 "CLILine_Lexer.cpp"

}
goto _0;
_24:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3071 "CLILine_Lexer.cpp"

}
goto _0;
_23:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self << CLI;
    RETURN;


#   line 3084 "CLILine_Lexer.cpp"

}
goto _0;
_22:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3096 "CLILine_Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_35:
    switch( target_state_index ) {
        case 6: { goto _6; }
        case 11: { goto _11; }
        case 14: { goto _14; }
        case 15: { goto _15; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_16:
    /* (RELOAD_FORWARD from 757)  */
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
    /* (RELOAD_FORWARD from 760)  */
    target_state_index = QUEX_LABEL(6); target_state_else_index = QUEX_LABEL(11);

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
#   undef CLI
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

