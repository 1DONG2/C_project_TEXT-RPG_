#include <stdio.h>
#include <String>
#include <windows.h>

#include "StatData.h" // 내가 짜놓은 함수 헤더파일...

// 프로그램 번호 이름 (순서대로 1 ~ 4)
enum { HUNT = 1, SHOP, STAT, EXIT }; 

void mainGame() {
	while (1) {
		showMenu(); // 선택 메뉴 내용 출력.
		int choice; // case 문에서 쓰일 선택 변수 선언.
		printf("\n~~~메뉴를 선택하세요~~~\n");
		printf("Please select a menu >> ");
		scanf_s("%d", &choice);
		fseek(stdin, 0, SEEK_END);
		system("cls"); // 화면 지우기.

		switch (choice) {
		case HUNT:
			hunt(); break;
		case SHOP:
			printf("1. 소  비\t 2. 무  기\n3. 퇴  장\n ");
			printf("무엇을 사시겠습니까? >> ");
			scanf_s("%d", &choice);
			switch (choice) {
			case 1:
				potionBuy(); break;
			case 2:
				weaponBuy(); break;
			case 3:
				break;
			}
			break;
		case STAT:
			status();
			system("pause");
			system("cls"); break;
		case EXIT:
			userSave();
			printf("게임을 종료합니다. \n");
			return; // while 무한루프 탈출 및 프로그램 종료
		default:
			printf("잘못된 입력입니다. \n");
			printf("다시 입력해주시기 바랍니다. \n"); break;
		}
	}
}

int main()
{
	char wel[9] = { 'W', 'E', 'L', 'C', 'O', 'M', 'E', '!' };
	// 상단에 띄울 프로그램명 및 크기 설정
	system("mode con cols=65 lines=30 | title TEXT_RPG.");
	
	gameLogo(); // 텍스트 알피지 아스키아트.
	
	switch (startMenu()) {
	case 0: // 스타트 메뉴에서 시작하기를 눌렀을 경우.
		system("cls");
		nameSet(); // 이름을 설정합니다.
		prologue(); // 프롤로그
		itemListLoad(); // 아이템 리스트 로드.
		mainGame();
		break;
	case 1: // 스타트 메뉴에서 불러오기를 눌렀을 경우.
		system("cls");
		userLoad(); // 세이브 파일을 불러옴.
		Sleep(1000); // 1초 정지
		for (int i = 0; i < strlen(wel); i++) {
			printf("%c\t", wel[i]);
			Sleep(150);
		}
		mainGame();
		break;
	case 2: // 스타트 메뉴에서 제작정보를 눌렀을 경우.
		system("cls");
		printf("\n\n\t군산대학교 소프트웨어융합공학과\n");
		printf("\t2021 1학년 2학기 프로그래밍및실습(2)\n");
		printf("\t팀프로젝트 과제물입니다.\n\n");
		printf("\t프로그래밍, 기획 : 1901619 한동엽\n");
		printf("\t테스터, 스토리, 보조 : 2101389 이재영\n\n");
		printf("\t2021.11.\n\n");
		printf("● 제작노트\n https://www.notion.so/1dong2/2-C-9c34428fe9a9466fb38ac1f737084dca \n\n");
		system("pause");
	case 3: // 스타트 메뉴에서 게임종료를 눌렀을 경우.
		system("cls");
		printf("\n\n또 이용해주시기 바랍니다! :D\n\n");
		return 0;
	}
}