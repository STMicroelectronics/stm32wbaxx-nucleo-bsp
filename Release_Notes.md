

# Release Notes for
# <mark>STM32WBAxx_Nucleo</mark>
Copyright &copy; 2022-2025 STMicroelectronics\
    
[![ST logo](_htmresc/st_logo_2020.png)](https://www.st.com)

# Purpose

This driver provides a set of functions to manage:

- LEDs
- Push-buttons
- COM port
- QuadSPI NOR Flash on NUCLEO-WBA25CE

available on STM32WBAxx_Nucleo Kit from STMicroelectronics


# Update History

<label for="collapse-section8" aria-hidden="true">V1.2.1 / 03-June-2026</label>
<div>

## Main Changes

### Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA2x**, **STM32WBA5x** and **STM32WBA6x** devices

## Contents

### Official Release of board drivers for **NUCLEO-WBA25CE**, **NUCLEO-WBA55CG** and **NUCLEO-WBA65RI** boards

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - NUCLEO-WBA65RI (MB1801-USB-D01/MB2130-WBA65RI-A01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)
- Add support for **NUCLEO-WBA25CE** board (MB1801-USB-D01/MB2293-WBA2-A01)
- Added new API for Push-Button 'Rising Edge' IT setting on BSP 

## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section7" aria-hidden="true">__V1.2.0 / 20-January-2026__</label>
<div>

## Main Changes

### Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA2x**, **STM32WBA5x** and **STM32WBA6x** devices

## Contents

### Official Release of board drivers for **NUCLEO-WBA25CE**, **NUCLEO-WBA55CG** and **NUCLEO-WBA65RI** boards

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - NUCLEO-WBA65RI (MB1801-USB-D01/MB2130-WBA65RI-A01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)
- Add support for **NUCLEO-WBA25CE** board (MB1801-USB-D01/MB2293-WBA2-A01)

## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section6" aria-hidden="true">__V1.1.1 / 07-October-2025__</label>
<div>

## Main Changes

### Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA5x** and **STM32WBA6x** devices

## Contents

### Official Release of board drivers for **NUCLEO-WBA55CG** and **NUCLEO-WBA65RI** boards

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - NUCLEO-WBA65RI (MB1801-USB-D01/MB2130-WBA65RI-A01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)


- **BSP Drivers** updates
  - Modify the LED pin configuration with no-pull to reduce extra power consumption


## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section5" aria-hidden="true">__V1.1.0 / 07-February-2025__</label>
<div>

## Main Changes

###  Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA5x** and **STM32WBA6x** devices

## Contents

###  Official Release of board drivers for **NUCLEO-WBA55CG** and **NUCLEO-WBA65RI** boards

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - **NUCLEO-WBA65RI** (MB1801-USB-D01/MB2130-WBA65RI-A01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)


## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section4" aria-hidden="true">__V1.0.3 / 22-October-2024__</label>
<div>

## Main Changes

###  Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA55xx** devices

## Contents

###  Official Release of board drivers for **NUCLEO-WBA55CG** board

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)


- **BSP Drivers** updates
  - No more support of deprecated board NUCLEO-WBA52CG (MB1801A-02/MB1863-WBA52CGA-A03)


## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section3" aria-hidden="true">__V1.0.2 / 07-February-2024__</label>
<div>

## Main Changes

###  Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA52xx** and **STM32WBA55xx** devices

## Contents

###  Official Release of board drivers for **NUCLEO-WBA52CG** board and **NUCLEO-WBA55CG** board

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA52CG (MB1801A-02/MB1863-WBA52CGA-A03)
  - NUCLEO-WBA55CG (MB1801D-01/MB1803-WBA55CGA-B01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)


- **BSP Drivers** updates
  - Enable internal pull-up for user push-buttons


## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section2" aria-hidden="true">__V1.0.1 / 02-November-2023__</label>
<div>

## Main Changes

###  Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA52xx** and **STM32WBA55xx** devices

## Contents

###  Official Release of board drivers for **NUCLEO-WBA52CG** board and **NUCLEO-WBA55CG** board

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA52CG (MB1801A-01/MB1863-WBA52CGA-A03)
  - NUCLEO-WBA55CG (MB1801A-01/MB1803-WBA55CGA-B01)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)


- **BSP Drivers** updates
  - Fix printf() output redirection for MDK-ARM


## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>

<label for="collapse-section1" aria-hidden="true">__V1.0.0 / 08-February-2023__</label>
<div>

## Main Changes

###  First Official Release of **STM32CubeWBA** Firmware package supporting **STM32WBA52xx** devices

## Contents

### First Official Release of board drivers for **NUCLEO-WBA52CG** board

- **BSP Drivers** are available for the boards :
  - NUCLEO-WBA52CG (MB1801A-01/MB1863-WBA52CGA-A03)
  - in line with STM32Cube BSP drivers development guidelines (UM2298 - revision 2)

## Known Limitations

- None

## Dependencies

- None

## Notes

- stm32wbaxx_nucleo_conf_template.h file must be copied in user application as
stm32wbaxx_nucleo_conf.h with optional configuration update


</div>


For complete documentation on STM32WBAxx,
visit: [www.st.com/stm32wba](http://www.st.com/stm32wba)

*This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.*
<abbr title="Based on template cx566953 version 2.0">Info</abbr>