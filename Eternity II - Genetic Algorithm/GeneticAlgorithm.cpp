// Title        : GeneticAlgorithm.cpp
// Purpose      : Container for the GA
// Author       : Matthew Jacques
// Date         : 9/11/2015


#include "GeneticAlgorithm.h" // Include header for the class


// Initialise the pointer to the instance
GeneticAlgorithm* GeneticAlgorithm::pAlgorithm = nullptr;


GeneticAlgorithm::GeneticAlgorithm(double eliteRate, double crossRate, 
                                   double mutationRate, int population)
{ // Constructor that sets the elite, crossover and mutation rates, along with
  // the size of the population for each generation

  double total = 0;

  elitism = eliteRate;
  crossover = crossRate;
  mutation = mutationRate;
  popSize = population;
  

  total = elitism + crossover + mutation;

  elitism = (int)(elitism * popSize / total);
  crossover = (int)(crossover * popSize / total);
  mutation = (int)(mutation * popSize / total);

  pBoard = Board::GetInstance();

} // GeneticAlgorithm()


void GeneticAlgorithm::CreateInstance(double eliteRate, double crossRate,
                                      double mutationRate, int popSize)
{ // Creates the static instance for the class, passing over the variables
  // needed for the GA to the constructor

  if (!pAlgorithm)
  { // If the instance of the algorithm has not already been created, create
    // a new instance by calling the constructor
    pAlgorithm = new GeneticAlgorithm(eliteRate, crossRate, mutationRate, 
                                      popSize);
  }

} // CreateInstance()


GeneticAlgorithm* GeneticAlgorithm::GetInstance()
{ // Returns the current instance of the class

  return pAlgorithm; // Return the pointer

} // GetInstance()


void GeneticAlgorithm::RunGA()
{ // Main function of the GA that continually runs

  /*
  start
  set generation
  get population size
  randomly create first generation
  evaluate generation
  sort generation by fitness
  decide elites
  crossover
  mutation
  end 
  
  */

} // RunGA()


  // Needs rewritten, checks for corner slot twice @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int GeneticAlgorithm::CheckFitness() 
{ // Check the fitness of a candidate from the GA

  int score = 0;

  for (int i = 0; i <= pBoard->boardSize; i++)
  { // Loops through the Y positions of the board to check for fitness

    for (int j = 0; j <= pBoard->boardSize; j++)
    { // Loops through the X positions of the board to check for fitness

     if (i == pBoard->boardSize || j == pBoard->boardSize
         || i == 0 || j == 0)
     { // Test if we are looking at an edge slot of the board

        if (i == j == 0 || i == 0 && j == pBoard->boardSize ||
            i == pBoard->boardSize && j == 0 ||
            i == pBoard->boardSize && j == pBoard->boardSize)
        { // Test to see if we are looking at a corner slot of the board

          if (pBoard->boardVec[i][j].GetType() == CORNER)
          { // If the piece in the corner slot is a corner piece, add the score

            score += 25;

            if (i == j == 0)
            { // Check for top left corner

              if (pBoard->boardVec[i][j].GetRight() ==
                  pBoard->boardVec[i][j + 1].GetLeft())
              { // Check for the side of the corner for match, if so add score

                score += 10;
              }

              if (pBoard->boardVec[i][j].GetBottom() ==
                  pBoard->boardVec[i + 1][j].GetTop())
              { // Check for the bottom of corner matches, if so add score

                score += 10;
              }
            } 
            else if (i == 0 && j == pBoard->boardSize)
            { // Check for top right corner

              if (pBoard->boardVec[i][j - 1].GetRight() ==
                  pBoard->boardVec[i][j].GetLeft())
              { // Check for the side of the corner for match, if so add score

                score += 10;
              }

              if (pBoard->boardVec[i][j].GetBottom() ==
                  pBoard->boardVec[i + 1][j].GetTop())
              { // Check for the bottom of corner matches, if so add score

                score += 10;
              }
            } 
            else if (i == pBoard->boardSize && j == 0)
            { // Check for bottom left corner

              if (pBoard->boardVec[i][j].GetRight() ==
                  pBoard->boardVec[i][j + 1].GetLeft())
              { // Check for the side of the corner for match, if so add score

                score += 10;
              }

              if (pBoard->boardVec[i - 1][j].GetBottom() ==
                  pBoard->boardVec[i][j].GetTop())
              { // Check for the top of corner matches, if so add score

                score += 10;
              }
            }
            else if (i == pBoard->boardSize && j == pBoard->boardSize)
            { // Check for bottom right corner

              if (pBoard->boardVec[i][j - 1].GetRight() ==
                  pBoard->boardVec[i][j].GetLeft())
              { // Check for the side of the corner for match, if so add score

                score += 10;
              }

              if (pBoard->boardVec[i - 1][j].GetBottom() ==
                  pBoard->boardVec[i][j].GetTop())
              { // Check for the top of corner matches, if so add score

                score += 10;
              }
            } // bottom right corner
            
          } // If piece is corner
         
        }
        else if (pBoard->boardVec[i][j].GetType() == EDGE)
        { // If the piece on the edge is a edge piece, add score

          if (i == 0)
          {
            if (pBoard->boardVec[i][j].GetBottom() == 
                pBoard->boardVec[i + 1][j].GetTop())
            {
              score += 5;
            }
          }
          else if (j == 0)
          {
            if (pBoard->boardVec[i][j].GetBottom() ==
              pBoard->boardVec[i + 1][j].GetTop())
            {
              score += 5;
            }
          }
          else if (j == pBoard->boardSize)
          {
            if (pBoard->boardVec[i][j].GetLeft() ==
                pBoard->boardVec[i][j - 1].GetRight())
            {
              score += 5;
            }
          }
          else if (i == pBoard->boardSize)
          {
            if (pBoard->boardVec[i][j].GetTop() ==
                pBoard->boardVec[i - 1][j].GetBottom())
            {
              score += 5;
            }
          }


          score += 15;
        } // If edge / corner piece check
     
     } 
     else // If not an edge slot check for inner connection score
     {
       if (pBoard->boardVec[i][j].GetRight() ==
         pBoard->boardVec[i][j + 1].GetLeft())
       { // Check for the sides of the pieces for match, if so add score

         score++;
       }

       if (pBoard->boardVec[i][j].GetBottom() ==
         pBoard->boardVec[i + 1][j].GetTop())
       { // Check for the top and bottom matching, if so add score

         score++;
       }
     }

    } // for X increment

  } // for Y increment

  return score;
} // CheckFitness()


GeneticAlgorithm::~GeneticAlgorithm()
{ // Destructor that deletes the instance of the class

  // Delete the instance of the GA
  delete pAlgorithm;

  // Change to pointer to null
  pAlgorithm = nullptr;

} // ~GeneticAlgorithm()
