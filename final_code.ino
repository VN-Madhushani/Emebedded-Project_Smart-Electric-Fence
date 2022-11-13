#include <Wire.h> 
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h> 
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(4, 5); 

#define relay1 10       // the pin that the LED is atteched to
#define relay2 11
#define voltsensor A0
#define buzzer 13

int pirsensor = 2;              // the pin that the sensor is atteched to
int state = LOW;             // by default, no motion detected
bool sendMsg=true;
int val = 0;                 // variable to store the sensor status (value)
int read_ADC;
int distance;

byte symbol[8] = {
        B00000,
        B00100,    
        B00100,
        B00100,
        B11111,
        B01110,
        B00100,
        B00000};


void setup() {
  lcd.begin();
  lcd.backlight();
  lcd.setBacklight(HIGH);
  Serial.begin(9600);            // initialize serial
  pinMode (pirsensor,INPUT); 
  pinMode(voltsensor,INPUT);     // initialize sensor as an input
  pinMode(relay1, OUTPUT);      // initalize LED as an output
  pinMode(relay2, OUTPUT);    // initialize sensor as an input
  pinMode(buzzer, OUTPUT);
  lcd.createChar(1, symbol);
  lcd.clear();
  lcd.setCursor(3, 0); // set the cursor to column 0, line 2
  lcd.print("Wild Life ");
  lcd.setCursor(3, 1); // set the cursor to column 0, line 2
  lcd.print("Sri Lanka");
  delay(2000);
  lcd.clear();
}

void loop(){
  PIR();  
}

void PIR(){
  val = digitalRead(pirsensor);   // read sensor value
  if (val == HIGH) {           // check if the sensor is HIGH
    if (state == LOW) {
      Serial.println("Motion detected!"); 
      state = HIGH;       // update variable state to HIGH
    }
  } 
  else {  
    if (state == HIGH){
      Serial.println("Motion stopped!");
      state = LOW;
      sendmsg();
      sendMsg=true;
       // update variable state to LOW
    }
  }
  if(state==HIGH){
    fault();}

  
}
void fault(){

  lcd.setCursor(4,0);
  lcd.print("L1");
  lcd.write(1);

  lcd.setCursor(10,0);
  lcd.print("L2");
  lcd.write(1);
        
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,LOW);  
  delay(1000);

  data();
  lcd.setCursor(4,1);
  if(distance>0){lcd.print(distance); lcd.print("KM  ");}
  else{lcd.print(" NF ");}

  digitalWrite(relay1,LOW);
  digitalWrite(relay2,HIGH); 
  delay(1000);

  data();

  lcd.setCursor(9,1);
  if(distance>0){lcd.print(distance); lcd.print("KM  ");}
  else{lcd.print(" NF ");}

}
void sendmsg(){
  //Serial.begin(9600);
   

  Serial.println("Initializing...");
   delay(1000);

   mySerial.println("AT");  //Once the handshake test is successful, it will back to OK
   updateSerial();

  mySerial.println("AT+CMGF=1");  // Configuring TEXT mode
  updateSerial();
    mySerial.println("AT+CMGS=\"+94712751532\"");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
    updateSerial();
   mySerial.print("Line break at: ");  //text content
    updateSerial();
   mySerial.print(distance);  //text content
   mySerial.write(26);
}
 
  void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
  
  

void data(){
  read_ADC = analogRead(voltsensor);
  Serial.print(read_ADC);
  Serial.print('\n');
  delay(500); 
  distance = read_ADC/100;
  Serial.print("Distance=");
  Serial.println(distance);

  if(distance>9)distance = 0;

  if(distance>0){
  digitalWrite(buzzer,HIGH);
  delay(200);
  digitalWrite(buzzer,LOW);  
  delay(200);
    if(sendMsg){
      //function
      sendMsg=false;
    }
  }

} 
