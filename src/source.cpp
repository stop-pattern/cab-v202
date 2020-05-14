#include <Arduino.h>

#include <vector>

#include "TR.BIDS.libs.h"
#include "data.h"
#include "define.h"

/*----------variants---------*/
extern bool outputStatus;
extern BIDS bids;
extern cData storedData;

/*----------functions---------*/

// check BIDS AutoSend update (supported v202 or later)
// return:
// bool :
bool checkAS() {
  bool isNonAS = false;
  if (!bids.ASDataCheck(&isNonAS) && isNonAS) {
    return true;
  }
  return false;
}

// output byte array to shift register
// arguments:
// std::array sendData
//      send data : bitflag(8bit)
void ShiftOut(const std::vector<byte> sendData) {
#ifdef _DEBUG_
  Serial.print("shiftOut :");  // debug
#endif                         // _DEBUG_
  for (size_t i = 0; i < sendData.size(); i++) {
    shiftOut(SER, SCK, MSBFIRST, sendData[i]);
#ifdef _DEBUG_
    Serial.print(sendData[i], BIN);  // debug
#endif                               // _DEBUG_
  }
  digitalWrite(RCK, LOW);
  digitalWrite(RCK, HIGH);
#ifdef _DEBUG_
  Serial.println("");  // debug
#endif                 // _DEBUG_
  return;
}

// calculate duty to write ledc
// arguments:
// uint8_t channel
//      ledc channel : 0-15
// double value
//      write value : speed (0-MAXGAUGE)
void pwmWRite(uint8_t channel, double value) {
  int ledcMaxValue = pow(2, LEDC_TIMER_BIT);
  if (fabs(value) >= MAXGAUGE) {
    ledcWrite(channel, ledcMaxValue);
    return;
  } else
    ledcWrite(channel, ledcMaxValue * fabs(value) / MAXGAUGE);
  return;
}

void signalChanged(int valI, double valF) {
  storedData.changeSignal(valI);
  outputStatus = true;
}
void indicatorChanged(int valI, double valF) {
  storedData.changeIndicator(valI);
  outputStatus = true;
}
void bellChanged(int valI, double valF) {
  storedData.changeBell(valI);
  outputStatus = true;
}
void doorChanged(int valI, double valF) {
  storedData.changeDoor(valI);
  outputStatus = true;
}
