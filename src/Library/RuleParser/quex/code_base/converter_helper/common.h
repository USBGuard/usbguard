/* Common include headers for all converter functions. 
 *
 * (C) Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                                                    */
#ifndef  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__COMMON_H
#define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__COMMON_H

#include <quex/code_base/definitions>
#include <quex/code_base/compatibility/stdint.h>
#include <quex/code_base/asserts>
#if ! defined(__QUEX_OPTION_PLAIN_C)
#   include <string>
#endif

/* Signalize: Converters are available! 
 *
 * Since converters can come from multiple sources it is not possible to 
 * include in a header always the right converters. To get along, users of
 * converters must at least check that before they are included, the converter
 * headers have been made available--by inclusion before they were included  */
#define QUEX_CHARACTER_CONVERTER_DECLARED

#endif                   /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__COMMON_H */
