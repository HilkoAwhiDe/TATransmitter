#ifndef ADHandler_H
#define ADHandler_H
#include <stdint.h>

class Adafruit_ADS1115;
class CADHandler
{
	public:
  CADHandler();
	void init();
	bool hasValue();
  int16_t getADValue();
  bool  TimerADGetter(void * timerNo);
  private:
  void lockMutx();
  void unlockMutx();
  Adafruit_ADS1115 * m_pAds;
};

#endif