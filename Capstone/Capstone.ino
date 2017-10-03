#include <MIDI.h>

/* Using Arduino as a MIDI controller
* Uses shiftIn Example 1.1  by Carlyn Maw and
* 
*/

//define where your pins are
int latchPin = 8;
int dataPin_1 = 9;
int dataPin_2 = 10;
int clockPin = 7;
MIDI midi(latchPin, clockPin, dataPin_1, dataPin_2);

state_t midi_state;

  
//Define variables to hold the data 
//for shift register.

int note_input = 72;  //01001000

void setup() {
  //start serial
  Serial.begin(31250);
  //define pin modes
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin_1, INPUT);
  pinMode(dataPin_2, INPUT);
  for(int i = 0; i<128; i++)
  {
    midi.play(i, 0);
    if(i<8)
      midi_state.noteState[i] = 0;
  }
  midi.modPin = A0;
}

void loop() {

  // get the note input
  note_input = midi.shiftIn(dataPin_1, dataPin_2, clockPin);
  midi.stateFunc(&midi_state, note_input);

}
