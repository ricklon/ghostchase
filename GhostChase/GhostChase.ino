#include "Hardware.h"
#include "Game.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library


#include <SPI.h> //SPI Library
#include <SD.h>
//#include <TinyGPS.h>

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
//TinyGPS gps;

void setup() 
{
  Serial.begin(9600);
  //Serial1.begin(9600);
  tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
  tft.fillScreen(ST7735_BLACK);

  if (!SD.begin(SD_CS)) 
  {
    drawString(10, 0, "SD Card", ST7735_RED,2,tft); 
    drawString(0, 20, "Unreadable", ST7735_RED,2,tft); 
    while(1);
  }

  pinMode(TILT_PINA, INPUT_PULLUP);
  pinMode(TILT_PINB, INPUT_PULLUP);
  pinMode(TILT_PINC, INPUT_PULLUP);
  pinMode(TILT_PIND, INPUT_PULLUP);

  tiltAState = digitalRead(TILT_PINA);
  tiltBState = digitalRead(TILT_PINB);
  tiltCState = digitalRead(TILT_PINC);
  tiltDState = digitalRead(TILT_PIND);

}


void loop()
{

  switch (gameState) {
  case GAME_TITLE:
    gameState = gameTitle(tft);  
    break;

  case GAME_INTRO:
    gameState = gameIntro(tft);  
    break;
    
     case GAME_INSTR:  
     gameState = gameInstr(tft);  
     break;

     case GAME_START:  
     gameState = gameStart(tft);  
     break;
     
     case GAME_PLAY:
     gameState = gamePlay(tft);  
     break;
     
     case GAME_DEATH:
     gameState = gameDeath(tft); 
     break;
     
     case GAME_WIN:  
     gameState = gameWin(tft); 
     break;
     
     case GAME_RESTART:
     gameState = gameRestart(tft);
     break;

  }
}






