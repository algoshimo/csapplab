## datalab

[第二章 信息的表示和处理 - 拾墨、 - 博客园 (cnblogs.com)](https://www.cnblogs.com/algoshimo/p/17785820.html)

### 整数类型

#### 1. bitand



本关任务：补充函数`bitAnd()`，只用`~`、`|`实现`x&y`，将结果`return`返回。



思路:

> ~~(x&y) == (x&y)，然后用摩根定律就行



```c++
int bitAnd(int x, int y) 
{ 
	return ~(~x | ~y);
}
```



#### 2.  getbyte

本关任务：补充函数`getByte()`，取出`x`中的`n`号字节，将结果`return`返回。

测试输入： `0x12345678` `1`

预期输出： `0x56`



思路：

> 将第n号字节移到末尾，然后&上一个0xff就行



```c++
int getByte(int x, int n)
{ 
	x>>=8*n;
    return x&0xff;
}
```



#### 3. logicalshift

本关任务：补充函数`logicalShift()`，将`x`逻辑右移`n`位(0<=*n*<=31) ，将结果`return`返回。



测试输入： `0x87654321` `4`

预期输出： `0x8765432`



思路：

> 逻辑右移需要补0，所以将一个各位全1的数移动n位再进行&操作就行



```c++
int logicalShift(int x, int n) 
{
	x>>=n;
    n = 0xFFFFFFFF>>n;
    return x&n;
}
```



#### 4. bitcount

本关任务：补充函数`bitCount()`，统计`x`的二进制表示中`1`的数量，将结果`return`返回。



测试输入：`5` 预期输出：`2`



思路：

> 若x为负，则+一个1<<31将x的最高位进位再统计



```c++
int bitCount(int x) 
{
    int cnt =  0;
    if(x<0) {x += (1<<31);cnt++;}
    while(x)
    {
        x-=x&(-x);
        cnt++;
    }
    return cnt;
}
```



#### 5. bang

本关任务：补充函数`bang()`，不使用`!`实现`!`操作符，将结果`return`返回。



测试输入：`4` 预期输出：0

思路:

> 首先要清楚只要x不是0就会返回1
>
> 
>
> 我们可以根据倍增的思想，先将前32位中后一半中的1挪到前一半，挪好后再将前16位中后一半的1挪到前一半......将前2位中后一半的1挪到前一半



```c++
int bang(int x) 
{
    x |= (x>>16);
    x |= (x>>8);
    x |= (x>>4);
    x |= (x>>2);
    x |= (x>>1);
    return 1-(x&1);
}
```



#### 6.  tmin

本关任务：补充函数`tmin()`，返回补码表示的整型的最小值，将结果`return`返回。

```c++
int tmin(void) 
{ 
	int x = 1<<31;
    return x;
}
```



#### 7. fitsbits

 本关任务：补充函数`fitsBits()`，如果`x`可以只用`n`位补码表示则返回`1`，否则返回`0`(1<=*n*<=32)



思路：

> 补码主要是为了带上负数，正数补码还是它本身
>
> 将x左移32-位，然后再移回来，若结果不变，则说明可以只用n位甚至小于n位来表示



```c++
int fitsBits(int x, int n) 
{
	int c = 32-n;
    int res = (x<<c)>>c;
    return !(res^x);
}
```



#### 8.  divpwr2

$$
本关任务：补充函数divpwr2()，计算\frac{x}{2^n}    ，(0<=n<=30) 结果向零取整。
$$



思路：

> 若不做处理就无脑右移n位得到的结果是向下取整而不是向0取整
>
> 
>
> 对于负数，将末位的1移走，会导致结果向下取整。
>
> 那么对于负数，我们每次右移前加上一个偏置:(1<<n)-1，这样原本为0的本来就要移走不会有影响，原本为1的会进位到更高位被保留下来
>
> 

```c++
int divpwr2(int x, int n) 
{
    if(x<0) x += (1<<n)-1;
    return x>>n;
}
```



#### 9. negate

本关任务：补充函数`negate()`，计算`-x`

```c++
int negate(int x) 
{ 
	return ~x+1;
}
```



#### 10. ispositive

本关任务：补充函数`isPositive()`，如果`x`大于`0`返回`1`，否则返回`0`



测试输入：`4` 预期输出：`1`



思路：

> 看最高位状态就好了



```c++
int isPositive(int x) 
{
    int is_zero = 0;
    if(x == 0) is_zero = 1;
	return !is_zero & ~((x>>31)&1);
}
```



#### 11. islessorequal

本关任务：补充函数`isLessOrEqual()`，如果`x`小于等于`y`则返回`1`，否则返回`0`。



测试输入： `5` `8`

预期输出： `1`

```
int isLessOrEqual(int x, int y) 
{
	int signx = (x>>31)&1;
    int signy = (y>>31)&1;

    int cond1 = !(x^y); //x == y(1)
    int cond2 = !((!signx)&signy);        //x- y+(1)
    int cond3 = signx&(!signy);                   //x+ y-(0)
    int cond4 = ((x+~y+1)>>31);       //x,y同号
    return cond1 || (cond2 && (cond3 || cond4));
}
```



#### 12. ilog2

本关任务：补充函数`ilog2()`，返回`x`求以`2`为底的对数的结果（向下取整）



测试输入：`64` 预期输出：`6`

```c++
int ilog2(int x) 
{
    int cnt = 0;
	while(x>>=1) cnt++;
    return cnt;
}
```



### 浮点数类型

![image-20240228151943311](C:\csapp\image-20240228151943311.png)



#### 1. float_neg

本关任务：补充函数`float_neg()`，返回`-uf`的位级表示。 



测试输入：`-111` 预期输出：`0xffffff91`

测试输入：`-12345` 预期输出：`0xffffcfc7`

```c++
unsigned float_neg(unsigned uf)
{  
    if((uf&0x7fffffff) > 0x7f800000) return uf; 
    //若uf为NAN，则返回NAN
    return uf^0x80000000; //否则将符号位反转
}
```



#### 2. float_f2i

本关任务：补充函数`float_f2i()`，实现由 `float` 到 `int`的类型转换。

- 操作符使用数量限制：`30`



```c++
unsigned float_f2i(int x) 
{
	unsigned s = x>>31&1; //符号位
    unsigned exp = x>>23&0xff; //阶码部分
    unsigned frac = x&0x7fffff; //尾数部分
    
    if(exp == 0) return 0; //非规格化表示0.xx，转到int就是0

    if(exp == 0xff) return 1<<31 //根据规定，如果x为NAN或者无穷，则返回1<<31

    int e = exp-127 //exp是阶码，其真值要减去一个偏置
    frac = frac | (1<<23); //将尾数原本隐式的0添上

    if(e>31) return 1<<31; //溢出
    if(e<0) return 0;   //1.xxx除以一个大于1的数转成int类型一定是0
    if(e>23) frac<<=(e-23) //若阶码真值大于23，也就是大于frac的位数说明float不会有小数点后面的舍去
    else frac>>=(23-e);

    if(s) frac = ~frac+1;
    return frac
}
```



#### 3. float_twice

本关任务：补充函数`float_twice()`，返回`2*f`的位级表示。



测试输入：`111` 预期输出：`0xde`



```c++
unsigned float_twice(unsigned uf) 
{
    unsigned frac = (uf & 0x7fffff);
    unsigned exp = (uf>>23) & (0xff);
    unsigned s = (uf>>31) & (0x1);

    if(exp == 0 && frac == 0) return uf; //特判0
    if(exp == 0xff ) return uf;     //特判NAN和无穷
    if(exp == 0)  //若为非规格化，因为非规格化exp必须是0，所以只能在尾数上乘2
    {
        frac<<=1;
        return (s<<31) | frac;
    }
	exp++; //若不是非规格数，则直接将阶码+1就好了
    return (s<<31) | (exp<<23) | (frac);
}
```

