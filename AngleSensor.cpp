#include "AngleSensor.hpp"
#include <Arduino.h>

#ifdef RADIANS
  #define RADIANPERTICK 0.0026179939
#else
  #define DEGREEPERTICK 0.15
#endif

uint8_t AngleSensor::m_num_sensors = 0;
AngleSensor* AngleSensor::m_AngleSensor_ptr[MaxNumofSensors];

AngleSensor::AngleSensor(uint8_t pinA, uint8_t pinB):m_encoder(pinA,pinB)
{
  m_AngleSensor_ptr[m_num_sensors] = this;
  m_num_sensors++;
}

uint8_t AngleSensor::get_num_sensors()
{
   return m_num_sensors;   
}

AngleSensor* AngleSensor::get_sensor_ptr(uint8_t index)
{
  return m_AngleSensor_ptr[index];
}

float AngleSensor::get_angle()
{
  #ifdef DEGREEPERTICK
    return (float)get_position()*DEGREEPERTICK;
  #else
    return (float)get_position()*RADIANPERTICK;
  #endif
}

//destructor
AngleSensor::~AngleSensor(){m_num_sensors--;}

int32_t AngleSensor::get_position()
{
    return m_encoder.read();
}

void AngleSensor::update_velocity(float sampling_freq)
{
  float curr_angle = get_angle();
  m_velocity = (curr_angle - m_last_angle) * sampling_freq;
  m_last_angle = curr_angle;
}

float AngleSensor::get_velocity()
{
  return m_velocity;
}

void AngleSensor::zero()
{
  m_velocity = 0;
  m_last_angle = 0;
  m_encoder.write(0);
}

void AngleSensor::set_angle(float angle)
{
  #ifdef DEGREEPERTICK
    int32_t pos = angle / DEGREEPERTICK;
  #else
    int32_t pos = angle / RADIANPERTICK;
  #endif

  m_last_angle = angle;
  m_encoder.write(pos);
}

void AngleSensor::sensor_updating(float freq)
{
  for (int i=0; i<AngleSensor::get_num_sensors(); i++)
  {
    AngleSensor::get_sensor_ptr(i)->update_velocity(freq);
  }
}

void AngleSensor::zero_all()
{
    for (int i=0; i<AngleSensor::get_num_sensors(); i++)
  {
    AngleSensor::get_sensor_ptr(i)->zero();
  }
}

