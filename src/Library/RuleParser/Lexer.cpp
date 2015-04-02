
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

    /* (783 from NONE)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

    __quex_debug("Init State\n");
    __quex_debug_state(783);
_9:
    __quex_debug("Init State\n");
    __quex_debug_state(783);
if     ( input >= 0xB )  goto _3;
else if( input == 0xA )  goto _4;
else if( input == 0x9 )  goto _2;
else                     goto _3;


    __quex_assert_no_passage();
_8:
    /* (783 from 787)  */
    __quex_debug_state(783);
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 785)  */
    goto _11;
    __quex_debug("Drop-Out Catcher\n");

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 784)  */
    goto _12;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 786)  */
    goto _13;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_2:
    /* (784 from 783)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(784);

    __quex_debug_state(784);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (785 from 783)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(785);

    __quex_debug_state(785);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (786 from 783)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(786);

    __quex_debug_state(786);
goto _7;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_13:
    __quex_debug("* TERMINAL LINE\n");
__QUEX_IF_COUNT_LINES_ADD((size_t)1);

    __QUEX_IF_COUNT_COLUMNS((me->counter._column_number_at_end) = (size_t)1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_12:
    __quex_debug("* TERMINAL GRID\n");
__QUEX_IF_COUNT_COLUMNS_ADD((size_t)(((me->buffer._input_p) - reference_p - 1)));

__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end -= 1);
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end &= ~ ((size_t)0x3));
__QUEX_IF_COUNT_COLUMNS(self.counter._column_number_at_end += 4 + 1);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

if( me->buffer._input_p != LexemeEnd ) goto _8;

goto _0;

_11:
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
#   define QUEX_LABEL_STATE_ROUTER _446
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
    QUEX_TYPE_ACCEPTANCE_ID        last_acceptance               /* un-initilized */;
    QUEX_TYPE_GOTO_LABEL           target_state_else_index        = QUEX_GOTO_LABEL_VOID;
    const size_t                   PositionRegisterN              = (size_t)0;
    QUEX_TYPE_CHARACTER_POSITION*  position                       = 0x0;
    QUEX_TYPE_CHARACTER            input                          = (QUEX_TYPE_CHARACTER)(0x00);
    QUEX_TYPE_GOTO_LABEL           target_state_index             = QUEX_GOTO_LABEL_VOID;

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
_445:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (640 from NONE)  */
    { last_acceptance = ((QUEX_TYPE_ACCEPTANCE_ID)-1); __quex_debug("last_acceptance = ((QUEX_TYPE_ACCEPTANCE_ID)-1)\n"); }

    __quex_debug("Init State\n");
    __quex_debug_state(640);
_231:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(640);

    __quex_debug("Init State\n");
    __quex_debug_state(640);
switch( input ) {
case 0x0: goto _144;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _9;
case 0x22: goto _13;
case 0x2A: goto _19;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _16;
case 0x3A: goto _12;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _7;
case 0x61: goto _15;
case 0x62: goto _14;
case 0x63: case 0x64: goto _16;
case 0x65: goto _8;
case 0x66: goto _16;
case 0x68: goto _18;
case 0x6E: goto _4;
case 0x6F: goto _20;
case 0x72: goto _21;
case 0x73: goto _6;
case 0x76: goto _17;
case 0x77: goto _5;
case 0x7B: goto _10;
case 0x7D: goto _11;
default: goto _107;
}


    __quex_assert_no_passage();
_142:
    /* (640 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(640);
    goto _231;


    __quex_assert_no_passage();
_118:
    /* (DROP_OUT from 679) (DROP_OUT from 684)  */
    __quex_debug("Drop-Out Catcher\n");

    goto _234;
    __quex_debug("Drop-Out Catcher\n");
_232:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_127:
    /* (DROP_OUT from 699)  */
    goto _236;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_121:
    /* (DROP_OUT from 690)  */
    goto _237;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_114:
    /* (DROP_OUT from 663)  */
    goto _238;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_117:
    /* (DROP_OUT from 678)  */
    goto _239;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_110:
    /* (DROP_OUT from 648)  */
    goto _240;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_139:
    /* (DROP_OUT from 734)  */
    goto _241;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_128:
    /* (DROP_OUT from 703)  */
    goto _242;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_122:
    /* (DROP_OUT from 691)  */
    goto _243;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_111:
    /* (DROP_OUT from 649)  */
    goto _244;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_140:
    /* (DROP_OUT from 738)  */
    goto _245;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_129:
    /* (DROP_OUT from 708)  */
    goto _235;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_112:
    /* (DROP_OUT from 650)  */
    goto _247;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_137:
    /* (DROP_OUT from 716)  */
    goto _248;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_115:
    /* (DROP_OUT from 668)  */
    goto _249;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_141:
    /* (DROP_OUT from 743)  */
    goto _250;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_108:
    /* (DROP_OUT from 646)  */
    goto _251;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_126:
    /* (DROP_OUT from 695)  */
    goto _252;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_120:
    /* (DROP_OUT from 682)  */
    goto _253;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_113:
    /* (DROP_OUT from 656)  */
    goto _254;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_138:
    /* (DROP_OUT from 721)  */
    goto _255;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_116:
    /* (DROP_OUT from 671)  */
    goto _256;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_109:
    /* (DROP_OUT from 647)  */
    goto _257;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_134:
    /* (DROP_OUT from 713)  */
        me->buffer._input_p -= 5;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_123:
    /* (DROP_OUT from 692)  */
        me->buffer._input_p -= 1;
goto _234;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_135:
    /* (DROP_OUT from 714)  */
        me->buffer._input_p -= 6;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_131:
    /* (DROP_OUT from 710)  */
        me->buffer._input_p -= 2;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_124:
    /* (DROP_OUT from 693)  */
        me->buffer._input_p -= 2;
goto _234;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_136:
    /* (DROP_OUT from 715)  */
        me->buffer._input_p -= 7;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_107:
    /* (DROP_OUT from 720) (DROP_OUT from 654) (DROP_OUT from 733) (DROP_OUT from 651) (DROP_OUT from 730) (DROP_OUT from 705) (DROP_OUT from 718) (DROP_OUT from 700) (DROP_OUT from 697) (DROP_OUT from 660) (DROP_OUT from 675) (DROP_OUT from 657) (DROP_OUT from 672) (DROP_OUT from 670) (DROP_OUT from 685) (DROP_OUT from 667) (DROP_OUT from 664) (DROP_OUT from 727) (DROP_OUT from 645) (DROP_OUT from 742) (DROP_OUT from 724) (DROP_OUT from 642) (DROP_OUT from 739) (DROP_OUT from 655) (DROP_OUT from 736) (DROP_OUT from 652) (DROP_OUT from 731) (DROP_OUT from 728) (DROP_OUT from 706) (DROP_OUT from 688) (DROP_OUT from 719) (DROP_OUT from 701) (DROP_OUT from 698) (DROP_OUT from 661) (DROP_OUT from 676) (DROP_OUT from 658) (DROP_OUT from 673) (DROP_OUT from 686) (DROP_OUT from 683) (DROP_OUT from 665) (DROP_OUT from 680) (DROP_OUT from 725) (DROP_OUT from 643) (DROP_OUT from 740) (DROP_OUT from 722) (DROP_OUT from 640) (DROP_OUT from 737) (DROP_OUT from 735) (DROP_OUT from 653) (DROP_OUT from 732) (DROP_OUT from 729) (DROP_OUT from 707) (DROP_OUT from 689) (DROP_OUT from 704) (DROP_OUT from 702) (DROP_OUT from 717) (DROP_OUT from 696) (DROP_OUT from 662) (DROP_OUT from 677) (DROP_OUT from 659) (DROP_OUT from 674) (DROP_OUT from 687) (DROP_OUT from 669) (DROP_OUT from 666) (DROP_OUT from 726) (DROP_OUT from 644) (DROP_OUT from 741) (DROP_OUT from 723) (DROP_OUT from 641)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _246;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_132:
    /* (DROP_OUT from 711)  */
        me->buffer._input_p -= 3;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_125:
    /* (DROP_OUT from 694)  */
        me->buffer._input_p -= 3;
