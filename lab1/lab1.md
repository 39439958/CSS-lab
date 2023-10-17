## Part A

exercise1-3，challenge1-2：基本功能都实现了，`printf`函数只实现了%d,%c,%s三个参数

## Part B

### Exercise 4

> -c：生成可执行文件
>
> -S：生成汇编文件
>
> -o：给生成的文件取名
>
> -m：选择指令集
>
> -v：显示详细的编译信息

-c,-S只能同时使用一个，-o,-m,-v可以随意叠加使用

### Exercise 5

> a = a + 5;

```asm
push %rax
movq $5, %rax
pop %rbx
addq %rbx, %rax
```

> print(a)

```asm
movq %rax, %rsi
leaq mcc_format(%rip), %rdi
call printf
```

### Exercise 6

mcc 编译器的 riscv 汇编代码生成函数中需要添加以下内容

```c
static void POP(){
    // TODO
    stackoffset--;
    sprintf(instr,"\tlw\ta2, %d(sp)\n", 4*stackoffset);
    emit(instr);
}

static void compile_exp(Exp_t exp){
    switch (exp->type) {
        case EXP_NUM:{
            Exp_Num e = (Exp_Num)exp;
            sprintf(instr, "\tli\ta1, %d\n", e->num);
            emit(instr);
            break;
        }
        case EXP_VAR: {
            // TODO
            Exp_Var e = (Exp_Var) exp;
            sprintf(instr, "\tlla\ta3, %s\n", e->name);
            emit(instr);
            sprintf(instr, "\tlw\ta1, 0(a3)\n");
            emit(instr);
            break;
        }
        case EXP_ADD: {
            Exp_Add e = (Exp_Add) exp;
            compile_exp(e->left);
            PUSH();
            compile_exp(e->right);
            POP();
            emit("\taddw\ta1, a1, a2\n");
            break;
        }
        case EXP_SUB: {
            // TODO
            Exp_Sub e = (Exp_Sub) exp;
            compile_exp(e->left);
            PUSH();
            compile_exp(e->right);
            POP();
            emit("\tsubw\ta1, a2, a1\n");
            break;
        }
        case EXP_TIMES: {
            // TODO
            Exp_Times e = (Exp_Times) exp;
            compile_exp(e->left);
            PUSH();
            compile_exp(e->right);
            POP();
            emit("\tmulw\ta1, a1, a2\n");
            break;
        }
        case EXP_DIV: {
            Exp_Div e = (Exp_Div) exp;
            compile_exp(e->left);
            PUSH();
            compile_exp(e->right);
            POP();
            emit("\tdivw\ta1, a2, a1\n");
            break;
        }
        default:{
            emit("\t# Unknown expression type\n");
            break;
        }
    }
}

static void compile_stm(Stm_t stm){
    switch (stm->type) {
        case STM_ASSIGN: {
            Stm_Assign s = (Stm_Assign) stm;
            set_add(s->x);
            compile_exp(s->exp);
            sprintf(instr, "\tlla\ta3, %s\n", s->x);
            emit(instr);
            sprintf(instr, "\tsw\ta1, 0(a3)\n");
            emit(instr);
            break;
        }
        case STM_PRINT: {
            // TODO
            Stm_Print s = (Stm_Print) stm;
            compile_exp(s->exp);
            emit("\tlla	a0, mcc_format\n");
            emit("\tcall printf\n");
            break;
        }
        case CMD_SEQ: {
            // TODO
            Stm_Seq s = (Stm_Seq) stm;
            compile_stm(s->left);
            compile_stm(s->right);
            break;
        }
        default: {
            emit("\t# Unknown statement type\n");
            break;
        }
    }
}
```

添加完后重新编译 mcc ，之后用下面的指令生成一个`input_riscv.out`的riscv机器可执行程序

```shell
$ ./mcc input.c -m RISCV -o input_riscv.out
```

在 spike 中执行上面生成的文件，检测是否正常运行

```shell
$ spike pk input_riscv.out
```

![image-20231009173007669](C:\Users\xuliz\AppData\Roaming\Typora\typora-user-images\image-20231009173007669.png)
