/*
NAMES:        David O'Riley, Mitchell Hirst, Tyler Calderone
DATE:  		  11/22/2016
PROGRAM NAME: BlackJack.cpp
DESCRIPTION:  This is the mail program that will execute the blackjack game. It will use function.h for input validation and game.h to actually run the blackjack game.
*/

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <ctime>
#include "functions.h"
#include "Game.h" 

//function declaration
void PlayerLoss(BlackJack& game);
void PlayerWin(BlackJack& game);
char ValidChar();
char PlayAnother();
char HitStandDouble(double bet,double wallet);
int ValidDeck();

int main()
{

	//variable declaration	
	int deckAmount;
	char computer;
	int numberOfComputers;
	string name;
	const int MIN_COMPUTERS = 1;
	const int MAX_COMPUTERS = 4;
	const int CARDS_REMAINING_MIN = 35;
	const string LOG_FILE_NAME = "GameplayLog.txt";
	ofstream logFile; //creates and opens the filestream
	
	double bet = 0;
	char choice;
	int handOutcome;
	bool continuePlaying = true;
	double betMax;
	bool fundsAvailable = true;
	string player1Outcome;
	
	//head output
	cout << "===================================" << endl
	 	 << "===================================" << endl
	 	 << "=             BLACKJACK           =" << endl
	 	 << "===================================" << endl
	 	 << "===================================" << endl
		 << "#                                 #" << endl
		 << "#                                 #" << endl
		 << "#                                 #" << endl
		 << "#                                 #" << endl
		 << "#             Welcome             #" << endl
		 << "#                                 #" << endl
		 << "#    Please Enter Your Name       #" << endl
		 << "#                                 #" << endl
		 << "#                                 #" << endl
		 << "###################################" << endl;
		 
	
	//get the user name and numbe of decks
	cout << "\nPlease enter your name: ";
	getline(cin,name);
	cout << "\nHow many decks do you with to play with(2/4/6/8)? : ";
	deckAmount = ValidDeck(); 
	cout << "\nDo you wish to play with any computer players?(Y/N) : ";
	
	do
	{
		computer = ValidChar();
		if(computer != 'Y' && computer != 'N')
			cout << "Please enter either Y or N";
		
	}while(computer != 'Y' && computer != 'N'); //if the user doesnt enter Y or N will continue to loop 
	
	BlackJack game1(name, deckAmount); //initilize the blackjack game
	game1.GetDeck().Shuffle(); //shuffle the deck
	
	if(computer == 'Y') //if Y set the comupter players otherwise do nothing
	{	
		stringstream message;
		message << "\nNumber of computer players must be between " << MIN_COMPUTERS << " and " << MAX_COMPUTERS; //message to be passed to getvaldintgercustom
		cout << "\nPlease enter a number of player between 1 - 4 inclusive: ";
		//function to get whole number
		numberOfComputers = functions::GetValidIntegerCustom(MIN_COMPUTERS,MAX_COMPUTERS, message.str());
		game1.SetComputerPlayers(numberOfComputers); //initilize the computer players
	}

		
	try
	{
		
		do
		{
			
			//check if the deck is less then 35 cards reinitilize and reshuffle deck.
			if(game1.GetDeck().CardsRemaining() < CARDS_REMAINING_MIN)
			{
				game1.GetDeck().Initialize(deckAmount);
				game1.GetDeck().Shuffle();
			}

			
			bool playerTurn = true;
			betMax = game1.GetPlayer1().GetWallet(); //return the amount of money player 1 has
			if(betMax < game1.MINIMUM_BET) //player doesnt have enough money to continue playing.
			{
				playerTurn = false;
				fundsAvailable = false;
				cout << endl
					 << "##############################" << endl
					 << "#                            #" << endl
					 << "#     Insufficient Funds     #" << endl
					 << "#                            #" << endl
					 << "#     Your Wallet $" << betMax  << endl
					 << "#                            #" << endl
					 << "#     Minimum Bet $25        #" << endl
					 << "#                            #" << endl
					 << "##############################" << endl;
					 
			}
			else
			{
				
				
				cout <<  "\n############################" << endl
					 << "# Current Wallet Amount: $" << betMax << endl //displays the player1 current wallet amount
					 << "############################" << endl;
					 
				stringstream message;
				message << "\nYou must place a minimum bet of " << game1.MINIMUM_BET << " and cannot bet more then whats in you wallet( $" << betMax << ")" << endl
						<< "Please place your bet: ";
				cout << "\nPlace your bet: ";
				//get a valid double from the function
				bet = functions::GetValidDoubleCustom(game1.MINIMUM_BET, betMax, message.str()); //get the players bet
				
				game1.GetPlayer1().SetBet(bet); //set the players bet
				
				//Computer Players set there bet
				game1.ComputerPlayersSetBet();
				
				game1.SetPlayerHands(); //deal the initial hand of cards to everyone
			
			}
			
			//open the logfile 
			logFile.open(LOG_FILE_NAME, ios_base::app);
				if (logFile.fail()) //if it fails to open throw an error
				{
					stringstream errorString;
					errorString << "Cannot connect to file " << LOG_FILE_NAME << " to record info.";
					throw invalid_argument(errorString.str());
				}
				time_t currentTime = time(0); //get the current time
				
				char* dateTime = ctime(&currentTime); //get the current date using the current time
				
				logFile << "\nBLACKJACK GAMEPLAY LOG" << endl
		 		<< "Game Played on " << dateTime << endl
		 		<< "=========================\n" << endl;
				
		
			while(playerTurn) //if pllayer turn is true continue to loop through
			{
				
				 //check the outcome of the hand delt to the player
				handOutcome = game1.CheckPlayer1Hand();
				if(handOutcome == 1) // didnt bust and its not blackjack
				{
					
					if (game1.ComputersNotSet() == false) //if there are computer players display theyre hand less on card
					{
						cout << game1.GetComputerCards(false);
						logFile << game1.GetComputerCardsBegin() << endl; //output to logfile
						
					}
					cout << game1.GetPlayer1Cards() << endl; //display player hand and total	
						
					cout << endl << endl;
					//display the dealers hand less one card
					cout << game1.GetDealerHand(false);
					
					//output to the log file the dealer and player one's hands of cards
					logFile<< game1.GetPlayer1Cards() << endl
					<< game1.GetDealerHand(true) << endl;
			
					//get the choice checking if they can double
					choice = HitStandDouble(bet, game1.GetPlayer1().GetWallet());
					
					switch(choice)
					{
						case 'S': //player choose to stand

							
							if(game1.ComputersNotSet() == false)	//if there are computer players
								logFile << game1.ComputerPlayersTurn() << endl; //do the computer playres turn and output to logfile
							
							logFile << game1.DealersTurn() << endl; //do the dealers turn
							
							if(game1.GetPlayer1().GetCardsValue() > game1.BLACK_JACK) //if the player got a value greater then 21
							{
								player1Outcome = "You busted!\nDealer Wins!";
								PlayerLoss(game1);
							}
							else if(game1.GetDealer().GetCardsValue() == game1.GetPlayer1().GetCardsValue()) //if the dealer and player got the same value
							{
								player1Outcome = "Push!";
							}
							else if(game1.GetDealer().GetCardsValue() > game1.GetPlayer1().GetCardsValue() && game1.GetDealer().GetCardsValue() <= game1.BLACK_JACK)
							{  //if the dealer got more then the player and less then 21
								player1Outcome = "Dealer Won!"; 
								PlayerLoss(game1);
							}
							else if(game1.GetDealer().GetCardsValue() > game1.BLACK_JACK) //if dealer draws greater then 21
							{
								player1Outcome = "Dealer Busted You Win!";
								PlayerWin(game1);
							}
							else //else dealer is less then the player
							{
									
								player1Outcome = "You Win!";
								PlayerWin(game1);
							}
							playerTurn = false;
							break;
							
							
						case 'H': //player chose to hit
							game1.Hit(game1.GetPlayer1()); //give player 1 another card
							
							if(game1.ComputersNotSet() == false) //computers do there turn	
								logFile << game1.ComputerPlayersTurn() << endl;
							
							if(game1.GetPlayer1().GetCardsValue() > game1.BLACK_JACK) //if the player hit and busted
							{
								player1Outcome = "You busted!\nDealer Wins!";
								PlayerLoss(game1);
								logFile << game1.ComputerPlayersFinishRound() << endl;//computer player finish the round by going through there turns 
								logFile << game1.DealersTurn() << endl; //dealer does hit turn after computers have completed theirs
								playerTurn = false;
							}
							break;
							
							
						case 'D':
							//double players bet, draw anther card and go to the dealer turn
							bet = (game1.GetPlayer1().GetBet() * 2); //set the bet to doule what it was
							game1.GetPlayer1().SetBet(bet);
							
							game1.Hit(game1.GetPlayer1()); //give player 1 one more card
							
							//computers and dealer have their turn
							if(game1.ComputersNotSet() == false)	
								logFile << game1.ComputerPlayersTurn() << endl;
							logFile << game1.DealersTurn() << endl;
							
							//same as stand
							if(game1.GetPlayer1().GetCardsValue() > game1.BLACK_JACK)
							{
								player1Outcome = "You busted! Dealer Wins!";
								PlayerLoss(game1);
							}
							else if(game1.GetDealer().GetCardsValue() == game1.GetPlayer1().GetCardsValue())
							{
								player1Outcome = "Push!";
							}
							else if(game1.GetDealer().GetCardsValue() > game1.GetPlayer1().GetCardsValue() && game1.GetDealer().GetCardsValue() <= game1.BLACK_JACK)
							{
								player1Outcome = "Dealer Won!"; 
								PlayerLoss(game1);
							}
							else if(game1.GetDealer().GetCardsValue() > game1.BLACK_JACK) //if dealer draws greater then 21
							{
								player1Outcome = "Dealer Busted You Win!";
								PlayerWin(game1);
							}
							else //else dealer is less then the player
							{
								player1Outcome = "You Win!";
								PlayerWin(game1);
							}
							playerTurn = false;
							break;
						}
					}
					else if(handOutcome == 2) //player hit blackjack
					{
						//increase player1 wallet with a 3:2 multiplyer of their bet
						double playerWallet = game1.GetPlayer1().GetWallet();
						playerWallet += (game1.GetPlayer1().GetBet() * 1.5);
						game1.SetPlayer1Wallet(playerWallet);
						player1Outcome = "Black Jack! You Win!";
						
						if (game1.ComputersNotSet() == false) //if computer players set
							logFile << game1.ComputerPlayersFinishRound() << endl; //computer player finish the round
							
						logFile << game1.DealersTurn() << endl;//dealer does hit turn
						playerTurn = false;
					}		
						
			} //end of the players turn
			
			if (game1.ComputersNotSet() == false)
			{
			cout << game1.GetComputerCards(true); //display all of the computer players cards
			}
					
			cout << game1.GetPlayer1Cards() << endl //display player1 cards and whethe they won or not
				 << player1Outcome << endl;
						 
			cout << game1.GetDealerHand(true) << endl; //show all of the dealers cards
			
			logFile << "FINAL RESULTS" << endl << endl; //input the results of the game into the log file
			if (game1.ComputersNotSet() == false) 
			       logFile << game1.GetComputerOutcome() << endl; //get the computer outcomes if there are computers playing
			       
			       logFile << game1.GetPlayer1Cards() << endl
			       		   << player1Outcome << endl
			        	   << game1.GetDealerHand(true) << endl;
			
			if (game1.ComputersNotSet() == false)
				logFile << game1.ComputerPlayersLeaveTable() << endl;
			
			logFile.close(); //close the log file link
			
            betMax = game1.GetPlayer1().GetWallet(); //return the amount of money player 1 has
			if(betMax < game1.MINIMUM_BET) //player doesnt have enough money to continue playing.
			{
				playerTurn = false;
				fundsAvailable = false;
				cout << endl
					 << "##############################" << endl
					 << "#                            #" << endl
					 << "#     Insufficient Funds     #" << endl
					 << "#                            #" << endl
					 << "#     Your Wallet $" << betMax  << endl
					 << "#                            #" << endl
					 << "#     Minimum Bet $25        #" << endl
					 << "#                            #" << endl
					 << "##############################" << endl;
					 
			}
			if(fundsAvailable == true) //check to see if player has enough money to continue playing
			{
				cout << "\nPlay another hand(Y) or exit(E)? : ";
				choice = PlayAnother();
				
				if(choice == 'E')
					continuePlaying = false;
			}
			else
			{
				continuePlaying = false;
				cout << "\nThanks for playing!";
			}
										

			
		
		}while(continuePlaying);
	}
	catch (invalid_argument& ex)
	{
		cerr << "Invalid Argument: " << ex.what(); 
	}
	catch(out_of_range& rx)
	{
		cerr << "Out_of_range: " << rx.what();
	}
	catch(...)
	{
		cerr <<  "error occured: ";
	}
	
	
	return 0;
}

