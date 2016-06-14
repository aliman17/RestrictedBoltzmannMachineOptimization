	.section __TEXT,__text
L..LNDBG_TX:
# mark_description "Intel(R) C++ Intel(R) 64 Compiler for applications running on Intel(R) 64, Version 16.0.0.083 Build 20150803";
# mark_description "";
# mark_description "-O3 -S -g -Wa,-alh -fverbose-asm";
	.file "c_update.cpp"
	.section	__TEXT, __text
L_TXTST0:
L_2__routine_start___Z8c_updatePdS_S_di_0:
# -- Begin  __Z8c_updatePdS_S_di
	.section	__TEXT, __text
# mark_begin;
       .align    4
	.globl __Z8c_updatePdS_S_di
# --- c_update(double *, double *, double *, double, int)
__Z8c_updatePdS_S_di:
# parameter 1(c): %rdi
# parameter 2(h0_cap): %rsi
# parameter 3(h1_cap): %rdx
# parameter 4(lambda): %xmm0
# parameter 5(n): %ecx
L_B1.1:                         # Preds L_B1.0
..LCFI1:
L____tag_value___Z8c_updatePdS_S_di.2:
L_L3:
                                                          #2.83
L..LN0:
        pushq     %r15                                          #2.83
..LCFI2:
L..LN1:
        movq      %rdx, %r8                                     #2.83
L..LN2:
        testl     %ecx, %ecx                                    #6.29
L..LN3:
        jle       L_B1.43       # Prob 50%                      #6.29
L..LN4:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 ecx xmm0
L_B1.2:                         # Preds L_B1.1
L..LN5:
        cmpq      %rsi, %rdi                                    #8.33
L..LN6:
        jbe       L_B1.4        # Prob 50%                      #8.33
L..LN7:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 ecx xmm0
L_B1.3:                         # Preds L_B1.2
L..LN8:
        movslq    %ecx, %rax                                    #2.6
L..LN9:
        movq      %rdi, %rdx                                    #8.33
L..LN10:
        subq      %rsi, %rdx                                    #8.33
L..LN11:
        lea       (,%rax,8), %r9                                #8.33
L..LN12:
        cmpq      %rdx, %r9                                     #8.33
L..LN13:
        jle       L_B1.6        # Prob 50%                      #8.33
L..LN14:
                                # LOE rax rbx rbp rsi rdi r8 r9 r12 r13 r14 ecx xmm0
L_B1.4:                         # Preds L_B1.2 L_B1.3
L..LN15:
        cmpq      %rdi, %rsi                                    #8.33
L..LN16:
        jbe       L_B1.37       # Prob 50%                      #8.33
L..LN17:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 ecx xmm0
L_B1.5:                         # Preds L_B1.4
L..LN18:
        movslq    %ecx, %rax                                    #2.6
L..LN19:
        movq      %rsi, %rdx                                    #8.33
L..LN20:
        subq      %rdi, %rdx                                    #8.33
L..LN21:
        lea       (,%rax,8), %r9                                #8.33
L..LN22:
        cmpq      %r9, %rdx                                     #8.33
L..LN23:
        jl        L_B1.37       # Prob 50%                      #8.33
L..LN24:
                                # LOE rax rbx rbp rsi rdi r8 r9 r12 r13 r14 ecx xmm0
L_B1.6:                         # Preds L_B1.5 L_B1.3
L..LN25:
        cmpq      %r8, %rdi                                     #8.45
L..LN26:
        jbe       L_B1.8        # Prob 50%                      #8.45
L..LN27:
                                # LOE rax rbx rbp rsi rdi r8 r9 r12 r13 r14 ecx xmm0
L_B1.7:                         # Preds L_B1.6
L..LN28:
        movq      %rdi, %rdx                                    #8.45
L..LN29:
        subq      %r8, %rdx                                     #8.45
L..LN30:
        cmpq      %rdx, %r9                                     #8.45
L..LN31:
        jle       L_B1.10       # Prob 50%                      #8.45
L..LN32:
                                # LOE rax rbx rbp rsi rdi r8 r9 r12 r13 r14 ecx xmm0
L_B1.8:                         # Preds L_B1.6 L_B1.7
L..LN33:
        cmpq      %rdi, %r8                                     #8.45
L..LN34:
        jbe       L_B1.37       # Prob 50%                      #8.45
L..LN35:
                                # LOE rax rbx rbp rsi rdi r8 r9 r12 r13 r14 ecx xmm0
L_B1.9:                         # Preds L_B1.8
L..LN36:
        movq      %r8, %rdx                                     #8.45
L..LN37:
        subq      %rdi, %rdx                                    #8.45
L..LN38:
        cmpq      %r9, %rdx                                     #8.45
L..LN39:
        jl        L_B1.37       # Prob 50%                      #8.45
