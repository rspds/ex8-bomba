
#include <SimpleModbusSlave_DUE.h>
#include <LiquidCrystal.h>

#define     menu     12
#define     enter    11
#define 		voltar   10
#define 		QUANT_C1 2
#define 		QUANT_C2 6
#define 		QUANT_C3 6

void loopC2();
void loopC3();
void camada1();  
bool camada2();  
bool camada3();
void readButtsC1();
bool readButtsC2();
bool readButtsC3();
void list_menu(int line[], int quantidade);
void atualiza();

LiquidCrystal lcd(7,  //RS no digital 7
		6,  //EN no digital 6
		5,  //D4 no digital 5
		4,  //D5 no digital 4
		3,  //D6 no digital 3
		2); //D7 no digital 2

int lineC1[QUANT_C1] = {0,1},
		lineC2[QUANT_C2] = {0,1,2,3},
		menu_number = 1,
		bomba_escolhida;

float temp[2] = {60, 45},
			niv[2] = {20, 50},
			corrente = 8,
			vazao = 6;

boolean menu_flag  = 0,
				enter_flag = 0,
				voltar_flag = 0,
				voltarSt=0,
				//sub_menuC1 = 0,
				sub_menuC2 = 0;

byte bombaAtual,
		 bombaErro[2];

enum{ TEMP1,
		  TEMP2,
		  NIV_INF,
		  NIV_SUP,
		  CORRENTE,
		  VAZAO,
		  QTD_ERRO_B1,
		  QTD_ERRO_B2,
			BOMBA,
      HOLDING_REGS_SIZE
};

unsigned int holdingRegs[HOLDING_REGS_SIZE];

void setup()
{
  modbus_configure(&Serial, 9600, 1, 6, HOLDING_REGS_SIZE, holdingRegs);
  /*
    modbus_configure(&Serial,
                     velocidade da comunicação,
                     ID do escravo
                     pino enable
                     HOLDING_REGS_SIZE,
                     holdingRegs);
  */

  modbus_update_comms(9600, 1);
  /*
     modbus_update_comms(velocidade da comunicação,
                         ID do escravo  );
  */

	pinMode(menu,  INPUT_PULLUP);
	pinMode(enter, INPUT_PULLUP);
	pinMode(voltar, INPUT_PULLUP);

	lcd.begin (16,3);
	lcd.setCursor(0,0);
	lcd.print(">");

}

void loop()
{
  atualiza();
	camada1();  
} 

void loopC2()
{
	do{
  atualiza();
	}while(camada2()==true);
}

void loopC3()
{
	do{
  atualiza();
	}while(camada3()==true);
}

void camada1()
{

	lcd.setCursor(0,1);
	lcd.print(" ");
	lcd.setCursor(1,lineC1[0]);
	if(bombaAtual == 0)
		lcd.print("1) Bomba1 On    ");
	else
		lcd.print("1) Bomba1 Off   ");
	lcd.setCursor(1,lineC1[1]);
	if(bombaAtual == 1)
		lcd.print("2) Bomba2 On    ");
	else
		lcd.print("2) Bomba2 Off   ");

  delay(10);
	readButtsC1();
}

bool camada2()
{
	lcd.setCursor(0,1);
	lcd.print(" ");
	lcd.setCursor(1,lineC2[0]);
	lcd.print("1) Nivel Inferior");
	lcd.setCursor(1,lineC2[1]);
	lcd.print("2) Nivel Superior");
	lcd.setCursor(1,lineC2[2]);
	lcd.print("3) Vazao        ");
	lcd.setCursor(1,lineC2[3]);
	lcd.print("4) Temperatura  ");
	lcd.setCursor(1,lineC2[4]);
	lcd.print("5) Corrente     ");
	lcd.setCursor(1,lineC2[5]);
	lcd.print("6) Qtd de Erros ");
  
	delay(10);
	return readButtsC2();
}

