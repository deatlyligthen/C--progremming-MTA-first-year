// 211440730
// Ofri Kuperberg
// MTA pizza project
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>

// struct for pizza style
typedef struct PIZZA
{
  int length;
  int width;
  char doughType;
  double olives;
  double mushrooms;
  double tomatoes;
  double pineapple;
  char topQ1;
  char topQ2;
  char topQ3;
  char topQ4;
  double price;
} PIZZA;

// receipt struct to keep track of the full order and costumer data
typedef struct receipt
{
  int ID;
  int numberOfPizzas;
  int delivery;
  double grossPrice;
  int finalPrice;
} RECEIPT;

// defining the prices of the store and the store topping and dough (can be later changed if needed)
#define BASIC_PIZZA_PRICE 70.00
#define BASIC_SIZE_PIZZA 2000.0
#define BASIC_LENGTH_PIZZA 40
#define BASIC_WIDTH_PIZZA 50
#define OLIVES_PRICE 10
#define MUSHROOMS_PRICE 12
#define TOMATOES_PRICE 9
#define PINEAPPLE_PRICE 14
#define WITH_DELIVERY 1
#define WITHOUT_DELIVERY 0
#define DELIVERY_PRICE 15
#define NO_TOPPING 0
#define FULL_TOPPING 1
#define HALF_TOPPING 0.5
#define QUARTER_TOPPING 0.25
#define REGULAR_DOUGH 'r'
#define VEGAN_DOUGH 'v'
#define WHOLE_WHEAT_DOUGH 'w'
#define NO_GLUTEN_DOUGH 'f'
#define OLIVES 'O'
#define MUSHROOMS 'M'
#define TOMATOES 'T'
#define PINEAPPLE 'P'
#define PLAIN ' '
#define TAX 1.17
#define REGULAR_DOUGH_PRICE 0
#define VEGAN_DOUGH_PRICE 5
#define WHOLE_WHEAT_PRICE 3
#define GLUTEN_FREE_PRICE 4
#define FIRST_QUARTER 1
#define SECOND_QUARTER 2
#define THIRD_QUARTER 3
#define FORTH_QUARTER 4

// Prototypes (later explained for each one of the programs)

// the ones required:
int getIDNumber();
void printMenu();
int getNumOfPizzas();
PIZZA getPizzaDimensions();
PIZZA getDoughType(PIZZA pizza);
PIZZA getToppings(PIZZA pizza);
void printPizzaDetails(PIZZA pizza, int i);
void printPizza(PIZZA pizza);
int getDelivery();
void getPayment(int totalPrice);

// the ones I created:
RECEIPT getFullOrder();
void PizzaTopResat(PIZZA *pizza);
bool toppingChoiceValid(PIZZA *pizza, int topChoice, double topCheck);
bool MaxTopCheck(int topChoice, double topNow);
void whereToPutTop(PIZZA *pizza, double *topping, char topLetter, int toppingChoice, int *quarterFill, double *topCheck);
PIZZA GetOnePizzaPrice(PIZZA pizza);
void printLine(int length, char startAndEnd, char sign1, char sign2);
void printFullOrder(RECEIPT receipt);

int main()
{
  int ID;
  // greeting
  printf("Welcome to MTA-Pizza!\n\n");
  printf("*****\n ***\n  *\n");

  // making new receipt and getting order:
  RECEIPT receipt;
  ID = getIDNumber();
  printMenu();
  receipt = getFullOrder();
  receipt.ID = ID;

  // printing full order:
  printFullOrder(receipt);

  // getting payment
  getPayment(receipt.finalPrice);
  return 0;
}

