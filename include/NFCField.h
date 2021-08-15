#ifndef NFC_FIELD_H
#define NFC_FIELD_H

#define TCAADDR 0x70
#include "Arduino.h"
#include <Wire.h>
#include "PN532_I2C.h"
#include "PN532.h"
  
// These values are specific to the NFC tags stickered to the back of my cards
// YMMV
// STRATEGY CARDS
#define NFC_TAG_LEADERSHIP 297         // 04 21 D1 01 36 01 03
#define NFC_TAG_DIPLOMACY 318         // 04 21 E0 01 3C 01 03
#define NFC_TAG_POLITICS 400          // 04 21 EB 01 83 01 03
#define NFC_TAG_CONSTRUCTION 489      // 04 21 D4 01 F3 01 03
#define NFC_TAG_TRADE 340             // 04 21 EF 01 43 01 03
#define NFC_TAG_WARFARE 374           // 04 21 DC 01 78 01 03
#define NFC_TAG_TECHNOLOGY 280        // 04 21 F3 01 03 01 03
#define NFC_TAG_IMPERIAL 429          // 04 21 D8 01 B3 01 03

// OTHER COMPONENTS
#define NFC_TAG_CUSTODIANS 250        // 04 72 48 01 3F 41 03
#define NFC_TAG_SPEAKER 407           // 04 72 4E 01 D6 41 03
#define NFC_TAG_NAALU 338             // 04 72 4B 01 94 41 03

enum NfcFlags 
{
    Leadership      = 0x0001,
    Diplomacy       = 0x0002,
    Politics        = 0x0004,
    Construction    = 0x0008,
    Trade           = 0x0010,
    Warfare         = 0x0020,
    Technology      = 0x0040,
    Imperial        = 0x0080,
    Custodians      = 0x0100,
    Speaker         = 0x0200,
    Naalu           = 0x0400
};

class NFCField
{

    public:
        NFCField();

        void begin();
        void loop(uint8_t numReaders);
        const uint16_t* readField() const { return _nfcReadings; }

    private:
        void tcaselect(uint8_t i);
        uint16_t _nfcReadings[8];
        PN532_I2C _pn532i2c;
        PN532 _nfc;
        uint8_t _readTarget = 0;
        

};

#endif