L..LN40:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx xmm0
L_B1.10:                        # Preds L_B1.7 L_B1.9
L..LN41:
        xorb      %r10b, %r10b                                  #6.9
L..LN42:
        cmpl      $8, %ecx                                      #6.9
L..LN43:
        jl        L_B1.45       # Prob 10%                      #6.9
L..LN44:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx r10b xmm0
L_B1.11:                        # Preds L_B1.10
L..LN45:
        movq      %rdi, %r9                                     #6.9
L..LN46:
        andq      $15, %r9                                      #6.9
L..LN47:
        testl     %r9d, %r9d                                    #6.9
L..LN48:
        je        L_B1.14       # Prob 50%                      #6.9
L..LN49:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx r9d r10b xmm0
L_B1.12:                        # Preds L_B1.11
L..LN50:
        testl     $7, %r9d                                      #6.9
L..LN51:
        jne       L_B1.45       # Prob 10%                      #6.9
L..LN52:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx r10b xmm0
L_B1.13:                        # Preds L_B1.12
L..LN53:
        movl      $1, %r9d                                      #6.9
L..LN54:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx r9d r10b xmm0
L_B1.14:                        # Preds L_B1.13 L_B1.11
L..LN55:
        lea       8(%r9), %edx                                  #6.9
L..LN56:
        cmpl      %edx, %ecx                                    #6.9
L..LN57:
        jl        L_B1.45       # Prob 10%                      #6.9
L..LN58:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 ecx r9d r10b xmm0
L_B1.15:                        # Preds L_B1.14
L..LN59:
        movl      %ecx, %edx                                    #6.9
L..LN60:
        xorl      %r15d, %r15d                                  #6.9
L..LN61:
        subl      %r9d, %edx                                    #6.9
L..LN62:
        andl      $7, %edx                                      #6.9
L..LN63:
        negl      %edx                                          #6.9
L..LN64:
        movl      %r9d, %r11d                                   #6.9
L..LN65:
        addl      %ecx, %edx                                    #6.9
L..LN66:
        testq     %r11, %r11                                    #6.9
L..LN67:
        jbe       L_B1.19       # Prob 9%                       #6.9
L..LN68:
                                # LOE rax rbx rbp rsi rdi r8 r11 r12 r13 r14 r15 edx ecx r9d r10b xmm0
L_B1.17:                        # Preds L_B1.15 L_B1.17
L_L7:           # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 1
                # PEELED LOOP FOR VECTORIZATION
L..LN69:
        movsd     (%rsi,%r15,8), %xmm1                          #8.33
L..LN70:
        subsd     (%r8,%r15,8), %xmm1                           #8.45
L..LN71:
        mulsd     %xmm0, %xmm1                                  #8.45
L..LN72:
        addsd     (%rdi,%r15,8), %xmm1                          #8.45
L..LN73:
        movsd     %xmm1, (%rdi,%r15,8)                          #8.9
L..LN74:
        incq      %r15                                          #6.9
L..LN75:
        cmpq      %r11, %r15                                    #6.9
L..LN76:
        jb        L_B1.17       # Prob 82%                      #6.9
L..LN77:
                                # LOE rax rbx rbp rsi rdi r8 r11 r12 r13 r14 r15 edx ecx r9d r10b xmm0
L_B1.19:                        # Preds L_B1.17 L_B1.15
L..LN78:
        movl      %r9d, %r9d                                    #8.45
L..LN79:
        lea       (%r8,%r9,8), %r15                             #8.45
L..LN80:
        testq     $15, %r15                                     #6.9
L..LN81:
        je        L_B1.23       # Prob 60%                      #6.9
L..LN82:
                                # LOE rax rbx rbp rsi rdi r8 r11 r12 r13 r14 edx ecx r10b xmm0
L_B1.20:                        # Preds L_B1.19
L..LN83:
        movddup   %xmm0, %xmm1                                  #2.6
L..LN84:
        movslq    %edx, %r9                                     #6.9
L..LN85:
                                # LOE rax rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 edx ecx r10b xmm0 xmm1
L_B1.21:                        # Preds L_B1.21 L_B1.20
L_L8:           # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 1
                # LOOP WAS VECTORIZED
                # ALTERNATE ALIGNMENT VECTOR LOOP
L..LN86:
        movsd     (%rsi,%r11,8), %xmm3                          #8.33
L..LN87:
        movsd     16(%rsi,%r11,8), %xmm5                        #8.33
L..LN88:
        movsd     32(%rsi,%r11,8), %xmm7                        #8.33
L..LN89:
        movsd     48(%rsi,%r11,8), %xmm9                        #8.33
L..LN90:
        movsd     (%r8,%r11,8), %xmm2                           #8.45
L..LN91:
        movsd     16(%r8,%r11,8), %xmm4                         #8.45
L..LN92:
        movsd     32(%r8,%r11,8), %xmm6                         #8.45
