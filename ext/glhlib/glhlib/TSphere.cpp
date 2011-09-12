// TSphere.cpp: implementation of the TSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TSphere.h"
#include <math.h>
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



TSphere::TSphere()
{
	GeneralReset();
}

TSphere::~TSphere()
{
	DeallocateAllMemory();
}

void TSphere::GeneralReset()
{
	TBaseShape::GeneralReset();

	RadiusA=RadiusB=RadiusC=0.0;
	Stacks=Slices=0;

	StartingStack=EndingStack=0;
}

//PURPOSE:	Deallocate all reserved memory
void TSphere::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}


//PURPOSE:	Must call this at startup to create the sphere.
//			This function uses polygonNumber to determine the number of stacks and slices.
//
//PARAMETERS:
//			[in] radius : >0.0, else set to 1.0
//			[in] polygonNumber : >=9, else default is 9.
//
//RETURNS:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByPolygon(sreal radius, sint polygonNumber)
{
	if(polygonNumber<9)						//If below 9, not enough polygons to make a sphere
		Stacks=Slices=3;
	else
		Stacks=Slices=(sint)sqrtf((sreal)polygonNumber);

	StartingStack=0;
	EndingStack=Stacks-1;
	
	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count

	if(radius<=0.0)
		RadiusA=RadiusB=RadiusC=1.0;
	else
		RadiusA=RadiusB=RadiusC=radius;

	return SameStartupPatternForSphere();
}

//PURPOSE:	Must call this at startup to create the sphere.
//			This function uses polygonNumber to determine the number of stacks and slices.
//			This is for creating an ellipsoid. A sphere is really a special case of an ellipsoid.
//			Radius will be set to 0.0
//
//PARAMETERS:
//			[in] aRadius : >0.0, else set to 1.0. X direction.
//			[in] bRadius : >0.0, else set to 1.0. Y direction.
//			[in] cRadius : >0.0, else set to 1.0. Z direction.
//			[in] polygonNumber : >=9, else default is 9.
//RETURNS:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByABCPolygon(sreal aRadius, sreal bRadius, sreal cRadius, sint polygonNumber)
{
	if(polygonNumber<9)						//If below 9, not enough polygons to make a sphere
		Stacks=Slices=3;
	else
		Stacks=Slices=(sint)sqrtf((sreal)polygonNumber);

	StartingStack=0;
	EndingStack=Stacks-1;

	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count
	
	if(aRadius<=0.0)
		RadiusA=1.0;
	else
		RadiusA=aRadius;
	if(bRadius<=0.0)
		RadiusB=1.0;
	else
		RadiusB=bRadius;
	if(cRadius<=0.0)
		RadiusC=1.0;
	else
		RadiusC=cRadius;

	return SameStartupPatternForSphere();
}

//PURPOSE:	Must call this at startup to create the sphere.
//			For defining a sphere with Stacks and Slices.
//
//PARAMETERS:
//			[in] radius : >0.0, else set to 1.0
//			[in] stacks : the number of layer about the z-axis. >=3
//			[in] slices : the number of slices about the z-axis. >=3
//
//RETURNS:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByStacksAndSlices(sreal radius, sint stacks, sint slices)
{
	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	StartingStack=0;
	EndingStack=Stacks-1;

	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count

	if(radius<=0.0)
		RadiusA=RadiusB=RadiusC=1.0;
	else
		RadiusA=RadiusB=RadiusC=radius;

	return SameStartupPatternForSphere();
}

