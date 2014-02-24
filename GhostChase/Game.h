#ifndef Game_h
#define Game_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SD.h>

//* Nov  2, 2012  <MLS> Changed defines to enum
//Game stages
enum 
{
  GAME_TITLE  = 0,
  GAME_INTRO,
  GAME_INSTR,
  GAME_START,
  GAME_PLAY,
  GAME_DEATH,
  GAME_WIN,
  GAME_RESTART
};


//Screen information
#define WIDTH 16
#define HEIGHT 2
#define RED_BG 12
#define GREEN_BG 14
#define BLUE_BG 15

//controls: tilt information
#define TILT_PINA 16
#define TILT_PINB 17
#define TILT_PINC 13
#define TILT_PIND 15
#define TILT_MID 1
#define TILT_L 0
#define TILT_R 2
#define TILT_F 3
#define TILT_B 4

//game time
extern unsigned long gTime;
extern long gPrevTime;

extern int ii;

extern int tiltAState;
extern int tiltBState;
extern int tiltCState;
extern int tiltDState;

extern int tiltAVal;
extern int tiltBVal;
extern int tiltCVal;
extern int tiltDVal;

extern int tiltACnt;
extern int tiltBCnt;
extern int tiltCCnt;
extern int tiltDCnt;


extern int gameState; //Start, Play, DEATH, SUCCESS, Restart
extern int numGhosts;
extern int playerHealth;
extern int tiltStatus;  //0 = left, 1 = middle ,2 = right 
extern int tiltStatusFB;  //0 = left, 1 = middle ,2 = right 

//combat
extern int leftEnergy;
extern int rightEnergy;

#define kCombatBuffLen  17

extern char combatBuff[kCombatBuffLen];
extern char spaces[kCombatBuffLen];
extern char bars[kCombatBuffLen];

extern byte squiGhost1L[8];
extern byte squiGhost1R[8];
extern byte squiGhost2L[8];
extern byte squiGhost2R[8];
extern byte squiGhost3L[8];
extern byte squiGhost3R[8];


int begin(Adafruit_ST7735 tft);
int gameTitle(Adafruit_ST7735 tft);
int gameIntro(Adafruit_ST7735 tft);
int gameInstr(Adafruit_ST7735 tft);
int gameStart(Adafruit_ST7735 tft);
int gamePlay(Adafruit_ST7735 tft);
int gameDeath(Adafruit_ST7735 tft);
int gameWin(Adafruit_ST7735 tft) ;
int gameRestart(Adafruit_ST7735 tft);
int getTilt();
int getTiltFB();

void titleAnimation(Adafruit_ST7735 tft);
void bmpDraw(char *filename, uint8_t x, uint8_t y, Adafruit_ST7735 tft); 
uint16_t read16(File f);
uint32_t read32(File f);
void drawString(int xx, int yy, const char *text, uint16_t color, uint8_t size,Adafruit_ST7735 tft);
void drawString(int xx, int yy,int text, uint16_t color, uint8_t size,Adafruit_ST7735 tft);
void drawString(int xx, int yy,float text, uint16_t color, uint8_t size,Adafruit_ST7735 tft);
void drawHealth(int amount, Adafruit_ST7735 tft);
void placeGhosts(int amount, Adafruit_ST7735 tft);


#endif