L..LN93:
        movsd     48(%r8,%r11,8), %xmm8                         #8.45
L..LN94:
        movhpd    8(%r8,%r11,8), %xmm2                          #8.45
L..LN95:
        movhpd    24(%r8,%r11,8), %xmm4                         #8.45
L..LN96:
        movhpd    40(%r8,%r11,8), %xmm6                         #8.45
L..LN97:
        movhpd    56(%r8,%r11,8), %xmm8                         #8.45
L..LN98:
        movhpd    8(%rsi,%r11,8), %xmm3                         #8.33
L..LN99:
        movhpd    24(%rsi,%r11,8), %xmm5                        #8.33
L..LN100:
        movhpd    40(%rsi,%r11,8), %xmm7                        #8.33
L..LN101:
        movhpd    56(%rsi,%r11,8), %xmm9                        #8.33
L..LN102:
        subpd     %xmm2, %xmm3                                  #8.45
L..LN103:
        subpd     %xmm4, %xmm5                                  #8.45
L..LN104:
        subpd     %xmm6, %xmm7                                  #8.45
L..LN105:
        subpd     %xmm8, %xmm9                                  #8.45
L..LN106:
        mulpd     %xmm1, %xmm3                                  #8.45
L..LN107:
        mulpd     %xmm1, %xmm5                                  #8.45
L..LN108:
        mulpd     %xmm1, %xmm7                                  #8.45
L..LN109:
        mulpd     %xmm1, %xmm9                                  #8.45
L..LN110:
        addpd     (%rdi,%r11,8), %xmm3                          #8.45
L..LN111:
        addpd     16(%rdi,%r11,8), %xmm5                        #8.45
L..LN112:
        addpd     32(%rdi,%r11,8), %xmm7                        #8.45
L..LN113:
        addpd     48(%rdi,%r11,8), %xmm9                        #8.45
L..LN114:
        movaps    %xmm3, (%rdi,%r11,8)                          #8.9
L..LN115:
        movaps    %xmm5, 16(%rdi,%r11,8)                        #8.9
L..LN116:
        movaps    %xmm7, 32(%rdi,%r11,8)                        #8.9
L..LN117:
        movaps    %xmm9, 48(%rdi,%r11,8)                        #8.9
L..LN118:
        addq      $8, %r11                                      #6.9
L..LN119:
        cmpq      %r9, %r11                                     #6.9
L..LN120:
        jb        L_B1.21       # Prob 82%                      #6.9
L..LN121:
        jmp       L_B1.26       # Prob 100%                     #6.9
L..LN122:
                                # LOE rax rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 edx ecx r10b xmm0 xmm1
L_B1.23:                        # Preds L_B1.19
L..LN123:
        movddup   %xmm0, %xmm1                                  #2.6
L..LN124:
        movslq    %edx, %r9                                     #6.9
L..LN125:
                                # LOE rax rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 edx ecx r10b xmm0 xmm1
L_B1.24:                        # Preds L_B1.24 L_B1.23
L_L9:           # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 1
                # LOOP WAS UNROLLED BY 4
                # LOOP WAS VECTORIZED
                # VECTORIZATION SPEEDUP COEFFECIENT 2.974609
                # VECTOR TRIP COUNT IS ESTIMATED CONSTANT
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.437500
L..LN126:
        movsd     (%rsi,%r11,8), %xmm2                          #8.33
L..LN127:
        movsd     16(%rsi,%r11,8), %xmm3                        #8.33
L..LN128:
        movsd     32(%rsi,%r11,8), %xmm4                        #8.33
L..LN129:
        movsd     48(%rsi,%r11,8), %xmm5                        #8.33
L..LN130:
        movhpd    8(%rsi,%r11,8), %xmm2                         #8.33
L..LN131:
        movhpd    24(%rsi,%r11,8), %xmm3                        #8.33
L..LN132:
        movhpd    40(%rsi,%r11,8), %xmm4                        #8.33
L..LN133:
        movhpd    56(%rsi,%r11,8), %xmm5                        #8.33
L..LN134:
        subpd     (%r8,%r11,8), %xmm2                           #8.45
L..LN135:
        subpd     16(%r8,%r11,8), %xmm3                         #8.45
L..LN136:
        subpd     32(%r8,%r11,8), %xmm4                         #8.45
L..LN137:
        subpd     48(%r8,%r11,8), %xmm5                         #8.45
L..LN138:
        mulpd     %xmm1, %xmm2                                  #8.45
L..LN139:
        mulpd     %xmm1, %xmm3                                  #8.45
L..LN140:
        mulpd     %xmm1, %xmm4                                  #8.45
L..LN141:
        mulpd     %xmm1, %xmm5                                  #8.45
L..LN142:
        addpd     (%rdi,%r11,8), %xmm2                          #8.45
