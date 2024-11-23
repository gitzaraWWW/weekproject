#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // _kbhit(), _getch() �Լ� ����� ���� ���
#include <windows.h>

#include "ElfMath.h"

#define Console_WIDTH 80 //�ܼ��� 80 * 24
#define Console_HEIGHT 24 
#define SB_WIDTH 40 //��ũ�� ���۴� 40 * 24
#define SB_HEIGHT 24
#define PI 3.1415926535

// ���� ������ ��ũ�� ���� ����
char screenBuffer[SB_WIDTH * SB_HEIGHT * 2]; // �� �ȼ��� char���ڿ� 2��
int screenWidth = SB_WIDTH;
int screenHeight = SB_HEIGHT;

// ȭ�� �ʱ�ȭ �Լ�
void ClearScreen() {
    for (int i = 0; i < SB_HEIGHT; i++) {
        for (int j = 0; j < SB_WIDTH; j++) {
            // �� �ȼ��� �� ���ڸ� �������� �ʱ�ȭ
            screenBuffer[(i * SB_WIDTH + j) * 2] = ' ';  // ù ��° ����
            screenBuffer[(i * SB_WIDTH + j) * 2 + 1] = ' ';  // �� ��° ����
        }
    }

}

// �� ���� �����ϴ� ������ �׸��� �Լ� (�극���� �˰���)
void DrawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < SB_WIDTH && y1 >= 0 && y1 < SB_HEIGHT) {
            // �� �ȼ��� �� ���ڸ� ����
            screenBuffer[(y1 * SB_WIDTH + x1) * 2] = '*';  // ù ��° ����
            screenBuffer[(y1 * SB_WIDTH + x1) * 2 + 1] = '*';  // �� ��° ����
        }

        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

float sunAngle = 0.0f;  // �¾��� ���� ����

// �¾��� �׸��� �Լ�
void DrawSun(float sunAngle) {
    int centerX = 20; // �¾� �߽� X ��ǥ
    int centerY = 12; // �¾� �߽� Y ��ǥ
    int sunsize = 5;   // �¾� ũ�� (�������� ������������ �Ÿ�)

    // ȸ�� ��� �� �̵� ��� ���
    Matrix3x3 translation_w = translation_matrix(centerX, centerY);
    Matrix3x3 rotation = rotation_matrix(sunAngle);  // ȸ�� ��� ����
    Matrix3x3 World = multiply_matrices(translation_w, rotation);

    // �¾� �ﰢ���� �� ������ ��ǥ ���
    Vector3 p1 = { 0, -sunsize, 1 };  // ù ��° ������ (����)
    Vector3 p2 = { sunsize, sunsize, 1 };  // �� ��° ������ (������)
    Vector3 p3 = { -sunsize, sunsize, 1 };  // �� ��° ������ (����)

    // ȸ�� �� ��ȯ�� �� ������ ���
    p1 = multiply_matrix_vector(World, p1);
    p2 = multiply_matrix_vector(World, p2);
    p3 = multiply_matrix_vector(World, p3);

    // ȸ���� ������ ȭ�鿡 �׸��� (�������� ����)
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // p1 -> p2
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // p2 -> p3
    DrawLine((int)p3.x, (int)p3.y, (int)p1.x, (int)p1.y); // p3 -> p1

    // �ﰢ�� ���� ä��� (y�� �����ϸ鼭 x�� ä��)
    int minY = (int)fmin(fmin(p1.y, p2.y), p3.y);
    int maxY = (int)fmax(fmax(p1.y, p2.y), p3.y);

    for (int y = minY; y <= maxY; y++) {
        // y�� �ش��ϴ� �� ���� x ���� ���
        int minX = INT_MAX;
        int maxX = INT_MIN;

        // �� ���� x ���� ���
        if (p1.y != p2.y) {
            int x1 = p1.x + (y - p1.y) * (p2.x - p1.x) / (p2.y - p1.y);
            if (y >= fmin(p1.y, p2.y) && y <= fmax(p1.y, p2.y)) {
                minX = fmin(minX, x1);
                maxX = fmax(maxX, x1);
            }
        }
        if (p2.y != p3.y) {
            int x2 = p2.x + (y - p2.y) * (p3.x - p2.x) / (p3.y - p2.y);
            if (y >= fmin(p2.y, p3.y) && y <= fmax(p2.y, p3.y)) {
                minX = fmin(minX, x2);
                maxX = fmax(maxX, x2);
            }
        }
        if (p3.y != p1.y) {
            int x3 = p3.x + (y - p3.y) * (p1.x - p3.x) / (p1.y - p3.y);
            if (y >= fmin(p3.y, p1.y) && y <= fmax(p3.y, p1.y)) {
                minX = fmin(minX, x3);
                maxX = fmax(maxX, x3);
            }
        }

        // x ���� ������ ä���
        for (int x = minX; x <= maxX; x++) {
            if (x >= 0 && x < SB_WIDTH && y >= 0 && y < SB_HEIGHT) {
                screenBuffer[(y * SB_WIDTH + x) * 2] = 'S';  // �¾� ���� ä���
                screenBuffer[(y * SB_WIDTH + x) * 2 + 1] = 'S';
            }
        }
    }
}

