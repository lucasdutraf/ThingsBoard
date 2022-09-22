//standard includes
#include <stdio.h>
#include <string.h>

//esp includes
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

//freertos includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

//driver includes
#include "nvs_flash.h"
#include "sdkconfig.h"
#include "driver/gpio.h"

//local includes
#include "wifi.h"
#include "http_client.h"
#include "mqtt.h"
#include "dht.h"
#include "delay.h"
#include "inclination_connection.h"
#include "led.h"


xSemaphoreHandle conexaoWifiSemaphore;
xSemaphoreHandle conexaoMQTTSemaphore;

void conectadoWifi(void * params)
{
    while(true) {
        if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY)) {
            // Processamento Internet
            mqtt_start();
        }
    }
}

void monitorInclination(void * params)
{
    while(true) {
        // if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)) {
            // Processamento Internet
        int inclination = getDigitalOutput();
        printf("Inclination: %d\n", inclination);
        led_state(inclination);
        vTaskDelay(100 / portTICK_PERIOD_MS);
            // if(getDigitalOutput() == 1){
            //     printf("Inclinação detectada\n");
            // }
            // else {
            //     printf("Inclinação não detectada\n");
            // }
            // int digitalOutput = getDigitalOutput();
            // printf("Digital Output: %d)", digitalOutput);
        // }
    }
}

void trataComunicacaoComServidor(void * params)
{
    char telemetry[200];
    char attributes[200];

    if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY)) {
        while(true) {
            float dhtTemperature = DHT11_read().temperature;
            float dhtHumidity = DHT11_read().humidity;
            int dhtStatus = DHT11_read().status;

            // assemble json
            sprintf(
                telemetry,
                "{\"temperature\": %f, \n\"humidity\": %f}",
                dhtTemperature,
                dhtHumidity
            );
            // send json
            mqtt_envia_mensagem("v1/devices/me/telemetry", telemetry);

            sprintf(
                attributes,
                "{\"status_code\": %d}",
                dhtStatus
            );
            mqtt_envia_mensagem("v1/devices/me/attributes", attributes);

            // delay 1s
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void)
{
    // Initialize DHT.
    DHT11_init(GPIO_NUM_4);

    // Initialize inclination sensor.
    inclination_sensor_setup();

    // Initialize LED.
    led_setup();

    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "MQTT connection", 4096, NULL, 1, NULL);
    // xTaskCreate(&trataComunicacaoComServidor, "Broker connection", 4096, NULL, 1, NULL);
    xTaskCreate(&monitorInclination, "Inclination connection", 4096, NULL, 1, NULL);

    // works only after unplugging and plugging the USB cable

}
