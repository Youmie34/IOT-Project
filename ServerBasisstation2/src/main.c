#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_check.h"
#include "esp_log.h"
#include "esp_event.h"
#include <string.h>
#include "wifi.h"
#include "server.h"
#include "client.h"
#include "tempHumi.h"
#include "urlEsp.h"

enum fenster
{
    zu,
    auf
};

void app_main()
{
    enum fenster window = auf;
    wifi_init();
    start_webserver();

    // settings for i2c communication with bme680
    ESP_ERROR_CHECK(i2cdev_init());
    // initializing and configuration for the bme680
    bme680_init();
    bme680_config();

    while (true)
    {
        if (wifi_established)
        {
            // measure temperatur and humidity
            startMeasurement();
            int temp = (int)getTemp();
            int humi = (int)getHumi();
            printf("Temperatur: %d °C, Humidität: %d%%\n", temp, humi);
            if ((temp >= 25 || humi >= 80) && window == zu)
            {
                printf("Fenster oeffnen wird gesendet\n");
                urlAufruf(urlOpen);
                window = auf;
            }
            if ((temp <= 25 && humi <= 80) && window == auf)
            {
                printf("Fenster schließen wird gesendet\n");
                urlAufruf(urlClose);
                window = zu;
            }
        }
        else
        {
            printf("Keine Verbindung zum Wlan\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // wait 1s
    }
}
