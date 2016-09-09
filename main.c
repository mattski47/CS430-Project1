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

/*
 * 
 */
int main(int argc, char** argv) {

    if (argc != 4) {
        fprintf(stderr, "Error: Arguments should be in format: format source dest");
        return(1);
    }
    
    FILE *sourcefp = fopen("imageP3.ppm", "r");
    
    
    
    return(0);
}

