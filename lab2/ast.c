#include "ast.h"
#include "alloc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

struct node *make_node(char *data, struct node *next){
    struct node *t = malloc(sizeof(*t));
    t->data = data;
    t->next = next;
    return t;
}

// constructor of each AST node
Cmd_t Cmd_Seq_new(Cmd_t left, Cmd_t right){
    Cmd_Seq cmd;
    NEW(cmd);
    cmd->type = CMD_SEQ;
    cmd->left = left;
    cmd->right = right;

    return (Cmd_t)cmd;
}

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
    cmd->flag = flag;

    return (Cmd_t)cmd;
}

Cmd_t Cmd_Atom_new(struct node *node){
    Cmd_Atom cmd;
    NEW(cmd);
    cmd->type = CMD_ATOM;
    cmd->node = node;

    return (Cmd_t)cmd;
}

// print AST
void Cmd_print(struct Cmd_t *cmd){
    switch(cmd->type){
        case CMD_ATOM: {
            struct Cmd_Atom  *t = (struct Cmd_Atom *) cmd;
            struct node *node = t->node;
            while (node) {
                printf("%s ", node->data);
                node = node->next;
            }
            break;
        }

        case CMD_SEQ: {
            printf("enter\n");
            struct Cmd_Seq *t = (struct Cmd_Seq *) cmd;
            printf("enter\n");
            Cmd_t left = t->left;
            Cmd_t right = t->right;

            Cmd_print(left);
            printf("; ");
            Cmd_print(right);
            break;
        }

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
        default:
            break;
    }
}

// run cmd
void Cmd_run(struct Cmd_t *cmd){
        switch(cmd->type){
            case CMD_ATOM: {
                char *arg[10] = {0};
                int i = 0;
                struct Cmd_Atom  *t = (struct Cmd_Atom *) cmd;
                struct node *node = t->node;
                while(node){
                    arg[i] = node->data;
                    node = node->next;
                    i++;
                }
                char *root = "/bin/";
                char binPath[50];
                strcpy(binPath, root);
                strcat(binPath, arg[0]);
                if(execv(binPath, arg) == -1){
                    char *path= "/usr/bin/";
                    char usrBinPath[50];
                    strcpy(usrBinPath, path);
                    strcat(usrBinPath, arg[0]);
                    if(execv(usrBinPath, arg) == -1){
                        fprintf(stderr,"cannot run command, check your input.\n");
                    }
                }
                break;
            }

            case CMD_SEQ: {
                struct Cmd_Seq *t = (struct Cmd_Seq *) cmd;
                Cmd_t left = t->left;
                Cmd_t right = t->right;

                if(fork() == 0){
                    Cmd_run(left);
                }
                wait(0);
                Cmd_run(right);
                break;
            }

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

            default:
                break;
        }

    exit(0);
}
