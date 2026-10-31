#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("mannaNAUTA LSM6DS - custom I2C address");

  if (!imu.begin(SDA_PIN, SCL_PIN, 0x6B)) {
    Serial.println("Failed to find the LSM6DS at 0x6B.");
    while (1) {
      delay(10);
    }
  }

  Serial.print("Detected sensor: ");
  Serial.println(imu.variantName());
}

void loop() {
  mannanauta_lsm6ds_vector_t accel;
  mannanauta_lsm6ds_vector_t gyro;
  mannanauta_lsm6ds_temperature_t temp;

  imu.getEvent(&accel, &gyro, &temp);

  Serial.print("X: ");
  Serial.print(accel.x);
  Serial.print(" Y: ");
  Serial.print(accel.y);
  Serial.print(" Z: ");
  Serial.print(accel.z);
  Serial.println(" m/s^2");

  delay(500);
}
