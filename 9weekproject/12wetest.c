#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>  // _kbhit(), _getch() 함수 사용을 위한 헤더
#include <windows.h>

#include "ElfMath.h"

#define Console_WIDTH 80 //콘솔은 80 * 24
#define Console_HEIGHT 24 
#define SB_WIDTH 40 //스크린 버퍼는 40 * 24
#define SB_HEIGHT 24
#define PI 3.1415926535

// 전역 변수로 스크린 버퍼 선언
char screenBuffer[SB_WIDTH * SB_HEIGHT * 2]; // 각 픽셀당 char문자열 2개
int screenWidth = SB_WIDTH;
int screenHeight = SB_HEIGHT;

// 화면 초기화 함수
void ClearScreen() {
    for (int i = 0; i < SB_HEIGHT; i++) {
        for (int j = 0; j < SB_WIDTH; j++) {
            // 각 픽셀의 두 문자를 공백으로 초기화
            screenBuffer[(i * SB_WIDTH + j) * 2] = ' ';  // 첫 번째 문자
            screenBuffer[(i * SB_WIDTH + j) * 2 + 1] = ' ';  // 두 번째 문자
        }
    }

}

// 두 점을 연결하는 직선을 그리는 함수 (브레젠햄 알고리즘)
void DrawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        if (x1 >= 0 && x1 < SB_WIDTH && y1 >= 0 && y1 < SB_HEIGHT) {
            // 각 픽셀에 두 문자를 삽입
            screenBuffer[(y1 * SB_WIDTH + x1) * 2] = '*';  // 첫 번째 문자
            screenBuffer[(y1 * SB_WIDTH + x1) * 2 + 1] = '*';  // 두 번째 문자
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

float sunAngle = 0.0f;  // 태양의 자전 각도

// 태양을 그리는 함수
void DrawSun(float sunAngle) {
    int centerX = 20; // 태양 중심 X 좌표
    int centerY = 12; // 태양 중심 Y 좌표
    int sunsize = 5;   // 태양 크기 (중점에서 꼭지점까지의 거리)

    // 회전 행렬 및 이동 행렬 계산
    Matrix3x3 translation_w = translation_matrix(centerX, centerY);
    Matrix3x3 rotation = rotation_matrix(sunAngle);  // 회전 행렬 생성
    Matrix3x3 World = multiply_matrices(translation_w, rotation);

    // 태양 삼각형의 세 꼭지점 좌표 계산
    Vector3 p1 = { 0, -sunsize, 1 };  // 첫 번째 꼭지점 (위쪽)
    Vector3 p2 = { sunsize, sunsize, 1 };  // 두 번째 꼭지점 (오른쪽)
    Vector3 p3 = { -sunsize, sunsize, 1 };  // 세 번째 꼭지점 (왼쪽)

    // 회전 및 변환된 각 꼭지점 계산
    p1 = multiply_matrix_vector(World, p1);
    p2 = multiply_matrix_vector(World, p2);
    p3 = multiply_matrix_vector(World, p3);

    // 회전된 점들을 화면에 그리기 (직선으로 연결)
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // p1 -> p2
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // p2 -> p3
    DrawLine((int)p3.x, (int)p3.y, (int)p1.x, (int)p1.y); // p3 -> p1

    // 삼각형 내부 채우기 (y가 증가하면서 x를 채움)
    int minY = (int)fmin(fmin(p1.y, p2.y), p3.y);
    int maxY = (int)fmax(fmax(p1.y, p2.y), p3.y);

    for (int y = minY; y <= maxY; y++) {
        // y에 해당하는 두 점의 x 범위 계산
        int minX = INT_MAX;
        int maxX = INT_MIN;

        // 각 변의 x 범위 계산
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

        // x 범위 내에서 채우기
        for (int x = minX; x <= maxX; x++) {
            if (x >= 0 && x < SB_WIDTH && y >= 0 && y < SB_HEIGHT) {
                screenBuffer[(y * SB_WIDTH + x) * 2] = 'S';  // 태양 내부 채우기
                screenBuffer[(y * SB_WIDTH + x) * 2 + 1] = 'S';
            }
        }
    }
}

// 사각형의 중심으로부터 12만큼 떨어진 위치에 회전하는 사각형 그리기
void DrawRectangle(float sunAngle) {
    int centerX = 20; // 태양 중심 X 좌표
    int centerY = 12; // 태양 중심 Y 좌표
    int distance = 12; // 태양 중심으로부터 떨어진 거리
    int rectangleSize = 2; // 사각형의 반지름 (즉, 사각형의 한 변 길이는 4)

    // 사각형 중심 좌표 계산 (태양 중심에서 12만큼 떨어진 위치)
    int rectCenterX = centerX + distance;
    int rectCenterY = centerY;

    // 사각형의 네 꼭지점 좌표 계산 (회전 전)
    Vector3 p1 = { rectCenterX - rectangleSize, rectCenterY - rectangleSize, 1 };  // 왼쪽 아래
    Vector3 p2 = { rectCenterX + rectangleSize, rectCenterY - rectangleSize, 1 };  // 오른쪽 아래
    Vector3 p3 = { rectCenterX + rectangleSize, rectCenterY + rectangleSize, 1 };  // 오른쪽 위
    Vector3 p4 = { rectCenterX - rectangleSize, rectCenterY + rectangleSize, 1 };  // 왼쪽 위

    // 회전 행렬 계산
    Matrix3x3 rotation = rotation_matrix(sunAngle);  // sunAngle을 기준으로 회전
    p1 = multiply_matrix_vector(rotation, p1);
    p2 = multiply_matrix_vector(rotation, p2);
    p3 = multiply_matrix_vector(rotation, p3);
    p4 = multiply_matrix_vector(rotation, p4);

    // 회전된 사각형을 화면에 그리기 (직선으로 연결)
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // p1 -> p2
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // p2 -> p3
    DrawLine((int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y); // p3 -> p4
    DrawLine((int)p4.x, (int)p4.y, (int)p1.x, (int)p1.y); // p4 -> p1
}

// 게임 초기화 함수
void Init() {

    ClearScreen();

    //커서 숨기기
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = FALSE;  // 커서를 숨김
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// 화면을 그리는 함수
void Draw(char* Buffer) {
    //0,0으로 이동
    COORD coord = { 0, 0 };  // 좌표 (0, 0)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    // 각 픽셀을 두 문자씩 출력
    for (int i = 0; i < SB_HEIGHT; i++) {
        for (int j = 0; j < SB_WIDTH; j++) {
            // 2개의 문자 출력
            putchar(screenBuffer[(i * SB_WIDTH + j) * 2]);      // 첫 번째 문자
            putchar(screenBuffer[(i * SB_WIDTH + j) * 2 + 1]);  // 두 번째 문자
        }
        putchar('\n');  // 줄바꿈
    }
}

// 화면 업데이트 함수
void Update() {
    ClearScreen();
    DrawSun(sunAngle);
    DrawRectangle(sunAngle);

    sunAngle -= 10;
    if (sunAngle < 0) sunAngle += 2 * PI; // 360도 초과시 다시 0도로
}
// 게임 루프
int main() {
    // 게임 초기화
    Init();

    // 프레임 타이머
    clock_t lastTime = clock();
    int fps = 60;
    double frameTime = 1000.0 / fps;

    while (1) {
        // ESC 키를 눌렀는지 확인
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {  // ESC 키가 눌리면 게임 종료
                break;
            }
        }

        // 현재 시간 계산
        clock_t now = clock();
        double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC * 1000.0;

        // 프레임 제한 (60fps 유지)
        if (deltaTime >= frameTime) {
            lastTime = now;

            // 게임 업데이트
            Update();  //+ ClearScreen(); Render(angle); 사용 ok

            // 화면 그리기
            //Draw(clearBuffer);
            Draw(screenBuffer);
        }
    }

    return 0;
}