#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "StatData.h" // 구조체, 함수 원형 선언된 헤더파일

bool eventExection[5] = { false }; // 이벤트 분기점 거짓으로 초기화
char text[1000]; // 텍스트 보여주는 변수

void prologue() { // 프롤로그 보여주는 함수.
	system("cls");
	FILE* f = 0; // 파일 포인터 함수.
	fopen_s(&f, "story\\prologue.txt", "r"); // 파일을 열자.
	if (f == NULL) // 게임 데이터에 문제가 있을 시...
		printf("이벤트 파일이 존재하지 않습니다!.\n\n");
	printf("\t프롤로그 ~~ 서장\n");
	while (1) {
		fgets(text, sizeof(text), f);
		// 특정 문자열을 찾아서. 플레이어의 이름을 출력한다.
		// 플레이어 이름은 구조체 전역변수로 선언되어있다. (헤더파일 참조)
		if (strstr(text, "ㅤ")) printf("%s", player.name);
		if (feof(f)) break; // 파일이 끝날때 까지 읽음.
		for (int i = 0; i < strlen(text); i++) {
			printf("%c", text[i]);
			Sleep(50);
		}
		printf("\n");
	}
	system("pause");
	system("cls");
	fclose(f);
	printf("처음 플레이 하신 것을 환영합니다!\n");
}

void chapterLoad(int t) { // 챕터 몇 장을 볼지 정해서 보여주는 함수.
	system("cls");
	FILE* f = 0; // 파일 포인터 함수.
	if(t == 1) // 레벨 5
		fopen_s(&f, "story\\chap1.txt", "r"); // 파일을 열자.
	if (t == 2) // 레벨 10
		fopen_s(&f, "story\\chap2.txt", "r"); // 파일을 열자.
	if (t == 3) // 레벨 15
		fopen_s(&f, "story\\chap3.txt", "r"); // 파일을 열자.
	if (t == 4) // 레벨 20
		fopen_s(&f, "story\\chap4.txt", "r"); // 파일을 열자.
	if (t == 5) // 레벨 25
		fopen_s(&f, "story\\chap5.txt", "r"); // 파일을 열자.
	if (t == 6) { // 레벨 30 (보스 몬스터 출현)
		fopen_s(&f, "story\\chap6.txt", "r"); // 파일을 열자.
		if (f == NULL) // 게임 데이터에 문제가 있을 시...
			printf("이벤트 파일이 존재하지 않습니다!.\n\n");
		printf("\t-- CHAPTER %d --\n", t);
		while (1) {
			fgets(text, sizeof(text), f);
			if (feof(f)) break; // 파일이 끝날때 까지 읽음.
			// 특정 문자열을 찾아서. 플레이어의 이름을 출력한다.
			// 플레이어 이름은 구조체 전역변수로 선언되어있다. (헤더파일 참조)
			if (strstr(text, "ㅤ")) printf("%s", player.name);
			for (int i = 0; i < strlen(text); i++) {
				printf("%c", text[i]);
				Sleep(50);
			}
			printf("\n");
		}
		system("pause");
		system("cls");
		bool vic = bossHunt(); // 보스 몬스터와 맞짱뜨기
		if (vic) { // 보스전에서 이겼다면?
			fopen_s(&f, "story\\happy_end.txt", "r"); // 파일을 열자.
		}
		else { // 졌으면.
			fopen_s(&f, "story\\bad_end.txt", "r"); // 파일을 열자.
		}
	}
	if (f == NULL) // 게임 데이터에 문제가 있을 시...
		printf("이벤트 파일이 존재하지 않습니다!.\n\n");
	printf("\t-- CHAPTER %d --\n", t);
	while (1) {
		fgets(text, sizeof(text), f);
		if (feof(f)) break; // 파일이 끝날때 까지 읽음.
		// 특정 문자열을 찾아서. 플레이어의 이름을 출력한다.
		// 플레이어 이름은 구조체 전역변수로 선언되어있다. (헤더파일 참조)
		if (strstr(text, "ㅤ")) printf("%s", player.name);
		for (int i = 0; i < strlen(text); i++) {
			printf("%c", text[i]);		
			Sleep(50);
		}
		printf("\n");
	}
	system("pause");
	system("cls");
	fclose(f);
	printf("CHAPTER %d장을 완료하였습니다.\n", t);
}

void Quiz(int level) {
	int choice;
	if (level == 5) {
		while (1) {
			printf("다음 중 출력에 사용되는 명령어는?\n 1. printf\t2. scanf\n 3. int\t\t4. double \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 1) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	if (level == 10)
	{
		while (1) {
			printf("다음 중 반복문에 사용되는 명령어는?\n 1. if\t2. for\n 3. double\t\t4. int \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 2) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	if (level == 15)
	{
		while (1) {
			printf("다음 중 프로그래밍언어및실습 교수님 성함은?\n 1. 장현준\t2. 정현준\n 3. 정한준\t\t4. 정혁준 \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 2) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	if (level == 20)
	{
		while (1) {
			printf("다음 중 가상현실, 증강현실, 사물인터넷 등 ICT기술과 결합해 현실감을 극대화하는 기술은?\n 1. 멀티버스\t2. 시내버스\n 3. 메타버스\t\t4. 밀리버스 \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 3) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	if (level == 25)
	{
		while (1) {
			printf("다음 중 컴퓨터가 이해할 수 있는 바이트와의 매핑 규칙은? \n 1. 인코딩\t2. XML\n 3. 그로스해킹\t\t4. 해킹 \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 1) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	if (level == 30)
	{
		while (1) {
			printf("다음 중 사용자 디바이스의 파일을 암호화하여 암호를 해제하기 위해서 금전적인 보상을 요구하는 범죄는?\n 1. 스미싱\t2. 파밍\n 3. 리팩토링\t\t4. 랜섬웨어 \n\n");
			printf("정답을 입력해주세요!>> ");
			scanf_s("%d", &choice);
			if (choice == 4) { // 답을 입력하면.
				printf("정답입니다!\n");
				break;
			} // 그 외의 입력들은.
			else printf("오답입니다. 다시 한번 생각해보세요.\n\n");
		}
	}
	system("pause");
	system("cls");
}