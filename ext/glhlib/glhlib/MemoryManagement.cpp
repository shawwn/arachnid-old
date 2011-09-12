#include "stdafx.h"
#include "MemoryManagement.h"







sint AllocateMemoryForStruct_1(void *(&pointerToArray), sint structSize, sint *totalArraySize, sint *usedArraySize, sint arrayExcess, sint amount)
{
	if(amount<=0)
		return 2;											//Misuse
	void *temporaryPointer;
	sint tempo;
	tempo=*usedArraySize+amount;
	if(tempo>*totalArraySize)								//If memory management is necessary
	{
		*totalArraySize=(tempo/arrayExcess)*arrayExcess+arrayExcess;		//New total
		temporaryPointer=new schar[*totalArraySize*structSize];		//Allocate this many bytes
		if(temporaryPointer==0)
			return 0;										//Memory error
		if(pointerToArray)
		{
			//Copy into temporary
			memcpy(temporaryPointer, pointerToArray, *usedArraySize*structSize);
			delete [] pointerToArray;						//Delete the old array
		}
		pointerToArray=temporaryPointer;					//Get the pointer
	}
	return 1;
}



