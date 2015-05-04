
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

    /* (801 from BEFORE_ENTRY)  */
    input = *(me->buffer._input_p);

__QUEX_IF_COUNT_COLUMNS(reference_p = (me->buffer._input_p));

_9:
    __quex_debug("Init State\n");
    __quex_debug_state(801);
if     ( input >= 0xB )  goto _3;
else if( input == 0xA )  goto _4;
else if( input == 0x9 )  goto _2;
else                     goto _3;


    __quex_assert_no_passage();
_8:
    /* (801 from 805)  */
    goto _9;


    __quex_assert_no_passage();
_6:
    /* (DROP_OUT from 803)  */
    goto _11;

    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_5:
    /* (DROP_OUT from 802)  */
    goto _12;


    __quex_assert_no_passage();
_7:
    /* (DROP_OUT from 804)  */
    goto _13;


    __quex_assert_no_passage();
_2:
    /* (802 from 801)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(802);
goto _5;


    __quex_assert_no_passage();
_3:
    /* (803 from 801)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(803);
goto _6;


    __quex_assert_no_passage();
_4:
    /* (804 from 801)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(804);
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
#   define QUEX_LABEL_STATE_ROUTER _435
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
_434:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (660 from BEFORE_ENTRY)  */
_224:
    input = *(me->buffer._input_p);


    __quex_debug("Init State\n");
    __quex_debug_state(660);
switch( input ) {
case 0x0: goto _141;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
case 0x22: goto _7;
case 0x2A: goto _16;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: goto _6;
case 0x3A: goto _17;
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: goto _6;
case 0x61: goto _8;
case 0x62: goto _15;
case 0x63: case 0x64: goto _6;
case 0x65: goto _11;
case 0x66: goto _6;
case 0x68: goto _4;
case 0x6E: goto _9;
case 0x6F: goto _18;
case 0x72: goto _10;
case 0x73: goto _19;
case 0x76: goto _5;
case 0x77: goto _12;
case 0x7B: goto _20;
case 0x7D: goto _14;
default: goto _105;
}


    __quex_assert_no_passage();
_139:
    /* (660 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    goto _224;


    __quex_assert_no_passage();
_105:
    /* (DROP_OUT from 738) (DROP_OUT from 733) (DROP_OUT from 748) (DROP_OUT from 730) (DROP_OUT from 711) (DROP_OUT from 693) (DROP_OUT from 708) (DROP_OUT from 705) (DROP_OUT from 703) (DROP_OUT from 700) (DROP_OUT from 697) (DROP_OUT from 663) (DROP_OUT from 678) (DROP_OUT from 660) (DROP_OUT from 757) (DROP_OUT from 675) (DROP_OUT from 754) (DROP_OUT from 672) (DROP_OUT from 685) (DROP_OUT from 667) (DROP_OUT from 727) (DROP_OUT from 724) (DROP_OUT from 739) (DROP_OUT from 721) (DROP_OUT from 749) (DROP_OUT from 731) (DROP_OUT from 728) (DROP_OUT from 709) (DROP_OUT from 706) (DROP_OUT from 688) (DROP_OUT from 701) (DROP_OUT from 698) (DROP_OUT from 679) (DROP_OUT from 661) (DROP_OUT from 676) (DROP_OUT from 755) (DROP_OUT from 752) (DROP_OUT from 686) (DROP_OUT from 668) (DROP_OUT from 683) (DROP_OUT from 665) (DROP_OUT from 680) (DROP_OUT from 740) (DROP_OUT from 722) (DROP_OUT from 737) (DROP_OUT from 735) (DROP_OUT from 750) (DROP_OUT from 732) (DROP_OUT from 695) (DROP_OUT from 710) (DROP_OUT from 692) (DROP_OUT from 704) (DROP_OUT from 702) (DROP_OUT from 699) (DROP_OUT from 696) (DROP_OUT from 662) (DROP_OUT from 759) (DROP_OUT from 756) (DROP_OUT from 753) (DROP_OUT from 669) (DROP_OUT from 684) (DROP_OUT from 666) (DROP_OUT from 681) (DROP_OUT from 760) (DROP_OUT from 726) (DROP_OUT from 741) (DROP_OUT from 723)  */

        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _227;
_225:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_132:
    /* (DROP_OUT from 743)  */
    goto _228;


    __quex_assert_no_passage();
_119:
    /* (DROP_OUT from 712)  */
    goto _229;


    __quex_assert_no_passage();
_112:
    /* (DROP_OUT from 682)  */
    goto _230;


    __quex_assert_no_passage();
_108:
    /* (DROP_OUT from 671)  */
    goto _232;


    __quex_assert_no_passage();
_137:
    /* (DROP_OUT from 758)  */
    goto _233;


    __quex_assert_no_passage();
_129:
    /* (DROP_OUT from 729)  */
    goto _234;


    __quex_assert_no_passage();
_116:
    /* (DROP_OUT from 691)  */
    goto _235;


    __quex_assert_no_passage();
_109:
    /* (DROP_OUT from 673)  */
    goto _236;


    __quex_assert_no_passage();
_138:
    /* (DROP_OUT from 761)  */
    goto _237;


    __quex_assert_no_passage();
_127:
    /* (DROP_OUT from 720)  */
    goto _238;


    __quex_assert_no_passage();
_130:
    /* (DROP_OUT from 734)  */
    goto _239;


    __quex_assert_no_passage();
_117:
    /* (DROP_OUT from 694)  */
    goto _240;


    __quex_assert_no_passage();
_110:
    /* (DROP_OUT from 674)  */
    goto _241;


    __quex_assert_no_passage();
_135:
    /* (DROP_OUT from 747)  */
    goto _242;


    __quex_assert_no_passage();
_113:
    /* (DROP_OUT from 687)  */
    goto _243;


    __quex_assert_no_passage();
_128:
    /* (DROP_OUT from 725)  */
    goto _244;


    __quex_assert_no_passage();
_106:
    /* (DROP_OUT from 664)  */
    goto _245;


    __quex_assert_no_passage();
_131:
    /* (DROP_OUT from 742)  */
    goto _246;


    __quex_assert_no_passage();
_118:
    /* (DROP_OUT from 707)  */
    goto _247;


    __quex_assert_no_passage();
_111:
    /* (DROP_OUT from 677)  */
    goto _248;


    __quex_assert_no_passage();
_136:
    /* (DROP_OUT from 751)  */
    goto _249;


    __quex_assert_no_passage();
_114:
    /* (DROP_OUT from 736) (DROP_OUT from 689)  */
    goto _231;


    __quex_assert_no_passage();
