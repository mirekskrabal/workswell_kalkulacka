#include "exprparser.h"
#include <math.h>


QString ExprParser::calculate(QString expr)
{
    strToArr(expr.toStdString());
    tokenize();
    infToPostf();
    /*
    rpnVec.clear();
    rpnVec.push_back("-");
    rpnVec.push_back("1");
    rpnVec.push_back("-");
    rpnVec.push_back("1");
    rpnVec.push_back("*");
    rpnVec.push_back("-");*/
    charArr.clear(); //clear for next
    tokenized.clear();
    return expr + " = " + evaluatePostfExpr() + '\n';
}

void ExprParser::tokenize()
{
    std::string word;
    std::string num;
    std::string tmp;
    auto i = charArr.begin();
    while (i != charArr.end()) {
        //multidigit number
        while (i != charArr.end() && isdigit(*i)) {
            num += *i;
            ++i;
        }
        if (num.length() != 0) {
            tokenized.push_back(num);
            num = "";
        }
        //function
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
    //find unary minuses and mark them as "m"
    auto token = tokenized.begin();
    if (*token == "-"){
        *token = "m";
        ++token;
    }
    for (; token != tokenized.end(); ++token){
        if (!isdigit((*(token - 1))[0]) && *(token -1) != ")"){
            *token = "m";
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
            rpnVec.push_back(i);
        }
        else if (i == "sin" || i == "cos" || i == "tg" || i == "cotg" ||
            i == "exp" || i == "log" || i[0] == '-') {
            tmp.push(i);
        }
        else if (i[0] != '(' && i[0] != ')'){
            if (i[0] == '*' || i[0] == '/') {
                while (!tmp.empty() && (tmp.top() == "*" || tmp.top() == "/")) {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
                tmp.push(i);
            }
            else if (i[0] == '+' || i[0] == '-') {
                while (!tmp.empty() && (tmp.top() == "+" || tmp.top() == "-" ||
                                        tmp.top() == "*" || tmp.top() == "/")) {
                    rpnVec.push_back(tmp.top());
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
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
                tmp.pop();
                std::string x = "";
                if (!tmp.empty())
                    x = tmp.top();
                if (x ==  "sin" || x == "cos" || x == "tg" || x == "cotg" ||
                    x == "exp" || x == "log") {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
            }
        }
    }
    while (!tmp.empty()) {
        rpnVec.push_back(tmp.top());
        tmp.pop();
    }
}

QString ExprParser::evaluatePostfExpr()
{
    std::stack<double> nums;
    double res = 0;
    double num1, num2;
    for (auto &i : rpnVec){
        if (isdigit(i[0])){
            nums.push(stod(i));
        }
        else {
            if ( i == "+" || i == "-" || i == "/" || i == "*"){
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
                else { // operator '/'
                    res = num2 / num1;
                }
            }
            else {
                num1 = nums.top();
                nums.pop();
                if (i == "sin"){
                    res = sin(num1);
                }
                else if (i == "cos"){
                    res = cos(num1);
                }
                else if (i == "tg"){
                    res = tan(num1);
                }
                else if (i == "cotg"){
                    res = cos(num1)/sin(num1);
                }
                else { //functin log
                    res = log(num1);
                }
            }
            nums.push(res);
        }
    }
    res = nums.top();
    rpnVec.clear();
    return QString::number(res);
}




