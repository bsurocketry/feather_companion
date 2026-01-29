
#include "companion_protocol.h"
#include "companion.h"
#include <stdlib.h>
#include <limits.h>

uint local_tick_count = 0;
flight_state_handler flight_state_handlers[ao_flight_max] = {
   [ao_flight_startup] = default_handler,
   [ao_flight_idle]    = default_handler,
   [ao_flight_pad]     = default_handler,
   [ao_flight_boost]   = default_handler,
   [ao_flight_fast]    = default_handler,
   [ao_flight_coast]   = default_handler,
   [ao_flight_drogue]  = default_handler,
   [ao_flight_main]    = default_handler,
   [ao_flight_landed]  = default_handler,
};

void * flight_state_handler_args[ao_flight_max] = {
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
   NULL,
};

int can_read_command() {
   return spi_is_readable(COMPANION_SPI);
}

void get_next_command(command * cmd) {
   spi_read_blocking(COMPANION_SPI,0xff,(uint8_t *)cmd,sizeof(command));
}

void send_setup_reply() {
   setup_reply reply = {
      .board_id         = SETUP_BOARD_ID,
      .board_id_inverse = ~SETUP_BOARD_ID,
      .update_period    = SETUP_UPDATE_PERIOD,
      .channels         = SETUP_CHANNELS,
   };
   spi_write_blocking(COMPANION_SPI,(uint8_t *)&reply,sizeof(setup_reply));
}

void send_fetch_response(fetch_reply * reply) {
   spi_write_blocking(COMPANION_SPI,(uint8_t *)reply,sizeof(fetch_reply));
}

/* just loop the same message over and over... */
void default_handler(command * cmd, void * arg) {
   DEBUG_PRINTF("running default handler");
   static char lain_string[15] = {
      'l','e','t','s','a','l','l','l','o','v','e','l','a','i','n'
   };
   static uint lain_pos = 0;

   fetch_reply reply;
   for (int i = 0; i < SETUP_CHANNELS; ++i) {
      reply.data[i] = lain_string[lain_pos%sizeof(lain_string)];
      lain_pos += 1;
   }
}

void add_state_handler(ao_flight_state state, flight_state_handler func, void * arg) {
   if (state >= ao_flight_max)
      return; /* jut ignore this */
   flight_state_handlers[state] = func;
   flight_state_handler_args[state] = arg;
}

void update_local_tick(uint32_t tick) {
   static int first_time = 1;
   static uint32_t last_seen = 0;

   if (first_time) {
      /* please be gentle... */
      first_time = 0;
      last_seen = tick;
   }
   else {
      /* the tick will overflow every roughly
       * ten minutes if I did my math right...
       * So we will want to be sure of this and
       * adjust accordingly.                    */
      if (tick < last_seen) {
         tick += USHRT_MAX;
      }
      local_tick_count += (tick - last_seen);
      last_seen = tick;
   }
}

void handle_fetch(command * cmd) {
   enum ao_flight_state state = cmd->flight_state;

   /* now pass things off to the current handler */
   DEBUG_PRINTF("handling fetch in state %s",
                ao_flight_state_string[state]);

   flight_state_handlers[state](cmd,flight_state_handler_args[state]);
}
