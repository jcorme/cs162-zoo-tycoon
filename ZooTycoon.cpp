/*********************************************************************
** Program Filename: ZooTycoon.cpp
** Author: Jason Chen
** Date: 02/19/2018
** Description: Runs the Zoo Tycoon game. 
** Input: None
** Output: None
*********************************************************************/
#include <iostream>
#include "Game.h"

int main() {
  std::cout << "Welcome to Zoo Tycoon!\n"
            << "Hit enter to start the game...";
  std::cin.ignore();
  std::cout << "\n\n" << std::endl;

  Game game;
  game.Run();

  return 0;
}