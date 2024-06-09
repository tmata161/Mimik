#ifndef INTERNALFILESYSTEM
#define INTERNALFILESYSTEM

#include"lfs.h"
#include"hardware/sync.h"
#include "hardware/regs/addressmap.h"
#include "hardware/flash.h"

//---------------STATIC MACROS----------
#define FS_SIZE (128 * 1024)
//-------------STATIC MACROS END---------

//--------------FUNCTION MACROS----------
#define FLUSH(p) free(p);p=NULL
//--------------FUNCTION MACROS END----------

//------------------USER APT FUNCTION DECLARATION--------------
void lfsMount(); //mounts internal filesystem
char* lfsFileRead(lfs_file_t* file, int fileSize); //returns the content of the file
int lfsFileSize(lfs_file_t* file); //returns the file size
void lfsFileWrite(lfs_file_t* file, char* fileContent); // writes content to the file
void lfsUnMount();//unmounts internal filesystem
//--------------------END----------------------------------

#endif