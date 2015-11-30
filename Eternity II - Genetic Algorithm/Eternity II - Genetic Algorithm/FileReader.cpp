// Title        : FileReader.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileReader.h"     // Include the header for the class
#include "Board.h"          // Include Board for piece array access
#include <iostream>         // Include input and output library


// Initialise the pointer to the instance
FileReader* FileReader::pInstance = nullptr;


FileReader::FileReader(const char* fileName)
{ // Opens the file using the filename provided

  // Open the file
  theFile.open(fileName);

} // FileReader()


FileReader* FileReader::CreateInstance(const char* fileName)
{ // If the instance is not already created, create the instaqnce of the class
  // then return the pointer to the instance

  // Check to see if instance already exists, if not, create the new instance
  if (!pInstance)
    pInstance = new FileReader(fileName);

  // Return pointer to the instance of the class
  return pInstance;

} // CreateInstance()


FileReader* FileReader::GetInstance()
{ // Returns the pointer to the instance of the class

  // Return instance
  return pInstance;

} // GetInstance()


void FileReader::ScanFileDirectory()
{

} // ScanFileDirectory()


void FileReader::ReadPieceFile()
{ // Reads in how many pieces are in the file then sorts information from
  // file into the puzzle peice array. Outputs error if file does not exist

  int count = 0;                        // Holds the piece count for array
  std::string line;                     // Stores current like to be parsed
  int* pData = new int[5];              // Holds parsed data from the line
  Board* pBoard = Board::GetInstance(); // So the piece array can be accessed

  if (theFile.is_open())
  {
    while (std::getline(theFile, line))
    {
      int j = 0;
      for (int i = 0; i <= 4; i++)
      {
        std::string data;
        
        while (line[j] != ' ' && j != line.length())
        {
          data += line[j];
          j++;
        }
        if (line[j] == ' ')
          j++;

        pData[i] = std::stoi(data);
        
      }
      pBoard->pPieceArray[count].SetPieceID(pData[0]);
      pBoard->pPieceArray[count].SetTop(pData[1]);
      pBoard->pPieceArray[count].SetLeft(pData[2]);
      pBoard->pPieceArray[count].SetBottom(pData[3]);
      pBoard->pPieceArray[count].SetRight(pData[4]);
      count++;
    }

    theFile.close();
  }
  else
  {
    std::cout << "File does not exist" << std::endl;
  }

} // ReadPieceFile()


FileReader::~FileReader()
{

  delete pInstance;
  pInstance = nullptr;

} // ~FileReader()
