#if !defined(_MEMORYMANAGEMENT_H_)
#define _MEMORYMANAGEMENT_H_

#include <memory.h>
#include "global.h"
#include <malloc.h>


//For SSE support, we use this
//DON'T USE THIS. DOESN'T WORK
//#define D3DX_ALIGN16 __declspec(align(16))



//PURPOSE:	Instead rewriting Total..., Used.... and so on everytime, let's have a struct for this
template<class T>
struct StructMemory
{
	sint	TotalElement, UsedElement, ElementExcess;
	T		*pElement;
	
	T& operator [] (sint index)
	{
		return pElement[index];
	}

	//Used amount of memory (Just like for std::vector)
	sint size() const { return UsedElement;}

	//Total amount of memory (Just like for std::vector)
	sint capacity() const { return TotalElement;}
	
	void init()			//Must be called or elements set manually
	{
		TotalElement=UsedElement=0;
		ElementExcess=512;
		pElement=NULL;
	}

	void clear()
	{
		TotalElement=UsedElement=0;
		if(pElement)
		{
			delete [] pElement;
			pElement=NULL;
		}
	}
};




//PURPOSE:	This is a generalized memory manager.
//			It is a template function.
//			amount is the extra amount needed.
//			arrayExcess must be larger than 0
//
//			**** usedArraySize must be adjusted externally ****
//			This is version 1, that's why we have a _1 at the end of the function name.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if amount is 0 or less (misuse of function)
template<class T>
sint AllocateMemory_1(T *(&pointerToArray), sint *totalArraySize, sint *usedArraySize, sint arrayExcess, sint amount)
{
	if((amount<=0)||(arrayExcess<1))
		return 2;										//Misuse

	T *temporaryPointer;
	sint tempo;
	tempo=*usedArraySize+amount;
	if(tempo>*totalArraySize)							//If memory management is necessary
	{
		*totalArraySize=(tempo/arrayExcess)*arrayExcess+arrayExcess;		//New total
		temporaryPointer=new T[*totalArraySize];
		if(temporaryPointer==0)
			return 0;									//Memory error

		if(pointerToArray)
		{
			//Copy into temporary
			//sint i;
			//for(i=0; i<*usedArraySize; i++)
			//	temporaryPointer[i]=pointerToArray[i];
			memcpy(temporaryPointer, pointerToArray, *usedArraySize*sizeof(T));
			delete [] pointerToArray;					//Delete the old array
		}
		pointerToArray=temporaryPointer;				//Get the pointer
	}
	return 1;
}

//PURPOSE:	Similar to the above, but we are using struct MemoryClass
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if amount is 0 or less (misuse of function)
template<class T>
sint AllocateMemory_2(StructMemory<T> *pStructMemory, sint amount)
{
	if((amount<=0)||(pStructMemory->ElementExcess<1))
		return 2;										//Misuse

	T *temporaryPointer;
	sint tempo;
	tempo=pStructMemory->UsedElement+amount;
	if(tempo>pStructMemory->TotalElement)							//If memory management is necessary
	{
		pStructMemory->TotalElement=(tempo/pStructMemory->ElementExcess)*pStructMemory->ElementExcess+pStructMemory->ElementExcess;		//New total
		temporaryPointer=new T[pStructMemory->TotalElement];
		if(temporaryPointer==0)
			return 0;									//Memory error

		if(pStructMemory->pElement)
		{
			//Copy into temporary
			//sint i;
			//for(i=0; i<pStructMemory->UsedElement; i++)
			//	temporaryPointer[i]=pStructMemory->pElement[i];
			memcpy(temporaryPointer, pStructMemory->pElement, pStructMemory->UsedElement*sizeof(T));
			delete [] pStructMemory->pElement;					//Delete the old array
		}
		pStructMemory->pElement=temporaryPointer;				//Get the pointer
	}
	return 1;
}

