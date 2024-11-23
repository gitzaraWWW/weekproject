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
float eAngle = 0.0f;  // ������ ���� ����
float earthAngle = 0.0f;  // ������ ���� ����
float mAngle = 0.0f;  // ���� ���� ����
float moonAngle = 0.0f;  // ���� ���� ����

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

// ������ �׸��� �Լ�
void DrawEarth(float earthAngle, float eAngle, float deltaTime) {
    int earthSize = 2;  // ���� ũ�� (�������� ������������ �Ÿ�)

    // ���� �ӵ� (2�ʴ� 1����, �� 360�� = 2 / PI ����)
    float rotationSpeed = PI / 2;

    // ������ �߽� ��ǥ (�¾��� �߽����� ����)
    int centerX = 20 + 12 * cosf(earthAngle);
    int centerY = 12 + 12 * sinf(earthAngle);

    // ���� ���� ���� ������Ʈ (�ð����)
    eAngle -= rotationSpeed * deltaTime;  // �ð�������� ȸ�� (���� ����)

    // ȸ�� ����� �̿��� ���� ���� ����
    Matrix3x3 rotation = rotation_matrix(eAngle);  // ���� ���� ������ ���� ȸ�� ��� ����

    // ������ �׸� ����� �׸��� ���� ȸ�� wjs
    Vector3 p1 = { centerX - earthSize, centerY - earthSize, 1 };  // ���� ���
    Vector3 p2 = { centerX + earthSize, centerY - earthSize, 1 };  // ������ ���
    Vector3 p3 = { centerX + earthSize, centerY + earthSize, 1 };  // ������ �ϴ�
    Vector3 p4 = { centerX - earthSize, centerY + earthSize, 1 };  // ���� �ϴ�

    // ȸ���� ���� ���
    p1 = multiply_matrix_vector(rotation, p1);
    p2 = multiply_matrix_vector(rotation, p2);
    p3 = multiply_matrix_vector(rotation, p3);
    p4 = multiply_matrix_vector(rotation, p4);

    // �׸� ��� �׸���
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // ��
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // ������
    DrawLine((int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y); // �Ʒ�
    DrawLine((int)p4.x, (int)p4.y, (int)p1.x, (int)p1.y); // ����

    // ���� ä���
    for (int y = (int)fmin(fmin(p1.y, p2.y), fmin(p3.y, p4.y)); y <= (int)fmax(fmax(p1.y, p2.y), fmax(p3.y, p4.y)); y++) {
        for (int x = (int)fmin(fmin(p1.x, p2.x), fmin(p3.x, p4.x)); x <= (int)fmax(fmax(p1.x, p2.x), fmax(p3.x, p4.x)); x++) {
            if (x >= 0 && x < SB_WIDTH && y >= 0 && y < SB_HEIGHT) {
                screenBuffer[(y * SB_WIDTH + x) * 2] = 'E';  // ���� ���� ä���
                screenBuffer[(y * SB_WIDTH + x) * 2 + 1] = 'E';
            }
        }
    }
}

// �� �׸���
void DrawMoon(float moonAngle, float earthAngle, float deltaTime) {
    int moonSize = 1;  // �� ũ�� (������ 3, ������ 1.5 �ݳ����ؼ� 1�� ǥ��)

    // ������ �߽� ��ǥ ��� (���� ���� ������ ����)
    int earthCenterX = 20 + 12 * cosf(earthAngle);
    int earthCenterY = 12 + 12 * sinf(earthAngle);

    // ���� ��ġ�� �����κ��� ������ �Ÿ� 4��ŭ
    int moonCenterX = earthCenterX + 4 * cosf(moonAngle);
    int moonCenterY = earthCenterY + 4 * sinf(moonAngle);

    // ���� ���� �ֱ�: 1�ʴ� 1���� (�ð����)
    float moonRotationSpeed = 2 * PI;  // 1�ʿ� 1���� ȸ�� (�ð����)

    // ���� ���� ������Ʈ (�ð����)
    moonAngle -= moonRotationSpeed * deltaTime;  // ���� ���� (�ð���� ȸ��)

    // ���� �׸��� ���� ���� ���� ����ϰ� ������ ���� (360�� ���� ���� ��� ����)
    int numPoints = 360;  // ���� �׸��� ���� ���� ���� (���е��� ������ �� ����)
    for (int i = 0; i < numPoints; i++) {
        float angle1 = i * 2 * PI / numPoints;  // ù ��° ����
        float angle2 = (i + 1) * 2 * PI / numPoints;  // �� ��° ����

        // ù ��° �� (����1�� �������� ���)
        int x1 = moonCenterX + moonSize * cosf(angle1);
        int y1 = moonCenterY + moonSize * sinf(angle1);

        // �� ��° �� (����2�� �������� ���)
        int x2 = moonCenterX + moonSize * cosf(angle2);
        int y2 = moonCenterY + moonSize * sinf(angle2);

        // �� ���� �����ϴ� ������ �׸��� (DrawLine �Լ� ���)
        DrawLine(x1, y1, x2, y2);
    }

    // �� ���� ä���
    for (int dx = -moonSize; dx <= moonSize; dx++) {
        for (int dy = -moonSize; dy <= moonSize; dy++) {
            if (dx * dx + dy * dy <= moonSize * moonSize) {
                int fillX = moonCenterX + dx;
                int fillY = moonCenterY + dy;

                // ȭ�� ���� �������� �׸���
                if (fillX >= 0 && fillX < SB_WIDTH && fillY >= 0 && fillY < SB_HEIGHT) {
                    screenBuffer[(fillY * SB_WIDTH + fillX) * 2] = 'M';  // �� ���� ä���
                    screenBuffer[(fillY * SB_WIDTH + fillX) * 2 + 1] = 'M';
                }
            }
        }
    }
}

// �¾�, ����, ���� 1�� ��ġ
void DrawSunEarthMoon() {
    int centerX = 20; // �¾� �߽� X ��ǥ
    int centerY = 12; // �¾� �߽� Y ��ǥ

    // �¾� �׸���
    DrawSun(0.0f);  // �¾� �׸��� (ȸ������ ����)

    // ���� �׸��� (��ġ�� �������� ��ġ)
    int earthX = centerX + 12;  // ���� X ��ǥ
    int earthY = centerY;       // ���� Y ��ǥ
    DrawEarth(0.0f, 0.0f, 1.0f);  // ���� �׸���

    // �� �׸���
    int moonX = earthX + 4;     // �� X ��ǥ
    int moonY = earthY;         // �� Y ��ǥ
    DrawMoon(0.0f, 0.0f, 1.0f);  // �� �׸���
}

// Ŀ���� ��ġ�� �����ϴ� �Լ�(printf�� �߾ӿ� ����)
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;  // X ��ǥ (����)
    coord.Y = y;  // Y ��ǥ (����)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// ȭ�鿡 �ؽ�Ʈ�� �߾ӿ� ����ϴ� �Լ�
void printCenteredText(const char* text) {
    int textLength = strlen(text);
    int startX = (Console_WIDTH - textLength) / 2;
    int startY = Console_HEIGHT / 2;

    gotoXY(startX, startY);  // Ŀ�� ��ġ�� �߾����� �̵�
    printf("%s", text);      // �߾ӿ� �ؽ�Ʈ ���
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

// ȸ�� �ӵ� ������ ���� ���� �߰�
float sunRotationSpeed = -2 * PI * 20.0;   // (����) �¾��� 3�ʿ� 1���� �ݽð�
float earthRotationSpeed = 2 * PI / 3.0f;  // (����) ������ 3�ʿ� 1���� �ð�
float moonRotationSpeed = -2 * PI / 2.0f;   //(����) ���� 2�ʿ� 1���� �ݽð�

clock_t lastTime;
// Update �Լ����� ȸ�� ���� ������Ʈ �κ� ����
void Update() {
    // deltaTime ��� (���� �ð��� ���� �ð��� ����)
    clock_t now = clock();
    double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC;  // deltaTime�� �� ������ ���
    lastTime = now;

    // ���� ������Ʈ : deltaTime�� ����ؼ� �ӵ� ����
    sunAngle += sunRotationSpeed * deltaTime;
    earthAngle += earthRotationSpeed * deltaTime;
    moonAngle += moonRotationSpeed * deltaTime;

    // 360���� ������ 0���� ����
    if (sunAngle >= 2 * PI) sunAngle -= 2 * PI;
    if (earthAngle >= 2 * PI) earthAngle -= 2 * PI;
    if (moonAngle >= 2 * PI) moonAngle -= 2 * PI;

    ClearScreen();
    DrawSun(sunAngle);
    DrawEarth(earthAngle,eAngle,deltaTime);
    DrawMoon(moonAngle, earthAngle, deltaTime);
}

// ���� ����
int main() {
    // ���� �ʱ�ȭ
    Init();

    // ������ Ÿ�̸�
    enum GameState { INTRO, LINEAR, ROTATE } gameState = INTRO; // ����: INTRO (�ʱ� ȭ��), LINEAR (1�� ��ġ), ROTATE (ȸ�� ����)
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

            if (ch == ' ') { // �����̽��ٰ� ������ ���� ����
                if (gameState == INTRO) {
                    gameState = LINEAR; // ù ��° �����̽���: 1�� ��ġ ���·� ����
                }
                else if (gameState == LINEAR) {
                    gameState = ROTATE; // �� ��° �����̽���: ����/���� ���� ���·� ����
                }
            }
        }

        // ���� �ð� ���
        clock_t now = clock();
        double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC * 1000.0;

        // ������ ���� (60fps ����)
        if (deltaTime >= frameTime) {
            lastTime = now;

            if (gameState == INTRO) {
                printCenteredText("20211413 �ɱ�ȣ");
            }
            else if (gameState == LINEAR) {
                // �� ��° �����̽��� �Է� ��, �༺���� 1�ڷ� ��ġ
                DrawSunEarthMoon();

            }
            else if (gameState == ROTATE) {
                // �� ��° �����̽��� �Է� ��, ������ ���� ����
                Update();
            }

            // ȭ�� �׸���
            Draw(screenBuffer);
        }
    }


    return 0;
}
