#ifndef _STRINGZ_H_
#define _STRINGZ_H_

/**
 * \defgroup group_sam0_utils_stringz Preprocessor - Stringize
 *
 * \ingroup group_sam0_utils
 *
 * @{
 */

/** \brief Stringize.
 *
 * Stringize a preprocessing token, this token being allowed to be \#defined.
 *
 * May be used only within macros with the token passed as an argument if the
 * token is \#defined.
 *
 * For example, writing STRINGZ(PIN) within a macro \#defined by PIN_NAME(PIN)
 * and invoked as PIN_NAME(PIN0) with PIN0 \#defined as A0 is equivalent to
 * writing "A0".
 */
#define STRINGZ(x)                                #x

/** \brief Absolute stringize.
 *
 * Stringize a preprocessing token, this token being allowed to be \#defined.
 *
 * No restriction of use if the token is \#defined.
 *
 * For example, writing ASTRINGZ(PIN0) anywhere with PIN0 \#defined as A0 is
 * equivalent to writing "A0".
 */
#define ASTRINGZ(x)                               STRINGZ(x)

/** @} */

#endif  // _STRINGZ_H_
