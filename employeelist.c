/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 2
 * Name: Justin Chin Wei Kit
 * Student ID: 13595766
 * Date of submission: 27th of September 2021
 * A brief statement on what you could achieve (less than 50 words):
 * I was able to complete all the required functionality set
 * in the assignment task and also handled the various scenarios of 
 * careless user inputs, including the handling of long inputs
 * A brief statement on what you could NOT achieve (less than 50 words):
 * Nothing, as every functionality (that was required in the assessment) 
 * was completed with all functions passing the test cases, 
 * everything was achieved and fulfilled to the required standards
 * 
*******************************************************************************/

/*******************************************************************************
 * List header files - do NOT use any other header files. Note that stdlib.h is
 * included in case you want to use any of the functions in there. However the
 * task can be achieved with stdio.h and string.h only.
*******************************************************************************/

#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, scanf, fprintf, fscanf, feof, 
					  rewind, fopen, fclose, ftell, fseek */
#include <string.h> /* strcpy, strlen, strcspn */

/*******************************************************************************
 * List preprocessing directives - you may define your own.
*******************************************************************************/

#define MAX_COMPANY_SIZE 5
#define MAX_NAME_SIZE 11

/*******************************************************************************
 * List structs - you may define struct date and struct employee only. Each
 * struct definition should have only the fields mentioned in the assignment
 * description.
*******************************************************************************/

struct date {
	int day;
	int month;
	int year;
};
typedef struct date date_t;

struct employee {
	char name[MAX_NAME_SIZE];
	float fte;
	int level;
	date_t birthday;
};
typedef struct employee employee_t;

/*******************************************************************************
 * Function prototypes - do NOT change the given prototypes. However you may
 * define your own functions if required.
*******************************************************************************/

/* Main Program Functions */
int add_employee(employee_t employee_list[], int employee_count);
int delete_last_employee(int employee_count);
void display_employee(const employee_t employee_list[], const int employee_count);
int save_to_db(const employee_t employee_list[], const int employee_count, char db[]);
int read_from_db(employee_t employee_list[], char db[]);

/* Helper Functions */
/* Getter Functions */
char* get_name();
date_t get_date(void);
float get_fte();
int get_level();

/* Print Functions */
void printMenu(void);
void print_space(const char str[], const int spaces);
void print_birthday(const date_t date);
void print_fte(const float fte);
void print_level(const int level);

/* Checking Functions */
int check_fte(const float fte);
int check_range(const int check_val, const int min, const int max);
int check_level(const int level);
int check_day(const int day);
int check_month(const int month);
int check_year(const int year);

/*******************************************************************************
 * Main
*******************************************************************************/

int main(void){
	int choice;
	char dbFileName[] = "database";
	employee_t employee_list[MAX_COMPANY_SIZE];
	int employee_count = 0;

	/* Select a Choice */
    printMenu();
	printf("Enter your choice>");
	scanf("%d", &choice);
	
	while ( choice != 6 ) {
		switch (choice) {
			case 1: /* Add an employee to the list */
				if ( employee_count < MAX_COMPANY_SIZE ) {
					getchar(); /* getchar fixes the fgets not catching the newline char */
					employee_count = add_employee(employee_list, employee_count);
				}
				else { 		/* Employee List is full */
					printf("Maximum number of employees reached.\n");
				}
				break;
			case 2: /* Deleting the last employee from the list */
				if ( employee_count == 0 ) {
					printf("No employee.\n");
				} else {	/* If there is an available employee, reduce the count by 1*/
					employee_count = delete_last_employee(employee_count);
				}
				break;
			case 3: /* Print a list of all employees and their details */
				if ( employee_count != 0 ) {
					display_employee(employee_list, employee_count);
				} else {
					printf("No employee.\n");
				}
				break;
			case 4: /* Save any employees found in the list into a database file */
				save_to_db(employee_list, employee_count, dbFileName);
				break;
			case 5: /* Load employees from a database file into the list */
				employee_count = read_from_db(employee_list, dbFileName);
				break;
			default:
				printf("Invalid choice.\n");
				break;	
		}
		/* Allow the user to retry if input was incorrect */
		printMenu();
		printf("Enter your choice>");
		scanf("%d", &choice);
	}
	return 0;
}

