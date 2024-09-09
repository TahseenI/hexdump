// hexdump.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 16
#define OUTPUTFILE_PATH "output"
FILE* fileToWrite;

void printHelp()
{
    printf("Usage:");

    printf("\n\t./hexdump <file_in>");
    printf("\n\t ./hexdump <file_in> <file_out>");
}

void printToFile(FILE* pOutputFile_, unsigned char* pucLineBuffer_, int iNumBytesToOutput_, int iMemoryOffset_, int iLineLength_) 
{
    // Left column (Memory Address)
    // offset must be incremented by the caller.
    fprintf(pOutputFile_, "0x%06X |", iMemoryOffset_);

    // Hexadecimal column
    for (int i = 0; i < iLineLength_; i++) 
    {
        // Group 2 bytes at a time
        if (i > 0 && i % 2 == 0) 
        {
            fprintf(pOutputFile_, "  ");
        }

        // Output bytes
        if (i < iNumBytesToOutput_) 
        {
            fprintf(pOutputFile_, " %02X", pucLineBuffer_[i]);
        } 
        else 
        {
            // End of input file - padding
            fprintf(pOutputFile_, "   ");
        }
    }

    // Right column (ASCII conversion)
    fprintf(pOutputFile_, " | ");
    for (int i = 0; i < iNumBytesToOutput_; i++) 
    {
        if (pucLineBuffer_[i] > 31 && pucLineBuffer_[i] < 127) 
        {
            fprintf(pOutputFile_, "%c", pucLineBuffer_[i]);
        } 
        else 
        {
            fprintf(pOutputFile_, ".");
        }
    }

    fprintf(pOutputFile_, "\n");
}

void printToLine(unsigned char* pucLineBuffer_, int iNumBytesToOutput_, int iMemoryOffset_, int iLineLength_) 
{
    // Left column (Memory Address)
    // offset must be incremented by the caller.
    printf("\t0x%06X |", iMemoryOffset_);

    // Hexadecimal column
    for (int i = 0; i < iLineLength_; i++) 
    {
        // Group 2 bytes at a time
        if (i > 0 && i % 2 == 0) 
        {
            printf(" ");
        }

        // Output bytes
        if (i < iNumBytesToOutput_) 
        {
            printf(" %02X", pucLineBuffer_[i]);
        } 
        else 
        {
            // End of input file - padding
            printf("   ");
        }
    }

    // Right column (ASCII conversion)
    printf(" | ");

    for (int i = 0; i < iNumBytesToOutput_; i++) 
    {
        if (pucLineBuffer_[i] > 31 && pucLineBuffer_[i] < 127) 
        {
            printf("%c", pucLineBuffer_[i]);
        } 
        else 
        {
            printf(".");
        }
    }

    printf( "\n");
}

void dumpFile(FILE *pFile_, int iFileSize_, int iLineLength_, int iOutputOption_)
{
    // Allocate a buffer for 1 line
    unsigned char* pucLineBuffer = (unsigned char*) malloc(iLineLength_);

    if (pucLineBuffer == NULL)
    {
        printf("ERROR: unable to allocate memory for the line buffer.");
        exit(EXIT_FAILURE);
    }

    int iBytesRemaining = iFileSize_;

    // Start at the beginning of file. Increment after each line
    int iMemoryOffset = 0;

    while (1) 
    {
        int iBytesToRead;

        // Determine the number of bytes to read.
        if (iBytesRemaining > iLineLength_) 
        {
            // Nominal case; get the full line
            iBytesToRead = iLineLength_;
        }
        else 
        {
            // iBytesRemaining < iLineLength; get the last line
            iBytesToRead = iBytesRemaining;
        }

        // Read as many chars as we can.
        // fread will at the bytes to read or EOF, whichever is first
        int iBytesReadFromFile = fread(pucLineBuffer, sizeof(unsigned char), iBytesToRead, pFile_);

        if (iBytesReadFromFile > 0) 
        {
            if (iOutputOption_ == 0)
            {
                printToLine(pucLineBuffer, iBytesReadFromFile, iMemoryOffset, iLineLength_);
            }
            else if (iOutputOption_ == 1)
            {
                printToFile(fileToWrite, pucLineBuffer, iBytesReadFromFile, iMemoryOffset, iLineLength_);
            }

            // update the offset to read from, and the bytes left to read
            iMemoryOffset += iBytesReadFromFile;
            iBytesRemaining -= iBytesReadFromFile;
        } 
        else 
        {
            // no more bytes to read
            break;
        }
    }

    // Free the buffer's memory upon exiting.
    free(pucLineBuffer);
}

int main(int argc, char** argv)
{
    // argv[0] contains the executable
    // argv[1] contains the filename
    // argv[2] contains output option (0 by default)
    if(argc == 1)
    {
        printHelp();
        return 0;
    }

#if DEBUG 
    printf("ARGS: \n");
    for (int i = 0; i < argc; i++)
    {
        printf("\n argv[%d] => %s", i, argv[i]);
    }
#endif

    // Arg parse for file names. Make sure it has a "." in the extension
    char* szInputFileName = argv[1];
    char* szOutputFileName = argv[2];

    int printOption = 0;

    // Open input file.
    FILE* pFileToRead;
    pFileToRead = fopen(szInputFileName, "rb");
    if(pFileToRead == NULL)
    {
        printf("\n\n<ERROR OPENING INPUT FILE> Please check if the file path is correct.");
        return -1;
    }

    // Determine the file size.
    size_t iBytesToRead = 0;
    fseek(pFileToRead, 0, SEEK_END);
    iBytesToRead = ftell(pFileToRead);

    // Set input file pointer back to the beginning
    fseek(pFileToRead, 0, SEEK_SET);
    printf("\nFile Name: %s | File Size: %d\n\n", szInputFileName, iBytesToRead);
    
    // Setup output file, if applicable
    if (szOutputFileName != NULL)
    {
        printOption = 1;
        char filePath[32];

        sprintf(filePath, "%s/%s", OUTPUTFILE_PATH, szOutputFileName);

        fileToWrite = fopen(filePath, "w");
        if (!fileToWrite)
        {
            printf("Can't create output file: %s. \n\nMake sure /output/ folder exists.", filePath);
            exit(EXIT_FAILURE);
        }
    }
    
#if DEBUG
    printf("\t\t | 00 01  02 03  04 05  06 07  08 09  0A 0B  0C 0D  0E 0F |\n\n");
#endif

    // Ready to dump File contents.
    dumpFile(pFileToRead, iBytesToRead, LINE_LENGTH, printOption);

    if (pFileToRead)
    {
        fclose(pFileToRead);
    }
    if(fileToWrite)
    {
        fclose(fileToWrite);
    }

    return 0;
}
