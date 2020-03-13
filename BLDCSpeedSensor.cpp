#include "BLDCSpeedSensor.hpp"
#include <Arduino.h>

#define DEGREES 360
#define RADIANS 6.283185


uint8_t BLDCSpeedSensor::m_num_sensors = 0;
BLDCSpeedSensor* BLDCSpeedSensor::m_BLDCSpeedSensor_ptr[MaxNumofSensors];

BLDCSpeedSensor::BLDCSpeedSensor(uint8_t pinA, uint8_t pinB, uint8_t pinC)
{
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    pinMode(pinC, INPUT_PULLUP);
    switch (m_num_sensors) {
        case 0:
            attachInterrupt(digitalPinToInterrupt(pinA), ISR_update_velocity0, FALLING);
            attachInterrupt(digitalPinToInterrupt(pinB), ISR_update_velocity0, FALLING);
            attachInterrupt(digitalPinToInterrupt(pinC), ISR_update_velocity0, FALLING);
            break;
            
        default:
            break;
    }
    m_BLDCSpeedSensor_ptr[m_num_sensors] = this;
    m_num_sensors ++;
    
    m_velocity = 0;
    m_micros_at_last_change = micros();
}

static void BLDCSpeedSensor::ISR_update_velocity0()
{
    unsigned long now = micros();
    BLDCSpeedSensor* sensor = get_sensor_ptr(0);
    unsigned long delta = now - sensor->m_micros_at_last_change;
    sensor->m_micros_at_last_change = now;

    if (delta > 1e5) sensor->m_velocity = 0;

    sensor->m_velocity = 0.33333333/(((float)delta / 1e6) / 60);  // rpm
}

float BLDCSpeedSensor::get_speed()
{
    return m_velocity;
}

//destructor
BLDCSpeedSensor::~BLDCSpeedSensor(){}

void BLDCSpeedSensor::remove_from_sensors()
{
    BLDCSpeedSensor* temp_array[MaxNumofSensors];
    int curr_index = 0;
    // gather all pointers to instances that are not being deleted
    for (int i=0; i < BLDCSpeedSensor::get_num_sensors(); i++)
    {
        if (BLDCSpeedSensor::get_sensor_ptr(i) != this)
        {
            temp_array[curr_index] = BLDCSpeedSensor::get_sensor_ptr(i);
        }
    }
    // put non-deleted instances first in static storage array
    for (int i=0; i < BLDCSpeedSensor::get_num_sensors(); i++)
    {
        m_BLDCSpeedSensor_ptr[i] == temp_array[i];
    }
    m_num_sensors--; // last pointer should now be ignored when looping through array
}

uint8_t BLDCSpeedSensor::get_num_sensors()
{
    return m_num_sensors;
}

BLDCSpeedSensor* BLDCSpeedSensor::get_sensor_ptr(uint8_t index)
{
    return m_BLDCSpeedSensor_ptr[index];
}