//PURPOSE:	Must call this at startup to create the sphere.
//			For defining a sphere with Stacks and Slices.
//
//PARAMETERS:
//			[in] indexFormat :  one of the defines in glhlib.h
//			[in] vertexFormat : one of the defines in glhlib.h
//			                    GLH_VERTEXFORMAT_VNTT3T3, T3 receives tangent and the other T3 receives binormal
//			[in] aRadius : >0.0, else set to 1.0
//			[in] bRadius : >0.0, else set to 1.0
//			[in] cRadius : >0.0, else set to 1.0
//			[in] stacks : >=3, else default is 3
//			[in] slices : >=3, else default is 3
//			[in] texCoordStyle : 0 for nothing to be written
//			                     1 for standard texture mapping
//			[in] scaleFactorS : to scale the texcoords
//			[in] scaleFactorT : to scale the texcoords
//
//RETURNS:
//			1 for success
//			-1 for array related array (DEBUG) remove it in the future
//			for tangent and binormal generation
sint TSphere::StartupByABCStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
						sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
						sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	StartingStack=0;
	EndingStack=Stacks-1;

	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count

	RadiusA=(aRadius<=0.0) ? 1.0 : aRadius;
	RadiusB=(bRadius<=0.0) ? 1.0 : bRadius;
	RadiusC=(cRadius<=0.0) ? 1.0 : cRadius;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 205");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 211");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 216");
		return -1;
	}

	return SameStartupPatternForSphere2();
}


//PURPOSE:	Must call this at startup to create the sphere.
//			For defining a sphere with Stacks and Slices.
//
//PARAMETERS:
//			[in] radius : >0.0, else set to 1.0
//			[in] polygonNumber : >=9, else default is 9.
//
//RETURNS:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByABCStacksAndSlices(sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices)
{
	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	StartingStack=0;
	EndingStack=Stacks-1;

	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count
	
	RadiusA=(aRadius<=0.0) ? 1.0 : aRadius;
	RadiusB=(bRadius<=0.0) ? 1.0 : bRadius;
	RadiusC=(cRadius<=0.0) ? 1.0 : cRadius;

	return SameStartupPatternForSphere();
}

//PURPOSE:	To reduce the number of lines.
sint TSphere::SameStartupPatternForSphere()
{
	sint returnVal=GenerateVertices();
	return returnVal;
}


//PURPOSE:	To reduce the number of lines.
sint TSphere::SameStartupPatternForSphere2()
{
	sint returnVal=GenerateVertices2();
	return returnVal;
}


