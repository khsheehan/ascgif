/*
 *
 * gifprocessor.c
 * ascgif 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* process_file(char* request) {
    char* result = (char*) malloc(4096);
    char* output; // The final output from the processed file
    char * args;  // Points to the arguments to be parsed out

    char * ftok; // For now, we'll only accept 'gif' as a function, but this
                 // could be expanded upon in the future to accept multiple file formats
    char * atok; // This token will hold the url of the file to be processed

    // Set response header status and information
    sprintf(result, "%s", "HTTP/1.1 200 OK\n\n");
    output = &(result[strlen("HTTP/1.1 200 OK\n\n")]);

    if (strncmp(request, "GET", 3) == 0) {

        args = request + (strlen("GET") + 1);
        while (args[0] == '/' || args[0] == '?') {
            args = args + 1;
        }

        // Parse out the function name and the url argument
        ftok = strtok(args, "=");
        ftok = strtok(NULL, "=");
        atok = strtok(NULL, "&");
        ftok = strtok(ftok, "&");
        atok = strtok(atok, " ");

        // Call the appropriate function and pass in the url
        if (strcmp(ftok, "gif") == 0) {
            sprintf(output, "%s", "Connection Established");
        } else {
            sprintf(output, "%s", "Faulty Function Name");
        }
    }
    return result;
}
