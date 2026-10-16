#include <OneWire.h>
#include <DallasTemperature.h>
#include <TM1637Display.h>

const int Relay = 6;
const int CLK = 8; 
const int DIO = 9; 
const int ONE_WIRE_BUS = 4;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
TM1637Display display(CLK, DIO);

unsigned long tiempoAnteriorRele = 0;
unsigned long tiempoAnteriorDisplay = 0;
bool estadoRele = false;

void setup() {
  Serial.begin(115200);
  pinMode(Relay, OUTPUT);
  sensors.begin();
  display.setBrightness(0x0f);
  
  uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
  display.setSegments(data);
  delay(2000);
  display.clear();
  delay(1000);
}

void loop() {
  unsigned long tiempoActual = millis();

  if (tiempoActual - tiempoAnteriorRele >= 1000) {
    tiempoAnteriorRele = tiempoActual;
    estadoRele = !estadoRele;
    digitalWrite(Relay, estadoRele);
  }

  if (tiempoActual - tiempoAnteriorDisplay >= 1000) {
    tiempoAnteriorDisplay = tiempoActual;
    
    sensors.requestTemperatures();
    float temperatura = sensors.getTempCByIndex(0);
    
    if (temperatura != DEVICE_DISCONNECTED_C && temperatura > -50) {
      int tempEntera = (int)temperatura;
      display.showNumberDec(tempEntera, false);
    } else {
      uint8_t errorData[] = { 0x40, 0x40, 0x40, 0x40 };
      display.setSegments(errorData);
    }
  }
}
