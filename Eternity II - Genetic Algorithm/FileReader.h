// Title        : FileReader.h
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#pragma once


#include "BoardManager.h"     // For PieceType & piece vector
#include "GeneticAlgorithm.h" // Settings struct
#include <fstream>            // Includes file input function
#include <string>             // Includes strings and stoi
#include <vector>             // Include vectors for filenames


class FileReader
{

private:
  std::ifstream theFile;              // File open to read from
  std::vector<std::string> filenames; // Holds all filenames held in directory

  // Opens the file using the filename provided return whether successful
  bool OpenFile(const char* fileName);
 
  // Returns the next number from the settings file to be loaded
  double GetNextSetting();

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

public:
  // Calls to scan the directory for piece data files
  FileReader();

  // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm
  Settings ReadSettingsFile();

  // Reads the piece file with the file name that matches the information passed
  // as parameter storing piece info in the piece collection vector
  void ReadDataFile(int size, int pattern);
};