L..LN143:
        addpd     16(%rdi,%r11,8), %xmm3                        #8.45
L..LN144:
        addpd     32(%rdi,%r11,8), %xmm4                        #8.45
L..LN145:
        addpd     48(%rdi,%r11,8), %xmm5                        #8.45
L..LN146:
        movaps    %xmm2, (%rdi,%r11,8)                          #8.9
L..LN147:
        movaps    %xmm3, 16(%rdi,%r11,8)                        #8.9
L..LN148:
        movaps    %xmm4, 32(%rdi,%r11,8)                        #8.9
L..LN149:
        movaps    %xmm5, 48(%rdi,%r11,8)                        #8.9
L..LN150:
        addq      $8, %r11                                      #6.9
L..LN151:
        cmpq      %r9, %r11                                     #6.9
L..LN152:
        jb        L_B1.24       # Prob 82%                      #6.9
L..LN153:
                                # LOE rax rbx rbp rsi rdi r8 r9 r11 r12 r13 r14 edx ecx r10b xmm0 xmm1
L_B1.26:                        # Preds L_B1.24 L_B1.21 L_B1.45
L..LN154:
        lea       1(%rdx), %r9d                                 #6.9
L..LN155:
        cmpl      %r9d, %ecx                                    #6.9
L..LN156:
        jb        L_B1.43       # Prob 50%                      #6.9
L..LN157:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 edx r10b xmm0
L_B1.27:                        # Preds L_B1.26
L..LN158:
        movslq    %edx, %rdx                                    #6.9
L..LN159:
        subq      %rdx, %rax                                    #6.9
L..LN160:
        cmpb      $1, %r10b                                     #6.9
L..LN161:
        jne       L_B1.29       # Prob 50%                      #6.9
L..LN162:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 edx xmm0
L_B1.28:                        # Preds L_B1.29 L_B1.27
L..LN163:
        xorl      %r11d, %r11d                                  #6.9
L..LN164:
        jmp       L_B1.33       # Prob 100%                     #6.9
L..LN165:
                                # LOE rax rbx rbp rsi rdi r8 r11 r12 r13 r14 edx xmm0
L_B1.29:                        # Preds L_B1.27
L..LN166:
        cmpq      $2, %rax                                      #6.9
L..LN167:
        jl        L_B1.28       # Prob 10%                      #6.9
L..LN168:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 edx xmm0
L_B1.30:                        # Preds L_B1.29
L..LN169:
        movslq    %edx, %rdx                                    #8.16
L..LN170:
        movl      %eax, %r11d                                   #6.9
L..LN171:
        andl      $-2, %r11d                                    #6.9
L..LN172:
        xorl      %r9d, %r9d                                    #6.9
L..LN173:
        movslq    %r11d, %r11                                   #6.9
L..LN174:
        movddup   %xmm0, %xmm1                                  #2.6
L..LN175:
        lea       (%r8,%rdx,8), %r10                            #8.45
L..LN176:
        lea       (%rsi,%rdx,8), %rcx                           #8.33
L..LN177:
        lea       (%rdi,%rdx,8), %r15                           #8.16
L..LN178:
                                # LOE rax rcx rbx rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 edx xmm0 xmm1
L_B1.31:                        # Preds L_B1.31 L_B1.30
L_L10:          # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 1
                # LOOP WAS VECTORIZED
                # REMAINDER LOOP FOR VECTORIZATION
                # VECTORIZATION SPEEDUP COEFFECIENT 1.252930
                # VECTOR TRIP COUNT IS ESTIMATED CONSTANT
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.875000
L..LN179:
        movsd     (%rcx,%r9,8), %xmm3                           #8.33
L..LN180:
        movsd     (%r10,%r9,8), %xmm2                           #8.45
L..LN181:
        movhpd    8(%r10,%r9,8), %xmm2                          #8.45
L..LN182:
        movhpd    8(%rcx,%r9,8), %xmm3                          #8.33
L..LN183:
        subpd     %xmm2, %xmm3                                  #8.45
L..LN184:
        mulpd     %xmm1, %xmm3                                  #8.45
L..LN185:
        addpd     (%r15,%r9,8), %xmm3                           #8.45
L..LN186:
        movaps    %xmm3, (%r15,%r9,8)                           #8.9
L..LN187:
        addq      $2, %r9                                       #6.9
L..LN188:
        cmpq      %r11, %r9                                     #6.9
L..LN189:
        jb        L_B1.31       # Prob 82%                      #6.9
L..LN190:
                                # LOE rax rcx rbx rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15 edx xmm0 xmm1
L_B1.33:                        # Preds L_B1.31 L_B1.28
L..LN191:
        cmpq      %rax, %r11                                    #6.9
L..LN192:
        jae       L_B1.43       # Prob 9%                       #6.9
L..LN193:
                                # LOE rax rbx rbp rsi rdi r8 r11 r12 r13 r14 edx xmm0
