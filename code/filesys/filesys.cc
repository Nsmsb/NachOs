// filesys.cc 
//	Routines to manage the overall operation of the file system.
//	Implements routines to map from textual file names to files.
//
//	Each file in the file system has:
//	   A file header, stored in a sector on disk 
//		(the size of the file header data structure is arranged
//		to be precisely the size of 1 disk sector)
//	   A number of data blocks
//	   An entry in the file system directory
//
// 	The file system consists of several data structures:
//	   A bitmap of free disk sectors (cf. bitmap.h)
//	   A directory of file names and file headers
//
//      Both the bitmap and the directory are represented as normal
//	files.  Their file headers are located in specific sectors
//	(sector 0 and sector 1), so that the file system can find them 
//	on bootup.
//
//	The file system assumes that the bitmap and directory files are
//	kept "open" continuously while Nachos is running.
//
//	For those operations (such as Create, Remove) that modify the
//	directory and/or bitmap, if the operation succeeds, the changes
//	are written immediately back to disk (the two files are kept
//	open during all this time).  If the operation fails, and we have
//	modified part of the directory and/or bitmap, we simply discard
//	the changed version, without writing it back to disk.
//
// 	Our implementation at this point has the following restrictions:
//
//	   there is no synchronization for concurrent accesses
//	   files have a fixed size, set when the file is created
//	   files cannot be bigger than about 3KB in size
//	   there is no hierarchical directory structure, and only a limited
//	     number of files can be added to the system
//	   there is no attempt to make the system robust to failures
//	    (if Nachos exits in the middle of an operation that modifies
//	    the file system, it may corrupt the disk)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "disk.h"
#include "bitmap.h"
#include "directory.h"
#include "filehdr.h"
#include "filesys.h"

#define MAX_STRING_LEN 128

// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known 
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1

// Initial file sizes for the bitmap and directory; until the file system
// supports extensible files, the directory size sets the maximum number 
// of files that can be loaded onto the disk.
#define FreeMapFileSize 	(NumSectors / BitsInByte)
#define NumDirEntries 		10
#define DirectoryFileSize 	(sizeof(DirectoryEntry) * NumDirEntries)
#define MaxDepth 3
#define NumOpenFiles 10



// utility function to parse file path
// it returns char** array of size MAX_DEPTH + 2 (depth, pathstring)
// last 3 elements are filename, depth, and string that contains names of folders
// (same string as path, but all '/' are remplaced by '\0')
char** parsePath(char *path) {
    char *dirNames = new char[MAX_STRING_LEN];
	char **directories = new char*[MaxDepth+2];
	int path_len = strlen(path);
	int depth = 0;
	int next_dir = 0;

	// skiping the '/' char
	if (path[0] == '/')
		path++;
	strcpy(dirNames, path);


	// replacing all '/' by '\0'
	int i = 0;
	for (; i < path_len && depth <= MaxDepth; i++)
	{
		if (dirNames[i] == '/')
		{
			dirNames[i] = '\0';
			directories[depth++] = &dirNames[next_dir];
			next_dir = i+1;
		}
	}
	
	directories[MaxDepth] = (char*) depth;
	directories[MaxDepth+1] = dirNames;
	return directories;	
}


// Utiliyu function: it takes file name (or path), return pointer points to
// the base name (file name).
char* findFileName(char *name)
{
	char *result = name;
	for (int i = 0; i < (int)strlen(name); i++)
		if (name[i] == '/') 
			result = &name[i+1];


	return result;
}

// FileSystem::static vraible
static OpenFileEntry *openFilesData[NumOpenFiles];
OpenFileEntry** FileSystem::OpenFilesTable = openFilesData;
BitMap* FileSystem::OpenFileMap = new BitMap(NumOpenFiles);


//----------------------------------------------------------------------
// FileSystem::FileSystem
// 	Initialize the file system.  If format = TRUE, the disk has
//	nothing on it, and we need to initialize the disk to contain
//	an empty directory, and a bitmap of free sectors (with almost but
//	not all of the sectors marked as free).  
//
//	If format = FALSE, we just have to open the files
//	representing the bitmap and the directory.
//
//	"format" -- should we initialize the disk?
//----------------------------------------------------------------------

