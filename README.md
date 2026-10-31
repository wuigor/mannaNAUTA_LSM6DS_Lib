# mannaNAUTA LSM6DS Library

Arduino library for using LSM6DS-family inertial sensors with the ESP32-S3-based
mannaNAUTA board.

The library provides a standalone I2C driver and accounts for the physical
orientation of the LSM6DS IC on the mannaNAUTA board.

## Features

- Accelerometer, gyroscope, and temperature readings
- Automatic sensor variant detection
- Configurable measurement ranges and output data rates
- Pedometer, wake-up, interrupt, and high-pass filter support
- Custom I2C pins, address, and bus
- Board-oriented axis correction, enabled by default

## Supported sensors

- LSM6DSOX
- LSM6DSO32
- LSM6DS3
- LSM6DS33
- LSM6DS3TR-C
- LSM6DSL
- ISM330DHCX

## Installation

Install **mannaNAUTA LSM6DS** from the Arduino IDE Library Manager. To install
it manually, download this repository and place it in your Arduino `libraries`
folder.

## Board defaults

By default, the library initializes the I2C bus with:

- SDA: GPIO 5
- SCL: GPIO 4
- I2C address: `0x6A`

Basic initialization:

```cpp
imu.begin();
```

This call uses the default mannaNAUTA board pins and address.

## Sensor orientation on the mannaNAUTA board

The LSM6DS IC is mounted in an orientation that differs from the natural frame
of reference used when moving the mannaNAUTA board.

The library therefore remaps the axes internally so readings represent the
board orientation instead of the IC's original physical orientation.

With the default correction enabled:

- tilting the board forward produces a forward-oriented reading;
- tilting it backward produces a backward-oriented reading;
- tilting it right produces a right-oriented reading;
- tilting it left produces a left-oriented reading.

This remapping is applied to accelerometer and gyroscope readings returned by:

```cpp
readAcceleration()
readGyroscope()
getEvent()
```

## Basic example

```cpp
#include <Wire.h>
#include <MannaNAUTA_LSM6DS.h>

MannaNAUTA_LSM6DS imu;

void setup() {
  Serial.begin(115200);

  if (!imu.begin()) {
    Serial.println("LSM6DS not found");
    while (1) delay(10);
  }
}

void loop() {
  mannanauta_lsm6ds_vector_t accel;
  mannanauta_lsm6ds_vector_t gyro;
  mannanauta_lsm6ds_temperature_t temp;

  imu.getEvent(&accel, &gyro, &temp);

  Serial.print("Accel X: ");
  Serial.println(accel.x);

  delay(100);
}
```

## Custom I2C pins

To use different I2C pins:

```cpp
imu.begin(21, 22);
```

## Custom I2C address

If the sensor uses address `0x6B`:

```cpp
imu.begin(5, 4, 0x6B);
```

You can also pass the `Wire` bus explicitly:

```cpp
imu.begin(5, 4, 0x6B, &Wire);
```

## Explicit sensor variant

The default mode attempts to detect the sensor variant automatically. To select
a variant explicitly:

```cpp
imu.begin(5, 4, 0x6A, &Wire, MANNANAUTA_LSM6DSOX);
```

## Disabling board orientation correction

For most mannaNAUTA board users, the correction should remain enabled.

To read the original physical axes of the LSM6DS IC, disable the correction:

```cpp
imu.setBoardOrientationCorrection(false);
```

To check its current state:

```cpp
bool enabled = imu.boardOrientationCorrectionEnabled();
```

## Included examples

- `BasicRead`: reads acceleration, angular velocity, and temperature.
- `ConfigureRanges`: configures measurement ranges and output data rates.
- `CustomAddress`: uses an alternative I2C address.
- `CustomPins`: uses custom I2C pins.
- `Pedometer`: reads the step counter.
- `AxisOrientationMatrix`: visually tests axis orientation on the 25-LED matrix.

The `AxisOrientationMatrix` example requires the
[mannaNAUTA_WS2812_Lib](https://github.com/wuigor/mannaNAUTA_WS2812_Lib)
library to control the WS2812 LEDs.

## License

Released under the MIT License. See [LICENSE](LICENSE) for details.
