/*

Metalabs RGB ledmatrix
24x24 pixels based on TM1809 pixel stripes 30 pixel/meter

You can buy those ledstipes @ the http://hackerspaceshop.com




You need the FastSPI_Leds arduino library found here: http://code.google.com/p/fastspi/


This code is meant to be compiled for a teensy 2.0 board that is attached over USB to your computer sending animations (for example a raspberry pi)

To send data to the ledmatrix you send the string 'FNORD'
followed by 24*24*3 = 1728  bytes of data.
each byte is one color R,G,B encoded  24 bit true color FTW! ;)

The matrix looks like this:


TOP left corner
 ________________________
| 1  2  3  4  5 6... 24  |
| 48 47 46 45 44  ... 25 |
| 49 50 51 .......... 72 |
| ...                    |
| ...                    |
| .................. 576 |
 ------------------------
                      BOTTOM right corner


happy hacking!
-flo

*/

#include <FastSPI_LED.h>

#define COLS 24
#define ROWS 24

#define NUM_LEDS 24*24 

struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
struct CRGB *leds;

#define PIN PIN_B2




const int frameSize = NUM_LEDS*3;

int frameBuffer[frameSize];


void setup()
{
  Serial.begin(115200); // this is currently at 9600 .. but should be faster in the future .. just dont wonder if your current setup doesn't work out of the box with this baudrate
  Serial.print("running");
  

  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);


  FastSPI_LED.setPin(PIN);
 
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  
  
  
   leds[0].r = 0;
   leds[0].g = 0;
   leds[0].b = 100;  
   leds[1].r = 100;
   leds[1].g = 0;
   leds[1].b = 100;  
   FastSPI_LED.show();
  

}

int serialReadBlocking() {
  while (!Serial.available()) {}
  return Serial.read();
}

void loop() {
   while (serialReadBlocking() != 'F') {}
   if (serialReadBlocking() != 'N') return;
   if (serialReadBlocking() != 'O') return;  
   if (serialReadBlocking() != 'R') return;
   if (serialReadBlocking() != 'D') return;

   for (int i=0; i < NUM_LEDS; i++) {
     leds[i].r = serialReadBlocking();
     leds[i].g = serialReadBlocking();
     leds[i].b = serialReadBlocking();
   }
    
   FastSPI_LED.show();
}



