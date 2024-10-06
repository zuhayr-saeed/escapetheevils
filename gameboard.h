
// Gameboard.h


#ifndef _GAMEBOARD_H
#define _GAMEBOARD_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <ctime>
#include <stdexcept>
#include <cstring>

#include "boardcell.h"
#include "grid.h"

using namespace std;

class GameBoard {
  private: 
      Grid<BoardCell*> board;
        size_t numRows;
        size_t numCols;
        int HeroRow; // Hero's position row
      int HeroCol; // Hero's position column
        int numMonsters;
        int numSuperMonsters;
        int numAbysses;
        int numBats;
        bool wonGame; // false, unless the Hero reached the exit successfully

  public: 
    /* default constructor */
        GameBoard() {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 50;
            numBats = 2;
            wonGame = false;

            this -> numRows = 15;
            this -> numCols = 40;

            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;

            blankBoard();
        }

        /* param constructor */
        GameBoard(size_t numRows, size_t numCols) {
            numMonsters = 4;
            numSuperMonsters = 2;
            numAbysses = 20;
            numBats = 3;
            wonGame = false;

            this -> numRows = numRows;
            this -> numCols = numCols;

            Grid<BoardCell*> boardnew(numRows, numCols);
            board = boardnew;

            blankBoard();
        }

