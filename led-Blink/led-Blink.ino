// This code is written by Jacob Reichle
#include <Keypad.h>
#include <string.h>

const int ROW_NUM = 4; // Four rows
const int COLUMN_NUM = 4; // Four columns

// The correct password
const String correctPassword = "4A2D";
String inputPassword = ""; // Holds user input password

// Char array of each key on the pad
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

int numArray[10][7] = {
  { 1,1,1,1,1,1,0 },    // 0
  { 0,1,1,0,0,0,0 },    // 1
  { 1,1,0,1,1,0,1 },    // 2
  { 1,1,1,1,0,0,1 },    // 3
  { 0,1,1,0,0,1,1 },    // 4
  { 1,0,1,1,0,1,1 },    // 5
  { 1,0,1,1,1,1,1 },    // 6
  { 1,1,1,0,0,0,0 },    // 7
  { 1,1,1,1,1,1,1 },    // 8
  { 1,1,1,0,0,1,1 }    // 9
};

int alphabet[6][7] = {
  { 1,1,1,0,1,1,1 },  // A
  { 0,0,1,1,1,1,1 },  // B
  { 1,0,0,1,1,1,0 },  // C
  { 0,1,1,1,1,0,1 },  // D
  { 1,0,0,1,1,1,1 },  // E
  { 1,0,0,0,1,1,1 }   // F
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

// Instantiate a new class of keypad to represent 4x4 Keypad for output
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
void displayDigit(int);
void showEntry(int);

int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  Serial.begin(9600); // pass in baud rate to communicate with keypad serially
  digitalWrite(led, LOW);  // Light set to OFF position

  // Configure pins 13-19 for 7-segment LED 
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  // Configure the buzzer at pin 12 to OUTPUT
  pinMode(12, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(12, LOW);  // set buzzer to OFF
  char key = keypad.getKey();

  if (key) {
    Serial.print(key);


    // Press '#' on the keypad to submit code currently in the input register
    if (key == '#')
    {
      // Compare password input to actual password
      if (inputPassword == correctPassword)
      {
        // if correct turn on board LED on, signal to serial monitor of correct password
        digitalWrite(led, HIGH);
        Serial.println("Correct!");
        rightAnswerBlink(); // blink the 7-segment piece to indicate correct password

        // Do routine blocked by passcode here         

      } else {
        inputPassword = ""; // clear password register
        Serial.println("Wrong!"); // Print incorrect to serial monitor
        wrongBuzzer(400);         // Play a buzzer tone 3 times at 400s delay intervals
    }
    } else if(key == '*'){
      // clear the input password by presssing '*'
      inputPassword = "";
      Serial.println("Password cleared.");
    } else {
      // User is still entering characters
      // Ensure password attempt resets after it exceeds 4 characters so infinite string doesn't build
      if (inputPassword.length() >= 4)
      {
        inputPassword = "";   // clear password
      } 

      // append new digit to password attempt
      inputPassword += key;
      showEntry(key); // Print key entry to the 7-pin LED 
    }
  }
}

// Function to display a digit from digit array
void displayDigit(int number)
{
  int pin = 13; // Starting pin of all 7 connected in LED

  // Turn each pin on or off according to binary representation in number array
  for (int j = 0 ; j < 7 ; j++)
  {
    digitalWrite(pin, numArray[number][j]);
    pin++;
  }
}

// Function to display letter from alphabet array
void displayLetter(int number)
{
  int pin = 13; // Starting pin of all 7 in LED

  // Turn each pin on or off according to alphabet array
  for (int k = 0 ; k < 7 ; k++)
  {
    digitalWrite(pin, alphabet[number][k]);
    pin++;    
  }
}

// Parses input 
void showEntry(int number)
{
  // Check if it's a number or letter
  if (number < 64)
  {
    // Case it's a number
    displayDigit(number - 48);
  } else {
    // Case it's a letter
    displayLetter(number - 65);
  }
}

// Animation on the 7-seg LED
void rightAnswerBlink()
{
  int pin = 13;

  // First shut down each segment
  for (int j = 13 ; j < 20 ; j++)
  {
    digitalWrite(j,0);
  }

  // Turn on each segment to make a zero slowly 
  for (int k = 0 ; k < 7 ; k++)
  {
    digitalWrite(pin, numArray[0][k]);
    pin++;
    delay(200); // short delay to "animate" the process
  }

  // Blink the segments 3 times
  for (int i = 0 ; i < 3 ; i++)
  {
    singleBlink(400);
  }  

}

// Void to blink every border LED segment that takes in a delay rate between blinks
void singleBlink(int del)
{
  // Now blink every segment 3 times    
  for (int j = 13 ; j < 19 ; j++)
  {
    digitalWrite(j, 0);
  }  
  delay(del);
  // Now blink every segment 3 times    
  for (int j = 13 ; j < 19 ; j++)
  {
    digitalWrite(j, 1);
  }
  delay(del);
  // Now blink every segment 3 times    
  for (int j = 13 ; j < 19 ; j++)
  {
    digitalWrite(j, 0);
  }
}

// Active buzzer alarm to indicate wrong password
void wrongBuzzer(int del)
{
  for(int i = 0 ; i < 3 ; i++)
  {
    digitalWrite(12, HIGH);
    delay(del);
    digitalWrite(12, LOW);
    delay(del);
  }
}

