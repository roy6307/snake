/*

--------해야 할일--------
1. 사과 뱀위치엔 스폰 X      V
2. 패배조건              
   ㄴ 맵탈출 or 꼬라박.      /
3. 승리조건                  V
   ㄴ 맵 꽉채움.
4. 반대방향 입력은 무시      V
5. 다만들고 자랑하기(중요)
-------------------------

*/

#include <windows.h>
#include <iostream>
#include <cmath>
#include <conio.h>
#include <random>
#include <vector>

#define UP 72
#define RIGHT 77
#define DOWN 80
#define LEFT 75

using namespace std;

random_device rd; // 랜
mt19937 gen(rd());// 덤
uniform_int_distribution<int> dis(0, 14); //랜덤
int toward = RIGHT; // 초기 방향
int x = 0, y = 0; // 머리 위치
int point = 0; // 점수
RECT rc; // 사각형용 좌표
HWND myconsole;
HDC mydc;

bool aps = false; // 사과 존재 여부
int z, c; //사과용 좌표

vector<int> tx; // 흔적
vector<int> ty; // 흔적


void move(int key) {
    tx.insert(tx.begin(), 1, x);
    ty.insert(ty.begin(), 1, y);
    rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };
    switch (key)
    {
    case LEFT:
        toward = LEFT;
        x -= 1;
        rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };
        FillRect(mydc, &rc, CreateSolidBrush(RGB(0, 153, 0)));
        break;

    case RIGHT:
        toward = RIGHT;
        x += 1;
        rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };
        FillRect(mydc, &rc, CreateSolidBrush(RGB(0, 153, 0)));
        break;

    case UP:
        toward = UP;
        y -= 1;
        rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };
        FillRect(mydc, &rc, CreateSolidBrush(RGB(0, 153, 0)));
        break;

    case DOWN:
        toward = DOWN;
        y += 1;
        rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };
        FillRect(mydc, &rc, CreateSolidBrush(RGB(0, 153, 0)));
        break;
    }
}

void spawn_apple() {
    if (aps == false) {

        bool temp = false;

        while (!temp) {

            z = dis(gen), c = dis(gen);

            if (tx.size() == 0) { // size가 0일때를 생각 못해서 몇시간을 삽질 한거니
                if (z == 0 && c == 0) {
                    temp = false;
                } else {
                    temp = true;
                }
            }

            for (int i = 0; i < tx.size(); i++) {
                if (tx[i] == z && ty[i] == c) {
                    temp = false;
                    break;
                } else { 
                    temp = true;  
                }
            }

            if (temp) {
                rc = { z * 20 + 14, c * 20 + 14, (z + 1) * 20 + 10, (c + 1) * 20 + 10 };
                FillRect(mydc, &rc, CreateSolidBrush(RGB(121, 121, 0)));
                aps = true;
                break;
            }
        }
    }
}

int main()
{
    myconsole = GetConsoleWindow();
    mydc = GetDC(myconsole);
    rc = { x * 20 + 14, y * 20 + 14, (x + 1) * 20 + 10, (y + 1) * 20 + 10 };

    for (int i = 10; i <= 314; i++) {

        SetPixel(mydc, i, 10, RGB(255, 255, 255));
        SetPixel(mydc, 10, i, RGB(255, 255, 255));
        SetPixel(mydc, i, 314, RGB(255, 255, 255));
        SetPixel(mydc, 314, i, RGB(255, 255, 255));

    }

    FillRect(mydc, &rc, CreateSolidBrush(RGB(0, 153, 0)));

    while (1) {

        spawn_apple();

        if (_kbhit()) {
            int key = _getch();
            if (key == 224) {
                key = _getch();

                if ((key == LEFT) != (toward == RIGHT) || (key == RIGHT) != (toward == LEFT) || (key == UP) != (toward == DOWN) || (key == DOWN) != (toward == UP)) move(key);
                else move(toward);
            }
        }
        else {
            move(toward);
        }

        if (x == z && y == c) {  // 점수 올리기
            point += 1;
            aps = false;
        }

        if (tx.size() >= point) { // 지우기
            for (int i = 0; i < tx.size() - point; i++) {
                rc = { tx.back() * 20 + 14, ty.back() * 20 + 14, (tx.back() + 1) * 20 + 10, (ty.back() + 1) * 20 + 10 };
                FillRect(mydc, &rc, CreateSolidBrush(RGB(12, 12, 12)));
                tx.pop_back();
                ty.pop_back();
            }
        }

        if (x < 0 || x > 14 || y < 0 || y > 14 || point == 224) break; // 패배: 맵탈출
        if (point == 224) break; // 승리: 맵 다 채움
        Sleep(120);
    }

    ReleaseDC(myconsole, mydc);
    cin.ignore();
    return 0;
}