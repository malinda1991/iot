#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int trigPin = 9;  
const int echoPin = 10; 
const int carDistanceThresholdCm = 30;

float duration, distance;  

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

void broadcastMessage(String message){
  Serial.println(message); 
  radio.write(&message, sizeof(message));
  Serial.println("Radio message sent!"); 
}

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);  
	pinMode(echoPin, INPUT);  
	Serial.begin(9600);  
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);  
	delayMicroseconds(2);  
	digitalWrite(trigPin, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(trigPin, LOW);  
  duration = pulseIn(echoPin, HIGH);  
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");  
	Serial.println(distance);   

  String message;
  if(distance < carDistanceThresholdCm){
    // car is in the garage
    message = "Car is in the garage :GREEN";
  }else{
    // car is not in the garage
    message = "Car is not in the garage :RED"; 
  }

  broadcastMessage(message);

  delay(5000); 
}
