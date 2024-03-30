## cachelab

只实现了主体功能



包括初始化cache ，缓存数据 ， 找到最近最久未使用的数据 



```
# include<iostream>
# include<stdlib.h>

struct Cache_line  //一行cacheline。不考虑偏移量b
{
    int valid; //有效位
    int tag;   //标记位
    int time_tamp;  //时间戳,越大说明越久没使用
};

struct Cache
{
    int S;
    int E;
    int B;
    Cache_line **line;  
    //之所以是一个二级指针是因为line[]是一个组。line[][]是一行cacheline
};

Cache *cache = nullptr;
int hit_count = 0 , miss_count = 0; //冲突不命中 ， 缓存不命中



void Init_Cache(int s , int E , int b)
{
    int S = 1<<s;
    int B = 1<<b;
    cache = (Cache*)malloc(sizeof(Cache));
    cache->S = S;
    cache->B = B;
    cache->E = E;
    cache->line = (Cache_line**)malloc(sizeof(Cache_line) * S * E); 
    for(int i=0;i<S;i++)
    {
        for(int j=0;j<E;j++) 
        {
            cache->line[i][j].valid = 0;
            cache->line[i][j].tag = -1;
            cache->line[i][j].time_tamp = 0;
        }
    }
}

void update(int i , int op_s , int op_tag)  //i是cacheline在第几行 , op_s是组号，op_tag是标记位
{
    cache->line[op_s][i].valid = 1;
    cache->line[op_s][i].tag = op_tag;
    for(int k=0;k<cache->E;k++)
    {
        if(cache->line[op_s][k].valid == 1)
            cache->line[op_s][k].time_tamp++;  
        //只需要将同组的存储块的cacheline的时间戳+1，而不需要将所有有效位=1的cacheline的时间戳+1
        //因为在进行舍弃操作的时候，也是同组之间比时间戳大小，时间戳最大的被舍弃
    }
    cache->line[op_s][i].time_tamp = 0; //将用过的cacheline的时间戳归0
}

int find_LRU(int op_s)   //找到op_s组中最近最久未使用的块
{
    int max_tamp = 0;
    int ans = 0;
    for(int i=0;i<cache->E;i++)
    {
        if(cache->line[op_s][i].time_tamp > max_tamp) 
        {
            ans = i;
            max_tamp = cache->line[op_s][i].time_tamp;
        }
    }
    return ans;
}

int get_index(int op_s , int op_tag)  //组为op_s，tag为op_tag的索引
{
    for(int i=0;i<cache->E;i++)
    {
        if(cache->line[op_s][i].tag == op_tag && cache->line[op_s][i].valid == 1) return i;
    }
    return -1;
}
 
int is_full(int op_s)   //若未满则返回第一个空缺位置的坐标
{
    for(int i=0;i<cache->E;i++)
    {
        if(cache->line[op_s][i].valid == 0) return i;
    }
    return -1;
}

void update_info(int op_s , int op_tag)  //更新的完整流程
{
    int index = get_index(op_s , op_tag);
    if(index == -1)
    {
        miss_count++;
        printf("miss ");
        int i = is_full(op_s);
        if(i) update(i , op_s , op_tag);
        else 
        {
            int t = find_LRU(op_s);
            update(t , op_s , op_tag);
        } 
    }
    else 
    {
        hit_count++;
        printf("hit ");
        update(index , op_s , op_tag);
    }
}

```



