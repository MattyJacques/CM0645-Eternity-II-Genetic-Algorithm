// Title        : FileHandler.h
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#pragma once


#include "BoardManager.h"     // For PieceType & piece vector
#include "GeneticAlgorithm.h" // Settings struct
#include <fstream>            // Includes file input function
#include <string>             // Includes strings and stoi
#include <vector>             // Include vectors for filenames


class FileHandler
{

private:
  std::fstream theFile;               // File open to read from
  std::vector<std::string> filenames; // Holds all filenames held in directory
  static std::string outFilename;     // Holds filename of output file

  // Opens the file using the filename provided return whether successful
  bool openFile(const char* fileName);                // *In*

  // Scans the directory for puzzle files, storing names in array for loading
  // if the user wants to use one of them
  void scanFileDirectory();
 
  // Parse int from next line of file placing value in int passed as parameter
  void parseInt(int* setting);                        // *Out*

  // Parse double from next line of file placing value in double passed as
  // parameter
  void parseDouble(double* setting);                  // *Out*

  // Parses the methods of selection, crossover and mutation from the int in
  // file into the enum values
  void parseMethods(Settings* setData);               // *Out*

  // Find the correct filename from the vector of puzzle file names found
  // during the directory scan
  int getDataFilename(int size,                       // *In*
                      int pattern);                   // *In*

  // Takes string of data and parses into the array of integers to use to create		
  // the puzzle piece		
  void parseData(std::string line,                    // *In*
                 int pData[5]);                       // *Out*

  // Creates a new puzzle piece and stores in the puzzle piece vector
  void createPiece(int pData[5]);                     // *In*

  // Checks to see what type of piece is currently being read, returning the
  // answer
  PieceType checkType(int* pData);                    // *In*

  // Calculate the output filename
  void setOutFilename(int boardSize,                  // *In*
                      int patternNum,                 // *In*
                      int select,                     // *In*
                      int crossover,                  // *In*
                      int mutation);                  // *In*

  // Outputs the board to file using the pattern IDs so user can see the
  // matches for themselves
  void outputMatches(Board* pBoard,                   // *In*
                     int genCount);                   // *In*

  // Output the board to file using the piece IDs and orientations so the user
  // can see which piece does where with the current solution
  void outputIDs(Board* pBoard,                       // *In*
                 int genCount);                       // *In*

  // Append the selection method and crossover method to filename
  void appendSelectCross(int select,                  // *In*
                         int crossover);              // *In*

  // Append the mutation method to filename
  void appendMutation(int mutation);                  // *In*

  // If no data file was found, generate a new random board and make the data
  // file that corrosponds to that board
  void makeDataFile(int size,                         // *In*
                    int pattern);                     // *In*

  // Creates the data file by outputting the pieces to the file one piece per
  // line
  void outputDataFile(std::string filename);          // *In*

public:
  // Calls to scan the directory for piece data files
  FileHandler();

  // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm
  Settings readSettingsFile();

  // Reads the piece file with the file name that matches the information passed
  // as parameter storing piece info in the piece collection vector
  void readDataFile(int size,                         // *In*
                    int pattern);                     // *In*

  // Output the board to a file to show progress or solved board, file name is
  // date, generation and time ran.
  void outputBoard(Board* pBoard,                     // *In* 
                   int genCount);                     // *In*

  // Calculate the output filename
  void outputFitness(int fitness);                    // *In*

}; // FileHandler