L_B1.34:                        # Preds L_B1.33
L..LN194:
        movslq    %edx, %rdx                                    #8.16
L..LN195:
        lea       (%r8,%rdx,8), %r8                             #8.45
L..LN196:
        lea       (%rsi,%rdx,8), %rcx                           #8.33
L..LN197:
        lea       (%rdi,%rdx,8), %rdx                           #8.16
L..LN198:
                                # LOE rax rdx rcx rbx rbp r8 r11 r12 r13 r14 xmm0
L_B1.35:                        # Preds L_B1.35 L_B1.34
L_L11:          # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 1
                # REMAINDER LOOP FOR VECTORIZATION
L..LN199:
        movsd     (%rcx,%r11,8), %xmm1                          #8.33
L..LN200:
        subsd     (%r8,%r11,8), %xmm1                           #8.45
L..LN201:
        mulsd     %xmm0, %xmm1                                  #8.45
L..LN202:
        addsd     (%rdx,%r11,8), %xmm1                          #8.45
L..LN203:
        movsd     %xmm1, (%rdx,%r11,8)                          #8.9
L..LN204:
        incq      %r11                                          #6.9
L..LN205:
        cmpq      %rax, %r11                                    #6.9
L..LN206:
        jb        L_B1.35       # Prob 82%                      #6.9
L..LN207:
        jmp       L_B1.43       # Prob 100%                     #6.9
L..LN208:
                                # LOE rax rdx rcx rbx rbp r8 r11 r12 r13 r14 xmm0
L_B1.37:                        # Preds L_B1.4 L_B1.5 L_B1.8 L_B1.9
L..LN209:
        movl      %ecx, %eax                                    #6.9
L..LN210:
        movl      $1, %r9d                                      #6.9
L..LN211:
        shrl      $31, %eax                                     #6.9
L..LN212:
        xorl      %edx, %edx                                    #6.9
L..LN213:
        addl      %ecx, %eax                                    #2.6
L..LN214:
        sarl      $1, %eax                                      #2.6
L..LN215:
        testl     %eax, %eax                                    #6.9
L..LN216:
        jbe       L_B1.41       # Prob 9%                       #6.9
        .align    4
L..LN217:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 eax edx ecx r9d xmm0
L_B1.39:                        # Preds L_B1.37 L_B1.39
L_L12:          # optimization report
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 2
                # LOOP WAS UNROLLED BY 2
                # %s was not vectorized: non-vectorizable loop instance from multiversioning
                # VECTOR TRIP COUNT IS ESTIMATED CONSTANT
L..LN218:
        lea       (%rdx,%rdx), %r9d                             #8.16
L..LN219:
        movslq    %r9d, %r9                                     #8.45
L..LN220:
        incl      %edx                                          #6.9
L..LN221:
        cmpl      %eax, %edx                                    #6.9
L..LN222:
        movsd     (%rsi,%r9,8), %xmm1                           #8.33
L..LN223:
        subsd     (%r8,%r9,8), %xmm1                            #8.45
L..LN224:
        mulsd     %xmm0, %xmm1                                  #8.45
L..LN225:
        addsd     (%rdi,%r9,8), %xmm1                           #8.45
L..LN226:
        movsd     %xmm1, (%rdi,%r9,8)                           #8.9
L..LN227:
        movsd     8(%rsi,%r9,8), %xmm2                          #8.33
L..LN228:
        subsd     8(%r8,%r9,8), %xmm2                           #8.45
L..LN229:
        mulsd     %xmm0, %xmm2                                  #8.45
L..LN230:
        addsd     8(%rdi,%r9,8), %xmm2                          #8.45
L..LN231:
        movsd     %xmm2, 8(%rdi,%r9,8)                          #8.9
L..LN232:
        jb        L_B1.39       # Prob 63%                      #6.9
L..LN233:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 eax edx ecx xmm0
L_B1.40:                        # Preds L_B1.39
L..LN234:
        lea       1(%rdx,%rdx), %r9d                            #8.9
L..LN235:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 ecx r9d xmm0
L_B1.41:                        # Preds L_B1.40 L_B1.37
L..LN236:
        lea       -1(%r9), %eax                                 #6.9
L..LN237:
        cmpl      %eax, %ecx                                    #6.9
L..LN238:
        jbe       L_B1.43       # Prob 9%                       #6.9
L..LN239:
                                # LOE rbx rbp rsi rdi r8 r12 r13 r14 r9d xmm0
L_B1.42:                        # Preds L_B1.41
L_L13:          # optimization report
                # REMAINDER LOOP
                # MULTIVERSIONED FOR DATA DEPENDENCE, VER 2
L..LN240:
        movslq    %r9d, %r9                                     #8.16
L..LN241:
        movsd     -8(%rsi,%r9,8), %xmm1                         #8.33
