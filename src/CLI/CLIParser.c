/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>
#line 1 "CLIParser.y"

#include <cstdint>
#line 11 "CLIParser.c"
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
**    CLIParserTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is CLIParserTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    CLIParserARG_SDECL     A static variable declaration for the %extra_argument
**    CLIParserARG_PDECL     A parameter declaration for the %extra_argument
**    CLIParserARG_STORE     Code to store %extra_argument into yypParser
**    CLIParserARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 40
#define YYACTIONTYPE unsigned char
#define CLIParserTOKENTYPE  String* 
typedef union {
  int yyinit;
  CLIParserTOKENTYPE yy0;
  std::string* yy5;
  DeviceSetPolicy::Options* yy31;
  RuleAppend::Options* yy39;
  uint32_t yy63;
  bool yy65;
  CLICommand* yy78;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define CLIParserARG_SDECL  PointerVector<CLICommand>* commands ;
#define CLIParserARG_PDECL , PointerVector<CLICommand>* commands 
#define CLIParserARG_FETCH  PointerVector<CLICommand>* commands  = yypParser->commands 
#define CLIParserARG_STORE yypParser->commands  = commands 
#define YYNSTATE 48
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
#define YY_ACTTAB_COUNT (54)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    40,    8,   15,   39,   38,   37,    1,   29,   27,    7,
 /*    10 */     6,    9,    5,   14,   48,   33,   20,   10,    4,   44,
 /*    20 */    43,   42,   35,   34,    3,   12,   21,   32,   19,   30,
 /*    30 */    16,   45,   84,   17,   24,   31,   83,   11,   18,   46,
 /*    40 */    13,   36,   84,   47,   22,   26,   25,   84,   28,    2,
 /*    50 */    84,   41,   84,   23,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    13,    3,    5,   16,   17,   18,    9,   10,   10,   11,
 /*    10 */    12,    6,    7,    8,    0,   21,   22,    3,    4,   13,
 /*    20 */    14,   15,   19,   20,    7,    8,   33,   23,   21,   30,
 /*    30 */    37,   30,   39,   33,   38,   36,   34,   35,   28,   26,
 /*    40 */    29,   32,   39,   25,   32,   30,   30,   39,   27,   30,
 /*    50 */    39,   31,   39,   31,
};
#define YY_SHIFT_USE_DFLT (-14)
#define YY_SHIFT_COUNT (20)
#define YY_SHIFT_MIN   (-13)
#define YY_SHIFT_MAX   (17)
static const signed char yy_shift_ofst[] = {
 /*     0 */   -14,    6,   -6,  -13,   -2,  -13,    6,    6,    6,    6,
 /*    10 */    -3,   14,    3,   17,    3,   -6,  -14,  -14,    5,    4,
 /*    20 */     7,
};
#define YY_REDUCE_USE_DFLT (-8)
#define YY_REDUCE_COUNT (17)
#define YY_REDUCE_MIN   (-7)
#define YY_REDUCE_MAX   (22)
static const signed char yy_reduce_ofst[] = {
 /*     0 */     2,   -1,   -7,   22,   21,   20,   19,   16,   15,    1,
 /*    10 */    13,   18,   12,   -4,    9,    0,   11,   10,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    50,   82,   82,   82,   82,   82,   82,   82,   82,   82,
 /*    10 */    82,   82,   69,   64,   82,   82,   66,   57,   53,   82,
 /*    20 */    82,   79,   68,   67,   65,   63,   62,   61,   52,   60,
 /*    30 */    59,   58,   81,   80,   78,   77,   56,   76,   75,   74,
 /*    40 */    73,   55,   72,   71,   70,   54,   51,   49,
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
  CLIParserARG_SDECL                /* A place to hold %extra_argument */
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
void CLIParserTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "TERMINATION",   "UNINITIALIZED",  "KEYWORD_RULE",
  "KEYWORD_DEVICE",  "KEYWORD_APPEND",  "OPTION_PARENT",  "OPTION_TIMEOUT",
  "OPTION_PERMANENT",  "KEYWORD_REMOVE",  "KEYWORD_LIST",  "KEYWORD_HASH",
  "KEYWORD_SET_POLICY",  "NUMBER_UNSIGNED_INTEGER",  "KEYWORD_LAST",  "KEYWORD_ROOT",
  "NUMBER_HOURS",  "NUMBER_MINUTES",  "NUMBER_DAYS",   "KEYWORD_FALSE",
  "KEYWORD_TRUE",  "STRING",        "DQ_STRING_BEGIN",  "DQ_STRING_END",
  "error",         "command",       "rule_command",  "device_command",
  "rule_append_options",  "device_set_policy_options",  "seqn_value",    "timeout_value",
  "boolean_value",  "string",        "cli",           "commands",    
  "rule_remove_args",  "target_value",  "device_set_policy_option",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "cli ::= commands",
 /*   1 */ "commands ::= commands command",
 /*   2 */ "commands ::=",
 /*   3 */ "command ::= KEYWORD_RULE rule_command",
 /*   4 */ "command ::= KEYWORD_DEVICE device_command",
 /*   5 */ "rule_command ::= KEYWORD_APPEND string rule_append_options",
 /*   6 */ "rule_append_options ::= rule_append_options OPTION_PARENT seqn_value",
 /*   7 */ "rule_append_options ::= rule_append_options OPTION_TIMEOUT timeout_value",
 /*   8 */ "rule_append_options ::= rule_append_options OPTION_PERMANENT boolean_value",
 /*   9 */ "rule_append_options ::=",
 /*  10 */ "rule_command ::= KEYWORD_REMOVE rule_remove_args",
 /*  11 */ "rule_remove_args ::= seqn_value",
 /*  12 */ "rule_command ::= KEYWORD_LIST",
 /*  13 */ "device_command ::= KEYWORD_LIST",
 /*  14 */ "device_command ::= KEYWORD_RULE seqn_value",
 /*  15 */ "device_command ::= KEYWORD_HASH seqn_value",
 /*  16 */ "device_command ::= KEYWORD_SET_POLICY seqn_value target_value device_set_policy_options",
 /*  17 */ "device_set_policy_options ::= device_set_policy_options device_set_policy_option",
 /*  18 */ "device_set_policy_options ::=",
 /*  19 */ "device_set_policy_option ::= OPTION_TIMEOUT timeout_value",
 /*  20 */ "device_set_policy_option ::= OPTION_PERMANENT boolean_value",
 /*  21 */ "device_set_policy_option ::= OPTION_PERMANENT",
 /*  22 */ "seqn_value ::= NUMBER_UNSIGNED_INTEGER",
 /*  23 */ "seqn_value ::= KEYWORD_LAST",
 /*  24 */ "seqn_value ::= KEYWORD_ROOT",
 /*  25 */ "timeout_value ::= NUMBER_UNSIGNED_INTEGER",
 /*  26 */ "timeout_value ::= NUMBER_HOURS",
 /*  27 */ "timeout_value ::= NUMBER_MINUTES",
 /*  28 */ "timeout_value ::= NUMBER_DAYS",
 /*  29 */ "boolean_value ::= KEYWORD_FALSE",
 /*  30 */ "boolean_value ::= KEYWORD_TRUE",
 /*  31 */ "target_value ::= string",
 /*  32 */ "string ::= STRING",
 /*  33 */ "string ::= DQ_STRING_BEGIN STRING DQ_STRING_END",
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
** to CLIParser and CLIParserFree.
*/
void *CLIParserAlloc(void *(*mallocProc)(size_t)){
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
  CLIParserARG_FETCH;
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
    case 3: /* KEYWORD_RULE */
    case 4: /* KEYWORD_DEVICE */
    case 5: /* KEYWORD_APPEND */
    case 6: /* OPTION_PARENT */
    case 7: /* OPTION_TIMEOUT */
    case 8: /* OPTION_PERMANENT */
    case 9: /* KEYWORD_REMOVE */
    case 10: /* KEYWORD_LIST */
    case 11: /* KEYWORD_HASH */
    case 12: /* KEYWORD_SET_POLICY */
    case 13: /* NUMBER_UNSIGNED_INTEGER */
    case 14: /* KEYWORD_LAST */
    case 15: /* KEYWORD_ROOT */
    case 16: /* NUMBER_HOURS */
    case 17: /* NUMBER_MINUTES */
    case 18: /* NUMBER_DAYS */
    case 19: /* KEYWORD_FALSE */
    case 20: /* KEYWORD_TRUE */
    case 21: /* STRING */
    case 22: /* DQ_STRING_BEGIN */
    case 23: /* DQ_STRING_END */
{
#line 10 "CLIParser.y"
 delete (yypminor->yy0); 
#line 440 "CLIParser.c"
}
      break;
    case 25: /* command */
    case 26: /* rule_command */
    case 27: /* device_command */
{
#line 13 "CLIParser.y"
 delete (yypminor->yy78); 
#line 449 "CLIParser.c"
}
      break;
    case 28: /* rule_append_options */
{
#line 22 "CLIParser.y"
 delete (yypminor->yy39); 
#line 456 "CLIParser.c"
}
      break;
    case 29: /* device_set_policy_options */
{
#line 25 "CLIParser.y"
 delete (yypminor->yy31); 
#line 463 "CLIParser.c"
}
      break;
    case 33: /* string */
{
#line 32 "CLIParser.y"
 delete (yypminor->yy5); 
#line 470 "CLIParser.c"
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
**       obtained from CLIParserAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void CLIParserFree(
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
int CLIParserStackPeak(void *p){
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
   CLIParserARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
   CLIParserARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 34, 1 },
  { 35, 2 },
  { 35, 0 },
  { 25, 2 },
  { 25, 2 },
  { 26, 3 },
  { 28, 3 },
  { 28, 3 },
  { 28, 3 },
  { 28, 0 },
  { 26, 2 },
  { 36, 1 },
  { 26, 1 },
  { 27, 1 },
  { 27, 2 },
  { 27, 2 },
  { 27, 4 },
  { 29, 2 },
  { 29, 0 },
  { 38, 2 },
  { 38, 2 },
  { 38, 1 },
  { 30, 1 },
  { 30, 1 },
  { 30, 1 },
  { 31, 1 },
  { 31, 1 },
  { 31, 1 },
  { 31, 1 },
  { 32, 1 },
  { 32, 1 },
  { 37, 1 },
  { 33, 1 },
  { 33, 3 },
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
  CLIParserARG_FETCH;
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
      case 1: /* commands ::= commands command */
#line 41 "CLIParser.y"
{
    commands->push_back(Pointer<CLICommand>(yymsp[0].minor.yy78));
}
#line 801 "CLIParser.c"
        break;
      case 3: /* command ::= KEYWORD_RULE rule_command */
#line 46 "CLIParser.y"
{ yygotominor.yy78 = yymsp[0].minor.yy78;   yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 807 "CLIParser.c"
        break;
      case 4: /* command ::= KEYWORD_DEVICE device_command */
#line 47 "CLIParser.y"
{ yygotominor.yy78 = yymsp[0].minor.yy78;   yy_destructor(yypParser,4,&yymsp[-1].minor);
}
#line 813 "CLIParser.c"
        break;
      case 5: /* rule_command ::= KEYWORD_APPEND string rule_append_options */
#line 52 "CLIParser.y"
{
    yygotominor.yy78 = new RuleAppend(*yymsp[-1].minor.yy5, *yymsp[0].minor.yy39);
    delete yymsp[0].minor.yy39;
  yy_destructor(yypParser,5,&yymsp[-2].minor);
}
#line 822 "CLIParser.c"
        break;
      case 6: /* rule_append_options ::= rule_append_options OPTION_PARENT seqn_value */
#line 57 "CLIParser.y"
{
    yygotominor.yy39 = yymsp[-2].minor.yy39;
    yygotominor.yy39->parent_seqn = yymsp[0].minor.yy63; 
  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
#line 831 "CLIParser.c"
        break;
      case 7: /* rule_append_options ::= rule_append_options OPTION_TIMEOUT timeout_value */
#line 62 "CLIParser.y"
{
    yygotominor.yy39 = yymsp[-2].minor.yy39;
    yygotominor.yy39->timeout = yymsp[0].minor.yy63;
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 840 "CLIParser.c"
        break;
      case 8: /* rule_append_options ::= rule_append_options OPTION_PERMANENT boolean_value */
#line 67 "CLIParser.y"
{
    yygotominor.yy39 = yymsp[-2].minor.yy39;
    yygotominor.yy39->permanent = yymsp[0].minor.yy65;
  yy_destructor(yypParser,8,&yymsp[-1].minor);
}
#line 849 "CLIParser.c"
        break;
      case 9: /* rule_append_options ::= */
#line 72 "CLIParser.y"
{
    yygotominor.yy39 = new RuleAppend::Options();
}
#line 856 "CLIParser.c"
        break;
      case 10: /* rule_command ::= KEYWORD_REMOVE rule_remove_args */
#line 80 "CLIParser.y"
{
  yy_destructor(yypParser,9,&yymsp[-1].minor);
}
#line 863 "CLIParser.c"
        break;
      case 12: /* rule_command ::= KEYWORD_LIST */
      case 13: /* device_command ::= KEYWORD_LIST */ yytestcase(yyruleno==13);
#line 86 "CLIParser.y"
{
  yy_destructor(yypParser,10,&yymsp[0].minor);
}
#line 871 "CLIParser.c"
        break;
      case 14: /* device_command ::= KEYWORD_RULE seqn_value */
#line 96 "CLIParser.y"
{
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
#line 878 "CLIParser.c"
        break;
      case 15: /* device_command ::= KEYWORD_HASH seqn_value */
#line 101 "CLIParser.y"
{
  yy_destructor(yypParser,11,&yymsp[-1].minor);
}
#line 885 "CLIParser.c"
        break;
      case 16: /* device_command ::= KEYWORD_SET_POLICY seqn_value target_value device_set_policy_options */
#line 106 "CLIParser.y"
{
  yy_destructor(yypParser,12,&yymsp[-3].minor);
  yy_destructor(yypParser,29,&yymsp[0].minor);
}
#line 893 "CLIParser.c"
        break;
      case 17: /* device_set_policy_options ::= device_set_policy_options device_set_policy_option */
#line 107 "CLIParser.y"
{
  yy_destructor(yypParser,29,&yymsp[-1].minor);
}
#line 900 "CLIParser.c"
        break;
      case 19: /* device_set_policy_option ::= OPTION_TIMEOUT timeout_value */
#line 109 "CLIParser.y"
{
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
#line 907 "CLIParser.c"
        break;
      case 20: /* device_set_policy_option ::= OPTION_PERMANENT boolean_value */
#line 110 "CLIParser.y"
{
  yy_destructor(yypParser,8,&yymsp[-1].minor);
}
#line 914 "CLIParser.c"
        break;
      case 21: /* device_set_policy_option ::= OPTION_PERMANENT */
#line 111 "CLIParser.y"
{
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
#line 921 "CLIParser.c"
        break;
      case 22: /* seqn_value ::= NUMBER_UNSIGNED_INTEGER */
#line 116 "CLIParser.y"
{
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
#line 928 "CLIParser.c"
        break;
      case 23: /* seqn_value ::= KEYWORD_LAST */
#line 117 "CLIParser.y"
{
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
#line 935 "CLIParser.c"
        break;
      case 24: /* seqn_value ::= KEYWORD_ROOT */
#line 118 "CLIParser.y"
{
  yy_destructor(yypParser,15,&yymsp[0].minor);
}
#line 942 "CLIParser.c"
        break;
      case 25: /* timeout_value ::= NUMBER_UNSIGNED_INTEGER */
#line 120 "CLIParser.y"
{
    yygotominor.yy63 = stringToNumber<uint32_t>(*yymsp[0].minor.yy0, 10);
}
#line 949 "CLIParser.c"
        break;
      case 26: /* timeout_value ::= NUMBER_HOURS */
#line 124 "CLIParser.y"
{
    yygotominor.yy63 = stringToNumber<uint32_t>(*yymsp[0].minor.yy0, 10);
    yygotominor.yy63 = yygotominor.yy63 * 60 * 60;
}
#line 957 "CLIParser.c"
        break;
      case 27: /* timeout_value ::= NUMBER_MINUTES */
#line 129 "CLIParser.y"
{
    yygotominor.yy63 = stringToNumber<uint32_t>(*yymsp[0].minor.yy0, 10);
    yygotominor.yy63 = yygotominor.yy63 * 60;
}
#line 965 "CLIParser.c"
        break;
      case 28: /* timeout_value ::= NUMBER_DAYS */
#line 134 "CLIParser.y"
{
    yygotominor.yy63 = stringToNumber<uint32_t>(*yymsp[0].minor.yy0, 10);
    yygotominor.yy63 = yygotominor.yy63 * 60 * 60 * 24;
}
#line 973 "CLIParser.c"
        break;
      case 29: /* boolean_value ::= KEYWORD_FALSE */
#line 139 "CLIParser.y"
{
    yygotominor.yy65 = false;
  yy_destructor(yypParser,19,&yymsp[0].minor);
}
#line 981 "CLIParser.c"
        break;
      case 30: /* boolean_value ::= KEYWORD_TRUE */
#line 143 "CLIParser.y"
{
    yygotominor.yy65 = true;
  yy_destructor(yypParser,20,&yymsp[0].minor);
}
#line 989 "CLIParser.c"
        break;
      case 31: /* target_value ::= string */
#line 147 "CLIParser.y"
{
  yy_destructor(yypParser,33,&yymsp[0].minor);
}
#line 996 "CLIParser.c"
        break;
      case 32: /* string ::= STRING */
#line 149 "CLIParser.y"
{
    yygotominor.yy5 = new std::string(*yymsp[0].minor.yy0);
}
#line 1003 "CLIParser.c"
        break;
      case 33: /* string ::= DQ_STRING_BEGIN STRING DQ_STRING_END */
#line 153 "CLIParser.y"
{
    yygotominor.yy5 = new std::string(*yymsp[-1].minor.yy0);
  yy_destructor(yypParser,22,&yymsp[-2].minor);
  yy_destructor(yypParser,23,&yymsp[0].minor);
}
#line 1012 "CLIParser.c"
        break;
      default:
      /* (0) cli ::= commands */ yytestcase(yyruleno==0);
      /* (2) commands ::= */ yytestcase(yyruleno==2);
      /* (11) rule_remove_args ::= seqn_value */ yytestcase(yyruleno==11);
      /* (18) device_set_policy_options ::= */ yytestcase(yyruleno==18);
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
  CLIParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  CLIParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
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
  CLIParserARG_FETCH;
#define TOKEN (yyminor.yy0)
#line 34 "CLIParser.y"

    throw std::runtime_error("CLI command syntax error");
#line 1081 "CLIParser.c"
  CLIParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  CLIParserARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  CLIParserARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "CLIParserAlloc" which describes the current state of the parser.
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
void CLIParser(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  CLIParserTOKENTYPE yyminor       /* The value for the token */
  CLIParserARG_PDECL               /* Optional %extra_argument parameter */
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
  CLIParserARG_STORE;

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
