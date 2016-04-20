// Title        : FileHandler.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileHandler.h"    // Include the header for the class
#include "Crossover.h"      // Selection and Crossover type
#include "Mutation.h"       // Mutation type
#include <iostream>         // Include input and output library
#include <windows.h>        // Include file directory functions


// Initialise output file string
std::string FileHandler::outFilename = "/0";


FileHandler::FileHandler()
{ // Calls to the file directory for available piece data files, make sure
  // puzzle piece vector is initialised

  if (BoardManager::getInstance()->getPieces()->size() == 0)
  { // Make sure puzzle piece vector is initialised before pushing

    for (int i = 0; i < 3; i++)
    { // Create new vector to hold the puzzle piece collection and push onto
      // vector of vectors that holds all pieces
      std::vector<PuzzlePiece> newBoard;
      BoardManager::getInstance()->getPieces()->push_back(newBoard);
    }
  }

  // Scan the directory for piece data files
  scanFileDirectory();

} // FileReader()


Settings FileHandler::readSettingsFile()
{ // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm

  Settings settingData;    // Holds all of the loaded settings data
  int startPiece = -1; // Holds parsed int for start piece constraint

  if (openFile("settings.ini"))
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    parseInt(&settingData.boardSize);        // Parse the board size
    parseInt(&settingData.patternNum);       // Parse the number of patterns
    parseInt(&settingData.popSize);          // Parse the population size

    // Parse the selection, crossover and mutation methods into the appropriate
    // enums
    parseMethods(&settingData);

    parseDouble(&settingData.mutRate);       // Parse the mutation rate
    parseInt(&settingData.eliteRate);        // Parse the elitism rate
    parseInt(&startPiece);               // Parse if start constraint is active

    if (startPiece == 1)
    { // If value from file equals 1, set start piece constraint to true
      settingData.startCons = true;
    }
    else if (startPiece == 0)
    { // If value from file equals 0, set start piece constraint to false
      settingData.startCons = false;
    }

    theFile.close();                     // Close file after use
  }

  // Read in puzzle pieces
  readDataFile(settingData.boardSize, settingData.patternNum);

  // Calculate output filename
  setOutFilename(settingData.boardSize, settingData.patternNum, settingData.selectType,
                 settingData.crossType, settingData.mutType);

  return settingData;  // Return parsed data

} // readSettingsFile()


void FileHandler::readDataFile(int size,                   // *In*
                               int pattern)                // *In*
{ // Reads the piece file with the file name that matches the information
  // passed as parameter storing piece info in the piece collection vector

  // Get the index of the file name matching the data of the board size and
  // number of patterns
  int index = getDataFilename(size, pattern);

  std::string inLine = "";                // Stores current line to be parsed
  int parsedData[5] = { 0, 0, 0, 0, 0 };  // Holds parsed data from the line

  if (index >= 0)
  {
    if (openFile(filenames[index].c_str()))
    { // Checks to see if the file is open before proceeding with reading of
      // the file

      while (std::getline(theFile, inLine))
      { // While getline actually returns a line of data, proceed with parsing
        parseData(inLine, parsedData);
        createPiece(parsedData);
      }

      theFile.close();                             // Close the file after use
      printf("Loaded: %s\n\n", filenames[index].c_str()); // Output success
    }
  }
  else
  { // If the file was not opened, create a new file
    makeDataFile(size, pattern);
  }

} // readDataFile()


void FileHandler::outputBoard(Board* theBoard,             // *In*
                              int genCount)                // *In*
{ // Output the board to a file to show progress or solved board, file name is
  // date generation and time ran.

  if (openFile(outFilename.c_str()))
  { // If file has been created and open successfully output the data

    outputMatches(theBoard, genCount);   // Output board with pattern IDs
    outputIDs(theBoard, genCount);       // Output board with piece IDs 
    theFile.close();                   // Close the file after use
  }

} // outputBoard()


void FileHandler::outputFitness(int fitness)               // *In*
{ // Appends the fitness to file for tracking of algorithm performance

  if (openFile(outFilename.c_str()))
  { // If output file opened successfully output the fitness

    // Convert the fitness integer and output the line to the file
    theFile << fitness << std::endl;

    // Close file after use
    theFile.close();
  }

} // outputFitness()


