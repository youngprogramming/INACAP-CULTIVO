#include <stdio.h>
#include <stdint.h>
#include "string.h"
#include <stdalign.h>
#include "driver/ledc.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_idf_lib_helpers.h"
#include "dht.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "protocol_examples_common.h"
#include "esp_http_server.h"
#include "driver/gpio.h"
#include <unistd.h>
#include "esp_log.h"
#include "esp_adc/adc_oneshot.h"

// UNIR index y mandarlos a la tarjeta como ensamblador

extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

#define GPIO_DHT 17 // pin de los datos

int32_t humedad = 0, temperatura = 0;
float hum = 0.0, temp = 0.0;

void leerDHT()
{
    if (dht_read_float_data(DHT_TYPE_AM2301, GPIO_DHT, &hum, &temp) == ESP_OK)
    {
        humedad = (int32_t)hum;
        temperatura = (int32_t)temp;
        printf("Lectura de sensor correcta\n");
    }
    else
    {
        printf("error lectura de sensor\n");
    }
};

#define EXAMPLE_ADC1_CHAN34 ADC_CHANNEL_6 // Canal ADC a utilizar
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_11 // Atenuacion de la señal de entrada

adc_oneshot_unit_handle_t adc1_handle; // Configuracion de la entrada analoga digital, variable tipo oneshot

adc_oneshot_unit_init_cfg_t init_config1 = {
    // Configuracion general del ADC
    .unit_id = ADC_UNIT_1,            // Uso del ADC1
    .ulp_mode = ADC_ULP_MODE_DISABLE, // modo bajo consumo de energia desactivado
};

adc_oneshot_chan_cfg_t config = {
    // Parametros especificos del ADC
    .atten = EXAMPLE_ADC_ATTEN,  // Atenuacion
    .bitwidth = ADC_BITWIDTH_10, // Ancho de bits
};

static int raw_value;
int suma = 0;
int contador = 0;
int co = 0;

void leerMQ()
{
    // Nuevo instancia ADC con las configuraciones establecidas anteriormente
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    // Uso del canal especifico con la configuracion
    adc_oneshot_config_channel(adc1_handle, EXAMPLE_ADC1_CHAN34, &config);

    // Lectura del valor analogico  con la funcion adc_one_shot_read
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, EXAMPLE_ADC1_CHAN34, &raw_value)); //(identificador de instancia,canal de instancia a leer, lugar donde se guardará la variable)

    // Impresion del valor
    printf("adc raw: %d\n", raw_value);

    // Retraso
    //  conversion
    int ppm = 1200 / 1023;
    co = ppm * raw_value;
    printf("co: %d\n", co);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    if (co >= 500)
    {
        printf("hay presencia de monoxido\n");
    }
    else
    {
        printf("Noo CO\n");
    }

    adc_oneshot_del_unit(adc1_handle); // Gestion de memoria y recursos (eliminar/liberar)
};

static esp_err_t api_get_handler(httpd_req_t *req)
{
    leerDHT();
    leerMQ();
    httpd_resp_set_hdr(req, "Content-Type", "application/json");

    char res[100];
    sprintf(res, "{\"humedad\": %ld, \"temperatura\": %ld , \"co\": %d }", humedad, temperatura, co);
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
};

static esp_err_t home_get_handler(httpd_req_t *req)
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

// funcion para el servidor http
void web_server_init()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &api);
        httpd_register_uri_handler(server, &home);
        return;
    };
    printf("ERROR al inicializar el servidor\n");
};

void app_main(void)
{

    // incializando para que guarde la configuracion de contraseña en mempria persistente
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    example_connect();

    // variable para optener info de la ip
    esp_netif_ip_info_t ip_info;
    // variable para almacenarlo que representa la interfaz de red
    esp_netif_t *netif = NULL;

    netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

    if (netif == NULL)
    {
        printf("No hay interfaz\n");
    }
    else
    {
        esp_netif_get_ip_info(netif, &ip_info);
        printf("IP: %d.%d.%d.%d\n", IP2STR(&ip_info.ip));

        web_server_init();
    }
}
