#include <EEPROM.h>
#include"Moving_Average_filter.h"

Averaging_Filter filter0 =  Averaging_Filter();
Averaging_Filter filter1 =  Averaging_Filter();
Averaging_Filter filter2 =  Averaging_Filter();
Averaging_Filter filter3 =  Averaging_Filter();
Averaging_Filter filter4 =  Averaging_Filter();
Averaging_Filter filter5 =  Averaging_Filter();
Averaging_Filter filter6 =  Averaging_Filter();

#define sensor_len 7
int analog_value[sensor_len];
int thresh[sensor_len];
int map_val[sensor_len];
int test[sensor_len];
int weights[sensor_len] = { -3, -2, -1, 0, 1, 2, 3};
int error[sensor_len];
int error_val, map_sum;
void calibrate()
{
  for (int i = 0; i < 50; i++)
  {
    sensor_read();
    for (int i = 0; i < sensor_len; i++)
    {
      Serial.print(analog_value[i]);
      Serial.print("  ");
    }
    Serial.println();
  }

  /******analog values on white colour******/
  Serial.println("threshold array");
  for (int i = 0; i < sensor_len; i++)
  {
    thresh[i] = analog_value[i];
    Serial.print(thresh[i]);
    Serial.print("  ");
  }
  Serial.println();

  for (int i = 0; i < sensor_len; i++)
  {
    EEPROM.write(i, (thresh[i] + 2));
  }
}

void threshold()
{
  Serial.println("threshold array");
  for (int i = 0; i < sensor_len; i++)
  {
    test[i] = EEPROM.read(i);
    Serial.print(test[i]);
    Serial.print("  ");
  }
  Serial.println();
}

void sensor_read()
{
  analog_value[0] = filter0.LowPassFilter(analogRead(A8));
  analog_value[1] = filter1.LowPassFilter(analogRead(A9));
  analog_value[2] = filter2.LowPassFilter(analogRead(A10));
  analog_value[3] = filter3.LowPassFilter(analogRead(A11));
  analog_value[4] = filter4.LowPassFilter(analogRead(A12));
  analog_value[5] = filter5.LowPassFilter(analogRead(A13));
  analog_value[6] = filter6.LowPassFilter(analogRead(A14));
}

void process()
{
  error_val = 0;
  map_sum = 0;
  for (int i = 0; i < sensor_len; i++)
  {
    if (analog_value[i] < test[i])
    {
      map_val[i] = 1;
    }
    else if ((analog_value[i] > test[i]) || (analog_value[i] == test[i]))
    {
      map_val[i] = 0;
    }
    map_sum = map_sum + map_val[i];
  }

  for (int i = 0; i < sensor_len; i++)
  {
    error[i] = weights[i] * map_val[i];
    error_val = error_val + error[i]; 
  }
}
void print_val()
{
  /*****analog readings*******/
  Serial.print("analog array : ");
  for (int i = 0; i < sensor_len; i++)
  {
    Serial.print(analog_value[i]);
    Serial.print("  ");
  }
  Serial.println();
  /*********mapped array********/
  Serial.print("mapped array : ");
  for (int i = 0; i < sensor_len; i++)
  {
    Serial.print(map_val[i]);
    Serial.print("   ");
  }
  Serial.println();

  /*******error array*********/
  Serial.print("error array : ");
  for (int i = 0; i < sensor_len; i++)
  {
    Serial.print(error[i]);
    Serial.print("   ");
  }
  Serial.println();
  Serial.print("error val : ");
  Serial.print(error_val);
  Serial.println();
  delay(500);
}


void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    if (Serial.read() == 'c')
    {
      calibrate();
    }
  }
  else
  {
    threshold();
    sensor_read();
    process();
    print_val();
  }
}
