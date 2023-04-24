#include "randomChessAi.h"


void randomAi::init(std::string fenBoardInput)
{
  currentChessBoard.importFENBoard(fenBoardInput);
}


void randomAi::gameUpdated(std::string moveMade)
{
  currentChessBoard.makeMove(moveMade);
}


void randomAi::gameEnd()
{

}


std::string randomAi::makeMove()
{
  std::vector<std::string> moveListList = currentChessBoard.getSortedMoveListStrings();
  int randomMoveInt = rand() % moveListList.size();
  currentChessBoard.makeMove(moveListList[randomMoveInt]);
  return std::string(moveListList[randomMoveInt]);
}


