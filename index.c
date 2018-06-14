/*Name: Sanid Singhal (75252882)
Student Email: singhal.s@alumni.ubc.ca
Date: Dec. 1, 2017
Lab Group: L1H
Purpose of Code: The following code will run a memorzing game called the Simon game.
The user is to prompt the series of numbers displayed on screen, and if they get it correct they will proceed to the next round. Each round one more light is to be memorized.
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <DAQlib.h>
#include <time.h>

/*Defining all symbols for readability*/


#define ON 1
#define OFF 0x
#define TRUE 1
#define FALSE 0

/*Defining all switches and corresponding buttons as symbolic constants */

#define GLED 0
#define GBUTTON 0
#define RBUTTON 1
#define RLED 1
#define YBUTTON 2
#define YLED 2
#define BBUTTON 3
#define BLED 3

/*Defining time constants*/

#define INDICATION_TIME (2000)
#define PAUSE_TIME (500)

/*Defing the amount of times the lights to blink as an indicator for the user to start */

#define BLINK 2

/* Defining max level and amount of lights in a sequence*/

#define MAXLEVEL 5

/* Function prototypes*/

void Intro(void);
void start(int numlights[], int userlights[]);
void lightorder(int numlights[], int level);
void flashlights(int numlights[], int level);
int  check(int numlights[], int userlights[], int level);
void correct(void);
void incorrect(void);
void restartgame(int numlights[], int userlights[]);

int main() {
	int setupnum;
	srand((unsigned)time(NULL));
	int numlights[MAXLEVEL];
	int userlights[MAXLEVEL];
	Intro();
	printf("Time to play! Enter 6 for the simulator.\n");
	scanf("%d", &setupnum);

	if (setupDAQ(setupnum) == TRUE) {
		while (continueSuperLoop() == TRUE) {
			start(numlights, userlights);
		}
	}
	else {
		printf("Error, code not recognized.");
	}
	system("PAUSE");
	return 0;
}

void Intro(void) {

	int begin;
	printf("Welcome to the Simon Game!\n Enter 1 for instructions. \n");
	scanf("%d", &begin);

	if (begin == 1) {
		printf("The Simons game is a memorization game. \n A light will be displayed, then you will have to push the buttons in the same order displayed. \n It will be you're turn to repeat the sequence \n once the blue and green lights blink two times together.\n Wait about 3 seconds after it is prompted your turn to begin pushing buttons. \n If correct, the green light will blink twice. \nIf incorrect the red light will blink twice. \n Each level up to 5, one more light is added to the sequence to memorize. \n Do not press multiple buttons at the same time. If you reach to five you win, good luck!");
	}
	else {
		printf("Thats too bad, see ya later!");
	}
}

void start(int numlights[], int userlights[]) {

	printf("Game starting now....");
	Sleep(INDICATION_TIME);

	while (continueSuperLoop() == TRUE) {

		int level = 1;


		while (level <= MAXLEVEL) {

			lightorder(numlights, level);
			Sleep(INDICATION_TIME);
			if (check(numlights, userlights, level) == TRUE) {
				correct();
				level++;
			}
			else if (check(numlights, userlights, level) == FALSE) {
				incorrect();
				printf("Sorry you have lost. \n ");
				restartgame(numlights, userlights);
			}
		}

		printf("Congrats you have won the game! \n");
		restartgame(numlights, userlights);

	}
}


void lightorder(int numlights[], int level) {

	int count;


	for (count = 0; count < level; count++) {
		/*Storing the lights supposed to be on in an array*/
		numlights[count] = rand() % 4;
	}
	flashlights(numlights, level);
}

void flashlights(int numlights[], int level) {

	int counter;

	for (counter = 0; counter < level; counter++) {

		digitalWrite(numlights[counter], ON);

		Sleep(PAUSE_TIME);

		digitalWrite(numlights[counter], OFF);
	}

	Sleep(INDICATION_TIME);
	int num;
	/*To indicate its the users turn, green and blue LEDs will blink twice*/
	for (num = 0; num < BLINK; num++) {
		digitalWrite(GLED, ON);
		digitalWrite(BLED, ON);

		Sleep(PAUSE_TIME);

		digitalWrite(GLED, OFF);
		digitalWrite(BLED, OFF);
	}
}

int check(int numlights[], int userlights[], int level) {
	int order = FALSE;

	int green;
	int red;
	int yellow;
	int blue;
	int counter = 0;

	while (counter < level) {

		do {
			green = digitalRead(GBUTTON);
			red = digitalRead(RBUTTON);
			yellow = digitalRead(YBUTTON);
			blue = digitalRead(BBUTTON);
		} while (green == 0 && red == 0 && yellow == 0 && blue == 0);

		if (green == ON) {
			userlights[counter] = 0;
		}

		else if (red == ON) {
			userlights[counter] = 1;
		}

		else if (yellow == ON) {
			userlights[counter] = 2;
		}

		else if (blue == ON) {
			userlights[counter] = 3;
		}

		do
		{
			green = digitalRead(GBUTTON);
			red = digitalRead(RBUTTON);
			yellow = digitalRead(YBUTTON);
			blue = digitalRead(BBUTTON);

		} while (green != OFF || red != OFF || yellow || OFF || blue != OFF);
		counter++;
	}


	int count;

	for (count = 0; count < level; count++) {

		if (numlights[count] != userlights[count]) {
			return order;

		}

	}

	order = TRUE;

	return order;
}


void correct(void) {
	Sleep(PAUSE_TIME);
	digitalWrite(GLED, ON);
	Sleep(PAUSE_TIME);
	digitalWrite(GLED, OFF);
	digitalWrite(GLED, ON);
	Sleep(PAUSE_TIME);
	digitalWrite(GLED, OFF);
	Sleep(INDICATION_TIME);
}

void incorrect(void) {
	Sleep(PAUSE_TIME);
	digitalWrite(RLED, ON);
	Sleep(PAUSE_TIME);
	digitalWrite(RLED, OFF);
	digitalWrite(RLED, ON);
	Sleep(PAUSE_TIME);
	digitalWrite(RLED, OFF);
	Sleep(INDICATION_TIME);

}

void restartgame(int numlights[], int userlights[]) {
	int restart;
	printf("Would you like to restart? Enter 1 to play again or any other key to quit.");
	scanf("%d", &restart);
	if (restart == 1) {
		start(numlights, userlights);
	}
}

























