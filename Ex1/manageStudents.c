/**
 * @file manageStudents.c
 * @author  Muaz Abdeen <muaz.abdeen@mail.huji.ac.il>
 * @date 20 April 2020
 *
 * @brief Program that receives from user a list of students info:
 * 			<ID>,<name>,<grade>,<age>,<country>,<city>
 * 		  and executes on the valid input one of the following:
 *        [1] Shows the best student according to the ratio (grade:age).
 *        [2] Sorts the students in ascending order by their grades using MergeSort.
 *        [3] Sorts the students in ascending order by their names using QuickSort.
 *
 * @section DESCRIPTION
 * Program that analyzes students info and sorts it.
 * Input  : Student's info: <ID>,<name>,<grade>,<age>,<country>,<city>
 * Process: Responds to the input by according to the run argument in shell:
 * 			[1] best: shows the best student.
 * 			[2] merge: MergeSort students by grade in ascending order.
 * 			[3] quick: QuickSort students by name in ascending order.
 * Output : > If the input is not valid - print informative message
 *          > Else if the input is valid – print best student info, or sorted list
 *            of students according to run argument.
 */

// ------------------------------ includes ------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ------------------------------ constants ------------------------------

// These are the valid run arguments
const char BEST[] = "best";
const char MERGE[] = "merge";
const char QUICK[] = "quick";

const int NUM_FIELDS = 6;

const char REQUEST_STUDENT_INFO[] = "Enter student info. To exit press q, then enter";
const char ARGS_NUM_ERR[] = "MUST insert ONE argument.";
const char ARGS_NOT_SUITS[] = "We support just these: best, merge, and quick.";
const char NOT_VALID_ID[] = "StudentID MUST be a TEN digit number NOT starts with ZERO.";
const char NOT_VALID_STUDENT_NAME[] = "Student name MAY contain ONLY alphabetical letters, "
									  "whitespaces, or '-'.";
const char NOT_VALID_GRADE[] = "Grade MUST be a number between (0-100) includes, NOT starts "
							   "with ZERO, unless it is ZERO.";
const char NOT_VALID_AGE[] = "Student Age Must be between (18-120) includes, NOT starts "
							 "with Zero.";
const char NOT_VALID_LOCATION[] = "Country and city, MAY contain ONLY alphabetical letters"
								  " and '-'.";
const char ERROR_NUM_FIELDS[] = "Number of fields must be EXACTLY SIX.";

// ------------------------------ functions & structs -----------------------------

struct Student;

int checkDigit(char *arr);
int checkID(char *stuID);
int checkName(char *stuName);
int checkGrade(char *stuGrade);
int checkAge(char *stuAge);
int checkLocation(char *location);
int checkField(char *userInput);
int checkInput(char *userInput);
int buildStudentsList(struct Student *stuList);
void best(struct Student *stuList);
void merge(struct Student *stuList);
void mergeHelper(int stuGrades[], int idxFirst1, int idxEnd1, int idxFirst2, int idxEnd2);
void mergeSort(int stuGrades[], int idxFirst, int idxEnd);
void swap(char **arr1, char **arr2);
int partition(char *stuName[], int idxLow, int idxHigh);
void quickSort(char *stuName[], int idxLow, int idxHigh);
void quick(struct Student *stuList);

// --------------------------------- Student struct ---------------------------------

/**
 * @brief A structure to represent student info.
 */
typedef struct Student
{
	int validLine;	/**< validity of info: 1 if valid, -1 if not  */
	int checked;	/**< if the line fully processed 1, otherwise 0 */
	char studentID[15], grade[5], age[5];
	char name[50], country[50], city[50];
	float achievement;	/**< student achievement = grade:age */
} Student;


/**
 * @brief Check if all the array chars are integer numbers (digits).
 * @param *arr pointer to the char array
 * @return 0 (= False) not integer number, 1 (= True) otherwise.
 */
int checkDigit(char *arr)
{
	for (int i = 0; i < (int) strlen(arr); i++)
	{
		if (! isdigit(arr[i]))
		{
			return 0;
		}
	}
	return 1;
}

