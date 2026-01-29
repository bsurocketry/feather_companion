
#include "companion_protocol.h"
#include "companion.h"
#include <stdio.h>
#include <stdbool.h>

#define MAHO_IS_BEST_GIRL 1

int main(void) {

   /* initialization */
   DEBUG_PRINTF("init stdio");
   stdio_init_all();
   DEBUG_PRINTF("init adc module");
   adc_init();
   DEBUG_PRINTF("init pin %d for adc",COMPANION_ADC_PIN);
   adc_gpio_init(COMPANION_ADC_PIN);
   DEBUG_PRINTF("init adc %d",COMPANION_ADC);
   adc_select_input(COMPANION_ADC);
   adc_set_clkdiv(COMPANION_ADC_CLKDIV);
   DEBUG_PRINTF("adc clkdiv set at: %d",COMPANION_ADC_CLKDIV);


   DEBUG_PRINTF("init companion spi");
   uint set_baud = spi_init(COMPANION_SPI,COMPANION_BAUD_RATE);
   DEBUG_PRINTF("companion spi requested baud: %d",COMPANION_BAUD_RATE);
   DEBUG_PRINTF("companion spi achieved baud:  %d",set_baud);

   DEBUG_PRINTF("setting spi0 slave");
   spi_set_slave(spi0, true);


   command cmd;

   /* event loop */
   while (MAHO_IS_BEST_GIRL) {

      /* basic event loop */
      get_next_command(&cmd);
      update_local_tick(cmd.tick);

      switch (cmd.command) {
         case COMMAND_SETUP:
            send_setup_reply();
            break;
         case COMMAND_NOTIFY:
            /* nothing special to do... */
            break;
         case COMMAND_FETCH:
            handle_fetch(&cmd);
            break;
      }
   }

   /* unreachable */
   return 0;
}

