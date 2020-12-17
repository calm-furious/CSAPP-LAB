# CSAPP Datalab 解题记录

##Int

### 用~ & 表示 ^

```c
int bitXor(int x, int y) {
  return ~(~(x&~y) & ~(~x&y)); //8
}
```

x&~y 是指当 x为1的时候，y为0的情况，把这种情况标记为1

于是可知 x ^ y  = (x & ~y) | (y & ~x)

用摩尔律 x | y = ~(~x & ~y) 即可得到结果

### tmin

即表示最小的two‘s complement的signed int, 因为前提说了可以假设都是32位，所以就是第32位上是1，其他全是0

```c
int tmin(void) {
  return 1 << 31;
}
```

### isTmax 

如果一个数是signed int two‘s complement的最大值，那么应该是011...1的形式。默认32位。

我想出的办法是，加一，然后判断是否是10...0的形式，也就是判断isTmin。此处可以利用 TMin取负数仍是TMin的性质：(~x+1)^x == 0。有特例是x=-1时，加一是0，0的负数还是0，也满足上述条件。此时可以单独将其排除。即 ~x != 0 => !!~x

```c
int isTmax(int x) {
  int a = ~x;
  return !!a & ! (a ^ (x+1)); //7
}
```

### allOddBits

构造a = 01010101的全偶数位是一的情况。x | a全是1的话即满足条件。构造的时候用了倍增思想。

```c
int allOddBits(int x) {
  int a = (0x55 << 8) | 0x55;
  // printf("%d",a);
  return !((((a << 16) | a) | x) +1); //7
}
```

### negate 取负数，也是整数减法操作

按two's complement定义。。。取反加1，英文叫 complement and increment

### 是否为asciiDigit

笨办法：

1. 从第4位开始（0开始算）以上的位都必须和0x30一模一样

2. 排除0x3a到0x3f，即对于最开始的字节：当第3位为1的时候（x&

   8），中间两位不能为1（x&6）。

```c
int isAsciiDigit(int x) { 
  return ! ((x^0x30)>>4) & !( !!(x&0x8) & !!(x&0x6) ); //12
}
```

（还有更好的方法吗？

### ？条件的实现

巧妙的一题（自认为）

1. 首先就是要构造出一种神奇的东西，它在x=0的时候就是0，和y与能把y消除掉。它在x!=0的时候是全1，和y与就是y。

2. 同时构造另一个神奇的东西对于z也要有类似但相反的性质。x=0时 是全1，否则是全0。

先构造后者（比较简单）就是!!x - 1 如果x有东西，就是0，没东西就是-1=0xFFFFFFFF

再构造前者，非常妙。还是!!x为基础，但是在x有东西的时候，我们希望!!x被减去2，从而获得-1。有东西的时候!!x 是1，没东西是0，利用这一点构造 ”有东西时候的2“ 即 (!!x)<<1

```c
int conditional(int x, int y, int z) {
  int notx = !x;
  return ( (~!notx + 1) & y) | (((notx << 31) >> 31 ) & z); //9
}
```

（妙是妙，应该有更简单的）

### isLessOrEqual 

**同符号加减是否不会溢出？？**极值法证明好像是对的。

自己想到了分类讨论，x=y 和 符号位不同的情况。符号位相同的情况没想出来。其实是漏掉了一个已知性质：整数减法就是加上negate的数！！！ 所以不能用- 也可以用 x+ (~y+1) 三个来代替。。。

```c
int isLessOrEqual(int x, int y) {//the negation is a-b==a+(~b+1)
  int c1 = !(x ^ y);
  int c2 = !! ((x & ~y) & (1 << 31));
  int c = ! ((~x & y) & (1 << 31));
  int c3 = ! ((y + (~x + 1)) >> 31);
  return c & (c1 | c2 | c3); //21
}
```



(应该有更简单的)

### logicalNeg 

此题没想出来，但是知道要利用全0和全1的一些性质

借鉴网上：全0时，取反加一还是0，最高位也是0；有东西时，取反加一，最高位一定要么本来是1，要么后来是1 

```c
int logicalNeg(int x) {
  return ((x|(~x+1))>>31)+1;
}
```

### 多少位来表示
做出来啦！！！激动 用了二分查找的思想。
首先 x 异或 x>>31 , 使得正负都变成正来处理。
暴力解法就是：最左边的1出现在什么位置，出现在 i 处，则需要 i+1 位来表示（符号位留出来）。
二分法就是一半一半地搜索，注意保存过程中的结果，在之后的查找中就可以知道是哪个区间。
找1的方法是：判断当前数右移了bias之后是否全0. 是的话在右侧找，否则在左侧找。一共是32位，一定找5次停下。然后结合btest测试debug。处理最终结果。