L..LN242:
        subsd     -8(%r8,%r9,8), %xmm1                          #8.45
L..LN243:
        mulsd     %xmm1, %xmm0                                  #8.45
L..LN244:
        addsd     -8(%rdi,%r9,8), %xmm0                         #8.45
L..LN245:
        movsd     %xmm0, -8(%rdi,%r9,8)                         #8.9
L..LN246:
                                # LOE rbx rbp r12 r13 r14
L_B1.43:                        # Preds L_B1.35 L_B1.41 L_B1.1 L_B1.33 L_B1.26
                                #       L_B1.42
..LCFI3:
L..LN247:
        popq      %r15                                          #35.1
..LCFI4:
L..LN248:
        ret                                                     #35.1
..LCFI5:
L..LN249:
                                # LOE
L_B1.45:                        # Preds L_B1.10 L_B1.12 L_B1.14 # Infreq
L..LN250:
        movb      $1, %r10b                                     #6.9
L..LN251:
        xorl      %edx, %edx                                    #6.9
L..LN252:
        jmp       L_B1.26       # Prob 100%                     #6.9
        .align    4
..LCFI6:
L..LN253:
                                # LOE rax rbx rbp rsi rdi r8 r12 r13 r14 edx ecx r10b xmm0
L..LN254:
# mark_end;
L..LN__Z8c_updatePdS_S_di.255:
L..LN__Z8c_updatePdS_S_di:
	.section	__DATA, __data
# -- End  __Z8c_updatePdS_S_di
	.section	__DATA, __data
	.section	__DATA, __debug_opt_rpt
L_L21:
	.ascii ".itt_notify_tab\0"
	.word	258
	.word	48
	.long	8
	.set	L_mach_tmp_00000001,L_L22 - L_L21
	.long	L_mach_tmp_00000001
	.long	48
	.set	L_mach_tmp_00000002,L_L23 - L_L21
	.long	L_mach_tmp_00000002
	.long	102
	.long	0x00000008,0x00000000
	.long	0
	.long	0
	.long	0
	.long	0
	.quad	L_L7
	.long	28
	.long	4
	.quad	L_L8
	.long	28
	.long	16
	.quad	L_L9
	.long	28
	.long	32
	.quad	L_L10
	.long	28
	.long	50
	.quad	L_L11
	.long	28
	.long	68
	.quad	L_L12
	.long	28
	.long	80
	.quad	L_L13
	.long	28
	.long	96
L_L22:
	.long	1769238639
	.long	1635412333
	.long	1852795252
	.long	1885696607
	.long	1601466991
	.long	1936876918
	.long	7237481
	.long	1769238639
	.long	1635412333
	.long	1852795252
	.long	1885696607
	.long	7631471
L_L23:
	.long	42012675
	.long	-2139090933
	.long	-2139061880
	.long	75530368
	.long	-2139090929
	.long	-2139061880
	.long	-2105507712
	.long	545292416
	.long	-2139090927
	.long	-1988066936
	.long	-2105507712
	.long	-929064832
	.long	269556674
	.long	-2121760640
	.long	-2139062144
	.long	-1736406400
	.long	499304400
	.long	-2139090933
	.long	-2139061880
	.long	142639232
	.long	-2139090929
	.long	-2055175544
	.long	-2139062144
	.long	1082163344
	.long	-2004873211
	.word	648
	.globl __Z8c_updatePdS_S_di.eh
// -- Begin DWARF2 SEGMENT __debug_info
	.section __DWARF,__debug_info,regular,debug
	.long 0x000001bd
	.short 0x0002
	.long 0x00000000
	.byte 0x08
//	DW_TAG_compile_unit:
	.byte 0x01
//	DW_AT_comp_dir:
	.long 0x6573552f
	.long 0x6d2f7372
	.long 0x502f6f6f
	.long 0x482f4468
	.long 0x4e465254
	.long 0x72702f43
	.long 0x63656a6f
	.long 0x61462f74
	.long 0x754e7473
	.long 0x6972656d
	.long 0x436c6163
	.long 0x2f65646f
	.long 0x2f6d6272
	.long 0x61647075
	.long 0x00326574
//	DW_AT_name:
	.long 0x70755f63
	.long 0x65746164
	.long 0x7070632e
	.byte 0x00
//	DW_AT_producer:
	.long 0x65746e49
	.long 0x2952286c
	.long 0x2b2b4320
	.long 0x746e4920
	.long 0x52286c65
	.long 0x34362029
	.long 0x6d6f4320
	.long 0x656c6970
	.long 0x6f662072
	.long 0x70612072
	.long 0x63696c70
	.long 0x6f697461
	.long 0x7220736e
	.long 0x696e6e75
	.long 0x6f20676e
	.long 0x6e49206e
	.long 0x286c6574
	.long 0x36202952
	.long 0x56202c34
	.long 0x69737265
	.long 0x31206e6f
	.long 0x2e302e36
	.long 0x38302e30
	.long 0x75422033
	.long 0x20646c69
	.long 0x35313032
	.long 0x33303830
	.short 0x000a
	.long 0x20334f2d
	.long 0x2d20532d
	.long 0x572d2067
	.long 0x612d2c61
	.long 0x2d20686c
	.long 0x72657666
	.long 0x65736f62
	.long 0x6d73612d
	.byte 0x00
