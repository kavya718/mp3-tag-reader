#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

/* ---------- Validate edit arguments ---------- */
Status validate_edit_args(int argc, char *argv[], MP3File *mp3_file)
{
    if (argc != 5)  // ./a.out -e -y 2025 sample.mp3
    {
        printf("Usage: ./a.out -e -<t|a|A|y|c|m> <value> <filename.mp3>\n");
        return FAILURE;
    }

    /* copying filename */
    strcpy(mp3_file->filename, argv[4]);

    /* opening original mp3 file */
    mp3_file->mp3_ptr = fopen(mp3_file->filename, "rb");
    if (mp3_file->mp3_ptr == NULL)
    {
        printf("Error: Unable to open mp3 file\n");
        return FAILURE;
    }

    return SUCCESS;
}

/* ---------- Convert size little endian ↔ big endian ---------- */
unsigned int convert_to_big_endian(unsigned int size)
{
    return (((size >> 24) & 0x000000FF) |
            ((size >> 8)  & 0x0000FF00) |
            ((size << 8)  & 0x00FF0000) |
            ((size << 24) & 0xFF000000));
}

/* ---------- Copy remaining MP3 data ---------- */
void copy_remaining_data(FILE *src, FILE *dest)
{
    char ch;
    while (fread(&ch, 1, 1, src) == 1)
    {
        fwrite(&ch, 1, 1, dest);
    }
}

/* ---------- Edit MP3 tag ---------- */
Status edit_mp3(int argc, char *argv[], MP3File *mp3_reader)
{
    char *opt = argv[2];   // -t / -a / -A / -y / -c / -m
    char *new_value = argv[3];

    /* find which tag must be edited */
    char edit_tag[5];

    if (strcmp(opt, "-t") == 0) strcpy(edit_tag, "TIT2");
    else if (strcmp(opt, "-a") == 0) strcpy(edit_tag, "TPE1");
    else if (strcmp(opt, "-A") == 0) strcpy(edit_tag, "TALB");
    else if (strcmp(opt, "-y") == 0) strcpy(edit_tag, "TYER");
    else if (strcmp(opt, "-c") == 0) strcpy(edit_tag, "TCON");
    else if (strcmp(opt, "-m") == 0) strcpy(edit_tag, "COMM");
    else
    {
        printf("Error: Unknown option %s\n", opt);
        return FAILURE;
    }

    /* duplicate file creation */
    FILE *duplicate;
    duplicate = fopen("duplicate.mp3", "wb");
    mp3_reader->duplicate = duplicate;

    /* rewind original */
    rewind(mp3_reader->mp3_ptr);

    /* copy 10 bytes header */
    char mp3header[10];
    fread(mp3header, 1, 10, mp3_reader->mp3_ptr);
    fwrite(mp3header, 1, 10, mp3_reader->duplicate);

    /* processing 6 tags */
    for (int i = 0; i < 6; i++)
    {
        char tag[5];
        fread(tag, 1, 4, mp3_reader->mp3_ptr);
        tag[4] = '\0';

        /* read old size */
        unsigned int size_old = 0;
        fread(&size_old, 1, 4, mp3_reader->mp3_ptr);

        /* convert big endian to little */
        size_old = ((size_old >> 24) & 0x000000FF) |
                   ((size_old >> 8)  & 0x0000FF00) |
                   ((size_old << 8)  & 0x00FF0000) |
                   ((size_old << 24) & 0xFF000000);

        /* read flags */
        char flag[2];
        fread(flag, 1, 2, mp3_reader->mp3_ptr);

        /* if tag matches edit tag */
        if (strcmp(tag, edit_tag) == 0)
        {
            /* writing tag */
            fwrite(tag, 1, 4, mp3_reader->duplicate);

            /* calculate new size */
            unsigned int size_new = strlen(new_value) + 1;  // +1 for null/encoding

            /* convert little endian → big endian */
            unsigned int size_convert = convert_to_big_endian(size_new);

            /* write new size */
            fwrite(&size_convert, 1, 4, mp3_reader->duplicate);

            /* write flag */
            fwrite(flag, 1, 2, mp3_reader->duplicate);

            /* write encoding byte */
            unsigned char encoding = 0x00;
            fwrite(&encoding, 1, 1, mp3_reader->duplicate);

            /* write new data */
            fwrite(new_value, 1, strlen(new_value), mp3_reader->duplicate);

            printf("Edited %s : %s\n", tag, new_value);

            /* skip old data in original */
            fseek(mp3_reader->mp3_ptr, size_old, SEEK_CUR);
        }
        else
        {
            /* write original tag */
            fwrite(tag, 1, 4, mp3_reader->duplicate);

            /* convert old size back to big endian */
            unsigned int size_big = convert_to_big_endian(size_old);

            /* write old size */
            fwrite(&size_big, 1, 4, mp3_reader->duplicate);

            /* write flag */
            fwrite(flag, 1, 2, mp3_reader->duplicate);

            /* allocate memory for old data */
            char *old_data = malloc(size_old);

            /* read old data */
            fread(old_data, 1, size_old, mp3_reader->mp3_ptr);

            /* write old data */
            fwrite(old_data, 1, size_old, mp3_reader->duplicate);

            free(old_data);
        }
    }

    /* copy remaining data */
    copy_remaining_data(mp3_reader->mp3_ptr, mp3_reader->duplicate);

    /* close all files */
    fclose(mp3_reader->mp3_ptr);
    fclose(mp3_reader->duplicate);

   
   remove(mp3_reader->filename);                      // delete old sample.mp3
   rename("duplicate.mp3", mp3_reader->filename);     // rename duplicate to sample.mp3



    printf("----- Tag Edited Successfully -----\n");

    /* reopen file and display view */
    mp3_reader->mp3_ptr = fopen(mp3_reader->filename, "rb");
    view_mp3(mp3_reader);

    return SUCCESS;
}
