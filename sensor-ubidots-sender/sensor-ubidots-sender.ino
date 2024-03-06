/**
 * Retransmissor de dados de sensores BME-280 / MQ-2 para a nuvem
 * ----------------------------------------------------------------------
 * 
 * Assignment para a matéria de "IOT e Conectividade", do curso de 
 * pós-graduação em "Programação Fullstack e Desenvolvimento em 
 * Linguagens Eficientes" da FIA Online.
 * 
 * A ideia de utilizar várias funções é para facilitar a leitura e o 
 * entendimento de cada parte, fazendo com que cada uma tenha um 
 * propósito específico.
 * 
 * Mais informações, ver `README.md`.
 * 
 * ----------------------------------------------------------------------
 * 
 * @author      Fabio Y. Goto <fabio.goto@gmail.com>
 * @version     1.0.0
 * @license     MIT
 */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <AsyncTimer.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <Ticker.h>
#include <Wire.h>
#include <Ubidots.h>
#include "pins_arduino.h"
#include "constants.h"
#include "credentials.h"

/* INSTÂNCIAS 
 * ------------------------------------------------------------------- */

/**
 * Handler para display OLED.
 * 
 * Por padrão, utilizamos a resolução 128x64.
 */
Adafruit_SSD1306 OLED(OLED_H_RESOLUTION, OLED_V_RESOLUTION);

/**
 * Instância do geranciador de timer assíncrono.
 */
AsyncTimer ASYNC_TIMER;

/**
 * Handler para documento JSON a ser recebido.
 */
JsonDocument DOCUMENT;

/**
 * Instância de SoftwareSerial para recepção de dados.
 */
SoftwareSerial SERIAL_HANDLER(SERIAL_RX_PIN, SERIAL_TX_PIN);

/**
 * Instância do cliente Ubidots, também gerencia a conexão WiFi.
 */
Ubidots UBIDOTS(UBIDOTS_TOKEN, UBI_HTTP);

/* GLOBAIS 
 * ------------------------------------------------------------------- */

/**
 * Handler para erro de serialização de JSON recebido.
 */
DeserializationError JSON_SERIALIZE_ERROR; 

/**
 * Se os dados do sensor foram inicializados.
 */
boolean INITIALIZED = false;

/**
 * Buffer para o caractere sendo lido no momento.
 */
char READ_CHAR;

/**
 * Armazena em texto o conteúdo do buffer de leitura serial para envio.
 */
String RECEIVED_DATA = "";

/**
 * Temperatura indicada pelo sensor.
 */
float SENSOR_TEMPERATURE = 0.0;

/**
 * Pressão detectada pelo sensor.
 */
float SENSOR_PRESSURE = 0.0;

/**
 * Altitude indicada pelo sensor.
 */
float SENSOR_ALTITUDE = 0.0;

/**
 * Umidade indicada pelo sensor.
 */
float SENSOR_HUMIDITY = 0.0;

/**
 * Nível de gás/fumaça indicado pelo sensor.
 */
float SENSOR_GAS_LEVEL = 0.0;

/**
 * Limiar de nível de gás/fumaça para detecção.
 */
float SENSOR_GAS_LEVEL_THRESHOLD = 0.0;

/**
 * Se o gás/fumaça foi detectado.
 */
boolean SENSOR_GAS_DETECTED = false;

/**
 * Buffer para armazenar os dados recebidos via serial.
 */
char SERIAL_BUFFER[512];

/**
 * Índice do buffer de leitura serial.
 */
int SERIAL_BUFFER_INDEX = 0;

/**
 * Se o buffer de envio via serial foi enviado, utilizado para trigger 
 * inicial de envio para a Ubidots.
 */
boolean SERIAL_BUFFER_SENT = false;

/* LIFECYCLE 
 * ------------------------------------------------------------------- */

void setup() {
  initializeOLEDDisplay();
  delay(100);

  initializeSerial();
  delay(100);

  initializeSoftwareSerial();
  delay(100);

  awaitForSerialCommunication();
  delay(100);

  initializeUbidots();
  delay(100);

  ASYNC_TIMER.setTimeout(
    submitDataToUbidots,
    HTTP_REQUEST_INTERVAL_IN_SECONDS * 1000
  );
}

void loop() {
  captureAndParseData();

  ASYNC_TIMER.handle();
}

/* INICIALIZAÇÃO 
 * ------------------------------------------------------------------- */

/**
 * Aguarda pela inicialização da comunicação serial, e imprime 
 * mensagem de inicialização.
 */
void awaitForSerialCommunication() {
  while (!Serial) {
    delay(100);
  }

  while (!SERIAL_HANDLER) {
    delay(100);
  }

  Serial.println("BME-280 / MQ-2 Data Sender");
  Serial.println("----------------------------------------");
  Serial.println("Author:  Fabio Y. Goto");
  Serial.println("Version: 1.0.0");
  Serial.println("----------------------------------------");
}

/**
 * Inicializa comunicação com display OLED (opcional).
 */
void initializeOLEDDisplay() {
  pinMode(OLED_PIN_INPUT, INPUT);
  pinMode(OLED_PIN_OUTPUT, OUTPUT);

  // Inicializa a comunicação com o display OLED no endereço I2C 0x3C
  OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);

  // Exibe mensagem de inicialização
  OLED.setTextSize(1);
  OLED.setCursor(0, 0);
  
  // Utiliza Extended ASCII
  OLED.cp437(true);

  OLED.println("BME-280/MQ-2 <-> WIFI");
  OLED.println("---------------------");
  OLED.println("Fabio Y. Goto || 2024");
  OLED.display();

  delay(2000);
  OLED.clearDisplay();
}

