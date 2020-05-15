#include <cstdlib>
#include <ctime>
#include <iostream>
#include<conio.h>
#include<windows.h>
#include <fstream>
using namespace std;

char grid[81];///stores the grid
bool gameEnd = false;

void printGrid(int num)
{
    system("CLS");
    cout << endl;
    cout << "+---+---+---+---+---+--+--+---+---+-";
    cout << endl;
    for(int i = 0; i<81; i = i + 9)
    {
        for(int j = i; j< i + 9; j++)
        {
        if(j == num)
        {
            CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
            HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            HANDLE inputHandle = GetStdHandle(STD_INPUT_HANDLE);

            GetConsoleScreenBufferInfo( outputHandle, &bufferInfo );

            SetConsoleTextAttribute( outputHandle ,0x5B  );
            cout<<" "<<grid[j]<<" ";
            FlushConsoleInputBuffer(inputHandle);
            SetConsoleTextAttribute( outputHandle, bufferInfo.wAttributes );

        }
        else cout<<" "<<grid[j]<<" ";
         cout << "|";
        }
        cout<<endl;
    }
    cout << "+---+---+---+---+---+--+--+---+---+-";
    cout << endl;
    cout<<"\n>Press space to enter/exit review mode"<<endl;
    cout<<">Use keyboard's left and right keys to change cursor(i.e '.') position"<<endl;
    cout<<">Hit 1 to 9 keys from keyboard to enter a value in the desired place"<<endl;
}

void writeFile(int *x, int emptySpaces)
{
   system("CLS");
   ofstream fout;
   char endFile[256];
   cout << "What file would you like to write your board to: ";
   cin  >> endFile;

///Opening file and checking for Error.
   fout.open(endFile);
   if (fout.fail())
   {
      cout << "Error. Can't Open"<<endl;
      return;
   }
   else
      cout << "Hurrah!!Board has been written successfully"<< endl;

    fout<<emptySpaces; ///writes the number of empty spaces

///Writing the Sudoku Board
   for (int i = 0; i < 81; i++) fout << grid[i];


    for(int i = 0; i < emptySpaces; i++) fout<<x[i]<<'*'; ///writes the list of changeable values

  fout.close();
}

bool isInRowCol(int coordinate, char value)
{
    /***returns true if there is an element in a row or column of the coordinate
    ****"value" that has the same value as the parameter***/

    int row = coordinate, col = coordinate;
    do
    {
        row = row - 9;
    }while(row >= 0);
    row = row + 9;

    do
    {
        if(col == 0) break;
        else if(col % 9 == 0)break;
        else col--;
    }while(1);
    for(int i = row; i<81; i = i + 9)
    {
        if(i!= coordinate)
        {
            if(grid[i] == value) return true;
        }
    }
    for(int i = 0; i<9; i++)
    {
        if(i + col!= coordinate)
        {
            if(grid [i + col] == value)return true;
        }
    }
    return false;
}

void threeBythree(int *output)///takes in an array and fills it with numbers specifying the start of 3x3 grids
{
    int counter = 0;
    for(int i = 0; i < 3; i++)///total three rows
    {
        for(int j = 0 + (27 * i);  j <= 8 +(27 * i); j++)///column index used to iterate through rows
        {
            if(j%3 == 0 || j == 0)
            {
                output[counter]= j;
                counter++;
            }
        }
    }
}
int belongsToBlock(int *numbers, int place) ///returns the 3x3 index number from where the 3x3 block starts
{
    for(int i = 0; i<9; i++)///nine is the size of the array numbers which contains the index of start of 3x3 grid
    {
        for(int j = numbers[i]; j < numbers[i] + 27; j = j + 9) ///iterating through the 3 rows
        {
            for(int index = j; index < j + 3; index++) ///iterating through the 3 columns
            {
                if(place == index)return numbers[i]; ///if the place specified belongs to a specific block, return the block's starting index
            }
        }
    }
}
bool isInBlock(char value, int blockIndex, int coordinate)
/***checks if there is another element in the 3x3 block
****with the same value as the value specified***/
{
    for(int i = blockIndex; i< blockIndex + 27; i = i + 9)///iterating through the 3 rows
    {
        for(int j = i; j < i + 3; j++)///iterating through the 3 columns
        {
            if(j!= coordinate) ///checks whether(the index from the block) is the same as our coordinate
            {
              if(grid[j] == value) return true; ///returns true meaning there is a value in the block with the same value as the value we specified
            }
        }
    }
    return false; ///returns false if the function does not return true
}

