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
float eAngle = 0.0f;  // 지구의 자전 각도
float earthAngle = 0.0f;  // 지구의 공전 각도
float mAngle = 0.0f;  // 달의 자전 각도
float moonAngle = 0.0f;  // 달의 공전 각도

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

// 지구를 그리는 함수
void DrawEarth(float earthAngle, float eAngle, float deltaTime) {
    int earthSize = 2;  // 지구 크기 (중점에서 꼭지점까지의 거리)

    // 자전 속도 (2초당 1바퀴, 즉 360도 = 2 / PI 라디안)
    float rotationSpeed = PI / 2;

    // 지구의 중심 좌표 (태양을 중심으로 공전)
    int centerX = 20 + 12 * cosf(earthAngle);
    int centerY = 12 + 12 * sinf(earthAngle);

    // 지구 자전 각도 업데이트 (시계방향)
    eAngle -= rotationSpeed * deltaTime;  // 시계방향으로 회전 (각도 감소)

    // 회전 행렬을 이용해 자전 각도 적용
    Matrix3x3 rotation = rotation_matrix(eAngle);  // 지구 자전 각도에 대한 회전 행렬 생성

    // 지구의 네모 모양을 그리기 전에 회전 wjs
    Vector3 p1 = { centerX - earthSize, centerY - earthSize, 1 };  // 왼쪽 상단
    Vector3 p2 = { centerX + earthSize, centerY - earthSize, 1 };  // 오른쪽 상단
    Vector3 p3 = { centerX + earthSize, centerY + earthSize, 1 };  // 오른쪽 하단
    Vector3 p4 = { centerX - earthSize, centerY + earthSize, 1 };  // 왼쪽 하단

    // 회전된 점들 계산
    p1 = multiply_matrix_vector(rotation, p1);
    p2 = multiply_matrix_vector(rotation, p2);
    p3 = multiply_matrix_vector(rotation, p3);
    p4 = multiply_matrix_vector(rotation, p4);

    // 네모 모양 그리기
    DrawLine((int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y); // 위
    DrawLine((int)p2.x, (int)p2.y, (int)p3.x, (int)p3.y); // 오른쪽
    DrawLine((int)p3.x, (int)p3.y, (int)p4.x, (int)p4.y); // 아래
    DrawLine((int)p4.x, (int)p4.y, (int)p1.x, (int)p1.y); // 왼쪽

    // 내부 채우기
    for (int y = (int)fmin(fmin(p1.y, p2.y), fmin(p3.y, p4.y)); y <= (int)fmax(fmax(p1.y, p2.y), fmax(p3.y, p4.y)); y++) {
        for (int x = (int)fmin(fmin(p1.x, p2.x), fmin(p3.x, p4.x)); x <= (int)fmax(fmax(p1.x, p2.x), fmax(p3.x, p4.x)); x++) {
            if (x >= 0 && x < SB_WIDTH && y >= 0 && y < SB_HEIGHT) {
                screenBuffer[(y * SB_WIDTH + x) * 2] = 'E';  // 지구 내부 채우기
                screenBuffer[(y * SB_WIDTH + x) * 2 + 1] = 'E';
            }
        }
    }
}

// 달 그리기
void DrawMoon(float moonAngle, float earthAngle, float deltaTime) {
    int moonSize = 1;  // 달 크기 (지름이 3, 반지름 1.5 반내림해서 1로 표현)

    // 지구의 중심 좌표 계산 (달은 지구 주위를 공전)
    int earthCenterX = 20 + 12 * cosf(earthAngle);
    int earthCenterY = 12 + 12 * sinf(earthAngle);

    // 달의 위치는 지구로부터 떨어진 거리 4만큼
    int moonCenterX = earthCenterX + 4 * cosf(moonAngle);
    int moonCenterY = earthCenterY + 4 * sinf(moonAngle);

    // 달의 자전 주기: 1초당 1바퀴 (시계방향)
    float moonRotationSpeed = 2 * PI;  // 1초에 1바퀴 회전 (시계방향)

    // 자전 각도 업데이트 (시계방향)
    moonAngle -= moonRotationSpeed * deltaTime;  // 각도 감소 (시계방향 회전)

    // 원을 그리기 위해 여러 점을 계산하고 선으로 연결 (360도 동안 점을 찍어 연결)
    int numPoints = 360;  // 원을 그리기 위한 점의 개수 (정밀도를 조정할 수 있음)
    for (int i = 0; i < numPoints; i++) {
        float angle1 = i * 2 * PI / numPoints;  // 첫 번째 각도
        float angle2 = (i + 1) * 2 * PI / numPoints;  // 두 번째 각도

        // 첫 번째 점 (각도1을 기준으로 계산)
        int x1 = moonCenterX + moonSize * cosf(angle1);
        int y1 = moonCenterY + moonSize * sinf(angle1);

        // 두 번째 점 (각도2를 기준으로 계산)
        int x2 = moonCenterX + moonSize * cosf(angle2);
        int y2 = moonCenterY + moonSize * sinf(angle2);

        // 두 점을 연결하는 직선을 그린다 (DrawLine 함수 사용)
        DrawLine(x1, y1, x2, y2);
    }

    // 원 내부 채우기
    for (int dx = -moonSize; dx <= moonSize; dx++) {
        for (int dy = -moonSize; dy <= moonSize; dy++) {
            if (dx * dx + dy * dy <= moonSize * moonSize) {
                int fillX = moonCenterX + dx;
                int fillY = moonCenterY + dy;

                // 화면 범위 내에서만 그리기
                if (fillX >= 0 && fillX < SB_WIDTH && fillY >= 0 && fillY < SB_HEIGHT) {
                    screenBuffer[(fillY * SB_WIDTH + fillX) * 2] = 'M';  // 달 내부 채우기
                    screenBuffer[(fillY * SB_WIDTH + fillX) * 2 + 1] = 'M';
                }
            }
        }
    }
}

// 태양, 지구, 달을 1자 배치
void DrawSunEarthMoon() {
    int centerX = 20; // 태양 중심 X 좌표
    int centerY = 12; // 태양 중심 Y 좌표

    // 태양 그리기
    DrawSun(0.0f);  // 태양 그리기 (회전하지 않음)

    // 지구 그리기 (위치를 선형으로 배치)
    int earthX = centerX + 12;  // 지구 X 좌표
    int earthY = centerY;       // 지구 Y 좌표
    DrawEarth(0.0f, 0.0f, 1.0f);  // 지구 그리기

    // 달 그리기
    int moonX = earthX + 4;     // 달 X 좌표
    int moonY = earthY;         // 달 Y 좌표
    DrawMoon(0.0f, 0.0f, 1.0f);  // 달 그리기
}

// 커서의 위치를 설정하는 함수(printf를 중앙에 찍자)
void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;  // X 좌표 (가로)
    coord.Y = y;  // Y 좌표 (세로)
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 화면에 텍스트를 중앙에 출력하는 함수
void printCenteredText(const char* text) {
    int textLength = strlen(text);
    int startX = (Console_WIDTH - textLength) / 2;
    int startY = Console_HEIGHT / 2;

    gotoXY(startX, startY);  // 커서 위치를 중앙으로 이동
    printf("%s", text);      // 중앙에 텍스트 출력
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

// 회전 속도 조절을 위한 변수 추가
float sunRotationSpeed = -2 * PI * 20.0;   // (자전) 태양은 3초에 1바퀴 반시계
float earthRotationSpeed = 2 * PI / 3.0f;  // (공전) 지구는 3초에 1바퀴 시계
float moonRotationSpeed = -2 * PI / 2.0f;   //(공전) 달은 2초에 1바퀴 반시계

clock_t lastTime;
// Update 함수에서 회전 각도 업데이트 부분 수정
void Update() {
    // deltaTime 계산 (현재 시간과 이전 시간의 차이)
    clock_t now = clock();
    double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC;  // deltaTime을 초 단위로 계산
    lastTime = now;

    // 각도 업데이트 : deltaTime을 고려해서 속도 조절
    sunAngle += sunRotationSpeed * deltaTime;
    earthAngle += earthRotationSpeed * deltaTime;
    moonAngle += moonRotationSpeed * deltaTime;

    // 360도를 넘으면 0도로 리셋
    if (sunAngle >= 2 * PI) sunAngle -= 2 * PI;
    if (earthAngle >= 2 * PI) earthAngle -= 2 * PI;
    if (moonAngle >= 2 * PI) moonAngle -= 2 * PI;

    ClearScreen();
    DrawSun(sunAngle);
    DrawEarth(earthAngle,eAngle,deltaTime);
    DrawMoon(moonAngle, earthAngle, deltaTime);
}

// 게임 루프
int main() {
    // 게임 초기화
    Init();

    // 프레임 타이머
    enum GameState { INTRO, LINEAR, ROTATE } gameState = INTRO; // 상태: INTRO (초기 화면), LINEAR (1자 배치), ROTATE (회전 시작)
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

            if (ch == ' ') { // 스페이스바가 눌리면 상태 변경
                if (gameState == INTRO) {
                    gameState = LINEAR; // 첫 번째 스페이스바: 1자 배치 상태로 변경
                }
                else if (gameState == LINEAR) {
                    gameState = ROTATE; // 두 번째 스페이스바: 자전/공전 시작 상태로 변경
                }
            }
        }

        // 현재 시간 계산
        clock_t now = clock();
        double deltaTime = (double)(now - lastTime) / CLOCKS_PER_SEC * 1000.0;

        // 프레임 제한 (60fps 유지)
        if (deltaTime >= frameTime) {
            lastTime = now;

            if (gameState == INTRO) {
                printCenteredText("20211413 심규호");
            }
            else if (gameState == LINEAR) {
                // 두 번째 스페이스바 입력 후, 행성들을 1자로 배치
                DrawSunEarthMoon();

            }
            else if (gameState == ROTATE) {
                // 세 번째 스페이스바 입력 후, 자전과 공전 시작
                Update();
            }

            // 화면 그리기
            Draw(screenBuffer);
        }
    }


    return 0;
}
