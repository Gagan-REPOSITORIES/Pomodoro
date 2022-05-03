// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//used push buttons on analog pins
#define up 5
#define down 4
#define left 3
#define right 2
#define buzzer 6

//variables used
bool pomodoro_active = false; //check whether pomodoro is active or stopped
bool stopwatch_active = false;//check whether stopwatch is active or stopped
byte menu = 0; //used for switching menus; initial is default menu
unsigned long pomodoro_start = 0; //value to store pomodoro start time
unsigned long stopwatch_start = 0; //value to store stopwatch start time
unsigned long pauseTime = 0; // to store when the stopwatch was paused
byte pomodoroTimer = 25; //Pomodoro Timer value
bool anybuttonPressed = false; //to avoid conflicts with the if loops
bool stopwatch_pause = false; // to check whether stopwatch is paused


void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // initialize the pins as input with pullup resistor
  //if button is not pressed then it will be always high; when button pressed it will be low
  pinMode(up,INPUT_PULLUP);
  pinMode(down,INPUT_PULLUP);
  pinMode(left,INPUT_PULLUP);
  pinMode(right,INPUT_PULLUP);

  init_setup();//used to check the working of a buttons
  
}

void loop() 
{
  lcd.clear();
  //if menu ==0 i.e default screen i.e date time 
  if(menu == 0)
  {
    default_screen();
  }
  
  //if menu ==1 i.e pomodoro screen
  if(menu == 1)
  {
    Pomodoro();
  }

  if(menu == 2)
  {
    Stopwatch();
  }

  //when menu is in default then only down button will work and it has show pomodoro menu
  if((menu == 0) && !digitalRead(down) && !anybuttonPressed)
  {
    menu = 1;
    delay(250);//button debounce
    anybuttonPressed = true;
  }
  //when menu is in pomodoro menu and up key is pressesd will lead to default screeen
  if((menu == 1) && !digitalRead(up) && !anybuttonPressed)
  {
    menu=0;
    delay(250);//button debounce
    anybuttonPressed = true;
  }
  //when menu is in pomodoro and down key is pressed will take to stopwatch menu
  if((menu ==1) && !digitalRead(down) && !anybuttonPressed)
  {
    menu = 2;
    delay(250);//button debounce
    anybuttonPressed = true;
  }
  //when menu is in stopwatch and up key is pressed
  if((menu == 2) && !digitalRead(up) && !anybuttonPressed)
  {
    menu = 1;
    delay(250);//button debounce
    anybuttonPressed = true;
  }

 
 //when in pomodoro menu and start button is pressed start the timer 
 //this will also resets the timer
  if((menu == 1) && !digitalRead(left) && !pomodoro_active && !anybuttonPressed)
     {
       pomodoro_active = true;
       pomodoro_start = millis()/1000;
       anybuttonPressed = true;
     } 
 //when pomodoro is active and again the start button is pressed will reset timer
  if((menu == 1) && !digitalRead(left) && pomodoro_active && !anybuttonPressed)
     {
       pomodoro_active = false;
       anybuttonPressed = true;
       //pomodoro_start = millis()/1000;
     }

  //when in stopwatch menu and start button is pressed then start the stopwatch
  if((menu == 2) && !digitalRead(left) && !stopwatch_active && !anybuttonPressed)
  {
    stopwatch_active = true;
    stopwatch_start = millis()/1000;
    anybuttonPressed = true;
    delay(1000);//to control the speed of switching
    stopwatch_pause = false;
    pauseTime = 0;
  }
  //when in stopwatch menu and start button is pressed will reset the stopwatch
  if((menu == 2) && !digitalRead(left) && stopwatch_active && !anybuttonPressed && !stopwatch_pause)
  {
    stopwatch_active = false; 
    anybuttonPressed = true;
    stopwatch_pause = false;
    pauseTime = 0;
    //stopwatch_start = millis()/1000;
  }

  //when the stopwatch is running and we need to pause it 
  if ((menu==2) && !digitalRead(right) && stopwatch_active && !stopwatch_pause && !anybuttonPressed)
  {
    pauseTime = ((millis()/1000)-stopwatch_start) + pauseTime;//to know when it got stopped
    stopwatch_pause = true; // now stop watch is true
  }

  //After pausing the stopwatch they can resume by pressing the start button
  if((menu == 2) && !digitalRead(left) && stopwatch_active && !anybuttonPressed && stopwatch_pause)
  {
    anybuttonPressed = true;
    stopwatch_pause = false;
    //pauseTime = 0;
    stopwatch_start = millis()/1000;
  }

  anybuttonPressed = false;
  
}

// checking conditions of working buttons
void init_setup(){
  do{
  lcd.print("ButtonAssignment");
  lcd.setCursor(0, 1);
  lcd.print("< > Changes Menu");
   delay(250);
  }while(digitalRead(up) && digitalRead(down));
  
  lcd.clear();
  
  do{
    lcd.print("[:start ]:stop");
    lcd.setCursor(0, 1);
    lcd.print("  ]*5s : RESET  ");
    delay(250);
  }while(digitalRead(left) && digitalRead(right));
  
  lcd.clear();
    
}

void default_screen()
{
  lcd.print("Current Time");
  lcd.setCursor(0, 1);
  unsigned long time2convert = millis()/1000;
  byte seconds = time2convert%60; //time in seconds
  byte minutes = (time2convert/60)%60; //time in minutes
  byte hours = (time2convert/3600)%24; //time in hours
  lcd.print(String(hours)+':'+String(minutes)+':'+String(seconds));
  delay(600);
}

void Pomodoro()
{
  int value2Convert;
  lcd.print("Timer 25min");
  lcd.setCursor(0, 1);
  if(!pomodoro_active)
  {
    lcd.print("25:00");
  }
  if(pomodoro_active)
  {
      value2Convert = (pomodoroTimer*60)-((millis()/1000)-pomodoro_start);
    //send remaining value to convert 
    if(value2Convert>0)
      ss2hh(value2Convert,true);
    
    //if time is up print take break
    else
    {
      lcd.print("Take Break");
      digitalWrite(buzzer, HIGH);
      delay(2000);
      digitalWrite(buzzer,LOW);  
    }
  }
  
  delay(600);
}

void Stopwatch()
{
  int value2Convert;
  lcd.print("Stopwatch");
  lcd.setCursor(0, 1);
  if(!stopwatch_active)
  {
    lcd.print("00:00:00");
  }
  if(stopwatch_active && !stopwatch_pause)
  {
      value2Convert = ((millis()/1000)-stopwatch_start) + pauseTime;
    //send remaining value to convert 
    if(value2Convert>0)
      ss2hh(value2Convert,true);
  }
  if(stopwatch_active && stopwatch_pause)
  {
    value2Convert = pauseTime;
    if(value2Convert>0)
      ss2hh(value2Convert,true);
  }
  
  delay(600);
}

//this function can take input seconds input and convert to hours format
int ss2hh(unsigned long time2convert,bool to_print)
{
  byte seconds = time2convert%60; //time in seconds
  byte minutes = (time2convert/60)%60; //time in minutes
  byte hours = (time2convert/3600)%24; //time in hours
  if (to_print)
    lcd.print(String(hours)+':'+String(minutes)+':'+String(seconds));
  else;
  return 0;
}
