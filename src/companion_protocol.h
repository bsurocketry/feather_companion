
#ifndef __COMPANION_PROTOCOL

#define __COMPANION_PROTOCOL

#include <stdint.h>

// Companion Command Message
typedef struct command {
   uint8_t  command;      // Command identifier
   uint8_t  flight_state; // Current flight computer stats
   uint16_t tick;         // Flight computer clock (100 ticks/second)
   uint16_t serial;       // Flight computer serial number
   uint16_t flight;       // Flight number
} command;
			
// Companion Command Identifiers
#define COMMAND_SETUP  1 // Supply the flight computer with companion information
#define COMMAND_FETCH  2 // Return the telemetry information
#define COMMAND_NOTIFY 3 // Tell companion board when flight state changes


// SETUP Reply Message
typedef struct setup_reply {
   uint16_t board_id;         // Board Identifier
   uint16_t board_id_inverse; // ~board_id
   uint8_t  update_period;    // Minimum time between FETCH commands
   uint8_t  channels;         // Number of data channels to retrieve in FETCH command
} setup_reply;


#define FETCH_SLOT_COUNT 12

// FETCH Reply Message
typedef struct fetch_reply {
   uint16_t data[FETCH_SLOT_COUNT];
} fetch_reply;

// ripped from altos/src/kernal/ao_flight.c
enum ao_flight_state {
	ao_flight_startup = 0,
	ao_flight_idle = 1,
	ao_flight_pad = 2,
	ao_flight_boost = 3,
	ao_flight_fast = 4,
	ao_flight_coast = 5,
	ao_flight_drogue = 6,
	ao_flight_main = 7,
	ao_flight_landed = 8,
	ao_flight_invalid = 9
};

#endif
