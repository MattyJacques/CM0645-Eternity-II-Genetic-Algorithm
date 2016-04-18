// Title        : Crossover.cpp
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#include "Crossover.h"
#include "GeneticAlgorithm.h"   // Random number generation
#include <iostream>             // Error output
#include <algorithm>            // Sorting functionality


Crossover::Crossover()
{ // Sets tournament size, initalise methods to default

  tournamentSize = 10;
  crossType = ONEPOINT;
  selectType = ROULETTE;

} // Crossover()


void Crossover::setMethod(CrossoverType cross,                        // *In*
                          SelectionType select,                       // *In*
                          int elite)                                  // *In*
{ // Sets the crossover and selection type to use for crossover along with
  // the elitism rate

  crossType = cross;
  selectType = select;
  eliteRate = elite;

} // setMethod()


void Crossover::doCrossover(int popSize)                              // *In*                           
{ // Selects the parent candidates then commences with crossover with chosen
  // methods

  int totalFitness = 0;      // Holds the combined fitness of all boards
  Board* parents[2];         // Two boards that have been selected as parents

  if (selectType == ROULETTE)
  { // If using roulette method, work out the total fitness now so not working
    // out the total fitness every select
    for (Board i : *BoardManager::getInstance()->getOldPop())
    { // Loops through all boards and total up all fitness scores from boards
      totalFitness += i.fitScore;
    }
  }

  while (BoardManager::getInstance()->getPop()->size() < popSize -
         (eliteRate * 2) - 1)
  { // While the new vector is not filled with the right population size
    // make more candidates

    // Loop to make sure both selected parents are not the same candidate
    do
    {
      selectParents(parents, popSize, totalFitness);
    } while (parents[0]->boardID == parents[1]->boardID);

    reproduce(parents);  // Breed the parents together
    checkDuplication();  // Check for any duplicate pieces
  }

  doElitism(); // Transfer the elites over to new generation

} // doCrossover()


void Crossover::selectParents(Board* parents[2],                      // *Out*                
                              int popSize,                            // *In*
                              int totalFitness)                       // *In*
{ // Selects with candidates to use for reproduction with the selection method  
  // chosen when the application was started

  if (selectType == ROULETTE) 
  { // If roulette method was chosen, work out the total fitness and do
    // roulette selection
    rouletteSelect(parents, totalFitness);
  }
  else if (selectType == TOURNAMENT) // If tournament method was chosen
  {
    tournamentSelect(parents, popSize);
  }
  else // Output error, method not recognised
  {
    std::cout << "Selection method not recognised" << std::endl;
  }


} // selectParents()


void Crossover::rouletteSelect(Board* parents[2],                     // *Out* 
                               int totalFitness)                      // *In*
{ // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3

  int oldFitness = 0;     // Holds the older combined fitness score

  // Index to hold which parent the iteration relates to
  int boardIndex = (int)BoardManager::getInstance()->getOldPop()->size() - 1;

  // Holds two random indexes to use as indexes in total fitness
  int randomIndex[2] = { -1, -1 };
    
  // Generate two random indexes to use in total fitness
  GeneticAlgorithm::GenRandomNum(0, totalFitness, &randomIndex[0]);
  GeneticAlgorithm::GenRandomNum(0, totalFitness, &randomIndex[1]);

  totalFitness = 0;   // Set to 0 to accumulate total fitness again
  
  for (unsigned int i = 0; i < BoardManager::getInstance()->getOldPop()->size(); i++)
  { // Loops through all boards accumulating the fitness scores, if random number
    // is between total fitness and the previous total fitness, set the ID of
    // the parent

    // Set the previous fitness and add on the fitness score of the next board
    oldFitness = totalFitness;
    totalFitness += BoardManager::getInstance()->getOldPop()->at(i).fitScore;

    // If section has been found for first parent, set ID
    if (totalFitness >= randomIndex[0] && randomIndex[0] >= oldFitness)
    {
      parents[0] = &BoardManager::getInstance()->getOldPop()->at(i);
    }

    // If section has been found for first parent, set ID
    if (totalFitness >= randomIndex[1] && randomIndex[1] >= oldFitness)
    {
      parents[1] = &BoardManager::getInstance()->getOldPop()->at(i);
    }
  } // for (unsigned int i = 0)

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

    // Initialise to -1 ready for next tournament
    highfitness = -1;
    boardID = -1;

    for (int j = 0; j < tournamentSize; j++)
    { // Test 20 random candidates storing highest fitness and ID

      // Generate a random boardID then 
      GeneticAlgorithm::GenRandomNum(0, popSize - 1, &boardID);

      // Test to see if the fitness score of that candidate is highest than the
      // current stored fitness
      if ((BoardManager::getInstance()->getOldPop()->at(boardID).fitScore) > highfitness)
      {
        parents[i] = &BoardManager::getInstance()->getOldPop()->at(boardID);
        highfitness = parents[i]->fitScore;
      }

    } // for j < tournamentSize

  } // for i < 2

} // tournamentSelect()