//PURPOSE:	Calculate all the vertices.
//			There won't be any duplicate vertices. Indices must be generated as well.
//			Number of components is 3 (xyz) or 4 (xyzw) where w will be set to 1.0
//
//RETURN:
//			1 for success.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::GenerateVertices()
{
	sint i, j, k, m;
	sint tempo, tempo2;
	sreal FTempo, FTempo3, FTempo4;

	//The top and bottom of the sphere is made of triangle_fan.
	//The rest is made with triangle strips.
	//Indices will of course be GL_TRIANGLES.
	
	UsedVertex=0;
	UsedIndex=0;

	//DeallocateMemoryForVertex();
	//DeallocateMemoryForIndex();

	if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
	{
		tempo2=1+1+(Stacks-1)*(Slices+1);			//One for the top, one for the bottom, the rest is in the body
		tempo=NumberOfComponentsForVertex*tempo2;
		AllocateExact_1(pVertex, &UsedVertex, tempo);

		UsedVertex=tempo;

		sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
		sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;
		
		if(NumberOfComponentsForVertex==3)
		{
			//Top vertex
			pVertex[0]=0.0;
			pVertex[1]=0.0;
			pVertex[2]=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo=RadiusC*cosf(Theta);
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);
			
			i=3;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i+=3)
				{
					if(i+2>=UsedVertex)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}
					pVertex[i  ]=RadiusA*cosf(Phi)*FTempo3;
					pVertex[i+1]=RadiusB*sinf(Phi)*FTempo3;
					pVertex[i+2]=FTempo4;
					Phi+=PhiIncrement;
				}
				if(i+2>=UsedVertex)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex[i  ]=pVertex[m  ];
				pVertex[i+1]=pVertex[m+1];
				pVertex[i+2]=pVertex[m+2];
				i+=3;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i+2>=UsedVertex)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}
			pVertex[i  ]=0.0;
			pVertex[i+1]=0.0;
			pVertex[i+2]=-RadiusC;
		}
		else if(NumberOfComponentsForVertex==4)
		{
			//Top vertex
			pVertex[0]=0.0;
			pVertex[1]=0.0;
			pVertex[2]=RadiusC;
			pVertex[3]=1.0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo=RadiusC*cosf(Theta);
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);
			
			i=4;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i+=4)
				{
					if(i+2>=UsedVertex)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}
					pVertex[i  ]=RadiusA*cosf(Phi)*FTempo3;
					pVertex[i+1]=RadiusB*sinf(Phi)*FTempo3;
					pVertex[i+2]=FTempo4;
					pVertex[i+3]=1.0;
					Phi+=PhiIncrement;
				}
				if(i+2>=UsedVertex)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex[i  ]=pVertex[m  ];
				pVertex[i+1]=pVertex[m+1];
				pVertex[i+2]=pVertex[m+2];
				pVertex[i+3]=1.0;
				i+=3;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i+2>=UsedVertex)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}
			pVertex[i  ]=0.0;
			pVertex[i+1]=0.0;
			pVertex[i+2]=-RadiusC;
			pVertex[i+3]=1.0;
		}
	}
	else //if(!((StartingStack==0)&&(EndingStack==Stacks-1)))		//Generate the partial sphere
	{
		if(StartingStack==0)
			tempo2=1+(EndingStack+1)*(Slices+1);
		else if(EndingStack==Stacks-1)
			tempo2=1+(EndingStack-StartingStack+1)*(Slices+1);
		else
			tempo2=(EndingStack-StartingStack+2)*(Slices+1);

		tempo=NumberOfComponentsForVertex*tempo2;
		AllocateExact_1(pVertex, &UsedVertex, tempo);

		UsedVertex=tempo;

		sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
		sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;
		
		if(NumberOfComponentsForVertex==3)
		{
			if(StartingStack==0)
			{
				//Top vertex
				pVertex[0]=0.0;
				pVertex[1]=0.0;
				pVertex[2]=RadiusC;

				Theta=ThetaIncrement;
				Phi=0.0;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
				
				i=3;
				for(j=1; j<Stacks; j++)
				{
					Phi=0.0;
					m=i;									//Save start index
					for(k=0; k<Slices; k++, i+=3)
					{
						if(i+2>=UsedVertex)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}
						pVertex[i  ]=RadiusA*cosf(Phi)*FTempo3;
						pVertex[i+1]=RadiusB*sinf(Phi)*FTempo3;
						pVertex[i+2]=FTempo4;
						Phi+=PhiIncrement;
					}
					if(i+2>=UsedVertex)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex[i  ]=pVertex[m  ];
					pVertex[i+1]=pVertex[m+1];
					pVertex[i+2]=pVertex[m+2];
					i+=3;
					Theta+=ThetaIncrement;
					FTempo3=sinf(Theta);
					FTempo4=RadiusC*cosf(Theta);
				}

				//Bottom vertex
				if(i+2>=UsedVertex)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}
				pVertex[i  ]=0.0;
				pVertex[i+1]=0.0;
				pVertex[i+2]=-RadiusC;
			}
		}
	}
	//else CONTINUE CODING!!!!!!!!! TODO500

	//********** END ******************
	sint returnVal=GenerateIndices();
	if(returnVal==0)
		return 0;
	else if(returnVal==-2)
		return -1;		//Boundary error!!!!! (DEBUG)


	return 1;
}

