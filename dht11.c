#include "dht11.h"
#include <stdint.h>
#include "hardware/gpio.h"
#include "pico/time.h"

static uint32_t data_pin = 999;     // GPIO pin connected to DHT11 data line
static uint8_t data[5];             // Data buffer for DHT11 readings
static int last_probe = -1;         // Timestamp of last successful data read in ms since boot
static int data_valid = 0;          // Flag indicating if the last read data is valid

// Sends the start signal to the DHT11 sensor
static void dht11_start_signal(uint32_t pin){

    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, 1);

    // Ensure at least 1 second has passed since boot
    if(to_ms_since_boot(get_absolute_time()) < 1000)
        sleep_ms(1000 - to_ms_since_boot(get_absolute_time()));

    gpio_put(pin, 0);
    sleep_ms(18);

    gpio_put(pin, 1);
    sleep_us(20);

    gpio_set_dir(pin, GPIO_IN);
}

// Returns 1 if response is valid, 0 otherwise
static int dht11_check_response(uint32_t pin){
    
    sleep_us(40);
    if(gpio_get(pin) == 0){
        sleep_us(80);
        if(gpio_get(pin) == 1){
            while(gpio_get(pin) == 1);
            return 1;
        }
    }
    return 0;
}

// Reads 40 bits of data from the DHT11 sensor into the provided buffer
static void dht11_read_data(uint32_t pin, uint8_t* data){

    for(int i = 0; i < 5; i++)
        data[i] = 0;

    for(int i = 0; i < 40; i++){
        while(gpio_get(pin) == 0);
        sleep_us(40);
        if(gpio_get(pin) == 1)
            data[i / 8] |= (1 << (7 - (i % 8)));
        while(gpio_get(pin) == 1);
    }
}

// Gathers data from the DHT11 sensor, returns 1 on success, 0 on failure
static int gather_data(){

    dht11_start_signal(data_pin);
    if(dht11_check_response(data_pin)){
        dht11_read_data(data_pin, data);
        last_probe = to_ms_since_boot(get_absolute_time());

        uint8_t checksum = data[0] + data[1] + data[2] + data[3];
        if(checksum != data[4])
            return 0;
        else return 1;
    }
    else return 0;
}

// Initializes the DHT11 sensor on the specified data pin. This function may be called again to change the data pin.
void dht11_init(uint32_t _data_pin){
    
    if(data_pin != 999)
        gpio_deinit(data_pin);
    if(_data_pin > 29)
        return; // Invalid pin
    data_pin = _data_pin;
    gpio_init(data_pin);
}

// Deinitializes the DHT11 sensor, freeing the data pin for other uses
void dht11_deinit(){
    if(data_pin > 29)
        return;
    gpio_deinit(data_pin);
    data_pin = 999;
}

// Returns temperature in Celsius, or -1.0 on failure
float get_temperature(){
    
    if(data_pin == 999)
        return -1.0;
    
    int success = 0;
    if(to_ms_since_boot(get_absolute_time()) - last_probe > 2000 || last_probe == -1){
        success = gather_data();
        data_valid = success;
    }
    if(data_valid)
        return data[2] + data[3] / 10.0;
    else
        return -1.0;
}

// Returns humidity in percentage, or -1.0 on failure
float get_humidity(){

    if(data_pin == 999)
        return -1.0;

    int success = 0;
    if(to_ms_since_boot(get_absolute_time()) - last_probe > 2000 || last_probe == -1){
        success = gather_data();
        data_valid = success;
    }
    if(data_valid)
        return data[0] + data[1] / 10.0;
    else
        return -1.0;
}
