// TAKI PROJECT
// OFRI KUPERBERG
// 211440730
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NUM_CARD 9
#define PLUS 10
#define STOP 11
#define CHANGE_DIRECTION 12
#define TAKI 13
#define CHANGE_COLOR 14
#define DIFFERENT_CARDS 14
#define NUM_OF_SPECIAL_CARD 0
#define NO_COLOUR ' '
#define CARDS_ON_START 4
#define MAX_NAME_LEN 20
#define YELLOW 1
#define RED 2
#define BLUE 3
#define GREEN 4
#define FULL_LINE 'F'
#define EMPTY_LINE 'E'

typedef struct card
{
  int type;    // 1-9 regular number card, 10-14 special card
  char colour; // if needed
} cardData;

typedef struct player
{
  char name[MAX_NAME_LEN];
  int playerNum;
  cardData *cards;
  int cardsInHand;
  int maxAvailableCards;
} playerData;

typedef struct cardStatistics
{
  int frequency;
  int type;
} gameStats;

// Prototypes
void greetThePlayers(int *NumOfPlayers);
void SetGame(playerData Players[], int NumOfPlayers, cardData *openingCard, gameStats cardsStats[]);
void GetCardsOnStart(cardData cardsOnStart[], gameStats cardsStats[]);
void playersMallocCheck(playerData *p);
void cardsMallocCheck(cardData *p);
void getCardFromDeck(cardData *pCard, gameStats cardsStats[]);
void printCard(cardData card);
void printLine(char type);
void playerMove(playerData *player, cardData *upperCard, bool *isTaki, bool *isStopOn, bool *isChangeDirectionOn, bool *isPlusOn, gameStats cardsStats[]);
bool isChoiceValid(cardData cards[], cardData upperCard, int numOfCards, int playerChoice, bool isTaki);
void swapCards(cardData *card1, cardData *card2);
void PlayGame(playerData players[], int numOfPlayers, cardData upperCard, int *winnerPlayerIndex, gameStats cardsStats[]);
bool isGameOver(playerData *player, cardData upperCard, int numberOfPlayers, int *WinnerPlayerIndex, gameStats cardsStats[]);
void MoveToNextPlayer(int *playerIndex, int numOfPlayers, bool clockWise);
char chooseColor();
void printGamesStats(gameStats cardsStats[]);
void mergeSortCards(gameStats cards[], int size);
void mergeArray(gameStats arr1[], int size1, gameStats arr2[], int size2, gameStats mergeArr[]);
void copyArr(gameStats dest[], gameStats src[], int size);
void ResatCardsStats(gameStats cardsStats[]);
void freeAll(playerData players[], int numOfPlayers);

int main()
{
  // setting all the variables needed for the whole game
  bool clockWise = true;
  int numOfPlayers;
  playerData *players;
  cardData upperCard;
  int playerIndex = 0;
  int WinnerPlayerIndex;
  gameStats cardsStats[DIFFERENT_CARDS];

  srand(time(NULL));              // to seed the random
  greetThePlayers(&numOfPlayers); // greeting and getting the number of players

  // allocating memory for the players and check if it was successful
  players = (playerData *)malloc(numOfPlayers * sizeof(playerData));
  playersMallocCheck(players);

  ResatCardsStats(cardsStats); // resat the game stats

  // opening up the game
  SetGame(players, numOfPlayers, &upperCard, cardsStats);

  // playing the game
  PlayGame(players, numOfPlayers, upperCard, &WinnerPlayerIndex, cardsStats);

  // printing the winner and the game statistics
  printf("The winner is...%s! Congratulations!", players[WinnerPlayerIndex].name);
  printGamesStats(cardsStats);

  // freeing all the allocated memory
  freeAll(players, numOfPlayers);
  return 0;
}

// this function gets a pointer for number of players, and greet the players and asking how many players want to play
void greetThePlayers(int *NumOfPlayers)
{
  printf("************  Welcome to TAKI game  ************");
  printf("\nPlease enter the number of players:\n");
  scanf(" %d", NumOfPlayers);
  while (*NumOfPlayers <= 0)
  {
    printf("Invalid number of players! try again.\n");
    scanf(" %d", NumOfPlayers);
  }
}

// this two function are in charge of checking if the malloc or realloc was successful, and if not we exit the program
void playersMallocCheck(playerData *p)
{
  if (p == NULL)
  {
    printf("Memory Allocation failed !!!");
    exit(1);
  }
}
void cardsMallocCheck(cardData *p)
{
  if (p == NULL)
  {
    printf("Memory Allocation failed !!!");
    exit(1);
  }
}

