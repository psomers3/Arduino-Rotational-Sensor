#include "BLDCAngleSensor.hpp"
#include <Arduino.h>

#define DEGREES 360
#define RADIANS 6.283185

uint8_t BLDCAngleSensor::m_num_sensors = 0;
BLDCAngleSensor* BLDCAngleSensor::m_BLDCAngleSensor_ptr[MaxNumofSensors];
float BLDCAngleSensor::m_global_update_freq = 50;

BLDCAngleSensor::BLDCAngleSensor(uint8_t pinA, uint8_t pinB, uint8_t pinC):m_encoder(pinA, pinB, pinC)  // <- encoder object is initialized here
{
    m_BLDCAngleSensor_ptr[m_num_sensors] = this;  // add pointer of this instance to array
    m_num_sensors++;
    m_pulses_per_rev = 2400;  // set resolution of testing encoder
    m_degree_per_tick = (float)DEGREES/(float)m_pulses_per_rev;
    m_radian_per_tick = (float)RADIANS/(float)m_pulses_per_rev;
    m_use_degrees = true;  // default to degrees
    m_micros_at_last_change = micros();
}

uint8_t BLDCAngleSensor::get_num_sensors()
{
    return m_num_sensors;
}

BLDCAngleSensor* BLDCAngleSensor::get_sensor_ptr(uint8_t index)
{
    return m_BLDCAngleSensor_ptr[index];
}

float BLDCAngleSensor::return_angle()
{
    return m_last_angle;
}

float BLDCAngleSensor::get_angle()
{
    if (m_use_degrees)
        return (float)get_position()*m_degree_per_tick;
    else
        return (float)get_position()*m_radian_per_tick;
}

//destructor
BLDCAngleSensor::~BLDCAngleSensor(){}

void BLDCAngleSensor::remove_from_sensors()
{
    BLDCAngleSensor* temp_array[MaxNumofSensors];
    int curr_index = 0;
    // gather all pointers to instances that are not being deleted
    for (int i=0; i < BLDCAngleSensor::get_num_sensors(); i++)
    {
        if (BLDCAngleSensor::get_sensor_ptr(i) != this)
        {
            temp_array[curr_index] = BLDCAngleSensor::get_sensor_ptr(i);
        }
    }
    // put non-deleted instances first in static storage array
    for (int i=0; i < BLDCAngleSensor::get_num_sensors(); i++)
    {
        m_BLDCAngleSensor_ptr[i] == temp_array[i];
    }
    m_num_sensors--; // last pointer should now be ignored when looping through array
}

int32_t BLDCAngleSensor::get_position()
{
    return m_encoder.read();
}

void BLDCAngleSensor::update_velocity(float sampling_freq)
{
    float curr_angle = get_angle();
    unsigned long current_time = micros();
    unsigned long time_delta = current_time - m_micros_at_last_change;
    unsigned long time_delta_2 = current_time - m_micros_at_pre_last_change;
    
    if (curr_angle - m_last_angle != 0)
    {
        m_velocity = (curr_angle - m_last_angle) / ((double)time_delta / 1e6);
        m_micros_at_pre_last_change = m_micros_at_last_change;
        m_micros_at_last_change = current_time;
        m_pre_last_angle = m_last_angle;
        m_last_angle = curr_angle;
    }
    else
    {
        m_velocity = (curr_angle - m_pre_last_angle) / ((double)time_delta_2 / 1e6);
        if (time_delta > 1e5)
            m_velocity = 0;
    }
}

float BLDCAngleSensor::get_velocity()
{
    return m_velocity;
}

void BLDCAngleSensor::zero()
{
    m_velocity = 0;
    m_last_angle = 0;
    m_pre_last_angle = 0;
    m_encoder.write(0);
}

void BLDCAngleSensor::set_angle(float angle)
{
    int32_t pos;
    m_use_degrees ? pos = angle / m_degree_per_tick : pos = angle/m_radian_per_tick;
    m_last_angle = angle;
    m_pre_last_angle = angle;
    m_encoder.write(pos);
}

static void BLDCAngleSensor::update_all()
{
    for (int i=0; i<BLDCAngleSensor::get_num_sensors(); i++)
    {
        BLDCAngleSensor::get_sensor_ptr(i)->update_velocity(m_global_update_freq);
    }
}

static void BLDCAngleSensor::zero_all()
{
    for (int i=0; i<BLDCAngleSensor::get_num_sensors(); i++)
    {
        BLDCAngleSensor::get_sensor_ptr(i)->zero();
    }
}

static void BLDCAngleSensor::set_global_update_freq(float freq)
{
    m_global_update_freq = freq;
}

void BLDCAngleSensor::set_degrees(bool use_degrees)
{
    m_use_degrees = use_degrees;
}

void BLDCAngleSensor::set_pulses_per_rev(uint16_t pulses_per_rev)
{
    m_pulses_per_rev = pulses_per_rev;
    m_degree_per_tick = DEGREES/(float)m_pulses_per_rev;
    m_radian_per_tick = RADIANS/(float)m_pulses_per_rev;
}
