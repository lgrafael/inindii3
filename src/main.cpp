// This library works only with the following circuit topology
// Vcc---NTC---ADC---SERIES_RESISTOR---GND
#include <Arduino.h>
#include <math.h>
#define ADC_RESOLUTION 1023
#define TEMPERATURENOMINAL 25
#define NTC_PIN A5


double getTempTermistorNTCBeta(const uint16_t analogValue, const uint16_t serialResistance, const uint16_t bCoefficient, const uint16_t nominalResistance)
{
  // convert the value to resistance
  const float resistance = (serialResistance / ((float)analogValue)) * ADC_RESOLUTION - serialResistance;
  const float temp = 1.0 / ((1.0 / (TEMPERATURENOMINAL + 273.15)) + (1.0 / bCoefficient) * log(resistance / nominalResistance)); // 1.0/( (1/To)+(1/B)*ln(R/Ro) )
  return (temp - 273.15);
}

double compensTempTermoparTipoT(const double temperatura)
{

  double E = 0.0;

  double c[] = {0.0000000000, 
              3.8748106364E-2, 
              3.3292227880E-5,
              2.0618243404E-7,
              -2.1882256846E-9, 
              1.0996880928E-11,
              -3.0815758772E-14, 
              4.5479135290E-17, 
              -2.7512901673E-20,
              0,
              0,
  };

  for (int i = 1; i < 11; i++)
  {
    E = E + c[i]*pow(temperatura,i); 
  }
 return E;
}

double tempTermoparTipoT(const double tensao)
{
  double Temp = 0;
  double c[] = {0.0000000000,
              2.5928000E1, 
              -7.6029610E-1,
              4.6377910E-2,
              -2.1882256846E-3, 
              6.0481440E-5,
              -7.2934220E-7, 
  };

  for (int i = 1; i < 7; i++)
  {
    Temp = Temp + c[i] * pow(tensao, i);
  }
  
  return Temp;
}

// 0.391 - 10
// 4.279 - 100

void setup()
{
  Serial.begin(115200); // INICIALIZA A SERIAL
  delay(1000);          // INTERVALO DE 1 SEGUNDO
}


void loop()
{
  if(Serial.available() > 0){

    uint16_t adc = analogRead(NTC_PIN);
    float temperature1 = getTempTermistorNTCBeta(adc,    // Analog Value
                                                10000,  // Nominal resistance at 25 ºC
                                                3455,   // thermistor's beta coefficient
                                                10000); // Value of the series resistor
    Serial.print(">Temp NTC: ");                        // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(temperature1);                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA

    double tensaomV = compensTempTermoparTipoT(temperature1);
    Serial.print(" e Tensao (mV): ");
    Serial.println(tensaomV);

    // Ler da serial e chamar funçaõ que calcula em mV
    double tensaoSerial = Serial.parseFloat();

    

    // Informa a tensao de junta e a tensao de leitura do termopar 
    double temperaturaTermoPar = tempTermoparTipoT(tensaoSerial + tensaomV);

    Serial.print(">Temp do TermoPar: ");                        // IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(temperaturaTermoPar);                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
    Serial.print(" e Tensao serial (mV): ");
    Serial.println(tensaoSerial);
    delay(1000); // INTERVALO DE 1 SEGUNDO
  }
}
