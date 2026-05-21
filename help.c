#include <stdio.h>

void display_help() // Function to display all available edit options
{
    printf("Available Edit Options:\n"); // Print the heading
    printf("     -t   Edit Title (TIT2)\n");
    printf("     -a   Edit Artist (TPE1)\n");
    printf("     -A   Edit Album (TALB)\n");
    printf("     -y   Edit Year  (TYER)\n");
    printf("     -c   Edit Genre (TCON)\n");
    printf("     -m   Edit Comment (COMM)\n\n");
}
