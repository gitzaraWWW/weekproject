//#include <stdio.h>
//#include <math.h>
//#include <windows.h>
//
//#define WIDTH 40 
//#define HEIGHT 20
//#define M_PI 3.14159265358979323846
//
//// 3x3 ��� ����
//typedef struct {
//    float m[3][3];
//} Matrix3x3;
//
////// 3x3 ��� �ʱ�ȭ
//void initIdentityMatrix(Matrix3x3* mat) {
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            mat->m[i][j] = (i == j) ? 1 : 0;
//        }
//    }
//}
//
//// ���� ����
//typedef struct {
//    float x, y, w; // x, y ��ǥ�� ���� ��ǥ w = 1
//} vec3;
//
//// �̵� ��� ����
//Matrix3x3 createTranslationMatrix(float tx, float ty) {
//    Matrix3x3 mat;
//    initIdentityMatrix(&mat);
//    mat.m[0][2] = tx; // X�� �̵�
//    mat.m[1][2] = ty; // Y�� �̵�
//    return mat;
//}
//
//// ȸ�� ��� ����
//Matrix3x3 createRotationMatrix(float angle) {
//    Matrix3x3 mat;
//    initIdentityMatrix(&mat);
//    float radian = angle * M_PI / 180;
//    mat.m[0][0] = cos(radian); // cos(��)
//    mat.m[0][1] = -sin(radian); // -sin(��)
//    mat.m[1][0] = sin(radian); // sin(��)
//    mat.m[1][1] = cos(radian); // cos(��)
//    return mat;
//}
//
//// ���Ϳ� ����� ����
//vec3 Mul(vec3 a, Matrix3x3 b) {
//    vec3 result;
//    result.x = a.x * b.m[0][0] + a.y * b.m[0][1];
//    result.y = a.x * b.m[1][0] + a.y * b.m[1][1];
//    result.w = 1.0f;
//    return result;
//}
//
//// ��� ����
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
//// �ܼ� ȭ�� �����
//void clearScreen() {
//    system("cls");
//}
//
//// ���簢�� ���
//void drawFR(int widthRect, int heightRect, float angle) {
//    clearScreen();
//    char screen[HEIGHT][WIDTH]; //ȭ���� 2d�迭�� ����
//
//    // ���簢���� ������ ���� ��ĭ���� �����
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            screen[y][x] = ' '; // ��� ȭ���� ��������
//        }
//    }
//
//    // ���簢�� ä���
//    for (float y = -heightRect / 2.0; y < heightRect / 2; y += 0.25) {
//        for (float x = -widthRect / 2.0; x < widthRect / 2; x += 0.25) {
//            vec3 fillVertex = { x, y, 1 }; //ȸ�� �Լ� ����ϱ�
//            vec3 rotatedFillVertex = Mul(fillVertex, createRotationMatrix(angle)); 
//            
//            // ȸ���� ���� ��ǥ�� ��ȯ�ϱ�
//            int fillX = (int)(rotatedFillVertex.x + WIDTH / 2);
//            int fillY = (int)(rotatedFillVertex.y + HEIGHT / 2);
//
//            // ȭ�� ���� ������ '#'�� ǥ��
//            if (fillX >= 0 && fillX < WIDTH && fillY >= 0 && fillY < HEIGHT) {
//                screen[fillY][fillX] = '#';
//            }
//        }
//    }
//
//    // ȭ�� ���
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            printf("%c", screen[y][x]);
//        }
//        printf("\n");
//    }
//}
//
//int main() {
//    // ���簢���� ũ�� ����
//    int widthRect = 11;  // ����
//    int heightRect = 5;  // ����
//
//    int angle = 0; // �ʱ� ������ 0����
//    while (1) {
//        drawFR(widthRect, heightRect, angle); // ȸ�� ������ ���� �׸���
//        Sleep(300);  // 0.3�� ���
//
//        // 360���� 3�ʿ� ������ 0.3�ʸ��� 36���� �����ؾ���
//        angle = (angle + 36) % 360;
//    }
//    return 0;
//}