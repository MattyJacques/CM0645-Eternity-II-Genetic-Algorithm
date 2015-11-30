// Title        : FileReader.h
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015

#pragma once

#include <fstream>  // Includes file input function
#include <string>   // Includes strings and stoi


class FileReader
{

private:
  static FileReader* pInstance;   // Holds the current instance for the class
  std::ifstream theFile;          // File open to read from

  // Constructor that opens the file ready for reading
  FileReader(const char* fileName);

public:
  // Creates the instance of the class, handing the filename over to be opened
  static FileReader* CreateInstance(const char* fileName);

  // Returns the pointer to the instance of the class
  static FileReader* GetInstance();

  // Scans the directory for puzzle files, storing names in array for loading
  // if the user wants to use one of them
  void ScanFileDirectory();

  // Reads the piece file that has already been opened, storing piece info
  // in the boards piece array
  void ReadPieceFile();

  // Deletes the instance of the call and nulls memory
  ~FileReader();

};

