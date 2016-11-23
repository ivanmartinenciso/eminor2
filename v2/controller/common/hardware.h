/*
    Defined hardware I/O functions that the controller software needs to interface with.
    This includes MIDI I/O, foot-switch momentary toggle switches, and LED indicators
    above foot-switches.

    NOTE: it is expected that 'types.h' is #included before this file
*/

// Features enabled/disable:

// Enable LCD display:
#define FEAT_LCD

// --------------- Compiler hacks:

#define STATIC_ASSERT(cond,ident) typedef char _static_assert_##ident[(cond)?1:-1]
#define COMPILE_ASSERT2(cond,line) STATIC_ASSERT(cond,line)
#define COMPILE_ASSERT(cond) COMPILE_ASSERT2(cond,__LINE__)

#ifndef __MCC18
#define rom
#endif

// Define a DEBUG_LOG0 macro:
#ifdef __MCC18
#define DEBUG_LOG0(fmt) (fmt)
#define DEBUG_LOG1(fmt,a1) (fmt)
#else
extern void debug_log(const char *fmt, ...);
#define DEBUG_LOG0(fmt) debug_log(fmt)
#define DEBUG_LOG1(fmt,a1) debug_log(fmt,a1)
#define DEBUG_LOG2(fmt,a1,a2) debug_log(fmt,a1,a2)
#endif

// --------------- Momentary toggle foot-switches and LEDs:

#define M_1 0x01U
#define M_2 0x02U
#define M_3 0x04U
#define M_4 0x08U
#define M_5 0x10U
#define M_6 0x20U
#define M_7 0x40U
#define M_8 0x80U

// Foot switch and LED on/off states are represented with u16 bit-fields;
// the bottom row takes up LSBs (bits 0-7) and top row takes up MSBs (bits 8-15).

// Poll 16 foot-switch states:
extern u16 fsw_poll(void);

// Explicitly set the state of all 16 LEDs:
extern void led_set(u16 leds);

#ifdef FEAT_LCD

// Example LCD display: http://www.newhavendisplay.com/nhd0420d3znswbbwv3-p-5745.html 4x20 characters
#define LCD_COLS    20
#define LCD_ROWS    4

// Get pointer to a specific LCD row:
// A terminating NUL character will clear the rest of the row with empty space.
extern u8 *lcd_row_get(u8 row);

// Mark the LCD display row as updated:
extern void lcd_updated_row(u8 row);
// Update all LCD display rows as updated:
extern void lcd_updated_all(void);

#endif

// --------------- MIDI I/O functions:

// Send a single MIDI byte:
extern void midi_send_byte(u8 data);

/* Send multi-byte MIDI commands
     0 <= cmd     <=  F   - MIDI command
     0 <= channel <=  F   - MIDI channel to send command to
    00 <= data1   <= FF   - first data byte of MIDI command
    00 <= data2   <= FF   - second (optional) data byte of MIDI command
*/
extern void midi_send_cmd1(u8 cmd, u8 channel, u8 data1);
extern void midi_send_cmd2(u8 cmd, u8 channel, u8 data1, u8 data2);

// --------------- Flash memory functions:

// Flash addresses are 0-based where 0 is the first available byte of
// non-program flash memory.

// Load `count` bytes from flash memory at address `addr` into `data`:
extern void flash_load(u16 addr, u16 count, u8 *data);

// Stores `count` bytes from `data` into flash memory at address `addr`:
extern void flash_store(u16 addr, u16 count, u8 *data);

// Get a pointer to flash memory at address:
extern u8 *flash_addr(u16 addr);

// Get a pointer to a lookup table:
extern u8 *lookup_table(u8 table);

// --------------- Controller logic interface functions:

/* export */ extern void controller_init(void);
/* export */ extern void controller_10msec_timer(void);
/* export */ extern void controller_handle(void);

#ifdef HWFEAT_LABEL_UPDATES

// --------------- Change button labels (for Win32 / HTML5 interfaces only):

/* export */ extern u8 **label_row_get(u8 row);
/* export */ extern void label_row_update(u8 row);

#endif
