#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

// Global constants
const int SUIT_NUM = 4;
const int MAX_CARD_NUM = 13;
const int BLACKJACK = 21;
const int DEALER_LIMIT = 17;

// Start up Header - BLACKJACK
void displayMenu(){
    cout << "BLACKJACK" << endl;
    cout << "=========" << endl;
}

string suitTypeRandom();

string cardTypeRandom(int cardNum);

void displayCard(int card);

int getCard();

int cardScore();

int deal(bool isPlayer, bool isShown);

int playHand();

int check21(int playerScore, int dealerScore);

int checkCardBust(int playerScore, int dealerScore);

int checkGreaterScore(int playerScore, int dealerScore);

char menuResponse();

int totalStats(int win, int loss, int draw);

string playerPrompt(string playerResponse);


int main(){

    // Get random number based on local time
    srand(time(nullptr));

    // Display Menu Header
    displayMenu();

    // Interactive Menu
    char response = ' ';

    int win = 0;
    int loss = 0;
    int draw = 0;

    response = menuResponse();

    // Looping playHand to play blackjack
    while (response == 'y'){
        
        // plays blackjack and calculates score
        int score = playHand();

        if (score == 1){ // add to win if playhand() returns 1
            win++;
        } else if (score == -1){ // add to loss if playhand() returns -1
            loss++;
        } else if (score == 0){ // add to draw if playhand() returns 0
            draw++;
        }

        // If not y, menuResponse will prompt to continue and totalStats();
        response = menuResponse();

        if (response != 'y'){

            return totalStats(win, loss, draw);
        }
    }

    totalStats(win, loss, draw); // if player quits right away
}

/*---------- PART 1 ----------- */
// Get random Number between 1 to 4 for Suit type --> displayCard
string suitTypeRandom(){
    
    int suitType = rand() % SUIT_NUM + 1;

    // Suit Type Cases
    if (suitType == 1){
        return "Spades";
    } else if (suitType == 2){
        return "Hearts";
    } else if (suitType == 3){
        return "Clubs";
    } else {
        return "Diamonds";
    }
}

// Get random Number between 1 to 13 for Card type --> displayCard
string cardTypeRandom(int cardNum){

    // Card Number Cases     
    if (cardNum == 1){
        return "Ace of ";
    } else if (cardNum == 2){
        return "Two of ";
    } else if (cardNum == 3){
        return "Three of ";
    } else if (cardNum == 4){
        return "Four of ";
    } else if (cardNum == 5){
        return "Five of ";
    } else if (cardNum == 6){
        return "Six of ";
    } else if (cardNum == 7){
        return "Seven of ";
    } else if (cardNum == 8){
        return "Eight of ";
    } else if (cardNum == 9){
        return "Nine of ";
    } else if (cardNum == 10){
        return "Ten of ";
    } else if (cardNum == 11){
        return "Jack of ";
    } else if (cardNum == 12){
        return "Queen of ";
    } else {
        return "King of ";
    }
}

// Print out Initial Cards Dealt
void displayCard(int card){
    cout << cardTypeRandom(card) << suitTypeRandom() << endl;  
}

/*----------- Part 2 ------------*/
// returns a card (value) from random numbers 1 to 13
int getCard(){
    return rand() % MAX_CARD_NUM + 1;
}

/*----------- Part 3 -------------*/
// Determines the cards score
int cardScore(int card){
    
    if (card > 1 && card <10){ // Numbered Cards
        return card;
    } else if (card > 9 && card < 14){ // Face Cards K, Q, J
        return 10;
    } else { 
        return 11; // Ace
    }
}

/*----------- Part 4 -------------*/
// Determine who the program is dealing to 
int deal(bool isPlayer, bool isShown){
    
    int cardDealt = getCard();
    string aceChoice = " ";

    // If it is the player's turn
    if (isPlayer){
        cout << "PLAYER was dealt ";
        displayCard(cardDealt);

        // Change Ace value
        if (cardDealt == 1){
            cout << "\nYou have been dealt an Ace. Do you want to change the value to 1 or keep it the same at 11?\n"
                    << "You can't change the card value later so choose wisely...\n"
                    << "Enter \"y\" to change, enter anything else to keep the value. "; 
            cin >> aceChoice;
            cout << endl;

            if (aceChoice == "y"){
                return 1;
            }
        }
        return cardScore(cardDealt);
    } 
    
    // If the dealer card is shown (First Card)
    else if (isShown){
        cout << "DEALER was dealt ";
        displayCard(cardDealt);
        return cardScore(cardDealt);
    } 
    
    else { // The dealer card not shown (Second card)
        cout << "DEALER was dealt a hidden card";
        return cardScore(cardDealt);
    } 
}

