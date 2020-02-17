#ifndef AngleSensor_hpp
#define AngleSensor_hpp

#define ENCODER_USE_INTERRUPTS
#include "./Encoder/Encoder.h"
#include <Arduino.h>

#define RADIANS
//#define DEGREES

#define MaxNumofSensors 5


class AngleSensor
{
private:
    static AngleSensor* m_AngleSensor_ptr[];
    static uint8_t m_num_sensors;
    float m_velocity; // angular velocity
    float m_last_angle;
    Encoder m_encoder; //encoder object used to get position
    void update_velocity(float sampling_freq);
    static uint8_t get_num_sensors();
    static AngleSensor* get_sensor_ptr(uint8_t index);

    
public:
    ///Constructor
    /**
     * Creates an AngleSensor object. One of the input pin params should be an interrupt enablable pin.
     * @param pinA Digital input pin the corresponds to A signal for encoder.
     * @param pinB Digital input pin the corresponds to B signal for encoder.
     */
    AngleSensor(uint8_t pinA, uint8_t pinB);
    
    ///Destructor
    ~AngleSensor();

    /// Updates the velocity and angles of each AngleSensor object
    /**
     * This function will cycle through each instance of AngleSensor and update the velocities and angular positions
     * @param freq Sampling frequency used to update the sensors
     */
    static void sensor_updating(float freq);

    /// Zeros all AngleSensor objects
    /*
     * This function will iterate through each instance of AngleSensor and zero the states (e.g. velocity and position)
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
     * This function will return the angular velocity of the sensor in units defined in AngleSensor.hpp (e.g. rad/sec)
     * @return float value with angular velocity of sensor
     */
    float get_velocity();

    ///Returns the current angular position of sensor
    /**
     * This function will return the angular position of the sensor in units defined in AngleSensor.hpp (e.g. rad or degrees)
     * @return float value with angular position of sensor
     */
    float get_angle();

    ///Zeros the sensor
    /**
     * This function will zero the states of the AngleSensor object (e.g. position and velocity)
     */
    void zero();                  //zeros sensor

    ///Sets the Anglular position of the Sensor
    /**
     * This function sets the sensor angular position to a specific angle. Units are defined in AngleSensor.hpp
     * @param angle the angle to set the sensor value to.
     */    
    void set_angle(float angle);  //sets current sensor angle
    
};//AngleSensor

#endif /* AngleSensor_hpp */