goto _234;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_119:
    /* (DROP_OUT from 681)  */
switch( last_acceptance ) {
case ((QUEX_TYPE_ACCEPTANCE_ID)-1):     me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _246;
case 6:     me->buffer._input_p -= 1;
goto _234;
}

    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_130:
    /* (DROP_OUT from 709)  */
        me->buffer._input_p -= 1;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_133:
    /* (DROP_OUT from 712)  */
        me->buffer._input_p -= 4;
goto _235;
    __quex_debug("Drop-Out Catcher\n");
    goto _232;


    __quex_assert_no_passage();
_4:
    /* (641 from 640) (641 from RELOAD_FORWARD)  */
    __quex_debug_state(641);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(641);

    __quex_debug_state(641);
switch( input ) {
case 0x0: goto _145;
case 0x61: goto _99;
case 0x6F: goto _98;
default: goto _107;
}


    __quex_assert_no_passage();
_5:
    /* (642 from RELOAD_FORWARD) (642 from 640)  */
    __quex_debug_state(642);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(642);

    __quex_debug_state(642);
if     ( input == 0x69 )  goto _85;
else if( input == 0x0 )   goto _146;
else                      goto _107;


    __quex_assert_no_passage();
_6:
    /* (643 from 640) (643 from RELOAD_FORWARD)  */
    __quex_debug_state(643);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(643);

    __quex_debug_state(643);
if     ( input == 0x65 )  goto _80;
else if( input == 0x0 )   goto _147;
else                      goto _107;


    __quex_assert_no_passage();
_7:
    /* (644 from RELOAD_FORWARD) (644 from 640)  */
    __quex_debug_state(644);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(644);

    __quex_debug_state(644);
switch( input ) {
case 0x0: goto _148;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _43;
default: goto _107;
}


    __quex_assert_no_passage();
_8:
    /* (645 from 640) (645 from RELOAD_FORWARD)  */
    __quex_debug_state(645);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(645);

    __quex_debug_state(645);
switch( input ) {
case 0x0: goto _149;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _43;
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _42;
case 0x71: goto _67;
default: goto _107;
}


    __quex_assert_no_passage();
_9:
    /* (646 from RELOAD_FORWARD) (646 from 640) (646 from 646)  */
    __quex_debug_state(646);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(646);

    __quex_debug_state(646);
switch( input ) {
case 0x0: goto _150;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _9;
default: goto _108;
}


    __quex_assert_no_passage();
_10:
    /* (647 from 640)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(647);

    __quex_debug_state(647);
goto _109;


    __quex_assert_no_passage();
_11:
    /* (648 from 640)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(648);

    __quex_debug_state(648);
goto _110;


    __quex_assert_no_passage();
_12:
    /* (649 from 640)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(649);

    __quex_debug_state(649);
goto _111;


    __quex_assert_no_passage();
_13:
    /* (650 from 640)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(650);

    __quex_debug_state(650);
goto _112;


    __quex_assert_no_passage();
_14:
    /* (651 from 640) (651 from RELOAD_FORWARD)  */
    __quex_debug_state(651);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(651);

    __quex_debug_state(651);
switch( input ) {
case 0x0: goto _151;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _43;
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _42;
case 0x6C: goto _63;
default: goto _107;
}


    __quex_assert_no_passage();
_15:
    /* (652 from RELOAD_FORWARD) (652 from 640)  */
    __quex_debug_state(652);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(652);

    __quex_debug_state(652);
switch( input ) {
case 0x0: goto _152;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _43;
case 0x61: case 0x62: goto _42;
case 0x63: goto _47;
case 0x64: case 0x65: case 0x66: goto _42;
case 0x6C: goto _48;
case 0x6E: goto _46;
default: goto _107;
}


    __quex_assert_no_passage();
_16:
    /* (653 from 640) (653 from RELOAD_FORWARD)  */
    __quex_debug_state(653);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(653);

    __quex_debug_state(653);
switch( input ) {
case 0x0: goto _153;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _42;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _43;
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _42;
default: goto _107;
}


    __quex_assert_no_passage();
_17:
    /* (654 from RELOAD_FORWARD) (654 from 640)  */
    __quex_debug_state(654);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(654);

    __quex_debug_state(654);
if     ( input == 0x69 )  goto _35;
else if( input == 0x0 )   goto _154;
else                      goto _107;


    __quex_assert_no_passage();
_18:
    /* (655 from 640) (655 from RELOAD_FORWARD)  */
    __quex_debug_state(655);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(655);

    __quex_debug_state(655);
if     ( input == 0x61 )  goto _32;
else if( input == 0x0 )   goto _155;
else                      goto _107;


    __quex_assert_no_passage();
_19:
    /* (656 from 640)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(656);

    __quex_debug_state(656);
goto _113;


    __quex_assert_no_passage();
_20:
    /* (657 from 640) (657 from RELOAD_FORWARD)  */
    __quex_debug_state(657);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(657);

    __quex_debug_state(657);
if     ( input == 0x6E )  goto _27;
else if( input == 0x0 )   goto _156;
else                      goto _107;


    __quex_assert_no_passage();
_21:
    /* (658 from RELOAD_FORWARD) (658 from 640)  */
    __quex_debug_state(658);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(658);

    __quex_debug_state(658);
if     ( input == 0x65 )  goto _22;
else if( input == 0x0 )   goto _157;
else                      goto _107;


    __quex_assert_no_passage();
_22:
    /* (659 from 658) (659 from RELOAD_FORWARD)  */
    __quex_debug_state(659);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(659);

    __quex_debug_state(659);
if     ( input == 0x6A )  goto _23;
else if( input == 0x0 )   goto _158;
else                      goto _107;


    __quex_assert_no_passage();
_23:
    /* (660 from 659) (660 from RELOAD_FORWARD)  */
    __quex_debug_state(660);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(660);

    __quex_debug_state(660);
if     ( input == 0x65 )  goto _24;
else if( input == 0x0 )   goto _159;
else                      goto _107;


    __quex_assert_no_passage();
_24:
    /* (661 from 660) (661 from RELOAD_FORWARD)  */
    __quex_debug_state(661);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(661);

    __quex_debug_state(661);
if     ( input == 0x63 )  goto _25;
else if( input == 0x0 )   goto _160;
else                      goto _107;


    __quex_assert_no_passage();
_25:
    /* (662 from RELOAD_FORWARD) (662 from 661)  */
    __quex_debug_state(662);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(662);

    __quex_debug_state(662);
if     ( input == 0x74 )  goto _26;
else if( input == 0x0 )   goto _161;
else                      goto _107;


    __quex_assert_no_passage();
