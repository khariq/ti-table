#include <Arduino.h>
#include "LEDStrip.h"
#include "NFCField.h"
#include "Wire.h"
#include "Buttons.h"

#define DELAY 1000

Buttons _buttons;
NFCField _nfcField;
bool strategyPhase;
unsigned long lastUpdate;
void setup() {
  
  Serial.begin(9600);
  
  Wire.begin();
  LEDStrip::setupLeds();
  _nfcField.begin();

  strategyPhase = true;
  lastUpdate = millis();
}


void chooseStrategyCards() 
{ 
	auto fieldReadings = _nfcField.readField();
	for (uint8_t player = 0; player < 8; player++) 
	{    
		auto tags  = fieldReadings[player] & 0x00FF;
		switch (tags)
		{
			case NfcFlags::Leadership: LEDStrip::displaySeatColor(player, STRATEGY_CARD_LEADERSHIP, 8); break;
      		case NfcFlags::Diplomacy:  LEDStrip::displaySeatColor(player, STRATEGY_CARD_DIPLOMACY, 8); break;
			case NfcFlags::Politics:   LEDStrip::displaySeatColor(player, STRATEGY_CARD_POLITICS, 8); break;
			case NfcFlags::Construction: LEDStrip::displaySeatColor(player, STRATEGY_CARD_CONSTRUCTION, 8); break;
			case NfcFlags::Trade: LEDStrip::displaySeatColor(player, STRATEGY_CARD_TRADE, 8); break;
			case NfcFlags::Warfare: LEDStrip::displaySeatColor(player, STRATEGY_CARD_WARFARE, 8); break;
			case NfcFlags::Technology: LEDStrip::displaySeatColor(player, STRATEGY_CARD_TECHNOLOGY, 8); break;
			case NfcFlags::Imperial: LEDStrip::displaySeatColor(player, STRATEGY_CARD_IMPERIAL, 8); break;
			default: LEDStrip::turnOffSeat(player, 8); break;
		}
  	} 
}

// find the next card in sequence:
// next = higher than currently active
uint8_t activePlayer = 255;
uint8_t activeStrategyCard = 255;

uint8_t nextStrategyCard() {
	auto fieldReadings = _nfcField.readField();

	if (activeStrategyCard == 255)
	{
		activeStrategyCard = 1;
		// if leadership is out in the field, thats the starting point
		for (int player = 0; player < 8; player++)
		{
			auto cardsAtSeat = fieldReadings[player] & 0x00FF;
			if ((cardsAtSeat & activeStrategyCard))
			{
				return activeStrategyCard;
			}
		}
	}	

	for (int i = activeStrategyCard * 2 ; i < 255; i *= 2)
	{
		for (int player = 0; player < 8; player++)
		{
			auto cardsAtSeat = fieldReadings[player] & 0x00FF;
			if ((cardsAtSeat & i) == i)
			{
				return i;
			}
		}
	}

	for (int i = 1; i < activeStrategyCard; i *= 2)
	{
		for (int player = 0; player < 8; player++)
		{
			auto cardsAtSeat = fieldReadings[player] & 0x00FF;
			if ((cardsAtSeat & i) == i)
			{
				return i;
			}
		}
	}

	return 255;
}

uint8_t setActivePlayer()
{

	auto fieldReadings = _nfcField.readField();

	for (uint8_t player = 0; player < 8; player++)
	{
		auto tags = fieldReadings[player] & 0x00FF;
		if (tags == activeStrategyCard) 
		{
			activePlayer = player;
			break;
		}
	}
}

void runActionPhase()
{
	// action phase is over, waiting for round button
	if (activeStrategyCard == 255) return;

	auto fieldReadings = _nfcField.readField();

	if (_buttons.buttonState() & (1 << activePlayer))
	{
		activeStrategyCard = nextStrategyCard();
		if (activeStrategyCard == 255)
		{
			return;
		}
		setActivePlayer();
	}

	// light up the strip the color of the active strategy card
	switch (activeStrategyCard)
	{
		case NfcFlags::Leadership: LEDStrip::displayColor(STRATEGY_CARD_LEADERSHIP); break;
		case NfcFlags::Diplomacy: LEDStrip::displayColor(STRATEGY_CARD_DIPLOMACY); break;
		case NfcFlags::Politics: LEDStrip::displayColor(STRATEGY_CARD_POLITICS);  break;
		case NfcFlags::Construction: LEDStrip::displayColor(STRATEGY_CARD_CONSTRUCTION); break;
		case NfcFlags::Trade: LEDStrip::displayColor(STRATEGY_CARD_TRADE);	break;
		case NfcFlags::Warfare: LEDStrip::displayColor(STRATEGY_CARD_WARFARE); break;
		case NfcFlags::Technology: LEDStrip::displayColor(STRATEGY_CARD_TECHNOLOGY); break;
		case NfcFlags::Imperial: LEDStrip::displayColor(STRATEGY_CARD_IMPERIAL); break;
		default: LEDStrip::turnOff(); break;
	}

}

void loop() 
{
	// put your main code here, to run repeatedly:
	_nfcField.loop(8);
	_buttons.loop();

	unsigned long currentTime = millis();
	if (currentTime - lastUpdate > DELAY) 
	{
		lastUpdate = currentTime;
		if (_buttons.buttonState() & ButtonFlags::Round)
		{
			Serial.println("Round button down");
			// all strategy cards should be back
			activeStrategyCard = nextStrategyCard();
			setActivePlayer();
			Serial.print("Next Strategy Card: "); Serial.println(activeStrategyCard);
			
			if (strategyPhase) strategyPhase = false;
			else strategyPhase = true;
		}
	}
	if (strategyPhase) 
	{
		chooseStrategyCards();
	}
	else 
	{
		runActionPhase();
	}  
}






