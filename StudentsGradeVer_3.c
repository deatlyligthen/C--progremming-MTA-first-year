// Ofri kuperberg
// 211440730
// Student_Grades
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_NAME_LEN 20
#define MAX_COURSES 10
#define GROUP_STUDENT_NUMBER 6
#define NUM_OF_GROUPS 3
#define MAX_LETTERS_IN_NAME 10
#define FIRST_GROUP 'A'
#define SECOND_GROUP 'B'
#define LAST_GROUP 'C'
#define MAX_FULL_NAME_LEN 26
#define STUDENT_IN_ACADEMY 18
#define LETTER_IN_GROUPS 7
#define START_OF_NAME_INDEX 7
#define NOT_FOUND -1
#define COUNTER_INDEX 0
#define NUM_OF_GRADES_AND_COUNTER 7
// struct for holding course data
typedef struct CourseInfo
{
  int courseNum;
  int grade;
} COURSE_INFO;

// struct for holding student data
struct Student
{
  char name[MAX_NAME_LEN];
  int identity;
  int nofCourses; // number of courses taken in semesterA
  COURSE_INFO course_info[MAX_COURSES];
};
typedef struct Student STUDENT;

// prototypes
void getStudentsData(STUDENT studentsData[]);
void getCourseAndGrade(COURSE_INFO data[], int size);
void greet();
void sortCourses(COURSE_INFO data[], int size);
void switchPlace(COURSE_INFO *place1, COURSE_INFO *place2);
int getStudentNames(STUDENT stuData[][GROUP_STUDENT_NUMBER], int rows, int cols, int courseNum, char stuNames[][MAX_FULL_NAME_LEN]);
int findCourse(COURSE_INFO searchArr[], int size, int searchNum, bool *foundCourse, int *counter);
void changeNames(char names[][MAX_FULL_NAME_LEN], int studentInCourse);
void CheckIfLois(char fullName[]);
void CheckIfLex(char fullName[]);
void getCourseGradesData(STUDENT stuData[][GROUP_STUDENT_NUMBER], int rows, int cols, int courseNum, int courseGrade[][NUM_OF_GRADES_AND_COUNTER]);
void printCourseGradeDetails(int courseGradeData[][NUM_OF_GRADES_AND_COUNTER], int courseNum);
void CourseGradesInfo(int grades[][NUM_OF_GRADES_AND_COUNTER], int rows);
void printMaxAndSecondMax(int grades[], int size);