FileSystem::FileSystem(bool format)
{ 
    DEBUG('f', "Initializing the file system.\n");
    if (format) {
        BitMap *freeMap = new BitMap(NumSectors);
        Directory *directory = new Directory(NumDirEntries);
	FileHeader *mapHdr = new FileHeader;
	FileHeader *dirHdr = new FileHeader;

        DEBUG('f', "Formatting the file system.\n");

    // First, allocate space for FileHeaders for the directory and bitmap
    // (make sure no one else grabs these!)
	freeMap->Mark(FreeMapSector);	    
	freeMap->Mark(DirectorySector);

    // Second, allocate space for the data blocks containing the contents
    // of the directory and bitmap files.  There better be enough space!

	ASSERT(mapHdr->Allocate(freeMap, FreeMapFileSize));
	ASSERT(dirHdr->Allocate(freeMap, DirectoryFileSize));

    // Flush the bitmap and directory FileHeaders back to disk
    // We need to do this before we can "Open" the file, since open
    // reads the file header off of disk (and currently the disk has garbage
    // on it!).

        DEBUG('f', "Writing headers back to disk.\n");
	mapHdr->WriteBack(FreeMapSector);    
	dirHdr->WriteBack(DirectorySector);

    // OK to open the bitmap and directory files now
    // The file system operations assume these two files are left open
    // while Nachos is running.

        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
     
    // Once we have the files "open", we can write the initial version
    // of each file back to disk.  The directory at this point is completely
    // empty; but the bitmap has been changed to reflect the fact that
    // sectors on the disk have been allocated for the file headers and
    // to hold the file data for the directory and bitmap.

        DEBUG('f', "Writing bitmap and directory back to disk.\n");
	freeMap->WriteBack(freeMapFile);	 // flush changes to disk
	directory->WriteBack(directoryFile);

	if (DebugIsEnabled('f')) {
	    freeMap->Print();
	    directory->Print();

        delete freeMap; 
	delete directory; 
	delete mapHdr; 
	delete dirHdr;
	}
    } else {
    // if we are not formatting the disk, just open the files representing
    // the bitmap and directory; these are left open while Nachos is running
        freeMapFile = new OpenFile(FreeMapSector);
        directoryFile = new OpenFile(DirectorySector);
    }
}

//----------------------------------------------------------------------
// FileSystem::isOpen
// 	search in OpenFilesTable, returns the index of the first Entry that matches the given FileSector
//	  if the file is not Open before, returns -1
//----------------------------------------------------------------------

int
FileSystem::isOpen(int fileSector)
{
	int result = -1;
	// TODO: acquire mutex
	for (int i = 0; i < NumOpenFiles; i++)
		if (OpenFilesTable[i] != NULL && OpenFilesTable[i]->sector == fileSector)
		{
			result = i;
			break;
		}
	return result;
}

int
FileSystem::FindDirectorySector(const char *name)
{
	char filePath[MAX_STRING_LEN];
	strcpy(filePath, name);

	char **directories = parsePath(filePath);
	int depth = (int)directories[MaxDepth];
	printf("depth = %d\n\n", depth);


	int sector = DirectorySector; // Start from root dir

    if(depth != 0) { // i.e. not root
        OpenFile* dirFile;
        Directory* dirTemp;

        for (int i = 0; i < depth; i++) {
            DEBUG('D', "Finding directory \"%s\" in sector \"%d\"\n", directories[i], sector);
            dirFile = new OpenFile(sector);
            dirTemp = new Directory(NumDirEntries);
            dirTemp->FetchFrom(dirFile);
            sector = dirTemp->Find(directories[i]);
            if (sector == -1)
                break; // Not found
        }
        delete dirFile;
        delete dirTemp;
    }

	delete directories[MaxDepth+1];
	delete directories;

    return sector;
}

