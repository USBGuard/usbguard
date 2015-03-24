/* -*- C++ -*- vim: set syntax=cpp:
 * PURPOSE: 
 *
 * Provide the implementation of identity string converter functions
 * for 'char' and 'wchar_t'.
 *
 *    QUEX_NAME(identical_string)(...)  for string and buffer 
 *    QUEX_NAME(identical_wstring)(...) for string and buffer 
 *
 * They exist mainly so that generalized code can be written in a homogenous
 * manner with the other converter functions. See 'token/CDefault.qx' for example.
 *
 * These functions ARE DEPENDENT on QUEX_TYPE_CHARACTER.
 * => Thus, they are placed in the analyzer's namespace.
 *
 * 2010 (C) Frank-Rene Schaefer; 
 * ABSOLUTELY NO WARRANTY                                                    */
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__IDENTITY_I) \
    ||   defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#if    ! defined(__QUEX_INCLUDE_GUARD__CONVERTER_HELPER__TMP_DISABLED)
#        define  __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__IDENTITY_I
#endif

#include <quex/code_base/converter_helper/identity>

QUEX_NAMESPACE_MAIN_OPEN

QUEX_INLINE void
QUEX_CONVERTER_STRING_DEF(identical, char)(const QUEX_TYPE_CHARACTER**  source_pp, 
                                           const QUEX_TYPE_CHARACTER*   SourceEnd, 
                                           char**                       drain_pp,  
                                           const char*                  DrainEnd)
{
    const QUEX_TYPE_CHARACTER* from_p = *source_pp;
    char*                      to_p   = *drain_pp;

    while( from_p != SourceEnd && to_p != DrainEnd )
    {
        *to_p++ = (char)*from_p++;    
    }
    *source_pp = from_p;
    *drain_pp  = to_p;
}

#if ! defined(__QUEX_OPTION_WCHAR_T_DISABLED)
QUEX_INLINE void
QUEX_CONVERTER_STRING_DEF(identical, wchar)(const QUEX_TYPE_CHARACTER**  source_pp, 
                                            const QUEX_TYPE_CHARACTER*   SourceEnd, 
                                            wchar_t**                    drain_pp,  
                                            const wchar_t*               DrainEnd)
{
    const QUEX_TYPE_CHARACTER* from_p = *source_pp;
    wchar_t*                   to_p   = *drain_pp;

    while( from_p != SourceEnd && to_p != DrainEnd )
    {
        *to_p++ = (wchar_t)*from_p++;    
    }
    *source_pp = from_p;
    *drain_pp  = to_p;
}

#endif

#if ! defined(__QUEX_OPTION_PLAIN_C)
    QUEX_INLINE std::basic_string<char>
    QUEX_CONVERTER_STRING_DEF(identical, char)(const std::basic_string<QUEX_TYPE_CHARACTER>& Source)
    { return std::string((const char*)Source.c_str()); }
#   if ! defined(__QUEX_OPTION_WCHAR_T_DISABLED)
    QUEX_INLINE std::basic_string<wchar_t>
    QUEX_CONVERTER_STRING_DEF(identical, wchar)(const std::basic_string<QUEX_TYPE_CHARACTER>& Source)
    { return std::wstring((const wchar_t*)Source.c_str()); }
#   endif
#endif

QUEX_NAMESPACE_MAIN_CLOSE

#endif /* __QUEX_INCLUDE_GUARD__CONVERTER_HELPER__IDENTITY_I */