/**
 * @brief Check if studentID is a ten digit number not starts with 0.
 * @param *stuID pointer to the char array that contains studentID.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkID(char *stuID)
{
	if (checkDigit(stuID))
	{
		int numDigits = (int) strlen(stuID);
		if (numDigits != 10 || stuID[0] == '0')
		{
			printf("ERROR: %s\n", NOT_VALID_ID);
			return 0;
		}
	}
	else
	{
		printf("ERROR: %s\n", NOT_VALID_ID);
		return 0;
	}
	return 1;
}

/**
 * @brief Check if student name only contains english letters, or whitespace, or "-".
 * @param *stuName pointer to the char array that contains student name.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkName(char *stuName)
{
	for (int i = 0; i < (int) strlen(stuName); i++)
	{
		if (((char) stuName[i] != '-') && ! isalpha(stuName[i]) && ! isspace(stuName[i]))
		{
			printf("ERROR: %s\n", NOT_VALID_STUDENT_NAME);
			return 0;
		}
	}
	return 1;

}

/**
 * @brief Check if student grade only contains integers between (0-100) and not starts with 0.
 * @param *stuGrade pointer to the char array that contains student grade.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkGrade(char *stuGrade)
{
	if (checkDigit(stuGrade))
	{
		// convert stuGrade from char array to number
		char *ptr;
		long grade = strtol(stuGrade, &ptr, 10);
		if (grade < 0 || grade > 100 || (grade != 0 && stuGrade[0] == '0'))
		{
			printf("ERROR: %s\n", NOT_VALID_GRADE);
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		printf("ERROR: %s\n", NOT_VALID_GRADE);
		return 0;
	}
}

/**
 * @brief Check if student age only contains integers between (18-120).
 * @param *stuAge pointer to the char array that contains student age.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkAge(char *stuAge)
{
	if (checkDigit(stuAge))
	{
		// convert stuAge from char array to number
		char *ptr;
		long age = strtol(stuAge, &ptr, 10);
		if (18 <= age && age <= 120 && stuAge[0] != '0')
		{
			return 1;
		}
	}
	printf("ERROR: %s\n", NOT_VALID_AGE);
	return 0;
}

/**
 * @brief Check if country ot city only contain letters or "-".
 * @param *location pointer to the char array that contains student country ot city.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkLocation(char *location)
{
	for (int i = 0; i < (int) strlen(location); i++)
	{
		if (((char) location[i] != '-') && ! isalpha(location[i]))
		{
			printf("ERROR: %s\n", NOT_VALID_LOCATION);
			return 0;
		}
	}
	return 1;
}

/**
 * @brief Check if student info contains exactly six fields.
 * @param *userInput pointer to the char array that contains student info.
 * @return 0 (= False) if it is not, 1 (= True) otherwise.
 */
int checkField(char *userInput)
{
	int count = 0;
	for (int i = 0; i < (int) strlen(userInput) - 1; i++)
	{
		if (userInput[i] == ',')
		{
			count++;
		}
	}
	if (count != (NUM_FIELDS - 1))
	{
		printf("ERROR: %s\n", ERROR_NUM_FIELDS);
		return 0;
	}
	return 1;
}

/**
 * @brief assembles all validity check functions.
 * @param *userInput pointer to the char array that contains student info.
 * @return 0 (= False) if it is valid, 1 (= True) otherwise.
 */
int checkInput(char *userInput)
{
	if (! checkField(userInput))
	{
		return 0;
	}
	char name[50], country[50], city[50];
	char tempStuID[50], tempGrade[50], tempAge[50];

	sscanf(userInput, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^'\n']", tempStuID, name,
		   tempGrade, tempAge, country, city);
	if (! checkID(tempStuID))
	{
		return 0;
	}
	if (! checkName(name))
	{
		return 0;
	}
	if (! checkGrade(tempGrade))
	{
		return 0;
	}
	if (! checkAge(tempAge))
	{
		return 0;
	}
	if (! checkLocation(country))
	{
		return 0;
	}
	if (! checkLocation(city))
	{
		return 0;
	}
	return 1;
}

/**
 * @brief Fills array of Student structs with students info & calculate his achievement.
 * @param *stuList pointer to the array of Student structs.
 * @return number of lines (valid or not) the user inserts.
 */
int buildStudentsList(Student *stuList)
{
	char studentInfo[100];

	printf ("%s\n", REQUEST_STUDENT_INFO);
	fgets(studentInfo, sizeof(studentInfo), stdin);

	int line = 0;	// number of student list lines
	while (strcmp(studentInfo, "q\n") != 0)	 // while user does not type "q"
	{
		if (checkInput(studentInfo))
		{
			sscanf(studentInfo, "%[^','],%[^','],%[^','],%[^','],%[^','],%[^'\n']",
				   stuList[line].studentID, stuList[line].name, stuList[line].grade,
				   stuList[line].age, stuList[line].country, stuList[line].city);

			// calculate student achievement (= grade:age)
			char *gradePtr, *agePtr;
			float stuGrade = (float) strtol(stuList[line].grade, &gradePtr, 10);
			float stuAge = (float) strtol(stuList[line].age, &agePtr, 10);
			stuList[line].achievement = stuGrade / stuAge;

			stuList[line].validLine = 1;
			stuList[line].checked = 0;
		}
		else
		{
			stuList[line].validLine = -1;
			printf("in line %d\n", line);
		}

		// Request another student info, and increment number of list lines by one
		printf ("%s\n", REQUEST_STUDENT_INFO);
		fgets(studentInfo, sizeof(studentInfo), stdin);
		line++;
	}
	return line;
}