//PURPOSE:	The indices will be generated whether vertices exist or not
//			but Stacks and Slices must be already set when calling this.
//
//RETURN:
//			1 for success
//			-1 if 16 bit indices are not enough, so switching to 32 bit.
//			-2 for array related array (DEBUG) remove it in the future.
sint TSphere::GenerateIndices()
{
	sint i, j, k, tempo2, tempo3, tempo4;
	sint vertexCount;
	tbool switchOccured=FALSE;


	DeallocateMemoryForIndex();

	if((Stacks<=0)||(Slices<=0))
		return 1;


	//Count vertices. We need this to determine if GLH_INDEXFORMAT_16BIT is possible.
	if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
	{
		vertexCount=1+1+(Stacks-1)*(Slices+1);			//One for the top, one for the bottom, the rest is in the body
	}
	else //StartingStack and EndingStack are different from the defaults
	{
		vertexCount=0;									//Start with 0

		if(StartingStack==0)							//For the top vertex
			vertexCount++;

		if(EndingStack==Stacks-1)						//For the bottom vertex
			vertexCount++;

		//Now, for the middle of the sphere vertices
		if(StartingStack==0)
		{
			if(EndingStack==Stacks-1)
				vertexCount+=EndingStack*(Slices+1);
			else
				vertexCount+=(EndingStack+1)*(Slices+1);
		}
		else //if(StartingStack!=0)
		{
			if(EndingStack==Stacks-1)
				vertexCount+=(EndingStack-StartingStack+1)*(Slices+1);
			else
				vertexCount+=(EndingStack-StartingStack+2)*(Slices+1);
		}
	}

	if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
	{
		tempo2=1+1+(Stacks-1)*(Slices+1);				//One for the top, one for the bottom, the rest is in the body

		//How many triangles needed?
		//2 * Top + body
		tempo3=(2*Slices)+(2*Slices*(Stacks-2));
		tempo3*=3;			//3 indices per triangles
	}
	else //StartingStack and EndingStack are different from the defaults
	{
		tempo2=0;										//Start with 0

		if(StartingStack==0)
			tempo2++;

		if(EndingStack==Stacks-1)						//For the bottom vertex
			tempo2++;

		//Now, for the middle of the sphere vertices
		if(StartingStack==0)
			tempo2+=(EndingStack+1)*(Slices+1);
		else
			tempo2+=(EndingStack-StartingStack+1)*(Slices+1);

		//How many triangles needed?
		//The first stack and last stack are one case.
		tempo3=0;

		if(StartingStack==0)
			tempo3+=Slices;

		if(EndingStack==Stacks-1)
			tempo3+=Slices;

		//The body part is another case
		if(StartingStack==0)
		{
			if(EndingStack==Stacks-1)
				tempo3+=2*Slices*(EndingStack-1);
			else
				tempo3+=2*Slices*(EndingStack);
		}
		else //if(StartingStack!=0)
		{
			if(EndingStack==Stacks-1)
				tempo3+=2*Slices*(EndingStack-StartingStack);
			else
				tempo3+=2*Slices*(EndingStack-StartingStack+1);
		}

		tempo3*=3;			//3 indices per triangles
	}


	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 368");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			AllocateExact_1(pIndex16Bit, &UsedIndex, tempo3);


			ushort centerVertexIndex=0;		//Top vertex
			ushort nextVertexIndex=1;
			ushort nextStackVertexIndexOffset;

			//Compose triangles
			//The top of the sphere is a triangle fan like structure.
			//We built it with individual triangles of course.
			i=0;
			for(j=0; j<Slices; j++, i+=3)
			{
				if(i+3>UsedIndex)
					return -2;

				pIndex16Bit[i  ]=centerVertexIndex;
				pIndex16Bit[i+1]=nextVertexIndex;
				pIndex16Bit[i+2]=++nextVertexIndex;
			}

			//Build body of the sphere
			nextVertexIndex=1;
			nextStackVertexIndexOffset=(ushort)(Slices+1);
			for(j=2; j<Stacks; j++)
			{
				for(k=0; k<Slices; k++)
				{
					//These are like quads, but we break them up into 2 tris.
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=nextVertexIndex+nextStackVertexIndexOffset;
					pIndex16Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex16Bit[i+2]=nextVertexIndex;
					i+=3;
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=nextVertexIndex;
					pIndex16Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex16Bit[i+2]=++nextVertexIndex;
					i+=3;
				}
				nextVertexIndex++;
			}

			//Build bottom of the sphere
			tempo2=1+1+(Stacks-1)*(Slices+1);			//One for the top, one for the bottom, the rest is in the body
			centerVertexIndex=(ushort)(tempo2-1);
			nextVertexIndex=(ushort)(tempo2-Slices-2);
			for(j=0; j<Slices; j++, i+=3)
			{
				if(i+3>UsedIndex)
					return -2;

				pIndex16Bit[i  ]=nextVertexIndex;
				pIndex16Bit[i+1]=centerVertexIndex;
				pIndex16Bit[i+2]=++nextVertexIndex;
			}
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			AllocateExact_1(pIndex16Bit, &UsedIndex, tempo3);


			ushort centerVertexIndex=0;		//Top vertex
			ushort nextVertexIndex=1;
			ushort nextStackVertexIndexOffset;

			//Compose triangles
			//The top of the sphere is a triangle fan like structure.
			//We built it with individual triangles of course.
			i=0;
			if(StartingStack==0)
			{
				for(j=0; j<Slices; j++, i+=3)
				{
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=centerVertexIndex;
					pIndex16Bit[i+1]=nextVertexIndex;
					pIndex16Bit[i+2]=++nextVertexIndex;
				}
			}

			if(StartingStack==0)
			{
				if(EndingStack==0)
					tempo4=0;								//No body triangles
				else if(EndingStack==Stacks-1)
					tempo4=EndingStack-1;
				else
					tempo4=EndingStack;
			}
			else if(StartingStack==EndingStack)
			{
				if(EndingStack==Stacks-1)
					tempo4=0;								//No body triangles
				else
					tempo4=1;
			}
			else
			{
				if(EndingStack==Stacks-1)
					tempo4=EndingStack-StartingStack;		//Yes, there will be some body triangles
				else
					tempo4=EndingStack-StartingStack+1;
			}

			//Build body of the sphere
			if(StartingStack==0)
				nextVertexIndex=1;
			else
				nextVertexIndex=0;						//Start index at 0 because there is no top vertex

			nextStackVertexIndexOffset=(ushort)(Slices+1);

			for(j=0; j<tempo4; j++)						//THIS PART HAS CHANGED
			{
				for(k=0; k<Slices; k++)
				{
					//These are like quads, but we break them up into 2 tris.
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=nextVertexIndex+nextStackVertexIndexOffset;
					pIndex16Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex16Bit[i+2]=nextVertexIndex;
					i+=3;
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=nextVertexIndex;
					pIndex16Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex16Bit[i+2]=++nextVertexIndex;
					i+=3;
				}
				nextVertexIndex++;
			}

			if(EndingStack==Stacks-1)
			{
				//Build bottom of the sphere
				centerVertexIndex=(ushort)(VertexCount-1);			//This is the index of the last vertex
				nextVertexIndex=(ushort)(VertexCount-Slices-2);
				for(j=0; j<Slices; j++, i+=3)
				{
					if(i+3>UsedIndex)
						return -2;

					pIndex16Bit[i  ]=nextVertexIndex;
					pIndex16Bit[i+1]=centerVertexIndex;
					pIndex16Bit[i+2]=++nextVertexIndex;
				}
			}
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			AllocateExact_1(pIndex32Bit, &UsedIndex, tempo3);


			uint centerVertexIndex=0;		//Top vertex
			uint nextVertexIndex=1;
			uint nextStackVertexIndexOffset;

			//Compose triangles
			//The top of the sphere is a triangle fan like structure.
			//We built it with individual triangles of course.
			i=0;
			for(j=0; j<Slices; j++, i+=3)
			{
				if(i+3>UsedIndex)
					return -2;

				pIndex32Bit[i  ]=centerVertexIndex;
				pIndex32Bit[i+1]=nextVertexIndex;
				pIndex32Bit[i+2]=++nextVertexIndex;
			}

			//Build body of the sphere
			nextVertexIndex=1;
			nextStackVertexIndexOffset=(uint)(Slices+1);
			for(j=2; j<Stacks; j++)
			{
				for(k=0; k<Slices; k++)
				{
					//These are like quads, but we break them up into 2 tris.
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=nextVertexIndex+nextStackVertexIndexOffset;
					pIndex32Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex32Bit[i+2]=nextVertexIndex;
					i+=3;
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=nextVertexIndex;
					pIndex32Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex32Bit[i+2]=++nextVertexIndex;
					i+=3;
				}
				nextVertexIndex++;
			}

			//Build bottom of the sphere
			tempo2=1+1+(Stacks-1)*(Slices+1);			//One for the top, one for the bottom, the rest is in the body
			centerVertexIndex=(uint)(tempo2-1);
			nextVertexIndex=(uint)(tempo2-Slices-2);
			for(j=0; j<Slices; j++, i+=3)
			{
				if(i+3>UsedIndex)
					return -2;

				pIndex32Bit[i  ]=nextVertexIndex;
				pIndex32Bit[i+1]=centerVertexIndex;
				pIndex32Bit[i+2]=++nextVertexIndex;
			}
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			AllocateExact_1(pIndex32Bit, &UsedIndex, tempo3);


			uint centerVertexIndex=0;		//Top vertex
			uint nextVertexIndex=1;
			uint nextStackVertexIndexOffset;

			//Compose triangles
			//The top of the sphere is a triangle fan like structure.
			//We built it with individual triangles of course.
			i=0;
			if(StartingStack==0)
			{
				for(j=0; j<Slices; j++, i+=3)
				{
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=centerVertexIndex;
					pIndex32Bit[i+1]=nextVertexIndex;
					pIndex32Bit[i+2]=++nextVertexIndex;
				}
			}

			if(StartingStack==0)
			{
				if(EndingStack==0)
					tempo4=0;								//No body triangles
				else if(EndingStack==Stacks-1)
					tempo4=EndingStack-1;
				else
					tempo4=EndingStack;
			}
			else if(StartingStack==EndingStack)
			{
				if(EndingStack==Stacks-1)
					tempo4=0;								//No body triangles
				else
					tempo4=1;
			}
			else
			{
				if(EndingStack==Stacks-1)
					tempo4=EndingStack-StartingStack;		//Yes, there will be some body triangles
				else
					tempo4=EndingStack-StartingStack+1;
			}

			//Build body of the sphere
			if(StartingStack==0)
				nextVertexIndex=1;
			else
				nextVertexIndex=0;						//Start index at 0 because there is no top vertex

			nextStackVertexIndexOffset=(uint)(Slices+1);

			for(j=0; j<tempo4; j++)						//THIS PART HAS CHANGED
			{
				for(k=0; k<Slices; k++)
				{
					//These are like quads, but we break them up into 2 tris.
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=nextVertexIndex+nextStackVertexIndexOffset;
					pIndex32Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex32Bit[i+2]=nextVertexIndex;
					i+=3;
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=nextVertexIndex;
					pIndex32Bit[i+1]=nextVertexIndex+nextStackVertexIndexOffset+1;
					pIndex32Bit[i+2]=++nextVertexIndex;
					i+=3;
				}
				nextVertexIndex++;
			}

			if(EndingStack==Stacks-1)
			{
				//Build bottom of the sphere
				centerVertexIndex=(uint)(VertexCount-1);			//This is the index of the last vertex
				nextVertexIndex=(uint)(VertexCount-Slices-2);
				for(j=0; j<Slices; j++, i+=3)
				{
					if(i+3>UsedIndex)
						return -2;

					pIndex32Bit[i  ]=nextVertexIndex;
					pIndex32Bit[i+1]=centerVertexIndex;
					pIndex32Bit[i+2]=++nextVertexIndex;
				}
			}
		}
	}

	if(switchOccured==FALSE)
		return 1;
	else
		return -1;
}

