/*********************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 1/27/2021
* Project: Lab 2 - Book Database
* File: main.c
* Description: This program allows a user to search a library for a book using its title,
* author, or ISBN and displays all book information that matches the input
*
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 500

typedef struct {
    char title[255];
    char author_name[50];
    char ISBN[10];
    int pages;
    int year_published;
} book;


// Declaring functions for later user
int parse_file(char filename[], book book_array[]);
void print_book(book print_book);
void search_title(book book_title[], int books, char title[]);
void search_author(book book_author[], int books, char author[]);
void search_ISBN(book book_ISBN[], int books, char ISBN[]);


int main()
{
    book my_book_array[360];
    char filename[MAX];
    int numBooks;
    int userInput;
    int status;
    char userInput2[255];
    char again;

    // Setting filename
    strcpy(filename, "BookList.csv");

    // Calling parse file and setting return value to numBooks
    numBooks = parse_file(filename, my_book_array);

    do
    {
        printf("Enter how you would like to search: \n");
        printf("[0] Search by Title\n");
        printf("[1] Search by Author\n");
        printf("[2] Search by ISBN\n");

        do
        {
            status = scanf(" %d", &userInput);  // scanning user input

            // Checking input for 0, 1, 2
            if(status == 0 || userInput < 0 || userInput > 2)
            {
                printf("\nPlease enter either 0, 1, or 2\n");
                fflush(stdin);  // flushes input if invalid
            }

        } while(status == 0 || userInput < 0 || userInput > 2);  // loops until valid input is entered


        if(userInput == 0)
        {
            printf("Please enter a title:\n");
            scanf(" %[^\n]s", userInput2);  // scanning user input until newline character
            search_title(my_book_array, numBooks, userInput2); // calling function to search by title
        }


        else if(userInput == 1)
        {
            printf("Please enter an author:\n");
            scanf(" %[^\n]s", userInput2);  // scanning user input until newline character
            search_author(my_book_array, numBooks, userInput2); // calling function to search by author
        }


        else if(userInput == 2)
        {
            printf("Please enter an ISBN:\n");
            scanf(" %[^\n]s", userInput2);  // scanning user input until newline character
            search_ISBN(my_book_array, numBooks, userInput2);  // calling function to search by ISBN
        }

        printf("Would you like to search again?\n");
        printf("If yes, enter 'Y'\n");
        printf("If not, enter 'N'\n");

        do
        {
            status = scanf(" %c", &again);

            // Checking for valid input
            if(status == 0 || (again != 'y' && again != 'n'))
            {
                printf("\nPlease enter a 'Y' or an 'N'\n");
                fflush(stdin); // flush invalid input
            }

        } while(status == 0 || (again != 'y' && again != 'n')); // loops unless user enters y or n

    } while(again == 'y'); // if user selects y program will run again


    return 0;
}


/****| parse_file | *****************************************
* Brief: This function will read in the file, break up the contents by
* title, author, and ISBN, and store those components in the structure
* param:
* (char) data: 2-character worth of data input
* return:
* total number of books
*************************************************************/

int parse_file(char filename[], book book_array[])
{
    char temp[512];
    char* pointer;
    int i;
    FILE* fp;

    fp = fopen(filename, "r"); // opening file

    // Checks if file exists
    if(fp == NULL)
    {
        return -1;
    }

    // Reads in all books
    while(fgets(temp, 512, fp))
    {
        pointer = strtok(temp, ","); // Replaces first comma in file with a null character
        strcpy(book_array[i].title, pointer); // Copies first string up to the null character

        pointer = strtok(NULL, ",\n"); // Replaces second comma or newline in file with a null character
        strcpy(book_array[i].author_name, pointer); // Copies second string into author_name

        pointer = strtok(NULL, ",\n"); // Replaces third comma or newline in file with a null character
        strcpy(book_array[i].ISBN, pointer); // Copies third string into ISBN

        pointer = strtok(NULL, ",\n"); // Replaces fourth comma or newline in file with a null character

            // Checking to see if pointer is N/A
            if(strcmp(pointer, "N/A"))
            {
                book_array[i].pages = atoi(pointer); // Changing pointer to int and storing in pages of book struct
            }


            else if(strcmp(pointer, "N/A") == 0)
            {
                book_array[i].pages = 0; //sets the pages of the book structure as zero
            }

        pointer = strtok(NULL, ",\n"); //replaces the fifth comma or newline character with a null character and stores it into pointer

            // Checking to see if  pointer is N/A
            if(strcmp(pointer, "N/A"))
            {
                book_array[i].year_published = atoi(pointer); // changing pointer to int and storing in year_published of book struct
            }


            else if(strcmp(pointer, "N/A"))
            {
                book_array[i].year_published = 0; // sets year_published of the book struct to 0
            }

        i++; // counting each book read in
    }

    fclose(fp); // closing file

    return i;
}


/****| print_book | *****************************************
* Brief: This function will print the approbriate book information out
* param:
* (book) data: 1 book structure worth of input
* return:
* N/A
*************************************************************/

void print_book(book print_book)
{
    printf("[Title]             %s\n", print_book.title);
    printf("[Author]            %s\n", print_book.author_name);
    printf("[ISBN]              %s\n", print_book.ISBN);


    // Prints number of pages or N/A
    if(print_book.pages == 0)
    {
        printf("[Pages]             N/A\n");
    }

    else if(print_book.pages !=0)
    {
        printf("[Pages]             %d\n", print_book.pages);
    }

    // Prints year or N/A
    if(print_book.year_published == 0)
    {
        printf("[Year Published]    N/A\n\n");
    }

    else if(print_book.year_published != 0)
    {
        printf("[Year Published]    %d\n\n", print_book.year_published);
    }
}


/****| search_title | *****************************************
* Brief: This function will cycle through all of the books in the file
* and select any book that matches the user input sent to be printed
* param:
* (book, int, char) data: 1 book structure worth of data input
*                         1 integer worth of data input
*                         1 character string worth of data input
* return:
* N/A
*************************************************************/

void search_title(book book_title[], int books, char title[])
{
    // Cycles through all books in file
    for(int i = 0; i <= books; i++)
    {
        // Checking for user input in title
        if(strstr(book_title[i].title, title))
        {
            print_book(book_title[i]); // calling function to print book info
        }
    }
}


/****| search_title | *****************************************
* Brief: This function will cycle through all of the books in the file
* and select any book that matches the user input sent to be printed
* param:
* (book, int, char) data: 1 book structure worth of data input
*                         1 integer worth of data input
*                         1 character string worth of data input
* return:
* N/A
*************************************************************/

void search_author(book book_author[], int books, char author[])
{
    // Cycles through all books in file
    for(int i = 0; i <= books; i++)
    {
        // Checking for user input in author names
        if(strstr(book_author[i].author_name, author))
        {
            print_book(book_author[i]); // calls function to print book info
        }
    }
}


/****| search_title | *****************************************
* Brief: This function will cycle through all of the books in the file
* and select any book that matches the user input sent to be printed
* param:
* (book, int, char) data: 1 book structure worth of data input
*                         1 integer worth of data input
*                         1 character string worth of data input
* return:
* N/A
*************************************************************/

void search_ISBN(book book_ISBN[], int books, char ISBN[])
{
    // Cycles through all books in the file
    for(int i = 0; i <= books; i++)
    {
        // Checking for user input in ISBN
        if(strstr(book_ISBN[i].ISBN, ISBN))
        {
            print_book(book_ISBN[i]); // calls function to print book info
        }
    }
}