void initialiseArray(int *arr, int Size)/// initializes array arr of Size wit value 10
{
    for(int i = 0; i<Size; i++) arr[i] = 10;

}

bool winner()
{
    int numbers[9];
    int blockIndex;
    threeBythree(numbers);
    for(int i = 0; i<81; i++)
    {
        if(grid[i] == ' ')return false;

        if(isInRowCol(i,grid[i]))return false;

        blockIndex = belongsToBlock(numbers, i);
        if(isInBlock(grid[i], blockIndex, i)) return false;

    }
    return true;
}

void printMenu()
{
    system("CLS");
    cout<<"\t\t\t\tPAUSE MENU"<<endl<<endl<<endl;
    cout<<"1.Press space to resume game."<<endl;
    cout<<"2.Press 1 to save game and continue playing."<<endl;
    cout<<"3.Press 2 to save and exit game."<<endl;
    cout<<"4.Press 3 to exit game without saving"<<endl;
}

void input(int *x, int counter)
{
    bool stop = false;///it is true if game is in pause mode and false otherwise
    char cursor, value; ///the value of the cursor and the value of the user input char to be stored on gird
    char pauseOption; ///stores the user selected option in the pause menu
    int turns = 0; ///turns stores the position of cursor and counter stores the count of elements in the array x

    printGrid(x[0]);/// cursor position set to first empty element if its the first run

    do
    {
        do
        {
            if(!stop) ///run these statements only if the game is not in pause/review mode
            {
                ///this nested while loop is used to get the cursor input, space(for pausing) input and also for
                cursor =  getch();
                if(cursor == 224 || cursor == 0 || cursor == -32)///checks if any one of the arrow keys is pressed
                {
                    cursor = getch();

                    if(cursor == 75) ///if 75 returned by getch(), it means left arrow key pressed
                    {
                        if(turns == 0) turns = counter - 1;///if start of grid reached, bring the cursor to the last element
                        else turns --; ///move the cursor back, to the previous changeable place
                        printGrid(x[turns]);

                    }
                    if(cursor == 77)///if 77 returned by getch(), it means right arrow key pressed
                    {
                        if(turns == counter - 1) turns = 0;///if end of grid reached, bring the cursor to the first element
                        else turns++; ///move the cursor back, to the next changeable place
                        printGrid(x[turns]);
                    }
                }
                else if (cursor >= '0' && cursor <= '9')///if user enters a value
                {
                    value = cursor;
                    break;
                }
                else if(cursor == 32) stop = true;///space button pressed, so game needs to be paused
            }
            if(stop)
            {
                printMenu();
                pauseOption = getch();
                switch(pauseOption)
                {
                    case 32:///space pressed means resume game
                        stop = false;
                        printGrid(x[0]);
                        break;
                    case '1':
                        writeFile(x, counter);
                        stop = false;
                        printGrid(x[0]);
                        break;
                    case '2':
                        writeFile(x, counter);
                        return;
                    case '3':
                        system("CLS");
                        cout<<"Are you sure you want to exit without saving?"<<endl;
                        cout<<"If yes, then press y.\nPress any other key to exit."<<endl;
                        pauseOption = getch();
                        if(pauseOption == 'y' || pauseOption == 'Y')return;
                    default:
                        cout<<"\nOption does not exist"<<endl;
                        Sleep(2000);
                        break;
                }
            }
        }while(1);

        if(value == '0')grid[x[turns]] = ' ';
        else grid[x[turns]] = value;
        printGrid(x[turns]);
        if(winner())
        {
            system("CLS");
            system("Color 0E");
            cout<<"\n\n\n\n\n\n\n\t\t\tCONGRATS! YOU WON!"<<endl;
            break;
        }
    }while(1);
}

