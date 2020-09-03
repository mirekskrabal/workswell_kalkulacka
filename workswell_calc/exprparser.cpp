#include "exprparser.h"



QString exprParser::calculate(QString expr)
{
    strToArr(expr.toStdString());
    tokenize();
    infToPostf();
    charArr.clear(); //clear for next
    return expr;
}

void exprParser::tokenize() {
    std::string word;
    std::string num;
    std::string tmp;
    for (auto i = charArr.begin(); i != charArr.end(); ++i) {
        while (isdigit(*i) && i != charArr.end()) {
            num += *i;
            ++i;
        }
        if (num.length() != 0) {
            tokenized.push_back(num);
            num = "";
        }
        while (isalpha(*i) && i != charArr.end()) {
            word += *i;
            ++i;
        }
        if (word.length() != 0) {
            tokenized.push_back(word);
            word = "";
        }
        if (!isdigit(*i)) {
            tokenized.push_back(std::string(1,*i));
        }
    }
}

void exprParser::strToArr(std::string &&raw){
    for (auto i = raw.begin(); i != raw.end(); ++i) {
        if (!isspace(*i)){
            charArr.push_back(*i);
        }
    }
}

void exprParser::infToPostf() {
    std::stack<std::string> tmp;
    for (auto &i : tokenized) {
        if (isdigit(i[0])) {
            rpnOutput.push(i);
        }
        if (i == "sin" || i == "cos" || i == "tg" || i == "cotg" ||
            i == "exp" || i == "log" || i[0] == '-') {
            tmp.push(i);
        }
        if (i[0] != '(' && i[0] != ')'){
            if (i[0] == '*' || i[0] == '/') {
                while (tmp.top() == "+" || tmp.top() == "-" ||
                       tmp.top() == "*" || tmp.top() == "/") {
                    rpnOutput.push(tmp.top());
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
                while (tmp.top() != "(") {
                    rpnOutput.push(tmp.top());
                    tmp.pop();
                }
                tmp.pop();
                auto x = tmp.top();
                if (x ==  "sin" || x == "cos" || x == "tg" || x == "cotg" ||
                    x == "exp" || x == "log") {
                    rpnOutput.push(tmp.top());
                    tmp.pop();
                }
            }
        }
    }
    while (!tmp.empty()) {
        rpnOutput.push(tmp.top());
        tmp.pop();
    }
}




