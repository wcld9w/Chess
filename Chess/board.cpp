#include "board.h"

using namespace std;



//ToDo:
//King movements
//Check if we are in check, and if we are in check find all valid moves (FUCK ME THIS SIS ASS SADFJIA:SDJFDSA)
//Check restricted recursive move list to ensure that it is valid
  //Posible solution is to compare the current location to the king and normalize the kings direction from the current position, normalized king direction must mach the move direction (or inverse move direction, which is identical)
//Create a function to check if a piece can take another piece
//Create variables to store the location of each king position
//Check all take checks use != isWhite not == isWhite 


  //---But what about if the piece moving results in a check
//This isn't possible due to checking if we are restricted for each piece, thus we cant




//Empty move constructor
Move::Move()
{
  startPosX = 'A';
  startPosY = 0;
  endPosX = 'B';
  endPosY = 1;
}

//Move constructor from proper form
Move::Move(char startX, int startY, char endX, int endY)
{
  startPosX = startX;
  startPosY = startY;
  endPosX = endX;
  endPosY = endY;
}

//Move constructor from array form
Move::Move(int startX, int startY, int endX, int endY)
{
  startPosX = columns[startX];
  startPosY = startY;
  endPosX = columns[startY];
  endPosY = endY;
}

//simple getter for converting from stored char form to int for array accessing
int Move::getStartXInt()
{
  switch (startPosX)
  {
  case 'A':
    return 0;
  case 'B':
    return 1;
  case 'C':
    return 2;
  case 'D':
    return 3;
  case 'E':
    return 4;
  case 'F':
    return 5;
  case 'G':
    return 6;
  case 'H':
    return 7;
  }
}

//simple getter for converting from stored char form to int for array accessing
int Move::getEndXInt()
{
  switch (endPosX)
  {
  case 'A':
    return 0;
  case 'B':
    return 1;
  case 'C':
    return 2;
  case 'D':
    return 3;
  case 'E':
    return 4;
  case 'F':
    return 5;
  case 'G':
    return 6;
  case 'H':
    return 7;
  }
};

//simple setter for converting from stored char form to int for array accessing
void Move::setStartXInt(int set)
{
  startPosX = columns[set];
}

//simple setter for converting from stored char form to int for array accessing
void Move::setEndXInt(int set)
{
  endPosX = columns[set];
}

//Pretty printing
void Move::printMove()
{
  cout << cleanString();
}

//Returns the expected string for the move
string Move::cleanString()
{
  string temp = "    ";
  temp[0] = startPosX;
  temp[1] = char(startPosY + 1);
  temp[2] = endPosX;
  temp[3] = char(endPosY + 1);
  return temp;
}

//Empty chessboard constructor, generates a board move 0
chessBoard::chessBoard()
{
  // true = whites turn
  turn = true;
  //Set castling rights to true
  for (int i = 0; i < 4; i++) canCastle[i] = true;
  //Set move counts to 0
  halfClock = 0;
  fullClock = 0;
  //Set enpassent to false
  enPassant[0] = 'Z';

  board.push_back("rnbqkbnr");
  board.push_back("pppppppp");
  board.push_back("        ");
  board.push_back("        ");
  board.push_back("        ");
  board.push_back("        ");
  board.push_back("PPPPPPPP");
  board.push_back("RNBQKBNR");

  //Generates the move list
  regenerateMoveList();
}

