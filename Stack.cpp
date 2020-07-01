#include <stdio.h>
#include <stdlib.h>

#include "Stack.h"

#define STACK_INCREMENT 10
int STACK_INIT_SIZE=100;

void InitStack(Stack &S)
{
    S.base=(int*)malloc(sizeof(int)*STACK_INIT_SIZE);
    S.top=S.base;
    S.stacksize=0;
}

bool EmptyStack(Stack S)
{
    if(S.top==S.base)
        return true;
    return false;
}

void PushStack(Stack &S,int i)
{
    S.stacksize++;
    if(S.stacksize>STACK_INIT_SIZE)
    {
        STACK_INIT_SIZE+=STACK_INCREMENT;
        S.base=(int*)realloc(S.base,sizeof(int)*STACK_INIT_SIZE);
        S.top=S.base+(S.stacksize-1);
    }
    *S.top=i;
    S.top++;
}

int PopStack(Stack &S)
{
    int i=*(S.top-1);
    S.top--;
    S.stacksize--;
    return i;
}

int GetTopStack(Stack S)
{
    return *(S.top-1);
}

int GetStackNum(Stack S)
{
    return S.top-S.base;
}

int GetStackElement(Stack S,int i)
{
    return *(S.base+i);
}

void PrintStack(Stack S)
{
    int i,j=S.top-S.base;
    for(i=0;i<j;i++)
        printf("%d ",*(S.base+i));
    printf("\n");
}
