/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#include <string.h>
#include "uECC_ll.h"

#ifndef uECC_PLATFORM
    #if defined(__AVR__) && __AVR__
        #define uECC_PLATFORM uECC_avr
    #elif defined(__thumb2__) || defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
        #define uECC_PLATFORM uECC_arm_thumb2
    #elif defined(__thumb__)
        #define uECC_PLATFORM uECC_arm_thumb
    #elif defined(__arm__) || defined(_M_ARM)
        #define uECC_PLATFORM uECC_arm
    #elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__I86__)
        #define uECC_PLATFORM uECC_x86
    #elif defined(__amd64__) || defined(_M_X64)
        #define uECC_PLATFORM uECC_x86_64
    #else
        #define uECC_PLATFORM uECC_arch_other
    #endif
#endif

#define uECC_WORD_SIZE 4

#if __STDC_VERSION__ >= 199901L
    #define RESTRICT restrict
#else
    #define RESTRICT
#endif

#define SUPPORTS_INT128 0

#define MAX_TRIES 64

/* #if uECC_WORD_SIZE == 4 */
typedef uint32_t uECC_word_t;
typedef uint64_t uECC_dword_t;
typedef unsigned wordcount_t;
typedef int swordcount_t;
typedef int bitcount_t;
typedef int cmpresult_t;

#define HIGH_BIT_SET 0x80000000
#define uECC_WORD_BITS 32
#define uECC_WORD_BITS_SHIFT 5
#define uECC_WORD_BITS_MASK 0x01F

#define uECC_WORDS_1 5
#define uECC_WORDS_2 6
#define uECC_WORDS_3 8
#define uECC_WORDS_4 8
#define uECC_WORDS_5 7

#define uECC_N_WORDS_1 6
#define uECC_N_WORDS_2 6
#define uECC_N_WORDS_3 8
#define uECC_N_WORDS_4 8
#define uECC_N_WORDS_5 7

#define Curve_P_1 {0x7FFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_P_2 {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_P_3 {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, \
                   0x00000000, 0x00000000, 0x00000001, 0xFFFFFFFF}
#define Curve_P_4 {0xFFFFFC2F, 0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_P_5 {0x00000001, 0x00000000, 0x00000000, 0xFFFFFFFF, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}

#define Curve_B_1 {0xC565FA45, 0x81D4D4AD, 0x65ACF89F, 0x54BD7A8B, 0x1C97BEFC}
#define Curve_B_2 {0xC146B9B1, 0xFEB8DEEC, 0x72243049, 0x0FA7E9AB, 0xE59C80E7, 0x64210519}
#define Curve_B_3 {0x27D2604B, 0x3BCE3C3E, 0xCC53B0F6, 0x651D06B0, \
                   0x769886BC, 0xB3EBBD55, 0xAA3A93E7, 0x5AC635D8}
#define Curve_B_4 {0x00000007, 0x00000000, 0x00000000, 0x00000000, \
                   0x00000000, 0x00000000, 0x00000000, 0x00000000}
#define Curve_B_5 {0x2355FFB4, 0x270B3943, 0xD7BFD8BA, 0x5044B0B7, \
                   0xF5413256, 0x0C04B3AB, 0xB4050A85}

#define Curve_G_1 { \
    {0x13CBFC82, 0x68C38BB9, 0x46646989, 0x8EF57328, 0x4A96B568}, \
    {0x7AC5FB32, 0x04235137, 0x59DCC912, 0x3168947D, 0x23A62855}}

#define Curve_G_2 { \
    {0x82FF1012, 0xF4FF0AFD, 0x43A18800, 0x7CBF20EB, 0xB03090F6, 0x188DA80E}, \
    {0x1E794811, 0x73F977A1, 0x6B24CDD5, 0x631011ED, 0xFFC8DA78, 0x07192B95}}

#define Curve_G_3 { \
    {0xD898C296, 0xF4A13945, 0x2DEB33A0, 0x77037D81,  \
     0x63A440F2, 0xF8BCE6E5, 0xE12C4247, 0x6B17D1F2}, \
    {0x37BF51F5, 0xCBB64068, 0x6B315ECE, 0x2BCE3357,  \
     0x7C0F9E16, 0x8EE7EB4A, 0xFE1A7F9B, 0x4FE342E2}}

#define Curve_G_4 { \
    {0x16F81798, 0x59F2815B, 0x2DCE28D9, 0x029BFCDB,  \
     0xCE870B07, 0x55A06295, 0xF9DCBBAC, 0x79BE667E}, \
    {0xFB10D4B8, 0x9C47D08F, 0xA6855419, 0xFD17B448,  \
     0x0E1108A8, 0x5DA4FBFC, 0x26A3C465, 0x483ADA77}}

#define Curve_G_5 { \
    {0x115C1D21, 0x343280D6, 0x56C21122, 0x4A03C1D3, \
     0x321390B9, 0x6BB4BF7F, 0xB70E0CBD}, \
    {0x85007E34, 0x44D58199, 0x5A074764, 0xCD4375A0, \
     0x4C22DFE6, 0xB5F723FB, 0xBD376388}}

#define Curve_N_1 {0xCA752257, 0xF927AED3, 0x0001F4C8, 0x00000000, 0x00000000, 0x00000001}
#define Curve_N_2 {0xB4D22831, 0x146BC9B1, 0x99DEF836, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_N_3 {0xFC632551, 0xF3B9CAC2, 0xA7179E84, 0xBCE6FAAD, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF}
#define Curve_N_4 {0xD0364141, 0xBFD25E8C, 0xAF48A03B, 0xBAAEDCE6, \
                   0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}
#define Curve_N_5 {0x5C5C2A3D, 0x13DD2945, 0xE0B8F03E, 0xFFFF16A2, \
                   0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}

/* #endif */

