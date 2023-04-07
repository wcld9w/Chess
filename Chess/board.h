using namespace std;

#ifndef CORDS
#define CORDS
#include <string>

struct Move
{
private:
  char columns[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

public:
  char startPosX;
  int startPosY;

  char endPosX;
  char endPosY;

  Move();
  Move(char startX, int startY, char endX, int endY);
  Move(int startX, int startY, int endX, int endY);
  
  int getStartXInt();
  int getEndXInt();
  void setStartXInt(int set);
  void setEndXInt(int set);

  void printMove();
  string cleanString();

};

#endif

#ifndef CHESSBOARD
#define CHESSBOARD

  #include <string>
  #include <iostream>
  #include <vector>
  #include <cstdlib>

  class chessBoard
  {
    private:
      vector<string> board;
      //turn = true means its whites turn
      bool turn;
      //True = can castle {whiteLeft, whiteRight, blackLeft, blackRight}
      bool canCastle[4];
      //Clocks
      int halfClock;
      int fullClock;
      //Stores when a 2 long pawn move is made (only one possible enpessant target) 
      char enPassant[2];
      //What stores the move list
      vector<Move> moveList;

      //Internal function called whenever a new board is loaded
      void regenerateMoveList();
      bool isPinned(bool isWhite, int xPos, int yPos);
      bool canTake(int xPos, int yPos, int xPos2, int yPos2);
      vector<Move> recursiveMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY);
      vector<Move> recursiveRestrictedMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY, int kingX, int kingY);
      vector<Move> bishopMoves(bool isWhite, int xPos, int yPos);
      vector<Move> rookMoves(bool isWhite, int xPos, int yPos);
      vector<Move> knightMoves(bool isWhite, int xPos, int yPos);
      vector<Move> pawnMoves(bool isWhite, int xPos, int yPos);
      vector<Move> queenMoves(bool isWhite, int xPos, int yPos);
      vector<Move> kingMoves(bool isWhite, int xPos, int yPos);
      vector<Move> inCheck(bool isWhite, int xPos, int yPos);
      vector<Move> getPieceMoves(int xPos, int yPos);


    public:
      chessBoard();

      void importFENBoard(string FEN);

      string exportFENBoard();

      vector<Move> getMoveList();

      void makeMove(Move move);



  };

#endif