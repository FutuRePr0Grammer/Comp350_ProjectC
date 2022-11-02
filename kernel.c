//kernel.c

//Joao Alves and David Rochon

void printString(char*);
void printChar(char c);
void readString(char*);
void readSector(char* buffer, int sector);
void readFile(char* filename);
void handleInterrupt21(int ax, int bx, int cx, int dx);

//hello Joao!


//char line[100];
//char buffer[512];

void main()
{
	int startVidMem = 0xb800;
	int vidMemOffSet = 0x0;
	int white = 0x7;
	char* letters = "Hello World\0";

	char line[100];
	char buffer[512];

	while (*letters != 0x0)
	{
		putInMemory(startVidMem, vidMemOffSet, *letters);
		vidMemOffSet++;
		putInMemory(startVidMem, vidMemOffSet, white);
		vidMemOffSet++;
		//advance letters pointers
		letters++;
	}

	//test for step 1 - print hello world to the screen when boot up OS
	//printString("Hello Worlds");

	//test for step 1 printChar function - prints char to the screen
	//printChar('A');

	//char line[80];
	printString("Enter a line: ");
	readString(line);
	printString(line);

	//char buffer[512];
	readSector(buffer, 30);
	printString(buffer);

	//call handleInterrupt21 to make our own interrupts!
	makeInterrupt21();

	//nothing passed, used for "21 works" message in handleInterrupt21 function below (debugging statement)
	//interrupt(0x21, 0, 0, 0, 0);

	//call printString() using interrupt 21
	interrupt(0x21, 0, line, 0, 0);	
	//call readString() using interrupt 21
	interrupt(0x21, 1, line, 0, 0);
	//call readSector() using interrupt 21
	interrupt(0x21, 2, buffer, 30, 0);
	//print buffer to print out the readsector message from message.txt
	interrupt(0x21, 0, buffer, 0, 0);
	//call interrupt 21 with ax = 3 to print the error message
	interrupt(0x21, 3, 0, 0, 0);

	while(1);
}

void printString(char* chars)
{
	//intital value
	int index = 0;
	//an array of 0 values
	char i = chars[index];

	while (i != 0)
	{	//interrupt(0x10, 0xe * 256 + 'Q', 0, 0, 0)
				//prints the letter 'Q'
		interrupt(0x10, 0xe * 256 + i, 0, 0, 0);
		i = chars[++index]; //prints out each char inside the array
					//moves onto the next char
	}				//pre-increments the index
						//value is first incremented
}

//printchar function (step 1 recommends we add this)
void printChar(char c)
{
	interrupt(0x10, 0xe * 256 + c, 0, 0, 0);
}


void readString(char* line)
{
	int index = 0;
	char i = 0x0;

	while(i != 0xd)
	{

		
		//backspace - tentative attempt 2!
		/*if(i == 0x08)
		{
			if(index != 0)
			{
				index--;
				//interrupt instead?
				printChar(0x08);
				printChar(0x20);
				printChar(0x08);
			}
		}*/

		// reading from user input
		i = interrupt(0x16, 0, 0, 0, 0);
		//pointer to char array/char
		*(line + index) = i;
		// printing user's input to the screen
		interrupt(0x10, 0xe * 0x100 + i, 0, 0, 0);
		index++;
	}
	

	printChar(0xa);
	line[index] = 0xa;
	printChar(0x0);
	index++;
	line[index] = 0x0;

/*	if(i == 0xd)
	{

		index++;	
		//adding line feed to end of the array and printing it
		*(line + index) = 0xa;
		interrupt(0x10, 0xe * 256 + index, 0, 0, 0);
		index++;
		//adding the end of string character to the end of the array and printing it
		*(line + index) = 0x0;
		interrupt(0x10, 0xe * 256 + index, 0, 0, 0);
	}*/
}

void readSector(char* buffer, int sector)
{
	int AH = 0x2;
	int AL = 0x1;

	//int BX = char*
	//int CH = (sector, 30);
	//int CL = (sector, 30) + 1;
	//int DH = (sector, 30);

	char* BX = buffer;
	int CH = 0x0;
	int CL = sector + 1;
	int DH = 0x0;

	int DL = 0x80;
	int AX = AH * 256 + AL;
	int CX = CH * 256 + CL;
	int DX = DH * 256 + DL;

	//interrupt(number, AX, BD, CX, DX)
	interrupt(0x13, AX, BX, CX, DX);

	//interrupt(0x13, AX, CX, DX, buffer);

}

//function to read a file and to load it, sector by sector, to buffer array
void readFile(char* filename)
{

	//work here
}


//makes interrupt 21 based on function in kernel.asm. Stores our code in the interrupt vector table at the base of memory
//when interrupt 21 happens, goes to the table in memory, executes our code
void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	//debugging statement
	//printString("Interrupt 21 Works!");

	//if ax equals zero, then call printString()
	if(ax == 0)
	{
		printString(bx);
	}

	//else readString() is called
	else if(ax == 1)
	{
		readString(bx);
	}

	//else read sector
	else if(ax == 2)
	{
		readSector(bx, cx);
	}
	else if(ax >= 3)
	{
		printString("Invalid value for AX. No function available! Please try again.");
	}

}