_107:
    /* (DROP_OUT from 670)  */
    goto _250;


    __quex_assert_no_passage();
_125:
    /* (DROP_OUT from 718)  */
        me->buffer._input_p -= 6;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_121:
    /* (DROP_OUT from 714)  */
        me->buffer._input_p -= 2;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_115:
    /* (DROP_OUT from 690) (DROP_OUT from 744)  */
        me->buffer._input_p -= 1;
goto _231;
    goto _225;


    __quex_assert_no_passage();
_133:
    /* (DROP_OUT from 745)  */
        me->buffer._input_p -= 2;
goto _231;
    goto _225;


    __quex_assert_no_passage();
_126:
    /* (DROP_OUT from 719)  */
        me->buffer._input_p -= 7;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_120:
    /* (DROP_OUT from 713)  */
        me->buffer._input_p -= 1;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_122:
    /* (DROP_OUT from 715)  */
        me->buffer._input_p -= 3;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_134:
    /* (DROP_OUT from 746)  */
        me->buffer._input_p -= 3;
goto _231;
    goto _225;


    __quex_assert_no_passage();
_123:
    /* (DROP_OUT from 716)  */
        me->buffer._input_p -= 4;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_124:
    /* (DROP_OUT from 717)  */
        me->buffer._input_p -= 5;
goto _229;
    goto _225;


    __quex_assert_no_passage();
