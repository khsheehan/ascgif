/*
 *
 * gifprocessor.c
 * ascgif 2014
 *
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "cv.h"
#include "highgui.h"

#include "magick/MagickCore.h"
#include "magick/studio.h"
#include "magick/exception.h"
#include "magick/image.h"
#include "wand/MagickWand.h"
#include "wand/convert.h"

const int MAX_URL_LENGTH = 3000;
const char* JSONP_CALLBACK = "respCallback";

int downloadRaw(char* file, char* type);
void preOps();
void postOps();
void processImage();
char* convertToASCII();
void jp2a(int width, char* file, char* destination);
void localRun(char* cmd);

char* processFile(char* request) {
    char* result = (char*) malloc(4096);
    char* output; // The final output from the processed file
    char * args;  // Points to the arguments to be parsed out

    char * ftok; // For now, we'll only accept 'gif' as a function, but this
                 // could be expanded upon in the future to accept multiple file formats
    char * atok; // This token will hold the url of the file to be processed

    char* ascii; // This will hold the single string of ascii that represents all frames
    char* jsonp; // This will hold the formatted jsonp to serve back to the browser

    if (strncmp(request, "GET", 3) == 0) {

        args = request + (strlen("GET") + 1);
        while (args[0] == '/' || args[0] == '?') {
            args = args + 1;
        }

        // Parse out the function name and the url argument
        ftok = strtok(args, "=");
        atok = strtok(NULL, "=");
        atok = strtok(atok, " ");
        atok = strtok(atok, "&");

        // Call the appropriate function and pass in the url
        if (strcmp(ftok, "gif") == 0) {
            preOps();

            if (downloadRaw(atok, "gif") != 0) { // Could not download
                sprintf(result, "%s", "HTTP/1.1 404 Bad Request\nContent-Type: application/javascript\n\n");
                return result;
            } else {
                sprintf(result, "%s", "HTTP/1.1 200 OK\nContent-Type: application/javascript\n\n");
                output = &(result[strlen("HTTP/1.1 200 OK\nContent-Type: application/javascript\n\n")]);
            }

            processImage();
            ascii = convertToASCII();
            jsonp = (char*) malloc(strlen(ascii) + strlen(JSONP_CALLBACK) + 50);
            sprintf(jsonp, "%s(\"%s\");", JSONP_CALLBACK, ascii);
            sprintf(output, "%s", jsonp);
            postOps();
        } else {
            sprintf(output, "%s", "Error, ascgif only supports gif conversion (for now).");
        }
    }
    return result;
}

int downloadRaw(char* file, char* type) {
    char* cmd = (char*) malloc(MAX_URL_LENGTH);
    sprintf(cmd, "wget \"%s\" -O \"../files/raw/raw.%s\" -P \"../files/raw/\"", file, type);
    return system(cmd);
}

void postOps() {
    system("rm -f ../files/raw/raw.*"); // Remove raw .gif file
    system("rm -f ../files/processed/*"); // Remove processed .jpg files
    system("rm -f ../files/result"); // Remove ASCII version
}

void preOps() {
    postOps(); // Call postOps in case it wasn't called due to some error
    system("touch ../files/result"); // Create a new, empty result file
}

void processImage() {
    char* gif2frames = "convert -negate ../files/raw/raw.gif frame.jpg";
    system(gif2frames);
    system("mv ./frame* ../files/processed/");
}

void jp2a(int width, char* file, char* destination) {
    char* widthFlag = "--width=";
    char* jp2a = "jp2a";
    char* cmd = malloc(1024); // Holds the call to jp2a
    sprintf(cmd, "./%s %s%d %s >> %s", jp2a, widthFlag, width, file, destination);
    localRun(cmd);
}

void localRun(char* cmd) {
    system(cmd);
}

char* convertToASCII() {
    const char* dirName = "../files/processed/";
    DIR *dir;
    struct dirent *ent;
    IplImage* s;
    CvSize roi = cvSize(3,5);
    int i = 0; // counter for frames

    if((dir = opendir(dirName)) != NULL) {
        while((ent = readdir(dir)) != NULL) {
            if(strncmp(ent->d_name, "frame-", 6) == 0) {
                printf("Found image: %s\n", ent->d_name);
                char* fileName = malloc(strlen("frame-") + 4);
                sprintf(fileName, "frame-%d.jpg", i);
                char* filePath = malloc(strlen("../files/processed/") + 20);
                sprintf(filePath, "../files/processed/%s", fileName);
                jp2a(200, filePath, "../files/result");
                system("echo thisisthedelimiterbro >> ../files/result");
                i++;
            }
        }
        closedir(dir);
    }
    else {
        fprintf(stderr, "Error: Cannot find directory.\n");
        return NULL;
    }

    char *rv;
    long rvSize;
    FILE *input = fopen("../files/result", "rb");
    fseek(input, 0, SEEK_END);
    rvSize = ftell(input);
    rewind(input);
    rv = malloc(rvSize * (sizeof(char)));
    fread(rv, sizeof(char), rvSize, input);
    fclose(input);

    char * finalReturn = malloc(sizeof(rv));

    int c = 0;

    for (i = 0; i < strlen(rv); i++) {
        if (rv[i] != '\n') {
            finalReturn[c] = rv[i];
            c++;
        }
    }

    return finalReturn;
}
