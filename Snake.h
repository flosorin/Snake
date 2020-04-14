#include <Event.h>
#include <Timer.h>
#include <TrueRandom.h>
#include <FourMatrix.h>
#include <LiquidCrystal.h>
#include "SnakeObject.h"
#include "SDManager.h"

/* Pins used by the MAX7219 controling the four leds matrices */
#define PIN_DATA_IN_SCREEN 10
#define PIN_CLK_SCREEN 9
#define PIN_LOAD_SCREEN 8
/* Pin used to recover the buttons value */
#define PIN_BUTTONS A1
/* Chip select of the SD card module */
#define PIN_CS 4
/* LCD pins */
#define LCD_D7 A2
#define LCD_D6 A3
#define LCD_D5 A4
#define LCD_D4 A5
#define LCD_EN 6
#define LCD_RS 7
