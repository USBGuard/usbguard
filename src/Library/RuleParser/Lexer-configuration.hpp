/* -*- C++ -*-   vim: set syntax=cpp:
 *
 * (C) 2005-2015 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                                                    */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__CONFIGURATION____QUEX____LEXER
#define __QUEX_INCLUDE_GUARD__ANALYZER__CONFIGURATION____QUEX____LEXER
/* Additionally to the 'local' include guard, there is an include indicator 
 *
 *         __INCLUDE_INDICATOR_QUEX__CONFIGURATION
 *
 * If it is defined, this means, that another lexical analyzer configuration
 * has been included before this. That in turn means, that multiple lexical
 * analyzers are used. The configuration settings of the previous analyzer
 * need to be undefined. And, this happens in "configuration/undefine".         
 *
 * NOTE: We do undefine without making sure that there is an older definition
 *       from a configuration file. This allows users to define some
 *       configuration options on the command line without being immediately
 *       deleted by "configuration/undefine".                                */
#ifdef __QUEX_INCLUDE_INDICATOR__ANALYZER__CONFIGURATION
#   include <quex/code_base/analyzer/configuration/undefine>
#else
#   define __QUEX_INCLUDE_INDICATOR__ANALYZER__CONFIGURATION
#endif

#define QUEX_SETTING_VERSION           "0.65.10"
#define QUEX_SETTING_BUILD_DATE        "Thu Jan 21 13:03:12 2016"
#define QUEX_SETTING_ANALYZER_VERSION  "0.0.0-pre-release"

#ifndef    __QUEX_OPTION_PLAIN_C
/* #define __QUEX_OPTION_PLAIN_C */
#endif

/* Following checks are best done here:
 *   -- Verification of the 'C++/C' version.
 *   -- Assert control by this configuration file.
 *
 * Errors would be hard to find if the two checks were made in
 *   -- 'quex/code_base/configuration/validation' or
 *   -- 'quex/code_base/configuration/derived'.                              */ 
#if ! defined(__QUEX_OPTION_PLAIN_C) && ! defined(__cplusplus)
#   error "__QUEX_OPTION_PLAIN_C must be defined if no C++ compiler is used! Call quex with option '--language C'."
#endif

#if defined(__QUEX_INCLUDE_INDICATOR__ASSERTS)
#   error "Asserts included before configuration file. However, the configuration file MUST control asserts!"
#endif
#
#if           defined(QUEX_OPTION_ASSERTS_DISABLED) || defined(NDEBUG)
#   ifdef     QUEX_OPTION_ASSERTS
#      undef  QUEX_OPTION_ASSERTS
#   endif
#else
#   ifndef    QUEX_OPTION_ASSERTS
#      define QUEX_OPTION_ASSERTS
#   endif
#endif

#ifndef  QUEX_OPTION_COMPUTED_GOTOS
/* #define QUEX_OPTION_COMPUTED_GOTOS */
#endif

#define    QUEX_SETTING_CHARACTER_NEWLINE_IN_ENGINE_CODEC  ('\n')

#define    __QUEX_SETTING_MAX_MODE_CLASS_N         (3)
#ifndef    __QUEX_SETTING_INITIAL_LEXER_MODE_ID
#   define __QUEX_SETTING_INITIAL_LEXER_MODE_ID    (QUEX_NAME(ModeID_RULE))
#endif
/* QUEX_TYPE_TOKEN_ID is later on defined inside the token class header.     */
#define    __QUEX_SETTING_TOKEN_ID_TERMINATION       ((QUEX_TYPE_TOKEN_ID)RULE_TOKEN_TERMINATION)
#define    __QUEX_SETTING_TOKEN_ID_UNINITIALIZED     ((QUEX_TYPE_TOKEN_ID)RULE_TOKEN_UNINITIALIZED)
#define    __QUEX_SETTING_TOKEN_ID_INDENT            ((QUEX_TYPE_TOKEN_ID)RULE_TOKEN_INDENT)
#define    __QUEX_SETTING_TOKEN_ID_DEDENT            ((QUEX_TYPE_TOKEN_ID)RULE_TOKEN_DEDENT)
#define    __QUEX_SETTING_TOKEN_ID_NODENT            ((QUEX_TYPE_TOKEN_ID)RULE_TOKEN_NODENT)
#define    __QUEX_SETTING_TOKEN_ID_REPETITION_TEST(TokenID) (false)

