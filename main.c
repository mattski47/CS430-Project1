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

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

FILE* sourcefp;
FILE* outputfp;
char format;
int i;
int h;
int w;
int mc;
char c;
Pixel* data;

void read_data_to_buffer();
void output_p3();
void output_p6();

int main(int argc, char* argv[]) {

    if (argc != 4) {
        fprintf(stderr, "Error: Arguments should be in format: 'format' 'source' 'dest'.");
        return(1);
    }
    
    sourcefp = fopen(argv[2], "r");
    
    if (!sourcefp) {
        fprintf(stderr, "Error: File not found.");
        return(1);
    }
    
    if (fgetc(sourcefp) != 'P') {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", argv[2]);
        return(1);
    }
    
    format = fgetc(sourcefp);
    
    if (format != '3' && format != '6') {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", argv[2]);
        return(1);
    }
    
    c = fgetc(sourcefp);
    
    while (isspace(c))
        c = fgetc(sourcefp);
    
    while (c == '#') {
        while (c != '\n')
            c = fgetc(sourcefp);
        
        while (isspace(c))
            c = fgetc(sourcefp);
    }
    
    fseek(sourcefp, -1, SEEK_CUR);
    fscanf(sourcefp, "%d", &w);
    fscanf(sourcefp, "%d", &h);
    if (h < 1 || w < 1) {
        fprintf(stderr, "Error: Invalid dimensions.");
        return(1);
    }
    
    fscanf(sourcefp, "%d", &mc);
    if (mc != CHANNEL_SIZE) {
        fprintf(stderr, "Error: Channel size must be 8 bits.");
        return(1);
    }
    
    fgetc(sourcefp);
    read_data_to_buffer();
    fclose(sourcefp);
    
    outputfp = fopen(argv[3], "w");
    if (atoi(argv[1]) == 3)
        output_p3();
    else
        output_p6();
    
    fclose(outputfp);
    
    return(0);
}

void read_data_to_buffer() {
    data = malloc(sizeof(Pixel)*h*w);
    Pixel temp;
    
    if (format == '3') {
        for(int i=0; i<(h*w); i++) {
            fscanf(sourcefp, "%d", &temp.r);
            fscanf(sourcefp, "%d", &temp.g);
            fscanf(sourcefp, "%d", &temp.b);
            data[i] = temp;
        }
        
    } else {
        for(int i=0; i<(h*w); i++) {
            temp.r = fgetc(sourcefp);
            temp.g = fgetc(sourcefp);
            temp.b = fgetc(sourcefp);
            data[i] = temp;
        }
    }
}

void output_p3() {
    fprintf(outputfp, "P3\n");
    fprintf(outputfp, "%d %d\n", h, w);
    fprintf(outputfp, "%d\n", mc);
    for(int i=0; i<(h*w); i++){
        fprintf(outputfp, "%d\n", data[i].r);
        fprintf(outputfp, "%d\n", data[i].g);
        fprintf(outputfp, "%d\n", data[i].b);
    }
}

void output_p6() {
    fprintf(outputfp, "P6\n");
    fprintf(outputfp, "%d %d\n", h, w);
    fprintf(outputfp, "%d\n", mc);
    fwrite(data, sizeof(Pixel), w*h, outputfp);
}