//Imports a board from a FEN string
void chessBoard::importFENBoard(string FEN)
{
  //Clear the board 
  board.clear();
  //Get the whole board from the FEN into the board variable
  bool loadingBoard = true;
  string lineLoad = "        ";
  int curPos = 0;
  string temp = "12345678";
  while (loadingBoard)
  {
    //If we reach end of a line in the FEN reset for next line
    if (FEN[0] == '/')
    {
      board.push_back(lineLoad);
      lineLoad = "";
      curPos = 0;
      FEN = FEN.substr(1, FEN.length());
    }
    //If we reach the end of all the lines exit
    else if (FEN[0] == ' ')
    {
      loadingBoard = false;
      board.push_back(lineLoad);
      FEN = FEN.substr(1, FEN.length());
    }
    //If there are empty spaces add them
    else if (temp.find(FEN[0]) != std::string::npos)
    {
      for (int i = 0; i < FEN[0]; i++)
      {
        lineLoad[curPos++] = ' ';
      }
      FEN = FEN.substr(1, FEN.length());
    }
    //If its a piece add the piece
    else
    {
      lineLoad[curPos++] = FEN[0];
      FEN = FEN.substr(1, FEN.length());
    }

  }

  //assign whose turn it is
  if (FEN[0] == 'w') turn = true;
  else turn = false;
  FEN = FEN.substr(2, FEN.length());

  //Load castling rights
  for (int i = 0; i < 4; i++)
  {
    canCastle[i] = false;
  }

  while (FEN[0] != ' ')
  {
    switch (FEN[0])
    {
    case 'Q':
      canCastle[0] = true;
      break;
    case 'K':
      canCastle[1] = true;
      break;
    case 'q':
      canCastle[2] = true;
      break;
    case 'k':
      canCastle[3] = true;
      break;
    }
    FEN = FEN.substr(1, FEN.length());
  }
  //Load enpassent 
  if (FEN[0] != '-')
  {
    enPassant[0] = FEN[0];
    enPassant[1] = FEN[1];
    FEN = FEN.substr(3, FEN.length());
  }
  //Load half Clock
  halfClock = stoi(FEN.substr(0, FEN.find(" ")));
  FEN = FEN.substr(FEN.find(" ") + 1, FEN.length());
  //Load full Clock 
  fullClock = stoi(FEN.substr(0, FEN.length()));

  //Generate the move list
  regenerateMoveList();
}

//This checks every position on the board for a piece, lots of reduction to different functions, probably slows it down but makes it easy to understand
void chessBoard::regenerateMoveList()
{
  moveList.clear();
  vector<Move> temp;
  //For each position on the board
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      if (board[i][j] != ' ')
      {
        //Get all the moves for that position
        temp = getPieceMoves(i, j);
        //Append it to the end
        moveList.insert(moveList.end(), temp.begin(), temp.end());
      }
    }
  }
}
 
bool chessBoard::canTake(int xPos, int yPos, int xPos2, int yPos2)
{
  if (isupper(board[xPos][yPos]) == isupper(board[xPos2][yPos2])) return false;
  return true;
}

vector<Move> chessBoard::getPieceMoves(int xPos, int yPos)
{
  char piece = board[xPos][yPos];
  bool isWhite = isupper(piece);
  vector<Move> temp;
  //RNBQKP
  switch (toupper(piece))
  {
    case('R'):
      temp = rookMoves(isWhite, xPos, yPos);
      break;
    case('N'):
      temp = knightMoves(isWhite, xPos, yPos);
      break;
    case('B'):
      temp = bishopMoves(isWhite, xPos, yPos);
      break;
    case('Q'):
      temp = queenMoves(isWhite, xPos, yPos);
      break;
    case('K'):
      temp = kingMoves(isWhite, xPos, yPos);
      break;
    case('P'):
      temp = pawnMoves(isWhite, xPos, yPos);
      break;
    default:
      break;
  }
  return temp;
}

