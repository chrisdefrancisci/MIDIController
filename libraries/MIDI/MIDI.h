/*
	MIDI.h
	a library containing:
		v0: 09/26/15: function to play notes
		v1: 10/11/15: replaces int state with struct
		v2: 10/12/15: pitchbend 
		v3: 11/08/15: modwheel/ other modulation
*/
#ifndef MIDI_h
#define MIDI_h

#include "Arduino.h"

struct state_t
{
	boolean noteState[15];
	unsigned long mod;
};

class MIDI
{
	public:
		MIDI(int latchPin, int clockPin, int dataPin_1, int dataPin_2);
		int bendPin;
		int modPin;

// Note functions

// MIDI::play(int note, int vel) 
// This function sends a note on message with the specified note /
// velocity information. This function is also used to turn notes "off", by 
// sending a message with velocity 0
		void play(int note, int vel);

// MIDI::modWheel(unsigned long mod)
// This function sends a control change message with the value mod to 
// controller 1. This will typically be the mod wheel.
		void modWheel(unsigned long mod);

// MIDI::stateFunc(state_t *midi_state, int note_input)
// This function is used to update what is currently playing by sending MIDI
// messages using MIDI::play and MIDI::modWheel. The notes specified in
// note_input is compared to the state of the notes in midi_state. Additionally,
// the value corresponding to the mod wheel parameter is read and compared to 
// to the value stored in midi_state. Any differing values cause a MIDI message
// to be sent and midi_state is updated.
		void stateFunc(state_t *midi_state, int note_input);

// MIDI::shiftIN(int myDataPin_1, int myDataPin_2, int myClockPin)
// This function returns which notes are to be played from the input 
// device / keyboard
		int shiftIn(int myDataPin_1, int myDataPin_2, int myClockPin);
	private:
		// necessary pins
		int _latchPin, _clockPin, _dataPin_1, _dataPin_2;
};

#endif