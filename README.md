========================================================================
2024-11-24
교수님 너무 어렵습니다... 살려주세요ㅠㅠ
# 멀티미디어공학과 게임제작 12주차 과제
콘솔창의 기본 해상도는 char형 문자 80*24 개로 구성되어있다.

스크린버퍼는 이 콘솔창에서 가로40 세로는 24픽셀로 하며 2개의 char 데이터형을 1개의 픽셀로 갖는 40*24 크기로 픽셀을 생성한다.

(ex> 문자열2개를 작성한 ‘00’ 을 픽셀 1칸이라고 생각하자.)

태양/지구/달 을 그릴 것이며, 모든 행성은 자전, 공전을 한다. (크기는 스크린버퍼의 1픽셀단위로 한다)

태양 (삼각형)
크기 : 중점에서 꼭지점까지의 거리 5
자전주기 : 반시계방향 3초당 1바퀴
공전주기 : 공전 안함
태양의 중점은 화면의 중간인 (20, 12) 과 같다.
‘SS’문자로 픽셀을 채우시오
지구 (정사각형)
크기 : 중점에서 꼭지점까지의 거리 3
자전주기 : 시계방향 2초당 1바퀴
공전주기 : 시계방향 3초당 1바퀴
태양의 중점으로부터 12만큼 떨어져있다.
‘EE’문자로 픽셀을 채우시오
달 (원)
크기 : 지름 3
자전주기 : 시계방향 1초당 1바퀴
공전주기 : 반시계방향 2초당 1바퀴
지구의 중점으로부터 4만큼 떨어져있다.
‘MM’문자로 픽셀을 채우시오
아무때나 ESC를 누르면 프로그램을 바로종료하도록 프로그래밍 하시오.

스페이스바를 누르기 전까진 화면 중앙에 학번과 이름이 뜨도록 프로그래밍하시오.

스페이스바를 누르면 행성들이 1자가 되도록 배치하세요.

한번 더 스페이스바를 누르면 자전과 공전을 시작하도록 하시오.

자전과 공전은 ESC를 누르기 전까지는 계속 동작하도록 하시오. (최소 5분간 프로그램이 종료되면 안됨)

이번주차 수업처럼 반드시 행성 하나당 ‘회전행렬, 이동행렬, 크기조정행렬’ 들을 조합한 행렬들을 곱해서 하나의 행렬을 만든 후, 좌표와 곱하는 방식으로 프로그래밍하시오.

수업시간에 배운 행렬의 형태를 사용하지 않고 프로그래밍 할 경우 0점처리 됩니다.

========================================================================
2024-11-08
# 멀티미디어공학과 게임제작 10주차 과제
콘솔창의 중앙에

가로 11 X 세로 5

사이즈의 안이 꽉찬 사각형을 3초에 1바퀴 돌도록 제작하시오

========================================================================
2024-11-02
# 멀티미디어공학과 게임제작 9주차 과제
struct vec3{ float x, float y, float w}

자료구조를 추가하고, // 코드 33줄

다음과 같은 함수를 구현하여

vec3 Mul(vec3 a, Matrix3x3 b); //코드 59줄

Matrix3x3 Mul(Matrix3x3 a, Matrix3x3 b); // 코드 69줄



행성의 회전을 다음과 유사한 형식으로 프로그래밍하시오. //코드 119줄

e = Mul(A, Mul(b,Mul(c,d))); 
========================================================================
