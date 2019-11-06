#include <LiquidCrystal.h>            // Inclusão da Biblioteca do LCD

#define INT_TECLADO 2
#define UP 3
#define ENTER 4
#define DOWN 5
#define MENU 6
#define TARA 7

#define  ADSK  12                     //Clock
#define  ADDO  13                     //Data Out


//Protótipos das Funções

unsigned long ReadCount();

void option_menu();
void cereais();
void verduras();
void frutas();
void carnes();
void leites();
void nutrientes();
void ovos();
void leguminosas();

//Declaração das Variáveis

byte cedilha[8] = {B01110, B10001, B10000, B10000, B10001, B01110, B00100, B01100};



int peso_g=100,
linha=0,
linha_copia=0,
flag_linha=0;

unsigned long convert;

long tara_inicio;

volatile bool flag = 0,
menu = 0,
tara = 0,
cima = 0,
baixo = 0,
sair_menu1=0,
sair_menu2=0,
sair_menu3=0,
confirma=0;
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); //rs en d4 d5 d6 d7

void setup()
{ //Inicio do Setup

    Serial.begin(9600);            //Baudrate

    pinMode(UP, INPUT_PULLUP);    // Teclado
    pinMode(ENTER, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);
    pinMode(MENU, INPUT_PULLUP);
    pinMode(TARA, INPUT_PULLUP);

    pinMode(ADDO, INPUT_PULLUP);   //Hx
    pinMode(ADSK, OUTPUT);

    pinMode(A0, OUTPUT);           //LCD
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(A3, OUTPUT);
    pinMode(A4, OUTPUT);
    pinMode(A5, OUTPUT);

    /* Configuração Interrupção Externa */
    EICRA=B00000010;       //Interrupção Por Borda de Descida
    EIMSK=B00000001;       //Habilita INT0
    sei();                 //Habilita as interrupções Globais

    lcd.createChar(0, cedilha);    //Criação do caractere "ç"
    lcd.begin(20,4);               //Inicilização do Display
    delay(10);
    lcd.setCursor(1, 1);           //Exibir "Balança Eletronica de Nutrientes" ao ligar
    lcd.print("BALAN");
    lcd.write((byte)0);
    lcd.print("A ELETRONICA");
    lcd.setCursor(4, 2);
    lcd.print("DE NUTRIENTES");
    delay(3000);                   //Delay na inicalização para evitar falhas na tara inicial.
    // tara_inicio = ReadCount();     //Atribui o valor lido como a tara.
}                                //Fim do Setup.
void loop()
{

    //convert = ReadCount();//Valor lido na balança.
    //peso_g = map(convert, tara_inicio, (tara_inicio + 697200) , 0, 1532); //Conversão para gramas.
    Serial.println(convert);
    lcd.clear();//Exibe o peso no display.
    lcd.setCursor(9, 1);
    lcd.print(peso_g);
    lcd.print("g");

    // if(tara==1) tara_inicio=ReadCount();
    if(menu==1) option_menu();
}
ISR(INT0_vect)
{
    if(digitalRead(UP)==LOW)
    {
        cima=1;
    }
    else if(digitalRead(ENTER)==LOW)
    {
        confirma=1;
    }
    else if(digitalRead(DOWN)==LOW)
    {
        baixo=1;
    }
    else if(digitalRead(MENU)==LOW)
    {
        menu=1;
    }
    else if(digitalRead(TARA)==LOW)
    {
        tara=1;
    }
}
unsigned long ReadCount()
{
    unsigned long Count = 0;
    unsigned char i;

    digitalWrite(ADSK, LOW);   //Dados não estão prontos.
    while (digitalRead(ADDO)); //Enquanto DOUT está high,o código não avançará.
    for (i = 0; i < 24; i++)   //24 pulsos em CLOCK.
    {
        digitalWrite(ADSK, HIGH);//Clock HIGH.
        Count = Count << 1;      //Shift Left, para que o próximo o bit seja menos significativo que o anterior.
        digitalWrite(ADSK, LOW); //Clock LOW, caracterizando 1 pulso.
        if (digitalRead(ADDO))   //Se DOUT for HIGH, então o bit a ser recebido é HIGH, logo Count recebe +1.
            Count++;
    }
    digitalWrite(ADSK, HIGH);  //o 25º pulso coloca DOUT para HIGH novamente.
    Count = Count ^ 0x800000;  //Conversão de Count.
    digitalWrite(ADSK, LOW);   //Fim do 25º Pulso
    return (Count);            //Retorna Count.
}
void nutrientes(float calorias,
                float umidade,
                float proteinas,
                float lipideos,
                float carboidratos,
                float fibras,
                float calcio,
                float ferro,
                float sodio)
{
    sair_menu1=0;
    sair_menu2=0;
    sair_menu3=0;
    linha=0;
    lcd.clear();
    while(sair_menu3==0)
    {   
        if(menu)
        {
          sair_menu3=1;
          sair_menu2=1;
          sair_menu1=1;
          menu=0;
          confirma=0;
        }
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>8)linha=8;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }

        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;
        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("Calorias:");
            lcd.print(calorias);
            lcd.print("kcal");
            lcd.setCursor(1,1);
            lcd.print("Umidade:");
            lcd.print(umidade);
            lcd.print("%");
            lcd.setCursor(1,2);
            lcd.print("Proteinas:");
            lcd.print(proteinas);
            lcd.print("g");
            lcd.setCursor(1,3);
            lcd.print("Lipideos:");
            lcd.print(lipideos);
            lcd.print("g");
        }
        else if (linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("Carboidratos:");
            lcd.print(carboidratos);
            lcd.print("g");
            lcd.setCursor(1,1);
            lcd.print("Fibras:");
            lcd.print(fibras);
            lcd.print("g");
            lcd.setCursor(1,2);
            lcd.print("Calcio:");
            lcd.print(calcio);
            lcd.print("mg");
            lcd.setCursor(1,3);
            lcd.print("Ferro:");
            lcd.print(ferro);
            lcd.print("mg");
        }
        else if(linha<9)
        {
            lcd.setCursor(1,0);
            lcd.print("Sodio:");
            lcd.print(sodio);
            lcd.print("mg");
        }
    }
}

