/**
 * @file i2cdev.h
 * @defgroup i2cdev i2cdev
 * @{
 *
 * ESP-IDF I2C master thread-safe functions for communication with I2C slave
 *
 * Copyright (C) 2018 Ruslan V. Uss <https://github.com/UncleRus>
 *
 * MIT Licensed as described in the file LICENSE
 */
#ifndef __I2CDEV_H__
#define __I2CDEV_H__

#include <driver/i2c.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <esp_err.h>
#include <sdkconfig.h>
#include <esp_idf_lib_helpers.h>

#ifdef __cplusplus
extern "C" {
#endif

#if HELPER_TARGET_IS_ESP32
//changed value to decimal
//I2CDEV_MAX_STRETCH_TIME matches the I2C_LL_MAX_TIMEOUT in the i2c.h library 
//to ensure i2c communication
#define I2CDEV_MAX_STRETCH_TIME 1048575
#elif HELPER_TARGET_IS_ESP8266
#define I2CDEV_MAX_STRETCH_TIME 0xffffffff
#endif

//added
#define CONFIG_I2CDEV_TIMEOUT 1000 // I2C transaction timeout, milliseconds

/**
 * I2C device descriptor
 */
typedef struct
{
    i2c_port_t port;         //!< I2C port number
    i2c_config_t cfg;        //!< I2C driver configuration
    uint8_t addr;            //!< Unshifted address
    SemaphoreHandle_t mutex; //!< Device mutex
    uint32_t timeout_ticks;  /*!< HW I2C bus timeout (stretch time), in ticks. 80MHz APB clock
                                  ticks for ESP-IDF, CPU ticks for ESP8266.
                                  When this value is 0, I2CDEV_MAX_STRETCH_TIME will be used */
} i2c_dev_t;

/**
 * @brief Init I2Cdev lib
 *
 * The function must be called before any other
 * functions of this library
 * @return ESP_OK on success
 */
esp_err_t i2cdev_init();

/**
 * @brief Finish work with I2CDev lib
 *
 * Uninstall i2c drivers
 * @return ESP_OK on success
 */
esp_err_t i2cdev_done();

/**
 * @brief Create mutex for device descriptor
 * @param[out] dev Device descriptor
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_create_mutex(i2c_dev_t *dev);

/**
 * @brief Delete mutex for device descriptor
 * @param[out] dev Device descriptor
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_delete_mutex(i2c_dev_t *dev);

/**
 * @brief Take device mutex
 * @param[out] dev Device descriptor
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_take_mutex(i2c_dev_t *dev);

/**
 * @brief Give device mutex
 * @param[out] dev Device descriptor
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_give_mutex(i2c_dev_t *dev);

/**
 * @brief Read from slave device
 *
 * Issue a send operation of \p out_data register adress, followed by reading \p in_size bytes
 * from slave into \p in_data .
 * Function is thread-safe.
 * @param[in] dev Device descriptor
 * @param[in] out_data Pointer to data to send if non-null
 * @param[in] out_size Size of data to send
 * @param[out] in_data Pointer to input data buffer
 * @param[in] in_size Number of byte to read
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_read(const i2c_dev_t *dev, const void *out_data,
        size_t out_size, void *in_data, size_t in_size);

/**
 * @brief Write to slave device
 *
 * Write \p out_size bytes from \p out_data to slave into \p out_reg register address.
 * Function is thread-safe.
 * @param[in] dev Device descriptor
 * @param[in] out_reg Pointer to register address to send if non-null
 * @param[in] out_reg_size Size of register address
 * @param[in] out_data Pointer to data to send
 * @param[in] out_size Size of data to send
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_write(const i2c_dev_t *dev, const void *out_reg,
        size_t out_reg_size, const void *out_data, size_t out_size);

/**
 * @brief Read from register with an 8-bit address
 *
 * Shortcut to i2c_dev_read().
 * @param[in] dev Device descriptor
 * @param[in] reg Register address
 * @param[out] in_data Pointer to input data buffer
 * @param[in] in_size Number of byte to read
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_read_reg(const i2c_dev_t *dev, uint8_t reg,
        void *in_data, size_t in_size);

/**
 * @brief Write to register with an 8-bit address
 *
 * Shortcut to i2c_dev_write().
 * @param[in] dev Device descriptor
 * @param[in] reg Register address
 * @param[in] out_data Pointer to data to send
 * @param[in] out_size Size of data to send
 * @return ESP_OK on success
 */
esp_err_t i2c_dev_write_reg(const i2c_dev_t *dev, uint8_t reg,
        const void *out_data, size_t out_size);

#define I2C_DEV_TAKE_MUTEX(dev) do { \
        esp_err_t __ = i2c_dev_take_mutex(dev); \
        if (__ != ESP_OK) return __;\
    } while (0)

#define I2C_DEV_GIVE_MUTEX(dev) do { \
        esp_err_t __ = i2c_dev_give_mutex(dev); \
        if (__ != ESP_OK) return __;\
    } while (0)

#define I2C_DEV_CHECK(dev,X) do { \
        esp_err_t ___ = X; \
        if (___ != ESP_OK) { \
            I2C_DEV_GIVE_MUTEX(dev); \
            return ___; \
        } \
    } while (0)

#ifdef __cplusplus
}
#endif

/**@}*/

#endif /* __I2CDEV_H__ */
