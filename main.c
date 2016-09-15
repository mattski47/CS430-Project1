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
int i;
int h;
int w;
int mc;
char c;
char height[8];
char width[8];
char maxcolor[8];
unsigned char* data;

void read_data_to_buffer();
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
    
    fgetc(sourcefp);
    c = fgetc(sourcefp);
    if (c == '#') {
        while (c == '#') {
            while (c != '\n')
                c = fgetc(sourcefp);
            c = fgetc(sourcefp);
        }
    }
    
    i = 0;
    
    while (c != ' ') {
        width[i] = c;
        c = fgetc(sourcefp);
        i++;
    }
    
    i = 0;
    c = fgetc(sourcefp);
    
    while (c != '\n') {
        height[i] = c;
        c = fgetc(sourcefp);
        i++;
    }
    
    printf("%s %s\n", width, height);
    
    w = atoi(width);
    h = atoi(height);
    
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
    
    if (format[1] == '3') {
        char temp[8];

        for(int i=0; i<(sizeof(Pixel)*h*w); i++) {
            fscanf(sourcefp, "%s", temp);
            data[i] = (char)atoi(temp);
        }
        
    } else {
        for(int i=0; i<(sizeof(Pixel)*h*w); i++)
            data[i] = fgetc(sourcefp);
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