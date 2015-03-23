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

// --------------- Momentary toggle foot-switches and LEDs:

#define M_1 0x01U
#define M_2 0x02U
#define M_3 0x04U
#define M_4 0x08U
#define M_5 0x10U
#define M_6 0x20U
#define M_7 0x40U
#define M_8 0x80U

#define m_channel_initial 0x80U

// FX button enable bitmasks:
#define fxm_compressor  0x01
#define fxm_filter      0x02
#define fxm_pitch       0x04
#define fxm_chorus      0x08
#define fxm_delay       0x10
#define fxm_reverb      0x20
#define fxm_noisegate   0x40
#define fxm_eq          0x80

// FX button labels:
#define fxb_compressor  0
#define fxb_filter      1
#define fxb_pitch       2
#define fxb_chorus      3
#define fxb_delay       4
#define fxb_reverb      5
#define fxb_noisegate   6
#define fxb_eq          7

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

// --------------- Controller logic interface functions:

/* export */ void controller_init(void);
/* export */ void controller_10msec_timer(void);
/* export */ void controller_handle(void);

// --------------- Non-standard / custom:

// JSD's custom persistent data structures per program:

// Program data structure loaded from / written to flash memory:
struct program {
    // Name of the program in ASCII, max 20 chars, NUL terminator is optional at 20 char limit; NUL padding is preferred:
    u8 name[20];

    // Scene descriptors:
    u8 scene_desc[6];
    // G-major effects enabled per scene (see fxm_*):
    u8 fx[6];
};

// NOTE(jsd): Struct size must be a divisor of 64 to avoid crossing 64-byte boundaries in flash!
// Struct sizes of 1, 2, 4, 8, 16, and 32 qualify.
COMPILE_ASSERT(sizeof(struct program) == 32);

// Set list entry
struct set_entry {
    u8 program;
};

struct set_list {
    u8 count;                       // number of songs in set list
    u8 d0, d1;                      // date of show (see DATES below)
    struct set_entry entries[29];
};

// DATES since 2014 are stored in 16 bits in the following form: (LSB on right)
//  yyyyyyym mmmddddd
//  |||||||| ||||||||
//  |||||||| |||\++++ day of month [0..30]
//  |||||||\-+++----- month [0..11]
//  \++++++---------- year since 2014 [0..127]

// NOTE(jsd): Struct size must be a divisor of 64 to avoid crossing 64-byte boundaries in flash!
// Struct sizes of 1, 2, 4, 8, 16, and 32 qualify.
COMPILE_ASSERT(sizeof(struct set_list) == 32);

// An RJM channel descriptor:
//
// 8 bits describes two RJM channels:
//
// bits:
// 7654 3210
// BBCC BBCC
// |||| ||||
// |||| ||\+- Channel (0-2, 3 ignored)
// |||| ||
// |||| \+--- Boost   -6dB, -3dB, 0dB, +3dB (signed 2-bit)
// ||||
// ||\+------ Channel (0-2, 3 ignored)
// ||
// \+-------- Boost   -6dB, -3dB, 0dB, +3dB (signed 2-bit)
//


// Mark V channel 1
#define rjm_channel_1   0x00
// Mark V channel 2
#define rjm_channel_2   0x01
// Mark V channel 3
#define rjm_channel_3   0x02

#define rjm_channel_mask        0x03

// Scene level as 2-bit signed integer
#define scene_level_mask (31 << 2)
#define scene_level_shr  2

// 5-bit signed values
#define scene_level_0    ((( 0 - 3) & 31) << 2)
#define scene_level_pos5 (((+5 - 3) & 31) << 2)
#define scene_level_neg3 (((-3 - 3) & 31) << 2)

#define scene_initial    0x80
