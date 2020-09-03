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
};

#endif // EXPRPARSER_H
