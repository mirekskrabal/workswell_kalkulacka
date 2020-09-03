#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QObject>

class exprParser : public QObject
{
    Q_OBJECT
public:
    exprParser(){};
    virtual ~exprParser() {}
    Q_INVOKABLE QString calculate(QString expr);
private:
    void tokenize();
    void strToArr(std::string &raw);
    std::string parseFunc(std::string &func);
    std::vector<char> charArr;
    std::vector<std::string> tokenized;
};

#endif // EXPRPARSER_H
