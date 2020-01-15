#ifndef MOVING_AVERAGE_FILTER
#define MOVING_AVERAGE_FILTER
#include "Arduino.h"

//#define average 1
//#define median 2

class Averaging_Filter
{
	public:
	Averaging_Filter();
	float LowPassFilter(float x);
	int len = 5;
	float buff[5];
	
	private:
	float sum = 0;
	float average = 0;
};



#endif