_26:
    /* (663 from 662)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(663);

    __quex_debug_state(663);
goto _114;


    __quex_assert_no_passage();
_27:
    /* (664 from 657) (664 from RELOAD_FORWARD)  */
    __quex_debug_state(664);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(664);

    __quex_debug_state(664);
if     ( input == 0x65 )  goto _28;
else if( input == 0x0 )   goto _162;
else                      goto _107;


    __quex_assert_no_passage();
_28:
    /* (665 from 664) (665 from RELOAD_FORWARD)  */
    __quex_debug_state(665);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(665);

    __quex_debug_state(665);
if     ( input == 0x2D )  goto _29;
else if( input == 0x0 )   goto _163;
else                      goto _107;


    __quex_assert_no_passage();
_29:
    /* (666 from RELOAD_FORWARD) (666 from 665)  */
    __quex_debug_state(666);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(666);

    __quex_debug_state(666);
if     ( input == 0x6F )  goto _30;
else if( input == 0x0 )   goto _164;
else                      goto _107;


    __quex_assert_no_passage();
_30:
    /* (667 from 666) (667 from RELOAD_FORWARD)  */
    __quex_debug_state(667);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(667);

    __quex_debug_state(667);
if     ( input == 0x66 )  goto _31;
else if( input == 0x0 )   goto _165;
else                      goto _107;


    __quex_assert_no_passage();
_31:
    /* (668 from 667)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(668);

    __quex_debug_state(668);
goto _115;


    __quex_assert_no_passage();
_32:
    /* (669 from 655) (669 from RELOAD_FORWARD)  */
    __quex_debug_state(669);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(669);

    __quex_debug_state(669);
if     ( input == 0x73 )  goto _33;
else if( input == 0x0 )   goto _166;
else                      goto _107;


    __quex_assert_no_passage();
_33:
    /* (670 from RELOAD_FORWARD) (670 from 669)  */
    __quex_debug_state(670);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(670);

    __quex_debug_state(670);
if     ( input == 0x68 )  goto _34;
else if( input == 0x0 )   goto _167;
else                      goto _107;


    __quex_assert_no_passage();
_34:
    /* (671 from 670)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(671);

    __quex_debug_state(671);
goto _116;


    __quex_assert_no_passage();
_35:
    /* (672 from RELOAD_FORWARD) (672 from 654)  */
    __quex_debug_state(672);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(672);

    __quex_debug_state(672);
if     ( input == 0x61 )  goto _36;
else if( input == 0x0 )   goto _168;
else                      goto _107;


    __quex_assert_no_passage();
_36:
    /* (673 from 672) (673 from RELOAD_FORWARD)  */
    __quex_debug_state(673);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(673);

    __quex_debug_state(673);
if     ( input == 0x2D )  goto _37;
else if( input == 0x0 )   goto _169;
else                      goto _107;


    __quex_assert_no_passage();
_37:
    /* (674 from RELOAD_FORWARD) (674 from 673)  */
    __quex_debug_state(674);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(674);

    __quex_debug_state(674);
if     ( input == 0x70 )  goto _38;
else if( input == 0x0 )   goto _170;
else                      goto _107;


    __quex_assert_no_passage();
_38:
    /* (675 from 674) (675 from RELOAD_FORWARD)  */
    __quex_debug_state(675);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(675);

    __quex_debug_state(675);
if     ( input == 0x6F )  goto _39;
else if( input == 0x0 )   goto _171;
else                      goto _107;


    __quex_assert_no_passage();
_39:
    /* (676 from 675) (676 from RELOAD_FORWARD)  */
    __quex_debug_state(676);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(676);

    __quex_debug_state(676);
if     ( input == 0x72 )  goto _40;
else if( input == 0x0 )   goto _172;
else                      goto _107;


    __quex_assert_no_passage();
_40:
    /* (677 from 676) (677 from RELOAD_FORWARD)  */
    __quex_debug_state(677);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(677);

    __quex_debug_state(677);
if     ( input == 0x74 )  goto _41;
else if( input == 0x0 )   goto _173;
else                      goto _107;


    __quex_assert_no_passage();
_41:
    /* (678 from 677)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(678);

    __quex_debug_state(678);
goto _117;


    __quex_assert_no_passage();
_42:
    /* (679 from 653) (679 from 652) (679 from 645) (679 from 651)  */
    { last_acceptance = 6; __quex_debug("last_acceptance = 6\n"); }

    __quex_debug_state(679);
_326:
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(679);

    __quex_debug_state(679);
switch( input ) {
case 0x0: goto _175;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _44;
default: goto _118;
}


    __quex_assert_no_passage();
_174:
    /* (679 from RELOAD_FORWARD)  */
    __quex_debug_state(679);
    goto _326;


    __quex_assert_no_passage();
_43:
    /* (680 from 653) (680 from 644) (680 from 645) (680 from RELOAD_FORWARD) (680 from 651) (680 from 652)  */
    __quex_debug_state(680);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(680);

    __quex_debug_state(680);
switch( input ) {
case 0x0: goto _176;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _44;
default: goto _107;
}


    __quex_assert_no_passage();
_44:
    /* (681 from 680) (681 from RELOAD_FORWARD) (681 from 684) (681 from 679)  */
    __quex_debug_state(681);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(681);

    __quex_debug_state(681);
switch( input ) {
case 0x0: goto _177;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _45;
default: goto _119;
}


    __quex_assert_no_passage();
_45:
    /* (682 from 681)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(682);

    __quex_debug_state(682);
goto _120;


    __quex_assert_no_passage();
_46:
    /* (683 from RELOAD_FORWARD) (683 from 652)  */
    __quex_debug_state(683);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(683);

    __quex_debug_state(683);
if     ( input == 0x79 )  goto _59;
else if( input == 0x0 )   goto _178;
else                      goto _107;


    __quex_assert_no_passage();
_179:
    /* (684 from RELOAD_FORWARD)  */
    __quex_debug_state(684);
_335:
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(684);

    __quex_debug_state(684);
switch( input ) {
case 0x0: goto _180;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _44;
case 0x74: goto _55;
default: goto _118;
}


    __quex_assert_no_passage();
_47:
    /* (684 from 652)  */
    { last_acceptance = 6; __quex_debug("last_acceptance = 6\n"); }

    __quex_debug_state(684);
    goto _335;


    __quex_assert_no_passage();
_48:
    /* (685 from 652) (685 from RELOAD_FORWARD)  */
    __quex_debug_state(685);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(685);

    __quex_debug_state(685);
if     ( input == 0x6C )  goto _49;
else if( input == 0x0 )   goto _181;
else                      goto _107;


    __quex_assert_no_passage();
_49:
    /* (686 from RELOAD_FORWARD) (686 from 685)  */
    __quex_debug_state(686);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(686);

    __quex_debug_state(686);
switch( input ) {
case 0x0: goto _182;
case 0x2D: goto _51;
case 0x6F: goto _50;
default: goto _107;
}


    __quex_assert_no_passage();
_50:
    /* (687 from 686) (687 from RELOAD_FORWARD)  */
    __quex_debug_state(687);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(687);

    __quex_debug_state(687);
if     ( input == 0x77 )  goto _54;
else if( input == 0x0 )   goto _183;
else                      goto _107;


    __quex_assert_no_passage();
_51:
    /* (688 from RELOAD_FORWARD) (688 from 686)  */
    __quex_debug_state(688);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(688);

    __quex_debug_state(688);
if     ( input == 0x6F )  goto _52;
else if( input == 0x0 )   goto _184;
else                      goto _107;


    __quex_assert_no_passage();
