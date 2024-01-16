/*
NAMES:        David O'Riley, Mitchell Hurst, Tyler Calderone
DATE:  		  11/22/2016
PROGRAM NAME: Players.h
DESCRIPTION:  Sets the values for a player in blackjack, including their name, current wallet, current bet, and their hand value.
*/

#ifndef _STD_PLAYERS
#define _STD_PLAYERS

#include <iostream>
#include <iomanip>
#include <queue>
#include <list>
#include <vector>
#include <stdexcept>	// for invalid_argument
#include <algorithm>
#include <ctime>
#include <sstream>
#include "PlayingCards.h"

using namespace std;

class Players
{		
	public:
		
		//constructors
		Players() { myName = ""; myWallet = 0; myBet = 0; myValue = 0;}
		Players(string name, double cash) {SetName(name); SetWallet(cash); myBet = 0; myValue = 0;}
		
		//accessors
		double GetWallet() const{return myWallet;}
		double GetBet() const{return myBet;}
		string GetName() const{return myName;}
		list<BlackJackCard> GetCards() const {return myCards;}		
		int GetCardsValue() const {return myValue;}
		friend ostream& operator<<(ostream& input, Players& player);
		
		//mutators
		void SetWallet(double cash);
		void SetBet(double bet);
		void SetCard(BlackJackCard);
		void RemoveCards(){myCards.clear();}
		void SetName(string name);
		void SetValue();
		string GetCardsString();
		string GetCardsLessOne();
		
	private:
		
		list<BlackJackCard> myCards;
		string myName;
		double myWallet;
		double myBet;
		int myValue;
};

//Function to set the bet value for the player
void Players::SetBet(double bet)
{
	//if the bet is negative or zero
	if (bet <= 0)
	{
		stringstream errorString;
		errorString << "Bet must be greater than 0 ";
		throw invalid_argument(errorString.str());
    }
    //if the bet is greater than the wallet allows
    else if (bet > myWallet)
    {
    	stringstream errorString;
		errorString << "Bet cannot be greater than your current wallet. You currently have $" << myWallet << " in your wallet";
		throw invalid_argument(errorString.str());
    }
    //set myBet to equal the bet
    else
    {
    	myBet=bet;
    }
}

//Function to set the wallet value for the player
void Players::SetWallet(double cash)
{
	//if the bet is negative or zero
	if (cash < 0)
	{
		stringstream errorString;
		errorString << "The amount of cash in the wallet must be greater than or equal to 0. Player " << myName << "    " << cash << " " << myWallet << "  " << myBet << " " << myValue;
		throw invalid_argument(errorString.str());
    }
    //Sets myWallet to be equal to cash
    else
    {
    	myWallet=cash;
    }
}
//Sets the player name
void Players::SetName(string name)
{
	  myName=name;
}

//Sets the output format of the player
ostream& operator<<(ostream& out, Players& player)
{
	out << "\nPlayer Name: " << player.myName
        << "\nCurrent Wallet:     " <<setw(10) << "$" << player.myWallet
        << "\nCurrent Bet:          " <<setw(10) << "$" << player.myBet;
    return out;  	
}

//add a card to the player's hand
void Players::SetCard(BlackJackCard card)
{
	myCards.push_back(card);
	SetValue(); //set the value of the hand of cards	
}

//returns a string of all the cards the player has in possession
string Players::GetCardsString() 
{
	//Iterator to cycle through the cards in the player's hand
	list<BlackJackCard>::iterator it;
	stringstream output;
	
	output << "Player " << GetName() << "'s Cards: " << endl;
	
	//Outputs the current hand of cards for the player
	for(it = myCards.begin(); it != myCards.end(); it++)
	{
		output << (string)*it << endl;
	}
	
	//returns the stringstream output
	return output.str();
}

//return all the cards of the player less the last cards
string Players::GetCardsLessOne()
{
	stringstream output;
	//iterator to cycle through the cards in the player's hand
	list<BlackJackCard>::iterator it;
	//iterator to track the end of the myCards list minus 1 (to not displayed the flipped down card)
	list<BlackJackCard>::iterator j;
	j = myCards.end();
	j--;
	
	
	//cycle through list to second last spot
	for(it = myCards.begin(); it != j; it++)
	{
		output << (string)*it << endl;
	}
	output << "??? of ???" << endl;
	
	//returns the stringstream output
	return output.str();
}

//returns a value total of all the myCards the person has in possession 
void Players::SetValue() 
{
	const int blackJackValue = 21;
	list<BlackJackCard>::iterator it;
	int valueTotal = 0;
	int aceCounter = 0;

	//Cycle through each card in the player's hand
	for(it = myCards.begin(); it != myCards.end(); it++)
	{
		if( (*it).getRank() == 1) //increment ace counter if one is found
			aceCounter++;
			
		valueTotal += (*it).getValue();
	}
	
	if(aceCounter != 0) //if an ace or more was found
	{
		/* Even if more then one ace is found, onle one can be of value 11, otherwise
		they will automatically be over 21. 11 x 2 = 22. Thus only one ace can be premoted
		If the current total plus 10 is less then or equal to 21 then add the remaining 10
		to the value for the premotion of one ace.
		*/
		if((valueTotal + 10) <= blackJackValue)
			valueTotal += 10;
	}
	
	myValue = valueTotal;
}


#endif