void Crossover::reproduce(Board* parents[2])                          // *In*
{ // Calls whichever crossover method that has been selected during the start of
  // the application

  if (crossType == ONEPOINT)
    onePoint(parents);
  else if (crossType == TWOPOINT)
    twoPoint(parents);
  else
    std::cout << "Crossover method not recognised" << std::endl;

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
{ // Takes two candidates, selects a point of the candidate to slice and exchanges
  // the data after that point with the second parent, explained fully in the
  // report, chapter 3

  Board offspring[2];         // Holds the two new offspring boards
  int index[2] = { 0, 0 };    // Index of the current piece to copy over

  // Work out number of pieces to avoid calculations for each check below.
  // + 1 to include the 0 index
  int numOfPieces = (BoardManager::getInstance()->getSize() + 1) *
                    (BoardManager::getInstance()->getSize() + 1);

  int crossPoint = -1;     // Holds the crossover point

  // Get random crossover point to split the boards
  GeneticAlgorithm::GenRandomNum(1, numOfPieces - 1, &crossPoint);

  // Initialise new empty boards
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

  // Get random crossover points to split the boards, making sure the second
  // generated number is after the first
  GeneticAlgorithm::GenRandomNum(1, numOfPieces - 1, &crossPoint[0]);
  GeneticAlgorithm::GenRandomNum(crossPoint[0] + 1, numOfPieces, 
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
  Board* pOffspring[2];   

  // Holds list of duplicate pieces found
  std::vector<PuzzlePiece> pieces[2];  

  // Holds the indexes of duplicate pieces
  std::vector<std::vector<int>> indexes[2];

  // Get the addresses of the last two boards created
  pOffspring[0] = &BoardManager::getInstance()->getPop()->at(
                    BoardManager::getInstance()->getPop()->size() - 2);
  pOffspring[1] = &BoardManager::getInstance()->getPop()->at(
                    BoardManager::getInstance()->getPop()->size() - 1);

  // Call to find out which pieces are duplicates, storing in the pieces
  // and index vectors ready for fixing
  for (int i = 0; i < 2; i++)
    getDuplicates(pOffspring[i], &pieces[i], &indexes[i]);

  // Calls to repair the boards after duplication was found, giving the pieces
  // and indexes of the duplicate slots, pieces vector switched to add pieces
  // that were found twice in other offspring
  for (int i = 0; i < 2; i++)
    fixDuplicates(pOffspring[i], pieces[1 - i], indexes[i]);

} // checkDuplication()


void Crossover::getDuplicates(Board* pBoard,                          // *In* 
                              std::vector<PuzzlePiece>* pieces,       // *Out*
                              std::vector<std::vector<int>>* indexes) // *Out*
{
  // Store the boardSize for easier code readability and quicker access
  int boardSize = BoardManager::getInstance()->getSize();

  // Vector to check IDs against
  std::vector<bool> checkIDs((*BoardManager::getInstance()->getPieces())[CORNER].size() +
                             (*BoardManager::getInstance()->getPieces())[EDGE].size() +
                             (*BoardManager::getInstance()->getPieces())[INNER].size(), 
                             false);

  // Check the corner slots for duplicates
  checkCorners(pBoard, pieces, indexes, boardSize, &checkIDs);

  // Check the edge slots for duplicates
  checkEdges(pBoard, pieces, indexes, boardSize, &checkIDs);

  // Check the inner slots for duplicates
  checkInners(pBoard, pieces, indexes, boardSize, &checkIDs);

} // getDuplicates()


void Crossover::checkCorners(Board* pBoard,                           // *In*
                             std::vector<PuzzlePiece>* pieces,        // *Out*
                             std::vector<std::vector<int>>* indexes,  // *Out*
                             int boardSize,                           // *In*
                             std::vector<bool>* checkIDs)             // *In*
{ // Checks all corners to see if there are any duplicate pieces within the
  // corner slots, if so store the piece and the index of the duplicate slot

  // Set the ID of the piece in the top left corner to true, can't be duplicate
  // as is the first corner to be checked
  checkIDs->at(pBoard->boardVecs[0][0].pieceID - 1) = true;

  // Check the top right corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVecs[0][boardSize].pieceID - 1))
  { 
    checkIDs->at(pBoard->boardVecs[0][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVecs[0][boardSize]);
    std::vector<int> index = { 0, boardSize };
    indexes->push_back(index);
  }

  // Check the bottom left corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVecs[boardSize][0].pieceID - 1))
  {
    checkIDs->at(pBoard->boardVecs[boardSize][0].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVecs[boardSize][0]);
    std::vector<int> index = { boardSize, 0 };
    indexes->push_back(index);
  }

  // Check the bottom right corner to see if that piece ID has already been found
  if (!checkIDs->at(pBoard->boardVecs[boardSize][boardSize].pieceID - 1))
  {
    checkIDs->at(pBoard->boardVecs[boardSize][boardSize].pieceID - 1) = true;
  }
  else
  { // Piece already found, add to duplicate pieces vector and index vector
    pieces->push_back(pBoard->boardVecs[boardSize][boardSize]);
    std::vector<int> index = { boardSize, boardSize };
    indexes->push_back(index);
  }

} // checkCorners()


void Crossover::checkEdges(Board* pBoard,                            // *In*
                           std::vector<PuzzlePiece>* pieces,         // *Out*
                           std::vector<std::vector<int>>* indexes,   // *Out*
                           int boardSize,                            // *In*
                           std::vector<bool>* checkIDs)              // *In-Out*
{ // Checks all edges to see if there are any duplicate pieces within the
  // edge slots, if so store the piece and the index of the duplicate slot

  for (int i = 1; i <= boardSize - 1; i++)
  { // Loop through as many times as there are edge slots on a edge checking
    // for duplicates on one slot of each edge at a time
    
    // If ID on top edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVecs[0][i].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVecs[0][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVecs[0][i]);
      std::vector<int> index = { 0, i};
      indexes->push_back(index);
    }

    // If ID on left edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVecs[i][0].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVecs[i][0].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVecs[i][0]);
      std::vector<int> index = { i, 0 };
      indexes->push_back(index);
    }

    // If ID on right edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVecs[i][boardSize].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVecs[i][boardSize].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVecs[i][boardSize]);
      std::vector<int> index = { i, boardSize };
      indexes->push_back(index);
    }

    // If ID on bottom edge has not already been found, set to found
    if (!checkIDs->at(pBoard->boardVecs[boardSize][i].pieceID - 1))
    {
      checkIDs->at(pBoard->boardVecs[boardSize][i].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVecs[boardSize][i]);
      std::vector<int> index = { boardSize, i };
      indexes->push_back(index);
    }

  } // for i < boardSize - 2

} // checkEdges()


