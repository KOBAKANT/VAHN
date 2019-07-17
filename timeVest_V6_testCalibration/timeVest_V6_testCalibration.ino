////////////////////////////////////////
// CODE for TIME TRAVEL
// VON HIER AN NEU
// KOBAKANT time-travel costume:
// --- time vest
// --- confilct antenna
// --- happiness brush
////////////////////////////////////////

#include <Adafruit_NeoPixel.h>
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

////////////////////////////
//////ADJUSTABLE VALUES/////
////////////////////////////

// THRESHOLDS and TRIGGERS:
#define CONFLICTvalueMIN_original 1023
#define CONFLICTvalueMAX_original 0

int countFLIP = 0;
#define flipTIME 300// cycles
#define adjustINC 0.1
#define CONFLICTrangeCAP 100
#define HAPPYrangeCAP 200

#define HAPPYvalueLDRmin_original 1023
#define HAPPYvalueLDRmax_original 0

#define HAPPYvalueFSRmin 380
#define HAPPYvalueFSRmax 900

#define TIMEtravelTRIGGER 700

// LED BRIGHTNESSes (0=off, 255=max):
#define timeLEDbrightness 50
#define conflictLEDbrightness 255
#define happyLEDbrightness 100

// TIME TRAVEL stuff:
#define TIMEtravelDURATION 1000 //cycles
int years[] = {2019, 2020, 2024, 2039};
#define timeOUTbetweenTRAVELS 1 //seconds - recommended to be at least 10 seconds, less for debugging

//////////////////////
///PIN CONNECTIONS////
//////////////////////
#define TIME_TRAVELpin A9  //23
#define SPEAKERpin 12
int CONFLICT_LDRpin = A6;  //20
int HAPPY_LDRpin = A8;     //22
#define HAPPY_FSRpin A7     //21

#define TIME_LED_PIN    8
#define CONFLICT_LED_PIN    10
#define HAPPY_LED_PIN    9

//segment display SDA --> 18
//segment display SCL --> 19

////////////////////////
///LED NUMBERS//////////
////////////////////////
#define TIME_LED_COUNT 75
#define CONFLICT_LED_COUNT 3
#define HAPPY_LED_COUNT 28

