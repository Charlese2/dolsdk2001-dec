#include <dolphin/gd/GDFile.h>
#include <dolphin/os.h>


typedef struct _GDFileHeader {
  u32 versionNumber;
  u32 numDLs;
  u32 numPLs;
  GDGList* DLDescArray;
  GDGList* PLDescArray;
} GDFileHeader;

#define ROUND(n, a) (((u32)(n) + (a)-1) & ~((a)-1))

s32 GDReadDLFile(char* fName, u32* numDLs, u32* numPLs,
                 GDGList** DLDescArray, GDGList** PLDescArray)
{
  DVDFileInfo finfo; // r1+0x1C
  u32 length; // r27
  u32 i; // r31
  u8* buf; // r28
  GDFileHeader* hdr; // r30

  *numDLs = *numPLs = 0;
  *DLDescArray = *PLDescArray = NULL;

  if (!DVDOpen(fName, &finfo)) {
    OSReport("Can't open file %s\n", fName);
    return -3;
  }

  length = finfo.length;

  if (NULL == (buf = OSAlloc(ROUND(length, 0x20)))) {
    OSReport("Alloc failed\n");
    DVDClose(&finfo);
    return -5;
  }

  if (DVDReadPrio(&finfo, buf, ROUND(length, 0x20), 0, 2)
      != ROUND(length, 0x20)) {
    OSReport("Error occurred when reading %s\n", fName);
    DVDClose(&finfo);
    OSFree(buf);
    return -2;
  }

  DVDClose(&finfo);

  hdr = (GDFileHeader*) buf;

  if (hdr->versionNumber != 0x11223344)
  {
    OSReport("Bad version number for GDL file %s\n", fName);
    OSFree(buf);
    return -4;
  }

  *numDLs = hdr->numDLs;
  *numPLs = hdr->numPLs;

  // Loaded pointers are relative to disc start, remap
  // them to be relative to RAM start
  *DLDescArray = (GDGList*)((u32)hdr->DLDescArray + (u32)hdr);
  *PLDescArray = (GDGList*)((u32)hdr->PLDescArray + (u32)hdr);

  // And internal pointers too
  for (i = 0; i < *numDLs; ++i) {
    (*DLDescArray)[i].ptr = (void*)((u32)hdr + (u32)(*DLDescArray)[i].ptr);
  }
  for (i = 0; i < *numPLs; ++i) {
    (*PLDescArray)[i].ptr = (void*)((u32)hdr + (u32)(*PLDescArray)[i].ptr);
  }

  return 0;
}