#define uECC_WORDS uECC_CONCAT(uECC_WORDS_, uECC_CURVE)
#define uECC_N_WORDS uECC_CONCAT(uECC_N_WORDS_, uECC_CURVE)

typedef struct EccPoint {
    uECC_word_t x[uECC_WORDS];
    uECC_word_t y[uECC_WORDS];
} EccPoint;

static const uECC_word_t curve_p[uECC_WORDS] = uECC_CONCAT(Curve_P_, uECC_CURVE);
static const uECC_word_t curve_b[uECC_WORDS] = uECC_CONCAT(Curve_B_, uECC_CURVE);
static const EccPoint curve_G = uECC_CONCAT(Curve_G_, uECC_CURVE);
static const uECC_word_t curve_n[uECC_N_WORDS] = uECC_CONCAT(Curve_N_, uECC_CURVE);

static void vli_clear(uECC_word_t *vli);
static uECC_word_t vli_isZero(const uECC_word_t *vli);
static uECC_word_t vli_testBit(const uECC_word_t *vli, bitcount_t bit);
static void vli_set(uECC_word_t *dest, const uECC_word_t *src);
static cmpresult_t vli_cmp(const uECC_word_t *left, const uECC_word_t *right);
static void vli_rshift1(uECC_word_t *vli);
static uECC_word_t vli_add(uECC_word_t *result,
                           const uECC_word_t *left,
                           const uECC_word_t *right);
static uECC_word_t vli_sub(uECC_word_t *result,
                           const uECC_word_t *left,
                           const uECC_word_t *right);
static void vli_mult(uECC_word_t *result, const uECC_word_t *left, const uECC_word_t *right);
static void vli_modAdd(uECC_word_t *result,
                       const uECC_word_t *left,
                       const uECC_word_t *right,
                       const uECC_word_t *mod);
static void vli_modSub(uECC_word_t *result,
                       const uECC_word_t *left,
                       const uECC_word_t *right,
                       const uECC_word_t *mod);
static void vli_mmod_fast(uECC_word_t *RESTRICT result, uECC_word_t *RESTRICT product);
static void vli_modMult_fast(uECC_word_t *result,
                             const uECC_word_t *left,
                             const uECC_word_t *right);
static void vli_modInv(uECC_word_t *result, const uECC_word_t *input, const uECC_word_t *mod);
#if uECC_SQUARE_FUNC
static void vli_square(uECC_word_t *result, const uECC_word_t *left);
static void vli_modSquare_fast(uECC_word_t *result, const uECC_word_t *left);
#endif

static int default_RNG(uint8_t *dest, unsigned size) {
    return 0;
}


static uECC_RNG_Function g_rng_function = &default_RNG;

void uECC_set_rng_ll(uECC_RNG_Function rng_function) {
    g_rng_function = rng_function;
}

#ifdef __GNUC__ /* Only support GCC inline asm for now */
    #if (uECC_ASM && (uECC_PLATFORM == uECC_arm || uECC_PLATFORM == uECC_arm_thumb || \
                      uECC_PLATFORM == uECC_arm_thumb2))
        #include "asm_arm.inc"
    #endif
#endif

#if !defined(asm_clear) || !asm_clear
static void vli_clear(uECC_word_t *vli) {
    wordcount_t i;
    for (i = 0; i < uECC_WORDS; ++i) {
        vli[i] = 0;
    }
}
#endif

/* Returns 1 if vli == 0, 0 otherwise. */
#if !defined(asm_isZero) || !asm_isZero
static uECC_word_t vli_isZero(const uECC_word_t *vli) {
    wordcount_t i;
    for (i = 0; i < uECC_WORDS; ++i) {
        if (vli[i]) {
            return 0;
        }
    }
    return 1;
}
#endif

/* Returns nonzero if bit 'bit' of vli is set. */
#if !defined(asm_testBit) || !asm_testBit
static uECC_word_t vli_testBit(const uECC_word_t *vli, bitcount_t bit) {
    return (vli[bit >> uECC_WORD_BITS_SHIFT] & ((uECC_word_t)1 << (bit & uECC_WORD_BITS_MASK)));
}
#endif

/* Sets dest = src. */
#if !defined(asm_set) || !asm_set
static void vli_set(uECC_word_t *dest, const uECC_word_t *src) {
    wordcount_t i;
    for (i = 0; i < uECC_WORDS; ++i) {
        dest[i] = src[i];
    }
}
#endif

/* Returns sign of left - right. */
#if !defined(asm_cmp) || !asm_cmp
static cmpresult_t vli_cmp(const uECC_word_t *left, const uECC_word_t *right) {
    swordcount_t i;
    for (i = uECC_WORDS - 1; i >= 0; --i) {
        if (left[i] > right[i]) {
            return 1;
        } else if (left[i] < right[i]) {
            return -1;
        }
    }
    return 0;
}
#endif

/* Computes vli = vli >> 1. */
#if !defined(asm_rshift1) || !asm_rshift1
static void vli_rshift1(uECC_word_t *vli) {
    uECC_word_t *end = vli;
    uECC_word_t carry = 0;

    vli += uECC_WORDS;
    while (vli-- > end) {
        uECC_word_t temp = *vli;
        *vli = (temp >> 1) | carry;
        carry = temp << (uECC_WORD_BITS - 1);
    }
}
#endif

/* Computes result = left + right, returning carry. Can modify in place. */
#if !defined(asm_add) || !asm_add
static uECC_word_t vli_add(uECC_word_t *result, const uECC_word_t *left, const uECC_word_t *right) {
    uECC_word_t carry = 0;
    wordcount_t i;
    for (i = 0; i < uECC_WORDS; ++i) {
        uECC_word_t sum = left[i] + right[i] + carry;
        if (sum != left[i]) {
            carry = (sum < left[i]);
        }
        result[i] = sum;
    }
    return carry;
}
#endif

