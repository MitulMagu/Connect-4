# AI Bases Connect-4 using Game Playing Algorithm

An AI based connect-4 game that allows the user to play against an AI opponent with varying levels of difficulty. The project uses the minimax algorithm that allows the AI to do a depth first search of the possible moves that the user can make and makes a decision based on which decision will have the highest outcome value. The project also incorporates alpha-beta pruning and eval functions to significantly reduce the time it takes to make a decision. The difficulty and response time can be modified by choosing an appropriate depth for the game playing algorithm to search, after which it makes a decision based on its eval function.

## Code Structure
The code structure can be explained by first mentioning the functions and classes used
1. Class gameStatus is used store the information regarding the current game state such as player1 score, turn, board data. This information will be used to determine what next move the AI will make.
2. Function printGameBoard is used to print the gameboard to the screen
3. Function printGameBoardToFile is used to print the gameboard to the file
4. Function playPiece is used to play the game by placing a piece in the specified column.
5. Function TerminalTest checks if there are 42 pieces in the gameboard.
6. Function Utility is used to return the utility value i.e. difference between opponent and player score.
7. Function Result return the state after a piece is placed in specified column.
8. Function eval_func is the eval function. Check eval_explanation.txt for its functioning.
9. Function Maxvalue is the maxvalue function with alpha-beta pruning and return a utility value.
10. Function Minvalue is the minvalue function with alpha-beta pruning and return a utility value.
11. Function Minimax decision finds the choice that corresponds to the maximum utility according to the minimax algorithm.
12. Function aiPlay is used for the AI to make a decision and will return a choice according to the minimax decision function.
13. Function countScore counts the score at terminal nodes.

## Main Function Description
The main function first reads the state of the game from the given input and assigns the AI and human player as player 1 or 2.
The program checks if it is interactive or one-move mode and which player is next and then plays the turn. If the computer is next it simply asks the AI to play the game and displays the result.
If human is next, the program asks for an input, plays it and then displays the result. Note that pressing 0 will exit the game for convinience.
This is repeated till the game ends.

## Running the Code
1. Download the connect4.cpp file.
2. Open Terminal on a Linux machine and install gcc-c++ if you don't already.
3. This can be done by first typing the command: ``sudo apt-get install yum``
4. Press enter and once installed type: ``yum install gcc-c++``
5. If this fails, type: ``sudo apt-get install g++``
6. Once the compiler has been installed for cpp, change the directory to where the file belongs
7. As an example, if the connect4.cpp belongs in the Codes folder in your Desktop, type the command: ``cd Desktop/Codes``
8. Once the directory shown in your command line, enter the command: ``g++ connect4.cpp -o connect``
9. Press enter and type: ``./connect interactive [input_file] [computer-next/human-next] [depth] or ./connect one-move [input_file] [output_file] [depth]``
10. This will execute the program once you press enter


Note: 
The program works correctly on Linux and has been tested on Ubuntu 14.04.1 as well as on Omega. 
Pressing 0 will intentionally exit the game for convinience.
