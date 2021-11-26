#include <iostream>
#include <cstring>
#include <fstream>
#include <string.h>
using namespace std;

void program();
void block();
void stmts();
void stmt();
void stmt1();
void Bool();
void expr();
void expr1();
void term();
void term1();
void factor();
void showError();

char prog[2000], token[30];
int p = 0, sym = 0;
char fileName[30];
char ch;
fstream file;
int line = 1;
char *keyword[21] = {"if", "else", "while", "do", "main", "int", "float", "double", "return", "const", "void", "continue", "break", "char", "unsigned", "enum", "long", "switch", "case", "auto", "static"};
string word[100];
int wordSyms[100] = {0};
int errorFind = 0;
int k = 0; //词法分析器总共识别的单词数
//保留字 1-21
// 数字  整数22 小数 23
//字符 24
//字符串 25
// == 26  =27
// <= 28 <<29  <30
// >=31  >>32 >33
// !=34
// && 35 &36
// ||37 |38
//  /39
// 16进制 40 8进制 41
// id 42
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
		sym = 42;
		while (ch >= 'a' && ch <= 'z'|| ch >= 'A' && ch <= 'Z' || ch >= '0' && ch <= '9' || ch == '_')
		{
			if (n < 20)
			{
				token[n++] = ch;
			}
			ch = prog[++p];
		}
		for (int i = 0; i < 21; i++)
			if (strcmp(token,keyword[i])==0)
				sym = i + 1 +42;
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
		   case EOF: sym=0;cout<<"finish！"<<endl;break;
		   default: sym = -2; p++; cout << endl << "词法分析出错，请检查是否输入非法字符!\n" << "位置：第" << line << "行" << endl;; break;
		}
	}
    
}

 
int main()
    {
        cout << "请输入源文件名：" << endl;
        while (1)
        {
            cin >> fileName;
            file.open(fileName, ios::in);
            if (file)
                break;
            else
                cout << "文件路径错误，请输入源文件名：" << endl;
        }
        cout << "源程序" << endl;
        cout << endl;
        do
        {
            ch = file.get();
            cout << ch;
            prog[p++] = ch;
        } while (ch != -1);
        file.close();
        cout << endl
             << "词法分析" << endl;
        p = 0;
        cout << "第" << line << "行：";
        ch = prog[p];
        int num = 0;
        while (p < 2000 && ch != '\0' && ch != EOF)
        {
            getToken();
            if (ch == EOF)
                break;
            switch (sym)
            {
            case -1:
                cout << sym << " "
                     << "小数格式错误!"
                     << "  ";
            case -2:
            case 0:
                break;
            case -4:
                cout << "(" << sym << " "
                     << "16进制格式错误"
                     << ")"
                     << " ";
                break;
            case -5:
                cout << "(" << sym << " "
                     << "8进制格式错误"
                     << ")"
                     << " ";
                break;
            case 100:
                cout << " "
                     << "这是一段注释"
                     << " ";
                break;
            default:
                cout << "(" << sym << "," << token << ")"
                     << "  ";
                word[k] = token;
                wordSyms[k++] = sym;
                break;
            }
        }
        k--; 
        /*while(k>-1){
        	cout<<wordSyms[k--]<<endl;
		}*/

        cout << endl
             << "词法分析完成！" << endl;
        cout << "递归下降语法分析：" << endl;
        p = 0;
        program();
        return 0;
    }   
    void showError()
    {
        cout << endl
             << "语法分析器分析错误！" << endl;
    }
    void program()
    {

        cout << "program-->block" << endl;
        block();
        if (errorFind == 1)
        {

            showError();
            return;
        }
        if (p == k+1)
            cout << "分析成功！";
    }
    void block()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        //cout<<wordSyms[p];
        if (p < k && wordSyms[p] == 47) //匹配 {
        {
            cout << "block-->{stmts}" << endl;
            p++;
            stmts();
            //cout<<p<<k;
            if (p <= k && wordSyms[p] == 48) //匹配 }
            {
                p++;
                return;
            }
            else
            {
                errorFind = 1;
                return;
            }
        }
        else
        {
            errorFind = 1;
            return;
        }
    }
    void stmts()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        if (p < k && wordSyms[p] == 48) //}
        {
            // p++;
            cout << "stmts-->null" << endl;
            return;
        }
        cout << "stmts-->stmt stmts" << endl;
        stmt();
        stmts();
    }
    void stmt()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        if (p < k && wordSyms[p] == 42) // id
        {
            cout << "stmt-->id = expr;" << endl;
            p++;
            if (p < k && wordSyms[p] == 27) //=
            {
                p++;
                expr();
                if (p < k && wordSyms[p] != 45) //;
                {
                    errorFind = 1;
                    return;
                }
                else
                {
                    p++;
                    return;
                }
            }
            else
            {
                errorFind = 1;
                return;
            }
        }
        else if (p < k && wordSyms[p] == 43) // if
        {
            cout << "stmt-->if(bool) stmt stmt1" << endl;
            p++;
            if (p < k && wordSyms[p] == 51) //(
            {
                p++;
                Bool();
                if (p < k && wordSyms[p] == 52) //)
                {
                    p++;
                    stmt();
                    stmt1();
                }
                else
                {
                    errorFind = 1;
                    return;
                }
            }
            else
            {
                errorFind = 1;
                return;
            }
        }
        else if (p < k && wordSyms[p] == 45) // while
        {
        	cout<<endl<<wordSyms[p]<<endl;
        	cout<<"stmt-->while(bool) stmt";
            p++;
            if (p < k && wordSyms[p] == 51) //(
            {
            	cout<<endl<<wordSyms[p]<<endl;
                p++;
                Bool();
                if (p < k && wordSyms[p] == 52) //)
                {
                    p++;
                    stmt();
                }
                else
                {
                    errorFind = 1;
                    return;
                }
            }
            else
            {
                errorFind = 1;
                return;
            }
        }
        else if (p < k && wordSyms[p] == 46) // do
        {
            p++;
            stmt();
            if (p < k && wordSyms[p] == 45) // while
            {
                p++;
                if (p < k && wordSyms[p] == 51) //(
                {
                    p++;
                    Bool();
                    if (p < k && wordSyms[p] != 52) //)
                    {
                        errorFind = 1;
                        return;
                    }
                }
                else
                {
                    errorFind = 1;
                    return;
                }
            }
            else
            {
                errorFind = 1;
                return;
            }
        }
        else if (p < k && wordSyms[p] == 55)
        {
            p++;
            if (p < k && wordSyms[p] == 45)
            {
                p++;
                return;
            }
            else
            {
                errorFind == 1;
                return;
            }
        }
        else
        {
            cout << "stmt-->block" << endl;
            block();
        }
    }
    void stmt1()
{
	if(errorFind==1||p==k)
	{
		return;
	}
	if(p<k&&wordSyms[p]==24)
	{
		p++;
		stmt();
	}
	else{
		
		return;
	}
}
	void Bool()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        expr();
        if (p < k && wordSyms[p] == 28) //<=
        {
            p++;
            cout << "bool-->expr <= expr" << endl;
            expr();
        }

        else if (p < k && wordSyms[p] == 33) //>
        {
            cout << "bool-->expr > expr" << endl;
            p++;
            expr();
        }
        else
        {
            errorFind = 1;
            return;
        }
    }
   
    void expr()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        cout << "expr-->term expr1" << endl;
        term();
        expr1();
    }
    void expr1()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        if (p < k && wordSyms[p] == 40) //+
        {
            cout << "expr1--> + term expr1" << endl;
            p++;
            term();
            expr1();
        }
        else if (p < k && wordSyms[p] == 41) //-
        {
            cout << "expr1--> - term expr1" << endl;
            p++;
            term();
            expr1();
        }
        else
        {
            cout << "expr1-->null" << endl;
            return;
        }
    }
    void term()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        cout << "term-->factor term1" << endl;
        factor();
        term1();
    }
    void term1()
    {
        if (errorFind == 1 || p == k)
        {
            return;
        }
        if (p < k && wordSyms[p] == 42) // *
        {
            cout << "term1-->*factor term1" << endl;
            p++;
            factor();
            term1();
        }
        else if (p < k && wordSyms[p] == 39) //  /
        {
            cout << "term1-->/factor term1" << endl;
            p++;
            factor();
            term1();
        }
        else
        {
            cout << "trem1-->null" << endl;
            return;
        }
    }
    void factor()
    {
    	cout<<endl<<"factor "<<word[p]<<endl;
        if (errorFind == 1 || p == k)
        {
            return;
        }
        if (p < k && wordSyms[p] == 51) //(
        {
            p++;
            expr();
            if (p < k && wordSyms[p] != 52) //)
            {
                errorFind = 1;
                return;
            }
            p++;
        }
        else if (p < k && wordSyms[p] == 42)
        {
            p++;
            cout << "factor-->id" << endl;
            return;
        }
        else if (p < k && wordSyms[p] == 22)
        {
            p++;
            cout << "factor-->num" << endl;
            return;
        }
        else
        {
            errorFind = 1;
            return;
        }
    }