bool FileHandler::openFile(const char* fileName)           // *In*
{ // Opens the file using the filename provided return whether successful
  
  // Open the file with in out and append flags
  theFile.open(fileName, std::ios::in | std::ios::out | std::ios::app);

  // Return whether succuessful
  return theFile.is_open();

} // openFile()


void FileHandler::scanFileDirectory()
{ // Scans the directory for puzzle files, storing names in a vector for loading
  // if the user wants to use one of them

  LPCWSTR dirPath = L"Puzzles/*";         // Directory path
  WIN32_FIND_DATA fileData;               // Holds file data

  // Create new handle and find the first file in the directory
  HANDLE findHandle = FindFirstFile(dirPath, &fileData);

  if (findHandle != INVALID_HANDLE_VALUE)
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
      // If another file found, do again
    } while (FindNextFile(findHandle, &fileData)); 

  } // if (findHandle != INVALID_HANDLE_VALUE)

} // scanFileDirectory()


void FileHandler::parseInt(int* setting)                   // *Out*
{ // Parse int from next line of file placing in int passed as parameter

  std::string inLine = "/0";         // Stores current line to be parsed
  char label[25] = "/0";             // Stores label of the line from file

  while (std::getline(theFile, inLine))
  { // Read in the next line
    if (!inLine.empty() && inLine[0] != '*')
    { // If line is not empty or comment line, parse the data in to data var
      // label storing junk label from file
      sscanf_s(inLine.c_str(), "%s %i", label, 25, setting);  // Parse value
      break;                                               // Break out of loop
    }
  }

} // parseInt()


void FileHandler::parseDouble(double* setting)             // *Out*
{ // Parse double from next line of file placing value in double passed as
  // parameter

  std::string inLine = "/0";         // Stores current line to be parsed
  char label[25] = "/0";             // Stores label of the line from file

  while (std::getline(theFile, inLine))
  { // Read in the next line
    if (!inLine.empty() && inLine[0] != '*')
    { // If line is not empty or comment line, parse the data in to data var
      // label storing junk label from file
      sscanf_s(inLine.c_str(), "%s %lf", label, 25, setting); // Parse value
      break;                                               // Break out of loop
    }
  }

} // parseDouble()


void FileHandler::parseMethods(Settings* setData)          // *Out*
{ // Parses the methods of selection, crossover and mutation from the int in
  // file into the enum values

  int setting = -1;          // Holds value read from file
  parseInt(&setting);        // Read in selection method

  if (setting == 0)
  { // If int is 0, set select method to roulette
    setData->selectType = ROULETTE;
  }
  else if (setting == 1)
  { // If int is 1, set select method to tournament
    setData->selectType = TOURNAMENT;
  }

  parseInt(&setting);        // Read in crossover method

  if (setting == 0)
  { // If int is 0, set crossover method to one-point
    setData->crossType = ONEPOINT;
  }
  else if (setting == 1)
  { // If int is 1, set crossover method to two-point
    setData->crossType = TWOPOINT;
  }

  parseInt(&setting);        // Read in mutation method

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

} // parseMethods()


int FileHandler::getDataFilename(int size,                 // *In*
                                 int pattern)              // *In*
{ // Find the correct filename from the vector of puzzle file names found
  // during the directory scan

  std::string filename = "Puzzles/BoardSize ";      // Hold file name to open
  int index = -1;                              // Hold index of filename
  char boardSize[3] = "/0";                     // Hold converted boardSize
  char patternNum[3] = "/0";                    // Hold converted pattern num

  // Convert the pattern num and boardSize
  _itoa_s(size, boardSize, 10);
  _itoa_s(pattern, patternNum, 10);

  // Construct the rest of the file name using the board size and pattern num
  filename += boardSize;
  filename += " - Pattern ";
  filename += patternNum;
  filename += ".e2";

  for (int i = 0; i < (int)filenames.size(); i++)
  { // Loop through all filenames, checking if filename needed
    if (filenames[i] == filename)
    { // If filename matches, store result
      index = i;
      break; // Break from loop if filename found
    }
  }

  return index; // Return index found

} // getDataFilename()


