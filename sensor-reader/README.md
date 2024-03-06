# Leitor de Sensores BME-280 / MQ-2 com transmissão via Serial

_Fabio Y. Goto - Mar/2024_

> Parte do assignment para a matéria de "**IOT e Conectividade**", do curso de pós-graduação em "**Programação Fullstack e Desenvolvimento em Linguagens Eficientes**" da **FIA Online**.

-----

## Descrição

Projeto para detector de gás inflamável e fumaça, além de medição de umidade, pressão atmosférica e temperatura, com transmissão de dados por comunicação serial via cabo.

A ideia é a de utilizar uma placa de baixo custo e consumo de energia, com fator de forma compacto, para a leitura dos dados em ambientes controlados e não controlados.

Foi selecionada a placa **Arduino Nano**, para esta unidade, por ser uma placa de baixo custo e fácil reposição, e por ter os recursos mínimos necessários para que o projeto seja executado.

Uma vez que, de acordo com a proposta do exercício, o local de leitura não possui rede sem fio, acredito que faça sentido utilizar esta solução.

O projeto foi desenvolvido em conjunto com o `sensor-network-sender`, um receptor que processará os dados enviados, assim como os enviará, via API, para um banco de dados. Este segundo projeto utiliza outra placa (veja `README` do mesmo para mais detalhes).

Estas informações posteriormente são utilizadas para análise e geração de relatórios, além de alertas em caso de detecção de anomalias na medição.

A ideia é fornecer um conjunto de dados o mais completo possível, por isso a transmissão envia também informações como altitude aproximada e o limite para acionamento de alerta de gás/fumaça.

Por questões de debug, durante e após o desenvolvimento, não utilizamos a conexão nativa `Serial` da placa, mas sim a biblioteca `SoftwareSerial`, para envio utilizando I/O digital para um conversor TTL para RS232, com uma conexão DB9.

#### Bônus

Como um bônus, o código-fonte também inclui meios para a adição de um buzzer ou outro tipo de campainha em caso de alerta.

-----

## Requisitos

- Placa **Arduino Nano**;
  - Recomenda-se _ATMEGA328_ ou _ATMEGA328P_, não testado com _ATMEGA168_;
  - Estamos utilizando para o modelo, como nas fotos apresentadas, uma versão do Nano com entrada Micro USB, por ser um cabo mais fácil de ser encontrado do que um Mini USB hoje em dia;
- **BME-280**: Sensor de umidade, temperatura, altitude e pressão atmosférica;
- **MQ-2**: Sensor de gás inflamável e fumaça;
- **Conversor TTL / RS232 com controlador MAX3232** (conector DB9): conversor necessário para que possamos realizar a transmissão de dados via cabo Serial;

-----

## Dependências

- **`Adafruit BME280 Library`** (_Adafruit_);
- **`ArduinoJson`** (_Benoit Blanchon_);
  - Utilizamos esta biblioteca para que possamos fornecer uma estrutura de dados mais amigável para transmissão e, também, leitura humana, o JSON (JavaScript Object Notation);
- **`SoftwareSerial`** (_Arduino_);
  - Esta biblioteca nos permite utilizar um par adicional de pinos GPIO digitais para transmitir dados por comunicação serial;
- **`Wire`** (_Arduino_);

-----

## Transmissão

Como citado acima, o projeto envia um payload JSON via comunicação serial, usando protocolo RS232 a um _baud rate_ de 9600 Bd/s.

A frequência de 9600 foi escolhida por ser um padrão industrial, especialmente em equipamentos de automação industrial e medição, como balanças rodoviárias e de piso.

A mensagem utiliza a seguinte estrutura, exemplicada com um struct `C++`:

```cpp
struct SensorJsonSchema {
  float temperature;
  float pressure;
  float altitude;
  float humidity;
  float gas_level;
  float gas_level_threshold;
  bool  gas_detected;
};
```

#### Detalhes

|Propriedade|Tipo|Unidade|Descrição|
|:---------:|:--:|:-----:|:-------:|
|`temperature`|`float`|`°C`|_Temperatura do ambiente, em graus Celsius_|
|`pressure`|`float`|`hPa`|_Pressão atmosférica local, em Hectopascal_|
|`altitude`|`float`|`m`|_Altitude em metros acima do nível do mar_|
|`humidity`|`float`|`%`|_Umidade relativa do ar_|
|`gas_level`|`float`|`ppm`|_Concentração de gás e fumaça, medida em partes por milhão, média de 100 leituras_|
|`gas_level_threshold`|`float`|`ppm`|_Valor de referência para acionamento de alerta de gás e fumaça_|
|`gas_detected`|`boolean`|--|_Status de detecção de gás ou fumaça_|

-----

## Observações

- Na documentação da biblioteca `ArduinoJson`, é recomendado que  não se confie muito em `SoftwareSerial` por ser instável, mas para este projeto não houve problemas de instabilidade;
- Caso haja problemas, recomenda-se a utilização de `NeoSWSerial`, porém esta biblioteca **não pode ser utilizada** com placas que não tenham controladores **ATMEGA**, ex.: **ESP32 e ESP8266**;
- Para que o sensor **MQ-2** forneça uma leitura o mais precisa possível, é recomendado que o sensor seja aquecido por 24 horas antes de utilização, caso ele nunca tenha sido utilizado ou esteja em desuso por um longo período;
- Caso o sensor **MQ-2** tenha sido utilizado recentemente, é recomendado que o sensor seja aquecido por, no mínimo, 5 minutos antes de utilização;

-----

_&copy;2024 Fabio Y. Goto_
