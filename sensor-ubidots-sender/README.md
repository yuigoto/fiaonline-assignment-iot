# Retransmissor de dados de sensores BME-280 / MQ-2 para o Ubidots

_Fabio Y. Goto - Mar/2024_

> Parte do assignment para a matéria de "**IOT e Conectividade**", do curso de 
> pós-graduação em "**Programação Fullstack e Desenvolvimento em Linguagens 
> Eficientes**" da **FIA Online**.

-----

## Antes de começar...

Abra o arquivo `credentials.h` e substitua os valores nele declarados, eles são:

|Nome|Tipo|Descrição|
|:--:|:--:|:-------:|
|`WIFI_SSID`|`char`|SSID da rede WiFi a ser utilizada. Não pode ser 5GHz.|
|`WIFI_PASS`|`char`|Senha da rede WiFi a ser utilizada.|
|`UBIDOTS_TOKEN`|`char`|Token para acesso à API da Ubidots, procure nas informações de sua conta no Ubidots.|

-----

## Descrição

Projeto para receptor de dados por comunicação serial e transmissão de dados para o serviço da Ubidots, utilizando conexão WiFi.

A ideia por trás deste projeto é a de utilizar uma placa de baixo custo e consumo de energia, com fator compacto, para a recepção de dados utilizando protocolo RS232 via cabo.

Foi selecionada a placa **Wemos D1 Mini**, para esta unidade, por ser uma placa de baixo custo e fácil reposição, e por ter os recursos mínimos necessários para que o projeto seja executado.

O projeto foi desenvolvido em conjunto com o `sensor-reader`, contendo um sensor de fumaça e gás inflamável (`MQ-2`) e um leitor de temperatura, umidade, pressão atmosférica e altitude (`BME-280`), que transmitem os dados de leitura via protocolo RS232, em um payload JSON.

Estas informações posteriormente são utilizadas para análise e geração de relatórios, além de alertas em caso de detecção de anomalias na medição.

A ideia é fornecer um conjunto de dados o mais completo possível, por isso a transmissão envia também informações como altitude aproximada e o limite para acionamento de alerta de gás/fumaça.

Por questões de debug, durante e após o desenvolvimento, não utilizamos a conexão nativa `Serial` da placa, mas sim a biblioteca `SoftwareSerial`, para envio utilizando I/O digital para um conversor TTL para RS232, com uma conexão DB9.

#### Bônus

Como um bônus, o código-fonte também inclui suporte à exibição dos dados recebidos em um mini display OLED I2C.

-----

## Requisitos

- Placa **Wemos D1 Mini**, com controlador **ESP8266**;
- **Conversor TTL / RS232 com controlador MAX3232** (conector DB9): conversor necessário para que possamos realizar a transmissão de dados via cabo Serial;
- **Display OLED I2C** de 4 pinos, com resolução de 128x64px;
  - _**IMPORTANTE**: caso utilize OLED, ele PRECISA ser a versão com 4 pinos_;

-----

## Dependências

- **`ArduinoJson`** (_Benoit Blanchon_);
  - Utilizamos esta biblioteca para que possamos fornecer uma estrutura de dados mais amigável para transmissão e, também, leitura humana, o JSON (JavaScript Object Notation);
- **`Adafruit_GFX`** (_Adafruit_);
  - Utilizamos para poder imprimir formas em nosso display OLED;
- **`Adafruit_SSD1306`** (_Adafruit_);
  - Necessário, caso seja desejável utilizar o display OLED;
- **`AsyncTimer`** (_Aasim-A_);
- **`ESP8266WiFi`** (_Ivan Grokhotkov_);
- **`SoftwareSerial`** (_Arduino_);
  - Esta biblioteca nos permite utilizar um par adicional de pinos GPIO digitais para transmitir dados por comunicação serial;
- **`Ubidots`** (_Ubidots_);
- **`Ticker`** (_Arduino_);
- **`Wire`** (_Arduino_);

> **IMPORTANTE**
> 
> Podem haver mais dependências a serem instaladas, caso a IDE do Arduino solicite, **instale todas as dependências necessárias**.
>
> Para instalar a biblioteca Ubidots, certifique-se de seguir os tutoriais e a documentação para a placa/controlador que utilizar.

-----

## Recepção

Como citado acima, o projeto recebe um payload JSON via comunicação serial, usando protocolo RS232 a um _baud rate_ de 9600 Bd/s.

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

-----

_&copy;2024 Fabio Y. Goto_
