/*Teclado numérico com leitura serial*/

//0x27

//Importação das libs
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>

#define endereco 0x27
#define colunasLCD 16
#define linhasLCD 2

LiquidCrystal_I2C lcd(endereco, colunasLCD, linhasLCD);

const int TRANCA = 10;

const int LED_VERDE = 11;
const int LED_VERMELHO = 12;

const char passwd[] = "9114"; //Senha
char writtenPasswd[5]; //Senha escrita

int counter = 0;

//Estado se trancado, false = aberto, true = fechado
bool lockState = true;

const byte rows[4] = { 9, 8, 7, 6 };     //Informa em quais pinos as linhas do teclado estão
const byte columns[4] = { 5, 4, 3, 2 };  //Informa em quais pinos as colunas do teclado estão

//Cria um array 2D para as teclas
char keys[4][4] = {

  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }

};

// Cria um objeto do tipo Keypad que vai traduzir
// pulsos elétricos em caracteres reais (como '1', 'A' ou '*').
Keypad mykeypad = Keypad(makeKeymap(keys), rows, columns, 4, 4);


void abreTrancaPorta(char senhaEscrita[]) {

  //strcmp é uma função em C que compara Strings
  if (strcmp(senhaEscrita, passwd) == 0) {

    lcd.clear();

    Serial.println("Abrindo porta...");
    lcd.print("Abrindo porta...");
    delay(3000);

    lcd.clear();

    lockState = false;

    digitalWrite(TRANCA, LOW);

    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);

  } else {
      
    lcd.clear();

    Serial.println("Acesso negado!");
    lcd.print("Acesso negado!");
    delay(3000);

    lcd.clear();

  }

}

void fechaTrancaPorta(char senhaEscrita[]) {

  //strcmp é uma função em C que compara Strings
  if (strcmp(senhaEscrita, passwd) == 0) {

    lcd.clear();

    Serial.println("Fechando porta...");
    lcd.print("Fechando porta...");
    delay(3000);

    lcd.clear();

    lockState = true;

    digitalWrite(TRANCA, HIGH);

    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

  } else {
    
    lcd.clear();

    Serial.println("Acesso negado!");
    lcd.print("Acesso negado!");
    delay(3000);

    lcd.clear();

  }

}

void montaSenha(char caractere) {

  Serial.println(caractere);

  writtenPasswd[counter] = caractere; //Adiciona a tecla no array
  counter++;

  if(counter == 4) {

    writtenPasswd[4] = '\0';

    if (lockState) {

      abreTrancaPorta(writtenPasswd);

    } else {
      
      fechaTrancaPorta(writtenPasswd);

    }

  }

}

void setup() {
  
  pinMode(TRANCA, OUTPUT);
  digitalWrite(TRANCA, HIGH);

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);

  Serial.begin(9600);  //Habilita Monitor Serial

  lcd.init();
  lcd.backlight();
  lcd.clear();

  lcd.print("Ola, Mundo!");
  delay(5000);

  lcd.clear();

}

void loop() {

  if(lockState) {

    Serial.println("Aguardando senha para abrir...");

    lcd.setCursor(0, 0);
    lcd.print("Aguardando senha");
    lcd.setCursor(0, 1);
    lcd.print("para abrir...");

  } else {

    Serial.println("Aguardando senha para fechar...");

    lcd.setCursor(0, 0);
    lcd.print("Aguardando senha");
    lcd.setCursor(0, 1);
    lcd.print("para fechar...");

  }

  while(counter != 4) {

    char myKey = mykeypad.getKey();  // Pega a tecla digitada e guarda na variável
  
    //Checa se alguma tecla foi clicada
    if (myKey) {
      
      //Método que monta a senha
      montaSenha(myKey);

    }

  }

  //Renicia o contador de index
  counter = 0;

  // memset Serve para preencher um determinado espaço de memória.
  // Nesse caso, pedimos para ele preencher cada um dos espaços de
  // writtenPasswd com 0.
  memset(writtenPasswd, 0, sizeof(writtenPasswd));

}