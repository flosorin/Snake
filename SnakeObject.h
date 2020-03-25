#include "Arduino.h"
#include <ButtonsController.h>

#define SNAKE_MAX_SIZE 14 // The snake shows up with 4 elements (head + 3 elements) and is reset at level changing when it gains 10 more

class TrueBool; // Forward declaration to avoid include conflicts

/* Structure representing a coordinate on the leds screen */
typedef struct
{
  int x = 0;
  int y = 0;
} coordinate;

class SnakeObject
{
  private :
    coordinate body[SNAKE_MAX_SIZE + 1];
    int lenght; // Length of the snake (number of elements)
  public:
    SnakeObject(); // Constructor: calls initSnake
    void initSnake(); // Initialize a snake with four elements
    TrueBool getSnake(); // Return the coordinates of the whole snake
    int getHeadX(); // Return the X coordinate of the snake's head
    int getHeadY(); // Return the Y coordinate of the snake's head
    TrueBool moveSnake(int mvt, TrueBool screenArray, coordinate apple, bool addElement); // Move the snake in the specified direction
};
