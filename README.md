# Arduino-Rotational-Sensor
An Arduino class for tracking a quadrature encoder's position and speed.


# class `AngleSensor` 

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`AngleSensor`](#class_angle_sensor_1a6a56f2c5271666cf043fc77e5975da1a)`(uint8_t pinA,uint8_t pinB)` | Constructor.
`public  `[`~AngleSensor`](#class_angle_sensor_1a12daf25594fe1aa08b0be9b36cf13740)`()` | Destructor.
`public void `[`update_velocity`](#class_angle_sensor_1a08f88fdef8d4b494a6af8759e5dadc31)`(float sampling_freq)` | 
`public int32_t `[`get_position`](#class_angle_sensor_1abb8f14ae9c51c8bea6725bcf8c3de818)`()` | Returns the current counter value of the encoder.
`public float `[`get_velocity`](#class_angle_sensor_1a9f4693cbdfc73dd73c07283c545df9a5)`()` | Returns the current angular velocity read by the sensor.
`public float `[`get_angle`](#class_angle_sensor_1a5ad536e88b8aaa119c4470857cfa1afe)`()` | Returns the current angular position of sensor.
`public void `[`zero`](#class_angle_sensor_1a79bf883f9af57a320296158819767599)`()` | Zeros the sensor.
`public void `[`set_angle`](#class_angle_sensor_1a79424ff2525bf988d81952a1e56079a7)`(float angle)` | Sets the Anglular position of the Sensor.
`public void `[`set_degrees`](#class_angle_sensor_1a91c872bc79ebd2768b6bb39184b2a531)`(bool use_degrees)` | Sets the sensor to use degrees or radians.
`public void `[`set_pulses_per_rev`](#class_angle_sensor_1abe3bdd3969c32b1f51ba8632856a2ca9)`(uint16_t pulses_per_rev)` | Sets the resolution of the encoder.
`public float `[`return_angle`](#class_angle_sensor_1a5da2bf4d5d1779b281f3c032ce83c8bf)`()` | Filler function to return last recorded angle from standard update.
`public void `[`remove_from_sensors`](#class_angle_sensor_1af85d19e2143731a6481f564130462418)`()` | Clean-up function to be called before deleting object.

## Members

#### `public  `[`AngleSensor`](#class_angle_sensor_1a6a56f2c5271666cf043fc77e5975da1a)`(uint8_t pinA,uint8_t pinB)` 

Constructor.

Creates an [AngleSensor](#class_angle_sensor) object. One of the input pin params should be an interrupt enablable pin. 
#### Parameters
* `pinA` Digital input pin the corresponds to A signal for encoder. 

* `pinB` Digital input pin the corresponds to B signal for encoder.

#### `public  `[`~AngleSensor`](#class_angle_sensor_1a12daf25594fe1aa08b0be9b36cf13740)`()` 

Destructor.

#### `public void `[`update_velocity`](#class_angle_sensor_1a08f88fdef8d4b494a6af8759e5dadc31)`(float sampling_freq)` 

#### `public int32_t `[`get_position`](#class_angle_sensor_1abb8f14ae9c51c8bea6725bcf8c3de818)`()` 

Returns the current counter value of the encoder.

This function will return a int32_t value as the counter value of the encoder 
#### Returns
int32_t value with number of counts on the encoder

#### `public float `[`get_velocity`](#class_angle_sensor_1a9f4693cbdfc73dd73c07283c545df9a5)`()` 

Returns the current angular velocity read by the sensor.

This function will return the angular velocity of the sensor in units defined in [AngleSensor.hpp](#_angle_sensor_8hpp) (e.g. rad/sec) 
#### Returns
float value with angular velocity of sensor

#### `public float `[`get_angle`](#class_angle_sensor_1a5ad536e88b8aaa119c4470857cfa1afe)`()` 

Returns the current angular position of sensor.

This function will return the angular position of the sensor in units defined in [AngleSensor.hpp](#_angle_sensor_8hpp) (e.g. rad or degrees) 
#### Returns
float value with angular position of sensor

#### `public void `[`zero`](#class_angle_sensor_1a79bf883f9af57a320296158819767599)`()` 

Zeros the sensor.

This function will zero the states of the [AngleSensor](#class_angle_sensor) object (e.g. position and velocity)

#### `public void `[`set_angle`](#class_angle_sensor_1a79424ff2525bf988d81952a1e56079a7)`(float angle)` 

Sets the Anglular position of the Sensor.

This function sets the sensor angular position to a specific angle. Units are defined in [AngleSensor.hpp](#_angle_sensor_8hpp)
#### Parameters
* `angle` the angle to set the sensor value to.

#### `public void `[`set_degrees`](#class_angle_sensor_1a91c872bc79ebd2768b6bb39184b2a531)`(bool use_degrees)` 

Sets the sensor to use degrees or radians.

#### Parameters
* `use_degrees` Whether or not to use degrees for returning values.

#### `public void `[`set_pulses_per_rev`](#class_angle_sensor_1abe3bdd3969c32b1f51ba8632856a2ca9)`(uint16_t pulses_per_rev)` 

Sets the resolution of the encoder.

#### Parameters
* `pulses_per_rev` Number of pulses for one full revolution of the encoder.

#### `public float `[`return_angle`](#class_angle_sensor_1a5da2bf4d5d1779b281f3c032ce83c8bf)`()` 

Filler function to return last recorded angle from standard update.

#### Returns
float value of the angle of the encoder shaft

#### `public void `[`remove_from_sensors`](#class_angle_sensor_1af85d19e2143731a6481f564130462418)`()` 

Clean-up function to be called before deleting object.

This is done because I don't know how to make sure the destructor is called.

Generated by [Moxygen](https://sourcey.com/moxygen)
