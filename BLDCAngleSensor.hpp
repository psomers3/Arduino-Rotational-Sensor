#ifndef BLDCAngleSensor_hpp
#define BLDCAngleSensor_hpp

#define ENCODER_USE_INTERRUPTS
#include "stdlib.h"
#include "BLDCEncoder.h"
#include <Arduino.h>

#define MaxNumofSensors 5


class BLDCAngleSensor
{
private:
    static BLDCAngleSensor* m_BLDCAngleSensor_ptr[];  // array of pointers to created BLDCAngleSensors
    static uint8_t m_num_sensors;             // how many BLDCAngleSensors exist
    float m_velocity;                         // last recorded angular velocity
    float m_last_angle;                       // last recorded angle
    float m_pre_last_angle;
    BLDCEncoder m_encoder;                        // encoder object used to get position
    static float m_global_update_freq;        // frequency in Hz to update sensors at
    uint16_t m_pulses_per_rev;                // resolution of the Encoder
    bool m_use_degrees;                       // Boolean value to use degrees or radians
    float m_degree_per_tick;                  // how many degrees per tick of the encoder
    float m_radian_per_tick;                  // how many radians per tick of the encoder
    unsigned long m_micros_at_last_change;
    unsigned long m_micros_at_pre_last_change;
    
    /// Returns number of existing BLDCAngleSensors
    /**
     * @return number of existing BLDCAngleSensor classes
     */
    static uint8_t get_num_sensors();
    
    /// Returns pointer to the sensor at index in m_BLDCAngleSensor_ptr
    /**
     * @param index The index value of the desired BLDCAngleSensor in m_BLDCAngleSensor_ptr
     *
     * @return A pointer to an BLDCAngleSensor object
     */
    static BLDCAngleSensor* get_sensor_ptr(uint8_t index);
    
    
public:
    ///Constructor
    /**
     * Creates an BLDCAngleSensor object. One of the input pin params should be an interrupt enablable pin.
     * @param pinA Digital input pin the corresponds to A signal for encoder.
     * @param pinB Digital input pin the corresponds to B signal for encoder.
     */
    BLDCAngleSensor(uint8_t pinA, uint8_t pinB, uint8_t pinC);
    
    ///Destructor
    ~BLDCAngleSensor();
    
    void update_velocity(float sampling_freq);
    
    
    /// Updates the velocity and angles of each BLDCAngleSensor object
    /**
     * This function will cycle through each instance of BLDCAngleSensor and update the velocities and angular positions
     * @param freq Sampling frequency used to update the sensors
     */
    static void update_all();
    
    /// Zeros all BLDCAngleSensor objects
    /**
     * This function will iterate through each instance of BLDCAngleSensor and zero the states (e.g. velocity and position)
     */
    static void zero_all();
    
    ///Returns the current counter value of the encoder
    /**
     * This function will return a int32_t value as the counter value of the encoder
     * @return int32_t value with number of counts on the encoder
     */
    int32_t get_position();
    
    ///Returns the current angular velocity read by the sensor
    /**
     * This function will return the angular velocity of the sensor in units defined in BLDCAngleSensor.hpp (e.g. rad/sec)
     * @return float value with angular velocity of sensor
     */
    float get_velocity();
    
    ///Returns the current angular position of sensor
    /**
     * This function will return the angular position of the sensor in units defined in BLDCAngleSensor.hpp (e.g. rad or degrees)
     * @return float value with angular position of sensor
     */
    float get_angle();
    
    ///Zeros the sensor
    /**
     * This function will zero the states of the BLDCAngleSensor object (e.g. position and velocity)
     */
    void zero();                  //zeros sensor
    
    ///Sets the Anglular position of the Sensor
    /**
     * This function sets the sensor angular position to a specific angle. Units are defined in BLDCAngleSensor.hpp
     * @param angle the angle to set the sensor value to.
     */
    void set_angle(float angle);  //sets current sensor angle
    
    /// Sets the update frequency to update all sensors at
    /**
     * @param freq Frequency to update at in Hz.
     */
    static void set_global_update_freq(float freq);
    
    /// Sets the sensor to use degrees or radians
    /**
     * @param use_degrees Whether or not to use degrees for returning values.
     */
    void set_degrees(bool use_degrees);
    
    /// Sets the resolution of the encoder
    /**
     * @param pulses_per_rev Number of pulses for one full revolution of the encoder.
     */
    void set_pulses_per_rev(uint16_t pulses_per_rev);
    
    /// Filler function to return last recorded angle from standard update
    /**
     * @return float value of the angle of the encoder shaft
     */
    float return_angle();
    
    /// Clean-up function to be called before deleting object.
    /** This is done because I don't know how to make sure the destructor is called.
     */
    void remove_from_sensors();
    
};//BLDCAngleSensor

#endif /* BLDCAngleSensor_hpp */