_52:
    /* (689 from 688) (689 from RELOAD_FORWARD)  */
    __quex_debug_state(689);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(689);

    __quex_debug_state(689);
if     ( input == 0x66 )  goto _53;
else if( input == 0x0 )   goto _185;
else                      goto _107;


    __quex_assert_no_passage();
_53:
    /* (690 from 689)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(690);

    __quex_debug_state(690);
goto _121;


    __quex_assert_no_passage();
_54:
    /* (691 from 687)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(691);

    __quex_debug_state(691);
goto _122;


    __quex_assert_no_passage();
_55:
    /* (692 from 684) (692 from RELOAD_FORWARD)  */
    __quex_debug_state(692);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(692);

    __quex_debug_state(692);
if     ( input == 0x69 )  goto _56;
else if( input == 0x0 )   goto _186;
else                      goto _123;


    __quex_assert_no_passage();
_56:
    /* (693 from 692) (693 from RELOAD_FORWARD)  */
    __quex_debug_state(693);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(693);

    __quex_debug_state(693);
if     ( input == 0x6F )  goto _57;
else if( input == 0x0 )   goto _187;
else                      goto _124;


    __quex_assert_no_passage();
_57:
    /* (694 from RELOAD_FORWARD) (694 from 693)  */
    __quex_debug_state(694);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(694);

    __quex_debug_state(694);
if     ( input == 0x6E )  goto _58;
else if( input == 0x0 )   goto _188;
else                      goto _125;


    __quex_assert_no_passage();
_58:
    /* (695 from 694)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(695);

    __quex_debug_state(695);
goto _126;


    __quex_assert_no_passage();
_59:
    /* (696 from 683) (696 from RELOAD_FORWARD)  */
    __quex_debug_state(696);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(696);

    __quex_debug_state(696);
if     ( input == 0x2D )  goto _60;
else if( input == 0x0 )   goto _189;
else                      goto _107;


    __quex_assert_no_passage();
_60:
    /* (697 from 696) (697 from RELOAD_FORWARD)  */
    __quex_debug_state(697);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(697);

    __quex_debug_state(697);
if     ( input == 0x6F )  goto _61;
else if( input == 0x0 )   goto _190;
else                      goto _107;


    __quex_assert_no_passage();
_61:
    /* (698 from RELOAD_FORWARD) (698 from 697)  */
    __quex_debug_state(698);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(698);

    __quex_debug_state(698);
if     ( input == 0x66 )  goto _62;
else if( input == 0x0 )   goto _191;
else                      goto _107;


    __quex_assert_no_passage();
_62:
    /* (699 from 698)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(699);

    __quex_debug_state(699);
goto _127;


    __quex_assert_no_passage();
_63:
    /* (700 from 651) (700 from RELOAD_FORWARD)  */
    __quex_debug_state(700);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(700);

    __quex_debug_state(700);
if     ( input == 0x6F )  goto _64;
else if( input == 0x0 )   goto _192;
else                      goto _107;


    __quex_assert_no_passage();
_64:
    /* (701 from 700) (701 from RELOAD_FORWARD)  */
    __quex_debug_state(701);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(701);

    __quex_debug_state(701);
if     ( input == 0x63 )  goto _65;
else if( input == 0x0 )   goto _193;
else                      goto _107;


    __quex_assert_no_passage();
_65:
    /* (702 from RELOAD_FORWARD) (702 from 701)  */
    __quex_debug_state(702);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(702);

    __quex_debug_state(702);
if     ( input == 0x6B )  goto _66;
else if( input == 0x0 )   goto _194;
else                      goto _107;


    __quex_assert_no_passage();
_66:
    /* (703 from 702)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(703);

    __quex_debug_state(703);
goto _128;


    __quex_assert_no_passage();
_67:
    /* (704 from RELOAD_FORWARD) (704 from 645)  */
    __quex_debug_state(704);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(704);

    __quex_debug_state(704);
if     ( input == 0x75 )  goto _68;
else if( input == 0x0 )   goto _195;
else                      goto _107;


    __quex_assert_no_passage();
_68:
    /* (705 from 704) (705 from RELOAD_FORWARD)  */
    __quex_debug_state(705);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(705);

    __quex_debug_state(705);
if     ( input == 0x61 )  goto _69;
else if( input == 0x0 )   goto _196;
else                      goto _107;


    __quex_assert_no_passage();
_69:
    /* (706 from RELOAD_FORWARD) (706 from 705)  */
    __quex_debug_state(706);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(706);

    __quex_debug_state(706);
if     ( input == 0x6C )  goto _70;
else if( input == 0x0 )   goto _197;
else                      goto _107;


    __quex_assert_no_passage();
_70:
    /* (707 from 706) (707 from RELOAD_FORWARD)  */
    __quex_debug_state(707);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(707);

    __quex_debug_state(707);
if     ( input == 0x73 )  goto _71;
else if( input == 0x0 )   goto _198;
else                      goto _107;


    __quex_assert_no_passage();
_71:
    /* (708 from 707) (708 from RELOAD_FORWARD)  */
    __quex_debug_state(708);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(708);

    __quex_debug_state(708);
if     ( input == 0x2D )  goto _72;
else if( input == 0x0 )   goto _199;
else                      goto _129;


    __quex_assert_no_passage();
_72:
    /* (709 from 708) (709 from RELOAD_FORWARD)  */
    __quex_debug_state(709);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(709);

    __quex_debug_state(709);
if     ( input == 0x6F )  goto _73;
else if( input == 0x0 )   goto _200;
else                      goto _130;


    __quex_assert_no_passage();
_73:
    /* (710 from RELOAD_FORWARD) (710 from 709)  */
    __quex_debug_state(710);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(710);

    __quex_debug_state(710);
if     ( input == 0x72 )  goto _74;
else if( input == 0x0 )   goto _201;
else                      goto _131;


    __quex_assert_no_passage();
_74:
    /* (711 from 710) (711 from RELOAD_FORWARD)  */
    __quex_debug_state(711);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(711);

    __quex_debug_state(711);
if     ( input == 0x64 )  goto _75;
else if( input == 0x0 )   goto _202;
else                      goto _132;


    __quex_assert_no_passage();
_75:
    /* (712 from RELOAD_FORWARD) (712 from 711)  */
    __quex_debug_state(712);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(712);

    __quex_debug_state(712);
if     ( input == 0x65 )  goto _76;
else if( input == 0x0 )   goto _203;
else                      goto _133;


    __quex_assert_no_passage();
_76:
    /* (713 from 712) (713 from RELOAD_FORWARD)  */
    __quex_debug_state(713);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(713);

    __quex_debug_state(713);
if     ( input == 0x72 )  goto _77;
else if( input == 0x0 )   goto _204;
else                      goto _134;


    __quex_assert_no_passage();
_77:
    /* (714 from RELOAD_FORWARD) (714 from 713)  */
    __quex_debug_state(714);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(714);

    __quex_debug_state(714);
if     ( input == 0x65 )  goto _78;
else if( input == 0x0 )   goto _205;
else                      goto _135;


    __quex_assert_no_passage();
_78:
    /* (715 from 714) (715 from RELOAD_FORWARD)  */
    __quex_debug_state(715);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(715);

    __quex_debug_state(715);
if     ( input == 0x64 )  goto _79;
else if( input == 0x0 )   goto _206;
else                      goto _136;


    __quex_assert_no_passage();
