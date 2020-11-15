#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <windows.h>
using namespace std;

class QuestionBank {
  protected:
    /**
     * var
     **/
    static const int maxn = 1e2;     // length of expression
    int remain, len; // num of oprand and range of number
    bool negative, decimal, bracket; // bool for negative decimal and bracket
    /**
     * method
     **/
    int icp(char a); // out-stack priority
    int isp(char a); // in-stack priority
    void change(string,
                char[maxn][maxn]); // infix expression to suffix expression
    string generate(int, int flag = 0); // generate problems randomize
    bool valid_expression(string); // judge if expression valid

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
    string get_question(); // api for function generate
    double calculate(string); // calculate suffix expression
};

extern "C" {
/**
 * c api for py
 **/
QuestionBank a;

char *py_test(char *s) { return s; }

void py_set_remain(int x) { a.set_remain(x); }

void py_set_len(int x) { a.set_len(x); }

void py_set_negative(int x) {
    if (x)
        a.set_negative(true);
    else
        a.set_negative(false);
}

void py_set_decimal(int x) {
    if (x)
        a.set_decimal(true);
    else
        a.set_decimal(false);
}

void py_set_bracket(int x) {
    if (x)
        a.set_bracket(true);
    else
        a.set_bracket(false);
}

const char *py_generate() {
    string s = a.get_question();
    char *p = new char[100];
    for (int i = 0; i < s.length(); i++)
        p[i] = s[i];
    p[s.length()] = '\x00';
    return p;
}

double py_calculate(char *s) { return a.calculate(s); }
}

void test() {
    /**
     * debug mode
     **/
    QuestionBank b;
    string c = b.get_question();
    cout << c << endl;
    cout << b.calculate("(821+593)*553-39*106") << endl;
}

int main() {
    cout << "**********************" << endl;
    cout << "**Designed By HeFeng**" << endl;
    cout << "**       NJUPT      **" << endl;
    cout << "**     2020-11-7    **" << endl;
    cout << "**********************" << endl;
    test();
    system("pause");
    return 0;
}

QuestionBank::QuestionBank()
    : remain(2), len(10), negative(false), decimal(false), bracket(false) {
    srand(GetCurrentTime());
    /**
     * constructor
     **/
}

QuestionBank::~QuestionBank() {
    /**
     * destructor
     **/
}

int QuestionBank::icp(char a) {
    switch (a) {
    case '#':
        return 0;
    case '(':
        return 12;
    case '*':
        return 8;
    case '/':
        return 6;
    case '+':
        return 4;
    case '-':
        return 2;
    case ')':
        return 1;
    default:
        return -1;
    }
}

int QuestionBank::isp(char a) {
    switch (a) {
    case '#':
        return 0;
    case '(':
        return 1;
    case '*':
        return 9;
    case '/':
        return 7;
    case '+':
        return 5;
    case '-':
        return 3;
    case ')':
        return 12;
    default:
        return -1;
    }
}

void QuestionBank::change(string infix_expression,
                          char suffix_expression[maxn][maxn]) {
    /**
     * turn infix expression to suffix expression
     **/
    int i = 0, j = 0;
    int ch;
    char temp;
    stack<char> s;
    s.push('#'); // push end-symbol
    while (infix_expression[i] != '#') {
        ch = infix_expression[i++];
        if ('0' <= ch && ch <= '9') { // operand
            int k = 1;
            suffix_expression[j][0] = 'd'; // mark the attribute of charecterize
            while (('0' <= ch && ch <= '9') || ch == '.') { // save all number
                suffix_expression[j][k++] = ch;
                ch = infix_expression[i++];
            }
            suffix_expression[j][k] = '\x00';
            i--;
            j++;
        } else if (ch == ')') // when meet ')' will pop stack until meet '('
            for (temp = s.top(), s.pop(); temp != '(';
                 temp = s.top(), s.pop()) {
                suffix_expression[j][0] = 'o'; // mark the attribute of charecterize
                suffix_expression[j][1] = temp;
                j++;
            }
        else {
            for (temp = s.top(), s.pop(); icp(ch) <= isp(temp);
                 temp = s.top(), s.pop()) { // pop all operator which has lower priority than current operator
                suffix_expression[j][0] = 'o'; // mark the attribute of charecterize
                suffix_expression[j][1] = temp;
                j++;
            }
            s.push(temp);
            s.push((char)ch);
        }
    }
    while (!s.empty()) { // pop all operator left
        suffix_expression[j][0] = 'o';
        suffix_expression[j][1] = s.top();
        j++;
        s.pop();
    }
}

