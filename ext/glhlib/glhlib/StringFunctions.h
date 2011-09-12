#if !defined(_STRINGFUNCTIONS_H_)
#define _STRINGFUNCTIONS_H_


#include "global.h"
#include <string>			//STL strings

using namespace std;












//PURPOSE: Concatenate 2 strings. If startPosition1 is -1, then concatenate
//         at the end of string1, else use startPosition.
//         Use startPosition2 for string2 and endPosition2 for string2. In this case, string2
//         will be copied until endPosition2 or when NULL is encountered in string2.
//         If endPosition2 is -1, then copy until NULL character in string2 is reached.
//         WARNING: endPosition2 may be the SIZE of the string and NOT last index value!
//         else copy until endPosition and put a NULL right *AFTER*.
//         If isAddNULLOn is one, then a NULL will be added, else if zero, it will not add.
//         REPEAT: string1 WILL HAVE A NULL!!!!!!!!!!!
//         Hint: call ConcatenateString(string1, string2); to do a standard stringcopy.
//         Warning: start and end must have proper values of course! start<end. This function
//                  won't bother checking!
//         Enough space must be allocated for string1 externally.
//         Returns string1 length.
template<class T1, class T2>
sint ConcatenateString(T1 *string1, const T2 *string2, sint startPosition1=-1, sint startPosition2=0, sint endPosition2=-1, sint isAddNULLOn=1)
{
	sint i, j;
	if((string1==NULL)||(string2==NULL))
		return 0;
	if(startPosition1==-1)
	{
		for(i=0; string1[i]!=0; i++);
		j=startPosition2;
		if(endPosition2==-1)
		{
			while(1)
			{
				if(string2[j]==0)
				{
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else
				{
					string1[i]=(T1)string2[j];
					i++;
					j++;
				}
			}
			return i;		//string length
		}
		else //if(endPosition2!=-1)
		{
			endPosition2--;
			while(1)
			{
				//Copy until end or end of string2
				if(string2[j]==0)
				{
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else if(j==endPosition2)
				{
					string1[i]=(T1)string2[j];
					i++;
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else
				{
					string1[i]=(T1)string2[j];
					i++;
					j++;
				}
			}
			return i;		//string length
		}
	}
	else
	{
		i=startPosition1;
		j=startPosition2;
		if(endPosition2==-1)
		{
			while(1)
			{
				if(string2[j]==0)
				{
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else
				{
					string1[i]=(T1)string2[j];
					i++;
					j++;
				}
			}
			return i;		//string length
		}
		else //if(endPosition2!=-1)
		{
			endPosition2--;
			while(1)
			{
				//Copy until end or end of string2
				if(string2[j]==0)
				{
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else if(j==endPosition2)
				{
					string1[i]=(T1)string2[j];
					i++;
					if(isAddNULLOn==1)
					{
						string1[i]=0;
						i++;
					}
					break;
				}
				else
				{
					string1[i]=(T1)string2[j];
					i++;
					j++;
				}
			}
			return i;		//string length
		}
	}
}



//PURPOSE:	Find a character in a string after afterposition. If afterPosition==-1 then
//			find the last one. If nothing found return -1 else the position.
//			If afterPosition!=-1 then it does a FORWARD search in string.
//			string must be NULL terminated.
template<class T>
sint FindCharacterInString(T character, const T *string, sint afterPosition)
{
	sint i;
	if(string==NULL)
		return 0;
	if(afterPosition==-1)
	{
		i=0;
		while(1)
		{
			if(string[i]==0)
				break;
			else
				i++;
		}
		//string length is i+1
		for(; i>=0; i--)
			if(character==string[i])
				break;
		return i;				//i is position or -1 if not found
	}
	else //if(afterPosition!=-1)
	{
		i=afterPosition;
		while(1)
		{
			if(string[i]==0)
			{
				i=-1;
				break;
			}
			else if(character==string[i])
				break;
			else
				i++;
		}
		return i;			//i is position or -1 if not found
	}
}




//PURPOSE: Count the string length including NULL, return it.
//         startPosition indicates starting position in the string.
template<class T>
sint StringLengthIncludingNULL(T *pstring, sint startPosition=0)
{
	sint i=startPosition;
	if(pstring==NULL)
		return 0;
	while(1)
	{
		if(pstring[i]!=0)
			i++;
		else
		{
			i++;
			return i;
		}
	}
}


//PURPOSE: Converts an integer string into an integer.
//         stringLength must not include terminating NULL character.
//         If stringLength is -1, then string is NULL terminated
//string must be stored as string[]="-356" for example and we can have
//a '+' in fron tof it.
template<class T>
sint ConvertStringIntoSINT(T *string, sint stringLength=-1)
{
	sint i, j;
	sint val=0, tempo, sign;
	if(string==NULL)
		return 0;
	if(stringLength==-1)
		stringLength=StringLengthIncludingNULL(string)-1;
	if((string[0]=='-')||(string[0]=='+'))
		sign=1;
	else
		sign=0;
	for(i=stringLength-1; i>=sign; i--)
	{	//the following for loop is like a pow function for integers
		tempo=1;
		for(j=0; j<stringLength-i-1; j++)
			tempo*=10;
		if(string[i]=='1')
			val+=tempo;
		else if(string[i]=='2')
			val+=tempo*2;
		else if(string[i]=='3')
			val+=tempo*3;
		else if(string[i]=='4')
			val+=tempo*4;
		else if(string[i]=='5')
			val+=tempo*5;
		else if(string[i]=='6')
			val+=tempo*6;
		else if(string[i]=='7')
			val+=tempo*7;
		else if(string[i]=='8')
			val+=tempo*8;
		else if(string[i]=='9')
			val+=tempo*9;
	}
	if(string[0]=='-')
		val=-val;
	
	return val;
}

//PURPOSE: Convert a sint integers to a NULL terminated string.
//         string will not exceed 12 bytes for sure.
//         Returns the string length.
template<class T> 
sint ConvertSINTIntoString(sint number, T *string)
{
	sint i, k=0, tempo, EnterZero=0;
	sint multiplier;
	if(string==NULL)
		return 0;
	if(number<0)
	{
		number=-number;
		tempo=number;
		string[k]='-';
		k++;
	}
	else
		tempo=number;
	//REPEAT
	multiplier=1000000000;
	for(i=0; i<2; i++)			//because 2^31-1=2xxxxxxxxx
	{
		if(tempo<(i+1)*multiplier)
			break;
	}
	if(i==1)
	{
		string[k]='1';
		EnterZero=1;
		k++;
	}
	else if(i==2)
	{
		string[k]='2';
		EnterZero=1;
		k++;
	}
	tempo-=(i*multiplier);
	//REPEAT
	multiplier/=10;
	while(multiplier>0)
	{
		for(i=0; i<9; i++)
		{
			if(tempo<(i+1)*multiplier)
				break;
		}
		if(EnterZero==1)
			if(i==0)
			{
				string[k]='0';
				k++;
			}
		if(i==1)
		{
			string[k]='1';
			EnterZero=1;
			k++;
		}
		else if(i==2)
		{
			string[k]='2';
			EnterZero=1;
			k++;
		}
		else if(i==3)
		{
			string[k]='3';
			EnterZero=1;
			k++;
		}
		else if(i==4)
		{
			string[k]='4';
			EnterZero=1;
			k++;
		}
		else if(i==5)
		{
			string[k]='5';
			EnterZero=1;
			k++;
		}
		else if(i==6)
		{
			string[k]='6';
			EnterZero=1;
			k++;
		}
		else if(i==7)
		{
			string[k]='7';
			EnterZero=1;
			k++;
		}
		else if(i==8)
		{
			string[k]='8';
			EnterZero=1;
			k++;
		}
		else if(i==9)
		{
			string[k]='9';
			EnterZero=1;
			k++;
		}
		tempo-=(i*multiplier);
		multiplier/=10;
	}
	if((number==0)&&(k==0))
	{
		string[k]='0';
		k++;
	}
	string[k]=0;			//NULL terminated
	k++;
	return k;				//string length
}





//PURPOSE: Compare two strings from start to finish of first and second string.
//         Returns 1 if the same and 0 if not the same.
//         It will not verify if entered values are correct.
//         Will only do a NULL test if neither string length is specified. NULL test
//         will indicate that both strings are perfectly identical.
template<class T>
sint CompareTwoStrings(const T *string1, const T *string2, sint NULL_TestIsOn=FALSE, sint startIndexPosition1=0, sint endIndexPosition1=-1, sint startIndexPosition2=0, sint endIndexPosition2=-1)
{
	sint i=startIndexPosition1, j=startIndexPosition2;
	if((string1==NULL)||(string2==NULL))
		return 0;
	if(endIndexPosition1==-1)
	{
		if(endIndexPosition2==-1)
		{
			for( ; 1; i++, j++)
			{
				if((string1[i]!=0)&&(string2[j]!=0))
				{
					if(string1[i]!=string2[j])
						return 0;
				}
				else
				{
					if(NULL_TestIsOn==0)
						return 1;
					else if(string1[i]==string2[j])	//both are NULL?
						return 1;
					else
						return 0;
				}
			}
		}
		else
		{
			for( ; j<endIndexPosition2; i++, j++)
			{
				if(string1[i]!=0)
					if(string1[i]!=string2[j])
						return 0;
			}
			return 1;
		}
	}
	else //if(endIndexPosition1!=-1)
	{
		if(endIndexPosition2==-1)
		{
			for( ; i<endIndexPosition1; i++, j++)
			{
				if(string2[j]!=0)
					if(string1[i]!=string2[j])
						return 0;
			}
			return 1;
		}
		else
		{
			if(endIndexPosition1>endIndexPosition2)
				endIndexPosition1=endIndexPosition2;
			for( ; i<endIndexPosition1; i++, j++)
			{
				if(string1[i]!=string2[j])
					return 0;
			}
			return 1;
		}
	}
	return -1;			//should not reach this point
}

//PURPOSE: Find out if character (8 bit) is a number 0 through 9.
//         Returns 1 if yes, else 0.
template<class T>
sint IsCharacterANumber(T characterANumber)
{
	if((characterANumber>=48)&&(characterANumber<=57))//from 0 to 9
		return 1;
	else
		return 0;
}
//PURPOSE: Find out if character (8 bit) is a number 0 through 9 or
//         a '+' or '-'.
//         Returns 1 if a number, 2 if sign value, else 0.
template<class T>
sint IsCharacterANumberOrSignValue(T characterANumber)
{
	if((characterANumber>=48)&&(characterANumber<=57))//from 0 to 9
		return 1;
	else if((characterANumber=='+')||(characterANumber=='-'))
		return 2;
	else
		return 0;
}
//PURPOSE: Find out if character (8 bit) is a number 0 through 9 or a '.'
//         This is useful for sreal numbers
//         Returns 1 if a number, 2 if a point, else 0.
template<class T>
sint IsCharacterANumberOrAPoint(T characterANumber)
{
	if((characterANumber>=48)&&(characterANumber<=57))//from 0 to 9
		return 1;
	else if(characterANumber=='.')
		return 2;
	else
		return 0;
}
//PURPOSE: Find out if character (8 bit) is a number 0 through 9 or a '.'
//         or a '+' or '-'.
//         Returns 1 if a number, 2 if a point, 3 if a sign value, else 0.
template<class T>
sint IsCharacterANumberOrAPointOrSignValue(T characterANumber)
{
	if((characterANumber>=48)&&(characterANumber<=57))//from 0 to 9
		return 1;
	else if(characterANumber=='.')
		return 2;
	else if((characterANumber=='+')||(characterANumber=='-'))
		return 3;
	else
		return 0;
}
//PURPOSE: Find out if string is a valid sreal number. Must be NULL terminated.
//         Returns 1 if yes, else 0.
//It is possible to have an exponent: 1.0e-4 or 1.0E-4 or not: 1.055444
template<class T>
sint IsStringAValidSREAL_WithSignValue(T *string)
{
	if((string[0]=='-')||(string[0]=='+'))
		string++;
	sint i, j, k=0, e_found=0;
	for(i=0; string[i]!=0; i++)
	{
		j=IsCharacterANumberOrAPoint(string[i]);
		if(j==0)
		{
			if(((string[i]=='-')||(string[i]=='+'))&&((string[i-1]!='e')&&(string[i-1]!='E')))
				return 0;
			if((string[i]!='-')&&(string[i]!='+'))
			{
				if((string[i]!='e')&&(string[i]!='E'))
					return 0;
				else if(e_found==1)
					return 0;
				else
					e_found=1;
			}
		}
		else if(j==2)	//a '.'
		{
			if(e_found==1)
				return 0;
			else if(k==0)	//Counter
				k=1;
			else
				return 0;
		}
	}
	return 1;	//All was ok
}




//PURPOSE: Convert a string to a sreal. If stringLength is -1, then string is NULL
//         terminated, else stringLength must be specified.
//         WARNING: stringLength must not exceed 128 chars including NULL.
template<class T>
sreal ConvertStringIntoSREAL(T *string, sint stringLength=-1)
{
	schar buffer[128];
	sint i=0;
	if(string==NULL)
		return 0.0f;
	if(stringLength==-1)
	{
		while(1)
		{
			if(string[i]!=0)
			{
				buffer[i]=(schar)string[i];
				i++;
			}
			else
			{
				buffer[i]=0;
				break;
			}
		}
		return (sreal)atof(buffer);
	}
	else
	{
		if(stringLength<=0)
			return 0.0f;
		for( ; i<stringLength; i++)
			buffer[i]=(schar)string[i];
		return (sreal)atof(buffer);
	}
}

//PURPOSE:	Copy one string2 into string1 until a certain character is met
//			or until stringLength is met.
//			It will not check if string1 or string2 are valid pointers.
//			If isAddNULLOn is one, then a NULL will be added, else if zero, it will not add.
//			Returns stringLength including NULL (it will add a NULL!!!)
template<class T1, class T2>
sint CopyStringUntilCharacterIsMet(T2 character, T1 *string1, const T2 *string2, sint stringLength, sint isAddNULLOn=1)
{
	sint i;
	for(i=0; ((i<stringLength)&&(string2[i]!=character)); i++)
	{
		if(string2[i]==0)
			break;
		string1[i]=(T1)string2[i];
	}

	if(isAddNULLOn==1)
	{
		string1[i]=0;
		i++;
	}
	return i;
}

//PURPOSE:	Similar to the above, but returns -1 if the character is not found and if stringLength2 is encountered.
template<class T1, class T2>
sint CopyStringUntilCharacterIsMet_v2(T2 character, T1 *string1, const T2 *string2, sint stringLength1, sint stringLength2, sint isAddNULLOn=1)
{
	sint i;
	for(i=0; ((i<stringLength1)&&(i<stringLength2)&&(string2[i]!=character)); i++)
	{
		string1[i]=(T1)string2[i];
	}

	if(i>=stringLength2)
		return -1;

	if(isAddNULLOn==1)
	{
		string1[i]=0;
		i++;
	}
	return i;
}

//PURPOSE:	Similar to CopyStringUntilCharacterIsMet, but this tests for two characters
//			instead of just one. See character1 and character2.
template<class T1, class T2>
sint CopyStringUntilCharacterIsMet_v3(T2 character1, T2 character2, T1 *string1, const T2 *string2, sint stringLength, sint isAddNULLOn=1)
{
	sint i;
	for(i=0; ((i<stringLength)&&(string2[i]!=character1)&&(string2[i]!=character2)); i++)
	{
		string1[i]=(T1)string2[i];
	}

	if(isAddNULLOn==1)
	{
		string1[i]=0;
		i++;
	}
	return i;
}

//PURPOSE:	Similar to CopyStringUntilCharacterIsMet_v2, but this tests for two characters
//			instead of just one. See character1 and character2.
template<class T1, class T2>
sint CopyStringUntilCharacterIsMet_v4(T2 character1, T2 character2, T1 *string1, const T2 *string2, sint stringLength1, sint stringLength2, sint isAddNULLOn=1)
{
	sint i;
	for(i=0; ((i<stringLength1)&&(i<stringLength2)&&(string2[i]!=character1)&&(string2[i]!=character2)); i++)
	{
		string1[i]=(T1)string2[i];
	}

	if(i>=stringLength2)
		return -1;

	if(isAddNULLOn==1)
	{
		string1[i]=0;
		i++;
	}
	return i;
}




#endif		//_STRINGFUNCTIONS_H_

