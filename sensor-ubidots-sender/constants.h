/**
 * constants.h
 * ----------------------------------------------------------------------
 * 
 * Contém todas as constantes utilizadas no projeto. 
 * 
 * ----------------------------------------------------------------------
 * 
 * @author      Fabio Y. Goto <fabio.goto@gmail.com>
 * @version     1.0.0
 * @license     MIT
 */
#include "pins_arduino.h"

/* SERIAL
 * ------------------------------------------------------------------- */

/**
 * Velocidade utilizada para leitura de informações via protocolo 
 * serial RS232.
 */
const long SERIAL_SPEED = 9600L;

/**
 * Endereço do pino utilizado para a transmissão de dados, utilizando a 
 * biblioteca SoftwareSerial.
 * 
 * O motivo de utilizarmos este pino, ao invés de TX, está documentado 
 * no texto principal do assignment.
 */
const int SERIAL_TX_PIN = D7;

/**
 * Endereço do pino utilizado para receber os dados, utilizando a 
 * biblioteca SoftwareSerial.
 * 
 * O motivo de utilizarmos este pino, ao invés de RX, está documentado 
 * no texto principal do assignment.
 */
const int SERIAL_RX_PIN = D6;

/* COMUNICAÇÃO
 * ------------------------------------------------------------------- */

/**
 * Intervalo de tempo, em segundos, entre cada requisição no Ubidots.
 */
const int HTTP_REQUEST_INTERVAL_IN_SECONDS = 2;

/* OLED
 * ------------------------------------------------------------------- */

/**
 * Endereço utilizado para a entrada de dados no display OLED.
 */
const int OLED_PIN_INPUT = D2;

/**
 * Endereço utilizado para a saída de dados no display OLED.
 */
const int OLED_PIN_OUTPUT = D1;

/**
 * Resolução horizontal do display OLED.
 */
const int OLED_H_RESOLUTION = 128;

/**
 * Resolução vertical do display OLED.
 */
const int OLED_V_RESOLUTION = 64;
