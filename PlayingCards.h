/** StandardPlayingCard.h
 *	
 *	A class representing a standard playing card.   
 *
 *   
 *	@author		Thom MacDonald
 *	@version	2014.02
 *	@since		Oct 2014
 *	@see		Bronson, G. (2012).  Chapter 10 Introduction to Classes. 
 *				  In A First Book of C++ (4th ed.). Boston, MA: Course Technology.
 *	@see		Classes.StaticMembers.Solution.cpp
*/

#ifndef _STD_PLAYINGCARD
#define _STD_PLAYINGCARD

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <vector>
using namespace std;	

class StandardPlayingCard // Class declaration section
{
	public:	
		
		// Static data members: shared between all objects of the class
		
		static const string RANKS[]; // an array that holds the names of the ranks								
		static const string SUITS[]; // an array that holds the names of the suits
		static const int MIN_RANK;
		static const int MAX_RANK;
		static const int MAX_SUIT;
		
		// Constructor(s):
		// Default constructor using an initialization list
		StandardPlayingCard(): myRank(1), mySuit(1), isFaceUp(true) {}
		
		// Parameterized constructor
		StandardPlayingCard(int rank, int suit, bool faceUp = true);
		
		// Accessor(s):
		string getRankString() const {return RANKS[myRank];}
		int getRank() const {return myRank;}
		int getSuit() const {return mySuit;} 
		string getSuitString() const {return SUITS[mySuit];} 
		operator string () const; // convert the obj to a string.
		friend ostream& operator<<(ostream& output, const StandardPlayingCard& card);

		
		// Mutator(s):
		virtual void setRank(int rank); // sets the rank number
		void setSuit(int suit); // sets the suit number
		bool flipCard() {return (isFaceUp = !isFaceUp);} // changes the face up state

		// Public member variables
		bool isFaceUp; // true if face-up, false if face-down
	
	// Private members are only accessible inside the class
	//	- typically member variables
	protected:
		int myRank; // value to represent the rank
		int mySuit; // value to represent the suit
		
		
}; // end of class declaration section

//card used for blackjack, contains the value of the card
class BlackJackCard : public StandardPlayingCard
{
	public:
		
		BlackJackCard(): StandardPlayingCard(), myValue(1) {}
		BlackJackCard(int rank, int suit) : StandardPlayingCard(rank,suit) {setRankValue(rank);}
		
		void setRank(int rank) override; //override the set rank function
		void setRankValue(int rank); //private method to set the value of the card
		void setValue(int value); //used to reset the value of a card, used for ace only
		
		int getValue() const{return myValue;}
		int getRank() const {return myRank;}
		friend ostream& operator<<(ostream& output, const BlackJackCard& card);
		operator string () const; 
	
	private:
		
		int myValue; // represents the value of a card in blackjack
};


// Class defintion section

// Static data member initialization
const string StandardPlayingCard::SUITS[] = { "Spades", "Hearts", "Diamonds", "Clubs" };
const string StandardPlayingCard::RANKS[] = { "Not Used", "Ace", "Two", "Three", "Four", "Five", 
								"Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King"};
const int StandardPlayingCard::MIN_RANK = 1;
const int StandardPlayingCard::MAX_RANK = 13;
const int StandardPlayingCard::MAX_SUIT = 3;

//if the rank is 10 or greater then the value is 10. Otherwise the value is the rank.
void BlackJackCard::setRankValue(int rank)
{
	if(rank >= 10 && rank <= 13)
	{
		myValue = 10;
	}
	else
	{
		myValue = rank;
	}
}

//setRank overriding the base class
void BlackJackCard::setRank(int rank)
{
	StandardPlayingCard::setRank(rank); //set the rank using the base class mutator
	setRankValue(rank); //then set the value of the card using the same rank value passed
}


ostream& operator<<(ostream& output, const BlackJackCard& card)
{
	if(card.isFaceUp)
	{
		// build a descriptive string from the obj state
		output << card.getRankString() << " of " << card.getSuitString();
	}
	else // card is face-down
	{
		// string indicates face-down
		output << "Face-Down";
	}
	return output;
}

BlackJackCard::operator string () const
{
	stringstream output; 
	if(isFaceUp)
	{
		// build a descriptive string from the obj state
		output << getRankString() << " of " << getSuitString();
	}
	else // card is face-down
	{
		// string indicates face-down
		output << "Face-Down";
	}
	return output.str();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Parameterized Constructor for StandardPlayingCard class
 *  @param	rank: int (a number between 1 and 13)
 *  @param	suit: int (a number between 0 and 3)
 *  @param	faceUp: bool (true if face-up, false if not)
 *	@throws	out_of_range exception if rank or suit params are invalid
 */
StandardPlayingCard::StandardPlayingCard(int rank, int suit, bool faceUp)
{
	// Set the rank and suit with validation
	setRank(rank); 
	setSuit(suit);
	// set the face-up field
	isFaceUp = faceUp;
}

/** setRank Method for StandardPlayingCard class
 *	Sets the rank of this StandardPlayingCard object.
 *  @param	rank: int (a number between 1 and 13)
 *	@throws	out_of_range exception if the param is invalid
 */
void StandardPlayingCard::setRank(int rank)
{
	// if the rank parameter is not a valid rank number
	if(rank < MIN_RANK || rank > MAX_RANK)
	{
		// declare a stringstream object
		stringstream strOut; 
		// build a descriptive error string.
		strOut << "Rank argument: " << rank
			   << " is out of range. Rank must be between "
			   << MIN_RANK << " and " << MAX_RANK << ". ";
		// throw an out_of_range exception initialized with
		// the error string
		throw out_of_range(strOut.str());
	}
	else // otherwise, the rank parameter is fine
	{
		// assign the parameter to the member variable
		myRank = rank;
	}
}

/** setSuit Method for StandardPlayingCard class
 *	Sets the suit of this StandardPlayingCard object.
 *  @param	suit: int (a number between 0 and 3)
 *	@throws	out_of_range exception if the param is invalid
 */
void StandardPlayingCard::setSuit(int suit)
{
	// Local declarations 
	const int MIN_SUIT = 0; // Spades
	// if the suit parameter is not a valid suit number
	if(suit < MIN_SUIT || suit > MAX_SUIT)
	{
		// declare a stringstream object
		stringstream strOut; 
		// build a descriptive error string.
		strOut << "Suit argument: " << suit
			   << " is out of range. Suit must be between "
			   << MIN_SUIT << " and " << MAX_SUIT << ". ";
		// throw an out_of_range exception initialized with
		// the error string
		throw out_of_range(strOut.str());
	}
	else // otherwise, the rank parameter is fine
	{
		// assign the parameter to the member variable
		mySuit = suit;
	}
}

/** operator string() Method for StandardPlayingCard class
 *	Converts the obj to a string.
 *	@return	the obj state as a string
 */
StandardPlayingCard::operator string () const
{
	// declare a stringstream object
	stringstream strOut; 
	// if the card is face-up
	if(isFaceUp)
	{
		// build a descriptive string from the obj state
		strOut << getRankString() << " of " << getSuitString();
	}
	else // card is face-down
	{
		// string indicates face-down
		strOut << "Face-Down";
	}
	// return the string
	return strOut.str();
}

ostream& operator<<(ostream& output, const StandardPlayingCard& card)
{
	if(card.isFaceUp)
	{
		// build a descriptive string from the obj state
		output << card.getRankString() << " of " << card.getSuitString();
	}
	else // card is face-down
	{
		// string indicates face-down
		output << "Face-Down";
	}
	return output;
}

#endif
