#ifndef STATDATA_H
#define STATDATA_H

struct People { // 플레이어에 쓰일 변수 구조체
	char name[30] = { 'h', 'u', 'm', 'a', 'n'};
	int level = 1;
	int exp = 0; // 경험치
	int levelUp = 100; // 레벨업에 필요한 경험치 
	int balance = 1000; // 돈
	int maxHealth = 100; // HP 최대체력
	int health = maxHealth; // HP 체력
	int attack = 50; // AT 공격력
	int defense = 50; // DF 방어력
};
struct Enemy { // 적(몬스터)에 쓰일 변수 구조체
	char name[30] = { 'e', 'n', 'e', 'm', 'y' };
	int level = 1;
	int maxHealth = 30; // HP 최대체력
	int health = maxHealth; // HP 체력
	int attack = 20; // AT 공격력
	int defense = 20; // DF 방어력
	int monCode; // 몬스터 고유코드
	char explain[1000]; // 몬스터 설명
};
struct Item { //아이템에 쓰일 변수 구조체
	char name[30] = { 'i','t','e','m' };
	int upHealth; // 체력 회복
	int upMaxHealth; // 최대체력 회복 늘려줌.
	int upExp;
	int sellCost; // 해당 아이템의 가격
	int upAttack = 30; // AT 공격력
	int upDefense = 30; // DF 방어력
	bool possession = false;  // 아이템 소지 여부
	int count = 0; // 물품 개수
	int itemCode; // 아이템 고유코드
	char explain[1000]; // 아이템 설명
};

extern struct People player;

// 반환 값이 필요한 함수 (숫자놀이)
int random(int set = 10); // 1~10 난수가 나오는 함수
int lostMoney(); // 플레이어가 돈을 잃는 함수. (차감액은 변동적)
int getMoney(); // 플레이어가 돈을 얻는 함수. (획득액은 변동적)

// 기본 게임 메뉴들.
void nameSet(); // 초기 이름 설정 메뉴
void ifExp(); // 경험치가 충족요건이 넘었는지 판단해주는 함수.
void expBar(); // 경험치 보기
void showMenu(); // 메뉴 리스트 출력
void healthBar(int h, int mh); // 체력바 표시.
void gameLogo(); // 게임 타이틀 로고 출력
int startMenu(); // 스타트 메뉴 출력. 메뉴 선택 시 0 ~ 3 숫자 반환.
void potionBuy(); // 소비 구매창
void weaponBuy(); // 무기 구매창

// 전투 시 필요한 메뉴
void hunt(); // 전투 시스템 시작
bool bossHunt(); // 보스 몬스터와 싸운다!, 보스 몬스터와 싸울 때는. 포션을 사용할 수 없다.
void playerBattle(); // 배틀 시 플레이어 턴
void monsterBattle(); // 배틀 시 몬스터 턴

// 데이터 관련
void shop(); // 상점 리스트 출력
void status(); // 유저 정보 출력
void userSave(); // 유저데이터 저장 함수
void userLoad(); // 게임 불러오기 시 유저 데이터를 불러온다.
void monsterDataLoad(); // 몬스터 데이터를 불러온다.
void itemListLoad(); // 무기 및 소비 아이템 데이터를 불러온다.

// 기타...
void gotoxy(int x, int y); // 커서 위치 이동함수
int keyControl(); // 입력한 키 값을 가져와 주는 함수.
void colorText(int colorSet); // 글씨색깔 바꿔주는 함수.
void itemTurn(int itemCode, int count);  // 공격 및 방어촉진제 사용 시, 5턴 동안 버프받게 해주는 변수.
void prologue(); // 프롤로그 보여주는 함수
void chapterLoad(int t); // 챕터 몇 장을 볼지 정해서 보여주는 함수.
void Quiz(int level); // 퀴즈퀴즈!!!

#endif



