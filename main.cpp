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
    return toInt64(x1) < toInt64(x2);
}

int64_t getMyArr(string inputString, short count = 0, bool isFileOpen = false)
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
                throw runtime_error("err");
        }
        return stoi(inputString);
    }
    catch (...)
    {
        if (isFileOpen)
        {
            cout << "Неверный ввод данных в файле" << endl;
            exit(1);
        }
        cout << "Значение стоящее " << count << " по порядку введено некорректно"
                                                " введите исправленое значение"
             << endl;
        getline(cin, inputString);
        return getMyArr(inputString, count, isFileOpen);
    }
}

bool isPrint(vector<vector<int64_t>> coord, int64_t firstPoint, int64_t secondPoint, bool is_i_j,
             bool isFileClose, fstream &userFile)
{
    int64_t distanceX_ij = abs(coord[secondPoint][0]) - abs(coord[firstPoint][0]);
    int64_t distanceY_ij = abs(coord[secondPoint][1]) - abs(coord[firstPoint][1]);
    double distance_ij = sqrt(pow(distanceX_ij, 2) + pow(distanceY_ij, 2));

    if (is_i_j)
    {
        if (isFileClose)
            userFile << "дистанция между i и j = " << distance_ij << endl;
        else
            cout << "дистанция между i и j = " << distance_ij << endl;
    }

    uint64_t secondArg = is_i_j ? firstPoint : secondPoint;
    for (int j = 0; j < coord.size(); j++)
    {
        if (j == firstPoint || j == secondPoint)
            continue;
        distanceX_ij = abs(coord[j][0]) - abs(coord[secondArg][0]);
        distanceY_ij = abs(coord[j][1]) - abs(coord[secondArg][1]);

        double buffDistance_ij = sqrt(pow(distanceX_ij, 2) + pow(distanceY_ij, 2));
        if (isSmaller(distance_ij, buffDistance_ij))
        {
            stringstream errMsg;
            string firSec = is_i_j ? "первое " : "второе ";
            errMsg << "не выполняется " << firSec << "условие, так как дистанция между точкой "
                   << j + 1 << " и " << firstPoint + 1 << " больше или равно дистанции между точками "
                   << firstPoint + 1 << " и " << secondPoint + 1 << endl;

            if (!isFileClose)
                cout << errMsg.str();
            else
                userFile << errMsg.str();
            return false;
        }
    }
    return true;
}

vector<int64_t> getTwoPoint(vector<vector<int64_t>> coord, bool *isPrintFirst, bool *isPrintSecond,
                            bool isFileOpen, fstream &userFile, bool isFileClose,
                            int64_t firstFilP = 0, int64_t secondFilP = 0)
{
    int64_t firstPoint, secondPoint;
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
                firstPoint = getMyArr(stringFirstNum, 1, isFileOpen) - 1;
                secondPoint = getMyArr(stringSecondNum, 2, isFileOpen) - 1;
                if (firstPoint != secondPoint)
                    break;
            }
            cout << "Ошибка. Введено неверное значение" << endl;
        }
    }
    else
    {
        firstPoint = firstFilP - 1;
        secondPoint = secondFilP - 1;
    }

    if (firstPoint < coord.size() && secondPoint < coord.size() && firstPoint > -1 && secondPoint > -1)
    {
        *isPrintFirst = isPrint(coord, firstPoint, secondPoint, true, isFileClose, userFile);
        *isPrintSecond = isPrint(coord, firstPoint, secondPoint, false, isFileClose, userFile);
    }
    else
    {
        cout << "Ошибка. Введите существующие точки" << endl;
        if (!isFileOpen)
            return getTwoPoint(coord, isPrintFirst, isPrintSecond, false, userFile, isFileClose);
        exit(1);
    }
    return {firstPoint, secondPoint};
}

