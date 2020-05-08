#include "humoments.h"

myHuMoments::myHuMoments(unsigned char* binaryImage, int width, int height)
{
    this->width = width;
    this->height = height;
    object = new unsigned char[width * height];
    this->object = binaryImage;
}

myHuMoments::~myHuMoments()
{
    delete[] object;
}

double myHuMoments::calcMoment(double p, double q)
{
    double sum = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            sum += object[x + y * width] * pow(x, p) * pow(y, q);
        }
    }
    return sum;
}

void myHuMoments::calcOrgins()
{
    orginOfX = calcMoment(1, 0) / calcMoment(0, 0);
    orginOfY = calcMoment(0, 1) / calcMoment(0, 0);
}

double myHuMoments::calcOrginMoment(double p, double q)
{
    double sum = 0;
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            sum += object[x + y * width] * pow((x - orginOfX), p) * pow((y - orginOfY), q);
        }
    }
    return sum;
}

double myHuMoments::calcNormalizedMoment(double p, double q)
{
    double temp = (p + q) / 2 + 1;
    double a = calcOrginMoment(p, q) / pow(calcOrginMoment(0, 0), temp);
    return a;
}

void myHuMoments::calcInvariantMoments()
{
    huMoments[0] = calcNormalizedMoment(2, 0) 
                        + calcNormalizedMoment(0, 2);

    huMoments[1] = pow( (calcNormalizedMoment(2, 0) - calcNormalizedMoment(0, 2) ), 2) 
                        + 4 * ( pow(calcNormalizedMoment(1, 1), 2) );

    huMoments[2] = pow( ( calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2) ) , 2)
                        + pow( ( 3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3) ) , 2);
    
    huMoments[3] = pow( (calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2) ), 2) 
                        + pow( (calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3) ), 2);
    
    huMoments[4] = ( calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2) ) 
                        * ( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2) ) 
                        * ( pow(calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) 
                        - 3 * pow(calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2) ) 
                        + ( 3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3) ) 
                        * ( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3) )
                        * ( 3 * pow( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2)
                        - pow( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2) );
    
    huMoments[5] = ( calcNormalizedMoment(2, 0) - calcNormalizedMoment(0, 2) ) 
                        * ( pow( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) 
                        - pow( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2) )
                        + ( 4 * calcNormalizedMoment(1, 1) 
                        * ( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2) ) 
                        * ( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3) ) );
    
    huMoments[6] = ( 3 * calcNormalizedMoment(2, 1) - calcNormalizedMoment(0, 3) ) 
                        * ( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2) ) 
                        * ( pow( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2)
                        - 3 * pow( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2) ) 
                        - ( calcNormalizedMoment(3, 0) - 3 * calcNormalizedMoment(1, 2) ) 
                        * ( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3) ) 
                        * ( 3 * pow( calcNormalizedMoment(3, 0) + calcNormalizedMoment(1, 2), 2) 
                        - pow( calcNormalizedMoment(2, 1) + calcNormalizedMoment(0, 3), 2) );
}

double* myHuMoments::getHuMoments()
{
    this->calcOrgins();
    this->calcInvariantMoments();
    return huMoments;
}

void myHuMoments::logTransform()
{
    for (int i = 0; i < 7; i++)
    {
        huMoments[i] = -1 * copysign(1.0, huMoments[i]) * log10(fabs(huMoments[i]));
    }
}