/**
 * @brief Prints to the stdout info of best student.
 * @param *stuList pointer to the array of Student structs.
 * @return nothing.
 */
void best(Student *stuList)
{
	int listSize = buildStudentsList(stuList);
	float bestAchievement = stuList[0].achievement;
	int bestNum = 0;	// line number that contains best student
	int validInput = 0;	// number of valid input lines
	for (int i = 0; i < listSize; i++)
	{
		if (stuList[i].achievement > bestAchievement)
		{
			bestAchievement = stuList[i].achievement;
			bestNum = i;
		}
		if (stuList[i].validLine == 1)
		{
			validInput++;
		}
	}
	if (validInput)	// there is at least one valid input inserted
	{
		printf("best student info is: %s,%s,%s,%s,%s,%s\n", stuList[bestNum].studentID,
			   stuList[bestNum].name, stuList[bestNum].grade, stuList[bestNum].age,
			   stuList[bestNum].country, stuList[bestNum].city);
	}
}

/**
 * @brief Sorts the students in ascending order by their grades using MergeSort.
 * @param *stuList pointer to the array of Student structs.
 * @return nothing.
 */
void merge(Student *stuList)
{
	// calculates size of the array to be sorted
	int listSize = buildStudentsList(stuList);
	int lenMergeArray = 0;
	for (int i = 0; i < listSize; i++)
	{
		if (stuList[i].validLine == 1)
		{
			lenMergeArray++;
		}
	}

	// Put the grade to be sorted in the to-sort array
	int mergeArray[5500];
	int j = 0; // index of mergeArray
	for (int i = 0; i < listSize; i++)
	{
		if (stuList[i].validLine == 1)
		{
			char *gradePtr;
			float stuGrade = (float) strtol(stuList[i].grade, &gradePtr, 10);
			mergeArray[j] = (int) stuGrade;
			j++;
		}
	}

	// execute of MergeSort
	mergeSort(mergeArray, 0, lenMergeArray - 1);

	// Prints the sorted list to the (stdout)
	for(int i = 0; i < lenMergeArray; i++)
	{
		for(int k = 0; k < listSize; k++)
		{
			char *gradePtr;
			float stuGrade = (float) strtol(stuList[k].grade, &gradePtr, 10);
			if (mergeArray[i] == (int) stuGrade && stuList[k].checked == 0)
			{
				stuList[k].checked = 1;
				printf("%s,%s,%s,%s,%s,%s\n", stuList[k].studentID, stuList[k].name,
					   stuList[k].grade, stuList[k].age, stuList[k].country, stuList[k].city);
				break;
			}
		}
	}
}

/**
 * @brief helper to MergeSort, merge two sub arrays into one sorted array.
 * @param stuGrades pointer to array of integers contains students grades
 * @param idxFirst1 starting index of first array
 * @param idxEnd1 ending index of first array
 * @param idxFirst2 starting index of second array
 * @param idxEnd2 ending index of second array
 */
void mergeHelper(int stuGrades[], int idxFirst1, int idxEnd1, int idxFirst2, int idxEnd2)
{
	int helperArray[5500];
	int idxHelper = 0;	//index for the helper array
	int subFirst = idxFirst1;	//beginning of the first subarray
	int subSecond = idxFirst2;	//beginning of the second subarray

	// while not reached to the end of one of the sub arrays
	while (subFirst <= idxEnd1 && subSecond <= idxEnd2)
	{
		if (stuGrades[subFirst] < stuGrades[subSecond])
		{
			helperArray[idxHelper++] = stuGrades[subFirst++];
		}
		else
		{
			helperArray[idxHelper++] = stuGrades[subSecond++];
		}
	}

	// move remaining items in first subarray
	while (subFirst <= idxEnd1)
	{
		helperArray[idxHelper++] = stuGrades[subFirst++];
	}

	// move remaining items in second subarray
	while (subSecond <= idxEnd2)
	{
		helperArray[idxHelper++] = stuGrades[subSecond++];
	}

	for (int i = idxFirst1, j = 0; i <= idxEnd2; i++, j++)
	{
		stuGrades[i] = helperArray[j];
	}
}

/**
 * @brief executes the MergeSort by divide and conquer
 * @param stuGrades pointer to array of integers contains students grades
 * @param idxFirst index to the first char in the array
 * @param idxEnd index to the last char in the array
 */
