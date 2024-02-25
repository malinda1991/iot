
const int magnetSensor = 4;
const int acRelay = 9;

int state;

void setup() {
  // put your setup code here, to run once:
  pinMode(magnetSensor, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  state = digitalRead(magnetSensor);

  if(state == HIGH){
    // magnet switch is open
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(acRelay, LOW);
  }else{
    // magnet switch is closed
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(acRelay, HIGH);
  }
  
  
  
}
