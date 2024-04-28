#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

using namespace std;

#define WIDTH 720
#define HEIGHT 720

inline int64_t toInt64(double x)
{
    int64_t a;
    memcpy(&a, &x, 8);
    uint64_t mask = (uint64_t)(a >> 63) >> 1;
    return a ^ mask;
}

inline bool isSmaller(double x1, double x2)
{
    return toInt64(x1) <= toInt64(x2);
}

int getMyArr(string inputString, short count = 0, bool isFileOpen = false)
{
    try
    {
        bool minusFlag = false;
        for (int j = 0; j < inputString.size(); j++)
        {
            if (inputString[0] == '-' && !minusFlag)
            {
                minusFlag = true;
                continue;
            }
            if (!isdigit(inputString[j]))
                throw std::runtime_error("err");
        }
        return stoi(inputString);
    }
    catch (...)
    {
        std::cout << "Значение стоящее " << count << " по порядку введено некорректно"
                                                     " введите исправленое значение"
                  << std::endl;
        if (isFileOpen)
            exit(1);
        string inputString;
        getline(cin, inputString);
        return getMyArr(inputString);
    }
}

bool isPrint(vector<vector<int>> coord, int firstPoint, int secondPoint, bool is_i_j, bool isFileClose, string userFile)
{
    int distanceX_ij, distanceY_ij;
    double distance_ij;
    distanceX_ij = abs(coord[secondPoint][0]) - abs(coord[firstPoint][0]);
    distanceY_ij = abs(coord[secondPoint][1]) - abs(coord[firstPoint][1]);
    distance_ij = sqrt(pow(distanceX_ij, 2) + pow(distanceY_ij, 2));
    if (is_i_j && !isFileClose)
        cout << "дистанция между i и j = " << distance_ij << endl;
    else if (is_i_j && isFileClose)
    {
        ofstream out(userFile);
        out << "дистанция между i и j = " << distance_ij << endl;
        out.close();
    }

    int buffDistanceX_ij, buffDistanceY_ij;
    for (int j = 0; j < coord.size(); j++)
    {
        if (j == firstPoint || j == secondPoint)
            continue;
        if (is_i_j)
        {
            buffDistanceX_ij = abs(coord[j][0]) - abs(coord[firstPoint][0]);
            buffDistanceY_ij = abs(coord[j][1]) - abs(coord[firstPoint][1]);
        }
        else
        {
            buffDistanceX_ij = abs(coord[j][0]) - abs(coord[secondPoint][0]);
            buffDistanceY_ij = abs(coord[j][1]) - abs(coord[secondPoint][1]);
        }
        double buffDistance_ij = sqrt(pow(buffDistanceX_ij, 2) + pow(buffDistanceY_ij, 2));
        if (isSmaller(distance_ij, buffDistance_ij))
        {
            if (!isFileClose)
                is_i_j ? printf("не выполняется первое условие, так как дистанция между точкой %i и %i"
                                " больше или равно дистанции между точками %i и %i\n",
                                j + 1, firstPoint + 1, firstPoint + 1, secondPoint + 1)
                       : printf("не выполняется второе условие, так как дистанция между точкой %i и %i"
                                " больше или равно дистанции между точками %i и %i\n",
                                j + 1, secondPoint + 1, firstPoint + 1, secondPoint + 1);
            else
            {
                ofstream out;
                out.open(userFile, ios::app);
                if (is_i_j)
                    out << "не выполняется первое условие, так как дистанция между точкой "
                        << j + 1 << " и " << firstPoint + 1 << " больше или равно дистанции между точками "
                        << firstPoint + 1 << " и " << secondPoint + 1 << endl;
                else
                    out << "не выполняется второе условие, так как дистанция между точкой "
                        << j + 1 << " и " << secondPoint + 1 << " больше или равно дистанции между точками "
                        << firstPoint + 1 << " и " << secondPoint + 1 << endl;
            }
            return false;
        }
    }
    return true;
}

vector<int> getTwoPoint(vector<vector<int>> coord, bool *isPrintFirst, bool *isPrintSecond,
                        bool isFileOpen, bool isFileClose = false, string userFile = "", int firstFilP = 0, int secondFilP = 0)
{
    int firstPoint, secondPoint;
    if (!isFileOpen)
    {
        cout << "Введите две разные точки через пробел: " << endl;
        while (1)
        {
            string inputString, stringFirstNum, stringSecondNum;
            getline(cin, inputString);
            stringstream stringNum;
            stringNum << inputString;

            if (getline(stringNum, stringFirstNum, ' ') && getline(stringNum, stringSecondNum))
            {
                firstPoint = getMyArr(stringFirstNum, 1) - 1;
                secondPoint = getMyArr(stringSecondNum, 2) - 1;
                if (firstPoint != secondPoint)
                    break;
            }
            cout << "Ошибка. Введено неверное значение" << endl;
        }
    }
    else
    {
        firstPoint = firstFilP;
        secondPoint = secondFilP;
    }

    if (firstPoint < coord.size() && secondPoint < coord.size())
    {
        *isPrintFirst = isPrint(coord, firstPoint, secondPoint, true, isFileClose, userFile);
        *isPrintSecond = isPrint(coord, firstPoint, secondPoint, false, isFileClose, userFile);
    }
    else
    {
        cout << "Ошибка. Введите существующие точки" << endl;
        if (!isFileOpen)
            return getTwoPoint(coord, isPrintFirst, isPrintSecond, false, isFileClose, userFile);
        exit(1);
    }
    return {firstPoint, secondPoint};
}