void FileHandler::parseData(std::string line,              // *In*
                            int parsedData[5])             // *Out*
{ // Takes string of data and parses into the array of integers to use to create		
  // the puzzle piece		
    
  int index = 0;                    // Holds the index of char for parsing		
  std::string data = "";            // Holds the data gathered from getline	
  
  for (int i = 0; i < 5; i++)
  { // For loop to loop through and get the 5 bits of data that are on each		
    // line in a e2 file

    data.clear();                   // Empty the string ready for next data
    
    while (line[index] != ' ' && index != line.length())
    { // Loops though to check for whitespace in the file or it has reached		
      // the end of the data from that line. Data is added to the string on		
      // each pass		
       
      data += line[index];    // Add the char to the string to be converted		
      index++;                // Increment char counter for parsing		
    }
   
    // If whitepsace was found in line, skip to next char		
    if (line[index] == ' ')
    {
      index++;
    }

    // Convert the data to a int and set it to appropriate element		
    parsedData[i] = std::stoi(data);
  } // for (int i = 0; i <= 4; i++)		
 
} // parseData()


void FileHandler::createPiece(int parsedData[5])           // *In*
{ // Creates a new puzzle piece and stores in the puzzle piece vector

  // Set each element of the piece to the parsed section of data
  PuzzlePiece newPiece;

  newPiece.pieceID = parsedData[0];
  newPiece.type = checkType(parsedData);
  newPiece.segments[TOP] = parsedData[1];
  newPiece.segments[RIGHT] = parsedData[2];
  newPiece.segments[BOTTOM] = parsedData[3];
  newPiece.segments[LEFT] = parsedData[4];
  newPiece.orientation = 0;

  // Add piece to appropriate vector within pieces vector
  if (newPiece.type == CORNER)
  {
    (*BoardManager::getInstance()->getPieces())[CORNER].push_back(newPiece);
  }
  else if (newPiece.type == EDGE)
  {
    (*BoardManager::getInstance()->getPieces())[EDGE].push_back(newPiece);
  }
  else if (newPiece.type == INNER)
  {
    (*BoardManager::getInstance()->getPieces())[INNER].push_back(newPiece);
  }

} // createPiece()


PieceType FileHandler::checkType(int* parsedData)          // *In*
{ // Checks to see what type of piece is currently being read, returning the
  // answer

  PieceType type = DEFAULT;    // Object to return, holds types of piece
  int edgeCount = 0;               // Count for how many times edge pattern occurs

  for (int i = 0; i <= 4; i++)
  { // Loop for each quadrant to check if the pattern ID matches the edge pattern

    if (parsedData[i] == 0)
    { // If the current quadrant has the edge pattern, increment the count
      edgeCount++;
    }
  }

  switch (edgeCount)
  { // Using how many edge patterns were counted, set the piece type

    case 0:  // If no edge patterns, piece is of the inner type
      type = INNER;
      break;

    case 1:  // If one edge pattern, piece is of the edge type
      type = EDGE;
      break;

    case 2:  // If two edge patterns, piece is of the corner type
      type = CORNER;
      break;

    default: // Make sure type is error type
      type = DEFAULT;
  }

  // Return the type that has been found
  return type;

} // checkType()


void FileHandler::setOutFilename(int boardSize,            // *In*
                                 int patternNum,           // *In*
                                 int select,               // *In*
                                 int crossover,            // *In*
                                 int mutation)             // *In*
{ // Calculate the output filename

  char intBuff[10] = "/0";               // Holds result of itoa

   // Set directory and board size label
  outFilename = "Solutions/BoardSize ";

  // Convert board size to char and append to filename
  _itoa_s(boardSize, intBuff, 10);
  outFilename += intBuff;

  outFilename += " Pattern ";            // Append pattern label

  // Convert the pattern number to char and append to filename
  _itoa_s(patternNum, intBuff, 10);
  outFilename += intBuff;

  appendSelectCross(select, crossover);  // Append select and crossover methods
  appendMutation(mutation);              // Append mutation method

  outFilename += ".txt";  // Append file extension to filename

} // setOutFilename()


void FileHandler::outputMatches(Board* theBoard,             // *In*
                                int genCount)              // *In*
{ // Outputs the board to file using the pattern IDs so user can see the
  // matches for themselves

  char buff[10] = "/0";       // Holds integer that has been converted to char
  std::string output[3] = { "/0", "/0", "/0" }; // Holds three rows of output

  // Ouput how many generations the solution took
  theFile << std::endl << "Generation: " << genCount << std::endl;

  for (int j = 0; j <= BoardManager::getInstance()->getSize(); j++)
  { // Y index for pieces to output
    for (int i = 0; i <= BoardManager::getInstance()->getSize(); i++)
    { // X index for pieces to ouput, parse a piece into three rows
      // of output.

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, TOP), buff, 
           10);

      output[0] += "  ";      // Add whitespace for formatting
      output[0] += buff;      // Add converted pattern ID to top line
      output[0] += "  ";      // Add more whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, LEFT), buff,
           10);

      output[1] += buff;     // Add converted pattern ID to the middle line
      output[1] += "   ";    // Add whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, RIGHT), buff,
           10);

      output[1] += buff;     // Add right pattern ID to middle line

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, BOTTOM), buff,
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

} // outputMatches()


