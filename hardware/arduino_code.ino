#include <HX711.h> 
#include <AccelStepper.h>
#include <SPI.h>
#include <WiFiNINA.h>

//USER DECLARED VARIABLES////////////////////////////////////////////////////////
char ssid[] = "Wifi Name";
char pass[] = "Password";
/////////////////////////////////////////////////////////////////////////////////

#define LOADCELL_SCK_PIN  2
#define LOADCELL_DOUT_PIN  3
#define DIR_PIN  5 //for stepper motor
#define STEP_PIN  6 //
#define BUTTON_1  14 //left
#define BUTTON_2  15 //middle
#define BUTTON_3  16 //right
#define CTRL_LED  17
#define TEMP_PIN  A7
#define STEP_SPEED  200.0 //speed of stepper motor in steps/sec
#define STEPS_PER_ROT 200.0
#define DIST_PER_ROT 0.008 //in meters
#define TEMP_RES 10000   
#define IPAddress 192.168.176.182

HX711 scale;

int button = 0;
int ledState = LOW;  // ledState used to set the LED
unsigned long previousMillis = 0;  // will store last time LED was updated
unsigned long currentMillis = 0;
long interval = 500;  // interval at which to blink (milliseconds)
int input = 0;  //for reading serial port
double calibration_factor = -20250; //-20250 worked for our PSD-S1 load cell
double data [256]; //to store readings
double testMax = 0;
double impEng = 0;
double temp = 25;
int testType = 0;

