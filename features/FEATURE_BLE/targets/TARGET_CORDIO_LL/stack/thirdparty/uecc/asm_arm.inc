#define DEC_5 4
#define DEC_6 5
#define DEC_7 6
#define DEC_8 7

#define DEC(N) uECC_CONCAT(DEC_, N)

#define REPEAT_1(stuff) stuff
#define REPEAT_2(stuff) REPEAT_1(stuff) stuff
#define REPEAT_3(stuff) REPEAT_2(stuff) stuff
#define REPEAT_4(stuff) REPEAT_3(stuff) stuff
#define REPEAT_5(stuff) REPEAT_4(stuff) stuff
#define REPEAT_6(stuff) REPEAT_5(stuff) stuff
#define REPEAT_7(stuff) REPEAT_6(stuff) stuff
#define REPEAT_8(stuff) REPEAT_7(stuff) stuff

#define REPEAT(N, stuff) uECC_CONCAT(REPEAT_, N)(stuff)

#define STR2(thing) #thing
#define STR(thing) STR2(thing)

#if (uECC_ASM == uECC_asm_fast)

static uint32_t vli_add(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    uint32_t carry = 0;
    uint32_t left_word;
    uint32_t right_word;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia %[lptr]!, {%[left]} \n\t"  /* Load left word. */
        "ldmia %[rptr]!, {%[right]} \n\t" /* Load right word. */
        "adds %[left], %[right] \n\t"     /* Add first word. */
        "stmia %[dptr]!, {%[left]} \n\t"  /* Store result word. */

        /* Now we just do the remaining words with the carry bit (using ADC) */
        REPEAT(DEC(uECC_WORDS),
            "ldmia %[lptr]!, {%[left]} \n\t"
            "ldmia %[rptr]!, {%[right]} \n\t"
            "adcs %[left], %[right] \n\t"
            "stmia %[dptr]!, {%[left]} \n\t")

        "adcs %[carry], %[carry] \n\t" /* Store carry bit. */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
    #if (uECC_PLATFORM == uECC_arm_thumb)
        : [dptr] "+l" (result), [lptr] "+l" (left), [rptr] "+l" (right),
          [carry] "+l" (carry), [left] "=l" (left_word), [right] "=l" (right_word)
    #else
        : [dptr] "+r" (result), [lptr] "+r" (left), [rptr] "+r" (right),
          [carry] "+r" (carry), [left] "=r" (left_word), [right] "=r" (right_word)
    #endif
        :
        : "cc", "memory"
    );
    return carry;
}
#define asm_add 1

static uint32_t vli_sub(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    uint32_t carry = 0;
    uint32_t left_word;
    uint32_t right_word;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia %[lptr]!, {%[left]} \n\t"  /* Load left word. */
        "ldmia %[rptr]!, {%[right]} \n\t" /* Load right word. */
        "subs %[left], %[right] \n\t"     /* Subtract. */
        "stmia %[dptr]!, {%[left]} \n\t"  /* Store result word. */

        /* Now we just do the remaining words with the carry bit (using SBC) */
        REPEAT(DEC(uECC_WORDS),
            "ldmia %[lptr]!, {%[left]} \n\t"
            "ldmia %[rptr]!, {%[right]} \n\t"
            "sbcs %[left], %[right] \n\t"
            "stmia %[dptr]!, {%[left]} \n\t")

        "adcs %[carry], %[carry] \n\t" /* Store carry bit. */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
    #if (uECC_PLATFORM == uECC_arm_thumb)
        : [dptr] "+l" (result), [lptr] "+l" (left), [rptr] "+l" (right),
          [carry] "+l" (carry), [left] "=l" (left_word), [right] "=l" (right_word)
    #else
        : [dptr] "+r" (result), [lptr] "+r" (left), [rptr] "+r" (right),
          [carry] "+r" (carry), [left] "=r" (left_word), [right] "=r" (right_word)
    #endif
        :
        : "cc", "memory"
    );
    return !carry; // note that on ARM, carry flag set means "no borrow" when subtracting
                   // (for some reason...)
}
#define asm_sub 1

