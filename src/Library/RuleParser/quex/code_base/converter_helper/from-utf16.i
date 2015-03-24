/* -*- C++ -*- vim: set syntax=cpp:
 * PURPOSE: 
 *
 * Provide the implementation of character and string converter functions
 * FROM utf16 to utf8, utf16, utf32, char, and wchar_t.
 *
 * STEPS:
 *
 * (1) Include the implementation of the character converters from utf16 
 *     to utf8, utf16, utf32, char, and wchar_t.
 *
 *     Use: "character-converter/from-utf16.i"
 *             --> implementation for utf16
 *
 *          "../generator/character-converter-char-wchar_t.gi"
 *             --> route 'char' and 'wchar_t' conversion to
 *                 one of the converters defined before.
 *
 * (2) Generate the implementation of the string converters in terms
 *     of those character converters.
 *
 *     Use: "../generator/implementation-string-converters.gi"
 *
 *          which uses
 *
 *              "../generator/string-converter.gi"
 *
 *          to implement each string converter from the given 
 *          character converters. 
 *
 * All functions are placed in the analyzer's namespace.
 *
 * 2010 (C) Frank-Rene Schaefer; 
 * ABSOLUTELY NO WARRANTY                                                    */
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF16_I) \
    ||   defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#        define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF16_I
#endif

#include <quex/code_base/converter_helper/from-utf16>

#define __QUEX_FROM       utf16
#define __QUEX_FROM_TYPE  uint16_t

/* (1) Implement the character converters utf8, utf16, utf32.
 *     (Note, that character converters are generated into namespace 'quex'.)*/
QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf16, utf8)(const uint16_t** input_pp, uint8_t** output_pp)
{
    uint32_t  x0      = (uint16_t)0;
    uint32_t  x1      = (uint16_t)0;
    uint32_t  unicode = (uint32_t)0;

    if ( **input_pp <= (uint16_t)0x7f ) {
        *((*output_pp)++) = (uint8_t)*(*input_pp);
        ++(*input_pp);

    } else if ( **input_pp <= (uint16_t)0x7ff ) {
        *((*output_pp)++) = (uint8_t)(0xC0 | (*(*input_pp) >> 6)); 
        *((*output_pp)++) = (uint8_t)(0x80 | (*(*input_pp) & (uint16_t)0x3F));
        ++(*input_pp);

    } else if ( **input_pp < (uint16_t)0xD800 ) { 
        *((*output_pp)++) = (uint8_t)(0xE0 |  *(*input_pp)                    >> 12);
        *((*output_pp)++) = (uint8_t)(0x80 | (*(*input_pp) & (uint16_t)0xFFF) >> 6);
        *((*output_pp)++) = (uint8_t)(0x80 | (*(*input_pp) & (uint16_t)0x3F));
        ++(*input_pp);

    } else if ( **input_pp < (uint16_t)0xE000 ) { 
        /* Characters > 0xFFFF need to be coded in two bytes by means of surrogates. */
        x0 = (uint32_t)(*(*input_pp)++ - (uint32_t)0xD800);
        x1 = (uint32_t)(*(*input_pp)++ - (uint32_t)0xDC00);
        unicode = (x0 << 10) + x1 + 0x10000;

        /* Assume that only character appear, that are defined in unicode. */
        __quex_assert(unicode <= (uint16_t)0x1FFFFF);

        *((*output_pp)++) = (uint8_t)(0xF0 | unicode                       >> 18);
        *((*output_pp)++) = (uint8_t)(0x80 | (unicode & (uint32_t)0x3FFFF) >> 12);
        *((*output_pp)++) = (uint8_t)(0x80 | (unicode & (uint32_t)0xFFF)   >> 6);
        *((*output_pp)++) = (uint8_t)(0x80 | (unicode & (uint32_t)0x3F));

    } else { 
        /* Always true: **input_pp <= 0xFFFF */
        *((*output_pp)++) = (uint8_t)(0xE0 |  *(*input_pp)                    >> 12);
        *((*output_pp)++) = (uint8_t)(0x80 | (*(*input_pp) & (uint16_t)0xFFF) >> 6);
        *((*output_pp)++) = (uint8_t)(0x80 | (*(*input_pp) & (uint16_t)0x3F));
        ++(*input_pp);
    } 
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf16, utf16)(const uint16_t**  input_pp, 
                                      uint16_t**        output_pp)
{
    if( **input_pp < (uint16_t)0xD800 || **input_pp >= (uint16_t)0xE000 ) {
        *((*output_pp)++) = *(*input_pp)++;
    } else { 
        *((*output_pp)++) = *(*input_pp)++;
        *((*output_pp)++) = *(*input_pp)++;
    }
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf16, utf32)(const uint16_t**  input_pp, 
                                      uint32_t**        output_pp)
{
    uint32_t  x0 = (uint32_t)0;
    uint32_t  x1 = (uint32_t)0;

    if( **input_pp < (uint16_t)0xD800 || **input_pp >= (uint16_t)0xE000 ) {
        *((*output_pp)++) = *(*input_pp)++;
    } else { 
        x0 = (uint32_t)(*(*input_pp)++) - (uint32_t)0xD800;
        x1 = (uint32_t)(*(*input_pp)++) - (uint32_t)0xDC00;
        *((*output_pp)++) = (x0 << 10) + x1 + (uint32_t)0x10000;
    }
}

/* (1b) Derive converters to char and wchar_t from the given set 
 *      of converters. (Generator uses __QUEX_FROM and QUEX_FROM_TYPE)      */
#include <quex/code_base/converter_helper/generator/character-converter-to-char-wchar_t.gi>

/* (2) Generate string converters to utf8, utf16, utf32 based on the
 *     definitions of the character converters.                             */
#include <quex/code_base/converter_helper/generator/implementations.gi>

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF16_I */