// this function resat the game stats
void ResatCardsStats(gameStats cardsStats[])
{
  int i;
  for (i = 0; i < DIFFERENT_CARDS; i++)
  {
    cardsStats[i].frequency = 0;
    cardsStats[i].type = i + 1;
  }
}

// this function gets array of players , the number of player, a pointer to the opening card, the game stats and sets up the game
void SetGame(playerData Players[], int NumOfPlayers, cardData *openingCard, gameStats cardsStats[])
{
  int i;
  for (i = 0; i < NumOfPlayers; i++) // pass through all the players
  {

    // first we get the name and set the player numer
    printf("Please enter the first name of player #%d:\n", i + 1);
    scanf(" %s", Players[i].name);
    Players[i].playerNum = i + 1;

    // allocating a cards array for the player
    Players[i].cards = (cardData *)malloc(CARDS_ON_START * sizeof(cardData));
    cardsMallocCheck(Players[i].cards);

    // getting the player 4 card to start with and update the players stats
    GetCardsOnStart(Players[i].cards, cardsStats);
    Players[i].cardsInHand = CARDS_ON_START;
    Players[i].maxAvailableCards = CARDS_ON_START;
  }
  getCardFromDeck(openingCard, cardsStats);
  while (openingCard->type > MAX_NUM_CARD) // as long as the card is not between 1 and 9
  {
    cardsStats[(openingCard->type) - 1].frequency--; // if the opening card was not a number we dont want to count it on the game stats
    getCardFromDeck(openingCard, cardsStats);
  }
}

// this function gets array of players , and the games stats, and is in charge of getting 4 cards for each plater from the deck
void GetCardsOnStart(cardData cardsOnStart[], gameStats cardsStats[])
{
  int i;
  for (i = 0; i < CARDS_ON_START; i++)
  {
    getCardFromDeck(&cardsOnStart[i], cardsStats);
  }
}

// this function gets a pointer to a card and update this card type can color (if needed), and also gets an array of cards that store the game statistics
void getCardFromDeck(cardData *pCard, gameStats cardsStats[])
{
  char colours[4] = {'B', 'R', 'G', 'Y'}; // all the colors options

  // first we get the card type, 1-9 regular number card, 10-14 Spacial card
  pCard->type = (rand() % (DIFFERENT_CARDS)) + 1;

  // if the card is colour it doesn't have colour or number
  if (pCard->type == CHANGE_COLOR)
  {
    pCard->colour = NO_COLOUR;
  }
  else // else the card must have a color
  {
    pCard->colour = colours[rand() % 4]; // getting the color
  }
  cardsStats[pCard->type - 1].frequency++; // each time increasing the frequency of the card that was chosen from the deck
}

// this function gets the card data and prints the card
void printCard(cardData card)
{
  // first prints the edges of the card
  printLine(FULL_LINE);
  printLine(EMPTY_LINE);

  if (card.type <= MAX_NUM_CARD) // if the card is a number card
  {
    printf("*   %d   *\n", card.type);
    printf("*   %c   *\n", card.colour);
  }
  else // if it was power card, checks which one
  {
    switch (card.type)
    {
    case PLUS:
      printf("*   +   *\n");
      printf("*   %c   *\n", card.colour);
      break;
    case STOP:
      printf("*  STOP *\n");
      printf("*   %c   *\n", card.colour);
      break;
    case CHANGE_DIRECTION:
      printf("*  <->  *\n");
      printf("*   %c   *\n", card.colour);
      break;
    case TAKI:
      printf("*  TAKI *\n");
      printf("*   %c   *\n", card.colour);
      break;
    case CHANGE_COLOR:
      printf("* COLOR *\n");
      printf("*   %c   *\n", card.colour);
      break;
    }
  }

  // close the card edges
  printLine(EMPTY_LINE);
  printLine(FULL_LINE);
  printf("\n");
}

// this function print a line in the card witch can be empty or full
void printLine(char type)
{
  int i;
  if (type == FULL_LINE) // print a full line of stars
  {
    for (i = 0; i < 9; i++)
    {
      printf("*");
    }
    printf("\n");
  }
  else // print an empty line of start (just the edges)
  {
    printf("*");
    for (i = 0; i < 7; i++)
    {
      printf(" ");
    }
    printf("*\n");
  }
}