/*******************************************************************************
 * Main Functionality
*******************************************************************************/

/* This functions adds a new employee of type employee_t 
 * into the employee_list array
 * @param employee_list - the employee array to add into
 * @param employee_count - current number of employees stored in employee_list
 * @return - the employee count after a new employee is added 
 */
int add_employee(employee_t employee_list[], int employee_count) {
	employee_t new_employee;

	/* Get the name of the employee */
	printf("Enter name>");
	strcpy(new_employee.name, get_name());

	/* Get the necessary fields of data and store it */
	new_employee.birthday = get_date();
	new_employee.fte = get_fte();
	new_employee.level = get_level();

	employee_list[employee_count++] = new_employee;
	return employee_count;
}

/* This function deletes the latest employee and reduces the count by one
 * The employee_list array does not need to be changed, because when a new
 * employee is added, it will overwrite the old space instead, hence
 * reducing the employee count is the only necessary step
 * @param employee_count - current number of employees
 * @return - employee count subtracted by one
 */
int delete_last_employee(int employee_count) {
	return --employee_count; 
}

/* This function displays all added employees onto the terminal
 * @param employee_list - the array used to retrieve and print the information
 * @param employee_count - current number of employees stored in employee_list
 */
void display_employee(const employee_t employee_list[], const int employee_count) {
	/* Print the header format */
	printf("Name       Birthday   FTE   Level\n");
	printf("---------- ---------- ----- -----\n");

	/* Loop around the list using the current employee count
	 * Print all the details in order of name, birthday dates,
	 * FTE and level in that specific order
	 * Each individual print is explaiend in the print functions section
	 */
	int i;
	for ( i = 0; i < employee_count; i++ ) {
		print_space(employee_list[i].name,MAX_NAME_SIZE);
		print_birthday(employee_list[i].birthday);
		print_fte(employee_list[i].fte);
		print_level(employee_list[i].level);
		printf("\n");
	}
}

/* This function saves any employees that are stored in the employee_list array
 * and will write it into a file named "database"
 * If the file does not exist, it will create one first
 * @param employee_list - the array used to retrieve and print the information
 * @param employee_count - current number of employees stored in employee_list
 * @param db - the database being written into
 */
int save_to_db(const employee_t employee_list[], const int employee_count, char db[]) {
	FILE *fp = fopen(db, "w");
	if (fp == NULL ) {
		printf("Write error\n");
		return 1;
	}

	/* Write all of the employee's details on one line
	 * Each detail is spaced out by one
	 * Format is name, fte, level, day, month, year in that order
	 */
	int i;
	for ( i = 0; i < employee_count; i++ ) {
		employee_t temp_emp = employee_list[i];
		fprintf( 
			fp, 
			"%s %f %d %d %d %d\n", 
			temp_emp.name, 
			temp_emp.fte, 
			temp_emp.level, 
			temp_emp.birthday.day, 
			temp_emp.birthday.month, 
			temp_emp.birthday.year
		);
	}
	fclose(fp);

	return 0;
}

/* This function loads employee's data from a file named "database"
 * @param employee_list - the employee list array it will load into
 * @param db - the database being read from
 * @return - the count of employees that were loaded from the database 
 * 			 into the list
 * 		     returns 0 if the file is empty or does not exist
 */