下面代码不合规范，需要把int定义写在函数最前面，（dissertation上说的。）
```c
int howManyBits(int x) {
  int a = x >> 31;
  int newx = (x ^ a); //2
  int temp1 = !!(newx >> 16);//5
  int ntemp1 = ~temp1 + 1; //7
  int n = 16 & ntemp1;     //8
  int temp2 = !!(newx >> (8 + n)); //12
  int ntemp2 = ~temp2 + 1; //14
  n = n + (ntemp2 & 8);
  int temp3 = !!(newx >> (4 + n));
  int ntemp3 = ~temp3 + 1;
  n = n + (ntemp3 & 4);
  int temp4 = !!(newx >> (2 + n));
  int ntemp4 = ~temp4 + 1;
  n = n + (ntemp4 & 2);
  int temp5 = !!(newx >> (1 + n));
  n = n + temp5 + 1 + !!newx; 
  return n;
}
```
## float
条件放宽了，什么运算都能用，if while 都能用
### floatScale2 浮点数乘以2
1. 判断 inf NaN 和 0 的特殊情况
2. 判断 denorm 的特殊情况
  * 如果是 frac 的首位是1，那么乘以2后将从 denorm 变成 norm ，此时 exp++
  * frac 左移 1 位
3. 其他情况，暴力 exp++，overflow应该要把 frac 清零，我没清也过了。。。

```c
unsigned floatScale2(unsigned uf) {
  unsigned int exp = uf & 0x7f800000;
  unsigned int sign = uf & 0x80000000;
  unsigned int frac = uf & 0x007fffff;

  if (exp == 0x7f800000 || !(uf & 0x7fffffff) )// inf or NaN
    return uf;

  if (exp == 0) { //denorm
    if (uf & 0x00200000) 
      exp += 0x00800000;
    frac = (uf & 0x003fffff) << 1;
  }
  else {
    exp += 0x00800000;
  }

  //reconstruct
  return (sign | exp | frac);
}
```

### floatFloat2Int 类型转换
* 判断 out of range：
  * 0x7fffffff 的表示方法 0b0 {157} {0b111...1} 比原数小， 
  * 0x80000000 的表示方法是 0b1 {158} {0b000...0}与原数相等，相等返回值也是 TMIN 。
  * 所以充要条件是：exp 大于 157
* 考虑大于1的情况：
  * 不为 denorm：frac 前面补 1 （0x00800000）
  * 把 frac 视为已经左移23位，如果 exp-bias 小于 23 ，则须右移
  * 结果注意按 sign bit 取负
* 其他情况直接返回0.
```c
int floatFloat2Int(unsigned uf) {
  unsigned int exp = (uf & 0x7f800000) >> 23;
  unsigned int sign = uf & 0x80000000;
  unsigned int frac = uf & 0x007fffff;
  int bias = 0;
  if (exp == 0xff)// inf/NaN 
    return 0x80000000u;
  
  if (exp > 157) {//out of range
    return 0x80000000u;
  }
  
  if (exp){
    frac += 0x00800000;
  }
  if (exp > 150) {
    frac <<= (exp - 150);
  }
  else if (exp > 126) {
    frac >>= (150 - exp);
  }
  else {
    return 0;
  }

  if (sign) {
    frac = ~frac+1;
  }
  return frac;
}
```

### floatPower2 以2为底的指数
首先大概分析一下，我们知道 float 在 x = 0 的左右两侧 2 的幂次数量是不对称的。
具体来说，右侧 x > 0 的个数由exp的位数决定，x < 0 的个数则由 exp 和 frac 共同决定。
* x > 0:
  * exp 从 10000000 到 11111110
  * frac = 0
* x < 0:
  * Norm：exp 从 01111111 到 00000001
  * Denorm：exp = 00000000；frac 从 100...0 到 000...1
超时了，单独测的时候加上-T 20 通过了
```c
unsigned floatPower2(int x) {
  unsigned int res = 0;

  if (x > 127) {
    return 0x7f800000u;
  }
  else if (x >= -126) {
    res = ((x + 127) << 23);
    return res;
  }
  else if (x >= -149) {
    res = (1 << (x + 149));
    return res;
  }
  else {
    return 0;
  }
  return 0;
}
```






