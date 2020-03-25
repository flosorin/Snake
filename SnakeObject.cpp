#include "SnakeObject.h"
#include "TrueBool.h"

/* Constructor: calls initSnake */
SnakeObject::SnakeObject()
{
  initSnake();
}

/* Initialize a snake with four elements */
void SnakeObject::initSnake()
{
  for (int i = 0; i < 4; i++)
  {
    body[i].x = 7;
    body[i].y = 11 + i;
  }

  lenght = 4;
}

/* Return the coordinates of the whole snake */
TrueBool SnakeObject::getSnake()
{
  TrueBool screenArray = TrueBool();
  
  for (int i = 0; i < lenght; i++)
  {
    screenArray.setBit(body[i].x, body[i].y, true);
  }

  return screenArray;
}

/* Return the X coordinate of the snake's head */
int SnakeObject::getHeadX()
{
  return body[0].x;
}
/* Return the Y coordinate of the snake's head */
int SnakeObject::getHeadY()
{
  return body[0].y;
}

/* Move the snake in the specified direction */
TrueBool SnakeObject::moveSnake(int mvt, TrueBool screenArray, coordinate apple, bool addElement)
{
  TrueBool tmpBool = TrueBool();
  bool isApple = false, isLastTail = false;
  coordinate lastTail;
  lastTail.x = body[lenght - 1].x;
  lastTail.y = body[lenght - 1].y;
  
  if (mvt != NONE)
  {
    /* Add an element to the snake if requested */
    if (addElement)
    {
      lenght ++;
    }
    
    /* Move all parts of the snake except head */
    for (int i = lenght - 1; i > 0; i--)
    {
      body[i].x = body[i - 1].x;
      body[i].y = body[i - 1].y;
    }
  
    /* Move the head according to the movement requested */
    switch (mvt)
    {
      case LEFT: 
        body[0].x --;
        break;
      case RIGHT: 
        body[0].x ++;
        break;
      case UP: 
        body[0].y ++;
        break;
      case DOWN: 
        body[0].y --;
        break;
    }

    isApple = (body[0].x == apple.x) && (body[0].y == apple.y); // Tells if head will encounter the apple
    isLastTail = (body[0].x == lastTail.x) && (body[0].y == lastTail.y); // Tells if head will replace the previous last element position

    /* Manage collision, i.e. head going where a led was already ON and is not the apple nor the previous last element position */
    if (screenArray.getBit(body[0].x, body[0].y) && !isApple && !isLastTail)
    {
      return screenArray; // Return the non modified screen array to tell the main program a collision occured
    }
 
    tmpBool = getSnake(); // Update the snake's position

    /* Delete last element display (because of moving) except if a new element has been added */
    if (!addElement)
    {
      screenArray.setBit(lastTail.x, lastTail.y, false);
    }  
 
    return screenArray | tmpBool;
  } 
}