//if a player looses subtract their bet amount from their wallet
void PlayerLoss(BlackJack& game)
{
	double playerWallet = game.GetPlayer1().GetWallet();
	playerWallet -= game.GetPlayer1().GetBet();
	game.SetPlayer1Wallet(playerWallet);
}

//if a player wins add there bet amount to their wallet
void PlayerWin(BlackJack& game)
{
	double playerWallet = game.GetPlayer1().GetWallet();
	playerWallet += game.GetPlayer1().GetBet();
	game.SetPlayer1Wallet(playerWallet);
}

//validate that the data entered is a valid character
char ValidChar()
{
	char input;
	cin >> input;
	
	if(cin.fail()) //if it fails then not a valid char
	{
		cout << "Entry must be one alphabetic character. Please try again : ";
		functions::ClearInputBuffer();
		ValidChar();
	}
	
	input = toupper(input); //make the input uppercase
	return input;
}

//make sure the user inputs a whole number and it is one of the numbers allowed for a deck
int ValidDeck()
{
	int deckInput = functions::GetValidInteger();
	if( deckInput != 2 && deckInput != 4 && deckInput != 6 && deckInput != 8 )
	{
		cout << "\nIncorrect Deck Amount\nPlease enter either 2/4/6/8 : ";
		ValidDeck(); //recursivly call the function again if they fail
	}
	return deckInput;
}

//check if the user picks a valid char and it either Y or E
char PlayAnother()
{
	char input;
	input = ValidChar();
	if( input != 'Y' && input != 'E')
	{
		cout << "\nInvalid Choice: Please try again (Y/E) : ";
		PlayAnother(); //recursively call the function is they fail
	}
	return input;
}

//get the user decision to hit stand or double
char HitStandDouble(double bet, double wallet)
{	
	char input;
	if( wallet < (bet*2)) //check that the wallet has enough money to do a double
	{
		cout << "\nDo you wish to Stand(S) or Hit(H)? : ";
	}
	else
	{
		cout << "\nDo you wish to Stand(S), Hit(H) or Double(D)? : ";
	}
	
	//get a valid character
	input = ValidChar();
	if(input != 'H' && input != 'S' && input != 'D')
	{
		cout << "\nInvalid Choice: Please try again";
		input = HitStandDouble(bet, wallet); //recursively call function if they fail
	}
	
	if( input == 'D')
	{
		if( wallet < (bet*2)) //if they did choose to double make sure again that they can actually double
		{
			cout << "\nYou dont have enough money to double. Please try again";
			input = HitStandDouble(bet, wallet); //else recursively call function again
		}
	}
	return input;
}
