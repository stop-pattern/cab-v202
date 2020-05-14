#pragma once

class cData {
 private:
  int signal;                   // speed indicator (bit flag)
  bool R, G, notice, P, X, BL;  // signal indicator
  bool L15, L60, ATS;           // ATS indicator
  int bell, bell_latest;        // bell status
  bool door;                    // door status
  bool indicator[32];           // speed indicator (array)

 public:
  cData();
  ~cData();
  int changeSignal(int arg);
  int changeIndicator(int arg);
  bool changeBell(int arg);
  bool changeDoor(int arg);
  void getArray(std::vector<byte> &array);
};