bool camada3()
{
	lcd.setCursor(0,1);
	lcd.print("                "); 
	switch(menu_number)
	{
		case 1: 
			lcd.setCursor(1,0);
			lcd.print("Nivel Inferior ");
			lcd.setCursor(0,1);
			lcd.print(niv[0]); 
			break;
		case 2: 
			lcd.setCursor(1,0);
			lcd.print("Nivel Superior ");
			lcd.setCursor(0,1);
			lcd.print(niv[1]); 
			break;
		case 3: 
			lcd.setCursor(1,0);
			lcd.print("Vazao          ");
			lcd.setCursor(0,1);
			lcd.print(vazao); 
			break;
		case 4: 
			lcd.setCursor(1,0);
			lcd.print("Temperatura    ");
			lcd.setCursor(0,1);
			lcd.print(temp[bomba_escolhida]); 
			break;
		case 5: 
			lcd.setCursor(1,0);
			lcd.print("Corrente       ");
			lcd.setCursor(0,1);
			lcd.print(corrente); 
			break;
		case 6: 
			lcd.setCursor(1,0);
			lcd.print("Qtd de Erro    ");
			lcd.setCursor(0,1);
			lcd.print(bombaErro[bomba_escolhida]); 
			break;
	}
	delay(10);
	return readButtsC3();

} //end menu_select


void readButtsC1()
{
	if(!digitalRead(menu))  menu_flag  = 0x01;
	if(!digitalRead(enter)) enter_flag = 0x01;
	if(!digitalRead(voltar)) voltar_flag = 0x01;

	if(digitalRead(menu) && menu_flag)
	{
		menu_flag = 0x00;
		list_menu(lineC1, QUANT_C1);
	}

	if(digitalRead(enter) && enter_flag)
	{
		enter_flag = 0x00; 
		loopC2();
		bomba_escolhida = lineC1[0];
	}

	if(digitalRead(voltar) && voltar_flag)
	{
		voltar_flag = 0x00;
	}



} //end readButts

bool readButtsC2()
{
	if(!digitalRead(menu))  menu_flag  = 0x01;
	if(!digitalRead(enter)) enter_flag = 0x01;
	if(!digitalRead(voltar)) voltar_flag = 0x01;

	if(digitalRead(menu) && menu_flag)
	{
		menu_flag = 0x00;
		list_menu(lineC2, QUANT_C2);
		menu_number+=1;
		if(menu_number > QUANT_C3)
			menu_number = 1;
	}

	if(digitalRead(enter) && enter_flag)
	{
		enter_flag = 0x00; 
		loopC3();
	}

	if(digitalRead(voltar) && voltar_flag)
	{
		voltar_flag = 0x00;
		return false;
	}

	return true;
}

bool readButtsC3()
{
	if(!digitalRead(menu))  menu_flag  = 0x01;
	if(!digitalRead(enter)) enter_flag = 0x01;
	if(!digitalRead(voltar)) voltar_flag = 0x01;

	if(digitalRead(menu) && menu_flag) 	//if para colocar modificacoes de valores
	{
		menu_flag = 0x00;
	}

	if(digitalRead(enter) && enter_flag)
	{
		enter_flag = 0x00; 
	}

	if(digitalRead(voltar) && voltar_flag)
	{
		voltar_flag = 0x00;
		return false;
	}

	return true;
}


void list_menu(int line[], int quantidade)
{
	int index, i, line_bk[quantidade];

	for(i=quantidade -1; i>-1; i--) 
	{
		index = i-1;
		line_bk[i] = line[i];

		if(index < 0) line[i] = line_bk[i+ quantidade-1];

		else line[i] = line[i-1];

	}

} //end list_menu

void atualiza()
{
  modbus_update(); // processando o pacote de comunicação
  temp[0] =  holdingRegs[TEMP1]; 
  temp[1] =  holdingRegs[TEMP2]; 
  niv[0] =  holdingRegs[NIV_INF]; 
  niv[1] =  holdingRegs[NIV_SUP]; 
  corrente =  holdingRegs[CORRENTE]; 
  vazao =  holdingRegs[VAZAO]; 
  bombaErro[0] =  holdingRegs[QTD_ERRO_B1]; 
  bombaErro[1] =  holdingRegs[QTD_ERRO_B2]; 
  bombaAtual =  holdingRegs[BOMBA];
}


