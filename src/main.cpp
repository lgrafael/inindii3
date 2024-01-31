// This library works only with the following circuit topology
// Vcc---NTC---ADC---SERIES_RESISTOR---GND
#include <Arduino.h>
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
}

double tempTermoparTipoT(const double miliVolts)
{
}

void setup()
{
  Serial.begin(115200); // INICIALIZA A SERIAL
  delay(1000);          // INTERVALO DE 1 SEGUNDO
}
void loop()
{
  uint16_t adc = analogRead(NTC_PIN);
  float temperature1 = getTempTermistorNTCBeta(adc,    // Analog Value
                                               10000,  // Nominal resistance at 25 ºC
                                               3455,   // thermistor's beta coefficient
                                               10000); // Value of the series resistor
  Serial.print(">Temp Beta: ");                        // IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.println(temperature1);                        // IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA

  delay(1000); // INTERVALO DE 1 SEGUNDO
}
