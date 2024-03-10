#include "server.h"
#include "urlEsp.h"

/* URI handler structure for GET / uri */
httpd_uri_t uri_get_root = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_root_handler,
    .user_ctx = NULL};
/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_example = {
    .uri = "/example",
    .method = HTTP_GET,
    .handler = get_example_handler,
    .user_ctx = NULL};

/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_favicon = {
    .uri = "/favicon.ico",
    .method = HTTP_GET,
    .handler = get_favicon_handler,
    .user_ctx = NULL};

/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_open = {
    .uri = "/open",
    .method = HTTP_GET,
    .handler = get_open_handler,
    .user_ctx = NULL};

/* URI handler structure for another GET / uri */
httpd_uri_t uri_get_close = {
    .uri = "/close",
    .method = HTTP_GET,
    .handler = get_close_handler,
    .user_ctx = NULL};

httpd_handle_t start_webserver(void)
{
    /* Generate default configuration */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    /* Port 80*/
    config.server_port = 80;
    /* Empty handle to esp_http_server */
    httpd_handle_t server = NULL;
    /* Start the httpd server */
    if (httpd_start(&server, &config) == ESP_OK)
    {
        /* Register URI handlers */
        httpd_register_uri_handler(server, &uri_get_root);
        httpd_register_uri_handler(server, &uri_get_example);
        httpd_register_uri_handler(server, &uri_get_favicon);
        httpd_register_uri_handler(server, &uri_get_open);
        httpd_register_uri_handler(server, &uri_get_close);
    }

    return server;
}

esp_err_t get_root_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Hello World!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_example_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "Hello World von Example!";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_favicon_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_open_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "open";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    urlAufruf(urlClose);
    printf("Fenster wird geoeffnet.\n");
    return ESP_OK;
}

esp_err_t get_close_handler(httpd_req_t *req)
{
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    /* Send a simple response */
    const char resp[] = "close";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    urlAufruf(urlClose);
    printf("Fenster wird geschlossen.\n");
    return ESP_OK;
}