/*------------ Part 5 ---------------*/
int playHand(){ // Main function of playing ONE game of BlackJack

    // Initializing for scores and dealing situation
    int playerScore = 0;
    int dealerScore = 0;
    bool isPlayer = 1;
    bool isShown = 1;
    
    // Initial four cards dealt
    playerScore += deal(isPlayer, !isShown);
    dealerScore += deal(!isPlayer, isShown);
    playerScore += deal(isPlayer, !isShown);
    
    // Hidden Card Score - Intial Deal
    int hiddenCard = dealerScore;
    dealerScore += deal(!isPlayer, !isShown);

    // Display Score
    cout << "\n\nPLAYER score = " << playerScore << endl;
    cout << "DEALER score = " << hiddenCard << endl;

    if (dealerScore == 21){ // Reveals hidden card if dealer has natural blackjack
        cout << "\nHidden card was revealed!\n" << "\nDEALER score = " << dealerScore << endl;
    }

    // Check initial card score for natural 21
    int blackjack = check21(playerScore, dealerScore);

    // Return a value back to main for if blackjack is true
    if (blackjack != 2){
        if (blackjack == 1){ //return 1 if player gets blackjack
            return 1;
        } else if (blackjack == -1){ // return -1 if dealer gets blackjack
            return -1;
        } else { // return 0 if both dealer and player draw blackjack
            return 0;
        }
    } 

    /* ------------ Player's Turn ------------------- */
    while (playerScore <= BLACKJACK){
        
        string playerResponse = " ";

        playerResponse = playerPrompt(playerResponse); // call for hit or stand prompt

        /* ------ Go to DEALER if STAND ------ */
        if (playerResponse == "s" || playerResponse == "S"){
            break;
        } 

        /* -------- Continue if HIT ---------- */    
        playerScore += deal(isPlayer, !isShown); 
        cout << "\nPLAYER score = " << playerScore << endl;
        cout << "DEALER score = " << hiddenCard << endl;

        // Check if player has a blackjack (unatural)
        if (playerScore == BLACKJACK){
            cout << "\nPLAYER got a BlackJack! Now it's DEALER's turn!\n";
            break;
        }
    }

    // Check for if player Busts
    int isBust = checkCardBust(playerScore, dealerScore);
    if (isBust != 2){
        if (isBust == -1){ // if isBust == 1, then player busted
            return -1;
        }
    }

    // Reveal hidden card and display new score
    cout << "\nDEALER reveals its second card!\n";
    cout << "\nPLAYER score = " << playerScore << endl;
    cout << "\nDEALER score = " << dealerScore << endl;

    /* ---------------- DEALER's Turn --------------- */
    if (dealerScore < DEALER_LIMIT){

        while (dealerScore < DEALER_LIMIT){ 
        
            // Deal card to dealer, add score, and print out score
            dealerScore += deal(!isPlayer, isShown);
            cout << "\n\nPLAYER score = " << playerScore << endl;
            cout << "DEALER score = " << dealerScore << endl; 
        }
    }

    // Check if dealer busts
    isBust = checkCardBust(playerScore, dealerScore);
    if (isBust !=2){
        // If dealer bust, player wins, return 1
        if (isBust == 1){
            return 1;
        }
    } 
    
    // [Determines winner if no blackjack and bust]
    return checkGreaterScore(playerScore, dealerScore);
}

/* ------ Fuctions to check win/loss/draw conditions -------- */
// Checks for initial and dealt card score = 21
int check21(int playerScore, int dealerScore){
    
    if (dealerScore == BLACKJACK && playerScore == BLACKJACK){ // if dealer and player both have blackjack, then draw

        cout << "\nDraw! Both PLAYER and DEALER got BlackJack!\n";
        return 0;
    } else if (playerScore == BLACKJACK){  // if player gets blackjack, then win

        cout << "\nYou got a BlackJack! You win!\n";
        return 1;
    } else if (dealerScore == BLACKJACK) {  // if dealer gets black jack, then loss

        cout << "\nDEALER got a BlackJack! You lose!\n";
        return -1;
    } 
    return 2;
}

// Checks for BUST
int checkCardBust(int playerScore, int dealerScore){
    
    if (playerScore > BLACKJACK){   // return -1 if player bust
        
        cout << "\nBust! Dealer wins!\n";
        return -1;
    } else if (dealerScore > BLACKJACK){  // return 1 if dealer busts
        
        cout << "\nDEALER bust! You win!\n";
        return 1;
    }
    return 2;
}

// Checks score after the dealer stands (dealerScore >= 17)
int checkGreaterScore(int playerScore, int dealerScore){

    if (playerScore > dealerScore){ // return 1 if player score higher
        
        cout << "\nPLAYER score is higher, you win!" << endl;
        return 1;
    } else if (playerScore < dealerScore){  // return -1 if dealer score higher
       
        cout << "\nDEALER score is higher, you lose!" << endl;
        return -1;
    }
    // return 0 if dealer and player draw
    cout << "\nPLAYER and DEALER score are equal, you draw!" << endl;
    return 0;
}

// prompt user for response
char menuResponse(){
    
    char response = ' ';
    cout << "\nDo you want to play BlackJack? (press y to play, anything else to quit) ";
    cin >> response;
    cout << endl;

    return response;
}

// Display Total Stats Scoreboard
int totalStats(int win, int loss, int draw){

    // Prints out Overal Stats
    cout << "\nOVERALL STATS\n" << "===========" << endl;
    cout << "Wins: " << win << endl;
    cout << "Losses: " << loss << endl;
    cout << "Draws: " << draw << endl;
    cout << "\nOverall score = " << (win - loss) << endl;
    cout << "\nBetter luck next time! o7 o/"; // "o7" is a person saluting, "o/" is a person waving

    return 0;
}

// ask player to hit or stand
string playerPrompt(string playerResponse){

    cout << "\nPress h to hit, s to stand: ";
    cin >> playerResponse;
    cout << endl;

    // Looping 'h', 'H', 's', 'S' valid response
    while (playerResponse != "h" && playerResponse != "H" && playerResponse != "s" && playerResponse != "S") {
        cout << "Please enter a valid character ";
        cin >> playerResponse;
        cout << endl;
    }

    return playerResponse;
}
