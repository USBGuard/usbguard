/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 1 "Parser.y"

#line 30 "Parser.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    RuleParserTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is RuleParserTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    RuleParserARG_SDECL     A static variable declaration for the %extra_argument
**    RuleParserARG_PDECL     A parameter declaration for the %extra_argument
**    RuleParserARG_STORE     Code to store %extra_argument into yypParser
**    RuleParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 51
#define YYACTIONTYPE unsigned char
#define RuleParserTOKENTYPE  quex::Token* 
typedef union {
  int yyinit;
  RuleParserTOKENTYPE yy0;
  String* yy14;
  StringVector* yy29;
  bool yy37;
  std::vector<USBInterfaceType>* yy56;
  USBInterfaceType* yy64;
  std::vector<RuleCondition*>* yy88;
  RuleCondition* yy90;
  Rule::SetOperator yy97;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define RuleParserARG_SDECL  RuleParserState* state ;
#define RuleParserARG_PDECL , RuleParserState* state 
#define RuleParserARG_FETCH  RuleParserState* state  = yypParser->state 
#define RuleParserARG_STORE yypParser->state  = state 
#define YYNSTATE             27
#define YYNRULE              45
#define YY_MAX_SHIFT         26
#define YY_MIN_SHIFTREDUCE   58
#define YY_MAX_SHIFTREDUCE   102
#define YY_MIN_REDUCE        103
#define YY_MAX_REDUCE        147
#define YY_ERROR_ACTION      148
#define YY_ACCEPT_ACTION     149
#define YY_NO_ACTION         150
/************* End control #defines *******************************************/

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE

**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (67)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    61,   62,   63,   64,   65,   11,   10,    9,    4,  149,
 /*    10 */    26,    7,    3,   83,   88,   85,   86,   87,   82,   16,
 /*    20 */    90,   85,   86,   87,   79,   92,  138,  100,   20,   25,
 /*    30 */    12,   70,   21,   81,   66,   67,   68,   75,   77,   17,
 /*    40 */   101,   74,   73,   96,   18,   25,   92,   76,   72,   20,
 /*    50 */     1,    8,    6,   98,    2,   13,    5,   14,  102,   20,
 /*    60 */    15,   19,   94,   22,  103,   23,   24,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     3,    4,    5,    6,    7,   11,   12,   13,   14,   42,
 /*    10 */    43,   44,   18,   17,   34,   19,   20,   21,   16,   45,
 /*    20 */    17,   19,   20,   21,   32,   22,   23,   36,   28,   38,
 /*    30 */    47,   48,   41,   34,    8,    9,   10,   32,   17,   40,
 /*    40 */    16,   32,   32,   36,   39,   38,   22,   16,   32,   28,
 /*    50 */    27,   33,   35,   49,   46,   15,   37,   15,   30,   28,
 /*    60 */    15,   29,   26,   25,    0,   24,   23,
};
#define YY_SHIFT_USE_DFLT (-7)
#define YY_SHIFT_COUNT (26)
#define YY_SHIFT_MIN   (-6)
#define YY_SHIFT_MAX   (64)
static const signed char yy_shift_ofst[] = {
 /*     0 */    -3,    3,   -6,   -4,   21,   24,    2,   26,   31,    0,
 /*    10 */     0,    0,   23,   -7,   -7,   -7,   -7,   40,   42,   28,
 /*    20 */    32,   45,   36,   38,   41,   43,   64,
};
#define YY_REDUCE_USE_DFLT (-34)
#define YY_REDUCE_COUNT (16)
#define YY_REDUCE_MIN   (-33)
#define YY_REDUCE_MAX   (19)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -33,   -9,  -17,   -1,    5,    7,  -20,  -26,   -8,    9,
 /*    10 */    10,   16,    4,   17,   18,   19,    8,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   105,  136,  144,  129,  123,  138,  148,  114,  148,  148,
 /*    10 */   148,  148,  104,  134,  125,  142,  116,  148,  148,  148,
 /*    20 */   148,  148,  148,  148,  140,  148,  148,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  RuleParserARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void RuleParserTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "TERMINATION",   "UNINITIALIZED",  "KEYWORD_ALLOW",
  "KEYWORD_BLOCK",  "KEYWORD_REJECT",  "KEYWORD_MATCH",  "KEYWORD_DEVICE",
  "DEVICE_VID",    "DEVICE_VPID",   "DEVICE_ANYID",  "KEYWORD_HASH",
  "KEYWORD_NAME",  "KEYWORD_SERIAL",  "KEYWORD_VIAPORT",  "CURLYBRACE_OPEN",
  "CURLYBRACE_CLOSE",  "SET_OPERATOR",  "KEYWORD_WITHINTERFACE",  "INTERFACE_TYPE_FULL",
  "INTERFACE_TYPE_SUBCLASS",  "INTERFACE_TYPE_CLASS",  "NEGATION",      "CONDITION_IDENTIFIER",
  "PQ_STRING_BEGIN",  "PQ_STRING",     "PQ_STRING_END",  "KEYWORD_IF",  
  "DQ_STRING_BEGIN",  "DQ_STRING",     "DQ_STRING_END",  "error",       
  "string",        "stringvec",     "usbiftype",     "usbiftypevec",
  "condition",     "conditionvec",  "negation_op",   "ports_set_op",
  "usbif_set_op",  "condition_set_op",  "rule",          "rule_spec",   
  "target",        "device_id",     "device_attributes",  "conditions",  
  "device_attribute",  "condition_spec",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "rule ::= rule_spec",
 /*   1 */ "rule_spec ::= target device_id device_attributes conditions",
 /*   2 */ "rule_spec ::=",
 /*   3 */ "target ::= KEYWORD_ALLOW",
 /*   4 */ "target ::= KEYWORD_BLOCK",
 /*   5 */ "target ::= KEYWORD_REJECT",
 /*   6 */ "target ::= KEYWORD_MATCH",
 /*   7 */ "target ::= KEYWORD_DEVICE",
 /*   8 */ "device_id ::= DEVICE_VID",
 /*   9 */ "device_id ::= DEVICE_VPID",
 /*  10 */ "device_id ::= DEVICE_ANYID",
 /*  11 */ "device_id ::=",
 /*  12 */ "device_attributes ::= device_attributes device_attribute",
 /*  13 */ "device_attributes ::=",
 /*  14 */ "device_attribute ::= KEYWORD_HASH string",
 /*  15 */ "device_attribute ::= KEYWORD_NAME string",
 /*  16 */ "device_attribute ::= KEYWORD_SERIAL string",
 /*  17 */ "device_attribute ::= KEYWORD_VIAPORT string",
 /*  18 */ "device_attribute ::= KEYWORD_VIAPORT ports_set_op CURLYBRACE_OPEN stringvec CURLYBRACE_CLOSE",
 /*  19 */ "ports_set_op ::= SET_OPERATOR",
 /*  20 */ "ports_set_op ::=",
 /*  21 */ "stringvec ::= stringvec string",
 /*  22 */ "stringvec ::=",
 /*  23 */ "device_attribute ::= KEYWORD_WITHINTERFACE usbiftype",
 /*  24 */ "device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op CURLYBRACE_OPEN usbiftypevec CURLYBRACE_CLOSE",
 /*  25 */ "usbif_set_op ::= SET_OPERATOR",
 /*  26 */ "usbif_set_op ::=",
 /*  27 */ "usbiftype ::= INTERFACE_TYPE_FULL",
 /*  28 */ "usbiftype ::= INTERFACE_TYPE_SUBCLASS",
 /*  29 */ "usbiftype ::= INTERFACE_TYPE_CLASS",
 /*  30 */ "usbiftypevec ::= usbiftypevec usbiftype",
 /*  31 */ "usbiftypevec ::=",
 /*  32 */ "condition_set_op ::= SET_OPERATOR",
 /*  33 */ "condition_set_op ::=",
 /*  34 */ "negation_op ::= NEGATION",
 /*  35 */ "negation_op ::=",
 /*  36 */ "condition ::= negation_op CONDITION_IDENTIFIER PQ_STRING_BEGIN PQ_STRING PQ_STRING_END",
 /*  37 */ "condition ::= negation_op CONDITION_IDENTIFIER",
 /*  38 */ "conditionvec ::= conditionvec condition",
 /*  39 */ "conditionvec ::=",
 /*  40 */ "conditions ::= conditions condition_spec",
 /*  41 */ "conditions ::=",
 /*  42 */ "condition_spec ::= KEYWORD_IF condition",
 /*  43 */ "condition_spec ::= KEYWORD_IF condition_set_op CURLYBRACE_OPEN conditionvec CURLYBRACE_CLOSE",
 /*  44 */ "string ::= DQ_STRING_BEGIN DQ_STRING DQ_STRING_END",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to RuleParserAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to RuleParser and RuleParserFree.
