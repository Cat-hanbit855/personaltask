#define _CRT_SECURE_NO_WARNINGS

#include <bangtal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

SceneID room1, puzzle, insafe;

ObjectID startButton, endButton, door1, safe, picture1, picture2, blanket, trace, knifetrace;

ObjectID knife, scissors, bag, writingcase, book, clip;

ObjectID key1, key2, key1copy, keypad;

TimerID timer1;

bool locked = true;

bool game = false;

bool puzzlegame = false;

//책 선택지
ObjectID bookselect1, bookselect2, bookselect3;
//timer1은 전체 게임 제한시간


void startGame() {
	hideObject(startButton);
	showTimer(timer1);
	startTimer(timer1);
}

void knifeend() {
	showMessage("칼이 열쇠를 부셨어요..");
}

void success() {
	showMessage("클립 가지고 있으면 문 열리는데!!");
}

void broken()
{
	showMessage("열쇠가 부셔져서 문이 안열려요");
}
void hidden() {
	showMessage("어캐 했누..?");
}

void timerCallback(TimerID timer) {
	if (timer == timer1) {
		showObject(startButton);
		showObject(endButton);
	}
}
//퍼즐 코드 

ObjectID piece[9], original_piece[9], finishpuzzle;
int blank = 8;

int indexX(int i) {
	return 300 + 200 * (i % 3);
}

int indexY(int i)
{
	return 480 - 200 * (i / 3);
}

int game_index(ObjectID object)
{
	for (int i = 0; i < 9; i++) {
		if (piece[i] == object) return i;
	}
	return -1;
}

bool possible_move(int i)
{
	if (i % 3 > 0 && blank == i - 1) return true;
	if (i % 3 < 2 && blank == i + 1) return true;
	if (i / 3 > 0 && blank == i - 3) return true;
	if (i / 3 < 2 && blank == i + 3) return true;

	return false;
}
bool game_end() {
	for (int i = 0; i < 9; i++) {
		if (piece[i] != original_piece[i]) return false;
	}
	return true;
}

int random()
{
	int i = -1;
	while (i == -1) {
		switch (rand() % 3) {
		case 0: if (blank % 3 > 0) i = blank - 1;
			break;
		case 1: if (blank % 3 < 2) i = blank + 1;
			break;
		case 2: if (blank / 3 > 0) i = blank - 3;
			break;
		case 3: if (blank / 3 < 2) i = blank + 3;
			break;

		}
	}
	return i;
}

void game_move(int i)
{
	ObjectID object = piece[i];
	piece[i] = piece[blank];
	locateObject(piece[i], puzzle, indexX(i), indexY(i));
	piece[blank] = object;
	locateObject(piece[blank], puzzle, indexX(blank), indexY(blank));

	blank = i;
}

void game_start()
{
	for (int i = 0; i < 56; i++) {
		game_move(random());
	}

}

SceneID game_init() {

	puzzle = createScene("빈화면", "noname.png");

	char path[50];
	for (int i = 0; i < 9; i++) {
		sprintf(path, "dog-00%d.png", i);
		piece[i] = createObject(path);
		locateObject(piece[i], puzzle, indexX(i), indexY(i));
		showObject(piece[i]);
		original_piece[i] = piece[i];
	}
	game_start();

	return puzzle;
}
//마우스 콜백
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	int i = game_index(object);
	if(game) {
		if (puzzlegame)
		{
			if (game_end()) {
				enterScene(room1);
				showObject(keypad);
				showObject(safe);
				showObject(trace);
				hideObject(picture1);
				puzzlegame = false;
				game = true;
			}
			else if (possible_move(i))
			{
				game_move(i);
			}
		}
		else
		{
			//여기부터  room1
			 if (object == startButton) {
				startGame();
				hideObject(endButton);
			}
			else if (object == endButton) {
				 endGame();
			}
			else if (object == door1) {
				if (knife == getHandObject() && key1 == getHandObject()) {
					//칼과 열쇠를 같은 가방에 넣으면 열쇠가 깨진다는 설정
					knifeend();
				}
				else if (knife == getHandObject()) {
					showMessage("비브라늄으로 된 문이 칼을 부셨어요.");
					dropObject(knife);
					hideObject(knife);
					pickObject(knifetrace);
					showObject(endButton);
					game = false;
					stopTimer(timer1);

				}
				else if (key1 == getHandObject()) {
					success();
					showObject(endButton);
					game = false;
					stopTimer(timer1);
				}
				else if (key2 == getHandObject()) {
					broken();
					showObject(endButton);
					game = false;
					stopTimer(timer1);
				}
				else if (clip == getHandObject()) {
					hidden();
					stopTimer(timer1);
					showObject(endButton);
					game = false;
					stopTimer(timer1);
				}
			}
			else if (object == picture1) {
				if (scissors == getHandObject()) {
					hideObject(picture1);
					showObject(safe);
					showObject(trace);
					showObject(keypad);
				}
				else {
					enterScene(game_init());
					puzzlegame = true;
				}
			}
			else if (object == trace) {
				pickObject(trace);
			}
			else if (object == picture2) {
				locateObject(picture2, room1, 653, 240);
				locateObject(key2, room1, 653, 240);
				showObject(key2);
			}
			else if (object == blanket && trace == getHandObject()) {
				showMessage("금고의 힌트 : angel");
			}
			else if (object == knife) {
				pickObject(knife);
			}
			else if (object == scissors) {
				pickObject(scissors);
			}
			else if (object == book) {
				showObject(bookselect1);
				showObject(bookselect2);
				showObject(bookselect3);
			}
			else if (object == bookselect1) {
				showObject(key1copy);
				hideObject(book);
				hideObject(bookselect1);
				hideObject(bookselect2);
				hideObject(bookselect3);
			}
			else if (object == bookselect2) {
				hideObject(book);
				hideObject(bookselect1);
				hideObject(bookselect2);
				hideObject(bookselect3);
			}
			else if (object == bookselect3) {
				decreaseTimer(timer1, 60.0f);
				hideObject(book);
				hideObject(bookselect1);
				hideObject(bookselect2);
				hideObject(bookselect3);
			}
			else if (object == clip) {
				pickObject(clip);
			}
			else if (object == safe) {
				if (locked == true) {
					showMessage("잠겨있습니다");
				}
				else {
					enterScene(insafe);
				}
			}
			else if (object == key1) {
				 pickObject(key1);
				 enterScene(room1);
			 }
			else if (object == key2) {
				 pickObject(key2);
				 enterScene(room1);
			 }
			else if (object == key1copy) {
			     pickObject(key1copy);
			 }
			else if (object == keypad) {
				showKeypad("1004", safe);
			}
		}
	}
	else  {
		if (object == startButton) {
			game = true;
		}
		else if (object == endButton) {
			endGame();
		}
	}
}

