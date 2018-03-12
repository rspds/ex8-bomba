
#include <SimpleModbusMaster_DUE.h> // incluindo biblioteca do mestre

//================DEFINIÇÕES GERAIS===================================
#define baud 9600 // velocidade
#define timeout 1000 // Tempo máximo que o mestre espera a resposta do escravo
#define polling 200 // taxa que o mestre requisita os escravos
#define retry_count 10 // Caso haja um erro, quantas tentativas de comunicação ele vai fazer
#define TxEnablePin 7 // Pino do arduino que sera o enable
#define TOTAL_NO_OF_REGISTER 2 // número de registradores que vai ser utilizado
//====================================================================

#define NUM_BOMBA 2
#define led0 10
#define led1 11
#define ledE 12
#define TROCA 20000
#define P_TEMP1 A2
#define P_TEMP2 A3
#define P_CORRENTE A4
#define P_VAZAO A5
#define P_NV_S A0
#define P_NV_I A1
#define TOLERANCIA 2


void processo();
void leitura();
void controle();
bool trocaBomba();
bool ligarBomba(byte bomba);
bool condicao(int bomba);


float temp[2];
float niv[2];
float corrente;
float vazao;
bool bombaSt[NUM_BOMBA] = {1, 1}; //status
unsigned long tempo = 0;
byte bombaAtual = 3,
		 bombaErro[2] = {0, 0};
bool necessidade = false;
bool ultimaBomba = 0;
bool flag = 0;
bool trava = 0;


/* SOBRE O enum : é uma função do proprio arduino que vai enumerar os pacotes, neste caso PACKET1 = 0 , TOTAL_NO_OF_PACKETS = 1

*/

enum { TEMP1,
			 TEMP2,
			 NIV_INF,
			 NIV_SUP,
			 CORRENTE,
			 VAZAO,
			 QTD_ERRO_B1,
			 QTD_ERRO_B2,
			 BOMBA,
       TOTAL_NO_OF_PACKETS
     };

//================================================================================

Packet packets[TOTAL_NO_OF_PACKETS]; //Cria um array com os pacotes que estão no enum


unsigned int regs[TOTAL_NO_OF_PACKETS]; // Cria um array chamado regs, todos os elementos desse array é do tipo unsigned int.
// Qual o tamanho desse array? é o tamanho de TOTAL_NO_OF_PACKETS
// Os elementos desse array são os registradores do mestre


