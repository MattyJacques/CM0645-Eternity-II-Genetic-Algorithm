// Title        : FileReader.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileReader.h"     // Include the header for the class
#include "Crossover.h"      // Selection and Crossover type
#include "Mutation.h"       // Mutation type
#include <iostream>         // Include input and output library
#include <windows.h>        // Include file directory functions


// Initialise output file string
std::string FileReader::outFilename = "/0";


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

  // Open the file with in out and append flags
  theFile.open(fileName, std::ios::in | std::ios::out | std::ios::app);

  // Check to see if file is open
  if (theFile.is_open())
    result = true;

  // Return whether seccuessful
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

        // Add folder name to the path
        std::string fullpath = "Puzzles/" + filename;

        // Push filename on to vector
        filenames.push_back(fullpath);
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
    
    // Parse the board data from the settings file using GetNextSetting(), 
    // setting value to appropriate fields in settings stuct
    setData.boardSize  = (int)GetNextSetting();
    setData.patternNum = (int)GetNextSetting();
    setData.popSize    = (int)GetNextSetting();

    // Parse the selection, crossover and mutation methods into the appropriate
    // enums
    ParseMethods(&setData); 

    // Parse the rates and the if the start piece constraint is active
    setData.mutRate    = GetNextSetting();
    setData.eliteRate  = (int)GetNextSetting();
    setData.startCons  = GetNextSetting() == 1;

    theFile.close();                     // Close file after use
    printf("Loaded: settings.ini\n\n");  // Output loading complete
  }

  // Read in puzzle pieces
  ReadDataFile(setData.boardSize, setData.patternNum);

  // Calculate output filename
  SetOutFilename(setData.boardSize, setData.patternNum, setData.selectType,
                 setData.crossType, setData.mutType);

  return setData;  // Return parsed data

} // ReadSettingsFile()


void FileReader::ParseMethods(Settings* setData)
{ // Parses the methods of selection, crossover and mutation from the int in
  // file into the enum values

  int setting = (int)GetNextSetting();    // Read in select method

  if (setting == 0)
  { // If int is 0, set select method to roulette
    setData->selectType = ROULETTE;
  }
  else if (setting == 1)
  { // If int is 1, set select method to tournament
    setData->selectType = TOURNAMENT;
  }

  setting = (int)GetNextSetting();        // Read in crossover method

  if (setting == 0)
  { // If int is 0, set crossover method to one-point
    setData->crossType = ONEPOINT;
  }
  else if (setting == 1)
  { // If int is 1, set crossover method to two-point
    setData->crossType = TWOPOINT;
  }

  setting = (int)GetNextSetting();        // Read in mutation method

  if (setting == 0)
  { // If int is 0, set mutation method to swap
    setData->mutType = SWAP;
  }
  else if (setting == 1)
  { // If int is 0, set mutation method to rotate
    setData->mutType = ROTATE;
  }
  else if (setting == 2)
  { // If int is 0, set mutation method to rotate & swap
    setData->mutType = ROTATESWAP;
  }
  else if (setting == 3)
  { // If int is 0, set mutation method to region swap
    setData->mutType = REGIONSWAP;
  }
  else if (setting == 4)
  { // If int is 0, set mutation method to region rotate
    setData->mutType = REGIONSWAP;
  }

} // ParseMethods()


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


void FileReader::ReadDataFile(int size, int pattern)
{ // Reads the piece file with the file name that matches the information
  // passed as parameter storing piece info in the piece collection vector

  // Get the index of the file name matching the data of the board size and
  // number of patterns
  int index = GetDataFilename(size, pattern);
  if (index > 0)
  {
    if (OpenFile(filenames[index].c_str()))
    { // Checks to see if the file is open before proceeding with reading of
      // the file

      std::string line;                     // Stores current line to be parsed
      int pData[5];                         // Holds parsed data from the line

      while (std::getline(theFile, line))
      { // While getline actually returns a line of data, proceed with parsing
        ParseData(line, pData);
        CreatePiece(pData);
      }

      theFile.close();                             // Close the file after use
      printf("Loaded: %s\n\n", filenames[index].c_str()); // Output success
    }
  }
  else
  { // If the file was not opened, create a new file
    MakeDataFile(size, pattern);
  }

} // ReadPieceFile()