// this function gets the player data and the middle card data and calls to a function "player move" (later explained), then it checks what he played and what action the function needs to do, and checks if the game is over
void PlayGame(playerData players[], int numOfPlayers, cardData upperCard, int *winnerPlayerIndex, gameStats cardsStats[])
{
  bool isChangeDirectionOn;
  bool gameOver = false;
  bool isTaki = false;
  bool isClockwise = true;
  bool isStopOn;
  bool isPlusOn;
  int playerIndex = 0;
  while (!(gameOver)) // while the game isn't finished
  {
    isStopOn = isChangeDirectionOn = isPlusOn = true; // reseat the power cards to "on"

    // make the player move
    playerMove(&(players[playerIndex]), &upperCard, &isTaki, &isStopOn, &isChangeDirectionOn, &isPlusOn, cardsStats);

    // checks if the player finished his cards and won
    if (isGameOver(&(players[playerIndex]), upperCard, numOfPlayers, winnerPlayerIndex, cardsStats))
    {
      gameOver = true;
    }
    else // the player still didn't win
    {
      // now we check which card the player putted
      if (upperCard.type == TAKI) // if its taki
      {
        if (isTaki) // if its activated let the player play taki flow
        {
          while (isTaki && !(gameOver))
          {
            playerMove(&(players[playerIndex]), &upperCard, &isTaki, &isStopOn, &isChangeDirectionOn, &isPlusOn, cardsStats);
            gameOver = isGameOver(&(players[playerIndex]), upperCard, numOfPlayers, winnerPlayerIndex, cardsStats);

            // if the player put change color the taki flow is over
            if (upperCard.type == CHANGE_COLOR)
            {
              isTaki = false;
            }
          }
          // isTaki = false; // if the card was taki but wasn't activated
        }
      }
      switch (upperCard.type) // checks which kind of special card
      {
      case PLUS:       // if the card is plus we check if its "on"
        if (!isPlusOn) // if the plus is not activated we move to the next player
        {
          MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        }
        // else the players gets another turn
        break;
      case STOP: // if the card was stop we check if its activated
      {
        if (isStopOn) // if its activated, skip a player
        {
          if (numOfPlayers != 1)
          {
            if (players[playerIndex].playerNum == numOfPlayers)
            {                  // if the last player was the one who put stop
              playerIndex = 1; // move to the second player
            }
            else // skip a player
            {
              playerIndex++;
              MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
            }
          }
        }
        else // if the stop is not activated we dont skip a player
        {
          MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        }
      }
      break;
      case CHANGE_DIRECTION:     // if the card was change directions
        if (isChangeDirectionOn) // we check if the change direction is on
        {
          isClockwise = !isClockwise; // changes the clockwise value (true to false/ false to true)
        }
        MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        break;
      case TAKI: // if the last card was TAKI but wasn't part of the flow
        MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        break;
      case CHANGE_COLOR: // if the card was change color
        if (upperCard.colour == NO_COLOUR)
        {
          // pick a color and update the upper card
          upperCard.colour = chooseColor();
        }
        MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        break;
      default: // regular number card
        MoveToNextPlayer(&playerIndex, numOfPlayers, isClockwise);
        break;
      }
    }
  }
}

// this function gets a player pointer, upper card and number of players and checks if the game is finished
bool isGameOver(playerData *player, cardData upperCard, int numberOfPlayers, int *winnerPlayerIndex, gameStats cardsStats[])
{
  if (player->cardsInHand == 0) // if the player finished his cards
  {
    if (upperCard.type != PLUS) // and the card he put is not plus
    {
      if (numberOfPlayers != 2 || upperCard.type != STOP) // and the game isn't two people and the last card was stop
      {
        // Ladies and gentleman's we have a winner!!!
        *winnerPlayerIndex = (player->playerNum) - 1; // saving the winner index
        return true;
      }
      else // if the game was two people and the last card was stop, pull a card from the deck
      {
        getCardFromDeck(&(player->cards[0]), cardsStats);
        player->cardsInHand++;
      }
    }
    else // if the last was card was plus, pull a card from the deck
    {
      getCardFromDeck(&(player->cards[0]), cardsStats);
      player->cardsInHand++;
    }
  }
  return false; // if the player hasn't finished its cards
}

// this function gets the playerIndex address, the numer of player and bool to check which direction we play
void MoveToNextPlayer(int *playerIndex, int numOfPlayers, bool isClockWise)
{
  if (isClockWise) // if we are playing clockwise game
  {
    // and we reached the final player, move to the first player
    if ((*playerIndex) == (numOfPlayers - 1))
    {
      *playerIndex = 0;
    }
    else // if we are not the final player move to the next player
    {
      (*playerIndex)++;
    }
  }
  else // if we are playing counterClockwise
  {
    if (*playerIndex == 0) // and we are at the first player, move to the last player
    {
      *playerIndex = numOfPlayers - 1;
    }
    else // if we are not at the last player
    {
      (*playerIndex)--; // move to the previous player
    }
  }
}

