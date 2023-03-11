// ofri kuperberg
// 211440730
// recursive functions

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SIZE 10
#define MAX_INT_SIZE 10
#define FUNC_COUNT 9
#define EXIT 0
#define ORIGEN 0
#define PATH_FOUND 1
#define LOWEST_POWER 1
#define START_OF_STRING 0
#define ONE_ELEMENT_ONLY 1
#define START_OF_ARRAY 0

// Add your recursive functions declarations here
bool isEven(int num, int dig);
int howManyPaths(int x, int y);
int biggestLowPower(int x, int num);
int partSum(int num);
void intToStr(int num, char s[]);
void fillMaxPrefixesArray(int numbers[], int n, int maxPrefixesArray[]);
void getMinToStart(int numbers[], int n);
void combineArrays(int sortedArr1[], int size1, int sortedArr2[], int size2);
int countSmaller(int arr[], int start, int end, int num);

// Testing functions
int readArray(int data[], int maxSize);
void printArray(int data[], int size);
void bubbleSort(int arr[], int size);
void swap(int arr[], int i, int j);
void checkQ1();
void checkQ2();
void checkQ3();
void checkQ4();
void checkQ5();
void checkQ6();
void checkQ7();
void checkQ8();
void checkQ9();

/*********** main - don't make any changes here !!! ***************************/
void main()
{
  int funcNum;
  bool exit = false;

  while (!exit)
  {
    printf("Enter the number of function you want to check (1-%d) or %d to exit the program: ", FUNC_COUNT, EXIT);
    scanf("%d", &funcNum);

    switch (funcNum)
    {
    case 1:
      checkQ1();
      break;
    case 2:
      checkQ2();
      break;
    case 3:
      checkQ3();
      break;
    case 4:
      checkQ4();
      break;
    case 5:
      checkQ5();
      break;
    case 6:
      checkQ6();
      break;
    case 7:
      checkQ7();
      break;
    case 8:
      checkQ8();
      break;
    case 9:
      checkQ9();
      break;
    case EXIT:
      exit = true;
      break;
    default:
      printf("Invalid choice.\n");
    }
    printf("\n");
  }
}

/***************** Testing functions - don't make any changes here!!! ***************/
void checkQ1()
{
  int num, dig;

  printf("Enter a positive integer: ");
  scanf("%d", &num);
  printf("Enter a digit: ");
  scanf("%d", &dig);
  if (isEven(num, dig))
  {
    printf("%d appears even number of times in %d\n", dig, num);
  }
  else
  {
    printf("%d appears odd number of times in %d\n", dig, num);
  }
}

void checkQ2()
{
  int x, y;

  printf("Enter two non-negative integers: ");
  scanf("%d%d", &x, &y);
  printf("There are %d paths from (0,0) to (%d,%d)\n", howManyPaths(x, y), x, y);
}

void checkQ3()
{
  int x, num;

  printf("Enter two positive integers: ");
  scanf("%d%d", &x, &num);
  printf("The biggest power of %d which is smaller than %d is %d\n", x, num, biggestLowPower(x, num));
}

void checkQ4()
{
  int num;

  printf("Enter a positive integer: ");
  scanf("%d", &num);
  printf("The partial sum of %d digits is %d\n", num, partSum(num));
}

void checkQ5()
{
  int num;
  char string[MAX_INT_SIZE + 1];

  printf("Enter a positive integer: ");
  scanf("%d", &num);
  intToStr(num, string);
  printf("The string representing the integer %d is %s\n", num, string);
}

void checkQ6()
{
  int numbers[SIZE];
  int maxPrefixes[SIZE];
  int size;

  size = readArray(numbers, SIZE);
  fillMaxPrefixesArray(numbers, size, maxPrefixes);
  printf("Max prefixes array: ");
  printArray(maxPrefixes, size);
}

void checkQ7()
{
  int numbers[SIZE];
  int size;

  size = readArray(numbers, SIZE);
  getMinToStart(numbers, size);
  printf("The minimal number is: %d\n", numbers[0]);

  // check if all other numbers are still inb array
  bubbleSort(numbers, size);
  printf("The sorted array: ");
  printArray(numbers, size);
}

