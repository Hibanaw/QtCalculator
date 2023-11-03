#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

#include <QWidget>
#include <QResizeEvent>
#include "SciNum.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CalculatorWindow; }
QT_END_NAMESPACE

class CalculatorWindow : public QWidget
{
    Q_OBJECT
private:
    static constexpr char operatorChar[] = {
        '+', '-', '*', '/', '^'
    };
    SciNum preAns;
    bool doPreAnsExist;
    std::string expression;
    void buttonConnection();
    void addExpression(char c);
    void labelFontSizeCheck();
    void resizeEvent(QResizeEvent *event);
    void easyInputCheck();

public:
    CalculatorWindow(QWidget *parent = nullptr);
    ~CalculatorWindow();
    void buttonClickEqual();
    void buttonClickInput(char c);
    void buttonClickInput(const char* str);
    void buttonClickBackspace();
    void buttonClickAllClear();

private:
    Ui::CalculatorWindow *ui;
};
#endif // CALCULATORWINDOW_H
