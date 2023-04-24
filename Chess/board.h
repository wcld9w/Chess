

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
  int endPosY;

  //Used for specific logic with whatever
  bool isKingCastle = false;
  bool isQueenCastle = false;
  bool isPromotion = false;

  Move();
  Move(char startX, int startY, char endX, int endY);
  Move(int startX, int startY, int endX, int endY);
  
  int getStartXInt();
  int getEndXInt();
  void setStartXInt(int set);
  void setEndXInt(int set);

  void printMove();
  std::string cleanString();
  std::string promotionString();
  std::vector<std::string> cleanStringVector();

};

#endif

#ifndef CHESSBOARD
#define CHESSBOARD

  #include <string>
  #include <iostream> 
  #include <vector> 
  #include <cstdlib> 

  #include <algorithm> //Used for sort

  class chessBoard
  {
    private:
      std::vector<std::string> board;
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
      std::vector<Move> moveList;
      //Variable to store the position of each king
      int kingPos[2][2];

      //Internal function called whenever a new board is loaded
      std::vector<std::vector<int>> piecesGivingCheck(bool isWhite, int xPos, int yPos);
      void regenerateMoveList(bool isWhite);
      bool isPinned(bool isWhite, int xPos, int yPos);
      bool canTake(int xPos, int yPos, int xPos2, int yPos2);
      void getKingPos(bool isWhite, int& kingPosX, int& kingPosY);
      std::vector<Move> recursiveMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY);
      std::vector<Move> recursiveRestrictedMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY, int kingX, int kingY);
      bool inCheck(bool isWhite, int xPos, int yPos);
      std::vector<Move> checkLogicMoves(bool isWhite, std::vector<Move> moveList);
      char firstPieceResursiveFind(bool isWhite, int xPos, int yPos, int xOffset, int yOffset);
      std::vector<Move> bishopMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> rookMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> knightMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> pawnMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> queenMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> kingMoves(bool isWhite, int xPos, int yPos);
      std::vector<Move> getPieceMoves(int xPos, int yPos);
      Move newMove(int xStart, int yStart, int xEnd, int yEnd);
      char pieceAtLocation(int xPos, int yPos);
      void reverseBoard();

      bool comparisonFunctionForMoveList(std::string a, std::string b);
 


    public:
      //Constructors
      chessBoard();
      chessBoard(chessBoard inputChessBoard, Move moveToMake);
      void copyFunction(int kingPos[2][2], char enPassentT[2], int& halfClock, int& fullClock, bool canCastle[4], vector<string>& board, bool& turn);
      //Psuedo constructor
      void importFENBoard(std::string FEN);

      //Get functions
      std::string exportFENBoard();
      std::vector<Move> getMoveList();
      std::vector<std::string> getSortedMoveListStrings();


      //IDK
      void makeMove(std::string move);



  };

#endif