bool readFile()
{
   system("CLS");
   int emptySpaces = -1;///total changeable sudokou board

    char fileChar; /// stores input from the file while filling the array x (i.e changeable values)
    int numberOfDigits = 0;///used in filling x
    string xNumbers = "99"; ///checks if the element thats going to be stored in x is single digit or double digit
    int xCounter = 0;///counter to store count of elements in x

   char legendFile[256];///legendFile stores the address specified by user for save file
   ifstream fin;   ///file to be used for input
    string fileRead; ///string to store the contents of the file

   cout << "Where is your Sudoku Board located?\nEnter an address like: C:\\Users\\board.txt"<<endl;
   cin  >> legendFile;

   ///Opening file and checking for Error.
   fin.open(legendFile);
   if (fin.fail())return false;

    getline(fin, fileRead); ///takes in input and stores it in the string named fileRead

    emptySpaces = ( 10 *(fileRead[0] - '0')) + (fileRead[1] - '0'); ///calculates the empty spaces in a file
    if(!(emptySpaces == 20 || emptySpaces == 30 || emptySpaces == 40)) return false; /// if spaces arent valid, return false

    int x[emptySpaces]; ///stores changeable indexes


   ///Reading the file
   for (int i = 2; i < 83; i++) grid[i - 2] = fileRead[i]; ///fills the grid with appropriate values


    for(int i = 83; i < fileRead.length(); i++) ///filling x
    {
       fileChar = fileRead[i];
       if(fileChar == '*')
       {
           if(numberOfDigits == 1) x[xCounter] = xNumbers[0] - '0'; ///takes only one digit
           else if(numberOfDigits == 2)x[xCounter] = (10 * (xNumbers[0] - '0')) + (xNumbers[1] - '0'); ///ensures both digit are included

           numberOfDigits = 0; ///for the next element
           xCounter++;///increments xCounter only if * is found
        }
       else
       {
           xNumbers[numberOfDigits] = fileChar; ///stores input from file in xNumbers
           numberOfDigits++;///increments numberOfDigits
       }
    }
    input(x, emptySpaces);
    fin.close();
    return true;
   }



bool isAlreadyThere(int *arr, int num, int sizeArr)
{
    /***checks if the specified integer num is already
    ****in the integer one-dimensional
    ****array arr with size sizeArr, both provided as parameters***/

    for(int i = 0; i< sizeArr; i++)///loops through the array from 0 to sizeArr
    {
        if(num == arr[i]) return true;///if there is a number equal to num, it returns true which means that num is already in the array
    }
    return false;///returns false after iterating through the whole array only if the function has not returned true
}

bool arrayIsZero(char possible[81][9], int row)
{
    /**returns true if all the elements in the specified
    ***row are '0'***/
    for(int i = 0; i< 9; i++) ///looping through the row number specified. This loop is the column index
    {
        if(possible[row][i] != '0') return false; ///returns false if it finds an element not equal to zero in the specified row
    }
    return true; ///returns true only if the function does not return false after checking through every element of the row
}

void initialisePossible(char possible[81][9], int row)
{
    /***initializes a row of the possible set of values meaning it
    ****initializes the set of possible values left for a single
    ****element on the grid. i.e the row number provided as a parameter***/

    for(int i = 1; i<10; i++) possible[row][i - 1] = '0' + i;///iterates through the row using column indexes and adds values like '1','2'...'9' etc
}

