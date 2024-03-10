#include "esp_check.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "../../credentials/credentials.h"

extern bool wifi_established;
extern char *TAG;

void wifi_init();
void wifi_event_handler(void *, esp_event_base_t, int32_t, void *);