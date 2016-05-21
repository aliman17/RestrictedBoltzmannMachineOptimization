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
	subq	$96, %rsp
	movq	%rdi, -200(%rbp)
	movq	%rdi, -168(%rbp)
	movsd	(%rdi), %xmm0           ## xmm0 = mem[0],zero
	movsd	%xmm0, -176(%rbp)
	movq	%xmm0, %xmm0            ## xmm0 = xmm0[0],zero
	movapd	%xmm0, -192(%rbp)

	movapd	%xmm0, -224(%rbp)
	movq	-200(%rbp), %rdi

	movq	%rdi, -8(%rbp)
	movapd	%xmm0, -32(%rbp)
	movq	-8(%rbp), %rdi
	movlpd	%xmm0, (%rdi)

	movq	-200(%rbp), %rdi
	movapd	-224(%rbp), %xmm0
	movq	%rdi, -40(%rbp)
	movapd	%xmm0, -64(%rbp)

	movq	-40(%rbp), %rdi
	movlpd	%xmm0, (%rdi)

	movq	-200(%rbp), %rdi
	movapd	-224(%rbp), %xmm0
	movq	%rdi, -72(%rbp)
	movapd	%xmm0, -96(%rbp)

	movq	-72(%rbp), %rdi
	movlpd	%xmm0, (%rdi)

	movq	-200(%rbp), %rdi
	movapd	-224(%rbp), %xmm0
	movq	%rdi, -104(%rbp)
	movapd	%xmm0, -128(%rbp)

	movq	-104(%rbp), %rdi
	movlpd	%xmm0, (%rdi)

	movq	-200(%rbp), %rdi
	movapd	-224(%rbp), %xmm0
	movq	%rdi, -136(%rbp)
	movapd	%xmm0, -160(%rbp)

	movq	-136(%rbp), %rdi
	movsd	%xmm0, (%rdi)

	addq	$96, %rsp
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