_4:
    /* (661 from 660) (661 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(661);
if     ( input == 0x61 )  goto _102;
else if( input == 0x0 )   goto _142;
else                      goto _105;


    __quex_assert_no_passage();
_5:
    /* (662 from RELOAD_FORWARD) (662 from 660)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(662);
if     ( input == 0x69 )  goto _95;
else if( input == 0x0 )   goto _143;
else                      goto _105;


    __quex_assert_no_passage();
_6:
    /* (663 from 660) (663 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(663);
switch( input ) {
case 0x0: goto _144;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _32;
default: goto _105;
}


    __quex_assert_no_passage();
_7:
    /* (664 from 660)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(664);
goto _106;


    __quex_assert_no_passage();
_8:
    /* (665 from RELOAD_FORWARD) (665 from 660)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(665);
switch( input ) {
case 0x0: goto _145;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: goto _32;
case 0x63: goto _79;
case 0x64: case 0x65: case 0x66: goto _32;
case 0x6C: goto _80;
case 0x6E: goto _78;
default: goto _105;
}


    __quex_assert_no_passage();
_9:
    /* (666 from 660) (666 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(666);
switch( input ) {
case 0x0: goto _146;
case 0x61: goto _70;
case 0x6F: goto _69;
default: goto _105;
}


    __quex_assert_no_passage();
_10:
    /* (667 from RELOAD_FORWARD) (667 from 660)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(667);
if     ( input == 0x65 )  goto _64;
else if( input == 0x0 )   goto _147;
else                      goto _105;


    __quex_assert_no_passage();
_11:
    /* (668 from 660) (668 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(668);
switch( input ) {
case 0x0: goto _148;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _32;
case 0x71: goto _51;
default: goto _105;
}


    __quex_assert_no_passage();
_12:
    /* (669 from 660) (669 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(669);
if     ( input == 0x69 )  goto _38;
else if( input == 0x0 )   goto _149;
else                      goto _105;


    __quex_assert_no_passage();
_13:
    /* (670 from RELOAD_FORWARD) (670 from 660) (670 from 670)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(670);
switch( input ) {
case 0x0: goto _150;
case 0x9: case 0xA: 
case 0xD: 
case 0x20: goto _13;
default: goto _107;
}


    __quex_assert_no_passage();
_14:
    /* (671 from 660)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(671);
goto _108;


    __quex_assert_no_passage();
_15:
    /* (672 from RELOAD_FORWARD) (672 from 660)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(672);
switch( input ) {
case 0x0: goto _151;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _32;
case 0x6C: goto _31;
default: goto _105;
}


    __quex_assert_no_passage();
_16:
    /* (673 from 660)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(673);
goto _109;


    __quex_assert_no_passage();
_17:
    /* (674 from 660)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(674);
goto _110;


    __quex_assert_no_passage();
_18:
    /* (675 from RELOAD_FORWARD) (675 from 660)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(675);
if     ( input == 0x6E )  goto _26;
else if( input == 0x0 )   goto _152;
else                      goto _105;


    __quex_assert_no_passage();
_19:
    /* (676 from 660) (676 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(676);
if     ( input == 0x65 )  goto _21;
else if( input == 0x0 )   goto _153;
else                      goto _105;


    __quex_assert_no_passage();
_20:
    /* (677 from 660)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(677);
goto _111;


    __quex_assert_no_passage();
_21:
    /* (678 from RELOAD_FORWARD) (678 from 676)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(678);
if     ( input == 0x72 )  goto _22;
else if( input == 0x0 )   goto _154;
else                      goto _105;


    __quex_assert_no_passage();
_22:
    /* (679 from 678) (679 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(679);
if     ( input == 0x69 )  goto _23;
else if( input == 0x0 )   goto _155;
else                      goto _105;


    __quex_assert_no_passage();
_23:
    /* (680 from RELOAD_FORWARD) (680 from 679)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(680);
if     ( input == 0x61 )  goto _24;
else if( input == 0x0 )   goto _156;
else                      goto _105;


    __quex_assert_no_passage();
_24:
    /* (681 from 680) (681 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(681);
if     ( input == 0x6C )  goto _25;
else if( input == 0x0 )   goto _157;
else                      goto _105;


    __quex_assert_no_passage();
_25:
    /* (682 from 681)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(682);
goto _112;


    __quex_assert_no_passage();
_26:
    /* (683 from 675) (683 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(683);
if     ( input == 0x65 )  goto _27;
else if( input == 0x0 )   goto _158;
else                      goto _105;


    __quex_assert_no_passage();
_27:
    /* (684 from 683) (684 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(684);
if     ( input == 0x2D )  goto _28;
else if( input == 0x0 )   goto _159;
else                      goto _105;


    __quex_assert_no_passage();
_28:
    /* (685 from 684) (685 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(685);
if     ( input == 0x6F )  goto _29;
else if( input == 0x0 )   goto _160;
else                      goto _105;


    __quex_assert_no_passage();
_29:
    /* (686 from RELOAD_FORWARD) (686 from 685)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(686);
if     ( input == 0x66 )  goto _30;
else if( input == 0x0 )   goto _161;
else                      goto _105;


    __quex_assert_no_passage();
_30:
    /* (687 from 686)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(687);
goto _113;


    __quex_assert_no_passage();
_31:
    /* (688 from RELOAD_FORWARD) (688 from 672)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(688);
if     ( input == 0x6F )  goto _35;
else if( input == 0x0 )   goto _162;
else                      goto _105;


    __quex_assert_no_passage();
_32:
    /* (689 from 672) (689 from 665) (689 from RELOAD_FORWARD) (689 from 668) (689 from 663)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(689);
switch( input ) {
case 0x0: goto _163;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _33;
default: goto _114;
}


    __quex_assert_no_passage();
_33:
    /* (690 from RELOAD_FORWARD) (690 from 689) (690 from 736)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(690);
switch( input ) {
case 0x0: goto _164;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _34;
default: goto _115;
}


    __quex_assert_no_passage();
_34:
    /* (691 from 690)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(691);
goto _116;


    __quex_assert_no_passage();
_35:
    /* (692 from RELOAD_FORWARD) (692 from 688)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(692);
if     ( input == 0x63 )  goto _36;
else if( input == 0x0 )   goto _165;
else                      goto _105;


    __quex_assert_no_passage();
_36:
    /* (693 from 692) (693 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(693);
if     ( input == 0x6B )  goto _37;
else if( input == 0x0 )   goto _166;
else                      goto _105;


    __quex_assert_no_passage();
_37:
    /* (694 from 693)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(694);
goto _117;


    __quex_assert_no_passage();
_38:
    /* (695 from 669) (695 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(695);
if     ( input == 0x74 )  goto _39;
else if( input == 0x0 )   goto _167;
else                      goto _105;


    __quex_assert_no_passage();
_39:
    /* (696 from RELOAD_FORWARD) (696 from 695)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(696);
if     ( input == 0x68 )  goto _40;
else if( input == 0x0 )   goto _168;
else                      goto _105;


    __quex_assert_no_passage();
_40:
    /* (697 from 696) (697 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(697);
if     ( input == 0x2D )  goto _41;
else if( input == 0x0 )   goto _169;
else                      goto _105;


    __quex_assert_no_passage();
_41:
    /* (698 from RELOAD_FORWARD) (698 from 697)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(698);
if     ( input == 0x69 )  goto _42;
else if( input == 0x0 )   goto _170;
else                      goto _105;


    __quex_assert_no_passage();
_42:
    /* (699 from 698) (699 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(699);
if     ( input == 0x6E )  goto _43;
else if( input == 0x0 )   goto _171;
else                      goto _105;


    __quex_assert_no_passage();
_43:
    /* (700 from 699) (700 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(700);
if     ( input == 0x74 )  goto _44;
else if( input == 0x0 )   goto _172;
else                      goto _105;


    __quex_assert_no_passage();
_44:
    /* (701 from 700) (701 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(701);
if     ( input == 0x65 )  goto _45;
else if( input == 0x0 )   goto _173;
else                      goto _105;


    __quex_assert_no_passage();
_45:
    /* (702 from RELOAD_FORWARD) (702 from 701)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(702);
if     ( input == 0x72 )  goto _46;
else if( input == 0x0 )   goto _174;
else                      goto _105;


    __quex_assert_no_passage();
_46:
    /* (703 from 702) (703 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(703);
if     ( input == 0x66 )  goto _47;
else if( input == 0x0 )   goto _175;
else                      goto _105;


    __quex_assert_no_passage();
_47:
    /* (704 from RELOAD_FORWARD) (704 from 703)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(704);
if     ( input == 0x61 )  goto _48;
else if( input == 0x0 )   goto _176;
else                      goto _105;


    __quex_assert_no_passage();
_48:
    /* (705 from 704) (705 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(705);
if     ( input == 0x63 )  goto _49;
else if( input == 0x0 )   goto _177;
else                      goto _105;


    __quex_assert_no_passage();
_49:
    /* (706 from RELOAD_FORWARD) (706 from 705)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(706);
if     ( input == 0x65 )  goto _50;
else if( input == 0x0 )   goto _178;
else                      goto _105;


    __quex_assert_no_passage();
_50:
    /* (707 from 706)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(707);
goto _118;


    __quex_assert_no_passage();
_51:
    /* (708 from 668) (708 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(708);
if     ( input == 0x75 )  goto _52;
else if( input == 0x0 )   goto _179;
else                      goto _105;


    __quex_assert_no_passage();
_52:
    /* (709 from 708) (709 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(709);
if     ( input == 0x61 )  goto _53;
else if( input == 0x0 )   goto _180;
else                      goto _105;


    __quex_assert_no_passage();
_53:
    /* (710 from RELOAD_FORWARD) (710 from 709)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(710);
if     ( input == 0x6C )  goto _54;
else if( input == 0x0 )   goto _181;
else                      goto _105;


    __quex_assert_no_passage();
_54:
    /* (711 from 710) (711 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(711);
if     ( input == 0x73 )  goto _55;
else if( input == 0x0 )   goto _182;
else                      goto _105;


    __quex_assert_no_passage();
_55:
    /* (712 from RELOAD_FORWARD) (712 from 711)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(712);
if     ( input == 0x2D )  goto _56;
else if( input == 0x0 )   goto _183;
else                      goto _119;


    __quex_assert_no_passage();
_56:
    /* (713 from 712) (713 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(713);
if     ( input == 0x6F )  goto _57;
else if( input == 0x0 )   goto _184;
else                      goto _120;


    __quex_assert_no_passage();
_57:
    /* (714 from RELOAD_FORWARD) (714 from 713)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(714);
if     ( input == 0x72 )  goto _58;
else if( input == 0x0 )   goto _185;
else                      goto _121;


    __quex_assert_no_passage();
_58:
    /* (715 from 714) (715 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(715);
if     ( input == 0x64 )  goto _59;
else if( input == 0x0 )   goto _186;
else                      goto _122;


    __quex_assert_no_passage();
_59:
    /* (716 from 715) (716 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(716);
if     ( input == 0x65 )  goto _60;
else if( input == 0x0 )   goto _187;
else                      goto _123;


    __quex_assert_no_passage();
_60:
    /* (717 from 716) (717 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(717);
if     ( input == 0x72 )  goto _61;
else if( input == 0x0 )   goto _188;
else                      goto _124;


    __quex_assert_no_passage();
_61:
    /* (718 from RELOAD_FORWARD) (718 from 717)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(718);
if     ( input == 0x65 )  goto _62;
else if( input == 0x0 )   goto _189;
else                      goto _125;


    __quex_assert_no_passage();
_62:
    /* (719 from 718) (719 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(719);
if     ( input == 0x64 )  goto _63;
else if( input == 0x0 )   goto _190;
else                      goto _126;


    __quex_assert_no_passage();
_63:
    /* (720 from 719)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(720);
goto _127;


    __quex_assert_no_passage();
_64:
    /* (721 from RELOAD_FORWARD) (721 from 667)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(721);
if     ( input == 0x6A )  goto _65;
else if( input == 0x0 )   goto _191;
else                      goto _105;


    __quex_assert_no_passage();
_65:
    /* (722 from RELOAD_FORWARD) (722 from 721)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(722);
if     ( input == 0x65 )  goto _66;
else if( input == 0x0 )   goto _192;
else                      goto _105;


    __quex_assert_no_passage();
_66:
    /* (723 from 722) (723 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(723);
if     ( input == 0x63 )  goto _67;
else if( input == 0x0 )   goto _193;
else                      goto _105;


    __quex_assert_no_passage();
_67:
    /* (724 from 723) (724 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(724);
if     ( input == 0x74 )  goto _68;
else if( input == 0x0 )   goto _194;
else                      goto _105;


    __quex_assert_no_passage();
_68:
    /* (725 from 724)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(725);
goto _128;


    __quex_assert_no_passage();
_69:
    /* (726 from RELOAD_FORWARD) (726 from 666)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(726);
if     ( input == 0x6E )  goto _73;
else if( input == 0x0 )   goto _195;
else                      goto _105;


    __quex_assert_no_passage();
_70:
    /* (727 from 666) (727 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(727);
if     ( input == 0x6D )  goto _71;
else if( input == 0x0 )   goto _196;
else                      goto _105;


    __quex_assert_no_passage();
_71:
    /* (728 from RELOAD_FORWARD) (728 from 727)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(728);
if     ( input == 0x65 )  goto _72;
else if( input == 0x0 )   goto _197;
else                      goto _105;


    __quex_assert_no_passage();
_72:
    /* (729 from 728)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(729);
goto _129;


    __quex_assert_no_passage();
_73:
    /* (730 from 726) (730 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(730);
if     ( input == 0x65 )  goto _74;
else if( input == 0x0 )   goto _198;
else                      goto _105;


    __quex_assert_no_passage();
_74:
    /* (731 from 730) (731 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(731);
if     ( input == 0x2D )  goto _75;
else if( input == 0x0 )   goto _199;
else                      goto _105;


    __quex_assert_no_passage();
_75:
    /* (732 from 731) (732 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(732);
if     ( input == 0x6F )  goto _76;
else if( input == 0x0 )   goto _200;
else                      goto _105;


    __quex_assert_no_passage();
_76:
    /* (733 from 732) (733 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(733);
if     ( input == 0x66 )  goto _77;
else if( input == 0x0 )   goto _201;
else                      goto _105;


    __quex_assert_no_passage();
_77:
    /* (734 from 733)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(734);
goto _130;


    __quex_assert_no_passage();
_78:
    /* (735 from RELOAD_FORWARD) (735 from 665)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(735);
if     ( input == 0x79 )  goto _91;
else if( input == 0x0 )   goto _202;
else                      goto _105;


    __quex_assert_no_passage();
_79:
    /* (736 from 665) (736 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(736);
switch( input ) {
case 0x0: goto _203;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _33;
case 0x74: goto _87;
default: goto _114;
}


    __quex_assert_no_passage();
_80:
    /* (737 from 665) (737 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(737);
if     ( input == 0x6C )  goto _81;
else if( input == 0x0 )   goto _204;
else                      goto _105;


    __quex_assert_no_passage();
_81:
    /* (738 from RELOAD_FORWARD) (738 from 737)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(738);
switch( input ) {
case 0x0: goto _205;
case 0x2D: goto _83;
case 0x6F: goto _82;
default: goto _105;
}


    __quex_assert_no_passage();
_82:
    /* (739 from 738) (739 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(739);
if     ( input == 0x77 )  goto _86;
else if( input == 0x0 )   goto _206;
else                      goto _105;


    __quex_assert_no_passage();
_83:
    /* (740 from 738) (740 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(740);
if     ( input == 0x6F )  goto _84;
else if( input == 0x0 )   goto _207;
else                      goto _105;


    __quex_assert_no_passage();
_84:
    /* (741 from 740) (741 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(741);
if     ( input == 0x66 )  goto _85;
else if( input == 0x0 )   goto _208;
else                      goto _105;


    __quex_assert_no_passage();
_85:
    /* (742 from 741)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(742);
goto _131;


    __quex_assert_no_passage();
_86:
    /* (743 from 739)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(743);
goto _132;


    __quex_assert_no_passage();
_87:
    /* (744 from RELOAD_FORWARD) (744 from 736)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(744);
if     ( input == 0x69 )  goto _88;
else if( input == 0x0 )   goto _209;
else                      goto _115;


    __quex_assert_no_passage();
_88:
    /* (745 from 744) (745 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(745);
if     ( input == 0x6F )  goto _89;
else if( input == 0x0 )   goto _210;
else                      goto _133;


    __quex_assert_no_passage();
_89:
    /* (746 from RELOAD_FORWARD) (746 from 745)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(746);
if     ( input == 0x6E )  goto _90;
else if( input == 0x0 )   goto _211;
else                      goto _134;


    __quex_assert_no_passage();
_90:
    /* (747 from 746)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(747);
goto _135;


    __quex_assert_no_passage();
_91:
    /* (748 from 735) (748 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(748);
if     ( input == 0x2D )  goto _92;
else if( input == 0x0 )   goto _212;
else                      goto _105;


    __quex_assert_no_passage();
_92:
    /* (749 from 748) (749 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(749);
if     ( input == 0x6F )  goto _93;
else if( input == 0x0 )   goto _213;
else                      goto _105;


    __quex_assert_no_passage();
_93:
    /* (750 from RELOAD_FORWARD) (750 from 749)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(750);
if     ( input == 0x66 )  goto _94;
else if( input == 0x0 )   goto _214;
else                      goto _105;


    __quex_assert_no_passage();
_94:
    /* (751 from 750)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(751);
goto _136;


    __quex_assert_no_passage();
_95:
    /* (752 from RELOAD_FORWARD) (752 from 662)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(752);
if     ( input == 0x61 )  goto _96;
else if( input == 0x0 )   goto _215;
else                      goto _105;


    __quex_assert_no_passage();
_96:
    /* (753 from 752) (753 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(753);
if     ( input == 0x2D )  goto _97;
else if( input == 0x0 )   goto _216;
else                      goto _105;


    __quex_assert_no_passage();
_97:
    /* (754 from RELOAD_FORWARD) (754 from 753)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(754);
if     ( input == 0x70 )  goto _98;
else if( input == 0x0 )   goto _217;
else                      goto _105;


    __quex_assert_no_passage();
_98:
    /* (755 from 754) (755 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(755);
if     ( input == 0x6F )  goto _99;
else if( input == 0x0 )   goto _218;
else                      goto _105;


    __quex_assert_no_passage();
_99:
    /* (756 from 755) (756 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(756);
if     ( input == 0x72 )  goto _100;
else if( input == 0x0 )   goto _219;
else                      goto _105;


    __quex_assert_no_passage();
_100:
    /* (757 from 756) (757 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(757);
if     ( input == 0x74 )  goto _101;
else if( input == 0x0 )   goto _220;
else                      goto _105;


    __quex_assert_no_passage();
_101:
    /* (758 from 757)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(758);
goto _137;


    __quex_assert_no_passage();
_102:
    /* (759 from 661) (759 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(759);
if     ( input == 0x73 )  goto _103;
else if( input == 0x0 )   goto _221;
else                      goto _105;


    __quex_assert_no_passage();
_103:
    /* (760 from RELOAD_FORWARD) (760 from 759)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(760);
if     ( input == 0x68 )  goto _104;
else if( input == 0x0 )   goto _222;
else                      goto _105;


    __quex_assert_no_passage();
_104:
    /* (761 from 760)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(761);
goto _138;

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_140:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(RULE_TOKEN_TERMINATION);


#   line 2212 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _2;
_227:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
QUEX_ERROR_EXIT("\n    Match failure in mode 'RULE'.\n"
    "    No 'on_failure' section provided for this mode.\n"
    "    Proposal: Define 'on_failure' and analyze 'Lexeme'.\n");


#   line 2228 "Lexer.cpp"

}
goto _1;
_250:
    __quex_debug("* TERMINAL [ \\t\\r\\n]+\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{


#   line 2238 "Lexer.cpp"

}
goto _0;
_236:
    __quex_debug("* TERMINAL \"*\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_ASTERISK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2251 "Lexer.cpp"

}
goto _0;
_248:
    __quex_debug("* TERMINAL \"{\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_OPEN);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2264 "Lexer.cpp"

}
goto _0;
_232:
    __quex_debug("* TERMINAL \"}\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_CURLYBRACE_CLOSE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2277 "Lexer.cpp"

}
goto _0;
_241:
    __quex_debug("* TERMINAL \":\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
self_send(RULE_TOKEN_COLON);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2290 "Lexer.cpp"

}
goto _0;
_235:
    __quex_debug("* TERMINAL [0-9a-fA-F]{4}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR4);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2304 "Lexer.cpp"

}
goto _0;
_231:
    __quex_debug("* TERMINAL [0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_HEXCHAR2);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2318 "Lexer.cpp"

}
goto _0;
_228:
    __quex_debug("* TERMINAL \"allow\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_ALLOW);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2331 "Lexer.cpp"

}
goto _0;
_240:
    __quex_debug("* TERMINAL \"block\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(5);
{
self_send(RULE_TOKEN_KEYWORD_BLOCK);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2344 "Lexer.cpp"

}
goto _0;
_244:
    __quex_debug("* TERMINAL \"reject\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_REJECT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2357 "Lexer.cpp"

}
goto _0;
_234:
    __quex_debug("* TERMINAL \"name\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_NAME);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2370 "Lexer.cpp"

}
goto _0;
_242:
    __quex_debug("* TERMINAL \"action\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_ACTION);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2383 "Lexer.cpp"

}
goto _0;
_237:
    __quex_debug("* TERMINAL \"hash\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
{
self_send(RULE_TOKEN_KEYWORD_HASH);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2396 "Lexer.cpp"

}
goto _0;
_233:
    __quex_debug("* TERMINAL \"via-port\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(8);
{
self_send(RULE_TOKEN_KEYWORD_VIAPORT);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2409 "Lexer.cpp"

}
goto _0;
_247:
    __quex_debug("* TERMINAL \"with-interface\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{
self_send(RULE_TOKEN_KEYWORD_WITHINTERFACE);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2422 "Lexer.cpp"

}
goto _0;
_230:
    __quex_debug("* TERMINAL \"serial\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
self_send(RULE_TOKEN_KEYWORD_SERIAL);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2435 "Lexer.cpp"

}
goto _0;
_246:
    __quex_debug("* TERMINAL \"all-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2449 "Lexer.cpp"

}
goto _0;
_249:
    __quex_debug("* TERMINAL \"any-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2463 "Lexer.cpp"

}
goto _0;
_243:
    __quex_debug("* TERMINAL \"one-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2477 "Lexer.cpp"

}
goto _0;
_239:
    __quex_debug("* TERMINAL \"none-of\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(7);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2491 "Lexer.cpp"

}
goto _0;
_229:
    __quex_debug("* TERMINAL \"equals\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(6);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2505 "Lexer.cpp"

}
goto _0;
_238:
    __quex_debug("* TERMINAL \"equals-ordered\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(14);
{
QUEX_NAME_TOKEN(take_text)(self_write_token_p(), &self, self.buffer._lexeme_start_p, self.buffer._input_p);
self_send(RULE_TOKEN_SET_OPERATOR);
QUEX_SETTING_AFTER_SEND_CONTINUE_OR_RETURN();


#   line 2519 "Lexer.cpp"

}
goto _0;
_245:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self_send(RULE_TOKEN_DQ_STRING_BEGIN);
    self << DQ_STRING_READER;
    RETURN;


#   line 2533 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_435:
    switch( target_state_index ) {
        case 4: { goto _4; }
        case 5: { goto _5; }
        case 6: { goto _6; }
        case 8: { goto _8; }
        case 9: { goto _9; }
        case 10: { goto _10; }
        case 11: { goto _11; }
        case 12: { goto _12; }
        case 13: { goto _13; }
        case 15: { goto _15; }
        case 18: { goto _18; }
        case 19: { goto _19; }
        case 21: { goto _21; }
        case 22: { goto _22; }
        case 23: { goto _23; }
        case 24: { goto _24; }
        case 26: { goto _26; }
        case 27: { goto _27; }
        case 28: { goto _28; }
        case 29: { goto _29; }
        case 31: { goto _31; }
        case 32: { goto _32; }
        case 33: { goto _33; }
        case 35: { goto _35; }
        case 36: { goto _36; }
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
        case 64: { goto _64; }
        case 65: { goto _65; }
        case 66: { goto _66; }
        case 67: { goto _67; }
        case 69: { goto _69; }
        case 70: { goto _70; }
        case 71: { goto _71; }
        case 73: { goto _73; }
        case 74: { goto _74; }
        case 75: { goto _75; }
        case 76: { goto _76; }
        case 78: { goto _78; }
        case 79: { goto _79; }
        case 80: { goto _80; }
        case 81: { goto _81; }
        case 82: { goto _82; }
        case 83: { goto _83; }
        case 84: { goto _84; }
        case 87: { goto _87; }
        case 88: { goto _88; }
        case 89: { goto _89; }
        case 91: { goto _91; }
        case 92: { goto _92; }
        case 93: { goto _93; }
        case 95: { goto _95; }
        case 96: { goto _96; }
        case 97: { goto _97; }
        case 98: { goto _98; }
        case 99: { goto _99; }
        case 100: { goto _100; }
        case 102: { goto _102; }
        case 103: { goto _103; }
        case 105: { goto _105; }
        case 107: { goto _107; }
        case 114: { goto _114; }
        case 115: { goto _115; }
        case 119: { goto _119; }
        case 120: { goto _120; }
        case 121: { goto _121; }
        case 122: { goto _122; }
        case 123: { goto _123; }
        case 124: { goto _124; }
        case 125: { goto _125; }
        case 126: { goto _126; }
        case 133: { goto _133; }
        case 134: { goto _134; }
        case 139: { goto _139; }
        case 140: { goto _140; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_175:
    /* (RELOAD_FORWARD from 703)  */
    target_state_index = QUEX_LABEL(46); target_state_else_index = QUEX_LABEL(105);