string QuestionBank::generate(int oprand, int flag) {
    /**
     * generate expression randomize
     **/
    string res; // save portion of expression
    res.clear();
    if (flag && bracket) // if set bracket flag and random permit
        res += '(';
    if (oprand <= 2 || rand() % 2) {
        char tmp[10];
        if (rand() % 2 && decimal) {
            itoa(rand() % len, tmp, 10);
            itoa(rand() % 10, tmp + strlen(tmp) + 1, 10);
            tmp[strlen(tmp)] = '.';
        } else {
            itoa(rand() % len, tmp, 10);
        }
        res += tmp;
        oprand--; // remain oprand subtract one
    } else { // if remain oprand suffice and random permit
        res += generate(oprand - 1, rand() % 2); // use self function to get a sub expression
        oprand = 1;
    }
    switch (rand() % 4) { // get operator randomize
    case 0:
        res += '+';
        break;
    case 1:
        res += '-';
        break;
    case 2:
        res += '*';
        break;
    case 3:
        res += '/';
    }
    if (oprand <= 1) { // if num of oprand insuffice then get one oprand
        char tmp[10];
        if (rand() % 2 && decimal) {
            itoa(rand() % len, tmp, 10);
            itoa(rand() % 10, tmp + strlen(tmp) + 1, 10);
            tmp[strlen(tmp)] = '.';
        } else {
            itoa(rand() % len, tmp, 10);
        }
        res += tmp;
    } else { // if remain oprand too much then get all of them by self function
        res += generate(oprand, rand() % 2);
    }
    if (flag && bracket)
        res += ')';
    return res;
}

bool QuestionBank::valid_expression(string infix_expression) {
    /**
     * judge if expression valid
     **/
    char suffix_expression[maxn][maxn];
    infix_expression.append(1, '#');
    change(infix_expression, suffix_expression);
    stack<double> s;
    int j = 0;
    while (true) {
        if (suffix_expression[j][0] == 'o' && suffix_expression[j][1] == '#') // end-symbol makes while end
            break;
        if (suffix_expression[j][0] == 'd') { // push oprand for the calculating after
            double tmp = 0;
            for (int k = 1; suffix_expression[j][k]; k++) {
                if (suffix_expression[j][k] == '.') {
                    tmp += (suffix_expression[j][k + 1] - '0') * 0.1;
                    break;
                } else
                    tmp = suffix_expression[j][k] - '0' + tmp * 10;
            }
            s.push(tmp);
        } else { // when get operator then pop two oprand for calculating
            double t1, t2;
            t1 = s.top();
            s.pop();
            t2 = s.top();
            s.pop();
            switch (suffix_expression[j][1]) {
            case '*':
                s.push(t2 * t1);
                break;
            case '/': // after the '/' cant be zero and if user asked for int,the '/' will get a int type
                if (t1 == 0 || (!decimal && int(t2 / t1) != t2 / t1))
                    return false;
                s.push(t2 / t1);
                break;
            case '+':
                s.push(t2 + t1);
                break;
            case '-': // if user asked for positive then every a-b will be postive
                if (!negative && t2 - t1 < 0)
                    return false;
                s.push(t2 - t1);
                break;
            }
        }
        j++;
    }
    return true;
}

void QuestionBank::set_remain(int x) { remain = x; }

void QuestionBank::set_len(int x) { len = x; }

void QuestionBank::set_negative(bool x) { negative = x; }

void QuestionBank::set_decimal(bool x) { decimal = x; }

void QuestionBank::set_bracket(bool x) { bracket = x; }

string QuestionBank::get_question() {
    /**
     * api for generate
     **/
    string res;
    while (true) {
        res = generate(remain); // get a expression
        if (!valid_expression(res)) // judge if the expression meet the requestion
            continue;
        break;
    }

    return res;
}
double QuestionBank::calculate(string infix_expression) {
    /**
     * calculate expression
     **/
    char suffix_expression[maxn][maxn];
    infix_expression.append(1, '#');
    change(infix_expression, suffix_expression); // turn infix expression to suffix expression
    stack<double> s;
    int j = 0;
    while (true) {
        if (suffix_expression[j][0] == 'o' && suffix_expression[j][1] == '#') // end-symbol makes while end
            break;
        if (suffix_expression[j][0] == 'd') { // push oprand for the calculating after
            double tmp = 0;
            for (int k = 1; suffix_expression[j][k]; k++) { // turn characterize to int or double
                if (suffix_expression[j][k] == '.') { 
                    tmp += (suffix_expression[j][k + 1] - '0') * 0.1;
                    break;
                } else
                    tmp = suffix_expression[j][k] - '0' + tmp * 10;
            }
            s.push(tmp);
        } else { // when get operator then pop two oprand for calculating
            double t1, t2;
            t1 = s.top();
            s.pop();
            t2 = s.top();
            s.pop();
            switch (suffix_expression[j][1]) { // use different operator by ch
            case '*':
                s.push(t2 * t1);
                break;
            case '/':
                s.push(t2 / t1);
                break;
            case '+':
                s.push(t2 + t1);
                break;
            case '-':
                s.push(t2 - t1);
                break;
            }
        }
        j++;
    }
    return s.top();
}