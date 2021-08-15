#ifndef LEDStrip_H
#define LEDStrip_H

#include <FastLED.h>

#define LED_PIN 2
#define NUM_LEDS 240
#define MAX_POWER_MILLIAMPS 500

namespace LEDStrip 
{
    void setupLeds();
    void displayColor(CHSV color);
    void turnOffSeat(uint8_t playerSeat, uint8_t maxPlayers);
    void displaySeatColor(uint8_t playerSeat, CHSV color, uint8_t maxPlayers);
    void turnOff();
    void fadeAll();
    void displaySegment(uint8_t start, uint8_t length, CHSV color);
    void flashColor(CHSV color);

}
// Save the memory by defining these
// the strip and these locations are static
#define PLAYER_COLOR_GREEN  CHSV(0, 255, 255)        
#define PLAYER_COLOR_RED    CHSV(96, 255, 255)         
#define PLAYER_COLOR_BLUE   CHSV(160, 255, 255)       
#define PLAYER_COLOR_YELLOW CHSV(64, 255, 255)      

#define PLAYER_COLOR_BLACK  CHSV(0, 0, 255)          
#define PLAYER_COLOR_PURPLE CHSV(192, 255, 255)     
#define PLAYER_COLOR_ORANGE CHSV(32, 255, 255)      
#define PLAYER_COLOR_PINK   CHSV(224, 105, 255)      

#define STRATEGY_CARD_LEADERSHIP    CHSV(0, 255, 255)         
#define STRATEGY_CARD_DIPLOMACY     CHSV(32, 255, 255)      
#define STRATEGY_CARD_POLITICS      CHSV(64, 255, 255)      
#define STRATEGY_CARD_CONSTRUCTION  CHSV(80, 255, 255)        

#define STRATEGY_CARD_TRADE         CHSV(140, 105, 255)      
#define STRATEGY_CARD_WARFARE       CHSV(160, 255, 255)       
#define STRATEGY_CARD_TECHNOLOGY    CHSV(176, 230, 200)      
#define STRATEGY_CARD_IMPERIAL      CHSV(192, 255, 255)     

#define PLAYER_1_LOWER_BOUND 0
#define PLAYER_1_UPPER_BOUND 11
#define PLAYER_1_WRAP_LOW 181
#define PLAYER_1_WRAP_HIGH 200

#define PLAYER_2_LOWER_BOUND 62
#define PLAYER_2_UPPER_BOUND 78

#define PLAYER_3_LOWER_BOUND 113
#define PLAYER_3_UPPER_BOUND 146

#define PLAYER_4_LOWER_BOUND 12
#define PLAYER_4_UPPER_BOUND 45

#define PLAYER_5_LOWER_BOUND 164
#define PLAYER_5_UPPER_BOUND 180

#define PLAYER_6_LOWER_BOUND 79
#define PLAYER_6_UPPER_BOUND 112

#define PLAYER_7_LOWER_BOUND 147
#define PLAYER_7_UPPER_BOUND 163

#define PLAYER_8_LOWER_BOUND 46
#define PLAYER_8_UPPER_BOUND 61

#endif