void gridGenerator()
{

    for(int i = 0; i < 81; i++)grid[i]= ' ';
    char difficultyLevel;
    int emptySpaces;///possible spaces
    int numbers[9];///all possible left corner positions of each 3x3 block in the 9x9 grid
    int content;
    threeBythree(numbers);
    char loadOption;
    char possibleValues[81][9];
    for(int i = 0; i< 81; i++) initialisePossible(possibleValues, i);

    int counter = 0;
    srand((unsigned)time(0));
    do
    {
        if(arrayIsZero(possibleValues, counter))
        {
            initialisePossible(possibleValues, counter);
            counter--;
            possibleValues[counter][(int)(grid[counter] - '1')] = '0';
            grid[counter] = ' ';
        }
        content = rand()%9;
        if(possibleValues[counter][content] != '0')
        {
            int blockIndex = belongsToBlock(numbers, counter);
            if(!isInBlock(possibleValues[counter][content], blockIndex, - 1))
            {
                if(!isInRowCol(counter,possibleValues[counter][content]))
                {
                    grid[counter] = possibleValues[counter][content];
                    counter++;
                }
                else possibleValues[counter][content] = '0';
            }
            else possibleValues[counter][content] = '0';
        }

    }while(counter < 81);

    system("CLS");
    cout<<"Yo! You wanna load some previous game?"<<endl;
    cout<<"Press y if yes, any other key if no"<<endl;
    loadOption = getch();
    if(loadOption == 'y' || loadOption == 'Y')
    {
        if(!readFile())
        {
            cout<<"ERROR: File either does not exist or is not valid!"<<endl;
            Sleep(1000);
        }
        else return;
    }
    do
    {
        system("CLS");
        cout<<"Choose a difficulty level level from the following and press its corresponding key"<<endl;
        cout<<"1. Easy\n2. Normal\n3. Hard"<<endl;
        difficultyLevel = getch();
        if(difficultyLevel <'1' || difficultyLevel > '3')
        {
            cout<<"Inappropriate difficulty level entered. Try again"<<endl;
            Sleep(1000);
        }
    }while(difficultyLevel < '1' || difficultyLevel> '3');
    if(difficultyLevel == '1')emptySpaces = 20;
    else if(difficultyLevel == '2') emptySpaces = 30;
    else if(difficultyLevel == '3') emptySpaces = 40;

    int x[emptySpaces]; /// stores possible values which can be changed

    int random, randomArr[emptySpaces], sizeRandom = 0;
    initialiseArray(randomArr, emptySpaces);
    srand((unsigned)time(0));
    do
    {
        random = rand() % 81;
        if(!isAlreadyThere(randomArr,random, sizeRandom))
        {
            randomArr[sizeRandom] = random;
            grid[random] = ' ';
            sizeRandom++;
        }
    }while(sizeRandom < emptySpaces);
    counter = 0; ///same counter being used to count elements in x
    for(int i = 0; i < 81; i++)
    {
        if(counter == emptySpaces) break;
        if(grid[i] == ' ')
        {
            x[counter] =  i;
            counter++;
        }
    }
    input(x, emptySpaces );
}

int main()
{
    int num;
    bool gameStart;
    char sel;

    cout <<"\n\n\n\n\n\n\n\n\n\t\t\t\tWELCOME TO SUDOKU!!!"<<endl;
    system("Color 64");
    Sleep(200);
    system("Color 93");
    Sleep(200);
    system("Color A1");
    Sleep(200);
    system("Color 25");
    Sleep(200);
    system("Color 04");
    Sleep(200);

    system("CLS");
START:
        system("Color 07");

	do {
        gameStart = false;
		cout<<"\t\t\t\tHOME SCREEN"<<endl<<endl<<endl<<endl<<endl;
		cout <<"1. Start Game?\n\n"<<endl;
		cout <<"2. Instructions?\n\n"<<endl;
        cout <<"3. Exit\n\n"<<endl;

		cout <<"Select an option, please..."<<endl;

		sel=getch();
		switch(sel){
		  case '1':
            gameStart = true;
			break;
		  case '2':
			cout << endl;
            cout << "Each puzzle consists of a 9x9 grid containing given clues in various places. The objective is to fill all empty squares so that the numbers 1 to 9 appear exactly once in each row, column and 3x3 box. The numbers can appear in any order and diagonals are not considered." << endl;
            cout << endl << endl;
            cout<<"Press any key to go back to option selection..."<<endl;
            sel = getch();
            system("CLS");
			break;
		  case '3':
            system("CLS");
            system("Color 0E");
            system("CLS");
            cout<<"\nThank You for playing this game! Bye-bye!"<<endl;
			return 0;
		  default:
			cout << "Wrong input! Please try again again" << endl;
            cout<<"Press any key to go back to option selection..."<<endl;
            sel = getch();
            system("CLS");
			break ;
		}
	} while(!gameStart);

    gridGenerator();
    Sleep(1000);
    cout<<"Press 'r' key to play a new game\nPress any other key to quit :("<<endl;
    char in = getch();
    if(in == 'r' || in== 'R')
    {
        system("CLS");
        goto START;
    }
    system("CLS");
    cout<<"\nThank You for playing this game! Bye-bye!"<<endl;
    return 0;
}