void checkQ8()
{
  int arr1[SIZE], arr2[2 * SIZE];
  int size1, size2;

  size1 = readArray(arr1, SIZE);
  size2 = readArray(arr2, 2 * SIZE - size1);

  // sort arrays
  bubbleSort(arr1, size1);
  bubbleSort(arr2, size2);

  combineArrays(arr1, size1, arr2, size2);
  printf("The combined sorted array: ");
  printArray(arr2, size1 + size2);
}

void checkQ9()
{
  int numbers[SIZE];
  int size, num;

  // read numbers from user (assumption: numbers are different from each other)
  size = readArray(numbers, SIZE);
  bubbleSort(numbers, size);

  printf("Please enter an integer: ");
  scanf("%d", &num);

  printf("There are %d numbers in array that are smaller than %d\n", countSmaller(numbers, 0, size - 1, num), num);
}

// This function reads a series of integers from user into data array.
// The function will first ask the user to enter the number of integers he wishes
// to enter to array. If number is bigger than maxSize, then only the first masSize
// numbers will be read.
// The fucntion returns the arrays logical size (number of elements inserted into array).
int readArray(int data[], int maxSize)
{
  int count;
  int i; // number of elements inserted into array

  printf("How many numbers would you like to enter to array ? (no more than %d) ", maxSize);
  scanf("%d", &count);
  if (count > maxSize)
  {
    count = maxSize;
  }
  printf("Please enter %d integers: ", count);

  for (i = 0; i < count; i++)
  {
    scanf("%d", &data[i]); // read current input number
  }

  return count;
}

// This function prints the first size elements of data array (integers).
void printArray(int data[], int size)
{
  int i;

  for (i = 0; i < size; i++)
  {
    printf("%d ", data[i]);
  }
  printf("\n");
}

// This functions sorts arr in increasing order using bubble sort algorithm
void bubbleSort(int arr[], int size)
{
  int i, j;

  for (i = 0; i < size - 1; i++)
    for (j = 0; j < size - i - 1; j++)
      if (arr[j] > arr[j + 1])
        swap(arr, j, j + 1);
}

