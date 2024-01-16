/*
NAMES:        David O'Riley, Mitchell Hurst, Tyler Calderone
DATE:  		  11/22/2016
PROGRAM NAME: Deck.h
DESCRIPTION:  This header file creates a deck out of blackjack cards for use in a blackjack game. Also has the card shoe class that has the ability to make a shoe of more then one deck.
*/

#ifndef _STD_DECK
#define _STD_DECK

#include <iostream>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <ctime>
#include "PlayingCards.h"
using namespace std;

class StandardDeck
{
	protected:
		list<BlackJackCard> myList;  //list where all the cards are kept
	
		
	public:
		
		//constructors
			StandardDeck() {Initialize();} //default constructor 
			
			
		//mutator
			virtual void Initialize(); //mutator to initialize the deck
			BlackJackCard DrawNextCard(); //return card at the top of the deck
			void Shuffle();
			
		
		//accessor
			int CardsRemaining() const {return myList.size();} //return the size of the cards array
			string DebugString() ;
};

//set all the cards in the deck
void StandardDeck::Initialize()
{
	const int suitSize = 13; //size of a suit in a deck
	const int numberOfSuits = 4; //number of suits in a deck
	
	

	for(int i = 0; i < numberOfSuits; i++)
	{
		//loop for the cards set for that suit
		for(int j = 1; j < suitSize+1; j++)
		{
			BlackJackCard card(j,i);

			myList.push_back(card);

		}
	}
}

BlackJackCard StandardDeck::DrawNextCard()
{
	BlackJackCard card = myList.front(); //get the card at the front of the deck 
	myList.pop_front(); //remove the card at the top of the deck
	
	return card;
}

void StandardDeck::Shuffle()
{
	srand(time(NULL)); //seed the random generator 

	//assign the values of the list to a vector container
	vector<BlackJackCard> deck(myList.begin(), myList.end());
	//shuffle that vector container 3 times
	random_shuffle(deck.begin(), deck.end());
	random_shuffle(deck.begin(), deck.end());
	random_shuffle(deck.begin(), deck.end());
	//assign the values of that vector container back to the list 
	myList.assign(deck.begin(), deck.end());
}



string StandardDeck::DebugString() 
{
	list<BlackJackCard>::iterator it;
	int i = 1;
	stringstream debug;
	debug << "\n==================== DEBUG ====================" << endl
		  << "Size:  " << myList.size() << endl;
	for (it = myList.begin(); it != myList.end(); it++, i++) 
	{
		debug << "[" << i << "]: " << (string)*it << endl;
	}

	debug << "\n===============================================" << endl;
	return debug.str();
}


//class card shoe for using multiple decks
//inherits all the private
class CardShoe : public StandardDeck
{
	public:
		
	CardShoe() {Initialize(1);}
	CardShoe(int numberOfDecks){Initialize(numberOfDecks);}
	void Initialize(int numberOfDecks);
};

//parameter constructor
void CardShoe::Initialize(int numberOfDecks)
{
	
	const int suitSize = 13; //size of a suit in a deck
	const int numberOfSuits = 4; //number of suits in a deck
	if (myList.empty() == false)
	{
		myList.clear();
	}
	
	for (int deckAmount=0; deckAmount < numberOfDecks; deckAmount++)
	{
		for(int i = 0; i < numberOfSuits; i++)
		{
			//loop for the cards set for that suit
			for(int j = 1; j < suitSize+1; j++)
			{
				BlackJackCard card(j,i);
	
				myList.push_back(card);
	
			}
		}
	}
}

#endif