_79:
    /* (716 from 715)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(716);

    __quex_debug_state(716);
goto _137;


    __quex_assert_no_passage();
_80:
    /* (717 from RELOAD_FORWARD) (717 from 643)  */
    __quex_debug_state(717);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(717);

    __quex_debug_state(717);
if     ( input == 0x72 )  goto _81;
else if( input == 0x0 )   goto _207;
else                      goto _107;


    __quex_assert_no_passage();
_81:
    /* (718 from RELOAD_FORWARD) (718 from 717)  */
    __quex_debug_state(718);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(718);

    __quex_debug_state(718);
if     ( input == 0x69 )  goto _82;
else if( input == 0x0 )   goto _208;
else                      goto _107;


    __quex_assert_no_passage();
_82:
    /* (719 from 718) (719 from RELOAD_FORWARD)  */
    __quex_debug_state(719);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(719);

    __quex_debug_state(719);
if     ( input == 0x61 )  goto _83;
else if( input == 0x0 )   goto _209;
else                      goto _107;


    __quex_assert_no_passage();
_83:
    /* (720 from RELOAD_FORWARD) (720 from 719)  */
    __quex_debug_state(720);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(720);

    __quex_debug_state(720);
if     ( input == 0x6C )  goto _84;
else if( input == 0x0 )   goto _210;
else                      goto _107;


    __quex_assert_no_passage();
_84:
    /* (721 from 720)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(721);

    __quex_debug_state(721);
goto _138;


    __quex_assert_no_passage();
_85:
    /* (722 from RELOAD_FORWARD) (722 from 642)  */
    __quex_debug_state(722);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(722);

    __quex_debug_state(722);
if     ( input == 0x74 )  goto _86;
else if( input == 0x0 )   goto _211;
else                      goto _107;


    __quex_assert_no_passage();
_86:
    /* (723 from 722) (723 from RELOAD_FORWARD)  */
    __quex_debug_state(723);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(723);

    __quex_debug_state(723);
if     ( input == 0x68 )  goto _87;
else if( input == 0x0 )   goto _212;
else                      goto _107;


    __quex_assert_no_passage();
_87:
    /* (724 from 723) (724 from RELOAD_FORWARD)  */
    __quex_debug_state(724);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(724);

    __quex_debug_state(724);
if     ( input == 0x2D )  goto _88;
else if( input == 0x0 )   goto _213;
else                      goto _107;


    __quex_assert_no_passage();
_88:
    /* (725 from 724) (725 from RELOAD_FORWARD)  */
    __quex_debug_state(725);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(725);

    __quex_debug_state(725);
if     ( input == 0x69 )  goto _89;
else if( input == 0x0 )   goto _214;
else                      goto _107;


    __quex_assert_no_passage();
_89:
    /* (726 from RELOAD_FORWARD) (726 from 725)  */
    __quex_debug_state(726);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(726);

    __quex_debug_state(726);
if     ( input == 0x6E )  goto _90;
else if( input == 0x0 )   goto _215;
else                      goto _107;


    __quex_assert_no_passage();
_90:
    /* (727 from 726) (727 from RELOAD_FORWARD)  */
    __quex_debug_state(727);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(727);

    __quex_debug_state(727);
if     ( input == 0x74 )  goto _91;
else if( input == 0x0 )   goto _216;
else                      goto _107;


    __quex_assert_no_passage();
_91:
    /* (728 from RELOAD_FORWARD) (728 from 727)  */
    __quex_debug_state(728);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(728);

    __quex_debug_state(728);
if     ( input == 0x65 )  goto _92;
else if( input == 0x0 )   goto _217;
else                      goto _107;


    __quex_assert_no_passage();
_92:
    /* (729 from 728) (729 from RELOAD_FORWARD)  */
    __quex_debug_state(729);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(729);

    __quex_debug_state(729);
if     ( input == 0x72 )  goto _93;
else if( input == 0x0 )   goto _218;
else                      goto _107;


    __quex_assert_no_passage();
_93:
    /* (730 from RELOAD_FORWARD) (730 from 729)  */
    __quex_debug_state(730);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(730);

    __quex_debug_state(730);
if     ( input == 0x66 )  goto _94;
else if( input == 0x0 )   goto _219;
else                      goto _107;


    __quex_assert_no_passage();
_94:
    /* (731 from 730) (731 from RELOAD_FORWARD)  */
    __quex_debug_state(731);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(731);

    __quex_debug_state(731);
if     ( input == 0x61 )  goto _95;
else if( input == 0x0 )   goto _220;
else                      goto _107;


    __quex_assert_no_passage();
_95:
    /* (732 from 731) (732 from RELOAD_FORWARD)  */
    __quex_debug_state(732);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(732);

    __quex_debug_state(732);
if     ( input == 0x63 )  goto _96;
else if( input == 0x0 )   goto _221;
else                      goto _107;


    __quex_assert_no_passage();
_96:
    /* (733 from 732) (733 from RELOAD_FORWARD)  */
    __quex_debug_state(733);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(733);

    __quex_debug_state(733);
if     ( input == 0x65 )  goto _97;
else if( input == 0x0 )   goto _222;
else                      goto _107;


    __quex_assert_no_passage();
_97:
    /* (734 from 733)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(734);

    __quex_debug_state(734);
goto _139;


    __quex_assert_no_passage();
_98:
    /* (735 from RELOAD_FORWARD) (735 from 641)  */
    __quex_debug_state(735);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(735);

    __quex_debug_state(735);
if     ( input == 0x6E )  goto _102;
else if( input == 0x0 )   goto _223;
else                      goto _107;


    __quex_assert_no_passage();
_99:
    /* (736 from 641) (736 from RELOAD_FORWARD)  */
    __quex_debug_state(736);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(736);

    __quex_debug_state(736);
if     ( input == 0x6D )  goto _100;
else if( input == 0x0 )   goto _224;
else                      goto _107;


    __quex_assert_no_passage();
_100:
    /* (737 from 736) (737 from RELOAD_FORWARD)  */
    __quex_debug_state(737);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(737);

    __quex_debug_state(737);
if     ( input == 0x65 )  goto _101;
else if( input == 0x0 )   goto _225;
else                      goto _107;


    __quex_assert_no_passage();
_101:
    /* (738 from 737)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(738);

    __quex_debug_state(738);
goto _140;


    __quex_assert_no_passage();
_102:
    /* (739 from RELOAD_FORWARD) (739 from 735)  */
    __quex_debug_state(739);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(739);

    __quex_debug_state(739);
if     ( input == 0x65 )  goto _103;
else if( input == 0x0 )   goto _226;
else                      goto _107;


    __quex_assert_no_passage();
_103:
    /* (740 from 739) (740 from RELOAD_FORWARD)  */
    __quex_debug_state(740);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(740);

    __quex_debug_state(740);
if     ( input == 0x2D )  goto _104;
else if( input == 0x0 )   goto _227;
else                      goto _107;


    __quex_assert_no_passage();
_104:
    /* (741 from 740) (741 from RELOAD_FORWARD)  */
    __quex_debug_state(741);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(741);

    __quex_debug_state(741);
if     ( input == 0x6F )  goto _105;
else if( input == 0x0 )   goto _228;
else                      goto _107;


    __quex_assert_no_passage();
_105:
    /* (742 from RELOAD_FORWARD) (742 from 741)  */
    __quex_debug_state(742);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(742);

    __quex_debug_state(742);
if     ( input == 0x66 )  goto _106;
else if( input == 0x0 )   goto _229;
else                      goto _107;


    __quex_assert_no_passage();