void swap(int arr[], int i, int j)
{
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

/******************************* Recursive functions **************************/
// Add recursive functions implementation here

/* function 1 ---- is even
this function gets a digit and a number and checks if the digit appears odd or even times in the number
*/
bool isEven(int num, int dig)
{

  if (num < 10)
  {
    if (num == dig) // if the number is smaller then 10 and equals to dig we have one dig(odd)
    {
      return false;
    }
    return true; // else we have 0 dig's (even)
  }

  if ((num % 10) == dig)
  {
    return (!(isEven(num / 10, dig)));
  }
  return (isEven(num / 10, dig));
}

/*  function 2 ---- how many paths
this function gets a coordinate (x,y) that are both nun negative and returns how many paths are to get to the point from the Origen to the point (we can only move 1 step each time (up or right))
 */
int howManyPaths(int x, int y)
{
  if (x == ORIGEN || y == ORIGEN) // if x or y reached 0 we found a path because now we only have to move x or y to 0
  {
    return PATH_FOUND; // returns found path (1)
  }

  else // if x and y are not equal to 0
  {
    return ((howManyPaths(x - 1, y)) + (howManyPaths(x, y - 1))); // first we move x until it reaches 0 and then y
  }
}

/* function 3 ---- smallest power to reach number
this function gets two numbers x and y (x ≥ 2) and returns the biggest x^n that is smaller or equal to y (n is a natural number)
 */
int biggestLowPower(int x, int num)
{
  if (x > num) // if the base is bigger then the number we have to set it to one in order to be smaller then the number
  {
    return LOWEST_POWER;
  }

  return x * (biggestLowPower(x, num / x)); // set n++ on the scale of x^n (2^2 ---> 2^3 / 3^4 ---> 3^5))
}
int partSum(int num)
{
  if (num == 0) // if the num is 0 return 0
  {
    return 0;
  }
  return (partSum(num / 10) + ((num / 10) % 10)); // return the number plus the second to last digit (this way only when we get to the end we "forgot" to add the last digit)
}
// this function takes a nun negative number and an array of chars and saves the numbers as a string in the array of chars
void intToStr(int num, char s[])
{
  if (num < 10) // if we reached a number smaller then 10, put it in the start of the string
  {
    s[START_OF_STRING] = (num % 10) + '0';
    s[START_OF_STRING + 1] = '\0';
  }
  else // after each call to the function put the digit in the right place
  {
    intToStr(num / 10, s);
    int index = strlen(s);
    s[index] = (num % 10) + '0';
    s[index + 1] = '\0'; // we need this in order to calculate the len of the string after each call to the function
  }
}
// this function runs over the array of of number and always lower the logical size by one until it reaches 1, then put the first element in the new array, then every time it returns the size gets bigger so each time we check if the new number we can now read is bigger then the current max number we found earlier, if so now the max is the new max and we put it the next place of new array, if not it means we haven't found a new max so we continue to put the previous max in the next place
void fillMaxPrefixesArray(int numbers[], int n, int maxPrefixesArray[])
{
  if (n == ONE_ELEMENT_ONLY) // if we reached the start of the array (only one element), put the first element of number to the new array
  {
    maxPrefixesArray[START_OF_ARRAY] = numbers[START_OF_ARRAY];
  }
  else
  {
    fillMaxPrefixesArray(numbers, n - 1, maxPrefixesArray); // call the function with less size

    if (numbers[n - 1] > maxPrefixesArray[n - 2]) // if the new number that was added to the array of numbers(last number index according to the size we got)
    {
      maxPrefixesArray[n - 1] = numbers[n - 1]; // put that number in his new place of the array
    }
    else
    {
      maxPrefixesArray[n - 1] = maxPrefixesArray[n - 2]; // if not it means we haven't found a new max number so we the previous max in the next place until we find a new max number
    }
  }
}
// this function puts the minium number at the start of the array, the other numbers are not in a particular order
void getMinToStart(int numbers[], int n)
{
  int temp; // for the switch if needed

  if (n == ONE_ELEMENT_ONLY) // if we reached the start of the array return
  {
    return;
  }
  getMinToStart(numbers, n - 1); // the recursive step

  if (numbers[n - 1] < numbers[START_OF_ARRAY]) // if the new number that was added to the array is now smaller then the numbers at the start of the array, switch places
  {
    temp = numbers[START_OF_ARRAY];
    numbers[START_OF_ARRAY] = numbers[n - 1];
    numbers[n - 1] = temp;
  }
}
// this function gets two sorted arrays and their sizes and then combine them together to one array(which is the second array) also sorted
void combineArrays(int sortedArr1[], int size1, int sortedArr2[], int size2)
{
  if (size1 == 0) // if we filled all of the first array we are done
  {
    return;
  }

  if (size2 == 0 && size1 != 0) // if the second array is done but the first array haven't finished, put all tje elements of the first array in the second array
  {
    for (; size1 > 0; size1--)
    {
      sortedArr2[size1 - 1] = sortedArr1[size1 - 1];
    }
  }
  else // both array are not finished
  {

    // this part checks who is the max in the last of arr1 and arr2 and then puts the max in the current end of arr2 and then move one step back on the in which the max was at
    if (sortedArr1[size1 - 1] > sortedArr2[size2 - 1]) // if max was at arr1
    {
      sortedArr2[size1 + size2 - 1] = sortedArr1[size1 - 1];
      combineArrays(sortedArr1, size1 - 1, sortedArr2, size2);
    }
    else // if it was on arr2
    {
      sortedArr2[size1 + size2 - 1] = sortedArr2[size2 - 1];
      combineArrays(sortedArr1, size1, sortedArr2, size2 - 1);
    }
  }
}
// this function checks how many numbers in the array are smaller then num
int countSmaller(int arr[], int start, int end, int num)
{
  int mid = (start + end) / 2; // set mid to use binary search

  if (arr[start] >= num) // if the start is bigger then the num it means that there aren't any numbers in the array the are smaller then num
  {
    return start;
  }
  else
  {
    if (arr[end] < num) // if the end is smaller then the number we found all the numbers that are smaller then num
    {
      return end + 1;
    }

    else if (arr[mid] >= num) // if the middle is bigger then the num it means we can cut the whole array by half and now the end is middle - 1
    {
      return countSmaller(arr, start, mid - 1, num);
    }
    else // if the middle wasn't bigger then num we can look only on right half of the array
    {
      return countSmaller(arr, mid + 1, end, num);
    }
  }
}