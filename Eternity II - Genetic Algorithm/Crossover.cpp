// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"          // Class declaration
#include "GeneticAlgorithm.h"   // Random number generation
#include <iostream>             // Console output
#include <algorithm>            // Sorting functionality


Crossover::Crossover()
{ // Sets tournament size, initalise methods to default

  tournamentSize = 10;     // Set tournament size to 10
  crossType = TWOPOINT;    // Set default crossover to two point
  selectType = TOURNAMENT; // Set default selection to tournament
  eliteRate = 2;           // Set default elitism rate to 2

} // Crossover()


void Crossover::setMethod(CrossoverType cross,                        // *In*
                          SelectionType select,                       // *In*
                          int elite)                                  // *In*
{ // Sets the crossover and selection type to use for crossover along with
  // the elitism rate

  crossType = cross;    // Set the crossover method
  selectType = select;  // Srt the selection method
  eliteRate = elite;    // Set the elitism rate

} // setMethod()


void Crossover::doCrossover(int popSize)                              // *In*                           
{ // Selects the parent candidates then commences with crossover with chosen
  // methods

  int totalFitness = 0;      // Holds the combined fitness of all boards
  int popCount = 0;          // Holds how many offspring have been created
  Board* parents[2];         // Two boards that have been selected as parents

  if (selectType == ROULETTE)
  { // If using roulette method, work out the total fitness now so not working
    // out the total fitness every select
    for (Board i : *BoardManager::getInstance()->getOldPop())
    { // Loops through all boards and total up all fitness scores from boards
      totalFitness += i.fitScore;   // Add fitness to total
    }
  }

  while (popCount < popSize -(eliteRate * 2) - 1)
  { // While the new vector is not filled with the right population size
    // make more candidates

    do
    { // Loop to make sure both selected parents are not the same candidate
      selectParents(parents, popSize, totalFitness);
    } while (parents[0]->boardID == parents[1]->boardID);

    reproduce(parents);  // Breed the parents together
    checkDuplication();  // Check for any duplicate pieces

    popCount += 2;       // Increment offspring count by 2
  }

  doElitism(); // Transfer the elites over to new generation

} // doCrossover()


void Crossover::selectParents(Board* parents[2],                      // *Out*                
                              int popSize,                            // *In*
                              int totalFitness)                       // *In*
{ // Selects with candidates to use for reproduction with the selection method  
  // chosen when the application was started

  if (selectType == ROULETTE) 
  { // If roulette method was chosen, do roulette
    rouletteSelect(parents, totalFitness, popSize);
  }
  else if (selectType == TOURNAMENT) 
  { // If tournament method was chosen, do tournament
    tournamentSelect(parents, popSize);
  }
  else 
  { // Output error, method not recognised
    std::cout << "Selection method not recognised" << std::endl;
  }


} // selectParents()


void Crossover::rouletteSelect(Board* parents[2],                     // *Out* 
                               int totalFitness,                      // *In*
                               int popSize)                           // *In*
{ // Selects candidates via the roulette wheel method mentioned within the 
  // report in chapter 3

  int oldFitness = 0;     // Holds the older combined fitness score

  // Index to hold which parent the iteration relates to
  int boardIndex = (int)BoardManager::getInstance()->getOldPop()->size() - 1;

  // Holds two random indexes to use as indexes in total fitness
  int randomIndex[2] = { -1, -1 };
    
  // Generate two random indexes to use in total fitness
  GeneticAlgorithm::genRandomNum(0, totalFitness, &randomIndex[0]);
  GeneticAlgorithm::genRandomNum(0, totalFitness, &randomIndex[1]);

  totalFitness = 0;   // Set to 0 to accumulate total fitness again
  
  for (int i = 0; i < popSize; i++)
  { // Loops through all boards accumulating the fitness scores, if random 
    // number is between total fitness and the previous total fitness, set the 
    // ID of the parent

    oldFitness = totalFitness;    // Store the total fitness

    // Sum current total fitness and fitness of next candidate
    totalFitness += BoardManager::getInstance()->getOldPop()->at(i).fitScore;

    if (totalFitness >= randomIndex[0] && randomIndex[0] >= oldFitness)
    { // If section has been found for first parent, set ID
      parents[0] = &BoardManager::getInstance()->getOldPop()->at(i);
    }

    if (totalFitness >= randomIndex[1] && randomIndex[1] >= oldFitness)
    { // If section has been found for first parent, set ID
      parents[1] = &BoardManager::getInstance()->getOldPop()->at(i);
    }
  } // for i < getOldPop()

} // rouletteSelect()


