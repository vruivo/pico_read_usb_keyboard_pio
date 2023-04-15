Read input from a USB keyboard connected to the Raspberry Pi Pico via an extra USB port, using Pico_PIO_USB

Based on code from https://github.com/sekigon-gonnoc/Pico-PIO-USB/tree/main/examples/host_hid_to_device_cdc

https://www.hobbytronics.co.uk/usb-connector-pinout
VCC, D-, D+, GND

init submodule Pico-PIO-USB
 * cd pico-sdk
 * git -C lib/tinyusb/ submodule update --init hw/mcu/raspberry_pi/Pico-PIO-USB

<img src="/pico_read_usb_pio.png" width=65%>

Note: the HC-05 board is just serving as a raw bluetooth serial console