*/
void *RuleParserAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  RuleParserARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
      /* TERMINAL Destructor */
    case 1: /* TERMINATION */
    case 2: /* UNINITIALIZED */
    case 3: /* KEYWORD_ALLOW */
    case 4: /* KEYWORD_BLOCK */
    case 5: /* KEYWORD_REJECT */
    case 6: /* KEYWORD_MATCH */
    case 7: /* KEYWORD_DEVICE */
    case 8: /* DEVICE_VID */
    case 9: /* DEVICE_VPID */
    case 10: /* DEVICE_ANYID */
    case 11: /* KEYWORD_HASH */
    case 12: /* KEYWORD_NAME */
    case 13: /* KEYWORD_SERIAL */
    case 14: /* KEYWORD_VIAPORT */
    case 15: /* CURLYBRACE_OPEN */
    case 16: /* CURLYBRACE_CLOSE */
    case 17: /* SET_OPERATOR */
    case 18: /* KEYWORD_WITHINTERFACE */
    case 19: /* INTERFACE_TYPE_FULL */
    case 20: /* INTERFACE_TYPE_SUBCLASS */
    case 21: /* INTERFACE_TYPE_CLASS */
    case 22: /* NEGATION */
    case 23: /* CONDITION_IDENTIFIER */
    case 24: /* PQ_STRING_BEGIN */
    case 25: /* PQ_STRING */
    case 26: /* PQ_STRING_END */
    case 27: /* KEYWORD_IF */
    case 28: /* DQ_STRING_BEGIN */
    case 29: /* DQ_STRING */
    case 30: /* DQ_STRING_END */
{
#line 11 "Parser.y"
 delete (yypminor->yy0); 
#line 523 "Parser.c"
}
      break;
      /* Default NON-TERMINAL Destructor */
    case 31: /* error */
    case 38: /* negation_op */
    case 39: /* ports_set_op */
    case 40: /* usbif_set_op */
    case 41: /* condition_set_op */
    case 42: /* rule */
    case 43: /* rule_spec */
    case 44: /* target */
    case 45: /* device_id */
    case 46: /* device_attributes */
    case 47: /* conditions */
    case 48: /* device_attribute */
    case 49: /* condition_spec */
{
#line 7 "Parser.y"
 (void)state; 
#line 543 "Parser.c"
}
      break;
    case 32: /* string */
{
#line 14 "Parser.y"
 delete (yypminor->yy14); 
#line 550 "Parser.c"
}
      break;
    case 33: /* stringvec */
{
#line 17 "Parser.y"
 delete (yypminor->yy29); 
#line 557 "Parser.c"
}
      break;
    case 34: /* usbiftype */
{
#line 20 "Parser.y"
 delete (yypminor->yy64); 
#line 564 "Parser.c"
}
      break;
    case 35: /* usbiftypevec */
{
#line 23 "Parser.y"
 delete (yypminor->yy56); 
#line 571 "Parser.c"
}
      break;
    case 36: /* condition */
{
#line 26 "Parser.y"
 delete (yypminor->yy90); 
#line 578 "Parser.c"
}
      break;
    case 37: /* conditionvec */
{
#line 29 "Parser.y"
 delete (yypminor->yy88); 
#line 585 "Parser.c"
}
      break;
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yyidx>=0 );
  yytos = &pParser->yystack[pParser->yyidx--];
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void RuleParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
#ifndef YYPARSEFREENEVERNULL
  if( pParser==0 ) return;