int getIDNumber() // Getting ID from the costumer and checking if it's valid
{
  int ID, idCheck, checkNum, units, tens, SumAll, num, index, testNum, oddSum, evenSum;
  bool flag;
  do
  {
    oddSum = 0;
    evenSum = 0;
    printf("\nPlease enter your ID number: ");
    scanf("%d", &ID);
    if (ID <= 0)
    {
      printf("\nInvalid ID number! Try again.");
      flag = false;
      continue;
    }

    idCheck = ID;

    checkNum = idCheck % 10;
    idCheck /= 10;
    for (index = 1; idCheck != 0 && index <= 9; index++)
    {
      if (index % 2 != 0)
      {
        num = idCheck % 10;
        num *= 2;
        units = num % 10;
        tens = num / 10;
        oddSum += (units + tens);
        idCheck /= 10;
      }
      else
      {
        evenSum += (idCheck % 10);
        idCheck /= 10;
      }
    }
    if (index > 9)
    {
      printf("\nInvalid ID number! Try again.");
      continue;
    }
    else
    {
      SumAll = oddSum + evenSum;
      testNum = 10 - (SumAll % 10);
      if (testNum == 10)
      {
        if (checkNum == 0)
        {
          flag = true;
        }
        else
        {
          printf("\nInvalid check digit! Try again.");
          flag = false;
        }
      }
      else
      {
        if (testNum == checkNum)
        {
          flag = true;
        }
        else
        {
          printf("\nInvalid check digit! Try again.");
          flag = false;
        }
      }
    }
  } while (index > 9 || !(flag));

  return ID;
}

void printMenu() // printing the restaurant menu
{

  printf("\n\nOur menu:");
  printf("\n*********\n");
  printf("Basic pizza: %.2f NIS for %dx%d size pizza\n", BASIC_PIZZA_PRICE, BASIC_LENGTH_PIZZA, BASIC_WIDTH_PIZZA);
  printf("\nToppings for basic size pizza:\n");
  printf("Olives: %d NIS\n", OLIVES_PRICE);
  printf("Mushrooms: %d NIS\n", MUSHROOMS_PRICE);
  printf("Tomatoes: %d NIS\n", TOMATOES_PRICE);
  printf("Pineapple: %d NIS\n", PINEAPPLE_PRICE);
  printf("\nDough type for basic size pizza:");
  printf("\nRegular: %d NIS\nVegan: %d NIS\nWhole wheat: %d NIS\nGluten free: %d NIS\n", REGULAR_DOUGH_PRICE, VEGAN_DOUGH_PRICE, WHOLE_WHEAT_PRICE, GLUTEN_FREE_PRICE);
}

// this program is in charge of all the order. which is: how many pizzas, topping and dough type for each pizza, drawing for each pizza, calculating the price for each pizza and then for the whole order, and asking for deliver. and then returning the receipt for main, in order to get payment:

RECEIPT getFullOrder()
{
  // setting variables requiring for the program
  int delivery;
  PIZZA pizza;                               // struct for each pizza
  RECEIPT receipt;                           // the receipt for the whole order
  receipt.grossPrice = 0;                    // initiating the price to 0
  receipt.numberOfPizzas = getNumOfPizzas(); // getting the number of pizzas the customer wants

  // for loop that gets the pizza details for each pizza the costumer ordered:
  for (int counter = 1; counter <= receipt.numberOfPizzas; counter++)
  {
    printf("\n*************************************************\n");
    printf("Pizza #%d:", counter);
    pizza = getPizzaDimensions();      // pizza LxW
    pizza = getDoughType(pizza);       // dough type for the pizza
    pizza = getToppings(pizza);        // topping for the pizza
    pizza = GetOnePizzaPrice(pizza);   // price for each pizza
    receipt.grossPrice += pizza.price; // summing the prices for each pizza
    printPizzaDetails(pizza, counter); // printing the pizza details
    printPizza(pizza);                 // printing the pizza
  }
  delivery = getDelivery(); // getting delivery choice from the costumer:

  // checking to see if there is delivery or not in order to know what to print in the receipt:
  if (delivery == 1)
  {
    receipt.delivery = 1;
    receipt.grossPrice += 15;
  }
  else
  {
    receipt.delivery = 0;
  }

  // calculating the final price after tax, rounded down:
  receipt.finalPrice = (int)(receipt.grossPrice * TAX);

  return receipt; // returning the receipt to main
}

