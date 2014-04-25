#include <Adafruit_NeoPixel.h>
 
#define PIN 4
#define STRIPSIZE 24
 

#define VCC  16
#define OUT  15
#define S2   14
#define S3   13
#define GND  12
#define S1   8
#define S0   7

#define  Filters_R  0
#define  Filters_G  1
#define  Filters_B  2

long count = 0;
int counter = 0;
int G_flag = 1;
int RGB = 0;
int RGB_val[3];
float scaleFactor[3] = {0.626637039, 0.655291881, 0.768657121 };
 
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
 
void setup() {
  
  pinMode(OUT, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(GND, OUTPUT);
 
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);
  
  
  digitalWrite(GND, LOW);
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    RGB=i;
    Select_Filters(i);
    count=0;
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    /*
    Serial.print("value: ");
    Serial.println(scaleFactor[i]*count);
    Serial.print("counter ");
    Serial.println(counter);
    */
    RGB_val[i] = scaleFactor[i]*count;
    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);  
  
  
strip.begin();
strip.setBrightness(5); // Lower brightness and save eyeballs!
strip.show(); // Initialize all pixels to 'off'
}
 
void loop() {
// Some example procedures showing how to display to the pixels:


//colorWipe(strip.Color(0,0,0), 25); // Black
/*
colorWipe(strip.Color(64, 0, 0), 25); // Red
colorWipe(strip.Color(0, 64, 0), 25); // Green
colorWipe(strip.Color(0, 0, 64), 25); // Blue
colorWave(25);
colorWipe(strip.Color(0,0,0), 25); // Black

rainbow(15);

colorWipe(strip.Color(0,0,0), 100); // Black
rainbowCycle(15);

*/

if ( RGB_val[0] < 10 && RGB_val[1] < 10 && RGB_val[2] < 10 ) {
  colorWave(30);
  rainbow(30);
  
}
else {
  
  colorChase(strip.Color(RGB_val[0], RGB_val[1], RGB_val[2]), 5, 65);
  colorChaseReverse(strip.Color(RGB_val[0], RGB_val[1], RGB_val[2]), 5, 65);
  colorWipe(strip.Color(RGB_val[0], RGB_val[1], RGB_val[2]), 50);
  colorWipe(strip.Color(0,0,0), 50); // Black
  colorWipeReverse(strip.Color(RGB_val[0], RGB_val[1], RGB_val[2]), 50);
  colorWipeReverse(strip.Color(0,0,0), 50); // Black

}


}

void colorChase(uint32_t c, int maxDots, uint8_t wait) {
  for (int a=0; a<maxDots; a++) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      if (i>a) {
        strip.setPixelColor((i-a-1), strip.Color(0,0,0));
      }
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  }
}

void colorChaseReverse(uint32_t c, int maxDots, uint8_t wait) {
  for (int a=0; a<maxDots; a++) {
    for(int i=strip.numPixels()-1; i>=0; i--) {
      if ((strip.numPixels()-i)>(a+1)) {
        strip.setPixelColor((i+a+1), strip.Color(0,0,0));
      }
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  }
}
 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
for(uint16_t i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, c);
strip.show();
delay(wait);
}
}

void colorWipeReverse(uint32_t c, uint8_t wait) {
  int pos = strip.numPixels();
  while (--pos >= 0) {
    strip.setPixelColor(pos, c);
    strip.show();
    delay(wait);
  }
}
 
void rainbow(uint8_t wait) {
uint16_t i, j;
 
for(j=0; j<256; j++) {
for(i=0; i<strip.numPixels(); i++) {
strip.setPixelColor(i, Wheel((i+j) & 255));
}
strip.show();
delay(wait);
}
}
 
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
uint16_t i, j;
 
for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
for(i=0; i< strip.numPixels(); i++) {
strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
}
strip.show();
delay(wait);
}
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
if(WheelPos < 85) {
return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
} else if(WheelPos < 170) {
WheelPos -= 85;
return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
} else {
WheelPos -= 170;
return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
}
}
 
/**
* ^ ^ ^
* ~~~~~ ColorWave ~~~~~
* V V V
*/
void colorWave(uint8_t wait) {
int i, j, stripsize, cycle;
float ang, rsin, gsin, bsin, offset;
 
static int tick = 0;
stripsize = strip.numPixels();
cycle = stripsize * 25; // times around the circle...
 
while (++tick % cycle) {
offset = map2PI(tick);
 
for (i = 0; i < stripsize; i++) {
ang = map2PI(i) - offset;
rsin = sin(ang);
gsin = sin(2.0 * ang / 3.0 + map2PI(int(stripsize/6)));
bsin = sin(4.0 * ang / 5.0 + map2PI(int(stripsize/3)));
strip.setPixelColor(i, strip.Color(trigScale(rsin), trigScale(gsin), trigScale(bsin)));
}
 
strip.show();
delay(wait);
}
 
}
 
/**
* Scale a value returned from a trig function to a byte value.
* [-1, +1] -> [0, 254]
* Note that we ignore the possible value of 255, for efficiency,
* and because nobody will be able to differentiate between the
* brightness levels of 254 and 255.
*/
byte trigScale(float val) {
val += 1.0; // move range to [0.0, 2.0]
val *= 127.0; // move range to [0.0, 254.0]
 
return int(val) & 255;
}
 
/**
* Map an integer so that [0, striplength] -> [0, 2PI]
*/
float map2PI(int i) {
return PI*2.0*float(i) / float(strip.numPixels());
}

void Select_Filters(int RGB)
{
  switch(RGB)
  {
    case Filters_R:          //Red
    digitalWrite(S2, LOW);    
    digitalWrite(S3, LOW);
    //Serial.println("-----select Red color");
    break;
    
    case Filters_G:          //Green
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, HIGH);
    //Serial.println("-----select Green color");
    break;
    
    case Filters_B:          //Blue
    digitalWrite(S2, LOW);    
    digitalWrite(S3, HIGH);
    //Serial.println("-----select Blue color");
    break;
    
    default:                  //Clear(no filter)
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, LOW);
    //Serial.println("-----no filter");
    break;
  }
}
