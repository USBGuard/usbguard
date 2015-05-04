/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "Parser.y"

#line 10 "Parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    RuleParserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is RuleParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    RuleParserARG_SDECL     A static variable declaration for the %extra_argument
**    RuleParserARG_PDECL     A parameter declaration for the %extra_argument
**    RuleParserARG_STORE     Code to store %extra_argument into yypParser
**    RuleParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
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
#define RuleParserARG_SDECL  Rule* rule ;
#define RuleParserARG_PDECL , Rule* rule 
#define RuleParserARG_FETCH  Rule* rule  = yypParser->rule 
#define RuleParserARG_STORE yypParser->rule  = rule 
#define YYNSTATE 54
#define YYNRULE 34
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

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
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
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
*/
#define YY_ACTTAB_COUNT (65)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */     9,    8,    7,    2,   89,   29,    3,    1,   32,   31,
 /*    10 */    30,   22,   39,   34,   45,   35,   11,   54,   14,   28,
 /*    20 */    23,   40,   41,   51,   13,   24,   20,   43,   19,   46,
 /*    30 */    36,   28,   24,   26,   42,   12,   27,   25,   17,   16,
 /*    40 */    21,   90,   38,   15,   18,    4,   33,    6,   10,   52,
 /*    50 */     5,   53,   49,   37,   28,   50,   90,   90,   90,   90,
 /*    60 */    90,   90,   48,   47,   44,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     9,   10,   11,   12,   29,   30,   31,   16,    3,    4,
 /*    10 */     5,    8,    8,    6,   14,    8,   32,    0,   34,   19,
 /*    20 */    17,   17,   14,   21,   13,   17,    6,   15,    8,   23,
 /*    30 */    15,   19,   17,   27,   25,   13,   20,   28,    7,    7,
 /*    40 */     7,   37,    8,    7,    7,   35,    8,   24,   18,   23,
 /*    50 */    26,   33,   23,   25,   19,   36,   37,   37,   37,   37,
 /*    60 */    37,   37,   23,   23,   23,
};
#define YY_SHIFT_USE_DFLT (-10)
#define YY_SHIFT_COUNT (29)
#define YY_SHIFT_MIN   (-9)
#define YY_SHIFT_MAX   (38)
static const signed char yy_shift_ofst[] = {
 /*     0 */     5,   15,   12,   20,   -9,    8,    0,   35,   35,   35,
 /*    10 */    35,   30,  -10,  -10,  -10,    7,    4,    3,   38,   37,
 /*    20 */    36,   34,   33,   32,   31,   22,   11,    2,   16,   17,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_COUNT (14)
#define YY_REDUCE_MIN   (-25)
#define YY_REDUCE_MAX   (41)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -25,    9,    6,  -16,   19,   28,   41,   40,   39,   29,
 /*    10 */    26,   18,   24,   23,   10,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    56,   79,   73,   64,   60,   88,   88,   88,   88,   88,
 /*    10 */    88,   86,   84,   75,   66,   88,   88,   88,   88,   88,
 /*    20 */    88,   88,   88,   88,   88,   88,   88,   88,   88,   88,
 /*    30 */    59,   58,   57,   63,   62,   61,   78,   83,   82,   81,
 /*    40 */    80,   77,   76,   72,   74,   71,   70,   69,   68,   67,
 /*    50 */    65,   87,   85,   55,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
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
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
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
void *RuleParserAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
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

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
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
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
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
#line 9 "Parser.y"
 delete (yypminor->yy0); 
#line 439 "Parser.c"
}
      break;
    case 23: /* string */
{
#line 12 "Parser.y"
 delete (yypminor->yy20); 
#line 446 "Parser.c"
}
      break;
    case 24: /* stringvec */
{
#line 15 "Parser.y"
 delete (yypminor->yy49); 
#line 453 "Parser.c"
}
      break;
    case 25: /* usbiftype */
{
#line 18 "Parser.y"
 delete (yypminor->yy46); 
#line 460 "Parser.c"
}
      break;
    case 26: /* usbiftypevec */
{
#line 21 "Parser.y"
 delete (yypminor->yy18); 
#line 467 "Parser.c"
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from RuleParserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void RuleParserFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
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
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
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
        return yy_find_shift_action(pParser, iFallback);
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
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
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
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
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
   RuleParserARG_STORE; /* Suppress warning about unused %extra_argument var */
}

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
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
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
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
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
      case 3: /* target ::= KEYWORD_ALLOW */
