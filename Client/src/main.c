#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_check.h"
#include "esp_log.h"
#include "server.h"
#include "servo.h"
#include "esp_event.h"
#include <string.h>

void app_main()
{
    wifi_init();
    start_webserver();
}