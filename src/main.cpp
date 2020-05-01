/*
   Curso de Arduino e AVR WR Kits Channel
   
   Aula 50 - Escrita e Leitura em cartão SD (teste do Shield SD Card)
   
   Hardware:
  Ligações do SD Card Arduino UNO
  
 > CS   - pin 10
 > MOSI - pin 11
 > MISO - pin 12
 > CLK  - pin 13


 Ligações do SD Card Arduino MEGA
  
 > CS   - pin 53
 > MOSI - pin 51
 > MISO - pin 50
 > CLK  - pin 13

   
    Autor: Eng. Wagner Rambo  Data: Janeiro de 2016
   
   www.wrkits.com.br | facebook.com/wrkits | youtube.com/user/canalwrkits
 	 
*/
 
#include <SD.h>                        //Biblioteca SD inclusa

File myFile;                           //Cria um ponteiro para arquivo


// --- Mapeamento de Hardware ---
#define CS_pin      53               //Comunicação SPI, CS_pin no digital 10
#define temp_pin    A0               //Sensor de temperatura no pino analógico 0
#define light_pin   A1               //Sensor de luminosidade no pino analógico 1


// --- Protótipo das Funções Auxiliares ---
int AVERAGE_TEMP();                  //Função para calcular a média de temperatura


// --- Variáveis Globais ---
const int voltage = 5;              //Alimentação do Arduino
const int celsius_unity = 100;      //Variação em Celsius
const int ad_resolution = 1024;     //Resolução do conversor AD utilizada (10 bits)

unsigned int store01;               //Variável de armazenamento
unsigned int resto01;               //Recebe o primeiro resto de divisão
unsigned int resto02;               //Recebe o segundo resto da divisão
unsigned int dec01;                 //Referente à primeira divisão, primeiro valor após ponto decimal
unsigned int dec02;                 //Referente à primeira divisão, primeiro valor após ponto decimal

unsigned long minuto = 1;           //Armazena os minutos transcorridos


// --- Configurações Iniciais ---
void setup()
{
 // -- Inicia comunicação serial e aguarda sua abertura --
  Serial.begin(9600);
   //while (!Serial) {  ;   }


  Serial.print("Inicializando SD card.....");     //Escreve frase de inicialização
 
  pinMode(CS_pin, OUTPUT);                            //Configura saída para Chip Select
   
  if (!SD.begin(CS_pin))                              //Testa se o cartão inicializa com sucesso
  {
    Serial.println("Falha no cartao!");           //Não, falha no cartão
    return;                                       //retorna
  }
  
  Serial.println("Sucesso na inicializacao!");    //Sim, inicialização ok
  
  
  myFile = SD.open("logger.csv", FILE_WRITE);     //Abre arquivo para escrita
  
  
  if (myFile)                                     //Arquivo aberto com sucesso?
  {                                               //Sim...
    myFile.println(", , ,"); // 
    String header = "Minuto, T_Celsius, T_Fahrenheit, Luminosidade, Teste01, Teste02";
    
    
    myFile.println(header);
    myFile.close();
    Serial.println(header);
  } //end if
  else                                            //Não...
  {
    
    Serial.println("Erro ao abrir arquivo");      //Informa que há algum erro
  }
  
   
  
} //end setup


// --- Loop Infinito
void loop()
{
 //Verifica nível de luminosidade
  int lum_nivel = analogRead(light_pin);
  
  //Verifica temperatura
  //int temp_store = analogRead(temp_pin);
  
  //Cálculo de resolução AD e conversão Celsius ---------------- |
  store01 = AVERAGE_TEMP() * voltage * celsius_unity;
  
  unsigned long temp_celsius = store01 / ad_resolution;
  
  resto01 = store01 % ad_resolution;
  
  dec01 = (resto01 * 10) / ad_resolution;
  
  resto02 = (resto01 * 10) % ad_resolution;
  
  dec02 = (resto02 * 10) / ad_resolution;
  //Final dos cálculos ------------------------------------------ |
  
  //int temp_celsius = temp_store*((5/1024)*100); //Converte grandeza analógica para Graus Celsius
  int temp_fahrenheit = (temp_celsius*1.8)+32;  //Converte Graus Celsius para Graus Fahrenheit


  int teste01 = 0 , teste02 = 0;
  
 
  //Cria string de dados para armazenar no cartão SD
  //Utilizando arquivo do tipo Comma Separete Value
  String dataString = String(minuto) + ", " + String(temp_celsius) + "." + String(dec01) + String(dec02) + ", " + String(temp_fahrenheit) + ", " + String(lum_nivel) + 
  
  ", " + String(teste01) + ", " + String(teste02);

  
  //Abre o arquivo para escrita
  //Apenas um arquivo pode ser aberto de cada vez
  File myFile = SD.open("logger.csv", FILE_WRITE);
  if(myFile)
  {
    myFile.println(dataString);
    myFile.close();
    Serial.println(dataString);
  } //end if logFile
  else
  {
    Serial.println("Erro ao abrir arquivo para escrita final");
  }
  
  //Incrementa os minutos
  minuto++;
  
  delay(60000);
  
  
} //end loop


// --- Desenvolvimento das Funções Auxiliares ---

int AVERAGE_TEMP() //retorna a média entre 60 leituras de temperatura
{
  int temp_store = 0;
  
  for(int i=0; i<60; i++)
  {
    temp_store += analogRead(temp_pin); //temp_store = temp_store + analogRead(temp_pin);
  }
  
  return(temp_store/60);

  
  


}
