//#include <stdio.h>
//#include <math.h>
//#include <windows.h>
//
//#define WIDTH 40 
//#define HEIGHT 20
//#define M_PI 3.14159265358979323846
//
//// 3x3 행렬 정의
//typedef struct {
//    float m[3][3];
//} Matrix3x3;
//
////// 3x3 행렬 초기화
//void initIdentityMatrix(Matrix3x3* mat) {
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            mat->m[i][j] = (i == j) ? 1 : 0;
//        }
//    }
//}
//
//// 벡터 정의
//typedef struct {
//    float x, y, w; // x, y 좌표와 동차 좌표 w = 1
//} vec3;
//
//// 이동 행렬 생성
//Matrix3x3 createTranslationMatrix(float tx, float ty) {
//    Matrix3x3 mat;
//    initIdentityMatrix(&mat);
//    mat.m[0][2] = tx; // X축 이동
//    mat.m[1][2] = ty; // Y축 이동
//    return mat;
//}
//
//// 회전 행렬 생성
//Matrix3x3 createRotationMatrix(float angle) {
//    Matrix3x3 mat;
//    initIdentityMatrix(&mat);
//    float radian = angle * M_PI / 180;
//    mat.m[0][0] = cos(radian); // cos(θ)
//    mat.m[0][1] = -sin(radian); // -sin(θ)
//    mat.m[1][0] = sin(radian); // sin(θ)
//    mat.m[1][1] = cos(radian); // cos(θ)
//    return mat;
//}
//
//// 벡터와 행렬의 곱셈
//vec3 Mul(vec3 a, Matrix3x3 b) {
//    vec3 result;
//    result.x = a.x * b.m[0][0] + a.y * b.m[0][1];
//    result.y = a.x * b.m[1][0] + a.y * b.m[1][1];
//    result.w = 1.0f;
//    return result;
//}
//
//// 행렬 곱셈
//Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b) {
//    Matrix3x3 result;
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
//        }
//    }
//    return result;
//}
//
//// 콘솔 화면 지우기
//void clearScreen() {
//    system("cls");
//}
//
//// 직사각형 출력
//void drawFR(int widthRect, int heightRect, float angle) {
//    clearScreen();
//    char screen[HEIGHT][WIDTH]; //화면을 2d배열로 정의
//
//    // 직사각형을 제외한 곳을 빈칸으로 만들기
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            screen[y][x] = ' '; // 모든 화면을 공백으로
//        }
//    }
//
//    // 직사각형 채우기
//    for (float y = -heightRect / 2.0; y < heightRect / 2; y += 0.25) {
//        for (float x = -widthRect / 2.0; x < widthRect / 2; x += 0.25) {
//            vec3 fillVertex = { x, y, 1 }; //회전 함수 사용하기
//            vec3 rotatedFillVertex = Mul(fillVertex, createRotationMatrix(angle)); 
//            
//            // 회전된 점을 좌표로 변환하기
//            int fillX = (int)(rotatedFillVertex.x + WIDTH / 2);
//            int fillY = (int)(rotatedFillVertex.y + HEIGHT / 2);
//
//            // 화면 내에 있으면 '#'로 표시
//            if (fillX >= 0 && fillX < WIDTH && fillY >= 0 && fillY < HEIGHT) {
//                screen[fillY][fillX] = '#';
//            }
//        }
//    }
//
//    // 화면 출력
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            printf("%c", screen[y][x]);
//        }
//        printf("\n");
//    }
//}
//
//int main() {
//    // 직사각형의 크기 지정
//    int widthRect = 11;  // 가로
//    int heightRect = 5;  // 세로
//
//    int angle = 0; // 초기 각도는 0으로
//    while (1) {
//        drawFR(widthRect, heightRect, angle); // 회전 각도에 따라 그리기
//        Sleep(300);  // 0.3초 대기
//
//        // 360도를 3초에 돌려면 0.3초마다 36도씩 증가해야함
//        angle = (angle + 36) % 360;
//    }
//    return 0;
//}