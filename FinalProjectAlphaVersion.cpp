//Final Project : 2048
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "__Typography.h"

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define _sTime 32

//Global Structures
struct Score{
	char Name[100];
	long int Score;
	struct Score* link;
};

//Global Variables
int score = 0,copyscore = 0,board [4][4],copyboard [4][4],x,y,columns, rows;
CONSOLE_SCREEN_BUFFER_INFO csbi;
bool Playing = true,Initilized=false;
time_t GameStarted, GameEnded;
char FakeMessages[7][60],PlayerName[100];

//Functions 
void Menu();
void Initializing();
int RandSpot();
void Display();
void Play();
int Full();
void Restart();
void Undo(int,int);//Recursive
void Up();
void Down();
void Left();
void Right();
int Win();
int Lose();
void End();
int Compare();
int Score(int,int,int);//Recursive
void RandNum();
void gotoxy( int column, int line );
void TypeMachine(char Str[], int msSleep);
void _BoardGame();
void _UpdateStatus();
void _ShowTimePassed(time_t Base);
void _ProgressBar();
void _ShowSortedScores();
int charIndex(char Str[], char cQuery,int hIndex);
void Ceremony();
void JustifyText(char fileName[], int LineCharCount, int Spaces);
void Text(char fileName[]);
void extractNameFromLine(char Line[], char* toBeSaved);
int extractScoreFromLine(char Line[]);
void LinkedListPush(struct Score** head_ref, char Name[], long int newScore);
void printScoresDatabase(struct Score *start);
void LinkedListInsertionSort(struct Score **head_ref);
void LinkedListInsertSorted(struct Score** head_ref, struct Score* new_node);

#define WINDOWS 1
void console_clear_screen() 
{
  #ifdef WINDOWS
  system("cls");
  #endif
  #ifdef LINUX
  system("clear");
  #endif
}

void SetColor(int ForgC)
{
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;            
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        wColor =(csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
     return;
}

int main()
{
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	rows = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	columns = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;	
	srand(time(NULL));
	strcpy(FakeMessages[0],"[+] Loading Libraries" );
	strcpy(FakeMessages[1],"[+] Initializing Global and Local Variables");
	strcpy(FakeMessages[2],"[+] Loading DLLs Needed");
	strcpy(FakeMessages[3],"[+] Overridding Game Stack");
	strcpy(FakeMessages[4],"[+] Preparing Fireworks");
	strcpy(FakeMessages[5],"[+] Loading Scores Log");
	strcpy(FakeMessages[6],"[-] Deleting Temproray Files");
	Menu();
	return 0;
}

void gotoxy( int column, int line )
{
  COORD coord;
  coord.X = column;
  coord.Y = line;
  SetConsoleCursorPosition(GetStdHandle( STD_OUTPUT_HANDLE ),coord);
}

void Menu()
{
	console_clear_screen();
    y = 0;
    x = y + 3;
	printstr(30,7,"2048",219,_GREEN);
	x += 5;
	gotoxy(columns,12);
	SetColor(7);
	TypeMachine("Copyright by Shabnam Zare", _sTime/2);
	gotoxy(columns,18);
	TypeMachine("Player\'s Full Name: ",_sTime/2+5);
	gets(PlayerName);
	gotoxy(columns,24);
	SetColor(8);
	printf("Press any key to continue...");
	getch(); 
	SetColor(15);
	Initializing();
}

void Initializing()
{
    for(int i=0;i<6;i++)
	{
    	console_clear_screen();
		printstr(30,7 - i,"2048",219,_GREEN);
		Sleep(_sTime/4+15);
	}

	_ProgressBar();

	int i , j , n , m , k;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			board[i][j] = 0;
			copyboard [i][j] = 0;
		}
	}
	k = rand() % 10;
	i = rand() % 4;
	j = rand() % 4;
	if (k < 5)
	{
		board[i][j] = 2;
	}
	else
	{
		board[i][j] = 4;
	}
	k = rand() % 10;
	m = rand() % 4;
	n = rand() % 4;
	if (k < 5)
	{
		board[m][n] = 2;
	}
	else
	{
		board[m][n] = 4;
	}
	GameStarted = time(0);
	Display();
}