// this function returns the char of the color  the player choose
char chooseColor()
{
  char PlayerColorChoice;
  int playerChoice;
  printf("Please enter your color choice:\n1 - Yellow\n2 - Red\n3 - Blue\n4 - Green\n");
  scanf(" %d", &playerChoice);
  while (playerChoice > GREEN || playerChoice < YELLOW)
  {
    printf("Invalid choice. try again\n");
    scanf(" %d", &playerChoice);
  }
  switch (playerChoice)
  {
  case YELLOW:
    PlayerColorChoice = 'Y';
    break;
  case RED:
    PlayerColorChoice = 'R';
    break;
  case BLUE:
    PlayerColorChoice = 'B';
    break;
  case GREEN:
    PlayerColorChoice = 'G';
    break;
  }
  return PlayerColorChoice;
}

// this function gets a player data, the upper card, bools for the power card,game stats to save, this function in charge of making sure the player move was a valid move and then makes that move
void playerMove(playerData *player, cardData *upperCard, bool *isTaki, bool *isStopOn, bool *isChangeDirectionOn, bool *isPlusOn, gameStats cardsStats[])
{
  int i;
  int playerChoice;
  bool ChoiceValid = true;
  printf("Upper card:\n");
  printCard(*upperCard);                    // print the middle card
  printf("%s's turn\n", player->name);      // print player name
  for (i = 0; i < player->cardsInHand; i++) // print cards
  {
    printf("Card #%d\n", i + 1);
    printCard(player->cards[i]);
  }
  if (*isTaki) // checking Which massage to print (if we are at taki or not)
  {
    printf("Please enter 0 if you want to finish your turn\n");
  }
  else
  {
    printf("Please enter 0 if you want to take a card from the deck\n");
  }

  printf("or 1-%d if you want to put one of your cards in the middle:\n", player->cardsInHand);
  scanf(" %d", &playerChoice);
  ChoiceValid = isChoiceValid(player->cards, *upperCard, player->cardsInHand, playerChoice, *isTaki);

  while (!ChoiceValid) // if the player choice is invalid, keeps asking the player for a valid choice
  {
    if (*isTaki) // if we are at taki flow
    {
      printf("Please enter 0 if you want to finish your turn\n");
    }
    else
    {
      printf("Please enter 0 if you want to take a card from the deck\n");
    }
    printf("or 1-%d if you want to put one of your cards in the middle:\n", player->cardsInHand);
    scanf(" %d", &playerChoice);
    ChoiceValid = isChoiceValid(player->cards, *upperCard, player->cardsInHand, playerChoice, *isTaki);
  }

  if (playerChoice == 0) // if the player wants to draw a card from the deck
  {

    if (!(*isTaki)) // if we are not at taki FLOW
    {
      // we "power down" the power cards because after a players draws a card from the deck they are no longer activated
      *isStopOn = *isChangeDirectionOn = *isPlusOn = false;

      if (player->cardsInHand == player->maxAvailableCards) // if the logical size of the array of cards reached the physical size
      {
        // increase the physical size times 2
        player->cards = (cardData *)realloc(player->cards, (player->maxAvailableCards * 2) * sizeof(cardData));
        cardsMallocCheck(player->cards);
        player->maxAvailableCards *= 2; // update the logical size
      }
      player->cardsInHand++;
      getCardFromDeck(&(player->cards[player->cardsInHand - 1]), cardsStats); // get card from the deck
    }
    else // if the player is in taki and wishes to end his turn
    {
      *isTaki = false;
    }
  }
  else // if the player choose a valid card to put in the middle
  {
    // update the upper card
    upperCard->type = player->cards[playerChoice - 1].type;
    upperCard->colour = player->cards[playerChoice - 1].colour;

    // if the player put a new taki card on the middle card we need to activate it
    if (player->cards[playerChoice - 1].type == TAKI)
    {
      *isTaki = true;
    }

    // decreasing the player cards
    swapCards(&(player->cards[playerChoice - 1]), &(player->cards[player->cardsInHand - 1]));
    player->cardsInHand--;
  }
}