//----------------------------------------------------------------------
// FileSystem::Create
// 	Create a file in the Nachos file system (similar to UNIX create).
//	Since we can't increase the size of files dynamically, we have
//	to give Create the initial size of the file.
//
//	The steps to create a file are:
//	  Make sure the file doesn't already exist
//        Allocate a sector for the file header
// 	  Allocate space on disk for the data blocks for the file
//	  Add the name to the directory
//	  Store the new file header on disk 
//	  Flush the changes to the bitmap and the directory back to disk
//
//	Return TRUE if everything goes ok, otherwise, return FALSE.
//
// 	Create fails if:
//   		file is already in directory
//	 	no free space for file header
//	 	no free entry for file in directory
//	 	no free space for data blocks for the file 
//
// 	Note that this implementation assumes there is no concurrent access
//	to the file system!
//
//	"name" -- name of file to be created
//	"initialSize" -- size of file to be created
//----------------------------------------------------------------------

bool
FileSystem::Create(const char *name, int initialSize)
{
    Directory *directory;
    BitMap *freeMap;
    FileHeader *hdr;
	OpenFile *parentDirFile;
	int parentDirSector;
    int sector;
    bool success;
	bool isDir = FALSE;

	// findinf base file name
	char path[strlen(name)];
	strcpy(path, name);
	char *baseName = findFileName(path);

    DEBUG('f', "Creating file %s, size %d\n", name, initialSize);

	parentDirSector = FindDirectorySector(name);	// finding parent directory
	if (parentDirSector == -1)						// wrong path, directory not found
		return FALSE;

	if (initialSize == -1)
	{
		isDir = TRUE;
		initialSize = DirectoryFileSize;
	}
	
	
	parentDirFile = new OpenFile(parentDirSector);
    directory = new Directory(NumDirEntries);
    // directory->FetchFrom(directoryFile);
    directory->FetchFrom(parentDirFile);

    if (directory->Find(baseName) != -1)
      success = FALSE;			// file is already in directory
    else {	
        freeMap = new BitMap(NumSectors);
        freeMap->FetchFrom(freeMapFile);
        sector = freeMap->Find();	// find a sector to hold the file header
    	if (sector == -1) 		
            success = FALSE;		// no free block for file header 
        else if (!directory->Add(baseName, sector))
            success = FALSE;	// no space in directory
		else {
			hdr = new FileHeader;
			if (!hdr->Allocate(freeMap, initialSize))
					success = FALSE;	// no space on disk for data
			else {	
				success = TRUE;
				// everthing worked, flush all changes back to disk
				if (isDir)					// setting file type
					hdr->setDirType();
				hdr->WriteBack(sector); 		
				directory->WriteBack(parentDirFile);
				if(isDir) {
					Directory* dir = new Directory(NumDirEntries);
					OpenFile* subDirFile = new OpenFile(sector);
					dir->WriteBack(subDirFile);
					delete dir;
					delete subDirFile;
				}
				freeMap->WriteBack(freeMapFile);
			}
			delete hdr;
		}
        delete freeMap;
    }
    delete directory;
    return success;
}

//----------------------------------------------------------------------
// FileSystem::CreateDir
// 	Create a directory file in the Nachos file system, it uses the FileSystem::Create
//----------------------------------------------------------------------

bool
FileSystem::CreateDir(const char *name)
{
	return Create(name, -1);
}

//----------------------------------------------------------------------
// FileSystem::RemoveDir
// 	Remove a directory file in the Nachos file system, it uses the FileSystem::Remove
//----------------------------------------------------------------------

bool
FileSystem::RemoveDir(const char *name)
{
	OpenFile *dirFile = Open(name);
	Directory *dir = new Directory(NumDirEntries);
	bool success = FALSE;

	if (dirFile != NULL)
	{
		dir->FetchFrom(dirFile);
		if (dir->isEmpty())
			success = Remove(name);
	}
	
	return success;	
}

//----------------------------------------------------------------------
// FileSystem::Open
// 	Open a file for reading and writing.  
//	To open a file:
//	  Find the location of the file's header, using the directory 
//	  Bring the header into memory
//
//	"name" -- the text name of the file to be opened
//----------------------------------------------------------------------