int FileReader::GetDataFilename(int size, int pattern)
{ // Find the correct filename from the vector of puzzle file names found
  // during the directory scan

  std::string name = "Puzzles/BoardSize ";      // Hold file name to open
  int result = -1;                              // Hold index of filename
  char boardSize[3] = "/0";                     // Hold converted boardSize
  char patternNum[3] = "/0";                    // Hold converted pattern num

  // Convert the pattern num and boardSize
  _itoa_s(size, boardSize, 10);
  _itoa_s(pattern, patternNum, 10);

  // Construct the rest of the file name using the board size and pattern num
  name += boardSize;
  name += " - Pattern ";
  name += patternNum;
  name += ".e2";

  for (int i = 0; i < filenames.size(); i++)
  { // Loop through all filenames, checking if filename needed
    if (filenames[i] == name)
    { // If filename matches, store result
      result = i;
      break; // Break from loop if filename found
    }
  }

  return result; // Return index found

} // GetDataFilename()


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


void FileReader::OutputBoard(Board* pBoard, int genCount)
{ // Output the board to a file to show progress or solved board, file name is
  // date generation and time ran.

  if (OpenFile(outFilename.c_str()))
  { // If file has been created and open successfully output the data
    
    OutputMatches(pBoard, genCount);   // Output board with pattern IDs
    OutputIDs(pBoard, genCount);       // Output board with piece IDs 
    theFile.close();                   // Close the file after use
  }

} // OutputBoard()


void FileReader::OutputMatches(Board* pBoard, int genCount)
{ // Outputs the board to file using the pattern IDs so user can see the
  // matches for themselves

  char buff[10] = "/0";       // Holds integer that has been converted to char
  std::string output[3];      // Holds the three rows of output

  theFile << std::endl << "Generation: " << genCount << std::endl;

  for (int j = 0; j <= BoardManager::GetInstance()->boardSize; j++)
  { // Y index for pieces to output
    for (int i = 0; i <= BoardManager::GetInstance()->boardSize; i++)
    { // X index for pieces to ouput, parse a piece into three rows
      // of output.

      // Convert pattern ID to char
      _itoa_s(BoardManager::GetInstance()->GetPattern(pBoard, i, j, TOP), buff, 
           10);

      output[0] += "  ";      // Add whitespace for formatting
      output[0] += buff;      // Add converted pattern ID to top line
      output[0] += "  ";      // Add more whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::GetInstance()->GetPattern(pBoard, i, j, LEFT), buff,
           10);

      output[1] += buff;     // Add converted pattern ID to the middle line
      output[1] += "   ";    // Add whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::GetInstance()->GetPattern(pBoard, i, j, RIGHT), buff,
           10);

      output[1] += buff;     // Add right pattern ID to middle line

      // Convert pattern ID to char
      _itoa_s(BoardManager::GetInstance()->GetPattern(pBoard, i, j, BOTTOM), buff,
           10);

      output[2] += "  ";    // Add whitespace for formatting
      output[2] += buff;    // Add bottom pattern ID to bottom line
      output[2] += "  ";    // Ass whitepsace for formatting
    } // for i < boardSize

    theFile << output[0] << std::endl;   // Output the top line to file
    theFile << output[1] << std::endl;   // Output the middle line to file
    theFile << output[2] << std::endl;   // Output the bottom line to file

    output[0].clear();                   // Clear top string for next line
    output[1].clear();                   // Clear middle string for next line
    output[2].clear();                   // Clear bottom string for next line
  } // for j < boardSize

} // OutputMatches()


void FileReader::OutputIDs(Board* pBoard, int genCount)
{ // Output the board to file using the piece IDs and orientations so the user
  // can see which piece does where with the current solution

  theFile << std::endl;   // Add whitepsace between two boards

  for (int j = 0; j <= BoardManager::GetInstance()->boardSize; j++)
  { // Y index for pieces to output
    for (int i = 0; i <= BoardManager::GetInstance()->boardSize; i++)
    { // X index for pieces to output

      // Output piece ID and add comma for separator
      theFile << pBoard->boardVecs[i][j].pieceID << ",";

      // Output the piece orientation and add some whitespace to seperate
      // piece data
      theFile << pBoard->boardVecs[i][j].orientation << "  ";
    }

    theFile << std::endl; // Add whitespace for next row
  }

} // OutputIDs()


void FileReader::OutputFitness(int fitness)
{ // Appends the fitness to file for tracking of algorithm performance

  if (OpenFile(outFilename.c_str()))
  { // If output file opened successfully output the fitness

    // Convert the fitness integer and output the line to the file
    theFile << fitness << std::endl;

    // Close file after use
    theFile.close();
  }

} // OutputFitness()


