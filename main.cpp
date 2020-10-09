/*

--------�ؾ� ����--------
1. ��� ����ġ�� ���� X      V
2. �й�����              
   �� ��Ż�� or �����.      /
3. �¸�����                  V
   �� �� ��ä��.
4. �ݴ���� �Է��� ����      V
5. �ٸ���� �ڶ��ϱ�(�߿�)
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

random_device rd; // ��
mt19937 gen(rd());// ��
uniform_int_distribution<int> dis(0, 14); //����
int toward = RIGHT; // �ʱ� ����
int x = 0, y = 0; // �Ӹ� ��ġ
int point = 0; // ����
RECT rc; // �簢���� ��ǥ
HWND myconsole;
HDC mydc;

bool aps = false; // ��� ���� ����
int z, c; //����� ��ǥ

vector<int> tx; // ����
vector<int> ty; // ����


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

            if (tx.size() == 0) { // size�� 0�϶��� ���� ���ؼ� ��ð��� ���� �ѰŴ�
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

        if (x == z && y == c) {  // ���� �ø���
            point += 1;
            aps = false;
        }

        if (tx.size() >= point) { // �����
            for (int i = 0; i < tx.size() - point; i++) {
                rc = { tx.back() * 20 + 14, ty.back() * 20 + 14, (tx.back() + 1) * 20 + 10, (ty.back() + 1) * 20 + 10 };
                FillRect(mydc, &rc, CreateSolidBrush(RGB(12, 12, 12)));
                tx.pop_back();
                ty.pop_back();
            }
        }

        if (x < 0 || x > 14 || y < 0 || y > 14 || point == 224) break; // �й�: ��Ż��
        if (point == 224) break; // �¸�: �� �� ä��
        Sleep(120);
    }

    ReleaseDC(myconsole, mydc);
    cin.ignore();
    return 0;
}