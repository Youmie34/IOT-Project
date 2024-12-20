#include "tempHumi.h"

// definition of global variables
bme680_t sensor;
float humi;
float temp;

void bme680_init()
{
    // initialize memory for the bme680 sensor to 0
    memset(&sensor, 0, sizeof(bme680_t));

    // initialize device descriptor, address for bme680 0x76
    ESP_ERROR_CHECK(bme680_init_desc(&sensor, ADDR, PORT, SDA_GPIO, SCL_GPIO));

    // init the sensor
    esp_err_t res = bme680_init_sensor(&sensor);
    if (res != ESP_OK)
    {
        printf("Failed to initialize BME680 sensor: %d\n", res);
        vTaskDelete(NULL);
    }
}

void bme680_config()
{
    // Changes the oversampling rates to 4x oversampling for temperature
    // and 2x oversampling for humidity. Pressure measurement is skipped.
    bme680_set_oversampling_rates(&sensor, BME680_OSR_4X, BME680_OSR_NONE, BME680_OSR_2X);

    // Change the IIR filter size for temperature and pressure to 7.
    bme680_set_filter_size(&sensor, BME680_IIR_SIZE_7);

    // Change the heater profile 0 to 200 degree Celcius for 100 ms.
    bme680_set_heater_profile(&sensor, 0, 200, 100);
    bme680_use_heater_profile(&sensor, 0);

    // Set ambient temperature to 10 degree Celsius
    bme680_set_ambient_temperature(&sensor, 10);
}

void startMeasurement()
{
    // as long as sensor configuration isn't changed, duration is constant
    uint32_t duration;
    bme680_get_measurement_duration(&sensor, &duration);
    // values measured by the bme680
    bme680_values_float_t values;

    // trigger the sensor to start one TPHG measurement cycle
    if (bme680_force_measurement(&sensor) == ESP_OK)
    {
        // passive waiting until measurement results are available
        vTaskDelay(duration);

        // get the results and set them
        if (bme680_get_results_float(&sensor, &values) == ESP_OK)
        {
            setTemp(values.temperature);
            setHumi(values.humidity);
        }
    }
}

void setTemp(float measuredTemp)
{
    temp = measuredTemp;
}

void setHumi(float measuredHumi)
{
    humi = measuredHumi;
}

float getTemp()
{
    return temp;
}

float getHumi()
{
    return humi;
}