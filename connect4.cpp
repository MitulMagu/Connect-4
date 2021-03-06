// Connect-4 code

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <algorithm>

class gameStatus 	
{ 
private:
  long * gameData;

public: 
  long * gameBoard[6];
  long currentTurn;
  int player1Score;
  int player2Score;
  int pieceCount;
  FILE *gameFile;

  gameStatus()
  {
    gameData = new long[42];
    gameBoard[0] = &(gameData[0]);
    gameBoard[1] = &(gameData[7]);
    gameBoard[2] = &(gameData[14]);
    gameBoard[3] = &(gameData[21]);
    gameBoard[4] = &(gameData[28]);
    gameBoard[5] = &(gameData[35]);

    int i;
    for (i = 0; i < 42; i++)
    {
      gameData[i] = 0;
    }

    currentTurn = 1;
    player1Score = 0;
    player2Score = 0;
    pieceCount = 0;
    gameFile = 0;
  }

  ~gameStatus()
  {
    delete [] gameData;
  }
};

//int MinimaxDecision(gameStatus currentGame);
float MaxValue(gameStatus &currentGame,float alpha,float beta,int depth_count);
float MinValue(gameStatus &currentGame,float alpha,float beta,int depth_count);
void countScore(gameStatus &currentGame);
int ai_player = 2;					//Is AI player 1 or 2. Player 2 by default
int depth = 5, depth_counter = 0, depth_limit = 7^5;	//The depth limit is 5 by default

// Output current game status to console
void printGameBoard(gameStatus &currentGame)
{
  int i = 0;
  int j = 0;
  printf(" -----------------\n");
  for(i = 0; i < 6; i++)
  {	
    printf(" | ");
    for(j = 0; j < 7; j++)
    {
      printf("%li ", currentGame.gameBoard[i][j]);
    }
    printf("| \n");
  }
  printf(" -----------------\n   ");
  for(i=1;i<8;i++)
    printf("^ ");
  printf("\n   ");
  for(i=1;i<8;i++)
    printf("%d ",i);
  printf("\n\n");
}

// Output current game status to file
void printGameBoardToFile(gameStatus &currentGame)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 6; i++)
	{	
		for(j = 0; j < 7; j++)
		{
			fprintf(currentGame.gameFile, "%li", currentGame.gameBoard[i][j]);
		}
		fprintf(currentGame.gameFile, "\r\n");
	}
	fprintf(currentGame.gameFile, "%li\r\n", currentGame.currentTurn);			//%d -> %li
}

// Given a column and which player it is, 
// place that player's piece in the requested column
int playPiece(int column, gameStatus &currentGame)
{
	// if column full, return 1
	if(currentGame.gameBoard[0][column] != 0)
  {
		return 0;
  }
	
  int i;
	// starting at the bottom of the board, place the piece into the
	// first empty spot
	for(i = 5; i >= 0; i--)
	{
		if(currentGame.gameBoard[i][column] == 0)
		{
			currentGame.gameBoard[i][column] = currentGame.currentTurn;
			currentGame.pieceCount++;
				if(currentGame.currentTurn==1)				//added Switch turns
				currentGame.currentTurn=2;
				else if(currentGame.currentTurn==2)
				currentGame.currentTurn=1;
			return 1;
		}
	}
  return 0;
}

int TerminalTest(gameStatus &currentGame)
{
if(currentGame.pieceCount == 42) {
	return 1;
	}
	else
	return 0;
}

int Utility(gameStatus &currentGame)
{
int Utility_Value;
countScore(currentGame);
if(ai_player == 1)
Utility_Value = currentGame.player1Score - currentGame.player2Score;
if(ai_player == 2)
Utility_Value = currentGame.player2Score - currentGame.player1Score;
return Utility_Value;
}

gameStatus &Result(gameStatus &currentGame, int column)
{
  int i;
	// starting at the bottom of the board, place the piece into the
	// first empty spot
	for(i = 5; i >= 0; i--)
	{
		if(currentGame.gameBoard[i][column] == 0)
		{
			currentGame.gameBoard[i][column] = currentGame.currentTurn;
			currentGame.pieceCount++;
				if(currentGame.currentTurn==1)				//added Switch turns
				currentGame.currentTurn=2;
				else if(currentGame.currentTurn==2)
				currentGame.currentTurn=1;
			return currentGame;
		}
	}
  return currentGame;
}