#endif
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int RuleParserStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    if( i==YY_SHIFT_USE_DFLT ) return yy_default[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
      if( iLookAhead>0 ){
#ifdef YYFALLBACK
        YYCODETYPE iFallback;            /* Fallback token */
        if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
               && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
          }
#endif
          assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
          iLookAhead = iFallback;
          continue;
        }
#endif
#ifdef YYWILDCARD
        {
          int j = i - iLookAhead + YYWILDCARD;
          if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
            j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
            j<YY_ACTTAB_COUNT &&
#endif
            yy_lookahead[j]==YYWILDCARD
          ){
#ifndef NDEBUG
            if( yyTraceFILE ){
              fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
                 yyTracePrompt, yyTokenName[iLookAhead],
                 yyTokenName[YYWILDCARD]);
            }
#endif /* NDEBUG */
            return yy_action[j];
          }
        }
#endif /* YYWILDCARD */
      }
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   RuleParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   RuleParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yystack[yypParser->yyidx].major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 42, 1 },
  { 43, 4 },
  { 43, 0 },
  { 44, 1 },
  { 44, 1 },
  { 44, 1 },
  { 44, 1 },
  { 44, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 0 },
  { 46, 2 },
  { 46, 0 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 2 },
  { 48, 5 },
  { 39, 1 },
  { 39, 0 },
  { 33, 2 },
  { 33, 0 },
  { 48, 2 },
  { 48, 5 },
  { 40, 1 },
  { 40, 0 },
  { 34, 1 },
  { 34, 1 },
  { 34, 1 },
  { 35, 2 },
  { 35, 0 },
  { 41, 1 },
  { 41, 0 },
  { 38, 1 },
  { 38, 0 },
  { 36, 5 },
  { 36, 2 },
  { 37, 2 },
  { 37, 0 },
  { 47, 2 },
  { 47, 0 },
  { 49, 2 },
  { 49, 5 },
  { 32, 3 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  RuleParserARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[-yysize].stateno);
  }
