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
        for(int j=0; j < width; j++){
            float sum_red=0, sum_green=0, sum_blue=0;
            int divider=0;
            
            for(int a = i-1; a<i+2; a++){
                for(int b = j-1; b<j+2; b++){

                    // if the pizel is out of bound, skip it
                    if ((a>=0 && a<=height-1) && (b>=0 && b<=width-1)){
                        sum_red += image[a][b].rgbtRed;
                        sum_blue += image[a][b].rgbtBlue;
                        sum_green += image[a][b].rgbtGreen;
                        divider++; // count the pixels that need to be divided to
                    }
                }
            }
            
            rounded.rgbtRed = round(sum_red/divider);
            rounded.rgbtGreen = round(sum_green/divider);
            rounded.rgbtBlue = round(sum_blue/divider);

            temp[i][j] = rounded;
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
    // compute Gx and Gy of the pixel
    // ignore non-existence pixels (corners and egdes cases)
    // the new value = root( Gx^2 + Gy^2 )
    // squaring makes a positive value
    // if value > 255, set it to 255

    //intit Gx and Gy array
    int Gx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
    int Gy[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
    
    // init total sobel value of Gx and Gy
    typedef struct 
    {
        float red;
        float green;
        float blue;
    }
    sobel;
    
    sobel sobel_x, sobel_y;

    RGBTRIPLE holder[height][width];
    int temp;

    int count_i, count_j;
    int x,y;

    //loop over all pixels
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){

            //reset sobel values to 0
            sobel_x.red = sobel_x.green = sobel_x.blue = 0;
            sobel_y.red = sobel_y.green = sobel_y.blue = 0;

            count_i = count_j = 0;
            // 3x3 pixel grid calulation
            for(int a = i-1; a<i+2; a++){
                for(int b = j-1; b<j+2; b++){
                    
                    x = Gx[count_i][count_j];
                    y = Gy[count_i][count_j];
                    
                    //** TODO: fix Gx and Gy implementation
                    //   DONE
                    //? Please check
                    if ((a>=0 && a<=height-1) && (b>=0 && b<=width-1)){
                        sobel_x.red += image[a][b].rgbtRed * x;
                        sobel_x.green += image[a][b].rgbtGreen * x;
                        sobel_x.blue += image[a][b].rgbtBlue * x;

                        sobel_y.red += image[a][b].rgbtRed * y;
                        sobel_y.green += image[a][b].rgbtGreen * y;
                        sobel_y.blue += image[a][b].rgbtBlue * y;
                    }
                    count_j++;
                }
                count_i++;
                count_j = 0;
            }
            
            // TODO: fix holder

            // holder for red channel
           temp = round(sqrt((pow(sobel_x.red,2)) + (pow(sobel_y.red,2))));

            if(temp > 255){
                holder[i][j].rgbtRed = 255;
            }
            else{
                holder[i][j].rgbtRed = temp;
            }
            
            //holder for green channel
            temp = round(sqrt(pow(sobel_x.blue,2) + pow(sobel_y.blue,2)));

            if(temp > 255){
                holder[i][j].rgbtBlue = 255;
            }
            else{
                holder[i][j].rgbtBlue = temp;
            }

            //holder for blue channel
            temp = round(sqrt(pow(sobel_x.green,2) + pow(sobel_y.green,2)));

            if(temp > 255){
                holder[i][j].rgbtGreen = 255;
            }
            else{
                holder[i][j].rgbtGreen = temp;
            }
        }
    }

    // apply holders data into image
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++){
            image[i][j] = holder[i][j];
        }
    }

    return;
}