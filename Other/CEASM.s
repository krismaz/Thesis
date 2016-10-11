void CompareExchange(int& A, int& B){
	if(B<A){
		std::swap(A,B);
	}
}

movl	(%rsi), %edx
	movl	(%rdi), %eax
	cmpl	%eax, %edx
	jge	.L1
	movl	%edx, (%rdi)
	movl	%eax, (%rsi)
.L1:
	ret

void CompareExchange(int& A, int& B){
	auto C = A;
	A = std::min(A, B);
	B = std::max(C, B);
}


movl	(%rdi), %eax
cmpl	%eax, (%rsi)
movl	%eax, %edx
cmovle	(%rsi), %edx
movl	%edx, (%rdi)
movl	(%rsi), %edx
cmpl	%edx, %eax
cmovl	%edx, %eax
movl	%eax, (%rsi)
ret

void CompareExchange(int& A, int& B){
	auto mask = ((A ^ B) & -(A < B));
	auto C = A;
	A = B ^ mask;
	B = C ^ mask;
}


movl	(%rdi), %eax
xorl	%ecx, %ecx
movl	(%rsi), %edx
movl	%eax, %r8d
cmpl	%edx, %eax
setl	%cl
xorl	%edx, %r8d
negl	%ecx
andl	%r8d, %ecx
xorl	%ecx, %edx
xorl	%ecx, %eax
movl	%edx, (%rdi)
movl	%eax, (%rsi)
ret