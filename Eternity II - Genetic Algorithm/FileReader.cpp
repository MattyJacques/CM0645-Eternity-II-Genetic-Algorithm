// Title        : FileReader.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileReader.h"     // Include the header for the class
#include "BoardManager.h"   // Include BoardManager for piece array access
#include <iostream>         // Include input and output library
#include <windows.h>
#include <sys\types.h>


// Initialise the pointer to the instance
FileReader* FileReader::pInstance = nullptr;


FileReader::FileReader()
{ 

} // FileReader()


FileReader* FileReader::GetInstance()
{ // If the instance is not already created, create the instaqnce of the class
  // then return the pointer to the instance

  // Check to see if instance already exists, if not, create the new instance
  if (!pInstance)
    pInstance = new FileReader();

  // Return pointer to the instance of the class
  return pInstance;

} // GetInstance()


void FileReader::OpenFile(const char * fileName)
{ // Opens the file using the filename provided

  // Open the file
  theFile.open(fileName);

} // OpenFile()


void FileReader::ScanFileDirectory()
{ // Scans the directory for puzzle files, storing names in a vector for loading
  // if the user wants to use one of them

  LPCWSTR dirPath = L"Puzzles/*";         // Directory path
  WIN32_FIND_DATA fileData;               // File object

  // Create new handle and find the first file in the directory
  HANDLE find = FindFirstFile(dirPath, &fileData);

  if (find != INVALID_HANDLE_VALUE)
  { // If there is a file in the directory loop through and push filenames on to
    // the vector of filenames

    do
    { // Add first filename to vector before trying to find another filename

      if (fileData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      { // If the found object is not a directory, convert filename and add to
        // filename vector

        // Make wide string that contains filename
        std::wstring convertString = fileData.cFileName;

        // Convert filename from wide string to standard string
        std::string filename(convertString.begin(), convertString.end());

        // Push filename on to vector
        filenames.push_back(filename);
      }

    } while (FindNextFile(find, &fileData)); // If another file found, do again

  }

} // ScanFileDirectory()


void FileReader::ReadPieceFile()
{ // Reads in how many pieces are in the file then sorts information from
  // file into the puzzle peice array. Outputs error if file does not exist

  std::string line;                     // Stores current like to be parsed
  int pData[5];                         // Holds parsed data from the line

  if (theFile.is_open())
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    while (std::getline(theFile, line))
    { // While getline actually returns a line of data, proceed with parsing

      ParseData(line, pData);
      CreatePiece(pData);
  
    } 

    theFile.close(); // Close the file after use
  }
  else
  { // If the file was not open, output file not exist error
    std::cout << "File does not exist" << std::endl;
  }

} // ReadPieceFile()


void FileReader::CreatePiece(int pData[5])
{ // Creates a new puzzle piece and stores in the puzzle piece vector

  // Set each element of the piece to the parsed section of data
  PuzzlePiece newPiece;

  newPiece.pieceID = pData[0];
  newPiece.type = CheckType(pData);
  newPiece.segments[TOP] = pData[1];
  newPiece.segments[LEFT] = pData[2];
  newPiece.segments[BOTTOM] = pData[3];
  newPiece.segments[RIGHT] = pData[4];
  newPiece.orientation = 0;

  // Add peice to vector of pieces
  BoardManager::GetInstance()->pieceVec.push_back(newPiece);

} // CreatePiece()


void FileReader::ParseData(std::string line, int pData[5])
{ // Takes string of data and parses into the array of integers to use to create
  // the puzzle piece

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

} // ParseData()


PieceType FileReader::CheckType(int* pData)
{ // Checks to see what type of piece is currently being read, returning the
  // answer

  PieceType type;     // Object to return, holds types of piece
  int count = 0;      // Count for how many times edge pattern occurs

  for (int i = 0; i <= 4; i++)
  { // Loop for each quadrant to check if the pattern ID matches the edge pattern

    if (pData[i] == 0)
    {
      count++;
    }
  }

  switch (count)
  { // Using how many edge patterns were counted, set the piece type

  case 0:
    type = INNER;
    break;

  case 1:
    type = EDGE;
    break;

  case 2:
    type = CORNER;
    break;

  default: // Output error string
    std::cout << "Piece type check error" << std::endl;

  }

  // Return the type that has been found
  return type;

} // CheckType()


FileReader::~FileReader()
{ // Deletes the instance of the class, then sets pointer to nullptr

  delete pInstance;
  pInstance = nullptr;

} // ~FileReader()
