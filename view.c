#include <stdio.h>
#include <string.h>
#include "mp3.h"

/* -------- READ & VALIDATE VIEW ARGS -------- */
Status read_and_validate_view_args(int argc, char *argv[], MP3File *mp3_file)
{
    if (argc != 3)
    {
        printf("Error: Usage: ./a.out -v <filename.mp3>\n");
        return FAILURE;
    }

    strcpy(mp3_file->filename, argv[2]);

    mp3_file->mp3_ptr = fopen(mp3_file->filename, "rb");
    if (mp3_file->mp3_ptr == NULL)
    {
        printf("Error: File not found!\n");
        return FAILURE;
    }

    printf("mp3file is present\n");
    printf("Read and validate is Successful\n");

    return SUCCESS;
}

/* -------- OPEN FILES -------- */
Status open_files(MP3File *mp3_file)
{
    if (mp3_file->mp3_ptr == NULL)
    {
        printf("Error opening MP3 file\n");
        return FAILURE;
    }

    printf("Mp3 file is opened\n");
    return SUCCESS;
}

/* ----------- SYNC-SAFE SIZE CONVERSION ----------- */
unsigned int convert_syncsafe(unsigned char b[4])
{
    return ((b[0] & 0x7F) << 21) |
           ((b[1] & 0x7F) << 14) |
           ((b[2] & 0x7F) << 7)  |
           (b[3] & 0x7F);
}

/* -------- VIEW MP3 TAGS -------- */
Status view_mp3(MP3File *mp3_file)
{
    FILE *fp = mp3_file->mp3_ptr;

    char tag[5];
    unsigned char size_bytes[4];
    unsigned int size;
    char data[500];

    printf("File opened successfully\n");

    /* Skip MP3 Header (10 bytes) */
    fseek(fp, 10, SEEK_SET);

    /* We know ID3v2 has many frames. Read 6 frames only */
    for (int i = 0; i < 6; i++)
    {
        /* Read Frame ID */
        fread(tag, 1, 4, fp);
        tag[4] = '\0';

        /* Read Size Bytes */
        fread(size_bytes, 1, 4, fp);

        size = convert_syncsafe(size_bytes);

        /* Skip Flags (2 bytes) */
        fseek(fp, 2, SEEK_CUR);

        /* Read actual frame data */
        fread(data, 1, size, fp);
        data[size] = '\0';

        /* ID3 text frames start with encoding byte → skip data[0] */
        char *value = data + 1;

        /* Print as required */
        if (strcmp(tag, "TIT2") == 0)
        {
            printf("TIT2 : %s\n", value);
        }
        else if (strcmp(tag, "TPE1") == 0)
        {
            printf("TPE1 : %s\n", value);
        }
        else if (strcmp(tag, "TALB") == 0)
        {
            printf("TALB : %s\n", value);
        }
        else if (strcmp(tag, "TYER") == 0)
        {
            printf("TYER : %s\n", value);
        }
        else if (strcmp(tag, "TCON") == 0)
        {
            printf("TCON : %s\n", value);
        }
        else if (strcmp(tag, "COMM") == 0)
        {
            printf("COMM : %s\n", value);
        }
        else
        {
            /* Unknown tag – skip silently */
        }
    }

    return SUCCESS;
}

