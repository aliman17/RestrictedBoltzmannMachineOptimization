	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 11
	.globl	_W_update
	.align	4, 0x90
_W_update:                              ## @W_update
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
	movq	%rdi, -8(%rbp)
    
	movq	-8(%rbp), %rdi
	movsd	(%rdi), %xmm0           ## xmm0 = mem[0],zero
	movq	-8(%rbp), %rdi
	movsd	%xmm0, (%rdi)
	popq	%rbp
	retq
	.cfi_endproc


.subsections_via_symbols
