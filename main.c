#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// #include "hardware/gpio.h"
#include "bsp/board.h"
#include "tusb.h"

uint8_t kbd_addr = 0;
uint8_t kbd_inst = 0;

void main() {
  board_init();
  
  // sm = pio_claim_unused_sm(pio, true);
  // offset = pio_add_program(pio, &ps2dev_program);
  // ps2dev_program_init(pio, sm, offset, CLKIN, CLKOUT, DATIN, DATOUT);
  
  // gpio_set_irq_enabled_with_callback(CLKIN, GPIO_IRQ_EDGE_RISE, true, &irq_callback);
  tusb_init();  // TinyUSB init
  // tuh_init(BOARD_TUH_RHPORT);  // alternative, init host stack on configured roothub port
  
  while(true) {
    tuh_task();
    
    // if(!pio_sm_is_rx_fifo_empty(pio, sm)) {
    //   ps2_receive(pio_sm_get(pio, sm));
    //   board_led_write(0);
    // }
    
    // if(repeating) {
    //   repeating = false;
      
    //   if(repeat) {
    //     maybe_send_e0(repeat);
    //     ps2_send(hid2ps2[repeat]);
    //   }
    // }
  }

  return 0;
}


//--------------------------------------------------------------------+
// TinyUSB Callbacks
//--------------------------------------------------------------------+

void tuh_mount_cb(uint8_t dev_addr)
{
  // application set-up
  printf("A device with address %d is mounted\r\n", dev_addr);
}

void tuh_umount_cb(uint8_t dev_addr)
{
  // application tear-down
  printf("A device with address %d is unmounted \r\n", dev_addr);
}


// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len) {
  printf("HID device address = %d, instance = %d is mounted\r\n", dev_addr, instance);
  if(tuh_hid_interface_protocol(dev_addr, instance) == HID_ITF_PROTOCOL_KEYBOARD) {
    kbd_addr = dev_addr;
    kbd_inst = instance;
    
    // blinking = true;
    // add_alarm_in_ms(1, blink_callback, NULL, false);
    
    tuh_hid_receive_report(dev_addr, instance);
    // if ( !tuh_hid_receive_report(dev_addr, instance) )
    // {
    //   printf("Error: cannot request to receive report\r\n");
    // }
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance) {
  printf("HID device address = %d, instance = %d is unmounted\r\n", dev_addr, instance);
  if(dev_addr == kbd_addr && instance == kbd_inst) {
    kbd_addr = 0;
    kbd_inst = 0;
  }
}



// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  if( !(dev_addr == kbd_addr && instance == kbd_inst) ) {
    printf("Invalid dev_addr or instance\r\n");
    return;
  }

  // if(report[1] != 0) {    // ???
  //   tuh_hid_receive_report(dev_addr, instance);
  //   return;
  // }

  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch (itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      TU_LOG2("HID receive boot keyboard report\r\n");
      printf("HID receive boot keyboard report\r\n");
      process_kbd_report( (hid_keyboard_report_t const*) report );
    break;
  }

  // board_led_write(1);  // ?????

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    printf("Error: cannot request to receive report\r\n");
  }
}


//--------------------------------------------------------------------+
// Keyboard
//--------------------------------------------------------------------+

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}

static void process_kbd_report(hid_keyboard_report_t const *report)
{
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released

  //------------- example code ignore control (non-printable) key affects -------------//
  for(uint8_t i=0; i<6; i++)
  {
    if ( report->keycode[i] )
    {
      if ( find_key_in_report(&prev_report, report->keycode[i]) )
      {
        // exist in previous report means the current key is holding
      }else
      {
        // not existed in previous report means the current key is pressed
        bool const is_shift = report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
        uint8_t ch = keycode2ascii[report->keycode[i]][is_shift ? 1 : 0];
        putchar(ch);
        if ( ch == '\r' ) putchar('\n'); // added new line for enter key

        fflush(stdout); // flush right away, else nanolib will wait for newline
      }
    }
    // TODO example skips key released
  }

  prev_report = *report;
}
