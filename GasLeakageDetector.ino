#include <SoftwareSerial.h>  //GSM Library

int Buzzer = 6; //Buzzer connected to Arduino pin D6
int MQ_2 = 7; //MQ_2 Connected to Arduino pin 7
int Sent = 0; //Tracking Variable to track weather sms was sent or not
                    //tx and RX
SoftwareSerial mySerial(4, 5); //Initializing the GSM pins and telling the Arduino where your have connected the RC and TX Pin to it

void setup() {
  Serial.begin(9600); //initializing the Serial Monitor to run on that Baud Rate
  mySerial.begin(9600); //Initializing the GSM Module to run on that Baud Rate

  pinMode(Buzzer, OUTPUT); //We are the setting the Buzzer as an OUTPUT Device hence will take two parameters either High or LOW
  pinMode(MQ_2, INPUT);  //We are setting the Gas Sensor as an INPUT so that the Arduino can Read data from that pin

  mySerial.println("AT");  //Once the handshake test is successful, it will back to OK
  updateSerial();
  delay(1000);
}

void loop() {
  int GasValue = digitalRead(MQ_2); //The arduino reads Gas Sensor Pin data and store it to the GasValue Variable

  if (GasValue == 0) { //Here the microcontroller checks the GasValue Variable if it matches with 0, and if its true then it execute what is in the loop
    Serial.println("Gas Detected");
    digitalWrite(Buzzer, HIGH);
    if (Sent == 0) {
      mySerial.println("AT+CMGF=1");  // Configuring TEXT mode
      updateSerial();
      mySerial.println("AT+CMGS=\"+254708264308\"");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
      updateSerial();
      mySerial.print("Gas Leakage Detected");  //text content
      updateSerial();
      mySerial.write(26);
      Sent  = 1;
    }
    delay(1000);
  } else if (GasValue == 1) {
    Sent = 0;
    Serial.println("Gas Not Detected");
    digitalWrite(Buzzer, LOW);
    delay(1000);
  }
}
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}