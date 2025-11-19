#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// O potenciometro simula BPM uma vez que n possuimos sensor de pulso
const int SENSOR_PIN = A0;

// LED na protoboard (pino 6)
const int LED_PIN = 6;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);

  Serial.println("Sistema de Estresse Humano Iniciado...");
}
// Calculo de estresse em pontos
int calcularEstresse(float temperatura, float umidade, int resp) {

  int pontos = 0;

  if (temperatura > 39) pontos += 50;
  else if (temperatura > 37.5) pontos += 30;
  else if (temperatura > 36.0) pontos += 10;
  else if (temperatura < 32) pontos += 40;

  if (umidade > 80) pontos += 40;
  else if (umidade > 60) pontos += 20;
  else if (umidade > 40) pontos += 10;

  if (resp > 800) pontos += 40;
  else if (resp > 600) pontos += 20;  
  else if (resp > 400) pontos += 10;

  return pontos;
}

void loop() {
  // Loop pricipal
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();
  int resp = analogRead(SENSOR_PIN);

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler DHT22!");
    delay(1000);
    return;
  }

  int pontos = calcularEstresse(temperatura, umidade, resp);

  String status;
  int intervaloPiscar = 0;
  // Condições
  if (pontos < 20) {
    status = "Relaxado 😌";
    intervaloPiscar = 1000;
  }
  else if (pontos < 40) {
    status = "Atento 🤨";
    intervaloPiscar = 700;
  }
  else if (pontos < 60) {
    status = "Alerta 😟";
    intervaloPiscar = 400;
  }
  else if (pontos < 90) {
    status = "Estressado 😣";
    intervaloPiscar = 200;
  }
  else {
    status = "Crítico ⚠️";
    intervaloPiscar = 80;
  }

  // LED pulsando conforme o estresse
  digitalWrite(LED_PIN, HIGH);
  delay(intervaloPiscar);
  digitalWrite(LED_PIN, LOW);
  delay(intervaloPiscar);

  // Exibir
  Serial.println("===== LEITURAS =====");
  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.println(" %");
  Serial.print("Bpm: ");
  Serial.print(A0);
  Serial.println(resp);
  Serial.print("Índice de Estresse: ");
  Serial.println(pontos);
  Serial.print("Estado: ");
  Serial.println(status);
  Serial.println("---------------------");
}

//SUGESTÃO 
// Ao rodar o projeto ajustar:
//  DHT22 
//  Temp ~= 36 
//  Umidade ~= 25