#ifndef    __QUEX_OPTION_LITTLE_ENDIAN
#define    __QUEX_OPTION_LITTLE_ENDIAN
#endif
#ifndef    __QUEX_OPTION_BIG_ENDIAN
/* #define __QUEX_OPTION_BIG_ENDIAN */
#endif
#ifndef    __QUEX_OPTION_SYSTEM_ENDIAN
#define    __QUEX_OPTION_SYSTEM_ENDIAN
#endif

#if (! defined(QUEX_OPTION_TOKEN_POLICY_QUEUE)) && (! defined(QUEX_OPTION_TOKEN_POLICY_QUEUE))
#ifndef QUEX_OPTION_TOKEN_POLICY_QUEUE
#define    QUEX_OPTION_TOKEN_POLICY_QUEUE
#endif
#ifndef QUEX_OPTION_TOKEN_POLICY_SINGLE
/* #define QUEX_OPTION_TOKEN_POLICY_SINGLE */
#endif
#endif

#ifndef QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY
/* #define QUEX_OPTION_USER_MANAGED_TOKEN_MEMORY */
#endif

#if defined (QUEX_OPTION_TOKEN_POLICY_QUEUE)
#   ifndef    QUEX_SETTING_TOKEN_QUEUE_SIZE
#      define QUEX_SETTING_TOKEN_QUEUE_SIZE          ((size_t)64)
#   endif
#   ifndef    QUEX_SETTING_TOKEN_QUEUE_SAFETY_BORDER
#      define QUEX_SETTING_TOKEN_QUEUE_SAFETY_BORDER ((size_t)16)
#   endif
#endif

#ifndef QUEX_OPTION_TOKEN_REPETITION_SUPPORT
/* #define QUEX_OPTION_TOKEN_REPETITION_SUPPORT */
#endif

/* OPTIONS: ___________________________________________________________________
 *
 * Activate/Deactivate Options via comment/uncomment. Options without a 
 * double underline '__' at the beginning can be turned off in the created 
 * engine. Options that do start with '__' configure the machine for the
 * specified behavior. Such options are better not touched.
 *
 * -- Line Number / Column Number Counting:
 *    Turning counting off may result in engine speed-up.                    */
#ifndef    QUEX_OPTION_LINE_NUMBER_COUNTING            
#define    QUEX_OPTION_LINE_NUMBER_COUNTING         
#endif
#ifndef    QUEX_OPTION_COLUMN_NUMBER_COUNTING          
#define    QUEX_OPTION_COLUMN_NUMBER_COUNTING       
#endif

#define    QUEX_IF_IS_NEWLINE(X)    if( X == '\n' )

#ifndef QUEX_OPTION_POST_CATEGORIZER
/* #define QUEX_OPTION_POST_CATEGORIZER */
#endif

/* Mode Transitions:
 *    If the engine was created without the flag '--no-mode-transition-check'
 *    then code for mode transition control is inserted. It can be deactivated
 *    by commenting the following option out.                                */
#ifndef QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
#define    QUEX_OPTION_RUNTIME_MODE_TRANSITION_CHECK
#endif

#ifndef    QUEX_SETTING_MODE_STACK_SIZE 
#   define QUEX_SETTING_MODE_STACK_SIZE   (size_t)8
#endif

/* Verbosity (uncomment the following, if you want it verbose.)              */
/* #define QUEX_OPTION_ASSERTS_WARNING_MESSAGE_DISABLED                      */
                                                                             
/* Include Stack Support                                                     */
#ifndef   QUEX_OPTION_INCLUDE_STACK
#define    QUEX_OPTION_INCLUDE_STACK
#endif

/* BLC -- Buffer Limit Code:
 *
 * This code is used as a delimiter for buffer borders. When the analyzer
 * hits a character with such a code, it knows that a border or the
 * end of file has been reached. 
 *
 * IT IS NOT SUPPOSED TO APPEAR IN THE NORMAL CHARACTER STREAM.              */
#ifndef    QUEX_SETTING_BUFFER_LIMIT_CODE
#   define QUEX_SETTING_BUFFER_LIMIT_CODE  ((QUEX_TYPE_CHARACTER)0x0)
#endif