void FileReader::SetOutFilename(int boardSize, int patternNum, int select,
                                int crossover, int mutation)
{ // Calculate the output filename

  char intBuff[10] = "/0";      // Holds result of itoa

  // Set directory and board size label
  outFilename = "Solutions/BoardSize ";

  // Convert board size to char and append to filename
  _itoa_s(boardSize, intBuff, 10);
  outFilename += intBuff;

  outFilename += " Pattern ";   // Append pattern label
  
  // Convert the pattern number to char and append to filename
  _itoa_s(patternNum, intBuff, 10);
  outFilename += intBuff;

  AppendSelectCross(select, crossover);  // Append select and crossover methods
  AppendMutation(mutation);              // Append mutation method

  outFilename += ".txt";  // Append file extension to filename

} // SetOutFilename()


void FileReader::AppendSelectCross(int select, int crossover)
{ // Append the selection method and crossover method to filename

  if (select == 0)
  { // If selection method is Roulette, append roulette
    outFilename += " Roulette";
  }
  else if (select == 1)
  { // If selection method is Tournament, append tournament
    outFilename += " Tournament";
  }

  if (crossover == 0)
  { // If crossover method is one point, append one point
    outFilename += " OnePoint";
  }
  else
  { // If crossover method is two point, append two point
    outFilename += " TwoPoint";
  }

} // AppendSelectCross()


void FileReader::AppendMutation(int mutation)
{ // Append the mutation method to filename

  if (mutation == 0)
  { // If mutation method is swap, append swap
    outFilename += " Swap";
  }
  else if (mutation == 1)
  { // If mutation method is rotate, append rotate
    outFilename += " Rotate";
  }
  else if (mutation == 2)
  { // If mutation method is rotate swap, append rotate swap
    outFilename += " RotateSwap";
  }
  else if (mutation == 3)
  { // If mutation method is region swap, append region swap
    outFilename += " RegionSwap";
  }
  else if (mutation == 4)
  { // If mutation method is region rotate, append region rotate
    outFilename += " RegionRotate";
  }

} // AppendMutation()


void FileReader::MakeDataFile(int size, int pattern)
{ // If no data file was found, generate a new random board and make the data
  // file that corrosponds to that board

  std::string filename = "Puzzles/BoardSize ";      // Hold file name to open
  char boardSize[3];                         // Hold converted boardSize
  char patternNum[3];                        // Hold converted pattern num


  // Convert the pattern num and boardSize
  _itoa_s(size, boardSize, 10);
  _itoa_s(pattern, patternNum, 10);

  // Construct the rest of the file name using the board size and pattern num
  filename += boardSize;
  filename += " - Pattern ";
  filename += patternNum;
  filename += ".e2";

  BoardManager::GetInstance()->GenerateBoard(size, pattern);

  OutputDataFile(filename);

} // MakeDataFile()


void FileReader::OutputDataFile(std::string filename)
{ // Creates the data file by outputting the pieces to the file one piece per
  // line
  
  if (OpenFile(filename.c_str()))
  {
    for (int i = 0; i < BoardManager::GetInstance()->pieceVec[0].size(); i++)
    { // Output corners to data file
      theFile << BoardManager::GetInstance()->pieceVec[0][i].pieceID << " ";
      theFile << BoardManager::GetInstance()->pieceVec[0][i].segments[0] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[0][i].segments[1] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[0][i].segments[2] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[0][i].segments[3];
      theFile << std::endl;
    }

    for (int i = 0; i < BoardManager::GetInstance()->pieceVec[1].size(); i++)
    { // Output edge pieces to data file
      theFile << BoardManager::GetInstance()->pieceVec[1][i].pieceID << " ";
      theFile << BoardManager::GetInstance()->pieceVec[1][i].segments[0] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[1][i].segments[1] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[1][i].segments[2] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[1][i].segments[3];
      theFile << std::endl;
    }

    for (int i = 0; i < BoardManager::GetInstance()->pieceVec[2].size(); i++)
    { // Output the inner pieces to data file
      theFile << BoardManager::GetInstance()->pieceVec[2][i].pieceID << " ";
      theFile << BoardManager::GetInstance()->pieceVec[2][i].segments[0] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[2][i].segments[1] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[2][i].segments[2] << " ";
      theFile << BoardManager::GetInstance()->pieceVec[2][i].segments[3];
      theFile << std::endl;
    }

    theFile.close();
  }
  else
  { 
    char buffer[200];
    perror(buffer);
  }

} // OutputDataFile()