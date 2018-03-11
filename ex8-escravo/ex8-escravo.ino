/*  CÓDIGO DO ESCRAVO

  CODIGO PARA A EXPLICAÇÃO DA BIBLIOTECA
*/

#define led 5 //potenciometro
#include <SimpleModbusSlave_DUE.h> //incluindo a biblioteca do slave


enum {
  BotaoSt,
  //VAZAO,
  //CORRENTE,
  //NIVEL,
  HOLDING_REGS_SIZE
};


unsigned int holdingRegs[HOLDING_REGS_SIZE]; //criando um array de nome holdingRegs
// esse array tem tamanho HOLDING_REGS_SIZE

void setup()
{
  pinMode(led, OUTPUT);

  modbus_configure(&Serial, 9600, 1, 2, HOLDING_REGS_SIZE, holdingRegs);

  /*
    modbus_configure(  &Serial,
                     velocidade da comunicação,
                     ID do escravo
                     pino enable
                     HOLDING_REGS_SIZE,
                     holdingRegs);
  */

  modbus_update_comms(9600, 1);

  /*
     modbus_update_comms(
                         velocidade da comunicação,
                         ID do escravo  );
  */

}

void loop()
{

  bool T;

  modbus_update(); // processando o pacote de comunicação
  T =  holdingRegs[BotaoSt];   //atribuindo o valor do registrador 0 do escravo a variavel T ; T = holdingRegs[0]
  //V =  holdingRegs[VAZAO];         //atribuindo o valor do registrador 1 do escravo a variavel V ; V = holdingRegs[1]
  //C =  holdingRegs[CORRENTE];      //atribuindo o valor do registrador 2 do escravo a variavel C ; C = holdingRegs[2]
  //N = holdingRegs[NIVEL];          //atribuindo o valor do registrador 3 do escravo a variavel N ; N = holdingRegs[3]

  digitalWrite(led, !T);

}