void option_menu()
{
    sair_menu1=0;
    menu=0;
    linha=0;
    confirma=0;
    baixo=0;
    cima=0;
    while(sair_menu1==0)
    {
        if(confirma==1)
        {
            confirma=0;
            switch(linha)
            {
                case 0:cereais();break;
                case 1:verduras();break;
                case 2:frutas();break;
                case 3:carnes();break;
                case 4:leites();break;
                case 5:ovos();break;
              //  case 6:leguminosas();break;
                }
                menu=0;
                lcd.clear();
        }
        if(menu)
        {
          sair_menu1=1;;
          menu=0;
        }
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>7)linha=7;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Cereais");
            lcd.setCursor(1,1);
            lcd.print("[2]Verduras/Hort.");
            lcd.setCursor(1,2);
            lcd.print("[3]Frutas");
            lcd.setCursor(1,3);
            lcd.print("[4]Carnes/Derivados");
        }

        if(linha>3)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]Leites/Derivados");
            lcd.setCursor(1,1);
            lcd.print("[6]Ovos e Derivados");
            lcd.setCursor(1,2);
            lcd.print("[7]Leguminosas");
        }
    }
}
void cereais()
{
    float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("Cereais");
    delay(1000);
    lcd.clear();

    sair_menu2=0;
    menu=0;
        linha=0;

    while(sair_menu2==0)
    {
        if(confirma==1)
        {
            confirma=0;
            switch(linha)
            {
                case  0:

                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Arroz Integral ");
                    calorias=peso_g*3.6;
                    umidade=12,2;
                    proteinas=peso_g*0.073;
                    lipideos=peso_g*0.019;
                    carboidratos=peso_g*0.775;
                    fibras=peso_g*0.048;
                    calcio=peso_g*0.05;
                    ferro=peso_g*0.003;
                    sodio=peso_g*0.01;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 1:
                    lcd.clear();
                    lcd.setCursor(4,1);
                    lcd.print("Arroz Branco");
                    calorias=peso_g*3.58;
                    umidade=13,2;
                    proteinas=peso_g*0.072;
                    lipideos=peso_g*0.003;
                    carboidratos=peso_g*0.788;
                    fibras=peso_g*0.016;
                    calcio=peso_g*0.04;
                    ferro=peso_g*0.007;
                    sodio=peso_g*0.01;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 2:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Cereal Matinal ");
                    calorias=peso_g*3.58;
                    umidade=13,2;
                    proteinas=peso_g*0.072;
                    lipideos=peso_g*0.003;
                    carboidratos=peso_g*0.788;
                    fibras=peso_g*0.016;
                    calcio=peso_g*0.04;
                    ferro=peso_g*0.007;
                    sodio=peso_g*0.01;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 3:
                    lcd.clear();
                    lcd.setCursor(2,1);
                    lcd.print("Macarrao de Trigo");
                    calorias=peso_g*3.71;
                    umidade=10.2;
                    proteinas=peso_g*0.1;
                    lipideos=peso_g*0.013;
                    carboidratos=peso_g*0.779;
                    fibras=peso_g*0.029;
                    calcio=peso_g*0.17;
                    ferro=peso_g*0.09;
                    sodio=peso_g*0.07;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 4:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Pao Frances");
                    calorias=peso_g*3;
                    umidade=28.5;
                    proteinas=peso_g*0.08;
                    lipideos=peso_g*0.031;
                    carboidratos=peso_g*0.586;
                    fibras=peso_g*0.023;
                    calcio=peso_g*0.16;
                    ferro=peso_g*0.01;
                    sodio=peso_g*6.48;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 5:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Pao de Forma");
                    calorias=peso_g*2.53;
                    umidade=34.7;
                    proteinas=peso_g*0.094;
                    lipideos=peso_g*0.037;
                    carboidratos=peso_g*0.499;
                    fibras=peso_g*0.069;
                    calcio=peso_g*1.32;
                    ferro=peso_g*0.03;
                    sodio=peso_g*5.07;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 6:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Pao de Forma Int.");
                    calorias=peso_g*2.92;
                    umidade=30.4;
                    proteinas=peso_g*0.083;
                    lipideos=peso_g*0.031;
                    carboidratos=peso_g*0.564;
                    fibras=peso_g*0.043;
                    calcio=peso_g*0,78;
                    ferro=peso_g*0.03;
                    sodio=peso_g*5.06;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 7:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Aveia de Flocos");
                    calorias=peso_g*3.94;
                    umidade=9.1;
                    proteinas=peso_g*0.139;
                    lipideos=peso_g*0.085;
                    carboidratos=peso_g*0.666;
                    fibras=peso_g*0.091;
                    calcio=peso_g*0.48;
                    ferro=peso_g*0.044;
                    sodio=peso_g*5;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 8:
                    lcd.clear();
                    lcd.setCursor(2,1);
                    lcd.print("Farinha de Trigo");
                    calorias=peso_g*3.6;
                    umidade=13;
                    proteinas=peso_g*0.098;
                    lipideos=peso_g*0.014;
                    carboidratos=peso_g*0.751;
                    fibras=peso_g*0.023;
                    calcio=peso_g*0.18;
                    ferro=peso_g*0.01;
                    sodio=peso_g*0.01;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;
                case 9:
                    lcd.clear();
                    lcd.setCursor(8,1);
                    lcd.print("Pipoca");
                    calorias=peso_g*4.48;
                    umidade=2.8;
                    proteinas=peso_g*0.099;
                    lipideos=peso_g*0.159;
                    carboidratos=peso_g*0.703;
                    fibras=peso_g*0.143;
                    calcio=peso_g*0.03;
                    ferro=peso_g*0.012;
                    sodio=peso_g*0.04;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;
            }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>9)linha=9;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Arroz Integral");
            lcd.setCursor(1,1);
            lcd.print("[2]Arroz Branco");
            lcd.setCursor(1,2);
            lcd.print("[3]Cereal Matinal");
            lcd.setCursor(1,3);
            lcd.print("[4]Macarrao Trigo");
        }
        else if(linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]Pao Frances");
            lcd.setCursor(1,1);
            lcd.print("[6]Pao de Forma");
            lcd.setCursor(1,2);
            lcd.print("[7]Pao de Forma Int.");
            lcd.setCursor(1,3);
            lcd.print("[8]Aveia de Flocos");
        }
        else if(linha<10)
        {
            lcd.setCursor(1,0);
            lcd.print("[9]Farinha de Trigo");
            lcd.setCursor(1,1);
            lcd.print("[10]Pipoca");
        }
    }
}
void verduras()
{
    float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("Verduras");
    lcd.setCursor(4,2);
    lcd.print("e Hortali");
    lcd.write((byte)0);
    lcd.print("as");
    delay(1000);
    lcd.clear();

    sair_menu2=0;
    menu=0;
    linha=0;

    while(sair_menu2==0)
    {
        if(confirma==1)
        {
            switch(linha)
            {
                confirma=0;

                case  0:

                    lcd.clear();
                    lcd.setCursor(7,1);
                    lcd.print("Alface");
                    calorias=peso_g*0.14;
                    umidade=95;
                    proteinas=peso_g*0.017;
                    lipideos=peso_g*0.001;
                    carboidratos=peso_g*0.024;
                    fibras=peso_g*0.023;
                    calcio=peso_g*0.28;
                    ferro=peso_g*0.006;
                    sodio=peso_g*0.04;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 1:
                    lcd.clear();
                    lcd.setCursor(3,1);
                    lcd.print("Batata Inglesa");
                    calorias=peso_g*0.64;
                    umidade=82.9;
                    proteinas=peso_g*0.018;
                    lipideos=peso_g*0;
                    carboidratos=peso_g*0.147;
                    fibras=peso_g*0.012;
                    calcio=peso_g*0.04;
                    ferro=peso_g*0.004;
                    sodio=peso_g*0;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 2:
                    lcd.clear();
                    lcd.setCursor(4,1);
                    lcd.print("Batata Doce");
                    calorias=peso_g*1.18;
                    umidade=69.5;
                    proteinas=peso_g*0.013;
                    lipideos=peso_g*0.001;
                    carboidratos=peso_g*0.282;
                    fibras=peso_g*0.026;
                    calcio=peso_g*0.21;
                    ferro=peso_g*0.004;
                    sodio=peso_g*0.09;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 3:
                    lcd.clear();
                    lcd.setCursor(4,1);
                    lcd.print("Beterraba");
                    calorias=peso_g*0.49;
                    umidade=86;
                    proteinas=peso_g*0.019;
                    lipideos=peso_g*0.001;
                    carboidratos=peso_g*0.111;
                    fibras=peso_g*0.034;
                    calcio=peso_g*0.18;
                    ferro=peso_g*0.003;
                    sodio=peso_g*0.1;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 4:
                    lcd.clear();
                    lcd.setCursor(6,1);
                    lcd.print("Brocolis");
                    calorias=peso_g*0.25;
                    umidade=91.2;
                    proteinas=peso_g*0.036;
                    lipideos=peso_g*0.003;
                    carboidratos=peso_g*0.04;
                    fibras=peso_g*0.029;
                    calcio=peso_g*0.86;
                    ferro=peso_g*0.006;
                    sodio=peso_g*0.03;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 5:
                    lcd.clear();
                    lcd.setCursor(6,1);
                    lcd.print("Cenoura");
                    calorias=peso_g*0.34;
                    umidade=91.2;
                    proteinas=peso_g*0.036;
                    lipideos=peso_g*0.003;
                    carboidratos=peso_g*0.04;
                    fibras=peso_g*0.029;
                    calcio=peso_g*0.86;
                    ferro=peso_g*0.006;
                    sodio=peso_g*0.03;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 6:
                    lcd.clear();
                    lcd.setCursor(4,1);
                    lcd.print("Chuchu");
                    calorias=peso_g*0.17;
                    umidade=94.8;
                    proteinas=peso_g*0.007;
                    lipideos=peso_g*0.001;
                    carboidratos=peso_g*0.041;
                    fibras=peso_g*0.013;
                    calcio=peso_g*0.12;
                    ferro=peso_g*0.002;
                    sodio=peso_g*0;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 7:
                    lcd.clear();
                    lcd.setCursor(5,1);
                    lcd.print("Couve-Flor");
                    calorias=peso_g*0.23;
                    umidade=92.8;
                    proteinas=peso_g*0.019;
                    lipideos=peso_g*0.002;
                    carboidratos=peso_g*0.045;
                    fibras=peso_g*0.024;
                    calcio=peso_g*0.18;
                    ferro=peso_g*0.005;
                    sodio=peso_g*0.03;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;

                case 8:
                    lcd.clear();
                    lcd.setCursor(6,1);
                    lcd.print("Mandioca");
                    calorias=peso_g*1.51;
                    umidade=61.8;
                    proteinas=peso_g*0.011;
                    lipideos=peso_g*0.003;
                    carboidratos=peso_g*0.362;
                    fibras=peso_g*0.019;
                    calcio=peso_g*0.15;
                    ferro=peso_g*0.003;
                    sodio=peso_g*0.02;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;
                case 9:
                    lcd.clear();
                    lcd.setCursor(1,1);
                    lcd.print("Pimentao Vermelho");
                    calorias=peso_g*0.23;
                    umidade=92.9;
                    proteinas=peso_g*0.01;
                    lipideos=peso_g*0.001;
                    carboidratos=peso_g*0.055;
                    fibras=peso_g*0.016;
                    calcio=peso_g*0.06;
                    ferro=peso_g*0.003;
                    sodio=peso_g*0;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;
                case 10:
                    lcd.clear();
                    lcd.setCursor(7,1);
                    lcd.print("Tomate");
                    calorias=peso_g*0.15;
                    umidade=95.1;
                    proteinas=peso_g*0.011;
                    lipideos=peso_g*0.002;
                    carboidratos=peso_g*0.031;
                    fibras=peso_g*0.012;
                    calcio=peso_g*0.07;
                    ferro=peso_g*0.002;
                    sodio=peso_g*0.01;
                    delay(1333);
                    nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                    break;
            }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>10)linha=10;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Alface");
            lcd.setCursor(1,1);
            lcd.print("[2]Batata Inglesa");
            lcd.setCursor(1,2);
            lcd.print("[3]Batata Doce");
            lcd.setCursor(1,3);
            lcd.print("[4]Beterraba");
        }
        else if(linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]Brocolis");
            lcd.setCursor(1,1);
            lcd.print("[6]Cenoura");
            lcd.setCursor(1,2);
            lcd.print("[7]Chuchu");
            lcd.setCursor(1,3);
            lcd.print("[8]Couve-flor");
        }
        else if(linha<12)
        {
            lcd.setCursor(1,0);
            lcd.print("[9]Mandioca");
            lcd.setCursor(1,1);
            lcd.print("[10]Pimentao Verm.");
            lcd.setCursor(1,2);
            lcd.print("[11]Tomate");
        }
    }
}
void frutas()
{
    float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    sair_menu1=0;
    sair_menu2=0;
        linha=0;

    lcd.clear();
    lcd.setCursor(7,1);
    lcd.print("Frutas");
    delay(1000);
    lcd.clear();
    while(sair_menu2==0)
    {
        if(confirma==1)
        {
          confirma=0;
        switch(linha)
        {
            case  0:
                lcd.clear();
                lcd.setCursor(6,1);
                lcd.print("Abacate");
                calorias=peso_g*0.96;
                umidade=83.8;
                proteinas=peso_g*0.012;
                lipideos=peso_g*0.084;
                carboidratos=peso_g*0.06;
                fibras=peso_g*0.063;
                calcio=peso_g*0.08;
                ferro=peso_g*0.002;
                sodio=peso_g*0;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 1:
                lcd.clear();
                lcd.setCursor(6,1);
                lcd.print("Abacaxi");
                calorias=peso_g*0.96;
                umidade=86.8;
                proteinas=peso_g*0.009;
                lipideos=peso_g*0.001;
                carboidratos=peso_g*0.123;
                fibras=peso_g*0.01;
                calcio=peso_g*0.22;
                ferro=peso_g*0.003;
                sodio=peso_g*0;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 2:
                lcd.clear();
                lcd.setCursor(8,1);
                lcd.print("A");
                lcd.write(byte(0));
                lcd.print("ai");
                calorias=peso_g*1.1;
                umidade=73.9;
                proteinas=peso_g*0.007;
                lipideos=peso_g*0.037;
                carboidratos=peso_g*0.215;
                fibras=peso_g*0.017;
                calcio=peso_g*0.22;
                ferro=peso_g*0.003;
                sodio=peso_g*0.15;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;
            case 3:
                lcd.clear();
                lcd.setCursor(4,1);
                lcd.print("Banana Prata");
                calorias=peso_g*0.98;
                umidade=71.9;
                proteinas=peso_g*0.013;
                lipideos=peso_g*0.001;
                carboidratos=peso_g*0.26;
                fibras=peso_g*0.02;
                calcio=peso_g*0.08;
                ferro=peso_g*0.004;
                sodio=peso_g*0;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 4:

                lcd.clear();
                lcd.setCursor(8,1);
                lcd.print("Kiwi");
                calorias=peso_g*0.51;
                umidade=85.9;
                proteinas=peso_g*0.013;
                lipideos=peso_g*0.006;
                carboidratos=peso_g*0.115;
                fibras=peso_g*0.027;
                calcio=peso_g*0.24;
                ferro=peso_g*0.003;
                sodio=peso_g*0;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 5:

                lcd.clear();
                lcd.setCursor(6,1);
                lcd.print("Laranja");
                calorias=peso_g*0.46;
                umidade=87;
                proteinas=peso_g*0.011;
                lipideos=peso_g*0.001;
                carboidratos=peso_g*0.115;
                fibras=peso_g*0.018;
                calcio=peso_g*0.31;
                ferro=peso_g*0.001;
                sodio=peso_g*0.01;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 6:

                lcd.clear();
                lcd.setCursor(7,1);
                lcd.print("Limao");
                calorias=peso_g*0.32;
                umidade=87.4;
                proteinas=peso_g*0.009;
                lipideos=peso_g*0.001;
                carboidratos=peso_g*0.111;
                fibras=peso_g*0.012;
                calcio=peso_g*0.5;
                ferro=peso_g*0.002;
                sodio=peso_g*0.01;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 7:

                lcd.clear();
                lcd.setCursor(5,1);
                lcd.print("Ma");
                lcd.write(byte(0));
                lcd.print("a");
                calorias=peso_g*0.56;
                umidade=84.3;
                proteinas=peso_g*0.003;
                lipideos=peso_g*0;
                carboidratos=peso_g*0.152;
                fibras=peso_g*0.013;
                calcio=peso_g*0.02;
                ferro=peso_g*0.001;
                sodio=peso_g*0.;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 8:

                lcd.clear();
                lcd.setCursor(4,1);
                lcd.print("Mamao Papaia");
                calorias=peso_g*0.4;
                umidade=88.6;
                proteinas=peso_g*0.005;
                lipideos=peso_g*0.001;
                carboidratos=peso_g*0.104;
                fibras=peso_g*0.01;
                calcio=peso_g*0.22;
                ferro=peso_g*0.002;
                sodio=peso_g*0.02;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

            case 9:

                lcd.clear();
                lcd.setCursor(7,1);
                lcd.print("Manga");
                calorias=peso_g*0.72;
                umidade=79.7;
                proteinas=peso_g*0.004;
                lipideos=peso_g*0.002;
                carboidratos=peso_g*0.194;
                fibras=peso_g*0.016;
                calcio=peso_g*0.05;
                ferro=peso_g*0.006;
                sodio=peso_g*0.02;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;
            case 10:

                lcd.clear();
                lcd.setCursor(6,1);
                lcd.print("Maracuja");
                calorias=peso_g*0.68;
                umidade=82.9;
                proteinas=peso_g*0.02;
                lipideos=peso_g*0.021;
                carboidratos=peso_g*0.123;
                fibras=peso_g*0.011;
                calcio=peso_g*0.05;
                ferro=peso_g*0.006;
                sodio=peso_g*0.02;
                delay(1333);
                nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
                break;

        }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>9)linha=9;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Abacate");
            lcd.setCursor(1,1);
            lcd.print("[2]Abacaxi");
            lcd.setCursor(1,2);
            lcd.print("[3]A");
            lcd.write(byte(0));
            lcd.print("ai");
            lcd.setCursor(1,3);
            lcd.print("[4]Banana Prata");
        }
        else if(linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]Kiwi");
            lcd.setCursor(1,1);
            lcd.print("[6]Laranja");
            lcd.setCursor(1,2);
            lcd.print("[7]Limao");
            lcd.setCursor(1,3);
            lcd.print("[8]Ma");
            lcd.write(byte(0));
            lcd.print("a");
        }
        else if(linha<10)
        {
            lcd.setCursor(1,0);
            lcd.print("[9]Mamao Papaia");
            lcd.setCursor(1,1);
            lcd.print("[10]Manga");
        }
    }
}
void carnes()
{
  
 float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    sair_menu1=0;
    sair_menu2=0;
        linha=0;

    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("Carnes e Derivados");
    delay(1000);
    lcd.clear();
    while(sair_menu2==0)
    {
        if(confirma==1)
        {
          confirma=0;
        switch(linha)
        {
            case  0:
                 lcd.clear();
              lcd.setCursor(5,1);
              lcd.print("Apresuntado");
              calorias=peso_g*1.29;
              umidade=73.7;
              proteinas=peso_g*0.135;
              lipideos=peso_g*0.067;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.23;
              ferro=peso_g*0.009;
              sodio=peso_g*9.49;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 1:
              lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Charque - Gado");
              calorias=peso_g*2.49;
              umidade=44.5;
              proteinas=peso_g*0.227;
              lipideos=peso_g*0.168;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.15;
              ferro=peso_g*0.015;
              sodio=peso_g*58.75;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

            case 2:
              lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Costela - Gado");
              calorias=peso_g*3.58;
              umidade=52.7;
              proteinas=peso_g*0.167;
              lipideos=peso_g*0.318;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0;
              ferro=peso_g*0.012;
              sodio=peso_g*0.7;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;           
            case 3:
               lcd.clear();
              lcd.setCursor(1,1);
              lcd.print("Coxao Mole - Gado");
              calorias=peso_g*1.69;
              umidade=68.4;
              proteinas=peso_g*0.212;
              lipideos=peso_g*0.057;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.03;
              ferro=peso_g*0.019;
              sodio=peso_g*0.49;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 4:

                  lcd.clear();
              lcd.setCursor(1,1);
              lcd.print("File Mignon - Gado");
              calorias=peso_g*1.43;
              umidade=71.9;
              proteinas=peso_g*0.216;
              lipideos=peso_g*0.056;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.03;
              ferro=peso_g*0.019;
              sodio=peso_g*0.49;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

            case 5:

               lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Picanha - Gado");
              calorias=peso_g*1.34;
              umidade=72.4;
              proteinas=peso_g*0.213;
              lipideos=peso_g*0.047;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.03;
              ferro=peso_g*0.021;
              sodio=peso_g*0.61;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 6:

           lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Coxa - Frango");
              calorias=peso_g*1.2;
              umidade=76.4;
              proteinas=peso_g*0.178;
              lipideos=peso_g*0.049;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.08;
              ferro=peso_g*0.008;
              sodio=peso_g*0.98;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 7:

                    lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Peito - Frango");
              calorias=peso_g*1.19;
              umidade=74.8;
              proteinas=peso_g*0.215;
              lipideos=peso_g*0.03;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.06;
              ferro=peso_g*0.009;
              sodio=peso_g*0.8;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 8:
  lcd.clear();
              lcd.setCursor(1,1);
              lcd.print("Sobrecoxa - Frango");
              calorias=peso_g*1.62;
              umidade=72.7;
              proteinas=peso_g*0.176;
              lipideos=peso_g*0.96;
              carboidratos=peso_g*0;
              fibras=peso_g*0;
              calcio=peso_g*0.06;
              ferro=peso_g*0.009;
              sodio=peso_g*0.8;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

            case 9:

            lcd.clear();
              lcd.setCursor(5,1);
              lcd.print("Mortadela");
              calorias=peso_g*2.69;
              umidade=56.4;
              proteinas=peso_g*0.12;
              lipideos=peso_g*0.216;
              carboidratos=peso_g*0.058;
              fibras=peso_g*0;
              calcio=peso_g*0.67;
              ferro=peso_g*0.015;
              sodio=peso_g*12.12;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

        }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>9)linha=9;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>11) linha_copia=linha-12;
        else if(linha>7) linha_copia=linha-8;
        else if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Apresuntado");
            lcd.setCursor(1,1);
            lcd.print("[2]Charque - Gado");
            lcd.setCursor(1,2);
            lcd.print("[3]Costela - Gado");
            lcd.setCursor(1,3);
            lcd.print("[4]Coxao Mole");
        }
        else if(linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]File Mignon");
            lcd.setCursor(1,1);
            lcd.print("[6]Picanha - Gado");
            lcd.setCursor(1,2);
            lcd.print("[7]Coxa - Frango");
            lcd.setCursor(1,3);
            lcd.print("[8]Peito - Frango");
        }
        else if(linha<10)
        {
            lcd.setCursor(1,0);
            lcd.print("[9]S.Coxa - Frango");
            lcd.setCursor(1,1);
            lcd.print("[10]Mortadela");
        }
    }
}
void leites()
{
  
 float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    sair_menu1=0;
    sair_menu2=0;
        linha=0;

    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("Leites e Derivados");
    delay(1000);
    lcd.clear();
    while(sair_menu2==0)
    {
        if(confirma==1)
        {
          confirma=0;
        switch(linha)
        {
            case  0:
                 lcd.clear();
              lcd.setCursor(1,1);
              lcd.print("Iorgute de Morango");
              calorias=peso_g*0.7;
              umidade=84.6;
              proteinas=peso_g*0.027;
              lipideos=peso_g*0.023;
              carboidratos=peso_g*0.097;
              fibras=peso_g*0.002;
              calcio=peso_g*1.01;
              ferro=peso_g*0;
              sodio=peso_g*0.38;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
              
            case 1:
              lcd.clear();
              lcd.setCursor(2,1);
              lcd.print("Queijo Mozarela");
              calorias=peso_g*3.3;
              umidade=45.3;
              proteinas=peso_g*0.226;
              lipideos=peso_g*0.252;
              carboidratos=peso_g*0.038;
              fibras=peso_g*0;
              calcio=peso_g*8.75;
              ferro=peso_g*0.003;
              sodio=peso_g*5.81;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

            case 2:
             lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Queijo Prato");
              calorias=peso_g*3.6;
              umidade=42.4;
              proteinas=peso_g*0.227;
              lipideos=peso_g*0.291;
              carboidratos=peso_g*0.019;
              fibras=peso_g*0;
              calcio=peso_g*9.4;
              ferro=peso_g*0.003;
              sodio=peso_g*5.8;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;       
            case 3:
                lcd.clear();
              lcd.setCursor(2,1);
              lcd.print("Queijo Requeijao");
              calorias=peso_g*2.57;
              umidade=62.5;
              proteinas=peso_g*0.096;
              lipideos=peso_g*0.234;
              carboidratos=peso_g*0.024;
              fibras=peso_g*0;
              calcio=peso_g*2.59;
              ferro=peso_g*0.001;
              sodio=peso_g*5.558;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
            case 4:

              lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Queijo Ricota");
              calorias=peso_g*1.4;
              umidade=73.6;
              proteinas=peso_g*0.126;
              lipideos=peso_g*0.081;
              carboidratos=peso_g*0.038;
              fibras=peso_g*0;
              calcio=peso_g*2.59;
              ferro=peso_g*0.001;
              sodio=peso_g*2.83;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;

           
        }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>9)linha=9;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Iorgute Morango");
            lcd.setCursor(1,1);
            lcd.print("[2]Queijo Mozarela");
            lcd.setCursor(1,2);
            lcd.print("[3]Queijo Prato");
            lcd.setCursor(1,3);
            lcd.print("[4]Queijo Requeijao");
        }
        else if(linha<8)
        {
            lcd.setCursor(1,0);
            lcd.print("[5]Queijo Ricota");
        }
    
    }
}

