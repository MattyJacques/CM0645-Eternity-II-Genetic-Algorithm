// Title        : FileHandler.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileHandler.h"    // Class declariation
#include "Crossover.h"      // Selection and Crossover type
#include "Mutation.h"       // Mutation type
#include <iostream>         // Console input and output
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
      std::vector<PuzzlePiece> newCol;    // Create a new vector for board

      // Push vector on to vector of vectors
      BoardManager::getInstance()->getPieces()->push_back(newCol);
    }
  }

  // Scan the directory for piece data files
  scanFileDirectory();

} // FileReader()


void FileHandler::readSettingsFile(int* boardSize,               // *Out*
                                   int* patternNum,              // *Out*
                                   int* popSize,                 // *Out*
                                   SelectionType* selectMethod,  // *Out*
                                   CrossoverType* crossMethod,   // *Out*
                                   MutateType* mutMethod,        // *Out*
                                   double* mutRate,              // *Out*
                                   int* eliteRate,               // *Out*
                                   bool* isStartPiece,           // *Out*
                                   bool* isSuccess)              // *Out*
{ // Reads the settings file named "settings.ini" in the root directory, 
  // setting the appropriate values that have been read in to the algorithm

  int inSize = -1;          // Holds input board size   
  int inPattern = -1;       // Holds input pattern num
  int inPopSize = -1;       // Holds input population size
  int inSelect = -1;        // Holds input input select
  int inCross = -1;         // Holds input crossover method
  int inMutMethod = -1;     // Holds input mutation method
  double inMutRate = -1;    // Holds input mutation rate
  int inElite = -1;         // Holds input elitism rate
  int startPiece = -1;      // Holds parsed int for start piece constraint

  if (openFile("settings.ini"))
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    parseInt(&inSize, "BoardSize:");           // Parse the board size
    parseInt(&inPattern, "NumberOfPatterns:"); // Parse the number of patterns
    parseInt(&inPopSize, "PopulationSize:");   // Parse the population size   
    parseInt(&inSelect, "SelectionMethod:");   // Parse selection method
    parseInt(&inCross, "CrossoverMethod:");    // Parse crossover method
    parseInt(&inMutMethod, "MutationMethod:"); // Parse mutation method
    parseDouble(&inMutRate, "MutationRate:");  // Parse the mutation rate
    parseInt(&inElite, "EliteRate:");          // Parse the elitism rate
    parseInt(&startPiece, "StartConstraint:"); // Parse start constraint active

    theFile.close();                     // Close file after use
  } // if (openFile("settings.ini"))

  if (CheckInput(inSize, inPattern, inPopSize, inSelect, inCross, inMutMethod,
                 inMutRate, inElite, startPiece))
  { // Check to see if the input that has been read in is valid. If the input
    // is valid, set the data members of the GA to the input

    *boardSize = inSize;               // Set the board size
    *patternNum = inPattern;           // Set the number of patterns
    *popSize = inPopSize;              // Set the population size

    // Parse the input ints into the enums of the selection, crossover and 
    // mutation methods
    parseMethods(inSelect, inCross, inMutMethod, selectMethod, crossMethod,
                 mutMethod);

    *mutRate = inMutRate;              // Set the mutation rate
    *eliteRate = inElite;              // Set the elitism rate
    *isStartPiece = (startPiece == 1); // Set if start piece is active
  }
  else
  { // If input is not valid, set success to false and set settings to 
    // Eternirty II size and number of patterns

    *isSuccess = false;  // Set input success to false
    inSize = 16;         // Set board size to Eternity II
    inPattern = 22;      // Set number of patterns to Eternity II
  }

  // Read in puzzle pieces
  readDataFile(inSize, inPattern);

  // Calculate output filename
  setOutFilename(inSize, inPattern, inSelect, inCross, inMutMethod);

} // readSettingsFile()


bool FileHandler::CheckInput(int inSize,                         // *In*         
                             int inPattern,                      // *In*
                             int inPopSize,                      // *In*
                             int inSelect,                       // *In*
                             int inCross,                        // *In*
                             int inMutMethod,                    // *In*
                             double inMutRate,                   // *In*
                             int inElite,                        // *In*
                             int startPiece)                     // *In*
{ // Checks all of the input that has been read in make sure they are valid
  // inputs

  bool result = true;         // Result of input check, changed to false if fail

  if (inSize < 3)
  { // If the board size is less than 3, set to failed input
    result = false;
  }
  else if (inPattern < 1)
  { // If the number of patterns is less than 1, set to failed input
    result = false;
  }
  else if (inPopSize < 1 || inPopSize < inElite + 1)
  { // If the population size is less than one or less than the elite rate + 1,
    // set to failed input
    result = false;
  }
  else if (inSelect < 0 || inSelect > 1)
  { // If the selection method is less than 0 or greater than 1, set to failed 
    // input
    result = false;
  }
  else if (inCross < 0 || inCross > 1)
  { // If the crossover method is less than 0 or greater than 1, set to failed 
    // input
    result = false;
  }
  else if (inMutMethod < 0 || inMutMethod > 4)
  { // If the mutation method is less than 0 or greather than 4, set to failed 
    // input
    result = false;
  }
  else if (inMutRate < 0)
  { // If the mutation rate is less than 0, set to failed input
    result = false;
  }
  else if (inElite < 0 || inElite > inPopSize - 1)
  { // If the elitism rate is less than 0 or greater than the population size -
    // 1, set to failed input
    result = false;
  }
  else if (startPiece != 0 && startPiece != 1 && inSize == 16)
  { // If the start piece constraint is not set to 0 or 1 and greater or equal
    // 16, set to failed input
    result = false;
  }

  return result;        // Return the result

} // CheckInput()


