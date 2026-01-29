
#ifndef __COMPANION

#define __COMPANION
#include "companion_protocol.h"
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#ifdef DEBUG
#define DEBUG_PRINTF(text, ...) \
   printf("(%04x): " text "\n", __LINE__, __VA_ARGS__)
#else
#define DEBUG_PRINTF(text, ...)
#endif

/* total ticks received by the system */
extern uint local_tick_count;

/* array of functions for handling fetch commands 
 * during different flight states.               */
typedef void (*flight_state_handler)(command * cmd, void * arg);
extern flight_state_handler flight_state_handlers[ao_flight_max];

/* block until the next command is recieved from the host device */
void get_next_command(command * cmd);

/* check if a command is waiting to be read by the companion */
int can_read_command();

/* board id for this device */
#define SETUP_BOARD_ID 0x1fa1
/* update period in 100Hz steps - as small as possible */
#define SETUP_UPDATE_PERIOD 1
/* total fields used in fetch response - max is 12 */
#define SETUP_CHANNELS 12

/* create a setup reply message and send it to the host device.
 * params are configured using the above macros and automatically
 * passed in a message when this function is called.             */
void send_setup_reply();

/* send the contents of the fetch reply message to the host device */
void send_fetch_response(fetch_reply * reply);

/* add a handler for fetch requests at a given flight state */
void add_state_handler(ao_flight_state state, flight_state_handler func, void * arg);

/* the flight state handler used when none is set */
void default_handler(command * cmd, void * arg);

/* process a host fetch command using the set handlers */
void handle_fetch(command * cmd);

/* update the variable local_tick_count to reflect the tick of the new message */
void update_local_tick(uint32_t tick);

#endif
