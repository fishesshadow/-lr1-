/*
产生式 
//（1）E→E+T
//（2）E→T
//（3）T→T*F
//（4）T→F
//（5）F→(E)
//（6）F→i
*/

/*
待规约式子 
//i+i*i#
*/

/*
//思路
// 先对符号栈，状态栈，字符串分别执行相应的初始化操作
//调用action函数，如果是移进则将字符串中的字符移入符号栈，同时相应的状态也进入状态栈
//如果是规约则根据相应的产生式将字符串规约，并将最终规约的非终结符入栈同时相应的状态入栈 
 
*/ 

#include<iostream>
#include<malloc.h>  
using namespace std;
#define MAX 20
int table[12][9] = {               // -1报错  1-11 移进   12 acc  22-26 归约
{5,-1,-1,4,-1,-1,1,2,3},
{-1,6,-1,-1,-1,12,-1,-1,-1},
{-1,22,7,-1,22,22,-1,-1,-1},
{-1,24,24,-1,24,24,-1,-1,-1},
{5,-1,-1,4,-1,-1,8,2,3},
{-1,26,26,-1,26,26,-1,-1,-1},
{5,-1,-1,4,-1,-1,-1,9,3},
{5,-1,-1,4,-1,-1,-1,-1,10},
{-1,6,-1,-1,11,-1,-1,-1,-1},
{-1,21,7,-1,21,21,-1,-1,-1},
{-1,23,23,-1,23,23,-1,-1,-1},
{-1,25,25,-1,25,25,-1,-1,-1}
};
//弹出栈的个数 ，索引即代表产生式的序号 
struct rule {
	char x;
	int y;
}r[6] = { {'E',3},{'E',1},{'T',3},{'T',1},{'F',3},{'F',1} };
char index_char[9] = { 'i','+','*','(',')','#','E','T','F' };
typedef struct {
	int stack[MAX];
	int top;
}status;
typedef struct {   //符号栈 struct
	char stack[MAX];
	int top;
}symbol_instr;



int get_index_char(char i) //获取对应符号的下标
{
	for (int j = 0;j < 9;j++)
	{
		if (index_char[j] == i)
			return j;
	}
	return -1;
}

void init_stack(status* p)  //初始化状态栈
{
	if (!p)
		cout << "初始化状态栈出错！\n";
	p->top = -1;
}
void push(status* p, int x) 
{
	if (p->top < MAX - 1)
	{
		p->top++;
		p->stack[p->top] = x;
	}
	else cout << "\n状态栈溢出！\n";
}
int pop(status* p) //状态栈弹出
{
	int x;
	if (p->top != 0)
	{
		x = p->stack[p->top];
		p->top--;
		return x;
	}
	else
	{
		cout << "\n状态栈1空!\n";
		return 0;
	}
}
int get_top(status* p)  // 获取状态栈栈顶
{
	int x;
	if (p->top != -1)
	{
		x = p->stack[p->top];
		return x;
	}
	else
	{
		cout << "\n状态栈2空!\n";
		return 0;
	}
}
void out_stack(status* p)  
{
	int i;
	if (p->top < 0)
	{
		cout << "\n状态栈3空!\n";
	}
	for (i = 0;i <= p->top;i++)
	{
		cout << p->stack[i];
	}
}

void init_stack(symbol_instr* p)   //初始化符号栈
{
	if (!p)
		cout << "\n初始化状态栈出错！\n";
	p->top = -1;
}
void push(symbol_instr* p, char x)  //符号栈的压入
{
	if (p->top < MAX - 1)
	{
		p->top++;
		p->stack[p->top] = x;
	}
	else cout << "\n状态栈溢出！\n";
}
char pop(symbol_instr* p) //符号栈的弹出
{
	char x;
	if (p->top != -1)
	{
		x = p->stack[p->top];
		p->top--;
		return x;
	}
	else
	{
		cout << "符号栈1空!\n";
		return 0;
	}
} 
char get_top(symbol_instr* p) //获取符号栈栈顶
{
	char x;
	if (p->top != -1)
	{
		x = p->stack[p->top];
		return x;
	}
	else
	{
		cout << "符号栈2空!\n";
		return 0;
	}
}
void out_stack1(symbol_instr* p){ //符号栈打印
	int i;
	if (p->top < 0)
	{
		cout << "符号栈3空!\n";
	}
	for (i = 0;i <= p->top;i++)
	{
		cout << p->stack[i];
	}
}
void out_stack2(symbol_instr* p)  //输入串打印
{
	int i;
	if (p->top < 0)
	{
		cout << "符号栈4空!\n";
	}
	for (i = p->top;i >= 0;i--)
	{
		cout << p->stack[i];
	}
}
void print(status* status_p, symbol_instr* symbol_p, symbol_instr* instr_p)  
{
	int i;
	out_stack(status_p);
	for (i = 0;i < 20 - status_p->top;i++)
		cout << " ";
	out_stack1(symbol_p);
	for (i = 0;i < 20;i++)
		cout << " ";
	out_stack2(instr_p);
	cout << endl;
}
int goto_char(status* status_p, symbol_instr* instr_p) {  
	char x;
	int y, z;
	x = get_top(instr_p);//获取字符串栈顶 
	y = get_top(status_p);//获取状态栈栈顶 
	z = get_index_char(x);//获取对应符号的下标 
	return table[y][z];//返回action对应的操作指令 

}
void action(status* status_p, symbol_instr* symbol_p, symbol_instr* instr_p) 
{
	int i, j, x;
	char a;
	i = goto_char(status_p, instr_p);  //获取当前栈顶符号的编号
	if (i == -1)
		cout << "归约出错!\n";
	if (i == 12)  //acc ‘12’
		cout << "归约成功!\n";
	if (i >= 0 && i <= 11) //移进
	{
		push(status_p, i);
		a = pop(instr_p);
		push(symbol_p, a);
		print(status_p, symbol_p, instr_p);
		action(status_p, symbol_p, instr_p);
	}
	if (i >= 21 && i <= 26) {  //归约
		x = r[i - 21].y;
		for (j = 0;j < x;j++)
		{
			pop(status_p);
			pop(symbol_p);
		}
		push(instr_p, r[i - 21].x);
		action(status_p, symbol_p, instr_p);
	}
}
int main()
{
	char x;
	status* status_p; //状态
	symbol_instr* symbol_p, * instr_p;  //符号 字符 
	status_p = (status*)malloc(sizeof(status));
	symbol_p = (symbol_instr*)malloc(sizeof(symbol_instr));
	instr_p = (symbol_instr*)malloc(sizeof(symbol_instr));
	init_stack(status_p);
	init_stack(symbol_p);
	init_stack(instr_p);
	push(status_p, 0);
	push(symbol_p, '#');
	cout << "请输入要归约的字符串，以'#'字符结束！\n";
	do
	{
		x = getchar();
		if (x != ' ' || x != '\t' || x != '\n')
			push(symbol_p, x);
	} while (x != '#');
	while (symbol_p->top != 0) {
		x = pop(symbol_p);
		push(instr_p, x);
	}
	cout << "\n\n";
	cout << "\n状态栈              符号栈                输入串\n";
	print(status_p, symbol_p, instr_p);
	action(status_p, symbol_p, instr_p);
	return 0;
}
