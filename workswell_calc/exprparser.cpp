#include "exprparser.h"



QString ExprParser::calculate(QString expr)
{
    strToArr(expr.toStdString());
    tokenize();
    infToPostf();
    charArr.clear(); //clear for next
    tokenized.clear();
    return evaluatePostfExpr() + '\n';
}

void ExprParser::tokenize()
{
    std::string word;
    std::string num;
    std::string tmp;
    auto i = charArr.begin();
    while (i != charArr.end()) {
        while (i != charArr.end() && isdigit(*i)) {
            num += *i;
            ++i;
        }
        if (num.length() != 0) {
            tokenized.push_back(num);
            num = "";
        }
        while (i != charArr.end() && isalpha(*i)) {
            word += *i;
            ++i;
        }
        if (word.length() != 0) {
            tokenized.push_back(word);
            word = "";
        }
        if (i != charArr.end() && !isdigit(*i)) {
            tokenized.push_back(std::string(1,*i));
            ++i;
        }
    }
}

void ExprParser::strToArr(std::string &&raw)
{
    for (auto i = raw.begin(); i != raw.end(); ++i) {
        if (!isspace(*i)){
            charArr.push_back(*i);
        }
    }
}

void ExprParser::infToPostf()
{
    std::stack<std::string> tmp;
    for (auto &i : tokenized) {
        if (isdigit(i[0])) {
            rpnStack.push(i);
        }
        else if (i == "sin" || i == "cos" || i == "tg" || i == "cotg" ||
            i == "exp" || i == "log" || i[0] == '-') {
            tmp.push(i);
        }
        else if (i[0] != '(' && i[0] != ')'){
            if (i[0] == '*' || i[0] == '/') {
                while (!tmp.empty() && (tmp.top() == "*" || tmp.top() == "/")) {
                    rpnStack.push(tmp.top());
                    tmp.pop();
                }
                tmp.push(i);
            }
            else if (i[0] == '+' || i[0] == '-') {
                while (!tmp.empty() && (tmp.top() == "+" || tmp.top() == "-" ||
                                        tmp.top() == "*" || tmp.top() == "/")) {
                    rpnStack.push(tmp.top());
                    tmp.pop();
                }
                tmp.push(i);
            }
        }
        else {
            if (i[0] == '('){
                tmp.push(i);
            }
            else {
                while (!tmp.empty() && tmp.top() != "(") {
                    rpnStack.push(tmp.top());
                    tmp.pop();
                }
                tmp.pop();
                std::string x = "";
                if (!tmp.empty())
                    auto x = tmp.top();
                if (x ==  "sin" || x == "cos" || x == "tg" || x == "cotg" ||
                    x == "exp" || x == "log") {
                    rpnStack.push(tmp.top());
                    tmp.pop();
                }
            }
        }
    }
    while (!tmp.empty()) {
        rpnStack.push(tmp.top());
        tmp.pop();
    }
}

QString ExprParser::evaluatePostfExpr()
{
    stackToVec();
    std::stack<double> nums;
    double res = 0;
    double num1, num2;
    for (auto &i : rpnVec){
        if (isdigit(i[0])){
            nums.push(stod(i));
        }
        else {
            num1 = nums.top();
            nums.pop();
            num2 = nums.top();
            nums.pop();
            if (i == "+") {
                res = num2 + num1;
            }
            else if (i == "-") {
                res = num2 - num1;
            }
            else if (i == "*") {
                res = num2 * num1;
            }
            else if (i == "/") {
                res = num2 / num1;
            }
            nums.push(res);
        }
    }
    res = nums.top();
    return QString::number(res);
}

void ExprParser::stackToVec()
{
    std::stack<std::string> tmp;
    while (!rpnStack.empty()){
        tmp.push(move(rpnStack.top()));
        rpnStack.pop();
    }
    while (!tmp.empty()){
        rpnVec.push_back(move(tmp.top()));
        tmp.pop();
    }
}




