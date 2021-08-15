#ifndef BUTTONS_H
#define BUTTONS_H
#include "Arduino.h"

enum ButtonFlags {
    Player1 = 0x0001,
    Player2 = 0x0002,
    Player3 = 0x0004,
    Player4 = 0x0008,
    Player5 = 0x0010,
    Player6 = 0x0020,
    Player7 = 0x0040,
    Player8 = 0x0080,
    Round = 0x0100
};

class Buttons 
{

    public:
        Buttons();
        ~Buttons();

        uint16_t buttonState() const { return _buttonState; }
        void loop();
        void clearFlags() { _buttonState = 0; }

    private:
        uint16_t _buttonState;
        unsigned long _debounceLastMillis[9];
        void readPlayerButton(ButtonFlags flag, uint8_t pin, uint8_t index);
};

#endif