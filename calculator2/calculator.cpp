#include "calculator.h"
#include <QKeyEvent>
#include <QApplication>
#include <math.h>
#include <QScriptEngine>

QString labelvalue; bool expressionn = false; QString value; double previous;
Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
    placeWidget();
    makeConnexions();


}
Calculator::~Calculator()
{
    delete disp;
    delete layout;
    delete buttonsLayout;
}


void Calculator::createWidgets()
{
    //Creating the layouts
    layout = new QVBoxLayout();
    layout->setSpacing(2);

    //grid layout
    buttonsLayout = new QGridLayout;


    //creating the buttons
    for(int i=0; i < 10; i++)
    {
        digits.push_back(new QPushButton(QString::number(i)));
        digits.back()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        digits.back()->resize(sizeHint().width(), sizeHint().height());
    }
    //enter button
    expr = new QRadioButton("Enable Scientific Mode",this);
    enter = new QPushButton("Enter",this);
    enter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    enter->resize(sizeHint().width(), sizeHint().height());

    //reset button

    reset = new QPushButton("C",this);
    clearLast = new QPushButton("CL",this);
    ans = new QPushButton("ANS",this);
    reset->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    reset->resize(sizeHint().width(), sizeHint().height());

    //operatiosn buttons
    operations.push_back(new QPushButton("+"));
    operations.push_back(new QPushButton("-"));
    operations.push_back(new QPushButton("*"));
    operations.push_back(new QPushButton("/"));

    exprOperations.push_back(new QPushButton("sqrt("));
    exprOperations.push_back(new QPushButton("cos("));
    exprOperations.push_back(new QPushButton("sin("));
    exprOperations.push_back(new QPushButton("pow("));
    exprOperations.push_back(new QPushButton("log("));
    exprOperations.push_back(new QPushButton(")"));
    exprOperations.push_back(new QPushButton("("));
    exprOperations.push_back(new QPushButton(","));
    exprOperations.push_back(new QPushButton("."));


    //creating the lcd
    disp = new QLCDNumber(this);
    disp2 = new QLabel(this);
    disp->setDigitCount(8);



}

void Calculator::placeWidget()
{       QFont font = disp2->font();
        font.setPointSize(30);
            disp2->setFont(font);
            disp2->setAlignment(Qt::AlignRight);
            disp2->setFixedHeight(70);
            disp->setFixedHeight(140);
            disp2->setStyleSheet("border: 1px solid black");
                disp2->setScaledContents(true);
    layout->addWidget(disp2);
    layout->addWidget(disp);
    layout->addLayout(buttonsLayout);


    //adding the buttons
    for(int i=1; i <10; i++)
        buttonsLayout->addWidget(digits[i], (i-1)/3, (i-1)%3);


    //Adding the operations
    for(int i=0; i < 4; i++)
        buttonsLayout->addWidget(operations[ i], i, 4);

    for(int i=0; i < 5; i++){
        buttonsLayout->addWidget(exprOperations[ i], i, 5);

}
    for(int i=0; i < 9; i++){
        exprOperations[i]->setEnabled(false);
}

        buttonsLayout->addWidget(exprOperations[ 5], 4, 4);
        buttonsLayout->addWidget(exprOperations[ 6], 4, 3);
        buttonsLayout->addWidget(exprOperations[ 7], 2, 3);
        buttonsLayout->addWidget(clearLast, 3, 3);
        buttonsLayout->addWidget(ans, 0, 3);
            ans->setEnabled(false);
            buttonsLayout->addWidget(exprOperations[8], 1, 3);


    //Adding the 0 button
    buttonsLayout->addWidget(digits[0], 3, 0);
    buttonsLayout->addWidget(enter, 3, 1, 1,1);
    buttonsLayout->addWidget(expr, 4, 0, 1,3);
    buttonsLayout->addWidget(reset, 3, 2, 1,1);
    setLayout(layout);
}

