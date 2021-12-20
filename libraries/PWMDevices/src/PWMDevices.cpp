/**************************************************************************/
/*!
    @file     PWMDevices.cpp
    @author   Romuald Rousseau
    @license  BSD (see license.txt)

    Write and Read PWM signals

    This is a library to write and read PWM signals

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/
#include "Arduino.h"

#define NO_PWM_OUTPUT_SUPPORT
#include "PWMDevices.h"

struct PWMChannel_t
{
  byte bit;
  byte port;
  long startTime;
  int command;
};

int                 	PWM_channel_last_id = 0;
volatile PWMChannel_t  	PWM_channel[PWM_MAX_CHANNELS];
volatile byte       	PWM_lastInputRegister[3] = {0, 0, 0};

/**************************************************************************/
/*!
    @brief  Attach a pwm input to a arduino pin
*/
/**************************************************************************/
void PWMDeviceInput::attach(int pin) {
  if(PWM_channel_last_id >= PWM_MAX_CHANNELS)
  {
    return;
  }
  
  pinMode(pin, INPUT_PULLUP);
  
  byte bit = digitalPinToBitMask(pin);
  byte port = digitalPinToPort(pin);

  if(port == NOT_A_PORT) {
    return;
  }

  port -= 2;

  if(port == 0) {
    PCMSK0 |= bit;
  }
  else if(port == 1) {
    PCMSK1 |= bit;
  }
  else if(port == 2) {
    PCMSK2 |= bit;
  }
  PCICR |= 0x01 << port;

  PWM_channel[PWM_channel_last_id].bit = bit;
  PWM_channel[PWM_channel_last_id].port = port;
  PWM_channel[PWM_channel_last_id].startTime = 0;
  PWM_channel[PWM_channel_last_id].command = 0;

  _channel_id = PWM_channel_last_id++;
}

/**************************************************************************/
/*!
    @brief  Read a pwm signal
*/
/**************************************************************************/
int PWMDeviceInput::read() {
  return PWM_channel[_channel_id].command;
}

// *********************** BEGIN INTERRUPT **************************
void processSignal(byte port) {
  byte curr = *portInputRegister(port + 2);
  byte* last = &PWM_lastInputRegister[port];
  
  for(int i = 0; i < PWM_channel_last_id; i++) {
    
    PWMChannel_t* c = &PWM_channel[i];
    if(c->port != port) {
      continue;
    }
    
    byte cmask = curr & c->bit;
    byte lmask = *last & c->bit; 
    if(cmask != lmask) {
      if(cmask > 0) { // Signal rising
        c->startTime = micros();
      }
      else {          // Signal falling
        c->command = micros() - c->startTime;
      }
    }
  }

  *last = curr;
}

SIGNAL(PCINT0_vect) {
  processSignal(0);
}

SIGNAL(PCINT1_vect) {
  processSignal(1);
}

SIGNAL(PCINT2_vect) {
  processSignal(2);
}
// *********************** END INTERRUPT **************************

