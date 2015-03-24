#ifndef __QUEX_INCLUDE_GUARD__COMPATIBILITY__ICONV_ARGUMENT_TYPES_H
#define __QUEX_INCLUDE_GUARD__COMPATIBILITY__ICONV_ARGUMENT_TYPES_H


#if ! defined (__QUEX_OPTION_PLAIN_C)
    /* NOTE: At the time of this writing 'iconv' is delivered on different 
     *       systems with different definitions for the second argument. The 
     *       following 'hack' by Howard Jeng does the adaption automatically. */
    struct __Adapter_FuncIconv_SecondArgument {
        __Adapter_FuncIconv_SecondArgument(uint8_t ** in) : data(in) {}
        uint8_t ** data;
        operator const char **(void) const { return (const char **)(data); }
        operator       char **(void) const { return (      char **)(data); }
    }; 

#   define __QUEX_ADAPTER_ICONV_2ND_ARG(ARG) (__Adapter_FuncIconv_SecondArgument(ARG))

#else
    /* For 'C' things do not work out that elegantly ... */
#   if defined(QUEX_SETTING_ICONV_2ND_ARG_CONST_CHARPP)
#       define __QUEX_ADAPTER_ICONV_2ND_ARG(ARG)  ((const char**)(ARG))
#   else
#       define __QUEX_ADAPTER_ICONV_2ND_ARG(ARG)  ((char**)(ARG))
#   endif

#endif

#endif /* __QUEX_INCLUDE_GUARD__COMPATIBILITY__ICONV_ARGUMENT_TYPES_H */
