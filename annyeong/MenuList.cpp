#include <stdio.h>
#include <iostream>
#include <String>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h> // _getch() 사용을 위해...

#include "StatData.h" // 구조체, 함수 원형 선언된 헤더파일
#define WEAPON "weapon.txt" // 소스파일 내 무기데이터
#define USERDATA "userdata.txt" // 유저데이터 파일 (게임세이브 파일)
#define POTION "potion.txt" // 소스파일 내 포션데이터
#define MONSTER "monster_data.txt" // 소스파일 내 몬스터데이터

#define WEAPONSIZE 5 // 게임에 있는 무기 개수 (상시 수정해야 함)
#define POTIONSIZE 4 // 게임에 있는 포션 개수 (상시 수정해야 함)
#define MONSTERSIZE 11 // 몬스터 개체 수 (상시 수정해야 함)

People player; // 구조체 플레이어 변수 선언.
Enemy monsterSelect[MONSTERSIZE]; // 몬스터 변수 선언. (차후 배열로 변경될 수 있음) 
Item weapon[WEAPONSIZE]; // 게임 내 무기 구조체 변수
Item potion[POTIONSIZE]; // 게임 내 포션 구조체 변수

// 방향키를 눌렀을 시... 나오는 값.
#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80 
#define SPACE 32
#define ENTER 13

// 전투 시스템 선택번호 (순서대로 1 ~ 4)
enum { AT = 1, CHAT, ITEM, RUN };

// 몬스터 포인터 전역변수 선언
static Enemy* monster = monsterSelect;


int random(int set) { // 매개변수에 아무것도 입력하지 않으면 기본값은 10이다.
	srand(time(NULL));
	int random = (rand() % set) + 1; // 1 ~ 10 난수 변수, 게임의 확률을 정한다.
	return random;
}
int lostMoney() {
	// 난수 1 ~ 10을 10을 곱하고 플레이어 레벨을 곱해 
	// 점점 레벨이 오를 수록 더 폭이 커진다.
	int lostCost = random() * 10 * player.level;
	player.balance -= lostCost; // 잃은비용을 플레이어 자금에서 뺀다.
	return lostCost;
}
int getMoney() {
	// 난수 1 ~ 10을 10을 곱하고 플레이어 레벨을 곱해 
	// 점점 레벨이 오를 수록 더 폭이 커진다.
	int getCost = random() * 10 * player.level;
	player.balance += getCost; // 획득비용을 플레이어 자금에서 더한다.
	return getCost;
}
int getExp() {
	// 난수 1 ~ 10을 10을 곱하고 플레이어 레벨을 곱해 
	// 점점 레벨이 오를 수록 더 폭이 커진다.
	int getExp = random() * 10 * player.level;
	player.exp += getExp; // 잃은비용을 플레이어 자금에서 뺀다.
	return getExp;
}
int playerFight() {
	// 플레이어의 공격력에서 몬스터 방어력 * 0.2을 뺀 데미지 값을 준다.
	int damage = player.attack - (monster->defense * 0.2);
	if (damage < 0) damage = 0; // 데미지가 0보다 작으면 0으로 만들어줌. (방어력이 상대방보다 압도적이여서 그럼)
	monster->health -= damage; // 몬스터 체력에서 데미지 뺀다.
	return damage;
}
int monsterFight() {
	// 플레이어의 공격력에서 몬스터 방어력 * 0.2을 뺀 데미지 값을 준다.
	int damage = monster->attack - (player.defense * 0.2);
	if (damage < 0) damage = 0; // 데미지가 0보다 작으면 0으로 만들어줌.
	player.health -= damage; // 몬스터 체력에서 데미지 뺀다.
	return damage;
}
int keyControl() { // 입력한 키 값을 가져와 주는 함수.
	char temp = _getch();
	if (temp == -32) { // 224가 입력되면?
		temp = _getch(); // 다시 한 번 읽는다.
		return temp;
	}
}
int startMenu() { // 스타트 메뉴 출력. 메뉴 선택 시 0 ~ 3 숫자 반환.
	int x = 25, y = 12;
	gotoxy(x, y); // -2 는 ▶ 때문이다.
	printf("처 음 시 작"); // 0번
	gotoxy(x, y + 1);
	printf("불 러 오 기"); // 1번
	gotoxy(x, y + 2);
	printf("제 작 정 보"); // 2번
	gotoxy(x, y + 3);
	printf("게 임 종 료"); // 3번
	while (1) {
		int n = keyControl(); // 키보드 누름값을 받아오기
		switch (n) {
		case UP: // 위쪽 눌렀을 경우
			if (y > 12) {
				gotoxy(x - 3, y); // -2 는 ▶ 때문이다.
				printf("  "); // 화살표 지우기
				gotoxy(x - 3, --y); // 아래로 이동
				printf("▶"); // 다시 그리기
			}
			break;
		case DOWN: // 아래키 눌렀을 경우
			if (y < 15) {
				gotoxy(x - 3, y);
				printf("  ");
				gotoxy(x - 3, ++y);
				printf("▶");
			}
			break;
		case SPACE:
			return y - 12;
		case ENTER:
			return y - 12;
		}
	}
}
void gotoxy(int x, int y) { // 커서 위치 이동함수
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // 콘솔 핸들 가져오기
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(consoleHandle, pos);
}
void colorText(int colorSet) {
	/*
		검정 0
	    어두운 파랑 1
		어두운 초록 2
		어두운 하늘 3
		어두운 빨강 4
		어두운 보라 5
		어두운 노랑 6
		회색 7
		어두운 회색 8
		파랑 9
		초록 10
		하늘 11
		빨강 12
		보라 13
		노랑 14
		하양 15												
	*/	 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorSet);
}
void gameLogo() { // 예 말 그대로 로고입니다. (아스키 아트라고 카더라)
	colorText(1);
	printf("\n ######   ######   ##  ##   ######     #####    #####     #####\n");
	printf("   ##     ##       ##  ##     ##       ##  ##   ##  ##   ##    \n");
	colorText(3);
	printf("   ##     #####     ####      ##       ##  ##   ##  ##   ##    \n");	
	printf("   ##     ##        ####      ##       #####    #####    ## ###\n");
	colorText(11);
	printf("   ##     ##       ##  ##     ##       ## ##    ##       ##  ##\n");
	printf("   ##     ######   ##  ##     ##       ##  ##   ##        #####\n\n");
	printf("=================================================================");
	colorText(15);
}

