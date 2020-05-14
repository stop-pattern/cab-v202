#include <Arduino.h>

#include <vector>
#include "data.h"
#include "define.h"

cData::cData() {
  this->signal = 0;
  this->R = false;
  this->G = false;
  this->notice = false;
  this->P = false;
  this->X = false;
  this->BL = false;
  this->L15 = false;
  this->L60 = false;
  this->ATS = false;
  this->bell = 0;
  this->bell_latest = 0;
  this->door = false;
  this->indicator[32] = {};
}
cData::~cData() {}

int cData::changeSignal(int arg) {
  this->signal = arg;
  return this->signal;
}

int cData::changeIndicator(int arg) {
  R = arg & 0b00000001;
  G = arg & 0b00000010;
  notice = arg & 0b00000100;
  P = arg & 0b00001000;
  X = arg & 0b00010000;
  BL = arg & 0b00100000;
  L15 = arg & 0b01000000;
  L60 = arg & 0b10000000;
  ATS = arg & 0b100000000;
  return arg;
}

bool cData::changeBell(int arg) {
  if (arg == 2) {  // ATS_SOUND_CONTINUE
    this->bell_latest = this->bell;
    this->bell = arg;
    if (bell_latest == -1000)
      return false;
    else
      return true;
  }

  this->bell_latest = 0;
  this->bell = arg;
  return this->bell;
}

bool cData::changeDoor(int arg) {
  this->bell = arg;
  return this->bell;
}

void cData::getArray(std::vector<byte> &array) {
  for (size_t i = 0; i < array.size(); i++) {
    array[i] = 0;
  }

  for (size_t i = 0; i < 32; i++) {
    array[i / 8] |= indicator[i] << i % 8;
  }

  array[4] |= R << 0;
  array[4] |= G << 1;
  array[4] |= notice << 2;
  array[4] |= P << 3;
  array[4] |= X << 4;
  array[4] |= 0 << 5;
  array[4] |= 0 << 6;
  array[4] |= 0 << 7;

  array[5] |= L15 << 0;
  array[5] |= L60 << 1;
  array[5] |= ATS << 2;
  array[5] |= bell << 3;
  array[5] |= door << 4;
  array[5] |= 0 << 5;
  array[5] |= 0 << 6;
  array[5] |= 0 << 7;

  return;
}