/* Computes result = left - right, returning borrow. Can modify in place. */
#if !defined(asm_sub) || !asm_sub
static uECC_word_t vli_sub(uECC_word_t *result, const uECC_word_t *left, const uECC_word_t *right) {
    uECC_word_t borrow = 0;
    wordcount_t i;
    for (i = 0; i < uECC_WORDS; ++i) {
        uECC_word_t diff = left[i] - right[i] - borrow;
        if (diff != left[i]) {
            borrow = (diff > left[i]);
        }
        result[i] = diff;
    }
    return borrow;
}
#endif

#if (!asm_mult || (uECC_SQUARE_FUNC && !asm_square) || uECC_CURVE == uECC_secp256k1)
static void muladd(uECC_word_t a,
                   uECC_word_t b,
                   uECC_word_t *r0,
                   uECC_word_t *r1,
                   uECC_word_t *r2) {
    uECC_dword_t p = (uECC_dword_t)a * b;
    uECC_dword_t r01 = ((uECC_dword_t)(*r1) << uECC_WORD_BITS) | *r0;
    r01 += p;
    *r2 += (r01 < p);
    *r1 = r01 >> uECC_WORD_BITS;
    *r0 = (uECC_word_t)r01;
}
#define muladd_exists 1
#endif

#if !asm_mult
static void vli_mult(uECC_word_t *result, const uECC_word_t *left, const uECC_word_t *right) {
    uECC_word_t r0 = 0;
    uECC_word_t r1 = 0;
    uECC_word_t r2 = 0;
    wordcount_t i, k;

    /* Compute each digit of result in sequence, maintaining the carries. */
    for (k = 0; k < uECC_WORDS; ++k) {
        for (i = 0; i <= k; ++i) {
            muladd(left[i], right[k - i], &r0, &r1, &r2);
        }
        result[k] = r0;
        r0 = r1;
        r1 = r2;
        r2 = 0;
    }
    for (k = uECC_WORDS; k < uECC_WORDS * 2 - 1; ++k) {
        for (i = (k + 1) - uECC_WORDS; i < uECC_WORDS; ++i) {
            muladd(left[i], right[k - i], &r0, &r1, &r2);
        }
        result[k] = r0;
        r0 = r1;
        r1 = r2;
        r2 = 0;
    }
    result[uECC_WORDS * 2 - 1] = r0;
}
#endif

#if uECC_SQUARE_FUNC

#if !asm_square
static void mul2add(uECC_word_t a,
                    uECC_word_t b,
                    uECC_word_t *r0,
                    uECC_word_t *r1,
                    uECC_word_t *r2) {
    uECC_dword_t p = (uECC_dword_t)a * b;
    uECC_dword_t r01 = ((uECC_dword_t)(*r1) << uECC_WORD_BITS) | *r0;
    *r2 += (p >> (uECC_WORD_BITS * 2 - 1));
    p *= 2;
    r01 += p;
    *r2 += (r01 < p);
    *r1 = r01 >> uECC_WORD_BITS;
    *r0 = (uECC_word_t)r01;
}

static void vli_square(uECC_word_t *result, const uECC_word_t *left) {
    uECC_word_t r0 = 0;
    uECC_word_t r1 = 0;
    uECC_word_t r2 = 0;

    wordcount_t i, k;

    for (k = 0; k < uECC_WORDS * 2 - 1; ++k) {
        uECC_word_t min = (k < uECC_WORDS ? 0 : (k + 1) - uECC_WORDS);
        for (i = min; i <= k && i <= k - i; ++i) {
            if (i < k-i) {
                mul2add(left[i], left[k - i], &r0, &r1, &r2);
            } else {
                muladd(left[i], left[k - i], &r0, &r1, &r2);
            }
        }
        result[k] = r0;
        r0 = r1;
        r1 = r2;
        r2 = 0;
    }

    result[uECC_WORDS * 2 - 1] = r0;
}
#endif

#else /* uECC_SQUARE_FUNC */

#define vli_square(result, left, size) vli_mult((result), (left), (left), (size))

#endif /* uECC_SQUARE_FUNC */


/* Computes result = (left + right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap mod. */
#if !defined(asm_modAdd) || !asm_modAdd
static void vli_modAdd(uECC_word_t *result,
                       const uECC_word_t *left,
                       const uECC_word_t *right,
                       const uECC_word_t *mod) {
    uECC_word_t carry = vli_add(result, left, right);
    if (carry || vli_cmp(result, mod) >= 0) {
        /* result > mod (result = mod + remainder), so subtract mod to get remainder. */
        vli_sub(result, result, mod);
    }
}
#endif

/* Computes result = (left - right) % mod.
   Assumes that left < mod and right < mod, and that result does not overlap mod. */
#if !defined(asm_modSub) || !asm_modSub
static void vli_modSub(uECC_word_t *result,
                       const uECC_word_t *left,
                       const uECC_word_t *right,
                       const uECC_word_t *mod) {
    uECC_word_t l_borrow = vli_sub(result, left, right);
    if (l_borrow) {
        /* In this case, result == -diff == (max int) - diff. Since -x % d == d - x,
           we can get the correct result from result + mod (with overflow). */
        vli_add(result, result, mod);
    }
}
#endif

#if !defined(asm_modSub_fast) || !asm_modSub_fast
    #define vli_modSub_fast(result, left, right) vli_modSub((result), (left), (right), curve_p)
#endif

#if !defined(asm_mmod_fast) || !asm_mmod_fast

#if uECC_CURVE == uECC_secp256r1

/* Computes result = product % curve_p
   from http://www.nsa.gov/ia/_files/nist-routines.pdf */
