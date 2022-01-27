#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define ERROR_CITY "ERROR: The given city format is incorrect.\n"
#define ERROR_COUNTRY "ERROR: The given country format is incorrect.\n"
#define ERROR_AGE "ERROR: The given age format is incorrect.\n"
#define ERROR_GRADE "ERROR: The given grade format is incorrect.\n"
#define ERROR_NAME "ERROR: The given name format is incorrect.\n"
#define ERROR_ID "ERROR: A wrong ID number was inserted.\n"
#define MAX_LINE 151
#define INPUT_MSG "Enter student info. To exit press q, then enter\n"
#define OUTPUT_MSG "best student info is: %s\t%s\t%s\t%s\t%s\t%s\t\n"
#define USAGE_MSG "USAGE: You have entered a wrong command/s, Please choose from the following:" \
    " best, merge or quick.\n"
#define REGEX "%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\t]\t%[^\n]"
#define OUT_REGEX "%s\t%s\t%s\t%s\t%s\t%s\t\n"
#define MAX_SIZE 41
#define BEST "best"
#define MERGE "merge"
#define QUICK "quick"
#define MIN_AGE 18
#define MAX_AGE 120
#define MIN_GRADE 0
#define MAX_GRADE 100
#define MAX_STUDENTS 500
#define LINE_ERROR "in line "
#define EXIT_INPUT_LX "q\r\n"
#define EXIT_INPUT_WN "q\n"


/**
 * A struct that represents a student.
 */
struct Students
{
    char ID[MAX_SIZE];
    char name[MAX_SIZE];
    char grade[MAX_SIZE];
    char age[MAX_SIZE];
    char country[MAX_SIZE];
    char city[MAX_SIZE];
    char extraArgs[MAX_SIZE];
};

void swap(struct Students *s1, struct Students *s2);

/**
 * A method that by the given grade sorts the array and puts them in a new one.
 * @param a An array of all the students that were inserted.
 * @param tmp The array to move the sorted students to.
 * @param left The starting index to sort.
 * @param mid The index to separate from.
 * @param right The end index to sort.
 */
void merge(struct Students a[], struct Students tmp[], int left, int mid, int right)
{
    int left_end, tmpPos;
    left_end = mid - 1;
    tmpPos = left;

    while ((left <= left_end) && (mid <= right))
    {
        if (strtod(a[left].grade, NULL) <= strtod(a[mid].grade, NULL))
        {
            tmp[tmpPos] = a[left];
            tmpPos = tmpPos + 1;
            left = left + 1;
        }
        else
        {
            tmp[tmpPos] = a[mid];
            tmpPos = tmpPos + 1;
            mid = mid + 1;
        }
    }

    while (left <= left_end)
    {
        tmp[tmpPos] = a[left];
        left = left + 1;
        tmpPos = tmpPos + 1;
    }
    while (mid <= right)
    {
        tmp[tmpPos] = a[mid];
        mid = mid + 1;
        tmpPos = tmpPos + 1;
    }
}

/**
 * A method that takes the given array and sorts each half of it separatly.
 * @param a The given students array.
 * @param tmp The array to move the sorted students to.
 * @param left The starting index to sort.
 * @param right The end index to sort.
 */
void mSort(struct Students *a, struct Students *tmp, int left, int right)
{
    int mid;
    if (right > left)
    {
        mid = (right + left) / 2;
        mSort(a, tmp, left, mid);
        mSort(a, tmp, mid + 1, right);
        merge(a, tmp, left, mid + 1, right);
    }
}

/**
 * A method that sorts the array with merge sort.
 * @param a The given students array.
 * @param tmp The array to move the sorted students to.
 * @param size The size of the array.
 */
void mergeSort(struct Students *a, struct Students *tmp, const int size)
{
    mSort(a, tmp, 0, size);
}

/**
 * A method that changes the place between two elements of the array.
 * @param s1 The first student.
 * @param s2 The second student.
 */