void setup() 
{
    pinMode(led0, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(ledE, OUTPUT);


  //============= configurando os pacotes de comunicação ==============================

  modbus_construct (&packets[TEMP1], 1, PRESET_SINGLE_REGISTER, 0, 1, 0);
  modbus_construct (&packets[TEMP2], 1, PRESET_SINGLE_REGISTER, 1, 1, 1);
  modbus_construct (&packets[NIV_INF], 1, PRESET_SINGLE_REGISTER, 2, 1, 2);
  modbus_construct (&packets[NIV_SUP], 1, PRESET_SINGLE_REGISTER, 3, 1, 3);
  modbus_construct (&packets[CORRENTE], 1, PRESET_SINGLE_REGISTER, 4, 1, 4);
  modbus_construct (&packets[VAZAO], 1, PRESET_SINGLE_REGISTER, 5, 1, 5);
  modbus_construct (&packets[QTD_ERRO_B1], 1, PRESET_SINGLE_REGISTER, 6, 1, 6);
  modbus_construct (&packets[QTD_ERRO_B2], 1, PRESET_SINGLE_REGISTER, 7, 1, 7);
  modbus_construct (&packets[BOMBA], 1, PRESET_SINGLE_REGISTER, 8, 1, 8);




  /*
    modbus_construct(numero do pacote,                                                // Digo qual pacote vai ser trasmitido nessa comunicação
                   ID do escravo,                                                     // digo com qual escravo o mestre vai se comunicas
                   Função,                                                            //digo o que o vai ter ter na comunicação : leitura ou escrita
                   registrador do escravo,                                            // digo qual registrador do ESCRAVO deve ser lido/escrito
                   Nº de registradores que vão ser lidos/escritos                     // Cada registrador tem 1byte, então é a quantidade de bytes a serem enviados,
                   Endereço do registrador onde estão os dados a serem transmitidos   //digo qual registrador do MESTRE vai ser escrito/lido nessa comunicação
  */

  modbus_configure(&Serial, baud, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs); // Iniciando as configurações de comunicação


}

void loop()
{
  modbus_update(); //Vai processar todos os pacotes de comunicação

	processo();


  //===SALVANDO VALORES NOS REGISTRADORES DO MESTRE====

  regs[TEMP1] = temp[0];
  regs[TEMP2] = temp[1];
  regs[NIV_INF] = niv[0];
  regs[NIV_SUP] = niv[1];
  regs[CORRENTE] = corrente;
  regs[VAZAO] = vazao;
  regs[QTD_ERRO_B1] = bombaErro[0];
  regs[QTD_ERRO_B2] = bombaErro[1];
  regs[BOMBA] = bombaAtual;
       

	delay(1000);
}

void processo()
{

	leitura();
	controle();

	if (flag)
	{
		trocaBomba();
		flag = 0x00;
	}
}

void leitura()
{
	niv[0] = analogRead(P_NV_I);
	niv[1] = analogRead(P_NV_S);
	temp[0] = analogRead(P_TEMP1);
	temp[1] = analogRead(P_TEMP2);
	corrente = analogRead(P_CORRENTE);
	vazao = analogRead(P_VAZAO);

	niv[0] = map(niv[0], 0, 1023, 0, 100);
	niv[1] = map(niv[1], 0, 1023, 0, 100);
	temp[0] = map(temp[0], 0, 1023, 20, 100);
	temp[1] = map(temp[1], 0, 1023, 20, 100);
	corrente = map(corrente, 0, 1023, 0, 30);
	vazao = map(vazao, 0, 1023, 0, 8);
}

void controle()
{
	int intervalo = millis() - tempo;
	byte compara = bombaAtual;
	//controla a ativacao das bombas (bombaAtual) 
	if(niv[0] < 20)
		bombaAtual = 2;
	else
	{
		if(trava)
			bombaAtual = 3;
		else
		{
			if(niv[1] >= 80)
			{
				bombaAtual = 3;
				necessidade = false;
			}
			else if(niv[1] < 20 && necessidade == false)
			{
				bombaAtual = 3;
				necessidade = true;
				bombaAtual = !ultimaBomba;
				ultimaBomba = bombaAtual;
			}
			else if(necessidade == true && bombaAtual == 3)
			{
				bombaAtual = !ultimaBomba;
				ultimaBomba = bombaAtual;
			}
			else
				if(bombaAtual == 2)
					bombaAtual = 3;
				else if(bombaAtual != 3)
				{
					bombaSt[bombaAtual] = condicao(bombaAtual);

					if(bombaSt[bombaAtual] == false)
						bombaErro[bombaAtual]++;
				}
		}
	}

	if(bombaAtual != compara || intervalo > TROCA / 2)
	{
		flag = 0x01;
		tempo = millis();
	}
	else if (bombaAtual != 2 && bombaAtual !=3)
		if(!bombaSt[bombaAtual])
		{
			flag = 0x01;
			tempo = millis();
		}
}


bool trocaBomba()
{
	byte bombaDesl;

	if (bombaAtual == 0)
		bombaDesl = 1;
	else if (bombaAtual == 1)
		bombaDesl = 0;
	//if(necessidade == true)
	else
	{
		ligarBomba(bombaAtual);
		return true;	
	}

	if (bombaErro[bombaDesl] < TOLERANCIA)
	{
		ligarBomba(bombaDesl);
		bombaAtual = bombaDesl;
		return true;
	}
	else if (bombaErro[bombaAtual] < TOLERANCIA)
	{
		ligarBomba(bombaAtual);
		return true;
	}
	else
	{
		ligarBomba(3);
		trava = true;
	}


	return false;
}

bool ligarBomba(byte bomba)
{
	if (bomba == 0)
	{
		digitalWrite(led0, HIGH);
		digitalWrite(led1, LOW);
		digitalWrite(ledE, LOW);
		return true;
	}
	else if (bomba == 1)
	{
		digitalWrite(led0, LOW);
		digitalWrite(led1, HIGH);
		digitalWrite(ledE, LOW);
		return true;
	}
	else if (bomba == 2)
	{
		digitalWrite(led0, LOW);
		digitalWrite(led1, LOW);
		digitalWrite(ledE, HIGH);
		return true;
	}
	else if (bomba == 3)
	{
		digitalWrite(led0, LOW);
		digitalWrite(led1, LOW);
		digitalWrite(ledE, LOW);
		return true;
	}
	return false;
}

bool condicao(int bomba)
{
	bool cond;

	if (temp[bomba] > 75)
		cond = false;
	else if (corrente > 20)
		cond = false;
	else if (vazao == 0)
		cond = false;
	else
		cond = true;

	return cond;
}



