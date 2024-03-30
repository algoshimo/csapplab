//shell其实只做了三件事，循环地读取指令，解析指令，执行解析的指令

//执行的命令有可能是环境变量中的一个程序，也有可能是shell的内置命令
//前者需要使用execvp新建一个进程来代替原先的子进程，后者则只需要在父进程直接运行就好了

# include<stdio.h>
# include<stdlib.h>

//读取一段命令的时候，我们无法判断究竟会读取多少命令
//我们不能说分配一块内存，然后祈祷读入的命令不会超过这段内存
//而是应该先分配一块内存，如果不够用的话继续分配

#define LSH_RL_BUFSIZE 1024
char* lsh_read_line(void)  //读命令
{
    int bufsize = LSH_RL_BUFSIZE;  //每次分配内存的大小
    int position = 0;             //指向buffer数组最后的元素
    char* buffer = malloc(sizeof(char) * bufsize); //存着读取的命令
    int c;

    if(!buffer){
        fprintf(stderr , "lsh : allocation error\n"); //分配失败则直接用标准错误流输出分配失败
        exit(EXIT_FAILURE);   //终止程序执行，并将失败状态码EXIT_FAILURE返回给操作系统
    }

    while(1){
        c = getchar();  //因为EOF是负整数常量，所以c是int类型

        if(c == EOF || c == '\n') 
        {
            buffer[position] = '\0'; 
            //如果碰到EOF或换行，则在字符串后加'\0'（字符串结束的标志）然后返回该字符串
            return buffer;
        }
        else 
        {
            buffer[position] = c;
        }
        position++;

        if(position >= bufsize)  //如果命令需要的内存超过了分配的，则需要再多分配LSH_RL_BUFSIZE
        {
            bufsize += LSH_RL_BUFSIZE;
            buffer = realloc(buffer , bufsize);

            if(!buffer)  
            {
                fprintf(stderr , "lsh : allocation error\n"); 
                exit(EXIT_FAILURE);
            }
        }
    }
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM "\t\r\n\a"
char** lsh_split_line(char*line)  //解析命令
{
    int bufsize = LSH_TOK_BUFSIZE , position = 0;  //bufsize是tokens的容量
    char **tokens = malloc(bufsize * sizeof(char*)); //line根据各种空格被分割成不同的字符串token，tokens储存token  
    char *token;  

    if(!tokens){   //若malloc失败
        fprintf(stderr , "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token  = strtok(line , LSH_TOK_DELIM);   //根据第二个参数分割，比如Hello world   hhh被分割成三个字符串:"hello" "world" "hhh"
    while(token)  //将line分离存入tokens
    {
        tokens[position++] = token;  
        
        if(position >= bufsize)  //若空间不够
        {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens , bufsize * sizeof(char*));
            if(!tokens)
            {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL , LSH_TOK_DELIM);
    }
    tokens[position] = NULL;  //参照95行注释，参数列表的最后一个位置必须是NULL
    return tokens;
}

int lsh_launch(char **args)   //若不为内置命令，则需要新建一个进程，然后使用execvp在子进程运行该命令所对应的程序
{
    pid_t pid , wpid;
    int status;

    pid = fork();
    if(pid == 0) 
    { 
        if(execvp(args[0] , args) == -1) 
        //若当前进程为子进程，则在环境变量中找到和args[0]同名的程序，并启动新程序args[0]来代替当前进程
        //args 是一个指向参数列表的指针数组，其中 args[0] 是可执行文件的名称，随后的元素是传递给程序的参数
        //最后一个参数必须是 NULL，用来表示参数列表的结束。
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else if(pid < 0) 
    {
        perror("lsh");
    }
    else //若为父进程，则等待子进程执行
    {
        do
        {
            wpid = waitpid(pid,&status,WUNTRACED); //等待具有进程ID为pid的子进程结束，并将子进程的状态信息存储在status中
        }
        while (!WIFEXITED(status) && !WIFSIGNALED(status));  //循环会继续，直到子进程正常退出或者因为信号而退出。
    }
}

//shell的内置命令
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int(*builtin_func[])(char **) = {  //函数指针数组，函数可以接收char**的参数
    &lsh_cd,
    &lsh_help, 
    &lsh_exit 
};

int lsh_num_builtins(){
    return sizeof(builtin_str) / sizeof(char*);
}

int lsh_cd(char **args)
{
    if(args[1] == NULL)
    {
        fprintf(stderr,"lsh:expected argument to \"cd\"\n");
    }
    else 
    {
        if(chdir(args[1]) != 0) //改变当前目录到args[1]。成功返回0，失败返回-1
        perror("lsh");
    }
    return 1;
}

int lsh_help(char **args)
{
    int i;
    printf("Stephen Brennan's LSH\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for(i=0;i<lsh_num_builtins;i++)
    {
        printf(" %s\n" , builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int lsh_exit(char **args)
{
    return 0;
}

int lsh_execute(char **args)  //执行命令
{
    int i;

    if(args[0] == NULL) return 1; //空指令

    for(i = 0;i < lsh_num_builtins();i++)
    {
        if(strcmmp(args[0] , builtin_str[i]) == 0)  //若为内置命令
        {
            return (*builtin_func[i])(args);
        }
    }    
 
    return launch(args);  //若不为内置命令
}

void lsh_loop(void)
{
    char *line;
    char **args;
    int status;

    do {
        printf(">");
        line = lsh_read_line(); //读取一段命令
        args = lsh_split_line(line); //解析这段命令，将程序和参数分开
        status = lsh_execute(args);  //执行解析好的命令

        free(line);
        free(args);
    }while(status);
}

int main(int argc , char **argv)
{
    lsh_loop();  // //shell其实只做了三件事，循环地读取指令，解析指令，执行解析的指令
    return EXIT_SUCCESS;
}