void FileHandler::readDataFile(int size,                         // *In*
                               int pattern)                      // *In*
{ // Reads the piece file with the file name that matches the information
  // passed as parameter storing piece info in the piece collection vector

  // Get the index of the file name matching the data of the board size and
  // number of patterns
  int index = getDataFilename(size, pattern);

  std::string inLine = "";                // Stores current line to be parsed
  int parsedData[5] = { 0, 0, 0, 0, 0 };  // Holds parsed data from the line

  if (index >= 0)
  { // If index was found, open file with filename from vector at index element

    if (openFile(filenames[index].c_str()))
    { // Checks to see if the file is open before proceeding with reading of
      // the file

      while (std::getline(theFile, inLine))
      { // While getline actually returns a line of data, proceed with parsing
        parseData(inLine, parsedData);    // Parse piece data
        createPiece(parsedData);          // Create new piece with data
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


void FileHandler::outputBoard(Board* theBoard,                   // *In*
                              int genCount)                      // *In*
{ // Output the board to a file to show progress or solved board, file name is
  // date generation and time ran.

  if (openFile(outFilename.c_str()))
  { // If file has been created and open successfully output the data

    outputMatches(theBoard, genCount);   // Output board with pattern IDs
    outputIDs(theBoard, genCount);       // Output board with piece IDs 
    theFile.close();                     // Close the file after use
  }

} // outputBoard()


void FileHandler::outputFitness(int fitness)                     // *In*
{ // Appends the fitness to file for tracking of algorithm performance

  if (openFile(outFilename.c_str()))
  { // If output file opened successfully output the fitness

    // Convert the fitness integer and output the line to the file
    theFile << fitness << std::endl;

    // Close file after use
    theFile.close();
  }

} // outputFitness()


bool FileHandler::openFile(const char* fileName)                 // *In*
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


void FileHandler::parseInt(int* setting,                         // *Out*
                           std::string label)                    // *In*
{ // Parse int from next line of file placing value in int passed as parameter,
  // checking label to make sure it is the right setting

  std::string inLine = "/0";         // Stores current line to be parsed
  char inLabel[25] = "/0";           // Stores label of the line from file
  int inputInt = -1;                    // Stores the int that has been read

  while (std::getline(theFile, inLine))
  { // Read in the next line
    if (!inLine.empty() && inLine[0] != '*')
    { // If line is not empty or comment line, parse the data in to data var
      // label checking if label is correct

      // Parse int and label into inputInt and inLabel
      sscanf_s(inLine.c_str(), "%s %i", inLabel, 25, &inputInt);

      if (inLabel == label)
      { // If the labels match, set the setting to the input
        *setting = inputInt;
      }

      break; // Break out of loop
    }
  }

} // parseInt()


void FileHandler::parseDouble(double* setting,                   // *Out*
                              std::string label)                 // *In*
{ // Parse double from next line of file placing value in double passed as
  // parameter, checking the labal to make sure it is the right setting

  std::string inLine = "/0";         // Stores current line to be parsed
  char inLabel[25] = "/0";           // Stores label of the line from file
  double inputDouble = -1;           // Stores the double that has been read

  while (std::getline(theFile, inLine))
  { // Read in the next line
    if (!inLine.empty() && inLine[0] != '*')
    { // If line is not empty or comment line, parse the data in to data var
      // label checking if label is correct

      // Parse double and label into inputDoublew and inLabal
      sscanf_s(inLine.c_str(), "%s %lf", inLabel, 25, &inputDouble);

      if (inLabel == label)
      { // If the labels match, set the setting to the input
        *setting = inputDouble;
      }

      break; // Break out of loop
    }
  }

} // parseDouble()


void FileHandler::parseMethods(int inSelect,                     // *In*
                               int inCross,                      // *In*
                               int inMutate,                     // *In*
                               SelectionType* selectType,        // *Out*
                               CrossoverType* crossType,         // *Out*
                               MutateType* mutType)              // *Out*
{ // Parses the methods of selection, crossover and mutation from the int into 
  // the enum values

  if (inSelect == 0)
  { // If select method is 0, set select method to roulette
    *selectType = ROULETTE;
  }
  else if (inSelect == 1)
  { // If select method is 1, set select method to tournament
    *selectType = TOURNAMENT;
  }

  if (inCross == 0)
  { // If crossover method is 0, set crossover method to one-point
    *crossType = ONEPOINT;
  }
  else if (inCross == 1)
  { // If crossover method is 1, set crossover method to two-point
    *crossType = TWOPOINT;
  }

  if (inMutate == 0)
  { // If mutation method is 0, set mutation method to swap
    *mutType = SWAP;
  }
  else if (inMutate == 1)
  { // If mutation method is 0, set mutation method to rotate
    *mutType = ROTATE;
  }
  else if (inMutate == 2)
  { // If mutation method is 0, set mutation method to rotate & swap
    *mutType = ROTATESWAP;
  }
  else if (inMutate == 3)
  { // If mutation method is 0, set mutation method to region swap
    *mutType = REGIONSWAP;
  }
  else if (inMutate == 4)
  { // If mutation method is 0, set mutation method to region rotate
    *mutType = REGIONSWAP;
  }

} // parseMethods()


int FileHandler::getDataFilename(int size,                       // *In*
                                 int pattern)                    // *In*
{ // Find the correct filename from the vector of puzzle file names found
  // during the directory scan

  std::string filename = "Puzzles/BoardSize ";  // Hold file name to open
  int index = -1;                               // Hold index of filename
  char boardSize[3] = "/0";                     // Hold converted boardSize
  char patternNum[3] = "/0";                    // Hold converted pattern num

  _itoa_s(size, boardSize, 10);                 // Convert board size to char[]
  _itoa_s(pattern, patternNum, 10);             // Convert pattern num to char[]

  filename += boardSize;                        // Append board size to name
  filename += " - Pattern ";                    // Append pattern label to name
  filename += patternNum;                       // Append pattern num to name
  filename += ".e2";                            // Append file extension

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


void FileHandler::parseData(std::string line,                    // *In*
                            int parsedData[5])                   // *Out*
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
   	
    if (line[index] == ' ')
    { // If whitepsace was found in line, skip to next char	
      index++;
    }

    // Convert the data to a int and set it to appropriate element		
    parsedData[i] = std::stoi(data);

  } // for (int i = 0; i <= 4; i++)		
 
} // parseData()


void FileHandler::createPiece(int parsedData[5])                 // *In*
{ // Creates a new puzzle piece and stores in the puzzle piece vector

  // Create a new puzzle piece
  PuzzlePiece newPiece;

  newPiece.pieceID = parsedData[0];           // Set piece ID
  newPiece.type = checkType(parsedData);      // Set piece type
  newPiece.segments[TOP] = parsedData[1];     // Set top pattern
  newPiece.segments[RIGHT] = parsedData[2];   // Set right pattern
  newPiece.segments[BOTTOM] = parsedData[3];  // Set bottom pattern
  newPiece.segments[LEFT] = parsedData[4];    // Set left pattern
  newPiece.orientation = 0;                   // Initialise orientation

  if (newPiece.type == CORNER)
  { // If corner, push to corner vector
    (*BoardManager::getInstance()->getPieces())[CORNER].push_back(newPiece);
  }
  else if (newPiece.type == EDGE)
  { // If edge, push to edge vector
    (*BoardManager::getInstance()->getPieces())[EDGE].push_back(newPiece);
  }
  else if (newPiece.type == INNER)
  { // If inner, push to inner vector
    (*BoardManager::getInstance()->getPieces())[INNER].push_back(newPiece);
  }

} // createPiece()


PieceType FileHandler::checkType(int* parsedData)                // *In*
{ // Checks to see what type of piece is currently being read, returning the
  // answer

  PieceType type = DEFAULT;    // Object to return, holds types of piece
  int edgeCount = 0;           // Count for how many times edge pattern occurs

  for (int i = 0; i <= 4; i++)
  { // Loop for each quadrant to check if the pattern ID matches the edge 
    // pattern

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


void FileHandler::setOutFilename(int boardSize,                  // *In*
                                 int patternNum,                 // *In*
                                 int select,                     // *In*
                                 int crossover,                  // *In*
                                 int mutation)                   // *In*
{ // Calculate the output filename

  char intBuff[10] = "/0";               // Holds result of itoa

   // Set directory and board size label
  outFilename = "Solutions/BoardSize ";

  _itoa_s(boardSize, intBuff, 10);       // Convert board size to char[]
  outFilename += intBuff;                // Append board size

  outFilename += " Pattern ";            // Append pattern label

  _itoa_s(patternNum, intBuff, 10);      // Convert pattern num to char[]
  outFilename += intBuff;                // Append pattern num

  appendSelectCross(select, crossover);  // Append select and crossover methods
  appendMutation(mutation);              // Append mutation method

  outFilename += ".txt";  // Append file extension to filename

} // setOutFilename()


void FileHandler::outputMatches(Board* theBoard,                 // *In*
                                int genCount)                    // *In*
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
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, TOP), 
              buff, 10);

      output[0] += "  ";      // Add whitespace for formatting
      output[0] += buff;      // Add converted pattern ID to top line
      output[0] += "  ";      // Add more whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, LEFT), 
              buff, 10);

      output[1] += buff;     // Add converted pattern ID to the middle line
      output[1] += "   ";    // Add whitespace for formatting

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, RIGHT), 
              buff, 10);

      output[1] += buff;     // Add right pattern ID to middle line

      // Convert pattern ID to char
      _itoa_s(BoardManager::getInstance()->getPattern(theBoard, i, j, BOTTOM), 
              buff, 10);

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


