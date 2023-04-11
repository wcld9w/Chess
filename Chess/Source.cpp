#include <iostream>
#include "board.h"

using namespace std;


int main()
{
  chessBoard newBoard;
  system("cls");
  cout << "on to fen" << endl;
  newBoard.importFENBoard("8/8/8/8/8/2p5/2KP4/BN6 w - - 0 1");



  return 0;
}