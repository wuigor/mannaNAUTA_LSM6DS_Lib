#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

#define MY_SDA_PIN 21
#define MY_SCL_PIN 22

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("mannaNAUTA LSM6DS - pinos customizados");

  if (!imu.begin(MY_SDA_PIN, MY_SCL_PIN)) {
    Serial.println("Falha ao encontrar o LSM6DS nos pinos customizados.");
    while (1) {
      delay(10);
    }
  }

  Serial.print("Sensor detectado: ");
  Serial.println(imu.variantName());
}

void loop() {
  float ax;
  float ay;
  float az;
  float gx;
  float gy;
  float gz;

  if (imu.accelerationAvailable()) {
    imu.readAcceleration(ax, ay, az);

    Serial.print("Aceleracao g: ");
    Serial.print(ax);
    Serial.print(", ");
    Serial.print(ay);
    Serial.print(", ");
    Serial.println(az);
  }

  if (imu.gyroscopeAvailable()) {
    imu.readGyroscope(gx, gy, gz);

    Serial.print("Giroscopio dps: ");
    Serial.print(gx);
    Serial.print(", ");
    Serial.print(gy);
    Serial.print(", ");
    Serial.println(gz);
  }

  delay(250);
}