float eval_func(gameStatus &currentGame)
{
  float result = 0;
  int fours1 = 0, fours2 = 0, threes1 = 0, threes2 = 0, twos1 = 0, twos2 = 0;

  //check horizontally for 4 in a Row
  int i;
  for(i = 0; i < 6; i++)
  {
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
			{fours1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
			{fours1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
			{fours1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
			{fours1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
			{fours2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
			{fours2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
			{fours2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
			{fours2++;}
	}

  //check horizontally for 3 in a Row
  for(i = 0; i < 6; i++)
  {
		//1st Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
			{threes1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
			{threes2++;}

		//2nd Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
			{threes1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
			{threes2++;}

		//3rd Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 1)
			{threes1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 1)
			{threes1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 2)
			{threes2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 2)
			{threes2++;}

		//4th Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0)
			{threes1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0)
			{threes1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 0)
			{threes1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 0)
			{threes1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0)
			{threes2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0)
			{threes2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 0)
			{threes2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 0)
			{threes2++;}
	}

 //check horizontally for 2 in a Row
  for(i = 0; i < 6; i++)
  {
		//1st Blank and 2nd Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
			{twos2++;}

		//2nd Blank and 3rd Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 1)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 2)
			{twos2++;}

		//3rd Blank and 4th Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 0)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 0)
			{twos2++;}

		//4th Blank and 1st Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 0)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 0)
			{twos2++;}

		//1st Blank and 3rd Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 1)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 1)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 0 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 0 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 0 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 0 && currentGame.gameBoard[i][5] == 2)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 0 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 0 && currentGame.gameBoard[i][6] == 2)
			{twos2++;}

		//2nd Blank and 4th Blank
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 0)
			{twos1++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 0)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 0 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 0 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 0 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 0)
			{twos2++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 0 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 0)
			{twos2++;}
	}

	//check vertically for 4 in a row
	int j;
	for(j = 0; j < 7; j++)
	{
		//check player 1
		if(currentGame.gameBoard[0][j] == 1 && currentGame.gameBoard[1][j] == 1 
			&& currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
			{fours1++;}
		if(currentGame.gameBoard[1][j] == 1 && currentGame.gameBoard[2][j] == 1 
			&& currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
			{fours1++;}
		if(currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1 
			&& currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
			{fours1++;}
		//check player 2
		if(currentGame.gameBoard[0][j] == 2 && currentGame.gameBoard[1][j] == 2 
			&& currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
			{fours2++;}
		if(currentGame.gameBoard[1][j] == 2 && currentGame.gameBoard[2][j] == 2 
			&& currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
			{fours2++;}
		if(currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2 
			&& currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
			{fours2++;}
	}

	//check vertically for 3 in a row
	for(j = 0; j < 7; j++)
	{
		//check player 1
		if(currentGame.gameBoard[0][j] == 0 && currentGame.gameBoard[1][j] == 1 
			&& currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
			{threes1++;}
		if(currentGame.gameBoard[1][j] == 0 && currentGame.gameBoard[2][j] == 1 
			&& currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
			{threes1++;}
		if(currentGame.gameBoard[2][j] == 0 && currentGame.gameBoard[3][j] == 1 
			&& currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
			{threes1++;}
		//check player 2
		if(currentGame.gameBoard[0][j] == 0 && currentGame.gameBoard[1][j] == 2 
			&& currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
			{threes2++;}
		if(currentGame.gameBoard[1][j] == 0 && currentGame.gameBoard[2][j] == 2 
			&& currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
			{threes2++;}
		if(currentGame.gameBoard[2][j] == 0 && currentGame.gameBoard[3][j] == 2 
			&& currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
			{threes2++;}
	}

	//check vertically for 2 in a row
	for(j = 0; j < 7; j++)
	{
		//check player 1
		if(currentGame.gameBoard[0][j] == 0 && currentGame.gameBoard[1][j] == 0 
			&& currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
			{twos1++;}
		if(currentGame.gameBoard[1][j] == 0 && currentGame.gameBoard[2][j] == 0 
			&& currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
			{twos1++;}
		if(currentGame.gameBoard[2][j] == 0 && currentGame.gameBoard[3][j] == 0 
			&& currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
			{twos1++;}
		//check player 2
		if(currentGame.gameBoard[0][j] == 0 && currentGame.gameBoard[1][j] == 0 
			&& currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
			{twos2++;}
		if(currentGame.gameBoard[1][j] == 0 && currentGame.gameBoard[2][j] == 0 
			&& currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
			{twos2++;}
		if(currentGame.gameBoard[2][j] == 0 && currentGame.gameBoard[3][j] == 0 
			&& currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
			{twos2++;}
	}

	//check diagonally for 4 in a row
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
			{fours1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
			{fours1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
			{fours1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
			{fours1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
			{fours1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
			{fours2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
			{fours2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
			{fours2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
			{fours2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
			{fours2++;}

	//check diagonally for 3 in a row
	//1st Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
			{threes1++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
			{threes2++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}

	//check diagonally for 3 in a row
	//2nd Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
			{threes1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
			{threes2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 0
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}

	//check diagonally for 3 in a row
	//3rd Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 1)
			{threes1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 1)
			{threes1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 1)
			{threes1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 1)
			{threes1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 1)
			{threes1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 2)
			{threes2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 2)
			{threes2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 2)
			{threes2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 2)
			{threes2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 2)
			{threes2++;}

	//check diagonally for 3 in a row
	//4th Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 0)
			{threes1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 0)
			{threes1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 0)
			{threes1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 0)
			{threes1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 0)
			{threes1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 0)
			{threes2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 0)
			{threes2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 0)
			{threes2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 0)
			{threes2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 0)
			{threes2++;}

	//check diagonally for 2 in a row
	//1st Blank and 2nd Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 0
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}

	//check diagonally for 3 in a row
	//2nd Blank and 3rd Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 1)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 2)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 0
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}

	//check diagonally for 2 in a row
	//3rd Blank and 4th Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 0)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 0)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}

	//check diagonally for 2 in a row
	//4th Blank and 1st Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 0)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 0)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}
	//check diagonally for 2 in a row
	//4th Blank and 2nd Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 0)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 0)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 0)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 0)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 0)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 0)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 0 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 0 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 0
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 0 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 0 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 0 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 0)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 0 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 0 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 0 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 0)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 0 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 0 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 0)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 0 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 0)
			{twos2++;}

	//check diagonally for 2 in a row
	//3rd Blank and 1st Blank
	//check player 1
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 1)
			{twos1++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 1)
			{twos1++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 1)
			{twos1++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 1)
			{twos1++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 1)
			{twos1++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 0 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][0] == 0 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][0] == 0 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][1] == 0 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][1] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][2] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 0 && currentGame.gameBoard[5][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][2] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][5] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 0 && currentGame.gameBoard[4][6] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][6] == 2)
			{twos2++;}

	if(currentGame.gameBoard[0][3] == 0 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 0 && currentGame.gameBoard[3][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][4] == 0 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 0 && currentGame.gameBoard[3][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][3] == 0 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 0 && currentGame.gameBoard[4][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][5] == 0 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][4] == 0 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 0 && currentGame.gameBoard[4][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][3] == 0 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 0 && currentGame.gameBoard[5][0] == 2)
			{twos2++;}
	if(currentGame.gameBoard[0][6] == 0 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][5] == 0 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 0 && currentGame.gameBoard[4][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][4] == 0 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 0 && currentGame.gameBoard[5][1] == 2)
			{twos2++;}
	if(currentGame.gameBoard[1][6] == 0 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 0 && currentGame.gameBoard[4][3] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][5] == 0 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 0 && currentGame.gameBoard[5][2] == 2)
			{twos2++;}
	if(currentGame.gameBoard[2][6] == 0 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 0 && currentGame.gameBoard[5][3] == 2)
			{twos2++;}

if(ai_player == 1)
result = (1*fours1) + (0.5*threes1) + (0.25*twos1) - (1*fours2) - (0.5*threes2) - (0.25*twos2);
if(ai_player == 2)
result = (1*fours2) + (0.5*threes2) + (0.25*twos2) - (1*fours1) - (0.5*threes1) - (0.25*twos1);
return result; 
}

float MaxValue(gameStatus &currentGame, float alpha, float beta, int depth_count)
{
float v = -1000;
int i, k, j, l, buffer;
countScore(currentGame);
depth_count++;
if(depth_count == depth)
{
return eval_func(currentGame);
}
if (TerminalTest(currentGame)==1)
{
return Utility(currentGame);
}
for(k=0;k<7;k++)
{
	buffer = 0;
	for(l=0;l<6;l++)
	{
	if(currentGame.gameBoard[l][k] == 0)
	buffer = 1;
	}
	if(buffer == 1)
	{
	gameStatus trialGame;
	for(i=0;i<6;i++)
	{
		for(j=0;j<7;j++)
		{
		trialGame.gameBoard[i][j] = currentGame.gameBoard[i][j];
		}
	}
	trialGame.pieceCount = currentGame.pieceCount;
	trialGame.currentTurn = currentGame.currentTurn;
	
	v = std::max(v,MinValue(Result(trialGame,k),alpha,beta,depth_count));
	if(v>=beta)
	return v;
	alpha = std::max(alpha,v);
	}
}
return v;
}

float MinValue(gameStatus &currentGame, float alpha, float beta, int depth_count)
{
float v = 1000;
int i, j, k, l, buffer = 0;
countScore(currentGame);
depth_count++;
if(depth_count == depth)
{
return eval_func(currentGame);
}
if (TerminalTest(currentGame)==1)
{
return Utility(currentGame);
}
for(k=0;k<7;k++)
{
	buffer = 0;
	for(l=0;l<6;l++)
	{
	if(currentGame.gameBoard[l][k] == 0)
	buffer = 1;
	}
	if(buffer == 1)
	{
	gameStatus trialGame;
	for(i=0;i<6;i++)
	{
		for(j=0;j<7;j++)
		{
		trialGame.gameBoard[i][j] = currentGame.gameBoard[i][j];
		}
	}
	trialGame.pieceCount = currentGame.pieceCount;
	trialGame.currentTurn = currentGame.currentTurn;

	v = std::min(v,MaxValue(Result(trialGame,k),alpha,beta,depth_count));
	if(v<=alpha)
	return v;
	beta = std::min(beta,v);
	}
}
return v;
}


int MinimaxDecision(gameStatus &currentGame)
{
int decision = 0, i, j, k = 0, check_back = 0, flag = 0, utility_point = 0, times = 0;
float move = 0, max = -1000, moves[6];
for(k=0;k<7;k++)
{
	gameStatus trialGame;
	for(i=0;i<6;i++)
	{
		for(j=0;j<7;j++)
		{
		trialGame.gameBoard[i][j] = currentGame.gameBoard[i][j];
		}
	}
	trialGame.pieceCount = currentGame.pieceCount;
	trialGame.currentTurn = currentGame.currentTurn;
	move = MinValue(Result(trialGame,k),-1000,1000,0);
	moves[k]=move;
	if(move > max)
	{
		max = move;
		decision = k;
	}
}
utility_point = decision;

check_again:
if(flag == 1)
{
moves[utility_point] = -1000;
for(i=0;i<7;i++)
{
max=std::max(max,moves[i]);
}
}
flag = 0;

check:
if((currentGame.gameBoard[0][decision]==0)&&(moves[decision]==max))
{
goto ok;
}
else
{
if((decision<6)&&(check_back!=1))
decision++;
else
check_back = 1;

if((decision>-1)&&(check_back == 1))
decision--;

else if((decision<0)&&(check_back == 1))
{
decision++;
flag = 1;
times++;
if(times>6)
{
goto ok;
}
goto check_again;
}
goto check;
}

ok:
printf("\nMinimax Decision taken: %d",decision+1);
return decision;
}

void aiPlay(gameStatus &currentGame)
{	
	int result = 0,i = 0,j = 0;
	gameStatus trialGame;
	for(i=0;i<6;i++)
	{
		for(j=0;j<7;j++)
		{
		trialGame.gameBoard[i][j] = currentGame.gameBoard[i][j];
		}
	}
	trialGame.pieceCount = currentGame.pieceCount;
	printf("\nAI is making decision..\n");
	result = MinimaxDecision(trialGame);
	playPiece(result,currentGame);
    printf("\n\nmove %d: Player %li, column %d\n", 					//added %li -> %d (in 2 arguments)
           currentGame.pieceCount, currentGame.currentTurn, result+1);
}

void countScore(gameStatus &currentGame)
{
  currentGame.player1Score = 0; 
  currentGame.player2Score = 0; 

  //check horizontally
  int i;
  for(i = 0; i < 6; i++)
  {
		//check player 1
		if(currentGame.gameBoard[i][0] == 1 && currentGame.gameBoard[i][1] == 1 
			&& currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1)
			{currentGame.player1Score++;}
		if(currentGame.gameBoard[i][1] == 1 && currentGame.gameBoard[i][2] == 1 
			&& currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1)
			{currentGame.player1Score++;}
		if(currentGame.gameBoard[i][2] == 1 && currentGame.gameBoard[i][3] == 1 
			&& currentGame.gameBoard[i][4] == 1 && currentGame.gameBoard[i][5] == 1)
			{currentGame.player1Score++;}
		if(currentGame.gameBoard[i][3] == 1 && currentGame.gameBoard[i][4] == 1 
			&& currentGame.gameBoard[i][5] == 1 && currentGame.gameBoard[i][6] == 1)
			{currentGame.player1Score++;}
		//check player 2
		if(currentGame.gameBoard[i][0] == 2 && currentGame.gameBoard[i][1] == 2 
			&& currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2)
			{currentGame.player2Score++;}
		if(currentGame.gameBoard[i][1] == 2 && currentGame.gameBoard[i][2] == 2 
			&& currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2)
			{currentGame.player2Score++;}
		if(currentGame.gameBoard[i][2] == 2 && currentGame.gameBoard[i][3] == 2 
			&& currentGame.gameBoard[i][4] == 2 && currentGame.gameBoard[i][5] == 2)
			{currentGame.player2Score++;}
		if(currentGame.gameBoard[i][3] == 2 && currentGame.gameBoard[i][4] == 2 
			&& currentGame.gameBoard[i][5] == 2 && currentGame.gameBoard[i][6] == 2)
			{currentGame.player2Score++;}
	}

	//check vertically
	int j;
	for(j = 0; j < 7; j++)
	{
		//check player 1
		if(currentGame.gameBoard[0][j] == 1 && currentGame.gameBoard[1][j] == 1 
			&& currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1)
			{currentGame.player1Score++;}
		if(currentGame.gameBoard[1][j] == 1 && currentGame.gameBoard[2][j] == 1 
			&& currentGame.gameBoard[3][j] == 1 && currentGame.gameBoard[4][j] == 1)
			{currentGame.player1Score++;}
		if(currentGame.gameBoard[2][j] == 1 && currentGame.gameBoard[3][j] == 1 
			&& currentGame.gameBoard[4][j] == 1 && currentGame.gameBoard[5][j] == 1)
			{currentGame.player1Score++;}
		//check player 2
		if(currentGame.gameBoard[0][j] == 2 && currentGame.gameBoard[1][j] == 2 
			&& currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2)
			{currentGame.player2Score++;}
		if(currentGame.gameBoard[1][j] == 2 && currentGame.gameBoard[2][j] == 2 
			&& currentGame.gameBoard[3][j] == 2 && currentGame.gameBoard[4][j] == 2)
			{currentGame.player2Score++;}
		if(currentGame.gameBoard[2][j] == 2 && currentGame.gameBoard[3][j] == 2 
			&& currentGame.gameBoard[4][j] == 2 && currentGame.gameBoard[5][j] == 2)
			{currentGame.player2Score++;}
	}

	//check diagonally
	
	//check player 1
	if(currentGame.gameBoard[2][0] == 1 && currentGame.gameBoard[3][1] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][3] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][0] == 1 && currentGame.gameBoard[2][1] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][3] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][4] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][0] == 1 && currentGame.gameBoard[1][1] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][1] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][4] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][5] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][1] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][2] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][5] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][5] == 1 && currentGame.gameBoard[5][6] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][2] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][5] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][5] == 1 && currentGame.gameBoard[4][6] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][6] == 1)
			{currentGame.player1Score++;}

	if(currentGame.gameBoard[0][3] == 1 && currentGame.gameBoard[1][2] == 1 
		&& currentGame.gameBoard[2][1] == 1 && currentGame.gameBoard[3][0] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][4] == 1 && currentGame.gameBoard[1][3] == 1 
		&& currentGame.gameBoard[2][2] == 1 && currentGame.gameBoard[3][1] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][3] == 1 && currentGame.gameBoard[2][2] == 1 
		&& currentGame.gameBoard[3][1] == 1 && currentGame.gameBoard[4][0] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][5] == 1 && currentGame.gameBoard[1][4] == 1 
		&& currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][4] == 1 && currentGame.gameBoard[2][3] == 1 
		&& currentGame.gameBoard[3][2] == 1 && currentGame.gameBoard[4][1] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][3] == 1 && currentGame.gameBoard[3][2] == 1 
		&& currentGame.gameBoard[4][1] == 1 && currentGame.gameBoard[5][0] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[0][6] == 1 && currentGame.gameBoard[1][5] == 1 
		&& currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][5] == 1 && currentGame.gameBoard[2][4] == 1 
		&& currentGame.gameBoard[3][3] == 1 && currentGame.gameBoard[4][2] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][4] == 1 && currentGame.gameBoard[3][3] == 1 
		&& currentGame.gameBoard[4][2] == 1 && currentGame.gameBoard[5][1] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[1][6] == 1 && currentGame.gameBoard[2][5] == 1 
		&& currentGame.gameBoard[3][4] == 1 && currentGame.gameBoard[4][3] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][5] == 1 && currentGame.gameBoard[3][4] == 1 
		&& currentGame.gameBoard[4][3] == 1 && currentGame.gameBoard[5][2] == 1)
			{currentGame.player1Score++;}
	if(currentGame.gameBoard[2][6] == 1 && currentGame.gameBoard[3][5] == 1 
		&& currentGame.gameBoard[4][4] == 1 && currentGame.gameBoard[5][3] == 1)
			{currentGame.player1Score++;}
		
	//check player 2
	if(currentGame.gameBoard[2][0] == 2 && currentGame.gameBoard[3][1] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][3] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][0] == 2 && currentGame.gameBoard[2][1] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][3] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][4] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][0] == 2 && currentGame.gameBoard[1][1] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][1] == 2 && currentGame.gameBoard[2][2] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][4] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][5] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][1] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][2] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][5] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][5] == 2 && currentGame.gameBoard[5][6] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][2] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][5] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][5] == 2 && currentGame.gameBoard[4][6] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][6] == 2)
			{currentGame.player2Score++;}

	if(currentGame.gameBoard[0][3] == 2 && currentGame.gameBoard[1][2] == 2 
		&& currentGame.gameBoard[2][1] == 2 && currentGame.gameBoard[3][0] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][4] == 2 && currentGame.gameBoard[1][3] == 2 
		&& currentGame.gameBoard[2][2] == 2 && currentGame.gameBoard[3][1] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][3] == 2 && currentGame.gameBoard[2][2] == 2
		&& currentGame.gameBoard[3][1] == 2 && currentGame.gameBoard[4][0] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][5] == 2 && currentGame.gameBoard[1][4] == 2 
		&& currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][4] == 2 && currentGame.gameBoard[2][3] == 2 
		&& currentGame.gameBoard[3][2] == 2 && currentGame.gameBoard[4][1] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][3] == 2 && currentGame.gameBoard[3][2] == 2 
		&& currentGame.gameBoard[4][1] == 2 && currentGame.gameBoard[5][0] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[0][6] == 2 && currentGame.gameBoard[1][5] == 2 
		&& currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][5] == 2 && currentGame.gameBoard[2][4] == 2 
		&& currentGame.gameBoard[3][3] == 2 && currentGame.gameBoard[4][2] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][4] == 2 && currentGame.gameBoard[3][3] == 2 
		&& currentGame.gameBoard[4][2] == 2 && currentGame.gameBoard[5][1] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[1][6] == 2 && currentGame.gameBoard[2][5] == 2 
		&& currentGame.gameBoard[3][4] == 2 && currentGame.gameBoard[4][3] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][5] == 2 && currentGame.gameBoard[3][4] == 2 
		&& currentGame.gameBoard[4][3] == 2 && currentGame.gameBoard[5][2] == 2)
			{currentGame.player2Score++;}
	if(currentGame.gameBoard[2][6] == 2 && currentGame.gameBoard[3][5] == 2 
		&& currentGame.gameBoard[4][4] == 2 && currentGame.gameBoard[5][3] == 2)
			{currentGame.player2Score++;}
}


