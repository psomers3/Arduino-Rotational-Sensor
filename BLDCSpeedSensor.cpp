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
            attachInterrupt(digitalPinToInterrupt(pinB), ISR_update_velocity1, FALLING);
            attachInterrupt(digitalPinToInterrupt(pinC), ISR_update_velocity2, FALLING);
            break;
            
        default:
            break;
    }
    unsigned long now = micros();
    for (uint8_t i = 0; i<0; i++)
    {
        m_micros_at_last_change[i] = now;
        m_micros_at_prev_to_last_change[i] = now;
    }
    for (uint8_t i = 0; i<HISTORY_LENGTH; i++)
    {
        m_history1[i] = 0;
        m_history2[i] = 0;
        m_history3[i] = 0;
    }
    for (uint8_t i = 0; i < 0; i++)
    {
        m_directions[i] = 1;
    }
    m_BLDCSpeedSensor_ptr[m_num_sensors] = this;
    m_num_sensors ++;
    m_last_pole = 0;
}

static void BLDCSpeedSensor::ISR_update_velocity0()
{
    unsigned long now = micros();
    BLDCSpeedSensor* sensor = get_sensor_ptr(0);
    sensor->m_directions[sensor->m_direction_index] = (sensor->m_last_pole == 1)? -1: 1;
    sensor->m_direction_index = (sensor->m_direction_index + 1) % 10;
    sensor->m_last_pole = 0;
    sensor->m_history1[sensor->history1_index] = now - sensor->m_micros_at_last_change[0];
    sensor->m_micros_at_last_change[0] = now;
    sensor->history1_index++;
    if (sensor->history1_index == HISTORY_LENGTH)
        sensor->history1_index = 0;
}

static void BLDCSpeedSensor::ISR_update_velocity1()
{
    unsigned long now = micros();
    BLDCSpeedSensor* sensor = get_sensor_ptr(0);
    sensor->m_directions[sensor->m_direction_index] = (sensor->m_last_pole == 2)? -1: 1;
    sensor->m_direction_index = (sensor->m_direction_index + 1) % 10;
    sensor->m_last_pole = 1;
    sensor->m_history2[sensor->history2_index] = now - sensor->m_micros_at_last_change[1];
    sensor->m_micros_at_last_change[1] = now;
    sensor->history2_index++;
    if (sensor->history2_index == HISTORY_LENGTH)
        sensor->history2_index = 0;
}

static void BLDCSpeedSensor::ISR_update_velocity2()
{
    unsigned long now = micros();
    BLDCSpeedSensor* sensor = get_sensor_ptr(0);
    sensor->m_directions[sensor->m_direction_index] = (sensor->m_last_pole == 0)? -1: 1;
    sensor->m_direction_index = (sensor->m_direction_index + 1) % 10;
    sensor->m_last_pole = 2;
    sensor->m_history1[sensor->history3_index] = now - sensor->m_micros_at_last_change[2];
    sensor->m_micros_at_last_change[2] = now;
    sensor->history3_index++;
    if (sensor->history3_index == HISTORY_LENGTH)
        sensor->history3_index = 0;
}

float BLDCSpeedSensor::get_speed()
{
    float total = 0;
    float average;
    float direction_avg = 0;
    unsigned long now = micros();
    uint16_t total_samples=0;
    for (uint8_t i = 0; i<3; i++)
    {
        if (now - m_micros_at_last_change[i] > 1e5)  // cutoff to set everything to 0
        {
            for (uint8_t j = 0; j<HISTORY_LENGTH; j++)
            {
                m_history1[j] = m_history2[j] = m_history3[j] = 0;
            }
            history1_index = history2_index = history3_index = 0;
            return 0.0;
        }
    }

    for (uint8_t i = 0; i<HISTORY_LENGTH; i++)
    {
        total += m_history1[i];
        total_samples ++;

        if (i < HISTORY_LENGTH-1)
        {
            if (m_history1[i+1] == 0)
                break;
        }
        total += m_history2[i];
        total_samples ++;

        if (i < HISTORY_LENGTH-1)
        {
            if (m_history2[i+1] == 0)
                break;
        }
        total += m_history3[i];
        total_samples ++;
        if (i < HISTORY_LENGTH-1)
        {
            if (m_history3[i+1] == 0)
                break;
        }
    }
    average = total/(float)total_samples;
    average = (60/(average/2e6));  // convert to rpm. divide by 2 assuming a hall effect sensors triggers twice per rotation
    
    for (uint8_t i = 0; i < 10; i++)
    {
        direction_avg += m_directions[i];
    }
    if (direction_avg >= 0)
    {
        return average;
    }
    else
    {
        return -average;
    }
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