void nameSet() {
	printf("환영합니다. 용사님! \n");
	printf("당신의 이름은 무엇입니까? >> ");
	//scanf_s는 문자열을 입력받을 때 뒤에 배열 사이즈를 입력해줘야 함.
	scanf_s("%s", player.name, sizeof(player.name)); //sizeof(player.name) = 30
	printf("반갑습니다. %s님. \n", player.name);
	Sleep(500); // 0.5초 정지
	char write[7] = { 'E','N','J','O','Y','!'};
	for (int i = 0; i < strlen(write); i++) {
		printf("%c\t", write[i]);
		Sleep(300);
	}
	system("cls");
}
void expBar() {
	// 퍼센트를 구한다 ~ 1~10 ! 바를 얼마나 채울지 정해주는 변수.
	int bar = (float)player.exp / (float)player.levelUp * 10;
	printf("EXP ");
	colorText(10);
	for (int i = 0; i < bar; i++)
		printf("■");
	for (int i = 0; i < 10 - bar; i++)
		printf("□");
	colorText(15);
	printf(" (%d/%d)", player.exp, player.levelUp);
}
void healthBar(int h, int mh) { // h 체력 / mh 최대체력
	// 퍼센트를 구한다 ~ 1~10 ! 바를 얼마나 채울지 정해주는 변수.
	int bar = (float)h / (float)mh * 10;
	printf("HP ");
	colorText(4);
	for (int i = 0; i < bar; i++)
		printf("■");
	for (int i = 0; i < 10 - bar; i++)
		printf("□");
	colorText(15);
	printf(" (%d/%d)", h, mh);
}
void ifExp() {
	int eventMaxLevel = 30;// 이벤트가 발생하는 레벨은 30레벨 까지이다. 5레벨 단위로 이벤트가 발생. (1...5...10...)
	if (player.exp > player.levelUp) {
		// 플레이어 경험치가 충족치가 넘었다면?
			// 충족치 넘은 경험치 다음 레벨로 넘겨주기 변수 ~
		int tempExp = player.exp - player.levelUp;
		printf("\t레벨 업!!! \n");
		player.level += 1; // 플레이어 레벨업.
		printf("%s 레벨이 %d로 올랐습니다.\n", player.name, player.level);
		player.exp = 0; // 경험치 0으로 초기화
		player.exp += tempExp; // 더해줌으로서 전에 있던 경험치 업.
		player.levelUp += player.level * 100; // 경험치 충족치 레벨에 비례해 올려줌.
		player.maxHealth *= (player.level * 0.5); // 최대체력 레벨에 비례해 올려줌.
		player.health = player.maxHealth; // 체력 회복.
		player.attack *= (player.level * 0.5); // 공격력 레벨에 비례해 올려줌.
		player.defense *= (player.level * 0.5); // 방어력 레벨에 비례해 올려줌.

		// 이벤트 발생 칸. 레벨업을 할 시 이벤트를 발생 시킬지 판단한다.
		// 이벤트가 발생하는 레벨은 30레벨 까지이다. 5레벨 단위로 이벤트가 발생. (1...5...10...)
		if ((player.level % 5) == 0) {
			if (player.level <= eventMaxLevel) { // 플레이어 레벨이 30보다 작으면...
				int temp = 0;
				temp += player.level / 5; // 레벨에 5를 나눠 temp에 더해줌. 챕터 몇장인지 정해주는거임.
				if (temp == 0) return; // 반면 temp에 아무것도 없다면 그냥 종료.
				else { // 아니라면?
					chapterLoad(temp);
				}
			}
		}
		Quiz(player.level); // 특정 레벨에 달성했을 때, 퀴즈 이벤트!
	}
}
void showMenu() {
	ifExp(); // 레벨업 진위 판단.
	printf("\n--------------------------------\n");
	printf("\t    GAME MENU\n");
	printf("--------------------------------\n");
	printf("이름 : %s | 레벨 : %d | 돈 : %d \n", player.name, player.level, player.balance);
	expBar(); printf("\n"); // 경험치 보기
	printf("\n(1) 전  투\t       * BATTLE\n");
	printf("(2) 상  점\t       * SHOP\n");
	printf("(3) 상  태\t       * STATUS\n");
	printf("(4) 종  료\t       * EXIT\n");
}
void hunt()
{
	int choice; // 전투화면 번호 선택함수
	int potionCount = 0; // 선택할 번호 부여
	int potionArr[POTIONSIZE + 1] = { 0 }; // 포션을 선택하기 위한... 그런 배열
	int temp = 0, itemTemp = 0, tempTurn = 1; // 잠시 숫자를 저장할 변수
	monster = monsterSelect; // 포인터 변수 초기화

	monsterDataLoad(); // 몬스터 데이터를 불러온다.
	// 플레이어 레벨이 높아질수록 몬스터가 다양하게 나온다.
	if (player.level >= 1 && player.level < 10) { // 레벨 10까지 몬스터 코드 중 1 ~ 3번만 등장한다.
		temp = random(3) - 1;
		monster += temp;
	}
	else if (player.level >= 10 && player.level < 20) { // 레벨 10 ~ 20까지
		// 몬스터 코드 중 1 ~ 6번만 등장한다.
		temp = random(6) - 1;
		monster += temp;
	}
	else if (player.level >= 20) { // 레벨 20부터는! 몬스터 코드 중 1 ~ 10번 다 등장!
		temp = random(10) - 1;
		monster += temp;
	}
	// 등장할 몬스터가 정해지면, 플레이어 레벨에 맞춰 능력치를 조정한다.
	monster->level = player.level;
	monster->maxHealth *= monster->level; // 최대체력 레벨에 비례해 올려줌.
	monster->health = monster->maxHealth; // 체력 회복.
	monster->attack *= (monster->level * 0.8); // 공격력 레벨에 비례해 올려줌.
	monster->defense *= (monster->level * 0.8); // 방어력 레벨에 비례해 올려줌.

	printf("야생의 %s(이)가 나타났다! \n", monster->name);
	Sleep(500); // 0.5초 정지

	while (1) {

		if (tempTurn == 4) { // 소비 아이템 3턴이 되는 시점이면 효과 종료.
			itemTurn(itemTemp, 1);
			itemTemp = 0;
		}
		printf("\n--------------------------------\n");
		printf("\t    BATTLE\n");
		printf("--------------------------------\n");
		printf("Player ~ %s ", player.name);
		printf("\n"); healthBar(player.health, player.maxHealth); printf("\n\n");
		printf("Enemy  ~ %s ", monster->name);
		printf("\n"); healthBar(monster->health, monster->maxHealth); printf("\n\n");
		printf("--------------------------------\n");
		printf("   1) 공격한다 \t 2) 상    태\n");
		printf("   3) 아 이 템 \t 4) 도망간다\n");
		printf("--------------------------------\n");
		Sleep(500); // 0.5초 정지
		printf("%s(은)는 무엇을 할까? >> ", player.name);
		scanf_s("%d", &choice);
		fseek(stdin, 0, SEEK_END);
		system("cls");

		switch (choice) {
		case AT:
			playerBattle(); 
			if (monster->health < 0) { // 몬스터 HP 소진하면 이긴다.
				printf("%s(이)가 쓰러졌다! \n", monster->name);
				Sleep(300); // 0.3초 정지
				printf("%s(은)는 경험치 %d와 %d원을 얻었다! \n", player.name, getExp(), getMoney());
				monster->health = 0; // 체력 초기화
				monster->health += monster->maxHealth; // 체력 초기화
				Sleep(2000);
				system("pause");
				system("cls");
				if (itemTemp != 0) {
					itemTurn(itemTemp, 1); // 버프 종료
					itemTemp = 0;
				}
				return; // 메인메뉴로 이동
			}
			monsterBattle();
			if (player.health < 0) { // 플레이어가 지면...
				printf("%s(은)는 전투에서 패배했다... \n", player.name);
				Sleep(300); // 0.3초 정지
				printf("%s(은)는 치료비로 %d원을 잃었다... \n", player.name, lostMoney());
				player.health = 0; // 체력 초기화
				player.health += player.maxHealth; // 체력 초기화
				Sleep(2000);
				system("pause");
				system("cls");
				if (itemTemp != 0) {
					itemTurn(itemTemp, 1); // 버프 종료
					itemTemp = 0;
				}
				return; // 메인메뉴로 이동
			}
			if (itemTemp != 0) {
				printf("%s을(를) 사용 한 뒤, %d턴 째입니다. \n", potion[itemTemp - 1].name, tempTurn);
				tempTurn++; // 아이템 사용을 시작했다면, 턴 수를 세기 시작.
			}
			Sleep(1000); // 1초 정지
			system("cls");
			break;
		case CHAT:
			printf("\n--------------------------------\n");
			printf("\t    PLAYER STATUS\n");
			printf("--------------------------------\n");
			printf("이  름 : %s\n", player.name);
			printf("레  벨 : %d\n", player.level);
			printf("체  력 : %d / %d\n", player.health, player.maxHealth);
			printf("공격력 : %d\n", player.attack);
			printf("방어력 : %d\n", player.defense);
			printf("\n--------------------------------\n");
			printf("\t    MONSTER STATUS\n");
			printf("--------------------------------\n");
			printf("이  름 : %s\n", monster->name);
			printf("레  벨 : %d\n", monster->level);
			printf("체  력 : %d / %d\n", monster->health, monster->maxHealth);
			printf("공격력 : %d\n", monster->attack);
			printf("방어력 : %d\n", monster->defense);
			printf("\n관찰내용 - %s\n\n", monster->explain);
			system("pause");
			system("cls");
			break;
		case ITEM:
			for (int i = 0; i < POTIONSIZE; i++) {// 가지고 있는 소비아이템과 개수 출력.
				if (potion[i].possession == 1) {
					potionCount++;
					potionArr[potionCount] = potion[i].itemCode; // 포션배열을 포션카운트대로 아이템코드를 넣는다.
					// 아이템코드) 1. HP포션 2. 원기회복석 3. 공격촉진제 4. 방어촉진제 ... 이하 순서
					printf("%d) %s %dEA \n", potionCount, potion[i].name, potion[i].count);
					printf("- %s\n", potion[i].explain);
				}
			}
			printf("\n");
			// 나에게 아이템이 하나도 없다면?
			if (potionCount == 0) { printf("현재 사용할 수 있는 아이템이 없습니다!\n"); break; } // 전투화면으로~ 
			while (1) {
				printf("%d ~ %d 중 사용할 아이템 선택 | 0 전투로 \n", 1, potionCount);
				printf("어떤 아이템을 고르시겠습니까? >> ");
				scanf_s("%d", &choice);
				if (choice >= 1 && choice <= potionCount) {
					for (int i = 0; i < POTIONSIZE; i++) {
						if (potionArr[choice] == potion[i].itemCode) { // 포션배열에 있는 숫자가 아이템코드와 일치할 시
							printf("%s을 사용하시겠습니까? \n", potion[i].name);
							printf("1. 예 | 2. 아니오 >> ");
							scanf_s("%d", &choice);
							if (choice == 1) {
								printf("\n%s은(는) %s을(를) 사용했다!", player.name, potion[i].name);
								if (player.maxHealth <= player.health + potion[i].upHealth) { // 회복아이템 사용 시 플레이어의 최대체력을 넘는다면...
									temp = player.health + potion[i].upHealth - player.maxHealth; // 넘을 때 얼마나 초과하는가? 계산
									player.health += (potion[i].upHealth - temp); // 최대체력이 넘지 않도록 조정.
									potion[i].count -= 1; // 포션 개수 1 감소
									if (potion[i].itemCode == 3 || potion[i].itemCode == 4) { // 공격촉진제이나 방어촉진제 사용 시
										if (itemTemp == 0) {
											itemTemp = potion[i].itemCode; // 3턴 동안 무슨 아이템을 썼는지 기억하는 변수. 무슨 효과를 줄것인지 정해줌.
											itemTurn(itemTemp, 0);
										}
										else { printf("\n이미 다른 효과가 있습니다! \n"); potion[i].count += 1; }
									}
									if (potion[i].count == 0) { potion[i].possession = 0; } // 개수가 0개라면 소지하지 않음.
								}
								else { // 넘지 않는다면.
									potion[i].count -= 1; // 포션 개수 1 감소
									player.health += potion[i].upHealth;
									if (potion[i].itemCode == 3 || potion[i].itemCode == 4) { // 공격촉진제이나 방어촉진제 사용 시
										if (itemTemp == 0) {
											itemTemp = potion[i].itemCode; // 3턴 동안 무슨 아이템을 썼는지 기억하는 변수. 무슨 효과를 줄것인지 정해줌.
											itemTurn(itemTemp, 0);
										}
										else { printf("\n다른 효과를 다 사용한 뒤 사용해주세요! \n"); potion[i].count += 1; }
									}
									if (potion[i].count == 0) { potion[i].possession = 0; } // 개수가 0개라면 소지하지 않음.
								}
								break;
							}
							else break;
						}
					}
				}
				break;
			}
			potionCount = 0; // 포션 카운트 초기화
			break;
		case RUN:
			// 도망가기는 50% 확률로 성공할 수 있음.
			printf("전투에서 도망칩니다... \n");
			Sleep(1500); // 1.5초 정지
			if (random() >= 5 && random() <= 10) { // 난수가 6~10가 나온다면?
				printf("전투에서 도망쳤습니다! \n");
				Sleep(300); // 0.3초 정지
				printf("도망치는 동안 %d원을 잃었습니다...\n\n", lostMoney());
				Sleep(2000); // 1초 정지
				system("pause");
				system("cls");
				if (itemTemp != 0) {
					itemTurn(itemTemp, 1); // 버프 종료
					itemTemp = 0;
				}
				return; // while 무한루프 탈출 및 프로그램 종료
			}
			else { // 1~5라면?
				printf("도망을 가지 못했습니다! \n");
				Sleep(1000); // 1초 정지
				printf("어어? 몬스터가 반격합니다! \n");
				monsterBattle();
				Sleep(2000);
				system("pause");
				system("cls");
				if (player.health < 0) {
					printf("%s(은)는 전투에서 패배했다... \n", player.name);
					Sleep(300); // 0.3초 정지
					printf("%s(은)는 치료비로 %d원을 잃었다... \n", player.name, lostMoney());
					Sleep(2000); // 0.5초 정지
					player.health = 0; // 체력 초기화
					player.health += player.maxHealth; // 체력 초기화
					system("pause");
					system("cls");
					if (itemTemp != 0) {
						itemTurn(itemTemp, 1); // 버프 종료
						itemTemp = 0;
					}
					return; // 메인 화면으로 이동
				}
				break;
			}
		default:
			printf("잘못된 입력입니다. \n"); break;
		}
	}
}
void itemTurn(int itemCode, int count) {  // 공격 및 방어촉진제 사용 시, 3턴 동안 버프받게 해주는 변수.
	for (int i = 0; i < POTIONSIZE; i++) {
		// 카운트가 0회일시 최초 1회 발동.
		if (potion[i].itemCode == itemCode && count == 0) {
			player.attack += potion[i].upAttack;
			player.defense += potion[i].upDefense;
			printf("\n앞으로 3턴 간 %s의 효과를 받게 됩니다. \n", potion[i].name);
		}
	}
	for (int i = 0; i < POTIONSIZE; i++) {
		// 카운트가 3회일시 버프 끝.
		if (potion[i].itemCode == itemCode && count == 1) {
			player.attack -= potion[i].upAttack;
			player.defense -= potion[i].upDefense;
			printf("\n%s의 효과가 사라졌습니다. \n", potion[i].name);
		}
	}
}
void playerBattle() {
	printf("%s(이)가 공격했다! \n", player.name);
	Sleep(1000); // 1초 정지
	printf("%s에게 %d만큼 데미지를 주었다. \n", monster->name, playerFight());
}
void monsterBattle() {
	printf("%s(이)가 공격했다! \n", monster->name);
	Sleep(1000); // 1초 정지
	printf("%s에게 %d만큼 데미지를 주었다. \n", player.name, monsterFight());
}
void monsterDataLoad() {
	FILE* f = 0; // 파일 포인터 함수.

	fopen_s(&f, MONSTER, "r"); // 몬스터 데이터 읽어오는 과정

	if (f == 0) printf("파일 읽기 실패");
	for (int i = 0; i < MONSTERSIZE; i++) {
		fscanf_s(f, "%s %[^\n]s", monsterSelect[i].name, sizeof(monsterSelect[i].name), monsterSelect[i].explain, sizeof(monsterSelect[i].explain)); // 간단한 무기설명 읽어옴.
		fscanf_s(f, "%d %d %d %d", &monsterSelect[i].maxHealth, &monsterSelect[i].attack, &monsterSelect[i].defense, &monsterSelect[i].monCode);
	}
	fclose(f); // 파일 닫기.
}
void status()
{
	printf("\n--------------------------------\n");
	printf("\t    STATUS\n");
	printf("--------------------------------\n");
	printf("이  름 : %s\n", player.name);
	printf("레  벨 : %d\n", player.level);
	printf("  돈   : %d\n", player.balance);
	printf("체  력 : %d / %d\n", player.health, player.maxHealth);
	printf("공격력 : %d\n", player.attack);
	printf("방어력 : %d\n", player.defense);

	printf("\n● 소지하고 있는 무기 \n");
	for (int i = 0; i < WEAPONSIZE; i++) {
		if (weapon[i].possession == true) {
			printf("%s\n", weapon[i].name);
		}
	}
	printf("\n● 소지하고 있는 아이템\n");
	for (int i = 0; i < POTIONSIZE; i++) {
		if (potion[i].possession == true) {
			printf("%s %dEA\n", potion[i].name, potion[i].count);
		}
	}
}

