#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <Wire.h>



#define DHTPIN A1
#define DHTTYPE DHT11
LiquidCrystal_I2C lcd(0x3F, 16, 2);


byte waterIcon[8] = {
  0b00100, 0b01010, 0b01010, 0b00100,
  0b01110, 0b11111, 0b01110, 0b00100
};

byte moistureIcon[8] = {
  0b00100, 0b00100, 0b01010, 0b01010,
  0b10001, 0b10001, 0b01110, 0b00100
};

byte tempIcon[8] = {
  0b00100, 0b01010, 0b10001, 0b10001,
  0b11111, 0b01110, 0b01110, 0b01110
};

byte humIcon[8] = {
  0b00000, 0b00100, 0b01110, 0b01110,
  0b01110, 0b00101, 0b01010, 0b01000
};

DHT dht(DHTPIN, DHTTYPE);
int moistureValue = 0;
int temperature = 0;
int humidity = 0;

void setup() {
  Serial.begin(9600);

  pinMode(7, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(A0, INPUT);



  lcd.init();
  lcd.backlight();


  lcd.createChar(0, waterIcon);
  lcd.createChar(1, moistureIcon);
  lcd.createChar(2, tempIcon);
  lcd.createChar(3, humIcon);

  lcd.clear();

  dht.begin();
  delay(1000);
}

void loop() {
  moistureValue = analogRead(A0);


  lcd.setCursor(0, 0);
  lcd.write(0);
  lcd.print(": ");

  if (moistureValue > 700) {
    lcd.print("On  ");
    digitalWrite(2, LOW);
  } else {
    lcd.print("Off ");
    digitalWrite(2, HIGH);
  }

  lcd.setCursor(8, 0);
  lcd.write(1);
  lcd.print(": ");

  if (moistureValue > 700) {
    lcd.print("Dry ");
  } else {
    lcd.print("Wet ");
  }




  delay(100);


  int temperature = (int)dht.readTemperature();
  int humidity = (int)dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(500);
    return;
  }


  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.print(": ");
  lcd.print(temperature);
  lcd.print((char)223);
  lcd.print("C ");

  lcd.setCursor(8, 1);
  lcd.write(3);
  lcd.print(": ");
  lcd.print(humidity);
  lcd.print("%");




  delay(100);

  Serial.print("m:");
  Serial.print(moistureValue);
  Serial.print(",t:");
  Serial.print(temperature);
  Serial.print(",h:");
  Serial.println(humidity);
  delay(100);
}
