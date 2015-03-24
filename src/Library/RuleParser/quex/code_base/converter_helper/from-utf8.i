/* -*- C++ -*- vim: set syntax=cpp:
 * PURPOSE: 
 *
 * Provide the implementation of character and string converter functions
 * FROM utf8 to utf8, utf16, utf32, char, and wchar_t.
 *
 * STEPS:
 *
 * (1) Include the implementation of the character converters from utf8 
 *     to utf8, utf16, utf32, char, and wchar_t.
 *
 *     Use: "character-converter/from-utf8.i"
 *             --> implementation for utf8
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
 * ACKNOWLEDGEMENT: Parts of the following utf8 conversion have been derived from 
 *                  segments of the utf8 conversion library of Alexey Vatchenko 
 *                  <av@bsdua.org>.    
 *
 * 2010 (C) Frank-Rene Schaefer; 
 * ABSOLUTELY NO WARRANTY                                                    */
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF8_I) \
    ||   defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#        define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF8_I
#endif

#include <quex/code_base/converter_helper/from-utf8>

#define __QUEX_FROM       utf8
#define __QUEX_FROM_TYPE  uint8_t

QUEX_NAMESPACE_MAIN_OPEN

/* (1) Implement the character converters utf8 to utf8, utf16, utf32.
 *     (Note, that character converters are generated into namespace 'quex'.)*/
QUEX_INLINE void
/* DrainEnd pointer is not returned, since the increment is always '1' */
QUEX_CONVERTER_CHAR_DEF(utf8, utf8)(const uint8_t** input_pp, uint8_t** output_pp)
{
    /* Just for comformity with other encodings: Do nothing but copying. */
    if( (**input_pp & (uint8_t)0x80) == (uint8_t)0 ) {
        *((*output_pp)++) = *(*input_pp)++;
    }
    else if( **input_pp < (uint8_t)0xE0 ) { 
        *((*output_pp)++) = *(*input_pp)++;
        *((*output_pp)++) = *(*input_pp)++;
    }
    else if( **input_pp < (uint8_t)0xF0 ) { 
        *((*output_pp)++) = *(*input_pp)++;
        *((*output_pp)++) = *(*input_pp)++; 
        *((*output_pp)++) = *(*input_pp)++;
    }
    else {
        *((*output_pp)++) = *(*input_pp)++; 
        *((*output_pp)++) = *(*input_pp)++; 
        *((*output_pp)++) = *(*input_pp)++; 
        *((*output_pp)++) = *(*input_pp)++;
    }
}

QUEX_INLINE void
/* DrainEnd pointer is not returned, since the increment is always '1' */
QUEX_CONVERTER_CHAR_DEF(utf8, utf16)(const uint8_t** input_pp, uint16_t** output_pp)
{
    const uint8_t*  iterator = *input_pp;
    uint32_t        tmp = 0;

    if( (*iterator & (uint8_t)0x80) == (uint8_t)0 ) {
        /* Header: 0xxx.xxxx */
        **output_pp = (uint16_t)*(iterator++);

        ++(*output_pp);
    }
    else if( *iterator < (uint8_t)0xE0 ) { /* ... max: 1101.1111 --> 0xDF, next: 0xE0               */
        /*    110x.xxxx 10yy.yyyy 
         * => 0000.0xxx:xxyy.yyyy                                                          */
        **output_pp = (uint16_t)(( ((uint16_t)*(iterator++)) & (uint16_t)0x1F ) << 6);
        **output_pp = (uint16_t)((**output_pp) | (( ((uint16_t)*(iterator++)) & (uint16_t)0x3F )));

        ++(*output_pp);
    }
    else if( *iterator < (uint8_t)0xF0 ) { /* ... max: 1110.1111 --> 0xEF, next: 0xF0               */
        /*    1110.xxxx 10yy.yyyy 10zz.zzzz
         * => xxxx.yyyy:yyzz.zzzz                                                          */
        **output_pp = (uint16_t)(( ((uint16_t)*(iterator++)) & (uint16_t)0x0F ) << 12);
        **output_pp = (uint16_t)((**output_pp) | (( ((uint16_t)*(iterator++)) & (uint16_t)0x3F ) << 6)); 
        **output_pp = (uint16_t)((**output_pp) | (( ((uint16_t)*(iterator++)) & (uint16_t)0x3F )));

        ++(*output_pp);
    }
    else {
        /* Unicode standard defines only chars until 0x10ffff, so max(len(utf8char)) == 4.
         *
         * NO CHECK: if( *iterator < 0xF8 ) { ... max: 1111.0111 --> 0xF7, next: 0xF8 
         *
         *    1111.0uuu 10xx.xxxx 10yy.yyyy 10zz.zzzz
         * => 000u.uuxx:xxxx.yyyy:yyzz.zzzz                                                */

        /* It happens that the UTF8 domain with 4 bytes is >= 0x10000 which is the
         * starting domain for surrogates (i.e. what is mapped into 0xD800-0xE000         */
        tmp = (uint32_t)(      (((uint32_t)*(iterator++)) & (uint32_t)0x07 ) << 18); 
        tmp = (uint32_t)(tmp | (((uint32_t)*(iterator++)) & (uint32_t)0x3F ) << 12); 
        tmp = (uint32_t)(tmp | (((uint32_t)*(iterator++)) & (uint32_t)0x3F ) << 6); 
        tmp = (uint32_t)(tmp | (((uint32_t)*(iterator++)) & (uint32_t)0x3F ));

        tmp               = (uint32_t)(tmp - (uint32_t)0x10000);
        *((*output_pp)++) = (uint16_t)((tmp >> 10)             | (uint32_t)0xD800);
        *((*output_pp)++) = (uint16_t)((tmp & (uint32_t)0x3FF) | (uint32_t)0xDC00);
    }
    *input_pp = iterator;
}