Adafruit_NeoPixel TIMEstrip(TIME_LED_COUNT, TIME_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel CONFLICTstrip(CONFLICT_LED_COUNT, CONFLICT_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel HAPPYstrip(HAPPY_LED_COUNT, HAPPY_LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_7segment matrix = Adafruit_7segment();

//////////////////////
///VARIABLES//////////
//////////////////////
int CONFLICTvalue;
int CONFLICTcolour;
float CONFLICTvalueMIN = 0;
float CONFLICTvalueMAX = 1023;
float HAPPYvalueLDRmin = 0;
float HAPPYvalueLDRmax = 1023;
int TIMEvalue;
int TIMEsound;
int yearNOW = years[0];
int timesTRAVELLED = 0;
int timeCOUNT = 0;
int HAPPYvalueLDR;
int HAPPYvalueFSR;
int HAPPYcolour;
int HAPPYlevel;
int HAPPYlevel_TIME;
int CONFLICTlevel_TIME;
int CONFLICTfade = 1;

int avgCONFLICT;
int avgHAPPY;


void setup() {
  Serial.begin(9600);
  pinMode(CONFLICT_LDRpin, INPUT);//4.7K pullup
  pinMode(HAPPY_LDRpin, INPUT_PULLUP);
  pinMode(HAPPY_FSRpin, INPUT); //3.3K pullup
  pinMode(TIME_TRAVELpin, INPUT); //1MOhm pullup
  pinMode(SPEAKERpin, OUTPUT);

  TIMEstrip.begin();
  TIMEstrip.show();
  TIMEstrip.setBrightness(timeLEDbrightness);

  CONFLICTstrip.begin();
  CONFLICTstrip.show();
  CONFLICTstrip.setBrightness(conflictLEDbrightness);

  HAPPYstrip.begin();
  HAPPYstrip.show();
  HAPPYstrip.setBrightness(happyLEDbrightness);

  matrix.begin(0x70);
  matrix.setBrightness(15);

  //DISPLAY:
  matrix.print(yearNOW);
  matrix.writeDisplay();

  // set initial avarage
  avgCONFLICT = analogRead(CONFLICT_LDRpin);
  avgHAPPY = analogRead(HAPPY_LDRpin);
}




void loop() {
  //   countFLIP++;
  //  if (countFLIP % flipTIME == 0) {
  //    CONFLICT_LDRpin = A8;
  //    HAPPY_LDRpin = A6;
  //    Serial.println("hhhhhhhhhdskfjdksljfdklasjdkls");
  //  }
  //  if (countFLIP % (flipTIME * 2) == 0) {
  //    CONFLICT_LDRpin = A6;
  //    HAPPY_LDRpin = A8;
  //  }

  TIMEvalue = analogRead(TIME_TRAVELpin);

  //TIME TRAVEL INITIATED:
  if (TIMEvalue < TIMEtravelTRIGGER && (millis() - timeCOUNT) > timeOUTbetweenTRAVELS * 1000) {
    timeTRAVEL();
  }

  //NO TIME TRAVEL//SENSING THE WORLD:
  else {
    TIMEsound = 0;
    SENSEtheWORLD();
    noTone(SPEAKERpin);
  }

  //debug();
  delay(1); // THIS DELAY AFFECTS TIMING OF SOME OF THE SEQUENCES!!!!
}




//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///////////////////////TIME TRAVEL////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void timeTRAVEL() {

  ////////////////////////////////
  ///////LED LIGHT EFFECT/////////
  ////////////////////////////////
  for (int i = 0; i < TIME_LED_COUNT; i++) {
    if (millis() % 2 == 0) {
      TIMEstrip.setPixelColor(i, TIMEstrip.Color(0, 0, 255));
    }
    else TIMEstrip.setPixelColor(i, TIMEstrip.Color(0, 0, 0));
  }
  TIMEstrip.show();

  ////////////////////////////////
  ////////SOUND EFFECT////////////
  ////////////////////////////////
  TIMEsound++;
  if (TIMEsound > TIMEtravelDURATION) timeJump();
  //Serial.println(TIMEsound);
  tone(SPEAKERpin, TIMEsound);
}


////////////////////////////////
///////time jump effect/////////
////////////////////////////////
void timeJump() {
  for (int i = 0; i < 50; i++) {
    TIMEsound = random (100, 2000);
    tone(SPEAKERpin, TIMEsound);
    int randomDisplay = random(1000, 9999);
    matrix.print(randomDisplay);
    matrix.writeDisplay();
    delay(100);
  }
  timesTRAVELLED++;
  timeCOUNT = millis();

  //DISPLAY:
  yearNOW = years[timesTRAVELLED];
  matrix.print(yearNOW);
  matrix.writeDisplay();
}
////////////////////////////////
////////////////////////////////



void debugCONFLICT() {
  Serial.print(1023);
  Serial.print("\t");
  Serial.print(CONFLICTvalue);
  Serial.print("\t");
  Serial.print(CONFLICTvalueMIN);
  Serial.print("\t");
  Serial.print(CONFLICTvalueMAX);
  Serial.print("\t");
  Serial.print(0);
  Serial.println();
}



void debugHAPPY() {
  Serial.print(1023);
  Serial.print("\t");
  Serial.print(HAPPYvalueLDR);
  Serial.print("\t");
  Serial.print(HAPPYvalueLDRmin);
  Serial.print("\t");
  Serial.print(HAPPYvalueLDRmax);
  Serial.print("\t");
  Serial.print(0);
  Serial.println();
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///////////////////////SENSE THE WORLD////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void SENSEtheWORLD() {

  //////////////////////////////////////////////////////////////////////
  ///////////////////////////////CONFLICT///////////////////////////////
  //////////////////////////////////////////////////////////////////////
  // READ: CONFLICT values
  CONFLICTvalue = analogRead(CONFLICT_LDRpin);
  CONFLICTvalue = (avgCONFLICT * 3 + CONFLICTvalue) / 4;
  avgCONFLICT = CONFLICTvalue;
  //CONFLICTvalue = (CONFLICTvalue * 3 + (1023-HAPPYvalueLDR) ) / 4;

  if (CONFLICTvalue > CONFLICTvalueMAX) CONFLICTvalueMAX = CONFLICTvalue;
  if (CONFLICTvalue < CONFLICTvalueMIN) CONFLICTvalueMIN = CONFLICTvalue;

  if (CONFLICTvalueMAX > CONFLICTvalue + CONFLICTrangeCAP)CONFLICTvalueMAX = CONFLICTvalueMAX - adjustINC;
  if (CONFLICTvalueMIN < CONFLICTvalue - CONFLICTrangeCAP)CONFLICTvalueMIN = CONFLICTvalueMIN + adjustINC;

  debugCONFLICT();

  // MAP: CONFLICT --> COLOUR
  CONFLICTcolour = map(CONFLICTvalue, CONFLICTvalueMIN, CONFLICTvalueMAX, 0, 255);
  CONFLICTcolour = (CONFLICTcolour * 3 + (HAPPYcolour) ) / 4;
  CONFLICTcolour = constrain(CONFLICTcolour, 0, 255);

  // WRITE: SENSOR LEDs
  for (int i = 0; i < CONFLICT_LED_COUNT; i++) {
    CONFLICTstrip.setPixelColor(i, CONFLICTstrip.Color(CONFLICTcolour,   255 - CONFLICTcolour, 0));
  }
  //  if (conflictLEDbrightness > 255) CONFLICTfade = -10;
  //  if (conflictLEDbrightness < 0) CONFLICTfade = 10;
  //  conflictLEDbrightness = conflictLEDbrightness + CONFLICTfade;
  //  CONFLICTstrip.setBrightness(conflictLEDbrightness);
  CONFLICTstrip.show();


  //////////////////////////////////////////////////////////////////////
  ///////////////////////////////HAPPY///////////////////////////////
  //////////////////////////////////////////////////////////////////////
  // READ: HAPPY values
  HAPPYvalueLDR = analogRead(HAPPY_LDRpin);
  HAPPYvalueFSR = analogRead(HAPPY_FSRpin);

  HAPPYvalueLDR = (avgHAPPY * 9 + HAPPYvalueLDR) / 10;
  avgHAPPY = HAPPYvalueLDR;

  CONFLICTvalue = analogRead(CONFLICT_LDRpin);
  if (HAPPYvalueLDR > HAPPYvalueLDRmax) HAPPYvalueLDRmax = HAPPYvalueLDR;
  if (HAPPYvalueLDR < HAPPYvalueLDRmin) HAPPYvalueLDRmin = HAPPYvalueLDR;

  if (HAPPYvalueLDRmax > HAPPYvalueLDR + HAPPYrangeCAP)HAPPYvalueLDRmax = HAPPYvalueLDRmax - adjustINC;
  if (HAPPYvalueLDRmin < HAPPYvalueLDR - HAPPYrangeCAP)HAPPYvalueLDRmin = HAPPYvalueLDRmin + adjustINC;

  //  HAPPYvalueLDRmax -= 1;
  //  HAPPYvalueLDRmin += 1;
  //  if (HAPPYvalueLDRmax < HAPPYvalueLDRmax_original)HAPPYvalueLDRmax = HAPPYvalueLDRmax_original;
  //  if (HAPPYvalueLDRmin > HAPPYvalueLDRmin_original)HAPPYvalueLDRmin = HAPPYvalueLDRmin_original;
  //debugHAPPY();

  // MAP: HAPPY --> COLOUR
  HAPPYcolour = map(HAPPYvalueLDR, HAPPYvalueLDRmax, HAPPYvalueLDRmin, 0, 255);
  HAPPYcolour = constrain(HAPPYcolour, 0, 255);

  // MAP: HAPPY --> LEVEL
  HAPPYlevel = map(HAPPYvalueFSR, HAPPYvalueFSRmax, HAPPYvalueFSRmin, 0, HAPPY_LED_COUNT);
  HAPPYlevel = constrain(HAPPYlevel, 0, HAPPY_LED_COUNT);

  // WRITE: SENSOR LEDs
  for (int i = 0; i < HAPPY_LED_COUNT; i++) {
    if (i < HAPPYlevel) {
      HAPPYstrip.setPixelColor(HAPPY_LED_COUNT - i, HAPPYstrip.Color(HAPPYcolour, 255 - HAPPYcolour, 0));
    }
    else {
      HAPPYstrip.setPixelColor(HAPPY_LED_COUNT - i, HAPPYstrip.Color(0, 0, 0));
    }
  }
  HAPPYstrip.show();

  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////SENSOR DISPLAY//////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  // WRITE: TIME LEDs for HAPPINESS and CONFLICT
  HAPPYlevel_TIME = map(HAPPYvalueLDR, HAPPYvalueLDRmin, HAPPYvalueLDRmax, 0, TIME_LED_COUNT / 3);
  HAPPYlevel_TIME = constrain(HAPPYlevel_TIME, 0, TIME_LED_COUNT / 3);
  CONFLICTlevel_TIME = map(CONFLICTvalue, CONFLICTvalueMIN, CONFLICTvalueMAX , 2 * TIME_LED_COUNT / 3, TIME_LED_COUNT);
  CONFLICTlevel_TIME = constrain(CONFLICTlevel_TIME, 2 * TIME_LED_COUNT / 3, TIME_LED_COUNT);

  for (int i = 0; i < TIME_LED_COUNT; i++) {

    if (i < HAPPYlevel_TIME) {
      TIMEstrip.setPixelColor( (TIME_LED_COUNT / 3) - i, TIMEstrip.Color(0, 255, 0));
    }
    else TIMEstrip.setPixelColor( (TIME_LED_COUNT / 3) - i, TIMEstrip.Color(0, 0, 0));


    if (i > TIME_LED_COUNT / 3 && i < CONFLICTlevel_TIME) {
      TIMEstrip.setPixelColor(i, TIMEstrip.Color(255, 0, 0));
    }
    else if (i > TIME_LED_COUNT / 2 && i > CONFLICTlevel_TIME) {
      TIMEstrip.setPixelColor(i, TIMEstrip.Color(0, 0, 0));
    }
  }
  TIMEstrip.show();



  /// experiments:

  //SET COLOUR:
  //  for (int i = 0; i < 256; i++) {
  //    colourValue=i;
  //  delay(100);
  //  }
  //  for (int i = 0; i < 4; i++) {
  //    //strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(colourValue)));
  //    //strip.setPixelColor(i, strip.ColorHSV(colourValue));
  //    strip.setPixelColor(i, strip.Color(colourValue,   255 - colourValue, 0));
  //  }
  //  strip.show(); // Update strip with new contents

}

////////////////////////////////
///////debug////////////////////
////////////////////////////////

void debug() {
  Serial.print("TIME: ");
  Serial.print(TIMEvalue);
  Serial.print("\t");
  Serial.print("CONFLICT: ");
  Serial.print(CONFLICTvalue);
  Serial.print("\t");
  Serial.print("c: ");
  Serial.print(CONFLICTcolour);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("HAPPYldr: ");
  Serial.print(HAPPYvalueLDR);
  Serial.print("\t");
  Serial.print("fsr: ");
  Serial.print(HAPPYvalueFSR);
  Serial.print("\t");
  Serial.print("\t");
  Serial.print("c: ");
  Serial.print(HAPPYcolour);
  Serial.print("\t");
  Serial.print("l: ");
  Serial.println(HAPPYlevel);
}
