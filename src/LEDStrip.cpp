#include "LEDStrip.h"



namespace LEDStrip 
{
    CRGB _strip[NUM_LEDS];    
}


void LEDStrip::setupLeds()
{    
    delay( 300); // delay for boot recovery, and a moment of silence
    pinMode(LED_PIN, OUTPUT);
    FastLED
        .addLeds<WS2812, LED_PIN, GRB>(_strip, NUM_LEDS)
        .setCorrection( TypicalLEDStrip );
        
    FastLED.setMaxPowerInVoltsAndMilliamps( 5, MAX_POWER_MILLIAMPS);
    FastLED.setBrightness(5);

    for (int i = 0; i < NUM_LEDS; i++) {
        _strip[i] = CRGB::Black;
    }
    
    FastLED.show();
}

void LEDStrip::displayColor(CHSV color) {
    FastLED.showColor(color);    
}


void LEDStrip::turnOffSeat(uint8_t playerSeat, uint8_t maxPlayers) {
    displaySeatColor(playerSeat, CHSV(0,0,0), maxPlayers);
}

void LEDStrip::turnOff() {
    for (uint8_t i = 0; i < NUM_LEDS; i++) 
    {
        _strip[i] = CRGB::Black;
    }
    FastLED.show();
}

void LEDStrip::fadeAll() 
{ 
  for(int i = 0; i < NUM_LEDS; i++) 
    _strip[i].nscale8(250); 
}

void LEDStrip::displaySegment(uint8_t start, uint8_t length, CHSV color)
{
    uint8_t stop = start + length;
    if (stop >= NUM_LEDS)
        stop = NUM_LEDS - 1;
    for (; start <= stop; start++) {
        _strip[start] = color;
    }
    
}

void LEDStrip::displaySeatColor(uint8_t playerSeat, CHSV color, uint8_t maxPlayers) 
{
    uint8_t start = 0;
    uint8_t stop = 0;
    switch (playerSeat)
    {
        case 0:
            start = PLAYER_1_LOWER_BOUND;
            stop = PLAYER_1_UPPER_BOUND;
            break;
        case 1:
            start = PLAYER_2_LOWER_BOUND;
            stop = PLAYER_2_UPPER_BOUND;
            break;
        case 2:
            start = PLAYER_3_LOWER_BOUND;
            stop = PLAYER_3_UPPER_BOUND;
            break;
        case 3:
            start = PLAYER_4_LOWER_BOUND;
            stop = PLAYER_4_UPPER_BOUND;
            break;
        case 4:
            start = PLAYER_5_LOWER_BOUND;
            stop = PLAYER_5_UPPER_BOUND;
            break;
        case 5:
            start = PLAYER_6_LOWER_BOUND;
            stop = PLAYER_6_UPPER_BOUND;
            break;
        case 6:
            start = PLAYER_7_LOWER_BOUND;
            stop = PLAYER_7_UPPER_BOUND;
            break;
        case 7:
            start = PLAYER_8_LOWER_BOUND;
            stop = PLAYER_8_UPPER_BOUND;
            break;
    }

    for (; start <= stop; start++) {
       _strip[start] = color;
    }
    
    // player seat 1 has special case for the break and wrap around
    if (playerSeat == 0)  {
        start = PLAYER_1_WRAP_LOW;
        stop = PLAYER_1_WRAP_HIGH;

        for (; start <= stop; start++) {
            _strip[start] = color;
        }
    }

    // players 2 and 8 share a bar, but if 8 isn't playing then don't restrict 2 to just 1/2 the bar
    if (playerSeat == 1 && maxPlayers < 8) 
    {
        start = PLAYER_8_LOWER_BOUND;
        stop = PLAYER_8_UPPER_BOUND;

        for (; start <= stop; start++) {
            _strip[start] = color;
        }
    }

    if (playerSeat == 4 && maxPlayers < 7)
    {
        start = PLAYER_7_LOWER_BOUND;
        stop = PLAYER_7_UPPER_BOUND;

        for (; start <= stop; start++) {
            _strip[start] = color;
        }
    }

    FastLED.show();
}

void LEDStrip::flashColor(CHSV color)
{
    FastLED.clear(true);
    FastLED.show();
    delay(500);
    displayColor(color);
    delay(500);
}