#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

typedef struct Stack
{
    int *top;
    int *base;
    int stacksize;
}Stack;

void InitStack(Stack &S);//初始化栈
bool EmptyStack(Stack S);//判断是否栈空
void PushStack(Stack &S,int i);//入栈
int PopStack(Stack &S);//出栈
int GetTopStack(Stack S);//获取栈顶元素
int GetStackNum(Stack S);//获取栈内元素数量
int GetStackElement(Stack S,int i);//获取栈内指定下标元素
void PrintStack(Stack S);

#endif // STACK_H_INCLUDED
