/**
 * Leitor de Sensores BME-280 / MQ-2 com transmissão via Serial
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
#include <Adafruit_BME280.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "constants.h";

/* INSTÂNCIAS 
 * ------------------------------------------------------------------- */

/**
 * Instância de SoftwareSerial para transmissão de dados.
 */
SoftwareSerial SERIAL_HANDLER(SERIAL_RX_PIN, SERIAL_TX_PIN);

/**
 * Handler para documento JSON a ser enviado.
 */
JsonDocument DOCUMENT;

/**
 * Instância para comunicação com o sensor BME280.
 */
Adafruit_BME280 BME_HANDLER;

// Descomente abaixo caso utilize SPI via hardware no BME280.
// Adafruit_BME280 BME_HANDLER(
//   BME280_CSB_PIN
// );
// Adafruit_BME280 BME_HANDLER(
//   BME280_CSB_PIN, 
//   BME280_SDA_PIN, 
//   BME280_SDO_PIN, 
//   BME280_SCL_PIN
// );

/* LIFECYCLE 
 * ------------------------------------------------------------------- */

void setup() {
  bootstrap();
}

void loop() {
  Serial.flush();
  SERIAL_HANDLER.flush();

  readBme280SensorData();
  readMq2SensorData();
  parseAndSendSensorData();
  
  delay(1000);
}

/* INICIALIZAÇÃO 
 * ------------------------------------------------------------------- */

/**
 * Executa todas as inicializações, com delay de 100, para garantir 
 * que tudo está operando corretamente.
 */
void bootstrap() {
  initializeSerial();
  delay(100);

  initializeSofwareSerial();
  delay(100);

  awaitForSerialCommunication();
  delay(100);

  initializeBmeSensor();
  delay(100);
}

/**
 * Aguarda pela inicialização da comunicação serial, e imprime 
 * mensagem de inicialização.
 */
void awaitForSerialCommunication() {
  while (!Serial) {
    delay(100);
  }

  Serial.println("BME-280 / MQ-2 Sensor Reader");
  Serial.println("----------------------------------------");
  Serial.println("Author:  Fabio Y. Goto");
  Serial.println("Version: 1.0.0");
  Serial.println("----------------------------------------");
}

/**
 * Inicializa o sensor BME280.
 */
void initializeBmeSensor() {
  unsigned status;
  status = BME_HANDLER.begin(BME280_CHIP_ADDRESS);

  if (!status) {
    Serial.println("Não encontramos um sensor válido.");
    Serial.println("Verifique a fiação ou endereço e tente");
    Serial.println("novamente.");
    Serial.println("----------------------------------------");
    Serial.print("ID do sensor: 0x");
    Serial.println(BME_HANDLER.sensorID(), 16);
    Serial.println("----------------------------------------");
    Serial.println("- 0xFF:    provavelmente indica endereço");
    Serial.println("           inválido, BMP180 ou BMP085.");
    Serial.println("- 0x56-58: indica um BMP280.");
    Serial.println("- 0x60:    indica um BME280.");
    Serial.println("- 0x61:    indica um BME680.");
    Serial.println("----------------------------------------");

    while (true) {
      delay(100);
    }
  }

  // Inicializando com configuração padrão do datasheet
  BME_HANDLER.setSampling(
    Adafruit_BME280::MODE_NORMAL,     // Modo operacional
    Adafruit_BME280::SAMPLING_X2,     // Oversampling de temperatura
    Adafruit_BME280::SAMPLING_X16,    // Oversampling de pressão
    Adafruit_BME280::SAMPLING_X1,     // Oversampling de umidade
    Adafruit_BME280::FILTER_X16,      // Filtro
    Adafruit_BME280::STANDBY_MS_500   // Tempo de espera
  );
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
void initializeSofwareSerial() {
  pinMode(SERIAL_RX_PIN, INPUT);
  pinMode(SERIAL_TX_PIN, OUTPUT);
  SERIAL_HANDLER.begin(SERIAL_SPEED);
  SERIAL_HANDLER.flush();
}

/* HELPERS 
 * ------------------------------------------------------------------- */

/**
 * Serializa o documento JSON em um buffer de 512 bytes e transmite.
 */
void parseAndSendSensorData() {
  char output[512];
  serializeJson(DOCUMENT, output);
  SERIAL_HANDLER.println(output);
  Serial.println(output);
}

/**
 * Realiza a leitura dos dados do sensor BME280, adicionando as chaves 
 * de temperatura, pressão, umidade e altitude ao documento JSON a ser 
 * retornado.
 */
void readBme280SensorData() {
  float temperature, pressure, humidity, altitude;

  temperature = BME_HANDLER.readTemperature();
  pressure = BME_HANDLER.readPressure();
  humidity = BME_HANDLER.readHumidity();
  altitude = BME_HANDLER.readAltitude(PRESSURE_LEVEL_SEA);

  DOCUMENT["temperature"] = temperature;
  DOCUMENT["pressure"] = pressure / 100.0F;
  DOCUMENT["humidity"] = humidity;
  DOCUMENT["altitude"] = altitude;
}

/**
 * Realiza a leitura do sensor MQ-2 utilizando por via analógica.
 * 
 * Realiza 100 leituras sequenciais, com um intervalo de 1ms entre 
 * cada leitura, para obter uma média.
 * 
 * Adiciona ao documento a ser retornado os valores de leitura, limite 
 * de referência e status de detecção.
 */
void readMq2SensorData() {
  float gasLevelData = 0.0;

  for (int t = 0; t < 100; t++) {
    gasLevelData += analogRead(MQ2_SENSOR_A0_PIN);
    delay(1);
  }

  float gasLevelDataAverage = gasLevelData / 100;

  DOCUMENT["gas_level"] = gasLevelDataAverage;
  DOCUMENT["gas_level_threshold"] = MQ2_SENSOR_THRESHOLD;
  DOCUMENT["gas_detected"] = gasLevelDataAverage > MQ2_SENSOR_THRESHOLD;
}
