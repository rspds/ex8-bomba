/*  CÓDIGO DO MESTRE

  CODIGO COMENTADO PARA A EXPLICAÇÃO DA BIBLIOTECA
*/


#include <SimpleModbusMaster_DUE.h> // incluindo biblioteca do mestre

//================DEFINIÇÕES GERAIS===================================
#define baud 9600 // velocidade
#define timeout 1000 // Tempo máximo que o mestre espera a resposta do escravo
#define polling 200 // taxa que o mestre requisita os escravos
#define retry_count 10 // Caso haja um erro, quantas tentativas de comunicação ele vai fazer
#define TxEnablePin 2 // Pino do arduino que sera o enable
#define TOTAL_NO_OF_REGISTER 2 // número de registradores que vai ser utilizado
//====================================================================

#define botao 5
#define LedMestre 11

/* SOBRE O enum : é uma função do proprio arduino que vai enumerar os pacotes, neste caso PACKET1 = 0 , TOTAL_NO_OF_PACKETS = 1

*/

enum { PACKET1,
       TOTAL_NO_OF_PACKETS
     };

//================================================================================

Packet packets[TOTAL_NO_OF_PACKETS]; //Cria um array com os pacotes que estão no enum


unsigned int regs[TOTAL_NO_OF_PACKETS]; // Cria um array chamado regs, todos os elementos desse array é do tipo unsigned int.
// Qual o tamanho desse array? é o tamanho de TOTAL_NO_OF_PACKETS
// Os elementos desse array são os registradores do mestre


void setup() 
{
    pinMode(LedMestre, OUTPUT);
    pinMode(botao, INPUT);


  //============= configurando os pacotes de comunicação ==============================

  modbus_construct (&packets[PACKET1], 1, PRESET_SINGLE_REGISTER, 0, 1, 0);
  //modbus_construct (&packets[PACKET2], 1, PRESET_SINGLE_REGISTER, 1, 1, 1);
  //modbus_construct (&packets[PACKET3], 1, PRESET_SINGLE_REGISTER, 2, 1, 2);
  //modbus_construct (&packets[PACKET4], 1, PRESET_SINGLE_REGISTER, 3, 1, 3);




  /*
    modbus_construct(numero do pacote,                                                     // Digo qual pacote vai ser trasmitido nessa comunicação
                   ID do escravo,                                                       // digo com qual escravo o mestre vai se comunicas
                   Função,                                                              //digo o que o vai ter ter na comunicação : leitura ou escrita
                   registrador do escravo,                                              // digo qual registrador do ESCRAVO deve ser lido/escrito
                   Nº de registradores que vão ser lidos/escritos                       // Cada registrador tem 1byte, então é a quantidade de bytes a serem enviados,
                   Endereço do registrador onde estão os dados a serem transmitidos         //digo qual registrador do MESTRE vai ser escrito/lido nessa comunicação
  */

  modbus_configure(&Serial, baud, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs); // Iniciando as configurações de comunicação


}

void loop()
{
  modbus_update(); //Vai processar todos os pacotes de comunicação

  bool botaoSt = digitalRead(botao);

  if (botaoSt == 1)
    digitalWrite(LedMestre, HIGH);
  else
    digitalWrite(LedMestre, LOW);




  //===SALVANDO VALORES NOS REGISTRADORES DO MESTRE====

  regs[0] = botaoSt;
  //regs[1] = Yvwieuhfisuehr ;


}
