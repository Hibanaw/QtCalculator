#include "calculatorwindow.h"
#include "./ui_calculatorwindow.h"

CalculatorWindow::CalculatorWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CalculatorWindow)
{
    ui->setupUi(this);
    buttonConnection();
}

void CalculatorWindow::buttonConnection(){
    connect(ui->equalButton,
            &QPushButton::clicked,
            this,
            &CalculatorWindow::buttonClickEqual);
    connect(ui->numInputButton_0,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('0');});
    connect(ui->numInputButton_1,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('1');});
    connect(ui->numInputButton_2,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('2');});
    connect(ui->numInputButton_3,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('3');});
    connect(ui->numInputButton_4,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('4');});
    connect(ui->numInputButton_5,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('5');});
    connect(ui->numInputButton_6,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('6');});
    connect(ui->numInputButton_7,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('7');});
    connect(ui->numInputButton_8,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('8');});
    connect(ui->numInputButton_9,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('9');});
    connect(ui->dotInputButton,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('.');});
    connect(ui->opInputButtonPlus,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('+');});
    connect(ui->opInputButtonMinus,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('-');});
    connect(ui->opInputButtonMul,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('*');});
    connect(ui->opInputButtonDiv,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('/');});
    connect(ui->opInputButtonPow,
            &QPushButton::clicked,
            this,
            [=]()
            {buttonClickInput('^');});
    connect(ui->backspaceButton,
            &QPushButton::clicked,
            this,
            &CalculatorWindow::buttonClickBackspace);
}

CalculatorWindow::~CalculatorWindow()
{
    delete ui;
}


void CalculatorWindow::buttonClickEqual(){
    qDebug() << "Button Click Equal" ;
    qDebug() << this->ui->label->text();
}


void CalculatorWindow::buttonClickInput(char c){
    qDebug() << "Button Click " << c ;
    expression = ui->label->text().toStdString();
    expression += c;
    this->ui->label->setText(expression.c_str());
    easyInputCheck();
}

void CalculatorWindow::labelFontSizeCheck(){
    int height = this->ui->label->height();
    QFont f(this->ui->label->font());
    if(height < 50){
        f.setPixelSize(18);
        this->ui->label->setFont(f);
    }
    else{
        f.setPixelSize(28);
        this->ui->label->setFont(f);
    }
}

void CalculatorWindow::easyInputCheck(){
    bool isL1Op(false), isL2Op(false);
    if(expression.size() < 2) return;
    char l1, l2;
    l1 = expression[expression.size()-1];
    l2 = expression[expression.size()-2];
    for(auto i(0); i < sizeof(operatorChar); i++){
        if(l1 == operatorChar[i]){
            isL1Op = true;
        }
        if(l2 == operatorChar[i]){
            isL2Op = true;
        }
    }
    if(isL1Op && isL2Op){
        expression.pop_back();
        expression.pop_back();
        expression.push_back(l1);
        ui->label->setText(expression.c_str());
    }
}

void CalculatorWindow::resizeEvent(QResizeEvent *event){
    labelFontSizeCheck();
}


void CalculatorWindow::buttonClickBackspace(){
    if(expression.empty()) return;
    expression.pop_back();
    ui->label->setText(expression.c_str());
}
