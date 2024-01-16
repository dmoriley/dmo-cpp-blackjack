/*
NAMES:        David O'Riley, Mitchell Hirst, Tyler Calderone
DATE:  		  11/22/2016
PROGRAM NAME: game.h
DESCRIPTION:  Header file used to create a blackjack game class including the ability to hit. Also keeps track of the dealer and the 
			  computer players. It dictates how they will behave in the execution of the blackjack game.
*/

#ifndef GAMES
#define GAMES

#include <iostream>
#include <iomanip>
#include <list>
#include <vector>
#include <stdexcept>	// for invalid_argument
#include <exception>
#include <ctime>
#include <sstream>
#include "PlayingCards.h"
#include "Deck.h"
#include "Players.h"

using namespace std;

class BlackJack
{
	public:
		
		//static constants
		static const int MAX_COMPUTER_PLAYERS;
		static const double WALLET_START;
		static const int BLACK_JACK;
		static const vector<string> NAMES; //array of names for computer players
		static const int DEALER_STAND_VALUE;
		static const double MINIMUM_BET;
		
		//construtors
		BlackJack(): myComputerPlayers(), myPlayer1(), myDeck(){SetDealer();}
		BlackJack(string player1Name, int deckCount){myDeck.Initialize(deckCount); SetHumanPlayer(player1Name); SetDealer();}
		
		//mutators
		void SetHumanPlayer(string name);
		void SetDealer();
		void ShuffleDeck() { myDeck.Shuffle();}
		void SetComputerPlayers(int count);
		void StartingHand(Players& temp);
		void Hit(Players &temp);
		void SetPlayer1Wallet(double cash);
		void SetPlayer1Bet(double bet);
		void SetPlayerHands();
		string DealersTurn();
		string ComputerPlayersTurn();
		string ComputerPlayersLeaveTable();
		string ComputerPlayersFinishRound();
		void ComputerPlayersSetBet();
		Players& GetPlayer1() {return myPlayer1;}
		Players& GetDealer() {return myDealer;}
		CardShoe& GetDeck() {return myDeck;}	
		
		//accessors	
		string ComputerOutcome(Players& computer);
		string GetPlayer1Cards();
		string GetComputerCards(bool showFullHand);
		string GetComputerCardsBegin();
		string GetComputerOutcome();
		string GetDealerHand(bool showFullHand);
		CardShoe GetDeck() const {return myDeck;}
		int CheckPlayer1Hand() const;
		bool ComputersNotSet() const {return myComputerPlayers.empty();}

		

		
		
		
	private:
		list<Players> myComputerPlayers; //computer players if any
		Players myDealer;
		Players myPlayer1;
		CardShoe myDeck;
};

//constant declarations
 const int BlackJack::MAX_COMPUTER_PLAYERS = 6;
 const int BlackJack::BLACK_JACK = 21;
 const int BlackJack::DEALER_STAND_VALUE = 17;
 const double BlackJack::MINIMUM_BET = 25.0;
 const double BlackJack::WALLET_START = 300.00;
 const vector<string> BlackJack::NAMES = {"Tyler the Noble", "David the Brave", "Mitchel The Valient", "Deborah", "James", "Dillon", "Angela", "Tim", "Anthony", "Jane", "Malisa"};

//returns the player1 cards and the value total 
string BlackJack::GetPlayer1Cards()
{
	stringstream output;
	output << "#################" << endl
		   << "#    Player 1   #" << endl
		   << "#################" << endl << endl;
	
	output	<< myPlayer1.GetCardsString() << endl
			<< "Total: " << myPlayer1.GetCardsValue()
			<< "\n\t************\n\n";
	
	return output.str();
}