vector<vector<int64_t>> getCoordPoints()
{
    cout << "Введите натуральное число координаты точки х1,y1,...хn,yn: " << endl;
    vector<vector<int64_t>> tmpCoord;
    for (short i = 0;; i++)
    {
        printf("Введите x%i и y%i через пробел: ", i + 1, i + 1);
        string inputString, stringFirstNum, stringSecondNum;
        getline(cin, inputString);
        if (inputString == "" && tmpCoord.size() > 1)
            break;
        stringstream stringNum;
        stringNum << inputString;

        int64_t coordX, coordY;
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
        tmpCoord.push_back(vector<int64_t>{coordX, coordY});
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

fstream inputUserPath(string defaultPath, bool isOpen)
{
    string userFile;
    cout << "Введите путь к файлу: ";
    getline(cin, userFile);
    fstream myFile;
    if (userFile == "")
    {
        myFile.open(defaultPath, isOpen ? ifstream::in : ifstream::out);
        return myFile;
    }
    else
        myFile.open(userFile, isOpen ? ifstream::in : ifstream::out);

    if (myFile)
    {
        myFile.close();
        return myFile;
    }
    else
    {
        cout << "Неверно указан путь к файлу\n";
        return inputUserPath(defaultPath, isOpen);
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
        vector<vector<int64_t>> coord;
        if (isFileOpen)
        {
            fstream input = inputUserPath("./in.txt", true);
            while (getline(input, myArrFile, '\n') && getline(input, s1, ' ') && getline(input, s2))
            {
                string tmpNum = "";
                int64_t j = 0;
                int32_t count = 0;
                vector<int64_t> tmpCoord;
                for (char i : myArrFile)
                {
                    count++;
                    if (i == ',' || i == ';' || count == myArrFile.size())
                    {
                        if (count == myArrFile.size())
                        {
                            tmpNum += i;
                            tmpCoord.push_back(getMyArr(tmpNum, 1, true));
                            coord.push_back(tmpCoord);
                            break;
                        }
                        tmpCoord.push_back(getMyArr(tmpNum, 1, true));
                        if (i == ';')
                        {
                            if (j != 1)
                            {
                                cout << "Неверный ввод данных в файле" << endl;
                                exit(1);
                            }
                            coord.push_back(tmpCoord);
                            tmpCoord.clear();
                            j = 0;
                        }
                        else
                            j++;

                        tmpNum = "";
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
        fstream userOutputFile;
        if (isFileClose)
            userOutputFile = inputUserPath("./out.txt", false);

        bool *isPrintFirst = new bool{true}, *isPrintSecond = new bool{true};
        vector<int64_t> twoPoint = getTwoPoint(coord, isPrintFirst, isPrintSecond,
                                               !isFileOpen ? false : true, userOutputFile,
                                               isFileClose ? true : false,
                                               !isFileOpen ? 0 : getMyArr(s1, 1, true),
                                               !isFileOpen ? 0 : getMyArr(s2, 2, true));

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

        int scale = 0;
        int count = 1;
        for (count;; count++)
        {
            scale++;
            if (!(coord[twoPoint[0]][0] / scale >= WIDTH || coord[twoPoint[0]][1] / scale >= WIDTH 
            || coord[twoPoint[1]][0] / scale >= WIDTH || coord[twoPoint[1]][1] / scale >= WIDTH))
                break;
        }

        if (isFileClose)
        {
            userOutputFile << "Масштаб изобрашения 1 к " << count << endl;
            userOutputFile.close();
        }
        else
            cout << "Масштаб изобрашения 1 к " << count << endl;

        sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "test");

        int centerWindow = WIDTH / 2;
        // sf::CircleShape centerCircle(1);
        sf::Vertex line[2];
        line[0].position.x = centerWindow + coord[twoPoint[0]][0] / scale;
        line[0].position.y = centerWindow - coord[twoPoint[0]][1] / scale;
        line[0].color = sf::Color::Red;
        line[1].position.x = centerWindow + coord[twoPoint[1]][0] / scale;
        line[1].position.y = centerWindow - coord[twoPoint[1]][1] / scale;
        line[1].color = sf::Color::Red;

        // centerCircle.setPosition(sf::Vector2f(centerWindow, centerWindow));
        // centerCircle.setFillColor(sf::Color::White);

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
            // window.draw(centerCircle);
            window.display();
        }

        if (isFileOpen)
            break;

        cout << "Повторить? ";
        if (!choice())
            break;
    }
}