_106:
    /* (743 from 742)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(743);

    __quex_debug_state(743);
goto _141;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_143:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 2523 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_246:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 2539 "Lexer.cpp"

}
goto _1;
_251:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{


#   line 2549 "Lexer.cpp"

}
goto _0;
_254:
    __quex_debug("* TERMINAL \"*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_ASTERISK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2562 "Lexer.cpp"

}
goto _0;
_257:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2575 "Lexer.cpp"

}
goto _0;
_240:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2588 "Lexer.cpp"

}
goto _0;
_244:
    __quex_debug("* TERMINAL \":\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_COLON);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2601 "Lexer.cpp"

}
goto _0;
_253:
    __quex_debug("* TERMINAL [0-9a-fA-F]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR4);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2615 "Lexer.cpp"

}
goto _0;
_234:
    __quex_debug("* TERMINAL [0-9a-f]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR2);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2629 "Lexer.cpp"

}
goto _0;
_243:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2642 "Lexer.cpp"

}
goto _0;
_242:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2655 "Lexer.cpp"

}
goto _0;
_238:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2668 "Lexer.cpp"

}
goto _0;
_245:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2681 "Lexer.cpp"

}
goto _0;
_252:
    __quex_debug("* TERMINAL \"action\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_ACTION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2694 "Lexer.cpp"

}
goto _0;
_256:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2707 "Lexer.cpp"

}
goto _0;
_239:
    __quex_debug("* TERMINAL \"via-port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{
self_send(RULE_TOKEN_KEYWORD_VIAPORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2720 "Lexer.cpp"

}
goto _0;
_241:
    __quex_debug("* TERMINAL \"with-interface\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{
self_send(RULE_TOKEN_KEYWORD_WITHINTERFACE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2733 "Lexer.cpp"

}
goto _0;
_255:
    __quex_debug("* TERMINAL \"serial\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_SERIAL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2746 "Lexer.cpp"

}
goto _0;
_237:
    __quex_debug("* TERMINAL \"all-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2760 "Lexer.cpp"

}
goto _0;
_236:
    __quex_debug("* TERMINAL \"any-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2774 "Lexer.cpp"

}
goto _0;
_249:
    __quex_debug("* TERMINAL \"one-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2788 "Lexer.cpp"

}
goto _0;
_250:
    __quex_debug("* TERMINAL \"none-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2802 "Lexer.cpp"

}
goto _0;
_235:
    __quex_debug("* TERMINAL \"equals\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2816 "Lexer.cpp"

}
goto _0;
_248:
    __quex_debug("* TERMINAL \"equals-ordered\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2830 "Lexer.cpp"

}
goto _0;
_247:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 2844 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_446:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 5: { goto _5; }
        case 6: { goto _6; }
        case 7: { goto _7; }
        case 8: { goto _8; }
        case 9: { goto _9; }
        case 14: { goto _14; }
        case 15: { goto _15; }
        case 16: { goto _16; }
        case 17: { goto _17; }
        case 18: { goto _18; }
        case 20: { goto _20; }
        case 21: { goto _21; }
        case 22: { goto _22; }
        case 23: { goto _23; }
        case 24: { goto _24; }
        case 25: { goto _25; }
        case 27: { goto _27; }
        case 28: { goto _28; }
        case 29: { goto _29; }
        case 30: { goto _30; }
        case 32: { goto _32; }
        case 33: { goto _33; }
        case 35: { goto _35; }
        case 36: { goto _36; }
        case 37: { goto _37; }
        case 38: { goto _38; }
        case 39: { goto _39; }
        case 40: { goto _40; }
        case 43: { goto _43; }
        case 44: { goto _44; }
        case 46: { goto _46; }
        case 48: { goto _48; }
        case 49: { goto _49; }
        case 50: { goto _50; }
        case 51: { goto _51; }
        case 52: { goto _52; }
        case 55: { goto _55; }
        case 56: { goto _56; }
        case 57: { goto _57; }
        case 59: { goto _59; }
        case 60: { goto _60; }
        case 61: { goto _61; }
        case 63: { goto _63; }
        case 64: { goto _64; }
        case 65: { goto _65; }
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
        case 80: { goto _80; }
        case 81: { goto _81; }
        case 82: { goto _82; }
        case 83: { goto _83; }
        case 85: { goto _85; }
        case 86: { goto _86; }
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
        case 98: { goto _98; }
        case 99: { goto _99; }
        case 100: { goto _100; }
        case 102: { goto _102; }
        case 103: { goto _103; }
        case 104: { goto _104; }
        case 105: { goto _105; }
        case 107: { goto _107; }
        case 108: { goto _108; }
        case 118: { goto _118; }
        case 119: { goto _119; }
        case 123: { goto _123; }
        case 124: { goto _124; }
        case 125: { goto _125; }
        case 129: { goto _129; }
        case 130: { goto _130; }
        case 131: { goto _131; }
        case 132: { goto _132; }
        case 133: { goto _133; }
        case 134: { goto _134; }
        case 135: { goto _135; }
        case 136: { goto _136; }
        case 142: { goto _142; }
        case 143: { goto _143; }
        case 174: { goto _174; }
        case 179: { goto _179; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_180:
    /* (RELOAD_FORWARD from 684)  */
    target_state_index = QUEX_LABEL(179); target_state_else_index = QUEX_LABEL(118);

_444:
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
_165:
    /* (RELOAD_FORWARD from 667)  */
    target_state_index = QUEX_LABEL(30); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_147:
    /* (RELOAD_FORWARD from 643)  */
    target_state_index = QUEX_LABEL(6); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_203:
    /* (RELOAD_FORWARD from 712)  */
    target_state_index = QUEX_LABEL(75); target_state_else_index = QUEX_LABEL(133);

    goto _444;


    __quex_assert_no_passage();
_170:
    /* (RELOAD_FORWARD from 674)  */
    target_state_index = QUEX_LABEL(37); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_223:
    /* (RELOAD_FORWARD from 735)  */
    target_state_index = QUEX_LABEL(98); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_194:
    /* (RELOAD_FORWARD from 702)  */
    target_state_index = QUEX_LABEL(65); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_192:
    /* (RELOAD_FORWARD from 700)  */
    target_state_index = QUEX_LABEL(63); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_161:
    /* (RELOAD_FORWARD from 662)  */
    target_state_index = QUEX_LABEL(25); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_214:
    /* (RELOAD_FORWARD from 725)  */
    target_state_index = QUEX_LABEL(88); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_159:
    /* (RELOAD_FORWARD from 660)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_183:
    /* (RELOAD_FORWARD from 687)  */
    target_state_index = QUEX_LABEL(50); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_229:
    /* (RELOAD_FORWARD from 742)  */
    target_state_index = QUEX_LABEL(105); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_150:
    /* (RELOAD_FORWARD from 646)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(108);

    goto _444;


    __quex_assert_no_passage();
_219:
    /* (RELOAD_FORWARD from 730)  */
    target_state_index = QUEX_LABEL(93); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_206:
    /* (RELOAD_FORWARD from 715)  */
    target_state_index = QUEX_LABEL(78); target_state_else_index = QUEX_LABEL(136);

    goto _444;


    __quex_assert_no_passage();
_188:
    /* (RELOAD_FORWARD from 694)  */
    target_state_index = QUEX_LABEL(57); target_state_else_index = QUEX_LABEL(125);

    goto _444;


    __quex_assert_no_passage();