_433:
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
_162:
    /* (RELOAD_FORWARD from 688)  */
    target_state_index = QUEX_LABEL(31); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_144:
    /* (RELOAD_FORWARD from 663)  */
    target_state_index = QUEX_LABEL(6); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_198:
    /* (RELOAD_FORWARD from 730)  */
    target_state_index = QUEX_LABEL(73); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_167:
    /* (RELOAD_FORWARD from 695)  */
    target_state_index = QUEX_LABEL(38); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_218:
    /* (RELOAD_FORWARD from 755)  */
    target_state_index = QUEX_LABEL(98); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_189:
    /* (RELOAD_FORWARD from 718)  */
    target_state_index = QUEX_LABEL(61); target_state_else_index = QUEX_LABEL(125);

    goto _433;


    __quex_assert_no_passage();
_187:
    /* (RELOAD_FORWARD from 716)  */
    target_state_index = QUEX_LABEL(59); target_state_else_index = QUEX_LABEL(123);

    goto _433;


    __quex_assert_no_passage();
_158:
    /* (RELOAD_FORWARD from 683)  */
    target_state_index = QUEX_LABEL(26); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_209:
    /* (RELOAD_FORWARD from 744)  */
    target_state_index = QUEX_LABEL(87); target_state_else_index = QUEX_LABEL(115);

    goto _433;


    __quex_assert_no_passage();
