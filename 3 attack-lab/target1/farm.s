
00000000004019a0 <addval_273>:
  4019a0:	8d 87
  ;4019a2 movq %rax %rdi
  48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq   

00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 
  ;4019ab pop %rax
  58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq   

00000000004019c3 <setval_426>:
  4019c3:	c7 07 
  ;4019c5 mov %rax %rdi
  48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq   

00000000004019ca <getval_280>:
  4019ca:	b8 29 
  ; pop %rax
  58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	retq   

00000000004019db <getval_481>:
  4019db:	b8 
  ;4019dc: popq %rsp
  5c 
  ;4019dd: mol %eax %edx
  89 c2 90       	mov    $0x90c2895c,%eax
  4019e0:	c3                   	retq   

0000000000401a03 <addval_190>:
  401a03:	8d 87 41 
  ;401a06 movq %rsp %rax
  48 89 e0    	lea    -0x1f76b7bf(%rdi),%eax
  401a09:	c3                   	retq   

0000000000401a11 <addval_436>:
  401a11:	8d 87 
  ;401a13: movl %ecx %esi
  89 ce 90 90    	lea    -0x6f6f3177(%rdi),%eax
  401a17:	c3                   	retq   

0000000000401a25 <addval_187>:
  401a25:	8d 87 
  ;movl %ecx %esi
  89 ce 38 c0    	lea    -0x3fc73177(%rdi),%eax
  401a2b:	c3                   	retq   

0000000000401a33 <getval_159>:
  401a33:	b8 
  ;401a34: movl %edx %ecx
  89 d1 38 c9       	mov    $0xc938d189,%eax
  401a38:	c3                   	retq   

0000000000401a39 <addval_110>:
  401a39:	8d 87 c8 
  ;401a3c: movl %esp %eax
  89 e0 c3    	lea    -0x3c1f7638(%rdi),%eax
  401a3f:	c3                   	retq   

0000000000401a40 <addval_487>:
  401a40:	8d 87 5
  ;401a42: movl %eax %edx
  89 c2 84 c0    	lea    -0x3f7b3d77(%rdi),%eax
  401a46:	c3                   	retq   

0000000000401a68 <getval_311>:
  401a68:	b8 
  ;movl %edx %ecx
  89 d1 08 db       	mov    $0xdb08d189,%eax
  401a6d:	c3                   	retq    

0000000000401a83 <addval_358>:
  401a83:	8d 87 08 
  ;movl %esp %eax
  89 e0 90    	lea    -0x6f1f76f8(%rdi),%eax
  401a89:	c3                   	retq      

0000000000401aab <setval_350>:
  401aab:	c7 07 48 89 e0 90    	movl   $0x90e08948,(%rdi)
  401ab1:	c3                   	retq   