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
#define YYNOCODE 38
#define YYACTIONTYPE unsigned char
#define RuleParserTOKENTYPE  quex::Token* 
typedef union {
  int yyinit;
  RuleParserTOKENTYPE yy0;
  std::vector<USBInterfaceType>* yy18;
  String* yy20;
  Rule::SetOperator yy23;
  USBInterfaceType* yy46;
  StringVector* yy49;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define RuleParserARG_SDECL  RuleParserState* state ;
#define RuleParserARG_PDECL , RuleParserState* state 
#define RuleParserARG_FETCH  RuleParserState* state  = yypParser->state 
#define RuleParserARG_STORE yypParser->state  = state 
#define YYNSTATE             30
#define YYNRULE              34
#define YY_MAX_SHIFT         29
#define YY_MIN_SHIFTREDUCE   54
#define YY_MAX_SHIFTREDUCE   87
#define YY_MIN_REDUCE        88
#define YY_MAX_REDUCE        121
#define YY_ERROR_ACTION      122
#define YY_ACCEPT_ACTION     123
#define YY_NO_ACTION         124
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
#define YY_ACTTAB_COUNT (61)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     9,    8,    7,    2,   57,   58,   59,    1,  123,   29,
 /*    10 */     3,   78,   76,   24,   72,   25,   70,   20,   28,   19,
 /*    20 */    26,   11,   77,   14,   71,   24,   62,   81,   61,   28,
 /*    30 */    22,   65,   83,   28,   10,   74,   80,   55,   69,   23,
 /*    40 */    68,   67,   63,   18,   85,   15,   12,   82,    6,    5,
 /*    50 */    21,   16,   17,   13,   88,   90,   87,   27,   90,   90,
 /*    60 */     4,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     9,   10,   11,   12,    3,    4,    5,   16,   29,   30,
 /*    10 */    31,   15,   25,   17,   15,   28,   23,    6,   19,    8,
 /*    20 */    27,   32,   14,   34,   14,   17,    6,    8,    8,   19,
 /*    30 */     8,   36,   25,   19,   18,   23,   17,   33,   23,   17,
 /*    40 */    23,   23,    8,    7,   23,    7,   13,    8,   24,   26,
 /*    50 */     7,    7,    7,   13,    0,   37,   21,   20,   37,   37,
 /*    60 */    35,
};
#define YY_SHIFT_USE_DFLT (-10)
#define YY_SHIFT_COUNT (29)
#define YY_SHIFT_MIN   (-9)
#define YY_SHIFT_MAX   (54)
static const signed char yy_shift_ofst[] = {
 /*     0 */     1,   -4,   -1,   11,   -9,    8,   10,   14,   14,   14,
 /*    10 */    14,   16,  -10,  -10,  -10,   20,   19,   22,   34,   36,
 /*    20 */    38,   39,   43,   44,   45,   33,   40,   35,   37,   54,
};
#define YY_REDUCE_USE_DFLT (-22)
#define YY_REDUCE_COUNT (14)
#define YY_REDUCE_MIN   (-21)
#define YY_REDUCE_MAX   (25)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -21,  -13,   -7,  -11,   -5,    7,   12,   15,   17,   18,
 /*    10 */    21,    4,   23,   24,   25,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    90,  113,  107,   98,   94,  122,  122,  122,  122,  122,
 /*    10 */   122,  120,  118,  109,  100,  122,  122,  122,  122,  122,
 /*    20 */   122,  122,  122,  122,  122,  122,  122,  122,  122,  122,
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
  "KEYWORD_BLOCK",  "KEYWORD_REJECT",  "HEXCHAR4",      "COLON",       
  "ASTERISK",      "KEYWORD_HASH",  "KEYWORD_NAME",  "KEYWORD_SERIAL",
  "KEYWORD_VIAPORT",  "CURLYBRACE_OPEN",  "CURLYBRACE_CLOSE",  "SET_OPERATOR",
  "KEYWORD_WITHINTERFACE",  "HEXCHAR2",      "KEYWORD_ACTION",  "DQ_STRING_BEGIN",
  "DQ_STRING",     "DQ_STRING_END",  "error",         "string",      
  "stringvec",     "usbiftype",     "usbiftypevec",  "ports_set_op",
  "usbif_set_op",  "rule",          "rule_spec",     "target",      
  "device_spec",   "action",        "device_id",     "device_attributes",
  "device_attribute",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "rule ::= rule_spec",
 /*   1 */ "rule_spec ::= target device_spec action",
 /*   2 */ "rule_spec ::=",
 /*   3 */ "target ::= KEYWORD_ALLOW",
 /*   4 */ "target ::= KEYWORD_BLOCK",
 /*   5 */ "target ::= KEYWORD_REJECT",
 /*   6 */ "device_spec ::= device_id device_attributes",
 /*   7 */ "device_id ::= HEXCHAR4 COLON ASTERISK",
 /*   8 */ "device_id ::= HEXCHAR4 COLON HEXCHAR4",
 /*   9 */ "device_id ::= ASTERISK COLON ASTERISK",
 /*  10 */ "device_id ::=",
 /*  11 */ "device_attributes ::= device_attributes device_attribute",
 /*  12 */ "device_attributes ::=",
 /*  13 */ "device_attribute ::= KEYWORD_HASH string",
 /*  14 */ "device_attribute ::= KEYWORD_NAME string",
 /*  15 */ "device_attribute ::= KEYWORD_SERIAL string",
 /*  16 */ "device_attribute ::= KEYWORD_VIAPORT string",
 /*  17 */ "device_attribute ::= KEYWORD_VIAPORT ports_set_op CURLYBRACE_OPEN stringvec CURLYBRACE_CLOSE",
 /*  18 */ "ports_set_op ::= SET_OPERATOR",
 /*  19 */ "ports_set_op ::=",
 /*  20 */ "stringvec ::= stringvec string",
 /*  21 */ "stringvec ::=",
 /*  22 */ "device_attribute ::= KEYWORD_WITHINTERFACE usbiftype",
 /*  23 */ "device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op CURLYBRACE_OPEN usbiftypevec CURLYBRACE_CLOSE",
 /*  24 */ "usbif_set_op ::= SET_OPERATOR",
 /*  25 */ "usbif_set_op ::=",
 /*  26 */ "usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON HEXCHAR2",
 /*  27 */ "usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON ASTERISK",
 /*  28 */ "usbiftype ::= HEXCHAR2 COLON ASTERISK COLON ASTERISK",
 /*  29 */ "usbiftypevec ::= usbiftypevec usbiftype",
 /*  30 */ "usbiftypevec ::=",
 /*  31 */ "action ::= KEYWORD_ACTION string",
 /*  32 */ "action ::=",
 /*  33 */ "string ::= DQ_STRING_BEGIN DQ_STRING DQ_STRING_END",
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
    case 6: /* HEXCHAR4 */
    case 7: /* COLON */
    case 8: /* ASTERISK */
    case 9: /* KEYWORD_HASH */
    case 10: /* KEYWORD_NAME */
    case 11: /* KEYWORD_SERIAL */
    case 12: /* KEYWORD_VIAPORT */
    case 13: /* CURLYBRACE_OPEN */
    case 14: /* CURLYBRACE_CLOSE */
    case 15: /* SET_OPERATOR */
    case 16: /* KEYWORD_WITHINTERFACE */
    case 17: /* HEXCHAR2 */
    case 18: /* KEYWORD_ACTION */
    case 19: /* DQ_STRING_BEGIN */
    case 20: /* DQ_STRING */
    case 21: /* DQ_STRING_END */
{
#line 11 "Parser.y"
 delete (yypminor->yy0); 
#line 497 "Parser.c"
}
      break;
      /* Default NON-TERMINAL Destructor */
    case 22: /* error */
    case 27: /* ports_set_op */
    case 28: /* usbif_set_op */
    case 29: /* rule */
    case 30: /* rule_spec */
    case 31: /* target */
    case 32: /* device_spec */
    case 33: /* action */
    case 34: /* device_id */
    case 35: /* device_attributes */
    case 36: /* device_attribute */
{
#line 7 "Parser.y"
 (void)state; 
#line 515 "Parser.c"
}
      break;
    case 23: /* string */
{
#line 14 "Parser.y"
 delete (yypminor->yy20); 
#line 522 "Parser.c"
}
      break;
    case 24: /* stringvec */
{
#line 17 "Parser.y"
 delete (yypminor->yy49); 
#line 529 "Parser.c"
}
      break;
    case 25: /* usbiftype */
{
#line 20 "Parser.y"
 delete (yypminor->yy46); 
#line 536 "Parser.c"
}
      break;
    case 26: /* usbiftypevec */
{
#line 23 "Parser.y"
 delete (yypminor->yy18); 
#line 543 "Parser.c"
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
  { 29, 1 },
  { 30, 3 },
  { 30, 0 },
  { 31, 1 },
  { 31, 1 },
  { 31, 1 },
  { 32, 2 },
  { 34, 3 },
  { 34, 3 },
  { 34, 3 },
  { 34, 0 },
  { 35, 2 },
  { 35, 0 },
  { 36, 2 },
  { 36, 2 },
  { 36, 2 },
  { 36, 2 },
  { 36, 5 },
  { 27, 1 },
  { 27, 0 },
  { 24, 2 },
  { 24, 0 },
  { 36, 2 },
  { 36, 5 },
  { 28, 1 },
  { 28, 0 },
  { 25, 5 },
  { 25, 5 },
  { 25, 5 },
  { 26, 2 },
  { 26, 0 },
  { 33, 2 },
  { 33, 0 },
  { 23, 3 },
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
#line 34 "Parser.y"
{
  yy_destructor(yypParser,30,&yymsp[0].minor);
}
#line 862 "Parser.c"
        break;
      case 1: /* rule_spec ::= target device_spec action */
#line 36 "Parser.y"
{
  yy_destructor(yypParser,31,&yymsp[-2].minor);
  yy_destructor(yypParser,32,&yymsp[-1].minor);
  yy_destructor(yypParser,33,&yymsp[0].minor);
}
#line 871 "Parser.c"
        break;
      case 3: /* target ::= KEYWORD_ALLOW */
#line 39 "Parser.y"
{
       state->rule.setTarget(Rule::Target::Allow);
  yy_destructor(yypParser,3,&yymsp[0].minor);
}
#line 879 "Parser.c"
        break;
      case 4: /* target ::= KEYWORD_BLOCK */
#line 43 "Parser.y"
{
       state->rule.setTarget(Rule::Target::Block);
  yy_destructor(yypParser,4,&yymsp[0].minor);
}
#line 887 "Parser.c"
        break;
      case 5: /* target ::= KEYWORD_REJECT */
#line 47 "Parser.y"
{
       state->rule.setTarget(Rule::Target::Reject);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 895 "Parser.c"
        break;
      case 6: /* device_spec ::= device_id device_attributes */
#line 51 "Parser.y"
{
  yy_destructor(yypParser,34,&yymsp[-1].minor);
  yy_destructor(yypParser,35,&yymsp[0].minor);
}
#line 903 "Parser.c"
        break;
      case 7: /* device_id ::= HEXCHAR4 COLON ASTERISK */
#line 53 "Parser.y"
{ // 1234:*
  	  state->rule.setVendorID(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()));
	  delete yymsp[-2].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 913 "Parser.c"
        break;
      case 8: /* device_id ::= HEXCHAR4 COLON HEXCHAR4 */
#line 58 "Parser.y"
{ // 1234:5678
	  state->rule.setVendorID(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()));
	  state->rule.setProductID(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
	  delete yymsp[-2].minor.yy0;
	  delete yymsp[0].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 924 "Parser.c"
        break;
      case 9: /* device_id ::= ASTERISK COLON ASTERISK */
#line 65 "Parser.y"
{
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 933 "Parser.c"
        break;
      case 11: /* device_attributes ::= device_attributes device_attribute */
#line 68 "Parser.y"
{
  yy_destructor(yypParser,35,&yymsp[-1].minor);
  yy_destructor(yypParser,36,&yymsp[0].minor);
}
#line 941 "Parser.c"
        break;
      case 13: /* device_attribute ::= KEYWORD_HASH string */
#line 71 "Parser.y"
{
		 state->rule.setDeviceHash(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 950 "Parser.c"
        break;
      case 14: /* device_attribute ::= KEYWORD_NAME string */
#line 76 "Parser.y"
{
		 state->rule.setDeviceName(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,10,&yymsp[-1].minor);
}
#line 959 "Parser.c"
        break;
      case 15: /* device_attribute ::= KEYWORD_SERIAL string */
#line 81 "Parser.y"
{
		 state->rule.setSerialNumber(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 968 "Parser.c"
        break;
      case 16: /* device_attribute ::= KEYWORD_VIAPORT string */
#line 86 "Parser.y"
{
		 state->rule.refDevicePorts().push_back(*yymsp[0].minor.yy20);
		 state->rule.setDevicePortsSetOperator(Rule::SetOperator::Equals);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,12,&yymsp[-1].minor);
}
#line 978 "Parser.c"
        break;
      case 17: /* device_attribute ::= KEYWORD_VIAPORT ports_set_op CURLYBRACE_OPEN stringvec CURLYBRACE_CLOSE */
#line 92 "Parser.y"
{
		 state->rule.refDevicePorts().insert(state->rule.refDevicePorts().end(), yymsp[-1].minor.yy49->begin(), yymsp[-1].minor.yy49->end());
		 state->rule.setDevicePortsSetOperator(yymsp[-3].minor.yy23);
		 delete yymsp[-1].minor.yy49;
  yy_destructor(yypParser,12,&yymsp[-4].minor);
  yy_destructor(yypParser,13,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
#line 990 "Parser.c"
        break;
      case 18: /* ports_set_op ::= SET_OPERATOR */
#line 98 "Parser.y"
{
		 yygotominor.yy23 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
		 delete yymsp[0].minor.yy0;
}
#line 998 "Parser.c"
        break;
      case 19: /* ports_set_op ::= */
#line 103 "Parser.y"
{
		    yygotominor.yy23 = Rule::SetOperator::OneOf;
}
#line 1005 "Parser.c"
        break;
      case 20: /* stringvec ::= stringvec string */
#line 107 "Parser.y"
{
	  yygotominor.yy49 = yymsp[-1].minor.yy49;
	  yygotominor.yy49->push_back(*yymsp[0].minor.yy20);
	  delete yymsp[0].minor.yy20;
}
#line 1014 "Parser.c"
        break;
      case 21: /* stringvec ::= */
#line 113 "Parser.y"
{
	  yygotominor.yy49 = new StringVector();
}
#line 1021 "Parser.c"
        break;
      case 22: /* device_attribute ::= KEYWORD_WITHINTERFACE usbiftype */
#line 117 "Parser.y"
{
	  state->rule.refInterfaceTypes().push_back(*yymsp[0].minor.yy46);
	  state->rule.setInterfaceTypesSetOperator(Rule::SetOperator::Equals);
	  delete yymsp[0].minor.yy46;
  yy_destructor(yypParser,16,&yymsp[-1].minor);
}
#line 1031 "Parser.c"
        break;
      case 23: /* device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op CURLYBRACE_OPEN usbiftypevec CURLYBRACE_CLOSE */
#line 123 "Parser.y"
{
	  state->rule.refInterfaceTypes().insert(state->rule.refInterfaceTypes().end(), yymsp[-1].minor.yy18->begin(), yymsp[-1].minor.yy18->end());
	  state->rule.setInterfaceTypesSetOperator(yymsp[-3].minor.yy23);
	  delete yymsp[-1].minor.yy18;
  yy_destructor(yypParser,16,&yymsp[-4].minor);
  yy_destructor(yypParser,13,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
#line 1043 "Parser.c"
        break;
      case 24: /* usbif_set_op ::= SET_OPERATOR */
#line 129 "Parser.y"
{
		    yygotominor.yy23 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
		    delete yymsp[0].minor.yy0;
}
#line 1051 "Parser.c"
        break;
      case 25: /* usbif_set_op ::= */
#line 134 "Parser.y"
{
		    yygotominor.yy23 = Rule::SetOperator::EqualsOrdered;
}
#line 1058 "Parser.c"
        break;
      case 26: /* usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON HEXCHAR2 */
#line 138 "Parser.y"
{
	     yygotominor.yy46 = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[-4].minor.yy0->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()), 16));
	     delete yymsp[-4].minor.yy0;
	     delete yymsp[-2].minor.yy0;
	     delete yymsp[0].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-3].minor);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 1072 "Parser.c"
        break;
      case 27: /* usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON ASTERISK */
#line 147 "Parser.y"
{
	     yygotominor.yy46 = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[-4].minor.yy0->get_text()), 16),
				      stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()), 16),
				      0,
				      USBInterfaceType::MatchClass|USBInterfaceType::MatchSubClass),
	     delete yymsp[-4].minor.yy0;
	     delete yymsp[-2].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-3].minor);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 1087 "Parser.c"
        break;
      case 28: /* usbiftype ::= HEXCHAR2 COLON ASTERISK COLON ASTERISK */
#line 156 "Parser.y"
{
	     yygotominor.yy46 = new USBInterfaceType(stringToNumber<uint8_t>(quex::unicode_to_char(yymsp[-4].minor.yy0->get_text()), 16),
				      0,
				      0,
				      USBInterfaceType::MatchClass);
	     delete yymsp[-4].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 1102 "Parser.c"
        break;
      case 29: /* usbiftypevec ::= usbiftypevec usbiftype */
#line 164 "Parser.y"
{
	yygotominor.yy18 = yymsp[-1].minor.yy18;
	yygotominor.yy18->push_back(*yymsp[0].minor.yy46);
	delete yymsp[0].minor.yy46;
}
#line 1111 "Parser.c"
        break;
      case 30: /* usbiftypevec ::= */
#line 170 "Parser.y"
{
	yygotominor.yy18 = new std::vector<USBInterfaceType>();
}
#line 1118 "Parser.c"
        break;
      case 31: /* action ::= KEYWORD_ACTION string */
#line 174 "Parser.y"
{
       state->rule.setAction(*yymsp[0].minor.yy20);
       delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,18,&yymsp[-1].minor);
}
#line 1127 "Parser.c"
        break;
      case 33: /* string ::= DQ_STRING_BEGIN DQ_STRING DQ_STRING_END */
#line 181 "Parser.y"
{
	  const String from_unicode = quex::unicode_to_char(yymsp[-1].minor.yy0->get_text());
	  const String unescaped = Rule::unescapeString(from_unicode);
	  yygotominor.yy20 = new String(unescaped);
	  delete yymsp[-1].minor.yy0;
  yy_destructor(yypParser,19,&yymsp[-2].minor);
  yy_destructor(yypParser,21,&yymsp[0].minor);
}
#line 1139 "Parser.c"
        break;
      default:
      /* (2) rule_spec ::= */ yytestcase(yyruleno==2);
      /* (10) device_id ::= */ yytestcase(yyruleno==10);
      /* (12) device_attributes ::= */ yytestcase(yyruleno==12);
      /* (32) action ::= */ yytestcase(yyruleno==32);
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
#line 28 "Parser.y"

  throw std::runtime_error("rule syntax error");
#line 1212 "Parser.c"
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
