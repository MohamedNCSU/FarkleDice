// ECE 209 - Program 2: Farkle
// This program allows multiple users to play a dice game named Farkle.
// Rules of the game: https://www.dicegamedepot.com/farkle-rules/
// Number of players: 2 to 4
// Mohamed Ali

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

int takeTurn();   // interact with user to roll dice and return score
void rollDice(DiceGroup, int);     // initialize a specified number of dice
void printDice(const DiceGroup);   // print dice as a sorted sequence of digits
int selectDice(DiceGroup dice, DiceGroup keep, int choice);   // move dice from initial set to "keep" set
int scoreDice(const DiceGroup);    // return highest score available for a set of dice
int setDice(DiceGroup, int);       // initialize dice using an integer representation
int testFarkle(const DiceGroup);   // return 1 if no scoring dice
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

    while (!winner) {
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
void printDice(const DiceGroup dice){ // print dicegroup array in order
    int i;
    for(i=0; i<dice[1]; i++){ // checks how many dice have face value 1, repeats for following for loops
        printf("1");
    }
    for(i=0; i<dice[2]; i++){
        printf("2");
    }
    for(i=0; i<dice[3]; i++){
        printf("3");
    }
    for(i=0; i<dice[4]; i++){
        printf("4");
    }
    for(i=0; i<dice[5]; i++){
        printf("5");
    }
    for(i=0; i<dice[6]; i++){
        printf("6");
    }
    }

    void rollDice(DiceGroup dice, int n){ // generates number of dice to be put in array
        int num;
        dice[0]=n; // 0 element is number

        for(int i=1; i<7; i++){ // initialize array to 0
            dice[i]=0;
        }

        for (int i=0; i<n; ++i){
            num = getRandom(6)+1;
            dice[num] +=1; // puts number of each dice in array using generator
        }
    }

    int setDice(DiceGroup dice, int data){ // checks validity of int data, if valid puts into array
    int tmp;
    int val;
    int digits;
    digits = 0;
    val = data;

    while (val > 0) {
        digits += 1;  // >0 means at least one non-zero digit
        val /= 10;    // decimal shift to the right
    }

    for(int i=0; i<7; i++){ // initialize dice to 0
        dice[i]=0;
    }
    for(int i=0; i<digits; i++){

    tmp = data % 10; // divide and mod to separate digits
    data = data/10;
    if(((tmp<1) || (tmp>6))){ // checks validity of value passed
        return 0;
    }
    else{
    dice[tmp] +=1; // if valid, increments corresponding element of array
    }
    }

    dice[0] = digits; // 0th element is number of dice which is number of digits
    return 1;
}

int takeTurn(){ // combines all functions to implement game
    int farkle; // declaring variables
    int user;
    int selectd;
    int keep[7];
    int temp[7]; // used to keep track of dice array incase changes occur
    int scored;
    int round =1;
    int rolling =6;
    char ans;
    int tempscore = 0;
    int legal = 1;


    DiceGroup dice = {0}; // initialize array
    rollDice(dice, 6);

    while(round>0){                 // loop to indicate number of rounds
        printf("Rolling %d dice...",rolling);
        printDice(dice); // prints dice in order
        printf("\n");

        farkle = testFarkle(dice); // called to check if there is a farkle
        if(farkle == 1){
            printf("FARKLE -- your turn is over.\n");
            return 0;
        }
        else{
            while(legal>0){ //loop to check if keep value is legal
                printf("Which to keep? ");


                scanf("%d", &user);


                for(int i = 0; i<7; i++){
                    temp[i]=dice[i];    // initialize temp incase keep user value is not legal
                }

                selectd = selectDice(dice, keep, user); // call selectDice to put values into keep array
                scored = scoreDice(keep); // passes keep to scoreDice to determine score

                if(selectd==0){
                    printf("No match, try again.\n");
                    continue; // go back to loop to check if legal
                }
                else if((keep[0]==0) || (scored == 0)){ // checks if there is no scoring dice
                    printf("Must keep scoring dice. Try again.\n");
                    for(int i = 0; i<7; i++){
                        dice[i]=temp[i]; // if there is no scoring dice, restore dice array
                    }
                    continue; // go back to beginning of the loop
                }
                break; // if all is legal, break out
            }
        }

        scored = scoreDice(keep); //determines score from keep array

        setDice(dice, user); //puts user values into dice

        printf("Keeping ");
        printDice(dice); //prints number of each dice value in order

        printf(", score = %d\n", scored);
        tempscore = tempscore + scored; // calculates total score for each iteration

        printf("Score so far = %d\n", tempscore);
        if(rolling<2){ // checks how many dice are left over to indicate hot dice
            printf("HOT DICE!  Roll 6 dice (y/n)? ");
            scanf(" %c", &ans);
            if(ans=='y'){
                rolling = 6;
                rollDice(dice, 6); //call roll dice again to put values in dice array
            }
            continue;
        }

        rolling=temp[0]-keep[0]; // indicates number of dice left
        printf("%d dice left -- roll again (y/n)? ", rolling);

        scanf(" %c", &ans);
        if(ans=='n'){
            round -=1;
        }

        else{
            rollDice(dice,rolling); // if user rolls again, pass rolling as number of rolls
          round = 1;
        }

    }
        return tempscore; // returns the score

}

int selectDice(DiceGroup dice, DiceGroup keep, int choice){
    int tmp;
    int val;
    int num=0;
    int digits;
    int valid[7]={0}; // initialize array to 0, used to restore dice values
    digits = 0;
    int check;
    val = choice;
    for(int i=0; i<7; i++){
        valid[i]=dice[i];
    }

    while (val > 0) {
        digits += 1;  // >0 means at least one non-zero digit
        val /= 10;    // decimal shift to the right
    }
    check = dice[0]; // check is number of dice in dice array

    for(int i=0; i<7; i++){ // initialize keep array
        keep[i]=0;
    }
    for(int i=0; i<digits; i++){

        tmp = choice % 10; // similar to setDice function
        choice = choice/10;
        if(((tmp<1) || (tmp>6))){
            for(int i=0; i<7; i++){
                dice[i]=valid[i];
            }
            return 0; // if invalid, return 0
        }
        else if(dice[tmp]<1){
            for(int i=0; i<7; i++){
                dice[i]=valid[i]; // restores array if invalid
            }
            return 0;
        }
        else{
            keep[tmp] +=1; // add to keep array
            dice[tmp] -=1; // remove from dice array
            check -=1; // indicates number of dice
            num +=1; // indicates number of dice
        }
    }
    for(int i=1; i<7; i++){
        if(dice[i]>0){

        }
    }
    keep[0]=num; // 0th element is number of dice
    dice[0]=check;

    return 1;
}

int scoreDice(const DiceGroup dice){ // function indicates score
    int total=1;
    int j=0;
    if(((dice[1]<3) && (dice[1]!=0)) || ((dice[1]>3) && (dice[1]<6))){ // checks for 1 or triples
        if(dice[1]>3){
            total = ((dice[1]-3)*100); // adds 100 for each 1 greater than 3
        }
        else{
            total = dice[1]*100; // adds 100 for each 1
        }
    }
    if(((dice[5]<3) && (dice[5]!=0)) || ((dice[5]>3) && dice[5]<6)){ // checks for 5 or triples
        if(total==1){
            total=0;
        }
        if(dice[5]>3){
            total = total+((dice[5]-3)*50); // ads 50 for each 5 greater than 3
        }
        else{
            total = total+(dice[5]*50); // adds 50 for each 5
        }
    }
    if((dice[1]>=3)){ // checks for triples of 1s
        if(total ==1){
            total=0;
            total+=1000;
        }
        else if(total>1){
            total = total+(1*1000);
        }
    }
    if((dice[2]>=3)){ // checks for triples of twos
        if(total ==1){ // if this is first triple, initialize total to 0 and add corresponding score value
            total=0;
            total+=200;
        }
        else if(total>1){
            total = total+(1*200);
        }
    }
    if((dice[3]>=3)){ // checks for triples of threes
        if(total ==1){
            total=0;
            total+=300;
        }
        else if(total>1){
            total = total+(1*300);
        }
    }
    if((dice[4]>=3)){ // checks for triples of fours
        if(total ==1){
            total=0;
            total += 400;
        }
        else if(total>1){
            total = total+(1*400);
        }
    }
    if((dice[5]>=3)){ // checks for triples of fives
        if(total ==1){
            total=0;
            total+=500;
        }
        else if(total>1){
            total = total+(1*500);
        }
    }
    if((dice[6]>=3)){ // checks for triples of sixes
        if(total ==1){
            total=0;
            total +=600;
        }
        else if(total>1){
            total = total+(1*600);
        }
    }

    if(total==1){
        total=0;
    }
    for(int i=1; i<7; i++){ // if there is two triples, double score
        if(dice[i]==6){
            total = total*2;
        }
    }
    for(int i=1; i<6; i++){ // checks for straight 1-2-3-4-5
        if(dice[i]==dice[i+1]){
            ++j;
        }
    }
    if(j==5){
        total = 1500; // appropriate score value for a straight
    }

    return total; // return score
}


int testFarkle(const DiceGroup dice){ // function tests for farkle
    int i;
    for(i=1; i<7; i++){
        if((dice[i]>=3) || (dice[1]>0) || (dice[5]>0)){ // if there is a triple,1, or 5, there is no farkle
            return 0;
        }
    }
    return 1; // else it is a farkle, return 1
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

