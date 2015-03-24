#ifndef __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDINT_H
#define __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDINT_H
/* NOTE: At the time of this writing (Aug. 2007) there are still some
 *       compilers that do not support C99 Standard completely and
 *       do not provided 'stdint.h' and 'stdint.h' along with their
 *       compiler package (e.g. a major Redmondian Company). For this
 *       reason this header creates som adaptions. */

#ifdef __cplusplus
extern "C" {
#endif

#if defined (_MSC_VER)
#   include <quex/code_base/compatibility/win/msc_stdint.h>
#elif defined(__BORLANDC__)
#   include <quex/code_base/compatibility/win/borland_stdint.h>
#elif defined(__sun) && defined(__sparc)
#   include <inttypes.h>      /* Thanks to sbellon@users.sourceforge.net */ 
#else
#   include <stdint.h>
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __QUEX_INCLUDE_GUARD__COMPATIBILITY__STDINT_H */
