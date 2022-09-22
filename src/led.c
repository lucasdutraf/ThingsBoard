#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "led.h"

#define LED 2

void led_setup(){
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void led_state(int state){
    gpio_set_level(LED, state);
}

void turn_on_led(){
    led_state(1);
}

void turn_off_led(){
    led_state(0);
}