void mergeSort(int stuGrades[], int idxFirst, int idxEnd)
{
	if (idxFirst < idxEnd)
	{
		int idxMid = (idxFirst + idxEnd) / 2;
		mergeSort(stuGrades, idxFirst, idxMid);
		mergeSort(stuGrades, idxMid + 1, idxEnd);
		mergeHelper(stuGrades, idxFirst, idxMid, idxMid + 1, idxEnd);
	}
}

/**
 * @brief helper function to swap two strings in an array
 * @param arr1 pointer to first string
 * @param arr2 pointer to second string
 */
void swap(char **arr1, char **arr2)
{
	char *temp = *arr1;
	*arr1 = *arr2;
	*arr2 = temp;
}

/**
 * @brief helper function to QuickSort, cuts the main array into two sub arrays,
 * 		  the left contains values less than pivot, the right contains values
 * 		  greater than pivot.
 * @param stuName pointer to the array of student names (strings)
 * @param idxLow index to the first name in the array
 * @param idxHigh index to the last name in the array
 * @return index of the pivot after partition
 */
int partition(char *stuName[], int idxLow, int idxHigh)
{
	char pivot[45];	// string to put the pivot name in
	strcpy(pivot, stuName[idxHigh]);	// pivot name (the last name)

	int idxSmallest = (idxLow - 1);
	for (int j = idxLow; j <= (idxHigh - 1); j++)
	{
//		char tempPivot[45];
//		char tempName[45];
//
//		// convert to lowercase before comparing
//		for (int i = 0; i < 45; i++)
//		{
//			tempPivot[i] = tolower(pivot[i]);
//			tempName[i] = tolower(stuName[j][i]);
//		}

		if (strcmp(stuName[j], pivot) < 0)
		{
			idxSmallest++;
			swap(&stuName[idxSmallest], &stuName[j]);
		}
	}
	swap(&stuName[idxSmallest + 1], &stuName[idxHigh]);
	return (idxSmallest + 1);
}

/**
 * @brief executes the QuickSort by divide and conquer
 * @param stuName pointer to the array of student names (strings)
 * @param idxLow index to the first name in the array
 * @param idxHigh index to the last name in the array
 */
void quickSort(char *stuName[], int idxLow, int idxHigh)
{
	if (idxLow < idxHigh)
	{
		int idxPart = partition(stuName, idxLow, idxHigh);

		quickSort(stuName, idxLow, idxPart - 1);
		quickSort(stuName, idxPart + 1, idxHigh);
	}
}

/**
 * @brief Sorts the students in ascending order by their names using QuickSort.
 * @param *stuList pointer to the array of Student structs.
 * @return nothing.
 */
void quick(Student *stuList)
{
	// calculates size of the array to be sorted
	int listSize = buildStudentsList(stuList);
	int lenSortArray = 0;
	for (int i = 0; i < listSize; i++)
	{
		if (stuList[i].validLine == 1)
		{
			lenSortArray++;
		}
	}

	// Put the names to be sorted in the to-sort array
	char *sortArray[5500];
	int j = 0; // index of sortArray
	for (int i = 0; i < listSize; i++)
	{
		if (stuList[i].validLine == 1)
		{
			sortArray[j] = stuList[i].name;
			j++;
		}
	}

	// execute QuickSort
	quickSort(sortArray, 0, lenSortArray - 1);

	// Prints the sorted list to the (stdout)
	for(int i = 0; i < lenSortArray; i++)
	{
		for(int k = 0; k < listSize; k++)
		{
			if (sortArray[i] == stuList[k].name && stuList[k].checked == 0)
			{
				stuList[k].checked = 1;
				printf("%s,%s,%s,%s,%s,%s\n", stuList[k].studentID, stuList[k].name,
					   stuList[k].grade, stuList[k].age, stuList[k].country, stuList[k].city);
				break;
			}
		}
	}
}


/**
 * @brief The main function.
 *        Prints Usage error given arguments not valid.
 *        Prints best student info if input is "best".
 *        Prints sorted list of students by grade if input is "merge".
 *        Prints sorted list of students by name if input is "quick".
 * @return 0, to tell the system the execution ended without errors, otherwise, 1.
 */

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("USAGE: %s\n", ARGS_NUM_ERR);
		return 1;
	}

	Student studentsList[5500];

	if (! strcmp(argv[1], BEST))
	{
		best(studentsList);
	}

	else if (! strcmp(argv[1], MERGE))
	{
		merge(studentsList);
	}

	else if (! strcmp(argv[1], QUICK))
	{
		quick(studentsList);
	}

	else
	{
		printf("USAGE: %s\n", ARGS_NOT_SUITS);
		return 1;
	}

	return 0;
}