_173:
    /* (RELOAD_FORWARD from 677)  */
    target_state_index = QUEX_LABEL(40); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_210:
    /* (RELOAD_FORWARD from 720)  */
    target_state_index = QUEX_LABEL(83); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_155:
    /* (RELOAD_FORWARD from 655)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_178:
    /* (RELOAD_FORWARD from 683)  */
    target_state_index = QUEX_LABEL(46); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_164:
    /* (RELOAD_FORWARD from 666)  */
    target_state_index = QUEX_LABEL(29); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_146:
    /* (RELOAD_FORWARD from 642)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_202:
    /* (RELOAD_FORWARD from 711)  */
    target_state_index = QUEX_LABEL(74); target_state_else_index = QUEX_LABEL(132);

    goto _444;


    __quex_assert_no_passage();
_169:
    /* (RELOAD_FORWARD from 673)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_222:
    /* (RELOAD_FORWARD from 733)  */
    target_state_index = QUEX_LABEL(96); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_201:
    /* (RELOAD_FORWARD from 710)  */
    target_state_index = QUEX_LABEL(73); target_state_else_index = QUEX_LABEL(131);

    goto _444;


    __quex_assert_no_passage();
_191:
    /* (RELOAD_FORWARD from 698)  */
    target_state_index = QUEX_LABEL(61); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_160:
    /* (RELOAD_FORWARD from 661)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_158:
    /* (RELOAD_FORWARD from 659)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_182:
    /* (RELOAD_FORWARD from 686)  */
    target_state_index = QUEX_LABEL(49); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_228:
    /* (RELOAD_FORWARD from 741)  */
    target_state_index = QUEX_LABEL(104); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_149:
    /* (RELOAD_FORWARD from 645)  */
    target_state_index = QUEX_LABEL(8); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_218:
    /* (RELOAD_FORWARD from 729)  */
    target_state_index = QUEX_LABEL(92); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_197:
    /* (RELOAD_FORWARD from 706)  */
    target_state_index = QUEX_LABEL(69); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_187:
    /* (RELOAD_FORWARD from 693)  */
    target_state_index = QUEX_LABEL(56); target_state_else_index = QUEX_LABEL(124);

    goto _444;


    __quex_assert_no_passage();
_172:
    /* (RELOAD_FORWARD from 676)  */
    target_state_index = QUEX_LABEL(39); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_217:
    /* (RELOAD_FORWARD from 728)  */
    target_state_index = QUEX_LABEL(91); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_154:
    /* (RELOAD_FORWARD from 654)  */
    target_state_index = QUEX_LABEL(17); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_177:
    /* (RELOAD_FORWARD from 681)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(119);

    goto _444;


    __quex_assert_no_passage();
_145:
    /* (RELOAD_FORWARD from 641)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_209:
    /* (RELOAD_FORWARD from 719)  */
    target_state_index = QUEX_LABEL(82); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_168:
    /* (RELOAD_FORWARD from 672)  */
    target_state_index = QUEX_LABEL(35); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_213:
    /* (RELOAD_FORWARD from 724)  */
    target_state_index = QUEX_LABEL(87); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_200:
    /* (RELOAD_FORWARD from 709)  */
    target_state_index = QUEX_LABEL(72); target_state_else_index = QUEX_LABEL(130);

    goto _444;


    __quex_assert_no_passage();
_190:
    /* (RELOAD_FORWARD from 697)  */
    target_state_index = QUEX_LABEL(60); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_167:
    /* (RELOAD_FORWARD from 670)  */
    target_state_index = QUEX_LABEL(33); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_157:
    /* (RELOAD_FORWARD from 658)  */
    target_state_index = QUEX_LABEL(21); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_205:
    /* (RELOAD_FORWARD from 714)  */
    target_state_index = QUEX_LABEL(77); target_state_else_index = QUEX_LABEL(135);

    goto _444;


    __quex_assert_no_passage();
_227:
    /* (RELOAD_FORWARD from 740)  */
    target_state_index = QUEX_LABEL(103); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_148:
    /* (RELOAD_FORWARD from 644)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_225:
    /* (RELOAD_FORWARD from 737)  */
    target_state_index = QUEX_LABEL(100); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_196:
    /* (RELOAD_FORWARD from 705)  */
    target_state_index = QUEX_LABEL(68); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_186:
    /* (RELOAD_FORWARD from 692)  */
    target_state_index = QUEX_LABEL(55); target_state_else_index = QUEX_LABEL(123);

    goto _444;


    __quex_assert_no_passage();
_163:
    /* (RELOAD_FORWARD from 665)  */
    target_state_index = QUEX_LABEL(28); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_216:
    /* (RELOAD_FORWARD from 727)  */
    target_state_index = QUEX_LABEL(90); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_153:
    /* (RELOAD_FORWARD from 653)  */
    target_state_index = QUEX_LABEL(16); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_185:
    /* (RELOAD_FORWARD from 689)  */
    target_state_index = QUEX_LABEL(52); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_144:
    /* (RELOAD_FORWARD from 640)  */
    target_state_index = QUEX_LABEL(142); target_state_else_index = QUEX_LABEL(143);

    goto _444;


    __quex_assert_no_passage();
_221:
    /* (RELOAD_FORWARD from 732)  */
    target_state_index = QUEX_LABEL(95); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_208:
    /* (RELOAD_FORWARD from 718)  */
    target_state_index = QUEX_LABEL(81); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_176:
    /* (RELOAD_FORWARD from 680)  */
    target_state_index = QUEX_LABEL(43); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_212:
    /* (RELOAD_FORWARD from 723)  */
    target_state_index = QUEX_LABEL(86); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_199:
    /* (RELOAD_FORWARD from 708)  */
    target_state_index = QUEX_LABEL(71); target_state_else_index = QUEX_LABEL(129);

    goto _444;


    __quex_assert_no_passage();
_181:
    /* (RELOAD_FORWARD from 685)  */
    target_state_index = QUEX_LABEL(48); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_166:
    /* (RELOAD_FORWARD from 669)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_156:
    /* (RELOAD_FORWARD from 657)  */
    target_state_index = QUEX_LABEL(20); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_204:
    /* (RELOAD_FORWARD from 713)  */
    target_state_index = QUEX_LABEL(76); target_state_else_index = QUEX_LABEL(134);

    goto _444;


    __quex_assert_no_passage();
_226:
    /* (RELOAD_FORWARD from 739)  */
    target_state_index = QUEX_LABEL(102); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_171:
    /* (RELOAD_FORWARD from 675)  */
    target_state_index = QUEX_LABEL(38); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_224:
    /* (RELOAD_FORWARD from 736)  */
    target_state_index = QUEX_LABEL(99); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_195:
    /* (RELOAD_FORWARD from 704)  */
    target_state_index = QUEX_LABEL(67); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_193:
    /* (RELOAD_FORWARD from 701)  */
    target_state_index = QUEX_LABEL(64); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_162:
    /* (RELOAD_FORWARD from 664)  */
    target_state_index = QUEX_LABEL(27); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_215:
    /* (RELOAD_FORWARD from 726)  */
    target_state_index = QUEX_LABEL(89); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_152:
    /* (RELOAD_FORWARD from 652)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_184:
    /* (RELOAD_FORWARD from 688)  */
    target_state_index = QUEX_LABEL(51); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_151:
    /* (RELOAD_FORWARD from 651)  */
    target_state_index = QUEX_LABEL(14); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_220:
    /* (RELOAD_FORWARD from 731)  */
    target_state_index = QUEX_LABEL(94); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_207:
    /* (RELOAD_FORWARD from 717)  */
    target_state_index = QUEX_LABEL(80); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_189:
    /* (RELOAD_FORWARD from 696)  */
    target_state_index = QUEX_LABEL(59); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_175:
    /* (RELOAD_FORWARD from 679)  */
    target_state_index = QUEX_LABEL(174); target_state_else_index = QUEX_LABEL(118);

    goto _444;


    __quex_assert_no_passage();