//if computer are set it will return each computer players cards and the 
//total of their cards
//takes boolean to show the full hand or hand less one card
string BlackJack::GetComputerCards(bool showFullHand)
{
	if(myComputerPlayers.empty() == true) //make sure players are set
		throw invalid_argument("Computer players is empty");
	
	stringstream output;
	
	output << "#################" << endl
		   << "# Other Players #" << endl
		   << "#################" << endl << endl;
	
	if(showFullHand)
	{
		list<Players>::iterator it = myComputerPlayers.begin();
		while(it != myComputerPlayers.end())
		{
				output 	<< (*it).GetName() << endl
					    << (*it).GetCardsString() << endl
						<< "Total: " << (*it).GetCardsValue() << endl
						<< ComputerOutcome(*it) << endl
						<< "\n\t************\n\n";
						it++;
		}
	}
	else
	{

	list<Players>::iterator it = myComputerPlayers.begin();
		while(it != myComputerPlayers.end())
		{
				output 	<< (*it).GetName() << endl
						<< (*it).GetCardsLessOne() << endl
						<< "\n\t************\n\n";
						++it;
		}
	
	}
	
	return output.str();
}

//finds out if the computer players won or lost their hand based on the
//total of their hand vs the dealer
//used specifically to enter computer results into the log file.
string BlackJack::GetComputerOutcome()
{
	if(myComputerPlayers.empty() == true) //make suer computer players are set
		throw invalid_argument("Computer players is empty");
	
	stringstream output;
	string outcome = "Computer Win";
	
		list<Players>::iterator it;
		//go through each computer player
		for(it = myComputerPlayers.begin(); it != myComputerPlayers.end(); it++)
		{	
			//deterine whether the current computer player won or lost
			if((*it).GetCardsValue() > BLACK_JACK)
			{
				outcome = "Computer Loss";
	
			}
			else if(myDealer.GetCardsValue() == (*it).GetCardsValue())
			{
				outcome = "Push!";
			}
			else if(myDealer.GetCardsValue() > (*it).GetCardsValue() && myDealer.GetCardsValue() <= BLACK_JACK)
			{
				outcome = "Computer Loss"; 
	
			}
		
			output 	<< (*it).GetName() << endl
							<< outcome << endl
							<< "\n\t************\n\n";
		}
				
			
		if((*it).GetWallet() < MINIMUM_BET) //if they have less then the minimum output player left the table
		{
			output << "\n***Player Left Table***" << endl;
		}
	return output.str();
}

//given a computer play look at there cards value and decide if the won or lost and return the outcome
//same as above but, also calculates wallet depending on wins and losses
string BlackJack::ComputerOutcome(Players& computer)
{
	string outcome = "Computer Win";
	double computerBet = computer.GetBet();
	double computerWallet = computer.GetWallet();
	double newWallet;
	
	
		if(computer.GetCardsValue() > BLACK_JACK)
		{
			outcome = "Computer Loss";
			newWallet = computerWallet - computerBet;
			computer.SetWallet((newWallet));
		}
		else if(myDealer.GetCardsValue() == computer.GetCardsValue())
		{
			outcome = "Push!";
		}
		else if(myDealer.GetCardsValue() > computer.GetCardsValue() && myDealer.GetCardsValue() <= BLACK_JACK)
		{
			outcome = "Computer Loss"; 
			newWallet = computerWallet - computerBet;
			computer.SetWallet((newWallet));
		}
		else if(myDealer.GetCardsValue() > BLACK_JACK) //if dealer draws greater then 21
		{
		
			newWallet = computerWallet + computerBet;
			computer.SetWallet((newWallet));
		}
		else //else dealer is less then the player
		{
			newWallet = computerWallet + computerBet;
			computer.SetWallet((newWallet));
		}
	
	return outcome;
}


void BlackJack::SetHumanPlayer(string name) //initilize player1
{
	myPlayer1.SetName(name);
	myPlayer1.SetWallet(WALLET_START);
}

void BlackJack::SetDealer() //initilize the dealer
{
	myDealer.SetName("Dealer");
	//have to give the dealer money because otherwise he wouldnt be dealt any cards
	//players with less then the minimum bet are not dealt cards.
	myDealer.SetWallet(10000.00);
}