int RandSpot()
{
	return rand()%4;
}

void Display()
{
	Playing = true;
	_UpdateStatus();
	console_clear_screen();
    int i , j , k;
    y = 5;
    x = 5;
    
    if (Lose() == 0 && Win() == 0)
    {
    	printstr(30,2,"2048",219,_GREEN);
		_BoardGame();
		x += 5;
		Play();
	}
}

void Play()
{
    gotoxy(columns,21);
    printf("Score: %d",score);
    score = Score(0,0,0);
    gotoxy(columns,22);
    printf("Press \'?\' to get some help");
    gotoxy(columns,23);
    printf("Press \'j\' for Privacy Policies");
    gotoxy(0,x);
	char choice , ch;
	int i , j , compare = 0 , arrow;
	
	while(true)
	{
		
		if( kbhit() )
		{
			choice = getch();
			if (choice != 'u' && choice != 'U')
			{
				for (i = 0 ; i < 4 ; i++)
				{
					for (j = 0 ; j < 4 ; j++)
					{
						copyboard[i][j] = board[i][j];
						copyscore = score;
					}
				}
			}
			
			arrow = choice;
			if (arrow == -32)
			{
				arrow = getch();
				switch (arrow)
				{
					case 72:
						choice = UP;
						break;
					case 80:
						choice = DOWN;
						break;
					case 75:
						choice = LEFT;
						break;
					case 77:
						choice = RIGHT;
						break;
				}
			}
			
			switch (choice)
			{
				case UP:
				case 'w':
				case 'W':
					Up();
					if (Compare() > 1) 
					RandNum ();	
					Display();	
					break;
					
				case DOWN:		
				case 's':
				case 'S':
					Down();
					if (Compare() > 1) 
					RandNum();	
					Display();	
					break;
				case LEFT:		
				case 'a':
				case 'A':
					Left();
					if (Compare() > 1) 
					RandNum ();	
					Display();	
					break;
					
				case RIGHT:	
				case 'd':
				case 'D':
					Right();
					if (Compare() > 1) 
					RandNum ();	
					Display();	
					break;
				case 'q':
				case 'Q':
					printf("\n\n\n\n\nAre you sure? (y/n)");
					ch = getch();
					if (ch == 'y' || ch == 'Y') 
					{
						End();
						break;
					}
					else 
					{
						Play();
						break;
					}
				case 'r':
				case 'R':
					Restart();
					break;
				case 'u':
				case 'U':
					score = Score(0,0,0);
					Undo(0,0);
					break;
				case 'v':
				case 'V':
					_ShowSortedScores();
					break;
				case '?':
					Playing = false;
					_UpdateStatus();
					gotoxy(columns, 22);
					SetColor(7);
					printf("[?] Use {W,A,S,D} to control number's moves");
					gotoxy(columns, 23);
					printf("[?] {R} to restart the game   ");
					gotoxy(columns, 24);
    				printf("[?] {V} Score DataBase     ");
    				gotoxy(columns, 25);
					printf("[?] {U} Undo the last move    ");
					gotoxy(columns, 26);
					printf("[?] {Q} Exit");
					gotoxy(columns, 28);
					SetColor(8);
					printf("Contact Author: [shabnamzare@gmail.com] || [TahaShieenavaz@gmail.com]" );
					SetColor(15);
					choice = getch();
					switch (choice)
					{
						case 'q':
						case 'Q':
							printf("\nAre you sure? (y/n)");
							ch = getch();
							if (ch == 'y' || ch == 'Y') 
							{
								End();
								break;
							}
							else 
							{
								Play();
								break;
							}	
						case 'r':
						case 'R':
							Restart();
							break;
						case 'u':
						case 'U':
							score = Score(0,0,0);
							Undo(0,0);
							break;
						case 'v':
						case 'V':
							_ShowSortedScores();
							break;
					}
					SetColor(8);
					if (choice != 'q' && choice != 'Q')
					{
						printf("\nPress any key to continue...");
						getch();
						Display();
						break;
					}
					else
					{
						break;
					}
				case 'j':
				case 'J':
					Ceremony();
					break;	
			}
			if (choice != 'q' && choice != 'Q')
			{
				printf("\nPress any key to continue...");
				getch();
				Display();
				break;
			}
			else
			{
				break;
			}
		}
		else
		{
			if(Playing)
			{
				gotoxy(columns, 20);
				printf("Time Estimated: ");
				_ShowTimePassed(GameStarted);
				for(int i=0;i<100;i++)
				{
					if(kbhit()) 
					continue;
					Sleep(2);
				}
			}

		}
	}
}

