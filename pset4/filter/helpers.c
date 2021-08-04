#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // run over each pixel, left to right, up to down
    // calculate the avg of the pixel color (r+g+b)/3 
    // set the value of them to the avg value (rounding to the nearest int)
    
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            float r = image[i][j].rgbtRed;
            float g = image[i][j].rgbtGreen;
            float b = image[i][j].rgbtBlue;

            float avg = (r+g+b)/3.0;
            int rounded = round(avg);

            image[i][j].rgbtRed = rounded;
            image[i][j].rgbtGreen = rounded;
            image[i][j].rgbtBlue = rounded;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{   
    // run over each row
    // calculate if the pixel in the row is even or odd
    // skip middle pixel if odd, don't if even
    // flip the pixels in pairs using the calculated even/odd integer with a temp pixel
    int counter;

    int temp_red, temp_blue, temp_green;
    RGBTRIPLE temp;
    for (int i=0; i < height; i++){
        
        if (width % 2 == 0){
            counter =  width / 2;
        }
        else {
            counter = (width - 1)/2;
        }

        for (int j = 0; j < counter; j++){
            //swaping with temp
            temp = image[i][j];
            
            image[i][j] = image[i][width-1-j];

            image[i][width-1-j] = temp;
            
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //get the info of the pixels in a 3x3 box
    //calculate the avg of all the values of rgb, change the value of the middle pixel into the new value
    
    // temp for the rounded, blurred pixel
    RGBTRIPLE temp[height][width];
    RGBTRIPLE rounded;

    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            
            // init the sums
            float sum_red=0, sum_green=0, sum_blue=0;
            
            // for corners (divide by 4)
            if(i==0 && j ==0){
                //top left
                for (int a = i; a<i+2; a++){
                    for (int b = j; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/4.0);
                rounded.rgbtGreen = round(sum_green/4.0);
                rounded.rgbtBlue = round(sum_blue/4.0);

                temp[i][j] = rounded;
            }
            else if (i==0 && j== width-1){
                //top right
                for (int a = i; a<i+2; a++){
                    for (int b = j-1; b<j+1; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/4.0);
                rounded.rgbtGreen = round(sum_green/4.0);
                rounded.rgbtBlue = round(sum_blue/4.0);

                temp[i][j] = rounded;
            }
            else if (i==height-1 && j==0){
                //bottom left
                for (int a = i-1; a<i+1; a++){
                    for (int b = j; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/4.0);
                rounded.rgbtGreen = round(sum_green/4.0);
                rounded.rgbtBlue = round(sum_blue/4.0);

                temp[i][j] = rounded;
            }
            else if(i==height-1 && j==width-1){
                //bottom right
                for (int a = i-1; a<i+1; a++){
                    for (int b = j-1; b<j+1; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/4.0);
                rounded.rgbtGreen = round(sum_green/4.0);
                rounded.rgbtBlue = round(sum_blue/4.0);

                temp[i][j] = rounded;
            }
            // for edges (divide by 6)
            else if (i==0 && 0<j<width-1){//edge top
                for (int a = i; a<i+2; a++){
                    for (int b = j-1; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/6.0);
                rounded.rgbtGreen = round(sum_green/6.0);
                rounded.rgbtBlue = round(sum_blue/6.0);

                temp[i][j] = rounded;
            }
            else if (i==height-1 && 0<j<width-1){//edge bot
                for (int a = i-1; a<i+1; a++){
                    for (int b = j-1; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/6.0);
                rounded.rgbtGreen = round(sum_green/6.0);
                rounded.rgbtBlue = round(sum_blue/6.0);

                temp[i][j] = rounded;
            }
            else if (0<i<height-1 && j == 0){//edge left
                for (int a = i-1; a<i+2; a++){
                    for (int b = j; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/6.0);
                rounded.rgbtGreen = round(sum_green/6.0);
                rounded.rgbtBlue = round(sum_blue/6.0);

                temp[i][j] = rounded;
            }
            else if (0<i<height-1 && j == width-1){//edge right
                for (int a = i-1; a<i+2; a++){
                    for (int b = j-1; b<j+1; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/6.0);
                rounded.rgbtGreen = round(sum_green/6.0);
                rounded.rgbtBlue = round(sum_blue/6.0);

                temp[i][j] = rounded;
            }
            // for middles (divide by 9)
            else{
                for (int a = i-1; a<i+2; a++){
                    for (int b = j-1; b<j+2; b++){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                    }
                }

                rounded.rgbtRed = round(sum_red/9.0);
                rounded.rgbtGreen = round(sum_green/9.0);
                rounded.rgbtBlue = round(sum_blue/9.0);

                temp[i][j] = rounded;
            }
        }
    }

    // apply temps data into image
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    
    return;
}