// this program ask the costumer how many pizza's he wants, and won't let him enter an invalid number (>= 0)
int getNumOfPizzas()
{
  int numOfPizzas;
  printf("\nHow many pizzas would you like to order? ");
  scanf("%d", &numOfPizzas);
  while (numOfPizzas <= 0)
  {
    printf("Invalid choice! Try again.");
    printf("\nHow many pizzas would you like to order? ");
    scanf("%d", &numOfPizzas);
  }
  return numOfPizzas;
}

// this program make a new pizza struct each time and ask's the costumer for pizza width and length and making sure the choice is valid
PIZZA getPizzaDimensions()
{
  PIZZA pizza;
  printf("\n\nPlease enter your pizza's length (cm): ");
  scanf("%d", &pizza.length);

  while (pizza.length % 2 == 1 || (!(pizza.length >= 10 && pizza.length <= 40)))
  {
    printf(" Invalid length! Try again.");
    printf("\n\nPlease enter your pizza's length (cm): ");
    scanf("%d", &pizza.length);
  }

  printf(" Please enter your pizza's width (cm): ");
  scanf("%d", &pizza.width);

  while (pizza.width % 2 == 1 || (!(pizza.width >= 10 && pizza.width <= 80)))
  {
    printf("Invalid width! Try again.");
    printf("\nPlease enter your pizza's width (cm): ");
    scanf("%d", &pizza.width);
  }
  return pizza;
}

// this program is getting the dough type for the pizza, and check's if the choice is a valid one
PIZZA getDoughType(PIZZA pizza)
{

  char DoughType;
  printf("\nPlease enter the pizza's dough type:");
  printf("\n%c - for regular\n%c - for vegan\n%c - for whole wheat\n%c - for gluten-free\n", REGULAR_DOUGH, VEGAN_DOUGH, WHOLE_WHEAT_DOUGH, NO_GLUTEN_DOUGH);
  scanf(" %c", &pizza.doughType);
  while (!(pizza.doughType == 'r' || pizza.doughType == 'v' || pizza.doughType == 'w' || pizza.doughType == 'f'))
  {
    printf("\nInvalid Choice! Try again.");
    printf("\nPlease enter the pizza's dough type:");
    printf("\n%c - for regular\n%c - for vegan\n%c - for whole wheat\n%c - for gluten-free\n", REGULAR_DOUGH, VEGAN_DOUGH, WHOLE_WHEAT_DOUGH, NO_GLUTEN_DOUGH);
    scanf(" %c", &pizza.doughType);
  }
  return pizza;
}

/* this program takes the user topping choice and checking two things
1.) if the topping choice is valid (0-3)
2.) by calling another function, it checks if the topping doesn't exceed the pizza (more then the whole pizza.
*/