//PURPOSE:	This version aligns memory on a boundary of 16 bytes, good for SSE and 3DNow!
//			It uses _aligned_malloc from malloc.h so we need _aligned_free to release it.
//			Use DeallocateMemory_1_ALIGN16 instead.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if amount is 0 or less (misuse of function)
template<class T>
sint AllocateMemory_1_ALIGN16(T *(&pointerToArray), sint *totalArraySize, sint *usedArraySize, sint arrayExcess, sint amount)
{
	const sint alignment=16;

	if((amount<=0)||(arrayExcess<1))
		return 2;										//Misuse

	T *temporaryPointer;
	sint tempo;
	tempo=*usedArraySize+amount;
	if(tempo>*totalArraySize)							//If memory management is necessary
	{
		*totalArraySize=(tempo/arrayExcess)*arrayExcess+arrayExcess;		//New total
		temporaryPointer=(T *)_aligned_malloc(*totalArraySize*sizeof(T), alignment);
		if(temporaryPointer==0)
			return 0;									//Memory error
		if(pointerToArray)
		{
			//Copy into temporary
			//sint i;
			//for(i=0; i<*usedArraySize; i++)
			//	temporaryPointer[i]=pointerToArray[i];
			memcpy(temporaryPointer, pointerToArray, *usedArraySize*sizeof(T));
			_aligned_free(pointerToArray);					//Delete the old array
		}
		pointerToArray=temporaryPointer;				//Get the pointer
	}
	return 1;
}

//PURPOSE:	This version aligns memory on a boundary of 16 bytes, good for SSE and 3DNow!
//			It uses _aligned_malloc from malloc.h so we need _aligned_free to release it.
//			Use DeallocateMemory_1_ALIGN16 instead.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if totalArraySize is 0 or less (misuse of function)
template<class T>
sint AllocateExact_1_ALIGN16(T *(&pointerToArray), sint totalArraySize)
{
	const sint alignment=16;

	if(totalArraySize<=0)
		return 2;										//Misuse

	pointerToArray=(T *)_aligned_malloc(totalArraySize*sizeof(T), alignment);
	if(pointerToArray==NULL)
		return 0;
	return 1;
}

//PURPOSE:	Use in conjunction with AllocateMemory_1_ALIGN16
//			**** DO NOT USE delete [] ****
template<class T>
void DeallocateMemory_1_ALIGN16(T *(&pointerToArray))
{
	if(pointerToArray)
	{
		_aligned_free(pointerToArray);
		pointerToArray=NULL;
	}
}


//PURPOSE:	This is a generalized memory manager for structures and possibly classes.
//			Pointers should not be used in those, otherwise we have trouble. Smart pointers
//			are not being used and those can solve this problem.
//			It is a template function.
//			amount is the extra amount needed.
//			**** usedArraySize must be adjusted externally ****
//			This is version 1, that's why we have a _1 at the end of the function name.
//
//			Call this function like this :
//			AllocateMemoryForStruct_1((void *&)pointer, sizeof(mystruct), ....);
//
//RETURN:
//			1 for success
//			2 for misuse
//			0 for memory error
sint AllocateMemoryForStruct_1(void *(&pointerToArray), sint structSize, sint *totalArraySize, sint *usedArraySize, sint arrayExcess, sint amount);


//PURPOSE:	Deallocate and reallocate memory specified by amount.
//			Will set usedArraySize to zero and totalArraySize to amount if successful,
//			else will set both to zero.
//			This is version 1, that's why we have a _1 at the end of the function name.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if amount is 0 or less (misuse of function)
template<class T>
sint ReAllocateMemory_1(T *(&pointerToArray), sint *totalArraySize, sint *usedArraySize, sint amount)
{
	if(amount<=0)
		return 2;									//Misuse
	if(pointerToArray)
		delete [] pointerToArray;
	pointerToArray=new T[amount];
	if(pointerToArray==0)
	{
		*usedArraySize=0;
		*totalArraySize=0;
		return 0;									//Memory error
	}
	else
	{
		*usedArraySize=0;
		*totalArraySize=amount;
		return 1;
	}
}


