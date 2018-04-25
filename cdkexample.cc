/*
 * Usage of CDK Matrix
 *
 * File:   example1.cc
 * Author: Stephen Perkins
 * Email:  stephen.perkins@utdallas.edu
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdint.h>
#include <inttypes.h>
#include "cdk.h"


#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Test Matrix"

using namespace std;

const int maxRecordStringLength = 25; 

class BinaryFileHeader 
{  
public:  
    uint32_t magicNumber;         /* Should be 0xFEEDFACE */   
    uint32_t versionNumber;   
    uint64_t numRecords; 
};
 
class BinaryFileRecord
{
public: 
  int strLength;   
  char stringBuffer[maxRecordStringLength]; 
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"a", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"a", "a", "b", "c"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */

  ifstream binInfile("cs3377.bin", ios::in|ios::binary);

  BinaryFileHeader *myHeader = new BinaryFileHeader();

  binInfile.read((char *) myHeader, sizeof(BinaryFileHeader));
  
  char magicStr[BOX_WIDTH];
  sprintf(magicStr, "Magic: %08x", myHeader->magicNumber);
  
  char versionStr[BOX_WIDTH];
  sprintf(versionStr, "Version: %"PRIu32"", myHeader->versionNumber);

  char numRecordsStr[BOX_WIDTH];
  sprintf(numRecordsStr, "NumRecords: %"PRIu64"", myHeader->numRecords); 
  
  BinaryFileRecord *myRecord = new BinaryFileRecord();

  for(int i = 0; i < 4; i++)
  {
    binInfile.read((char*) myRecord, sizeof(BinaryFileRecord));
    char stringlength[BOX_WIDTH];
    sprintf(stringlength, "strlen: %d", myRecord->strLength);
    setCDKMatrixCell(myMatrix, i+2, 1, stringlength);
    char happy[BOX_WIDTH];
    sprintf(happy, "%s", myRecord->stringBuffer);
    setCDKMatrixCell(myMatrix, i+2, 2, happy);
  }
  
  setCDKMatrixCell(myMatrix, 1, 1, magicStr);
  setCDKMatrixCell(myMatrix, 1, 2, versionStr);
  setCDKMatrixCell(myMatrix, 1, 3, numRecordsStr);
  binInfile.close();

  drawCDKMatrix(myMatrix, true);    /* required  */
  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