#if uECC_WORD_SIZE == 4
static void vli_mmod_fast(uint32_t *RESTRICT result, uint32_t *RESTRICT product) {
    uint32_t tmp[uECC_WORDS];
    int carry;

    /* t */
    vli_set(result, product);

    /* s1 */
    tmp[0] = tmp[1] = tmp[2] = 0;
    tmp[3] = product[11];
    tmp[4] = product[12];
    tmp[5] = product[13];
    tmp[6] = product[14];
    tmp[7] = product[15];
    carry = vli_add(tmp, tmp, tmp);
    carry += vli_add(result, result, tmp);

    /* s2 */
    tmp[3] = product[12];
    tmp[4] = product[13];
    tmp[5] = product[14];
    tmp[6] = product[15];
    tmp[7] = 0;
    carry += vli_add(tmp, tmp, tmp);
    carry += vli_add(result, result, tmp);

    /* s3 */
    tmp[0] = product[8];
    tmp[1] = product[9];
    tmp[2] = product[10];
    tmp[3] = tmp[4] = tmp[5] = 0;
    tmp[6] = product[14];
    tmp[7] = product[15];
    carry += vli_add(result, result, tmp);

    /* s4 */
    tmp[0] = product[9];
    tmp[1] = product[10];
    tmp[2] = product[11];
    tmp[3] = product[13];
    tmp[4] = product[14];
    tmp[5] = product[15];
    tmp[6] = product[13];
    tmp[7] = product[8];
    carry += vli_add(result, result, tmp);

    /* d1 */
    tmp[0] = product[11];
    tmp[1] = product[12];
    tmp[2] = product[13];
    tmp[3] = tmp[4] = tmp[5] = 0;
    tmp[6] = product[8];
    tmp[7] = product[10];
    carry -= vli_sub(result, result, tmp);

    /* d2 */
    tmp[0] = product[12];
    tmp[1] = product[13];
    tmp[2] = product[14];
    tmp[3] = product[15];
    tmp[4] = tmp[5] = 0;
    tmp[6] = product[9];
    tmp[7] = product[11];
    carry -= vli_sub(result, result, tmp);

    /* d3 */
    tmp[0] = product[13];
    tmp[1] = product[14];
    tmp[2] = product[15];
    tmp[3] = product[8];
    tmp[4] = product[9];
    tmp[5] = product[10];
    tmp[6] = 0;
    tmp[7] = product[12];
    carry -= vli_sub(result, result, tmp);

    /* d4 */
    tmp[0] = product[14];
    tmp[1] = product[15];
    tmp[2] = 0;
    tmp[3] = product[9];
    tmp[4] = product[10];
    tmp[5] = product[11];
    tmp[6] = 0;
    tmp[7] = product[13];
    carry -= vli_sub(result, result, tmp);

    if (carry < 0) {
        do {
            carry += vli_add(result, result, curve_p);
        } while (carry < 0);
    } else {
        while (carry || vli_cmp(curve_p, result) != 1) {
            carry -= vli_sub(result, result, curve_p);
        }
    }
}
#endif /* uECC_WORD_SIZE */
#endif /* uECC_CURVE */
#endif /* !asm_mmod_fast */

/* Computes result = (left * right) % curve_p. */
static void vli_modMult_fast(uECC_word_t *result,
                             const uECC_word_t *left,
                             const uECC_word_t *right) {
    uECC_word_t product[2 * uECC_WORDS];
    vli_mult(product, left, right);
    vli_mmod_fast(result, product);
}

#if uECC_SQUARE_FUNC

/* Computes result = left^2 % curve_p. */
static void vli_modSquare_fast(uECC_word_t *result, const uECC_word_t *left) {
    uECC_word_t product[2 * uECC_WORDS];
    vli_square(product, left);
    vli_mmod_fast(result, product);
}

#else /* uECC_SQUARE_FUNC */

#define vli_modSquare_fast(result, left) vli_modMult_fast((result), (left), (left))

#endif /* uECC_SQUARE_FUNC */


#define EVEN(vli) (!(vli[0] & 1))
/* Computes result = (1 / input) % mod. All VLIs are the same size.
   See "From Euclid's GCD to Montgomery Multiplication to the Great Divide"
   https://labs.oracle.com/techrep/2001/smli_tr-2001-95.pdf */
#if !defined(asm_modInv) || !asm_modInv
static void vli_modInv(uECC_word_t *result, const uECC_word_t *input, const uECC_word_t *mod) {
    uECC_word_t a[uECC_WORDS], b[uECC_WORDS], u[uECC_WORDS], v[uECC_WORDS];
    uECC_word_t carry;
    cmpresult_t cmpResult;

    if (vli_isZero(input)) {
        vli_clear(result);
        return;
    }

    vli_set(a, input);
    vli_set(b, mod);
    vli_clear(u);
    u[0] = 1;
    vli_clear(v);
    while ((cmpResult = vli_cmp(a, b)) != 0) {
        carry = 0;
        if (EVEN(a)) {
            vli_rshift1(a);
            if (!EVEN(u)) {
                carry = vli_add(u, u, mod);
            }
            vli_rshift1(u);
            if (carry) {
                u[uECC_WORDS - 1] |= HIGH_BIT_SET;
            }
        } else if (EVEN(b)) {
            vli_rshift1(b);
            if (!EVEN(v)) {
                carry = vli_add(v, v, mod);
            }
            vli_rshift1(v);
            if (carry) {
                v[uECC_WORDS - 1] |= HIGH_BIT_SET;
            }
        } else if (cmpResult > 0) {
            vli_sub(a, a, b);
            vli_rshift1(a);
            if (vli_cmp(u, v) < 0) {
                vli_add(u, u, mod);
            }
            vli_sub(u, u, v);
            if (!EVEN(u)) {
                carry = vli_add(u, u, mod);
            }
            vli_rshift1(u);
            if (carry) {
                u[uECC_WORDS - 1] |= HIGH_BIT_SET;
            }
        } else {
            vli_sub(b, b, a);
            vli_rshift1(b);
            if (vli_cmp(v, u) < 0) {
                vli_add(v, v, mod);
            }
            vli_sub(v, v, u);
            if (!EVEN(v)) {
                carry = vli_add(v, v, mod);
            }
            vli_rshift1(v);
            if (carry) {
                v[uECC_WORDS - 1] |= HIGH_BIT_SET;
            }
        }
    }
    vli_set(result, u);
}
#endif /* !asm_modInv */