//	DW_AT_language:
	.byte 0x04
//	DW_AT_low_pc:
	.quad L..LN0
//	DW_AT_high_pc:
	.quad L..LN__Z8c_updatePdS_S_di.255
//	DW_AT_stmt_list:
	.long 0x00000000
//	DW_TAG_namespace:
	.byte 0x02
//	DW_AT_name:
	.long 0x00647473
//	DW_TAG_namespace:
	.byte 0x02
//	DW_AT_name:
	.long 0x6e675f5f
	.long 0x78635f75
	.short 0x0078
//	DW_TAG_namespace:
	.byte 0x02
//	DW_AT_name:
	.long 0x78635f5f
	.long 0x69626178
	.short 0x3176
	.byte 0x00
//	DW_TAG_subprogram:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.long 0x70755f63
	.long 0x65746164
	.byte 0x00
	.long 0x63385a5f
	.long 0x6470755f
	.long 0x50657461
	.long 0x535f5364
	.long 0x0069645f
//	DW_AT_low_pc:
	.quad L_L3
//	DW_AT_high_pc:
	.quad L..LN__Z8c_updatePdS_S_di.255
//	DW_AT_external:
	.byte 0x01
//	DW_TAG_formal_parameter:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.long 0x000001aa
//	DW_AT_name:
	.short 0x0063
//	DW_AT_location:
	.short 0x5501
//	DW_TAG_formal_parameter:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.long 0x000001aa
//	DW_AT_name:
	.long 0x635f3068
	.short 0x7061
	.byte 0x00
//	DW_AT_location:
	.short 0x5401
//	DW_TAG_formal_parameter:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.long 0x000001aa
//	DW_AT_name:
	.long 0x635f3168
	.short 0x7061
	.byte 0x00
//	DW_AT_location:
	.short 0x5101
//	DW_TAG_formal_parameter:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.long 0x000001af
//	DW_AT_name:
	.long 0x626d616c
	.short 0x6164
	.byte 0x00
//	DW_AT_location:
	.short 0x6101
//	DW_TAG_formal_parameter:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x02
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.long 0x000001b9
//	DW_AT_name:
	.short 0x006e
//	DW_AT_location:
	.short 0x5201
//	DW_TAG_lexical_block:
	.byte 0x05
//	DW_AT_decl_line:
	.byte 0x06
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_low_pc:
	.quad L..LN2
//	DW_AT_high_pc:
	.quad L..LN__Z8c_updatePdS_S_di.255
//	DW_TAG_variable:
	.byte 0x06
//	DW_AT_decl_line:
	.byte 0x06
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.short 0x0069
//	DW_AT_type:
	.long 0x000001b9
	.byte 0x00
	.byte 0x00
//	DW_TAG_pointer_type:
	.byte 0x07
//	DW_AT_type:
	.long 0x000001af
//	DW_TAG_base_type:
	.byte 0x08
//	DW_AT_byte_size:
	.byte 0x08
//	DW_AT_encoding:
	.byte 0x04
//	DW_AT_name:
	.long 0x62756f64
	.short 0x656c
	.byte 0x00
//	DW_TAG_base_type:
	.byte 0x08
//	DW_AT_byte_size:
	.byte 0x04
//	DW_AT_encoding:
	.byte 0x05
//	DW_AT_name:
	.long 0x00746e69
	.byte 0x00
// -- Begin DWARF2 SEGMENT __debug_line
	.section __DWARF,__debug_line,regular,debug
	.long 0x00000235
	.short 0x0002
	.long 0x00000025
	.byte 0x01
	.byte 0x01
	.byte 0xff
	.byte 0x04
	.byte 0x0a
	.long 0x01010100
	.long 0x00000001
	.byte 0x01
	.byte 0x00
	.long 0x70755f63
	.long 0x65746164
	.long 0x7070632e
	.byte 0x00
	.long 0xe7f8e900
	.long 0x069405b9
	.byte 0x00
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN0
	.byte 0x0c
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN2
	.short 0x0403
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN5
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN8
	.short 0x7a03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN9
	.short 0x0603
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN18
	.short 0x7a03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN19
	.short 0x0603
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN41
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN69
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN74
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN78
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN80
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN83
	.short 0x7c03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN84
	.short 0x0403
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN86
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN118
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN123
	.short 0x7c03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN124
	.short 0x0403
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN126
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN150
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN169
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN170
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN174
	.short 0x7c03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN175
	.short 0x0603
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN187
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN194
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN204
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN213
	.short 0x7c03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN215
	.short 0x0403
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN218
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN220
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN222
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN232
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN234
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN236
	.short 0x7e03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN240
	.byte 0x0d
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN247
	.short 0x1b03
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN250
	.short 0x6303
	.byte 0x01
	.byte 0x00
	.byte 0x09
	.byte 0x02
	.quad L..LN__Z8c_updatePdS_S_di
	.byte 0x00
	.byte 0x01
	.byte 0x01
