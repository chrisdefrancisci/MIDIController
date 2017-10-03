
#include "Arduino.h"
#include "MIDI.h"

const int _noteArray[15] = {60, 62, 64, 65, 67, 69, 71, 
						72, 74, 76, 77, 79, 81, 83, 84};

MIDI::MIDI(int latchPin, int clockPin, int dataPin_1, int dataPin_2)
{
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT); 
	pinMode(dataPin_1, INPUT);
	pinMode(dataPin_2, INPUT);
	_latchPin = latchPin;
	_clockPin = clockPin;
	_dataPin_1 = dataPin_1;
	_dataPin_2 = dataPin_2;
}

// sends midi note on/"off" messages
void MIDI::play(int note, int vel)
{
	Serial.write(0x90);
	Serial.write(note);
	Serial.write(vel);
	return;
}

// sends mod wheel / control change messages
void MIDI::modWheel(unsigned long mod)
{
	int message = mod;
	Serial.write(0xB0);
	Serial.write(0x01); 
	Serial.write(message);
}

// stateFunc reads in note from keyboard & plays it
void MIDI::stateFunc(state_t *midi_state, int note_input)
{
	int i; 
	unsigned long mod;
	//reads analog pin for mod
	//converts to a a value between 0 & 127
	mod = analogRead(A0);
	mod *= 127;
	mod /= 1023; 

	//determines what messages to send
	for(i=0; i<15; i++)
	{
		// turn on if input && previously off
		if( (note_input & (1 << i)) && !(midi_state->noteState[i]) )
		{
			play(_noteArray[i], 100);
			midi_state->noteState[i] = 1;
		}
		// turn off if no input && previously on
		else if( !(note_input & (1 << i)) && (midi_state->noteState[i]))
		{
			play(_noteArray[i], 0);
			midi_state->noteState[i] = 0;
		}
	}

	if(mod != midi_state->mod)
	{
		modWheel(midi_state->mod);
		midi_state->mod = mod;
	}

	return;
}


// read in note data from two shift registers
int MIDI::shiftIn(int myDataPin_1, int myDataPin_2, int myClockPin) { 
  int i;
  int temp_1 = 0;
  int pinState;
  byte myDataIn_1 = 0;
  // for second shift register
  int temp_2 = 0;
  int myDataIn_2 = 0;
  int myDataIn;

  //Pulse the latch pin:
  //set it to 1 to collect parallel data
  digitalWrite(_latchPin,1);
  //set it to 1 to collect parallel data, wait
  delayMicroseconds(5);
  //set it to 0 to transmit data serially  
  digitalWrite(_latchPin,0);
  
  for (i=7; i>=0; i--)
  {
    digitalWrite(myClockPin, 0);
    delayMicroseconds(0.2);
    temp_1 = digitalRead(myDataPin_1);
    if (temp_1) {
      pinState = 1;
      myDataIn_1 = myDataIn_1 | (1 << i);
    }
    else {
      pinState = 0;
    }

    temp_2 = digitalRead(myDataPin_2);
    if (temp_2) {
      myDataIn_2 = myDataIn_2 | (1 << i);
    }

    digitalWrite(myClockPin, 1);

  }


  // final output has results from both shift registers
  myDataIn = myDataIn_1 | (myDataIn_2 << 8);
  return myDataIn;
}
