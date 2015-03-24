/* -*- C++ -*- vim: set syntax=cpp:
 * PURPOSE: 
 *
 * Provide the implementation of character and string converter functions
 * FROM utf32 to utf8, utf16, utf32, char, and wchar_t.
 *
 * STEPS:
 *
 * (1) Include the implementation of the character converters from utf32 
 *     to utf8, utf16, utf32, char, and wchar_t.
 *
 *     Use: "character-converter/from-utf32.i"
 *             --> implementation for utf32
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
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF32_I) \
    ||   defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#        define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF32_I
#endif

#include <quex/code_base/converter_helper/from-utf32>

#define __QUEX_FROM       utf32
#define __QUEX_FROM_TYPE  uint32_t

QUEX_NAMESPACE_MAIN_OPEN

/* (1) Implement the character converters utf8, utf16, utf32.
 *     (Note, that character converters are generated into namespace 'quex'.)*/
QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf32, utf8)(const uint32_t**  input_pp, 
                                     uint8_t**         output_pp)
{
    /* PURPOSE: This function converts the specified unicode character
     *          into its utf8 representation. The result is stored
     *          at the location where utf8_result points to. Thus, the
     *          user has to make sure, that enough space is allocated!
     *
     * NOTE:    For general applicability let utf8_result point to a space
     *          of 7 bytes! This way you can store always a terminating
     *          zero after the last byte of the representation.
     *
     * RETURNS: Pointer to the fist position after the last character.      */
    uint32_t  Unicode = **input_pp;
    /**/

    if (Unicode <= 0x0000007f) {
        *((*output_pp)++) = (uint8_t)Unicode;
    } else if (Unicode <= 0x000007ff) {
        *((*output_pp)++) = (uint8_t)(0xC0 | (Unicode >> 6)); 
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0x3f));
    } else if (Unicode <= 0x0000ffff) {
        *((*output_pp)++) = (uint8_t)(0xE0 | Unicode           >> 12);
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0xFFF) >> 6);
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0x3F));
    } else { 
        /* Assume that only character appear, that are defined in unicode. */
        __quex_assert(Unicode <= (uint32_t)0x1FFFFF);
        /* No surrogate pairs (They are reserved even in non-utf16).       */
        __quex_assert(! (Unicode >= 0xd800 && Unicode <= 0xdfff) );

        *((*output_pp)++) = (uint8_t)(0xF0 | Unicode >> 18);
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0x3FFFF) >> 12);
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0xFFF)   >> 6);
        *((*output_pp)++) = (uint8_t)(0x80 | (Unicode & (uint32_t)0x3F));
    }
    /* NOTE: Do not check here for forbitten UTF-8 characters.
     * They cannot appear here because we do proper conversion. */
    ++(*input_pp);
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf32, utf16)(const uint32_t**  input_pp, 
                                      uint16_t**        output_pp)
{
    uint32_t   tmp = 0;

    if( **input_pp < 0x10000 ) {
        *((*output_pp)++) = (uint16_t)**input_pp;
    } else { 
        tmp             = (uint32_t)(**input_pp - (uint32_t)0x10000);

        *(((*output_pp)++)) = (uint16_t)((tmp >> 10)             | (uint16_t)0xD800);
        *(((*output_pp)++)) = (uint16_t)((tmp & (uint32_t)0x3FF) | (uint16_t)0xDC00);
    }
    ++(*input_pp);
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(utf32, utf32)(const uint32_t**  input_pp, 
                                      uint32_t**        output_pp)
{
    *((*output_pp)++) = (uint32_t)(*(*input_pp)++);
}

/* (1b) Derive converters to char and wchar_t from the given set 
 *      of converters. (Generator uses __QUEX_FROM and QUEX_FROM_TYPE)      */
#include <quex/code_base/converter_helper/generator/character-converter-to-char-wchar_t.gi>

/* (2) Generate string converters to utf8, utf16, utf32 based on the
 *     definitions of the character converters.                             */
#include <quex/code_base/converter_helper/generator/implementations.gi>

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF32_I */