_211:
    /* (RELOAD_FORWARD from 722)  */
    target_state_index = QUEX_LABEL(85); target_state_else_index = QUEX_LABEL(107);

    goto _444;


    __quex_assert_no_passage();
_198:
    /* (RELOAD_FORWARD from 707)  */
    target_state_index = QUEX_LABEL(70); target_state_else_index = QUEX_LABEL(107);

    goto _444;

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


goto _445;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _446; /* in QUEX_GOTO_STATE       */
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

    /* (828 from NONE)  */
    __quex_debug("Init State\n");
    __quex_debug_state(828);
_19:
    input = *(me->buffer._input_p);

    __quex_debug("Init State\n");
    __quex_debug_state(828);

    __quex_debug("Init State\n");
    __quex_debug_state(828);
switch( input ) {
case 0x0: goto _16;
case 0x1: 
case 0xA: goto _9;
case 0x22: goto _8;
case 0x5C: goto _7;
default: goto _6;
}


    __quex_assert_no_passage();
_14:
    /* (828 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    __quex_debug_state(828);
    goto _19;


    __quex_assert_no_passage();
_10:
    /* (DROP_OUT from 829) (DROP_OUT from 830)  */
    __quex_debug("Drop-Out Catcher\n");

    goto _22;
    __quex_debug("Drop-Out Catcher\n");
_20:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 832)  */
    goto _23;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_11:
    /* (DROP_OUT from 831)  */
    goto _25;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 833)  */
    goto _26;
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_9:
    /* (DROP_OUT from 828)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _24;
    __quex_debug("Drop-Out Catcher\n");
    goto _20;


    __quex_assert_no_passage();
_4:
    /* (832 from 830)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(832);

    __quex_debug_state(832);
goto _12;


    __quex_assert_no_passage();
_5:
    /* (833 from 830)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(833);

    __quex_debug_state(833);
goto _13;


    __quex_assert_no_passage();
_6:
    /* (829 from 828)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(829);

    __quex_debug_state(829);
goto _10;


    __quex_assert_no_passage();
_7:
    /* (830 from RELOAD_FORWARD) (830 from 828)  */
    __quex_debug_state(830);

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(830);

    __quex_debug_state(830);
switch( input ) {
case 0x0: goto _17;
case 0x22: goto _4;
case 0x5C: goto _5;
default: goto _10;
}


    __quex_assert_no_passage();
_8:
    /* (831 from 828)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);

    __quex_debug_state(831);

    __quex_debug_state(831);
goto _11;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_15:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 3971 "Lexer.cpp"

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


#   line 3985 "Lexer.cpp"

}
goto _2;
_23:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3998 "Lexer.cpp"

}
goto _0;
_26:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 4011 "Lexer.cpp"

}
goto _0;
_25:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self << RULE;
    RETURN;


#   line 4024 "Lexer.cpp"

}
goto _0;
_22:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 4036 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_35:
    switch( target_state_index ) {
        case 7: { goto _7; }
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
    /* (RELOAD_FORWARD from 828)  */
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
    /* (RELOAD_FORWARD from 830)  */
    target_state_index = QUEX_LABEL(7); target_state_else_index = QUEX_LABEL(10);

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
   static const char  token_id_str_ASTERISK[]              = "ASTERISK";
   static const char  token_id_str_COLON[]                 = "COLON";
   static const char  token_id_str_CURLYBRACE_CLOSE[]      = "CURLYBRACE_CLOSE";
   static const char  token_id_str_CURLYBRACE_OPEN[]       = "CURLYBRACE_OPEN";
   static const char  token_id_str_DQ_STRING[]             = "DQ_STRING";
   static const char  token_id_str_DQ_STRING_BEGIN[]       = "DQ_STRING_BEGIN";
   static const char  token_id_str_DQ_STRING_END[]         = "DQ_STRING_END";
   static const char  token_id_str_HEXCHAR2[]              = "HEXCHAR2";
   static const char  token_id_str_HEXCHAR4[]              = "HEXCHAR4";
   static const char  token_id_str_KEYWORD_ACTION[]        = "KEYWORD_ACTION";
   static const char  token_id_str_KEYWORD_ALLOW[]         = "KEYWORD_ALLOW";
   static const char  token_id_str_KEYWORD_BLOCK[]         = "KEYWORD_BLOCK";
   static const char  token_id_str_KEYWORD_HASH[]          = "KEYWORD_HASH";
   static const char  token_id_str_KEYWORD_NAME[]          = "KEYWORD_NAME";
   static const char  token_id_str_KEYWORD_REJECT[]        = "KEYWORD_REJECT";
   static const char  token_id_str_KEYWORD_SERIAL[]        = "KEYWORD_SERIAL";
   static const char  token_id_str_KEYWORD_VIAPORT[]       = "KEYWORD_VIAPORT";
   static const char  token_id_str_KEYWORD_WITHINTERFACE[] = "KEYWORD_WITHINTERFACE";
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
   case RULE_TOKEN_CURLYBRACE_CLOSE:      return token_id_str_CURLYBRACE_CLOSE;
   case RULE_TOKEN_CURLYBRACE_OPEN:       return token_id_str_CURLYBRACE_OPEN;
   case RULE_TOKEN_DQ_STRING:             return token_id_str_DQ_STRING;
   case RULE_TOKEN_DQ_STRING_BEGIN:       return token_id_str_DQ_STRING_BEGIN;
   case RULE_TOKEN_DQ_STRING_END:         return token_id_str_DQ_STRING_END;
   case RULE_TOKEN_HEXCHAR2:              return token_id_str_HEXCHAR2;
   case RULE_TOKEN_HEXCHAR4:              return token_id_str_HEXCHAR4;
   case RULE_TOKEN_KEYWORD_ACTION:        return token_id_str_KEYWORD_ACTION;
   case RULE_TOKEN_KEYWORD_ALLOW:         return token_id_str_KEYWORD_ALLOW;
   case RULE_TOKEN_KEYWORD_BLOCK:         return token_id_str_KEYWORD_BLOCK;
   case RULE_TOKEN_KEYWORD_HASH:          return token_id_str_KEYWORD_HASH;
   case RULE_TOKEN_KEYWORD_NAME:          return token_id_str_KEYWORD_NAME;
   case RULE_TOKEN_KEYWORD_REJECT:        return token_id_str_KEYWORD_REJECT;
   case RULE_TOKEN_KEYWORD_SERIAL:        return token_id_str_KEYWORD_SERIAL;
   case RULE_TOKEN_KEYWORD_VIAPORT:       return token_id_str_KEYWORD_VIAPORT;
   case RULE_TOKEN_KEYWORD_WITHINTERFACE: return token_id_str_KEYWORD_WITHINTERFACE;
   case RULE_TOKEN_SET_OPERATOR:          return token_id_str_SET_OPERATOR;

   }
}

QUEX_NAMESPACE_TOKEN_CLOSE

