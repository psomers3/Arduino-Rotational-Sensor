#ifndef BLDCSpeedSensor_hpp
#define BLDCSpeedSensor_hpp

#define ENCODER_USE_INTERRUPTS
#include "stdlib.h"
#include <Arduino.h>

#define MaxNumofSensors 5
constexpr uint8_t HISTORY_LENGTH = 20;


class BLDCSpeedSensor
{
private:
    static BLDCSpeedSensor* m_BLDCSpeedSensor_ptr[];  // array of pointers to created SpeedSensors
    static uint8_t m_num_sensors;             // how many SpeedSensors exist
    unsigned long m_micros_at_last_change[3];
    unsigned long m_micros_at_prev_to_last_change[3];
    unsigned long m_history1[HISTORY_LENGTH];
    unsigned long m_history2[HISTORY_LENGTH];
    unsigned long m_history3[HISTORY_LENGTH];
    int history1_index;
    int history2_index;
    int history3_index;
    uint8_t m_last_pole;
    int8_t m_directions[10];
    uint8_t m_direction_index;

    
    /// Returns number of existing BLDCSpeedSensors
    /**
     * @return number of existing BLDCSpeedSensor classes
     */
    static uint8_t get_num_sensors();
    
    /// Returns pointer to the sensor at index in m_BLDCSpeedSensor_ptr
    /**
     * @param index The index value of the desired BLDCSpeedSensor in m_BLDCSpeedSensor_ptr
     *
     * @return A pointer to an BLDCSpeedSensor object
     */
    static BLDCSpeedSensor* get_sensor_ptr(uint8_t index);
    
    static void ISR_update_velocity0();
    static void ISR_update_velocity1();
    static void ISR_update_velocity2();

public:
    ///Constructor
    /**
     * Creates an BLDCSpeedSensor object. One of the input pin params should be an interrupt enablable pin.
     * @param pinA Digital input pin the corresponds to A signal for encoder.
     * @param pinB Digital input pin the corresponds to B signal for encoder.
     */
    BLDCSpeedSensor(uint8_t pinA, uint8_t pinB, uint8_t pinC);
    
    ///Destructor
    ~BLDCSpeedSensor();
    
    float get_speed();
    
    /// Clean-up function to be called before deleting object.
    /** This is done because I don't know how to make sure the destructor is called.
     */
    void remove_from_sensors();

};//BLDCSpeedSensor

#endif /* BLDCSpeedSensor_hpp */