//initilize the computer players with random names to the amount specified
void BlackJack::SetComputerPlayers(int count)
{	
	if( count > MAX_COMPUTER_PLAYERS)
		throw out_of_range("Too many computer players entered");
	
	if(myComputerPlayers.empty() == false) //check if there are alread any computer players then clear them
		myComputerPlayers.clear();

	vector<string> nameList;
	nameList.assign(NAMES.begin(),NAMES.end()); //get a copy of the names list
												//need copy because names are deleted
												//from the list once used
	
	srand(time(NULL)); //seed random 
	
	for(int i = 0; i < count; i++)
	{
		int randomNumber = rand() % nameList.size(); //get random number from zero to namelist size
		string name = nameList[randomNumber]; //get random name from the list
		nameList.erase(nameList.begin() + randomNumber); //erase the name that was used
		Players temp(name, WALLET_START); //create the random player
		myComputerPlayers.push_back(temp); //add player to the list
	}
	
}

//remove all cards and add 2 new cards to players hand
void BlackJack::StartingHand(Players& temp)
{
	const int startingHandCount = 2;
	
	if(temp.GetCardsValue() != 0) //check if current player has cards and clear them
		temp.RemoveCards();
	
	if(temp.GetWallet() >= MINIMUM_BET) //deal them cards only if they arent broke
	{
		for(int i = 0; i < startingHandCount; i++)
		{
			temp.SetCard(myDeck.DrawNextCard());
		}
	}
}

void BlackJack::SetPlayerHands()
{
	//set stating hand for the dealer and the player
	StartingHand(myPlayer1);
	StartingHand(myDealer);
	
	if(myComputerPlayers.empty() == false) //if computer players have been set
	{
		list<Players>::iterator it;
		for(it = myComputerPlayers.begin(); it != myComputerPlayers.end(); it++)
		{
			StartingHand(*it); //pass each player to the starting hand to get their first cards.
		}
	}
}

//output the dealers hand 
//boolean on whether to show the full hand or hand less one card
string BlackJack::GetDealerHand(bool showFullHand)
{
	stringstream output;
	
	if(showFullHand)
	{
		output << "#################" << endl
		  	   << "#    Dealer     #" << endl
		   	   << "#################" << endl << endl
		   	   << myDealer.GetCardsString() << endl
			   << "Total: " << myDealer.GetCardsValue();	
	}
	else //show dealer hand up to second last postision
	{
	output << "#################" << endl
		   << "#    Dealer     #" << endl
		   << "#################" << endl << endl
			<< myDealer.GetCardsLessOne();

	}
	
	return output.str();
}

//add a card to a players hand
void BlackJack::Hit(Players &temp)
{
	if (temp.GetCardsValue() == 0)
		throw out_of_range("Player cannot hit if they dont have any cards");
	
	temp.SetCard(myDeck.DrawNextCard());
}

int BlackJack::CheckPlayer1Hand() const
{
	/* This accessor will return 1 if the player card total value is under 21
	and is good to continue to play. 2 is the player hit 21 and 3 if the player
	busted and is over 21. */
	
	if (myPlayer1.GetCardsValue() == 0)
		throw out_of_range("Cannot check player hand if they dont have any cards");
	
	int returnValue = 1;
	
	if(myPlayer1.GetCardsValue() == 21)
	{
		returnValue = 2;	
	}
	else if(myPlayer1.GetCardsValue() > 21)
	{
		returnValue = 3;	
	}
	
	return returnValue;
}

//set the wallet of the player1
void BlackJack::SetPlayer1Wallet(double cash)
{
	myPlayer1.SetWallet(cash);
}

//set the player1bet
void BlackJack::SetPlayer1Bet(double bet)
{
	myPlayer1.SetBet(bet);
}

//the dealer does its turn deciding weather to hit or stay
string BlackJack::DealersTurn()
{	
stringstream returnValue;
	while(myDealer.GetCardsValue() < DEALER_STAND_VALUE)
	{
		Hit(myDealer);
		returnValue << "Dealer hit." << endl;
	}
	
	if (myDealer.GetCardsValue() > BLACK_JACK)
	{
		returnValue << "Dealer busted." << endl;
	}
	else
	{
	    		    	
	    	returnValue << "Dealer stands." << endl;
	}
return returnValue.str();
}