void potionBuy() { // 소비 구매창

	FILE* f = 0;
	int choice; // 선택 변수
	int count = 0, m = player.balance; // 플레이어가 얼마만큼 포션을 살 수 있는지 세주는 변수.
	printf("\n--------------------------------\n");
	printf("\t    POTION\n");
	printf("--------------------------------\n");

	printf("%2s %13s %6s %9s %9s %4s \n", "순", "품목", "회복량", "공격증가", "방어증가", "가격");
	for (int i = 0; i < POTIONSIZE; i++) {
		// 무기, 공격, 방어, 체력, 가격 출력
		printf("%2d %12s %6d %9d %9d %4d \n %s\n\n", i + 1, potion[i].name, potion[i].upHealth, potion[i].upAttack, potion[i].upDefense, potion[i].sellCost, potion[i].explain);
	}

	printf("1 ~ %d 구입 할 물품 선택 | 0 퇴장\n", POTIONSIZE);
	printf("어떤 물품을 고르시겠습니까? >> ");
	scanf_s("%d", &choice);
	for (int i = 1; i <= POTIONSIZE; i++) {
		if (choice == i) { // 아이템 배열을 골라 찾는다.
			printf("\n%s을 구입하시겠습니까? \n", potion[i - 1].name);
			printf("1. 예\t 2. 아니오 >> ");
			scanf_s("%d", &choice);
			fseek(stdin, 0, SEEK_END);
			if (choice == 1) {
				while (1) { // 플레이어가 이 물건을 최대 얼마까지 살 수 있는지 가르쳐주기 위한 반복문.
					m -= potion[i - 1].sellCost;
					if (m < 0) break;
					count++;
				}
				printf("\n몇 개를 구입하시겠습니까? \n");
				printf("수량을 입력해주세요. (%d개까지 구매가능) >> ", count);
				scanf_s("%d", &choice);
				fseek(stdin, 0, SEEK_END);
				if (choice <= 0) { printf("또 오세요!\n"); return; }
				if (player.balance >= potion[i - 1].sellCost * choice) {
					Sleep(1000);
					printf("구입이 완료되었습니다!\n");
					player.balance -= potion[i - 1].sellCost * choice; // 샀으니 물건 값이 잔액에서 빠짐
					potion[i - 1].possession = true;
					potion[i - 1].count += choice; // 아이템 수량 증가.
					Sleep(1000);
					system("cls");
					return;
				}
				else if (player.balance <= potion[i - 1].sellCost) {
					// 돈이 모자를때.
					Sleep(1000);
					printf("잔액이 부족합니다.\n");
					Sleep(1000);
				}
			}
			else {
				printf("또 오세요!\n");
				Sleep(1000);
				system("cls");
				return;
			}
		}
	}
	printf("또 오세요!\n");
	system("cls");
	return;
}
void weaponBuy() { // 무기 구매창

	FILE* f = 0;
	int choice; // 선택 변수

	printf("\n--------------------------------\n");
	printf("\t    WEAPON\n");
	printf("--------------------------------\n");

	printf("%2s %13s %6s %6s %6s %4s \n", "순", "무기", "공격력", "방어력", "체력", "가격");
	for (int i = 0; i < WEAPONSIZE; i++) {
		// 무기, 공격, 방어, 체력, 가격 출력
		printf("%2d %12s %6d %6d %6d %4d \n %s\n\n", i + 1, weapon[i].name, weapon[i].upAttack, weapon[i].upDefense, weapon[i].upMaxHealth, weapon[i].sellCost, weapon[i].explain);
	}

	printf("1 ~ %d 구입 할 무기 선택 | 0 퇴장\n", WEAPONSIZE);
	printf("어떤 무기를 고르시겠습니까? >> ");
	scanf_s("%d", &choice);
	for (int i = 1; i <= WEAPONSIZE; i++) {
		if (choice == i) { // 아이템 배열을 골라 찾는다.
			printf("\n%s을 구입하시겠습니까? \n", weapon[i - 1].name);
			printf("1. 예\t 2. 아니오 >> ");
			scanf_s("%d", &choice);
			if (choice == 1) {
				if (player.balance >= weapon[i - 1].sellCost && weapon[i - 1].possession == false) {
					Sleep(1000);
					printf("구입이 완료되었습니다!\n");
					player.balance -= weapon[i - 1].sellCost; // 샀으니 물건 값이 잔액에서 빠짐
					player.attack += weapon[i - 1].upAttack;
					player.defense += weapon[i - 1].upDefense;
					player.maxHealth += weapon[i - 1].upMaxHealth;
					weapon[i - 1].possession = true; // 소지가 된 것으로 표시.
					Sleep(1000);
					system("cls");
					return;
				}
				else if (player.balance <= weapon[i - 1].sellCost) {
					// 돈이 모자를때.
					Sleep(1000);
					printf("잔액이 부족합니다...\n");
					Sleep(1000);
				}
				else if (weapon[i - 1].possession == true) {
					// 이미 소지하고 있는 물건이면?
					Sleep(1000);
					printf("이미 소지하고 있는 물건입니다.\n");
					Sleep(1000);
				}
			}
			else if (choice == 2) {
				printf("또 오세요!\n");
				Sleep(1000);
				system("cls");
				return;
			}
			else {
				printf("잘못된 입력입니다.\n");
			}
		}
	}
	if (choice == 0) {
		printf("또 오세요!\n");
		Sleep(1000);
		system("cls");
		return;
	}
}
void userSave() { // 게임 종료 시 유저 데이터를 저장한다.
	FILE* f = 0; // 유저데이터 포인터 함수.
	fopen_s(&f, USERDATA, "wt"); // 파일을 기록하자.
	if (f == NULL) { // 게임 데이터에 문제가 있을 시...
		printf("저장에 실패하였습니다.\n");
		return;
	}
	fprintf(f, "%s\n", player.name);
	fprintf(f, "%d\n", player.level);
	fprintf(f, "%d\n", player.balance);
	fprintf(f, "%d\n", player.exp);
	fprintf(f, "%d\n", player.levelUp);
	fprintf(f, "%d\n", player.health);
	fprintf(f, "%d\n", player.maxHealth);
	fprintf(f, "%d\n", player.attack);
	fprintf(f, "%d\n", player.defense); // 유저 데이터 스탯 저장.
	for (int i = 0; i < WEAPONSIZE; i++) // 무기 소지여부 저장
		fprintf(f, "%d\n", weapon[i].possession);
	for (int i = 0; i < POTIONSIZE; i++) // 포션 소지여부 저장
		fprintf(f, "%d %d\n", potion[i].possession, potion[i].count);
	fclose(f); // 파일 닫기.
	printf("세이브 저장에 성공하였습니다.\n");
	return;
}

