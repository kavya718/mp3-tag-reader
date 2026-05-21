/* 
NAME :KAVYA P

DISCRIPTION : This project is designed to read and edit ID3 tags of MP3 audio files.
ID3 tags store information like Title, Artist, Album, Year, Genre, and Comments inside an MP3 file.
The program supports two main operations:
View Mode (-v) – Reads and displays all existing MP3 tags.
Edit Mode (-e) – Allows the user to update specific tags such as Title, Artist, Album, Year, Genre, and Comments.
It validates file extensions, ensures correct arguments, and safely reads/writes data using C file-handling techniques.

DATE :- 24/11/2025

SAMPLE OUTPUT:-
    Selected operation : VIEW MODE
    mp3file is present
    Read and validate is Successful
    Mp3 file is opened
    File opened successfully
    TIT2 : kavyaa_song
    TPE1 : Yo Yo Honey Singh - [SongsPk.CC]
    TALB : Yaariyan
    TYER : 2016
    TCON : Bollywood Music - [SongsPk.CC]
    COMM : eng

    -----MP3 Tag viewing completed successfully-----
SAMPLE OUTPUT:-
    Selected Operation: EDIT MODE
    Edited TIT2 : kavya_song
    ----- Tag Edited Successfully -----
    File opened successfully
    TIT2 : kavya_song
    TPE1 : Yo Yo Honey Singh - [SongsPk.CC]
    TALB : Yaariyan
    TYER : 2016
    TCON : Bollywood Music - [SongsPk.CC]
    COMM : eng
*/
#include <stdio.h>
#include <string.h>
#include "mp3.h"

// Function to identify whether user selected view or edit mode
OperationType identify_operation(int argc, char *argv[])
{
    if (argc < 2) return e_unsupported; // No option given
    if (strcmp(argv[1], "-v") == 0) return VIEW_MODE; // View mode
    if (strcmp(argv[1], "-e") == 0) return EDIT_MODE; // Edit mode
    return e_unsupported; // Invalid option
}

int main(int argc, char *argv[])
{
    /* ----------- HELP OPTION ----------- */
    if (argc == 2 && strcmp(argv[1], "-h") == 0) // If user types -h
    {
        display_help(); // Show help menu
        return 0;
    }

    MP3File mp3_file = {0}; // Structure to store MP3 details
    Status status;
    OperationType op = identify_operation(argc, argv); // Check selected operation

    /* ----------- FILE EXTENSION VALIDATION ----------- */
    if (argc >= 3)  // filename present
    {
        char *fname = argv[argc - 1];   // last argument is filename
        int len = strlen(fname); // Length of filename

        if (len < 4 || strcmp(fname + len - 4, ".mp3") != 0)
        {
            printf("Error: Only .mp3 files are allowed.\n");
            return 1;
        }
    }

    /* ----------- VIEW MODE ----------- */
    if (op == VIEW_MODE)
    {
        printf("Selected operation : VIEW MODE\n");

        // Validate view arguments
        status = read_and_validate_view_args(argc, argv, &mp3_file);
        if (status == FAILURE) {
            return 1;
        }

        // Open MP3 file
        status = open_files(&mp3_file);
        if (status == FAILURE) return 1;

        // Read and display tags
        status = view_mp3(&mp3_file);
        if (status == FAILURE) {
            return 1;
        }

        printf("\n-----MP3 Tag viewing completed successfully-----\n");
    }

    /* ----------- EDIT MODE ----------- */
    else if (op == EDIT_MODE)
    {
        printf("Selected Operation: EDIT MODE\n");

        // Check if edit arguments are correct
        status = validate_edit_args(argc, argv, &mp3_file);
        if (status == FAILURE) return 1;

        // Perform edit operation
        status = edit_mp3(argc, argv, &mp3_file);
        if (status == FAILURE) return 1;
    }

    /* ----------- INVALID OPTION ----------- */
    else
    {
        printf("Invalid option or missing arguments.\n");
        printf("Usage:\n  View: %s -v <filename.mp3>\n", argv[0]);
        printf("  Edit: %s -e -<t|a|A|m|y|c> <new_value> <filename.mp3>\n", argv[0]);
        return 1;
    }

    return 0; // Program finished successfully
}