#endif /* NDEBUG */
  yygotominor = yyzerominor;

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      case 0: /* rule ::= rule_spec */
#line 64 "Parser.y"
{
  yy_destructor(yypParser,43,&yymsp[0].minor);
}
#line 915 "Parser.c"
        break;
      case 1: /* rule_spec ::= target device_id device_attributes conditions */
#line 66 "Parser.y"
{
  yy_destructor(yypParser,44,&yymsp[-3].minor);
  yy_destructor(yypParser,45,&yymsp[-2].minor);
  yy_destructor(yypParser,46,&yymsp[-1].minor);
  yy_destructor(yypParser,47,&yymsp[0].minor);
}
#line 925 "Parser.c"
        break;
      case 3: /* target ::= KEYWORD_ALLOW */
#line 69 "Parser.y"
{
  state->rule.setTarget(Rule::Target::Allow);
  yy_destructor(yypParser,3,&yymsp[0].minor);
}
#line 933 "Parser.c"
        break;
      case 4: /* target ::= KEYWORD_BLOCK */
#line 73 "Parser.y"
{
  state->rule.setTarget(Rule::Target::Block);
  yy_destructor(yypParser,4,&yymsp[0].minor);
}
#line 941 "Parser.c"
        break;
      case 5: /* target ::= KEYWORD_REJECT */
