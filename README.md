# CK05 Control box firmware

This project aims to provide a robust and efficient solution for managing the various subsystems of a Formula SAE race car. Leveraging the STM32 microcontroller and AzureRTOS framework, this firmware ensures precise control and real-time monitoring and logging capabilities crucial for competitive racing environments.

          ▒▄,
    ▀▓▄,   ▀▓▓▓▒▄,                                    ¬╖,
     ╙▓▓▓▓▓▒▄▄▄▄,`╙^^        ,,,╓╓╓╖╖╖╓╓,,,,             ▀▓▓▄▄,
       ▀▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▀▀▀▀^`^`       `└"^"¬─-    ▓▓▓▓▓▓▓▒╖
        ▀▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▀▀`                           ╓▄▓▓▓▓▓▓▓▓▓▓▓▓▒╖
         ╙▓▓▓▓▓▓▓▓▓▓▓▀╙                ╙▀█▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▄,
           ▀▓▓▓▓▓▓▀`                         `╙▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀╙^^^"▀▀▀▀▀▓▓▓▓▄,
            ▀▓▓▓▀                                                           `▀▀▄
             ╙▀                                                                  `

## NCKU Formula Racing

-📍Tainan, Taiwan(R.O.C.)\
-📧nckufs.club@gs.ncku.edu.tw\
-🔗[Facebook](https://www.facebook.com/nckuformularacing/) |
[Instagram](https://www.instagram.com/nckuformularacing/) |
[Youtube](https://www.youtube.com/@nckuformularacing4968/)

## [CAN Bus Address Summary](https://hackmd.io/@nckufs/control-fw-can-bus-table)

## Project's File Structure

### AZURE_RTOS

Initialization files for Azure RTOS, laying the groundwork for real-time
operating system functionalities crucial for managing the car's operations.

### Core

Source files primarily responsible for initializing STM32, featuring
auto-generated code to establish the foundational aspects of the car's control
systems.

### Drivers

STM32 BSP (Board Support Package) drivers and custom libraries tailored to the
specific requirements of the FSAE car.

Configs: Configuration loader aiding in firmware setup, enabling dynamic
adjustments and optimizations for different racing scenarios.

Sensors: Repository for sensor-related functionalities, encompassing modules for
interfacing with various sensors crucial for monitoring the car's performance.
ADC: Analog-to-Digital Converter functionalities for precise data acquisition
from analog sensors. IMU: Inertial Measurement Unit functionalities for accurate
motion tracking and vehicle dynamics analysis. Traction: Components related to
traction control mechanisms, optimizing the car's traction under varying driving
conditions. Inverter: Modules responsible for managing inverters, enhancing
power delivery efficiency and control.

### FileX

Integration of FileX, a high-performance file system optimized for deeply
embedded applications, facilitating data logging and storage functionalities
vital for post-race analysis and system diagnostics.

### Tasks

Task-specific files organizing code based on operational tasks within the FSAE
car, facilitating modularity and scalability in system development and
maintenance.

This meticulously structured approach ensures a robust foundation for developing
an embedded system tailored specifically for the unique requirements and
challenges posed by the high-performance environment of an FSAE car. From
low-level hardware initialization to high-level task management and control
algorithms, each directory serves a distinct purpose in enabling efficient
development, deployment, and maintenance of the embedded system driving the FSAE
car to success on the track.

## Requirements

- [make](https://www.gnu.org/software/make/)
- [cmake](https://cmake.org/)
- [AArch32 bare-metal target (arm-none-eabi)](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [ninja](https://ninja-build.org/)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [pyocd](https://pyocd.io/)
- [clang-format >= 18](https://clang.llvm.org/docs/ClangFormat.html)

## Build

```
$ make
```

The compiled binary file will be `build/fsae-2024.bin` or `build/fsae-2024.elf`

## Debug

```
$ make debug
```

## Flash

```
$ make flash
```

## Monitor

This firmware use
[Segger RTT](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/)
library with default CB (control block) address `0x20000000`.

```
$ make monitor
```

## License

```
Copyright 2024 NCKU Formula Racing

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
