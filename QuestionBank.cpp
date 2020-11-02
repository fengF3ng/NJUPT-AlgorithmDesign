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
		static const int maxn = 1e2;//length of expression
		static const int questions = 10; // nums of questions
		int remain,len;
		bool negative,decimal,bracket;
		/**
		 * method
		 **/
		int icp(char a); // in-stack priority
		int isp(char a);
		void change(string,char[maxn][maxn]); // infix expression to suffix expression
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
		double calculate(string); // calculate suffix expression	
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
	cout<<b.calculate("(1.2+1.3)*2.5")<<endl;
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

void QuestionBank::change(string infix_expression,char suffix_expression[maxn][maxn]){
	int i=0,j=0;
	int ch;
	char temp;
	stack<char>s;
	s.push('#');
	while(infix_expression[i]!='#'){
		ch=infix_expression[i++];
		if('0'<=ch && ch<='9'){ // operand
			int k=1;
			suffix_expression[j][0] = 'd';
			while(('0'<=ch && ch<='9')||ch=='.'){
				suffix_expression[j][k++] = ch;
				ch = infix_expression[i++];
			}
			suffix_expression[j][k] = '\x00';
			i--;
			j++;
		}
		else if(ch==')')
			for(temp=s.top(),s.pop();temp!='(';temp=s.top(),s.pop()){
				suffix_expression[j][0] = 'o';
				suffix_expression[j][1] = temp;
				j++;
			}
		else{
			for(temp=s.top(),s.pop();icp(ch)<=isp(temp);temp=s.top(),s.pop()){
				suffix_expression[j][0] = 'o';
				suffix_expression[j][1] = temp;
				j++;
			}
			s.push(temp);
			s.push((char)ch);
		}
	}
	while(!s.empty()){
		suffix_expression[j][0] = 'o';
		suffix_expression[j][1] = s.top();
		j++;
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
		if(rand()%2 && decimal){
			itoa(rand()%len,tmp,10);
			itoa(rand()%10,tmp+strlen(tmp)+1,10);
			tmp[strlen(tmp)] = '.';
		}else{
			itoa(rand()%len,tmp,10);
		}
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
		if(rand()%2 && decimal){
			itoa(rand()%len,tmp,10);
			itoa(rand()%10,tmp+strlen(tmp)+1,10);
			tmp[strlen(tmp)] = '.';
		}else{
			itoa(rand()%len,tmp,10);
		}
		res += tmp;
	}else{
		string tmp = generate(seed*seed*seed+time(0),rand()%2);
	}
	
	if(flag && bracket)res += ')';
	return res;
}

double QuestionBank::calculate(string infix_expression){
	char suffix_expression[maxn][maxn];
	infix_expression.append(1,'#');
	change(infix_expression,suffix_expression);
	stack<double>s;
	int j=0;
	while(true){
		if(suffix_expression[j][0] == 'o' && suffix_expression[j][1]=='#')break;
		if(suffix_expression[j][0] == 'd'){
			double tmp = 0;
			for(int k = 1;suffix_expression[j][k];k++){
				if(suffix_expression[j][k]=='.'){
					tmp += (suffix_expression[j][k+1]-'0')*0.1;
					break;
				}
				else tmp = suffix_expression[j][k]-'0'+tmp*10;
			}
			cout<<tmp<<endl;
			s.push(tmp);
		}	
		else{
			double t1,t2;
			t1 = s.top();
			s.pop();
			t2 = s.top();
			s.pop();
			switch(suffix_expression[j][1]){
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
		j++;
	}
	return s.top();
}