#line 77 "Parser.y"
{
  state->rule.setTarget(Rule::Target::Reject);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 949 "Parser.c"
        break;
      case 6: /* target ::= KEYWORD_MATCH */
#line 81 "Parser.y"
{
  state->rule.setTarget(Rule::Target::Match);
  yy_destructor(yypParser,6,&yymsp[0].minor);
}
#line 957 "Parser.c"
        break;
      case 7: /* target ::= KEYWORD_DEVICE */
#line 85 "Parser.y"
{
  state->rule.setTarget(Rule::Target::Device);
  yy_destructor(yypParser,7,&yymsp[0].minor);
}
#line 965 "Parser.c"
        break;
      case 8: /* device_id ::= DEVICE_VID */
#line 89 "Parser.y"
{ // 1234:*
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), tokens, ":", /*trim_empty=*/false);
  state->rule.setVendorID(tokens[0]);
  delete yymsp[0].minor.yy0;
}
#line 975 "Parser.c"
        break;
      case 9: /* device_id ::= DEVICE_VPID */
#line 96 "Parser.y"
{ // 1234:5678
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), tokens, ":", /*trim_empty=*/false);
  state->rule.setVendorID(tokens[0]);
  state->rule.setProductID(tokens[1]);
  delete yymsp[0].minor.yy0;
}
#line 986 "Parser.c"
        break;
      case 10: /* device_id ::= DEVICE_ANYID */
#line 104 "Parser.y"
{
  yy_destructor(yypParser,10,&yymsp[0].minor);
}
#line 993 "Parser.c"
        break;
      case 12: /* device_attributes ::= device_attributes device_attribute */
#line 107 "Parser.y"
{
  yy_destructor(yypParser,46,&yymsp[-1].minor);
  yy_destructor(yypParser,48,&yymsp[0].minor);
}
#line 1001 "Parser.c"
        break;
      case 14: /* device_attribute ::= KEYWORD_HASH string */
