# LSM6DS Lib for mannaNAUTA

Biblioteca Arduino para usar sensores da família LSM6DS no hardware mannaNAUTA com ESP32-S3.

Esta biblioteca possui driver I2C próprio.

Por padrão a biblioteca inicializa o barramento I2C com:

- SDA: GPIO 5
- SCL: GPIO 4
- Endereco I2C: `0x6A`

## Uso basico

```cpp
#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);

  if (!imu.begin()) {
    Serial.println("LSM6DS não encontrado");
    while (1) delay(10);
  }
}

void loop() {
  mannanauta_lsm6ds_vector_t accel, gyro;
  mannanauta_lsm6ds_temperature_t temp;
  imu.getEvent(&accel, &gyro, &temp);

  Serial.println(accel.x);
  delay(100);
}
```

## Pinos customizados

```cpp
imu.begin(21, 22);
```

## Endereco I2C customizado

```cpp
imu.begin(5, 4, 0x6B);
```

## Variante explicita

O modo padrão tenta detectar automaticamente a variante do sensor.
Se quiser fixar a variante:

```cpp
imu.begin(5, 4, 0x6A, &Wire, MANNANAUTA_LSM6DSOX);
```