/* PTC -- Path Termination code:
 * 
 * Only required for path compression (see option '--path-compression' and 
 * '--path-compression-uniform' for quex on the command line). PTC must be 
 * different from the BLC so that the pathwalker does not get confuses if
 * the input pointer stands on a buffer border and at the same time the
 * path iterator stands at the end of the path. 
 *
 * IT IS NOT SUPPOSED TO APPEAR IN THE NORMAL CHARACTER STREAM.              */
#ifndef    QUEX_SETTING_PATH_TERMINATION_CODE
#   define QUEX_SETTING_PATH_TERMINATION_CODE  ((QUEX_TYPE_CHARACTER)0x1)
#endif

/* NOTE: A cast to 'size_t' would it make impossible to use the macro in 
 *       pre-processor comparisons.                                          */
#ifndef    QUEX_OPTION_BUFFER_BASED_ANALYZIS
/* #define QUEX_OPTION_BUFFER_BASED_ANALYZIS */
#endif
#ifndef     QUEX_SETTING_BUFFER_SIZE
     /* This setting must be defined as plain number, since there might
      * be some pre-processor comparison operations depending on it.         */
#    define QUEX_SETTING_BUFFER_SIZE  65536
#endif

#ifndef    QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE
#   define QUEX_SETTING_BUFFER_FILLER_SEEK_TEMP_BUFFER_SIZE  (512)
#endif
#ifndef    QUEX_SETTING_ICU_PIVOT_BUFFER_SIZE
#   define QUEX_SETTING_ICU_PIVOT_BUFFER_SIZE                (64)
#endif

#ifndef  QUEX_OPTION_STRING_ACCUMULATOR
#define    QUEX_OPTION_STRING_ACCUMULATOR
#endif 
/* Initial size of the character accumulator.                                */
#ifndef     QUEX_SETTING_ACCUMULATOR_INITIAL_SIZE
#   define  QUEX_SETTING_ACCUMULATOR_INITIAL_SIZE        (256)
#endif

/* Granularity, if new memory has to be allocated. The new memory will be by
 * this factor greater than the previous.  Example: At start, memory contains
 * 256 characters; then new allocation becomes necessary; if factor = 0.5, then
 * the new memory will contain (256 + 128) = 384 characters. The next time, the
 * new memory of size (384 + 192) = 576 characters.                          */

#ifndef     QUEX_SETTING_ACCUMULATOR_GRANULARITY_FACTOR
#   define  QUEX_SETTING_ACCUMULATOR_GRANULARITY_FACTOR  (0.8)
#endif

/* If one mode requires indentation support, then the lexical analyser class
 * must be setup for indentation counting. The following flag is defined or
 * undefined by the lexical analyser generator quex.                         */
/* #define QUEX_OPTION_INDENTATION_TRIGGER */
#if    defined(QUEX_OPTION_INDENTATION_TRIGGER)
#   ifndef    QUEX_SETTING_INDENTATION_STACK_SIZE
#      define QUEX_SETTING_INDENTATION_STACK_SIZE  (64)
#   endif
#   ifndef    QUEX_TYPE_INDENTATION
#      define QUEX_TYPE_INDENTATION  size_t
#   endif
#endif
  
/* Quex can determine whether certain handlers are not used at all.  If so,
 * computation time can be spared and quex comments the following options out.
 *                                                                           */
#define    __QUEX_OPTION_ON_ENTRY_HANDLER_PRESENT
#define    __QUEX_OPTION_ON_EXIT_HANDLER_PRESENT
/* #define QUEX_OPTION_CONVERTER_ICONV */
/* #define QUEX_OPTION_CONVERTER_ICU */
#if defined(QUEX_OPTION_CONVERTER_ICONV) || defined(QUEX_OPTION_CONVERTER_ICU)
#    ifndef     QUEX_SETTING_TRANSLATION_BUFFER_SIZE
#        define QUEX_SETTING_TRANSLATION_BUFFER_SIZE ((size_t)65536)
#    endif
#else
#    ifndef     QUEX_SETTING_TRANSLATION_BUFFER_SIZE
#        define QUEX_SETTING_TRANSLATION_BUFFER_SIZE ((size_t)0)
#    endif
#endif
/* The call the the 'new' function for the converter, i.e. the function that
 * allocates the converter and initializes it. If it is 0x0, this means that
 * the converter pointer is set to 0x0.                                      */
