#ifndef MP3_H
#define MP3_H

#include "types.h"

/* ----- Structure definition ----- */
typedef struct _MP3File
{
    char filename[256];       // MP3 filename
    FILE *mp3_ptr;            // file pointer for reading
    unsigned int filesize;    // size of mp3 file in bytes
    char mp3_data[256];       // buffer (unused in this simplified API)

    FILE *duplicate; 
    
} MP3File;

  
/* ----- function prototypes ----- */

/* To check operation type from command-line-arguments*/
OperationType identify_operation(int argc, char *argv[]);

/* To validate and  read arguments from read mode */
Status read_and_validate_view_args(int argc, char *argv[], MP3File *mp3_file);

/* To open input and output files (simple check) */
Status open_files(MP3File *mp3_file);

/* To display mp3 file contents */
Status view_mp3(MP3File *mp3_file);

/* To validate and read arguments for edit mode */
Status validate_edit_args(int argc, char *argv[], MP3File *mp3_file);

/* To edit mp3 tag content */
Status edit_mp3(int argc, char *argv[], MP3File *mp3_file);

void display_help();


#endif /* MP3_H */