int main(int argc, char ** argv)
{
  char ** command_line = argv;

  if (argc != 5)
  {
    printf("Four command-line arguments are needed:\n");
    printf("Usage: %s interactive [input_file] [computer-next/human-next] [depth]\n", command_line [0]);
    printf("or:  %s one-move [input_file] [output_file] [depth]\n", command_line [0]);

    return 0;
  }

  char * game_mode = command_line [1];
  int computer_done = 0;
  char * output = command_line [3];
  char * whose_next = command_line [3];
  char * given_depth_limit = command_line [4];
  depth = atoi(given_depth_limit);
  printf("Depth Limit: %d\n",depth);

  if (strcmp(game_mode, "interactive") == 0)
  {
  printf("Interactive mode activated!\n");

	if ((strcmp(whose_next, "human-next") != 0)&&(strcmp(whose_next, "computer-next") != 0))
  {
		printf("\nSpecify who will play next with 'computer-next' or 'human-next'. This is needed to proceed\n");
		return 0;
  }
  }

  else if (strcmp (game_mode, "one-move") == 0)
  {
  printf("One-move mode activated!\n");
	}

  else if ((strcmp (game_mode, "one-move") != 0)||(strcmp (game_mode, "interactive") != 0))
  {
    printf("%s is an unrecognized game mode\n", game_mode);
    return 0;
  }

  char * input = command_line[2];
  gameStatus currentGame;	 // Declare current game
  printf("\nConnect-4 Game\n");

  currentGame.gameFile = fopen(input, "r");
  printf("Game state before move:\n");
  
  //set currentTurn
  char current = 0;
  int i, j, choice = 1, ret = 0, turn = 0;
  if (currentGame.gameFile != 0)
  {
    for(i = 0; i < 6; i++)
    {
      for(j = 0; j < 7; j++)
      {
        do 
        {
          current = getc(currentGame.gameFile);
        }
        while ((current == ' ') || (current == '\n') || (current == '\r'));

        currentGame.gameBoard[i][j] = current - 48;
	if(currentGame.gameBoard[i][j] > 0)
        {
	  currentGame.pieceCount++;
	}
      }
    }
 
    do 
    {
      current = getc(currentGame.gameFile);
    }
    while ((current == ' ') || (current == '\n') || (current == '\r'));
    
    currentGame.currentTurn = current - 48;
    fclose(currentGame.gameFile);
  }

if(strcmp(whose_next, "computer-next") == 0)
ai_player = currentGame.currentTurn;
if(strcmp(whose_next, "human-next") == 0)
{
if(currentGame.currentTurn==2)
ai_player = 1;
if(currentGame.currentTurn==1)
ai_player = 2;
}

while(1) 
{
  turn++;
  printf("Turn: %d\n",turn);
  printGameBoard(currentGame);
  countScore(currentGame);
  printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);

  if(currentGame.pieceCount == 42)
  {
    printf("\nBOARD FULL\n");
    printf("Game over!\n");
		if(currentGame.player1Score > currentGame.player2Score)
			printf("Player 1 wins!");
		else if(currentGame.player1Score < currentGame.player2Score)
			printf("Player 2 wins!");
		else
			printf("DRAW!");
		printf("\n\n");
    return 1;
  }

	if((strcmp(whose_next,"human-next")==0)||(computer_done == 1))
	{
  again:
  printf("Where will you place token? (1 - 7)\n");
  scanf("%d",&choice);
  if(choice==0)
  exit(0);
  else if((choice>7)||(choice<1))
	{
	printf("\nIllegal Choice! Choose from 1 - 7 only\n");
	goto again;
  }
  ret=playPiece(choice-1,currentGame);
  if(ret==0)
  {
  printf("\nColumn Full! Choose another column\n");
  goto again;
  }
  printf("game state after move:\n");	//added section
  printGameBoard(currentGame);
  countScore(currentGame);
  printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
	}

  aiPlay(currentGame);
  printf("game state after AI made move:\n");
  printGameBoard(currentGame);
  countScore(currentGame);
  printf("Score: Player 1 = %d, Player 2 = %d\n\n", currentGame.player1Score, currentGame.player2Score);
	computer_done = 1;
  
  if (strcmp (game_mode, "one-move") == 0)
	{
  currentGame.gameFile = fopen(output, "w");
  if (currentGame.gameFile != 0)
  {
    printGameBoardToFile(currentGame);
    fclose(currentGame.gameFile);
  }
  else
  {
    printf("\nError: could not open output file %s\n", output);
  }
	goto done;
}
}
	done:
  return 1;
}
