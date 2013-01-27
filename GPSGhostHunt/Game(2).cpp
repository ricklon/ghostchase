#include "Game.h"
#include "Hardware.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library

#define BUFFPIXEL 20

unsigned long gTime;
long gPrevTime = 0;

int ii = 0;

int tiltAState;
int tiltBState;
int tiltCState;
int tiltDState;

int tiltAVal;
int tiltBVal;
int tiltCVal;
int tiltDVal;

int tiltACnt = 0;
int tiltBCnt = 0;
int tiltCCnt = 0;
int tiltDCnt = 0;

int gameState = 0; //Start, Play, DEATH, SUCCESS, Restart
int numGhosts = 3;
int playerHealth = 3;
int tiltStatus;  //0 = left, 1 = middle ,2 = right 
//combat
int leftEnergy;
int rightEnergy;


int begin(Adafruit_ST7735 tft) 
{
  tft.fillScreen(ST7735_BLACK);
  return GAME_TITLE;
}

int gameTitle(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK);
  titleAnimation(tft);
  
  delay(1000);
  return GAME_INTRO;
}

int gameIntro(Adafruit_ST7735 tft) {

  tft.fillScreen(ST7735_BLACK); 

  while (1) {
    tiltStatus = getTilt();

    drawString(10, 0, "<--Help ", ST7735_RED,2,tft);
    drawString(10, 20, " Start-->", ST7735_RED,2,tft); 
    drawString(0, 35,"Survive",ST7735_RED,2,tft);
    drawString(0, 50,"Real World",ST7735_RED,2,tft);
    drawString(10, 65,"Ghosts!",ST7735_RED,2,tft);

    if (tiltStatus == TILT_L) {
      return GAME_INSTR;
    } 
    else if (tiltStatus == TILT_R) {
      return GAME_START;
    }
  }
  return GAME_START;

}

int gameInstr(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK); 
  drawString(5, 20, "Intro -->", ST7735_RED,2,tft); 
  drawString(10, 40,"GPS tracking",ST7735_RED,1,tft);
  drawString(10, 60,"ghost attack",ST7735_RED,1,tft);
  drawString(10, 80,"as you move!",ST7735_RED,1,tft);
  drawString(10, 100,"Tilt controller",ST7735_RED,1,tft);
  drawString(10, 120,"combat!",ST7735_RED,1,tft);

  tiltACnt = 0;

  while (1) {
    tiltStatus = getTilt();
    if (tiltStatus == TILT_L) {
      tiltACnt++;
    }
    if (tiltStatus == TILT_R) {
      return GAME_START;
    }  
  }
  return GAME_INTRO;
}
int gameStart(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK); 

  long prevTime = millis();
  while (1) {
    gTime = millis();
    drawString(0, 0,"Survive!",ST7735_RED,2,tft);
    tft.setCursor(0,1);

    if (gTime - prevTime > 2000) {
      drawString(10, 20,"3",ST7735_RED,2,tft);
    }
    if (gTime - prevTime > 3500) {
      drawString(15, 40," 2,",ST7735_RED,2,tft);
    }
    if (gTime - prevTime > 5000) {
      drawString(25, 60,"  1...",ST7735_RED,2,tft);
    }
    if (gTime - prevTime > 6500) {
      break;
    }
  }
  return GAME_PLAY;
}

int gamePlay(Adafruit_ST7735 tft) {
  tiltACnt = 0;
  tiltBCnt = 0;
  tft.fillScreen(ST7735_BLACK); 

  while(1) {
    tiltStatus = getTilt();

    switch (tiltStatus) {
    case TILT_L:
      tiltACnt++;
      break;

    case TILT_R: 
      tiltBCnt++;
      break;

    case TILT_MID:
      break;
    }
    break;
  }
  return GAME_DEATH;
}

