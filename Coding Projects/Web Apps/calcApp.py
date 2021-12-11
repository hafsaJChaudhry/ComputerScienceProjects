from tkinter import*

# FORTH CODE THIS : define operations
def btnClick(numbers):
    # define a global variable
    global operator
    operator = operator + str(numbers)
    text_Input.set(operator)

def btnClearDisplay():
    # define a global variable
    global operator
    operator = ""
    text_Input.set("")

def btnEqualsInput():
    # define a global variable
    global operator
    # manually find % cuz python doesnt have in eval() lib
    if "%" in operator:
      operator = operator.replace("%","/100")
      sum_up = str(eval(operator))
      text_Input.set(sum_up)
    elif "±" in operator:
      operator = operator.replace("±","*-1")
      sum_up = str(eval(operator))
      text_Input.set(sum_up)
    # ALL other expressions(add,mult,etc)
    else:
      sum_up = str(eval(operator))
      text_Input.set(sum_up)
       # you need to now empty your operator
    operator = ""



    # FIRST CODE THIS
# Create object. give our object ("system") a name...cal
cal = Tk()
# give it a title
cal.title("Calculator")
# create an operator to hold onto all inputs
operator = ""
text_Input = StringVar()

# if you want to adjust size:
cal.geometry("300x400")

# change background color of python tkinter window
cal.configure(bg="#000000")

# dynamically resize the window and its widget
Grid.rowconfigure(cal,0, weight=1)
Grid.columnconfigure(cal,0, weight=1)

Grid.rowconfigure(cal,1,weight=1)
Grid.rowconfigure(cal,2,weight=1)
Grid.rowconfigure(cal,3,weight=1)
Grid.rowconfigure(cal,4,weight=1)
Grid.rowconfigure(cal,5,weight=1)
Grid.columnconfigure(cal,1, weight=1)
Grid.columnconfigure(cal,2, weight=1)
Grid.columnconfigure(cal,3, weight=1)

# Grid.columnconfigure(cal,4, weight=1)

# SECOND CODE THIS
""" create a text box display for your calculator "cal"  with font(font, size, style), 
property (text variable aka input), border size, insert width, back color, 
and justify if data is right...
 then define your grid w/ column span """
txtDisplay = Entry(cal,font=('arial',50, 'bold'), textvariable= text_Input,
                  insertwidth=40, bg="gray26", justify='right').grid(
    columnspan=4, sticky = 'nesw')
# use the code above and run it to see an interactive display :)

# THIRD CODE THIS
# lets create one button and take it from there
""" interactive button, then call the name of your app "cal", add padding (makes the button taller and wider),
border, font color, font, define what will be on our button and also the position of our 
button aka row 1 column 0"""
""" after you defined functions in step 4, declare functions in btn with command 
like so: command=lambda 
lambda is an invisible function that the system recognizes 
IT ALLOWS THE NUMBER TO DISPLAY ON DISPLAY TEXTBOX WHEN BUTTON IS CLICKED """
btn7 = Button(cal, padx=16, pady=16, fg="black", font=('arial',20, 'bold'),
              text="7", command=lambda:btnClick(7), bg="grey60").grid(row=2,column=0, sticky = 'nesw')
# copy and paste the other buttons
btn8 = Button(cal, padx=16, pady=16, fg="black", font=('arial',20, 'bold'),
              text="8", command=lambda:btnClick(8),bg="grey60").grid(row=2,column=1, sticky = 'nesw')
btn9 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="9", command=lambda:btnClick(9),bg="grey60").grid(row=2,column=2, sticky = 'nesw')
Addition = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="+", command=lambda:btnClick("+"),bg="orange2").grid(row=4,column=3, sticky = 'nesw')
#=======================================================#
### ROW 2 ###
btn4 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="4", command=lambda:btnClick(4),bg="grey60").grid(row=3,column=0, sticky = 'nesw')
# copy and paste the other buttons
btn5 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="5", command=lambda:btnClick(5),bg="grey60").grid(row=3,column=1, sticky = 'nesw')
btn6 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="6", command=lambda:btnClick(6),bg="grey60").grid(row=3,column=2, sticky = 'nesw')
Subtraction = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="-", command=lambda:btnClick("-"),bg="orange2").grid(row=3,column=3, sticky = 'nesw')
#=======================================================#
### ROW 3 ###
btn1 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="1", command=lambda:btnClick(1),bg="grey60").grid(row=4,column=0, sticky = 'nesw')
# copy and paste the other buttons
btn2 = Button(cal, padx=16,pady=16, fg="black", font=('arial',20, 'bold'),
              text="2", command=lambda:btnClick(2), bg="grey60").grid(row=4,column=1, sticky = 'nesw')
