#ifndef EXPRPARSER_H
#define EXPRPARSER_H

#include <QObject>
#include <stack>

class ExprParser : public QObject
{
    Q_OBJECT
public:
    ExprParser(): lastRes("Nan"){};
    virtual ~ExprParser() {}
    //invokes all function calls needed to evaluate given expression
    Q_INVOKABLE QString calculate(QString expr);
    Q_PROPERTY(QString res READ getRes);
    QString getRes() const {return lastRes;}
private:
    //tokenizes input
    void tokenize();
    //transforms input string to array and removes all ws
    void strToArr(std::string &&raw);
    //parses tokenized input to rpn using shunting yard algorithm
    void infToPostf();
    //evaluates postfix expression stored in rpnOutput
    QString evaluatePostfExpr();

    //input divided into chars
    std::vector<char> charArr;
    //tokenized input
    std::vector<std::string> tokenized;
    //tokenized input in reversed polish notation
    std::vector<std::string> rpnVec;
    //stores last calculated result
    QString lastRes;
};

#endif // EXPRPARSER_H
