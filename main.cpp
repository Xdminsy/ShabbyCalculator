#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

enum class Operator{
    PLUS, MINUS, STAR, SLASH
};

double operate(Operator op, double op1, double op2){
    double result = 0;
    switch(op){
        case Operator::PLUS:
            result = op1 + op2;
            break;
        case Operator::MINUS:
            result = op1 - op2;
            break;
        case Operator::STAR:
            result = op1 * op2;
            break;
        case Operator::SLASH:
            result = op1 / op2;
            break;
        default:
            throw "What the fscking operator is.";
    }
    return result;
}

int precedence(Operator op){
    int result = 0;
    switch(op){
        case Operator::PLUS:
        case Operator::MINUS:
            result = 4;
            break;
        case Operator::STAR:
        case Operator::SLASH:
            result = 3;
            break;
        default:
            result = -1;
    }
    return result;
}

inline bool isOperator(char c){
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int findMatch(const char* exp, int len){
    int count = -1, pos = 0;
    while(pos < len){
        char c = exp[pos];
        if(c == '('){
            ++count;
        }else if(c == ')'){
            if(!count) return pos;
            else --count;
        }
        ++pos;
    }
    return -1;
}

template <typename T> inline T pop(stack<T>& s){
    T temp = s.top();
    s.pop();
    return temp;
}

double eval(const char* exp, size_t len){
    // cout << "eval: " << exp  << " " << len << endl;
    stack<double> vs;
    stack<Operator> ops;
    Operator op;
    bool numing = false;
    double result = 0;
    size_t pos = 0;
    while(pos < len){
        char c = exp[pos];
        if(numing){
            if(!isdigit(c) || c != '.'){
                numing = false;
            }
        }else if(isdigit(c) || c == '.'){
            numing = true;
            sscanf(exp + pos, "%lf", &result);
            vs.push(result);
        }else if(c == '('){
            int match = findMatch(exp + pos, len - pos);
            if(match == -1) throw "Parentheses not match";
            result = eval(exp + pos + 1, match);
            vs.push(result);
            pos = pos + match + 1;
        }else if(c == ')') throw "Parentheses not match";
        else if(isOperator(c)){
            if(c == '+'){
                op = Operator::PLUS;
            }else if(c == '-'){
                op = Operator::MINUS;
            }else if(c == '*'){
                op = Operator::STAR;
            }else if(c == '/'){
                op = Operator::SLASH;
            }
            while(!ops.empty() && precedence(op) >= precedence(ops.top())){
                double op1, op2;
                op2 = pop(vs);
                op1 = pop(vs);
                result = operate(pop(ops), op1, op2);
                vs.push(result);
            }
            ops.push(op);
        }else if(c == ' '){
        }else{
            throw "Meet illegal character";
        }
        ++pos;
    }
    vs.empty() || (result = vs.top());
    while(!vs.empty() && !ops.empty()){
        double op1, op2;
        op2 = pop(vs);
        op1 = pop(vs);
        vs.push(result = operate(pop(ops), op1, op2));
    }
    return result;
}

int main(){
    string s;
    double value;
    while(getline(cin, s)){
        try{
            value = eval(s.c_str(), s.length());
        }catch(char const* s){
            perror(s);
        }
        cout << "Result: " << value << endl;
    }
}

