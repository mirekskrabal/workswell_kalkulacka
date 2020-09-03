#include "exprparser.h"



QString exprParser::calculate(QString expr)
{

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
        while (isalpha(*i) && i != charArr.end()) {
            word += *i;
            ++i;
        }
        if (!isdigit(*i)) {
            tokenized.push_back(std::string(1,*i));
            ++i;
        }
        if (num.length() != 0) {
            tokenized.push_back(num);
            num = "";
        }
        if (word.length() != 0) {
            tokenized.push_back(word);
        }
    }
}

void exprParser::strToArr(std::string &raw){
    for (auto i = raw.begin(); i != raw.end(); ++i) {
        if (!isspace(*i)){
            charArr.push_back(*i);
        }
    }
}

std::string exprParser::parseFunc(std::string &func)
{

}


