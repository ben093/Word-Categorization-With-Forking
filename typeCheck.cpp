#include "stdlib.h"
#include "iostream"
#include "stdio.h"
#include "string"
using namespace std;
using std::string;

void typeCheck(string word)
{
	string result;
	// 1 = Integer
	// 2 = Floating Point Number
	// 3 = String
	
	bool isInt = false;
	bool isFloat = false;
	int periodCount = 0;	
	
	// Integer check - loop through characters and check if isdigit()
	for(string::const_iterator k = word.begin(); k != word.end(); ++k)
	{
		isInt = isdigit(*k);
		
		if(!isInt)
			break; // Not an integer, check if float
	}
	
	// float check
	if(!isInt)
	{		
		for(string::const_iterator k = word.begin(); k != word.end(); ++k)
		{
			isFloat = isdigit(*k);
						
			if(!isFloat)
			{
				if(*k == '.' && periodCount < 1)
				{
					isFloat = true;
					periodCount++;
				}
				else
					break;
			}
		}
	}

	// Set up for the return
	if(isInt)
		exit(1);//result = "integer";
	else if(isFloat)
		exit(2);//result = "float";
	else
		exit(3);//result = "string";	
}
