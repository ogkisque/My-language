	.text
	.file	"llvm.ll"
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function func0
.LCPI0_0:
	.quad	0x3ff0000000000000              # double 1
	.text
	.globl	func0
	.p2align	4, 0x90
	.type	func0,@function
func0:                                  # @func0
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movsd	%xmm0, -8(%rbp)
	movabsq	$4607182418800017408, %rax      # imm = 0x3FF0000000000000
	movq	%rax, -24(%rbp)
	movq	%rax, -16(%rbp)
	movsd	.LCPI0_0(%rip), %xmm0           # xmm0 = mem[0],zero
	.p2align	4, 0x90
.LBB0_1:                                # %while0
                                        # =>This Inner Loop Header: Depth=1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movsd	-8(%rbp), %xmm1                 # xmm1 = mem[0],zero
	movsd	%xmm1, -16(%rcx)
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rsp
	movq	%rax, -16(%rdx)
	movsd	-16(%rcx), %xmm1                # xmm1 = mem[0],zero
	ucomisd	%xmm0, %xmm1
	jbe	.LBB0_3
# %bb.2:                                # %body_while0
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movsd	-16(%rbp), %xmm1                # xmm1 = mem[0],zero
	movsd	%xmm1, -16(%rcx)
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rsp
	movsd	-8(%rbp), %xmm1                 # xmm1 = mem[0],zero
	movsd	%xmm1, -16(%rdx)
	mulsd	-16(%rcx), %xmm1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movsd	%xmm1, -16(%rcx)
	movsd	%xmm1, -16(%rbp)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movsd	-8(%rbp), %xmm1                 # xmm1 = mem[0],zero
	movsd	%xmm1, -16(%rcx)
	movq	%rsp, %rdx
	leaq	-16(%rdx), %rsp
	movq	%rax, -16(%rdx)
	movsd	-16(%rcx), %xmm1                # xmm1 = mem[0],zero
	subsd	%xmm0, %xmm1
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movsd	%xmm1, -16(%rcx)
	movsd	%xmm1, -8(%rbp)
	jmp	.LBB0_1
.LBB0_3:                                # %end_while0
	movsd	-16(%rbp), %xmm0                # xmm0 = mem[0],zero
	movq	%rbp, %rsp
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end0:
	.size	func0, .Lfunc_end0-func0
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function main
.LCPI1_0:
	.quad	0x3ff0000000000000              # double 1
.LCPI1_1:
	.quad	0x0000000000000000              # double 0
	.text
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	pushq	%rbx
	subq	$56, %rsp
	.cfi_offset %rbx, -24
	movabsq	$4620693217682128896, %rax      # imm = 0x4020000000000000
	movq	%rax, -56(%rbp)
	movq	%rax, -32(%rbp)
	movabsq	$4666723172467343360, %rax      # imm = 0x40C3880000000000
	movq	%rax, -48(%rbp)
	movq	%rax, -16(%rbp)
	movabsq	$4607182418800017408, %rbx      # imm = 0x3FF0000000000000
	movq	%rbx, -40(%rbp)
	movq	%rbx, -24(%rbp)
	.p2align	4, 0x90
.LBB1_1:                                # %while1
                                        # =>This Inner Loop Header: Depth=1
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	-16(%rbp), %xmm0                # xmm0 = mem[0],zero
	movsd	%xmm0, -16(%rax)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	$0, -16(%rcx)
	movsd	-16(%rax), %xmm0                # xmm0 = mem[0],zero
	ucomisd	.LCPI1_1(%rip), %xmm0
	jbe	.LBB1_3
# %bb.2:                                # %body_while1
                                        #   in Loop: Header=BB1_1 Depth=1
	movsd	-32(%rbp), %xmm0                # xmm0 = mem[0],zero
	callq	func0
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	%xmm0, -16(%rax)
	movsd	%xmm0, -24(%rbp)
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	-16(%rbp), %xmm0                # xmm0 = mem[0],zero
	movsd	%xmm0, -16(%rax)
	movq	%rsp, %rcx
	leaq	-16(%rcx), %rsp
	movq	%rbx, -16(%rcx)
	movsd	-16(%rax), %xmm0                # xmm0 = mem[0],zero
	subsd	.LCPI1_0(%rip), %xmm0
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movsd	%xmm0, -16(%rax)
	movsd	%xmm0, -16(%rbp)
	jmp	.LBB1_1
.LBB1_3:                                # %end_while1
	movsd	-24(%rbp), %xmm0                # xmm0 = mem[0],zero
	movl	$.L.str.1, %edi
	movb	$1, %al
	callq	printf@PLT
	xorl	%eax, %eax
	leaq	-8(%rbp), %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa %rsp, 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%lf"
	.size	.L.str, 4

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"%lf\n"
	.size	.L.str.1, 5

	.section	".note.GNU-stack","",@progbits
