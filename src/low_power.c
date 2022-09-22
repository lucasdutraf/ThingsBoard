#include <time.h>
#include <stdio.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/rom/uart.h"
#include "driver/rtc_io.h"

#define BOOT_BUTTON_GPIO 0

void light_sleep()
{
    gpio_pad_select_gpio(BOOT_BUTTON_GPIO);
    gpio_set_direction(BOOT_BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_wakeup_enable(BOOT_BUTTON_GPIO, GPIO_INTR_LOW_LEVEL);

    esp_sleep_enable_gpio_wakeup();

    if (rtc_gpio_get_level(BOOT_BUTTON_GPIO) == 0) {
        printf("Button pressed, not going to sleep...\n");
        do {
            vTaskDelay(pdMS_TO_TICKS(10));
        } while (rtc_gpio_get_level(BOOT_BUTTON_GPIO) == 0);
    }
    
    printf("Going to light sleep now.");

    uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

    esp_light_sleep_start();
}

void deep_sleep() {
    rtc_gpio_pullup_en(BOOT_BUTTON_GPIO);
    rtc_gpio_pulldown_dis(BOOT_BUTTON_GPIO);
    esp_sleep_enable_ext0_wakeup(BOOT_BUTTON_GPIO, 0);

    printf("Woke up!\n");
    printf("Going to Deep Sleep now\n");

    // Coloca a ESP no Deep Sleep
    esp_deep_sleep_start();
}