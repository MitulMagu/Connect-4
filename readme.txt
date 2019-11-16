Name: Mitul Magu
Programming Language Used: C++

Structure of Code:
The code structure can be explained by first mentioning the functions and classes used -
1.class gameStatus is used store the information regarding the current game state such as player1 score, turn, board data. This information will be used to determine what next move the AI will make.
2.function printGameBoard is used to print the gameboard to the screen
3.function printGameBoardToFile is used to print the gameboard to the file
4.function playPiece is used to play the game by placing a piece in the specified column.
5.function TerminalTest checks if there are 42 pieces in the gameboard.
6.function Utility is used to return the utility value i.e. difference between opponent and player score.
7.function Result return the state after a piece is placed in specified column.
8.function eval_func is the eval function. Check eval_explanation.txt for its functioning.
9.function Maxvalue is the maxvalue function with alpha-beta pruning and return a utility value.
10.function Minvalue is the minvalue function with alpha-beta pruning and return a utility value.
11.function Minimax decision finds the choice that corresponds to the maximum utility according to the minimax algorithm.
12.function aiPlay is used for the AI to make a decision and will return a choice according to the minimax decision function.
13.function countScore counts the score at terminal nodes.

In the main function-
The main function first reads the state of the game from the given input and assigns the AI and human player as player 1 or 2.
The program checks if it is interactive or one-move mode and which player is next and then plays the turn. If the computer is next it simply asks the AI to play the game and displays the result.
If human is next, the program asks for an input, plays it and then displays the result. Note that pressing 0 will exit the game for convinience.
This is repeated till the game ends.

How to run the Code:
1. Save the connect4.txt file as connect4.cpp file.
2. Open Terminal on a Linux machine and install gcc-c++ if you don't already.
3. This can be done by first typing the command: sudo apt-get install yum
4. Press enter and once installed type: yum install gcc-c++
5. If this fails, type: sudo apt-get install g++
6. Once the compiler has been installed for cpp, change the directory to where the file belongs
7. As an example, if the connect4.cpp belongs in the Codes folder in your Desktop, type the command: cd Desktop/Codes 
8. Once the directory shown in your command line, enter the command: g++ connect4.cpp -o connect
9. Press enter and type: ./connect interactive [input_file] [computer-next/human-next] [depth] or ./connect one-move [input_file] [output_file] [depth]
10. This will execute the program once you press enter


Note:
The program has been intentionally been saved with a .txt extension. 
The program works correctly on Linux and has been tested on Ubuntu 14.04.1 as well as on Omega. 
Pressing 0 will intentionally exit the game for convinience.



 
