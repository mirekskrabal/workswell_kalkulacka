#include "exprparser.h"
#include <math.h>
#include <QDebug>

QString ExprParser::calculate(QString expr)
{
    prevExpr = expr;
    strToArr(expr.toStdString());
    tokenize();
    try {
         infToPostf();
         evaluatePostfExpr();
    }  catch (const std::invalid_argument &ia) {
        clear();
        invalid = true;
        return ia.what();
    }
    //clear for next calculation
    clear();
    invalid = false; //no invalid argument was encountered
    return expr + " = " + QString::number(lastRes) + '\n';

}

void ExprParser::tokenize()
{
    std::string word;
    std::string num;
    std::string tmp;
    auto i = charArr.begin();
    while (i != charArr.end()) {
        //multidigit number
        while (i != charArr.end() && (isdigit(*i) || *i == '.')) {
            num += *i;
            ++i;
        }
        if (num.length() != 0) {
            tokenized.push_back(num);
            num = "";
        }
        //function was found
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
    ++token;
    for (; token != tokenized.end(); ++token){
        if (!isdigit((*(token - 1))[0]) && *(token -1) != ")" &&
            *token == "-"){
            *token = "m";
        }
    }
}

void ExprParser::strToArr(std::string &&raw)
{
    for (auto i = raw.begin(); i != raw.end(); ++i) {
        //skip ws
        if (!isspace(*i)){
            if(*i == ',')
                *i = '.';
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
            i == "exp" || i == "log" || i[0] == 'm' || i[0] == '^') {
            tmp.push(i);
        }
        else if (i[0] != '(' && i[0] != ')'){
            if (i[0] == '*' || i[0] == '/') {
                //pop all operators will higher or equal precedence
                while (!tmp.empty() && (tmp.top() == "*" || tmp.top() == "/" || tmp.top() == "m"
                                        || tmp.top() == "^")) {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
                tmp.push(i);
            }
            //pop all operators will higher or equal precedence
            else if (i[0] == '+' || i[0] == '-') {
                while (!tmp.empty() && (tmp.top() == "+" || tmp.top() == "-" ||
                                        tmp.top() == "*" || tmp.top() == "/" |    //unary minus flag - once one expression will be calculated|
                                        tmp.top() == "m" || tmp.top() == "^")) {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
                tmp.push(i);
            }
            else { //invalid input encountered
                throw std::invalid_argument("Invalid argument!\n");
            }
        }
        else {
            if (i[0] == '('){
                tmp.push(i);
            }
            else if (i[0] == ')') {
                while (!tmp.empty() && tmp.top() != "(") {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
                if (tmp.empty()) //parenthesis mismatch
                    throw std::invalid_argument("Parentheses mismatch!\n");
                tmp.pop();
                std::string x = "";
                if (!tmp.empty())
                    x = tmp.top();
                if (x ==  "sin" || x == "cos" || x == "tg" || x == "cotg" ||
                    x == "exp" || x == "log" || x == "m") {
                    rpnVec.push_back(tmp.top());
                    tmp.pop();
                }
            }
            else {//invalid input encountered
                throw std::invalid_argument("Invalid argument!\n");
            }
        }
    }
    while (!tmp.empty()) {
        if (tmp.top() == ")" || tmp.top() == "(") //parenthesis mismatch
            throw std::invalid_argument("Parentheses mismatch!\n");
        rpnVec.push_back(tmp.top());
        tmp.pop();
    }
}

void ExprParser::evaluatePostfExpr()
{
    std::stack<double> nums;
    double res = 0; //stores results of suboperations
    double num1, num2; //operands of suboperations
    for (auto token = rpnVec.begin(); token != rpnVec.end(); ++token){
        //operands are pushed to the stack
        if (isdigit((*token)[0])){
            nums.push(stod(*token));
        }
        else {//pop all needed operands for given operation and store the result in res
            if (nums.empty()){
                throw std::invalid_argument("Invalid argument!\n");
            }
            if ( *token == "+" || *token == "-" || *token == "/" || *token == "*" || *token == "^" ){
                num1 = nums.top();
                nums.pop();
                if (nums.empty()){
                    throw std::invalid_argument("Invalid argument!\n");
                }
                num2 = nums.top();
                nums.pop();
                if (*token == "+") {
                    res = num2 + num1;
                }
                else if (*token == "-") {
                    res = num2 - num1;
                }
                else if (*token == "*") {
                    res = num2 * num1;
                }
                else if (*token == "/"){
                    res = num2 / num1;
                }
                else{ //^ operator
                    res = pow(num2, num1);
                }
            }
            else if (*token == "m"){//unary minus ecountered - it immediately follows its operand
                                    //it is needed to multiply the operand on the top by -1
                res = nums.top() * -1;
                nums.pop();
            }
            else {
                num1 = nums.top();
                nums.pop();
                if (*token == "sin"){
                    res = sin(num1);
                }
                else if (*token == "cos"){
                    res = cos(num1);
                }
                else if (*token == "tg"){
                    res = tan(num1);
                }
                else if (*token == "cotg"){
                    res = cos(num1)/sin(num1);
                }
                else { //functin log
                    res = log10(num1);
                }
            }
            nums.push(res);
        }
    }
    if (nums.size() != 1){
        throw std::invalid_argument("Invalid argument!\n");
    }
    res = nums.top();
    lastRes = res;
}

void ExprParser::clear()
{
    charArr.clear();
    tokenized.clear();
    rpnVec.clear();
}




