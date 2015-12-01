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

  std::string line;                     // Stores current like to be parsed
  int* pData = new int[5];              // Holds parsed data from the line
  Board* pBoard = Board::GetInstance(); // So the piece array can be accessed

  if (theFile.is_open())
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    while (std::getline(theFile, line))
    { // While getline actually returns a line of data, proceed with parsing

      int j = 0;    // Holds the number of char for parsing

      for (int i = 0; i <= 4; i++)
      { // For loop to loop through and get the 5 bits of data that are on each
        // line in a e2 file

        std::string data; // Holds the data gathered from getline
        
        while (line[j] != ' ' && j != line.length())
        { // Loops though to check for whitespace in the file or it has reached
          // the end of the data from that line. Data is added to the string on
          // each pass

          data += line[j];    // Add the char to the string to be converted
          j++;                // Increment char counter for parsing
        }

        // If whitepsace was found in line, skip to next char
        if (line[j] == ' ')
          j++;

        // Convert the data to a int and set it to appropriate element
        pData[i] = std::stoi(data);
      } // for (int i = 0; i <= 4; i++)

      // Set each element of the piece to the parsed section of data
      PuzzlePiece newPiece;

      newPiece.SetPieceID(pData[0]);
      newPiece.SetTop(pData[1]);
      newPiece.SetLeft(pData[2]);
      newPiece.SetBottom(pData[3]);
      newPiece.SetRight(pData[4]);

      pBoard->pieceVec.push_back(newPiece);
  
    } 

    theFile.close(); // Close the file after use
  }
  else
  { // If the file was not open, output file not exist error
    std::cout << "File does not exist" << std::endl;
  }

} // ReadPieceFile()


FileReader::~FileReader()
{ // Deletes the instance of the class, then sets pointer to nullptr

  delete pInstance;
  pInstance = nullptr;

} // ~FileReader()
