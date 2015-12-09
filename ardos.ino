#include <Ping.h>

Ping ping = Ping(8,0,0);

#include <AddicoreRFID.h>
#include <SPI.h>
#define uchar unsigned char
#define uint unsigned int
//4 bytes tag serial number, the first 5 bytes for the checksum byte
uchar fifobytes;
uchar 
fifoValue;
AddicoreRFID myRFID; // create AddicoreRFID object to control the RFID module
/////////////////////////////////////////////////////////////////////
//set the pins
/////////////////////////////////////////////////////////////////////
const int 
chipSelectPin = 10;
const int NRSTPD = 5;
//Maximum length of the array
#define MAX_LEN 16
int RFIDAuth = 0;





String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

long day = 86400000; // 86400000 milliseconds in a day
long hour = 3600000; // 3600000 milliseconds in an hour
long minute = 60000; // 60000 milliseconds in a minute
long second =  1000; // 1000 milliseconds in a second
unsigned long previousMillisPing = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 100;
const long intervalPing = 9999;

long lastLock;

long historyVal;
long lifetime = millis();


int lockHistory = 0;
int lockLoopStop = 1;
int pingPrints = 0;
int pingPrint = 0;

int pingPrintsCounter = 0;

int led3 = 3;
int led2 = 2; // LED connected to digital pin 13
int inPin = 7;   // pushbutton connected to digital pin 7
int val = 0;
int relay = 6;// variable to store the read value
int counterBool = 0;
int buzz = 1;
int passWord = 0;
int statusLed = 0;
int RFIDTimeout = 0;

void setup() {

  SPI.begin();
  pinMode(chipSelectPin,OUTPUT);              // Set digital pin 10 as OUTPUT to connect it to the RFID /ENABLE 

  digitalWrite(chipSelectPin, LOW);         // Activate the RFID reader
  pinMode(NRSTPD,OUTPUT);               
  // Set digital pin 10 , Not Reset and Power


  digitalWrite(NRSTPD, HIGH);
  myRFID.AddicoreRFID_Init();  


  
  // initialize serial:
  Serial.begin(115200);
  Serial.println("setup");
  pinMode(led2, OUTPUT);      // sets the digital pin 13 as output
  pinMode(led3, OUTPUT);      // sets the digital pin 13 as output
  pinMode(inPin, INPUT);      // sets the digital pin 7 as input
  pinMode(relay, OUTPUT);
  Serial.println("setup, done\nWelcome to ArdOS");
    Serial.println("enter password");

  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  ping.fire();

}