/**
 * Inicializa a comunicação serial nativa, para debug via USB.
 */
void initializeSerial() {
  Serial.begin(SERIAL_SPEED);
  Serial.flush();
}

/**
 * Inicializa o necessário para uso de `SoftwareSerial`.
 */
void initializeSoftwareSerial() {
  pinMode(SERIAL_RX_PIN, INPUT);
  pinMode(SERIAL_TX_PIN, OUTPUT);
  SERIAL_HANDLER.begin(SERIAL_SPEED);
  SERIAL_HANDLER.flush();
}

/**
 * Initializa a conexão com WiFi e serviço Ubidots.
 */
void initializeUbidots() {
  UBIDOTS.wifiConnect(WIFI_SSID, WIFI_PASS);
  Serial.println("----------------------------------------");
  Serial.println("Connected to WiFi");
  Serial.println("----------------------------------------");
}

/* HELPERS 
 * ------------------------------------------------------------------- */

/**
 * Captura e analisa os dados recebidos via serial, para então executar 
 * as ações necessárias.
 * 
 * Importante: por conta da captura contínua de dados, precisamos que 
 * todas as funções relacionadas à impressão e envio de dados sejam 
 * executadas dentro do laço.
 * 
 * Isso é necessário para garantir que os dados sejam devidamente 
 * impressos, sem perda de informações, uma vez que executar os métodos 
 * fora pode resultar em perda de dados.
 */
void captureAndParseData() {
  while (SERIAL_HANDLER.available() > 0) {
    READ_CHAR = SERIAL_HANDLER.read();
    SERIAL_BUFFER[SERIAL_BUFFER_INDEX++] = READ_CHAR;

    RECEIVED_DATA += READ_CHAR;

    if (READ_CHAR == '\n') {
      deserializeData();
      printDataToSerial();
      printDataToOLED();
      clearBuffer();
    }
  }
}

/**
 * Limpa o buffer de leitura serial.
 */
void clearBuffer() {
  RECEIVED_DATA = "";
  memset(SERIAL_BUFFER, 0, sizeof(SERIAL_BUFFER));
  SERIAL_BUFFER_INDEX = 0;
}

/**
 * Deserializa os dados recebidos em RECEIVED, armazenando em DOCUMENT.
 */
void deserializeData() {
  JSON_SERIALIZE_ERROR = deserializeJson(DOCUMENT, RECEIVED_DATA);

  if (JSON_SERIALIZE_ERROR) {
    Serial.print("Failed to deserialize JSON: ");
    Serial.println(JSON_SERIALIZE_ERROR.c_str());
  } else {
    INITIALIZED = true;
  }
}

/**
 * Imprime os dados do buffer serial no display OLED.
 */
void printDataToOLED() {
  SENSOR_TEMPERATURE          = DOCUMENT["temperature"];
  SENSOR_PRESSURE             = DOCUMENT["pressure"];
  SENSOR_ALTITUDE             = DOCUMENT["altitude"];
  SENSOR_HUMIDITY             = DOCUMENT["humidity"];
  SENSOR_GAS_LEVEL            = DOCUMENT["gas_level"];
  SENSOR_GAS_LEVEL_THRESHOLD  = DOCUMENT["gas_level_threshold"];
  SENSOR_GAS_DETECTED         = DOCUMENT["gas_detected"];
  
  OLED.clearDisplay();
  OLED.setCursor(0, 0);
  OLED.setTextSize(1);

  if (JSON_SERIALIZE_ERROR) {
    OLED.println("AWAITING DATA...");
  } else {
    OLED.println("PRESSURE: " + String(SENSOR_PRESSURE) + " hPa");
    OLED.print("TEMPERATURE: " + String(SENSOR_TEMPERATURE) + " ");
    OLED.print((char) 248); // ° 👈🫠
    OLED.println("C");
    OLED.println("HUMIDITY: " + String(SENSOR_HUMIDITY) + " %");
    OLED.println("ALTITUDE: " + String(SENSOR_ALTITUDE) + " m");
    OLED.println("GAS: " + String(SENSOR_GAS_LEVEL) + " ppm");
    OLED.println("THRESHOLD: " + String(SENSOR_GAS_LEVEL_THRESHOLD) + " ppm");
    OLED.print("LEAKAGE: ");
    if (SENSOR_GAS_DETECTED) {
      OLED.println("TRUE");
    } else {
      OLED.println("FALSE");
    }
  }
  OLED.display();
}

/**
 * Imprime os dados recebidos via serial.
 */
void printDataToSerial() {
  Serial.write(SERIAL_BUFFER, SERIAL_BUFFER_INDEX);
}

/**
 * Realiza o envio de dados recebidos no buffer para o serviço da Ubidots.
 */
void submitDataToUbidots() {
  if (JSON_SERIALIZE_ERROR) {
    Serial.println("Failed to serialize JSON, skipping...");
  } else {
    UBIDOTS.add("temperature", SENSOR_TEMPERATURE);
    UBIDOTS.add("pressure", SENSOR_PRESSURE);
    UBIDOTS.add("altitude", SENSOR_ALTITUDE);
    UBIDOTS.add("humidity", SENSOR_HUMIDITY);
    UBIDOTS.add("gas_level", SENSOR_GAS_LEVEL);
    UBIDOTS.add("gas_level_threshold", SENSOR_GAS_LEVEL_THRESHOLD);
    UBIDOTS.add("gas_detected", SENSOR_GAS_DETECTED);
    UBIDOTS.send();
  }

  ASYNC_TIMER.setTimeout(
    submitDataToUbidots,
    HTTP_REQUEST_INTERVAL_IN_SECONDS * 1000
  );
}