_156:
    /* (RELOAD_FORWARD from 680)  */
    target_state_index = QUEX_LABEL(23); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_178:
    /* (RELOAD_FORWARD from 706)  */
    target_state_index = QUEX_LABEL(49); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_147:
    /* (RELOAD_FORWARD from 667)  */
    target_state_index = QUEX_LABEL(10); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_214:
    /* (RELOAD_FORWARD from 750)  */
    target_state_index = QUEX_LABEL(93); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_201:
    /* (RELOAD_FORWARD from 733)  */
    target_state_index = QUEX_LABEL(76); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_183:
    /* (RELOAD_FORWARD from 712)  */
    target_state_index = QUEX_LABEL(55); target_state_else_index = QUEX_LABEL(119);

    goto _433;


    __quex_assert_no_passage();
_170:
    /* (RELOAD_FORWARD from 698)  */
    target_state_index = QUEX_LABEL(41); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_205:
    /* (RELOAD_FORWARD from 738)  */
    target_state_index = QUEX_LABEL(81); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_152:
    /* (RELOAD_FORWARD from 675)  */
    target_state_index = QUEX_LABEL(18); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_174:
    /* (RELOAD_FORWARD from 702)  */
    target_state_index = QUEX_LABEL(45); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_161:
    /* (RELOAD_FORWARD from 686)  */
    target_state_index = QUEX_LABEL(29); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_143:
    /* (RELOAD_FORWARD from 662)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_197:
    /* (RELOAD_FORWARD from 728)  */
    target_state_index = QUEX_LABEL(71); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_166:
    /* (RELOAD_FORWARD from 693)  */
    target_state_index = QUEX_LABEL(36); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_217:
    /* (RELOAD_FORWARD from 754)  */
    target_state_index = QUEX_LABEL(97); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_196:
    /* (RELOAD_FORWARD from 727)  */
    target_state_index = QUEX_LABEL(70); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_186:
    /* (RELOAD_FORWARD from 715)  */
    target_state_index = QUEX_LABEL(58); target_state_else_index = QUEX_LABEL(122);

    goto _433;


    __quex_assert_no_passage();