void Crossover::tournamentSelect(Board* parents[2],                   // *Out*
                                 int popSize)                         // *In*
{ // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3. Does not remove candidate from selection
  // after being selected so candidate can be in tournament multiple times.

  int highfitness = -1;          // Holds the current highest fitness found
  int boardID = -1;              // Holds random index to compare fitness

  for (int i = 0; i < 2; i++)
  { // Loop to find both parents

    highfitness = -1;    // Set current highest fitness to -1
    boardID = -1;        // Set current board ID to -1

    for (int j = 0; j < tournamentSize; j++)
    { // Test 20 random candidates storing highest fitness and ID

      // Generate a random boardID then 
      GeneticAlgorithm::genRandomNum(0, popSize - 1, &boardID);

      if ((BoardManager::getInstance()->getOldPop()->at(boardID).fitScore) > 
          highfitness)
      { // Test to see if the fitness score of that candidate is highest than the
        // current stored fitness

        // Store board ID
        parents[i] = &BoardManager::getInstance()->getOldPop()->at(boardID);

        // Store current highest fitness
        highfitness = parents[i]->fitScore;
      }

    } // for j < tournamentSize

  } // for i < 2

} // tournamentSelect()


void Crossover::reproduce(Board* parents[2])                          // *In*
{ // Calls whichever crossover method that has been selected during the start of
  // the application

  if (crossType == ONEPOINT)
  { // If crossover is one point, do one point
    onePoint(parents);
  }
  else if (crossType == TWOPOINT)
  { // If crossover is two point, do two point
    twoPoint(parents);
  }
  else
  { // Output error of crossover not recognised
    std::cout << "Crossover method not recognised" << std::endl;
  }

} // reproduce()


void Crossover::copyPieces(int numOfPieces,                           // *In*
                           int index[2],                              // *In*
                           Board* parent1,                            // *In*
                           Board* parent2,                            // *In*
                           Board* offspring1,                         // *Out*
                           Board* offspring2)                         // *Out*
{ // Adds multiple pieces from the parent to appropriate offspring (parent1 
  // into offpsing1, parent2 into offspring2). Number of pieces is passed in
  // as a parameter

  for (int i = 0; i < numOfPieces; i++)
  { // Copy pieces from the parent to the corrosponding offspring

    // Copy piece from parent1 into offspring1
    offspring1->boardVecs[index[0]].push_back(parent1->
                                              boardVecs[index[0]][index[1]]);

    // Copy piece from parent2 into offspring2
    offspring2->boardVecs[index[0]].push_back(parent2->
                                              boardVecs[index[0]][index[1]]);

    index[0]++;  // Increment the xIndex to move to next slot in row

    if (index[0] == BoardManager::getInstance()->getSize() + 1)
    { // If we have reached the end of the line of the board, increment row
      index[0] = 0;
      index[1]++;
    }
  }

} // copyPiece()


void Crossover::onePoint(Board* parents[2])                           // *In*
{ // Takes two candidates, selects a point of the candidate to slice and 
  // exchanges the data after that point with the second parent, explained 
  // fully in the report, chapter 3

  Board offspring[2];         // Holds the two new offspring boards
  int index[2] = { 0, 0 };    // Index of the current piece to copy over

  // Work out number of pieces to avoid calculations for each check below.
  // + 1 to include the 0 index
  int numOfPieces = (BoardManager::getInstance()->getSize() + 1) *
                    (BoardManager::getInstance()->getSize() + 1);

  int crossPoint = -1;     // Holds the crossover point

  // Get random crossover point to split the boards
  GeneticAlgorithm::genRandomNum(1, numOfPieces - 1, &crossPoint);

  // Initialise new empty boards for both offspring
  BoardManager::getInstance()->initEmptyBoard(&offspring[0]);
  BoardManager::getInstance()->initEmptyBoard(&offspring[1]);

  // Copy pieces from parent1 to offspring1 and parent2 to offspring2 until
  // the crossover point has been reached
  copyPieces(crossPoint, index, parents[0], parents[1], &offspring[0], 
              &offspring[1]);

  // Copy pieces from parent1 to offspring2 and parent2 to offspring1 until
  // the end of the board
  copyPieces((numOfPieces - crossPoint), index, parents[0], parents[1], 
              &offspring[1], &offspring[0]);
  
  for (int i = 0; i < 2; i++)
  { // Set boardID and push the new offspring on to the new population

    // Set boardID
    offspring[i].boardID = (int)BoardManager::getInstance()->getPop()->
      size() + 1;

    // Push on to board vector
    BoardManager::getInstance()->getPop()->push_back(offspring[i]);
  }

} // onePoint()


