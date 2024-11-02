#include <stdio.h>
#include <math.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

//#ifndef M_PI
#define M_PI 3.14159265358979323846
//#endif
// 33줄 : struct vec3{ float x, float y, float w} 자료구조를 추가하고, 
// 59줄 : 다음과 같은 함수를 구현하여 vec3 Mul(vec3 a, Matrix3x3 b)
// 69줄 : Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b);
// 119줄 : 행성의 회전을 다음과 유사한 형식으로 프로그래밍하시오.e = Mul(A, Mul(b, Mul(c, d)));
typedef struct {
    float m[3][3]; // 3x3 행렬
} Matrix3x3;

// 3x3 행렬 초기화
void initIdentityMatrix(Matrix3x3* mat) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j) {
                mat->m[i][j] = 1;
            }
            else {
                mat->m[i][j] = 0;
            }
        }
    }
}

//벡터
typedef struct {
    float x, y, w; // x, y 좌표와 동차 좌표 w = 1고정해야함
} vec3;

// 이동 행렬 생성
Matrix3x3 createTranslationMatrix(float tx, float ty) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    mat.m[0][2] = tx; // X축 이동
    mat.m[1][2] = ty; // Y축 이동
    return mat;
}

// 회전 행렬 생성
Matrix3x3 createRotationMatrix(float angle) {
    Matrix3x3 mat;
    initIdentityMatrix(&mat);
    float radian = angle * M_PI / 180;
    mat.m[0][0] = cos(radian); // cos(θ)
    mat.m[0][1] = -sin(radian); // -sin(θ)
    mat.m[1][0] = sin(radian); // sin(θ)
    mat.m[1][1] = cos(radian); // cos(θ)
    return mat;
}

// 1. vec3 Mul(vec3 a, Matrix3x3 b); 구현하기 / 벡터와 행렬 곱셈 
vec3 Mul(vec3 a, Matrix3x3 b) {
    vec3 result;
    result.x = a.x * b.m[0][0] + a.y * b.m[0][1]; // x 좌표 계산하기, w는 1고정
    result.y = a.x * b.m[1][0] + a.y * b.m[1][1]; // y 좌표 계산하기, w는 1고정
    result.w = 1.0f; // 동차 좌표는 항상 1로 고정하기
    return result;
}


// 2. Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b); 구현하기 /행렬곱셈
Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b) {
    Matrix3x3 result;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j];
        }
    }
    return result;
}

// 점을 변환
void transformPoint(float* x, float* y, Matrix3x3 mat) {
    float newX = mat.m[0][0] * *x + mat.m[0][1] * *y + mat.m[0][2];
    float newY = mat.m[1][0] * *x + mat.m[1][1] * *y + mat.m[1][2];
    *x = newX;
    *y = newY;
}

// 콘솔 화면 지우기
void clearScreen() {
    system("cls");
}


// 화면 그리기
void draw(int angle) {
    clearScreen();

    // S(태양) 위치
    int sunX = WIDTH / 2; 
    int sunY = HEIGHT / 2;

    // E(지구)의 초기 위치
    vec3 e = { 0.0f, 3.0f, 1.0f }; // 동차 좌표 w를 1로 설정;

    // M(달)의 초기 위치
    vec3 m = { 0.0f, 1.5f, 1.0f };  // 동차 좌표 w를 1로 설정;
    
    // 지구의 회전 행렬
    Matrix3x3 rotationE = createRotationMatrix(-angle);
   
    // E의 위치를 S의 위치로 이동시키는 변환 행렬
    Matrix3x3 translationToP = createTranslationMatrix(sunX, sunY);

    // O의 위치 변환
    Matrix3x3 combinedE = Mul(rotationE, createTranslationMatrix(0, 3));
    combinedE = Mul(translationToP, combinedE);
    transformPoint(&e.x, &e.y, combinedE);

    // M의 위치 표현: M = Mul(A, Mul(B, Mul(C, D))) 활용하기
    
    //case 1)
    Matrix3x3 rotationM = createRotationMatrix(-angle * 4); // 달의 회전 속도
    Matrix3x3 translationFromE = createTranslationMatrix(m.x, 0); // 달의 위치 (지구에서의 상대적 위치)
    Matrix3x3 rotationAroundE = createRotationMatrix(-angle); // 지구 주위를 도는 회전

    Matrix3x3 combinedM = Mul(rotationAroundE, Mul(translationFromE, rotationM));
    
    //case 2)
    //Matrix3x3 rotationM = createRotationMatrix(-angle * 4);
    //Matrix3x3 translationFromE = createTranslationMatrix(m.x, m.y);
    //Matrix3x3 combinedM = Mul(rotationM, translationFromE);
    
    transformPoint(&m.x, &m.y, combinedM);
    
    // M의 위치를 O에 더하여 최종 위치 계산
    m.x += e.x;
    m.y += e.y;


    // 화면 그리기
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == sunX && y == sunY) {
                printf("S"); // P 출력
            }
            else if ((int)e.x == x && (int)e.y == y) {
                printf("E"); // O 출력
            }
            else if ((int)m.x == x && (int)m.y == y) {
                printf("M"); // M 출력
            }
            else {
                printf(" "); // 빈 공간 출력
            }
        }
        printf("\n");
    }
}


int main() {
    for (int angle = 0; angle < 720; angle += 5) {
        draw(angle); // 회전 각도에 따라 그리기
        Sleep(100); // 0.1초 대기
    }
    return 0;
}