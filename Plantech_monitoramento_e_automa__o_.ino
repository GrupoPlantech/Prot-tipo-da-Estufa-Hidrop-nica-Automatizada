/*          Grupo Plantech
    Automação e monitoramento de estufas hidropônicas

    Turma: Técnico em mecatrônica 2019003

    Instrutores: Leandro Oliveira
                 Romulo Costa
                 
    Membros: Hugo Castelar Lack Veiga
             Iury Cabral Chermont
             Luiz Felipe Macario da Cruz
             Vinícius Santiago de Paula
             Ricardo Calmona Souza
*/


#include <SimpleModbusSlave.h>

#include <dht.h>
dht DHT;

#define DHT11_PIN 2
#define ledOn 3
#define luz 6
#define cooler 8
#define bomba 9

int sensorNv = A1;    
int LDR = A0;

enum 
{
  LUMINOSIDADE,
  TEMPERATURA,
  UMIDADE,
  NIVEL,
  ILUMINACAO,
  REFRIGERACAO,
  BOMBEAMENTO,            
  HOLDING_REGS_SIZE 
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; 


void setup(){
 
    Serial.begin(9600);
    
    modbus_configure(&Serial, 9600, SERIAL_8N1, 1, 2, HOLDING_REGS_SIZE, holdingRegs);
    modbus_update_comms(9600, SERIAL_8N1, 1);
    
    pinMode(ledOn,  OUTPUT);
    pinMode(luz,    OUTPUT);
    pinMode(cooler, OUTPUT);
    pinMode(bomba,  OUTPUT);
    pinMode(LDR, INPUT);
    pinMode(sensorNv, INPUT);

    digitalWrite(cooler, HIGH);
    digitalWrite(bomba, HIGH);
    digitalWrite(luz, HIGH);
    
}
void loop() {
  
  modbus_update();

   digitalWrite(ledOn, HIGH);

    int nivel = (analogRead(sensorNv)*0.09775);

    int intensidade = (analogRead(LDR)*0.09775);

    int chk = DHT.read11(DHT11_PIN);
   
    holdingRegs[LUMINOSIDADE] =   analogRead(LDR); 
    holdingRegs[TEMPERATURA] =    DHT.temperature;
    holdingRegs[UMIDADE] =        DHT.humidity;
    holdingRegs[NIVEL] =          analogRead(sensorNv);
    holdingRegs[REFRIGERACAO] =   digitalRead(cooler);
    holdingRegs[BOMBEAMENTO] =    digitalRead(bomba);
    holdingRegs[ILUMINACAO] =     digitalRead(luz);

      if(intensidade > 25)
      {digitalWrite(luz, HIGH);  
      }
      else
      {digitalWrite(luz, LOW);
      }

      if(DHT.humidity > 80 || DHT.temperature > 30)
      {digitalWrite(cooler, LOW);
      }
      else
      {digitalWrite(cooler, HIGH);
      }
          
      if (nivel > 30){
      digitalWrite(bomba, LOW);
      }
      else{;
      digitalWrite(bomba, HIGH);
      }
} 
