#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>
#include <mannaNAUTA_WS2812.h>
#include <math.h>

MannaNAUTA_LSM6DS imu;
mannaNAUTA_WS2812 matrix = mannaNAUTA_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

const float TILT_THRESHOLD_DEG = 12.0;

const uint8_t ARROW_R = 0;
const uint8_t ARROW_G = 80;
const uint8_t ARROW_B = 20;

const uint8_t BACK_R = 80;
const uint8_t BACK_G = 20;
const uint8_t BACK_B = 0;

const uint8_t CENTER_R = 0;
const uint8_t CENTER_G = 30;
const uint8_t CENTER_B = 80;

uint8_t xyToLed(uint8_t x, uint8_t y) {
  return y * 5 + x;
}

void setXY(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  if (x < 5 && y < 5) {
    matrix.setLedColorData(xyToLed(x, y), r, g, b);
  }
}

void clearBuffer() {
  matrix.setAllLedsColorData(0);
}

void drawNeutral() {
  clearBuffer();
  setXY(1, 2, CENTER_R, CENTER_G, CENTER_B);
  setXY(2, 1, CENTER_R, CENTER_G, CENTER_B);
  setXY(2, 2, CENTER_R, CENTER_G + 30, CENTER_B);
  setXY(2, 3, CENTER_R, CENTER_G, CENTER_B);
  setXY(3, 2, CENTER_R, CENTER_G, CENTER_B);
  matrix.show();
}

void drawForward() {
  clearBuffer();
  setXY(2, 0, ARROW_R, ARROW_G, ARROW_B);
  setXY(1, 1, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 1, ARROW_R, ARROW_G, ARROW_B);
  setXY(3, 1, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 3, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 4, ARROW_R, ARROW_G, ARROW_B);
  matrix.show();
}

void drawBack() {
  clearBuffer();
  setXY(2, 0, BACK_R, BACK_G, BACK_B);
  setXY(2, 1, BACK_R, BACK_G, BACK_B);
  setXY(2, 2, BACK_R, BACK_G, BACK_B);
  setXY(1, 3, BACK_R, BACK_G, BACK_B);
  setXY(2, 3, BACK_R, BACK_G, BACK_B);
  setXY(3, 3, BACK_R, BACK_G, BACK_B);
  setXY(2, 4, BACK_R, BACK_G, BACK_B);
  matrix.show();
}

void drawRight() {
  clearBuffer();
  setXY(0, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(1, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(3, 1, ARROW_R, ARROW_G, ARROW_B);
  setXY(3, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(3, 3, ARROW_R, ARROW_G, ARROW_B);
  setXY(4, 2, ARROW_R, ARROW_G, ARROW_B);
  matrix.show();
}

void drawLeft() {
  clearBuffer();
  setXY(0, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(1, 1, ARROW_R, ARROW_G, ARROW_B);
  setXY(1, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(1, 3, ARROW_R, ARROW_G, ARROW_B);
  setXY(2, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(3, 2, ARROW_R, ARROW_G, ARROW_B);
  setXY(4, 2, ARROW_R, ARROW_G, ARROW_B);
  matrix.show();
}

void showDirection(float rollDeg, float pitchDeg) {
  float absRoll = fabs(rollDeg);
  float absPitch = fabs(pitchDeg);

  if (absRoll < TILT_THRESHOLD_DEG && absPitch < TILT_THRESHOLD_DEG) {
    drawNeutral();
    Serial.println("NEUTRO");
    return;
  }

  if (absPitch >= absRoll) {
    if (pitchDeg < 0) {
      drawForward();
      Serial.println("FRENTE");
    } else {
      drawBack();
      Serial.println("TRAS");
    }
  } else {
    if (rollDeg > 0) {
      drawRight();
      Serial.println("DIREITA");
    } else {
      drawLeft();
      Serial.println("ESQUERDA");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  matrix.begin();
  matrix.setBrightness(20);
  drawNeutral();

  Serial.println("mannaNAUTA LSM6DS - teste visual dos eixos");

  if (!imu.begin()) {
    Serial.println("LSM6DS nao encontrado em 0x6A. Tentando 0x6B...");

    if (!imu.begin(SDA_PIN, SCL_PIN, 0x6B, &Wire)) {
      Serial.println("LSM6DS nao encontrado.");
      matrix.setAllLedsColor(80, 0, 0);
      while (1) {
        delay(10);
      }
    }
  }

  Serial.print("Sensor detectado: ");
  Serial.println(imu.variantName());
}

void loop() {
  mannanauta_lsm6ds_vector_t accel;
  mannanauta_lsm6ds_vector_t gyro;
  mannanauta_lsm6ds_temperature_t temp;

  if (!imu.getEvent(&accel, &gyro, &temp)) {
    Serial.println("Falha na leitura do LSM6DS.");
    matrix.setAllLedsColor(80, 0, 0);
    delay(250);
    return;
  }

  float ax = accel.x;
  float ay = accel.y;
  float az = accel.z;

  float rollDeg = atan2(ay, az) * 180.0 / PI;
  float pitchDeg = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

  Serial.print("Roll: ");
  Serial.print(rollDeg, 1);
  Serial.print("  Pitch: ");
  Serial.print(pitchDeg, 1);
  Serial.print("  Direcao: ");

  showDirection(rollDeg, pitchDeg);

  delay(120);
}