void objectCallback(ObjectID object, EventID event) {
	if (object == safe) {
		if (event == EventID::EVENT_KEYPAD) {
			locked = false;
			showMessage("잠금 해제되었습니다");
		}
	}
}

int main() {

	srand(time(NULL));

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, true);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);

	setTimerCallback(timerCallback);

	setObjectCallback(objectCallback);

	room1 = createScene("배경 방", "방.png");

	puzzle = createScene("빈화면", "noname.png");

	insafe = createScene("금고안", "금고안.png");

	finishpuzzle = createObject("완성된 그림.png");
	locateObject(finishpuzzle, puzzle, 0, 0);

	startButton = createObject("start.png");
	locateObject(startButton, room1, 600, 200);
	showObject(startButton);

	endButton = createObject("end.png");
	locateObject(endButton, room1, 600, 130);
	showObject(endButton);

	door1 = createObject("문.png");
	locateObject(door1, room1, 876, 275);
	showObject(door1);

    safe = createObject("금고.png");
	locateObject(safe, room1, 244, 344);

	picture1 = createObject("그림1.png");
	locateObject(picture1, room1, 213, 354);
	showObject(picture1);

	trace = createObject("흔적.png");
	locateObject(trace, room1, 240, 380);
	scaleObject(trace, 0.3f);

	picture2 = createObject("그림2.png");
	locateObject(picture2, room1, 653, 480);
	showObject(picture2);

	blanket = createObject("이불.png");
	locateObject(blanket, room1, 133, 280);
	scaleObject(blanket, 0.7f);
	showObject(blanket);

	knife = createObject("칼.png");
	locateObject(knife, room1, 556, 306);
	scaleObject(knife, 0.7f);
	showObject(knife);

	scissors = createObject("가위.png");
	locateObject(scissors, room1, 1040, 168);
	scaleObject(scissors, 0.25f);
	showObject(scissors);

	bag = createObject("가방.png");
	locateObject(bag, room1, 502, 250);
	scaleObject(bag, 0.3f);
	showObject(bag);

	writingcase = createObject("필통.png");
	locateObject(writingcase, room1, 983, 147);
	scaleObject(writingcase, 0.25f);
	showObject(writingcase);

	book = createObject("책.png");
	locateObject(book, room1, 935, 182);
	scaleObject(book, 0.2f);
	showObject(book);

	clip = createObject("클립.png");
	locateObject(clip, room1, 1021, 218);
	scaleObject(clip, 0.333333333f);
	showObject(clip);

	knifetrace = createObject("칼흔적.png");
	locateObject(knifetrace, room1, 830, 275);
	scaleObject(knifetrace, 0.3f);

	key1 = createObject("열쇠1.png");
	locateObject(key1, insafe, 130, 370);
	scaleObject(key1, 0.1f);
	showObject(key1);

	key1copy = createObject("열쇠1.png");
	locateObject(key1copy, room1, 935, 182);
	scaleObject(key1copy, 0.05f);

	key2 = createObject("열쇠2.png");
	locateObject(key2, insafe, 570, 92);
	scaleObject(key2, 0.1f);
	showObject(key2);
	

	bookselect1 = createObject("책선택1.png");
	locateObject(bookselect1, room1, 200, 570);
	scaleObject(bookselect1, 0.8f);

	bookselect2 = createObject("책선택2.png");
	locateObject(bookselect2, room1, 200, 420);
	scaleObject(bookselect2, 0.8f);

	bookselect3 = createObject("책선택3.png");
	locateObject(bookselect3, room1, 200, 270);
	scaleObject(bookselect3, 0.8f);

	keypad = createObject("keypad.png");
	locateObject(keypad, room1, 300, 430);
	scaleObject(keypad, 0.1f);

	timer1 = createTimer(180.0f);

	startGame(room1);
}
