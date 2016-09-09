/* 
 * File:   main.c
 * Author: Matthew
 *
 * Created on September 8, 2016, 1:24 PM
 */

#include <stdio.h>
#include <stdlib.h>
#define CHANNEL_SIZE 255

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} Pixel;

FILE* sourcefp;
FILE* outputfp;
char format[8];
int h;
int w;
int mc;
char height[8];
char width[8];
char maxcolor[8];
unsigned char* data;

void write_data_to_buffer();
void output_p3();
void output_p6();

/*
 * 
 */
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
    
    fscanf(sourcefp, "%s", format);
    if (format[0] != 'P' || (format[1] != '3' && format[1] != '6')) {
        fprintf(stderr, "Error: Invalid image format. '%s' needs to be either 'P3' or 'P6'.", argv[2]);
        return(1);
    }
    
    fscanf(sourcefp, "%s %s", height, width);
    
    h = atoi(height);
    w = atoi(width);
    
    if (h < 1 || w < 1) {
        fprintf(stderr, "Error: Invalid dimensions.");
        return(1);
    }
    
    fscanf(sourcefp, "%s", maxcolor);
    
    mc = atoi(maxcolor);
    
    if (mc != CHANNEL_SIZE) {
        fprintf(stderr, "Error: Channel size must be 8 bits.");
        return(1);
    }
    
    printf("%s\n", format);
    printf("%d %d\n", h, w);
    printf("%s", maxcolor);
    
    write_data_to_buffer(h, w);
    fclose(sourcefp);
    
    outputfp = fopen(argv[3], "w");
    if (atoi(argv[1]) == 3)
        output_p3();
    else
        output_p6();
    
    fclose(outputfp);
    
    return(0);
}

void write_data_to_buffer() {
    
    data = malloc(sizeof(Pixel)*h*w);
    
    if (format[1] == '3') {
        char temp[3];

        for(int i=0; i<(sizeof(Pixel)*h*w); i++) {
            fscanf(sourcefp, "%s", temp);
            data[i] = (char)atoi(temp);
        }
    } else {
        fscanf(sourcefp, "%s", data);
    }
    
}

void output_p3() {
    fprintf(outputfp, "P3\n");
    fprintf(outputfp, "%s %s\n", height, width);
    fprintf(outputfp, "%s\n", maxcolor);
    for(int i=0; i<(sizeof(Pixel)*h*w); i++)
        fprintf(outputfp, "%d\n", data[i]);
}

void output_p6() {
    fprintf(outputfp, "P6\n");
    fprintf(outputfp, "%s %s\n", height, width);
    fprintf(outputfp, "%s\n", maxcolor);
    fwrite(data, sizeof(Pixel), w*h, outputfp);
}