/* ------ Point operations ------ */

/* Returns 1 if 'point' is the point at infinity, 0 otherwise. */
static cmpresult_t EccPoint_isZero(const EccPoint *point) {
    return (vli_isZero(point->x) && vli_isZero(point->y));
}

/* Point multiplication algorithm using Montgomery's ladder with co-Z coordinates.
From http://eprint.iacr.org/2011/338.pdf
*/

/* Double in place */
static void EccPoint_double_jacobian(uECC_word_t * RESTRICT X1,
                                     uECC_word_t * RESTRICT Y1,
                                     uECC_word_t * RESTRICT Z1) {
    /* t1 = X, t2 = Y, t3 = Z */
    uECC_word_t t4[uECC_WORDS];
    uECC_word_t t5[uECC_WORDS];

    if (vli_isZero(Z1)) {
        return;
    }

    vli_modSquare_fast(t4, Y1);   /* t4 = y1^2 */
    vli_modMult_fast(t5, X1, t4); /* t5 = x1*y1^2 = A */
    vli_modSquare_fast(t4, t4);   /* t4 = y1^4 */
    vli_modMult_fast(Y1, Y1, Z1); /* t2 = y1*z1 = z3 */
    vli_modSquare_fast(Z1, Z1);   /* t3 = z1^2 */

    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = x1 + z1^2 */
    vli_modAdd(Z1, Z1, Z1, curve_p); /* t3 = 2*z1^2 */
    vli_modSub_fast(Z1, X1, Z1);     /* t3 = x1 - z1^2 */
    vli_modMult_fast(X1, X1, Z1);    /* t1 = x1^2 - z1^4 */

    vli_modAdd(Z1, X1, X1, curve_p); /* t3 = 2*(x1^2 - z1^4) */
    vli_modAdd(X1, X1, Z1, curve_p); /* t1 = 3*(x1^2 - z1^4) */
    if (vli_testBit(X1, 0)) {
        uECC_word_t l_carry = vli_add(X1, X1, curve_p);
        vli_rshift1(X1);
        X1[uECC_WORDS - 1] |= l_carry << (uECC_WORD_BITS - 1);
    } else {
        vli_rshift1(X1);
    }
    /* t1 = 3/2*(x1^2 - z1^4) = B */

    vli_modSquare_fast(Z1, X1);   /* t3 = B^2 */
    vli_modSub_fast(Z1, Z1, t5);  /* t3 = B^2 - A */
    vli_modSub_fast(Z1, Z1, t5);  /* t3 = B^2 - 2A = x3 */
    vli_modSub_fast(t5, t5, Z1);  /* t5 = A - x3 */
    vli_modMult_fast(X1, X1, t5); /* t1 = B * (A - x3) */
    vli_modSub_fast(t4, X1, t4);  /* t4 = B * (A - x3) - y1^4 = y3 */

    vli_set(X1, Z1);
    vli_set(Z1, Y1);
    vli_set(Y1, t4);
}

/* Modify (x1, y1) => (x1 * z^2, y1 * z^3) */
static void apply_z(uECC_word_t * RESTRICT X1,
                    uECC_word_t * RESTRICT Y1,
                    const uECC_word_t * RESTRICT Z) {
    uECC_word_t t1[uECC_WORDS];

    vli_modSquare_fast(t1, Z);    /* z^2 */
    vli_modMult_fast(X1, X1, t1); /* x1 * z^2 */
    vli_modMult_fast(t1, t1, Z);  /* z^3 */
    vli_modMult_fast(Y1, Y1, t1); /* y1 * z^3 */
}

/* P = (x1, y1) => 2P, (x2, y2) => P' */
static void XYcZ_initial_double(uECC_word_t * RESTRICT X1,
                                uECC_word_t * RESTRICT Y1,
                                uECC_word_t * RESTRICT X2,
                                uECC_word_t * RESTRICT Y2,
                                const uECC_word_t * RESTRICT initial_Z) {
    uECC_word_t z[uECC_WORDS];
    if (initial_Z) {
        vli_set(z, initial_Z);
    } else {
        vli_clear(z);
        z[0] = 1;
    }

    vli_set(X2, X1);
    vli_set(Y2, Y1);

    apply_z(X1, Y1, z);
    EccPoint_double_jacobian(X1, Y1, z);
    apply_z(X2, Y2, z);
}

/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P' = (x1', y1', Z3), P + Q = (x3, y3, Z3)
   or P => P', Q => P + Q
*/
static void XYcZ_add(uECC_word_t * RESTRICT X1,
                     uECC_word_t * RESTRICT Y1,
                     uECC_word_t * RESTRICT X2,
                     uECC_word_t * RESTRICT Y2) {
    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
    uECC_word_t t5[uECC_WORDS];

    vli_modSub_fast(t5, X2, X1);  /* t5 = x2 - x1 */
    vli_modSquare_fast(t5, t5);   /* t5 = (x2 - x1)^2 = A */
    vli_modMult_fast(X1, X1, t5); /* t1 = x1*A = B */
    vli_modMult_fast(X2, X2, t5); /* t3 = x2*A = C */
    vli_modSub_fast(Y2, Y2, Y1);  /* t4 = y2 - y1 */
    vli_modSquare_fast(t5, Y2);   /* t5 = (y2 - y1)^2 = D */

    vli_modSub_fast(t5, t5, X1);  /* t5 = D - B */
    vli_modSub_fast(t5, t5, X2);  /* t5 = D - B - C = x3 */
    vli_modSub_fast(X2, X2, X1);  /* t3 = C - B */
    vli_modMult_fast(Y1, Y1, X2); /* t2 = y1*(C - B) */
    vli_modSub_fast(X2, X1, t5);  /* t3 = B - x3 */
    vli_modMult_fast(Y2, Y2, X2); /* t4 = (y2 - y1)*(B - x3) */
    vli_modSub_fast(Y2, Y2, Y1);  /* t4 = y3 */

    vli_set(X2, t5);
}

