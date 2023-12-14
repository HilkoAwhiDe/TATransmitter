#include "ADValToTA.h"
#include <stdio.h>
#include <WString.h>
#include "HardwareSerial.h"
//extern HardwareSerial Serial;

#define TA_MAX 6.5
/*
Pos: 1 ADVal: -20200  	TA: 0.0
Pos: 2 ADVal: -19451  	TA: 0.5
Pos: 3 ADVal: -18664  	TA: 1
Pos: 4 ADVal:  -6690  	TA: 2
Pos: 5 ADVal:   2350  	TA: 3
Pos: 6 ADVal:   9459  	TA: 4
Pos: 7 ADVal:  14452  	TA: 5
Pos: 8 ADVal:  18465 	TA: 6
Pos: 9 ADVal:  18740 	TA: 6.5
Pos:10 ADVal:  20192 	TA: offen
ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
+5V
!_____________
             !
 R10K       R10K
  !          !
  !--A1      !-- A0
  !          !
 R10K       Dose
__!__________!
!
GND
*/
//        ADAal   TA
//std::map<int16_t, float> taMap = { {-20200 , 0.0}, {-19451, 0.5}, {-18664 , 1.0}, {-6690, 2.0},{2350, 3.0}, {9459, 4.0}, {14452, 5.0}, {18465, 6.0}, {18740, 6.5}, {20192, 6.5} };

int16_t  arADVal[] = { -20200, -19451, -18664, -6690, 2350, 9459, 14452, 18465, 18740, 20192 };
float    arTAVal[] = {    0.0,    0.5,    1.0,   2.0,  3.0,  4.0,   5.0,   6.0,   6.5,   6.5 };

char cc[200];
//typedef std::map<int16_t, float>::const_iterator map_iterator;


float CADValToTA::getTA(const int16_t& taVal)
{
	String ret;
	float tmp = TA_MAX;
  const int arSize = sizeof(arADVal) / sizeof(int16_t);
	for (register short int adPos  = 0; adPos < arSize ; ++adPos)
	{
    if (adPos == 0 && taVal <= arADVal[adPos] )
    {
      tmp = 0.0;
      break;
    } else
    if (taVal ==  arADVal[adPos] )
    {
      tmp = arTAVal[adPos];//(*i).second;
      break;
    } else if(taVal <  arADVal[adPos])
    {
      if ((adPos + 1) >= arSize || (adPos ) == 0 )
      {
        break;
      } else
      {
        float detaADVal = abs(arADVal[adPos] - arADVal[adPos-1]);
        float detaTA = abs(arTAVal[adPos]- arTAVal[adPos-1]);
        float restADValIn = abs(taVal- arADVal[adPos-1]);
        float prozentADVal =  (restADValIn/(detaADVal/100))/100;
        //Serial.print(" Prozent:" );
        //Serial.print( prozentADVal);
        //Serial.print(" Found:" );
        //Serial.print( arTAVal[adPos -1]);

        tmp = arTAVal[adPos-1] +(detaTA * prozentADVal);
        
        break;
      }

    }
	}
  
  
  //Serial.print(tmp );// <<tmp   <<" c:"   <<c  <<" ret:"  <<ret;
	return tmp;
}