void FileHandler::outputIDs(Board* theBoard,                 // *In*
                            int genCount)                  // *In*
{ // Output the board to file using the piece IDs and orientations so the user
  // can see which piece does where with the current solution

  theFile << std::endl;   // Add whitepsace between two boards

  for (int j = 0; j <= BoardManager::getInstance()->getSize(); j++)
  { // Y index for pieces to output
    for (int i = 0; i <= BoardManager::getInstance()->getSize(); i++)
    { // X index for pieces to output

      // Output piece ID and add comma for separator
      theFile << theBoard->boardVecs[i][j].pieceID << ",";

      // Output the piece orientation and add some whitespace to seperate
      // piece data
      theFile << theBoard->boardVecs[i][j].orientation << "  ";
    }

    theFile << std::endl; // Add whitespace for next row
  }

} // outputIDs()


void FileHandler::appendSelectCross(int select,            // *In*
                                    int crossover)         // *In*
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
  else if (crossover == 1)
  { // If crossover method is two point, append two point
    outFilename += " TwoPoint";
  }

} // appendSelectCross()


void FileHandler::appendMutation(int mutation)             // *In*
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

} // appendMutation()


void FileHandler::makeDataFile(int size,                   // *In* 
                               int pattern)                // *In*
{ // If no data file was found, generate a new random board and make the data
  // file that corrosponds to that board

  std::string filename = "Puzzles/BoardSize ";    // Hold file name to open
  char boardSize[3] = "/0";                       // Hold converted boardSize
  char patternNum[3] = "/0";                      // Hold converted pattern num


  // Convert the pattern num and boardSize
  _itoa_s(size, boardSize, 10);
  _itoa_s(pattern, patternNum, 10);

  // Construct the rest of the file name using the board size and pattern num
  filename += boardSize;
  filename += " - Pattern ";
  filename += patternNum;
  filename += ".e2";

  BoardManager::getInstance()->generateBoard(size, pattern);

  outputDataFile(filename);

} // makeDataFile()


void FileHandler::outputDataFile(std::string filename)     // *In*
{ // Creates the data file by outputting the pieces to the file one piece per
  // line
  
  if (openFile(filename.c_str()))
  {
    for (int i = 0; i < (int)BoardManager::getInstance()->getPieces()[CORNER].
         size(); i++)
    { // Output corners to data file
      theFile << (*BoardManager::getInstance()->getPieces())[0][i].pieceID <<
        std::endl;
      theFile << (*BoardManager::getInstance()->getPieces())[0][i].segments[0]
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[0][i].segments[1]
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[0][i].segments[2]
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[0][i].segments[3];
      theFile << std::endl;
    }

    for (int i = 0; i < (int)(*BoardManager::getInstance()->getPieces())[EDGE].
         size(); i++)
    { // Output edge pieces to data file
      theFile << (*BoardManager::getInstance()->getPieces())[1][i].pieceID << 
        " ";
      theFile << (*BoardManager::getInstance()->getPieces())[1][i].segments[0] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[1][i].segments[1] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[1][i].segments[2] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[1][i].segments[3];
      theFile << std::endl;
    }

    for (int i = 0; i < (int)(*BoardManager::getInstance()->getPieces())[INNER].
         size(); i++)
    { // Output the inner pieces to data file
      theFile << (*BoardManager::getInstance()->getPieces())[2][i].pieceID << 
        " ";
      theFile << (*BoardManager::getInstance()->getPieces())[2][i].segments[0] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[2][i].segments[1] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[2][i].segments[2] 
        << " ";
      theFile << (*BoardManager::getInstance()->getPieces())[2][i].segments[3];
      theFile << std::endl;
    }

    theFile.close();
  }
  else
  { 
    char buffer[200];
    perror(buffer);
  }

} // outputDataFile()