//Checks if the piece at a given position is pinned
bool chessBoard::isPinned(bool isWhite, int xPos, int yPos)
{
  bool isPinned = false;
  //Pieces that can pin other pieces: bishop rook queen
  
  //First find allied king
  int kingX = -1;
  int kingY = -1;
  for (int i = 0; i < 8 && kingX == -1; i++)
  {
    for (int j = 0; j < 8 && kingX == -1; j++)
    {
      if ((board[i][j] == isWhite ? 'K' : 'k'))
      {
        kingX = i;
        kingY = j;
      }
    }
  }
  //if on a diagonal or line
  if (kingX == xPos) //Vertical line between them
  {
    //Get spaces between and check for pieces
    //If pieces exist not pinned
    for (int i = kingY + ((kingY < yPos) ? 1 : -1); i != yPos; i = i + ((kingY < yPos) ? 1 : -1))
    {
      if (board[xPos][i] != ' ') return false;
    }
    //Else check in ray towards opposite direction from king
    for (int i = yPos + ((kingY < yPos) ? 1 : -1); (i != 8) && i != -1; i = i + ((kingY < yPos) ? 1 : -1))
    {
      //for first piece on king, check if it is a piece that is 
      if (board[xPos][i] != ' ')
      {
        //A: not the same player
        //B: able to move multiple tiles in correct direction
        if (isupper(board[xPos][i]) != isWhite && (toupper(board[xPos][i]) == 'Q' || toupper(board[xPos][i]) == 'R')) return true;
        return false;
      }   
    }
  }
  else if (kingY == yPos) //Horizontal line between them
  {
    //Get spaces between and check for pieces
    //If pieces exist not pinned
    for (int i = kingX + ((kingX < xPos) ? 1 : -1); i != xPos; i = i + ((kingX < xPos) ? 1 : -1))
    {
      if (board[i][yPos] != ' ') return false;
    }
    //Else check in ray towards opposite direction from king
    for (int i = xPos + ((kingX < xPos) ? 1 : -1); (i != 8) && i != -1; i = i + ((kingX < xPos) ? 1 : -1))
    {
      //for first piece on king, check if it is a piece that is 
      if (board[i][yPos] != ' ')
      {
        //A: not the same player
        //B: able to move multiple tiles in correct direction
        if (isupper(board[i][yPos]) != isWhite && (toupper(board[i][yPos]) == 'Q' || toupper(board[i][yPos]) == 'R')) return true;
        return false;
      }
    }
  }
  //If the difference between height and width are the same then its a square IE diagonal
  else if (abs(kingX - xPos) == abs(kingY - yPos))
  {
    int xInc = kingX < xPos ? 1 : -1;
    int yInc = kingY < yPos ? 1 : -1;
    //First check all tiles between king and piece
    for (int i = 1; i < abs(kingX - xPos); i++)
    {
      if (board[kingX + xInc * i][kingY + yInc * i] != ' ') return false;
    }
    //Then check all tiles after
    for (int i = 1; (xPos + xInc*i) != kingX < xPos ? 8 : -1 && (yPos + yInc * i) != kingY < yPos ? 8 : -1; i++)
    {
      //If the board has a piece at the position
      if (board[xPos + xInc * i][yPos + yInc * i] != ' ')
      {
        //Check if it is the same player
        if (isupper(board[xPos + xInc * i][yPos + yInc * i]) != isWhite && (toupper(board[xPos + xInc * i][yPos + yInc * i]) == 'Q' || toupper(board[xPos + xInc * i][yPos + yInc * i]) == 'B')) return true;
        //Else check if it can move in the correct direction
        return false;
      }
    }
  }

  //We return true or false at any point where the king is in line with the given piece, to get here would imply that the king is not in any line with the given piece, thus not pinned
  return false;
}

//A simple function to make inline calls for a recursive move finding in a certain direction from a given 
vector<Move> chessBoard::recursiveMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY)
{
  vector<Move> moveList;
  //For a max of 8 distance
  for (int i = 1; i < 8; i++)
  {
    //Check if we are about to exit the board and if we are return all found moves
    if (xPos + incX * i == incX > 0 ? 8 : -1 || yPos + incY * i == incY > 0 ? 8 : -1) return moveList;
    //Then check if the space we are checking is empty, if it is we add it to the list and continue iteration
    else if (board[xPos + incX * i][yPos + incY * i] == ' ') moveList.push_back(Move(xPos, yPos, xPos + incX * i, yPos + incY * i));
    //Then we check if the piece on the tile is the same color(this is because if it isn't empty, we can assume it has a piece)
    else if (isupper(board[xPos + incX * i][yPos + incY * i]) != isWhite)
    {
      moveList.push_back(Move(xPos, yPos, xPos + incX * i, yPos + incY * i));
      return moveList;
    }
    //Then if nothing else finds we return the list, I don't think this specific thing is every reached but better safe than sorry
    else return moveList;
  }
  //Not possible to hit but better safe than sorry
  return moveList;
}

vector<Move> chessBoard::recursiveRestrictedMoveCheck(bool isWhite, int xPos, int yPos, int incX, int incY, int kingX, int kingY)
{
  vector<Move> moveList;
  //The same as normal recursive check except first we check if the increments allow us to continue in the same line as the king
  //if the king and piece are in the same vertical column, only allow vertical movement
  if (kingX == xPos && incX != 0) return moveList;
  //Same for horizontal
  else if (kingY == yPos && incY != 0) return moveList;
  //else we would only call this function if there is a diagonal pin, this is where things get trucky
  else
  {
    //Signs either match or they don't match
    if ((xPos - kingX) / abs(xPos - kingX) == (yPos - kingY) / abs(yPos - kingY))
    {
      if (incX != incY) return moveList;
    }
    else
    {
      if (incX == incY) return moveList;
    }
  }
   
  
  //For a max of 8 distance
  for (int i = 1; i < 8; i++)
  {
    //Check if we are about to exit the board and if we are return all found moves
    if (xPos + incX * i == incX > 0 ? 8 : -1 || yPos + incY * i == incY > 0 ? 8 : -1) return moveList;
    //Then check if the space we are checking is empty, if it is we add it to the list and continue iteration
    else if (board[xPos + incX * i][yPos + incY * i] == ' ') moveList.push_back(Move(xPos, yPos, xPos + incX * i, yPos + incY * i));
    //Then we check if the piece on the tile is the same color(this is because if it isn't empty, we can assume it has a piece)
    else if (isupper(board[xPos + incX * i][yPos + incY * i]) != isWhite)
    {
      moveList.push_back(Move(xPos, yPos, xPos + incX * i, yPos + incY * i));
      return moveList;
    }
    //Then if nothing else finds we return the list, I don't think this specific thing is every reached but better safe than sorry
    else return moveList;
  }
  //Not possible to hit but better safe than sorry
  return moveList;
}

