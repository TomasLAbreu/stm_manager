# STM manager
Software for interfacing with STM32F767ZI-NUCLEO. Contains a reduced list of commands, regarding:
- User interface with micro controller ([doc](https://github.com/ESRGgroup9/stm_manager/blob/main/doc/LPI2_Guia_PL1_Interface_v4.5.pdf))
- Digital signal acquisition and filtering ([doc](https://github.com/ESRGgroup9/stm_manager/blob/main/doc/LPI2_Guia_PL2_AquisicaoDeDados_v8.6.pdf))

## Tools
- STM32CubeIDE 1.6.1
- PuTTY: interface STM via UART
- MATLAB R2019a: digital filter generation

## List of commands
For extended information, please read [command syntax doc](https://github.com/ESRGgroup9/stm_manager/blob/main/doc/command_syntax.pdf) (currently in Portuguese).
### List of utils:
|Util|    Util functionallity|
|---|---------------------|
|$  |    Insert last valid command|
|ESC|    Clears input line|

### List of valid commands:
|cmd|    Command designation|
|---|---------------------|
|?  |    Help information|
|VER|    Display firmware version|
|MR |    Memory Read|
|MW |    Memory Write|
|MI |    Memory Make Input|
|MO |    Memory Make Output|
|RD |    Read Digital Pin|
|WD |    Write Digital Pin|
|RA |    Read Analog|
|SP |    Sampling Period|
|AC |    Analog Channel|
|FN |    Filter ON|
|FF |    Filter OFF|
|S  |    Sample|
|ST |    Stop Sampling|
|WG |    Wave Generator|

### Relevant GPIO Pins
Remember STM32F767ZI GPIO pins mapping:\
![STM32F767ZI_GPIO_Pins](doc/GPIO_pins.png)

Note that, by default the following GPIO pins are assigned:
|Pin|    Function|
|---|---------------------|
|PA5|    DAC - WaveGen: wave generator|
|PA0|    ADC - Default Sampling ADC|
|PA4|    DAC - Filtered signal (if filter was on)|

--------
Done by:
- Tomás Abreu
- Diogo Fernandes

Engenharia Eletrónica Industrial e Computadores @ Universidade do Minho, 2022
