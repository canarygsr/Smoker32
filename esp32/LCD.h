//https://www.youtube.com/watch?v=sTYPuDMPva8 scroll hints x minX ****negative

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/SansSerif_bold_10.h>
#include <Fonts/Tiny3x3a2pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 //64 better OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   13 //9   //D1
#define OLED_CLK   14 //10   //D0
#define OLED_DC    12//3 //11 //DC
#define OLED_CS    15//1 //CS ****1change back to 1 and change QuadMaxx back to 16  // 12
#define OLED_RESET 4 //13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
 int x,xx,y, totalstringwidth,scrollwidth;
 int z=1, zz=1;
/* Comment out above, uncomment this block to use hardware SPI
#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RESET, OLED_CS);
*/

void setuplcd() {
  //Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(500); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextWrap(false);
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setFont();
  display.setCursor(0,0);             // Start at top-left corner
}



void debugtoLCD(const char* text_to_print){
 // display.clearDisplay();

 // display.println(text_to_print);
  display.display();
  delay(200);  
}

void cleardisplay(void) {
 display.clearDisplay();
   display.setCursor(0,0);
     display.display();  
}
void updatedisplay(void) {
//display.clearDisplay();
display.print(F("Time:"));
  display.display();
  //delay(2000);


  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setFont();
  display.setCursor(0,2);               // Start at top-left corner seems to need an offsed down 2 pixels
  display.print(F("Time:"));
 // display.print(timenow()); //display.print(cooktime());
  display.setTextColor(BLACK, WHITE);
  display.print(F(" "));
  display.print(F("FAN:100%"));
  display.print(F(" "));
  display.print(F("OT"));
  display.display();
  //report lines
  //   //moved into for loop
  display.setTextColor(WHITE); // Draw 'inverse' text
  display.setFont(&SansSerif_bold_10);
  display.setTextSize(1);
  for ( int i = 0; i < 4; i++ ) {
  y = (i * 12) + 25; // where 17 is the height of the top rows and 12 is the height of the other text
  display.setCursor(xx,y);
  display.print(Probe[i].ProbeName);
  display.print(F(": "));
  display.print(Probe[i].Setpoint);
  display.print(F(" "));
  display.println(Probe[i].CurrentTemp);
  }
  display.display();
  //delay(2000);

   totalstringwidth = (Probe[0].stringwidth + 11)*6.7; //where 11 = temp values and 6.7 is the averge number of pixels in a letter

   scrollwidth = totalstringwidth - display.width(); //how many pixelxs we need to scroll
   Serial.print("StingWidth");
   Serial.println(Probe[0].stringwidth);   
   Serial.print("TotalStingWidth");
   Serial.println(totalstringwidth);
   Serial.print("ScrollWidth");
   Serial.println(scrollwidth);
   Serial.print("XX scroll left or right");  
   Serial.println(xx);
   if (scrollwidth < 0){zz = 0; } else {zz=1;}
   
  x=x-(4*z*zz); // number of pixles to scroll
  xx= x - (scrollwidth/2) ; // offset of where to start the scroll sohuld be minus or plus??
  if((x*x) > ((scrollwidth*scrollwidth/4)+9))// squared to remove -ve, /4 is to half it and +9 is some padding so the first letter does not dissapear to quickly
  {//x = 0;
   z = z*(-1);}
   display.setCursor(0,0);  

}