int read_from_db(employee_t employee_list[], char db[]) {
	FILE *fp = fopen(db, "r");
	if ( fp == NULL ) {
		printf("Read error\n");
		return 0;
	}
	/* Checks if the file is empty first before proceeding */
	fseek(fp, 0, SEEK_END);
	if ( ftell(fp) == 0 ) {
		return 0; /* breaks if the file is empty*/
	} else {
		rewind(fp); /* sets the pointer to the beginning of the file */
	}				/* if the file is not empty */
	int i = 0;
	while ( !feof(fp) ) { /* feof checks if the end of file has been reached */
		/* read the contents in the same order it was written in save_to_db */
		fscanf(
			fp,
			"%s %f %d %d %d %d\n",
			employee_list[i].name,
			&employee_list[i].fte,
			&employee_list[i].level,
			&employee_list[i].birthday.day,
			&employee_list[i].birthday.month,
			&employee_list[i].birthday.year
		);
		++i;
	}
	fclose(fp);
	return i;
	
	/*
	int count = 0;
	char line[1000];
	char space[] = " ";

	if ( fp == NULL ) {
		printf("Read error\n");
		return 0;
	}

	while ( fgets(line, sizeof(line), fp) ) {
		char *ptr = strtok (line, space);
		char *array[20];
		int i = 0;
		while ( ptr != NULL ) {
			array[i++] = ptr;
			ptr = strtok(NULL, space);
		}

		employee_t emp;
		strcpy(emp.name,array[0]);
		emp.fte = atoi(array[1]);
		emp.level = atoi(array[2]);
		emp.birthday.day = atoi(array[3]);
		emp.birthday.month = atoi(array[4]);
		emp.birthday.year = atoi(array[5]);
		employee_list[count++] = emp;
	} 

	return count; */
}

/*******************************************************************************
 * Getter Functions
*******************************************************************************/

/* This functions returns a char pointer of the employees name 
 * that has been adjusted to a specific length (MAX_NAME_SIZE - 1)
 * @return - the name of the employee corrected to the proper length 
 */
char* get_name() {
	char* input = (char*)malloc(1000);
	char* output = (char*)malloc(sizeof(MAX_NAME_SIZE));
	/* Using 1000 solves long inputs issue by giving it a bigger buffer */
	fgets( input, 1000, stdin );
	/* strcspn removes the newline character that is added by fgets */
	input[strcspn(input,"\r\n")] = 0;
	
	/* Keep the first 11 inputs only */
	int i;
	for ( i = 0; i < MAX_NAME_SIZE-1; i++ ) {
		output[i] = input[i];
	}
	return output;
}

/* This functions returns a char pointer of the employees name 
 * 
 * At each date input, if the entered values are not valid
 * It will prompt the user once again to input a new value until it is valid
 * 
 * Day	   |  1 - 31 (Inclusive)
 * Month   |  1 - 12 (Inclusive)
 * Year    |  1800 - 2017 (Inclusive)
 *
 * @return - a date_t object with valid date inclusions
 */
date_t get_date(void) {
	date_t new_date;

	/* Input proper day until it is valid */
	printf("Enter birthday: day>");
	scanf("%d",&new_date.day);
	while ( !check_day(new_date.day) ) {
		printf("Invalid day. ");
		printf("Enter birthday: day>");
		scanf("%d",&new_date.day);
	}

	/* Input proper month until it is valid */
	printf("Enter birthday: month>");
	scanf("%d",&new_date.month);
	if ( !check_month(new_date.month) ) {
		printf("Invalid month. ");
		printf("Enter birthday: month>");
		scanf("%d",&new_date.month);
	}
	
	/* Input proper year until it is valid */
	printf("Enter birthday: year>");
	scanf("%d",&new_date.year);
	if ( !check_year(new_date.year) ) {
		printf("Invalid year. ");
		printf("Enter birthday: year>");
		scanf("%d",&new_date.year);
	}

	return new_date;
}

/* This functions returns a float value an employee's FTE rating
 * @return - the proper fte value after it has been validated
 */
float get_fte() {
	float fte_val;
	printf("Enter FTE>");
	scanf("%f",&fte_val);
	while (!check_fte(fte_val)) { /* Retry until it is valid */
		printf("Invalid FTE. ");
		printf("Enter FTE>");
		scanf("%f",&fte_val);
	}
	return fte_val;
}