QUEX_INLINE void
/* DrainEnd pointer is not returned, since the increment is always '1' */
QUEX_CONVERTER_CHAR_DEF(utf8, utf32)(const uint8_t** input_pp, uint32_t** output_pp)
{
    const uint8_t*  iterator = *input_pp;

    if( (*iterator & (uint8_t)0x80) == (uint8_t)0 ) {
        /* Header: 0xxx.xxxx */
        **output_pp = (uint32_t)*(iterator++);
    }
    else if( *iterator < (uint8_t)0xE0 ) { /* ... max: 1101.1111 --> 0xDF, next: 0xE0               */
        /*    110x.xxxx 10yy.yyyy 
         * => 0000.0xxx:xxyy.yyyy                                                          */
        **output_pp = (                          ( ((uint32_t)*(iterator++)) & (uint32_t)0x1F ) << 6);
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ));
    }
    else if( *iterator < (uint8_t)0xF0 ) { /* ... max: 1110.1111 --> 0xEF, next: 0xF0               */
        /*    1110.xxxx 10yy.yyyy 10zz.zzzz
         * => xxxx.yyyy:yyzz.zzzz                                                          */
        **output_pp = (                          ( ((uint32_t)*(iterator++)) & (uint32_t)0x0F ) << 12);
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ) << 6); 
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ));
    }
    else {
        /* Unicode standard defines only chars until 0x10ffff, so max(len(utf8char)) == 4.
         *
         * NO CHECK: if( *iterator < 0xF8 ) { ... max: 1111.0111 --> 0xF7, next: 0xF8 
         *
         *    1111.0uuu 10xx.xxxx 10yy.yyyy 10zz.zzzz
         * => 000u.uuxx:xxxx.yyyy:yyzz.zzzz                                                */
        **output_pp = (                          ( ((uint32_t)*(iterator++)) & (uint32_t)0x07 ) << 18); 
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ) << 12); 
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ) << 6); 
        **output_pp = (uint32_t)((**output_pp) | ( ((uint32_t)*(iterator++)) & (uint32_t)0x3F ));
    }
    ++(*output_pp);
    *input_pp = iterator;
}


/* (1b) Derive converters to char and wchar_t from the given set 
 *      of converters. (Generator uses __QUEX_FROM and QUEX_FROM_TYPE)      */
#include <quex/code_base/converter_helper/generator/character-converter-to-char-wchar_t.gi>

/* (2) Generate string converters to utf8, utf16, utf32 based on the
 *     definitions of the character converters.                             */
#include <quex/code_base/converter_helper/generator/implementations.gi>

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UTF8_I */