#line 110 "Parser.y"
{
  try {
    state->rule.setDeviceHash(*yymsp[0].minor.yy14);
    delete yymsp[0].minor.yy14;
  }
  catch(...) {
    state->error.setHint("Invalid device hash value: " + *yymsp[0].minor.yy14);
    delete yymsp[0].minor.yy14;
    throw state->error;
  }
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 1017 "Parser.c"
        break;
      case 15: /* device_attribute ::= KEYWORD_NAME string */
#line 122 "Parser.y"
{
		 state->rule.setDeviceName(*yymsp[0].minor.yy14);
		 delete yymsp[0].minor.yy14;
  yy_destructor(yypParser,12,&yymsp[-1].minor);
}
#line 1026 "Parser.c"
        break;
      case 16: /* device_attribute ::= KEYWORD_SERIAL string */
#line 127 "Parser.y"
{
		 state->rule.setSerialNumber(*yymsp[0].minor.yy14);
		 delete yymsp[0].minor.yy14;
  yy_destructor(yypParser,13,&yymsp[-1].minor);
}
#line 1035 "Parser.c"
        break;
      case 17: /* device_attribute ::= KEYWORD_VIAPORT string */
#line 132 "Parser.y"
{
		 state->rule.refDevicePorts().push_back(*yymsp[0].minor.yy14);
		 state->rule.setDevicePortsSetOperator(Rule::SetOperator::Equals);
		 delete yymsp[0].minor.yy14;
  yy_destructor(yypParser,14,&yymsp[-1].minor);
}
#line 1045 "Parser.c"
        break;
      case 18: /* device_attribute ::= KEYWORD_VIAPORT ports_set_op CURLYBRACE_OPEN stringvec CURLYBRACE_CLOSE */
#line 138 "Parser.y"
{
		 state->rule.refDevicePorts().insert(state->rule.refDevicePorts().end(), yymsp[-1].minor.yy29->begin(), yymsp[-1].minor.yy29->end());
		 state->rule.setDevicePortsSetOperator(yymsp[-3].minor.yy97);
		 delete yymsp[-1].minor.yy29;
  yy_destructor(yypParser,14,&yymsp[-4].minor);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
#line 1057 "Parser.c"
        break;
      case 19: /* ports_set_op ::= SET_OPERATOR */
#line 144 "Parser.y"
{
		 yygotominor.yy97 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
		 delete yymsp[0].minor.yy0;
}
#line 1065 "Parser.c"
        break;
      case 20: /* ports_set_op ::= */
#line 149 "Parser.y"
{
		    yygotominor.yy97 = Rule::SetOperator::OneOf;
}
#line 1072 "Parser.c"
        break;
      case 21: /* stringvec ::= stringvec string */
#line 153 "Parser.y"
{
	  yygotominor.yy29 = yymsp[-1].minor.yy29;
	  yygotominor.yy29->push_back(*yymsp[0].minor.yy14);
	  delete yymsp[0].minor.yy14;
}
#line 1081 "Parser.c"
        break;
      case 22: /* stringvec ::= */
#line 159 "Parser.y"
{
	  yygotominor.yy29 = new StringVector();
}
#line 1088 "Parser.c"
        break;
      case 23: /* device_attribute ::= KEYWORD_WITHINTERFACE usbiftype */
#line 163 "Parser.y"
{
	  state->rule.refInterfaceTypes().push_back(*yymsp[0].minor.yy64);
	  state->rule.setInterfaceTypesSetOperator(Rule::SetOperator::Equals);
	  delete yymsp[0].minor.yy64;
  yy_destructor(yypParser,18,&yymsp[-1].minor);
}
#line 1098 "Parser.c"
        break;
      case 24: /* device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op CURLYBRACE_OPEN usbiftypevec CURLYBRACE_CLOSE */
#line 169 "Parser.y"
{
	  state->rule.refInterfaceTypes().insert(state->rule.refInterfaceTypes().end(), yymsp[-1].minor.yy56->begin(), yymsp[-1].minor.yy56->end());
	  state->rule.setInterfaceTypesSetOperator(yymsp[-3].minor.yy97);
	  delete yymsp[-1].minor.yy56;
  yy_destructor(yypParser,18,&yymsp[-4].minor);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
#line 1110 "Parser.c"
        break;
      case 25: /* usbif_set_op ::= SET_OPERATOR */
#line 175 "Parser.y"
{
		    yygotominor.yy97 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
		    delete yymsp[0].minor.yy0;
}
#line 1118 "Parser.c"
        break;
      case 26: /* usbif_set_op ::= */
#line 180 "Parser.y"
{
		    yygotominor.yy97 = Rule::SetOperator::EqualsOrdered;
}
#line 1125 "Parser.c"
        break;
      case 27: /* usbiftype ::= INTERFACE_TYPE_FULL */
#line 184 "Parser.y"
{
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), tokens, ":", /*trim_empty=*/false);

  yygotominor.yy64 = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           stringToNumber<uint8_t>(tokens[1], 16),
                           stringToNumber<uint8_t>(tokens[2], 16));
  delete yymsp[0].minor.yy0;
}
#line 1138 "Parser.c"
        break;
      case 28: /* usbiftype ::= INTERFACE_TYPE_SUBCLASS */
#line 194 "Parser.y"
{
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), tokens, ":", /*trim_empty=*/false);
  yygotominor.yy64 = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           stringToNumber<uint8_t>(tokens[1], 16),
                           0,
                           USBInterfaceType::MatchClass|USBInterfaceType::MatchSubClass),
  delete yymsp[0].minor.yy0;
}
#line 1151 "Parser.c"
        break;
      case 29: /* usbiftype ::= INTERFACE_TYPE_CLASS */
