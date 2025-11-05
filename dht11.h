#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

/**
 * @brief Initialize the DHT11 sensor on the given GPIO data pin.
 *
 * This prepares the pin for use by the library. Call before get_temperature()
 * or get_humidity(). Calling again will reassign the data pin.
 *
 * @param _data_pin GPIO pin number connected to the DHT11 data line.
 */
void dht11_init(uint32_t _data_pin);

/**
 * @brief Deinitialize the DHT11 sensor and free the GPIO pin.
 */
void dht11_deinit();

/**
 * @brief Read the latest temperature in Celsius.
 *
 * Triggers a sensor read if the cached data is older than the sensor's
 * minimum sampling interval. Returns -1.0 on error.
 *
 * @return temperature in Celsius, or -1.0 on failure.
 */
float get_temperature();

/**
 * @brief Read the latest relative humidity in percent.
 *
 * Triggers a sensor read if the cached data is older than the sensor's
 * minimum sampling interval. Returns -1.0 on error.
 *
 * @return relative humidity in % (0–100), or -1.0 on failure.
 */
float get_humidity();

#endif // DHT11_H