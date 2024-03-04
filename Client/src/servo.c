#include "servo.h"

// configure servo
servo_config_t servo_cfg = {
    .max_angle = 180,
    .min_width_us = 690,  // equals "0°"
    .max_width_us = 2780, // equals "180°"
    .freq = 50,
    .timer_number = LEDC_TIMER_0,
    .channels = {
        .servo_pin = {SERVO_PIN},
        .ch = {LEDC_CHANNEL_0},
    },
    .channel_number = 1,
};

void ledOn()
{
    gpio_set_level(SERVO_LED, 1); // set GPIO low , LED goes on
}

void ledOff()
{
    gpio_set_level(SERVO_LED, 0); // set GPIO low , LED goes off
}

// open window
void openWindow()
{
    initServo();
    for (int angle = 0; angle <= 180; angle += 10)
    {
        // TODO: LED to show movement
        ledOn();
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
        printf("angle %d\n", angle);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    ledOff();
    deinitServo();
}

// close window
void closeWindow()
{
    initServo();
    for (int angle = 180; angle >= 0; angle -= 10)
    {
        // TODO: LED to show movement
        ledOn();
        iot_servo_write_angle(LEDC_LOW_SPEED_MODE, 0, angle);
        printf("angle %d\n", angle);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    ledOff();
    deinitServo();
}

void initServo()
{
    gpio_set_direction(SERVO_LED, GPIO_MODE_OUTPUT); // set GPIO as output

    // initialize servo
    esp_err_t err = iot_servo_init(LEDC_LOW_SPEED_MODE, &servo_cfg);
    if (err != ESP_OK)
    {
        ESP_LOGE("app_main", "Servo initialization failed");
        vTaskDelete(NULL);
    }
}

void deinitServo()
{
    // deinitialize servo
    iot_servo_deinit(LEDC_LOW_SPEED_MODE);

    // delete task
    vTaskDelete(NULL);
}