#ifndef QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW 
/* #   define QUEX_SETTING_BUFFER_FILLERS_CONVERTER_NEW  */
#endif

/* Begin of line pre-condition requires an extra flag in the buffer
 * structure. Only out-comment this in case of tough memory restrictions,
 * if no begin of line pre-condition is required.                            */
/* #define __QUEX_OPTION_SUPPORT_BEGIN_OF_LINE_PRE_CONDITION */

/* Following switch is also set, if a user defined converter is defined.     */
#ifndef    __QUEX_OPTION_CONVERTER
/* #define __QUEX_OPTION_CONVERTER */
#endif
#ifndef    __QUEX_OPTION_CONVERTER_HELPER
/* #define __QUEX_OPTION_CONVERTER_HELPER */
#endif

/* Type Definitions 
 *
 * CHARACTER TYPE: Type that represents the number of bytes used in the engine
 * to store a character. This is independent of the coding (ASCII, WinArabic,
 * ...)
 *
 * Both character types are clearly related, because they are supposed to
 * relate to the same type of objects in memory. The following example shows,
 * that 'uint8_t' and 'char' belong together, the lexeme type cannot be
 * 'uint8_t' because the std-string functions accept 'char*' and not 'uint8_t'
 * (which is most propperly 'unsigned char').
 *
 * NOTE: If in parallel another engine is built with other bytes per character
 * settings, then the engines will also include their own header with their own
 * definition of QUEX_TYPE_CHARACTER. Thus there is no danger at all.
 * Templatifying the lexer would be possible, but the author has in mind to
 * bring out a 'pure C' version of the quex generated engine. Thus templating
 * would make this goal harder achievable.
 *                                                                           */
#ifndef    QUEX_TYPE_CHARACTER
#   define QUEX_TYPE_CHARACTER          uint8_t
#   define QUEX_TYPE_CHARACTER_BACKUP   uint8_t
#endif 
#ifndef    QUEX_SETTING_CHARACTER_SIZE
    /* Size of a QUEX_TYPE_CHARACTER in bytes. A numeric value is required
     * here!  '-1' stands for 'Cannot be determined at code generation time'.
     * In this case, it must be relied upon 'sizeof(QUEX_TYPE_CHARACTER)'.   */
#   define QUEX_SETTING_CHARACTER_SIZE  1
#endif
#ifndef    QUEX_SETTING_CHARACTER_CODEC   
#   define QUEX_SETTING_CHARACTER_CODEC unicode
#endif
/* The following flag indicates that the engine is running on a specific
 * codec. Thus no converter is necessary. Use the flag to detect misuse.     */
/* #define __QUEX_OPTION_ENGINE_RUNNING_ON_CODEC */

#ifndef    QUEX_TYPE_ACCEPTANCE_ID
#   define QUEX_TYPE_ACCEPTANCE_ID   int
#endif

/* QUEX_TYPE_X  --> Type of X in global namespace 
 * QUEX_TYPE0_X --> Type of X in local namespace (namespace omitted)         */
#if defined(__QUEX_OPTION_PLAIN_C)
#   define QUEX_FUNCTION_PREFIX        quex_Lexer_
#   define QUEX_FUNCTION_DEF_PREFIX    quex_Lexer_
    /* In 'C' there are no namespaces, so namespaces are coded directly
     * into the type name. Both, global and local names are equal.           */
#   define QUEX_TYPE0_ANALYZER         struct quex_Lexer_tag
#   define QUEX_TYPE_ANALYZER          struct quex_Lexer_tag
#   define QUEX_NAME_COMPLETE_ANALYZER quex_Lexer
#   define QUEX_TYPE_DERIVED_ANALYZER  struct quex_Lexer_tag

#   define QUEX_TYPE0_TOKEN            struct quex_Token_tag
#   define QUEX_TYPE_TOKEN             struct quex_Token_tag
#   define QUEX_NAME_COMPLETE_TOKEN    quex_Token

#   define QUEX_NAMESPACE_MAIN         quex_Lexer
#   define QUEX_NAMESPACE_MAIN_OPEN   
#   define QUEX_NAMESPACE_MAIN_CLOSE  

#   define QUEX_NAMESPACE_TOKEN   
#   define QUEX_NAMESPACE_TOKEN_OPEN  
#   define QUEX_NAMESPACE_TOKEN_CLOSE 