/* This functions returns an integer an employee's level
 * @return - the proper level value after it has been validated
 */
int get_level() {
	int level;
	printf("Enter level>");
	scanf("%d",&level);
	while ( !check_level(level) ) { /* Retry until it is valid */
		printf("Invalid level. ");
		printf("Enter level>");
		scanf("%d",&level);
	}
	return level;
}

/*******************************************************************************
 * This function prints the initial menu with all instructions on how to use
 * this program.
 * inputs:
 * - none
 * outputs:
 * - none
*******************************************************************************/

void printMenu(void){
    printf("\n\n"
    	"1. add employee\n"
    	"2. delete last employee\n"
    	"3. display employee list\n"
    	"4. save the employee list to the database\n"
    	"5. read the employee list from the database\n"
    	"6. exit the program\n");
}

/* This functions uses the length of the input string to add
 * the minimum number of whitespaces to the right of the sprint
 * This function is used in display_employee (function 3)
 * It is used to provide the necessary spaces after the name text
 * 
 * @param str - the string that is being used to add spaces to
 * @param spaces - the minimum whitespace amount
 */
void print_space(const char str[], const int spaces) {
	int i;
	printf("%s",str);
	for ( i = strlen(str); i < spaces; i++ ) {
		printf(" ");
	}
}

/* This functions prints the birthday date portion in display_employee
 * Format of DD-MM-YYYY is printed
 * @param date - birthday date of the employee
 */
void print_birthday(const date_t date) {
	printf("%02d-%02d-%04d ",date.day, date.month, date.year);
}

/* This functions prints the fte field in display_employee
 * The format required is 3 decimal places
 * @param fte - the fte float value
 */
void print_fte(const float fte) {
	printf("%.3lf ",fte);
}

/* This functions prints the level field in display_employee
 * The format required is XX where a 0 is added to the left
 * if the number is in single digits
 * @param level - the level value
 */
void print_level(const int level) {
	printf("%02d", level);
}

/*******************************************************************************
 * Validation Functions
*******************************************************************************/

/* This function checks if a float is between a set range (inclusive)
 * @param - the float value being checked
 * @return - 1 if the number in range, 0 if it is outside the range
 */
int check_fte(const float fte) {
	return ( fte >= 0 && fte <= 1.0 ) ? 1 : 0;
}

/* This function checks if an integer is between a set range (inclusive)
 * @param check_val - the integer being checked
 * @param min - the minimum value of the range (inclusive)
 * @param max - the maximum value of the range (inclusive)
 * @return - 1 if the number in range, 0 if it is outside the range
 */
int check_range(const int check_val, const int min, const int max) {
	return ( check_val >= min && check_val <= max ) ? 1 : 0;
}

/* This function uses check_range to determine if the level is valid
 * The range should be between 7 - 18 (inclusive)
 * @param level - the level value being checked
 * @return - 1 if valid, 0 not valid
 */
int check_level(const int level) {
	return check_range( level, 7, 18 );
}

/* This function uses check_range to determine if the birthday day is valid
 * The range should be between 1 - 31 (inclusive)
 * @param day - the day value being checked
 * @return - 1 if valid, 0 not valid
 */
int check_day(const int day) {
	return check_range( day, 1, 31 );
}

/* This function uses check_range to determine if the birthday month is valid
 * The range should be between 1 - 12 (inclusive)
 * @param month - the month value being checked
 * @return - 1 if valid, 0 not valid
 */
int check_month(const int month) {
	return check_range( month, 1, 12 );
}

/* This function uses check_range to determine if the birthday year is valid
 * The range should be between 1800 - 2017 (inclusive)
 * @param year - the year value being checked
 * @return - 1 if valid, 0 not valid
 */
int check_year(const int year) {
	return check_range( year, 1800, 2017 );
}