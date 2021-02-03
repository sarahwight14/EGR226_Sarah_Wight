/*********************************************************************************************
* Author: Sarah Wight
* Course: EGR 226 - 902
* Date: 1/27/2021
* Project: Lab 2 - Resistor Analysis Tool Part 2
* File: ResistorAnalysisToolP2.c
* Description: This program gives the option to convert from a user inputted resistor value
* to its corresponding color band or to convert from the color bands to the corresponding
* resistance
*
**********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>


// Declaring Functions
void prompt(void);
void calcResistorColors(int resistance);
void getColorBands(char* input1, char* input2, char* input3, char* input4);
void calcResistance(char input1, char input2, char input3, char input4);

void main(void){

    char answer;

        // Asks user if they would like to run program
        printf("Press 'Y' if you would like to convert a resistor value.\n");
        printf("Press 'N' if you would like to exit the program.\n");
        fflush(stdin);
        scanf("%c", &answer);

        // Calls prompt if yes
        if (answer == 'Y' || 'y')
        {
            prompt();

        }
        else
        {
            printf("Thank you.");
        }

     return 0;
}


/****| prompt | *****************************************
* Brief: This function will display the resistor color codes
* and ask user whether they would like to convert to resistance
* or convert to color bands
* param:
* (void) data: no data inputted
* return:
* N/A
*************************************************************/

void prompt(void)
{

 int answer;
 int resistance;
 char* input1, input2, input3, input4;


 printf("--------------------Resistor Codes---------------------\n");
 printf("|Character| Color | Band 1 & 2 |  Band 3   |  Band 4 |\n");
 printf("|    K    | Black |      0     |*1         |+/- 1%%   |\n");
 printf("|    N    | Brown |      1     |*10        |+/- 2%%   |\n");
 printf("|    R    | Red   |      2     |*100       |         |\n");
 printf("|    O    | Orange|      3     |*1,000     |         |\n");
 printf("|    Y    | Yellow|      4     |*10,000    |         |\n");
 printf("|    G    | Green |      5     |*100,000   |+/- 0.5%% |\n");
 printf("|    B    | Blue  |      6     |*1,000,000 |+/- 0.25%%|\n");
 printf("|    V    | Violet|      7     |*10,000,000|+/- 0.1%% |\n");
 printf("|    E    | Grey  |      8     |           |+/- 0.05%%|\n");
 printf("|    W    | White |      9     |           |         |\n");
 printf("|    D    | Gold  |            |*0.1       |+/- 5%%   |\n");
 printf("|    S    | Silver|            |*0.01      |+/- 10%%  |\n");
 printf("-------------------------------------------------------\n\n");


 printf("Press 1 if you would like to convert resistance to color bands.\n");
 printf("Press 2 if you would like to convert color bands to resistance\n");

 // Flushing previous input and scanning in new input
 fflush(stdin);
 scanf("%d", &answer);

    // Calling function based on response
    if (answer == 1)
    {
        calcResistorColors(resistance);
    }
    else if (answer == 2)
    {
        getColorBands(input1, input2, input3, input4);
    }

    // Checking to make sure input is valid
    else
    {
        printf("Please enter 1 or 2. Try again. \n\n");
        prompt();
    }

}

/****| calcResistorColors | *****************************************
* Brief: This function will convert a user inputted value of resistance
* to the first three color bands on its corresponding resistor
* param:
* (int) data: 1-integer worth of data input
* return:
* N/A
*************************************************************/

void calcResistorColors(int resistance)
{

 int i=1;
 int firstBand;
 int secondBand;
 char answer;


 printf("Enter the resistance you would like to be converted:\n");
 int status = scanf("%d", &resistance);

    // Checking to make sure input is valid
    if (status == 1)
    {
        printf("\nYou entered: %d\n\n", resistance);
    }
    else
    {
        printf("Please try again, this time entering an integer.\n\n");
        main();
    }

    // Checking to make sure input is valid
    if (resistance < 0 || resistance > 99E6)
        {
        printf("This is an invalid number. ");
        printf("Resistance must be between 1-99M ohms. Please try again.\n\n");
        main();
        }

        // Determining multiplier with a counter
        do
        {
            resistance = resistance/10;
            ++i;
        }
        while (resistance/10 >= 100);

    printf("The color of the first three bands on your resistor are:\n");

    // Assigning first digit to firstBand
  	firstBand = resistance/pow(10,2);
  	firstBand = firstBand % 10;


    // Assigning color to input
  	switch(firstBand)
  	{
    case 0:
        printf("Black-");
        break;
    case 1:
        printf("Brown-");
        break;
    case 2:
        printf("Red-");
        break;
    case 3:
        printf("Orange-");
        break;
    case 4:
        printf("Yellow-");
        break;
    case 5:
        printf("Green-");
        break;
    case 6:
        printf("Blue-");
        break;
    case 7:
        printf("Violet-");
        break;
    case 8:
        printf("Grey-");
        break;
    case 9:
        printf("White-");
        break;
  	}

  	// Assigning second digit to secondband
  	secondBand = resistance/pow(10,1);
  	secondBand = secondBand % 10;

    // Assigning color to input
    switch(secondBand)
  	{
    case 0:
        printf("Black-");
        break;
    case 1:
        printf("Brown-");
        break;
    case 2:
        printf("Red-");
        break;
    case 3:
        printf("Orange-");
        break;
    case 4:
        printf("Yellow-");
        break;
    case 5:
        printf("Green-");
        break;
    case 6:
        printf("Blue-");
        break;
    case 7:
        printf("Violet-");
        break;
    case 8:
        printf("Grey-");
        break;
    case 9:
        printf("White-");
        break;
  	}


    // Assigning color for multiplier
    switch(i)
  	{
    case 0:
        printf("Black\n\n");
        break;
    case 1:
        printf("Brown\n\n");
        break;
    case 2:
        printf("Red\n\n");
        break;
    case 3:
        printf("Orange\n\n");
        break;
    case 4:
        printf("Yellow\n\n");
        break;
    case 5:
        printf("Green\n\n");
        break;
    case 6:
        printf("Blue\n\n");
        break;
    case 7:
        printf("Violet\n\n");
        break;
  	}
    main();
}


