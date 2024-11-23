//#include <stdio.h>
//#include <math.h>
//#include <windows.h>
//
//#define WIDTH 40
//#define HEIGHT 20
//
////#ifndef M_PI
//#define M_PI 3.14159265358979323846
////#endif
//// 33�� : struct vec3{ float x, float y, float w} �ڷᱸ���� �߰��ϰ�, 
//// 59�� : ������ ���� �Լ��� �����Ͽ� vec3 Mul(vec3 a, Matrix3x3 b)
//// 69�� : Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b);
//// 119�� : �༺�� ȸ���� ������ ������ �������� ���α׷����Ͻÿ�.e = Mul(A, Mul(b, Mul(c, d)));
//typedef struct {
//    float m[3][3]; // 3x3 ���
//} Matrix3x3;
//
//// 3x3 ��� �ʱ�ȭ
//void initIdentityMatrix(Matrix3x3* mat) {
//    for (int i = 0; i < 3; i++) {
//        for (int j = 0; j < 3; j++) {
//            if (i == j) {
//                mat->m[i][j] = 1;
//            }
//            else {
//                mat->m[i][j] = 0;
//            }
//        }
//    }
//}
//
////����
//typedef struct {
//    float x, y, w; // x, y ��ǥ�� ���� ��ǥ w = 1�����ؾ���
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
//// 1. vec3 Mul(vec3 a, Matrix3x3 b); �����ϱ� / ���Ϳ� ��� ���� 
//vec3 Mul(vec3 a, Matrix3x3 b) {
//    vec3 result;
//    result.x = a.x * b.m[0][0] + a.y * b.m[0][1]; // x ��ǥ ����ϱ�, w�� 1����
//    result.y = a.x * b.m[1][0] + a.y * b.m[1][1]; // y ��ǥ ����ϱ�, w�� 1����
//    result.w = 1.0f; // ���� ��ǥ�� �׻� 1�� �����ϱ�
//    return result;
//}
//
//
//// 2. Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b); �����ϱ� /��İ���
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
//// ���� ��ȯ
//void transformPoint(float* x, float* y, Matrix3x3 mat) {
//    float newX = mat.m[0][0] * *x + mat.m[0][1] * *y + mat.m[0][2];
//    float newY = mat.m[1][0] * *x + mat.m[1][1] * *y + mat.m[1][2];
//    *x = newX;
//    *y = newY;
//}
//
//// �ܼ� ȭ�� �����
//void clearScreen() {
//    system("cls");
//}
//
//
//// ȭ�� �׸���
//void draw(int angle) {
//    clearScreen();
//
//    // S(�¾�) ��ġ
//    int sunX = WIDTH / 2; 
//    int sunY = HEIGHT / 2;
//
//    // E(����)�� �ʱ� ��ġ
//    vec3 e = { 0.0f, 3.0f, 1.0f }; // ���� ��ǥ w�� 1�� ����;
//
//    // M(��)�� �ʱ� ��ġ
//    vec3 m = { 0.0f, 1.5f, 1.0f };  // ���� ��ǥ w�� 1�� ����;
//    
//    // ������ ȸ�� ���
//    Matrix3x3 rotationE = createRotationMatrix(-angle);
//   
//    // E�� ��ġ�� S�� ��ġ�� �̵���Ű�� ��ȯ ���
//    Matrix3x3 translationToP = createTranslationMatrix(sunX, sunY);
//
//    // O�� ��ġ ��ȯ
//    Matrix3x3 combinedE = Mul(rotationE, createTranslationMatrix(0, 3));
//    combinedE = Mul(translationToP, combinedE);
//    transformPoint(&e.x, &e.y, combinedE);
//
//    // M�� ��ġ ǥ��: M = Mul(A, Mul(B, Mul(C, D))) Ȱ���ϱ�
//    
//    //case 1)
//    Matrix3x3 rotationM = createRotationMatrix(-angle * 4); // ���� ȸ�� �ӵ�
//    Matrix3x3 translationFromE = createTranslationMatrix(m.x, 0); // ���� ��ġ (���������� ����� ��ġ)
//    Matrix3x3 rotationAroundE = createRotationMatrix(-angle); // ���� ������ ���� ȸ��
//
//    Matrix3x3 combinedM = Mul(rotationAroundE, Mul(translationFromE, rotationM));
//    
//    //case 2)
//    //Matrix3x3 rotationM = createRotationMatrix(-angle * 4);
//    //Matrix3x3 translationFromE = createTranslationMatrix(m.x, m.y);
//    //Matrix3x3 combinedM = Mul(rotationM, translationFromE);
//    
//    transformPoint(&m.x, &m.y, combinedM);
//    
//    // M�� ��ġ�� O�� ���Ͽ� ���� ��ġ ���
//    m.x += e.x;
//    m.y += e.y;
//
//
//    // ȭ�� �׸���
//    for (int y = 0; y < HEIGHT; y++) {
//        for (int x = 0; x < WIDTH; x++) {
//            if (x == sunX && y == sunY) {
//                printf("S"); // P ���
//            }
//            else if ((int)e.x == x && (int)e.y == y) {
//                printf("E"); // O ���
//            }
//            else if ((int)m.x == x && (int)m.y == y) {
//                printf("M"); // M ���
//            }
//            else {
//                printf(" "); // �� ���� ���
//            }
//        }
//        printf("\n");
//    }
//}
//
//
//int main() {
//    for (int angle = 0; angle < 720; angle += 5) {
//        draw(angle); // ȸ�� ������ ���� �׸���
//        Sleep(100); // 0.1�� ���
//    }
//    return 0;
//}