//Performs a check for all valid bishop moves from given position and color
vector<Move> chessBoard::bishopMoves(bool isWhite, int xPos, int yPos)
{
  vector<Move> moveList;

  //Logic for if we aren't pinned
  if (!isPinned(isWhite, xPos, yPos))
  {
    //upRight check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, 1, 1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //upLeft check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, 1, -1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downLeft check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, -1, -1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downRight check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, -1, 1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
  }
  else
  {
    //First find allied king
    int kingX = -1;
    int kingY = -1;
    for (int i = 0; i < 8 && kingX == -1; i++)
    {
      for (int j = 0; j < 8 && kingX == -1; j++)
      {
        if ((board[i][j] == isWhite ? 'K' : 'k'))
        {
          kingX = i;
          kingY = j;
        }
      }
    }
    //Logic to run the recursiveRestrictedMoveCheck function
    //upRight check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, 1, 1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //upLeft check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, 1, -1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downLeft check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, -1, -1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downRight check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, -1, 1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
  }



  return moveList;
}

//Performs a check for all valid bishop moves from given position and color
vector<Move> chessBoard::rookMoves(bool isWhite, int xPos, int yPos)
{
  vector<Move> moveList;

  //Logic for if we aren't pinned
  if (!isPinned(isWhite, xPos, yPos))
  {
    //upRight check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, 1, 0);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //upLeft check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, -1, 0);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downLeft check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, 0, -1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downRight check
    vector<Move> temp = recursiveMoveCheck(isWhite, xPos, yPos, 0, 1);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
  }
  else
  {
    //First find allied king
    int kingX = -1;
    int kingY = -1;
    for (int i = 0; i < 8 && kingX == -1; i++)
    {
      for (int j = 0; j < 8 && kingX == -1; j++)
      {
        if ((board[i][j] == isWhite ? 'K' : 'k'))
        {
          kingX = i;
          kingY = j;
        }
      }
    }
    //Logic to run the recursiveRestrictedMoveCheck function
    //upRight check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, 0, 1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //upLeft check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, 0, -1, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downLeft check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, -1, 0, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
    //downRight check
    vector<Move> temp = recursiveRestrictedMoveCheck(isWhite, xPos, yPos, 1, 0, kingX, kingY);
    moveList.insert(moveList.end(), temp.begin(), temp.end());
  }

  return moveList;
}

//Performs a check for all valid bishop moves from given position and color
vector<Move> chessBoard::knightMoves(bool isWhite, int xPos, int yPos)
{
  vector<Move> moveList;
  //Knights can't move and maintain a pin
  if (isPinned(isWhite, xPos, yPos)) return moveList;
  //Check all 8 knight moves.
  else
  {
    //List of all moves
    int moves[8][2] = { {2,1}, {2,-1}, {-2, 1}, {-2, -1}, {1, 2}, {-1, 2}, {-1, -2}, {1, -2} };
    for (int i = 0; i < 8; i++)
    {
      //For each move check if: space is open or enemy and inside the bounds of the 
      if (xPos + moves[i][0] < 8 && xPos + moves[i][0] > 0 && yPos + moves[i][1] < 8 && yPos + moves[i][1] > 0)
        if (board[xPos + moves[i][0]][yPos + moves[i][1]] == ' ' || isupper(board[xPos + moves[i][0]][yPos + moves[i][1]]) == isWhite) moveList.push_back(Move(xPos, yPos, xPos + moves[i][0], yPos + moves[i][1]));
    }
  }
}