int Full()
{
	int counter = 0 , i , j;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			if (board[i][j] != 0) counter++;
		}
	}
	return counter;
}

void Restart()
{
	char c;
	printf("\nAre you sure? (y/n)");
	c = getch();
	if (c == 'y' || c == 'Y') 
		Initializing();
	else 
		Display ();
}

void Undo(int p , int q)
{
	if (p < 4)
	{
		if (q < 4)
		{
			board[p][q] = copyboard[p][q];
			Undo(p,q+1);
		}
		else Undo(p+1,0);
	}
	else
	{
		score = Score(0,0,0);
		Display ();
	}
}

void Up()
{
	int i , j , k;
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 0 ; i < 4 ; i++)
		{
			if (board[i][j])//board[i][j] != 0
			{
				for (k = i ; k > 0 ; k--)
				{
					if (board[k-1][j] == 0)
					{
						board[k-1][j] = board[k][j];
						board[k][j] = 0;
					}
				}
			}
		}
	}
	
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 0 ; i < 3 ; i++)
		{
			if (board[i][j] && board[i][j] == board[i+1][j])
			{
				board[i][j] *= 2;
				board[i+1][j] = 0;
			}
		}
	}
	
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 0 ; i < 4 ; i++){
			if (board[i][j]){
				for (k = i ; k > 0 ; k--){
					if (board[k-1][j] == 0){
						board[k-1][j] = board[k][j];
						board[k][j] = 0;
					}
				}
			}
		}
	}
}

void Down()
{
	int i , j , k;
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 3 ; i >= 0 ; i--)
		{
			if (board[i][j])//board[i][j] != 0
			{
				for (k = i ; k < 3 ; k++)
				{
					if (!board[k+1][j])//board[k+1][j] == 0
					{
						board[k+1][j] = board[k][j];
						board[k][j] = 0;
					}
				}
			}
		}
	}
	
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 3 ; i > 0 ; i--)
		{
			if (board[i][j] && board[i][j] == board[i-1][j])
			{
				board[i][j] *= 2;
				board[i-1][j] = 0;
			}
		}
	}
	
	for (j = 0 ; j < 4 ; j++)
	{
		for (i = 3 ; i >= 0 ; i--)
		{
			if (board[i][j])//board[i][j] != 0
			{
				for (k = i ; k < 3 ; k++)
				{
					if (!board[k+1][j])//board[k+1][j] == 0
					{
						board[k+1][j] = board[k][j];
						board[k][j] = 0;
					}
				}
			}
		}
	}
}

void Left()
{
	int i , j , k;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			if (board[i][j])//board[i][j] != 0
			{
				for (k = j ; k > 0 ; k--)
				{
					if (!board[i][k-1])//board[i][k-1] == 0
					{
						board[i][k-1] = board[i][k];
						board[i][k] = 0;
					}
				}
			}
		}
	}
	
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 3 ; j++)
		{
			if (board[i][j] && board[i][j] == board[i][j+1])
			{
				board[i][j] *= 2;
				board[i][j+1] = 0;
			}
		}
	}
	
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			if (board[i][j])//board[i][j] != 0
			{
				for (k = j ; k > 0 ; k--)
				{
					if (!board[i][k-1])//board[i][k-1] == 0
					{
						board[i][k-1] = board[i][k];
						board[i][k] = 0;
					}
				}
			}
		}
	}
}