void Calculator::makeConnexions()
{
                for(int i=0; i <10; i++){
                    connect(digits[i], &QPushButton::clicked,
                            this, &Calculator::newDigit);
                    connect(digits[i], &QPushButton::clicked,
                            this, &Calculator::displayLabel);
                }
                for(int i=0; i <4; i++){
                    connect(operations[i], &QPushButton::clicked,
                            this, &Calculator::changeOperation);
                    connect(operations[i], &QPushButton::clicked,
                            this, &Calculator::displayLabel);
                }
                for(int i=0; i < 5; i++){
                    connect(exprOperations[i], &QPushButton::clicked,
                            this, &Calculator::displayExpr);
            }
                for(int i=5; i < 9; i++){
                    connect(exprOperations[i], &QPushButton::clicked,
                            this, &Calculator::displayLabel);
            }

    connect(expr,SIGNAL(clicked()),this,SLOT(scientificOn()));
    connect(enter, &QPushButton::clicked,
            this, &Calculator::showresult);
    connect(ans, &QPushButton::clicked,
            this, &Calculator::displayPrevious);
    connect(clearLast, &QPushButton::clicked,
            this, &Calculator::clearLastchar);
    connect(reset, &QPushButton::clicked,
            this, &Calculator::Reset);
}
void Calculator::Reset(){
    left=nullptr;
    right=nullptr;
    operation=nullptr;
    labelvalue = "";
    disp2->setText("");
    disp->display(0);


}
void Calculator::whichop(){
    if(*op=="+"){
       *left=*left+*right;
    }
    else if(*op=="-"){*left=*left-(*right);
        }
    else if(*op=="*"){*left=(*left)*(*right);
        }
    else if(*op=="/"){ if(*right==0)disp->display("error");else { *left=*left/((*right));
        }}
}
void Calculator::showresult(){
    if(!expr->isChecked()){
            if(*operation=="+"){
                    *left=*left+*right;
                    disp->display(*left); right = new double{0};}
            else if(*operation=="-"){*left=*left-(*right);
                disp->display(*left);right = new double{0};}
            else if(*operation=="*"){*left=(*left)*(*right);
                disp->display(*left);right = new double{1};}
            else if(*operation=="/"){ if(*right==0)disp->display("error");else {*left=*left/((*right));
                disp->display(*left);right = new double{1};}}
     }
        else{
            disp->display(expressionEval(disp2->text()));
        }
    }
void Calculator::changeOperation()
{
    //Getting the sender button
    auto button = dynamic_cast<QPushButton*>(sender());

    //Storing the operation
    operation = new QString{button->text()};
    if(right){
            whichop();
    }
    //Initiating the right button
    right = new double{0};
    op= new QString{button->text()};


    //reseting the display
disp->display(0);
}
void Calculator::newDigit( )
{
    //getting the sender

            auto button = dynamic_cast<QPushButton*>(sender());

            //getting the value
            double value = button->text().toDouble();


            //Check if we have an operation defined
            if(operation)
            {
                //check if we have a value or not



                    *right = 10 * (*right) + value;

    if(!expr->isChecked()){

        disp->display(*right);}


            }
            else
            {
                if(!left)
                    left = new double{value};
                else
                    *left = 10 * (*left) + value;
if(!expr->isChecked()){
                disp->display(*left);}

            }
}
void Calculator::displayLabel(){
    setFontSizeDyn(disp2);
    auto button = dynamic_cast<QPushButton*>(sender());

    //getting the value
    labelvalue += button->text();
    disp2->setText(labelvalue);

}
void Calculator::setFontSizeDyn(QLabel *a){
    QFont font = a->font();

        QFontMetrics fm(font);
        QRect bound = fm.boundingRect(0,0, a->width(), a->height(), Qt::TextWordWrap | Qt::AlignLeft, a->text());

        if (bound.width() <= a->width() && font.pointSize()>15 && a->height()> 50){
            font.setPointSize(font.pointSize()-1);
            a->setFont(font);
            a->setFixedHeight(a->height()-2);
        }

}
double Calculator::expressionEval(QString expression_string){
    double value;
    QScriptEngine expression1;
   value=expression1.evaluate(expression_string).toNumber();
   previous = value;
   return value;
}
void Calculator::displayPrevious(){
    labelvalue+=QString::number(previous);
    disp2->setText(labelvalue);
}
void Calculator::scientificOn(){
    if(expr->isChecked()){
            for(int i=0; i < 9; i++){
                exprOperations[i]->setEnabled(true);
        }
            ans->setEnabled(true);
    }
   else{
                for(int i=0; i < 9; i++){
                    exprOperations[i]->setEnabled(false);
            }
                ans->setEnabled(false);
    }
}
void Calculator::displayExpr(){
    auto button = dynamic_cast<QPushButton*>(sender());

    //getting the value
    QString value = "Math.";
    value += button->text();
    labelvalue += value;
    disp2->setText(labelvalue);
    setFontSizeDyn(disp2);
}
void Calculator::clearLastchar(){
    labelvalue.chop(1);
    disp2->setText(labelvalue);
}
void Calculator::keyPressEvent(QKeyEvent *e)
{
    //Exiting the application by a click on space
    if( e->key() == Qt::Key_Escape)
        qApp->exit(0);

     labelvalue += e->text();
     disp2->setText(labelvalue);

    //You could add more keyboard interation here (like digit to button)
}
