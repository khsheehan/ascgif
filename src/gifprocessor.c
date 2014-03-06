/*
 *
 * gifprocessor.c
 * ascgif 2014
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX_URL_LENGTH = 3000;

char* download_raw(char* file, char* type);
void remove_raw();

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
        atok = strtok(NULL, "=");
        atok = strtok(atok, " ");

        // Call the appropriate function and pass in the url
        if (strcmp(ftok, "gif") == 0) {
            sprintf(output, "%s", download_raw(atok, "gif"));
            // TODO: Add the rest of the image processing logic here
            //       There will be a file "raw.gif" in ../files/raw/
            remove_raw();
        } else {
            // TODO: Sent a more useful error message
            sprintf(output, "%s", "Error. Bad Url.");
        }
    }
    return result;
}

char* download_raw(char* file, char* type) {
    // TODO: Possibly validate url / safety check it
    char* cmd = (char*) malloc(MAX_URL_LENGTH);
    sprintf(cmd, "wget \"%s\" -O \"../files/raw/raw.%s\" -P \"../files/raw/\"", file, type);
    system(cmd);
    return "Downloaded File.";
}

void remove_raw() {
    system("rm -f ../files/raw/raw.*");
}