/****| getColorBands | *****************************************
* Brief: This function will take in four user inputs and then
* call a function to convert the inputs to a resistor value
* param:
* (char*) data: 4 character pointers worth of data input
* return:
* N/A
*************************************************************/
void getColorBands(char* input1, char* input2, char* input3, char* input4)
{
    // Prompting user to enter col
    printf("Enter the colors:\n");

    // Scanning users values
    scanf(" %c%c%c%c", &input1, &input2, &input3, &input4);
    //printf("\nRecieved: %c%c%c%c\n", input1, input2, input3, input4);

    // Sending values to calcResistance function
    calcResistance(input1, input2, input3, input4);
}

/****| calcResistance | *****************************************
* Brief: This function will determine the resistor value of 4
* user inputted colors
* param:
* (char) data: 4 characters worth of data input
* return:
* N/A
*************************************************************/

void calcResistance(char input1, char input2, char input3, char input4)
{
    float combine;

    //printf("%c%c%c%c\n", input1, input2, input3, input4);

    // Assigns first value
    switch(input1)
    {
    case 'K':
    case 'k':
        input1 = 0;
        break;
    case 'N':
    case 'n':
        input1 = 1;
        break;
    case 'R':
    case 'r':
        input1 = 2;
        break;
    case 'O':
    case 'o':
        input1 = 3;
        break;
    case 'Y':
    case 'y':
        input1 = 4;
        break;
    case 'G':
    case 'g':
        input1 = 5;
        break;
    case 'B':
    case 'b':
        input1 = 6;
        break;
    case 'V':
    case 'v':
        input1 = 7;
        break;
    case 'E':
    case 'e':
        input1 = 8;
        break;
    case 'W':
    case 'w':
        input1 = 9;
        break;
    default:
        errorChecker();
        break;
    }

    // Assigns second value
    switch(input2)
    {
    case 'K':
    case 'k':
        input2 = 0;
        break;
    case 'N':
    case 'n':
        input2 = 1;
        break;
    case 'R':
    case 'r':
        input2 = 2;
        break;
    case 'O':
    case 'o':
        input2 = 3;
        break;
    case 'Y':
    case 'y':
        input2 = 4;
        break;
    case 'G':
    case 'g':
        input2 = 5;
        break;
    case 'B':
    case 'b':
        input2 = 6;
        break;
    case 'V':
    case 'v':
        input2 = 7;
        break;
    case 'E':
    case 'e':
        input2 = 8;
        break;
    case 'W':
    case 'w':
        input2 = 9;
        break;
    default:
        errorChecker();
        break;
    }

    // Combines first two inputs to be multiplied by multiplier
    combine = (float)input1*10;
    combine += (float)input2;

    //printf("COMBO: %0.0f", combine);

    // Assigns multiplier
    switch(input3)
    {
    case 'K':
    case 'k':
        combine *= 1;
        break;
    case 'N':
    case 'n':
        combine *= 10;
        break;
    case 'R':
    case 'r':
        combine *= 100;
        break;
    case 'O':
    case 'o':
        combine *= 1000;
        break;
    case 'Y':
    case 'y':
        combine *= 10000;
        break;
    case 'G':
    case 'g':
        combine *= 100000;
        break;
    case 'B':
    case 'b':
        combine *= 1000000;
        break;
    case 'V':
    case 'v':
        combine *= 10000000;
        break;
    case 'D':
    case 'd':
        combine *= 0.1;
        break;
    case 'S':
    case 's':
        combine *= 0.01;
        break;
    default:
        errorChecker();
        break;
    }

    printf("\n\Resistance: %0.0f", combine);

    // Assigns tolerance
    switch(input4)
    {
    case 'K':
    case 'k':
        printf(" +/- 1%%\n\n");
        break;
    case 'N':
    case 'n':
        printf(" +/- 2%%\n\n");
        break;
    case 'G':
    case 'g':
        printf(" +/- 0.5%%\n\n");
        break;
    case 'B':
    case 'b':
        printf(" +/- 0.25%%\n\n");
        break;
    case 'V':
    case 'v':
        printf(" +/- 0.1%%\n\n");
        break;
    case 'E':
    case 'e':
        printf(" +/- 0.05%%\n\n");
        break;
    case 'D':
    case 'd':
        printf(" +/- 5%%\n\n");
        break;
    case 'S':
    case 's':
        printf(" +/- 10%%\n\n");
        break;
    default:
        errorChecker();
        break;
    }

    main();
}

/****| errorChecker | *****************************************
* Brief: This function displays an error message and sends
* the user back to the main function
* param:
* (void) data: none
* return:
* N/A
*************************************************************/

void errorChecker(void)
{
    // Sends user back to main if input is invalid
    printf("\nYour input is invalid. Please try again\n\n");
    main();
}