        /* destructor */
        virtual ~GameBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    delete board(row, col);
                }
            }
        }

        void blankBoard() {
            for (size_t row = 0; row < board.numrows(); row++) {
                for (size_t col = 0; col < board.numcols(row); col++) {
                    board(row, col) = new Nothing(row,col);
                }
            }
        }

        char getCellDisplay(size_t r, size_t c) {
            return board(r,c)->display();
        }

        void setCell(BoardCell* myCell, size_t r, size_t c) {
            board(r,c) = myCell;
        }

        void freeCell(size_t r, size_t c) {
            delete board(r,c);
        }

        // fills board with by randomly placing...
        //  - Hero (H) in the first three columns
        //  - EscapeLadder (*) in last three columns
        //  - 3 vertical Walls (+), each 1/2 of board height, in middle segment
        //  - Abyss cells (#), quantity set by numAbysses, in middle segment
        //  - Baddies [Monsters (m), Super Monsters (M), & Bats (~)] in middle segment;
        //    number of Baddies set by numMonsters, numSuperMonsters, & numBats
        void setupBoard(int seed) {
            srand(seed);
            size_t r,c;

            r = rand() % numRows;
            c = rand() % 3;
            delete board(r,c);
            board(r,c) = new Hero(r,c);
            HeroRow = r;
            HeroCol = c;

            r = rand() % numRows;
            c = numCols - 1 - (rand() % 3);
            delete board(r,c);
            board(r,c) = new EscapeLadder(r,c);

            int sizeMid = numCols - 6;

            c = 3 + (rand() % sizeMid);
            for (r = 0; r < numRows/2; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }
            size_t topc = c;

            while (c == topc || c == topc-1 || c == topc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows-1; r > numRows/2; --r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);           
            }
            size_t botc = c;

            while (c == topc || c == topc-1 || c == topc+1 || c == botc || c == botc-1 || c == botc+1) {
                c = 3 + (rand() % sizeMid);
            }
            for (r = numRows/4; r < 3*numRows/4; ++r) {
                delete board(r,c);
                board(r,c) = new Wall(r,c);
            }

            for (int i = 0; i < numMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c);  
                board(r,c)->setPower(1);        
            }

            for (int i = 0; i < numSuperMonsters; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Monster(r,c); 
                board(r,c)->setPower(2);               
            }

            for (int i = 0; i < numBats; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Bat(r,c); 
            }

            for (int i = 0; i < numAbysses; ++i) {
                r = rand() % numRows;
                c = 3 + (rand() % sizeMid);
                while (board(r,c)->display() != ' ') {
                    r = rand() % numRows;
                    c = 3 + (rand() % sizeMid);
                }
                delete board(r,c);
                board(r,c) = new Abyss(r,c);              
            }
        }

        // neatly displaying the game board 
    void display( ) {
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;
            for (size_t row = 0; row < board.numrows(); row++) {
                cout << '|';
                for (size_t col = 0; col < board.numcols(row); col++) {
                    cout << board(row,col)->display();
                }
                cout << '|';
                cout << endl;
            }
            cout << '-';
            for (size_t col = 0; col < board.numcols(0); col++) {
                cout << '-';
            }
            cout << '-';
            cout << endl;

        }

        bool getWonGame() {
            return wonGame;
        }

        // distributing total number of monsters so that 
        //  ~1/3 of num are Super Monsters (M), and
        //  ~2/3 of num are Regular Monsters (m)
        void setNumMonsters(int num) {
            numSuperMonsters = num/3;
            numMonsters = num - numSuperMonsters;
        }

        void setNumAbysses(int num) {
            numAbysses = num;
        }

        void setNumBats(int num) {
            numBats = num;
        }

        size_t getNumRows() {
            return numRows;
        }

        size_t getNumCols() {
            return numCols;
        }


        //---------------------------------------------------------------------------------
        // void getHeroPosition(size_t& row, size_t& col)
        //
        // getter for Hero's position
    
        //---------------------------------------------------------------------------------
        void getHeroPosition(size_t& row, size_t& col) {

            row = static_cast<size_t>(HeroRow);
            col = static_cast<size_t>(HeroCol);
        }


        //---------------------------------------------------------------------------------
        // void setHeroPosition(size_t row, size_t col)
        //
        // setter for Hero's position
        //---------------------------------------------------------------------------------
        void setHeroPosition(size_t row, size_t col) {

            HeroRow = static_cast<int>(row);
            HeroCol = static_cast<int>(col);

        }


        //---------------------------------------------------------------------------------
        // findHero()
        //
        // updater for Hero's position
        // this function should find Hero in board and update
        //      HeroRow and HeroCol with the Hero's updated position;
        // if Hero cannot be found in board, then set Hero's position to (-1,-1)
        //---------------------------------------------------------------------------------
        void findHero() {
            for (size_t r = 0; r < board.numrows(); ++r) {
                for (size_t c = 0; c < board.numcols(r); ++c) {
                    if (board(r, c)->display() == 'H') {
                        HeroRow = static_cast<int>(r);
                        HeroCol = static_cast<int>(c);
                        return; // Hero found, exit loop
                    }
                }
            }
            // If Hero is not found, set position to (-1, -1)
            HeroRow = -1;
            HeroCol = -1;

        }


        //---------------------------------------------------------------------------------
        // bool makeMoves(char HeroNextMove)
        // 
        // This is the primary gameplay operation for a single round of the game. 
        // General steps:
        //  - Allow user to input their next move 
        //  - Get the attempted move position for the Hero
        //  - Move the hero, catching/resolving any potential collision exceptions...
        //      - attempted move out-of-bounds: change row &/or col to stay on board
        //      - attempted move into a barrier: change row &/or col to stay off barrier
        //      - attempted move to the exit: remove hero from board, hero escaped!
        //      - attempted move into a hole: remove hero from board, hero did not escape
        //      - attempted move to a baddie: remove hero from board, hero did not escape
        //      - attempted move to an empty space: actual move is the attempted move
        //  - For each baddie (regular Monster, super Monster, or Bat) on the board...
        //      - check that this baddies hasn't already moved this round
        //      - get its attempted move position
        //      - move the baddie, catching/resolving any potential collision exceptions...
        //          - attempted move out-of-bounds: change row &/or col to stay on board
        //          - attempted move into a barrier: change row &/or col to stay off barrier
        //          - attempted move to the exit: change row &/or col to stay off exit
        //          - attempted move into a hole: remove baddie from board
        //          - attempted move to hero: remove hero from board, hero did not escape
        //          - attempted move to a baddie: ignore attempted move, no position change
        //          - attempted move to an empty space: actual move is the attempted move 
        // return true if Hero is still on board at the end of the round
        // return false if Hero is NOT on board at the end of the round
        //---------------------------------------------------------------------------------
        bool makeMoves(char HeroNextMove) {

            // Determine where the Hero proposes to move to
            bool capHero = false;
            bool RowCol;
            size_t newR, newC;
            // Set the next move for the Hero on the board
            board(HeroRow, HeroCol)->setNextMove(HeroNextMove);
            // Attempt to move the Hero to the new position
            board(HeroRow, HeroCol)->attemptMoveTo(newR, newC, HeroRow, HeroCol);

            // Move the Hero, catching/resolving any potential collision exceptions
            try {
                // Hero attempts to move out-of-bounds in rows
                if (newR < 0 || newR >= numRows) {
                    // Adjust row to stay on the board
                    newR = (newR < 0) ? 0 : (newR >= numRows) ? numRows - 1 : newR;
                    RowCol = true;
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid row");
                }

                // Hero attempts to move out-of-bounds in columns
                if (newC < 0 || newC >= numCols) {
                    // Adjust column to stay on the board
                    newC = (newC < 0) ? 0 : (newC >= numCols) ? numCols - 1 : newC;
                    RowCol = false;
                    throw runtime_error("Hero trying to move out-of-bounds with an invalid column");
                }

                // Check the type of cell the Hero is attempting to move to
                char targetCell = board(newR, newC)->display();

                // Handle different types of cells
                if (targetCell == ' ') {
                    // The actual move is the attempted move to an empty cell
                    delete board(newR, newC);
                    board(newR, newC) = board(HeroRow, HeroCol);
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    HeroRow = static_cast<int>(newR);
                    HeroCol = static_cast<int>(newC);
                } else if (targetCell == '*') {
                    // Hero reached the exit, remove the Hero from the board
                    delete board(HeroRow, HeroCol);
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    HeroRow = -1;
                    HeroCol = -1;
                    wonGame = true;
                } else if (targetCell == '#') {
                    // Hero landed on an Abyss, remove the Hero from the board
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    HeroRow = -1;
                    HeroCol = -1;
                    wonGame = false; // Losing result
                } else if (targetCell == 'm' || targetCell == 'M' || targetCell == '~') {
                    // Hero landed on a cell occupied by a Baddie, remove the Hero from the board
                    board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                    HeroRow = -1;
                    HeroCol = -1;
                    wonGame = false; // Losing result
                } else if (targetCell == '+') {
                    // Handle special case when Hero encounters a Wall
                    if (!(HeroNextMove == 'w' || HeroNextMove == 'x' || HeroNextMove == 'a' || HeroNextMove == 'd')) {
                        if (board(newR, HeroCol)->display() != '+') {
                            if (board(newR, HeroCol)->display() == ' ') {
                                // Move the Hero vertically if the Wall is not in the way
                                board(newR, HeroCol) = board(HeroRow, HeroCol);
                                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                                HeroRow = static_cast<int>(newR);
                            } else {
                                // Hero collided with another non-empty cell, losing the game
                                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                                HeroRow = -1;
                                HeroCol = -1;
                                wonGame = false;
                            }
                        } else {
                            if (board(HeroRow, newC)->display() == ' ') {
                                // Move the Hero horizontally if the Wall is not in the way
                                board(HeroRow, newC) = board(HeroRow, HeroCol);
                                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                                HeroCol = static_cast<int>(newC);
                            } else {
                                // Hero collided with another non-empty cell, losing the game
                                board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                                HeroRow = -1;
                                HeroCol = -1;
                                wonGame = false;
                            }
                        }
                    }
                }
            } catch (out_of_range &oor) {
                cout << oor.what() << endl;
            } catch (runtime_error &excpt) {
                // Handle collision exceptions
                cout << excpt.what() << endl;
                newR = HeroRow;
                bool end;

                // Handle diagonal movement (q, e, z, c) in case of collision
                if (HeroNextMove == 'q' || HeroNextMove == 'e' || HeroNextMove == 'z' || HeroNextMove == 'c') {
                    if (!RowCol) {
                        if (HeroNextMove == 'q' || HeroNextMove == 'e') {
                            if (HeroNextMove == 'q') {
                                newC = 0;
                            }
                            newR = HeroRow - 1;
                            end = true;
                        } else if (HeroNextMove == 'z' || HeroNextMove == 'c') {
                            if (HeroNextMove == 'z') {
                                newC = 0;
                            }
                            newR = HeroRow + 1;
                            end = true;
                        }
                    } else {
                        if (HeroNextMove == 'q' || HeroNextMove == 'z') {
                            newC = HeroCol - 1;
                            end = true;
                        } else if (HeroNextMove == 'e' || HeroNextMove == 'c') {
                            newC = HeroCol + 1;
                            end = true;
                        }
                    }
                    if (end && board(newR, newC)->display() == ' ') {
                        // Move the Hero to the new position if it's an empty cell
                        delete board(newR, newC);
                        board(newR, newC) = board(HeroRow, HeroCol);
                        board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                        HeroRow = static_cast<int>(newR);
                        HeroCol = static_cast<int>(newC);
                    } else if (end && board(newR, newC)->display() == '*') {
                        // Hero reached the exit, remove the Hero from the board
                        board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                        HeroRow = -1;
                        HeroCol = -1;
                        wonGame = true;
                    } else if (end && board(newR, newC)->display() == '+') {
                        // Hero encountered a Wall, stay in the same position
                        HeroRow = static_cast<int>(newR);
                        HeroCol = static_cast<int>(newC);
                    } else if (end) {
                        // Hero collided with another non-empty cell, losing the game
                        board(HeroRow, HeroCol) = new Nothing(HeroRow, HeroCol);
                        HeroRow = -1;
                        HeroCol = -1;
                        wonGame = false;
                    }
                }
            }
            catch (...) {
                    cout << "An unknown error occurred." << endl;
            }

            // Update Baddies and check for their moves
            // Reset the moved flag for all cells at the beginning of each round
            for (size_t i = 0; i < numRows; ++i) {
                for (size_t j = 0; j < numCols; ++j) {
                    if (board(i, j)->isBaddie()) {
                        board(i, j)->setMoved(false);
                    }
                }
            }

            // Iterate through each cell to check Baddies' moves
            for (size_t row = 0; row < numRows; ++row) {
                for (size_t col = 0; col < numCols; ++col) {
                    // Check if the cell is a Baddie and has not moved in this round
                    if (board(row, col)->isBaddie() && !board(row, col)->getMoved()) {
                        // Get the attempted move position for the Baddie
                        size_t newR, newC;
                        board(row, col)->attemptMoveTo(newR, newC, HeroRow, HeroCol);

                        try {
                            // Baddie attempts to move out-of-bounds in rows
                            if (newR < 0 || newR >= numRows) {
                                newR = (newR < 0) ? 0 : (newR >= numRows) ? numRows - 1 : newR;
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid row");
                            }

                            // Baddie attempts to move out-of-bounds in columns
                            if (newC < 0 || newC >= numCols) {
                                newC = (newC < 0) ? 0 : (newC >= numCols) ? numCols - 1 : newC;
                                throw runtime_error("Baddie trying to move out-of-bounds with an invalid column");
                            }

                            // Check the type of cell the Baddie is attempting to move to
                            char targetCell = board(newR, newC)->display();

                            // Handle different types of cells
                            if (targetCell == ' ') {
                                // The actual move is the attempted move to an empty cell
                                delete board(newR, newC);
                                board(newR, newC) = board(row, col);
                                board(row, col) = new Nothing(row, col);
                                board(newR, newC)->setRow(newR);
                                board(newR, newC)->setCol(newC);
                                board(newR, newC)->setMoved(true);
                            } else if (targetCell == '#') {
                                // Baddie landed on an Abyss, remove the Baddie from the board
                                delete board(row, col);
                                board(row, col) = new Nothing(row, col);
                                board(row, col)->setMoved(true);
                            } else if (targetCell == 'H') {
                                // Baddie landed on the Hero, remove the Hero from the board
                                newR = HeroRow;
                                newC = HeroCol;
                                delete board(HeroRow, HeroCol);
                                board(HeroRow, HeroCol) = board(row, col);
                                board(row, col) = new Nothing(row, col);
                                board(newR, newC)->setRow(newR);
                                board(newR, newC)->setCol(newC);
                                board(newR, newC)->setMoved(true);
                                capHero = true;
                            } else if (targetCell == '+' || targetCell == '*') {
                                // Baddie landed on a Wall or EscapeLadder, do not move
                                board(row, col)->setMoved(true);
                            } else if (targetCell == 'm' || targetCell == 'M' || targetCell == '~') {
                                // Baddie landed on another Baddie, do not move
                                board(row, col)->setMoved(true);
                            }
                        } catch (out_of_range &oor) {
                            cout << oor.what() << endl;
                            // Handle out-of-range exception
                        } catch (runtime_error &excpt) {
                            // Handle collision exceptions for Baddies
                            cout << excpt.what() << endl;
                        } catch (...) {
                            cout << "An unknown error occurred." << endl;
                            // Handle unknown error
                        }
                    }
                }
            }
            if (capHero) {
                // If Hero was captured by a Baddie, remove the Hero from the board
                HeroRow = -1;
                HeroCol = -1;
            }

            // Return true if Hero is still on the board, false otherwise
            return (HeroRow != -1 && HeroCol != -1);
        }
};

#endif //_GAMEBOARD_H
