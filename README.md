
Signal and Slots
================  
Introduction:
---

- In GUI programming, when we change one widget, we often want another widget to be notified. More generally, we want objects of any kind to be able to communicate with one another. For example, if a user clicks a **Close** button, we probably want the window's [close()](https://doc.qt.io/qt-5/qwidget.html#close) function to be called.

- The signals and slots mechanism is a central feature of Qt and probably the part that differs most from the features provided by other frameworks. Signals and slots are made possible by Qt's [meta-object system](https://doc.qt.io/qt-5/metaobjects.html)*. *Other toolkits achieve this kind of communication using callbacks( like `matlab`).*

  ![](https://user.oc-static.com/files/103001_104000/103834.png)

  > **Meta Object System** is a part of [Qt framework](<https://en.wikipedia.org/wiki/Qt_(framework)> "Qt (framework)") core provided to support Qt extensions to [C++](https://en.wikipedia.org/wiki/C%2B%2B "C++") like [signals/slots](https://en.wikipedia.org/wiki/Signals_and_slots "Signals and slots") for inter-object communication, run-time [type information](https://en.wikipedia.org/wiki/Type_system "Type system"), and the dynamic property system.[[1]](https://en.wikipedia.org/wiki/Meta-object_System#cite_note-1)

  _For further details you can check [Signals And Slots](https://doc.qt.io/qt-5/signalsandslots.html) documentation ._

# Implements our first Gui Interactive Widget.

- [Implementing basic functionalities](#implementing-basic-functionalities)
  - [Setup](#setup)
  - [Custom Slots](#custom-slots)
  - [Digits Interaction](#digits-interaction)
  - [Integer numbers](#integer-numbers)
  - [Operation Interaction](#operation-interaction)
  - [Enter Button](#enter-button)
- [Enhancements](#enhancements)
  - [Adding a screen where the operations performed are displayed](#adding-a-screen-where-the-operations-performed-are-displayed)
  - [Adding the reset button (C)](#adding-the-reset-button)
  - [Scientific mode](#scientific-mode)

# Implementing basic functionalities

Our goal consists of adding **interactive** functionality to the **calculator** widgets written in the previous homework. We will be using Qt [Signals and Slots](https://doc.qt.io/qt-5/signalsandslots.html) to simulate a basic calculator behavior. The supported operations are `*, +, -, /` , _we will be adding other features in the [Enhancements](#enhancements) part_.

> _The output should be similar to this window:_

![](https://anassbelcaid.github.io/CS311/homeworks/05_signals/calculator_preview.png)

## Setup[](#setup)

Our instructor has provided us with the code of the application but without any functionality here [Callcultaor.zip](https://anassbelcaid.github.io/CS311/homeworks/05_signals/Calculator.zip) .

- We need first to configure and run the project and get a clear Idea about the attributes and methods of the class.

- Then we will be adding the functionalities needed to make our calculator work.

In order to have a computing functionality, we will represent any mathematical operation by:

```cpp
    left   (op)  right
```

where `(op)` is one of the following allowed operation `+ - * /`.

Hence we will add the following private members to our class `calculator.h`.

```cpp
    private:
        int * left = nullptr;          //left operand
        int * right = nullptr;         // right operand
        QString *operation = nullptr;  // Pointer on the current operation
```

> It's better to assign our pointer to null to avoid segmentation fault.

## Custom Slots[](#custom-slots)

Our first step is to respond to each **digit** click.

- The trivial solution is to create a **slot** for each button, but that will be cumbersome.

  ```cpp
    public slots:
            void button0Clicked();
            void button1Clicked();
            ...
            void button9Clicked();
  ```

- So in order to avoid the trouble we will define only one slot that behaves **differently** according to which digit was pressed :

  ```cpp
   public slots:
      void newDigit();
  ```

  > _we will use the **Sender** approach which allow a slot to get the **identity** of the sender object, From that we could get which button was clicked._

## Digits Interaction[](#digits-interaction)

The idea of this new connection, it to connect **all the button** to this slot. This function will use the [Sender()](https://doc.qt.io/qt-5/qobject.html#sender) method to get the identity of which button was clicked and act accordingly.

1.  Hence we will [connect](https://doc.qt.io/qt-5/qobject.html#connect) all the digits buttons to this slot.

    ```cpp
         //Connecting the digits
         for(int i=0; i <10; i++)
             connect(digits[i], &QPushButton::clicked,
                     this, &Calculator::newDigit);
    ```

2.  Now, we will implement the `newDigit` slot to show the digit in the `LCDNumber`.

    ```cpp
       //Getting the identity of the button using dynamic_cast
       auto button  = dynamic_cast<QPushButton*>(sender());

       // Each button has his own digit in the text
       auto value = button->text()->toInt();

       //Displaying the digit
       disp->display(value);
    ```

Now each time, you will click on a button, his digit will be **displayed** in the [LCDNumber](https://doc.qt.io/qt-5/qlcdnumber.html).

## Integer numbers[](#integer-numbers)

Now that we can react to each digit, it is time to correctly implement the `newDigit` slot. We should clarify two points to clearly understand the implementation:

- Which number, should be constructing `left` or `right`

  > The response to this question is easy, If we have an operation, then we already have our **left operand** and we should focus on the right.

- How to add digit to an existing number.

  > Suppose we are working on left = 43. What should happen if we clicked the digit 2. Simply we should move all digit by one digit **(x10)** and then **add** the **2**. Programmatically speaking this could done by :

  ```cpp
        *left = (*left) * 10 + digit
  ```

Here is the full implementation of this function using the _mentioned_ details:

```cpp
    void Calculator::newDigit( )
    {
        //getting the sender
        auto button = dynamic_cast<QPushButton*>(sender());

        //getting the value
        int value = button->text().toInt();

        //Check if we have an operation defined
        if(operation)
        {
            //check if we have a value or not
            if(!right)
                right = new int{value};
            else
                *right = 10 * (*right) + value;

            disp->display(*right);

        }
        else
        {
            if(!left)
                left = new int{value};
            else
                *left = 10 * (*left) + value;

            disp->display(*left);
        }
    }
```

There is another approach that allows us to get the same result **_(we won't be working with this approach because of some limitations in the enhancements):_**

- > _We found out that using this method`display(const QString &s)` we can display [QString](https://doc.qt.io/qt-5/qstring.html) on the QLCDNumber , so whenever we click on a new digit we just append the `button->text()` to a QString that we will display later on._
  ```cpp
  	  QPushButton  *button=(QPushButton  *)sender();
  	  varValue+=button->text();
  	  dispResult->display(varValue);
  	  fnum = varValue.toDouble();
  ```
- > _where **fnum** is **left** and we will define an **snum(right)** whenever we want to calculate the result._

## Operation Interaction[](#operation-interaction)

Now we will move on the `operation` of the four buttons. We will exploit the same mechanism using the `sender` method. Hence we will define a single slot to handle the click on the operations buttons:

```cpp
    public slots:
        void changeOperation();  //Slot to handle the click on operations
        void newDigit();
```

And in order to perform multiple operations at the same time we will define as well the method :

```cpp
protected:
 void  whichop();
```

And a pointer that refers to a QString :

```cpp
private:
QString  *op=nullptr;
```

which has the following implementation:

```cpp
void Calculator::whichop(){
   if(*op=="+"){
      *left=*left+*right;
   }
   else if(*op=="-"){*left=*left-(*right);
       }
   else if(*op=="*"){*left=(*left)*(*right);
       }
   else if(*op=="/"){
   if(*right==0)disp->display("error");
   else{ *left=*left/((*right));}
     }
}
```

This slot will simply execute the following operations:

1.  Gets the identity of the sender button.
2.  Stores the clicked operation.
3.  Checks if right exists (\***_right_** has an int value) if it does executes the method `whichop()`.
    > \*The `whichop()` method do the same job as `showresult()` it does the math and it assigns the **_result_** to **_left_** and **0/1** to **_right_** except displaying (until we click enter).\*
4.  Reset the display to 0

    ```cpp
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
    ```

## Enter Button[](#enter-button)

The Enter button the one button that displays the result .Hence we will define a single slot to handle the click on the Enter buttons:

```cpp
public slots:
  void changeOperation();//Slot to handle the click on operations
  void newDigit();//Slot that handles the click on digits
  void showresult();//Slot that handles enter button
```

This slot will simply execute the following operations:

1. Checks which operation are we working on.
2. Do the math.
3. Then display.

```cpp
void  Calculator::showresult(){
  if(*operation=="+"){
  *left=*left+*right;
  disp->display(*left);  right  =  new  double{0};}
  else  if(*operation=="-"){*left=*left-(*right);
  disp->display(*left);right  =  new  double{0};}
  else  if(*operation=="*"){*left=(*left)*(*right);
  disp->display(*left);right  =  new  double{1};}
  else  if(*operation=="/"){  if(*right==0)disp->display("error");
  else  {*left=*left/((*right));
  disp->display(*left);right  =  new  double{1};}}
  }
  }
```

> _The Enter button calculates the last 2-digit-operation (if `whichop()` is called) then display the result_

## Enhancements[](#enhancements)

Our calculator is quite basic, you could enhance his capabilities by:

## Adding a screen where the operations performed are displayed[](#Operation-history)

- *In order to display operations on screen , we thought of creating a label `disp2` :*
  ` QLabel *disp2; // Where to display the opperations`  

- *And we will be creating a slot `void displayLabel()` that will be connected to digits and operations so that every button inner text will be written on this label :*

```cpp
void Calculator::displayLabel(){
	     setFontSizeDyn(disp2)

			    auto button = dynamic_cast<QPushButton*>(sender());

			    //getting the value
			    labelvalue += button->text();
			    disp2->setText(labelvalue);

			}
```

> - `labelvalue` _is global QString variable defined earlier._
> - `setFontSizeDyn()` _lower the font size of the label so that the more we write the smaller the font size and the label height gets._  


```cpp
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
```

## Adding the reset button

- We will have to add a reset button and a `Reset()` slot:  
   `QPushButton *reset; // reset button`  
   `void Reset(); // slot that handles reset button`
  - and here is the implementation:\*

    ```cpp
    void Calculator::Reset(){
    	  left=nullptr;
    	  right=nullptr;
    	  operation=nullptr;
    	  labelvalue = "";
    	  disp2->setText("");
    	  disp->display(0);
    	}
    ```

## Scientific mode

_The calculator that we've made so far is pretty basic, not efficient there are no roots no power no cosine/sine functions. So we thought of creating a mode that can use some of the primordial mathematical functions_.

> _let's start:_

- **First, we should add the buttons that refers to the mathematical functions:**

  > *We will be adding a square root, cosine, sine and a power button ,and the will connect them to the slot `void displayexpr()` that we will create:*
  
  `QVector<QPushButton*>  exprOperations;`
  
  ```cpp
  exprOperations.push_back(new  QPushButton("sqrt("));
  exprOperations.push_back(new  QPushButton("cos("));
  exprOperations.push_back(new  QPushButton("sin("));
  exprOperations.push_back(new  QPushButton("pow("));
  exprOperations.push_back(new  QPushButton("log("));
  exprOperations.push_back(new  QPushButton(")"));
  exprOperations.push_back(new  QPushButton("("));
  ```
    ```cpp
	  void Calculator::displayExpr(){
	    auto button = dynamic_cast<QPushButton*>(sender());

	    //getting the value
	    QString value = "Math.";
	    value += button->text();
	    labelvalue += value;
	    disp2->setText(labelvalue);
	    setFontSizeDyn(disp2);
	}
  ```
	 > the function that we will use in order to evaluates the string in diplayLabel takes `math.sqrt(5)` as `sqrt(5)` so we have to add `Math.` before appending `button->text()`.
  
  

- **Then, we should add a radio button that enables the scientific mode:**

  > These buttons are disabled until the radio button is checked, so we added a slot `sientificOn()` that has the following implementation :
  
	 ```cpp
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
	```
	> *As we said this function only enables a set of buttons.*
	
	* By the way `ans` button returns the result of the previous operation :
		 ```cpp
			void Calculator::displayPrevious(){
		    labelvalue+=QString::number(previous);
		    disp2->setText(labelvalue);
		}
		```
		> We will be assigning a result previous in our evaluation function.

- **And now, we create a feature that evaluates and displays the result of the expression written in the `displayLabel`:**

  > *We thought of creating a function that will evaluate a string expression (just like the `eval()` function in python).*
  ```cpp
	  double  Calculator::expressionEval(QString  expression_string){
	  double  value;
	  QScriptEngine  expression1;
	  value=expression1.evaluate(expression_string).toNumber();
	  previous  =  value;
	  return  value;
	}
  ```
  > *Our function will take `displayLabel->text()` and evaluates it and then returns the result that will be displayed in our slot `showResult()`.*

  > *You will have to download QScript (from Qt Maintenance tool) in order to include QScriptEngine Class that contains our function `evaluate()`.*
 
- **At last we only need to modify the `showResult()` slot to make it differentiate between the 2 modes :**
  ```cpp
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
  ```
	 > we only added a condition `if(!expr->isChecked())` the process would be normal `else` we will only display the result of our evaluation function.
 
 **TADAAAA!** , now we can add as many functions as we want we just need to create the buttons and boom the math is done.