#line 204 "Parser.y"
{
  StringVector tokens;
  tokenizeString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), tokens, ":", /*trim_empty=*/false);
  yygotominor.yy64 = new USBInterfaceType(stringToNumber<uint8_t>(tokens[0], 16),
                           0, 0, USBInterfaceType::MatchClass);
  delete yymsp[0].minor.yy0;
}
#line 1162 "Parser.c"
        break;
      case 30: /* usbiftypevec ::= usbiftypevec usbiftype */
#line 212 "Parser.y"
{
	yygotominor.yy56 = yymsp[-1].minor.yy56;
	yygotominor.yy56->push_back(*yymsp[0].minor.yy64);
	delete yymsp[0].minor.yy64;
}
#line 1171 "Parser.c"
        break;
      case 31: /* usbiftypevec ::= */
#line 218 "Parser.y"
{
	yygotominor.yy56 = new std::vector<USBInterfaceType>();
}
#line 1178 "Parser.c"
        break;
      case 32: /* condition_set_op ::= SET_OPERATOR */
#line 222 "Parser.y"
{
  yygotominor.yy97 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
  delete yymsp[0].minor.yy0;
}
#line 1186 "Parser.c"
        break;
      case 33: /* condition_set_op ::= */
#line 227 "Parser.y"
{
  yygotominor.yy97 = Rule::SetOperator::EqualsOrdered;
}
#line 1193 "Parser.c"
        break;
      case 34: /* negation_op ::= NEGATION */
#line 231 "Parser.y"
{
  yygotominor.yy37 = true;
  yy_destructor(yypParser,22,&yymsp[0].minor);
}
#line 1201 "Parser.c"
        break;
      case 35: /* negation_op ::= */
#line 235 "Parser.y"
{
  yygotominor.yy37 = false;
}
#line 1208 "Parser.c"
        break;
      case 36: /* condition ::= negation_op CONDITION_IDENTIFIER PQ_STRING_BEGIN PQ_STRING PQ_STRING_END */
#line 239 "Parser.y"
{
  const String identifier = quex::unicode_to_char(yymsp[-3].minor.yy0->get_text());
  const String parameter = quex::unicode_to_char(yymsp[-1].minor.yy0->get_text());
  const bool negated = yymsp[-4].minor.yy37;
  yygotominor.yy90 = RuleCondition::getImplementation(identifier, parameter, negated);
  delete yymsp[-3].minor.yy0;
  delete yymsp[-1].minor.yy0;
  yy_destructor(yypParser,24,&yymsp[-2].minor);
  yy_destructor(yypParser,26,&yymsp[0].minor);
}
#line 1222 "Parser.c"
        break;
      case 37: /* condition ::= negation_op CONDITION_IDENTIFIER */
#line 248 "Parser.y"
{
  const String identifier = quex::unicode_to_char(yymsp[0].minor.yy0->get_text());
  const String parameter;
  const bool negated = yymsp[-1].minor.yy37;
  yygotominor.yy90 = RuleCondition::getImplementation(identifier, parameter, negated);
  delete yymsp[0].minor.yy0;
}
#line 1233 "Parser.c"
        break;
      case 38: /* conditionvec ::= conditionvec condition */
#line 256 "Parser.y"
{
  yygotominor.yy88 = yymsp[-1].minor.yy88;
  yygotominor.yy88->push_back(yymsp[0].minor.yy90->clone());
  delete yymsp[0].minor.yy90;
}
#line 1242 "Parser.c"
        break;
      case 39: /* conditionvec ::= */
#line 262 "Parser.y"
{
  yygotominor.yy88 = new std::vector<RuleCondition*>();
}
#line 1249 "Parser.c"
        break;
      case 40: /* conditions ::= conditions condition_spec */
