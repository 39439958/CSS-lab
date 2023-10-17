### Exercise 1: If you add a background command after the pipeline command, can the pipeline work?

![image-20231010144911539](C:\Users\xuliz\AppData\Roaming\Typora\typora-user-images\image-20231010144911539.png)

同时运行了导管和&，发现命令行出了点问题，好像先打印出了prompt，也就是说父进程在很早就返回了，没看shell的源码，自己实现完简易shell再试试会不会有问题

![image-20231010145105217](C:\Users\xuliz\AppData\Roaming\Typora\typora-user-images\image-20231010145105217.png)

自己实现了一下，发现也出现问题了，查看了一下`Cmd_run()`函数中`CMD_BACK`和`CMD_PIPE`的实现，发现pipe在执行后半段时由于是后台执行，所以父进程直接返回了，进一步直接返回到main了，所以打印了prompt

### Exercise 2

```c
 int pid = fork();
    if(pid > 0){
        printf("parent: child=%d\n", pid);
        pid = wait((int *) 0);
        printf("child %d is done\n", pid);
    } else if(pid == 0){
        printf("child: exiting\n");
        exit(0);
    } else {
        printf("fork error\n");
    }
```

如果没有`pid = wait((int *)0);`，父进程会直接打印`child is done`然后返回



### Exercise 3: Can fork and exec be combined in one call?

可以，fork 和 exec 结合在一起就类似于`CreatProcess()`，分开使用可以方便一些操作的进行，例如在 pipe 中 fork 之后需要进行IO重定向再执行exec



### Exercise 4: How does the above program implement input redirection?

```c
char *argv[2];
argv[0] = "cat";
argv[1] = 0;
if(fork() == 0) {
    close(0);
    open("input.txt", O_RDONLY);
    exec("cat", argv);
}
```

上述代码通过关闭标准输入`close(0);`，然后打开`input.txt`，由于新打开的文件描述符是可用序号中最小的那个，所以0号标准输入被换成了这个新的文件



### Exercise 5: For the two lines close(0); dup(p[0]); can we reorder them?

不能切换顺序，因为只有在关闭掉0号描述符之后，`dup(p[0])` 使文件描述符0引用管道的读端



### Exercise 6: What are the advantages of pipes over temporary files in this situation?

`pipes`的使用看上去更简洁，且不会产生临时文件`xyz`



### Exercise7

实现`AST Node`的创建

```c
Cmd_t Cmd_Back_new(Cmd_t back){
    // TODO
    Cmd_Back cmd;
    NEW(cmd);
    cmd->type = CMD_BACK;
    cmd->back = back;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Pipe_new(Cmd_t left, Cmd_t right){
    // TODO
    Cmd_Pipe cmd;
    NEW(cmd);
    cmd->type = CMD_PIPE;
    cmd->left = left;
    cmd->right = right;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Redir_new(Cmd_t left, Cmd_t right, int fd, int flag){
    // TODO
    Cmd_Redir cmd;
    NEW(cmd);
    cmd->type = CMD_REDIR;
    cmd->left = left;
    cmd->right = right;
    cmd->fd = fd;
    cmd->flag = flag;//用于表示输入重定向还是输出重定向

    return (Cmd_t)cmd;
}
```



### Exercise 8

实现`AST Node`打印

```c
case CMD_BACK: {
            // TODO
            struct Cmd_Back *t = (struct Cmd_Back *)cmd;
            Cmd_t back = t->back;

            Cmd_print(back);
            printf(" &");
            break;
        }

        case CMD_PIPE: {
            // TODO
            struct Cmd_Pipe *t = (struct Cmd_Pipe *) cmd;
            Cmd_t left = t->left;
            Cmd_t right = t->right;

            Cmd_print(left);
            printf(" | ");
            Cmd_print(right);
            break;
        }

        case CMD_REDIR: {
            // TODO
            struct Cmd_Redir *t = (struct Cmd_Redir *) cmd;
            Cmd_t left = t->left;
            Cmd_t right = t->right;

            Cmd_print(left);
            if (t->flag == 1)
                printf(" > ");
            else 
                printf(" < ");
            Cmd_print(right);
            break;
        }
```



### Exercise 9

实现指令的运行

```c
case CMD_BACK: {
                // TODO
                struct Cmd_Back *t = (struct Cmd_Back *)cmd;
                Cmd_t back = t->back;
                pid_t pid = 0;
                
                pid = fork();
                if(pid == 0){
                    Cmd_run(back);
                }
                printf("back process[%d]\n", pid);
                break;
            }

            case CMD_PIPE: {
                // TODO
                struct Cmd_Pipe *t = (struct Cmd_Pipe *) cmd;
                Cmd_t left = t->left;
                Cmd_t right = t->right;
                int p[2];

                pipe(p);
                if (fork() == 0) {
                    close(1);
                    dup(p[1]);
                    close(p[0]);
                    close(p[1]);
                    Cmd_run(left);
                }
                wait(0);
                if (fork() == 0) {
                    close(0);
                    dup(p[0]);
                    close(p[0]);
                    close(p[1]);
                    Cmd_run(right);
                }
                close(p[0]);
                close(p[1]);
                wait(0);
                break;
            }

            case CMD_REDIR: {
                // TODO
                struct Cmd_Redir *t = (struct Cmd_Redir *) cmd;
                Cmd_t left = t->left;
                Cmd_t right = t->right;
                struct Cmd_Atom  *tmp = (struct Cmd_Atom *) right;
                struct node *node = tmp->node;
                char *path = node->data;
            
                if (t->flag == 1) {
                    if (fork() == 0) {
                        close(1);
                        int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
                        Cmd_run(left);
                    }
                    wait(0);
                } else if (t->flag == 2) {
                    if (fork() == 0) {
                        close(0);
                        int fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
                        Cmd_run(left);
                    }
                    wait(0);
                }
                break;
            }
```



### Challenge

在上述代码中以及实现，需要修改一下`scanner.l`和`parser.y`

```c
// scanner.l
">"|"<"                                    {return yytext[0];}

//parser.y
%left '>' '<'
    
|  command '>' command		{ $$ = Cmd_Redir_new($1, $3, 1, 1);}
|  command '<' command		{ $$ = Cmd_Redir_new($1, $3, 1, 2);}
```



### 运行结果

![image-20231010143811519](C:\Users\xuliz\AppData\Roaming\Typora\typora-user-images\image-20231010143811519.png)