int gameDeath(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK); 
  drawString(0, 0,"Soul Death!",ST7735_RED,2,tft);

  long prevTime = millis();

  while (1) {
    gTime = millis();
    for (ii = 0; ii < 90; ii = ii+10)
    {
      bmpDraw("guy.bmp", ii+40, 130, tft);
      delay(20);
      bmpDraw("ghost.bmp", ii, 61, tft);
    }
    if (gTime - prevTime > 4000) {
      break;
    }
  }
  return GAME_RESTART;
}

int gameWin(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK); 
  drawString(0, 0,"Soul Safe!",ST7735_RED,2,tft);
  drawString(0, 20,"You Win!!!",ST7735_RED,2,tft);
   for (ii = 0; ii < 90; ii = ii+10)
    {
      bmpDraw("guy.bmp", ii+40, 130, tft);
    }
  delay(4000);
  return GAME_RESTART;
}

int gameRestart(Adafruit_ST7735 tft) {
  tft.fillScreen(ST7735_BLACK); 
  drawString(0, 0,"Refocus",ST7735_RED,2,tft);
  drawString(0, 20,"To Start-->",ST7735_RED,2,tft);
  
  while (1) {
    tiltStatus = getTilt();
    if (tiltStatus == TILT_R) {
      break;
    }  
  }
  return GAME_START;
}


int getTilt() {
  bool AA = digitalRead(TILT_PINA);
  bool BB = digitalRead(TILT_PINB);

  if (AA == LOW) {
    return TILT_L;
  }
  if (BB == LOW) {
    return TILT_R;
  }
  return TILT_MID; 
}


int getGap(int star, int bar) {
  int gap = 16 - (star + bar);
  return gap;
}

void printGhost(int col, int row, int wait, Adafruit_ST7735 tft) {

  tft.setCursor(col,row);
  tft.write((byte)0);
  tft.write((byte)1);
  delay(wait);
  tft.setCursor(col,row);
  tft.write((byte)2);
  tft.write((byte)3);
  delay(wait);
  tft.setCursor(col,row);
  tft.write((byte)4);
  tft.write((byte)5);
}