/* Input P = (x1, y1, Z), Q = (x2, y2, Z)
   Output P + Q = (x3, y3, Z3), P - Q = (x3', y3', Z3)
   or P => P - Q, Q => P + Q
*/
static void XYcZ_addC(uECC_word_t * RESTRICT X1,
                      uECC_word_t * RESTRICT Y1,
                      uECC_word_t * RESTRICT X2,
                      uECC_word_t * RESTRICT Y2) {
    /* t1 = X1, t2 = Y1, t3 = X2, t4 = Y2 */
    uECC_word_t t5[uECC_WORDS];
    uECC_word_t t6[uECC_WORDS];
    uECC_word_t t7[uECC_WORDS];

    vli_modSub_fast(t5, X2, X1);     /* t5 = x2 - x1 */
    vli_modSquare_fast(t5, t5);      /* t5 = (x2 - x1)^2 = A */
    vli_modMult_fast(X1, X1, t5);    /* t1 = x1*A = B */
    vli_modMult_fast(X2, X2, t5);    /* t3 = x2*A = C */
    vli_modAdd(t5, Y2, Y1, curve_p); /* t5 = y2 + y1 */
    vli_modSub_fast(Y2, Y2, Y1);     /* t4 = y2 - y1 */

    vli_modSub_fast(t6, X2, X1);     /* t6 = C - B */
    vli_modMult_fast(Y1, Y1, t6);    /* t2 = y1 * (C - B) = E */
    vli_modAdd(t6, X1, X2, curve_p); /* t6 = B + C */
    vli_modSquare_fast(X2, Y2);      /* t3 = (y2 - y1)^2 = D */
    vli_modSub_fast(X2, X2, t6);     /* t3 = D - (B + C) = x3 */

    vli_modSub_fast(t7, X1, X2);  /* t7 = B - x3 */
    vli_modMult_fast(Y2, Y2, t7); /* t4 = (y2 - y1)*(B - x3) */
    vli_modSub_fast(Y2, Y2, Y1);  /* t4 = (y2 - y1)*(B - x3) - E = y3 */

    vli_modSquare_fast(t7, t5);   /* t7 = (y2 + y1)^2 = F */
    vli_modSub_fast(t7, t7, t6);  /* t7 = F - (B + C) = x3' */
    vli_modSub_fast(t6, t7, X1);  /* t6 = x3' - B */
    vli_modMult_fast(t6, t6, t5); /* t6 = (y2 + y1)*(x3' - B) */
    vli_modSub_fast(Y1, t6, Y1);  /* t2 = (y2 + y1)*(x3' - B) - E = y3' */

    vli_set(X1, t7);
}

/* Computes result = x^3 + ax + b. result must not overlap x. */
static void curve_x_side(uECC_word_t * RESTRICT result, const uECC_word_t * RESTRICT x) {
#if (uECC_CURVE == uECC_secp256k1)
    vli_modSquare_fast(result, x); /* r = x^2 */
    vli_modMult_fast(result, result, x); /* r = x^3 */
    vli_modAdd(result, result, curve_b, curve_p); /* r = x^3 + b */
#else
    uECC_word_t _3[uECC_WORDS] = {3}; /* -a = 3 */

    vli_modSquare_fast(result, x); /* r = x^2 */
    vli_modSub_fast(result, result, _3); /* r = x^2 - 3 */
    vli_modMult_fast(result, result, x); /* r = x^3 - 3x */
    vli_modAdd(result, result, curve_b, curve_p); /* r = x^3 - 3x + b */
#endif
}

enum
{
  ECC_POINT_MULT_STATE_INIT,
  ECC_POINT_MULT_STATE_BIT_ADDC,
  ECC_POINT_MULT_STATE_BIT_ADD,
  ECC_POINT_MULT_STATE_EXIT_ADDC,
  ECC_POINT_MULT_STATE_EXIT_ADD,
  ECC_POINT_MULT_STATE_COMPLETE
};

typedef struct
{
  uint8_t     state;
  uECC_word_t Rx[2][uECC_WORDS];
  uECC_word_t Ry[2][uECC_WORDS];
  bitcount_t  i;
} EccPointMultCtx;

