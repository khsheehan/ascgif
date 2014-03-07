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
const char* JSONP_CALLBACK = "respCallback";

int downloadRaw(char* file, char* type);
void removeRaw();
void processImage();
char* convertToASCII();

char* processFile(char* request) {
    char* result = (char*) malloc(4096);
    char* output; // The final output from the processed file
    char * args;  // Points to the arguments to be parsed out

    char * ftok; // For now, we'll only accept 'gif' as a function, but this
                 // could be expanded upon in the future to accept multiple file formats
    char * atok; // This token will hold the url of the file to be processed

    char* ascii; // This will hold the single string of ascii that represents all frames
    char* jsonp; // This will hold the formatted jsonp to serve back to the browser

    // Set response header status and information
    sprintf(result, "%s", "HTTP/1.1 200 OK\nContent-Type: application/json\n\n");
    output = &(result[strlen("HTTP/1.1 200 OK\nContent-Type: application/json\n\n")]);

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
            // TODO @ Kyle: Uncomment this when everything is built
                // downloadRaw(atok, "gif");
            // TODO @ Toni: build this function
                processImage();
            // TODO @ Cory: build this function
                ascii = convertToASCII();
            jsonp = (char*) malloc(strlen(ascii) + strlen(JSONP_CALLBACK) + 50);
            sprintf(jsonp, "%s(\"%s\");", JSONP_CALLBACK, ascii);
            sprintf(output, "%s", jsonp);
            // TODO @ Kyle: Uncomment this when everything is built
                // removeRaw();
        } else {
            // TODO @ Kyle: Sent a more useful error message
            sprintf(output, "%s", "Error. Bad Url.");
        }
    }
    return result;
}

int downloadRaw(char* file, char* type) {
    char* cmd = (char*) malloc(MAX_URL_LENGTH);
    sprintf(cmd, "wget \"%s\" -O \"../files/raw/raw.%s\" -P \"../files/raw/\"", file, type);
    system(cmd);
    // TODO @ Kyle: Check that the file download was successful
    return 0;
}

void removeRaw() {
    system("rm -f ../files/raw/raw.*");
}

void processImage() {
    // TODO @ Toni

    // 1. There will be a .gif file in ../files/raw/raw.gif
    // 2. Take that image and resize it so that the width is 600px (ignore the height)
    // 3. Then convert the image into grayscale
    // 4. Split up the .gif into individual frames (this will be the trickiest part)
    // 5. Save each of those frames in ../files/processed/
    // 6. Each frame should be in sequence and be saved as frame.1, frame.2, frame.3...
    //    a. Note that it's possible the way I described doing this above will not work for a .gif
    //    b. It may be easier to split the image up first, then resize each, conver to grayscale, etc.

    // By the time this function has finished executing, there should be a set of files...
    // 1. All with uniform dimensions (a width of 600px)
    // 2. All grayscale
    // 3. All named (in order) as frame.1, frame.2, etc.
    // 4. Saved into ../files/processed/

    // Final Notes...
    // 1. It's possible we'll need to get frame-rate metadata from the gif. Don't worry about this for now
    //    but keep it in mind
    // 2. For every new gif that is requested, all existing data will be blown away, so don't worry
    //    about existing and overwrites

}

char* convertToASCII() {
    // TODO @ Cory

    // 1. There will be a set of equally sized (all 600px wide) grayscale images in ../files/processed/
    // 2. These files will be named frame.1, frame.2, frame.3, etc. (in the order they appear in the .gif)
    // 3. Convert each image file into an ASCII representation
    // 4. I would suggest creating a temporary file (ascii.tmp?), and appending the new ASCII for each image
    // 5. Each 'frame' will be a string, with 'frames' being separated by a predefined string (I chose "|**$**$**|")
    // 6. Once there is a file with all of the frames, return that entire file as a string

    // Keep the following in mind...
    // 1. Each image will be exactly 600px wide
    // 2. The font that will be used in courier, then monospace if that isn't found (it'll be courier)
    // 3. The font size is 5px;
    // 4. The font size, coupled with the width of the images / containing box, allows for X characters across
    // 5. Don't worry about line breaks (in fact, we have to remove them), just send a single stream of characters
    //    to represent each frame (the css will take care of breaking)

    // Final Notes...
    // 1. I would suggest starting really small with this. Drop in a grayscale image that conforms to our size
    //    constraints, name it frame.1 and convert that single frame to ASCII (and return the file string here)

    // Cut this when you're ready, it's just a demo return for some of my testing. The animation will look like so...
    // [frame 1] 0
    // [frame 2] 00
    // [frame 3] 000
    // [frame 4] 0000
    // [frame 5] 00000
    // [frame 6] 000000
    // Then it will loop back
    return "0|**$**$**|00|**$**$**|000|**$**$**|0000|**$**$**|00000|**$**$**|000000";
}

