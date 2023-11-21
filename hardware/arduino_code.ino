#include "HX711.h" 
#include <AccelStepper.h>

#define LOADCELL_SCK_PIN  2
#define LOADCELL_DOUT_PIN  4
#define DIR_PIN  5 //for stepper motor
#define STEP_PIN  6 //
#define BUTTON_1  14 //left
#define BUTTON_2  15 //middle
#define BUTTON_3  16 //right
#define CTRL_LED  17
#define STEP_SPEED  100 //speed of stepper motor

HX711 scale;

int button = 0;
int ledState = LOW;  // ledState used to set the LED
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis = 0;
long interval = 500;  // interval at which to blink (milliseconds)
int input = 0;  //for reading serial port
float calibration_factor = -20250; //-20250 worked for our PSD-S1 load cell
float data [100]; //to store readings
float testMax = 0;


AccelStepper myStepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); // setup stepper motor

void setup() {
  pinMode(CTRL_LED, OUTPUT);
  pinMode(BUTTON_1, INPUT);
  pinMode(BUTTON_2, INPUT);
  pinMode(BUTTON_3, INPUT);

  myStepper.setMaxSpeed(1000);
  myStepper.setSpeed(STEP_SPEED); // speed the motor will turn at
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare();  //zero the scale

  Serial.begin(9600);
  Serial.println("Welcome to Project Block Breaker");

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
  //choice 1
  button = 0;
  Serial.println("1: Adjust Left, 2: Chose Test, 3: Adjust Right");
  while(button == 0){
    digitalWrite(CTRL_LED, LOW);
    if(digitalRead(BUTTON_1)==HIGH){
      button = 1;}
    else if(digitalRead(BUTTON_2)==HIGH){
      button = 2;}
    else if(digitalRead(BUTTON_3)==HIGH){
      button = 3;}
    if (Serial.available() > 0) {
      input = Serial.read();
      if((input >= 49) && (input <= 51)){
        button = input - 48;
      }
    }
  }
  delay(500);

  //choice 1 result
  test_select:
  if(button == 1){
    Serial.println("Adjusting motor left, press any button to stop");
    //adjust motor left
    manualMove(STEP_SPEED);
  }
  else if(button == 3){
    Serial.println("Adjusting motor right, press any button to stop");
    //adjust motor right
    manualMove(-STEP_SPEED);
  }
  else if(button == 2){
    //select test/choice 2
    button = 0;
    Serial.println("1: Impact Test, 2: Cancel, 3: Tensile Test");
    while(button == 0){
      digitalWrite(CTRL_LED, HIGH);
      if(digitalRead(BUTTON_1)==HIGH){
        button = 1;}
      else if(digitalRead(BUTTON_2)==HIGH){
        button = 2;}
      else if(digitalRead(BUTTON_3)==HIGH){
        button = 3;}
      if (Serial.available() > 0) {
        input = Serial.read();
        if((input >= 49) && (input <= 51)){
          button = input - 48;
        }
      }
    }
    delay(500);
  }
  else{button = 0;}

  //choice 2 result
  if(button == 1){
    Serial.println("Running Impact Test...");
    //Run Impact Test/////////////////////////////////////////////////////////////////////////////////////////
    testMax = 0;
    for(int i=0; i<100; i++){
      data[i] = 0; // clear data for new test
    }
    int i = 0;
    scale.set_scale(calibration_factor);
    myStepper.setSpeed(STEP_SPEED);
    myStepper.runSpeed();
    button = 0;
    while(button == 0){
      if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
        button = 1;
      }
      data[i] = (scale.get_units()*4.4482); //calibrated for lbs, so converting to newtons
      Serial.print(data[i]);
      Serial.println(" N");

      if(data[i] > testMax){
        testMax = data[i];
      }
      else if(data[i]+3 < (testMax/2)){
        button = 1;
      }
      i++;
      delay(500);
    }
    myStepper.stop();
    button = 0;
    //calculate work here
    delay(500);
    Serial.print("Impact Test Complete, Peak Force: ");
    Serial.print(testMax);
    Serial.println(" N");
    //end of test//////////////////////////////////////////////////////////////////////////////////////////////////
  }
  else if(button == 3){
    Serial.println("Running Tensile Test, press any button to cancel");
    //Run Tensile Test/////////////////////////////////////////////////////////////////////////////////////////
    testMax = 0;
    for(int i=0; i<100; i++){
      data[i] = 0; // clear data for new test
    }
    int i = 0;
    scale.set_scale(calibration_factor);
    myStepper.setSpeed(-STEP_SPEED);
    myStepper.runSpeed();
    button = 0;
    while(button == 0){
      if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
        button = 1;
      }
      data[i] = -(scale.get_units()*4.4482); //calibrated for lbs, so converting to newtons
      Serial.print(data[i]);
      Serial.println(" N");

      if(data[i] > testMax){
        testMax = data[i];
      }
      else if(data[i]+3 < (testMax/2)){
        button = 1;
      }
      i++;
      delay(500);
    }
    myStepper.stop();
    button = 0;
    delay(500);
    Serial.print("Tensile Test Complete, Tensile Strength: ");
    Serial.print(testMax);
    Serial.println(" N");
    //end of test//////////////////////////////////////////////////////////////////////////////////////////////////
  }
  else if(button == 2){
      //return to start
      button = 0;
  }
  else{button = 0;}

  //choice 3
  if(button != 0){
    button = 0;
    Serial.println("1: Upload Results, 2: Cancel to Start, 3: Add Test");
    while(button == 0){
      digitalWrite(CTRL_LED, HIGH);
      if(digitalRead(BUTTON_1)==HIGH){
        button = 1;}
      else if(digitalRead(BUTTON_2)==HIGH){
        button = 2;}
      else if(digitalRead(BUTTON_3)==HIGH){
        button = 3;}
      if (Serial.available() > 0) {
        input = Serial.read();
        if((input >= 49) && (input <= 51)){
          button = input - 48;
        }
      }
    }
    delay(500);
  }

  if(button == 1){
    Serial.println("Uploading...");
    //Upload results
    Serial.println("Upload Complete");
  }
  else if(button == 3){
    Serial.println("Running Another Test");
    //Prep for another test
    button = 2;
    goto test_select;
  }
  else if(button == 2){
      //return to start
      button = 0;
  }
  else{button = 0;}
}

void manualMove(int speed){
  myStepper.setSpeed(speed);
  myStepper.runSpeed();
  button = 0;
  while(button == 0){
    if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
      button = 1;
    }
  }
  myStepper.stop();
  button = 0;
  delay(500);
}