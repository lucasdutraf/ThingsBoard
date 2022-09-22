#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "inclination_connection.h"

#define INCLINATION_GPIO_PORT 13

void inclination_sensor_setup(){
    gpio_pad_select_gpio(INCLINATION_GPIO_PORT);
    gpio_set_direction(INCLINATION_GPIO_PORT, GPIO_MODE_INPUT);
    gpio_pulldown_en(INCLINATION_GPIO_PORT);
    gpio_pullup_dis(INCLINATION_GPIO_PORT);
}

int getDigitalOutput(){
    return gpio_get_level(INCLINATION_GPIO_PORT);
}