void Crossover::twoPoint(Board* parents[2])                           // *In*
{ // Takes two candidates, selects two points of the candidate to slice and
  // exchanges the data after that point with the second parent, switching
  // again after the second point. Explained fully in the report, chapter 3.

  Board offspring[2];             // Holds the two new offspring boards
  int index[2] = { 0, 0 };        // xIndex of the current piece to copy over
  int crossPoint[2] = { -1, -1 }; // Holds the two crossover points

  // Work out number of pieces to avoid calculations for each check below.
  // + 1 to include the 0 index
  int numOfPieces = (BoardManager::getInstance()->getSize() + 1) *
                    (BoardManager::getInstance()->getSize() + 1);

  // Get two random crossover points to split the boards, making sure the second
  // generated number is after the first
  GeneticAlgorithm::genRandomNum(1, numOfPieces - 1, &crossPoint[0]);
  GeneticAlgorithm::genRandomNum(crossPoint[0] + 1, numOfPieces, 
                                 &crossPoint[1]);

  // Initialise new empty boards and give the boards an ID
  BoardManager::getInstance()->initEmptyBoard(&offspring[0]);
  BoardManager::getInstance()->initEmptyBoard(&offspring[1]);

  // Copy pieces from parent1 to offspring1 and parent2 to offspring2 until
  // the crossover point has been reached
  copyPieces(crossPoint[0], index, parents[0], parents[1], &offspring[0],
             &offspring[1]);

  // Copy pieces from parent1 to offspring2 and parent2 to offspring1 until
  // the end of the board
  copyPieces((crossPoint[1] - crossPoint[0]), index, parents[0], parents[1],
             &offspring[1], &offspring[0]);

  // Copy pieces from parent1 to offspring1 and parent2 to offspring2 until
  // the crossover point has been reached
  copyPieces((numOfPieces - crossPoint[1]), index, parents[0], parents[1], 
             &offspring[0], &offspring[1]);

  for (int i = 0; i < 2; i++)
  { // Set boardID and push the new offspring on to the new population

    // Set boardID
    offspring[i].boardID = (int)BoardManager::getInstance()->getPop()->
      size() + 1;

    // Push on to board vector
    BoardManager::getInstance()->getPop()->push_back(offspring[i]);
  }

} // twoPoint()


void Crossover::checkDuplication()
{ // Scans through candidate to check if puzzle pieces end up appearing twice
  // within the same candidate, taking the duplicate list from one candidate
  // to place pieces within the candidate that no longer has them

  // Holds location of both offspring
  Board* offspring[2];   

  // Holds list of duplicate pieces found
  std::vector<PuzzlePiece> pieces[2];  

  // Holds the indexes of duplicate pieces
  std::vector<std::vector<int>> indexes[2];

  // Get the addresses of the last two boards created
  offspring[0] = &BoardManager::getInstance()->getPop()->at(
                    BoardManager::getInstance()->getPop()->size() - 2);
  offspring[1] = &BoardManager::getInstance()->getPop()->at(
                    BoardManager::getInstance()->getPop()->size() - 1);

  for (int i = 0; i < 2; i++)
  { // Call to find out which pieces are duplicates, storing in the pieces
  // and index vectors ready for fixing
    getDuplicates(offspring[i], &pieces[i], &indexes[i]);
  }

  for (int i = 0; i < 2; i++)
  { // Calls to repair the boards after duplication was found, giving the pieces
  // and indexes of the duplicate slots, pieces vector switched to add pieces
  // that were found twice in other offspring
    fixDuplicates(offspring[i], pieces[1 - i], indexes[i]);
  }

} // checkDuplication()


