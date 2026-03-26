/*Teste do teclado numérico com leitura serial*/

//0x27

//Importação das libs
#include <Keypad.h>
#include <stdio.h>
#include <string.h>

const char passwd[] = "1234"; //Senha
char writtenPasswd[5]; //Senha escrita
int counter = 0;

const byte rows[4] = { 2, 3, 4, 5 };     //Informa em quais pinos as linhas do teclado estão
const byte columns[4] = { 6, 7, 8, 9 };  //Informa em quais pinos as colunas do teclado estão

//Cria um array 2D para as teclas
char keys[4][4] = {

  { 'D', 'C', 'B', 'A' },
  { '#', '9', '6', '3' },
  { '0', '8', '5', '2' },
  { '*', '7', '4', '1' }

};

// Cria um objeto do tipo Keypad que vai traduzir
// pulsos elétricos em caracteres reais (como '1', 'A' ou '*').
Keypad mykeypad = Keypad(makeKeymap(keys), rows, columns, 4, 4);

void setup() {
  
  Serial.begin(9600);  //Habilita Monitor Serial

}

void loop() {

  char myKey = mykeypad.getKey();  // Pega a tecla digitada e guarda na variável
  
  //Checa se alguma tecla foi clicada
  if (myKey) {
    Serial.println(myKey);

    writtenPasswd[counter] = myKey; //Adiciona a tecla no array
    counter++;

    if(counter == 4) {
      
      writtenPasswd[4] = '\0';

      //strcmp é uma função em C que compara Strings
      if (strcmp(writtenPasswd, passwd) == 0) {

        Serial.println("ACESSO LIBERADO!");

      } else {
        
        Serial.println("ACESSO NEGADO!");

      }

      //Renicia o contador de index
      counter = 0;

      // memset Serve para preencher um determinado espaço de memória.
      // Nesse caso, pedimos para ele preencher cada um dos espaços de
      // writtenPasswd com 0.
      memset(writtenPasswd, 0, sizeof(writtenPasswd));

    }

  }

}