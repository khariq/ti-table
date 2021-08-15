#include "Buttons.h"
#include "Arduino.h"

#define DEBOUNCE 500
#define PLAYER_BUTTON_1 3
#define PLAYER_BUTTON_2 4
#define PLAYER_BUTTON_3 5
#define PLAYER_BUTTON_4 6
#define PLAYER_BUTTON_5 7
#define PLAYER_BUTTON_6 8
#define PLAYER_BUTTON_7 9
#define PLAYER_BUTTON_8 10
#define ROUND_BUTTON 11

Buttons::Buttons() 
{
    pinMode(PLAYER_BUTTON_1, INPUT);
    pinMode(PLAYER_BUTTON_2, INPUT);
    pinMode(PLAYER_BUTTON_3, INPUT);
    pinMode(PLAYER_BUTTON_4, INPUT);
    pinMode(PLAYER_BUTTON_5, INPUT);
    pinMode(PLAYER_BUTTON_6, INPUT);
    pinMode(PLAYER_BUTTON_7, INPUT);
    pinMode(PLAYER_BUTTON_8, INPUT);
    pinMode(ROUND_BUTTON, INPUT);

    auto currTime = millis();
    for (auto i = 0; i < 9; i++)
        _debounceLastMillis[i] = currTime;

    _buttonState = 0x00;
}

Buttons::~Buttons()
{

}

void Buttons::loop()
{
    readPlayerButton(ButtonFlags::Player1, PLAYER_BUTTON_1, 0);
    readPlayerButton(ButtonFlags::Player2, PLAYER_BUTTON_2, 1);
    readPlayerButton(ButtonFlags::Player3, PLAYER_BUTTON_3, 2);
    readPlayerButton(ButtonFlags::Player4, PLAYER_BUTTON_4, 3);
    readPlayerButton(ButtonFlags::Player5, PLAYER_BUTTON_5, 4);
    readPlayerButton(ButtonFlags::Player6, PLAYER_BUTTON_6, 5);
    readPlayerButton(ButtonFlags::Player7, PLAYER_BUTTON_7, 6);
    readPlayerButton(ButtonFlags::Player8, PLAYER_BUTTON_8, 7);
    readPlayerButton(ButtonFlags::Round, ROUND_BUTTON, 8);
}

void Buttons::readPlayerButton(ButtonFlags flag, uint8_t pin, uint8_t index) 
{
    auto t = millis();
    
    if (t - _debounceLastMillis[index] > DEBOUNCE) 
    {
        auto buttonDown = digitalRead(pin);
        if (buttonDown) 
        {
            _buttonState ^= flag;
            _debounceLastMillis[index] = t;
        } 
        else if (_buttonState & flag)
        {
            _buttonState &= ~flag;
        }   
    } 
}