static int EccPoint_mult(EccPointMultCtx *pCtx,
                         EccPoint * RESTRICT result,
                         const EccPoint * RESTRICT point,
                         const uECC_word_t * RESTRICT scalar,
                         const uECC_word_t * RESTRICT initialZ,
                         bitcount_t numBits) {
    uECC_word_t nb;
    uECC_word_t z[uECC_WORDS];

    switch (pCtx->state) {
        case ECC_POINT_MULT_STATE_INIT:
            vli_set(pCtx->Rx[1], point->x);
            vli_set(pCtx->Ry[1], point->y);

            XYcZ_initial_double(pCtx->Rx[1], pCtx->Ry[1], pCtx->Rx[0], pCtx->Ry[0], initialZ);

            pCtx->i = numBits - 2;
            pCtx->state = ECC_POINT_MULT_STATE_BIT_ADDC;
            return 0;

        case ECC_POINT_MULT_STATE_BIT_ADDC:
            nb = !vli_testBit(scalar, pCtx->i);
            XYcZ_addC(pCtx->Rx[1 - nb], pCtx->Ry[1 - nb], pCtx->Rx[nb], pCtx->Ry[nb]);

            pCtx->state = ECC_POINT_MULT_STATE_BIT_ADD;
            return 0;

        case ECC_POINT_MULT_STATE_BIT_ADD:
            nb = !vli_testBit(scalar, pCtx->i);
            XYcZ_add(pCtx->Rx[nb], pCtx->Ry[nb], pCtx->Rx[1 - nb], pCtx->Ry[1 - nb]);

            pCtx->i--;
            pCtx->state = (pCtx->i > 0) ? ECC_POINT_MULT_STATE_BIT_ADDC : ECC_POINT_MULT_STATE_EXIT_ADDC;
            return 0;

        case ECC_POINT_MULT_STATE_EXIT_ADDC:
            nb = !vli_testBit(scalar, 0);
            XYcZ_addC(pCtx->Rx[1 - nb], pCtx->Ry[1 - nb], pCtx->Rx[nb], pCtx->Ry[nb]);

            pCtx->state = ECC_POINT_MULT_STATE_EXIT_ADD;
            return 0;

        case ECC_POINT_MULT_STATE_EXIT_ADD:
            nb = !vli_testBit(scalar, 0);

            /* Find final 1/Z value. */
            vli_modSub_fast(z, pCtx->Rx[1], pCtx->Rx[0]);   /* X1 - X0 */
            vli_modMult_fast(z, z, pCtx->Ry[1 - nb]);       /* Yb * (X1 - X0) */
            vli_modMult_fast(z, z, point->x);               /* xP * Yb * (X1 - X0) */
            vli_modInv(z, z, curve_p);                      /* 1 / (xP * Yb * (X1 - X0)) */
            vli_modMult_fast(z, z, point->y);               /* yP / (xP * Yb * (X1 - X0)) */
            vli_modMult_fast(z, z, pCtx->Rx[1 - nb]);       /* Xb * yP / (xP * Yb * (X1 - X0)) */
            /* End 1/Z calculation */

            XYcZ_add(pCtx->Rx[nb], pCtx->Ry[nb], pCtx->Rx[1 - nb], pCtx->Ry[1 - nb]);
            apply_z(pCtx->Rx[0], pCtx->Ry[0], z);

            vli_set(result->x, pCtx->Rx[0]);
            vli_set(result->y, pCtx->Ry[0]);

            pCtx->state = ECC_POINT_MULT_STATE_COMPLETE;
            return 1;

        case ECC_POINT_MULT_STATE_COMPLETE:
        default:
            return 1;
    }
}

#if uECC_WORD_SIZE == 4

static void vli_nativeToBytes(uint8_t *bytes, const uint32_t *native) {
    unsigned i;
    for (i = 0; i < uECC_WORDS; ++i) {
        uint8_t *digit = bytes + 4 * (uECC_WORDS - 1 - i);
        digit[0] = (uint8_t)(native[i] >> 24);
        digit[1] = (uint8_t)(native[i] >> 16);
        digit[2] = (uint8_t)(native[i] >> 8);
        digit[3] = (uint8_t)(native[i]);
    }
}

static void vli_bytesToNative(uint32_t *native, const uint8_t *bytes) {
    unsigned i;
    for (i = 0; i < uECC_WORDS; ++i) {
        const uint8_t *digit = bytes + 4 * (uECC_WORDS - 1 - i);
        native[i] = ((uint32_t)digit[0] << 24) | ((uint32_t)digit[1] << 16) |
                    ((uint32_t)digit[2] << 8) | (uint32_t)digit[3];
    }
}

#endif /* uECC_WORD_SIZE */

enum
{
  ECC_MAKE_KEY_STATE_INIT,
  ECC_MAKE_KEY_STATE_ECC_POINT_MULT,
  ECC_MAKE_KEY_STATE_EXIT,
  ECC_MAKE_KEY_STATE_COMPLETE
};

typedef struct EccMakeKeyCtx {
  uint8_t         state;
  uECC_word_t     private[uECC_WORDS];
  EccPoint        public;
  uECC_word_t     tmp1[uECC_WORDS];
  uECC_word_t     tmp2[uECC_WORDS];
  uECC_word_t     *p2[2];
  uECC_word_t     carry;

  EccPointMultCtx pointMultCtx;
} EccMakeKeyCtx;

typedef struct EccSharedSecretCtx {
  uECC_word_t     random[uECC_WORDS];
  uECC_word_t     *initial_Z;
  EccPoint        public;
  EccPoint        product;
  uECC_word_t     private[uECC_WORDS];
  uECC_word_t     tmp[uECC_WORDS];
  uECC_word_t     *p2[2];
  uECC_word_t     carry;

  EccPointMultCtx pointMultCtx;
} EccSharedSecretCtx;

typedef union EccCtx {
  EccMakeKeyCtx      makeKey;
  EccSharedSecretCtx sharedSecret;
} EccCtx;

static EccCtx uECC_ctx;

/**************************************************************************************************
  Make Key
**************************************************************************************************/

void uECC_make_key_start(const uint8_t private_key[uECC_BYTES]) {
    memset(&uECC_ctx, 0, sizeof(uECC_ctx));
    uECC_ctx.makeKey.state = ECC_MAKE_KEY_STATE_INIT;
    vli_bytesToNative(uECC_ctx.makeKey.private, private_key);
}