OpenFile *
FileSystem::Open(const char *name)
{ 
    Directory *directory = new Directory(NumDirEntries);
    OpenFile *parentDirFile = NULL;
    OpenFile *openFile = NULL;
	int parentDirSector;
	int openFileIndex;
    int sector = -1;

	// findinf base file name
	char path[strlen(name)];
	strcpy(path, name);
	char *baseName = findFileName(path);

    DEBUG('f', "Opening file %s\n", name);
	parentDirSector = FindDirectorySector(name);
	// ASSERT(parentDirSector != -1);		
	if (parentDirSector != 1)
	{
		parentDirFile = new OpenFile(parentDirSector);
		// directory->FetchFrom(directoryFile);
		directory->FetchFrom(parentDirFile);
		sector = directory->Find(baseName); 
	}
    if (sector >= 0) {				// name was found in directory 
		openFileIndex = OpenFileMap->Find();
		if (openFileIndex != -1)	// there is an empty case in the OpenFiles Table to open the file
		{
			openFile = new OpenFile(sector);	
			OpenFilesTable[openFileIndex] = new OpenFileEntry(sector, openFile);
		}
	}
    delete directory;
	delete parentDirFile;
    return openFile;				// return NULL if not found
}

//----------------------------------------------------------------------
// FileSystem::Remove
// 	Delete a file from the file system.  This requires:
//	    Remove it from the directory
//	    Delete the space for its header
//	    Delete the space for its data blocks
//	    Write changes to directory, bitmap back to disk
//
//	Return TRUE if the file was deleted, FALSE if the file wasn't
//	in the file system.
//
//	"name" -- the text name of the file to be removed
//----------------------------------------------------------------------

bool
FileSystem::Remove(const char *name)
{ 
    Directory *directory;
    BitMap *freeMap;
    FileHeader *fileHdr;
	OpenFile *parentDirFile;
	int parentDirSector;
    int sector;

	// findinf base file name
	char path[strlen(name)];
	strcpy(path, name);
	char *baseName = findFileName(path);

	// finding parent dir
	parentDirSector = FindDirectorySector(name);
	if (parentDirSector == -1)			// making sure we're working with valide path
		return FALSE;
	parentDirFile = new OpenFile(parentDirSector);
    directory = new Directory(NumDirEntries);
	directory->FetchFrom(parentDirFile);

	// finding file sector
	sector = directory->Find(baseName); 
    if (sector == -1) {
       delete directory;
       return FALSE;			 // file not found 
    }
    fileHdr = new FileHeader;
    fileHdr->FetchFrom(sector);

    freeMap = new BitMap(NumSectors);
    freeMap->FetchFrom(freeMapFile);

    fileHdr->Deallocate(freeMap);  		// remove data blocks
    freeMap->Clear(sector);			// remove header block
    directory->Remove(name);

    freeMap->WriteBack(freeMapFile);		// flush to disk
    directory->WriteBack(directoryFile);        // flush to disk
    delete fileHdr;
    delete directory;
    delete freeMap;
    return TRUE;
} 

//----------------------------------------------------------------------
// FileSystem::List
// 	List all the files in the file system directory.
//----------------------------------------------------------------------

void
FileSystem::List()
{
    Directory *directory = new Directory(NumDirEntries);

    directory->FetchFrom(directoryFile);
    directory->List();
    delete directory;
}

//----------------------------------------------------------------------
// FileSystem::Print
// 	Print everything about the file system:
//	  the contents of the bitmap
//	  the contents of the directory
//	  for each file in the directory,
//	      the contents of the file header
//	      the data in the file
//----------------------------------------------------------------------

void
FileSystem::Print()
{
    FileHeader *bitHdr = new FileHeader;
    FileHeader *dirHdr = new FileHeader;
    BitMap *freeMap = new BitMap(NumSectors);
    Directory *directory = new Directory(NumDirEntries);

    printf("Bit map file header:\n");
    bitHdr->FetchFrom(FreeMapSector);
    bitHdr->Print();

    printf("Directory file header:\n");
    dirHdr->FetchFrom(DirectorySector);
    dirHdr->Print();

    freeMap->FetchFrom(freeMapFile);
    freeMap->Print();

    directory->FetchFrom(directoryFile);
    directory->Print();

    delete bitHdr;
    delete dirHdr;
    delete freeMap;
    delete directory;
} 


// OpenFileEntery Class

OpenFileEntry::OpenFileEntry(int fileSector, OpenFile *file)
{
	this->sector = fileSector;
	this->openFile = file;
}

OpenFileEntry::~OpenFileEntry()
{

}