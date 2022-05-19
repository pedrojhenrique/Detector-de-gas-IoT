//==========================================================================================

// Mapeamento de hardware //
#define MQ2 34
#define BUZZER 18
#define MUTARBUZ 5
#define RED 32
#define GREEN 33

// Debounce de 10 milissegundos para evitar 
// erros de acionamento do botão
#define TEMPO_DEBOUNCE 10

// Número de pontos de leitura da média móvel
#define      nimm     100  

//==========================================================================================

// Bibliotecas necessárias
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

//==========================================================================================

// Token a ser inserido para se ter acesso ao código no Blynk
char auth[] = "x5r4OPqnOePDK-FoYkEz7zIhOWNfe2bA";

// Nome e senha do WiFi que será utilizado
char ssid[] = "Nasi";
char pass[] = "26118950";

//==========================================================================================

// Declaração de variáveis //
                 
// Variável tipo short para leitura de gás 
short nivelSensor = 0;  

// Estado inicial do buzzer
int estadoBuz = HIGH;

// Flag para o botão que silencia o buzzer
int flag=0;

// Função para tempo de último acionamento da interrupção
unsigned long tempo_ultimo_acionamento = 0;

// Função que conta tempo "atual" do buzzer
unsigned long tempoBuz = 0;

// Constante para acionar buzzer em 500 milissegundos
const long intervaloBuz = 500;

// Armazena o tempo da ultima vez que a leitura foi enviada
unsigned long tempoP = 0;

// Intervalo de tempo entre leituras do sensor
const long intervalo = 3000;

// Recebe o valor de MQ2
int  original,
// Recebe o valor original filtrado          
     filtrado;   
                 
//vetor com os valores para média móvel
int       numbers[nimm];        

//==========================================================================================

// Protótipos das funções auxiliares //

void configuracao();
void inicializacao();
void leNivel();
void mediamovel();
void nivelAlto();
void lcdAlto();
void nivelBaixo();
void lcdBaixo();

// Função para filtro de média móvel
long moving_average();

//==========================================================================================

// Inicializa o display no endereço hexa 0x27
LiquidCrystal_I2C lcd(0x27,16,2);

//==========================================================================================

// Função ISR (chamada quando há interrupção externa) //

  void IRAM_ATTR funcao_ISR()
{
// Conta acionamentos do botão considerando debounce 
  if ( (millis() - tempo_ultimo_acionamento) >= TEMPO_DEBOUNCE )
{
  tempo_ultimo_acionamento = millis();
  digitalWrite(BUZZER, LOW);
  flag=1;
}
}

//==========================================================================================

// Desliga buzzer através do botão virtual no Blynk

BLYNK_WRITE(1) 
{
  // Variável tipo inteira para lógica Blynk
  int i=param.asInt();
  if (i==1) 
{
  digitalWrite(BUZZER, LOW);
  flag=1;
}
}

//==========================================================================================

// Função principal //
             
void setup()
{
   configuracao(); 
}

//==========================================================================================

// Loop eterno //
            
void loop()
{
   inicializacao(); 
   mediamovel();
}

//==========================================================================================

// Função para integrar ao Blynk e fazer a relação do nível de gás //

void nivelGas()
{ 
    leNivel();
       
if (nivelSensor > 600)
{
   nivelAlto(); 
   lcdAlto();
}

else
{
   nivelBaixo();
   lcdBaixo();     
}
}

//==========================================================================================

// Função que lê o nível através do sensor //
 
void leNivel()
{
    // Variável que armazena o valor do tempo atual 
    unsigned long tempoAtual = millis();
    
    // Verifica se o intervalo já foi atingido
    if (tempoAtual - tempoP >= intervalo)
{
    tempoP = tempoAtual;
    nivelSensor = analogRead(MQ2);
    nivelSensor = map(nivelSensor,0,4095,-1000,900);
    Blynk.virtualWrite(V0, nivelSensor);
}
}

//==========================================================================================

// Função para se ter o filtro de média móvel //

long moving_average()
{
  
// Desloca os elementos do vetor de média móvel
for(int i= nimm-1; i>0; i--) numbers[i] = numbers[i-1];

// Posição inicial do vetor recebe a leitura original
   numbers[0] = original; 
   
// Acumulador para somar os pontos da média móvel
   long acc = 0;  
           
// Faz a somatória do número de pontos
   for(int i=0; i<nimm; i++) acc += numbers[i]; 

// Retorna a média móvel
   return acc/nimm;  
}

//==========================================================================================
 
// Função que realiza o filtro de média móvel e a imprime na serial //

  void mediamovel()
{
  original = analogRead(MQ2);
  filtrado = moving_average();
  Serial.print(original);
  Serial.print(" ");
  Serial.println(filtrado);
  delay(20); 
}

//==========================================================================================

// Função que configura parte do Display LCD e inicializa o app Blynk //
   
  void inicializacao()
{
  lcd.setBacklight(HIGH);
  lcd.setCursor(2,0);
  lcd.print("Nivel de Gas");
  Blynk.run();
  timer.run();
}

//==========================================================================================

// Função que configura pinos e a utilização do app Blynk //

  void configuracao()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pinMode(MQ2, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(MUTARBUZ, INPUT);
  attachInterrupt(MUTARBUZ, funcao_ISR, RISING);
  pinMode(GREEN, OUTPUT);
  pinMode(RED, OUTPUT);
  timer.setInterval(1000L, nivelGas);
  lcd.init();
}

//==========================================================================================

// Função que controla LEDs e envia alertas visuais e sonoros com presença de gás //
 
  void nivelAlto()
{ 
  Blynk.notify("Fumaça/Gás detectado!");
  Blynk.email("pedrojhenrique@outlook.com", "ESP/MQ2 Alerta", "Fumaça/Gás detectado!"); 
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, HIGH); 
   
  if(nivelSensor > 600 && flag == 0)
{
  unsigned long tempoA = millis();
  if (tempoA - tempoBuz >= intervaloBuz)
{
  tempoBuz = tempoA;
  estadoBuz = !estadoBuz;
  digitalWrite(BUZZER,estadoBuz);
}
}    
}

//==========================================================================================

// Função para exibir a presença e nível de gás no Display LCD //

  void lcdAlto()
{
  lcd.setCursor(0,1);
  lcd.print("                      ");
  lcd.setCursor(0,1);
  lcd.print("Presente: ");
  lcd.setCursor(13,1);
  lcd.print("ppm");
  lcd.setCursor(9,1);
  lcd.print(nivelSensor);
}

//==========================================================================================

// Função que comanda LEDs sem a presença de gás //
  
  void nivelBaixo()
{
  digitalWrite(GREEN, HIGH);
  digitalWrite(RED, LOW);  
  digitalWrite(BUZZER, LOW); 
  flag=0; 
}

//==========================================================================================

// Função para exibir nível e ausência de gás no Display LCD //

  void lcdBaixo()
{
  lcd.setCursor(0 ,1);
  lcd.print("Ausente:               ");
  lcd.setCursor(12,1);
  lcd.print("ppm");
  lcd.setCursor(8,1);
  lcd.print(nivelSensor);
}

//==========================================================================================
