// ========================================================================================================
// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal.h>


// ========================================================================================================
// --- Mapeamento de Hardware ---
#define     menu     12
#define     enter    11
#define 		voltar   10
#define 		QUANT_C1 2
#define 		QUANT_C2 4
#define 		QUANT_C3 4


// ========================================================================================================
// --- Protótipo das Funções ---
void loopC2();
void loopC3();
void camada1();  
bool camada2();  
bool camada3();
void readButtsC1();
bool readButtsC2();
bool readButtsC3();
void list_menu(int line[], int quantidade);


// ========================================================================================================
// --- Declaração de Objetos ---
LiquidCrystal lcd(7,  //RS no digital 7
		6,  //EN no digital 6
		5,  //D4 no digital 5
		4,  //D5 no digital 4
		3,  //D6 no digital 3
		2); //D7 no digital 2


// ========================================================================================================
// --- Variáveis Globais ---
int      lineC1[QUANT_C1] = {0,1},
				 lineC2[QUANT_C2] = {0,1,2,3},
				 menu_number = 1,
				 bomba_escolhida;

float temp[2] = {60, 45},
			niv = 20,
			corrente = 8,
			vazao = 6;

boolean  menu_flag  = 0,
				 enter_flag = 0,
				 voltar_flag = 0,
				 voltarSt=0,
				 //sub_menuC1 = 0,
				 sub_menuC2 = 0;


// ========================================================================================================
// --- Configurações Iniciais ---
void setup()
{

	//Serial.begin(9600);
	pinMode(menu,  INPUT_PULLUP);
	pinMode(enter, INPUT_PULLUP);
	pinMode(voltar, INPUT_PULLUP);

  Serial.begin(9600);

	lcd.begin (16,3);
	lcd.setCursor(0,0);
	lcd.print(">");


} //end setup


// ========================================================================================================
// --- Loop Infinito ---
void loop()
{
	camada1();  
} 

void loopC2()
{
	while(camada2()==true);
}

void loopC3()
{
	while(camada3()==true);
}

void camada1()
{
	lcd.setCursor(0,1);
	lcd.print(" ");
	lcd.setCursor(1,lineC1[0]);
	lcd.print("1) Bomba1 On    ");
	lcd.setCursor(1,lineC1[1]);
	lcd.print("2) Bomba2 Off   ");

  delay(10);
	readButtsC1();
}

bool camada2()
{
	lcd.setCursor(0,1);
	lcd.print(" ");
	lcd.setCursor(1,lineC2[0]);
	lcd.print("1) Nivel        ");
	lcd.setCursor(1,lineC2[1]);
	lcd.print("2) Vazao        ");
	lcd.setCursor(1,lineC2[2]);
	lcd.print("3) Temperatura  ");
	lcd.setCursor(1,lineC2[3]);
	lcd.print("4) Corrente     ");
  
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
			lcd.print("Nivel Control  ");
			lcd.setCursor(0,1);
			lcd.print(niv); 
			break;
		case 2: 
			lcd.setCursor(1,0);
			lcd.print("Vazao Control  ");
			lcd.setCursor(0,1);
			lcd.print(vazao); 
			break;
		case 3: 
			lcd.setCursor(1,0);
			lcd.print("Temperatura Ctr");
			lcd.setCursor(0,1);
			lcd.print(temp[bomba_escolhida]); 
			break;
		case 4: 
			lcd.setCursor(1,0);
			lcd.print("Corrente Ctrl  ");
			lcd.setCursor(0,1);
			lcd.print(corrente); 
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




