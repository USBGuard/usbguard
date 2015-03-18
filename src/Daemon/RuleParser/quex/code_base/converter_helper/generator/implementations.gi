/* PURPOSE:
 * 
 * Generate function implementations for all string converters FROM 
 * a given codec:
 *
 *        QUEX_CONVERTER_STRING(FROM, utf8)(...)
 *        QUEX_CONVERTER_STRING(FROM, utf16)(...)
 *        QUEX_CONVERTER_STRING(FROM, utf32)(...)
 *        QUEX_CONVERTER_STRING(FROM, char)(...)
 *        QUEX_CONVERTER_STRING(FROM, wchar_t)(...)
 *
 * It is Assumed that the character converters for each function are 
 * available!
 *
 * PARAMETERS:
 *
 *   __QUEX_FROM        Name of the input character codec.
 *   __QUEX_FROM_TYPE   Type of the input characters.
 *
 * (C) 2012 Frank-Rene Schaefer; ABSOLUTELY NO WARRANTY                      */ 
#if ! defined(__QUEX_FROM)
#    error "__QUEX_FROM must be defined!"
#elif ! defined(__QUEX_FROM_TYPE)
#    error "__QUEX_FROM_TYPE must be defined!"
#endif

#define  __QUEX_TO         utf8
#define  __QUEX_TO_TYPE    uint8_t
#include <quex/code_base/converter_helper/generator/string-converter.gi>
#define  __QUEX_TO         utf16
#define  __QUEX_TO_TYPE    uint16_t
#include <quex/code_base/converter_helper/generator/string-converter.gi>
#define  __QUEX_TO         utf32
#define  __QUEX_TO_TYPE    uint32_t
#include <quex/code_base/converter_helper/generator/string-converter.gi>
#define  __QUEX_TO         char
#define  __QUEX_TO_TYPE    char
#include <quex/code_base/converter_helper/generator/string-converter.gi>

#if ! defined(__QUEX_OPTION_WCHAR_T_DISABLED)
#   define  __QUEX_TO         wchar
#   define  __QUEX_TO_TYPE    wchar_t
#   include <quex/code_base/converter_helper/generator/string-converter.gi>
#   undef   __QUEX_TO     
#   undef   __QUEX_TO_TYPE 
#endif

#undef __QUEX_FROM
#undef __QUEX_FROM_TYPE