int uECC_make_key_continue(void) {
    switch (uECC_ctx.makeKey.state) {
        case ECC_MAKE_KEY_STATE_INIT:
            /* Make sure the private key is in the range [1, n-1]. */
            if (vli_isZero(uECC_ctx.makeKey.private)) {
                return 0;
            }

            if (vli_cmp(curve_n, uECC_ctx.makeKey.private) != 1) {
                return 0;
            }

            // Regularize the bitcount for the private key so that attackers cannot use a side channel
            // attack to learn the number of leading zeros.
            uECC_ctx.makeKey.p2[0] = uECC_ctx.makeKey.tmp1;
            uECC_ctx.makeKey.p2[1] = uECC_ctx.makeKey.tmp2;
            uECC_ctx.makeKey.carry = vli_add(uECC_ctx.makeKey.tmp1, uECC_ctx.makeKey.private, curve_n);
            vli_add(uECC_ctx.makeKey.tmp2, uECC_ctx.makeKey.tmp1, curve_n);

            uECC_ctx.makeKey.pointMultCtx.state = 0;
            uECC_ctx.makeKey.state = ECC_MAKE_KEY_STATE_ECC_POINT_MULT;
            return 0;

        case ECC_MAKE_KEY_STATE_ECC_POINT_MULT:
            if (EccPoint_mult(&uECC_ctx.makeKey.pointMultCtx,
                              &uECC_ctx.makeKey.public,
                              &curve_G,
                              uECC_ctx.makeKey.p2[!uECC_ctx.makeKey.carry],
                              0,
                              (uECC_BYTES * 8) + 1)) {
                uECC_ctx.makeKey.state = ECC_MAKE_KEY_STATE_EXIT;
            }
            return 0;

        case ECC_MAKE_KEY_STATE_EXIT:
            if (EccPoint_isZero(&uECC_ctx.makeKey.public)) {
                uECC_ctx.makeKey.state = ECC_MAKE_KEY_STATE_INIT;
                return 0;
            }

            uECC_ctx.makeKey.state = ECC_MAKE_KEY_STATE_COMPLETE;
            return 1;

        case ECC_MAKE_KEY_STATE_COMPLETE:
        default:
            return 1;
    }
}

void uECC_make_key_complete(uint8_t public_key[uECC_BYTES*2], uint8_t private_key[uECC_BYTES]) {
    vli_nativeToBytes(private_key, uECC_ctx.makeKey.private);
    vli_nativeToBytes(public_key, uECC_ctx.makeKey.public.x);
    vli_nativeToBytes(public_key + uECC_BYTES, uECC_ctx.makeKey.public.y);
}

/**************************************************************************************************
  Validate Key
**************************************************************************************************/

int uECC_valid_public_key_ll(const uint8_t public_key[uECC_BYTES*2]) {
    uECC_word_t tmp1[uECC_WORDS];
    uECC_word_t tmp2[uECC_WORDS];
    EccPoint public;

    vli_bytesToNative(public.x, public_key);
    vli_bytesToNative(public.y, public_key + uECC_BYTES);

    // The point at infinity is invalid.
    if (EccPoint_isZero(&public)) {
        return 0;
    }

    // x and y must be smaller than p.
    if (vli_cmp(curve_p, public.x) != 1 || vli_cmp(curve_p, public.y) != 1) {
        return 0;
    }

    vli_modSquare_fast(tmp1, public.y); /* tmp1 = y^2 */
    curve_x_side(tmp2, public.x); /* tmp2 = x^3 + ax + b */

    /* Make sure that y^2 == x^3 + ax + b */
    return (vli_cmp(tmp1, tmp2) == 0);
}

/**************************************************************************************************
  Shared Secret
**************************************************************************************************/

void uECC_shared_secret_start(const uint8_t public_key[uECC_BYTES*2],
                              const uint8_t private_key[uECC_BYTES]) {
    uECC_word_t tries;

    memset(&uECC_ctx, 0, sizeof(uECC_ctx));

    // Try to get a random initial Z value to improve protection against side-channel
    // attacks. If the RNG fails every time (eg it was not defined), we continue so that
    // uECC_shared_secret() can still work without an RNG defined.
    uECC_ctx.sharedSecret.initial_Z = NULL;
    for (tries = 0; tries < MAX_TRIES; ++tries) {
        if (g_rng_function((uint8_t *)uECC_ctx.sharedSecret.random, sizeof(uECC_ctx.sharedSecret.random)) && !vli_isZero(uECC_ctx.sharedSecret.random)) {
            uECC_ctx.sharedSecret.initial_Z = uECC_ctx.sharedSecret.random;
            break;
        }
    }

    vli_bytesToNative(uECC_ctx.sharedSecret.private, private_key);
    vli_bytesToNative(uECC_ctx.sharedSecret.public.x, public_key);
    vli_bytesToNative(uECC_ctx.sharedSecret.public.y, public_key + uECC_BYTES);

    // Regularize the bitcount for the private key so that attackers cannot use a side channel
    // attack to learn the number of leading zeros.
    uECC_ctx.sharedSecret.p2[0] = uECC_ctx.sharedSecret.private;
    uECC_ctx.sharedSecret.p2[1] = uECC_ctx.sharedSecret.tmp;
    uECC_ctx.sharedSecret.carry = vli_add(uECC_ctx.sharedSecret.private, uECC_ctx.sharedSecret.private, curve_n);
    vli_add(uECC_ctx.sharedSecret.tmp, uECC_ctx.sharedSecret.private, curve_n);

    uECC_ctx.sharedSecret.pointMultCtx.state = 0;
}

int uECC_shared_secret_continue(void) {
    return EccPoint_mult(&uECC_ctx.sharedSecret.pointMultCtx,
                         &uECC_ctx.sharedSecret.product,
                         &uECC_ctx.sharedSecret.public,
                         uECC_ctx.sharedSecret.p2[!uECC_ctx.sharedSecret.carry],
                         uECC_ctx.sharedSecret.initial_Z,
                         (uECC_BYTES * 8) + 1);
}

void uECC_shared_secret_complete(uint8_t secret[uECC_BYTES]) {
    vli_nativeToBytes(secret, uECC_ctx.sharedSecret.product.x);
}
