#include "SDManager.h"
#include "TrueBool.h"

/* Constructor: define chip select pin and initialize SD card */
SDManager::SDManager(int csPinInit)
{
  csPin = csPinInit;
  initSD();
}

void SDManager::initSD()
{
  Serial.print("Initializing SD card...");

  if (!SD.begin(csPin)) 
  {
    Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
}

/* Read a level from txt file and convert it to ON/OFF state of leds */
TrueBool SDManager::readLevel(const int numLevel)
{
  File lvlFile;
  String fileName = String("Snake/Levels/Lvl") + numLevel + String(".txt");
  String lvlDraw = "";
  TrueBool screenArray = TrueBool();
  
  /* Open level file in reading mode */
  lvlFile = SD.open(fileName);
  Serial.print(String("Reading from ") + fileName + String("..."));
  
  /* Read the file */
  if (lvlFile) 
  {
    for (int y = 15; y >= 0; y--)
    {
      lvlDraw = lvlFile.readStringUntil('\n');
      
      for (int x = 15; x >= 0; x--)
      {   
        if (lvlDraw[x] == 'x')
        {
          screenArray.setBit(x, y, true);
        }
        else
        {
          screenArray.setBit(x, y, false);
        }
      }
    }
    lvlFile.close();
    Serial.println("Done.");
  } 
  else 
  {
    Serial.println(String("Error opening ") + fileName);
  }

  return screenArray;
}

/* Save the new best score in the dedicated file */
void SDManager::writeBestScore(int bestScore)
{
  File bestScoreFile;
  String fileName = "Snake/best.txt";
  
  if (SD.exists(fileName)) 
  {
     SD.remove(fileName);
  }

  /* Open file in writing mode */
  bestScoreFile = SD.open(fileName, FILE_WRITE);

  /* Write score in the file */
  if (bestScoreFile) 
  {
    Serial.print(String("Writing to ") + fileName + String("..."));
    bestScoreFile.println(bestScore);
    bestScoreFile.close();
    Serial.println("Done.");
  } 
  else 
  {
    Serial.println(String("Error opening ") + fileName);
  }
}

/* Return the best score stored in the dedicated file */
int SDManager::getBestScore()
{
  File bestScoreFile;
  String fileName = "Snake/best.txt";
  
  int i = 0;
  char tmp[10];
  
  /* Open file in reading mode */
  bestScoreFile = SD.open(fileName);
  Serial.print(String("Reading from ") + fileName + String("..."));
  
  /* Read the score in the file and return it as integer */
  if (bestScoreFile) 
  {
    while (bestScoreFile.available()) 
    {
      tmp[i] = bestScoreFile.read();
      i++;
    }
      bestScoreFile.close();
      Serial.println("Done.");

      return atoi(tmp);
  } 
  else 
  {
    Serial.println(String("Error opening ") + fileName);
  }

  return -1;
}
