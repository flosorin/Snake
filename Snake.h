#include <Event.h>
#include <Timer.h>
#include <TrueRandom.h>
#include <FourMatrix.h>
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
