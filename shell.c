// Joao Alves & David Rochon
int fileType(char* string);
int executeFile(char* string);


//void type(char* filename);



char type [] = "type";
char execute [] = "exec";

char lines[100];
char badCommand [] = "File Not Found, Try again \r \n";
char buffer [13312];


int main()
{

	while(1)
	{
		char getfilename;
		char filename;
		int sectorsRead;

		syscall( 0, "SHELL> ", 0, 0); //promt for shell
                syscall( 1, lines, 0 , 0); //read user's string to the screen


		/* Different approach for getting 'type' instead of my function fileType

		// loades the file
		if(lines[0] == 't' && lines[1] == 'y' && lines[2] == 'p' && lines[3] == 'e')
		{	//syscall( 3, lines + 5, buffer, 0); //stores into a buffer
			//syscall( 0, buffer, 0 , 0);  //prints the message from buffer
			//getfilename(lines,filename,5);
			type(filename);
		}*/



		// read the file
		if(fileType(lines) == 1)
		{
			syscall( 3, lines + 5, buffer, &filename);
			syscall(0, buffer, 0, 0);
		}

		// executes the file
		else if(executeFile(lines) == 1)
		{
			syscall( 3, lines + 5, buffer, &filename);
			syscall( 0, buffer, 0, 0);
		}

		else
		{
			syscall(0, badCommand, 0, 0);
			// prints error message if no file matches the user input
		}


	}
}


// function to read file

/*void type(char* filename)
{
	int sectorsread;

	while(1);
	syscall(3,buffer, filename, &sectorsread);
	if(sectorsread==0)
		syscall(0, "cant find the file \r\n");
	else
		syscall(0, buffer);

}*/


//function to recognize 'type'
int fileType(char* string)
{

	int i = 0;
	int index = 1;

	while(i < 4) //searching with 'type'
	{
		if (string[i] != type[i])  //comparing the first letter with user input
		{
			index = 0; //if it is not equal, start over
			break;
		}

		i++; //move onto the next letter

	}
	return index;
}

// function to recognize 'exec'
int executeFile(char* string)
{
        int i = 0;
        int index = 1;

        while(i < 4) //searching with 'exec'
        {
                if (string[i] != execute[i])
                {
                        index = 0;
                        break;
                }

                i++;

        }
        return index;
}
