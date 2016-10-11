	movl	(%rsi), %edx
	movl	(%rdi), %eax
	cmpl	%eax, %edx
	jge	.L2
	movl	%edx, (%rdi)
	movl	%eax, (%rsi)
.L2:
	movl	4(%rsi), %edx
	movl	4(%rdi), %eax
	cmpl	%eax, %edx
	jge	.L3
	movl	%edx, 4(%rdi)
	movl	%eax, 4(%rsi)
.L3:
	movl	8(%rsi), %edx
	movl	8(%rdi), %eax
	cmpl	%eax, %edx
	jge	.L4
	movl	%edx, 8(%rdi)
	movl	%eax, 8(%rsi)
.L4:
	movl	12(%rsi), %edx
	movl	12(%rdi), %eax
	cmpl	%eax, %edx
	jge	.L1
	movl	%edx, 12(%rdi)
	movl	%eax, 12(%rsi)
.L1:
	ret