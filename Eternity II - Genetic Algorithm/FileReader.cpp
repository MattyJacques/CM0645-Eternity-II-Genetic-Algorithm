// Title        : FileReader.cpp
// Purpose      : Reads piece information from a e2 file, storing in Board.h
// Author       : Matthew Jacques
// Date         : 19/11/2015


#include "FileReader.h"     // Include the header for the class
#include "Crossover.h"      // Selection and Crossover type
#include "Mutation.h"       // Mutation type
#include <iostream>         // Include input and output library
#include <windows.h>        // Include file directory functions


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

  // Open the file
  theFile.open(fileName);

  // Check to see if file is open
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

  // Read in puzzle pieces
  ReadDataFile(setData.boardSize, setData.patternNum);

  // Calculate output filename
  SetOutFilename(setData.boardSize, setData.patternNum);

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


void FileReader::ReadDataFile(int size, int pattern)
{ // Reads the piece file with the file name that matches the information passed
  // as parameter storing piece info in the piece collection vector

  // Get the index of the file name matching the data of the board size and
  // number of patterns
  int index = GetDataFilename(size, pattern);
  if (OpenFile(filenames[index].c_str()))
  { // Checks to see if the file is open before proceeding with reading of the
    // file

    std::string line;                     // Stores current line to be parsed
    int pData[5];                         // Holds parsed data from the line

    while (std::getline(theFile, line))
    { // While getline actually returns a line of data, proceed with parsing
      ParseData(line, pData);
      CreatePiece(pData);
    } 

    theFile.close();                                    // Close the file after use
    printf("Loaded: %s\n\n", filenames[index].c_str()); // Output success
  }
  else
  { // If the file was not open, output file not exist error
    printf("File does not exist.\n");
  }

} // ReadPieceFile()


int FileReader::GetDataFilename(int size, int pattern)
{ // Find the correct filename from the vector of puzzle file names found
  // during the directory scan

  std::string name = "Puzzles/BoardSize ";      // Hold file name to open
  int result = 0;                               // Hold index of filename
  char boardSize[3];                            // Hold converted boardSize
  char patternNum[3];                           // Hold converted pattern num

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

  std::string fileName = "Solutions/Generation ";  // Filename of output
  char intBuff[10];             // Holds integer that has been converted to char
  std::string topLine;         // Holds the first row of output
  std::string midLine;         // Holds the middle row of output
  std::string botLine;         // Holds the bottom row of output

  // Convert the number of generations to a char
  itoa(genCount, intBuff, 10);

  // Add converted char to string then add file extension
  fileName += intBuff;
  fileName += ".txt";

  // If the solution directory does not exist, create it
  if (!DirExists("Solutions"))
    CreateDir("Solutions");

  // Create new file object with output permission. (New object needed due to
  // constructor creating a new file)
  std::ofstream output(fileName, std::ios::out);

  if (output.is_open())
  { // If file has been created and open successfully format the output

    for (int j = 0; j <= BoardManager::GetInstance()->boardSize; j++)
    { // Y index for pieces to output
      for (int i = 0; i <= BoardManager::GetInstance()->boardSize; i++)
      { // X index for pieces to ouput, parse a piece into three rows
        // of output.

        // Reset the char array for conversion and convert pattern ID to char
        intBuff[0] = '/0';
        itoa(BoardManager::GetInstance()->GetPattern(pBoard, i, j, TOP), 
             intBuff, 10);

        topLine += "  ";    // Add whitespace for formatting
        topLine += intBuff;  // Add converted pattern ID to top line
        topLine += "  ";    // Add more whitespace for formatting

        // Reset the char array for conversion and convert pattern ID to char
        intBuff[0] = '/0';
        itoa(BoardManager::GetInstance()->GetPattern(pBoard, i, j, LEFT), 
             intBuff, 10);

        midLine += intBuff;  // Add converted pattern ID to the middle line
        midLine += "   ";   // Add whitespace for formatting

        // Reset the char array for conversion and convert pattern ID to char
        intBuff[0] = '/0';
        itoa(BoardManager::GetInstance()->GetPattern(pBoard, i, j, RIGHT), 
             intBuff, 10);

        midLine += intBuff;  // Add right pattern ID to middle line

        // Reset the char array for conversion and convert pattern ID to char
        intBuff[0] = '/0';
        itoa(BoardManager::GetInstance()->GetPattern(pBoard, i, j, BOTTOM), 
             intBuff, 10);

        botLine += "  ";    // Add whitespace for formatting
        botLine += intBuff;  // Add bottom pattern ID to bottom line
        botLine += "  ";    // Ass whitepsace for formatting
      } // for i < boardSize

      output << topLine << std::endl;   // Output the top line to file
      output << midLine << std::endl;   // Output the middle line to file
      output << botLine << std::endl;   // Output the bottom line to file

      topLine.clear();                  // Clear top string for next line
      midLine.clear();                  // Clear middle string for next line
      botLine.clear();                  // Clear bottom string for next line
    } // for j < boardSize

    // Close the file after use 
    output.close();

  } // if file is open

} // OutputBoard()


bool FileReader::CreateDir(const char* dirName)
{ // Makes a directory in the application root folder using the name provided

  bool result = false;    // Hold if successful or not
  wchar_t wtext[50];      // Holds converted dirName

  // Convert the dirName to wide char
  mbstowcs(wtext, dirName, strlen(dirName + 1));

  // If directory created successfully change result to true
  if (CreateDirectory(wtext, NULL))
    result = true;

  return result;   // Return result

} // CreateDir()


bool FileReader::DirExists(const char* dirName)
{ // Checks to see if the directory with the name provided exists in the 
  // applications root folder

  // Get the folder attributes with the given path
  DWORD fileAtt = ::GetFileAttributesA(dirName);

  // Return if a directory and if there is a fault with directory
  return (fileAtt & FILE_ATTRIBUTE_DIRECTORY && 
          fileAtt != INVALID_FILE_ATTRIBUTES);

} // DirExists()


void FileReader::OutputFitness(int fitness)
{ // Appends the fitness to file for tracking of algorithm performance

  std::ofstream output(outFilename.c_str(), std::ios::out | std::ios::app);

  if (output.is_open())
  { // If output file opened successfully output the fitness

    char intBuff[10];  // For conversion of fitness int

    // Convert the fitness integer and output the line to the file
    itoa(fitness, intBuff, 10);
    output << intBuff << std::endl;

    // Close file after use
    output.close();
  }

} // OutputFitness()


void FileReader::SetOutFilename(int boardSize, int patternNum)
{ // Calculate the output filename

  outFilename = "Solutions/BoardSize ";
  char intBuff[10];

  itoa(boardSize, intBuff, 10);
  outFilename += intBuff;

  outFilename += " Pattern ";

  intBuff[0] = '/0';
  itoa(patternNum, intBuff, 10);
  outFilename += intBuff;

  outFilename += ".txt";

} // SetOutFilename()