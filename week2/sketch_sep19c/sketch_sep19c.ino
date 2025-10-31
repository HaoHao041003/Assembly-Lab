const int vibrate_pin = 2;
const int switch_pin = 11;
const int button_pin = 7;
int buttonState = 0;
int isShinning = 0;
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(vibrate_pin, INPUT);
  pinMode(switch_pin, OUTPUT);
}

void loop() {
  buttonState = digitalRead(button_pin);
  // put your main code here, to run repeatedly:
  if(digitalRead(vibrate_pin) == HIGH || isShinning == 1){
    digitalWrite(switch_pin, HIGH);
    delay(100);
    digitalWrite(switch_pin, LOW);
    delay(100);
  }

  if(buttonState == LOW && isShinning == 0){
    isShinning = 1;
    delay(500);
  }
  else if(buttonState == LOW && isShinning == 1){
    isShinning = 0;
    delay(500);
  }

}
