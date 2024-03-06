# IoT e Conectividade - Assignment

_Fabio Y. Goto - Mar/2024_

> Repositório de código para projetos vinculados ao assignment para a disciplina de "**IoT e Conectividade**" do curso de pós-graduação em **Programação Fullstack e Desenvolvimento em Linguagens Eficientes** da **FIA Online**.

## Descrição

Ambos os projetos fazem parte do assignment para a criação de duas placas de monitoramento para uma caldeira industrial, de modo que uma cuida da leitura de sensores de temperatura, pressão, umidade e presença de gás inflamável/fumaça, transmitindo estes dados para uma segunda placa, responsável por consumir e retransmitir estes dados para uma plataforma online, no caso a [**Ubidots**](https://stem.ubidots.com/).

-----

## Requisitos

- **Arduino IDE**: versão 2.3.0 ou maior, disponível [neste link](https://www.arduino.cc/en/software);
- **Driver CH340**: para conectar e corretamente detectar os controladores as placas **Arduino Nano** e **Wemos D1 Mini** ao ligá-las ao computador via USB, assim como qualquer eventual cabo Serial/USB para testes, disponível [neste link](https://www.arduined.eu/ch340-windows-10-driver-download/);

Requisitos para cada projeto se encontram nos seus respectivos arquivos `README`.

-----

## Projetos

Os projetos dentro deste repositório são:

- [**`sensor-reader`**](/sensor-reader): projeto para leitor de temperatura, umidade, pressão e presença de gás/fumaça, usando uma placa **Arduino Nano**, sensores **BME-280** e **MQ-2**, com transmissão de dados por conexão serial (RS232 via conectro DB9);
- [**`sensor-ubidots-sender`**](/sensor-ubidots-sender): projeto para receptor de informações por conexão serial (RS232) e envio de informações coletadas para a plataforma **Ubidots**, utilizando uma placa **Wemos D1 Mini**, com controlador **ESP8266** e uso opcional de display **OLED 128x64**;

Na raíz deste repositório você pode encontrar, também, os esquemas de protótipo visual de ambas as placas feitos na ferramenta [**Fritzing**](https://fritzing.org).

-----

_&copy;2024 Fabio Y. Goto_
