#ifndef AINAME
#define AINAME

#include <string>
#include <iostream>

#include "board.h"


class AINAME
{
  chessBoard currentChessBoard;

  void init(std::string fenBoardInput);
  void gameUpdated(std::string moveMade);
  void gameEnd();
  std::string makeMove();

};

#endif