void bmpDraw(char *filename, uint8_t x, uint8_t y,  Adafruit_ST7735 tft) {

  File     bmpFile;
  int      bmpWidth, bmpHeight;   // W+H in pixels
  uint8_t  bmpDepth;              // Bit depth (currently must be 24)
  uint32_t bmpImageoffset;        // Start of image data in file
  uint32_t rowSize;               // Not always = bmpWidth; may have padding
  uint8_t  sdbuffer[3*BUFFPIXEL]; // pixel buffer (R+G+B per pixel)
  uint8_t  buffidx = sizeof(sdbuffer); // Current position in sdbuffer
  boolean  goodBmp = false;       // Set to true on valid header parse
  boolean  flip    = true;        // BMP is stored bottom-to-top
  int      w, h, row, col;
  uint8_t  r, g, b;
  uint32_t pos = 0, startTime = millis();

  if((x >= tft.width()) || (y >= tft.height())) return;

  Serial.println();
  Serial.print("Loading image '");
  Serial.print(filename);
  Serial.println('\'');

  // Open requested file on SD card
  if ((bmpFile = SD.open(filename)) == NULL) {
    Serial.print("File not found");
    return;
  }

  // Parse BMP header
  if(read16(bmpFile) == 0x4D42) { // BMP signature
    Serial.print("File size: "); 
    Serial.println(read32(bmpFile));
    (void)read32(bmpFile); // Read & ignore creator bytes
    bmpImageoffset = read32(bmpFile); // Start of image data
    Serial.print("Image Offset: "); 
    Serial.println(bmpImageoffset, DEC);
    // Read DIB header
    Serial.print("Header size: "); 
    Serial.println(read32(bmpFile));
    bmpWidth  = read32(bmpFile);
    bmpHeight = read32(bmpFile);
    if(read16(bmpFile) == 1) { // # planes -- must be '1'
      bmpDepth = read16(bmpFile); // bits per pixel
      Serial.print("Bit Depth: "); 
      Serial.println(bmpDepth);
      if((bmpDepth == 24) && (read32(bmpFile) == 0)) { // 0 = uncompressed

        goodBmp = true; // Supported BMP format -- proceed!
        Serial.print("Image size: ");
        Serial.print(bmpWidth);
        Serial.print('x');
        Serial.println(bmpHeight);

        // BMP rows are padded (if needed) to 4-byte boundary
        rowSize = (bmpWidth * 3 + 3) & ~3;

        // If bmpHeight is negative, image is in top-down order.
        // This is not canon but has been observed in the wild.
        if(bmpHeight < 0) {
          bmpHeight = -bmpHeight;
          flip      = false;
        }

        // Crop area to be loaded
        w = bmpWidth;
        h = bmpHeight;
        if((x+w-1) >= tft.width())  w = tft.width()  - x;
        if((y+h-1) >= tft.height()) h = tft.height() - y;

        // Set TFT address window to clipped image bounds
        tft.setAddrWindow(x, y, x+w-1, y+h-1);

        for (row=0; row<h; row++) { // For each scanline...

          // Seek to start of scan line.  It might seem labor-
          // intensive to be doing this on every line, but this
          // method covers a lot of gritty details like cropping
          // and scanline padding.  Also, the seek only takes
          // place if the file position actually needs to change
          // (avoids a lot of cluster math in SD library).
          if(flip) // Bitmap is stored bottom-to-top order (normal BMP)
            pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
          else     // Bitmap is stored top-to-bottom
          pos = bmpImageoffset + row * rowSize;
          if(bmpFile.position() != pos) { // Need seek?
            bmpFile.seek(pos);
            buffidx = sizeof(sdbuffer); // Force buffer reload
          }

          for (col=0; col<w; col++) { // For each pixel...
            // Time to read more pixel data?
            if (buffidx >= sizeof(sdbuffer)) { // Indeed
              bmpFile.read(sdbuffer, sizeof(sdbuffer));
              buffidx = 0; // Set index to beginning
            }

            // Convert pixel from BMP to TFT format, push to display
            b = sdbuffer[buffidx++];
            g = sdbuffer[buffidx++];
            r = sdbuffer[buffidx++];
            tft.pushColor(tft.Color565(r,g,b));
          } // end pixel
        } // end scanline
        Serial.print("Loaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
      } // end goodBmp
    }
  }

  bmpFile.close();
  if(!goodBmp) Serial.println("BMP format not recognized.");
}


// These read 16- and 32-bit types from the SD card file.
// BMP data is stored little-endian, Arduino is little-endian too.
// May need to reverse subscript order if porting elsewhere.

uint16_t read16(File f) 
{
  uint16_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read(); // MSB
  return result;
}

uint32_t read32(File f) 
{
  uint32_t result;
  ((uint8_t *)&result)[0] = f.read(); // LSB
  ((uint8_t *)&result)[1] = f.read();
  ((uint8_t *)&result)[2] = f.read();
  ((uint8_t *)&result)[3] = f.read(); // MSB
  return result;
}



void titleAnimation(Adafruit_ST7735 tft) 
{
  drawString(10, 0, "GPS Ghost", ST7735_RED,2,tft); 
  drawString(30, 20, "Chase!", ST7735_RED,2,tft); 

  for (ii = 0; ii < 90; ii = ii+10)
  {
    bmpDraw("guy.bmp", ii+40, 130, tft);
    delay(20);
    bmpDraw("ghost.bmp", ii, 61, tft);
    //delay(25); slow drawing no need for dealy

  }
  bmpDraw("sghost.bmp", 0, 0, tft);
  bmpDraw("sghost.bmp", 20, 20, tft);
}

void drawString(int xx, int yy, const char *text, uint16_t color, uint8_t size, Adafruit_ST7735 tft) {
  tft.setCursor(xx, yy);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(text);
}

void drawString(int xx, int yy,int text, uint16_t color, uint8_t size, Adafruit_ST7735 tft) {
  tft.setCursor(xx, yy);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(text);
}

void drawString(int xx, int yy,float text, uint16_t color, uint8_t size, Adafruit_ST7735 tft) {
  tft.setCursor(xx, yy);
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.print(text);
}














