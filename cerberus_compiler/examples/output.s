	.text
	.file	"CoreModule"
	.globl	aga                             # -- Begin function aga
	.p2align	4, 0x90
	.type	aga,@function
aga:                                    # @aga
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$32, %rsp
	movl	$0, -12(%rbp)
	movb	$1, -1(%rbp)
	movl	$0, -20(%rbp)
	movl	$65535, -16(%rbp)               # imm = 0xFFFF
	movl	$0, -8(%rbp)
	leaq	-12(%rbp), %rsi
	movl	$.L__unnamed_1, %edi
	xorl	%eax, %eax
	callq	scanf@PLT
	jmp	.LBB0_1
	.p2align	4, 0x90
.LBB0_4:                                # %if.end
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	-8(%rbp)
.LBB0_1:                                # %while.cond
                                        # =>This Inner Loop Header: Depth=1
	cmpb	$1, -1(%rbp)
	jne	.LBB0_5
# %bb.2:                                # %while.body
                                        #   in Loop: Header=BB0_1 Depth=1
	movq	%rsp, %rax
	leaq	-16(%rax), %rsp
	movl	-20(%rbp), %ecx
	subl	-16(%rbp), %ecx
	sarl	%ecx
	movl	%ecx, -16(%rax)
	cmpl	-12(%rbp), %ecx
	jne	.LBB0_4
# %bb.3:                                # %if
                                        #   in Loop: Header=BB0_1 Depth=1
	movb	$0, -1(%rbp)
	jmp	.LBB0_4
.LBB0_5:                                # %while.end
	movl	-8(%rbp), %esi
	movl	$.L__unnamed_2, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	movq	%rbp, %rsp
	popq	%rbp
	retq
.Lfunc_end0:
	.size	aga, .Lfunc_end0-aga
                                        # -- End function
	.globl	advent1                         # -- Begin function advent1
	.p2align	4, 0x90
	.type	advent1,@function
advent1:                                # @advent1
# %bb.0:                                # %entry
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%r15
	pushq	%r14
	pushq	%rbx
	subq	$24, %rsp
	movq	%rdi, -40(%rbp)
	movl	$0, -28(%rbp)
	movl	$0, -44(%rbp)
	jmp	.LBB1_1
	.p2align	4, 0x90
.LBB1_10:                               # %while.end3
                                        #   in Loop: Header=BB1_1 Depth=1
	movl	(%r15), %eax
	leal	(%rax,%rax,4), %eax
	addl	%eax, %eax
	addl	(%r14), %eax
	movl	%eax, -44(%rbp)
.LBB1_1:                                # %while.cond
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB1_3 Depth 2
	movl	-28(%rbp), %ebx
	movq	-40(%rbp), %rdi
	callq	strlen@PLT
	cmpl	%eax, %ebx
	jge	.LBB1_9
# %bb.2:                                # %while.body
                                        #   in Loop: Header=BB1_1 Depth=1
	movq	%rsp, %rax
	leaq	-16(%rax), %r15
	movq	%r15, %rsp
	movl	$-1, -16(%rax)
	movq	%rsp, %rax
	leaq	-16(%rax), %r14
	movq	%r14, %rsp
	movl	$-1, -16(%rax)
	.p2align	4, 0x90
.LBB1_3:                                # %while.cond1
                                        #   Parent Loop BB1_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movq	-40(%rbp), %rax
	movslq	-28(%rbp), %rcx
	cmpb	$92, (%rax,%rcx)
	je	.LBB1_10
# %bb.4:                                # %while.body2
                                        #   in Loop: Header=BB1_3 Depth=2
	movl	(%r15), %ebx
	movq	-40(%rbp), %rax
	movslq	-28(%rbp), %rcx
	movzbl	(%rax,%rcx), %edi
	callq	isdigit@PLT
	testb	$1, %al
	je	.LBB1_7
# %bb.5:                                # %while.body2
                                        #   in Loop: Header=BB1_3 Depth=2
	cmpl	$-1, %ebx
	jne	.LBB1_7
