#include "NFCField.h"
#include <Wire.h>
#include "PN532.h"
#include "PN532_I2C.h"

// #define DEBUG

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}


NFCField::NFCField()
    :  _pn532i2c(Wire), 
      _nfc(_pn532i2c)
{
    
}

void NFCField::begin()
{
    for (uint8_t index = 0; index < 8; index++) 
    {
        tcaselect(index);
        
        // configure board to read RFID tags
        _nfc.begin();        
         uint32_t versiondata = _nfc.getFirmwareVersion();
        if (! versiondata) {
            Serial.println(F("Didn't find PN53x board"));
        } else {
            Serial.print(F("Listening at NFC ")); Serial.println(index);
        }
        _nfc.SAMConfig();        
        _nfcReadings[index] = 0x00;        
    }
}

void NFCField::loop(uint8_t numReaders)
{
    if (numReaders == 0) 
    {
        return;
    }

    #ifdef DEBUG
    Serial.print(F("Reading NFC field.  Size == ")); Serial.print(numReaders); Serial.print(F(" Readings: "));
    for (uint8_t i = 0; i < numReaders; i++) {
        Serial.print(F("[ ")); Serial.print(_nfcReadings[i], HEX); Serial.print(F(" ]"));
    }
    Serial.print(F("Free Memory: ")); Serial.println(freeMemory());
    #endif 

    // erase any previous readings for this target
    _nfcReadings[_readTarget] = 0x00;

    #ifdef DEBUG
    Serial.print(F("Reading from ")); Serial.println(_readTarget);
    #endif

    tcaselect(_readTarget);

    #ifdef DEBUG
    Serial.print(_readTarget); Serial.println(" port selected on TCA interface");
    #endif

    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
        
    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = _nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
    if (success) 
    {   
        // identify the tag and set the bit
        // the middle four bytes are the unique identifier for my tags
        int tagId = uid[4] + uid[3] + uid[2] + uid[1];
        switch (tagId)
        {
            case NFC_TAG_LEADERSHIP: _nfcReadings[_readTarget] |= NfcFlags::Leadership; break;
            case NFC_TAG_DIPLOMACY: _nfcReadings[_readTarget] |= NfcFlags::Diplomacy; break;
            case NFC_TAG_POLITICS: _nfcReadings[_readTarget] |= NfcFlags::Politics; break;
            case NFC_TAG_CONSTRUCTION: _nfcReadings[_readTarget] |= NfcFlags::Construction; break;
            case NFC_TAG_TRADE: _nfcReadings[_readTarget] |= NfcFlags::Trade; break;
            case NFC_TAG_WARFARE: _nfcReadings[_readTarget] |= NfcFlags::Warfare; break;
            case NFC_TAG_TECHNOLOGY: _nfcReadings[_readTarget] |= NfcFlags::Technology; break;
            case NFC_TAG_IMPERIAL: _nfcReadings[_readTarget] |= NfcFlags::Imperial; break;
            
            case NFC_TAG_NAALU: _nfcReadings[_readTarget] |= NfcFlags::Naalu; break;
            case NFC_TAG_SPEAKER: _nfcReadings[_readTarget] |= NfcFlags::Speaker; break;
            case NFC_TAG_CUSTODIANS: _nfcReadings[_readTarget] |= NfcFlags::Custodians; break;
        }   
    }
   
    _readTarget++;
    if (_readTarget == numReaders)
    {
        _readTarget = 0;
    }

}

void NFCField::tcaselect(uint8_t i)
{
    if (i > 7) return;
    
    Wire.beginTransmission(TCAADDR);
    Wire.write(1 << i);
    Wire.endTransmission(); 
}