_157:
    /* (RELOAD_FORWARD from 681)  */
    target_state_index = QUEX_LABEL(24); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_155:
    /* (RELOAD_FORWARD from 679)  */
    target_state_index = QUEX_LABEL(22); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_177:
    /* (RELOAD_FORWARD from 705)  */
    target_state_index = QUEX_LABEL(48); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_146:
    /* (RELOAD_FORWARD from 666)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_213:
    /* (RELOAD_FORWARD from 749)  */
    target_state_index = QUEX_LABEL(92); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_192:
    /* (RELOAD_FORWARD from 722)  */
    target_state_index = QUEX_LABEL(65); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_182:
    /* (RELOAD_FORWARD from 711)  */
    target_state_index = QUEX_LABEL(54); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_169:
    /* (RELOAD_FORWARD from 697)  */
    target_state_index = QUEX_LABEL(40); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_212:
    /* (RELOAD_FORWARD from 748)  */
    target_state_index = QUEX_LABEL(91); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_151:
    /* (RELOAD_FORWARD from 672)  */
    target_state_index = QUEX_LABEL(15); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_173:
    /* (RELOAD_FORWARD from 701)  */
    target_state_index = QUEX_LABEL(44); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_142:
    /* (RELOAD_FORWARD from 661)  */
    target_state_index = QUEX_LABEL(4); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_204:
    /* (RELOAD_FORWARD from 737)  */
    target_state_index = QUEX_LABEL(80); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_165:
    /* (RELOAD_FORWARD from 692)  */
    target_state_index = QUEX_LABEL(35); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_208:
    /* (RELOAD_FORWARD from 741)  */
    target_state_index = QUEX_LABEL(84); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_195:
    /* (RELOAD_FORWARD from 726)  */
    target_state_index = QUEX_LABEL(69); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_185:
    /* (RELOAD_FORWARD from 714)  */
    target_state_index = QUEX_LABEL(57); target_state_else_index = QUEX_LABEL(121);

    goto _433;


    __quex_assert_no_passage();
