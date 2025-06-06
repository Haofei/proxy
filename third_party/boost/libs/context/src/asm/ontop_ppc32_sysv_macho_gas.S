/*
            Copyright Oliver Kowalke 2009.
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENSE_1_0.txt or copy at
          http://www.boost.org/LICENSE_1_0.txt)
*/

/******************************************************
 *                                                     *
 *  -------------------------------------------------  *
 *  |  0  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  *
 *  -------------------------------------------------  *
 *  |  0  |  4  |  8  |  12 |  16 |  20 |  24 |  28 |  *
 *  -------------------------------------------------  *
 *  |    F14    |    F15    |    F16    |    F17    |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  8  |  9  |  10 |  11 |  12 |  13 |  14 |  15 |  *
 *  -------------------------------------------------  *
 *  |  32 |  36 |  40 |  44 |  48 |  52 |  56 |  60 |  *
 *  -------------------------------------------------  *
 *  |    F18    |    F19    |    F20    |    F21    |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  16 |  17 |  18 |  19 |  20 |  21 |  22 |  23 |  *
 *  -------------------------------------------------  *
 *  |  64 |  68 |  72 |  76 |  80 |  84 |  88 |  92 |  *
 *  -------------------------------------------------  *
 *  |    F22    |    F23    |    F24    |    F25    |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  24 |  25 |  26 |  27 |  28 |  29 |  30 |  31 |  *
 *  -------------------------------------------------  *
 *  |  96 | 100 | 104 | 108 | 112 | 116 | 120 | 124 |  *
 *  -------------------------------------------------  *
 *  |    F26    |    F27    |    F28    |    F29    |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  32 |  33 |  34 |  35 |  36 |  37 |  38 |  39 |  *
 *  -------------------------------------------------  *
 *  | 128 | 132 | 136 | 140 | 144 | 148 | 152 | 156 |  *
 *  -------------------------------------------------  *
 *  |    F30    |    F31    |   fpscr   | R13 | R14 |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  40 |  41 |  42 |  43 |  44 |  45 |  46 |  47 |  *
 *  -------------------------------------------------  *
 *  | 160 | 164 | 168 | 172 | 176 | 180 | 184 | 188 |  *
 *  -------------------------------------------------  *
 *  | R15 | R16 | R17 | R18 | R19 | R20 | R21 | R22 |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  48 |  49 |  50 |  51 |  52 |  53 |  54 |  55 |  *
 *  -------------------------------------------------  *
 *  | 192 | 196 | 200 | 204 | 208 | 212 | 216 | 220 |  *
 *  -------------------------------------------------  *
 *  | R23 | R24 | R25 | R26 | R27 | R28 | R29 | R30 |  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  56 |  57 |  58 |  59 |  60 |  61 |  62 |  63 |  *
 *  -------------------------------------------------  *
 *  | 224 | 228 | 232 | 236 | 240 | 244 | 248 | 252 |  *
 *  -------------------------------------------------  *
 *  | R31 |hiddn|  CR |  LR |  PC |bchai|linkr| FCTX|  *
 *  -------------------------------------------------  *
 *  -------------------------------------------------  *
 *  |  64 |                                         |  *
 *  -------------------------------------------------  *
 *  | 256 |                                         |  *
 *  -------------------------------------------------  *
 *  | DATA|                                         |  *
 *  -------------------------------------------------  *
 *                                                     *
 *******************************************************/

