#ifndef __QUEX_INCLUDE_GUARD__MULTI_I
#define __QUEX_INCLUDE_GUARD__MULTI_I
/* PURPOSE: 
 *
 * Solving the problem of multiple definitions of same functions when multiple
 * analyzers are linked together.
 *
 * HOWTO: 
 *
 * Compile all files of the lexical analyzer(s) with option 
 *
 *                         -DQUEX_OPTION_MULTI
 *
 * must be defined. In one single file include
 *
 *                     #include <quex/code_base/multi.i>
 *
 * Option 'QUEX_OPTION_MULTI' avoids the inclusion of some critical files 
 * during the implementation of all files. Instead, the explicit inclusion of 
 * 'multi.i' in one file delivers the reqired implementations. A good candidate
 * for '#includ'-ing is the interface module where the lexical analyzers are 
 * called.
 *
 * If the global quex namespace is chosen to be different from 'quex', the 
 * define option
 *                         -DQUEX_NAMESPACE_QUEX=mySpace
 *
 * In the above example the user's quex namespace is 'mySpace'. 
 *
 * EXPLANATION:
 *
 * This file includes implementations of functions which supposedly are exactly
 * the same for any instance of a lexical analyzer. They do not depend on an 
 * analyzer's configuration--except for the definition of the global namespace
 * of 'quex'.
 *
 * All of the functions below are located in the namespace quex. This may cause
 * problems when multiple analyzers are linked together. The option 
 * QUEX_OPTION_MULTI prevents those common headers from being included in any 
 * file. Including this file, however, allows for one single file to get all
 * implementations required.
 *
 * (C) Frank-Rene Schaefer                                                   */
#if ! defined(QUEX_OPTION_MULTI) && ! defined(QUEX_OPTION_MULTI_ALLOW_IMPLEMENTATION)
#    error "This file can only be included if option 'QUEX_OPTION_MULTI' has been set."
#endif

#include <quex/code_base/bom.i>
#include <quex/code_base/MemoryManager.i>

#endif /* __QUEX_INCLUDE_GUARD__MULTI_I__ */
