/* -*- C++ -*- vim: set syntax=cpp:
 * PURPOSE: 
 *
 * Provide the implementation of character and string converter functions
 * FROM the buffer's unicode to utf8, utf16, utf32, char, and wchar_t.
 *
 * STEPS:
 *
 * (1) Include the COMPLETE implementation of a reference transformation.
 *
 *             "character-converter-from-utf8.i"
 *             "character-converter-from-utf16.i"
 *             "character-converter-from-utf32.i"
 *
 *     Sometimes the size of QUEX_TYPE_CHARACTER could not be determined at 
 *     code generation time, therefore all need to be included.
 *
 * (2) Map the functions of the pattern
 *
 *        QUEX_CONVERTER_CHAR(unicode, *)(....)
 *
 *     to what is appropriate from the given headers, e.g.
 *
 *        QUEX_CONVERTER_CHAR(utf8, *)(....)
 *
 * (2b) Generate converters towards 'char' and 'wchar_t'
 *
 *        generator/character-converter-to-char-wchar_t.gi
 *
 * (3) Derive string converter functions from character converters.
 *
 *        generator/implementations.gi
 *
 * These functions ARE DEPENDENT on QUEX_TYPE_CHARACTER.
 * => Thus, they are placed in the analyzer's namespace.
 *
 * 2010 (C) Frank-Rene Schaefer; 
 * ABSOLUTELY NO WARRANTY                                                    */
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UNICODE_BUFFER_I) \
    ||   defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#        define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UNICODE_BUFFER_I
#endif

#include <quex/code_base/converter_helper/from-unicode-buffer>

/* (1) Access the implementation of the converter that will implement
 *     the unicode conversion.                                               */
#include <quex/code_base/converter_helper/from-utf8.i>
#include <quex/code_base/converter_helper/from-utf16.i>
#include <quex/code_base/converter_helper/from-utf32.i>

QUEX_NAMESPACE_MAIN_OPEN

/* (2) Route the converters from 'unicode' to the implementing converter.    */
QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(unicode, utf8)(const QUEX_TYPE_CHARACTER**  input_pp, 
                                       uint8_t**                    output_pp)
{ 
    switch( sizeof(QUEX_TYPE_CHARACTER) )
    {
    case 1:  QUEX_CONVERTER_CHAR(utf8, utf8)((const uint8_t**)input_pp, output_pp);   break;
    case 2:  QUEX_CONVERTER_CHAR(utf16, utf8)((const uint16_t**)input_pp, output_pp); break;
    case 4:  QUEX_CONVERTER_CHAR(utf32, utf8)((const uint32_t**)input_pp, output_pp); break;
    default: QUEX_ERROR_EXIT("Cannot derive converter for given element size.");
    }
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(unicode, utf16)(const QUEX_TYPE_CHARACTER**  input_pp, 
                                        uint16_t**                   output_pp)
{ 
    switch( sizeof(QUEX_TYPE_CHARACTER) )
    {
    case 1:  QUEX_CONVERTER_CHAR(utf8, utf16)((const uint8_t**)input_pp, output_pp);   break;
    case 2:  QUEX_CONVERTER_CHAR(utf16, utf16)((const uint16_t**)input_pp, output_pp); break;
    case 4:  QUEX_CONVERTER_CHAR(utf32, utf16)((const uint32_t**)input_pp, output_pp); break;
    default: QUEX_ERROR_EXIT("Cannot derive converter for given element size.");
    }
}

QUEX_INLINE void
QUEX_CONVERTER_CHAR_DEF(unicode, utf32)(const QUEX_TYPE_CHARACTER**  input_pp, 
                                        uint32_t**                   output_pp)
{ 
    switch( sizeof(QUEX_TYPE_CHARACTER) )
    {
    case 1:  QUEX_CONVERTER_CHAR(utf8, utf32)((const uint8_t**)input_pp, output_pp);   break;
    case 2:  QUEX_CONVERTER_CHAR(utf16, utf32)((const uint16_t**)input_pp, output_pp); break;
    case 4:  QUEX_CONVERTER_CHAR(utf32, utf32)((const uint32_t**)input_pp, output_pp); break;
    default: QUEX_ERROR_EXIT("Cannot derive converter for given element size.");
    }
}


/* (2b) Derive converters to char and wchar_t from the given set 
 *      of converters. (Generator uses __QUEX_FROM and QUEX_FROM_TYPE)      */
#define  __QUEX_FROM       unicode
#define  __QUEX_FROM_TYPE  QUEX_TYPE_CHARACTER
#include <quex/code_base/converter_helper/generator/character-converter-to-char-wchar_t.gi>

/* (3) Generate string converters to utf8, utf16, utf32 based on the
 *     definitions of the character converters.                             */
#include <quex/code_base/converter_helper/generator/implementations.gi>

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__FROM_UNICODE_BUFFER_I */
