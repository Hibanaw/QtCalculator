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

}

CalculatorWindow::~CalculatorWindow()
{
    delete ui;
}


void CalculatorWindow::buttonClickEqual(){
    qDebug() << "Button Click Equal" ;
//    this->ui->label->
    qDebug() << this->ui->lineEdit->text();
}


void CalculatorWindow::buttonClickInput(char c){
    qDebug() << "Button Click " << c ;
    std::string exp = this->ui->lineEdit->text().toStdString();
    exp += c;
    QString qexp(exp.c_str());
    this->ui->lineEdit->setText(qexp);
}

void CalculatorWindow::lineEditFontSizeCheck(){
    int height = this->ui->lineEdit->height();
    QFont f(this->ui->lineEdit->font());
    if(height < 50){
        f.setPixelSize(12);
        this->ui->lineEdit->setFont(f);
    }
    else{
        f.setPixelSize(28);
        this->ui->lineEdit->setFont(f);
    }
}

void CalculatorWindow::resizeEvent(QResizeEvent *event){
    lineEditFontSizeCheck();
}
