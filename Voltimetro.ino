// TÍTULO: Programa Para Medir Tensão CC
// AUTOR: Ângelo Meneguini

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>                    // Inclui a biblioteca para o display de lcd
//~~
#define pin_entrada A0                         // Defini o pino analógico A0 como entrada da tensão
#define TAMANHO_VETOR_VCC 220
#define TAMANHO_MEDIA_TEMPORAL_VCC 80
float v_cc[TAMANHO_VETOR_VCC];                 // As 300 posições são necessárias para armazenar ao menos
                                               // um ciclo completo do sinal, fazendo possível medir o valor de pico

float v_pico_para_media[TAMANHO_MEDIA_TEMPORAL_VCC];
int ultima_posicao_armazenada = - 1;
bool vetor_cheio = false;


                                              
LiquidCrystal_I2C lcd(0x27, 16, 2);        // Informa a biblioteca quais pinos digitais são reservados para acionar o display

void setup() {
  
  pinMode(pin_entrada, INPUT);                     // Configura a porta A0 como entrada analógica
  
  lcd.begin();                             // Inicia o display informando o tipo de display, 16x2                              // Limpa os dados em cache do display
  lcd.setCursor(0,0);
  lcd.print(" Lab Fotonica ");
  lcd.setCursor(0,1);                          // Posiciona o cursor para coluna 2, linha 1
  lcd.print("  Nano Sagnac ");// Inprime no display o tipo de protótipo, Voltímetro CC
  delay(1000);
  lcd.clear();
} // fim do setup

void loop() {
  //Lendo o sinal de entrada
  for(int i = 0; i < TAMANHO_VETOR_VCC; i++)   // Armaneza no vetor v_cc a tensão de entrada, em 50 espaços de memória
  {
    v_cc[i] = analogRead(pin_entrada);         // Lê o valor analógico 
    v_cc[i] = (5*v_cc[i])/1024;                // Converte o valor lido pelo analogRead em tensãod e 0 a 5V.
  } // fim laço 


  //Obtendo o valor de pico 
  float v_pico = 0;
  for(int i = 0; i < TAMANHO_VETOR_VCC; i++)   
  {
    if (v_cc[i] > v_pico) {
      v_pico = v_cc[i];                        // Armazena o maior valor do vetor v_cc
    }
  } // fim laço for
  
  lcd.setCursor(0,1);                          // Posiciona o cursor na coluna 1, linha 2
  lcd.print("Vp  = ");                         // Inprime o texto Vcc =
  lcd.setCursor(6,1);                          // Posiciona o cursor na coluna 7, linha 2
  lcd.print(v_pico,4);                         // Imprime o valor da tensão calculada, que corresponderá a tensão medida

  //Armazenando o valor de pico em 'v_pico_para_media' sobre o valor mais antigo
  if (ultima_posicao_armazenada == TAMANHO_MEDIA_TEMPORAL_VCC - 1){
    v_pico_para_media[0] = v_pico;
    ultima_posicao_armazenada = 0;    
  } else {
    v_pico_para_media[ultima_posicao_armazenada + 1] = v_pico;
    ultima_posicao_armazenada = ultima_posicao_armazenada + 1;
  }

  // Nos primeiros momentos não teremos 100 medidas para fazer a média, valor armazenado em
  // TAMANHO_MEDIA_TEMPORAL_VCC. Dessa forma a média tem que ser feita com o número de medidas
  // realmente realizas. Caso não tenhamos todas as medidas a variavel vetor_cheio permanece falsa.
  // Uma vez alcancadas as 100 medidas a variável vetor_cheio se tornará verdadeira e não mudará mais.
  if (vetor_cheio == false && ultima_posicao_armazenada == TAMANHO_MEDIA_TEMPORAL_VCC - 1){
    vetor_cheio = true;
  }
  
  //Fazer média. Caso vetor_chei seja verdadeiro, temos TAMANHO_MEDIA_TEMPORAL_VCC medidas e a média é feita
  //usando esse valor. Caso contrario a média é feita usando o número real de medidas, armazenados na variável
  //ultima_posicao_armazenada
  float v_pico_medio = 0;
  if (vetor_cheio == true) {
    for (int i = 0; i < TAMANHO_MEDIA_TEMPORAL_VCC; i++) {
      v_pico_medio = v_pico_medio + v_pico_para_media[i];
    }
    v_pico_medio = v_pico_medio/TAMANHO_MEDIA_TEMPORAL_VCC;
  } else {
    for (int i = 0; i < ultima_posicao_armazenada; i++) {
      v_pico_medio = v_pico_medio + v_pico_para_media[i];
    }
    v_pico_medio = v_pico_medio/ultima_posicao_armazenada;
  }
  
  lcd.setCursor(0,0);                          
  lcd.print("Vmedio = ");                      
  lcd.setCursor(9,0);                          
  lcd.print(v_pico_medio,4);                   

  delay(200);                                 // Espera por 200 milisegundos

} // fim do loop
