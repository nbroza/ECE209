// ECE 209 - Program 2: Farkle
// This program allows multiple users to play a dice game named Farkle.
// Rules of the game: https://www.dicegamedepot.com/farkle-rules/
// Number of players: 2 to 4
// Nick Broza

#include <stdio.h>
// DO NOT include or use any other standard library routines
// There is NO NEED to use strings in this program.
// NO GLOBAL VARIABLES

// functions and variables for random number generation
void seedRandom(unsigned int seed);  // initialize random number generator
unsigned int getRandom(unsigned int limit);  // returns random integer between 0 and limit-1

// dice data structure: an array of seven values
// index 0: total number of dice in the collection
// index 1-6: element i is the number of dice with face value = i
typedef int DiceGroup[7];
// if you don't understand how typedef works, or just prefer to not use this type,
// just declare an array of 7 integers instead -- you will not lose points for this

void printarr(DiceGroup dice);

int takeTurn();   // interact with user to roll dice and return score
void rollDice(DiceGroup dice, int n);     // initialize a specified number of dice
void printDice(const DiceGroup dice);  // print dice as a sorted sequence of digits
void clearArray(DiceGroup dice);
int selectDice(DiceGroup dice, DiceGroup keep, int choice);   // move dice from initial set to "keep" set
int scoreDice(const DiceGroup dice);    // return highest score available for a set of dice
int setDice(DiceGroup dice, int data);      // initialize dice using an integer representation
int testFarkle(const DiceGroup dice);   // return 1 if no scoring dice
// You may declare additional functions here if they are defined below.

// DO NOT DEFINE functions above the main function -- you will lose points
// main function: user interface
int main() {
    int players = 0;  // number of players -- must be 2 to 4
    int scores[4] = {0};  // player scores -- start at zero
    int winner = 0;       // winning player (zero until someone wins)
    int p = 0;            // current player
    int i;        // used for loops
    unsigned int seed;    // initializer for random number generator

    setbuf(stdout,0);
    printf("Welcome to Farkle!\nHow many players? ");
    scanf("%d", &players);
    if ((players < 1) || (players > 4)) {
        printf("This game only supports 1 to 4 players. How many? ");
        scanf("%d", &players);
    }
    printf("Enter a seed integer (decimal or hexadecimal): ");
    scanf("%i", &seed);
    seedRandom(seed);

    while(!winner) {
        int score = 0;
        printf("\nSCORES -- ");
        for (i=0; i<players; i++) printf("%d: %d  ", i+1, scores[i]);
        printf("\nPlayer %d's turn\n", p+1);
        score = takeTurn();
        printf("Turn score = %d\n", score);
        if (players == 1) return 0;    // 1 player means one turn (used for testing)
        scores[p] += score;
        if (scores[p] >= 10000) winner = p+1;
        p = (p + 1) % players;
    }
    printf("\nSCORES -- ");
    for (i=0; i<players; i++) printf("%d: %d  ", i+1, scores[i]);
    printf("\nPlayer %d wins!", winner);
    return 0;
}

// DEFINE your functions here -- above the PRNG code
int takeTurn() {
	DiceGroup dice = {6,0,0,0,0,0,0}; //initializes the dice array with 6 at the beginning because we use this later for setting n between iterations
	int n = 6; //number of dice
	int score = 0;
	while (1) {
        
        for (int i = 1; i < 7; i++) { //clears the data part of the array between iterations
            dice[i] = 0;
        }
	    
		n = dice[0];
		rollDice(dice, n);
		
		printf("Rolling %d dice...", n);
		printDice(dice);
		printf("\n");
		
		if(testFarkle(dice)) {
			printf("FARKLE -- your turn is over.\n");
			return 0;
		}
		
		DiceGroup keep = {0,0,0,0,0,0,0}; //creates empty keep array
		int choice = 0;
		while (1) {
	        printf("Which to keep? ");
            scanf("%d", &choice);	
			if(selectDice(dice, keep, choice)) {
			    break; //ends keep loop if the dice selected is valid
			}
            continue; //keeps prompting for user input if the input is invalid
		}
		
		printf("Keeping ");
        printDice(keep);
        printf(", score = %d\n", scoreDice(keep));
        score += scoreDice(keep);
        printf("Score so far = %d\n", score);
		
		char ans;
		if(dice[0] == 0) {
            printf("HOT DICE!  Roll 6 dice (y/n)? ");
            scanf(" %c", &ans);		
			if (ans == 'n') {
				return score;
			}
			dice[0] = 6;
			continue;
		}
		
        printf("%d dice left -- roll again (y/n)? ", dice[0]);
        scanf(" %c", &ans);
        if(ans == 'n') {
			return score;
		}
        continue;		
	}
}

