#include <iostream>
#include "board.h"

using namespace std;


int main()
{
  chessBoard newBoard;
  system("cls");
  cout << "on to fen" << endl;
  newBoard.importFENBoard("3k2bn/6p1/4P2P/8/8/8/8/8 b - - 0 1");



  return 0;
}