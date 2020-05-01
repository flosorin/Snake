/************************************************************************
* Classic Snake game displayed on LED matrices by an Arduino UNO
* https://github.com/flosorin/Snake.git
*
* Using Simon Monk's Timer library
* http://www.doctormonk.com/2012/01/arduino-timer-library.html
* 
* Using Peter Knight's TrueRandom library
* http://code.google.com/p/tinkerit
* 
* FourMatrix library by Florian Sorin (2018) 
* https://github.com/flosorin/FourMatrix.git
* 
* ButtonsController library by Florian Sorin (2020)
* https://github.com/flosorin/ButtonsController.git
************************************************************************/

#include "Snake.h"

/* Screen management (16x16 panel composed of four 8x8 LEDs matrices)  */
FourMatrix screen = FourMatrix(PIN_DATA_IN_SCREEN, PIN_CLK_SCREEN, PIN_LOAD_SCREEN);
TrueBool leds = TrueBool();

/* Management of snake and apples */
SnakeObject snake = SnakeObject();
coordinate apple;
bool addElement = false; // Tells if the snake needs to grow

/* Buttons management */
ButtonsController buttons = ButtonsController(PIN_BUTTONS);

/* Movement management */
int mvtWay = DOWN;
Timer movingTimer;

/* Game over management */
bool isGameOver = false;
bool isFirstLoopSinceGameOver = true;

/* Score management */
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
int score = 0, bestScore = 0;
bool needToIncreaseLvl = false;

/* SD card management (store levels pattern) */
SDManager SDCard = SDManager(PIN_CS);

void setup() 
{
  Serial.begin(9600); // Serial for debug
  
  /* Initialize score display */
  lcd.begin(16, 2); // 16 characters on each line, 2 lines
  int tmpBest = SDCard.getBestScore();
  bestScore = (tmpBest != -1) ? tmpBest : 0;
  updateScoreDisplay();
  
  defineLevel(1); // Load first level pattern
}

void loop() 
{  
  if (!isGameOver)
  {
    buttons.updateButtonValue();
    modifyMvt(); // Update the movement way if needed (i.e. button pressed)
    movingTimer.update();
    if (needToIncreaseLvl)
    {
      defineLevel((int)(score / 10) + 1);
      needToIncreaseLvl = false;
    }
  }
  else if (isFirstLoopSinceGameOver)
  {
    movingTimer.stop(0);
    leds = SDCard.readLevel(0);
    screen.setMultiLED(leds);
    if (score > bestScore) {
      bestScore = score;
      SDCard.writeBestScore(bestScore);
      updateScoreDisplay();
    }
    isFirstLoopSinceGameOver = false; 
  }
}

void defineLevel(int level)
{
  /* Stop the movement and initialize snake position */
  if (level > 1)
  {
    movingTimer.stop(0);
    snake.initSnake();
    mvtWay = DOWN;
  }

  /* Clear screen */
  screen.setAllLED(false);
  
  /* Generate the new screen */
  leds = SDCard.readLevel((level < 8) ? level : 8); // Load level pattern
  leds = leds | snake.getSnake(); // Get snake coordinates
  generateApple(); // Generate an apple
  screen.setMultiLED(leds); // Update the screen according to all previous elements

  /* Launch the movement */
  movingTimer.every(1000, moveSnake);
}

/* Snake movement management */
void moveSnake()
{
  TrueBool tmpBool = TrueBool();
  tmpBool = snake.moveSnake(mvtWay, leds, apple, addElement);

  /* Collision management */
  if (leds == tmpBool)
  {
    isGameOver = true; 
  }
  else
  {
    leds = tmpBool;
  }

  /* Update display and manage apple eating */
  if (!isGameOver)
  {
    screen.setMultiLED(leds);
    eatingManager();
  }
}

/* Apple eating management */
void eatingManager()
{
  /* If an apple has been eaten */
  if (snake.getHeadX() == apple.x && snake.getHeadY() == apple.y)
  {
    score++;
    updateScoreDisplay();

    /* If score is a multiple of 10, go to next level */
    if (score % 10 == 0)
    {
      needToIncreaseLvl = true;
    }
    else
    {
      addElement = true; // The snake needs to grow at next movement
      generateApple();
    }
  }
  else
  {
    addElement = false; // The snake does not need to grow at next movement
  }
}

/* Display an "apple" on the screen (i.e. turn ON a random LED not on the snake or on the walls) */
void generateApple()
{
  apple.x = TrueRandom.random(1, 15);
  apple.y = TrueRandom.random(1, 15);

  /* Recursive call while the coordinates corresponds to an already set led (i.e. walls or snake)  */
  if (leds.getBit(apple.x, apple.y) == true)
  {
    generateApple();
  }
  else
  {
    leds.setBit(apple.x, apple.y, true);
  }
}

/* Modify the movement way if needed (i.e. button pressed and movement possible) */
void modifyMvt()
{
  bool isModifAuthorized = false;
  ButtonsValue buttonValue = buttons.getButtonValue();
  
  if ((buttonValue != NONE) && (buttonValue != mvtWay))
  {
    switch (buttonValue)
    {
      case LEFT: 
        isModifAuthorized = (mvtWay != RIGHT);
        break;
      case RIGHT: 
        isModifAuthorized = (mvtWay != LEFT);
        break;
      case UP: 
        isModifAuthorized = (mvtWay != DOWN);
        break;
      case DOWN: 
        isModifAuthorized = (mvtWay != UP);
        break;
    }

    if (isModifAuthorized)
    {
       mvtWay = buttonValue;
    }
  }
}

void updateScoreDisplay()
{
  lcd.setCursor(3, 0);
  lcd.print(String("Score: ") + score);
  lcd.setCursor(3, 1);
  lcd.print(String("Best: ") + bestScore);
}

/* Debug */
void displayHeadCoordinates()
{
  Serial.print(snake.getHeadX());
  Serial.print(" -- ");
  Serial.println(snake.getHeadY());
}
