
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc2/dht.h"
#include "inc2/bp180.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C

#define SENSOR_UMIDADE 26  // Pino ADC para sensor de umidade do solo
#define SENSOR_CHUVA 27    // Pino ADC para sensor de chuva

#define LED_R 13  // Pino PWM para LED vermelho
#define LED_G 11  // Pino PWM para LED verde

ssd1306_t ssd; // Inicializa a estrutura do display

// Função para obter a temperatura da RP2040
float ler_temperatura() {
    adc_select_input(4);  // Seleciona o canal do sensor de temperatura
    uint16_t raw_value = adc_read();
    float voltage = raw_value * 3.3f / (1 << 12);
    float referencia = 0.5928f;
    float coeficiente = 0.0021f;
    return 27.0f - (voltage - referencia) / coeficiente;
}

// Função para obter a umidade do solo
float ler_umidade() {
    adc_select_input(0);  // Canal ADC0 (pino 26)
    uint16_t raw_value = adc_read();
    return (100 - ((raw_value / 4095.0) * 100)); // Converte para porcentagem
}



// Configuração do PWM
void configurar_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, 255); // Define a resolução (0-255)
    pwm_set_enabled(slice, true);
}

// Função para ajustar a cor do LED com base na umidade
void ajustar_led(float umidade) {
    if (umidade > 40) { // Umidade boa -> LED verde
        pwm_set_gpio_level(LED_R, 0);
        pwm_set_gpio_level(LED_G, 255);
    } else if (umidade >= 20 && umidade <= 40) { // Umidade intermediária -> LED amarelo
        pwm_set_gpio_level(LED_R, 200);
        pwm_set_gpio_level(LED_G, 200);
    } else { // Umidade baixa -> LED vermelho
        pwm_set_gpio_level(LED_R, 255);
        pwm_set_gpio_level(LED_G, 0);
    }
}

int main() {
    stdio_init_all(); // Inicializa as funções de I/O
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display OLED
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, ENDERECO, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    adc_init();
    sleep_ms(500);

    // Configuração do PWM para os LEDs
    configurar_pwm(LED_R);
    configurar_pwm(LED_G);

    // Inicializa o sensor BMP180
    i2c_init_bmp180();

    while (true) {
        // Leitura de temperatura e pressão do BMP180
        data_from_bmp180 current_data = get_bmp180_sensor_data(3);

        // Leitura de outros sensores
        float temperatura_rp = ler_temperatura();
        float umidade = ler_umidade();
        // const char* chuva = ler_chuva();

        // Ajuste da cor do LED com base na umidade
        ajustar_led(umidade);

        // Formata os dados para exibição
        char buffer_temp[20], buffer_umid[20], buffer_chuva[20], buffer_pressao[20];
        sprintf(buffer_temp, "Temp: %.1f C", temperatura_rp);
        sprintf(buffer_umid, "Umidade: %.1f%%", umidade);
        
        sprintf(buffer_pressao, "Pressao: %.1f Pa", current_data.atmospheric_pressure_in_Pa);

        // Exibe os dados no display OLED
        ssd1306_fill(&ssd, false);
        ssd1306_draw_string(&ssd, buffer_temp, 10, 10);
        ssd1306_draw_string(&ssd, buffer_umid, 10, 20);
        // ssd1306_draw_string(&ssd, buffer_chuva, 10, 30);
        ssd1306_draw_string(&ssd, buffer_pressao, 10, 40);
        ssd1306_send_data(&ssd);

        // Imprime os dados no console para debug
        printf("Temp: %.2f C | Umidade: %.2f%% | %s | Pressao: %.1f Pa\n",
               temperatura_rp, umidade, current_data.atmospheric_pressure_in_Pa);

        sleep_ms(1000); // Aguarda 1 segundo antes de atualizar os dados
    }
}
