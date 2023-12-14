#include "esp32-hal.h"
#include "ADHandler.h"
#include <stdio.h>
#include <WString.h>
#include <Adafruit_ADS1X15.h>
//#include "ESP32TimerInterrupt.h"
#include "HardwareSerial.h"


//#define interrupts() sei()
//#define noInterrupts() cli()

#define TIMER0_INTERVAL_MS        1000
static const int AR_AD_SIZE = 16;
#define AR_AD_SIZE                16

static volatile int16_t arADValueIN[AR_AD_SIZE]; 
static volatile int16_t nArIn = 0; int16_t nArOut = 0;
static volatile int16_t nMutex = 0;


//Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */

extern CADHandler adHandler;




CADHandler::CADHandler()
{
  m_pAds = new Adafruit_ADS1115;
}
void CADHandler::init()
{
  Serial.println("CADHandler::init()");

  ::nArIn = 0; ::nArOut = 0;

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
   //m_pAds->setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
   m_pAds->setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
   //m_pAds->setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // m_pAds->setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // m_pAds->setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // m_pAds->setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

if (!m_pAds->begin()) {
    Serial.println("Failed to initialize m_pAds->");
    while (1);
  }
  Serial.println("Initialize ADS OK.");
  m_pAds->startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/false);

  Serial.println("startADCReading done.");
  
  Serial.println("done CADHandler::init()");

  //sei();
  //ITimer0.restartTimer();
}

	bool CADHandler::hasValue()
  {
    //Serial.println("CADHandler::hasValue()");
    adHandler.TimerADGetter(NULL);
    bool ret = false;
    lockMutx();
    ret = ::nArIn != ::nArOut;
    unlockMutx();
    return ret;
  }


int16_t CADHandler::getADValue()
{
  int16_t tmp = 0;
  lockMutx();
  tmp = arADValueIN[::nArOut];
  ::nArOut = (::nArOut + 1)%AR_AD_SIZE;
  unlockMutx();
	return tmp;
}
bool  CADHandler::TimerADGetter(void * timerNo)
{

  lockMutx();
  //Serial.print("Val Index: ");
  //Serial.println(::nArIn);
  if (m_pAds->conversionComplete() ) {
    int16_t tmp = 0;
    tmp = m_pAds->getLastConversionResults();
    arADValueIN[::nArIn] = tmp;
    ::nArIn = (::nArIn + 1)%AR_AD_SIZE;
    m_pAds->startADCReading(ADS1X15_REG_CONFIG_MUX_DIFF_0_1, /*continuous=*/false);
  }
  unlockMutx();
  return true;
}

  void CADHandler::lockMutx()
  {

    //cli();
    // while (1 == nMutex)
    //  {
    //   delay(1);
    //  }
    //  nMutex = 1;
  }

  void CADHandler::unlockMutx()
  {
  //  nMutex = 0;

    //sei();

  }

