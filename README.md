# stm_manager
Basic software for interfacing with STM32F767ZI-NUCLEO

### Relevant GPIO Pins
Note that, by default the following GPIO pins are assigned:
|Pin|    Function|
|---|---------------------|
|PA5|    DAC - WaveGen: wave generator|
|PA0|    ADC - Default Sampling ADC|
|PA4|    DAC - Filtered (if filter was on) signal|

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

--------
Done by:
- Tomás Abreu
- Diogo Fernandes

Engenharia Eletrónica Industrial e Computadores @ Universidade do Minho, 2022