void userLoad() { // 게임 불러오기 시 유저 데이터를 불러온다.

	FILE* f = 0; // 파일 포인터 함수.
	fopen_s(&f, USERDATA, "r"); // 파일을 기록하자.
	if (f == NULL) { // 게임 데이터에 문제가 있을 시...
		printf("불러오기에 실패하였습니다.\n");
		printf("데이터 파일이 없거나 손상되었습니다!\n");
		Sleep(1000);
		exit(1);
	}
	fscanf_s(f, "%s", player.name, sizeof(player.name));
	fscanf_s(f, "%d", &player.level);
	fscanf_s(f, "%d", &player.balance);
	fscanf_s(f, "%d", &player.exp);
	fscanf_s(f, "%d", &player.levelUp);
	fscanf_s(f, "%d", &player.health);
	fscanf_s(f, "%d", &player.maxHealth);
	fscanf_s(f, "%d", &player.attack);
	fscanf_s(f, "%d", &player.defense); // 유저 데이터 스탯 불러옴.
	for (int i = 0; i < WEAPONSIZE; i++) // 무기 소지여부 로드
		fscanf_s(f, "%d", &weapon[i].possession);
	for (int i = 0; i < POTIONSIZE; i++) // 포션 소지여부 로드
		fscanf_s(f, "%d %d", &potion[i].possession, &potion[i].count);

	fclose(f); // 파일 닫기.

	itemListLoad(); // 아이템리스트 읽어오기

	printf("세이브 로드에 성공하였습니다.\n");
	printf("안녕하세요! %s님.\n", player.name);
	return;
}
void itemListLoad() {
	FILE* f = 0; // 파일 포인터 함수.

	fopen_s(&f, WEAPON, "r"); // 무기 데이터 읽어오는 과정

	if (f == 0) printf("파일 읽기 실패");
	for (int i = 0; i < WEAPONSIZE; i++) {
		fscanf_s(f, "%s %[^\n]s", weapon[i].name, sizeof(weapon[i].name), weapon[i].explain, sizeof(weapon[i].explain)); // 간단한 무기설명 읽어옴.
		fscanf_s(f, "%d %d %d %d", &weapon[i].upAttack, &weapon[i].upDefense, &weapon[i].upMaxHealth, &weapon[i].sellCost);
	}
	fclose(f); // 파일 닫기.

	fopen_s(&f, POTION, "r"); // 포션 데이터 읽어오는 과정

	if (f == 0) printf("파일 읽기 실패");


	for (int i = 0; i < POTIONSIZE; i++) {
		fscanf_s(f, "%s %[^\n]s", potion[i].name, sizeof(potion[i].name), potion[i].explain, sizeof(potion[i].explain)); // 이름과 상품설명 읽어옴.
		fscanf_s(f, "%d %d %d %d %d", &potion[i].upHealth, &potion[i].upAttack, &potion[i].upDefense, &potion[i].sellCost, &potion[i].itemCode);
	}
	fclose(f);
}
bool bossHunt() // 보스 몬스터와 싸운다!, 보스 몬스터와 싸울 때는. 포션을 사용할 수 없다.
{
	int choice; // 전투화면 번호 선택함수
	int temp = 0, itemTemp = 0, tempTurn = 1; // 잠시 숫자를 저장할 변수
	monsterDataLoad(); // 몬스터 데이터를 불러온다.
	monster = monsterSelect; // 포인터 변수 초기화
	monster += 10; // 보스 몬스터가 10번째 목록에 있다.
	// 등장할 몬스터가 정해지면, 플레이어 레벨에 맞춰 능력치를 조정한다.
	monster->level = player.level;
	monster->maxHealth *= (monster->level * 0.8); // 최대체력 레벨에 비례해 올려줌.
	monster->health = monster->maxHealth; // 체력 회복.
	monster->attack *= (monster->level * 0.7); // 공격력 레벨에 비례해 올려줌.
	monster->defense *= (monster->level * 0.7); // 방어력 레벨에 비례해 올려줌.
	printf("\t    DANGER!!!\n");
	Sleep(500); // 0.5초 정지
	printf("%s(이)가 위협해온다. \n", monster->name);
	Sleep(500); // 0.5초 정지
	printf("알 수 없는 힘으로 아이템을 사용할 수 없다!\n");
	printf("그리고 도망을 갈 수 없어! 전면전이다!\n");
	while (1) {
		printf("\n--------------------------------\n");
		printf("    BOSS !!!! BATTLE\n");
		printf("--------------------------------\n");
		printf("Player ~ %s ", player.name);
		printf("\n"); healthBar(player.health, player.maxHealth); printf("\n\n");
		printf("Enemy  ~ %s ", monster->name);
		printf("\n"); healthBar(monster->health, monster->maxHealth); printf("\n\n");
		printf("--------------------------------\n");
		printf("   1) 공격한다 \t 2) 상    태\n");
		printf("   3) 아 이 템 \t 4) 도망간다\n");
		printf("--------------------------------\n");
		Sleep(500); // 0.5초 정지
		printf("%s(은)는 무엇을 할까? >> ", player.name);
		scanf_s("%d", &choice);
		fseek(stdin, 0, SEEK_END);
		system("cls");

		switch (choice) {
		case AT:
			playerBattle();
			if (monster->health < 0) { // 몬스터 HP 소진하면 이긴다.
				printf("%s(은)는 %s를 쓰러트렸다! \n", player.name, monster->name);
				printf("세상에 평화가 찾아왔다... \n", player.name, monster->name);
				monster->health = 0; // 체력 초기화
				monster->health += monster->maxHealth; // 체력 초기화
				Sleep(2000);
				system("pause");
				system("cls");
				return true; // 메인메뉴로 이동
			}
			monsterBattle();
			if (player.health < 0) { // 플레이어가 지면...
				printf("%s(은)는 전투에서 패배했다... \n", player.name);
				Sleep(300); // 0.3초 정지
				printf("%s(은)는 %s에게 무력해지고 말았다...\n", player.name, monster->name);
				player.health = 0; // 체력 초기화
				player.health += player.maxHealth; // 체력 초기화
				Sleep(2000);
				system("pause");
				system("cls");
				return false;
			}
			Sleep(1000); // 1초 정지
			system("cls");
			break;
		case CHAT:
			printf("\n--------------------------------\n");
			printf("\t    PLAYER STATUS\n");
			printf("--------------------------------\n");
			printf("이  름 : %s\n", player.name);
			printf("레  벨 : %d\n", player.level);
			printf("체  력 : %d / %d\n", player.health, player.maxHealth);
			printf("공격력 : %d\n", player.attack);
			printf("방어력 : %d\n", player.defense);
			printf("\n--------------------------------\n");
			printf("\t    MONSTER STATUS\n");
			printf("--------------------------------\n");
			printf("이  름 : %s\n", monster->name);
			printf("레  벨 : %d\n", monster->level);
			printf("체  력 : %d / %d\n", monster->health, monster->maxHealth);
			printf("공격력 : %d\n", monster->attack);
			printf("방어력 : %d\n", monster->defense);
			printf("\n관찰내용 - %s\n\n", monster->explain);
			system("pause");
			system("cls");
			break;
		case ITEM:
			printf("보스전에서는 아이템을 사용 할 수 없습니다!\n");
			break;
		case RUN:
			printf("보스전에서는 도망가기를 할 수 없습니다!\n"); break;
		default:
			printf("잘못된 입력입니다. \n"); break;
		}
	}
}