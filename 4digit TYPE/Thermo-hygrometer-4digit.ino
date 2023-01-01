#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int i;
float temperature;
float humidity;
bool chattering = false;

bool setting_state = true; // True : Show temperature, False: Show humidity
boolean data[10][7] = { // Define LED layout
//{g,f,e,d,c,b,a}
  {0,1,1,1,1,1,1},  // 0
  {0,0,0,0,1,1,0},  // 1
  {1,0,1,1,0,1,1},  // 2
  {1,0,0,1,1,1,1},  // 3
  {1,1,0,0,1,1,0},  // 4
  {1,1,0,1,1,0,1},  // 5
  {1,1,1,1,1,0,1},  // 6
  {0,1,0,0,1,1,1},  // 7
  {1,1,1,1,1,1,1},  // 8
  {1,1,0,1,1,1,1}   // 9
};

unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
long interval = 2000;

void setup() {
  Serial.begin(9600);
  for (i = 3; i <= 17; i++){
    pinMode(i, OUTPUT);
  }
  pinMode(2, INPUT_PULLUP);
  digitalWrite(16, OUTPUT);
  attachInterrupt(0, ChangeLED, FALLING);
  dht.begin();
}

void Display(float temp, float humid) {
  int i, j;
  for (i = 1; i <= 4; i++) {
    for (j = 3; j <= 6; j++){
      if (j == i + 2) {
        digitalWrite(j, LOW);
      } else {
        digitalWrite(j, HIGH);
      }
    }
    int output = GetDigit(i, temp, humid);
    if (i == 2) {
      digitalWrite(7, HIGH);
    } else {
      digitalWrite(7, LOW);
    }
    for (j = 8; j <= 14; j++) {
      digitalWrite(j, data[output][j - 8]);
    }
    delay(5);
  }
}

int GetDigit(int digit, float temp, float humid) {
  int i, data , digit_num = 10000;
  if (setting_state == false) {
    data = humid*100;
  } else {
    data = temp*100;
  }
  for (i = 1; i < digit; i++){
    digit_num /= 10;
    data %= digit_num;
  }
  return data/(digit_num/10);
}

void ChangeLED(void) {
  if (chattering) {
    Serial.print("Change LED");
    Serial.print("\n");
    setting_state = !setting_state;
    digitalWrite((setting_state == true) ? 16 : 17, HIGH);
    digitalWrite((setting_state == true) ? 17 : 16, LOW);   
    chattering = false;
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval) {   
    previousMillis = currentMillis;
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (isnan(temperature) || isnan(humidity)) {
      Serial.print("Failed to read from DHT sensor!");
      return;
    }
    setting_state = !setting_state;
    digitalWrite((setting_state == true) ? 16 : 17, HIGH);
    digitalWrite((setting_state == true) ? 17 : 16, LOW);  
    chattering = true;
  }
  Display(temperature, humidity);
}
