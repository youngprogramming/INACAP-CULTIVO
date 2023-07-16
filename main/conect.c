#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include <stdalign.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_idf_lib_helpers.h"
#include "dht.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"      
#include "esp_adc_cal.h"
#include "driver/gpio.h"
#include <unistd.h>
#include"esp_log.h"
#include"esp_adc/adc_oneshot.h"
// UNIR index y mandarlos a la tarjeta como ensamblador

extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

#define GPIO_DHT 17 //pin de los datos 

int32_t humedad = 0, temperatura = 0;
float hum = 0.0, temp = 0.0;

void leerDHT(){
     if (dht_read_float_data(DHT_TYPE_AM2301, GPIO_DHT, &hum, &temp) == ESP_OK)
    {
        humedad = (int32_t)hum;
        temperatura = (int32_t)temp;
        printf("Lectura de sensor correcta\n");
    }
    else{
        printf("error lectura de sensor\n");
    }

};

void leerMQ7(){
    
}

static esp_err_t api_get_handler(httpd_req_t *req)
{ 
    leerDHT();
    httpd_resp_set_hdr(req, "Content-Type", "application/json");

    char res[60];
    sprintf(res, "{\"humedad\": %ld, \"temperatura\": %ld }", humedad, temperatura);
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

static esp_err_t home_get_handler(httpd_req_t * req)
{
    httpd_resp_set_type(req, "text/html");

    const uint32_t index_len = index_end - index_start;
    httpd_resp_send(req, index_start, index_len);
    return ESP_OK;
};

static const httpd_uri_t api = {
    .uri = "/api",
    .method = HTTP_GET,
    .handler = api_get_handler
};

static const httpd_uri_t home = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = home_get_handler
};

//funcion para el servidor http 
void web_server_init(){
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &api);
        httpd_register_uri_handler(server, &home);
        return;
    };
    printf("ERROR al inicializar el servidor\n");
};

void app_main(void){

    //incializando para que guarde la configuracion de contraseña en mempria persistente
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    example_connect();

    //variable para optener info de la ip 
    esp_netif_ip_info_t ip_info;
    //variable para almacenarlo que representa la interfaz de red 
    esp_netif_t* netif = NULL;
    
    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");


    if (netif == NULL) {
        printf("No hay interfaz\n");
    }
    else 
    {
        esp_netif_get_ip_info(netif, &ip_info);
        printf("IP: %d.%d.%d.%d\n", IP2STR(&ip_info.ip));



        web_server_init();

    }
       
      
}
