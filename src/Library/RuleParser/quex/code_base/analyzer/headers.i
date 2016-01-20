/* -*- C++ -*- vim:set syntax=cpp:
 * (C) 2005-2010 Frank-Rene Schaefer
 * ABSOLUTELY NO WARRANTY                      */
#ifndef __QUEX_INCLUDE_GUARD__ANALYZER__HEADERS_I
#define __QUEX_INCLUDE_GUARD__ANALYZER__HEADERS_I

#if ! defined(__QUEX_INCLUDE_INDICATOR__ANALYZER__CONFIGURATION)
#   error "No configuration header included before this header."
#endif

#include    <quex/code_base/aux-string.i>

#include    <quex/code_base/analyzer/asserts.i>

/* Token sending happens only via macros, for flexibility. */
#include    <quex/code_base/analyzer/member/token-receiving.i>
#include    <quex/code_base/analyzer/member/mode-handling.i>
#include    <quex/code_base/analyzer/member/misc.i>
#include    <quex/code_base/analyzer/member/navigation.i>
#include    <quex/code_base/analyzer/struct/constructor.i>
#include    <quex/code_base/analyzer/struct/reset.i>

#include    <quex/code_base/analyzer/struct/basic.i>
#include    <quex/code_base/analyzer/Mode.i>

#include    <quex/code_base/buffer/Buffer.i>
#include    <quex/code_base/buffer/filler/BufferFiller.i>
#include    <quex/code_base/buffer/loader/ByteLoader>

#if defined(__QUEX_OPTION_CONVERTER)
#   include <quex/code_base/buffer/filler/BufferFiller_Converter.i>
#endif

#ifdef      __QUEX_OPTION_COUNTER
#   include <quex/code_base/analyzer/Counter.i>
#endif
#ifdef      QUEX_OPTION_INCLUDE_STACK
#   include <quex/code_base/analyzer/struct/include-stack.i>
#endif
#ifdef      QUEX_OPTION_STRING_ACCUMULATOR
#   include <quex/code_base/analyzer/Accumulator.i>
#endif
#ifdef      QUEX_OPTION_TOKEN_POLICY_QUEUE
#   include <quex/code_base/token/TokenQueue.i>
#endif
#ifdef      QUEX_OPTION_POST_CATEGORIZER
#   include <quex/code_base/analyzer/PostCategorizer.i>
#endif
#ifdef      QUEX_OPTION_INDENTATION_TRIGGER
#   include <quex/code_base/analyzer/member/on_indentation.i>
#endif

#endif /* __QUEX_INCLUDE_GUARD__ANALYZER__HEADERS_I */