// �簢���� �߽����κ��� 12��ŭ ������ ��ġ�� ȸ���ϴ� �簢�� �׸���
void DrawRectangle(float sunAngle) {
    int centerX = 20; // �¾� �߽� X ��ǥ
    int centerY = 12; // �¾� �߽� Y ��ǥ
    int distance = 12; // �¾� �߽����κ��� ������ �Ÿ�
    int rectangleSize = 2; // �簢���� ������ (��, �簢���� �� �� ���̴� 4)

    // �簢�� �߽� ��ǥ ��� (�¾� �߽ɿ��� 12��ŭ ������ ��ġ)
    int rectCenterX = centerX + distance;
    int rectCenterY = centerY;

    // �簢���� �� ������ ��ǥ ��� (ȸ�� ��)
    Vector3 p1 = { rectCenterX - rectangleSize, rectCenterY - rectangleSize, 1 };  // ���� �Ʒ�
    Vector3 p2 = { rectCenterX + rectangleSize, rectCenterY - rectangleSize, 1 };  // ������ �Ʒ�
    Vector3 p3 = { rectCenterX + rectangleSize, rectCenterY + rectangleSize, 1 };  // ������ ��
    Vector3 p4 = { rectCenterX - rectangleSize, rectCenterY + rectangleSize, 1 };  // ���� ��

    // ȸ�� ��� ���
    Matrix3x3 rotation = rotation_matrix(sunAngle);  // sunAngle�� �������� ȸ��
    p1 = multiply_matrix_vector(rotation, p1);
    p2 = multiply_matrix_vector(rotation, p2);
    p3 = multiply_matrix_vector(rotation, p3);
    p4 = multiply_matrix_vector(rotation, p4);

    // ȸ���� �簢���� ȭ�鿡 �׸��� (�������� ����)
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // p1 -> p2
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // p2 -> p3
    DrawLine((int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y); // p3 -> p4
    DrawLine((int)p4.x, (int)p4.y, (int)p1.x, (int)p1.y); // p4 -> p1
}

// ���� �ʱ�ȭ �Լ�
void Init() {

    ClearScreen();

    //Ŀ�� �����
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;  // Ŀ���� ����
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// ȭ���� �׸��� �Լ�
void Draw(char* Buffer) {
    //0,0���� �̵�
    COORD coord = { 0, 0 };  // ��ǥ (0, 0)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    // �� �ȼ��� �� ���ھ� ���
    for (int i = 0; i < SB_HEIGHT; i++) {
        for (int j = 0; j < SB_WIDTH; j++) {
            // 2���� ���� ���
            putchar(screenBuffer[(i * SB_WIDTH + j) * 2]);      // ù ��° ����
            putchar(screenBuffer[(i * SB_WIDTH + j) * 2 + 1]);  // �� ��° ����
        }
        putchar('\n');  // �ٹٲ�
    }
}

// ȭ�� ������Ʈ �Լ�
void Update() {
    ClearScreen();
    DrawSun(sunAngle);
    DrawRectangle(sunAngle);

    sunAngle -= 10;
    if (sunAngle < 0) sunAngle += 2 * PI; // 360�� �ʰ��� �ٽ� 0����
}
// ���� ����
int main() {
    // ���� �ʱ�ȭ
    Init();

    // ������ Ÿ�̸�
    clock_t lastTime = clock();
    int fps = 60;
    double frameTime = 1000.0 / fps;

    while (1) {
        // ESC Ű�� �������� Ȯ��
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {  // ESC Ű�� ������ ���� ����
                break;
            }
        }

        // ���� �ð� ���
        clock_t now = clock();
        double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC * 1000.0;

        // ������ ���� (60fps ����)
        if (deltaTime >= frameTime) {
            lastTime = now;

            // ���� ������Ʈ
            Update();  //+ ClearScreen(); Render(angle); ��� ok

            // ȭ�� �׸���
            //Draw(clearBuffer);
            Draw(screenBuffer);
        }
    }

    return 0;
}