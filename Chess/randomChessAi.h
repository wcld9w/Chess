#ifndef RANDOMCHESS
#define RANDOMCHESS

#include <string>
#include <iostream>
#include <random>

#include "board.h"


class randomAi
{
  chessBoard currentChessBoard;

  void init(std::string fenBoardInput);
  void gameUpdated(std::string moveMade);
  void gameEnd();
  std::string makeMove();

};

#endif