_164:
    /* (RELOAD_FORWARD from 690)  */
    target_state_index = QUEX_LABEL(33); target_state_else_index = QUEX_LABEL(115);

    goto _433;


    __quex_assert_no_passage();
_154:
    /* (RELOAD_FORWARD from 678)  */
    target_state_index = QUEX_LABEL(21); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_200:
    /* (RELOAD_FORWARD from 732)  */
    target_state_index = QUEX_LABEL(75); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_222:
    /* (RELOAD_FORWARD from 760)  */
    target_state_index = QUEX_LABEL(103); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_145:
    /* (RELOAD_FORWARD from 665)  */
    target_state_index = QUEX_LABEL(8); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_220:
    /* (RELOAD_FORWARD from 757)  */
    target_state_index = QUEX_LABEL(100); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_191:
    /* (RELOAD_FORWARD from 721)  */
    target_state_index = QUEX_LABEL(64); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_181:
    /* (RELOAD_FORWARD from 710)  */
    target_state_index = QUEX_LABEL(53); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_160:
    /* (RELOAD_FORWARD from 685)  */
    target_state_index = QUEX_LABEL(28); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_211:
    /* (RELOAD_FORWARD from 746)  */
    target_state_index = QUEX_LABEL(89); target_state_else_index = QUEX_LABEL(134);

    goto _433;


    __quex_assert_no_passage();
_150:
    /* (RELOAD_FORWARD from 670)  */
    target_state_index = QUEX_LABEL(13); target_state_else_index = QUEX_LABEL(107);

    goto _433;


    __quex_assert_no_passage();
_180:
    /* (RELOAD_FORWARD from 709)  */
    target_state_index = QUEX_LABEL(52); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_141:
    /* (RELOAD_FORWARD from 660)  */
    target_state_index = QUEX_LABEL(139); target_state_else_index = QUEX_LABEL(140);

    goto _433;


    __quex_assert_no_passage();
_216:
    /* (RELOAD_FORWARD from 753)  */
    target_state_index = QUEX_LABEL(96); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_203:
    /* (RELOAD_FORWARD from 736)  */
    target_state_index = QUEX_LABEL(79); target_state_else_index = QUEX_LABEL(114);

    goto _433;


    __quex_assert_no_passage();
_172:
    /* (RELOAD_FORWARD from 700)  */
    target_state_index = QUEX_LABEL(43); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_207:
    /* (RELOAD_FORWARD from 740)  */
    target_state_index = QUEX_LABEL(83); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_194:
    /* (RELOAD_FORWARD from 724)  */
    target_state_index = QUEX_LABEL(67); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_176:
    /* (RELOAD_FORWARD from 704)  */
    target_state_index = QUEX_LABEL(47); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_163:
    /* (RELOAD_FORWARD from 689)  */
    target_state_index = QUEX_LABEL(32); target_state_else_index = QUEX_LABEL(114);

    goto _433;


    __quex_assert_no_passage();
_153:
    /* (RELOAD_FORWARD from 676)  */
    target_state_index = QUEX_LABEL(19); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_199:
    /* (RELOAD_FORWARD from 731)  */
    target_state_index = QUEX_LABEL(74); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_221:
    /* (RELOAD_FORWARD from 759)  */
    target_state_index = QUEX_LABEL(102); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_168:
    /* (RELOAD_FORWARD from 696)  */
    target_state_index = QUEX_LABEL(39); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_219:
    /* (RELOAD_FORWARD from 756)  */
    target_state_index = QUEX_LABEL(99); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_190:
    /* (RELOAD_FORWARD from 719)  */
    target_state_index = QUEX_LABEL(62); target_state_else_index = QUEX_LABEL(126);

    goto _433;


    __quex_assert_no_passage();
_188:
    /* (RELOAD_FORWARD from 717)  */
    target_state_index = QUEX_LABEL(60); target_state_else_index = QUEX_LABEL(124);

    goto _433;


    __quex_assert_no_passage();
_159:
    /* (RELOAD_FORWARD from 684)  */
    target_state_index = QUEX_LABEL(27); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_210:
    /* (RELOAD_FORWARD from 745)  */
    target_state_index = QUEX_LABEL(88); target_state_else_index = QUEX_LABEL(133);

    goto _433;


    __quex_assert_no_passage();
_149:
    /* (RELOAD_FORWARD from 669)  */
    target_state_index = QUEX_LABEL(12); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_179:
    /* (RELOAD_FORWARD from 708)  */
    target_state_index = QUEX_LABEL(51); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_148:
    /* (RELOAD_FORWARD from 668)  */
    target_state_index = QUEX_LABEL(11); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_215:
    /* (RELOAD_FORWARD from 752)  */
    target_state_index = QUEX_LABEL(95); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_202:
    /* (RELOAD_FORWARD from 735)  */
    target_state_index = QUEX_LABEL(78); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_184:
    /* (RELOAD_FORWARD from 713)  */
    target_state_index = QUEX_LABEL(56); target_state_else_index = QUEX_LABEL(120);

    goto _433;


    __quex_assert_no_passage();
_171:
    /* (RELOAD_FORWARD from 699)  */
    target_state_index = QUEX_LABEL(42); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_206:
    /* (RELOAD_FORWARD from 739)  */
    target_state_index = QUEX_LABEL(82); target_state_else_index = QUEX_LABEL(105);

    goto _433;


    __quex_assert_no_passage();
_193:
    /* (RELOAD_FORWARD from 723)  */
    target_state_index = QUEX_LABEL(66); target_state_else_index = QUEX_LABEL(105);

    goto _433;

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


goto _434;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _2; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _1; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _435; /* in QUEX_GOTO_STATE       */
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
#   define QUEX_LABEL_STATE_ROUTER _49
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
_48:
    me->buffer._lexeme_start_p = me->buffer._input_p;
    QUEX_LEXEME_TERMINATING_ZERO_UNDO(&me->buffer);

    /* (858 from BEFORE_ENTRY)  */
_27:
    input = *(me->buffer._input_p);


    __quex_debug("Init State\n");
    __quex_debug_state(858);
switch( input ) {
case 0x0: goto _23;
case 0x1: 
case 0xA: goto _12;
case 0x22: goto _8;
case 0x5C: goto _9;
default: goto _7;
}


    __quex_assert_no_passage();