void printDice(const DiceGroup dice) {
    for (int i = 1; i < 7; i++) { //loop for each "dice" part of array
        for (int j = dice[i]; j > 0; j--) { //loop to print the number the number of times specified in array
            printf("%d", i);
        }
    }
}

void rollDice(DiceGroup dice, int n) {
    dice[0] = n;
    for (int i = 0; i < n; i++) {
        int rnum = getRandom(6) + 1; //creates random number with offset 1 as getRandom returns 0 to n-1 and we need 1-6
        dice[rnum]++;
    }
}

int setDice(DiceGroup dice, int data) {
    clearArray(dice);
    int tempdata = data;
    while (tempdata != 0) { //this loop is horrible however it checks for invalid data correctly
        if (tempdata % 10 >= 7 || tempdata % 10 == 0) {
            return 0;
        }
        tempdata /= 10;
    }
    
    while (data != 0) { //this loop could be combined with the above loop however that takes too much brainpower currently
        int num = data % 10;
        dice[0]++;
        dice[num]++;
        data /= 10;
    }
    return 1;
}

int testFarkle(const DiceGroup dice) {
    if (dice[1] > 0 && dice[2] > 0 && dice[3] > 0 && dice[4] > 0 && dice[5] > 0 && dice[6] > 0) {
        return 0;
    }
    
    if (dice[1] == 0 && dice[5] == 0 && dice[2] < 3 && dice[3] < 3 && dice[4] < 3 && dice[6] < 3) {
        return 1;
    }
    
    return 0;
}

int scoreDice(const DiceGroup dice) {
    if (dice[1] > 0 && dice[2] > 0 && dice[3] > 0 && dice[4] > 0 && dice[5] > 0 && dice[6] > 0){
        return 1500; //straight case
    }
    
    //i hate everything to do with this section, it is horrible but it works
    
    DiceGroup tempdice;
    for (int i = 0; i < 7; i++) {
        tempdice[i] = dice[i];
    }
    
    int total = 0;
    if (tempdice[1] == 6) {
        return 2000;
    } else if(tempdice[1] >= 3) {
        total += 1000;
        tempdice[1] -= 3;
    }
    for (int i = 2; i < 7; i++) { ////i looked at numbers hard enough and i realized theyre all logically related for 2-7
        if (tempdice[i] == 6) {
            return 2*i*100;
        } else if (tempdice[i] >= 3) {
            total += i * 100;
            tempdice[i] -= 3;
        }
    }
    if (tempdice[1] > 0)
    {
        total += tempdice[1] * 100;
    }
    if (tempdice[5] > 0) {
        total += tempdice[5] * 50;
    }
    return total;
}

int selectDice(DiceGroup dice, DiceGroup keep, int choice) {
    setDice(keep, choice);
    
    for(int i = 0; i < 7; i++) {
        if(dice[i] - keep[i] < 0) {
            clearArray(keep);
            printf("No match, try again.\n");
            return 0;
        }
    }
    
    if(testFarkle(keep)) {
        clearArray(keep);
        printf("Must keep scoring dice. Try again.\n");
        return 0;
    }
    
    for(int i = 1; i < 7; i++) { //performs magic to dice part of array
        dice[i] -= keep[i];
        dice[0] -= keep[i];
    }
    return 1;
}

void clearArray(DiceGroup dice) { //this should be self explanatory
    for (int i = 0; i < 7; i++) {
        dice[i] = 0;
    }
}

// DO NOT CHANGE or ADD any code below this line
//-------------------------------------------------------------------------------------------
// RANDOM NUMBER FUNCTIONS -- DO NOT CHANGE BELOW THIS LINE
// Using 16-bit linear feedback shift register (LFSR) for pseudorandom number generator
unsigned int lfsr = 0;  // global variable -- state of random number generator

// lfsrNext -- function to advance an LFSR for pseudorandom number generation
// uses global variable (lfsr)
// code from: https://en.wikipedia.org/wiki/Linear-feedback_shift_register
unsigned int lfsrNext(void) {
    lfsr ^= lfsr >> 7;
    lfsr ^= lfsr << 9;
    lfsr ^= lfsr >> 13;
    return lfsr;
}

// set starting value for LFSR-based random number generator
void seedRandom(unsigned int seed) {
    if (seed) lfsr = seed;
}
// return a random integer between 0 and limit-1
unsigned int getRandom(unsigned int limit) {
    return lfsrNext() % limit;  //
}