# Pi-Pico-DHT11-Sensor-Driver

A hardware driver for DHT11 temperature and humidity sensor for Raspberry Pi Pico.

The datasheet for DHT11 can be found here: https://www.mouser.com/datasheet/2/758/DHT11-Technical-Data-Sheet-Translated-Version-1143054.pdf?srsltid=AfmBOorHk04hXvajxTtvPQNMt1NmSQ4ka0VTqYCkNY3Le2Wc7Gs2OeAv

## API Usage

### `void dht11_init(uint32_t _data_pin)`

Initialize the DHT11 sensor on the given GPIO data pin.
This prepares the pin for use by the library. Call before `get_temperature()`
or `get_humidity()`. Calling again will reassign the data pin.

Parameter `_data_pin`: GPIO pin number connected to the DHT11 data line.

### `void dht11_deinit()`

Deinitialize the DHT11 sensor and free the GPIO pin.

### `float get_temperature()`

Read the latest temperature in Celsius. Triggers a sensor read if the cached data is older than the sensor's minimum sampling interval. Returns -1.0 on error.

Returns: temperature in Celsius, or -1.0 on failure.

### `float get_humidity()`

Read the latest relative humidity in percent. Triggers a sensor read if the cached data is older than the sensor's minimum sampling interval. Returns -1.0 on error.

Returns: relative humidity in % (0–100), or -1.0 on failure.