_21:
    /* (858 from RELOAD_FORWARD)  */
    ++(me->buffer._input_p);

    goto _27;


    __quex_assert_no_passage();
_13:
    /* (DROP_OUT from 859) (DROP_OUT from 861)  */

    goto _30;
_28:
    __quex_debug("Drop-Out Catcher\n");


    __quex_assert_no_passage();
_19:
    /* (DROP_OUT from 866)  */
    goto _32;


    __quex_assert_no_passage();
_17:
    /* (DROP_OUT from 864)  */
    goto _33;


    __quex_assert_no_passage();
_15:
    /* (DROP_OUT from 862)  */
    goto _34;


    __quex_assert_no_passage();
_14:
    /* (DROP_OUT from 860)  */
    goto _35;


    __quex_assert_no_passage();
_12:
    /* (DROP_OUT from 858)  */
        me->buffer._input_p = me->buffer._lexeme_start_p + 1;
goto _31;
    goto _28;


    __quex_assert_no_passage();
_18:
    /* (DROP_OUT from 865)  */
        me->buffer._input_p -= 2;
goto _30;
    goto _28;


    __quex_assert_no_passage();
_16:
    /* (DROP_OUT from 863)  */
        me->buffer._input_p -= 1;
goto _30;
    goto _28;


    __quex_assert_no_passage();
_4:
    /* (864 from 861)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(864);
goto _17;


    __quex_assert_no_passage();
_5:
    /* (865 from RELOAD_FORWARD) (865 from 863)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(865);
switch( input ) {
case 0x0: goto _20;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _6;
default: goto _18;
}


    __quex_assert_no_passage();
_6:
    /* (866 from 865)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(866);
goto _19;


    __quex_assert_no_passage();
_7:
    /* (859 from 858)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(859);
goto _13;


    __quex_assert_no_passage();
_8:
    /* (860 from 858)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(860);
goto _14;


    __quex_assert_no_passage();
_9:
    /* (861 from RELOAD_FORWARD) (861 from 858)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(861);
switch( input ) {
case 0x0: goto _24;
case 0x22: goto _4;
case 0x5C: goto _10;
case 0x78: goto _11;
default: goto _13;
}


    __quex_assert_no_passage();
_10:
    /* (862 from 861)  */
    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(862);
goto _15;


    __quex_assert_no_passage();
_11:
    /* (863 from 861) (863 from RELOAD_FORWARD)  */

    ++(me->buffer._input_p);

    input = *(me->buffer._input_p);


    __quex_debug_state(863);
switch( input ) {
case 0x0: goto _25;
case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35: case 0x36: case 0x37: 
case 0x38: case 0x39: 
case 0x41: case 0x42: case 0x43: case 0x44: case 0x45: case 0x46: 
case 0x61: case 0x62: case 0x63: case 0x64: case 0x65: case 0x66: goto _5;
default: goto _16;
}

    /* (*) Terminal states _______________________________________________________
     *
     * States that implement actions of the 'winner patterns.                     */
_22:
    __quex_debug("* TERMINAL END_OF_STREAM\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
self_send(__QUEX_SETTING_TOKEN_ID_TERMINATION);


#   line 3699 "Lexer.cpp"

}
    /* End of Stream FORCES a return from the lexical analyzer, so that no
     * tokens can be filled after the termination token.
     */
goto _1;
_31:
    __quex_debug("* TERMINAL FAILURE\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
{
    throw std::runtime_error("Invalid string");


#   line 3713 "Lexer.cpp"

}
goto _2;
_33:
    __quex_debug("* TERMINAL \"\\\\\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3726 "Lexer.cpp"

}
goto _0;
_34:
    __quex_debug("* TERMINAL \"\\\\\\\\\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(2);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3739 "Lexer.cpp"

}
goto _0;
_32:
    __quex_debug("* TERMINAL \"\\\\x\"[0-9a-fA-F]{2}\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(4);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3752 "Lexer.cpp"

}
goto _0;
_35:
    __quex_debug("* TERMINAL \"\\\"\"\n");
__QUEX_IF_COUNT_SHIFT_VALUES();
__QUEX_IF_COUNT_COLUMNS_ADD(1);
{
    self << RULE;
    RETURN;


#   line 3765 "Lexer.cpp"

}
goto _0;
_30:
    __quex_debug("* TERMINAL .\n");
__QUEX_COUNT_VOID(&self, LexemeBegin, LexemeEnd);
QUEX_LEXEME_TERMINATING_ZERO_SET(&me->buffer);
{
self_accumulator_add(Lexeme, LexemeEnd);


#   line 3777 "Lexer.cpp"

}
goto _0;
#   ifndef QUEX_OPTION_COMPUTED_GOTOS
    __quex_assert_no_passage();
_49:
    switch( target_state_index ) {
        case 5: { goto _5; }
        case 9: { goto _9; }
        case 11: { goto _11; }
        case 13: { goto _13; }
        case 16: { goto _16; }
        case 18: { goto _18; }
        case 21: { goto _21; }
        case 22: { goto _22; }

        default:
            __QUEX_STD_fprintf(stderr, "State router: index = %i\n", (int)target_state_index);
            QUEX_ERROR_EXIT("State router: unknown index.\n");
    }
#   endif /* QUEX_OPTION_COMPUTED_GOTOS */


    __quex_assert_no_passage();
_24:
    /* (RELOAD_FORWARD from 861)  */
    target_state_index = QUEX_LABEL(9); target_state_else_index = QUEX_LABEL(13);

_47:
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
_20:
    /* (RELOAD_FORWARD from 865)  */
    target_state_index = QUEX_LABEL(5); target_state_else_index = QUEX_LABEL(18);

    goto _47;


    __quex_assert_no_passage();
_25:
    /* (RELOAD_FORWARD from 863)  */
    target_state_index = QUEX_LABEL(11); target_state_else_index = QUEX_LABEL(16);

    goto _47;


    __quex_assert_no_passage();
_23:
    /* (RELOAD_FORWARD from 858)  */
    target_state_index = QUEX_LABEL(21); target_state_else_index = QUEX_LABEL(22);

    goto _47;

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


goto _48;

    __quex_assert_no_passage();

    /* Following labels are referenced in macros. It cannot be detected
     * whether the macros are applied in user code or not. To avoid compiler.
     * warnings of unused labels, they are referenced in unreachable code.   */
    goto _1; /* in RETURN                */
    goto _0; /* in CONTINUE              */
    goto _2; /* in CONTINUE and skippers */
#   if ! defined(QUEX_OPTION_COMPUTED_GOTOS)
    goto _49; /* in QUEX_GOTO_STATE       */
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

