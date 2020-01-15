
#include"Moving_Average_filter.h"
#include "Arduino.h"

Averaging_Filter::Averaging_Filter()
{
	len = 5;
	float buff[10] = {0};
}

float Averaging_Filter::LowPassFilter(float x)
{
	
	sum = 0;
	average = 0;
	for(int i = 0; i < len-1;i++)
	{
		buff[i] = buff[i+1];
		sum = sum+buff[i];
	}
	buff[len-1] = x;
	sum = sum+x;
	average = sum/len;
	return average;

}
