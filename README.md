# LSM6DS Lib for mannaNAUTA

Biblioteca Arduino para usar sensores da família LSM6DS no hardware mannaNAUTA
com ESP32-S3.

Esta biblioteca possui driver I2C próprio e foi preparada para a forma como o
CI LSM6DS é soldado na placa mannaNAUTA.

## Padrões da placa

Por padrão, a biblioteca inicializa o barramento I2C com:

- SDA: GPIO 5
- SCL: GPIO 4
- Endereço I2C: `0x6A`

Uso básico:

```cpp
imu.begin();
```

Esse comando já usa os pinos e o endereço padrão da placa mannaNAUTA.

## Orientação do CI na placa mannaNAUTA

Nas placas mannaNAUTA, o CI LSM6DS é soldado com uma orientação física diferente
do referencial esperado pelo usuário ao movimentar a placa.

Por isso, esta biblioteca já remapeia os eixos internamente para que as leituras
representem a orientação da placa mannaNAUTA, e não a orientação física original
do CI.

Com a correção padrão habilitada:

- inclinar a placa para frente gera leitura coerente com frente;
- inclinar a placa para trás gera leitura coerente com trás;
- inclinar a placa para direita gera leitura coerente com direita;
- inclinar a placa para esquerda gera leitura coerente com esquerda.

Esse remapeamento é aplicado nas leituras de acelerômetro e giroscópio retornadas
por:

```cpp
readAcceleration()
readGyroscope()
getEvent()
```

## Exemplo básico

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
  mannanauta_lsm6ds_vector_t accel;
  mannanauta_lsm6ds_vector_t gyro;
  mannanauta_lsm6ds_temperature_t temp;

  imu.getEvent(&accel, &gyro, &temp);

  Serial.print("Accel X: ");
  Serial.println(accel.x);

  delay(100);
}
```

## Pinos customizados

Se for necessário usar outros pinos de I2C:

```cpp
imu.begin(21, 22);
```

## Endereço I2C customizado

Se o sensor estiver no endereço `0x6B`:

```cpp
imu.begin(5, 4, 0x6B);
```

Também é possível passar explicitamente o barramento `Wire`:

```cpp
imu.begin(5, 4, 0x6B, &Wire);
```

## Variante explícita

O modo padrão tenta detectar automaticamente a variante do sensor. Se quiser
fixar uma variante:

```cpp
imu.begin(5, 4, 0x6A, &Wire, MANNANAUTA_LSM6DSOX);
```

## Desabilitar correção de orientação

Para a maioria dos usuários da placa mannaNAUTA, a correção deve permanecer
habilitada.

Se precisar ler os eixos físicos originais do CI LSM6DS, desabilite a correção:

```cpp
imu.setBoardOrientationCorrection(false);
```

Para verificar o estado atual:

```cpp
bool habilitada = imu.boardOrientationCorrectionEnabled();
```

## Exemplos incluídos

- `BasicRead`: leitura básica de acelerômetro, giroscópio e temperatura.
- `ConfigureRanges`: configuração de faixa e taxa de atualização.
- `CustomAddress`: uso de endereço I2C alternativo.
- `CustomPins`: uso de pinos I2C customizados.
- `Pedometer`: contador de passos.
- `AxisOrientationMatrix`: teste visual básico da orientação dos eixos na matriz
  de 25 LEDs.

Aviso: o exemplo `AxisOrientationMatrix` precisa da biblioteca
[mannaNAUTA_WS2812_Lib](https://github.com/wuigor/mannaNAUTA_WS2812_Lib)
para controlar os LEDs WS2812.
