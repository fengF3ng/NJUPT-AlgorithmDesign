#include<iostream>
#include<vector>
#include<string.h>
#include<stack>
#include<stdlib.h>
#include<time.h>
using namespace std;

const int maxn = 1e5;//length of expression
class QuestionBank{
	protected:
		/***
		var
		***/
		int questions = 10;//nums of questions
		int remain = 10;
		int len = 3;
		bool negative = false;
		bool decimal = false;
		bool bracket = true;
		string infix_expression;
		int suffix_expression[maxn];
		/***
		method
		***/
		int icp(char a);//in-stack priority
		int isp(char a);
		void change();//infix expression to suffix expression
		int calculate();//calculate suffix expression
		string generate(unsigned int seed,int flag = 0); 
	public:
		QuestionBank();
		~QuestionBank();
		/*
		void set_remain();
		void set_len();
		void set_negative();
		void set_decimal();
		void set bracket();*/
		void get_questions();
		
		
}; 
int main(){
	QuestionBank a;
	a.get_questions();
}

QuestionBank::QuestionBank(){
	infix_expression.clear();
	memset(suffix_expression,0,sizeof(suffix_expression));
}
QuestionBank::~QuestionBank(){
	int a;
}

void QuestionBank::get_questions(){
	int tmp = remain;
	infix_expression = generate(time(0));
	infix_expression.append(1,'#');
	remain = tmp;
	cout<<infix_expression<<endl;
	change();
	cout<<"?"<<endl;
	cout<<calculate();
}
int QuestionBank::icp(char a){
	switch(a){
		case '#':return 0;
		case '(':return 12;
		//case '!':return 10;
		case '*':return 8;
		case '/':return 6;
		case '+':return 4;
		case '-':return 2;
		case ')':return 1;
	}
}
int QuestionBank::isp(char a){
	switch(a){
		case '#':return 0;
		case '(':return 1;
		//case '!':return 11;
		case '*':return 9;
		case '/':return 7;
		case '+':return 5;
		case '-':return 3;
		case ')':return 12;
	}
}
void QuestionBank::change(){
	int i=0,j=0;
	int ch;
	char temp;
	stack<char>s;
	s.push('#');
	while(infix_expression[i]!='#'){
		ch=infix_expression[i++];
		if('0'<=ch && ch<='9'){ // operand
			int tnum = 0;
			while('0'<=ch && ch<='9'){
				tnum = 10*tnum+ch-'0';
				ch = infix_expression[i++];
			}
			i--;
			suffix_expression[j++] = tnum;
		}
		else if(ch==')')//遇到右括号弹出栈内元素直到遇到 ( 
			for(temp=s.top(),s.pop();temp!='(';temp=s.top(),s.pop())
				suffix_expression[j++] = temp;
		else{//从栈中取出所有比当前操作符优先级高的 
			for(temp=s.top(),s.pop();icp(ch)<=isp(temp);temp=s.top(),s.pop())
				suffix_expression[j++] = temp;
			s.push(temp);
			s.push((char)ch);
		}
	}
	while(!s.empty()){//将剩余部分弹出 
		suffix_expression[j++] = s.top();
		s.pop();	
	}
}
int QuestionBank::calculate(){
	stack<int>s;
	int j=0;
	while(true){
		int ch=suffix_expression[j++];
		if(ch=='#'||ch==0)break;
		if(ch!='+' && ch!='-' && ch!='*' && ch!='/')//中缀表达式中的变元值入栈 
			s.push(ch);
		else{//遇到操作符进行计算 
			int t1,t2;//左右操作数 
			t1 = s.top();
			s.pop();
			t2 = s.top();
			s.pop();
			switch(ch){
				case '*':s.push(t2*t1);//合取运算 
						break;
				case '/':s.push(t2/t1);//析取运算 
						break;
				case '+':s.push(t2+t1);
						break;
				case '-':s.push(t2-t1);
						break;
			}
		}
	}
	return s.top();
}
string QuestionBank::generate(unsigned int seed,int flag){
	string res;
	res.clear();
	srand(seed);
	remain--;
	if(flag && bracket)res += '(';
	if(remain<=1){
		string tmp;
		tmp.clear();
		for(int i = len;i;i--){
			char p = rand()%10+'0';
			tmp.append(1,p);
		}
		res += tmp;
	}else{
		res += generate(seed*seed,rand()%2);
	}
	switch(rand()%4){
		case 0:res += '+';
				break;
		case 1:res += '-';
				break;
		case 2:res += '*';
				break;
		case 3:res += '/';
	}
	if(remain<=1){
		string tmp;
		tmp.clear();
		for(int i = len;i;i--){
			char p = rand()%10+'0';
			tmp.append(1,p);
		}
		res += tmp;
	}else{
		res += generate(seed*seed*seed,rand()%2);
	}
	if(flag && bracket)res += ')';
	return res;
}
