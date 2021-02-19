# ADCS Equipment Handler
CubeADCS firmware serves as the controller for ADCS control program (ACP) and bootloader. The functions developed in this repository are created based on the [CubeADCS firmware manual](https://drive.google.com/file/d/1IjYBjc_tx08WxeTU52L2j7Qh9YvF63TF/view). The functionalities can be categorized into 3 groups: Common, Bootloader, and ACP where each contain functions that could be telecommands, telemetries, or config messages. Please note that, in some cases, mainly telemetries, a more general functions were developed to prevent redundancy. To match the functions with their respective TC/TM in the manual, use the IDs in the [header](https://github.com/AlbertaSat/ex2_adcs_software/blob/master/inc/adcs_types.h), the [equipment handler API](https://docs.google.com/document/d/1sR0T6L22xEFiPRgvFZPy8pQCw7zIKXek2D79r6hg8_s), or the comments.

## Testing
To validate the functions without hardware, using [Ceedling](https://github.com/ThrowTheSwitch/Ceedling), I/O functionalities (in uart_i2c.c) have been mocked by [CMOCK](https://github.com/ThrowTheSwitch/CMock/blob/master/docs/CMock_Summary.md). So the frame that we anticipate is transceived through -currently I2C and UART- communications, has been given to test the logic of the code.
### Ceedling Instructions
After installing ceedling:

- Set `#define MOCKED` in adcs_io.c
- Select communication protocol, in adcs_handler.c: `#define USE_UART` for UART or `#define USE_I2C` for I2c.
- `cd` into ex2_adcs_software and run :
```
ceedling test:adcs_handler      // if testing UART
ceedling test:adcs_handler_i2c  // if testing I2C
```

NOTE: uart_i2c.c is implemented but not tested. It may need further modification when hardware testing is done.

