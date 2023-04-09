// TÍTULO: Programa Para Medir Tensão de pico

#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Inclui a biblioteca para o display de lcd

#define pin_entrada A0
#define TAMANHO_VETOR_VCC 220
#define TAMANHO_MEDIA_TEMPORAL_VCC 80
float v_cc[TAMANHO_VETOR_VCC]; // As 300 posições são necessárias para armazenar ao menos um ciclo completo do sinal, fazendo possível medir o valor de pico

float v_pico_para_media[TAMANHO_MEDIA_TEMPORAL_VCC];
int ultima_posicao_armazenada = -1;
bool vetor_cheio = false;
volatile float v_maximo_global = 0; // Armazena o maior valor de tensão de pico registrado.

const byte interruptPin = 2;
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup()
{
    attachInterrupt(digitalPinToInterrupt(interruptPin), reset_V_pico_maximo, RISING);

    // Usado para resetar o v_maximo_global
    Serial.begin(9600);
    pinMode(pin_entrada, INPUT); // Configura A0 como entrada analógica
    lcd.setCursor(0, 0);
    lcd.print(" Lab Fotonica ");
    lcd.setCursor(0, 1); // Posiciona o cursor para coluna 2, linha 1
    lcd.print(" Nano Sagnac ");
    delay(1000);
    lcd.clear();
} // fim do setup

void loop()
{
    for (int i = 0; i < TAMANHO_VETOR_VCC; i++) // Armazena no vetor v_cc a tensão de entrada, em 50 espaços de memória
    {
        v_cc[i] = analogRead(pin_entrada); // Lê o valor analógico
        v_cc[i] = (5 * v_cc[i]) / 1024;    // converte o valor lido pelo analogRead em tensãod e 0 a 5V.
    }

    // Obtendo o valor de pico
    float v_pico = 0;
    for (int i = 0; i < TAMANHO_VETOR_VCC; i++)
    {
        if (v_cc[i] > v_pico)
        {
            v_pico = v_cc[i]; // Armazena o maior valor do vetor v_cc
        }
    }

    lcd.setCursor(0, 1); // Posiciona o cursor na coluna 1, linha 2
    lcd.print("Vp = ");
    lcd.setCursor(6, 1);  // Posiciona o cursor na coluna 7, linha 2
    lcd.print(v_pico, 4); // Imprime o valor da tensão calculada, que corresponderá a
    tensão medida
        Serial.print("Vp = ");
    Serial.println(v_pico);

    // Atualizando o v_maximo_global
    if (v_pico > v_maximo_global)
    {
        v_maximo_global = v_pico;
    }

    lcd.setCursor(0, 0);
    lcd.print("Vmax = "); // Imprime o texto Vmax =
    lcd.setCursor(7, 0);
    lcd.print(v_maximo_global, 4);
    Serial.print("Vmax = ");
    Serial.println(v_maximo_global);
    delay(400);
} // fim do loop

// Função chamada pela interrupção para zerar o v_maximo_global
void reset_V_pico_maximo()
{
    v_maximo_global = 0;
}