void swap(struct Students *s1, struct Students *s2)
{
    struct Students temp;
    temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

/**
 * A method that takes 2 indices and returns the half of their addition.
 * @param left The starting index to sort.
 * @param right The end index to sort.
 * @return The half of the addition of the 2 indices.
 */
int choosePivot(int left, int right )
{
    return((left + right) / 2);
}

/**
 * A method that sorts the given array with recursion.
 * @param list The given student list to sort through.
 * @param left the starting index to sort.
 * @param right the end index to sort.
 */
void quickSort(struct Students *list, int left, int right)
{
    int i, j, k;
    char key[MAX_SIZE];
    if(left < right)
    {
        k = choosePivot(left, right);
        swap(&list[left], &list[k]);
        strcpy(key, list[left].name);
        i = left + 1;
        j = right;
        while(i <= j)
        {
            while((i <= right) && (strcmp(list[i].name, key) <= 0))
            {
                i++;
            }

            while((j >= left) && (strcmp(list[j].name, key) > 0))
            {
                j--;
            }

            if(i < j)
            {
                swap(&list[i], &list[j]);
            }

        }
        /* swap two elements */
        swap(&list[left], &list[j]);

        /* recursively sort the smaller list */
        quickSort(list, left, j - 1);
        quickSort(list, j + 1, right);
    }
}

/**
 * A method that checks that the given array includes only valid characters.
 * @param ID An array of characters.
 * @return 0 if the array contains invalid characters, 1 otherwise.
 */
int checkID(const char *ID)
{
    if (strlen(ID) != 10)
    {
        printf(ERROR_ID);
        return 0;
    }
    if (ID[0] < 49 || ID[0] > 57)
    {
        printf(ERROR_ID);
        return 0;
    }
    for (int i = 1; i<9; i++)
    {
        if(ID[i] < 48 || ID[i] > 57)
        {
            printf(ERROR_ID);
            return 0;
        }
    }
    return 1;
}

/**
 * A method that checks that the given int is within the valid range.
 * @param grade An array of characters.
 * @return 0 if the array contains invalid characters, 1 otherwise.
 */
int checkGrade(const char *grade)
{
    if (strlen(grade) < 1 || strlen(grade) > 3)
    {
        printf(ERROR_GRADE);
        return 0;
    }
    for (unsigned long i = 0; i < strlen(grade); i++)
    {
        if (i == 0 && (grade[i]<49 || grade[i]>57) && strlen(grade) !=1)
        {
            printf(ERROR_AGE);
            return 0;
        }
        else if(grade[i]<48 || grade[i]>57)
        {
            printf(ERROR_GRADE);
            return 0;
        }
    }
    double gradeNum = strtod(grade, NULL);
    if (gradeNum < MIN_GRADE || gradeNum > MAX_GRADE)
    {
        printf(ERROR_GRADE);
        return 0;
    }
    return 1;
}

/**
 * A method that checks that the given int is within the valid range.
 * @param age An An array of characters.
 * @return 0 if the array contains invalid characters, 1 otherwise.
 */
int checkAge(const char *age)
{
    if (strlen(age) < 2 || strlen(age) > 3)
    {
        printf(ERROR_AGE);
        return 0;
    }
    for (unsigned long i = 0; i < strlen(age); i++)
    {
        if (i == 0 && (age[i]<49 || age[i]>57) && strlen(age) != 1)
        {
            printf(ERROR_AGE);
            return 0;
        }
        else if(age[i]<48 || age[i]>57)
        {
            printf(ERROR_AGE);
            return 0;
        }
    }
    double ageNum = strtod(age, NULL);
    if (ageNum < MIN_AGE || ageNum > MAX_AGE)
    {
        printf(ERROR_AGE);
        return 0;
    }
    return 1;
}

/**
 * A method that checks that the given array includes only valid characters.
 * @param arr An array of characters.
 * @param flag An int value representing the given type: 0 = name, 1 = country, 2  = city.
 * @return 0 if the array contains invalid characters, 1 otherwise.
 */
int checkCharArr(const char *arr, int const flag)
{
    if (strlen(arr) == 0)
    {
        if (flag == 0)
        {
            printf(ERROR_NAME);
        }
        else if (flag == 1)
        {
            printf(ERROR_COUNTRY);
        }
        else
        {
            printf(ERROR_CITY);
        }
        return 0;
    }
    for (unsigned long i = 0; i<strlen(arr) ; i++)
    {
        if (flag == 0) // The name array was given.
        {
            if ((arr[i] >= 65 && arr[i] <= 90) || (arr[i] >= 97 && arr[i] <= 122) || arr[i] == 32\
                || arr[i] == 45)
            {
                continue;
            }
            else
            {
                printf(ERROR_NAME);
                return 0;
            }
        }
        else // City or country array was given.
        {
            if ((arr[i] >= 65 && arr[i] <= 90) || (arr[i] >= 97 && arr[i] <= 122) || arr[i] == 45)
            {
                continue;
            }
            else if (flag == 1) // Country array was given.
            {
                printf(ERROR_COUNTRY);
                return 0;
            }
            else // City array was given.
            {
                if (strcmp(&arr[strlen(arr)-1], "\n") != 0 && (strcmp(&arr[strlen(arr)-1], "\r") != 0))
                {
                    printf(ERROR_CITY);
                    return 0;
                }
            }
        }
    }
    return 1;
}

/**
 * A method that checks if the given input fields are valid
 * @param ID A given char array representing the ID.
 * @param name A given char array representing the name.
 * @param grade A given char array representing the grade.
 * @param age A given char array representing the age.
 * @param country A given char array representing the country.
 * @param city A given char array representing the city.
 * @param num A given integer representing the number of arguments given through the input.
 * @return  2 if the character "q" was entered, 1 if the student input was valid,
 * 0 for error in a field.
 */
int checkValid(const char *ID, const char *name, const char *grade, const char *age,\

               const char *country, const char *city)
{
    int flag = 0;
    if (strcmp(ID, EXIT_INPUT_LX) == 0 || strcmp(ID, EXIT_INPUT_WN) == 0) // Check if "q" has been pressed.
    {
        return 2;
    }
    flag = checkID(ID);
    if (flag != 0)
    {
        flag = checkCharArr(name, 0);
    }
    if (flag != 0)
    {
        flag = checkGrade(grade);
    }
    if (flag != 0)
    {
        flag = checkAge(age);
    }
    if (flag != 0)
    {
        flag = checkCharArr(country, 1);
    }
    if (flag != 0)
    {
        flag = checkCharArr(city, 2);
    }
    if (flag != 0)
    {
        return 1; // If all the fields are valid.
    }
    return 0;
}

/**
 * A method that asks the user for input.
 * @return The number of valid students entered by the user.
 */
int getStudents(struct Students *students_Array)
{
    int numStudents = 0, numLine = 0;
    char buffer[MAX_LINE] = {0};
    printf(INPUT_MSG);
    fgets(buffer, MAX_LINE, stdin);
    while (strcmp(buffer, EXIT_INPUT_LX) != 0 || strcmp(buffer, EXIT_INPUT_WN) != 0)
    {
        struct Students stu;
        sscanf(buffer, REGEX , stu.ID, stu.name, stu.grade, stu.age, stu.country,\
         stu.city, stu.extraArgs);
        int out = checkValid(stu.ID, stu.name, stu.grade, stu.age, stu.country, stu.city);
        if(out == 2)
        {
            break;
        }
        if(out == 1)
        {
            students_Array[numStudents] = stu;
            numStudents++;
        }
        else
        {
            printf("%s%d\n", LINE_ERROR, numLine);
        }
        numLine++;
        printf(INPUT_MSG);
        fgets(buffer, MAX_LINE, stdin);
    }
    return numStudents;
}

/**
 * A method that checks that the commands past in the CLI were correct.
 * @param argc The amount of arguments passed through the command line.
 * @param argv An array of arguments passed through the command line.
 * @return 0 if the arguments are valid, 0 otherwise.
 */
int checkCommand(int argc, char *const *argv)
{
    if (argc != 2)
    {
        printf(USAGE_MSG);
        return 1;
    }
    else if (strcmp(argv[1], BEST) != 0  && strcmp(argv[1], MERGE) != 0\
             && strcmp(argv[1], QUICK) != 0)
    {
        printf(USAGE_MSG);
        return 1;
    }
    return 0;
}

/**
 * A method that goes through the list of students and given the command of the user
 * prints the students.
 * @param argv The list of arguments given by the user.
 * @param studentsArray the array of all the valid students entered by the user.
 * @param numStudents The number of valid students entered by the user.
 */
void printStu(char *const *argv, struct Students *studentsArray, int numStudents)
{
    if (numStudents == 1 && strcmp(argv[1], BEST) == 0)
    {
        printf(OUTPUT_MSG, studentsArray[0].ID, studentsArray[0].name, studentsArray[0].grade, \
        studentsArray[0].age, studentsArray[0].country, studentsArray[0].city);
        return;
    }
    else if (numStudents == 1)
    {
        printf(OUT_REGEX, studentsArray[0].ID, studentsArray[0].name, studentsArray[0].grade, \
        studentsArray[0].age, studentsArray[0].country, studentsArray[0].city);
        return;
    }
    if (strcmp(argv[1], BEST) == 0)
    {
        int place = 0;
        double max = 0;
        for (int j = 0; j < numStudents; j++)
        {
            int grade = (int) strtol(studentsArray[j].grade, NULL, 10);
            int age = (int) strtol(studentsArray[j].age, NULL, 10);
            double value = (double) grade / age;
            if (value > max)
            {
                max = value;
                place = j;
            }
        }
        printf(OUTPUT_MSG, studentsArray[place].ID, studentsArray[place].name, studentsArray[place].grade, \
        studentsArray[place].age, studentsArray[place].country, studentsArray[place].city);
    }
    else if (strcmp(argv[1], MERGE) == 0)
    {
        struct Students temp[MAX_STUDENTS];
        mergeSort(studentsArray, temp, numStudents);
        for (int i = 0; i < numStudents; i++)
        {
            printf(OUT_REGEX, studentsArray[i].ID, studentsArray[i].name, studentsArray[i].grade, \
        studentsArray[i].age, studentsArray[i].country, studentsArray[i].city);
        }
    }
    else
    {
        quickSort(studentsArray, 0, numStudents - 1);
        for (int i = 0; i < numStudents; i++)
        {
            printf(OUT_REGEX, studentsArray[i].ID, studentsArray[i].name, studentsArray[i].grade, \
        studentsArray[i].age, studentsArray[i].country, studentsArray[i].city);
        }
    }
}

/**
 * The main method that runs the program.
 * @param argc An int representing The amount of arguments given through the command line.
 * @param argv An array of the arguments given through the command line.
 * @return 0 if the program ran as intended, 0 otherwise.
 */
int main(int argc, char* argv[])
{
    if(checkCommand(argc, argv) == 1)
    {
        return 1;
    }
    struct Students students_Array[MAX_STUDENTS];
    int numStudents = getStudents(students_Array);
    if (numStudents == 0)
    {
        return 0;
    }
    printStu(argv, students_Array, numStudents);
    return 0;
}