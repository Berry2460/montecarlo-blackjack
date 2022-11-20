#include <stdio.h>
#include <stdlib.h>

#define KING 10
#define QUEEN 10
#define JACK 10
#define ACE 11
#define ACE_LOW 1

#define CARDS 13

#define BLACKJACK 21
#define HAND_BLACKJACK 5
#define DEALER_HOLD 17

#define GAMES 10000

typedef struct Hand{
	int cards[5];
	int size;
}Hand;

int deck[CARDS]={2, 3, 4, 5, 6, 7, 8, 9, 10, JACK, QUEEN, KING, ACE};

int evalHand(Hand hand);
int doRoutine(Hand *hand, int hold);
int win(Hand player, Hand dealer);
int grabCard(Hand *hand);
Hand newHand();

int evalHand(Hand hand){
	int eval=0;
	for (int i=0; i<hand.size; i++){
		eval+=hand.cards[i];
	}
	if (eval > BLACKJACK){
		for (int i=0; i<hand.size; i++){
			if (hand.cards[i] == ACE){
				eval-=ACE-ACE_LOW;
				if (eval <= BLACKJACK){
					break;
				}
			}
		}
	}
	if (eval < BLACKJACK && hand.size == HAND_BLACKJACK){
		eval=BLACKJACK;
	}
	return eval;
}

int doRoutine(Hand *hand, int hold){
	int allow=0;
	if (evalHand(*hand) < hold){
		allow=grabCard(hand);
	}
	int out=(evalHand(*hand) < hold && allow);
	return out;
}

int win(Hand player, Hand dealer){
	int out=(evalHand(player) <= BLACKJACK && (evalHand(dealer) > BLACKJACK || evalHand(player) > evalHand(dealer)));
	return out;
}

int grabCard(Hand *hand){
	if (hand->size < HAND_BLACKJACK){
		hand->cards[hand->size]=deck[rand()%CARDS];
		hand->size++;
	}
	int out=(hand->size < HAND_BLACKJACK);
	return out;
}

Hand newHand(){
	Hand hand;
	hand.size=2;
	hand.cards[0]=deck[rand()%CARDS];
	hand.cards[1]=deck[rand()%CARDS];
	return hand;
}

int main(){
	printf("Monte Carlo algorithm for finding the best Blackjack hand to hold on!\n");
	printf("Playing %d games per interval...\n\n", GAMES);
	for (int i=KING; i<=BLACKJACK; i++){
		int wins=0;
		int ties=0;
		for (int j=0; j<GAMES; j++){
			Hand dealer=newHand();
			Hand player=newHand();
			while (doRoutine(&dealer, DEALER_HOLD)){};
			while (doRoutine(&player, i)){};
			if (win(player, dealer)){
				wins++;
			}
			else if (evalHand(player) == evalHand(dealer)){
				ties++;
			}
		}
		printf("won %d%% of games by holding on %d and tied %d%% of games.\n", (int)((float)wins/GAMES * 100), i, (int)((float)ties/GAMES * 100));
	}
	return 0;
}