void loop() {

  
  
  // print the string when a newline arrives:
  if (stringComplete) {
    //----Serial.println(inputString); 
    // clear the string:
    inputString = "";
    stringComplete = false;
  }


  if (inputString == "***"){
     passWord = 0;
     buzzer();
     Serial.println("---------------------------------------");
     Serial.println("logged out, ? for help.");
     RFIDAuth = 0;
     delay(10);

  }

    if (inputString == "13323"){
      passWord = 1;
      buzzer();
     Serial.println("---------------------------------------");
     Serial.println("logged in as Simeon Ratliff,\ntype ? for help.\nyou now have admin powers");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     delay(10);
  }
  if (inputString == "ppoff"){
     Serial.println("---------------------------------------");
     Serial.println("turning off ping prints");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     pingPrint = 0;
     delay(10);
  }
  
    if (inputString == "-"){
     Serial.println("---------------------------------------");
     Serial.println("turning on ping prints");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     Serial.println("---------------------------------------");
     Serial.print("Microseconds: ");
        Serial.print(ping.microseconds());
        Serial.print(" | Inches ");
        Serial.print(ping.inches());
        Serial.print(" | Centimeters: ");
        Serial.print(ping.centimeters());
        Serial.println();
        pingTime();
        
     pingPrint = 1;
     delay(10);
  }
  
   if (inputString == ".4"){ //print all avail vals for debug
    
        Serial.println("---------------------------------------\n-----------------ADMIN-----------------\n---------------------------------------");
        Serial.println("passWord var...");
        Serial.println(passWord);
        Serial.println("statusLed var...");
        Serial.println(statusLed);
        Serial.println("lockHistory var..");
        Serial.println(lockHistory);
        Serial.println("lockLoopStop var...");
        Serial.println(lockLoopStop);
        Serial.println("val var...");
        Serial.println(val);
        Serial.println("RFIDAuth var...");        
        Serial.println(RFIDAuth);
        Serial.println("RFIDTimeout var...");
        Serial.println(RFIDTimeout);
        Serial.println("counterbool var...");        
        Serial.println(counterBool);
        Serial.println("buzz var...");
        Serial.println(buzz);
        
         Serial.println("pingPrints var...");
        Serial.println(pingPrints);
        Serial.println("passWord var...");

        Serial.println(passWord);
      
  }
  
    if (inputString == ".1"){ //print all avail vals for debug
      if(passWord == 1){
        Serial.println("---------------------------------------\n-----------------ADMIN-----------------\n---------------------------------------");
        Serial.println("passWord var...");
        Serial.println(passWord);
        Serial.println("statusLed var...");
        Serial.println(statusLed);
        Serial.println("lockHistory var..");
        Serial.println(lockHistory);
        Serial.println("lockLoopStop var...");
        Serial.println(lockLoopStop);
        Serial.println("val var...");
        Serial.println(val);
        Serial.println("RFIDAuth var...");        
        Serial.println(RFIDAuth);
        Serial.println("RFIDTimeout var...");
        Serial.println(RFIDTimeout);
        Serial.println("counterbool var...");        
        Serial.println(counterBool);
        Serial.println("buzz var...");
        Serial.println(buzz);
        Serial.println("passWord var...");

        Serial.println(passWord);
        } else { Serial.println("enter password"); delay(10);}
  }
  if (inputString == ".2"){
    if(passWord == 1){
      Serial.println("---------------------------------------\n-----------------ADMIN-----------------\n---------------------------------------");
      Serial.println(counterBool);
      Serial.println("counterbool var...");
      counterBool ++;
      Serial.println(counterBool);
      Serial.println("counterbool var new...");  
                                

    } else { Serial.println("enter password"); delay(10);}
  }
  if(inputString == ".3"){
    if(passWord == 1){
      Serial.println("---------------------------------------\n-----------------ADMIN-----------------\n---------------------------------------");
      Serial.println("System started at...");
      time();
      Serial.println("System locked at...");
      timeOfLock();
//      Serial.println("delaying to see if lock is still on...");
//      Serial.println("System started at...");
//      time();
//      Serial.println("System locked at...");
//      delay(1500);
//      timeOfLock();
//      Serial.println("If array 3 is difrent than 4, system is locked...");
    } else { Serial.println("enter password"); delay(10);}
  }
  if (inputString == "?"){
    Serial.println("---------------------------------------");
    Serial.println(".1 to print var");
    Serial.println(".2 to ++ -> counterBool");
    Serial.println("*** to log out");
  } 
  
  

  

    
  val = digitalRead(inPin);
    //Serial.println(val);
    //Serial.println(counterBool);
      

  if(val == 1){
    //thing
    RFIDTimeout = 1;
 
  }
  if(counterBool == 2){
    //thing
    counterBool= 0;
    buzz = 0;
    if(buzz == 0){
      tone(2, 500);
      delay(2000);
      noTone(2);
      buzz = 1;
    }
  }
  
  

  if(counterBool == 1){
    lastLock = millis();
    lastLock -= lifetime = historyVal;
    //Serial.println(historyVal);
    digitalWrite(relay, HIGH);
    statusLed = 1;
    if(buzz == 1){
      buzzer();
      buzz = 0;
      }
  } else { digitalWrite(relay, LOW);
    statusLed = 0;
    digitalWrite(led2, LOW);
    }


  if(statusLed == 1){
  digitalWrite(led3, HIGH);
  } else {
    digitalWrite(led3, LOW);
  }

//RFID--------------
  switch(RFIDAuth){

   case 1:
     counterBool++; 
     RFIDAuth = 0;
   break;
   case 2:  
    passWord = 1;
   break;
  
    
    
  }

unsigned long currentMillisPing = millis();
  if(currentMillisPing - previousMillisPing >= intervalPing) {
     //save the last time you blinked the LED 
   previousMillisPing = currentMillisPing;
    pingAlarm();

}



 if(RFIDTimeout == 1){
   unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {
     //save the last time you blinked the LED 
   previousMillis = currentMillis;   


  uchar i, tmp, checksum1;
  uchar status;
  uchar str[MAX_LEN];
  uchar RC_size;
  uchar blockAddr;
  //Selection operation block address 0 to 63
  String mynum = "";
  str[1] = 0x4400;
  //Find tags, return tag type
  status = myRFID.AddicoreRFID_Request(PICC_REQIDL, str);
  if (status == MI_OK)
  {
  Serial.println("---------------------------------------");
  Serial.println("RFID tag detected"
  );
  //Serial.print(str[0],BIN);
 // Serial.print(" , ");
  //Serial.print(str[1],BIN);
  //Serial.println(" ");
  //Serial.println("---------------------------------------");
  } 

  //Anti

  status = myRFID.AddicoreRFID_Anticoll(str);
  if (status == MI_OK)
  {
  checksum1 = str[0] ^ str[1] ^ str[2] ^ str[3];
  //Serial.println("The tag's number is  : ");
  //Serial.print(2);
  //Serial.print(str[0]);
  //Serial.print(" , ");
  //Serial.print(str[1],BIN);
  //Serial.print(" , ");
  //Serial.print(str[2],BIN);
  //Serial.print(" , ");
 // Serial.print(str[3],BIN);
  //Serial.print(" , ");
  //Serial.print(str[4],BIN);
  //Serial.print(" , ");
  //Serial.
  //println(checksum1,BIN);
  Serial.println("---------------------------------------");
  // Should really check all pairs, but for now we'll just use the first
  if(str[0] == 133 && str[1] == 42){
  Serial.print("Hello Simeon Ratliff!\n");
  Serial.println("---------------------------------------");
  RFIDAuth = 2;
  RFIDTimeout = 0;
  buzzer();
  Serial.println("Giving admin powers to you...");

  } else if(str[0] == 195 && str[1] == 154) {             //You can change this to the first byte of your tag by finding the card's ID 
  RFIDAuth = 1;
  RFIDTimeout = 0;
  Serial.print("Hello Simeon!\n");
  }
  Serial.println();
  }else 
  myRFID.AddicoreRFID_Halt();
  }


 }
//RFID--------------

  
}

















    /*
      SerialEvent occurs whenever a new data comes in the
     hardware serial RX.  This routine is run between each
     time loop() runs, so using delay inside loop can delay
     response.  Multiple bytes of data may be available.
     */
    void serialEvent() {
      while (Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read(); 
        // add it to the inputString:
        inputString += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
          stringComplete = true;
        } 
      }
    }



    void timeOfLock(){
      int days = lastLock / day ;                                //number of days
      int hours = (lastLock % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
      int minutes = ((lastLock % day) % hour) / minute ;         //and so on...
      int seconds = (((lastLock % day) % hour) % minute) / second;

      // digital clock display of current time
      Serial.print(days,DEC);  
      printDigitsLock(hours);  
      printDigitsLock(minutes);
      printDigitsLock(seconds);
      Serial.println();  
     
    }

    void printDigitsLock(byte digits){
      // utility function for digital clock display: prints colon and leading 0
      Serial.print(":");
      if(digits < 10)
      Serial.print('0');
      Serial.print(digits,DEC);  
    }

  void pingAlarm(){
      ping.fire();
      //func here finish with comp
  }



  void buzzer(){
    tone(2, 500);
      delay(200);
      noTone(2);
      delay(250);
      tone(2, 500);
      delay(200);
      noTone(2);
      delay(250);
      tone(2, 500);
      delay(200);
      noTone(2);

    
  }

 void pingTime(){
      long pingTimeNow = millis();
     
      int days = pingTimeNow / day ;                                //number of days
      int hours = (pingTimeNow % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
      int minutes = ((pingTimeNow % day) % hour) / minute ;         //and so on...
      int seconds = (((pingTimeNow % day) % hour) % minute) / second;

      // digital clock display of current time
      Serial.print(days,DEC);  
      pingPrintDigits(hours);  
      pingPrintDigits(minutes);
      pingPrintDigits(seconds);
      Serial.println();  
     
    }

    void pingPrintDigits(byte digits){
      // utility function for digital clock display: prints colon and leading 0
      Serial.print(":");
      if(digits < 10)
      Serial.print('0');
      Serial.print(digits,DEC);  
    }

    void time(){
      long timeNow = millis();
     
      int days = timeNow / day ;                                //number of days
      int hours = (timeNow % day) / hour;                       //the remainder from days division (in milliseconds) divided by hours, this gives the full hours
      int minutes = ((timeNow % day) % hour) / minute ;         //and so on...
      int seconds = (((timeNow % day) % hour) % minute) / second;

      // digital clock display of current time
      Serial.print(days,DEC);  
      printDigits(hours);  
      printDigits(minutes);
      printDigits(seconds);
      Serial.println();  
     
    }

    void printDigits(byte digits){
      // utility function for digital clock display: prints colon and leading 0
      Serial.print(":");
      if(digits < 10)
      Serial.print('0');
      Serial.print(digits,DEC);  
    }

     
