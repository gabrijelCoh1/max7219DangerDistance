#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10
#define scrollSpeed  70   // scroll speed
#define scrollPause  0    // ms of pause after finished displaying message

// Hardware SPI connection
MD_Parola P = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// sets scrolling direction if slider in middle at start
textEffect_t scrollEffect = PA_SCROLL_LEFT;
textPosition_t scrollAlign = PA_RIGHT;  // how to align the text

#define  BUF_SIZE  75  // Maximum of 75 characters
#define trigPin  4
#define echoPin  5
#define buzzerPin 2
#define maxDistance 100  //max distance in cm
#define dangerDistance 5  //danger distance in cm

char testStr[10] = { "" };
String heightStr = "";
long duration;
int distance;
int height;

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  P.begin();
}

void loop() {
  measure();

  if (distance >= maxDistance) {
    P.displayText("e", PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
    noTone(buzzerPin);
  } else if(distance <= dangerDistance){
    P.displayText("DANGER", PA_LEFT, 0, 0, PA_PRINT, PA_NO_EFFECT);
    tone(buzzerPin, 100);

  }
   else {
    height = distance;
    heightStr = String(height, DEC) + "cm";
    heightStr.toCharArray(testStr, 75);
    P.displayText(testStr, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    noTone(buzzerPin);
  }
  P.displayAnimate();
  delay(500);
}

void measure() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.017;

}
