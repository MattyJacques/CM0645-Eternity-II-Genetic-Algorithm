// Title        : FileReader.h
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015

#pragma once


#include "BoardManager.h"   // For PieceType
#include <fstream>          // Includes file input function
#include <string>           // Includes strings and stoi
#include <vector>           // Include vectors for filenames


class FileReader
{

private:
  static FileReader* pInstance;       // Holds the current instance for the class
  std::ifstream theFile;              // File open to read from
  std::vector<std::string> filenames; // Holds all filenames held in directory

  // Private for singleton
  FileReader();

public:
  // Returns the pointer to the instance of the class
  static FileReader* GetInstance();

  // Opens the file with the name given in the parameters
  void OpenFile(const char* fileName);

  // Scans the directory for puzzle files, storing names in array for loading
  // if the user wants to use one of them
  void ScanFileDirectory();

  // Reads the piece file that has already been opened, storing piece info
  // in the boards piece array
  void ReadPieceFile();

  // Creates a new puzzle piece and stores in the puzzle piece vector
  void CreatePiece(int pData[5]);

  // Takes string of data and parses into the array of integers to use to create
  // the puzzle piece
  void ParseData(std::string line, int pData[5]);

  // Checks to see what type of piece is currently being read, returning the
  // answer
  PieceType CheckType(int* pData);

  // Deletes the instance of the call and nulls memory
  ~FileReader();

};