int main()
{
  // setting variables for the program
  STUDENT StudentData[NUM_OF_GROUPS][GROUP_STUDENT_NUMBER];
  char studentsNames[STUDENT_IN_ACADEMY][MAX_FULL_NAME_LEN];
  int courseGradeData[NUM_OF_GROUPS][NUM_OF_GRADES_AND_COUNTER];
  char group = FIRST_GROUP;
  int numOfGroup = 0, courseNum, numOfStudentsInCourse; // every group is a different array
  greet();
  for (; group <= LAST_GROUP; group++)
  {
    printf("Enter students data for GROUP %c:", group);
    printf("\n________________________________");
    getStudentsData(StudentData[numOfGroup]); // passing the array of every group
    numOfGroup++;
  }
  printf("\nEnter a course number: \n");
  scanf("%d", &courseNum);
  printf("Names of students in course#%d:\n", courseNum);
  numOfStudentsInCourse = getStudentNames(StudentData, numOfGroup, GROUP_STUDENT_NUMBER, courseNum, studentsNames);
  changeNames(studentsNames, numOfStudentsInCourse);
  getCourseGradesData(StudentData, numOfGroup, GROUP_STUDENT_NUMBER, courseNum, courseGradeData);
  printCourseGradeDetails(courseGradeData, courseNum);
  printf("Maximum grades in course#%d:", courseNum);
  CourseGradesInfo(courseGradeData, NUM_OF_GROUPS);
}
void greet()
{
  printf("********************\n");
  printf("* Welcome Students *\n");
  printf("********************\n");
}
void getStudentsData(STUDENT studentsData[])
{
  int studentIndex = 0;
  char firstName[MAX_LETTERS_IN_NAME];
  char lastName[MAX_LETTERS_IN_NAME];
  char fullName[MAX_NAME_LEN];

  for (studentIndex = 0; studentIndex < GROUP_STUDENT_NUMBER; studentIndex++)
  {
    printf("\nEnter student first name and last name (seperated by spaces): ");
    scanf(" %s %s", firstName, lastName);
    strcpy(fullName, firstName);
    strcat(fullName, " ");
    strcat(fullName, lastName);
    strcpy(studentsData[studentIndex].name, fullName);
    printf("Enter student ID: ");
    scanf("%d", &studentsData[studentIndex].identity);
    printf("\nEnter number of courses taken in semester A:  ");
    scanf("%d", &studentsData[studentIndex].nofCourses);
    getCourseAndGrade(studentsData[studentIndex].course_info, studentsData[studentIndex].nofCourses);
    sortCourses(studentsData[studentIndex].course_info, studentsData[studentIndex].nofCourses);
  }
}
void getCourseAndGrade(COURSE_INFO data[], int size)
{
  for (int i = 0; i < size; i++)
  {
    printf("Enter Course number and grade: ");
    scanf("%d %d", &data[i].courseNum, &data[i].grade);
  }
}
void sortCourses(COURSE_INFO data[], int size)
{
  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size - i - 1; j++)
    {
      if (data[j].courseNum > data[j + 1].courseNum)
      {
        switchPlace(&data[j], &data[j + 1]);
      }
    }
  }
}

void switchPlace(COURSE_INFO *place1, COURSE_INFO *place2)
{
  COURSE_INFO temp;
  temp = *place1;
  *place1 = *place2;
  *place2 = temp;
}
int getStudentNames(STUDENT stuData[][GROUP_STUDENT_NUMBER], int rows, int cols, int courseNum, char stuNames[][MAX_FULL_NAME_LEN])
{
  int groupIndex = 0, studentIndex, studentInCourseIndex = 0, counter = 0;
  char groupChar = FIRST_GROUP;

  for (; groupIndex < rows; groupIndex++)
  {
    for (studentIndex = 0; studentIndex < cols; studentIndex++)
    {
      bool foundCourse = false;
      findCourse(stuData[groupIndex][studentIndex].course_info, stuData[groupIndex][studentIndex].nofCourses, courseNum, &foundCourse, &counter);
      printf("Group%c ",groupChar);

    }
    groupChar++;
  }
  return counter;
}
int findCourse(COURSE_INFO searchArr[], int size, int searchNum, bool *foundCourse, int *counter)

{
  int left = 0, right = size - 1;
  int middle = (left + right) / 2;
  while ((left <= right) && (!(*foundCourse)))
  {
    if (searchArr[middle].courseNum == searchNum)
    {
      *foundCourse = true;
      *counter = *counter + 1;
      return middle;
    }
    else if (searchArr[middle].courseNum > searchNum)
    {
      right = middle - 1;
      middle = (right + left) / 2;
    }
    else
    {
      left = middle + 1;
      middle = (left + right) / 2;
    }
  }
  return NOT_FOUND;
}
void changeNames(char names[][MAX_FULL_NAME_LEN], int studentInCourse)
{
  for (int i = 0; i < studentInCourse; i++)
  {
    CheckIfLois(names[i]);
  }
  printf("\nNames after changing \"Lois\" to \"Lane\":\n");
  for (int k = 0; k < studentInCourse; k++)
  {
    printf("%s\n", names[k]);
  }
  for (int i = 0; i < studentInCourse; i++)
  {
    CheckIfLex(names[i]);
  }
  printf("\nNames after deleting names with \"Lex\":");
  for (int k = 0; k < studentInCourse; k++)
  {
    if (names[k][0] != '\0') // if the name is not empty
    {
      printf("\n%s", names[k]);
    }
  }
}

