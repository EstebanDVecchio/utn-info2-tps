#include "DHT.h"
#include <LiquidCrystal_I2C.h> 
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int botonArriba = 2;  // Avanzar pestaña o subir en historial
const int botonAbajo = 3;   // Retroceder pestaña o bajar en historial
const int fotoResistor = 4; // Sensor de luz

// Creamos una estructura que agrupa los tres datos climáticos
struct Medicion {
  float temperatura;
  float humedad;
  int luz;
};

const int MAX_MEDICIONES = 12;             // CORRECCIÓN: Agregado 'const'
Medicion historico[MAX_MEDICIONES];        // CORRECCIÓN: Eliminado el 'int' previo

int indiceRegistro = 0;                    // Indica en qué posición vamos a guardar
int totalRegistros = 0;                    // 12 registros

int pestanaActual = 1;
int totalPestanas = 3;       // 1: Tiempo real T/H, 2: Tiempo real Luz, 3: Histórico
int indiceHistorial = 0;     // Indica qué registro histórico estamos viendo en la pestaña 3

unsigned long tiempoAnteriorRegistro = 0;
const unsigned long INTERVALO_REGISTRO = 5000; // Registro cada 5 segundos para simulación

void setup() {
  lcd.init();
  lcd.backlight();
  dht.begin(); 

  pinMode(botonArriba, INPUT_PULLUP);
  pinMode(botonAbajo, INPUT_PULLUP);
  pinMode(fotoResistor, INPUT);

  mostrarPestana();
}

void loop() {
  // 1. Tarea de Registro Periódico (cada 5 segundos)
  if (millis() - tiempoAnteriorRegistro >= INTERVALO_REGISTRO) {
    tiempoAnteriorRegistro = millis();
    registrarDatos();
    
    // Si estamos viendo datos en tiempo real, refrescamos la pantalla
    if (pestanaActual == 1 || pestanaActual == 2) {
      mostrarPestana(); 
    }
  }

  // 2. Tarea de Interfaz de Usuario (Lectura de Botones)
  
  // Lógica del Botón Arriba
  if (digitalRead(botonArriba) == LOW) {
    if (pestanaActual != 3) {
      pestanaActual = pestanaActual + 1;
      if (pestanaActual > totalPestanas) pestanaActual = 1;
    } else {
      if (totalRegistros > 0) {
        indiceHistorial = indiceHistorial + 1;
        if (indiceHistorial >= totalRegistros) indiceHistorial = 0; 
      }
    }
    mostrarPestana();
    delay(250); 
  }

  // Lógica del Botón Abajo
  if (digitalRead(botonAbajo) == LOW) {
    if (pestanaActual != 3) {
      // Si NO estamos en el histórico, retrocedemos de pestaña
      pestanaActual = pestanaActual - 1;
      if (pestanaActual < 1) pestanaActual = totalPestanas;
    } else {
      // Si estamos en el histórico, navegamos por los registros guardados (bajamos)
      if (totalRegistros > 0) {
        indiceHistorial = indiceHistorial - 1;
        if (indiceHistorial < 0) indiceHistorial = totalRegistros - 1;
      }
    }
    mostrarPestana();
    delay(250); 
  }
}

// Función que toma los datos y los guarda en la estructura
void registrarDatos() {
  // Leemos los sensores
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int l = digitalRead(fotoResistor);

  // Guardamos en la posición actual del búfer
  historico[indiceRegistro].temperatura = t;
  historico[indiceRegistro].humedad = h;
  historico[indiceRegistro].luz = l;

  // Actualizamos los contadores del búfer circular
  indiceRegistro = indiceRegistro + 1;
  
  // Si llegamos a 12, volvemos a la posición 0 (sobreescritura)
  if (indiceRegistro >= MAX_MEDICIONES) {
    indiceRegistro = 0;
  }
  
  // Mantenemos cuenta de cuántos registros tenemos hasta llenar el arreglo
  if (totalRegistros < MAX_MEDICIONES) {
    totalRegistros = totalRegistros + 1;
  }
}

void mostrarPestana() {
  lcd.clear(); // limpia toda la pantalla

  switch (pestanaActual) {
    case 1: // Tiempo Real: Temp y Humedad
      lcd.setCursor(0, 0);
      lcd.print("Tiempo Real");
      lcd.setCursor(0, 1);
      lcd.print(dht.readTemperature(), 1); // 1 decimal
      lcd.print("C  ");
      lcd.print(dht.readHumidity(), 1);
      lcd.print("%");
      break;
      
    case 2: // Tiempo Real: Luz
      lcd.setCursor(0, 0);
      lcd.print("T. Real - Luz");
      lcd.setCursor(0, 1);
      if (digitalRead(fotoResistor) == HIGH) {
        lcd.print("Alta");
      } else {
        lcd.print("Baja");
      }
      break;
      
    case 3: // Histórico
      lcd.setCursor(0, 0);
      lcd.print("Historial ");
      lcd.print(indiceHistorial + 1); // Mostramos el número 1 a 12
      
      lcd.setCursor(0, 1);
      if (totalRegistros == 0) {
        lcd.print("Sin datos aun");
      } else {
        // Mostramos un resumen del registro almacenado en esa posición
        lcd.print(historico[indiceHistorial].temperatura, 1);
        lcd.print("C ");
        lcd.print(historico[indiceHistorial].humedad, 1);
        lcd.print("% ");
        lcd.print(historico[indiceHistorial].luz);
      }
      break;
  }
}