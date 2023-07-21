# INACAP-CULTIVO
Automatización de cultivo hidropónico con ESP32

Índice Documentación

1. Introducción al proyecto
   * Propósito
   * Objetivo
  
2. Visión general del proyecto
   * Componentes electrónicos
   * Tecnologias 
   * Librerias de código
  
3. Guía de configuración del proyecto
     * Instalación ESP-IDF en WINDOWS
     * Instalación ESP-IDF en LINUX/MAC
     * Creación de proyectos en ESP-IDF
     * Ejecutar código en ESP32

4. Solucion Problemas
   * Problemas de compatibilidad
   * Problemas Frecuentes
  

## Intruduccióm a proyecto 

### Proposito 

El propósito general de este proyecto es combinar la tecnología de automatización de ESP32 (tecnología 4.0), utilizando IoT (Internet de las cosas), con la técnica
de cultivo hidropónico para aumentar la eficiencia, la calidad y el control en la producción de cultivos,
al mismo tiempo que se optimizan los recursos utilizados.

### Objetivo 

#### Facilitar la supervisión y control LOCAL:
* Diseñar e implementar un sistema de control automatizado: El objetivo principal es desarrollar un sistema de automatización que pueda monitorear y controlar 
diferentes variables, como la temperatura, la humedad, gas, la iluminación, entre otros, para optimizar el crecimiento de 
las plantas en el sistema hidropónico.

#### Facilitar la supervisión y control REMOTO:

* La implementación de tecnología IoT (Internet de las cosas) con el ESP32 permite monitorear
el estado del cultivo de manera remota, lo que facilita la toma de decisiones y la detección temprana de problemas potenciales.

* La automatización incorpora sensores para detectar situaciones anormales,
como desequilibrios en los niveles de gas o condiciones de temperatura o  humedad, y enviar notificaciones al cultivador
para tomar medidas preventivas o correctivas.

* Establecer un sistema de monitoreo en tiempo real: El objetivo es proporcionar información en tiempo real sobre las condiciones del cultivo,
 como la temperatura, la humedad y otros parámetros importantes, tanto en la interfaz local como en la remota.

## Visión general del proyecto 

### Componentes electronico

* ESP32-WROM32
* AM2303 (Temperatura y humedad)
* MQ7 (sensor de gas)

  
### Tecnologias

* C
* Html Js CSS
* Bootstrap
* ESP-IDF
* GIT

### Librerias 

#### Libreias comunes de C para manejar texto y otros 
* #include <stdio.h>
* #include <stdint.h>
* #include "string.h"
* #include <stdalign.h>
* #include <unistd.h>

#### Obtiene el conjunto completo de bibliotecas necesarias para crear productos integrados y de IoT conectados y seguros

* "freertos/FreeRTOS.h"
* "freertos/task.h"


#### Sensor AM2302
*  "esp_idf_lib_helpers.h"
* "dht.h"

#### sensor MQ-7
* "esp_adc/adc_oneshot.h"
* "esp_log.h"
* "driver/adc.h"

#### Drivers 
* "driver/ledc.h"
* "driver/gpio.h"

#### Servidor y wifi
* "protocol_examples_common.h"
* "esp_http_server.h"      
* "esp_wifi.h"
* "nvs_flash.h"

## Guia configuracion proyecto 

### instalacion ESP-IDF WINDOWS

ESP-IDF requiere que se instalen algunas herramientas de requisitos previos para que pueda crear firmware para chips compatibles. Las 
herramientas de requisitos previos incluyen Python, Git, compiladores cruzados, herramientas de compilación CMake y Ninja.

#### Link para instalar ESP-IDF 

Instalara ultima version 
> https://dl.espressif.com/dl/esp-idf/?idf=4.4

Para mas detalles 
> https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/windows-setup.html
#### Lanzamiento del entorno ESP-IDF

Al final del proceso de instalación, puede consultar la opción Ejecutar ESP-IDF PowerShell Environment o Ejecutar símbolo del sistema 
ESP-IDF (cmd.exe). El instalador inicia el entorno ESP-IDF en el mensaje seleccionado.

Uso del símbolo del sistema
Para los pasos restantes de Introducción, vamos a utilizar el símbolo del sistema de Windows.

El instalador de herramientas ESP-IDF también crea un acceso directo en el menú Inicio para iniciar el símbolo del sistema ESP-IDF. Este acceso directo inicia el símbolo del sistema (cmd.exe) y ejecuta el script export.bat para configurar las variables de entorno (PATH, IDF_PATH y otras). Dentro de este símbolo del sistema, todas las herramientas instaladas están disponibles.

Tenga en cuenta que este acceso directo es específico para el directorio ESP-IDF seleccionado en el instalador de herramientas ESP-IDF. Si tiene varios directorios ESP-IDF en la computadora (por ejemplo, para trabajar con diferentes versiones de ESP-IDF), tiene dos opciones para usarlos:

1. Cree una copia del acceso directo creado por el instalador de herramientas ESP-IDF y cambie el directorio de trabajo del nuevo acceso directo al directorio ESP-IDF que desea usar.

2. Alternativamente, ejecute cmd.exe, luego cambie al directorio ESP-IDF que desea usar y ejecute export.bat. Tenga en cuenta que, a diferencia de la opción anterior, esta forma requiere que Python y Git estén presentes en PATH. Si obtiene errores relacionados con Python o Git que no se encuentran, use la primera opción.


### instalación ESP-IDF LINUX/MAC

Para mas detalles
> https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html

Descargar archivos:
> brew install cmake ninja dfu-util
> xcode-select --install.
> brew install python3


Para descargar esp-idf:

> mkdir -p ~/esp
cd ~/esp
git clone --recursive https://github.com/espressif/esp-idf.git

Siempre ejecutar: 
> cd ~/esp/esp-idf
./install.sh all
> $HOME/esp/esp-idf/export.sh


### Creación de proyectos en ESP-IDF 


1. Vamos a donde almacenaremos el proyecto
2. ejecutamos
   > idf.py create-project name

Esto nos entregara todo para un proyecto basico 
en esta carpeta es donde debemos copiar todo los archivos que tenenmos en git 

### Ejecutar codigo en ESP32

Para configurar tarjeta:
aqui nos aparecera un menú y es muy importante agregar el nombre de red y contraseña 
para que posteriormente la tarjeta pueda conectarse a la wifi 

>idf.py menuconfig

Para pasar a binario el codigo
>idf.py build 

Para pasar a la tarjeta esp32 el binario:
>idf.py flash

Para ver que esta pasando: 
>idf.py monitor 

## Problemas 

### Problemas de compatibilidad
* Descargar drivers en caso de que no reconozca la placa disponibles en Espressif programming guide 
* Verificar que el cable este buen estado(idealmente ocupar uno nuevo)
  
### Problemas Frecuentes y su solución

#### No me tomaba el wifi: 
* Verificar que el router de internet se encuentre los suficientemente cerca para que pueda tomar wifi
* Verifica que tenga la ruta a las librerias necesarias 