#if (uECC_PLATFORM != uECC_arm_thumb)
#if (uECC_WORDS == 5)
static void vli_mult(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;
    register const uint32_t *r2 __asm__("r2") = right;

    __asm__ volatile (
        ".syntax unified \n\t"
        "add r0, 12 \n\t"
        "add r2, 12 \n\t"
        "ldmia r1!, {r3,r4} \n\t"
        "ldmia r2!, {r6,r7} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adc r10, r14 \n\t"
        "stmia r0!, {r9, r10} \n\t"

        "sub r0, 28 \n\t"
        "sub r2, 20 \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"
        "ldmia r1!, {r5} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r4, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "umull r14, r9, r4, r7 \n\t"
        "adds r10, r14 \n\t"
        "adc r11, r9 \n\t"
        "stmia r0!, {r10, r11} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1), "+r" (r2)
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_mult 1
#endif /* (uECC_WORDS == 5) */

#if (uECC_WORDS == 6)
static void vli_mult(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;
    register const uint32_t *r2 __asm__("r2") = right;

    __asm__ volatile (
        ".syntax unified \n\t"
        "add r0, 12 \n\t"
        "add r2, 12 \n\t"
        "ldmia r1!, {r3,r4,r5} \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "umull r9, r10, r5, r8 \n\t"
        "adds r11, r9 \n\t"
        "adc r12, r10 \n\t"
        "stmia r0!, {r11, r12} \n\t"

        "sub r0, 36 \n\t"
        "sub r2, 24 \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r4, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r1!, {r5} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r2!, {r8} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r3, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r4, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "mov r14, #0 \n\t"
        "umull r9, r10, r4, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r5, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "umull r10, r11, r5, r8 \n\t"
        "adds r12, r10 \n\t"
        "adc r14, r11 \n\t"
        "stmia r0!, {r12, r14} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1), "+r" (r2)
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_mult 1
#endif /* (uECC_WORDS == 6) */

#if (uECC_WORDS == 7)
static void vli_mult(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;
    register const uint32_t *r2 __asm__("r2") = right;

    __asm__ volatile (
        ".syntax unified \n\t"
        "add r0, 24 \n\t"
        "add r2, 24 \n\t"
        "ldmia r1!, {r3} \n\t"
        "ldmia r2!, {r6} \n\t"

        "umull r9, r10, r3, r6 \n\t"
        "stmia r0!, {r9, r10} \n\t"

        "sub r0, 20 \n\t"
        "sub r2, 16 \n\t"
        "ldmia r2!, {r6, r7, r8} \n\t"
        "ldmia r1!, {r4, r5} \n\t"

        "umull r9, r10, r3, r6 \n\t"
        "stmia r0!, {r9} \n\t"

        "mov r14, #0 \n\t"
        "umull r9, r12, r3, r7 \n\t"
        "adds r10, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r9, r11, r4, r6 \n\t"
        "adds r10, r9 \n\t"
        "adcs r12, r11 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "mov r12, #0 \n\t"
        "umull r14, r9, r5, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "umull r9, r10, r3, r6 \n\t"
        "adds r11, r9 \n\t"
        "adc r12, r10 \n\t"
        "stmia r0!, {r11, r12} \n\t"

        "sub r0, 44 \n\t"
        "sub r1, 16 \n\t"
        "sub r2, 28 \n\t"
        "ldmia r1!, {r3,r4,r5} \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"

        "umull r9, r10, r3, r6 \n\t"
        "stmia r0!, {r9} \n\t"

        "mov r14, #0 \n\t"
        "umull r9, r12, r3, r7 \n\t"
        "adds r10, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r9, r11, r4, r6 \n\t"
        "adds r10, r9 \n\t"
        "adcs r12, r11 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r5, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r1!, {r5} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r3, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r4, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r4, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r5, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r4, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r5, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r3, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r4, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r5, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r2!, {r8} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r4, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r3, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "umull r10, r11, r3, r6 \n\t"
        "adds r12, r10 \n\t"
        "adc r14, r11 \n\t"
        "stmia r0!, {r12, r14} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1), "+r" (r2)
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_mult 1
#endif /* (uECC_WORDS == 7) */

#if (uECC_WORDS == 8)
static void vli_mult(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;
    register const uint32_t *r2 __asm__("r2") = right;

    __asm__ volatile (
        ".syntax unified \n\t"
        "add r0, 24 \n\t"
        "add r2, 24 \n\t"
        "ldmia r1!, {r3,r4} \n\t"
        "ldmia r2!, {r6,r7} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adc r10, r14 \n\t"
        "stmia r0!, {r9, r10} \n\t"

        "sub r0, 28 \n\t"
        "sub r2, 20 \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"
        "ldmia r1!, {r5} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r4, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "umull r14, r9, r4, r7 \n\t"
        "adds r10, r14 \n\t"
        "adc r11, r9 \n\t"
        "stmia r0!, {r10, r11} \n\t"

        "sub r0, 52 \n\t"
        "sub r1, 20 \n\t"
        "sub r2, 32 \n\t"
        "ldmia r1!, {r3,r4,r5} \n\t"
        "ldmia r2!, {r6,r7,r8} \n\t"

        "umull r11, r12, r3, r6 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r9, r3, r7 \n\t"
        "adds r12, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r11, r14, r4, r6 \n\t"
        "adds r12, r11 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r11, #0 \n\t"
        "umull r12, r14, r3, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r5, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r4, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r5, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r4, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r1!, {r5} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r3, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r5, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r4, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r5, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r1!, {r4} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r5, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r3, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r12, #0 \n\t"
        "umull r14, r9, r5, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r3, r8 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r4, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r14, #0 \n\t"
        "umull r9, r10, r5, r7 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r3, r6 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "umull r9, r10, r4, r8 \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, r10 \n\t"
        "adc r14, #0 \n\t"
        "ldr r9, [r0] \n\t"
        "adds r11, r9 \n\t"
        "adcs r12, #0 \n\t"
        "adc r14, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "ldmia r2!, {r8} \n\t"
        "mov r9, #0 \n\t"
        "umull r10, r11, r5, r8 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r3, r7 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "umull r10, r11, r4, r6 \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, r11 \n\t"
        "adc r9, #0 \n\t"
        "ldr r10, [r0] \n\t"
        "adds r12, r10 \n\t"
        "adcs r14, #0 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "ldmia r2!, {r6} \n\t"
        "mov r10, #0 \n\t"
        "umull r11, r12, r5, r6 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r8 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r4, r7 \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "ldr r11, [r0] \n\t"
        "adds r14, r11 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r14} \n\t"

        "ldmia r2!, {r7} \n\t"
        "mov r11, #0 \n\t"
        "umull r12, r14, r5, r7 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r3, r6 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "umull r12, r14, r4, r8 \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, r14 \n\t"
        "adc r11, #0 \n\t"
        "ldr r12, [r0] \n\t"
        "adds r9, r12 \n\t"
        "adcs r10, #0 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "mov r12, #0 \n\t"
        "umull r14, r9, r3, r7 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "umull r14, r9, r4, r6 \n\t"
        "adds r10, r14 \n\t"
        "adcs r11, r9 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r10} \n\t"

        "umull r9, r10, r4, r7 \n\t"
        "adds r11, r9 \n\t"
        "adc r12, r10 \n\t"
        "stmia r0!, {r11, r12} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1), "+r" (r2)
        :
        : "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_mult 1
#endif /* (uECC_WORDS == 8) */

#if uECC_SQUARE_FUNC
#if (uECC_WORDS == 5)
static void vli_square(uint32_t *result, const uint32_t *left) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia r1!, {r2,r3,r4,r5,r6} \n\t"

        "umull r11, r12, r2, r2 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r2, r3 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11, #0 \n\t"
        "adc r9, #0 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r12, r2, r4 \n\t"
        "adds r11, r11 \n\t"
        "adcs r12, r12 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r3 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r5 \n\t"
        "umull r1, r14, r3, r4 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r2, r6 \n\t"
        "umull r1, r14, r3, r5 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "umull r1, r14, r4, r4 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r3, r6 \n\t"
        "umull r1, r14, r4, r5 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r8, #0 \n\t"
        "umull r1, r10, r4, r6 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r8, #0 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "umull r1, r10, r5, r5 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r11, #0 \n\t"
        "umull r1, r10, r5, r6 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r11, #0 \n\t"
        "adds r12, r1 \n\t"
        "adcs r8, r10 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r1, r10, r6, r6 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r10 \n\t"
        "stmia r0!, {r8, r11} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1)
        :
        : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_square 1
#endif /* (uECC_WORDS == 5) */

#if (uECC_WORDS == 6)
static void vli_square(uint32_t *result, const uint32_t *left) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia r1!, {r2,r3,r4,r5,r6,r7} \n\t"

        "umull r11, r12, r2, r2 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r2, r3 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11, #0 \n\t"
        "adc r9, #0 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r12, r2, r4 \n\t"
        "adds r11, r11 \n\t"
        "adcs r12, r12 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r3 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r5 \n\t"
        "umull r1, r14, r3, r4 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r2, r6 \n\t"
        "umull r1, r14, r3, r5 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "umull r1, r14, r4, r4 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r7 \n\t"
        "umull r1, r14, r3, r6 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "umull r1, r14, r4, r5 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r3, r7 \n\t"
        "umull r1, r14, r4, r6 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "umull r1, r14, r5, r5 \n\t"
        "adds r8, r1 \n\t"
        "adcs r9, r14 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r4, r7 \n\t"
        "umull r1, r14, r5, r6 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r14 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r8, #0 \n\t"
        "umull r1, r10, r5, r7 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r8, #0 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "umull r1, r10, r6, r6 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r11, #0 \n\t"
        "umull r1, r10, r6, r7 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r11, #0 \n\t"
        "adds r12, r1 \n\t"
        "adcs r8, r10 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r1, r10, r7, r7 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r10 \n\t"
        "stmia r0!, {r8, r11} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1)
        :
        : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_square 1
#endif /* (uECC_WORDS == 6) */

#if (uECC_WORDS == 7)
static void vli_square(uint32_t *result, const uint32_t *left) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia r1!, {r2} \n\t"
        "add r1, 20 \n\t"
        "ldmia r1!, {r5} \n\t"
        "add r0, 24 \n\t"
        "umull r8, r9, r2, r5 \n\t"
        "stmia r0!, {r8, r9} \n\t"
        "sub r0, 32 \n\t"
        "sub r1, 28 \n\t"

        "ldmia r1!, {r2, r3, r4, r5, r6, r7} \n\t"

        "umull r11, r12, r2, r2 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r2, r3 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11, #0 \n\t"
        "adc r9, #0 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r12, r2, r4 \n\t"
        "adds r11, r11 \n\t"
        "adcs r12, r12 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r3 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r5 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r3, r4 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r2, r6 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r3, r5 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r4, r4 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r7 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r3, r6 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r4, r5 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "ldmia r1!, {r2} \n\t"
        "mov r10, #0 \n\t"
        "umull r8, r9, r3, r7 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r4, r6 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r5, r5 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r3, r2 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r4, r7 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r5, r6 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r4, r2 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r5, r7 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r6, r6 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r5, r2 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r6, r7 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r8, #0 \n\t"
        "umull r1, r10, r6, r2 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r8, #0 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "umull r1, r10, r7, r7 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r11, #0 \n\t"
        "umull r1, r10, r7, r2 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r11, #0 \n\t"
        "adds r12, r1 \n\t"
        "adcs r8, r10 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r1, r10, r2, r2 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r10 \n\t"
        "stmia r0!, {r8, r11} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1)
        :
        : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_square 1
#endif /* (uECC_WORDS == 7) */

#if (uECC_WORDS == 8)
static void vli_square(uint32_t *result, const uint32_t *left) {
    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;

    __asm__ volatile (
        ".syntax unified \n\t"
        "ldmia r1!, {r2, r3} \n\t"
        "add r1, 16 \n\t"
        "ldmia r1!, {r5, r6} \n\t"
        "add r0, 24 \n\t"

        "umull r8, r9, r2, r5 \n\t"
        "stmia r0!, {r8} \n\t"

        "umull r12, r10, r2, r6 \n\t"
        "adds r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r9} \n\t"

        "umull r8, r9, r3, r6 \n\t"
        "adds r10, r8 \n\t"
        "adc r11, r9, #0 \n\t"
        "stmia r0!, {r10, r11} \n\t"

        "sub r0, 40 \n\t"
        "sub r1, 32 \n\t"
        "ldmia r1!, {r2,r3,r4,r5,r6,r7} \n\t"

        "umull r11, r12, r2, r2 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r9, #0 \n\t"
        "umull r10, r11, r2, r3 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11, #0 \n\t"
        "adc r9, #0 \n\t"
        "adds r12, r10 \n\t"
        "adcs r8, r11 \n\t"
        "adc r9, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "mov r10, #0 \n\t"
        "umull r11, r12, r2, r4 \n\t"
        "adds r11, r11 \n\t"
        "adcs r12, r12 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "umull r11, r12, r3, r3 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r5 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r3, r4 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r2, r6 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r3, r5 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r4, r4 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r2, r7 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r3, r6 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r4, r5 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "ldmia r1!, {r2} \n\t"
        "mov r10, #0 \n\t"
        "umull r8, r9, r3, r7 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r4, r6 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r5, r5 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r3, r2 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r4, r7 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r5, r6 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "ldmia r1!, {r3} \n\t"
        "mov r10, #0 \n\t"
        "umull r8, r9, r4, r2 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r5, r7 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r9, #0 \n\t"
        "adc r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r6, r6 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r4, r3 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r5, r2 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r6, r7 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "ldr r14, [r0] \n\t"
        "adds r8, r14 \n\t"
        "adcs r11, #0 \n\t"
        "adc r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r10, #0 \n\t"
        "umull r8, r9, r5, r3 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r6, r2 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r9, r9 \n\t"
        "adc r10, r10 \n\t"
        "mov r14, r9 \n\t"
        "umlal r8, r9, r7, r7 \n\t"
        "cmp r14, r9 \n\t"
        "it hi \n\t"
        "adchi r10, #0 \n\t"
        "adds r8, r11 \n\t"
        "adcs r9, r12 \n\t"
        "adc r10, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r12, #0 \n\t"
        "umull r8, r11, r6, r3 \n\t"
        "mov r14, r11 \n\t"
        "umlal r8, r11, r7, r2 \n\t"
        "cmp r14, r11 \n\t"
        "it hi \n\t"
        "adchi r12, #0 \n\t"
        "adds r8, r8 \n\t"
        "adcs r11, r11 \n\t"
        "adc r12, r12 \n\t"
        "adds r8, r9 \n\t"
        "adcs r11, r10 \n\t"
        "adc r12, #0 \n\t"
        "stmia r0!, {r8} \n\t"

        "mov r8, #0 \n\t"
        "umull r1, r10, r7, r3 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r8, #0 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "umull r1, r10, r2, r2 \n\t"
        "adds r11, r1 \n\t"
        "adcs r12, r10 \n\t"
        "adc r8, #0 \n\t"
        "stmia r0!, {r11} \n\t"

        "mov r11, #0 \n\t"
        "umull r1, r10, r2, r3 \n\t"
        "adds r1, r1 \n\t"
        "adcs r10, r10 \n\t"
        "adc r11, #0 \n\t"
        "adds r12, r1 \n\t"
        "adcs r8, r10 \n\t"
        "adc r11, #0 \n\t"
        "stmia r0!, {r12} \n\t"

        "umull r1, r10, r3, r3 \n\t"
        "adds r8, r1 \n\t"
        "adcs r11, r10 \n\t"
        "stmia r0!, {r8, r11} \n\t"
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : "+r" (r0), "+r" (r1)
        :
        : "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11", "r12", "r14", "cc", "memory"
    );
}
#define asm_square 1
#endif /* (uECC_WORDS == 8) */
#endif /* uECC_SQUARE_FUNC */

#endif /* (uECC_PLATFORM != uECC_arm_thumb) */
#endif /* (uECC_ASM == uECC_asm_fast) */

#if !defined(asm_add) || !asm_add
static uint32_t vli_add(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    uint32_t counter = uECC_WORDS;
    uint32_t carry = 0;
    uint32_t left_word;
    uint32_t right_word;

    __asm__ volatile (
        ".syntax unified \n\t"
        "1: \n\t"
        "ldmia %[lptr]!, {%[left]} \n\t"  /* Load left word. */
        "ldmia %[rptr]!, {%[right]} \n\t" /* Load right word. */
        "lsrs %[carry], #1 \n\t"          /* Set up carry flag (carry = 0 after this). */
        "adcs %[left], %[right] \n\t"     /* Add with carry. */
        "adcs %[carry], %[carry] \n\t"    /* Store carry bit. */
        "stmia %[dptr]!, {%[left]} \n\t"  /* Store result word. */
        "subs %[ctr], #1 \n\t"            /* Decrement counter. */
        "bne 1b \n\t"                     /* Loop until counter == 0. */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
    #if (uECC_PLATFORM == uECC_arm_thumb)
        : [dptr] "+l" (result), [lptr] "+l" (left), [rptr] "+l" (right),
          [ctr] "+l" (counter), [carry] "+l" (carry),
          [left] "=l" (left_word), [right] "=l" (right_word)
    #else
        : [dptr] "+r" (result), [lptr] "+r" (left), [rptr] "+r" (right),
          [ctr] "+r" (counter), [carry] "+r" (carry),
          [left] "=r" (left_word), [right] "=r" (right_word)
    #endif
        :
        : "cc", "memory"
    );
    return carry;
}
#define asm_add 1
#endif

#if !defined(asm_sub) || !asm_sub
static uint32_t vli_sub(uint32_t *result, const uint32_t *left, const uint32_t *right) {
    uint32_t counter = uECC_WORDS;
    uint32_t carry = 1; /* carry = 1 initially (means don't borrow) */
    uint32_t left_word;
    uint32_t right_word;

    __asm__ volatile (
        ".syntax unified \n\t"
        "1: \n\t"
        "ldmia %[lptr]!, {%[left]} \n\t"  /* Load left word. */
        "ldmia %[rptr]!, {%[right]} \n\t" /* Load right word. */
        "lsrs %[carry], #1 \n\t"          /* Set up carry flag (carry = 0 after this). */
        "sbcs %[left], %[right] \n\t"     /* Subtract with borrow. */
        "adcs %[carry], %[carry] \n\t"    /* Store carry bit. */
        "stmia %[dptr]!, {%[left]} \n\t"  /* Store result word. */
        "subs %[ctr], #1 \n\t"            /* Decrement counter. */
        "bne 1b \n\t"                     /* Loop until counter == 0. */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
    #if (uECC_PLATFORM == uECC_arm_thumb)
        : [dptr] "+l" (result), [lptr] "+l" (left), [rptr] "+l" (right),
          [ctr] "+l" (counter), [carry] "+l" (carry),
          [left] "=l" (left_word), [right] "=l" (right_word)
    #else
        : [dptr] "+r" (result), [lptr] "+r" (left), [rptr] "+r" (right),
          [ctr] "+r" (counter), [carry] "+r" (carry),
          [left] "=r" (left_word), [right] "=r" (right_word)
    #endif
        :
        : "cc", "memory"
    );
    return !carry;
}
#define asm_sub 1
#endif

#if !defined(asm_mult) || !asm_mult
static void vli_mult(uint32_t *result, const uint32_t *left, const uint32_t *right) {
#if (uECC_PLATFORM != uECC_arm_thumb)
    uint32_t c0 = 0;
    uint32_t c1 = 0;
    uint32_t c2 = 0;
    uint32_t k = 0;
    uint32_t i;
    uint32_t t0, t1;

    __asm__ volatile (
        ".syntax unified \n\t"

        "1: \n\t" /* outer loop (k < uECC_WORDS) */
        "movs %[i], #0 \n\t" /* i = 0 */
        "b 3f \n\t"

        "2: \n\t" /* outer loop (k >= uECC_WORDS) */
        "movs %[i], %[k] \n\t"      /* i = k */
        "subs %[i], %[eccdm1] \n\t" /* i = k - (uECC_WORDS - 1) (times 4) */

        "3: \n\t" /* inner loop */
        "subs %[t0], %[k], %[i] \n\t" /* t0 = k-i */

        "ldr %[t1], [%[right], %[t0]] \n\t" /* t1 = right[k - i] */
        "ldr %[t0], [%[left], %[i]] \n\t"   /* t0 = left[i] */

        "umull %[t0], %[t1], %[t0], %[t1] \n\t" /* (t0, t1) = left[i] * right[k - i] */

        "adds %[c0], %[t0] \n\t" /* add low word to c0 */
        "adcs %[c1], %[t1] \n\t" /* add high word to c1, including carry */
        "adcs %[c2], #0 \n\t"    /* add carry to c2 */

        "adds %[i], #4 \n\t"     /* i += 4 */
        "cmp %[i], %[eccd] \n\t" /* i < uECC_WORDS (times 4)? */
        "bge 4f \n\t"            /*   if not, exit the loop */
        "cmp %[i], %[k] \n\t"    /* i <= k? */
        "ble 3b \n\t"            /*   if so, continue looping */

        "4: \n\t" /* end inner loop */

        "str %[c0], [%[result], %[k]] \n\t" /* result[k] = c0 */
        "mov %[c0], %[c1] \n\t"     /* c0 = c1 */
        "mov %[c1], %[c2] \n\t"     /* c1 = c2 */
        "movs %[c2], #0 \n\t"       /* c2 = 0 */
        "adds %[k], #4 \n\t"        /* k += 4 */
        "cmp %[k], %[eccd] \n\t"    /* k < uECC_WORDS (times 4) ? */
        "blt 1b \n\t"               /*   if not, loop back, start with i = 0 */
        "cmp %[k], %[eccd2m1] \n\t" /* k < uECC_WORDS * 2 - 1 (times 4) ? */
        "blt 2b \n\t"               /*   if not, loop back, start with i = (k + 1) - uECC_WORDS */
        /* end outer loop */

        "str %[c0], [%[result], %[k]] \n\t" /* result[uECC_WORDS * 2 - 1] = c0 */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : [c0] "+r" (c0), [c1] "+r" (c1), [c2] "+r" (c2),
          [k] "+r" (k), [i] "=&r" (i), [t0] "=&r" (t0), [t1] "=&r" (t1)
        : [result] "r" (result), [left] "r" (left), [right] "r" (right),
          [eccd] "I" (uECC_WORDS * 4), [eccdm1] "I" ((uECC_WORDS-1) * 4),
          [eccd2m1] "I" ((uECC_WORDS * 2 - 1) * 4)
        : "cc", "memory"
    );

#else /* Thumb-1 */

    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;
    register const uint32_t *r2 __asm__("r2") = right;

    __asm__ volatile (
        ".syntax unified \n\t"
        "movs r3, #0 \n\t" /* c0 = 0 */
        "movs r4, #0 \n\t" /* c1 = 0 */
        "movs r5, #0 \n\t" /* c2 = 0 */
        "movs r6, #0 \n\t" /* k = 0 */

        "push {r0} \n\t" /* keep result on the stack */

        "1: \n\t" /* outer loop (k < uECC_WORDS) */
        "movs r7, #0 \n\t" /* r7 = i = 0 */
        "b 3f \n\t"

        "2: \n\t" /* outer loop (k >= uECC_WORDS) */
        "movs r7, r6 \n\t"        /* r7 = k */
        "subs r7, %[eccdm1] \n\t" /* r7 = i = k - (uECC_WORDS - 1) (times 4) */

        "3: \n\t" /* inner loop */
        "push {r3, r4, r5, r6} \n\t" /* push things, r3 (c0) is at the top of stack. */
        "subs r0, r6, r7 \n\t"       /* r0 = k - i */

        "ldr r4, [r2, r0] \n\t" /* r4 = right[k - i] */
        "ldr r0, [r1, r7] \n\t" /* r0 = left[i] */

        "lsrs r3, r0, #16 \n\t" /* r3 = a1 */
        "uxth r0, r0 \n\t"      /* r0 = a0 */

        "lsrs r5, r4, #16 \n\t" /* r5 = b1 */
        "uxth r4, r4 \n\t"      /* r4 = b0 */

        "movs r6, r3 \n\t"     /* r6 = a1 */
        "muls r6, r5, r6 \n\t" /* r6 = a1 * b1 */
        "muls r3, r4, r3 \n\t" /* r3 = b0 * a1 */
        "muls r5, r0, r5 \n\t" /* r5 = a0 * b1 */
        "muls r0, r4, r0 \n\t" /* r0 = a0 * b0 */

        "movs r4, #0 \n\t"  /* r4 = 0 */
        "adds r3, r5 \n\t"  /* r3 = b0 * a1 + a0 * b1 */
        "adcs r4, r4 \n\t"  /* r4 = carry */
        "lsls r4, #16 \n\t" /* r4 = carry << 16 */
        "adds r6, r4 \n\t"  /* r6 = a1 * b1 + carry */

        "lsls r4, r3, #16 \n\t" /* r4 = (b0 * a1 + a0 * b1) << 16 */
        "lsrs r3, #16 \n\t"     /* r3 = (b0 * a1 + a0 * b1) >> 16 */
        "adds r0, r4 \n\t"      /* r0 = low word = a0 * b0 + ((b0 * a1 + a0 * b1) << 16) */
        "adcs r6, r3 \n\t"      /* r6 = high word = a1 * b1 + carry + ((b0 * a1 + a0 * b1) >> 16) */

        "pop {r3, r4, r5} \n\t" /* r3 = c0, r4 = c1, r5 = c2 */
        "adds r3, r0 \n\t"      /* add low word to c0 */
        "adcs r4, r6 \n\t"      /* add high word to c1, including carry */
        "movs r0, #0 \n\t"      /* r0 = 0 (does not affect carry bit) */
        "adcs r5, r0 \n\t"      /* add carry to c2 */

        "pop {r6} \n\t" /* r6 = k */

        "adds r7, #4 \n\t"     /* i += 4 */
        "cmp r7, %[eccd] \n\t" /* i < uECC_WORDS (times 4)? */
        "bge 4f \n\t"          /*   if not, exit the loop */
        "cmp r7, r6 \n\t"      /* i <= k? */
        "ble 3b \n\t"          /*   if so, continue looping */

        "4: \n\t" /* end inner loop */

        "ldr r0, [sp, #0] \n\t" /* r0 = result */

        "str r3, [r0, r6] \n\t"   /* result[k] = c0 */
        "mov r3, r4 \n\t"         /* c0 = c1 */
        "mov r4, r5 \n\t"         /* c1 = c2 */
        "movs r5, #0 \n\t"        /* c2 = 0 */
        "adds r6, #4 \n\t"        /* k += 4 */
        "cmp r6, %[eccd] \n\t"    /* k < uECC_WORDS (times 4) ? */
        "blt 1b \n\t"             /*   if not, loop back, start with i = 0 */
        "cmp r6, %[eccd2m1] \n\t" /* k < uECC_WORDS * 2 - 1 (times 4) ? */
        "blt 2b \n\t"             /*   if not, loop back, start with i = (k + 1) - uECC_WORDS */
        /* end outer loop */

        "str r3, [r0, r6] \n\t" /* result[uECC_WORDS * 2 - 1] = c0 */
        "pop {r0} \n\t"         /* pop result off the stack */

        ".syntax divided \n\t"
        :
        : [r0] "l" (r0), [r1] "l" (r1), [r2] "l" (r2), [eccd] "I" (uECC_WORDS * 4), [eccdm1] "I" ((uECC_WORDS-1) * 4), [eccd2m1] "I" ((uECC_WORDS * 2 - 1) * 4)
        : "r3", "r4", "r5", "r6", "r7", "cc", "memory"
    );
#endif
}
#define asm_mult 1
#endif /* !asm_mult */

#if uECC_SQUARE_FUNC
#if !defined(asm_square) || !asm_square
static void vli_square(uint32_t *result, const uint32_t *left) {
#if (uECC_PLATFORM != uECC_arm_thumb)
    uint32_t c0 = 0;
    uint32_t c1 = 0;
    uint32_t c2 = 0;
    uint32_t k = 0;
    uint32_t i, tt;
    uint32_t t0, t1;

    __asm__ volatile (
        ".syntax unified \n\t"

        "1: \n\t" /* outer loop (k < uECC_WORDS) */
        "movs %[i], #0 \n\t" /* i = 0 */
        "b 3f \n\t"

        "2: \n\t" /* outer loop (k >= uECC_WORDS) */
        "movs %[i], %[k] \n\t"      /* i = k */
        "subs %[i], %[eccdm1] \n\t" /* i = k - (uECC_WORDS - 1) (times 4) */

        "3: \n\t" /* inner loop */
        "subs %[tt], %[k], %[i] \n\t" /* tt = k-i */

        "ldr %[t1], [%[left], %[tt]] \n\t" /* t1 = left[k - i] */
        "ldr %[t0], [%[left], %[i]] \n\t"  /* t0 = left[i] */

        "umull %[t0], %[t1], %[t0], %[t1] \n\t" /* (t0, t1) = left[i] * right[k - i] */

        "cmp %[i], %[tt] \n\t" /* (i < k - i) ? */
        "bge 4f \n\t"          /*   if i >= k - i, skip */
        "lsls %[t1], #1 \n\t"  /* high word << 1 */
        "adc %[c2], #0 \n\t"   /* add carry bit to c2 */
        "lsls %[t0], #1 \n\t"  /* low word << 1 */
        "adc %[t1], #0 \n\t"   /* add carry bit to high word */

        "4: \n\t"

        "adds %[c0], %[t0] \n\t" /* add low word to c0 */
        "adcs %[c1], %[t1] \n\t" /* add high word to c1, including carry */
        "adc %[c2], #0 \n\t"     /* add carry to c2 */

        "adds %[i], #4 \n\t"          /* i += 4 */
        "cmp %[i], %[k] \n\t"         /* i <= k? */
        "bge 5f \n\t"                 /*   if not, exit the loop */
        "subs %[tt], %[k], %[i] \n\t" /* tt = k - i */
        "cmp %[i], %[tt] \n\t"        /* i <= k - i? */
        "ble 3b \n\t"                 /*   if so, continue looping */

        "5: \n\t" /* end inner loop */

        "str %[c0], [%[result], %[k]] \n\t" /* result[k] = c0 */
        "mov %[c0], %[c1] \n\t"     /* c0 = c1 */
        "mov %[c1], %[c2] \n\t"     /* c1 = c2 */
        "movs %[c2], #0 \n\t"       /* c2 = 0 */
        "adds %[k], #4 \n\t"        /* k += 4 */
        "cmp %[k], %[eccd] \n\t"    /* k < uECC_WORDS (times 4) ? */
        "blt 1b \n\t"               /*   if not, loop back, start with i = 0 */
        "cmp %[k], %[eccd2m1] \n\t" /* k < uECC_WORDS * 2 - 1 (times 4) ? */
        "blt 2b \n\t"               /*   if not, loop back, start with i = (k + 1) - uECC_WORDS */
        /* end outer loop */

        "str %[c0], [%[result], %[k]] \n\t" /* result[uECC_WORDS * 2 - 1] = c0 */
    #if (uECC_PLATFORM != uECC_arm_thumb2)
        ".syntax divided \n\t"
    #endif
        : [c0] "+r" (c0), [c1] "+r" (c1), [c2] "+r" (c2),
          [k] "+r" (k), [i] "=&r" (i), [tt] "=&r" (tt), [t0] "=&r" (t0), [t1] "=&r" (t1)
        : [result] "r" (result), [left] "r" (left),
          [eccd] "I" (uECC_WORDS * 4), [eccdm1] "I" ((uECC_WORDS-1) * 4),
          [eccd2m1] "I" ((uECC_WORDS * 2 - 1) * 4)
        : "cc", "memory"
    );

#else

    register uint32_t *r0 __asm__("r0") = result;
    register const uint32_t *r1 __asm__("r1") = left;

    __asm__ volatile (
        ".syntax unified \n\t"
        "movs r2, #0 \n\t" /* c0 = 0 */
        "movs r3, #0 \n\t" /* c1 = 0 */
        "movs r4, #0 \n\t" /* c2 = 0 */
        "movs r5, #0 \n\t" /* k = 0 */

        "push {r0} \n\t" /* keep result on the stack */

        "1: \n\t" /* outer loop (k < uECC_WORDS) */
        "movs r6, #0 \n\t" /* r6 = i = 0 */
        "b 3f \n\t"

        "2: \n\t" /* outer loop (k >= uECC_WORDS) */
        "movs r6, r5 \n\t"        /* r6 = k */
        "subs r6, %[eccdm1] \n\t" /* r6 = i = k - (uECC_WORDS - 1) (times 4) */

        "3: \n\t" /* inner loop */
        "push {r2, r3, r4, r5} \n\t" /* push things, r2 (c0) is at the top of stack. */
        "subs r7, r5, r6 \n\t"       /* r7 = k - i */

        "ldr r3, [r1, r7] \n\t" /* r3 = left[k - i] */
        "ldr r0, [r1, r6] \n\t" /* r0 = left[i] */

        "lsrs r2, r0, #16 \n\t" /* r2 = a1 */
        "uxth r0, r0 \n\t"      /* r0 = a0 */

        "lsrs r4, r3, #16 \n\t" /* r4 = b1 */
        "uxth r3, r3 \n\t"      /* r3 = b0 */

        "movs r5, r2 \n\t"     /* r5 = a1 */
        "muls r5, r4, r5 \n\t" /* r5 = a1 * b1 */
        "muls r2, r3, r2 \n\t" /* r2 = b0 * a1 */
        "muls r4, r0, r4 \n\t" /* r4 = a0 * b1 */
        "muls r0, r3, r0 \n\t" /* r0 = a0 * b0 */

        "movs r3, #0 \n\t"  /* r3 = 0 */
        "adds r2, r4 \n\t"  /* r2 = b0 * a1 + a0 * b1 */
        "adcs r3, r3 \n\t"  /* r3 = carry */
        "lsls r3, #16 \n\t" /* r3 = carry << 16 */
        "adds r5, r3 \n\t"  /* r5 = a1 * b1 + carry */

        "lsls r3, r2, #16 \n\t" /* r3 = (b0 * a1 + a0 * b1) << 16 */
        "lsrs r2, #16 \n\t"     /* r2 = (b0 * a1 + a0 * b1) >> 16 */
        "adds r0, r3 \n\t"      /* r0 = low word = a0 * b0 + ((b0 * a1 + a0 * b1) << 16) */
        "adcs r5, r2 \n\t"      /* r5 = high word = a1 * b1 + carry + ((b0 * a1 + a0 * b1) >> 16) */

        "movs r3, #0 \n\t"  /* r3 = 0 */
        "cmp r6, r7 \n\t"   /* (i < k - i) ? */
        "mov r7, r3 \n\t"   /* r7 = 0 (does not affect condition)*/
        "bge 4f \n\t"       /*   if i >= k - i, skip */
        "lsls r5, #1 \n\t"  /* high word << 1 */
        "adcs r7, r3 \n\t"  /* r7 = carry bit for c2 */
        "lsls r0, #1 \n\t"  /* low word << 1 */
        "adcs r5, r3 \n\t"  /* add carry from shift to high word */

        "4: \n\t"
        "pop {r2, r3, r4} \n\t" /* r2 = c0, r3 = c1, r4 = c2 */
        "adds r2, r0 \n\t"      /* add low word to c0 */
        "adcs r3, r5 \n\t"      /* add high word to c1, including carry */
        "movs r0, #0 \n\t"      /* r0 = 0 (does not affect carry bit) */
        "adcs r4, r0 \n\t"      /* add carry to c2 */
        "adds r4, r7 \n\t"      /* add carry from doubling (if any) */

        "pop {r5} \n\t" /* r5 = k */

        "adds r6, #4 \n\t"     /* i += 4 */
        "cmp r6, r5 \n\t"      /* i <= k? */
        "bge 5f \n\t"          /*   if not, exit the loop */
        "subs r7, r5, r6 \n\t" /* r7 = k - i */
        "cmp r6, r7 \n\t"      /* i <= k - i? */
        "ble 3b \n\t"          /*   if so, continue looping */

        "5: \n\t" /* end inner loop */

        "ldr r0, [sp, #0] \n\t" /* r0 = result */

        "str r2, [r0, r5] \n\t"   /* result[k] = c0 */
        "mov r2, r3 \n\t"         /* c0 = c1 */
        "mov r3, r4 \n\t"         /* c1 = c2 */
        "movs r4, #0 \n\t"        /* c2 = 0 */
        "adds r5, #4 \n\t"        /* k += 4 */
        "cmp r5, %[eccd] \n\t"    /* k < uECC_WORDS (times 4) ? */
        "blt 1b \n\t"             /*   if not, loop back, start with i = 0 */
        "cmp r5, %[eccd2m1] \n\t" /* k < uECC_WORDS * 2 - 1 (times 4) ? */
        "blt 2b \n\t"             /*   if not, loop back, start with i = (k + 1) - uECC_WORDS */
        /* end outer loop */

        "str r2, [r0, r5] \n\t" /* result[uECC_WORDS * 2 - 1] = c0 */
        "pop {r0} \n\t"        /* pop result off the stack */

        ".syntax divided \n\t"
        : [r0] "+l" (r0), [r1] "+l" (r1)
        : [eccd] "I" (uECC_WORDS * 4), [eccdm1] "I" ((uECC_WORDS-1) * 4),
          [eccd2m1] "I" ((uECC_WORDS * 2 - 1) * 4)
        : "r2", "r3", "r4", "r5", "r6", "r7", "cc", "memory"
    );
#endif
}
#define asm_square 1
#endif /* !asm_square */
#endif /* uECC_SQUARE_FUNC */
