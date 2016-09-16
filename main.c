/* 
 * File:   main.c
 * Author: Matthew
 *
 * Created on September 8, 2016, 1:24 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define CHANNEL_SIZE 255

// struct that stores color data
typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

FILE* sourcefp;
FILE* outputfp;
char format;
int h;
int w;
int mc;
char c;
Pixel* data;

void read_data_to_buffer();
void output_p3();
void output_p6();

int main(int argc, char* argv[]) {
    // check for correct number of inputs
    if (argc != 4) {
        fprintf(stderr, "Error: Arguments should be in format: 'format' 'source' 'dest'.");
        return(1);
    }
    
    // check output format
    if (*argv[1] != '3' && *argv[1] != '6') {
        fprintf(stderr, "Error: Invalid output format. ('3' or '6')");
        return(1);
    }
    
    // open source file
    sourcefp = fopen(argv[2], "r");
    
    // check that source exists
    if (!sourcefp) {
        fprintf(stderr, "Error: File not found.");
        return(1);
    }
    
    // checks that source is either P3 or P6
    if (fgetc(sourcefp) != 'P') {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", argv[2]);
        return(1);
    }
    // saves input format
    format = fgetc(sourcefp);
    if (format != '3' && format != '6') {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", argv[2]);
        return(1);
    }
    
    // initialize c and move through whitespace
    c = fgetc(sourcefp);
    while (isspace(c))
        c = fgetc(sourcefp);
    
    // check for comments
    while (c == '#') {
        while (c != '\n')
            c = fgetc(sourcefp);
        
        while (isspace(c))
            c = fgetc(sourcefp);
    }
    
    // move one char back
    fseek(sourcefp, -1, SEEK_CUR);
    
    // get width and height
    fscanf(sourcefp, "%d", &w);
    fscanf(sourcefp, "%d", &h);
    // check width and height
    if (h < 1 || w < 1) {
        fprintf(stderr, "Error: Invalid dimensions.");
        return(1);
    }
    
    // get channel size
    fscanf(sourcefp, "%d", &mc);
    // check channel size
    if (mc != CHANNEL_SIZE) {
        fprintf(stderr, "Error: Channel size must be 8 bits.");
        return(1);
    }
    
    // move one more char over before data is read
    fgetc(sourcefp);
    read_data_to_buffer();
    // close source
    fclose(sourcefp);
    
    // open destination
    outputfp = fopen(argv[3], "w");
    // choose function based on input 
    if (*argv[1] == '3')
        output_p3();
    else
        output_p6();
    
    // close output file
    fclose(outputfp);
    
    return(0);
}

// reads data from input file into buffer
void read_data_to_buffer() {
    data = malloc(sizeof(Pixel)*h*w);
    Pixel temp;
    
    // if input is P3
    if (format == '3') {
        for(int i=0; i<(h*w); i++) {
            // reads chars into Pixel temp's rgb spots
            fscanf(sourcefp, "%d", &temp.r);
            fscanf(sourcefp, "%d", &temp.g);
            fscanf(sourcefp, "%d", &temp.b);
            // puts Pixel into data buffer
            data[i] = temp;
        }
        
    // if input is P6
    } else {
        for(int i=0; i<(h*w); i++) {
            // reads chars into Pixel temp's rgb spots
            temp.r = fgetc(sourcefp);
            temp.g = fgetc(sourcefp);
            temp.b = fgetc(sourcefp);
            // puts Pixel into data buffer
            data[i] = temp;
        }
    }
}

// converts data in buffer to P3 format then outputs to file
void output_p3() {
    // create header
    fprintf(outputfp, "P3\n");
    fprintf(outputfp, "%d %d\n", h, w);
    fprintf(outputfp, "%d\n", mc);
    // converts chars from Pixel into ints before writing to output file
    for(int i=0; i<(h*w); i++){
        fprintf(outputfp, "%d\n", data[i].r);
        fprintf(outputfp, "%d\n", data[i].g);
        fprintf(outputfp, "%d\n", data[i].b);
    }
}

// outputs data in buffer to output file
void output_p6() {
    // create header
    fprintf(outputfp, "P6\n");
    fprintf(outputfp, "%d %d\n", h, w);
    fprintf(outputfp, "%d\n", mc);
    // writes buffer to output Pixel by Pixel
    fwrite(data, sizeof(Pixel), w*h, outputfp);
}
