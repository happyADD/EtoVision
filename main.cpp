#include "opencv2/opencv.hpp"
#include "string"
using namespace cv;
#define HUMAN
void templates_init(std::vector<Mat>& templates);
void print_chessboard();
bool is_Can_Cancelled(int x1, int y1, int x2, int y2);

int chessboard[12][9];
int a[10][7];
double b[10][7];

int main()
{
    std::vector<Mat> templates;
    Mat example = imread("/home/dcy/CLionProjects/EtoVision/example.jpg");
    Mat result, N;
    templates_init(templates);
    cvtColor(example(Rect(60, 460, 750, 1020)), example, COLOR_BGR2GRAY);

    Mat pic[7][10];

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
                if (currentMaxVal > maxVal)
                {
                    maxVal = currentMaxVal;
                    a[j][i] = k;
                    b[j][i] = maxVal; //可存于map中
                }
            }
        }
    }
    for (auto& i : chessboard)
        for (int& j : i)
            j = -1;
    for (int i = 1; i < 11; ++i)
        for (int j = 1; j < 8; ++j)
            chessboard[i][j] = a[i - 1][j - 1];
    print_chessboard();
#ifdef HUMAN
    std::cout << "Recognize Finished! Now is in User Mode." << std::endl;
    bool isfinish = false;
    int cnt = 0;
    while (!isfinish)
    {
        std::cout << "Please enter two points likes this: x1 y1 x2 y2" << std::endl;
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        if (is_Can_Cancelled(x1, y1, x2, y2))
        {
            std::cout << "Can be cancelled!" << std::endl;
            chessboard[y1 + 1][x1 + 1] = -1;
            chessboard[y2 + 1][x2 + 1] = -1;
            print_chessboard();
            cnt += 2;
        }
        else
        {
            std::cout << "Can not be cancelled!" << std::endl;
            print_chessboard();
        }
        if (cnt == 70) isfinish = true;
    }

#else
    std::cout << "Recognize Finished! Now is in Algorithm Mode." << std::endl;
    int x=1,y=1,cnt=0;
    while(x<8 && y<11)
    {
        for (int i = 1; i < 11; ++i)
            for(int j = 1; j < 8; ++j)
            {
                if(chessboard[i][j] == chessboard[y][x] && is_Can_Cancelled(x,y,j,i))
                {
                    chessboard[y][x] = -1;
                    chessboard[i][j] = -1;
                    x++;
                    if(x==8)
                    {
                        x=1;
                        y++;
                    }
                    cnt++;
                    std::cout << "Algorithm cancel " << x - 1 << " " << y - 1 << " " << j - 1 << " " << i - 1 << std::endl;
                }
            }
        if(cnt==70)
        {
            std::cout << "Algorithm Cancelled Finished!" << std::endl;
            break;
        }
    }
#endif


    return 0;
}

void templates_init(std::vector<Mat>& templates)
{
    Mat temp[7];
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/0.jpg"), temp[0], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/1.jpg"), temp[1], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/2.jpg"), temp[2], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/3.jpg"), temp[3], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/4.png"), temp[4], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/5.png"), temp[5], COLOR_BGR2GRAY);
    cvtColor(imread("/home/dcy/CLionProjects/EtoVision/templates/6.jpg"), temp[6], COLOR_BGR2GRAY);
    for (auto& i : temp)
    {
        resize(i, i, Size(90, 85));
        templates.emplace_back(i);
    }
}

void print_acccuracy()
{
    for (auto& i : a)
    {
        for (int j : i)
        {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "======" << std::endl;
    for (auto& i : b)
    {
        for (double j : i)
        {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

void print_chessboard()
{
    std::cout << "+————>x" << std::endl;
    std::cout << "|  " << std::endl;
    std::cout << "v  " << std::endl;
    std::cout << "y  " << std::endl;
    std::cout << "colomn:\t";
    for (int i = 0; i < 7; ++i)
    {
        std::cout << i << ":\t";
    }
    std::cout << std::endl;
    for (int i = 1; i < 11; ++i)
    {
        std::cout << "row " << i - 1 << ":\t";
        for (int j = 1; j < 8; ++j)
        {
            std::cout << chessboard[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

bool is_Can_Cancelled(int x1, int y1, int x2, int y2)
{
    x1 = x1 + 1;
    y1 = y1 + 1;
    x2 = x2 + 1;
    y2 = y2 + 1;
    bool flag = false;

    int temp11[9] = {0}, temp12[9] = {0};
    for (int i = x1 - 1; i >= 0; i--) //向左搜索
    {
        if (chessboard[y1][i] != -1) break;
        temp11[i] = 1; //1为可行区域，0为有阻挡
    }
    for (int i = x1 + 1; i < 9; i++) //向右搜索
    {
        if (chessboard[y1][i] != -1) break;
        temp11[i] = 1;
    } //以上对p1判断
    for (int i = x2 - 1; i >= 0; i--) //向左搜索
    {
        if (chessboard[y2][i] != -1) break;
        temp12[i] = 1;
    }
    for (int i = x2 + 1; i < 9; i++) //向右搜索
    {
        if (chessboard[y2][i] != -1) break;
        temp12[i] = 1;
    }
    temp11[x1] = 1;
    temp12[x2] = 1;
    for (int i = 0; i < 9; i++) //判断是否可以连接
        if (temp11[i] && temp12[i])
        {
            if (y1 < y2)
            {
                for (int j = y1 + 1; j < y2; j++)
                    if (chessboard[j][i] != -1) temp11[i] = temp12[i] = 0;
            }
            else
            {
                for (int j = y2 + 1; j < y1; j++)
                    if (chessboard[j][i] != -1) temp11[i] = temp12[i] = 0;
            }
            if (temp11[i] && temp12[i]) flag = true;
        }


    int temp21[12] = {0}, temp22[12] = {0};
    for (int i = y1 - 1; i >= 0; i--) //向上搜索
    {
        if (chessboard[i][x1] != -1) break;
        temp21[i] = 1;
    }
    for (int i = y1 + 1; i < 12; i++) //向下搜索
    {
        if (chessboard[i][x1] != -1) break;
        temp21[i] = 1;
    } //p1
    for (int i = y2 - 1; i >= 0; i--) //up
    {
        if (chessboard[i][x2] != -1) break;
        temp22[i] = 1;
    }
    for (int i = y2 + 1; i < 12; i++) //down
    {
        if (chessboard[i][x2] != -1) break;
        temp22[i] = 1;
    }
    temp21[y1] = 1;
    temp22[y2] = 1;
    for (int i = 0; i < 12; i++)
        if (temp21[i] && temp22[i])
        {
            if (x1 < x2)
            {
                for (int j = x1 + 1; j < x2; ++j)
                    if (chessboard[i][j] != -1) temp21[i] = temp22[i] = 0;
            }
            else
            {
                for (int j = x2 + 1; j < x1; ++j)
                    if (chessboard[i][j] != -1) temp21[i] = temp22[i] = 0;
            }
            if (temp21[i] && temp22[i]) flag = true;
        }
    return flag;
}


