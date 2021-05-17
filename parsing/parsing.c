#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_LEN 2048
typedef struct states_stack{
    int State;
    struct states_stack* next;
}States_stack;

int lr(const char w[]);
States_stack* push(States_stack * stack,int a);
States_stack* pop(States_stack * stack);
int for_line(char a);
int pop_len(int a);
int reduce(int a);
int print_state(States_stack * stack);
int print_symbol(States_stack * stack);


int main()
{
	char w[BUFFER_LEN];//存储需要分析的字符串
	scanf("%s", &w);
	lr(w);
    return 0;
}

int lr(const char w[])
{
//    保存分析表，acc用666代替，正数为状态，负数为规约
    int lr_action[16][11]={{0,0,0,0,4,0,5,0,1,2,3},
                           {6,7,0,0,0,0,0,666,0,0,0},
                           {-3,-3,8,9,0,-3,0,-3,0,0,0},
                           {-6,-6,-6,-6,0,-6,0,-6,0,0,0},
                           {0,0,0,0,4,0,5,0,10,2,3},
                           {-8,-8,-8,-8,0,-8,0,-8,0,0,0},
                           {0,0,0,0,4,0,5,0,0,11,3},
                           {0,0,0,0,4,0,5,0,0,12,3},
                           {0,0,0,0,4,0,5,0,0,0,13},
                           {0,0,0,0,4,0,5,0,0,0,14},
                           {6,7,0,0,0,15,0,0,0,0,0},
                           {-1,-1,8,9,0,-1,0,-1,0,0,0},
                           {-2,-2,8,9,0,-2,0,-2,0,0,0},
                           {-4,-4,-4,-4,0,-4,0,-4,0,0,0},
                           {-5,-5,-5,-5,0,-5,0,-5,0,0,0},
                           {-7,-7,-7,-7,0,-7,0,-7,0,0,0}};
//    初始状态入栈
    States_stack * states=(States_stack*)malloc(sizeof(States_stack));
    states->next = NULL;
    states = push(states, 0);
    States_stack* symbol=(States_stack*)malloc(sizeof(States_stack));
    symbol->next = NULL;
    symbol = push(symbol, 1);
//    字符串w所在的位置(用字符串代替指针)
    int now_w = 0;
    do {
        print_state(states);
        print_symbol(symbol);
        printf("输入：");
        for (int i = now_w; i <strlen(w) ; ++i) {
            printf("%c",w[i]);
        }
        printf("$\n");
        //读取的是数字
        while (w[now_w]<='9' && w[now_w]>='0' && w[now_w+1] <= '9' && w[now_w+1] >= '0')
        {
            now_w++;
        }
        int a = for_line(w[now_w]);
        int now_state = lr_action[states->State][a];
        if(now_state==666)
        {
            printf("accept!\n");
            break;
        }
        if(now_state>0)
        {
            printf("shift %d\n\n",now_state);
            states = push(states, now_state);
            symbol = push(symbol, a);
            now_w++;
        }
        else
        {
            if(now_state<0)
            {
                a = reduce(now_state);
                for (int i=0;i<pop_len(now_state);i++)
                {
                    states = pop(states);
                    symbol = pop(symbol);
                }
                symbol = push(symbol, a);
                states = push(states, lr_action[states->State][a]);
            } else
            {
                printf("error");
                break;
            }
        }
    } while (1);
	return 0;
}

States_stack* push(States_stack * stack,int a){
    States_stack * line=(States_stack*)malloc(sizeof(States_stack));
    line->State=a;
    line->next= (struct States_stack*) stack;
    stack=line;
    return stack;
}
States_stack * pop(States_stack * stack){
    if (stack) {
        States_stack * p=stack;
        stack= (States_stack *) stack->next;
//        printf("弹栈元素：%c ",p->State);
        if (stack) {
//            printf("栈顶元素：%c\n",stack->State);
        }else{
//            printf("栈已空\n");
        }
        free(p);
    }else{
//        printf("栈内没有元素");
        return stack;
    }
    return stack;
}

int for_line(char a) {
    char line[12] = {'+','-','*','/','(',')','1','\0','E','F','T','\0'};
    int r = 0;
    if(a<='9' && a>='0')
    {
        r = 6;
    } else
    {
        for (int i=0;i<12;i++)
        {
            if(line[i]==a)
            {
                r = i;
                break;
            }
        }
    }
    return r;
}

int pop_len(int a) {
//    判断要弹出多少个栈顶元素
    int r = 3;
    if(a==-3 || a==-6 || a==-8)
    {
        r = 1;
    }
    return r;
}

int reduce(int a) {
    int r;
    printf("reduce ");
    switch (a) {
        case -1:
            printf("E->E+T\n");
            r = 8;
            break;
        case -2:
            printf("E->E-T\n");
            r = 8;
            break;
        case -3:
            printf("E->T\n");
            r = 8;
            break;
        case -4:
            printf("T->T*F\n");
            r = 9;
            break;
        case -5:
            printf("T->T/F\n");
            r = 9;
            break;
        case -6:
            printf("T->F\n");
            r = 9;
            break;
        case -7:
            printf("F->(E)\n");
            r = 10;
            break;
        case -8:
            printf("F->num\n");
            r = 10;
            break;
        default:
            r = 0;
            break;
    }
    printf("\n");
    return r;
}

int print_state(States_stack *stack) {
    States_stack * line = (States_stack*)malloc(sizeof(States_stack));
    line = stack;
    printf("state:");
    while (line->next)
    {
        printf("%d ",line->State);
        line = (States_stack *) line->next;
    }
    printf("\n");
    return 0;
}

int print_symbol(States_stack *stack) {
    char list[12] = {'+','-','*','/','(',')','1','$','E','T','F','\0'};
    States_stack * line = (States_stack*)malloc(sizeof(States_stack));
    line = stack;
    printf("symbol:");
    while (line->next)
    {
        if(line->State == 6)
        {
            printf("num ");
        }
        else {
            printf("%c ", list[line->State]);
        }
        line = (States_stack *) line->next;
    }
    printf("\n");
    return 0;
}