void Crossover::checkInners(Board* pBoard,                           // *In*
                            std::vector<PuzzlePiece>* pieces,        // *Out*
                            std::vector<std::vector<int>>* indexes,  // *Out*
                            int boardSize,                           // *In*
                            std::vector<bool>* checkIDs)             // *In-Out*
{ // Checks all inner slots to see if there are any duplicate pieces within the
  // inner slots, if so store the piece and the index of the duplicate slot

  int yIndex = 1;  // Y index for slot
  int xIndex = 1;  // X index for slot

  for (int i = 0; i < (*BoardManager::getInstance()->getPieces())[INNER].size(); i++)
  { // Loop through for every inner slot changing the appropriate element
    // to true if piece was found

    if (!checkIDs->at(pBoard->boardVecs[xIndex][yIndex].pieceID - 1))
    { // If piece has not already been found, change element to true
      checkIDs->at(pBoard->boardVecs[xIndex][yIndex].pieceID - 1) = true;
    }
    else
    { // Piece already found, add to duplicate pieces vector and index vector
      pieces->push_back(pBoard->boardVecs[xIndex][yIndex]);
      std::vector<int> index = { xIndex, yIndex };
      indexes->push_back(index);
    }

    xIndex++;    // Increment X index to move to the slot on the right

    if (xIndex == BoardManager::getInstance()->getSize())
    { // If we have reached the end of the line of the board, increment
      // to next line
      xIndex = 1;
      yIndex++;
    }
  }

} // checkInners()


void Crossover::fixDuplicates(Board* pBoard,                         // *In-Out*
                              std::vector<PuzzlePiece> pieces,       // *In*
                              std::vector<std::vector<int>> indexes) // *In*   
{ // Uses the vector of pieces along with the vector of slot indexes to place
  // pieces that are needed to to have every unique puzzle piece with in the
  // candidate

  for (int i = 0; i < pieces.size(); i++)
  { // Loop right placing the next piece that needs to be placed within the
    // board to the next slot with a duplicate piece and call to fix orientation
    pBoard->boardVecs[indexes[i][0]][indexes[i][1]] = pieces[i];
    
    if (pBoard->boardVecs[indexes[i][0]][indexes[i][1]].type == CORNER ||
        pBoard->boardVecs[indexes[i][0]][indexes[i][1]].type == EDGE)
    { // If piece is an edge or corner piece, make sure piece is properly rotated
      BoardManager::getInstance()->fixOrientation(&pBoard->boardVecs[indexes[i][0]]
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