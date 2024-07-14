#include "opencv2/opencv.hpp"
#include "string"
using namespace cv;

#define thresholdnum 100

void templates_init(std::vector<Mat>& templates);


int main()
{
    std::vector<Mat> templates;
    Mat example = imread("/home/dcy/ROSLearn/untitled1/example.jpg");
    Mat result, N;
    templates_init(templates);
    cvtColor(example(Rect(60, 460, 750, 1020)), example, COLOR_BGR2GRAY);

    Mat pic[7][10];
    int a[10][7];
    double b[10][7];
    for (int i = 0; i < 7; ++i)
    {
        for (int j = 0; j < 10; ++j)
        {
            pic[i][j] = example(Rect(i * 103, j * 103, 90, 85));
            double maxVal = -1;
            for (int k = 0; k < 7; ++k)
            {
                matchTemplate(pic[i][j], templates[k], result, 5);
                double minVal, currentMaxVal = -1;
                Point minLoc, maxLoc;
                minMaxLoc(result, &minVal, &currentMaxVal, &minLoc, &maxLoc);
                if (currentMaxVal > maxVal )
                {
                    maxVal = currentMaxVal;
                    a[j][i] = k;
                    b[j][i] = maxVal;
                }
            }
        }
    }

    for (auto & i : a)
    {
        for (int j : i)
        {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout <<"======"<< std::endl;
    for (auto & i : b)
    {
        for (double j : i)
        {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

void templates_init(std::vector<Mat>& templates)
{
    Mat temp[7];
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/0.jpg"), temp[0], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/1.jpg"), temp[1], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/2.jpg"), temp[2], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/3.jpg"), temp[3], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/4.png"), temp[4], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/5.png"), temp[5], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/ROSLearn/untitled1/templates/6.jpg"), temp[6], COLOR_BGR2GRAY);
    for (auto& i : temp)
    {
        resize(i, i, Size(90, 85));
        templates.emplace_back(i);
    }
}
