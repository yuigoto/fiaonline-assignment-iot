/**
 * credentials.h
 * ----------------------------------------------------------------------
 * 
 * Credenciais para comunicação via WiFi, MQTT e APIs.
 * 
 * ----------------------------------------------------------------------
 * 
 * @author      Fabio Y. Goto <fabio.goto@gmail.com>
 * @version     1.0.0
 * @license     MIT
 */

/* WIFI
 * ------------------------------------------------------------------- */

/**
 * SSID da rede WiFi.
 * 
 * IMPORTANTE: o ESP8266 não se conecta a redes WiFi 5GHz.
 */
const char *WIFI_SSID = "SSID_DA_REDE";

/**
 * Senha da rede WiFi.
 */
const char *WIFI_PASS = "SENHA_DA_REDE";

/* UBIDOTS
 * ------------------------------------------------------------------- */

/**
 * Chave de API para comunicação com a plataforma Ubidots.
 */
const char *UBIDOTS_TOKEN = "COLOQUE_O_SEU_TOKEN_AQUI";