void Right()
{
	int i , j , k;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 3 ; j  >= 0 ; j--)
		{
			if (board[i][j] != 0)
			{
				for (k = j ; k < 3 ; k++)
				{
					if (!board[i][k+1])
					{
						board[i][k+1] = board[i][k];
						board[i][k] = 0;
					}
				}
			}
		}
	}
	
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 3 ; j > 0 ; j--)
		{
			if (board[i][j] && board[i][j] == board[i][j-1])
			{
				board[i][j] *= 2;
				board[i][j-1] = 0;
			}
		}
	}
	
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 3 ; j  >= 0 ; j--)
		{
			if (board[i][j] != 0)
			{
				for (k = j ; k < 3 ; k++)
				{
					if (!board[i][k+1])
					{
						board[i][k+1] = board[i][k];
						board[i][k] = 0;
					}
				}
			}
		}
	}
}

int Win()
{
	int i , j;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			if (board[i][j] == 2048)
			{
				console_clear_screen();
				gotoxy(columns + 20,rows/16); 
				printf("Congratualtions!");
				gotoxy(columns + 20,rows/16 + 3);
				printf("Your score is %d",score);
				return 1;
				break;
			}
			else
			{
				return 0;
			}
		}
	}
}

int Lose()
{
	if (Full() == 16)
	{
		Playing = false;
		console_clear_screen();
		
		FILE *scoresFile;
		scoresFile = fopen("Scores.txt", "a");
		fprintf(scoresFile,"%s:%d\n", PlayerName, score);
		fclose(scoresFile);
		
		printstr(30,5,"2048",219,_RED);
		gotoxy(30,10); 
		SetColor(7);
		printf("Copyright By Shabnam Zare");
		gotoxy(30,14);
		SetColor(4);
		TypeMachine("Game Over!",_sTime * 2);
		gotoxy(30,15);
		TypeMachine("Your Final Score:",_sTime * 2);
		printf(" %d", score);
		gotoxy(30,18);
		SetColor(7);
		TypeMachine("Be Lucky Next Time!",_sTime * 2);
		gotoxy(30,19);
		TypeMachine("Do you want to play again ? (Y/n) ", _sTime * 2 + 1);
		char playAgainChoice = getch();
		if( playAgainChoice == 'y' || playAgainChoice == 'Y' )
			Initializing();
		else
			Menu();
			return 1;
	}
	else return 0;
}

void End()
{
	Playing = false;
	console_clear_screen();
	gotoxy(columns + 20,rows/16);
	printf("Your score is %d",score);
	gotoxy(columns + 20,rows/16 + 3);
	printf("Hope you enjoyed!\n\n\n\n");
}

int Compare()
{
	int move_flag = 0 , i , j;
	for (i = 0 ; i < 4 ; i++)
	{
		for (j = 0 ; j < 4 ; j++)
		{
			if (board[i][j] != copyboard[i][j])
				move_flag++;
		}
	}
	return move_flag;
}

int Score(int i,int j,int sc)
{
	if (i < 4)
	{
		if (j < 4)
		{
			sc += board[i][j];
			Score(i,j+1,sc);
		}
		else
		{
			Score(i+1,0,sc);
		}
	}
	else
	{
		return sc;
	}
}

void RandNum()
{
	int i , j , k , flag = 0 , n;
	i = rand()%10;
	while (flag == 0) 
	{
		j = RandSpot();
		k = RandSpot();
		if (!board[j][k])
		{
			if (i < 5)
			{
				board[j][k] = 2;
			}
			else
			{
				board[j][k] = 4;
			}
			flag = 1;
		}	
	}
}

void TypeMachine(char Str[], int msSleep)
{
	int i=0;
	while(Str[i] != '\0')
	{
		printf("%c", Str[i++]);
		Sleep(msSleep);
	}
}