//cycle through each computer player and decide if they should hit or stay
//if computer player cards is 0 means they ran out of money so dont deal them
string BlackJack::ComputerPlayersTurn()
{
	stringstream returnValue;
	list<Players>::iterator it;
	for(it = myComputerPlayers.begin(); it != myComputerPlayers.end(); it++)
	{ 
		//checks if they are less then 17 and have cards
		//if they have no cards they cant hit
		if((*it).GetCardsValue() < DEALER_STAND_VALUE && (*it).GetCardsValue() != 0)
		{
			Hit(*it);
			returnValue << (*it).GetName() << " hit." << endl; 
		}
		else if ((*it).GetCardsValue() > BLACK_JACK)
		{		
		    returnValue << (*it).GetName() << " busted." << endl;
	    }
	    else
	    {
	    		    	
	    	returnValue << (*it).GetName() << " stands." << endl;
	    }
	}
	return returnValue.str();
}

//player1 busts or gets blackjack, computer players finish theyre turn.
string BlackJack::ComputerPlayersFinishRound()
{
	stringstream returnValue;
	list<Players>::iterator it;
	for(it = myComputerPlayers.begin(); it != myComputerPlayers.end(); it++)
	{
		if((*it).GetCardsValue() != 0 && (*it).GetCardsValue() < BLACK_JACK) //if the player has cards, thus still playing
		{
			while((*it).GetCardsValue() < DEALER_STAND_VALUE) //continue hitting until player reaches 17 or above
			{
				Hit(*it);
				returnValue << (*it).GetName() << " hit." << endl; 	
			}	
		}
		else if ((*it).GetCardsValue() > BLACK_JACK)
		{		
		    returnValue << (*it).GetName() << " busted." << endl;
	    }
	    else
	    {
	    		    	
	    	returnValue << (*it).GetName() << " stands." << endl;
	    }
	}
	return returnValue.str();
}

//gets rid of a computer player when their wallet is less then the minimum bet
string BlackJack::ComputerPlayersLeaveTable()
{
	stringstream outputStatement;
	
		list<Players>::iterator it = myComputerPlayers.begin();
		while(it != myComputerPlayers.end())
		{
			if((*it).GetWallet() < MINIMUM_BET)
			{
				cout << (*it).GetName() << " left the table!" << endl;
				outputStatement << (*it).GetName() << " left the table!" << endl;
				myComputerPlayers.erase(it++);
				
			}
			else{
			++it;
		}
}
return outputStatement.str();
}

//cycle through each computer player and set they bet
//based on random number from minimum bet to their wallet value
void BlackJack::ComputerPlayersSetBet()
{
	srand(time(NULL));
	int bet;
	list<Players>::iterator it;
	for(it = myComputerPlayers.begin(); it != myComputerPlayers.end(); it++)
	{
		double maxBet = (*it).GetWallet();
		
		if(maxBet >= MINIMUM_BET)
		{
			bet = 1 + (rand() % (int)maxBet);
			
			if(bet < MINIMUM_BET) //if the random number is below 25 make it 25
				bet = MINIMUM_BET;
			
			(*it).SetBet((double)bet);
		}
			
	}
}

//gets all card information except the outcome used specifically for the logfile
//if the getcomputercards function was used it would calculate the outcome
//which would throw and error...
string BlackJack::GetComputerCardsBegin()
{
	stringstream output;
	list<Players>::iterator it = myComputerPlayers.begin();
		while(it != myComputerPlayers.end())
		{

				output 	<< (*it).GetName() << endl
					    << (*it).GetCardsString() << endl
						<< "Total: " << (*it).GetCardsValue() << endl
						<< "\n\t************\n\n";
						it++;
		}
		return output.str();
}
#endif
