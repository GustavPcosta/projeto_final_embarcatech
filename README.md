Sistema de Monitoramento de Temperatura, Umidade e Pressão para Máquinas industriais

Sobre o Projeto

Este projeto consiste no desenvolvimento de um sistema embarcado utilizando a placa BitDogLab RP2040 para monitoramento em tempo real de temperatura, umidade e pressão em máquinas industriais. O objetivo é prevenir falhas, melhorar a eficiência operacional e garantir a segurança dos equipamentos.

Principais Funcionalidades

Medição em tempo real de temperatura, umidade e pressão;

Exibição dos dados no display OLED para monitoramento rápido;

Indicação visual com LEDs PWM, alertando sobre condições críticas;

Possibilidade de expansão para envio de alertas via Wi-Fi/Bluetooth;

Armazenamento de dados para análise preditiva e otimização dos processos.

Componentes Utilizados

Placa: BitDogLab RP2040

Sensores:

Sensor de umidade do solo (ADC - pino 26)

Sensor de chuva (ADC - pino 27)

Sensor de temperatura da RP2040

Sensor BMP180 para medição de temperatura e pressão

Display OLED SSD1306 para exibição dos dados

LEDs RGB (PWM) para indicação visual

Comunicação via I2C para sensores e display


Fluxograma do Software

Inicializa os periféricos (ADC, I2C, PWM, display OLED);

Lê os valores dos sensores;

Processa os dados e calcula níveis de umidade e temperatura;

Exibe os dados no display OLED;

Ajusta os LEDs conforme os valores lidos;

(Opcional) Envia dados para armazenamento ou alerta remoto;

Repete o ciclo a cada segundo


Instalação e Configuração

Clone este repositório:

git clone https://github.com/seuusuario/projeto-monitoramento.git

Instale as dependências da RP2040 (caso necessário);

Compile e faça o upload do código para a placa.




Possíveis Melhorias

Integração com plataformas IoT (ex: Firebase, MQTT);

Implementação de histórico de dados com armazenamento em banco de dados;

Uso de comunicação sem fio (Wi-Fi/Bluetooth) para monitoramento remoto.
