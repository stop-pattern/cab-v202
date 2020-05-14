#include <Arduino.h>

#include <vector>

#include "TR.BIDS.libs.h"
#include "data.h"
#include "define.h"

uint bidsVer = VERSION;
bool outputStatus = false;
std::vector<byte> latest = {};
BIDS bids = BIDS(&Serial);
cData storedData = cData();

void setup() {
  Serial.begin(19200);
  while (!Serial) delay(1);

#ifdef _DEBUG_
  Serial.println("init...");  // debug
#endif                        // _DEBUG_

  pinMode(IO0, INPUT);
  pinMode(SW, INPUT);
  pinMode(LED, OUTPUT);

  pinMode(PWM1, OUTPUT);
  ledcSetup(0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(PWM1, 0);
  pinMode(PWM2, OUTPUT);
  ledcSetup(1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcAttachPin(PWM2, 1);

  pinMode(SER, OUTPUT);
  pinMode(RCK, OUTPUT);
  pinMode(SCK, OUTPUT);
  pinMode(OE, OUTPUT);
  pinMode(SCL, OUTPUT);

  digitalWrite(OE, LOW);
  digitalWrite(SCL, HIGH);
  digitalWrite(LED, HIGH);

#ifdef _DEBUG_
  Serial.println("set auto send");  // debug
#endif                              // _DEBUG_

  bids.AddAutoSend('P', 100, signalChanged);
  bids.AddAutoSend('P', 255, indicatorChanged);
  bids.AddAutoSend('S', 2, bellChanged);
  bids.AddAutoSend('D', 0, doorChanged);

#ifdef _DEBUG_
  Serial.println("init finished");  // debug
#endif                              // _DEBUG_
}

void loop() {
  bidsVer = bids.CmdSenderI("TRV202");
  if (bidsVer >= 202) {
    digitalWrite(LED, HIGH);
    while (true) {
      loop_v202();
    }
  } else {
    digitalWrite(LED, LOW);
    uint8_t cnt = 0;
    while (true) {
      loop_v100(cnt);
      cnt++;
    }
  }
}

void loop_v100(uint8_t cnt) {
  double Speed = bids.CmdSenderF("TRIE1");
  pwmWRite(0, Speed);
  double ORP = bids.CmdSenderF("TRIP135") / 10;
  pwmWRite(0, ORP);

  if (cnt >= 5) {
    signalChanged(bids.CmdSenderF("TRIP00"), 0);
    indicatorChanged(bids.CmdSenderF("TRIP255"), 0);
    bellChanged(bids.CmdSenderF("TRIS2"), 0);
    doorChanged(bids.CmdSenderF("TRID0"), 0);
    cnt = 0;
  }
  if (outputStatus) {
    std::vector<byte> indicator = {};
    storedData.getArray(indicator);
    if (indicator == latest) {
      ShiftOut(indicator);
      latest = indicator;
    }
    outputStatus = false;
  }
}

void loop_v202() {
  double Speed = bids.CmdSenderF("TRIE1");
  pwmWRite(0, Speed);
  double ORP = bids.CmdSenderF("TRIP135") / 10;
  pwmWRite(0, ORP);

  if (checkAS()) {
    while (true)
      ;  // assert
  }
  if (outputStatus) {
    std::vector<byte> indicator = {};
    storedData.getArray(indicator);
    if (indicator == latest) {
      ShiftOut(indicator);
      latest = indicator;
    }
    outputStatus = false;
  }
}