void CheckIfLois(char fullName[])
{
  int nameIndex = START_OF_NAME_INDEX;
  int lenNameCheck = strlen(fullName) - 3;
  for (; nameIndex < lenNameCheck; nameIndex++)
  {
    if (fullName[nameIndex] == 'L')
    {
      if (fullName[nameIndex + 1] == 'o')
      {
        if (fullName[nameIndex + 2] == 'i')
        {
          if (fullName[nameIndex + 3] == 's')
          {
            fullName[nameIndex + 1] = 'a';
            fullName[nameIndex + 2] = 'n';
            fullName[nameIndex + 3] = 'e';
          }
        }
      }
    }
  }
}
void CheckIfLex(char fullName[])
{
  int nameIndex = START_OF_NAME_INDEX;
  int lenNameCheck = strlen(fullName) - 2;
  // runs until the index+2 is now the end of the string
  for (; nameIndex < lenNameCheck; nameIndex++)
  {
    if (fullName[nameIndex] == 'L')
    {
      if (fullName[nameIndex + 1] == 'e')
      {
        if (fullName[nameIndex + 2] == 'x')
        {
          strcpy(fullName, "\0");
        }
      }
    }
  }
}

void getCourseGradesData(STUDENT stuData[][GROUP_STUDENT_NUMBER], int rows, int cols, int courseNum, int courseGrade[][NUM_OF_GRADES_AND_COUNTER])
{
  int groupIndex = 0, studentIndex, counter = 0, courseIndex;
  int courseGradesIndex = 1, groupNum = 0;

  for (; groupIndex < rows; groupIndex++)
  {
    for (studentIndex = 0; studentIndex < cols; studentIndex++)
    {
      bool foundCourse = false;
      courseIndex = findCourse(stuData[groupIndex][studentIndex].course_info, stuData[groupIndex][studentIndex].nofCourses, courseNum, &foundCourse, &counter);

      if (foundCourse)
      {
        courseGrade[groupNum][courseGradesIndex] = stuData[groupIndex][studentIndex].course_info[courseIndex].grade;
        courseGradesIndex++;
      }
    }
    courseGrade[groupNum][COUNTER_INDEX] = counter;
    counter = 0;
    courseGradesIndex = 1;
    groupNum++;
  }
}
void printCourseGradeDetails(int courseGradeData[][NUM_OF_GRADES_AND_COUNTER], int courseNum)
{
  int groupNum = 0;
  char groupLetter = FIRST_GROUP;
  printf("\n\nGrades in course#%d:\n", courseNum);
  for (; groupLetter <= LAST_GROUP; groupLetter++)
  {
    printf("Group%c: ", groupLetter);
    for (int i = 1; i <= courseGradeData[groupNum][COUNTER_INDEX]; i++)
    {
      printf("%d ", courseGradeData[groupNum][i]);
    }
    groupNum++;
    printf("\n");
  }
}
void CourseGradesInfo(int grades[][NUM_OF_GRADES_AND_COUNTER], int rows)
{
  char groupLetter = FIRST_GROUP;
  for (int i = 0; i < rows; i++)
  {
    printf("\nGroup%c: ", groupLetter);
    if (grades[i][COUNTER_INDEX] == 0)
    {
      printf("no grades found in group");
    }
    else
    {
      printMaxAndSecondMax(grades[i], grades[i][COUNTER_INDEX]);
    }
    groupLetter++;
  }
}
void printMaxAndSecondMax(int grades[], int size)
{
  int max = 0;
  int max2 = 0;
  int maxIndex;
  if (size == 1)
  {
    max = grades[1];
    printf("max grade is: %d (no second max found)", max);
  }
  else
  {
    for (int i = 1; i <= size; i++)
    {
      if (grades[i] > max)
      {
        max = grades[i];
        maxIndex = i;
      }
    }
    for (int i = 1; i <= size; i++)
    {
      if (grades[i] > max2 && grades[i] <= max && i != maxIndex)
      {
        max2 = grades[i];
      }
    }
    printf("max grade is: %d and second max is: %d", max, max2);
  }
}
