// arduino automatic light
#define reference 50
#define waitingTime 5000
#define ldr A1
#define led 11
#define touch 6
#define blinkInterval 200  //200ms
#define off 0
#define on 1
#define offWarning 5000
#define debug 0
bool ison = 1,ledon=1;
int value = 255;
uint8_t tapCounter = 0;
int current_reference = 0, timeEscaped = 0;

unsigned long pre, pressedat, lasttimepressed, curr, pressedfor;
void setup() {
  if(debug){ Serial.begin(9600);}
  pinMode(led, OUTPUT);
}

int avg() {
  long sum = 0;
  for (int i = 0; i < 50; i++) {
    sum = sum + analogRead(ldr);
    delay(1);
  }
  int r= (sum / 50);

   Serial.print(r);
  Serial.print("  ");
  Serial.println(analogRead(ldr));
  return r;
}
void turnOff() {
 bool blinkState=1;
  Serial.println("turning off");
 /* for (int i = 0; i < 5; i++) {
    digitalWrite(led, off);
    delay(100);
    digitalWrite(led, on);
    delay(200);
  }
  digitalWrite(led, on);*/
unsigned long lastBlink, currentBlinkTime;
  pre = millis();
  while ( (millis()-pre) < offWarning) {

    if (digitalRead(touch) == 1) {
      ison = true;
      Serial.println("Turned on");
      return;
    }
    currentBlinkTime=millis();
    if (currentBlinkTime-lastBlink>blinkInterval){
      blinkState=!blinkState;
     analogWrite(led, max(0, value - (50 * blinkState))); // value will be 50 less if blinkState is 1
      lastBlink=currentBlinkTime;
    
    }
  }
  digitalWrite(led,0);
  Serial.println("Led off");
  ledon=0;

}
void lamp() {
 
  if (avg() > 800) {
    ledon=1;
    analogWrite(led, value);
    Serial.println("Led on");
    delay(500);
    current_reference = avg();
  }
   if(!ledon){return;}
 
  // delay(1000);
  // delay(5000);
  while (avg() < current_reference - reference) {
    delay(500);
    timeEscaped++;
    if (timeEscaped > 5) {
      turnOff();

      timeEscaped = 0;
      break;
    }
  }
}


int8_t sign = -1;
void loop() {
  pressedfor = 0;

Serial.println(ison);
 // delay(1000);
  if (ison == true) {
    lamp();
    curr = millis();
    while (digitalRead(touch) == 1 && pressedfor <= 777) {  // exist the loop after 1 second
      pressedat = millis();
      pressedfor = millis() - curr;
    }  // now we know when pressed and how much pressed
    if (pressedfor > 50 && pressedfor < 500) {
      ison = false;
      turnOff();
    }

    while (digitalRead(touch) == 1 && pressedfor > 776) {  //tapcounter indicate no. of short tapped

      if (sign == -1) {
        sign = 1;
        while (digitalRead(touch) == 1) {
          if (value < 255) {  // prevent value overflowing;
            value++;
            analogWrite(led, value);
            Serial.println(value);
            delay(15);
          }
        }
      } else if (sign == 1) {
        sign = -1;
        while (digitalRead(touch) == 1) {
          if (value > 20) {  // prevent value overflowing;
            value--;
            analogWrite(led, value);
            Serial.println(value);
            delay(15);
          }
        }
      }
    }


  } else {
    curr = millis();
    while (digitalRead(touch) == 1) {
      pressedfor = millis() - curr;
    }
    if (pressedfor > 30 && pressedfor < 500) {
      ison = 1;
      Serial.println("Turned on1");

      delay(500);
    }
  }
}