void _BoardGame()
{
	int i,j,k;
	gotoxy(columns,8);
	printf("Playing as ( ");
	SetColor(9);
	printf("%s",PlayerName);
	SetColor(15);
	printf(" )");
	gotoxy(columns,9);
	printf("Game Status: ");
	if( Playing )
	{
		SetColor(10);
		printf("In Progress");
	}
	else
	{
		SetColor(12);
		printf("Paused");
	}
	SetColor(15);
	gotoxy(columns,10);
	for (i = 0 ; i <= 56 ; i++) 
		printf("%c",'-');
	x += 6;
	for (i = 0 ; i < 4 ; i++)
	{
		gotoxy(columns,x);
		for (j = 0 ; j < 4 ; j++)
		{
			if (!board[i][j])
			{
				printf("%c             ",179);
			
			}
			else
			{
				if (board[i][j] < 16)
				{
					printf("%c      %d      ",179,board[i][j]);
				}
				else if (board[i][j] < 128)
				{
					printf("%c     %d      ",179,board[i][j]);
				}
				else if (board[i][j] < 1024)
				{
					printf("%c     %d     ",179,board[i][j]);
				}
				else
				{
					printf("%c    %d     ",179,board[i][j]);
				}
			}
		}
		printf("%c",179);
		gotoxy(columns,x+1);
		for (k = 0 ; k <= 56 ; k++)
		{
			printf("%c",'-');
		}
		x += 2;
	}
}

void _UpdateStatus()
{
	gotoxy(columns+13,9);
	if(Playing)
	{
		printstr(30,2,"2048",219,_GREEN);
		SetColor(10);
		printf("In Progress");
	}
	else
	{
		printstr(30,2,"2048",219,_RED);
		SetColor(12);
		printf("Paused     ");
	}
	SetColor(15);
}

void _ShowTimePassed(time_t Base)
{
	time_t Passed = time(0) - Base;
	int Hours = Passed / 3600, Minutes = (Passed - Hours * 3600) / 60 ,Seconds = (Passed - Hours * 3600 - Minutes * 60);
	if( Hours == 0 )
		printf("00:");
	else if(Hours < 10)
		printf("0%d:", Hours);
	else
		printf("%d:",Hours);
		
	if( Minutes == 0 )
		printf("00:");
	else if(Minutes < 10)
		printf("0%d:", Minutes);
	else
		printf("%d:",Minutes);
	
	if( Seconds == 0 )
		printf("00");
	else if(Seconds < 10)
		printf("0%d", Seconds);
	else
		printf("%d",Seconds);
	
}

void _ProgressBar()
{
    if( !Initilized )
	{
    	int RandomDelayKey = rand()%7, sRandomDelayKey = rand()%4+1;
		gotoxy(30 , 12);
		int j = 1,logLineIndicator = 0, ProgressKeyCount = 10;
		SetColor(8);
		for (int i = 0 ; i <= ProgressKeyCount ; i += 1)
		{
			printf("%c",222);
			if( i%13 == 7 || i%13 == 1 )
				Sleep(_sTime * 2 + i/2 + 80);
			else if( i%21 == 2 || i%21 == 19 )
				Sleep(_sTime * 2 + 50 + i/3);
			else
				Sleep(_sTime * 2);

			if( i%(ProgressKeyCount/6) == 0 )
			{
				gotoxy(30, 14+logLineIndicator);
				TypeMachine(FakeMessages[logLineIndicator], 8);
				if(logLineIndicator == RandomDelayKey || logLineIndicator == sRandomDelayKey)
					Sleep(550);
				logLineIndicator++;
			}

			gotoxy(30 + j,12);
			j++;
		}  
		Sleep(1500);
    	Initilized = true;
	}
}

void _ShowSortedScores()
{
	Playing = false;
	console_clear_screen();
	struct Score *ScoresHead = NULL;
	FILE *sf;
	int bufferLength = 255;
	char buffer[bufferLength],Name[100];
	sf = fopen("Scores.txt","r");

	while(fgets(buffer, bufferLength, sf)) 
	{
	    extractNameFromLine(buffer,Name);
	    LinkedListPush(&ScoresHead, Name, extractScoreFromLine(buffer));
	}
	LinkedListInsertionSort(&ScoresHead);
	printScoresDatabase(ScoresHead);
	fclose(sf);
}

