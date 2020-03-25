#include "Arduino.h"
#include <SPI.h>
#include <SD.h>

class TrueBool; // Forward declaration to avoid include conflicts

/********* PIN Mapping *******
 * MOSI - pin 11
 * MISO - pin 12
 * CLK - pin 13
 *****************************/

class SDManager
{
  private :
    int csPin;
    void initSD();

  public:
    SDManager(int csPinInit); // Constructor: define chip select pin and initialize SD card
    TrueBool readLevel(const int numLevel); // Read a level from txt file and convert it to ON/OFF state of leds
    void writeBestScore(int bestScore); // Save the new best score in the dedicated file
    int getBestScore(); // Return the best score stored in the dedicated file
};