void ovos()
{
  
 float calorias=0,
    umidade=0,
    proteinas=0,
    lipideos=0,
    carboidratos=0,
    fibras=0,
    calcio=0,
    ferro=0,
    sodio=0;

    sair_menu1=0;
    sair_menu2=0;
        linha=0;

    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Ovos e Derivados");
    delay(1000);
    lcd.clear();
    while(sair_menu2==0)
    {
        if(confirma==1)
        {
          confirma=0;
        switch(linha)
        {
            case  0:
                lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Ovo de Codorna");
              calorias=peso_g*1.77;
              umidade=71.7;
              proteinas=peso_g*0.137;
              lipideos=peso_g*0.127;
              carboidratos=peso_g*0.008;
              fibras=peso_g*0;
              calcio=peso_g*0.79;
              ferro=peso_g*0.033;
              sodio=peso_g*1.29;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
              
            case 1:
               lcd.clear();
              lcd.setCursor(3,1);
              lcd.print("Ovo de Galinha");
              calorias=peso_g*1.43;
              umidade=75.6;
              proteinas=peso_g*0.13;
              lipideos=peso_g*0.089;
              carboidratos=peso_g*0.016;
              fibras=peso_g*0;
              calcio=peso_g*0.42;
              ferro=peso_g*0.016;
              sodio=peso_g*1.68;
              delay(1333);
              nutrientes(calorias,umidade,proteinas,lipideos,carboidratos,fibras,calcio,ferro,sodio);
              break;
           }
        }
        if(menu)sair_menu2=1;;
        if(baixo)
        {
            baixo=0;
            linha++;
            if(linha>1)linha=1;
            lcd.clear();
            flag_linha=0;
        }
        else if(cima)
        {
            cima=0;
            linha--;
            if(linha<0) linha=0;
            lcd.clear();
            flag_linha=0;
        }
        if(linha>3) linha_copia=linha-4;
        else linha_copia=linha;

        lcd.setCursor(0,linha_copia);
        lcd.print(">");

        if(linha<4)
        {
            lcd.setCursor(1,0);
            lcd.print("[1]Ovo de Galinha");
            lcd.setCursor(1,1);
            lcd.print("[2]Ovo de Codorna");
           
        }
    }
}

