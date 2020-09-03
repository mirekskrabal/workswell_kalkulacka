#include "exprparser.h"



QString exprParser::calculate(QString expr)
{
    strToArr(expr.toStdString());
    tokenize();
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