void FileHandler::outputIDs(Board* theBoard,                     // *In*
                            int genCount)                        // *In*
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


void FileHandler::appendSelectCross(int select,                  // *In*
                                    int crossover)               // *In*
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


void FileHandler::appendMutation(int mutation)                   // *In*
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


void FileHandler::makeDataFile(int size,                         // *In* 
                               int pattern)                      // *In*
{ // If no data file was found, generate a new random board and make the data
  // file that corrosponds to that board

  std::string filename = "Puzzles/BoardSize ";   // Hold file name to open
  char boardSize[3] = "/0";                      // Hold converted boardSize
  char patternNum[3] = "/0";                     // Hold converted pattern num

  _itoa_s(size, boardSize, 10);                 // Convert board size to char[]
  _itoa_s(pattern, patternNum, 10);             // Convert pattern num to char[]

  filename += boardSize;                     // Append the board size to name
  filename += " - Pattern ";                 // Append the pattern labal to name
  filename += patternNum;                    // Append the pattern num
  filename += ".e2";                         // Append file extension

  // Generate a new random board
  BoardManager::getInstance()->generateBoard(size, pattern);

  outputDataFile(filename);                 // Output the board to the data file

} // makeDataFile()


void FileHandler::outputDataFile(std::string filename)           // *In*
{ // Creates the data file by outputting the pieces to the file one piece per
  // line
  
  if (openFile(filename.c_str()))
  { // If file was opened, output data to file

    // Get the address of the piece vectors
    std::vector<std::vector<PuzzlePiece>>* pieceVecs =
      BoardManager::getInstance()->getPieces();

    for (int i = 0; i <= (int)pieceVecs[CORNER].size(); i++)
    { // Output the various bits of data of the corner pieces to the file

      theFile << (*pieceVecs)[0][i].pieceID << " ";     // Output piece ID    
      theFile << (*pieceVecs)[0][i].segments[0] << " "; // Output top pattern
      theFile << (*pieceVecs)[0][i].segments[1] << " "; // Output right pattern
      theFile << (*pieceVecs)[0][i].segments[2] << " "; // Output bottom pattern
      theFile << (*pieceVecs)[0][i].segments[3];        // Output left pattern
      
      theFile << std::endl;   // Move to next line in file
    }

    for (int i = 0; i < (int)(*pieceVecs)[EDGE].
         size(); i++)
    { // Output the various bits of data of the edge pieces to the file

      theFile << (*pieceVecs)[1][i].pieceID << " ";     // Output piece ID    
      theFile << (*pieceVecs)[1][i].segments[0] << " "; // Output top pattern
      theFile << (*pieceVecs)[1][i].segments[1] << " "; // Output right pattern
      theFile << (*pieceVecs)[1][i].segments[2] << " "; // Output bottom pattern
      theFile << (*pieceVecs)[1][i].segments[3];        // Output left pattern

      theFile << std::endl;  // Move to next line in file
    }

    for (int i = 0; i < (int)(*BoardManager::getInstance()->getPieces())[INNER].
         size(); i++)
    { // Output the various bits of data of the inner pieces to the file

      theFile << (*pieceVecs)[2][i].pieceID << " ";     // Output piece ID    
      theFile << (*pieceVecs)[2][i].segments[0] << " "; // Output top pattern
      theFile << (*pieceVecs)[2][i].segments[1] << " "; // Output right pattern
      theFile << (*pieceVecs)[2][i].segments[2] << " "; // Output bottom pattern
      theFile << (*pieceVecs)[2][i].segments[3];        // Output left pattern

      theFile << std::endl;  // Move to next line in file
    }

    theFile.close();         // Close file after use

  } // if (openFile(filename.c_str()))


} // outputDataFile()