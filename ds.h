#ifndef DS_H
#define DS_H

//#include <QImage>

#include <random>
#include <time.h>
#include <vector>
#include <algorithm>

class DS {
private:
    //    QImage *image;
    std::vector<std::vector<float> > vec;
    int size;

public:
    DS(int n)
    {
        size = (1 << n) + 1;
        //        You can compile it with Qt and get high map as grayscale image
        //        image = new QImage(size, QImage::Format_Grayscale8);
        vec.clear();
        vec.resize(size, std::vector<float>(size, 0.0f));
    }

    ~DS()
    {
    }

    float getRand()
    {
        float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return r;
    }

    float getRandTo()
    {
        float r2 = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 255));
        return r2;
    }

    std::vector<std::vector<float>> getMap()
    {

        srand(time(NULL));
        vec[0][0] = vec[0][size - 1]
                  = vec[size - 1][0]
                  = vec[size - 1][size - 1]
                  = getRandTo();

        float h = 1000.0f; //the range (-h -> +h) for the average offset
        //side length is distance of a single square side
        //or distance of diagonal in diamond
        for (int sideLength = size - 1;
             //side length must be >= 2 so we always have
             //a new value (if its 1 we overwrite existing values
             //on the last iteration)
             sideLength >= 2;
             //each iteration we are looking at smaller squares
             //diamonds, and we decrease the variation of the offset
             sideLength /= 2,
                 h /= 2.0) {
            //half the length of the side of a square
            //or distance from diamond center to one corner
            //(just to make calcs below a little clearer)
            int halfSide = sideLength / 2;

            //generate the new square values
            for (int x = 0; x < size - 1; x += sideLength) {
                for (int y = 0; y < size - 1; y += sideLength) {
                    //x, y is upper left corner of square
                    //calculate average of existing corners
                    float avg = vec[x][y] +                     //top left
                        vec[x + sideLength][y] +                //top right
                        vec[x][y + sideLength] +                //lower left
                        vec[x + sideLength][y + sideLength];    //lower right
                    avg /= 4.0;

                    //center is average plus random offset
                    vec[x + halfSide][y + halfSide] =
                        //We calculate random value in range of 2h
                        //and then subtract h so the end value is
                        //in the range (-h, +h)
                        avg + (getRand() * 2 * h) - h;
                }
            }

            //generate the diamond values
            //since the diamonds are staggered we only move x
            //by half side
            //NOTE: if the data shouldn't wrap then x < size
            //to generate the far edge values
            for (int x = 0; x < size - 1; x += halfSide) {
                //and y is x offset by half a side, but moved by
                //the full side length
                //NOTE: if the data shouldn't wrap then y < size
                //to generate the far edge values
                for (int y = (x + halfSide) % sideLength; y < size - 1; y += sideLength) {
                    //x, y is center of diamond
                    //note we must use mod  and add size for subtraction
                    //so that we can wrap around the array to find the corners
                    float avg = vec[(x - halfSide + size - 1) % (size - 1)][y] +  //left of center
                        vec[(x + halfSide) % (size - 1)][y] +                              //right of center
                        vec[x][(y + halfSide) % (size - 1)] +                              //below center
                        vec[x][(y - halfSide + size - 1) % (size - 1)];           //above center
                    avg /= 4.0;

                    //new value = average plus random offset
                    //We calculate random value in range of 2h
                    //and then subtract h so the end value is
                    //in the range (-h, +h)
                    avg = avg + (getRand() * 2 * h) - h;
                    //update value for center of diamond
                    vec[x][y] = avg;

                    //wrap values on the edges, remove
                    //this and adjust loop condition above
                    //for non-wrapping values.
                    if (x == 0) {
                        vec[size - 1][y] = avg;
                    }
                    if (y == 0) {
                        vec[x][size - 1] = avg;
                    }
                }
            }
        }

        int min = 10000000, max = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                min = std::min(min, abs((int)vec[i][j]));
                max = std::max(max, abs((int)vec[i][j]));
            }
        }

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                vec[i][j] = abs((int)(vec[i][j] / max * 255));
//                int qq = qAbs((int)(vec[i][j] / max * 255));
//                image->setPixelColor(i, j, QColor(qq, qq, qq));
            }
        }
        return vec;
    }
};

#endif // DS_H