//PURPOSE:	Generate the appropriate normal vectors. The sphere must be initialized first before
//			calling this. The normals will be smoothed over the sphere.
//			One normal per vertex.
//			Number of components is 3 (xyz) or 4 (xyzw) where w will be set to 0.0.
//
//PARAMETERS:
//
//RETURNS:
//			1 for success
//			0 for memory error plus message.
//			-1 plus message if no vertices generated.
sint TSphere::GenerateNormals()
{
	sint i, j, tempo;
	sreal xscale, yscale, zscale;

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. Can't compute normal. @ GenerateNormals 1319");
		return -1;
	}

	UsedNormals=0;
	//DeallocateMemoryForNormals();


	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
	AllocateExact_1(pNormals, &UsedNormals, tempo);

	UsedNormals=tempo;

	xscale=1.0/RadiusA;
	yscale=1.0/RadiusB;
	zscale=1.0/RadiusC;


	if((NumberOfComponentsForVertex==3)&&(NumberOfComponentsForNormals==3))
	{
		//The sphere is generate with center at {0, 0, 0}
		for(i=0; i<UsedNormals; i+=3)
		{
			pNormals[i  ]=pVertex[i  ];
			pNormals[i+1]=pVertex[i+1];
			pNormals[i+2]=pVertex[i+2];
			NormalizeVectorFLOAT_2(&pNormals[i]);

			//Scale it, then renormalize
			pNormals[i  ]*=xscale;
			pNormals[i+1]*=yscale;
			pNormals[i+2]*=zscale;
			NormalizeVectorFLOAT_2(&pNormals[i]);
		}
	}
	else if((NumberOfComponentsForVertex==3)&&(NumberOfComponentsForNormals==4))
	{
		//The sphere is generate with center at {0, 0, 0}
		for(i=j=0; j<UsedNormals; i+=3, j+=4)
		{
			pNormals[j  ]=pVertex[i  ];
			pNormals[j+1]=pVertex[i+1];
			pNormals[j+2]=pVertex[i+2];
			pNormals[j+3]=0.0;
			NormalizeVectorFLOAT_2(&pNormals[j]);

			//Scale it, then renormalize
			pNormals[j  ]*=xscale;
			pNormals[j+1]*=yscale;
			pNormals[j+2]*=zscale;
			NormalizeVectorFLOAT_2(&pNormals[j]);
		}
	}
	else if((NumberOfComponentsForVertex==4)&&(NumberOfComponentsForNormals==3))
	{
		//The sphere is generate with center at {0, 0, 0}
		for(i=j=0; j<UsedNormals; i+=4, j+=3)
		{
			pNormals[j  ]=pVertex[i  ];
			pNormals[j+1]=pVertex[i+1];
			pNormals[j+2]=pVertex[i+2];
			NormalizeVectorFLOAT_2(&pNormals[j]);

			//Scale it, then renormalize
			pNormals[j  ]*=xscale;
			pNormals[j+1]*=yscale;
			pNormals[j+2]*=zscale;
			NormalizeVectorFLOAT_2(&pNormals[j]);
		}
	}
	else if((NumberOfComponentsForVertex==4)&&(NumberOfComponentsForNormals==4))
	{
		//The sphere is generate with center at {0, 0, 0}
		for(i=0; i<UsedNormals; i+=4)
		{
			pNormals[i  ]=pVertex[i  ];
			pNormals[i+1]=pVertex[i+1];
			pNormals[i+2]=pVertex[i+2];
			pNormals[i+3]=0.0;
			NormalizeVectorFLOAT_2(&pNormals[i]);

			//Scale it, then renormalize
			pNormals[i  ]*=xscale;
			pNormals[i+1]*=yscale;
			pNormals[i+2]*=zscale;
			NormalizeVectorFLOAT_2(&pNormals[i]);
		}
	}

	return 1;
}

//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//


