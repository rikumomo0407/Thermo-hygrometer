#include "DHT.h"

#define DHTPIN 15
#define SW_PIN = 2;
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

bool setting_state = true; // True : Show temperature, False: Show humidity
boolean data[10][7] = { // LEDレイアウトの配列を定義
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

int i;

void setup() {
  Serial.begin(9600);
  for (i = 3; i <= 14; i++){
      pinMode(i, OUTPUT);              // D3~D14を出力に
  }
  pinMode(SW_PIN, INPUT_PULLUP);
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
    if (i == 3) {
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

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.print("Failed to read from DHT sensor!");
    return;
  }
  Serial.print(temperature);
  Serial.print("\n");
  for (i = 0; i < 400; i++) {
    Display(temperature, humidity);
  }
  // if (digitalRead(SW_PIN) == FALLING) {
  //   Serial.print("Change value.\n");
  //   setting_state = !setting_state;
  // }
}
