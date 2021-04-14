#include <bangtal.h>
#include <stdio.h>

SceneID room1, puzzle;
ObjectID startButton, endButton, door1,safe, picture1, picture2, table, flowerbase;
ObjectID knife, scissors, bag, writingcase, book, clip;
ObjectID key1, key2, keypad;
TimerID timer1, timer2;

//책 선택지
ObjectID bookselect1, bookselect2, bookselect3;
//timer1은 전체 게임 제한시간
//timer2는 퍼즐 제한 시간
//시간이 된다면 날아가는 벌레 구현

void startGame() {
	hideObject(startButton);
	showTimer(timer1);
	startTimer(timer1);
}

void knifeend() {
	showMessage("칼이 열쇠를 부셨어요..");
}

void success() {
	showMessage("클립 누르면 문열리는거 모르는 흑우 없제 ?");
}

void hidden() {
	showMessage("어캐 했누..?");
}

void keypuzzle() {

}

void keypadGame() {

}

void timerCallback(TimerID timer) {
	if (timer == timer1) {
		endGame();
		showMessage("아쉽지만 시간이 지났군요");
	}
}
//ObjectID startButton, endButton, door1, safe, picture1, picture2, table, flowerbase;
//ObjectID knife, scissors, bag, writingcase, book, clip;
void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == startButton) {
		startGame();
	}
	else if (object == endButton) {
		endGame();
	}
	else if (object == door1) {
		if (knife == getHandObject()) {
			//칼과 열쇠를 같은 가방에 넣으면 열쇠가 깨진다는 설정
			knifeend();
		}
		else if (key1 == getHandObject()) {
			success();
		}
		else if (clip == getHandObject()) {
			hidden();
		}
	}
	else if (object == picture1) {
		enterScene(puzzle);
	}
	else if (object == keypad) {
		showKeypad("生死苦樂", door1);
	}
	else if (object == picture1) {
		if (scissors == getHandObject()) {
			hideObject(picture1);
		}
	}
	else if (object == picture2) {
		locateObject(picture2, room1,  );
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
		showObject(key1);
	}
	else if (object == bookselect2) {
		hideObject(book);
	}
	else if (object == bookselect3) {
		//시간 2분 줄이는 방법 고민하자
	}
	else if (object == clip) {
		pickObject(clip);
	}
}

int main() {
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, true);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_ROOM_TITLE, false);

	setMouseCallback(mouseCallback);

	room1 = createScene("배경 방", "빈방.png");

	door1 = createObject("문.png");
	locateObject(door1, room1, 800, 270);
	showObject(door1);

    safe = createObject("금고.png");
	locateObject(safe, room1, 800, 270);

	picture1 = createObject("그림1.png");
	locateObject(picture1, room1, 800, 270);
	showObject(picture1);

	picture2 = createObject("그림2.png");
	locateObject(picture2, room1, 800, 270);
	showObject(picture2);

	table = createObject("책상.png");
	locateObject(table, room1, 800, 270);
	showObject(table);

	flowerbase = createObject("꽃병.png");
	locateObject(flowerbase, room1, 800, 270);
	showObject(flowerbase);

	knife = createObject("칼.png");
	locateObject(knife, room1, 800, 270);
	showObject(knife);

	scissors = createObject("가위.png");
	locateObject(scissors, room1, 800, 270);
	showObject(scissors);

	bag = createObject("가방.png");
	locateObject(bag, room1, 800, 270);
	showObject(bag);

	writingcase = createObject("필통.png");
	locateObject(writingcase, room1, 800, 270);
	showObject(writingcase);

	book = createObject("책.png");
	locateObject(book, room1, 800, 270);
	showObject(book);

	clip = createObject("클립.png");
	locateObject(clip, room1, 800, 270);
	showObject(clip);

	bookselect1 = createObject("책선택1.png");
	locateObject(bookselect1, room1, 800, 270);

	bookselect2 = createObject("책선택2.png");
	locateObject(bookselect2, room1, 800, 270);

	bookselect3 = createObject("책선택3.png");
	locateObject(bookselect3, room1, 800, 270);

	keypad = createObject("keypad.png");
	locateObject(keypad, room1, 800, 270);

	timer1 = createTimer(300.0f);

	startGame(room1);
}

//퍼즐 가자~
//퍼즐이 끝나면서 showObject(safe)와 showObject(keypad)넣어줄 것