// this function gets the cards of the user, the upper card, the number of cards the player have and the player choice checks if the card choice was valid
bool isChoiceValid(cardData cards[], cardData upperCard, int numOfCards, int playerChoice, bool isTaki)
{

  if (playerChoice == 0)
  {
    return true;
  }
  if (playerChoice > numOfCards || playerChoice < 0)
  {
    printf("Invalid Choice! Try again.\n");
    return false;
  }

  else if (isTaki) // if we are at taki we can only put the same color or color change card
  {
    if (cards[playerChoice - 1].colour == upperCard.colour || cards[playerChoice - 1].colour == NO_COLOUR)
    {
      return true;
    }
    else
    {
      printf("Invalid card! try again.\n");
      return false;
    }
  }
  else if (cards[playerChoice - 1].type == CHANGE_COLOR) // you can put change color on any card

  {
    return true;
  }
  else if ((cards[playerChoice - 1].type != upperCard.type) && (cards[playerChoice - 1].colour != upperCard.colour)) // if the card the player choose is invalid card
  {
    printf("Invalid card! try again.\n");
    return false;
  }
  return true; // if the choice is valid returns true
}

// this function gets two cards pointers of cards and switches between them
void swapCards(cardData *card1, cardData *card2)

{
  cardData tempCard;
  tempCard = *card1;
  *card1 = *card2;
  *card2 = tempCard;
}

// this function prints out the game statistics
void printGamesStats(gameStats cardsStats[])
{
  int i;

  // first we sort the array of statistics
  mergeSortCards(cardsStats, DIFFERENT_CARDS);
  printf("\n************ Game Statistics ************\n");
  printf("Card # | Frequency\n");
  printf("_____________________\n");
  for (i = 0; i < DIFFERENT_CARDS; i++)
  {
    switch (cardsStats[i].type) // for each type of card we print something else
    {
    case PLUS:
      printf("  +   |   %d\n", cardsStats[i].frequency);
      break;
    case CHANGE_DIRECTION:
      printf(" <->  |   %d\n", cardsStats[i].frequency);
      break;
    case STOP:
      printf(" STOP |   %d\n", cardsStats[i].frequency);
      break;
    case TAKI:
      printf("TAKI  |   %d\n", cardsStats[i].frequency);
      break;
    case CHANGE_COLOR:
      printf("COLOR |   %d\n", cardsStats[i].frequency);
      break;
    default:
      printf("  %d   |   %d\n", cardsStats[i].type, cardsStats[i].frequency);
      break;
    }
  }
}

// this function gets an array of structs of gamesStats and order them from max to min
void mergeSortCards(gameStats cardsSet[], int size)
{
  gameStats *tempCards;
  if (size <= 1)
  {
    return;
  }

  mergeSortCards(cardsSet, size / 2);
  mergeSortCards(cardsSet + size / 2, size - (size / 2));

  tempCards = (gameStats *)malloc(DIFFERENT_CARDS * sizeof(gameStats));
  if (tempCards)
  {
    mergeArray(cardsSet, size / 2, cardsSet + size / 2, size - size / 2, tempCards);
    copyArr(cardsSet, tempCards, size);
    free(tempCards);
  }
  else
  {
    printf("Memory Allocation failed!!!\n");
    exit(1);
  }
}

// this function gets: two arrays of gamesStats struct and their sizes and one array for the merge, and then it merges the two arrays together from max to min
void mergeArray(gameStats arr1[], int size1, gameStats arr2[], int size2, gameStats mergeArr[])
{
  int iArr1 = 0;
  int iArr2 = 0;
  int mergeIndex = 0;
  while (iArr1 < size1 && iArr2 < size2)
  {
    if (arr1[iArr1].frequency >= arr2[iArr2].frequency)
    {
      mergeArr[mergeIndex] = arr1[iArr1];
      iArr1++;
    }
    else
    {
      mergeArr[mergeIndex] = arr2[iArr2];
      iArr2++;
    }
    mergeIndex++;
  }
  while (iArr1 < size1)
  {
    mergeArr[mergeIndex] = arr1[iArr1];
    iArr1++;
    mergeIndex++;
  }
  while (iArr2 < size2)
  {
    mergeArr[mergeIndex] = arr2[iArr2];
    iArr2++;
    mergeIndex++;
  }
}

// this function gets to array and their sizes and copy one to the other
void copyArr(gameStats dest[], gameStats src[], int size)

{
  int i;
  for (i = 0; i < size; i++)
  {
    dest[i] = src[i];
  }
}

// this function gets the players array and free all the allocated memory we allocated throughout the game
void freeAll(playerData players[], int numOfPlayers)
{
  int i;
  for (i = 0; i < numOfPlayers; i++)
  {
    free(players[i].cards);
  }
  free(players);
}