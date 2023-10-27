#ifndef CALCULATORWINDOW_H
#define CALCULATORWINDOW_H

#include <QWidget>
#include <QResizeEvent>

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
    std::string expression;
    void buttonConnection();
    void addExpression(char c);
    void lineEditFontSizeCheck();
    void resizeEvent(QResizeEvent *event);
    void easyInputCheck();

public:
    CalculatorWindow(QWidget *parent = nullptr);
    ~CalculatorWindow();
    void buttonClickEqual();
    void buttonClickInput(char c);
    void keyboardInput();

private:
    Ui::CalculatorWindow *ui;
};
#endif // CALCULATORWINDOW_H