// -- Begin DWARF2 SEGMENT __debug_abbrev
	.section __DWARF,__debug_abbrev,regular,debug
	.byte 0x01
	.byte 0x11
	.byte 0x01
	.byte 0x1b
	.byte 0x08
	.byte 0x03
	.byte 0x08
	.byte 0x25
	.byte 0x08
	.short 0x7681
	.byte 0x08
	.byte 0x13
	.byte 0x0b
	.byte 0x11
	.byte 0x01
	.byte 0x12
	.byte 0x01
	.byte 0x10
	.byte 0x06
	.short 0x0000
	.byte 0x02
	.byte 0x39
	.byte 0x00
	.byte 0x03
	.byte 0x08
	.short 0x0000
	.byte 0x03
	.byte 0x2e
	.byte 0x01
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x03
	.byte 0x08
	.short 0x4087
	.byte 0x08
	.byte 0x11
	.byte 0x01
	.byte 0x12
	.byte 0x01
	.byte 0x3f
	.byte 0x0c
	.short 0x0000
	.byte 0x04
	.byte 0x05
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x49
	.byte 0x13
	.byte 0x03
	.byte 0x08
	.byte 0x02
	.byte 0x0a
	.short 0x0000
	.byte 0x05
	.byte 0x0b
	.byte 0x01
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x11
	.byte 0x01
	.byte 0x12
	.byte 0x01
	.short 0x0000
	.byte 0x06
	.byte 0x34
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x03
	.byte 0x08
	.byte 0x49
	.byte 0x13
	.short 0x0000
	.byte 0x07
	.byte 0x0f
	.byte 0x00
	.byte 0x49
	.byte 0x13
	.short 0x0000
	.byte 0x08
	.byte 0x24
	.byte 0x00
	.byte 0x0b
	.byte 0x0b
	.byte 0x3e
	.byte 0x0b
	.byte 0x03
	.byte 0x08
	.short 0x0000
	.byte 0x00
// -- Begin DWARF2 SEGMENT __debug_frame
	.section __DWARF,__debug_frame,regular,debug
	.long 0x00000014
	.long 0xffffffff
	.long 0x78010001
	.long 0x08070c10
	.long 0x00000190
	.long 0x00000000
	.long 0x00000034
	.long 0x00000000
	.quad ..LCFI1
	.set L_Qlab2,..LCFI6-..LCFI1
	.quad L_Qlab2
	.byte 0x04
	.set L_lab5,..LCFI2-..LCFI1
	.long L_lab5
	.long 0x028f100e
	.byte 0x04
	.set L_lab6,..LCFI3-..LCFI2
	.long L_lab6
	.short 0x04cf
	.set L_lab7,..LCFI4-..LCFI3
	.long L_lab7
	.short 0x080e
	.byte 0x04
	.set L_lab8,..LCFI5-..LCFI4
	.long L_lab8
	.long 0x028f100e
	.byte 0x00
// -- Begin SEGMENT __eh_frame
	.section __TEXT,__eh_frame,coalesced,no_toc+strip_static_syms+live_support
__eh_frame_seg:
EH_frame0:
L_fde_cie_0:
	.long 0x0000001c
	.long 0x00000000
	.long 0x52507a01
	.long 0x10780100
	.short 0x9b06
	.long ___gxx_personality_v0@GOTPCREL+0x4
	.long 0x08070c10
	.long 0x01900190
	.short 0x0000
__Z8c_updatePdS_S_di.eh:
	.long 0x00000034
	.long 0x00000024
	.quad ..LCFI1-__Z8c_updatePdS_S_di.eh-0x8
	.set L_Qlab1,..LCFI6-..LCFI1
	.quad L_Qlab1
	.short 0x0400
	.set L_lab1,..LCFI2-..LCFI1
	.long L_lab1
	.long 0x028f100e
	.byte 0x04
	.set L_lab2,..LCFI3-..LCFI2
	.long L_lab2
	.short 0x04cf
	.set L_lab3,..LCFI4-..LCFI3
	.long L_lab3
	.short 0x080e
	.byte 0x04
	.set L_lab4,..LCFI5-..LCFI4
	.long L_lab4
	.long 0x028f100e
# End
	.subsections_via_symbols