.text
.private_extern _ontop_fcontext
.globl _ontop_fcontext
.align 2
_ontop_fcontext:
    ; reserve space on stack
    subi  r1, r1, 244

    stfd  f14, 0(r1)  ; save F14
    stfd  f15, 8(r1)  ; save F15
    stfd  f16, 16(r1)  ; save F16
    stfd  f17, 24(r1)  ; save F17
    stfd  f18, 32(r1)  ; save F18
    stfd  f19, 40(r1)  ; save F19
    stfd  f20, 48(r1)  ; save F20
    stfd  f21, 56(r1)  ; save F21
    stfd  f22, 64(r1)  ; save F22
    stfd  f23, 72(r1)  ; save F23
    stfd  f24, 80(r1)  ; save F24
    stfd  f25, 88(r1)  ; save F25
    stfd  f26, 96(r1)  ; save F26
    stfd  f27, 104(r1)  ; save F27
    stfd  f28, 112(r1)  ; save F28
    stfd  f29, 120(r1)  ; save F29
    stfd  f30, 128(r1)  ; save F30
    stfd  f31, 136(r1)  ; save F31
    mffs  f0  ; load FPSCR
    stfd  f0, 144(r1)  ; save FPSCR

    stw  r13, 152(r1)  ; save R13
    stw  r14, 156(r1)  ; save R14
    stw  r15, 160(r1)  ; save R15
    stw  r16, 164(r1)  ; save R16
    stw  r17, 168(r1)  ; save R17
    stw  r18, 172(r1)  ; save R18
    stw  r19, 176(r1)  ; save R19
    stw  r20, 180(r1)  ; save R20
    stw  r21, 184(r1)  ; save R21
    stw  r22, 188(r1)  ; save R22
    stw  r23, 192(r1)  ; save R23
    stw  r24, 196(r1)  ; save R24
    stw  r25, 200(r1)  ; save R25
    stw  r26, 204(r1)  ; save R26
    stw  r27, 208(r1)  ; save R27
    stw  r28, 212(r1)  ; save R28
    stw  r29, 216(r1)  ; save R29
    stw  r30, 220(r1)  ; save R30
    stw  r31, 224(r1)  ; save R31
    stw  r3,  228(r1)  ; save hidden

    ; save CR
    mfcr  r0
    stw   r0, 232(r1)
    ; save LR
    mflr  r0
    stw   r0, 236(r1)
    ; save LR as PC
    stw   r0, 240(r1)

    ; store RSP (pointing to context-data) in R7
    mr  r7, r1

    ; restore RSP (pointing to context-data) from R4
    mr  r1, r4

    lfd  f14, 0(r1)  ; restore F14
    lfd  f15, 8(r1)  ; restore F15
    lfd  f16, 16(r1)  ; restore F16
    lfd  f17, 24(r1)  ; restore F17
    lfd  f18, 32(r1)  ; restore F18
    lfd  f19, 40(r1)  ; restore F19
    lfd  f20, 48(r1)  ; restore F20
    lfd  f21, 56(r1)  ; restore F21
    lfd  f22, 64(r1)  ; restore F22
    lfd  f23, 72(r1)  ; restore F23
    lfd  f24, 80(r1)  ; restore F24
    lfd  f25, 88(r1)  ; restore F25
    lfd  f26, 96(r1)  ; restore F26
    lfd  f27, 104(r1)  ; restore F27
    lfd  f28, 112(r1)  ; restore F28
    lfd  f29, 120(r1)  ; restore F29
    lfd  f30, 128(r1)  ; restore F30
    lfd  f31, 136(r1)  ; restore F31
    lfd  f0,  144(r1)  ; load FPSCR
    mtfsf  0xff, f0  ; restore FPSCR

    lwz  r13, 152(r1)  ; restore R13
    lwz  r14, 156(r1)  ; restore R14
    lwz  r15, 160(r1)  ; restore R15
    lwz  r16, 164(r1)  ; restore R16
    lwz  r17, 168(r1)  ; restore R17
    lwz  r18, 172(r1)  ; restore R18
    lwz  r19, 176(r1)  ; restore R19
    lwz  r20, 180(r1)  ; restore R20
    lwz  r21, 184(r1)  ; restore R21
    lwz  r22, 188(r1)  ; restore R22
    lwz  r23, 192(r1)  ; restore R23
    lwz  r24, 196(r1)  ; restore R24
    lwz  r25, 200(r1)  ; restore R25
    lwz  r26, 204(r1)  ; restore R26
    lwz  r27, 208(r1)  ; restore R27
    lwz  r28, 212(r1)  ; restore R28
    lwz  r29, 216(r1)  ; restore R29
    lwz  r30, 220(r1)  ; restore R30
    lwz  r31, 224(r1)  ; restore R31
    lwz  r3,  228(r1)  ; restore hidden

    ; restore CR
    lwz   r0, 232(r1)
    mtcr  r0
    ; restore LR
    lwz   r0, 236(r1)
    mtlr  r0
    ; ignore PC

    ; adjust stack
    addi  r1, r1, 244

    ; Need to pass ontop_fcontext_tail(
    ;   hidden R3,
    ;   R4 = ignore,
    ;   R5 = data,
    ;   R6 = ontop-function,
    ;   R7 = fcontext_t
    ; )
    ; All of these registers are correctly set at this point
    b _ontop_fcontext_tail
