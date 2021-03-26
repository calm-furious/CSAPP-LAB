# Bomblab

## main

程序首先进入main函数，在其中处理了一些IO之后（从文件读取和标准输入读取），将输入读入%rdi，并调用了phase_1函数。

## Phase 1

phase_1函数主要拿输入和0x402400处的字符串比较（应该是程序中的静态字串）。通过命令：`x/52xb 0x402400`得到字串比特值（52是由gdb中找对应寄存器获得的长度，用53可发现第53个byte是0号字符），根据 `man ascii`对照得到一开始的单词是border relations with canada，于是再去找`strings bomb`里可打印字串的相应结果，得到答案。

> Border relations with Canada have never been better.

此处用 x/s 0x402400 命令可以直接读出字符串，更方便。

```yaml
0000000000400ee0 <phase_1>:
  400ee0:	48 83 ec 08          	sub    $0x8,%rsp
  400ee4:	be 00 24 40 00       	mov    $0x402400,%esi
  400ee9:	e8 4a 04 00 00       	callq  401338 <strings_not_equal>
  400eee:	85 c0                	test   %eax,%eax
  400ef0:	74 05                	je     400ef7 <phase_1+0x17>
  400ef2:	e8 43 05 00 00       	callq  40143a <explode_bomb>
  400ef7:	48 83 c4 08          	add    $0x8,%rsp
  400efb:	c3                   	retq   
```

## Phase 2

调用了一个叫read_six_numbers的函数，仔细观察了一下就是往stack里读了6个int数  scanf会反回有效个数，在40148f处判断是否是6个。其余的没仔细看，应该是直接往rsp的地址里读6个数吧。

```yaml
000000000040145c <read_six_numbers>:
  40145c:	48 83 ec 18          	sub    $0x18,%rsp
  401460:	48 89 f2             	mov    %rsi,%rdx  #0x7fff8bc017d0
  401463:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
  401467:	48 8d 46 14          	lea    0x14(%rsi),%rax
  40146b:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
  401470:	48 8d 46 10          	lea    0x10(%rsi),%rax
  401474:	48 89 04 24          	mov    %rax,(%rsp)
  401478:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
  40147c:	4c 8d 46 08          	lea    0x8(%rsi),%r8
  401480:	be c3 25 40 00       	mov    $0x4025c3,%esi
  401485:	b8 00 00 00 00       	mov    $0x0,%eax
  40148a:	e8 61 f7 ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
  40148f:	83 f8 05             	cmp    $0x5,%eax
  401492:	7f 05                	jg     401499 <read_six_numbers+0x3d>
  401494:	e8 a1 ff ff ff       	callq  40143a <explode_bomb>
  401499:	48 83 c4 18          	add    $0x18,%rsp
  40149d:	c3                   	retq  
```

然后在 phase_2 函数中就是先比较 1 和输入的第一个数，在比较后一个输入是否是前一个数的2倍 `add %eax,%eax`

> 1 2 4 8 16 32

## Phase 3

程序通过一个类似跳表的东西跳回phase_3的下半部分代码

答案有多个

> 7 327

> 6 682

> 5 206

> 4 389

> 3 256

> 2 707

> 1 207 (此条没确认) 

> 0 207

## Phase 4

> 7 0

func4里面干了啥神奇的事儿？（懒得看）

我只知道等于7的时候可以跳过两大波代码，非常容易就过了。

## Phase 5

```asm
40107f:	83 f8 06             	cmp    $0x6,%eax
401082:	74 4e                	je     4010d2 <phase_5+0x70>
401084:	e8 b1 03 00 00       	callq  40143a <explode_bomb>
```

要求输入6个字符

然后比较内存中 0x40245e 位置的值 与 修改后的输入。反推得结果，结果可以有多种，只要最后4个bit的ascii值一样就行。

> 9?>567

## Phase 6

0x6032d0 处躺着一个链表，要按照链表内第一个16位字来排序。排序的方式是用第二个16位字的编号。然后还要吧数字全部用7减一下。上图：

![image-20210325205636242](../../../../Data/noteimg/image-20210325205636242.png)

这是我排完序后的，可以看见冒号后第三列已经不是按顺序链接的了（原来是按顺序链接的）

第一列是待排序的数，把第二列当作编号的话应该按照 “3 -> 4 -> 5 -> 6 -> 1 -> 2”的顺序连起来。

所以答案是：4 3 2 1 6 5

## 隐藏阶段！

### 发现隐藏阶段的线索有：

1. phase_6下的func_7
2. 运行strings bomb后找到phase_1解答的地方上面一行就提示了
3. 课上的readme文件中似乎有

![image-20210326141249107](../../../../Data/noteimg/image-20210326141249107.png)

于是找到了 secret_phase, 是在phase_defused下的, 想要进入还不太容易：

### 进入方式

```asm
4015f0:	be 19 26 40 00       	mov    $0x402619,%esi
4015f5:	bf 70 38 60 00       	mov    $0x603870,%edi
4015fa:	e8 f1 f5 ff ff       	callq  400bf0 <__isoc99_sscanf@plt>
```

调用了sscanf，参数地址都告诉我们了。发现输入格式是”%d %d %s“，而输入内容是“7 0”。（用 x/s 命令）

一看到7 0 就想起 phase_4 中还有好多代码没看懂，一瞧果然 phase_4 答案是 7 0.

又发现phase_4 scanf 格式是 “%d %d” 也就是说多一个字符串是不会炸的！

> 7 0 DrEvil

### secret_phase

#### 1

```asm
40125a:	48 89 c3             	mov    %rax,%rbx
40125d:	8d 40 ff             	lea    -0x1(%rax),%eax            
401260:	3d e8 03 00 00       	cmp    $0x3e8,%eax               
401265:	76 05                	jbe    40126c <secret_phase+0x2a>
401267:	e8 ce 01 00 00       	callq  40143a <explode_bomb>
```

输入一个1～1000的数才不会触发0x401267

#### 2

```asm
401273:	e8 8c ff ff ff       	callq  401204 <fun7>
401278:	83 f8 02             	cmp    $0x2,%eax
40127b:	74 05                	je     401282 <secret_phase+0x40>
40127d:	e8 b8 01 00 00       	callq  40143a <explode_bomb>
```

要让 fun7 返回 2

### func7

明显是个递归函数

```
0000000000401204 <fun7>:
	...
	401217:	e8 e8 ff ff ff       	callq  401204 <fun7>
	...
	40122d:	e8 d2 ff ff ff       	callq  401204 <fun7>
```

我们希望返回值是 2 ，就关注所有和 rax 相关的操作，找出一条递归路径使得eax最后等于2.

那么一定是先将eax清零0，执行到这步需要 esi == edx

```asm
401220:	b8 00 00 00 00       	mov    $0x0,%eax
```

然后eax = rax + rax + 1， 需要 esi > (rdi)

```asm
401232:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
```

最后 eax += eax，需要 esi < (rdi)

```asm
40121c:	01 c0                	add    %eax,%eax
```

把这三次递归调用的逻辑理出来后，发现只需要让传入的 rsi 满足三次的条件，（运行时三次条件是倒过来的）即：

1. esi < 0x24
2. esi > 0x8
3. esi = 0x16

于是输入的十进制值答案就是：22

## psol.txt

> Border relations with Canada have never been better.
> 1 2 4 8 16 32
> 7 327
> 7 0 DrEvil
> 9?>567
> 4 3 2 1 6 5
> 22

## 问题汇总

每次反编译地址不会变，是因为操作系统给程序提供的抽象吗？

载入的 sscanf 这些函数的机制是什么（后续应该会学

