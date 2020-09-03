#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QObject>
#include <stack>

class exprParser : public QObject
{
    Q_OBJECT
public:
    exprParser(){};
    virtual ~exprParser() {}
    Q_INVOKABLE QString calculate(QString expr);
private:
    //tokenizes input
    void tokenize();
    //transforms input string to array and removes all ws
    void strToArr(std::string &&raw);
    //parses tokenized input to rpn using shunting yard algorithm
    void infToPostf();
    std::vector<char> charArr;
    std::vector<std::string> tokenized;
    std::stack<std::string> rpnOutput;
};

#endif // EXPRPARSER_H
