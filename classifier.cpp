/*=============================================================
	COURSE:	CSC460, assignment3
	PROGRAMMERS: 	Ben (bb), Anthony (av)
	MODIFIED BY: Ben, Anthony
	LAST MODIFIED DATE:	3/17/2016
	DESCRIPTION:	This program is an implementation of the trapezoidal rule.
	NOTE: 50% Ben (Step 3,4,5), 50% Anthony (Step 1,2,3)
	FILES:	makefile, classifier.cpp
	IDE/COMPILER/INTERPRETER:	GNU g++
	INSTRUCTION FOR COMPILATION AND EXECUTION:
	1. make	to COMPILE
	2. type:	./classifier <fileName> to EXECUTE
=============================================================*/

#include "iostream"
#include "stdio.h"
#include "stdlib.h"
#include "fstream"
#include "sstream" 
#include "typeCheck.h"
#include "vector"
#include "unistd.h"
#include "sys/wait.h"

using namespace std;
using std::string;

int main(int argCount, char* argList[])
{
	if(argCount == 1)
	{
		cout << "Please pass a file name as an argument." << endl;
		return 0;
	}
	else if(argCount > 2)
	{
		cout << "This program only takes one argument; the name of a file." << endl;
		return 0;
	}	
	
	string filePath = "/tmp/";
	string fileName = argList[1];
	
	// ---------------------------
	// 1. Attempt to open/read file
	// ---------------------------
	ifstream input_stream;
	input_stream.open((filePath + fileName).c_str());//open the input file

	if(input_stream.fail() == true)
    {	// Failed to open
		cout << "The file " << fileName << " in " << filePath << " was unable to open successfully." << endl;
        return -1;
    }
	
	stringstream readStream;
	readStream << input_stream.rdbuf(); //read the entire file
	string contents = readStream.str(); //contents holds the content of the file
	
	input_stream.close();
	
	// ---------------------------
	// 2. Fill vector with words
	// ---------------------------
	vector<string> words;
	
	istringstream iss(contents);
	
	while(iss)
	{
		string word;
		iss >> word;
		words.push_back(word);		
	}
	
	// Stream adds one blank string in the back, pop it off for accuracy.
	words.pop_back();
	if(words.size() > 0)
	{
		cout << "There are " << words.size() << " words in the file " << fileName << endl;
	}
	else
	{
		cout << "Error: File does not contain any \"words\"." << endl;
		return -1;
	}
	
	// ---------------------------
	// 3. Loop through vector and fork a child for each.
	// ---------------------------
	int num_children = (int)words.size();
	
	vector<int> childPIDs(num_children);
	vector<string> childTypes(num_children);
	//vector<int> startOrder(num_children); //Process ID's
	vector<int> finishOrder(num_children); //Process ID's: First to finish = [0]
	
	int parents_id = getpid();	
	int firstChildID;
	//int file_pipe;
	
	for(int i = 0; i < num_children; i++) //
	{
		if(parents_id == getpid())
		{
			// ---------------------------
			// 4.Create child process for each word in the file.
			// ---------------------------
			int returnValue = fork();
			
			if(i == 0)				
			{	
				if(returnValue == -1)
				{
					cout << "First child failed. No children created." << endl;
					return 0;
				}
				// Get first childs ID
				firstChildID = getpid();
			}
			
			// push back the childs id
			childPIDs.push_back(getpid());
			
			if(returnValue == 0) // Child created successfully
			{				
				// ---------------------------
				// 5.Identify the type of each word (Integer, float, or string)
				// ---------------------------
				typeCheck(words[i]);			
				
				exit(-1); //Error, should never reach here.
			}
			else if (returnValue == -1)
			{
				cout << "Error: stopping child. Stopping parent from creating more children." << endl;
				// Set num_children to 0 to create future children from being created.
				num_children = 0;				
				break;
			}
		}
	}

	// Wait for all children to finish
	int status = 0;
	int process_id;

	int integerCounts = 0, floatCounts = 0, stringCounts = 0;
	
	int order = 0;
	while((process_id = wait(&status)) > 0)
	{				
		status = WEXITSTATUS(status);
		
		int index = process_id % firstChildID - 1;
		
		//Assign finish order.
		finishOrder[order] = process_id;
		
		switch(status)
		{
			case 1:
				integerCounts++;
				childTypes[index] = "integer";				
				break;
			case 2:
				floatCounts++;
				childTypes[index] = "float";
				break;
			case 3:
				stringCounts++;
				childTypes[index] = "string";
				break;	
		}

		cout << index << ". " << "Process ID: " << process_id << ". Word: " 
			 << words[index] << " - Type: " << childTypes[index] << endl;
		
		order++;
	}
	
	// ---------------------------
	// 6. Print Information
	// ---------------------------
	cout << "There are " << integerCounts << " integers." << endl;
	cout << "There are " << floatCounts << " floats." << endl;
	cout << "There are " << stringCounts << " strings." << endl;
	
	
	
	return 1;
}