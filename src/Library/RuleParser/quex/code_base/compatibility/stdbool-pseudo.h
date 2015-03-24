/* PURPOSE: This header defines standard bool data types for use
 *          in plain 'C' lexical analyser engines. This is done
 *          here, in wise prediction that some compiler distributions
 *          may not provide this standard header. For the standard
 *          reference, please review: "The Open Group Base Specifications 
 *          Issue 6, IEEE Std 1003.1, 2004 Edition".
 *
 * (C) 2008  Frank-Rene Schaefer */           
#ifndef __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_PSEUDO_H
#define __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_PSEUDO_H

#if defined(__QUEX_OPTION_PLAIN_C)

/* According to the C99 Standard 'bool' would have to be defined as equal to
 * '_Bool' which is also defined in some standard. The author of this header,
 * though, found it particularly hard to determine whether the compiler obeys
 * these standards or not. Even prominent compilers, such as gcc at the time of
 * this writing, did not provide __STDC_VERSION__. Thus, the 'easy' solution to
 * simply define it as 'int' and call this file a 'stdbool-pseudo.h'. */
#ifndef bool
#   define bool int
#endif

#define true  ((int)(1))
#define false ((int)(0))

#define __bool_true_false_are_defined ((int)(1))

#endif /* __QUEX_OPTION_PLAIN_C */
#endif /* __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDBOOL_PSEUDO_H */
