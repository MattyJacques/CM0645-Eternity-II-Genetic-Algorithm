// Title        : FileReader.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileReader.h"     // Include the header for the class
#include "Crossover.h"      // Selection and Crossover type
#include "Mutation.h"       // Mutation type
#include <iostream>         // Include input and output library
#include <windows.h>        // Include file directory functions


FileReader::FileReader()
{ // Calls to the file directory for available piece data files, make sure
  // puzzle piece vector is initialised

  if (BoardManager::GetInstance()->pieceVec.size() == 0)
  { // Make sure puzzle piece vector is initialised before pushing

    for (int i = 0; i < 3; i++)
    { // Create new puzzle piece vector and push onto puzzle piece vector

      std::vector<PuzzlePiece> newVec;
      BoardManager::GetInstance()->pieceVec.push_back(newVec);
    }
  }

  // Scan the directory for piece data files
  ScanFileDirectory();

} // FileReader()


bool FileReader::OpenFile(const char* fileName)
{ // Opens the file using the filename provided return whether successful
  
  bool result = false;

  // Open the file
  theFile.open(fileName);

  if (theFile.is_open())
    result = true;

  return result;

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


Settings FileReader::ReadSettingsFile()
{ // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm

  Settings setData;    // Holds all of the loaded settings data

  if (OpenFile("settings.ini"))
  { // Checks to see if the file is open before proceeding with reading of the
    // file
    
    // Parse the data from the settings file using GetNextSetting(), setting 
    // value to appropriate fields in settings stuct
    setData.boardSize  = (int)GetNextSetting();
    setData.patternNum = (int)GetNextSetting();
    setData.popSize    = (int)GetNextSetting();
    setData.selectType = (SelectionType)(int)GetNextSetting();
    setData.crossType  = (CrossoverType)(int)GetNextSetting();
    setData.mutType    = (MutateType)(int)GetNextSetting();
    setData.mutRate    = GetNextSetting();
    setData.eliteRate  = (int)GetNextSetting();
    setData.startCons  = GetNextSetting() == 1;

    theFile.close();                     // Close file after use
    printf("Loaded: settings.ini\n\n");  // Output loading complete
  }

  return setData;  // Return parsed data

} // ReadSettingsFile()


double FileReader::GetNextSetting()
{ // Returns the next number from the settings file to be loaded

  std::string line;         // Stores current line to be parsed
  double data = -1;         // Holds the data that has been parsed

  while (std::getline(theFile, line))
  { // Read in the next line
    if (!line.empty() && line[0] != '*')
    { // If line is not empty or comment line, parse the data in to data var

      char temp[25];                                     // Throwaway label
      sscanf_s(line.c_str(), "%s %lf", temp, 25, &data); // Parse value
      break;                                             // Break out of loop
    }
  }

  return data; // Return parsed data

} // GetNextSetting()


void FileReader::ReadDataFile(const char* fileName)
{ // Reads the piece file that with the name that has been passed in storing
  // piece info in the piece collection vector

  if (OpenFile(fileName))
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    std::string line;                     // Stores current line to be parsed
    int pData[5];                         // Holds parsed data from the line

    while (std::getline(theFile, line))
    { // While getline actually returns a line of data, proceed with parsing
      ParseData(line, pData);
      CreatePiece(pData);
    } 

    theFile.close();                   // Close the file after use
    printf("Loaded: %s\n\n", fileName); // Output success
  }
  else
  { // If the file was not open, output file not exist error
    printf("File does not exist.\n");
  }

} // ReadPieceFile()


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


void FileReader::CreatePiece(int pData[5])
{ // Creates a new puzzle piece and stores in the puzzle piece vector

  // Set each element of the piece to the parsed section of data
  PuzzlePiece newPiece;

  newPiece.pieceID = pData[0];
  newPiece.type = CheckType(pData);
  newPiece.segments[TOP] = pData[1];
  newPiece.segments[RIGHT] = pData[2];
  newPiece.segments[BOTTOM] = pData[3];
  newPiece.segments[LEFT] = pData[4];
  newPiece.orientation = 0;

  // Add piece to appropriate vector within pieces vector
  if (newPiece.type == CORNER)
    BoardManager::GetInstance()->pieceVec[CORNER].push_back(newPiece);
  else if (newPiece.type == EDGE)
    BoardManager::GetInstance()->pieceVec[EDGE].push_back(newPiece);
  else
    BoardManager::GetInstance()->pieceVec[INNER].push_back(newPiece);

} // CreatePiece()


PieceType FileReader::CheckType(int* pData)
{ // Checks to see what type of piece is currently being read, returning the
  // answer

  PieceType type;     // Object to return, holds types of piece
  int count = 0;      // Count for how many times edge pattern occurs

  for (int i = 0; i <= 4; i++)
  { // Loop for each quadrant to check if the pattern ID matches the edge pattern

    if (pData[i] == 0)
    { // If the current quadrant has the edge pattern, increment the count
      count++;
    }
  }

  switch (count)
  { // Using how many edge patterns were counted, set the piece type

  case 0: // If no edge patterns, piece is of the inner type
    type = INNER;
    break;

  case 1: // If one edge pattern, piece is of the edge type
    type = EDGE;
    break;

  case 2: // If two edge patterns, piece is of the corner type
    type = CORNER;
    break;

  default: // Output error string
    printf("Piece type check error\n");

  }

  // Return the type that has been found
  return type;

} // CheckType()