#   define QUEX_NAMESPACE_LEXEME_NULL_OPEN     
#   define QUEX_NAMESPACE_LEXEME_NULL_CLOSE    
#   define QUEX_LEXEME_NULL_IN_ITS_NAMESPACE   quex_LexemeNullObject
#   define QUEX_LEXEME_NULL                    quex_LexemeNullObject

#   define QUEX_NAME(NAME)             quex_Lexer_ ## NAME
#   define QUEX_NAME_TOKEN(NAME)       quex_Token_ ## NAME
#   define QUEX_MEMBER(NAME)           void /* Undefined, not required */
    /* When creating a 'shared token class', macros are temporarily redefined.
     * This happens to generate converters in the token's namespace. After 
     * that, the original macros need to be restored. Thus: Backup!          */  
#   define QUEX_NAMESPACE_MAIN_OPEN_BACKUP   
#   define QUEX_NAMESPACE_MAIN_CLOSE_BACKUP  
#   define QUEX_FUNCTION_PREFIX_BACKUP       quex_Lexer_
#   define QUEX_FUNCTION_DEF_PREFIX_BACKUP   quex_Lexer_

#else
    /* Assumption: here is IN the namespace for definition.                  */
#   define QUEX_FUNCTION_DEF_PREFIX    
#   define QUEX_FUNCTION_PREFIX        ::quex::
    /* Add namespaces for the global names of the classes of analyzer
     * and token.                                                            */
#   define QUEX_TYPE0_ANALYZER         Lexer
#   define QUEX_TYPE_ANALYZER          ::quex::Lexer
#   define QUEX_NAME_COMPLETE_ANALYZER ::quex::Lexer
#   define QUEX_TYPE_DERIVED_ANALYZER  Lexer

#   define QUEX_TYPE0_TOKEN            Token
#   define QUEX_TYPE_TOKEN             ::quex::Token
#   define QUEX_NAME_COMPLETE_TOKEN    ::quex::Token

#   define QUEX_NAMESPACE_MAIN         ::quex
#   define QUEX_NAMESPACE_MAIN_OPEN    namespace quex {\

#   define QUEX_NAMESPACE_MAIN_CLOSE   } /* Closing Namespace 'quex' */\


#   define QUEX_NAMESPACE_TOKEN        ::quex
#   define QUEX_NAMESPACE_TOKEN_OPEN   namespace quex {\

#   define QUEX_NAMESPACE_TOKEN_CLOSE  } /* Closing Namespace 'quex' */\


#   define QUEX_NAMESPACE_LEXEME_NULL_OPEN     namespace quex {\

#   define QUEX_NAMESPACE_LEXEME_NULL_CLOSE    } /* Closing Namespace 'quex' */\

#   define QUEX_LEXEME_NULL_IN_ITS_NAMESPACE   LexemeNullObject
#   define QUEX_LEXEME_NULL                    ::quex::LexemeNullObject

#   define QUEX_NAME(NAME)             Lexer_ ## NAME
#   define QUEX_NAME_TOKEN(NAME)       Token_ ## NAME
#   define QUEX_MEMBER(NAME)           Lexer::NAME                
    /* When creating a 'shared token class', macros are temporarily redefined.
     * This happens to generate converters in the token's namespace. After 
     * that, the original macros need to be restored. Thus: Backup!          */  
#   define QUEX_NAMESPACE_MAIN_OPEN_BACKUP   namespace quex {\

#   define QUEX_NAMESPACE_MAIN_CLOSE_BACKUP  } /* Closing Namespace 'quex' */\

#   define QUEX_FUNCTION_PREFIX_BACKUP        
#   define QUEX_FUNCTION_DEF_PREFIX_BACKUP   ::quex::
#endif

#ifndef    QUEX_TYPE_TOKEN_ID
#   define QUEX_TYPE_TOKEN_ID        uint32_t
#endif
#ifndef    QUEX_TYPE_TOKEN_LINE_N
#   define QUEX_TYPE_TOKEN_LINE_N    size_t
#endif
#ifndef    QUEX_TYPE_TOKEN_COLUMN_N
#   define QUEX_TYPE_TOKEN_COLUMN_N  size_t
#endif

#include <quex/code_base/analyzer/configuration/derived>

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__CONFIGURATION____QUEX____LEXER */
