// Title        : Crossover.h
// Purpose      : Reproduces candidates into offspring and fixes duplication
// Author       : Matthew Jacques
// Date         : 21/1/2016


#pragma once


#include <vector>               // Ability to create vector


// Readability for methods of crossover
enum CrossoverType { ONEPOINT, TWOPOINT };

// Readability for methods of selection
enum SelectionType { ROULETTE, TOURNAMENT };

struct Board;              // Usage of board types
struct PuzzlePiece;        // Usage of puzzle piece types

class Crossover
{

private:
  CrossoverType crossType;              // Holds what crossover method to use
  SelectionType selectType;             // Holds method of candidate selection
  int tournamentSize;                   // Holds how large tournaments are
  int eliteRate;                        // Holds how many candidates to keep

  // Selects with candidates to use for reproduction with the selection method  
  // chosen when the application was started
  void SelectParents(Board* parents[2],                         // *Out*
                     int popSize,                               // *In*
                     int totalFitness);                         // *In*

  // Selects candidates via the roulette wheel method mentioned within the report
  // in chapter 3
  void RouletteSelect(Board* parents[2],                        // *Out*
                      int fitness);                             // *In*

  // Selects candidates via the tournament selection method mentioned within the
  // report in chapter 3. Does not remove candidate from selection
  // after being selected so candidate can be in tournament multiple times.
  void TournamentSelect(Board* parents[2],                      // *Out*
                        int popSize);                           // *In*

  // Calls whichever crossover method that has been selected during the start of
  // the application
  void Reproduce(Board* parents[2]);                            // *In*

  // Adds multiple pieces from the parent to appropriate offspring (parent1 
  // into offpsing1, parent2 into offspring2). Number of pieces is passed in
  // as a parameter
  void CopyPieces(int numOfPieces,                              // *In*
                  int index[2],                                 // *In*
                  Board* parent1,                               // *In*
                  Board* parent2,                               // *In*
                  Board* offspring1,                            // *Out*
                  Board* offspring2);                           // *Out*

  // Takes two candidates, selects a point of the candidate to slice and exchanges
  // the data after that point with the second parent, explained fully in the
  // report, chapter 3
  void OnePoint(Board* parents[2]);                             // *In*

  // Takes two candidates, selects two points of the candidate to slice and
  // exchanges the data after that point with the second parent, switching
  // again after the second point. Explained fully in the report, chapter 3.
  void TwoPoint(Board* parents[2]);                             // *In*

  // Scans through candidate to check if puzzle pieces end up appearing twice
  // within the same candidate, taking the duplicate list from one candidate
  // to place pieces within the candidate that no longer has them
  void CheckDuplication();

  // Scans through the candidate board to see if there are any pieces that appear
  // more than once within the candidate, uses vector to store puzzle pieces
  void GetDuplicates(Board* pBoard,                             // *In*
                     std::vector<PuzzlePiece>* pieces,          // *Out*
                     std::vector<std::vector<int>>* indexes);   // *Out*

  // Checks all corners to see if there are any duplicate pieces within the
  // corner slots, if so store the piece and the index of the duplicate slot
  void CheckCorners(Board* pBoard,                              // *In*
                    std::vector<PuzzlePiece>* pieces,           // *Out*
                    std::vector<std::vector<int>>* indexes,     // *Out*
                    int boardSize,                              // *In*
                    std::vector<bool>* checkIDs);               // *In-Out*

  // Checks all edges to see if there are any duplicate pieces within the
  // edge slots, if so store the piece and the index of the duplicate slot
  void CheckEdges(Board* pBoard,                                // *In*
                  std::vector<PuzzlePiece>* pieces,             // *Out*
                  std::vector<std::vector<int>>* indexes,       // *Out*
                  int boardSize,                                // *In*
                  std::vector<bool>* checkIDs);                 // *In-Out*

  // Checks all inner slots to see if there are any duplicate pieces within the
  // inner slots, if so store the piece and the index of the duplicate slot
  void CheckInners(Board* pBoard,                               // *In*
                   std::vector<PuzzlePiece>* pieces,            // *Out*
                   std::vector<std::vector<int>>* indexes,      // *Out*
                   int boardSize,                               // *In*
                   std::vector<bool>* checkIDs);                // *In-Out*

  // Uses the vector of pieces along with the vector of slot indexes to place
  // pieces that are needed to to have every unique puzzle piece with in the
  // candidate
  void FixDuplicates(Board* pBoard,                             // *In-Out*
                     std::vector<PuzzlePiece> pieces,           // *In*
                     std::vector<std::vector<int>> indexes);    // *In*

  // Adds the best and worst candidtes from the previous generation to the new
  // generation. The amount of candidates is declared in eliteRate
  void DoElitism();

public:
  // Sets tournament size, initalise methods to default
  Crossover();

  // Sets the crossover and selection type to use for crossover along with
  // the elitism rate
  void SetMethod(CrossoverType cross,                           // *In*
                 SelectionType select,                          // *In*
                 int elite);                                    // *In*

  // Selects the parent candidates then commences with crossover with chosen
  // methods
  void DoCrossover(int popSize);                                // *In*

}; // Crossover