btn3 = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="3", command=lambda:btnClick(3),bg="grey60").grid(row=4,column=2, sticky = 'nesw')
Multiply = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="×", command=lambda:btnClick("*"),bg="orange2").grid(row=2,column=3, sticky = 'nesw')
#=======================================================#
### ROW 4 ###
""" add in clear, equals function below by using command and 
declaring it within the button """
btn0 = Button(cal, padx=16, pady=16, fg="black", font=('arial',20, 'bold'),
              text="0",command=lambda:btnClick("0"), bg="grey60").grid(row=5,column=0,columnspan=2, sticky = 'nesw')
# copy and paste the other buttons
btnClear = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="C", bg="grey40", command=btnClearDisplay).grid(row=1,column=0, sticky = 'nesw')
btnEquals = Button(cal, padx=16, pady=16, fg="black", font=('arial',20, 'bold'),
              text="=", bg="orange2", command=btnEqualsInput).grid(row=5,column=3, sticky = 'nesw')
Divide = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="÷", command=lambda:btnClick("/"),bg="orange2").grid(row=1,column=3, sticky = 'nesw')
Decimal = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text=".", command=lambda:btnClick("."),bg="grey60").grid(row=5,column=2, sticky = 'nesw')
Negate = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="±",command=lambda:btnClick("±"), bg="grey40").grid(row=1,column=1, sticky = 'nesw')
Percentage = Button(cal, padx=16, pady=16,fg="black", font=('arial',20, 'bold'),
              text="%",command=lambda: btnClick("%"),bg="grey40").grid(row=1,column=2, sticky = 'nesw')
# Exponent = Button(cal, padx=16, pady=16, fg="black", font=('arial',20,'bold'),text="^",command=lambda:btnClick("^"),
#             bg="grey40").grid(row=1,column=3,sticky='nesw')
# ParanthesisL = Button(cal, padx=16,pady=16,fg="black",font=('arial',20,'bold'),
#               text="(",command=lambda:btnClick("("),bg="grey40").grid(row=1,column=2,sticky='nesw')
# ParanthesisR = Button(cal,padx=16,pady=16, fg="black", font=('arial',20,'bold'),text=")",
#               command=lambda:btnClick(")"), bg="grey40").grid(row=1,column=2,sticky='nesw')
"""
 Exponent = Button(cal, padx=16, pady=16, fg="black", font=('arial',20,'bold'),text="^",command=lambda:btnClick("^"),
            bg="grey40").grid(row=1,column=3,sticky='nesw')
 ParanthesisL = Button(cal, padx=16, pady=16, fg="black", font=('arial',20,'bold'),
            text="(", command=lambda:btnClick("("),bg="grey40").grid(row=1,column=2,sticky='nesw')
 ParanthesisR = Button(col,padx=16,pady=16, fg="black", font=('arial',20,'bold'),text=")", 
            command=lambda:btnClick(")"), bg="grey40").grid(row=1,column=2,sticky='nesw')
    
    when executing make sure to 
"""

# execute tkinter
cal.mainloop()

""" 
############################# ALL NOTES and SUMMARY ################################ 
order of operations from above comments: 
1 # FIRST CODE THIS 
2 # SECOND CODE THIS 
3 # THIRD CODE THIS 
4 # FORTH CODE THIS : define operations 
5 - system.mainloop() 

1 - sets up tkinter system. Takes in string inputs 
2 - input display box 
3 - all the buttons 
4 - set up functions so buttons work and do something 
5 - to make system run 

Code to make all the numerical expressions calculate: 
    sum_up = str(eval(operator)) 
    text_Input.set(sum_up) 

Break down this code: 
btnEquals = Button(cal, padx=16, pady=16, bd=8, fg="black", font=('arial',20, 'bold'), 
              text="=", bg="powder blue", command=btnEqualsInput).grid(row=5,column=0, 
              columnspan=4, sticky = 'nesw') 
  Button( ... ) 
 -cal is system name, padx and pady is dimensions. technically dont need bc in the end I use 
 sticky = 'nesw' which automatically adjusts the buttons to fit and fill the size 
 -bd is border size for button 
 -fg stands for foreground but changes font color of # on button (so fg = "red" will make 8 be red on calc) 
 -font=(font, size, style) 
 -text= text that displays when button is pressed 
 - bg is background color 
 - command= set to function you made in step 4 
 - Button( ... ).grid( ... ) 
 - grid is where you place the button 
 - row, column, columnspan or rowspan is when button is bigger than others 
 - sticky = 'nesw' makes the button fit into empty space 
            
from the mainloop make sure to readjust the execution so that the jump isn't 
            
     
  
"""