vector<vector<int>> getCoordPoints()
{
    cout << "Введите натуральное число координаты точки х1,y1,...хn,yn: " << endl;
    vector<vector<int>> tmpCoord;
    for (short i = 0;; i++)
    {
        printf("Введите x%i и y%i через пробел: ", i + 1, i + 1);
        string inputString, stringFirstNum, stringSecondNum;
        getline(cin, inputString);
        if (inputString == "" && tmpCoord.size() > 1)
            break;
        stringstream stringNum;
        stringNum << inputString;

        int coordX, coordY;
        if (getline(stringNum, stringFirstNum, ' ') && getline(stringNum, stringSecondNum))
        {
            coordX = getMyArr(stringFirstNum, 1);
            coordY = getMyArr(stringSecondNum, 2);
        }
        else
        {
            cout << "Ошибка. Введено неверное значение" << endl;
            i--;
            continue;
        }

        tmpCoord.push_back(vector<int>{coordX, coordY});
    }
    return tmpCoord;
}

bool choice()
{
    string tmp;
    getline(cin, tmp);
    if (tmp.size() == 1 && (tmp[0] == 'y' || tmp[0] == 'n'))
        return tmp == "y" ? true : false;
    cout << "Неверное значение\n";
    return choice();
}

string inputUserPath(string defaultPath)
{
    string userFile;
    cout << "Введите путь к файлу: ";
    getline(cin, userFile);
    if (userFile == "")
        return defaultPath;
    ofstream out(userFile);
    if (out)
    {
        out.close();
        return userFile;
    }
    else
    {
        cout << "Неверно указан путь к файлу\n";
        return inputUserPath(defaultPath);
    }
}

int main()
{
    while (1)
    {
        cout << "***************" << endl;

        cout << "Cчитать данные с файла? (y/n) ";
        bool isFileOpen = choice();
        string s1, s2, myArrFile;
        vector<vector<int>> coord;
        if (isFileOpen)
        {
            string userInputFpath = inputUserPath("./in.txt");
            ifstream input(userInputFpath);
            while (getline(input, myArrFile, '\n') && getline(input, s1, ' ') && getline(input, s2))
            {
                string tmpNum = "";
                bool flag = false;
                int j = 0;
                vector<int> tmpCoord;
                for (char i : myArrFile)
                {
                    if (i == ',' || i == ';')
                    {
                        tmpCoord.push_back(getMyArr(tmpNum, true));
                        if (j == 1)
                        {
                            coord.push_back(tmpCoord);
                            tmpCoord.clear();
                            j = 0;
                        }
                        tmpNum = "";
                        j++;
                        continue;
                    }
                    else
                        tmpNum += i;
                }
            }
            input.close();
        }
        else
            coord = getCoordPoints();

        cout << "вывести данные в файл? (y/n) ";
        bool isFileClose = choice();
        string userOutputFpath;
        if (isFileClose)
            userOutputFpath = inputUserPath("./out.txt");

        bool *isPrintFirst = new bool{true}, *isPrintSecond = new bool{true};
        vector<int> twoPoint;
        if (isFileClose)
        {
            if (!isFileOpen)
                twoPoint = getTwoPoint(coord, isPrintFirst, isPrintSecond, false, true, userOutputFpath);
            else
                twoPoint = getTwoPoint(coord, isPrintFirst, isPrintSecond, true, true, userOutputFpath,
                                       getMyArr(s1, true) - 1, getMyArr(s2, true) - 1);
        }
        else
        {
            if (!isFileOpen)
                twoPoint = getTwoPoint(coord, isPrintFirst, isPrintSecond, false);
            else
                twoPoint = getTwoPoint(coord, isPrintFirst, isPrintSecond, true, false, "",
                                       getMyArr(s1, true) - 1, getMyArr(s2, true) - 1);
        }

        if (!(*isPrintFirst || *isPrintSecond))
        {
            delete isPrintFirst;
            delete isPrintSecond;

            if (isFileOpen)
                break;

            cout << "Повторить? ";
            if (!choice())
                break;

            continue;
        }
        delete isPrintFirst;
        delete isPrintSecond;

        double scale = 1;
        int count = 0;
        for (int i = 700;; i *= 10)
        {
            count++;
            if (coord[twoPoint[0]][0] >= i || coord[twoPoint[0]][0] >= i || coord[twoPoint[1]][0] >= i || coord[twoPoint[1]][1] >= i)
                scale /= 10;
            else
                break;
        }

        if (isFileClose)
        {
            ofstream out;
            out.open(userOutputFpath, ios::app);
            out << "Масштаб изобрашения 1 к " << count << endl;
            out.close();
        }
        else
            cout << "Масштаб изобрашения 1 к " << count << endl;

        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "test");

        int centerWindow = WIDTH / 2;
        sf::Vertex line[2];
        line[0].position.x = centerWindow + coord[twoPoint[0]][0] * scale;
        line[0].position.y = centerWindow - coord[twoPoint[0]][1] * scale;
        line[0].color = sf::Color::Red;
        line[1].position.x = centerWindow + coord[twoPoint[1]][0] * scale;
        line[1].position.y = centerWindow - coord[twoPoint[1]][1] * scale;
        line[1].color = sf::Color::Red;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(line, 2, sf::Lines);
            window.display();
        }

        if (isFileOpen)
            break;

        cout << "Повторить? ";
        if (!choice())
            break;
    }
}