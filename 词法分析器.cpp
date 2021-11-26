#include<iostream>
#include<cstring>
#include<fstream>
#include<string.h>
using namespace std;

char prog[2000], token[30];
int p = 0,sym = 0; 
char fileName[30];
char ch;
fstream file;
int line=1;
char* keyword[21]={"if","else","while","do","main","int","float","double","return","const","void","continue","break","char","unsigned","enum","long","switch","case","auto","static"};

//保留字 1-21
// 数字  整数22 小数 23
//字符 24 
//字符串 25
// == 26  =27 
// <= 28 <<29  <30 
// >=31  >>32 >>33
// !=34
// && 35 &36
// ||37 |38
//  /39
// 16进制 40 8进制 41 
void getToken(){
	ch = prog[p];
	 //清空token数组 
	for (int i = 0; i < 30; i++)
		token[i] = '\0';
	int n = 0;
	while (ch == ' ' || ch == '\n' || ch == '\t')
	{
		if (ch == '\n')
		{
			line++;
			cout << endl;
			cout << "第" << line << "行：";
		}
		ch = prog[++p];
	}
	//识别标识符 
	if (ch >= 'a' && ch <= 'z'|| ch >= 'A' && ch <= 'Z' || ch == '_')
	{
		token[n++] = ch;
		ch = prog[++p];
		sym = 40;
		int temp = p;
		while (ch >= 'a' && ch <= 'z'|| ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '_')
		{
			if (n < 20)
			{
				token[n++] = ch;
			}
			ch = prog[++p];
		}
		if (temp != p)sym = 52;
		for (int i = 0; i < 21; i++)
			if (strcmp(token,keyword[i])==0)
				sym = i + 1;
	}
	//识别数字 
	else if (ch >= '0' && ch <= '9')
	{   n = 0;
	    if(ch=='0'&&prog[p+1]!=' '&&prog[p+1]!=';' ){
	    	token[n++]=ch;
	    	ch=prog[++p];
	    	//16进制 
	    	if(ch=='x'||ch=='X'){
	    		token[n++]=ch;
	    		ch=prog[++p];
	    		while(ch>='0'&&ch<='9' || ch>='A'&&ch<='F'){
	    			token[n++]=ch;
	    			ch=prog[++p];
				}
				if(ch>'F'&&ch<='Z'){
					sym=-4;
					return;
				}
				sym=40;
				return;
				
			}
			//8进制 
			else{
			  		while(ch>='0'&&ch<='7'){
	    			token[n++]=ch;
	    			ch=prog[++p];
				}
				if(ch>='8'&&ch<='9'){
					sym=-5;
					return;
				}
				sym=41;
				return;
			}
		}
	    token[n++] = ch;
		ch = prog[++p];
		sym = 22;
		while (ch >= '0' && ch <= '9'||ch=='.')
		{
				if (ch == '.')
				{
					token[n++] = ch;
					break;
				}
				token[n++] = ch;
				ch = prog[++p];
			
		}
		if (token[n-1]!='.')sym = 22;
		else if(token[n - 1] == '.') 
		{
			ch = prog[++p];
			int flag=0;
			while (ch >= '0' && ch <= '9')
			{   
			    flag=1;
				if (n < 30)
				{
					token[n++] = ch;
				}
				ch = prog[++p];
			}
			if(!flag){
				//小数点后无数字,报错 
				sym=-1;
				return;	
			}
			sym = 23;
		}
	}
    else if (ch == '"')
	{
		
		token[n] = ch;
		ch = prog[++p];
		while (ch != '"'&&n<29)
		{
			token[++n] = ch;
			ch = prog[++p];
		}
		if (ch == '"')
		{
			token[++n] = ch;
			sym = 25;
			p++;
		}
		else
		{
			sym = -2;
			p++;
		}
	}
	else if (ch == '=')
	{
		char s = prog[++p];
		if (s == '=')
		{
			token[0] = ch;
			token[1] = s;
			p++;
			sym = 26;
		}
		else
		{
			token[0] = ch;
			sym = 27;
		}
	}
	else if (ch == '<')
	{
	    char s = prog[++p];
	    if (s == '=')
	    {
		    token[0] = ch;
		    token[1] = s;
		    p++;
		    sym = 28;
	    }
	    else if(s=='<')
	    {
			token[0] = ch;
			token[1] = s;
			p++;
		    sym = 29;
	    }
		else
		{
			token[0] = ch;
			sym = 30;
		}
	}
	else if (ch == '>')
	{
	    char s = prog[++p];
	    if (s == '=')
	    {
		    token[0] = ch;
		    token[1] = s;
		    p++;
		    sym = 31;
	    }
		else if (s == '>')
		{
			token[0] = ch;
			token[1] = s;
			p++;
			sym = 32;
		}
	    else
	    {
			token[0] = ch;
		    sym = 33;
	    }
	}
	else if (ch == '!')
	{
	    char s = prog[++p];
	    if (s == '=')
	    {
		    token[0] = ch;
		    token[1] = s;
		    p++;
		    sym = 34;
	    }
	    else
	    {
		    sym = -2;
			p++;
	    }
	}
	else if (ch == '&')
	{
	    char s = prog[++p];
	    if (s == '&')
	    {
		    token[0] = ch;
		    token[1] = s;
		    p++;
		    sym = 35;
	    }
	    else
	    {
			token[0] = ch;
		    sym = 36;
	    }
	}
	else if (ch == '|')
	{
	    char s = prog[++p];
	    if (s == '|')
	    {
		    token[0] = ch;
		    token[1] = s;
		    p++;
		    sym = 37;
	    }
	    else
	    {
			token[0] = ch;
		    sym = 38;
	    }
	}
	else if (ch == '/')
	{
	    char s = prog[++p];
		if (s == '/')
		{
			while (ch != '\n')
			{
				ch = prog[++p];
			}
			sym=100;
		}
		else if (s == '*')
		{
			p++;
			while (1)
			{   if(prog[p]=='\n'){
				line++;
			}
				if (prog[p] == '*' && prog[p + 1] == '/')
					break;
				p++;
			}
			p += 2;
			sym=100;
		}
		else
		{
			sym = 39;
		}
    }
    else
	{
		switch (ch)
		{
		   case '+':sym = 40; token[0] = ch; p++; break;
		   case '-':sym = 41; token[0] = ch; p++; break;
		   case '*':sym = 42; token[0] = ch; p++; break;
		   case '%':sym = 43; token[0] = ch; p++; break;
		   case ',':sym = 44; token[0] = ch; p++; break;
		   case ';':sym = 45; token[0] = ch; p++; break;
		   case '#':sym = 46; token[0] = ch; p++; break;
		   case '{':sym = 47; token[0] = ch; p++; break;
		   case '}':sym = 48; token[0] = ch; p++; break;
		   case '[':sym = 49; token[0] = ch; p++; break;
		   case ']':sym = 50; token[0] = ch; p++; break;
		   case '(':sym = 51; token[0] = ch; p++; break;
		   case ')':sym = 52; token[0] = ch; p++; break;
		   case EOF: cout<<"finish！"<<endl;break;
		   default: sym = -2; p++; cout << endl << "词法分析出错，请检查是否输入非法字符!\n" << "位置：第" << line << "行" << endl;; break;
		}
	}
    
}
int main(){
	cout << "请输入源文件名：" << endl;
	while (1)
	{
		cin >> fileName;
		file.open(fileName, ios::in);
		if (file)break;
		else cout << "文件路径错误，请输入源文件名：" << endl;
	}
	cout << "源程序" << endl;
	cout<<endl;
	do
	{
		ch=file.get();
		cout << ch;
		prog[p++] = ch;
	} while (ch!=-1);
	cout <<endl<< "词法分析" << endl;
	p = 0;
	cout << "第" << line << "行：";
	ch = prog[p];
	while (p < 2000 && ch != '\0' && ch != EOF)
	{
		getToken();
		if (ch == EOF)break;
		switch (sym)
		{
		   case -1: cout<<sym<<" "<<"小数格式错误!"<<"  ";
		   case -2:break;
		   case -4:cout<< "("<<sym<<" "<<"16进制格式错误"<< ")"<<" ";break;
		   case -5:cout<< "("<<sym<<" "<<"8进制格式错误"<< ")"<<" ";break;
		   case 100:cout<<" "<<"这是一段注释"<<" "; break;
		   default: cout << "(" << sym << "," << token << ")" << "  "; break;
		}
	}
	return 0;
}
