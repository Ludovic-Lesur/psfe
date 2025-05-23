# Description

The aim of the **ATXFox project** was to design a laboratory power supply from a standard ATX power supply, by adding some useful features such as specific connectors, voltage and current monitoring, IHM, etc...

Each voltage output of the ATX has its own front-end, formed by a **PSFE** and **TRCS** boards pair. The PSFE board is a generic power supply interface including control switches, standard connectors, LCD screen for voltage and current display, and Sigfox connectivity for optional remote monitoring. The TRCS is dedicated to current sensing with three ranges, driven by the PSFE board.

# Hardware

The boards were designed on **Circuit Maker V1.3**. Below is the list of hardware revisions:

| Hardware revision | Description | Status |
|:---:|:---:|:---:|
| [PSFE HW1.0](https://365.altium.com/files/C6DA5B00-C92D-11EB-A2F6-0A0ABF5AFC1B) | Initial version. | :white_check_mark: |
| [TRCS HW1.0](https://365.altium.com/files/C4EB9CA3-C92D-11EB-A2F6-0A0ABF5AFC1B) | Initial version. | :white_check_mark: |

# Embedded software

## Environment

As of version `sw1.0.2` the embedded software is developed under **Eclipse IDE** version 2024-09 (4.33.0) and **GNU MCU** plugin. The `script` folder contains Eclipse run/debug configuration files and **JLink** scripts to flash the MCU.

> [!WARNING]
> To compile any version under `sw4.0`, the `git_version.sh` script must be patched when `sscanf` function is called: the `SW` prefix must be replaced by `sw` since Git tags have been renamed in this way.

## Target

The PSFE board is based on the **STM32L031G6U6** microcontroller of the STMicroelectronics L0 family. Each hardware revision has a corresponding **build configuration** in the Eclipse project, which sets up the code for the selected board version.

## Structure

The project is organized as follow:

* `drivers` :
    * `device` : MCU **startup** code and **linker** script.
    * `registers` : MCU **registers** address definition.
    * `peripherals` : internal MCU **peripherals** drivers.
    * `components` : external **components** drivers.
    * `utils` : **utility** functions.
* `middleware` :
    * `analog` : High level **analog measurements** driver.
    * `hmi` : **HMI** driver.
    * `serial` : **Serial monitoring** driver.
    * `sigfox` : **Sigfox monitoring** driver.
* `application` : Main **application**.
