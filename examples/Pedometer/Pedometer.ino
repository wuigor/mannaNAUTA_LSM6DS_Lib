#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  Serial.println("mannaNAUTA LSM6DS - pedometer");

  if (!imu.begin()) {
    Serial.println("Failed to find the LSM6DS.");
    while (1) {
      delay(10);
    }
  }

  Serial.print("Detected sensor: ");
  Serial.println(imu.variantName());

  // Configure the accelerometer for 2 g and a 26 Hz output data rate.
  imu.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  imu.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);
  imu.setAccelDataRate(LSM6DS_RATE_26_HZ);
  imu.setGyroDataRate(LSM6DS_RATE_26_HZ);

  // Route step detection to INT1 if the pin is connected on the hardware.
  imu.configInt1(false, false, false, true);

  imu.resetPedometer();
  imu.enablePedometer(true);

  Serial.println("Pedometer enabled.");
}

void loop() {
  Serial.print("Steps: ");
  Serial.println(imu.readPedometer());

  delay(100);
}