void Ceremony()
{
	Playing = false;
	int RandomIndicator = rand()%5;
	console_clear_screen();
	printstr(30,2,"2048",219,_WHITE);
	gotoxy(30,7);
	SetColor(7);
	printf("Copyright by Shabnam Zare");
	SetColor(15);
	gotoxy(30,12);
	JustifyText("policy.txt",45,30);
	SetColor(7);
	Text("copyrights.txt");
	printf("\n\n\n\n");
	for(int i=0;i<30;i++) 
		printf(" ");
		SetColor(8);
	printf("Press any key to continue...");
	getch();
	SetColor(15);
	Display();
}

int StringLength(char Str[])
{
	int i=0;
	while( Str[i] != '\0' ) 
		i++;
	return i+1;
}

void JustifyText(char fileName[], int LineCharCount, int Spaces)
{
	int LbLCharCounter=0;
	char ch;
	FILE *fp;
	fp = fopen(fileName, "r");
	while((ch = fgetc(fp)) != EOF)
	{
		printf("%c", ch);
		LbLCharCounter++;
		if( LineCharCount <= LbLCharCounter )
		{
			printf("\n");
			for(int s=0;s<Spaces;s++) 
				printf(" ");
			LbLCharCounter = 0;
		}
	}
	fclose(fp);
}

void Text(char fileName[])
{
	char ch;
	FILE *fp;
	fp = fopen(fileName, "r");
	while((ch = fgetc(fp)) != EOF) 
		printf("%c", ch);
	fclose(fp);
}

int charIndex(char Str[], char cQuery, int hIndex)
{
	if( Str[hIndex] == '\0'  )
	{
		return -1;
	}
	else
	{
		if(Str[hIndex] == cQuery) return hIndex;
		return charIndex(Str,cQuery,hIndex+1);
	}
}

void extractNameFromLine(char Line[], char* toBeSaved)
{
	int i=0;
	while(Line[i] != ':')
	{
		*(toBeSaved + i) = Line[i];
		i++;
	}
	*(toBeSaved + i) = '\0';
}

int extractScoreFromLine(char Line[])
{
	int Score = 0;
	for(int i=charIndex(Line, ':', 0)+1;Line[i] != '\n';i++)
	{
		Score = Score * 10 + ( Line[i] - '0' );
	}
	return Score;
}

void LinkedListPush(struct Score** head_ref, char Name[], long int newScore) 
{ 
    struct Score* nodeToBeAppended = (struct Score*) malloc(sizeof(struct Score)); 
    nodeToBeAppended->Score = newScore; 
    nodeToBeAppended->link = (*head_ref);
    strcpy(nodeToBeAppended->Name, Name);
    (*head_ref) = nodeToBeAppended; 
}

void LinkedListInsertSorted(struct Score** head_ref, struct Score* new_node) 
{ 
    struct Score* current; 
    if (*head_ref == NULL || (*head_ref)->Score >= new_node->Score) 
	{ 
        new_node->link = *head_ref; 
        *head_ref = new_node; 
    } 
    else
	{ 
        current = *head_ref; 
        while (current->link!=NULL && current->link->Score < new_node->Score) current = current->link; 
        new_node->link = current->link; 
        current->link = new_node; 
    } 
}
 
void LinkedListInsertionSort(struct Score **head_ref) 
{ 
    struct Score *sorted = NULL; 
    struct Score *current = *head_ref; 
    while (current != NULL)
	{ 
        struct Score *next = current->link; 
        LinkedListInsertSorted(&sorted, current); 
        current = next; 
    } 
    *head_ref = sorted; 
} 

/* Helper Functions */
void printScoresDatabase(struct Score *start) 
{ 
    struct Score *temp = start; 
    printf("\n"); 
    while (temp!=NULL)
	{ 
        printf("%d Beaten by %s\n", temp->Score, temp->Name); 
        temp = temp->link; 
    } 
} 
