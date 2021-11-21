#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QRadioButton>

class Calculator : public QWidget
{
    Q_OBJECT
public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
 // Add you custom slots here
protected:
    void createWidgets();        //Function to create the widgets
    void placeWidget();         // Function to place the widgets
    void makeConnexions();      // Create all the connectivity
    void whichop();
    void setFontSizeDyn(QLabel *a);
    double expressionEval(QString expression);
//events
public slots:
   void newDigit();
   void changeOperation();
   void showresult();
   void displayLabel();
   void scientificOn();
   void displayExpr();
   void Reset();
   void clearLastchar();
   void displayPrevious();
//keyEvent
protected:
    void keyPressEvent(QKeyEvent *e)override;     //Override the keypress events
//variables
private:
    QGridLayout *buttonsLayout; // layout for the buttons
    QVBoxLayout *layout;        //main layout for the button
    QVector<QPushButton*> digits;  //Vector for the digits
    QPushButton *enter;
    QRadioButton *expr;
    QPushButton *reset; // enter button
    QPushButton *clearLast;
    QPushButton *ans;
    QPushButton *button;
    QVector<QPushButton*> operations; //operation buttons
    QVector<QPushButton*> exprOperations;
    QLCDNumber *disp; // Where to display the numbers
    QLabel *disp2;
    double * left = nullptr;          //left operand
    double * right = nullptr;
    QString *operation = nullptr;
    QString *op = nullptr;// Pointer on the current operation
};
#endif // CALCULATOR_H