void Crossover::getDuplicates(Board* theBoard,                        // *In* 
                              std::vector<PuzzlePiece>* pieces,       // *Out*
                              std::vector<std::vector<int>>* indexes) // *Out*
{
  // Store the boardSize for easier code readability and quicker access
  int boardSize = BoardManager::getInstance()->getSize();

  // Vector to check IDs against
  std::vector<bool> isFound((*BoardManager::getInstance()->getPieces())[CORNER].
                            size() +
                             (*BoardManager::getInstance()->getPieces())[EDGE].
                            size() +
                             (*BoardManager::getInstance()->getPieces())[INNER].
                            size(), 
                             false);

  // Check the corner slots for duplicates
  checkCorners(theBoard, pieces, indexes, boardSize, &isFound);

  // Check the edge slots for duplicates
  checkEdges(theBoard, pieces, indexes, boardSize, &isFound);

  // Check the inner slots for duplicates
  checkInners(theBoard, pieces, indexes, boardSize, &isFound);

} // getDuplicates()


void Crossover::checkCorners(Board* theBoard,                         // *In*
                             std::vector<PuzzlePiece>* pieces,        // *Out*
                             std::vector<std::vector<int>>* indexes,  // *Out*
                             int boardSize,                           // *In*
                             std::vector<bool>* isFound)              // *In*
{ // Checks all corners to see if there are any duplicate pieces within the
  // corner slots, if so store the piece and the index of the duplicate slot

  // Set the ID of the piece in the top left corner to true, can't be duplicate
  // as is the first corner to be checked
  isFound->at(theBoard->boardVecs[0][0].pieceID - 1) = true;

  if (!isFound->at(theBoard->boardVecs[0][boardSize].pieceID - 1))
  { // Check the top right corner to see if that piece ID has already been found
    isFound->at(theBoard->boardVecs[0][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(theBoard->boardVecs[0][boardSize]); // Store piece
    std::vector<int> index = { 0, boardSize };            // Create index
    indexes->push_back(index);                            // Store index
  }

  if (!isFound->at(theBoard->boardVecs[boardSize][0].pieceID - 1))
  { // Check the bottom left corner to see if that piece ID has already been 
    // found
    isFound->at(theBoard->boardVecs[boardSize][0].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(theBoard->boardVecs[boardSize][0]); // Store piece
    std::vector<int> index = { boardSize, 0 };            // Create index
    indexes->push_back(index);                            // Store idnex
  }

  if (!isFound->at(theBoard->boardVecs[boardSize][boardSize].pieceID - 1))
  { // Check the bottom right corner to see if that piece ID has already been 
    // found
    isFound->at(theBoard->boardVecs[boardSize][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(theBoard->boardVecs[boardSize][boardSize]); // Store piece
    std::vector<int> index = { boardSize, boardSize };           // Create index
    indexes->push_back(index);                                    // Store index
  }

} // checkCorners()


void Crossover::checkEdges(Board* theBoard,                          // *In*
                           std::vector<PuzzlePiece>* pieces,         // *Out*
                           std::vector<std::vector<int>>* indexes,   // *Out*
                           int boardSize,                            // *In*
                           std::vector<bool>* isFound)               // *In-Out*
{ // Checks all edges to see if there are any duplicate pieces within the
  // edge slots, if so store the piece and the index of the duplicate slot

  for (int i = 1; i <= boardSize - 1; i++)
  { // Loop through as many times as there are edge slots on a edge checking
    // for duplicates on one slot of each edge at a time
    
    if (!isFound->at(theBoard->boardVecs[0][i].pieceID - 1))
    { // If ID on top edge has not already been found, set to found
      isFound->at(theBoard->boardVecs[0][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(theBoard->boardVecs[0][i]);      // Store piece
      std::vector<int> index = { 0, i};                  // Create index
      indexes->push_back(index);                         // Store index
    }

    if (!isFound->at(theBoard->boardVecs[i][0].pieceID - 1))
    { // If ID on left edge has not already been found, set to found
      isFound->at(theBoard->boardVecs[i][0].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(theBoard->boardVecs[i][0]);      // Store piece
      std::vector<int> index = { i, 0 };                 // Create index
      indexes->push_back(index);                         // Store index
    }

    if (!isFound->at(theBoard->boardVecs[i][boardSize].pieceID - 1))
    { // If ID on right edge has not already been found, set to found
      isFound->at(theBoard->boardVecs[i][boardSize].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(theBoard->boardVecs[i][boardSize]); // Store piece
      std::vector<int> index = { i, boardSize };            // Create index
      indexes->push_back(index);                            // Store index
    }

    if (!isFound->at(theBoard->boardVecs[boardSize][i].pieceID - 1))
    { // If ID on bottom edge has not already been found, set to found
      isFound->at(theBoard->boardVecs[boardSize][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(theBoard->boardVecs[boardSize][i]);  // Store piece
      std::vector<int> index = { boardSize, i };             // Create index
      indexes->push_back(index);                             // Store index
    }

  } // for i < boardSize - 2

} // checkEdges()


void Crossover::checkInners(Board* theBoard,                         // *In*
                            std::vector<PuzzlePiece>* pieces,        // *Out*
                            std::vector<std::vector<int>>* indexes,  // *Out*
                            int boardSize,                           // *In*
                            std::vector<bool>* isFound)              // *In-Out*
{ // Checks all inner slots to see if there are any duplicate pieces within the
  // inner slots, if so store the piece and the index of the duplicate slot

  int yIndex = 1;  // Y index for slot
  int xIndex = 1;  // X index for slot

  for (int i = 0; i < (int)(*BoardManager::getInstance()->getPieces())[INNER].
       size(); i++)
  { // Loop through for every inner slot changing the appropriate element
    // to true if piece was found

    if (!isFound->at(theBoard->boardVecs[xIndex][yIndex].pieceID - 1))
    { // If piece has not already been found, change element to true
      isFound->at(theBoard->boardVecs[xIndex][yIndex].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(theBoard->boardVecs[xIndex][yIndex]); // Store piece
      std::vector<int> index = { xIndex, yIndex };            // Create index
      indexes->push_back(index);                              // Store index
    }

    xIndex++;    // Increment X index to move to the slot on the right

    if (xIndex == BoardManager::getInstance()->getSize())
    { // If we have reached the end of the line of the board, increment
      // to next line
      xIndex = 1;   // Set x back to 1
      yIndex++;     // Increment y for next row
    }
  }

} // checkInners()


void Crossover::fixDuplicates(Board* theBoard,                       // *In-Out*
                              std::vector<PuzzlePiece> pieces,       // *In*
                              std::vector<std::vector<int>> indexes) // *In*   
{ // Uses the vector of pieces along with the vector of slot indexes to place
  // pieces that are needed to to have every unique puzzle piece with in the
  // candidate

  for (int i = 0; i < (int)pieces.size(); i++)
  { // Loop right placing the next piece that needs to be placed within the
    // board to the next slot with a duplicate piece and call to fix orientation
    theBoard->boardVecs[indexes[i][0]][indexes[i][1]] = pieces[i];
    
    if (theBoard->boardVecs[indexes[i][0]][indexes[i][1]].type == CORNER ||
        theBoard->boardVecs[indexes[i][0]][indexes[i][1]].type == EDGE)
    { // If piece is an edge or corner piece, make sure piece is properly 
      // rotated
      BoardManager::getInstance()->fixOrien(&theBoard->boardVecs[indexes[i][0]]
                                            [indexes[i][1]], indexes[i][0],
                                            indexes[i][1]);
    }
  }

} // fixDuplicates()


void Crossover::doElitism()
{ // Adds the best and worst candidtes from the previous generation to the new
  // generation. The amount of candidates is declared in eliteRate

  // Sort the vector in ascending order to for easy access to elites
  std::sort(BoardManager::getInstance()->getOldPop()->begin(),
            BoardManager::getInstance()->getOldPop()->end());

  for (int i = 1; i < eliteRate + 1; i++)
  { // Take the best and the worst candidates from the previous generation
    // and push to the new generation vector

    // Push worst on to new population
    BoardManager::getInstance()->getPop()->push_back(BoardManager::getInstance()
                                                     ->getOldPop()->begin()[i]);

    // Push best on to new population
    BoardManager::getInstance()->getPop()->push_back(BoardManager::getInstance()
                                                     ->getOldPop()->end()[-i]);
  }

} // doElitism()