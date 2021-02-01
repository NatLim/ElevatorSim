#include "..\..\rt.h"
#include <stdlib.h>

int main() {

	while (true) {

		int E1_floor = rand() % 1000;
		int E2_floor = rand() % 1000;

		//first elevator
		switch (E1_floor) {
		case 0:
			for (int i = 0; i < 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i -1 << "|  |";
			}
			MOVE_CURSOR(0, 9);
			cout << 0 << "|E1|";
			break;

		case 1:
			for (int i = 0; i < 8; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 8);
			cout << 1 << "|E1|";
			MOVE_CURSOR(0, 9);
			cout << 0 << "|  |";
			break;

		case 2:
			for (int i = 0; i < 7; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1  << "|  |";
			}
			MOVE_CURSOR(0, 7);
			cout << 2 << "|E1|";
			for (int i = 8; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10-i-1 << "|  |";
			}
			break;

		case 3:
			for (int i = 0; i < 6; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 6);
			cout << 3 << "|E1|";
			for (int i = 7; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 4:
			for (int i = 0; i < 5; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 5);
			cout << 4 << "|E1|";
			for (int i = 6; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 5:
			for (int i = 0; i < 4; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 4);
			cout << 5 << "|E1|";
			for (int i = 5; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 6:
			for (int i = 0; i < 3; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 3);
			cout << 6 << "|E1|";
			for (int i = 4; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 7:
			for (int i = 0; i < 2; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 2);
			cout << 7 <<"|E1|";
			for (int i = 3; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 8:
			for (int i = 0; i < 1; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(0, 1);
			cout << 8 << "|E1|";
			for (int i = 2; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;


		case 9:
			MOVE_CURSOR(0, 0);
			cout << 9 << "|E1|";
			
			for (int i = 1; i <= 9; i++) {
				MOVE_CURSOR(0, i);
				cout << 10-i-1 << "|  |";
			}
			break;
		}

		switch (E2_floor) {
		case 0:
			for (int i = 0; i < 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";

			}
			MOVE_CURSOR(10, 9);
			cout << 0 << "|E2|";
			break;

		case 1:
			for (int i = 0; i < 8; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 8);
			cout << 1 << "|E2|";
			MOVE_CURSOR(10, 9);
			cout << 0 << "|  |";
			break;

		case 2:
			for (int i = 0; i < 7; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 7);
			cout << 2 << "|E2|";
			for (int i = 8; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 3:
			for (int i = 0; i < 6; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 6);
			cout << 3 << "|E2|";
			for (int i = 7; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 4:
			for (int i = 0; i < 5; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 5);
			cout << 4 << "|E2|";
			for (int i = 6; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 5:
			for (int i = 0; i < 4; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 4);
			cout << 5 << "|E2|";
			for (int i = 5; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 6:
			for (int i = 0; i < 3; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 3);
			cout << 6 << "|E2|";
			for (int i = 4; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 7:
			for (int i = 0; i < 2; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 2);
			cout << 7 << "|E2|";
			for (int i = 3; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;

		case 8:
			for (int i = 0; i < 1; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			MOVE_CURSOR(10, 1);
			cout << 8 << "|E2|";
			for (int i = 2; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;


		case 9:
			MOVE_CURSOR(10, 0);
			cout << 9 << "|E2|";

			for (int i = 1; i <= 9; i++) {
				MOVE_CURSOR(10, i);
				cout << 10 - i - 1 << "|  |";
			}
			break;
		}

		
	}
		return 0;
}
