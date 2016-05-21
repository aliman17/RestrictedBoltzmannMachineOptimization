	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_W_update3
	.align	4, 0x90
_W_update3:                             ## @W_update3
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -136(%rbp)
	movq	%rsi, -144(%rbp)
	movq	-136(%rbp), %rsi
	movq	%rsi, -104(%rbp)
	movsd	(%rsi), %xmm0           ## xmm0 = mem[0],zero
	movsd	%xmm0, -112(%rbp)
	movq	%xmm0, %xmm0            ## xmm0 = xmm0[0],zero
	movapd	%xmm0, -128(%rbp)
	movapd	%xmm0, -160(%rbp)
	movq	-144(%rbp), %rsi
	movq	%rsi, -8(%rbp)
	movsd	(%rsi), %xmm0           ## xmm0 = mem[0],zero
	movsd	%xmm0, -16(%rbp)
	movq	%xmm0, %xmm0            ## xmm0 = xmm0[0],zero
	movapd	%xmm0, -32(%rbp)
	movapd	%xmm0, -176(%rbp)
	movapd	-160(%rbp), %xmm1
	movapd	%xmm1, -48(%rbp)
	movapd	%xmm0, -64(%rbp)
	movapd	-48(%rbp), %xmm1
	addsd	%xmm0, %xmm1
	movapd	%xmm1, -48(%rbp)
	movapd	%xmm1, -160(%rbp)
	movq	-136(%rbp), %rsi
	movq	%rsi, -72(%rbp)
	movapd	%xmm1, -96(%rbp)
	movq	-72(%rbp), %rsi
	movsd	%xmm1, (%rsi)
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
