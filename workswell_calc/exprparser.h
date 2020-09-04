#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QObject>
#include <stack>

class ExprParser : public QObject
{
    Q_OBJECT
public:
    ExprParser(){};
    virtual ~ExprParser() {}
    Q_INVOKABLE QString calculate(QString expr);
private:
    //tokenizes input
    void tokenize();
    //transforms input string to array and removes all ws
    void strToArr(std::string &&raw);
    //parses tokenized input to rpn using shunting yard algorithm
    void infToPostf();
    //evaluates postfix expression stored in rpnOutput
    QString evaluatePostfExpr();

    std::vector<char> charArr;
    std::vector<std::string> tokenized;
    std::vector<std::string> rpnVec;
};

#endif // EXPRPARSER_H
