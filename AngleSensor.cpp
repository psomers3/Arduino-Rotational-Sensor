#include "AngleSensor.hpp"
#include <Arduino.h>

#define DEGREES 360
#define RADIANS 6.283185

uint8_t AngleSensor::m_num_sensors = 0;
AngleSensor* AngleSensor::m_AngleSensor_ptr[MaxNumofSensors];
float AngleSensor::m_global_update_freq = 20;

AngleSensor::AngleSensor(uint8_t pinA, uint8_t pinB):m_encoder(pinA,pinB)  // <- encoder object is initialized here
{
    m_AngleSensor_ptr[m_num_sensors] = this;  // add pointer of this instance to array
    m_num_sensors++;
    m_pulses_per_rev = 2400;  // set resolution of testing encoder
    m_degree_per_tick = (float)DEGREES/(float)m_pulses_per_rev;
    m_radian_per_tick = (float)RADIANS/(float)m_pulses_per_rev;
    m_use_degrees = true;  // default to degrees
    m_counts_since_last_tick = 0;
}

uint8_t AngleSensor::get_num_sensors()
{
   return m_num_sensors;   
}

AngleSensor* AngleSensor::get_sensor_ptr(uint8_t index)
{
  return m_AngleSensor_ptr[index];
}

float AngleSensor::return_angle()
{
    return m_last_angle;
}

float AngleSensor::get_angle()
{
    if (m_use_degrees)
        return (float)get_position()*m_degree_per_tick;
    else
        return (float)get_position()*m_radian_per_tick;
}

//destructor
AngleSensor::~AngleSensor(){}

void AngleSensor::remove_from_sensors()
{
    AngleSensor* temp_array[MaxNumofSensors];
    int curr_index = 0;
    // gather all pointers to instances that are not being deleted
    for (int i=0; i < AngleSensor::get_num_sensors(); i++)
    {
        if (AngleSensor::get_sensor_ptr(i) != this)
        {
            temp_array[curr_index] = AngleSensor::get_sensor_ptr(i);
        }
    }
    // put non-deleted instances first in static storage array
    for (int i=0; i < AngleSensor::get_num_sensors(); i++)
    {
        m_AngleSensor_ptr[i] == temp_array[i];
    }
    m_num_sensors--; // last pointer should now be ignored when looping through array
}

int32_t AngleSensor::get_position()
{
    return m_encoder.read();
}

void AngleSensor::update_velocity(float sampling_freq)
{
    float curr_angle = get_angle();
    
    if (curr_angle - m_last_angle != 0)
    {
        m_counts_since_last_tick=0;
    }
    m_counts_since_last_tick ++;
    m_velocity = (curr_angle - m_last_angle) * sampling_freq*m_counts_since_last_tick;
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
    int32_t pos;
    m_use_degrees ? pos = angle / m_degree_per_tick : pos = angle/m_radian_per_tick;
    m_last_angle = angle;
    m_encoder.write(pos);
}

static void AngleSensor::update_all()
{
  for (int i=0; i<AngleSensor::get_num_sensors(); i++)
  {
    AngleSensor::get_sensor_ptr(i)->update_velocity(m_global_update_freq);
  }
}

static void AngleSensor::zero_all()
{
    for (int i=0; i<AngleSensor::get_num_sensors(); i++)
  {
    AngleSensor::get_sensor_ptr(i)->zero();
  }
}

static void AngleSensor::set_global_update_freq(float freq)
{
    m_global_update_freq = freq;
}

void AngleSensor::set_degrees(bool use_degrees)
{
    m_use_degrees = use_degrees;
}

void AngleSensor::set_pulses_per_rev(uint16_t pulses_per_rev)
{
    m_pulses_per_rev = pulses_per_rev;
    m_degree_per_tick = DEGREES/(float)m_pulses_per_rev;
    m_radian_per_tick = RADIANS/(float)m_pulses_per_rev;
}