PIZZA getToppings(PIZZA pizza)
{
  int toppingChoice;
  double topCheck = 0;
  int quarter_fill = 0;
  bool topValid;

  PizzaTopResat(&pizza); // resat the pizza toppings and quarters

  // asking the costumer for topping selection:
  printf("\nPlease choose the toppings:\n\n");
  printf("Olives (choose 0-3):\n");
  printf("0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
  scanf("%d", &toppingChoice);

  topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  while (!topValid) // while the costumer enters invalid number, keeps asking
  {
    printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
    scanf("%d", &toppingChoice);
    topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  }
  whereToPutTop(&pizza, &pizza.olives, OLIVES, toppingChoice, &quarter_fill, &topCheck);

  if (quarter_fill == FORTH_QUARTER) // if the pizza is full exit the program
  {
    return pizza;
  }

  printf("\nMushrooms (choose 0-3):\n");
  printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
  scanf("%d", &toppingChoice);

  topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  while (!topValid) // while the costumer enters invalid number, keeps asking
  {
    printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
    scanf("%d", &toppingChoice);
    topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  }

  whereToPutTop(&pizza, &pizza.mushrooms, MUSHROOMS, toppingChoice, &quarter_fill, &topCheck);

  if (quarter_fill == FORTH_QUARTER) // if the pizza is full exit the program
  {
    return pizza;
  }

  printf("\nTomatos (choose 0-3):\n");
  printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
  scanf("%d", &toppingChoice);

  topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  while (!topValid) // while the costumer enters invalid number, keeps asking
  {
    printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
    scanf("%d", &toppingChoice);
    topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  }
  whereToPutTop(&pizza, &pizza.tomatoes, TOMATOES, toppingChoice, &quarter_fill, &topCheck);

  if (quarter_fill == FORTH_QUARTER) // if the pizza is full exit the program
  {
    return pizza;
  }

  printf("\nPineapple (choose 0-3):\n");
  printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
  scanf("%d", &toppingChoice);

  topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  while (!topValid) // while the costumer enters invalid number, keeps asking
  {
    printf("\n0. None\n1. Whole pizza\n2. Half pizza\n3. Quarter pizza\n");
    scanf("%d", &toppingChoice);
    topValid = toppingChoiceValid(&pizza, toppingChoice, topCheck);
  }
  whereToPutTop(&pizza, &pizza.pineapple, PINEAPPLE, toppingChoice, &quarter_fill, &topCheck);

  return pizza;
}

void PizzaTopResat(PIZZA *pizza)
{
  // setting all the quartes to plain:
  pizza->topQ1 = PLAIN;
  pizza->topQ2 = PLAIN;
  pizza->topQ3 = PLAIN;
  pizza->topQ4 = PLAIN;

  // setting all toppings to none:
  pizza->olives = NO_TOPPING;
  pizza->mushrooms = NO_TOPPING;
  pizza->tomatoes = NO_TOPPING;
  pizza->pineapple = NO_TOPPING;
}

bool toppingChoiceValid(PIZZA *pizza, int toppingChoice, double topCheck)
{
  if (toppingChoice > 3 || toppingChoice < 0)
  {
    printf("\nInvalid Choice! Try again.");
    return false;
  }
  else if (MaxTopCheck(toppingChoice, topCheck) == false)
  {
    printf("You have exceeded the maximum amount of toppings allowed on one pizza! Try again.");
    return false;
  }
  else
  {
    return true;
  }
}

// this program checks if the user entered a topping choice resulting in filling more then full pizza price, return test result (false or true):
bool MaxTopCheck(int topChoice, double topNow)
{
  double topAdd;
  switch (topChoice) // getting the topping quantity choice accordingly to the costumer choice
  {
  case 0:
    topAdd = 0;
    break;
  case 1:
    topAdd = FULL_TOPPING;
    break;
  case 2:
    topAdd = HALF_TOPPING;
    break;
  case 3:
    topAdd = QUARTER_TOPPING;
    break;
  }

  // checking if the user choice now and the amount of topping already on the pizza exceed the whole pizza
  if (topNow + topAdd > 1)
  {
    return false; // exceed, return FALSE (0)
  }
  else
  {
    return true; // doesn't exceed, return TRUE (1)
  }
}

// this program checks where to put the current topping the costumer wants
void whereToPutTop(PIZZA *pizza, double *topping, char topLetter, int toppingChoice, int *quarterFill, double *topCheck)
{
  switch (toppingChoice)
  {
  case 0: // costumer doesn't want that topping
    *topping = NO_TOPPING;
    break;
  case 1: // if the pizza is still empty, and the costumer wants the whole pizza to be filled, exit the program
    *topping = FULL_TOPPING;
    pizza->topQ1 = topLetter;
    pizza->topQ2 = topLetter;
    pizza->topQ3 = topLetter;
    pizza->topQ4 = topLetter;
    *quarterFill = FORTH_QUARTER;
    *topCheck += FULL_TOPPING;
    break;
  case 2: // the costumer wants half pizza, we need to check where we can fill it
    *topping = HALF_TOPPING;
    switch (*quarterFill)
    {
    case SECOND_QUARTER:
      pizza->topQ3 = topLetter;
      pizza->topQ4 = topLetter;
      *quarterFill = FORTH_QUARTER;
      break;
    case FIRST_QUARTER:
      pizza->topQ2 = topLetter;
      pizza->topQ3 = topLetter;
      *quarterFill = THIRD_QUARTER;
      break;
    default:
      pizza->topQ1 = topLetter;
      pizza->topQ2 = topLetter;
      *quarterFill = SECOND_QUARTER;
      break;
    }
    *topCheck += HALF_TOPPING;
    break;

  case 3: // the costumer wants one quarter, we need to check where we can fill it:
    *topping = QUARTER_TOPPING;
    switch (*quarterFill)
    {
    case THIRD_QUARTER:
      pizza->topQ4 = topLetter;
      *quarterFill = FORTH_QUARTER;
      break;
    case SECOND_QUARTER:
      pizza->topQ3 = topLetter;
      *quarterFill = THIRD_QUARTER;
      break;
    case FIRST_QUARTER:
      pizza->topQ2 = topLetter;
      *quarterFill = SECOND_QUARTER;
      break;
    default:
      pizza->topQ1 = topLetter;
      *quarterFill = FIRST_QUARTER;
      break;
    }
    *topCheck += QUARTER_TOPPING;
    break;
  }
}

// program for calculating the price for one pizza
PIZZA GetOnePizzaPrice(PIZZA pizza)
{
  // setting the pizza size and ratio
  int pizzaSize = (pizza.length * pizza.width);
  double pizzaRatio = pizzaSize / BASIC_SIZE_PIZZA;

  // setting other variables required for getting the prices
  double doughPrice, toppingPrice, olivesPrice, mushroomsPrice, tomatoesPrice, pineapplePrice, sizePrice;

  // getting prices for dough
  switch (pizza.doughType)
  {
  case REGULAR_DOUGH:
    doughPrice = REGULAR_DOUGH_PRICE;
    break;
  case VEGAN_DOUGH:
    doughPrice = pizzaRatio * VEGAN_DOUGH_PRICE;
    break;
  case WHOLE_WHEAT_DOUGH:
    doughPrice = pizzaRatio * WHOLE_WHEAT_PRICE;
    break;
  case NO_GLUTEN_DOUGH:
    doughPrice = pizzaRatio * GLUTEN_FREE_PRICE;
    break;
  }

  // getting prices to each topping
  olivesPrice = ((pizzaSize * pizza.olives) / BASIC_SIZE_PIZZA) * OLIVES_PRICE;
  mushroomsPrice = ((pizzaSize * pizza.mushrooms) / BASIC_SIZE_PIZZA) * MUSHROOMS_PRICE;
  tomatoesPrice = ((pizzaSize * pizza.tomatoes) / BASIC_SIZE_PIZZA) * TOMATOES_PRICE;
  pineapplePrice = ((pizzaSize * pizza.pineapple) / BASIC_SIZE_PIZZA) * PINEAPPLE_PRICE;

  // summing the whole topping prices
  toppingPrice = olivesPrice + mushroomsPrice + tomatoesPrice + pineapplePrice;

  // getting price for size
  sizePrice = pizzaRatio * BASIC_PIZZA_PRICE;

  // getting full price (size + dough + toppings)
  pizza.price = (sizePrice + doughPrice + toppingPrice);
  return pizza;
}

// program for printing one pizza details
void printPizzaDetails(PIZZA pizza, int i)
{
  printf("\nPizza #%d details:", i);
  printf("\n*******************\n");
  printf("Pizza size: %dx%d", pizza.length, pizza.width);
  printf("\nPizza price (without tax): %.2lf\n", pizza.price);
}

// program for printing out the pizza
void printPizza(PIZZA pizza)
{

  // setting variables for the printing
  int toppingLength = (pizza.length) - 2; // the length the only the topping fills
  int toppingWidth = (pizza.width) - 2;   // the width that only the topping fills
  int l, w, b;                            // for the for loops
  char pizzaParameter = pizza.doughType;  // char for the pizza parameter

  // print first line of the pizza
  for (int b = 1; b <= pizza.width; b++)
  {
    printf("%c", pizzaParameter);
  }
  printf("\n");
  // print first half length of the pizza (quarter 4 and 1). by calling another function (to be explained later)
  for (l = 1; l <= toppingLength / 2; l++)
  {
    printLine(toppingWidth, pizzaParameter, pizza.topQ4, pizza.topQ1);
  }
  l--; // resat the line in the middle (to not skip a line)

  // printing the second half length of the pizza (quarter 3 and 2)
  for (; l < toppingLength; l++)
  {
    printLine(toppingWidth, pizzaParameter, pizza.topQ3, pizza.topQ2);
  }
  // printing the last line
  for (b = 1; b <= pizza.width; b++)
  {
    printf("%c", pizzaParameter);
  }
  printf("\n");
}

// this program is in charge of printing one line of the pizza, by getting the pizza width, dough and toppings
void printLine(int length, char startAndEnd, char sign1, char sign2)
{
  // first line half
  int i;
  printf("%c", startAndEnd); // printing line start (dough type)

  for (i = 1; i <= length / 2; i++) // printing  the first half line (quarter 4 / quarter 3)
  {
    printf("%c", sign1);
  }
  i--; // setting the curser back in the middle in order to get the full line

  for (; i < length; i++) // printing the remaining line (quarter 1 / quarter 2)
  {
    printf("%c", sign2);
  }

  printf("%c", startAndEnd); // printing line ending (dough type)
  printf("\n");              // move down a line
}

// program for getting delivery choice and checking it it's valid option
int getDelivery()
{
  int delivery;
  printf("\n*************************************************\n");

  // asking the costumer if he wants a delivery and checks if the choice is valid:
  printf("Do you want delivery for the price of 15 NIS? Enter 1 for delivery or 0 for pick-up: ");
  scanf("%d", &delivery);
  while (!(delivery == WITH_DELIVERY || delivery == WITHOUT_DELIVERY))
  {
    printf(" Invalid choice! Try again.");
    printf("\nDo you want delivery for the price of 15 NIS? Enter 1 for delivery or 0 for pick-up: ");
    scanf("%d", &delivery);
  }
  return delivery;
}

// program for printing the whole order choice
void printFullOrder(RECEIPT receipt)
{
  printf("\n\nYour order details:");
  printf("\n*******************\n");
  printf("ID number: %09d\n", receipt.ID);
  printf("Number of pizzas: %d", receipt.numberOfPizzas);
  if (receipt.delivery == 1)
  {
    printf("\nDelivery");
  }
  else
  {
    printf("\nPick-up");
  }

  printf("\nTotal price: %.2f", receipt.grossPrice);
  printf("\nTotal price with tax (rounded down): %d\n", receipt.finalPrice);
}

// program that takes payment from the costumer and return the change by a minium quantity of coins (if there is a charge left)
void getPayment(int totalPrice)
{
  int payment, balance, change;
  int coin_10, coin_5, coin_2, coin_1;

  printf("\nPlease enter your payment: ");
  scanf("%d", &payment);
  balance = totalPrice - payment;
  while (balance > 0)
  {
    printf("Your remaining balance is: %d\n", balance);
    printf("Please enter your payment: \n");
    scanf("%d", &payment);
    balance -= payment;
  }

  if (balance == 0) /* If the exact sum was paid */
  {
    printf("\nThank you for your order!");
  }
  else /* otherwise, calculating the change */
  {
    change = -balance;
    printf("\nYour change is %d NIS using:\n", change);

    if (change >= 10)
    {
      coin_10 = change / 10;
      printf("%d coin(s) of ten\n", coin_10);
      change %= 10;
    }
    if (change >= 5)
    {
      coin_5 = change / 5;
      printf("%d coin(s) of five\n", coin_5);
      change %= 5;
    }
    if (change >= 2)
    {
      coin_2 = change / 2;
      printf("%d coin(s) of two\n", coin_2);
      change %= 2;
    }
    if (change > 0)
    {
      coin_1 = 1;
      printf("%d coin(s) of one\n", coin_1);
    }
    printf("\nThank you for your order!");
  }
}
