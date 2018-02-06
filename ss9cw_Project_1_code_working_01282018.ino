/*
 * This program is to design an automatic window shade. The shade closes when it detects significant 
 * light and opens when there is low light. 
 * A light sensor is used for measurement of the ambient light level and a motor to control the shade.
 * A button is also used to control the shade, toggling between opening and closing.
 * An LED by the window/shade turns on when the shade is in the process of opening or closing manually.
 * 
 * Created 26 Jan 2018
 * Modified 28 Jan 2018
 * by Shradha Shalini
 * 
 */


#include <Stepper.h> //using an existing library

#define STEPS 100 //gving a name to a constant value, it is the number of steps in one revolution of motor
#define LIGHT_SENSOR A0 // Light sensor is connected to A0 analog input of the Arduino

Stepper stepper(STEPS, 8, 10, 9, 11); // initialize the stepper library on pins 8 through 11
int pushButton = 2; // digital pin 2 has a pushbutton attached to it. 
int ledPin = 13; // digital pin 13 has LED pin attached
bool curtainOpen = false; // curtainOpen is a boolean variable to check the state of the curtain, it set to 0 initially
bool on = false;          // on is a boolean variable initially set to false 
bool buttonPressed = false; // buttonPressed is a boolean variable set to 0
int flag = 0;          // flag value is 0 initially
int buttonState = 0;   // button stae is initialized to 0, the value becomes 1 when the push button is pressed
int sensorValue = 200; // sensorValue initially initialized to 200 so that the intensity is high and the curtain is in closed state

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bits per second
  pinMode(pushButton, INPUT); // make the pushbutton's pin an input
  pinMode(ledPin, OUTPUT); //make the LED pin an output
}

void loop() {
  //takes the push button's input and stores it
  buttonState = digitalRead(pushButton);
  Serial.println("buttonState is");
  Serial.println(buttonState);

  //takes the light sensor input and stores it
  sensorValue = analogRead(LIGHT_SENSOR);
  Serial.println("The sensor value is");
  Serial.println(sensorValue);

  // manually operating with the help of button
  if(buttonState == 1) {    
    Serial.println("After pressing the button");
    if(on == true) {
      on = false;
      buttonPressed = true;
    } 
    else if(on == false){
      on = true;
      buttonPressed = true;
    }
  }

  if(buttonPressed == true) {
    if(on == true && curtainOpen == false) {
      digitalWrite(ledPin, HIGH);  
      Serial.println("switch open window");
      openWindow();
      digitalWrite(ledPin, LOW);
      buttonPressed = false;
      sensorValue = analogRead(LIGHT_SENSOR);
      Serial.println("Current sensor value is");
      Serial.println(sensorValue);
      curtainOpen = true;
    } 
    else if(on == false && curtainOpen == true) {
      digitalWrite(ledPin, HIGH); 
      Serial.println("switch close window");
      closeWindow();
      digitalWrite(ledPin, LOW);
      flag = 0;
      buttonPressed = false;
      sensorValue = analogRead(LIGHT_SENSOR);
      Serial.println("Current sensor value is");
      Serial.println(sensorValue);
      curtainOpen = false;
    }
  }

  // low intensity of light so opens the shade automatically
  if(sensorValue <= 100 && flag == 0 && curtainOpen == false) {  
    delay(4000);
    Serial.println("The sensor value is");
    Serial.println(sensorValue);
    Serial.println("auto open window");
    openWindow();
    flag = 1;
    curtainOpen = true;
    on = true;  
  }

  // high intensity of light so closes the shade automatically
  else if(sensorValue >= 101 && flag == 1 && curtainOpen == true) {   
    delay(4000);
    Serial.println("auto close window");
    closeWindow();
    flag = 0;
    curtainOpen = false;
    on = false;
  }
}

void openWindow(){
  stepper.setSpeed(30); // set the speed at 30 rpm:
  stepper.step(2000); //2000 steps
}

void closeWindow(){
  stepper.setSpeed(30); 
  stepper.step(-2000);  //2000 steps
}   

