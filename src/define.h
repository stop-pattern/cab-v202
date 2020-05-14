#pragma once

/*--------settings-------*/
// static constexpr auto _DEBUG_
static constexpr auto VERSION = 202;          // BIDS version
static constexpr auto BAUDRATE = 115200;      // Serial baudrate
static constexpr auto MAXGAUGE = 120;         // PWM max value
static constexpr auto LEDC_TIMER_BIT = 16;    // PWM resolution
static constexpr auto LEDC_BASE_FREQ = 1000;  // PWM frequency

/*----------pins---------*/
static constexpr auto IO0 = 0;
static constexpr auto SW = 21;
static constexpr auto LED = 22;
static constexpr auto PWM1 = 25;
static constexpr auto PWM2 = 21;

#define SER 12
#define RCK 27
#define SCK 33
#define OE 14
#define SCL 32

/*----------functions---------*/
void setup();
void loop();
void loop_v100(uint8_t cnt);
void loop_v202();
bool checkAS();
void ShiftOut(const std::vector<byte> sendData);
void pwmWRite(uint8_t channel, double value);
void signalChanged(int valI, double valF);
void indicatorChanged(int valI, double valF);
void bellChanged(int valI, double valF);
void doorChanged(int valI, double valF);
