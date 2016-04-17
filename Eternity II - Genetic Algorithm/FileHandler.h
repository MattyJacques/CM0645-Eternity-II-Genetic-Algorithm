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
  std::fstream theFile;              // File open to read from
  std::vector<std::string> filenames; // Holds all filenames held in directory
  static std::string outFilename;     // Holds filename of output file

  // Opens the file using the filename provided return whether successful
  bool OpenFile(const char* fileName);
 
  // Parse int from next line of file placing value in int passed as parameter
  void ParseInt(int* setting);

  // Parse double from next line of file placing value in double passed as
  // parameter
  void ParseDouble(double* setting);

  // Parses the methods of selection, crossover and mutation from the int in
  // file into the enum values
  void ParseMethods(Settings* setData);

  // Scans the directory for puzzle files, storing names in array for loading
  // if the user wants to use one of them
  void ScanFileDirectory();

  // Find the correct filename from the vector of puzzle file names found
  // during the directory scan
  int GetDataFilename(int size, int pattern);

  // Takes string of data and parses into the array of integers to use to create		
  // the puzzle piece		
  void ParseData(std::string line, int pData[5]);

  // Creates a new puzzle piece and stores in the puzzle piece vector
  void CreatePiece(int pData[5]);

  // Checks to see what type of piece is currently being read, returning the
  // answer
  PieceType CheckType(int* pData);

  // Calculate the output filename
  void SetOutFilename(int boardSize, int patternNum, int select, int crossover,
                      int mutation);

  // Outputs the board to file using the pattern IDs so user can see the
  // matches for themselves
  void OutputMatches(Board* pBoard, int genCount);

  // Output the board to file using the piece IDs and orientations so the user
  // can see which piece does where with the current solution
  void OutputIDs(Board* pBoard, int genCount);

  // Append the selection method and crossover method to filename
  void AppendSelectCross(int select, int crossover);

  // Append the mutation method to filename
  void AppendMutation(int mutation);

  // If no data file was found, generate a new random board and make the data
  // file that corrosponds to that board
  void MakeDataFile(int size, int pattern);

  // Creates the data file by outputting the pieces to the file one piece per
  // line
  void OutputDataFile(std::string filename);

public:
  // Calls to scan the directory for piece data files
  FileHandler();

  // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm
  Settings ReadSettingsFile();

  // Reads the piece file with the file name that matches the information passed
  // as parameter storing piece info in the piece collection vector
  void ReadDataFile(int size, int pattern);

  // Output the board to a file to show progress or solved board, file name is
  // date, generation and time ran.
  void OutputBoard(Board* pBoard, int genCount);

  // Calculate the output filename
  void OutputFitness(int fitness);
};