# %bb.6:                                # %if
                                        #   in Loop: Header=BB1_3 Depth=2
	movq	-40(%rbp), %rax
	movslq	-28(%rbp), %rcx
	movzbl	(%rax,%rcx), %eax
	addb	$-48, %al
	movb	%al, (%r15)
.LBB1_7:                                # %if.end
                                        #   in Loop: Header=BB1_3 Depth=2
	movq	-40(%rbp), %rax
	movslq	-28(%rbp), %rcx
	movzbl	(%rax,%rcx), %edi
	callq	isdigit@PLT
	testb	$1, %al
	je	.LBB1_3
# %bb.8:                                # %if5
                                        #   in Loop: Header=BB1_3 Depth=2
	movq	-40(%rbp), %rax
	movslq	-28(%rbp), %rcx
	movzbl	(%rax,%rcx), %eax
	addb	$-48, %al
	movb	%al, (%r14)
	jmp	.LBB1_3
.LBB1_9:                                # %while.end
	movl	-44(%rbp), %eax
	leaq	-24(%rbp), %rsp
	popq	%rbx
	popq	%r14
	popq	%r15
	popq	%rbp
	retq
.Lfunc_end1:
	.size	advent1, .Lfunc_end1-advent1
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	pushq	%rbx
	subq	$16, %rsp
	movl	$50, 12(%rsp)
	movl	$0, 8(%rsp)
	movb	$0, 7(%rsp)
	leaq	8(%rsp), %rbx
	jmp	.LBB2_1
	.p2align	4, 0x90
.LBB2_3:                                # %if
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	$.L__unnamed_3, %edi
.LBB2_4:                                # %while.cond
                                        #   in Loop: Header=BB2_1 Depth=1
	xorl	%eax, %eax
	callq	printf@PLT
.LBB2_1:                                # %while.cond
                                        # =>This Inner Loop Header: Depth=1
	cmpb	$0, 7(%rsp)
	jne	.LBB2_8
# %bb.2:                                # %while.body
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	$.L__unnamed_4, %edi
	movq	%rbx, %rsi
	xorl	%eax, %eax
	callq	scanf@PLT
	movl	8(%rsp), %eax
	cmpl	12(%rsp), %eax
	jg	.LBB2_3
# %bb.5:                                # %if.else
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	8(%rsp), %eax
	cmpl	12(%rsp), %eax
	jge	.LBB2_7
# %bb.6:                                # %if1
                                        #   in Loop: Header=BB2_1 Depth=1
	movl	$.L__unnamed_5, %edi
	jmp	.LBB2_4
	.p2align	4, 0x90
.LBB2_7:                                # %if.else2
                                        #   in Loop: Header=BB2_1 Depth=1
	movb	$1, 7(%rsp)
	movl	$.L__unnamed_6, %edi
	jmp	.LBB2_4
.LBB2_8:                                # %while.end
	xorl	%eax, %eax
	addq	$16, %rsp
	popq	%rbx
	retq
.Lfunc_end2:
	.size	main, .Lfunc_end2-main
                                        # -- End function
	.type	.L__unnamed_1,@object           # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d"
	.size	.L__unnamed_1, 3

	.type	.L__unnamed_2,@object           # @1
.L__unnamed_2:
	.asciz	"Znalezione w %d krokach"
	.size	.L__unnamed_2, 24

	.type	.L__unnamed_4,@object           # @2
.L__unnamed_4:
	.asciz	"%d"
	.size	.L__unnamed_4, 3

	.type	.L__unnamed_3,@object           # @3
.L__unnamed_3:
	.asciz	"Mniej"
	.size	.L__unnamed_3, 6

	.type	.L__unnamed_5,@object           # @4
.L__unnamed_5:
	.asciz	"Wiecej"
	.size	.L__unnamed_5, 7

	.type	.L__unnamed_6,@object           # @5
.L__unnamed_6:
	.asciz	"Gratulacje"
	.size	.L__unnamed_6, 11

	.section	".note.GNU-stack","",@progbits