//wifi setup
int port = 8080;
char server[] = "IPAddress";
WiFiClient client;
unsigned long lastRequestTime = 0;
const unsigned long requestInterval = 60000;


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

  //Wifi Setup//////////////////////////////////////////////////////////////////////////////////////////
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    Serial.println("Attempting to connect to WiFi...");
    status = WiFi.begin(ssid, pass);
    delay(10000); // Adjust the delay as needed
    Serial.println(WiFi.status());
  }

  Serial.println("Connected to WiFi");

  
  char aePayld[256]; // Make sure the array is large enough to hold the payload
  sprintf(aePayld, "{ \"m2m:ae\": { \"rr\": true, \"api\": \"NR_AE001\", \"apn\": \"IOTApp\", \"csz\": [ \"application/json\" ], \"srv\": [ \"2a\" ], \"rn\": \"Data\", \"poa\": [ \"http://192.168.176.182:8080\" ] } }");

  if (client.connect(server, port)) {
    Serial.println("Connected to server");
    client.println("POST /id-in HTTP/1.1");
    client.println("Host: 192.168.176.182:8080");
    client.println("Connection: close");
    client.println("X-M2M-RI: xyz1");
    client.println("X-M2M-Origin: CData");
    client.println("X-M2M-RVI: 3");
    client.println("Content-Type: application/json;ty=2");
    client.print("Content-Length: ");
    client.println(strlen(aePayld));
    client.println();
    client.println(aePayld);
  } else {
    Serial.println("Connection to server failed");
  }
  while (client.connected()) {
    if (client.available()) {
        char c = client.read();
        Serial.print(c);
    }
  }
  client.stop();

  char cntPayld[] = "{ \"m2m:cnt\": { \"rn\": \"CNT001\", \"lbl\": [ \"key1\", \"key2\" ], \"mni\": 10 } }";
  Serial.println("Container Create Request");
  Serial.println("************************");
  Serial.println(cntPayld);
  Serial.println("************************\n");
  if(client.connect(server, port)){
    client.println("POST /cse-in/Data HTTP/1.1");
    client.println("Host: 192.168.176.182:8080");
    client.println("Connection: close");
    client.println("X-M2M-RI: xyz1");
    client.println("X-M2M-Origin: CData");
    client.println("X-M2M-RVI: 3");
    client.println("Content-Type: application/json;ty=3");
    client.print("Content-Length: ");
    client.println(strlen(cntPayld));
    client.println();
    client.println(cntPayld);

  }
  else{
    Serial.println("Connection to server failed2");
  }
    while (client.connected()) {
      if (client.available()) {
          char c = client.read();
          Serial.print(c);
      }
  }
  client.stop();

  //setup stop//////////////////////////////////////////////////////////////////////////////////////////

  Serial.begin(9600);
  Serial.println("Welcome to Project Block Breaker");

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
  //choice 1
  double temp = 25;
  int testType = 0;
  testMax = 0;
    for(int i=0; i<100; i++){
      data[i] = 0; // clear data for new test
    }

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
    digitalWrite(CTRL_LED, LOW);
  }
  else{button = 0;}

  //choice 2 result
  if(button == 1){
    Serial.println("Running Impact Test...");
    //Run Impact Test/////////////////////////////////////////////////////////////////////////////////////////
    testMax = 0;
    impEng = 0;
    testType = 2;
    temp = 0;
    for(int i=0; i<100; i++){
      data[i] = 0; // clear data for new test
    }
    int i = 0;
    scale.set_scale(calibration_factor);
    myStepper.setSpeed(STEP_SPEED);
    
    button = 0;
    while(button == 0){
      myStepper.runSpeed();
      if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
        button = 1;
      }
      if (Serial.available() > 0) {
        input = Serial.read();
        if((input >= 49) && (input <= 51)){
          button = 1;
        }
      }
      data[i] = (scale.get_units()*4.4482); //calibrated for lbs, so converting to newtons
      Serial.print(data[i]);
      Serial.println(" N");

      if(data[i] > testMax){
        testMax = data[i];
      }
      else if(abs(data[i]+3) < abs(testMax/2)){
        button = 1;
      }
      i++;

      temp = getTemp()/256;

      digitalWrite(CTRL_LED, LOW);
      for(int i = 0; i<250; i++){
        myStepper.runSpeed();
        delay(1);
      }
      digitalWrite(CTRL_LED, HIGH);
      for(int i = 0; i<250; i++){
        myStepper.runSpeed();
        delay(1);
      }
    }
    myStepper.stop();
    button = 1;
    //calculate temp
    temp = temp*(256/(i+1));
    //calculate impact energy here
    for(int i = 0; i<255; i++){
      impEng += data[i] * ((STEP_SPEED/2.0) / STEPS_PER_ROT) * DIST_PER_ROT;
      if(data[i] == testMax){break;}
    }
    
    delay(500);
    Serial.print("Impact Test Complete, Peak Force: ");
    Serial.print(testMax);
    Serial.println(" N");
    Serial.print("Impact Energy: ");
    Serial.print(impEng);
    Serial.println(" J");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
    //end of test//////////////////////////////////////////////////////////////////////////////////////////////////
  }
  else if(button == 3){
    Serial.println("Running Tensile Test, press any button to cancel");
    //Run Tensile Test/////////////////////////////////////////////////////////////////////////////////////////
    testMax = 0;
    testType = 1;
    temp = 0;
    impEng = 0;
    for(int i=0; i<100; i++){
      data[i] = 0; // clear data for new test
    }
    int i = 0;
    scale.set_scale(calibration_factor);
    myStepper.setSpeed(-STEP_SPEED);
    
    button = 0;
    while(button == 0){
      myStepper.runSpeed();
      if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
        button = 1;
      }
      if (Serial.available() > 0) {
        input = Serial.read();
        if((input >= 49) && (input <= 51)){
          button = 1;
        }
      }
      data[i] = (scale.get_units()*4.4482)*(-1); //calibrated for lbs, so converting to newtons
      Serial.print(data[i]);
      Serial.println(" N");

      if(data[i] > testMax){
        testMax = data[i];
      }
      else if(abs(data[i]+3) < abs(testMax/2)){
        button = 1;
      }
      i++;

      temp = getTemp()/256;

      digitalWrite(CTRL_LED, LOW);
      for(int i = 0; i<250; i++){
        myStepper.runSpeed();
        delay(1);
      }
      digitalWrite(CTRL_LED, HIGH);
      for(int i = 0; i<250; i++){
        myStepper.runSpeed();
        delay(1);
      }
    }
    myStepper.stop();
    //calculate temp
    temp = temp*(256/(i+1));
    delay(500);
    button = 1;
    Serial.print("Tensile Test Complete, Tensile Strength: ");
    Serial.print(testMax);
    Serial.println(" N");
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" C");
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
    digitalWrite(CTRL_LED, HIGH);
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
    digitalWrite(CTRL_LED, LOW);
  }

  if(button == 1){
    Serial.println("Uploading...");
    //Upload results//////////////////////////////////////////////////////////////////////////////
    //int testType _ double testMax _ double impEng _ double temp

    std::string strData = std::to_string(testType) + " " + std::to_string(testMax) + " " + std::to_string(impEng) + " " + std::to_string(temp);
    char string64[256];
    strcpy(string64, strData.c_str()); 

    String ciPayld = "{ \"m2m:cin\": { \"cnf\": \"application/text:0\", \"con\": \"" + String(string64) + "\" } }";
    Serial.println("CI Create Request");
    Serial.println("************************");
    Serial.println(ciPayld);
    Serial.println("************************\n");
    if(client.connect(server, port)){
      client.println("POST /cse-in/Data/CNT001 HTTP/1.1");
      client.println("Host: 192.168.176.182:8080");
      client.println("Connection: close");
      client.println("X-M2M-RI: xyz1");
      client.println("X-M2M-Origin: CData");
      client.println("X-M2M-RVI: 3");
      client.println("Content-Type: application/json;ty=4");
      client.print("Content-Length: ");
      client.println(ciPayld.length());
      client.println();
      client.println(ciPayld);
    }
    else{
      Serial.println("Connection to server failed3");
    }
      while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            Serial.print(c);
        }
    }
    client.stop();
    
    
//end upload/////////////////////////////////////////////////////////////////////////////////
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
double getTemp(){
  float reading;
  
  reading = analogRead(TEMP_PIN);
 
  reading = (1023 / reading)  - 1;     // (1023/ADC - 1) 
  reading = TEMP_RES / reading;  // 10K / (1023/ADC - 1)
  
  //1/T = a + b(ln(Rt/R25)) + c(ln(Rt/R25))^2 + d(ln(Rt/R25))^3
  double temp = (1.0/(0.003354 + 0.0002563*log(reading/3000.0) + pow(0.000002083*log(reading/3000.0), 2.0)) + pow(0.000000073*log(reading/3000.0), 3.0))-273.15;

  return(temp);
}

void manualMove(int speed){
  myStepper.setSpeed(speed);
  button = 0;
  while(button == 0){
    myStepper.runSpeed();
    if((digitalRead(BUTTON_1)==HIGH)||(digitalRead(BUTTON_2)==HIGH)||(digitalRead(BUTTON_3)==HIGH)){
      button = 1;
    }
    if (Serial.available() > 0) {
      input = Serial.read();
      if((input >= 49) && (input <= 51)){
        button = 1;
      }
    }
  }
  myStepper.stop();
  button = 0;
  digitalWrite(CTRL_LED, HIGH);
  delay(250);
  digitalWrite(CTRL_LED, LOW);
  delay(250);
}