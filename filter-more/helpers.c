#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // average all the values then reassign them to each color for grayscale
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;
            int avg = round((float) (blue + green + red) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++) // only iterate up to midway point
        {
            // perform a swap using a buffer/temp RGBTRIPLE variable
            RGBTRIPLE temp;
            temp = image[i][width - 1 - j];
            image[i][width - 1 - j] = image[i][j];
            image[i][j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imageCOPY[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            imageCOPY[i][j] = image[i][j];
        }
    }

    // iterating over 2d array to apply box blur to every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int neighbors = 0;
            int blueSUM = 0;
            int blueAVG = 0;
            int greenSUM = 0;
            int greenAVG = 0;
            int redSUM = 0;
            int redAVG = 0;
            for (int r = i - 1; r < i + 2; r++) // now iterate over 3x3 mini 2d array, accounting for edges
            {
                if (r < 0) // if past top row
                {
                    continue;
                }
                else if (r >= height) // if past bottom row
                    break;
                for (int c = j - 1; c < j + 2; c++)
                {
                    if (c < 0) // if edge on left
                        continue;
                    else if (c >= width) // if edge on right
                        break;
                    else
                    {
                        blueSUM += image[r][c].rgbtBlue;
                        greenSUM += image[r][c].rgbtGreen;
                        redSUM += image[r][c].rgbtRed;
                        neighbors++;
                    }
                }
            }
            blueAVG = round((float) (blueSUM) / neighbors);
            greenAVG = round((float) (greenSUM) / neighbors);
            redAVG = round((float) (redSUM) / neighbors);
            imageCOPY[i][j].rgbtBlue = blueAVG;
            imageCOPY[i][j].rgbtGreen = greenAVG;
            imageCOPY[i][j].rgbtRed = redAVG;
        }
    }

    // iterate over imageCOPY to update changes made to original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imageCOPY[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imageCOPY[height][width];

    for (int i = 0; i < height; i++) // populate array copy with input image pixels
    {
        for (int j = 0; j < width; j++)
        {
            imageCOPY[i][j] = image[i][j];
        }
    }

    // iterating over 2d array to apply edges to every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blueGxGy = 0;
            int blueGx = 0;
            int blueGy = 0;
            int greenGxGy = 0;
            int greenGx = 0;
            int greenGy = 0;
            int redGxGy = 0;
            int redGx = 0;
            int redGy = 0;
            for (int r = i - 1; r < i + 2; r++) // now iterate over 3x3 mini 2d array, accounting for edges
            {
                if (r < 0) // if kernel past top row
                {
                    continue;
                }
                else if (r >= height) // if kernel past bottom row
                    break;
                for (int c = j - 1; c < j + 2; c++)
                {
                    if (c < 0) // if kernel touces edge on left
                        continue;
                    else if (c >= width) // if kernel touches edge on right
                        break;
                    else if (c == j - 1) // left column of kernel
                    {
                        if (r == i - 1)
                        {
                            blueGx += image[r][c].rgbtBlue * -1;
                            greenGx += image[r][c].rgbtGreen * -1;
                            redGx += image[r][c].rgbtRed * -1;
                            blueGy += image[r][c].rgbtBlue * -1;
                            greenGy += image[r][c].rgbtGreen * -1;
                            redGy += image[r][c].rgbtRed * -1;
                        }
                        if (r == i)
                        {
                            blueGx += image[r][c].rgbtBlue * -2;
                            greenGx += image[r][c].rgbtGreen * -2;
                            redGx += image[r][c].rgbtRed * -2;
                            blueGy += image[r][c].rgbtBlue * 0;
                            greenGy += image[r][c].rgbtGreen * 0;
                            redGy += image[r][c].rgbtRed * 0;
                        }
                        if (r == i + 1)
                        {
                            blueGx += image[r][c].rgbtBlue * -1;
                            greenGx += image[r][c].rgbtGreen * -1;
                            redGx += image[r][c].rgbtRed * -1;
                            blueGy += image[r][c].rgbtBlue * 1;
                            greenGy += image[r][c].rgbtGreen * 1;
                            redGy += image[r][c].rgbtRed * 1;
                        }
                    }
                    else if (c == j) // middle column of kernel
                    {
                        if (r == i - 1)
                        {
                            blueGx += image[r][c].rgbtBlue * 0;
                            greenGx += image[r][c].rgbtGreen * 0;
                            redGx += image[r][c].rgbtRed * 0;
                            blueGy += image[r][c].rgbtBlue * -2;
                            greenGy += image[r][c].rgbtGreen * -2;
                            redGy += image[r][c].rgbtRed * -2;
                        }
                        if (r == i)
                        {
                            blueGx += image[r][c].rgbtBlue * 0;
                            greenGx += image[r][c].rgbtGreen * 0;
                            redGx += image[r][c].rgbtRed * 0;
                            blueGy += image[r][c].rgbtBlue * 0;
                            greenGy += image[r][c].rgbtGreen * 0;
                            redGy += image[r][c].rgbtRed * 0;
                        }
                        if (r == i + 1)
                        {
                            blueGx += image[r][c].rgbtBlue * 0;
                            greenGx += image[r][c].rgbtGreen * 0;
                            redGx += image[r][c].rgbtRed * 0;
                            blueGy += image[r][c].rgbtBlue * 2;
                            greenGy += image[r][c].rgbtGreen * 2;
                            redGy += image[r][c].rgbtRed * 2;
                        }
                    }
                    else if (c == j + 1) // right column of kernel
                    {
                        if (r == i - 1)
                        {
                            blueGx += image[r][c].rgbtBlue * 1;
                            greenGx += image[r][c].rgbtGreen * 1;
                            redGx += image[r][c].rgbtRed * 1;
                            blueGy += image[r][c].rgbtBlue * -1;
                            greenGy += image[r][c].rgbtGreen * -1;
                            redGy += image[r][c].rgbtRed * -1;
                        }
                        if (r == i)
                        {
                            blueGx += image[r][c].rgbtBlue * 2;
                            greenGx += image[r][c].rgbtGreen * 2;
                            redGx += image[r][c].rgbtRed * 2;
                            blueGy += image[r][c].rgbtBlue * 0;
                            greenGy += image[r][c].rgbtGreen * 0;
                            redGy += image[r][c].rgbtRed * 0;
                        }
                        if (r == i + 1)
                        {
                            blueGx += image[r][c].rgbtBlue * 1;
                            greenGx += image[r][c].rgbtGreen * 1;
                            redGx += image[r][c].rgbtRed * 1;
                            blueGy += image[r][c].rgbtBlue * 1;
                            greenGy += image[r][c].rgbtGreen * 1;
                            redGy += image[r][c].rgbtRed * 1;
                        }
                    }
                }
            }
            blueGxGy = round(sqrt(pow(blueGx, 2) + pow(blueGy, 2)));
            greenGxGy = round(sqrt(pow(greenGx, 2) + pow(greenGy, 2)));
            redGxGy = round(sqrt(pow(redGx, 2) + pow(redGy, 2)));
            if (blueGxGy > 255)
                blueGxGy = 255;
            if (greenGxGy > 255)
                greenGxGy = 255;
            if (redGxGy > 255)
                redGxGy = 255;

            imageCOPY[i][j].rgbtBlue = blueGxGy;
            imageCOPY[i][j].rgbtGreen = greenGxGy;
            imageCOPY[i][j].rgbtRed = redGxGy;
        }
    }

    // iterate over imageCOPY to update changes made to original image array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imageCOPY[i][j];
        }
    }
    return;
}
