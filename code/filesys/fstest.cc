// fstest.cc 
//	Simple test routines for the file system.  
//
//	We implement:
//	   Copy -- copy a file from UNIX to Nachos
//	   Print -- cat the contents of a Nachos file 
//	   Perftest -- a stress test for the Nachos file system
//		read and write a really large file in tiny chunks
//		(won't work on baseline system!)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "utility.h"
#include "filesys.h"
#include "directory.h"
#include "system.h"
#include "thread.h"
#include "disk.h"
#include "stats.h"

#define TransferSize 	10 	// make it small, just to be difficult

//----------------------------------------------------------------------
// Copy
// 	Copy the contents of the UNIX file "from" to the Nachos file "to"
//----------------------------------------------------------------------

void
Copy(const char *from, const char *to)
{
    FILE *fp;
    OpenFile* openFile;
    int amountRead, fileLength;
    char *buffer;

// Open UNIX file
    if ((fp = fopen(from, "r")) == NULL) {	 
	printf("Copy: couldn't open input file %s\n", from);
	return;
    }

// Figure out length of UNIX file
    fseek(fp, 0, 2);		
    fileLength = ftell(fp);
    fseek(fp, 0, 0);

// Create a Nachos file of the same length
    DEBUG('f', "Copying file %s, size %d, to file %s\n", from, fileLength, to);
    if (!fileSystem->Create(to, fileLength)) {	 // Create Nachos file
	printf("Copy: couldn't create output file %s\n", to);
	fclose(fp);
	return;
    }
    
    openFile = fileSystem->Open(to);
    ASSERT(openFile != NULL);
    
// Copy the data in TransferSize chunks
    buffer = new char[TransferSize];
    while ((amountRead = fread(buffer, sizeof(char), TransferSize, fp)) > 0)
	openFile->Write(buffer, amountRead);	
    delete [] buffer;

// Close the UNIX and the Nachos files
    delete openFile;
    fclose(fp);
}

//----------------------------------------------------------------------
// Print
// 	Print the contents of the Nachos file "name".
//----------------------------------------------------------------------

void
Print(char *name)
{
    OpenFile *openFile;    
    int i, amountRead;
    char *buffer;

    if ((openFile = fileSystem->Open(name)) == NULL) {
	printf("Print: unable to open file %s\n", name);
	return;
    }
    
    buffer = new char[TransferSize];
    while ((amountRead = openFile->Read(buffer, TransferSize)) > 0)
	for (i = 0; i < amountRead; i++)
	    printf("%c", buffer[i]);
    delete [] buffer;

    delete openFile;		// close the Nachos file
    return;
}

//----------------------------------------------------------------------
// PerformanceTest
// 	Stress the Nachos file system by creating a large file, writing
//	it out a bit at a time, reading it back a bit at a time, and then
//	deleting the file.
//
//	Implemented as three separate routines:
//	  FileWrite -- write the file
//	  FileRead -- read the file
//	  PerformanceTest -- overall control, and print out performance #'s
//----------------------------------------------------------------------

#define FileName 	"TestFile"
#define Contents 	"1234567890"
#define ContentSize 	strlen(Contents)
#define FileSize 	((int)(ContentSize * 5000))

