#include<iostream>
#include<vector>
#include<string.h>
#include<stack>
#include<stdlib.h>
#include<time.h>
using namespace std;


class QuestionBank{
	protected:
		/**
		 * var
		 **/
		static const int maxn = 1e5;//length of expression
		static const int questions = 10; // nums of questions
		int remain,len;
		bool negative,decimal,bracket;
		/**
		 * method
		 **/
		int icp(char a); // in-stack priority
		int isp(char a);
		void change(string,int[]); // infix expression to suffix expression
	public:
		QuestionBank();
		~QuestionBank();
		/**
		 * set attr
		 **/
		void set_remain(int);
		void set_len(int);
		void set_negative(bool);
		void set_decimal(bool);
		void set_bracket(bool);
		/**
		 * main public method
		 **/
		string generate(unsigned int seed = rand(),int flag = 0); 
		int calculate(string); // calculate suffix expression	
}; 

extern "C"{
	/**
	 * c api for py
	 **/
	QuestionBank a;

	char* py_test(char *s){
		return s;
	}

	void py_set_remain(int x){
		a.set_remain(x);
	}

	void py_set_len(int x){
		a.set_len(x);
	}

	void py_set_negative(int x){
		if(x)a.set_negative(true);
		else a.set_negative(false);
	}

	void py_set_decimal(int x){
		if(x)a.set_decimal(true);
		else a.set_decimal(false);
	}

	void py_set_bracket(int x){
		if(x)a.set_bracket(true);
		else a.set_bracket(false);
	}

	const char* py_generate(){
		string s = a.generate();
		char *p = new char[20];
		for(int i=0;i<s.length();i++)p[i] = s[i];
		p[s.length()] = '\x00';
		return p;
    }

	int py_calculate(char *s){
		return a.calculate(s);
	}
}

void test(){
	/**
	 * debug mode!!
	 **/
	QuestionBank b;
	string s = b.generate();
	cout<<s<<endl;
	cout<<b.calculate("1*0*8")<<endl;
	cout<<s<<endl;
}

int main(){
	cout<<"**********************"<<endl;
	cout<<"**Designed By HeFeng**"<<endl;
	cout<<"**********************"<<endl;
	test();
	system("pause");
	return 0;
}

QuestionBank::QuestionBank():remain(2),len(10),negative(false),decimal(false),bracket(false){
	// constructor
}

QuestionBank::~QuestionBank(){
	// destructor
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
		default:return -1;
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
		default:return -1;
	}
}

void QuestionBank::change(string infix_expression,int suffix_expression[]){
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
		else if(ch==')')
			for(temp=s.top(),s.pop();temp!='(';temp=s.top(),s.pop())
				suffix_expression[j++] = temp;
		else{
			for(temp=s.top(),s.pop();icp(ch)<=isp(temp);temp=s.top(),s.pop())
				suffix_expression[j++] = temp;
			s.push(temp);
			s.push((char)ch);
		}
	}
	while(!s.empty()){
		suffix_expression[j++] = s.top();
		s.pop();	
	}
}

void QuestionBank::set_remain(int x){
	remain = x;
}

void QuestionBank::set_len(int x){
	len = x;
}

void QuestionBank::set_negative(bool x){
	negative = x;
}

void QuestionBank::set_decimal(bool x){
	decimal = x;
}

void QuestionBank::set_bracket(bool x){
	bracket = x;
}

string QuestionBank::generate(unsigned int seed,int flag){
	string res;
	res.clear();
	srand(seed);
	remain--;
	if(flag && bracket)res += '(';
	if(remain<=1){
		char tmp[10];
		itoa(rand()%len,tmp,10);
		res += tmp;
	}else{
		res += generate(seed*seed+time(0),rand()%2);
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
		char tmp[10];
		itoa(rand()%len,tmp,10);
		res += tmp;
	}else{
		res += generate(seed*seed*seed+time(0),rand()%2);
	}
	if(flag && bracket)res += ')';
	return res;
}

int QuestionBank::calculate(string infix_expression){
	int suffix_expression[maxn];
	infix_expression.append(1,'#');
	change(infix_expression,suffix_expression);
	stack<int>s;
	int j=0;
	while(true){
		int ch=suffix_expression[j++];
		if(ch=='#')break;
		if(ch!='+' && ch!='-' && ch!='*' && ch!='/')
			s.push(ch);
		else{
			int t1,t2;
			t1 = s.top();
			s.pop();
			t2 = s.top();
			s.pop();
			switch(ch){
				case '*':s.push(t2*t1);
						break;
				case '/':s.push(t2/t1);
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