// +--------------------------------+
// | Wi-Fi Controlled 4WD Robot Car |
// +--------------------------------+
//
// (c) 2021, 2023, 2025 Daniel Faria Muniz (a.k.a. dfmuniz74, formerly doctorxyz)
//
// Based on the following references:
// - Guide for assembling the 4WD robotic platform (https://blog.eletrogate.com/guia-de-montagem-da-plataforma-robotica-4wd/)
// - Controlling motors via WiFi using ESP Motor Shield (https://blog.eletrogate.com/esp-motor-shield-controlando-motores-via-wifi/)
// - 11 Steps to build a WiFi car using NodeMCU (https://circuitbest.com/arduino-smartphone-controlled-wifi-car-using-nodemcu-esp8266/)
// - Smartphone-controlled Arduino 4WD robot car (https://www.hackster.io/andriy-baranov/smartphone-controlled-arduino-4wd-robot-car-14d239)
// - 360-degree turning of a 4WD chassis without encoders (https://www.codeproject.com/Articles/1255314/Turning-360-degrees-a-4WD-chassis-without-any-enco)
// - Difference between L293D and L298N motor drivers (https://robu.in/what-is-the-difference-between-l293d-and-l298n-motor-driver/)
// - How to make a WIFI control car with the new Blynk app step by step (https://srituhobby.com/how-to-make-a-wifi-control-car-with-the-new-blynk-app-step-by-step/)
// - [SOLVED] Help me how to control joystick (https://community.blynk.cc/t/help-me-how-to-control-joystick/57160/2)

// Blynk Data Streams https://ny3.blynk.cloud/dashboard/(...))/datastreams
// Virtual PIN -> Name – X / PIN – V0 / MIN – 0 / MAX – 100
// Virtual PIN -> Name – Y / PIN – V1 / MIN – 0 / MAX – 100

#define BLYNK_PRINT Serial	// Enable serial monitor debugging
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Motor control pin definitions
#define RightMotorSpeed 5
#define RightMotorDir   0 
#define LeftMotorSpeed  4
#define LeftMotorDir    2

// Blynk authentication token and WiFi credentials
char auth[] = "YOUR_BLYNK_AUTH_TOKEN"; // Replace with your Blynk authentication token
char ssid[] = "YOUR_WIFI_SSID";        // Replace with your WiFi network name
char pass[] = "YOUR_WIFI_PASSWORD";    // Replace with your WiFi password

// Variables for the Blynk widget values
int x = 128;
int y = 128;

// Joystick control ranges
int minRange = 256;
int maxRange = 767;
int minSpeed = 450;
int maxSpeed = 1020;
int noSpeed = 0;

void drive(int x, int y){

  // Move forward
  if(y >= maxRange && x >= minRange && x <= maxRange){
    digitalWrite(RightMotorDir,HIGH); 
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // Move forward and turn right
  else if(x >= maxRange && y >= maxRange){
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,minSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // Move forward and turn left
  else if(x <= minRange && y >= maxRange){
    digitalWrite(RightMotorDir,HIGH);
    digitalWrite(LeftMotorDir,HIGH);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,minSpeed);
  }

  // Stop (neutral position)
  else if(y < maxRange && y > minRange && x < maxRange && x > minRange){
    analogWrite(RightMotorSpeed,noSpeed);
    analogWrite(LeftMotorSpeed,noSpeed);
  }

  // Move backward
  else if(y <= minRange && x >= minRange && x <= maxRange){
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed);
  }

  // Move backward and turn right
  else if(y <= minRange && x <= minRange){
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,maxSpeed);
    analogWrite(LeftMotorSpeed,minSpeed);
  }

  // Move backward and turn left
  else if(y <= minRange && x >= maxRange){
    digitalWrite(RightMotorDir,LOW);
    digitalWrite(LeftMotorDir,LOW);
    analogWrite(RightMotorSpeed,minSpeed);
    analogWrite(LeftMotorSpeed,maxSpeed); 
  }
}


void setup(){

  Serial.begin(9600); // Initialize serial communication
  Blynk.begin(auth, ssid, pass); // Connect to Blynk cloud service
 
  // Set motor control pins as outputs
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  
  // Initial motor setup: stop movement, set direction forward
  digitalWrite(RightMotorSpeed, LOW);
  digitalWrite(LeftMotorSpeed, LOW);
  digitalWrite(RightMotorDir, HIGH);
  digitalWrite(LeftMotorDir,HIGH);

  // Initialize the Blynk library
  Blynk.begin(auth, ssid, pwd, "blynk.cloud", 80);

}

// Get the joystick values
BLYNK_WRITE(V0){
  x = param[0].asInt();
}

BLYNK_WRITE(V1){
  y = param[0].asInt();
}

void loop(){
   Blynk.run(); // Process Blynk events
}

// Handle joystick input from Blynk app
BLYNK_WRITE(V1){
  int x = param[0].asInt(); // Read x-axis value
  int y = param[1].asInt(); // Read y-axis value
  drive(x,y); // Control movement based on joystick input
}















