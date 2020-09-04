#include "exprparser.h"
#include <math.h>


QString ExprParser::calculate(QString expr)
{
    strToArr(expr.toStdString());
    tokenize();
    infToPostf();
    //clear for next calculation
    charArr.clear();
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
        ++token; //by this the "previous" char was already checked
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
        if (!isspace(*i)){
            charArr.push_back(*i);
        }
    }
}

void ExprParser::infToPostf()
{
    std::stack<std::string> tmp;
    for (auto &i : tokenized) {
        if (isdigit(i[0]) || i == "m") {
            rpnVec.push_back(i);
        }
        else if (i == "sin" || i == "cos" || i == "tg" || i == "cotg" ||
            i == "exp" || i == "log" || i[0] == 'm') {
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
    //unary minus flag - once one expression will be calculated
    int unMin = 1;
    int unMin2 = 1;
    for (auto token = rpnVec.begin(); token != rpnVec.end(); ++token){
        if (isdigit((*token)[0])){
            nums.push(stod(*token));
        }
        else {
            if ( *token == "+" || *token == "-" || *token == "/" || *token == "*"){
                num1 = nums.top() * unMin2;
                nums.pop();
                num2 = nums.top() * unMin;
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
                else { // operator '/'
                    res = num2 / num1;
                }
            }
            else if (*token == "m"){
                //set flag for unary minus
                if (unMin == 1){
                    unMin = -1;
                    continue;
                }
                if (unMin == -1){
                    unMin2 = -1;
                    continue;
                }
//                ++token;
//                res = -stod(*token);
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
                res *= unMin;
            }
            nums.push(res);
            unMin = 1;
            unMin2 = 1;
        }
    }
    res = nums.top();
    rpnVec.clear();
    return QString::number(res);
}