//PURPOSE:	At least this much memory allocated, or else deletes everything (everything is lost)
//			and reallocates to fit the need. If amount is not a multiple of arrayExcess,
//			more memory than needed will be allocated.
//			amountshould be >0 but we won't report an error otherwise.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if amount is 0 or less (misuse of function) or if arrayExcess smaller than 1.
template<class T>
sint ReAllocateMemoryAtLeast_1(T *(&pointerToArray), sint *totalArraySize, sint arrayExcess, sint amount)
{
	if((amount<=0)||(arrayExcess<1))
		return 2;		//Misuse
	if(amount>*totalArraySize)
	{
		if(pointerToArray)
			delete [] pointerToArray;
		*totalArraySize=(amount/arrayExcess)*arrayExcess;
		if(amount%arrayExcess!=0)	//Not a multiple
			if(*totalArraySize<amount)
				*totalArraySize+=arrayExcess;
		if(*totalArraySize==0)
			*totalArraySize+=arrayExcess;
		pointerToArray=new T[*totalArraySize];
		if(pointerToArray==0)
			return 0;				//Memory error
	}

	return 1;
}

//PURPOSE:	For allocating the exact amount needed. The exact amount needed
//			turns out to be 0, then NULL is assigned to the pointer.
//			Should use RemoveExact_1() for removing.
//			InsertExact_1 for inserting.
//			totalArraySize is NOT automatically incremented and newly assigned quantity
//			is NOT set to NULL.
//RETURN:
//			1 for success
//			0 for memory error
//			2 for function misuse
template<class T>
sint AllocateExact_1(T *(&pointerToArray), sint *totalArraySize, sint amount)
{
	sint i;
	if(amount<=0)
		return 2;										//Misuse

	T *temporaryPointer=new T[*totalArraySize+amount];
	if(temporaryPointer==0)
		return 0;
	for(i=0; i<*totalArraySize; i++)
		temporaryPointer[i]=pointerToArray[i];
	if(pointerToArray)
		delete [] pointerToArray;
	pointerToArray=temporaryPointer;
	*totalArraySize+=amount;
	return 1;
}

//PURPOSE:	A simple version to allocate the exact amount needed
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if totalArraySize is 0 or less (misuse of function)
template<class T>
sint AllocateExact_2(T *(&pointerToArray), sint totalArraySize)
{
	if(totalArraySize<=0)
		return 2;										//Misuse

	pointerToArray=new T[totalArraySize];
	if(pointerToArray==0)
		return 0;
	return 1;
}

//PURPOSE:	Check out AllocateExact_1()
//			Remove an element from the array.
//			totalArraySize adjusted automatically.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if bad index
template<class T>
sint RemoveExact_1(T *(&pointerToArray), sint *totalArraySize, sint index)
{
	sint i;
	if((index<0)||(index>=*totalArraySize))
		return 2;
	if(*totalArraySize==1)
	{
		delete [] pointerToArray;
		pointerToArray=0;
		*totalArraySize=0;
		return 1;
	}

	T *temporaryPointer=new T[*totalArraySize-1];
	if(temporaryPointer==0)
		return 0;
	for(i=0; i<index; i++)
		temporaryPointer[i]=pointerToArray[i];
	for(i++; i<*totalArraySize; i++)
		temporaryPointer[i-1]=pointerToArray[i];
	delete [] pointerToArray;
	pointerToArray=temporaryPointer;
	*totalArraySize=*totalArraySize-1;
	return 1;
}


//PURPOSE:	Check out AllocateExact_1()
//			totalArraySize adjusted automatically.
//			newValue is the value to insert at position index, and everything there and above
//			will get pushed up.
//
//RETURN:
//			1 for success
//			0 for memory error
//			2 if bad index
template<class T>
sint InsertExact_1(T *(&pointerToArray), sint *totalArraySize, sint index, T newValue)
{
	sint i;
	if((index<0)||(index>=*totalArraySize))
		return 2;
	T *temporaryPointer=new T[*totalArraySize+1];
	if(temporaryPointer==0)
		return 0;
	for(i=0; i<index; i++)
		temporaryPointer[i]=pointerToArray[i];
	temporaryPointer[i]=newValue;
	for(; i<*totalArraySize; i++)
		temporaryPointer[i+1]=pointerToArray[i];
	delete [] pointerToArray;
	pointerToArray=temporaryPointer;
	*totalArraySize=*totalArraySize+1;
	return 1;
}



#endif	//_MEMORYMANAGEMENT_H_