#line 37 "Parser.y"
{
       rule->setTarget(Rule::Target::Allow);
  yy_destructor(yypParser,3,&yymsp[0].minor);
}
#line 799 "Parser.c"
        break;
      case 4: /* target ::= KEYWORD_BLOCK */
#line 41 "Parser.y"
{
       rule->setTarget(Rule::Target::Block);
  yy_destructor(yypParser,4,&yymsp[0].minor);
}
#line 807 "Parser.c"
        break;
      case 5: /* target ::= KEYWORD_REJECT */
#line 45 "Parser.y"
{
       rule->setTarget(Rule::Target::Reject);
  yy_destructor(yypParser,5,&yymsp[0].minor);
}
#line 815 "Parser.c"
        break;
      case 7: /* device_id ::= HEXCHAR4 COLON ASTERISK */
#line 51 "Parser.y"
{ // 1234:*
  	  rule->setVendorID(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()));
	  delete yymsp[-2].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 825 "Parser.c"
        break;
      case 8: /* device_id ::= HEXCHAR4 COLON HEXCHAR4 */
#line 56 "Parser.y"
{ // 1234:5678
	  rule->setVendorID(quex::unicode_to_char(yymsp[-2].minor.yy0->get_text()));
	  rule->setProductID(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
	  delete yymsp[-2].minor.yy0;
	  delete yymsp[0].minor.yy0;
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 836 "Parser.c"
        break;
      case 9: /* device_id ::= ASTERISK COLON ASTERISK */
#line 63 "Parser.y"
{
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,7,&yymsp[-1].minor);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 845 "Parser.c"
        break;
      case 13: /* device_attribute ::= KEYWORD_HASH string */
#line 69 "Parser.y"
{
		 rule->setDeviceHash(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 854 "Parser.c"
        break;
      case 14: /* device_attribute ::= KEYWORD_NAME string */
#line 74 "Parser.y"
{
		 rule->setDeviceName(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,10,&yymsp[-1].minor);
}
#line 863 "Parser.c"
        break;
      case 15: /* device_attribute ::= KEYWORD_SERIAL string */
#line 79 "Parser.y"
{
		 rule->setSerialNumber(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 872 "Parser.c"
        break;
      case 16: /* device_attribute ::= KEYWORD_VIAPORT string */
#line 84 "Parser.y"
{
		 rule->refDevicePorts().push_back(*yymsp[0].minor.yy20);
		 delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,12,&yymsp[-1].minor);
}
#line 881 "Parser.c"
        break;
      case 17: /* device_attribute ::= KEYWORD_VIAPORT ports_set_op CURLYBRACE_OPEN stringvec CURLYBRACE_CLOSE */
#line 89 "Parser.y"
{
		 rule->refDevicePorts().insert(rule->refDevicePorts().end(), yymsp[-1].minor.yy49->begin(), yymsp[-1].minor.yy49->end());
		 rule->setDevicePortsSetOperator(yymsp[-3].minor.yy23);
		 delete yymsp[-1].minor.yy49;
  yy_destructor(yypParser,12,&yymsp[-4].minor);
  yy_destructor(yypParser,13,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
#line 893 "Parser.c"
        break;
      case 18: /* ports_set_op ::= SET_OPERATOR */
      case 24: /* usbif_set_op ::= SET_OPERATOR */ yytestcase(yyruleno==24);
#line 95 "Parser.y"
{
		    yygotominor.yy23 = Rule::setOperatorFromString(quex::unicode_to_char(yymsp[0].minor.yy0->get_text()));
		    delete yymsp[0].minor.yy0;
}
#line 902 "Parser.c"
        break;
      case 19: /* ports_set_op ::= */
      case 25: /* usbif_set_op ::= */ yytestcase(yyruleno==25);
#line 100 "Parser.y"
{
		    yygotominor.yy23 = Rule::SetOperator::EqualsOrdered;
}
#line 910 "Parser.c"
        break;
      case 20: /* stringvec ::= stringvec string */
#line 104 "Parser.y"
{
	  yygotominor.yy49 = yymsp[-1].minor.yy49;
	  yygotominor.yy49->push_back(*yymsp[0].minor.yy20);
	  delete yymsp[0].minor.yy20;
}
#line 919 "Parser.c"
        break;
      case 21: /* stringvec ::= */
#line 110 "Parser.y"
{
	  yygotominor.yy49 = new StringVector();
}
#line 926 "Parser.c"
        break;
      case 22: /* device_attribute ::= KEYWORD_WITHINTERFACE usbiftype */
#line 114 "Parser.y"
{
	  rule->refInterfaceTypes().push_back(*yymsp[0].minor.yy46);
	  delete yymsp[0].minor.yy46;
  yy_destructor(yypParser,16,&yymsp[-1].minor);
}
#line 935 "Parser.c"
        break;
      case 23: /* device_attribute ::= KEYWORD_WITHINTERFACE usbif_set_op CURLYBRACE_OPEN usbiftypevec CURLYBRACE_CLOSE */
#line 119 "Parser.y"
{
	  rule->refInterfaceTypes().insert(rule->refInterfaceTypes().end(), yymsp[-1].minor.yy18->begin(), yymsp[-1].minor.yy18->end());
	  rule->setInterfaceTypesSetOperator(yymsp[-3].minor.yy23);
	  delete yymsp[-1].minor.yy18;
  yy_destructor(yypParser,16,&yymsp[-4].minor);
  yy_destructor(yypParser,13,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
#line 947 "Parser.c"
        break;
      case 26: /* usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON HEXCHAR2 */
#line 134 "Parser.y"
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
#line 961 "Parser.c"
        break;
      case 27: /* usbiftype ::= HEXCHAR2 COLON HEXCHAR2 COLON ASTERISK */
#line 143 "Parser.y"
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
#line 976 "Parser.c"
        break;
      case 28: /* usbiftype ::= HEXCHAR2 COLON ASTERISK COLON ASTERISK */
#line 152 "Parser.y"
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
#line 991 "Parser.c"
        break;
      case 29: /* usbiftypevec ::= usbiftypevec usbiftype */
#line 160 "Parser.y"
{
	yygotominor.yy18 = yymsp[-1].minor.yy18;
	yygotominor.yy18->push_back(*yymsp[0].minor.yy46);
	delete yymsp[0].minor.yy46;
}
#line 1000 "Parser.c"
        break;
      case 30: /* usbiftypevec ::= */
#line 166 "Parser.y"
{
	yygotominor.yy18 = new std::vector<USBInterfaceType>();
}
#line 1007 "Parser.c"
        break;
      case 31: /* action ::= KEYWORD_ACTION string */
#line 170 "Parser.y"
{
       rule->setAction(*yymsp[0].minor.yy20);
       delete yymsp[0].minor.yy20;
  yy_destructor(yypParser,18,&yymsp[-1].minor);
}
#line 1016 "Parser.c"
        break;
      case 33: /* string ::= DQ_STRING_BEGIN DQ_STRING DQ_STRING_END */
#line 177 "Parser.y"
{
	  const String from_unicode = quex::unicode_to_char(yymsp[-1].minor.yy0->get_text());
	  const String unescaped = Rule::unescapeString(from_unicode);
	  yygotominor.yy20 = new String(unescaped);
	  delete yymsp[-1].minor.yy0;
  yy_destructor(yypParser,19,&yymsp[-2].minor);
  yy_destructor(yypParser,21,&yymsp[0].minor);
}
#line 1028 "Parser.c"
        break;
      default:
      /* (0) rule ::= rule_spec */ yytestcase(yyruleno==0);
      /* (1) rule_spec ::= target device_spec action */ yytestcase(yyruleno==1);
      /* (2) rule_spec ::= */ yytestcase(yyruleno==2);
      /* (6) device_spec ::= device_id device_attributes */ yytestcase(yyruleno==6);
      /* (10) device_id ::= */ yytestcase(yyruleno==10);
      /* (11) device_attributes ::= device_attributes device_attribute */ yytestcase(yyruleno==11);
      /* (12) device_attributes ::= */ yytestcase(yyruleno==12);
      /* (32) action ::= */ yytestcase(yyruleno==32);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
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
#line 26 "Parser.y"

  throw std::runtime_error("rule syntax error");
#line 1101 "Parser.c"
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
  int yyendofinput;     /* True if we are at the end of input */
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
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  RuleParserARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
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
                        YYERRORSYMBOL)) >= YYNSTATE
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
  return;
}