static void 
FileWrite()
{
    OpenFile *openFile;    
    int i, numBytes;

    printf("Sequential write of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);
    if (!fileSystem->Create(FileName, 0)) {
      printf("Perf test: can't create %s\n", FileName);
      return;
    }
    openFile = fileSystem->Open(FileName);
    if (openFile == NULL) {
	printf("Perf test: unable to open %s\n", FileName);
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Write(Contents, ContentSize);
	if (numBytes < 10) {
	    printf("Perf test: unable to write %s\n", FileName);
	    delete openFile;
	    return;
	}
    }
    delete openFile;	// close file
}

static void 
FileRead()
{
    OpenFile *openFile;    
    char *buffer = new char[ContentSize];
    int i, numBytes;

    printf("Sequential read of %d byte file, in %zd byte chunks\n", 
	FileSize, ContentSize);

    if ((openFile = fileSystem->Open(FileName)) == NULL) {
	printf("Perf test: unable to open file %s\n", FileName);
	delete [] buffer;
	return;
    }
    for (i = 0; i < FileSize; i += ContentSize) {
        numBytes = openFile->Read(buffer, ContentSize);
	if ((numBytes < 10) || strncmp(buffer, Contents, ContentSize)) {
	    printf("Perf test: unable to read %s\n", FileName);
	    delete openFile;
	    delete [] buffer;
	    return;
	}
    }
    delete [] buffer;
    delete openFile;	// close file
}

void
PerformanceTest()
{
    printf("Starting file system performance test:\n");
    stats->Print();
    FileWrite();
    FileRead();
    if (!fileSystem->Remove(FileName)) {
      printf("Perf test: unable to remove %s\n", FileName);
      return;
    }
    stats->Print();
}

void
LsDir(char *name)
{
	OpenFile *dirFile = fileSystem->Open(name);
	Directory *dir = new Directory(10);		//NumDirEntries
	dir->FetchFrom(dirFile);
	dir->List();
	fileSystem->Close(dirFile);
	delete dir;
	delete dirFile;	
}

void
FsTest()
{
	bool success = FALSE;
	char fileNames[10][128+1];
	char dirNames[5][128+1];

	// directories
	strcpy(dirNames[0], "dir1");
	strcpy(dirNames[1], "dir1/dir2");
	strcpy(dirNames[2], "dir3");
	strcpy(dirNames[3], "dir3/dir4");
	strcpy(dirNames[4], "dir3/dir4/dir5");

	// files
	strcpy(fileNames[0], "f1");
	strcpy(fileNames[1], "f2");
	strcpy(fileNames[2], "dir1/f3");
	strcpy(fileNames[3], "dir1/f4");
	strcpy(fileNames[4], "dir1/dir2/f5");
	strcpy(fileNames[5], "dir1/dir2/f6");
	strcpy(fileNames[6], "dir3/dir4/dir5/f7");
	strcpy(fileNames[7], "dir3/dir4/f8");
	strcpy(fileNames[8], "dir3/f9");
	strcpy(fileNames[9], "f10");
	



	printf("########### FILE SYSTEM TEST ###########\n\n");
	printf("creating the next files tree (5 directories, 10 files)\n");
	printf(".\n");
	printf("├── dir1\n");
	printf("│   ├── dir2\n");
	printf("│   │   ├── f5\n");
	printf("│   │   └── f6\n");
	printf("│   ├── f3\n");
	printf("│   └── f4\n");
	printf("├── dir3\n");
	printf("│   ├── dir4\n");
	printf("│   │   ├── dir5\n");
	printf("│   │   │   └── f7\n");
	printf("│   │   └── f8\n");
	printf("│   └── f9\n");
	printf("├── f1\n");
	printf("├── f10\n");
	printf("└── f2\n");

	printf("\n########### FILE SYSTEM Creating Directories ###########\n\n");

	for (int i = 0; i < 5; i++)
	{
		success = fileSystem->CreateDir(dirNames[i]);
		printf("[CreateDir() test %d] Directory %s ", i, dirNames[i]);
		printf(success ? "\033[32;1mOK\033[0m\n" : "ERROR\n");
		ASSERT(success);
	}
	
	printf("\n########### FILE SYSTEM Creating Files ###########\n\n");
	for (int i = 0; i < 10; i++)
	{
		success = fileSystem->CreateDir(fileNames[i]);
		printf("[Create() test %d] File %s ", i, fileNames[i]);
		printf(success ? "\033[32;1mOK\033[0m\n" : "ERROR\n");
		ASSERT(success);
	}



}
