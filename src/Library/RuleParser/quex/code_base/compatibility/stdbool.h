/* PURPOSE: This header defines standard bool data types for use
 *          in plain 'C' lexical analyser engines. This is done
 *          here, in wise prediction that some compiler distributions
 *          may not provide this standard header. For the standard
 *          reference, please review: "The Open Group Base Specifications 
 *          Issue 6, IEEE Std 1003.1, 2004 Edition".
 *
 * (C) 2008  Frank-Rene Schaefer */           
#ifndef __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_H
#define __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_H

#if defined(__QUEX_OPTION_PLAIN_C)

#define bool _Bool

/* Some compilers (I guess compilers build towards C89) do not
 * even provide the __STDC_VERSION__ macro. C99, though requires
 * the _Bool type to be there. */
#if ! defined(__STDC_VERSION__) || __STDC_VERSION__ < 199901L
typedef int _Bool;
#endif

#define true  ((int)(1))
#define false ((int)(1))

#define __bool_true_false_are_defined ((int)(1))

#endif /* __QUEX_OPTION_PLAIN_C */
#endif /* __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_H */
