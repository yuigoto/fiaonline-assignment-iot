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
#include "pins_arduino.h";

/* SERIAL
 * ------------------------------------------------------------------- */

/**
 * Velocidade utilizada para transmissão de informações via protocolo 
 * serial RS232.
 */
const long SERIAL_SPEED = 9600L;

/**
 * Endereço do pino utilizado para a transmissão de dados, utilizando a 
 * biblioteca `SoftwareSerial`.
 * 
 * O motivo de utilizarmos este pino, ao invés de TX, está documentado 
 * no texto principal do assignment.
 */
const int SERIAL_TX_PIN = 8;

/**
 * Endereço do pino utilizado para receber os dados, utilizando a 
 * biblioteca `SoftwareSerial`.
 * 
 * O motivo de utilizarmos este pino, ao invés de RX, está documentado 
 * no texto principal do assignment.
 */
const int SERIAL_RX_PIN = 9;

/* VALORES DE REFERÊNCIA
 * ------------------------------------------------------------------- */

/**
 * Valor padrão, de referência, para pressão atmosférica ao nível do 
 * mar.
 * 
 * Precisa ser ajustado de acordo com a previsão do tempo local, uma 
 * vez que umidade e outros fatores podem alterá-la.
 * 
 * Medido em hPa (hectopascal).
 */
const float PRESSURE_LEVEL_SEA = 1013.25;

/**
 * Valor de referência utilizado para o município de São Paulo, na 
 * data indicada.
 * 
 * Medido em hPa (hectopascal).
 */
const float PRESSURE_LEVEL_SP20240214 = 1021;

/* MQ-2
 * ------------------------------------------------------------------- */

/**
 * Pino para leitura analógica de dados do sensor MQ-2.
 */
const byte MQ2_SENSOR_A0_PIN = A0;

/**
 * Pino para leitura digital de dados do sensor MQ-2.
 */
const byte MQ2_SENSOR_D0_PIN = 7;

/**
 * Limiar de detecção de gás para o sensor MQ-2.
 */
const float MQ2_SENSOR_THRESHOLD = 550.0;

/* BME-280
 * ------------------------------------------------------------------- */

/**
 * Endereço padrão para conexão com o sensor BME-280.
 * 
 * Normalmente 0x77 (119), mas pode ser 0x76 (118) em alguns casos.
 */
const uint8_t BME280_CHIP_ADDRESS = (0x76);

/**
 * Identificador do chip.
 * 
 * Valor padrão: 0x58 (88).
 */
const uint8_t BME280_SENSOR_CHIP_ID = (0x58);

/**
 * Pino SCL/SCK (Serial Clock), utilizado com protocolo SPI.
 */
const byte BME280_SCL_PIN = 13;

/**
 * Pino SDO/MOSI (Main Out/Secondary In), utilizado com protocolo SPI.
 */
const byte BME280_SD0_PIN = 12;

/**
 * Pino SDA/MISO (Main In/Secondary Out), utilizado com protocolo SPI.
 */
const byte BME280_SDA_PIN = 11;

/**
 * Pino utilizado para SSB/CSB (Secondary Select/Chip Select), usado 
 * apenas com protocolo SPI.
 */
const byte BME280_CSB_PIN = 10;

/* EXTRA
 * ------------------------------------------------------------------- */

/**
 * Frequência para uso do buzzer, em Hz.
 * 
 * Opcional, apenas se utilizar buzzer.
 */
const int BUZZER_FREQUENCY = 440;

/**
 * Duração do sinal do buzzer, em milissegundos.
 */
const int BUZZER_DURATION = 1000;

/**
 * Pino utilizado para envio de sinal ao buzzer.
 */
const byte BUZZER_GPIO_PIN = 10;