//Performs a check for all valid bishop moves from given position and color
vector<Move> chessBoard::pawnMoves(bool isWhite, int xPos, int yPos)
{
  int incY = isWhite ? 1 : -1;
  if (isPinned(isWhite, xPos, yPos))
  {
    //First find allied king
    int kingX = -1;
    int kingY = -1;
    for (int i = 0; i < 8 && kingX == -1; i++)
    {
      for (int j = 0; j < 8 && kingX == -1; j++)
      {
        if ((board[i][j] == isWhite ? 'K' : 'k'))
        {
          kingX = i;
          kingY = j;
        }
      }
    }
    //If we are pinned from the side then there is no move a pawn can make that would allow it to maintain the pin, thus we don't check for it
    //If pinned from front we can only move forward
    if (xPos == kingX)
    {
      if (board[xPos][yPos + incY] == ' ')
      {
        moveList.push_back(Move(xPos, yPos, xPos, yPos + incY));
        //Then 2 spaces
        if (board[xPos][yPos + 2 * incY] == ' ') moveList.push_back(Move(xPos, yPos, xPos, yPos + 2 * incY));
      }
    }
    //If pinned diagnolly check for take/enpessant from that direction
    else
    {
      int normKingX = (kingX - xPos) / abs(kingX - xPos); //1 = right -1 = left
      int normKingY = (kingY - yPos) / abs(kingY - yPos);
      //First if is to make sure that the xpos is in the board
      if (xPos + (normKingX * normKingY) < 0 || xPos + (normKingX * normKingY) > 7);
      else if (board[xPos + (normKingX * normKingY)][yPos + incY] != ' ' && isupper(board[xPos + 1][yPos + incY]) == !isWhite)
      {
        moveList.push_back(Move(xPos, yPos, xPos + (normKingX * normKingY), yPos + incY));
      }
      //Else check enpessant take
      else if (enPassant[0] != 'Z')
      {
        Move temp = Move('A', yPos, enPassant[0], int(enPassant[1]));

        if ((temp.getEndXInt() + 1 == xPos || temp.getEndXInt() - 1 == xPos) && temp.endPosY == yPos + incY && isupper(board[temp.getEndXInt()][temp.endPosY]) != isWhite)
        {
          //We have a valid opportunity to enpassant now we need to get the correct direction
          moveList.push_back(Move(xPos, yPos, temp.getEndXInt(), temp.endPosY));
        }
      }
    }
  }
  else
  {
    //First check if can move forward 1 space
    if (board[xPos][yPos + incY] == ' ')
    {
      moveList.push_back(Move(xPos, yPos, xPos, yPos + incY));
      //Then 2 spaces
      if(board[xPos][yPos+2*incY] == ' ') moveList.push_back(Move(xPos, yPos, xPos, yPos + 2*incY));
    }
    //Then check takes
    if (xPos != 0 && toupper(board[xPos-1][yPos+incY]) != ' ' && isupper(board[xPos-1][yPos+incY]) == isWhite) moveList.push_back(Move(xPos, yPos, xPos - 1, yPos + incY));
    if (xPos != 7 && toupper(board[xPos + 1][yPos + incY]) != ' ' && isupper(board[xPos + 1][yPos + incY]) == isWhite) moveList.push_back(Move(xPos, yPos, xPos + 1, yPos + incY));
    //Then check impasaunts 
    if (enPassant[0] != 'Z')
    {
      //We use this move to convert from char to array location, jenk and should not be done. Enpassant storage should be modified instead
      Move temp = Move('A', yPos, enPassant[0], int(enPassant[1]));
      if ((temp.getEndXInt() + 1 == xPos || temp.getEndXInt() - 1 == xPos) && temp.endPosY == yPos + incY && isupper(board[temp.getEndXInt()][temp.endPosY]) != isWhite)
      {
        //We have a valid opportunity to enpassant now we need to get the correct direction
        moveList.push_back(Move(xPos, yPos, temp.getEndXInt(), temp.endPosY));
      }
    }
  }
  return moveList;
}

//Performs a check for all valid queen moves from given position and color
//Takes advantage of the fact that queen is just a bishop and rook combined
vector<Move> chessBoard::queenMoves(bool isWhite, int xPos, int yPos)
{
  vector<Move> moveList;
  vector<Move> temp;
  temp = rookMoves(isWhite, xPos, yPos);
  moveList.insert(moveList.end(), temp.begin(), temp.end());
  
  temp = bishopMoves(isWhite, xPos, yPos);
  moveList.insert(moveList.end(), temp.begin(), temp.end());

  return moveList;
}

//Performs a check for all valid bishop moves from given position and color
vector<Move> chessBoard::kingMoves(bool isWhite, int xPos, int yPos)
{
  //Fuck me need to check all possible checking directions so you can't move into check
}




//Converts the board to FEN format and returns that string
string chessBoard::exportFENBoard()
{

  return "";
}

//Returns the move list
vector<Move> chessBoard::getMoveList()
{


  return moveList;
}

//Makes a move on the board, quicker than regenerating the entire move list
void chessBoard::makeMove(Move move)
{

}