#line 266 "Parser.y"
{
  yy_destructor(yypParser,47,&yymsp[-1].minor);
  yy_destructor(yypParser,49,&yymsp[0].minor);
}
#line 1257 "Parser.c"
        break;
      case 42: /* condition_spec ::= KEYWORD_IF condition */
#line 269 "Parser.y"
{
  state->rule.internal()->refConditions().push_back(yymsp[0].minor.yy90);
  state->rule.setConditionSetOperator(Rule::SetOperator::EqualsOrdered);
  yy_destructor(yypParser,27,&yymsp[-1].minor);
}
#line 1266 "Parser.c"
        break;
      case 43: /* condition_spec ::= KEYWORD_IF condition_set_op CURLYBRACE_OPEN conditionvec CURLYBRACE_CLOSE */
#line 274 "Parser.y"
{
  state->rule.internal()->refConditions().insert(state->rule.internal()->refConditions().end(),
                                                 yymsp[-1].minor.yy88->begin(), yymsp[-1].minor.yy88->end());
  state->rule.setConditionSetOperator(yymsp[-3].minor.yy97);
  yy_destructor(yypParser,27,&yymsp[-4].minor);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
#line 1278 "Parser.c"
        break;
      case 44: /* string ::= DQ_STRING_BEGIN DQ_STRING DQ_STRING_END */
#line 280 "Parser.y"
{
	  const String from_unicode = quex::unicode_to_char(yymsp[-1].minor.yy0->get_text());
	  const String unescaped = Rule::unescapeString(from_unicode);
	  yygotominor.yy14 = new String(unescaped);
	  delete yymsp[-1].minor.yy0;
  yy_destructor(yypParser,28,&yymsp[-2].minor);
  yy_destructor(yypParser,30,&yymsp[0].minor);
}
#line 1290 "Parser.c"
        break;
      default:
      /* (2) rule_spec ::= */ yytestcase(yyruleno==2);
      /* (11) device_id ::= */ yytestcase(yyruleno==11);
      /* (13) device_attributes ::= */ yytestcase(yyruleno==13);
      /* (41) conditions ::= */ yytestcase(yyruleno==41);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno>=0 && yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact <= YY_MAX_SHIFTREDUCE ){
    if( yyact>YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
    /* If the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
      yyTraceShift(yypParser, yyact);
    }else{
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YY_ACCEPT_ACTION );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  RuleParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  RuleParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  RuleParserARG_FETCH;
#define TOKEN (yyminor.yy0)
/************ Begin %syntax_error code ****************************************/
#line 36 "Parser.y"

#ifndef NDEBUG
  std::string possible_tokens;
  const size_t n = sizeof(yyTokenName) / sizeof(yyTokenName[0]);
  for (size_t i = 0; i < n; ++i) {
    const int a = yy_find_shift_action(yypParser, (YYCODETYPE)i);
    if (a < YYNSTATE + YYNRULE) {
      if (!possible_tokens.empty()) {
        possible_tokens.append(", ");
      }
      possible_tokens.append(yyTokenName[i]);
    }
  }
  const std::string hint_prefix = "Syntax error!";
  if (!possible_tokens.empty()) {
    state->error.setHint(hint_prefix + " Possible tokens: " + possible_tokens);
  }
  else {
    state->error.setHint(hint_prefix);
  }
#else
  state->error.setHint("Syntax error!");
#endif
  throw state->error;
#line 1385 "Parser.c"
/************ End %syntax_error code ******************************************/
  RuleParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  RuleParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  RuleParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "RuleParserAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void RuleParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  RuleParserTOKENTYPE yyminor       /* The value for the token */
  RuleParserARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sInitialize. Empty stack. State 0\n",
              yyTracePrompt);
    }
#endif
  }
  yyminorunion.yy0 = yyminor;
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  RuleParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      if( yyact > YY_MAX_SHIFT ) yyact += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
#ifndef NDEBUG
  if( yyTraceFILE ){
    int i;
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE,"%c%s", i==1 ? '[' : ' ', 
              yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
