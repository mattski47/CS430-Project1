/* 
 * File:   main.c
 * Author: Matthew
 *
 * Created on September 8, 2016, 1:24 PM
 */

#include <stdio.h>
#include <stdlib.h>
#define CHANNEL_SIZE 255;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

char format[2];
int height;
int width;
int maxcolor;
char* source = "imageP3.ppm";

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc != 4) {
        fprintf(stderr, "Error: Arguments should be in format: 'format' 'source' 'dest'.");
        return(1);
    }
    
    FILE* sourcefp = fopen(source, "r");
    
    if (!sourcefp) {
        fprintf(stderr, "Error: File not found.");
        return(1);
    }
    
    fscanf(sourcefp, "%s", format);
    if (format[0] != 'P' || (format[1] != '3' && format[1] != '6')) {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", source);
        return(1);
    }
    
    
    return(0);
}

