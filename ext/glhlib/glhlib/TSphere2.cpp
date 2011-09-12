// TSphere2.cpp: implementation of the TSphere class.
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










//PURPOSE:	Calculate the texture coordinates for the sphere for a specific texture unit.
//
//PARAMETERS:
//			[in] texUnit : Must be between 0 and 7 inclusively. We need 2 components at minimum.
//			[in] numberOfComponents : 2, 3 or 4
//			[in] isGlobeMapOn  : If TRUE, then texture mapping will be such that we have a 'globe', meaning
//			                     an entire texture will be wrapped around the sphere. This is perfect for games.
//			                     s maps CCW looking from the +z direction, starting from 0.0 and comming back to 1.0 when it
//			                     returns to the same position (but different vertex).
//			                     t maps from +z to -z as 0.0 to 1.0.
//			[in] invertS : If FALSE and if isGlobeMapOn == TRUE, 0.0 to 1.0 CCW, else 1.0 to 0.0
//			[in] invertT : If FALSE and if isGlobeMapOn == TRUE, 0.0 to 1.0 , else 1.0 to 0.0
//			[in] texturingMode : Is one of many modes, look in defines header file. (ignored)
//			[in] chooseVertex : Is the vertices index where we begin texturing. Texture coordinates are (0, 0) (ignored)
//			                    Does not depend on (x, y, z) triplet, so it must be multiplied by three.
//			[in] orientationAngle : The sphere has an intrinsic orientation in memory, and this value
//			                        is used to orient the texture at chooseVertex.
//			[in] widthTextureLimit : Is the maximum width of the texture (this is for the future, leave it on 1.0 for now.)
//			[in] heightTextureLimit : Is the maximum width of the texture (this is for the future, leave it on 1.0 for now.)
//			[in] sScale: Is the width ratio to space units. (ignored)
//			[in] tScale: Is the height ratio to space unts. (ignored)
//
//RETURN:
//			-1 if texUnit is out of range.
//			-2 if numberOfComponents is not 2, 3 or 4.
//			-3 no vertices have been generated yet.
//			1 if successful.
//			-10 for array related array (DEBUG) remove it in the future.
//
//NOTE: I have only implemented numberOfComponents==2.
sint TSphere::GenerateTexCoords(sint texUnit, sint numberOfComponents, tbool isGlobeMapOn, tbool invertS, tbool invertT, sint texturingMode, sint chooseVertex, sreal orientationAngle, sreal widthTextureLimit, sreal heightTextureLimit, sreal sScale, sreal tScale)
{
	sint i, j, k, tempo;
	sreal FTempo, FTempo2;
	sreal texIncrementS, texIncrementT;		//Increment factors in texture coordinates

	if((texUnit<0)||(texUnit>=NUMBEROFTEXTUREUNITS))
	{
		strcpy(ErrorMessage, "Tex unit is out of range. @ GenerateTexCoords 496");
		return -1;
	}

	if((numberOfComponents!=2)&&(numberOfComponents!=3)&&(numberOfComponents!=4))
	{
		strcpy(ErrorMessage, "Number of components for texcoord is not 2, 3 or 4. @ GenerateTexCoords 59");
		return -2;
	}

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. Can't compute normal. @ GenerateTexCoords 68");
		return -3;
	}

	if(isGlobeMapOn)
	{
		UsedTexCoords[texUnit]=0;
		//DeallocateMemoryForTexCoords(texUnit);
		NumberOfComponentsForTexCoords[texUnit]=numberOfComponents;

		tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForTexCoords[texUnit];
		AllocateExact_1(pTexCoords[texUnit], &UsedTexCoords[texUnit], tempo);
		UsedTexCoords[texUnit]=tempo;

		//////////////////////////////////////////////////
		texIncrementS=(sreal)1.0/(sreal)Slices;
		texIncrementT=(sreal)1.0/(sreal)Stacks;

		if(NumberOfComponentsForTexCoords[texUnit]==2)
		{
			//Top of the sphere (triangle_fan)
			//The center-bottom of the texture applied to top of sphere
			i=0;
			if(i+1>=UsedTexCoords[texUnit])
			{
				return -10;		//Boundary error!!!!! (DEBUG)
			}

			pTexCoords[texUnit][i  ]=0.5;
			if(invertT==FALSE)
				pTexCoords[texUnit][i+1]=0.0;
			else
				pTexCoords[texUnit][i+1]=1.0;

			i+=2;
			FTempo=0.0;
			if(invertT==FALSE)
				FTempo2=texIncrementT;
			else
			{
				FTempo2=1.0-texIncrementT;
				texIncrementT=-texIncrementT;
			}

			if(invertS==TRUE)
				texIncrementS=-texIncrementS;

			for(j=1; j<Stacks; j++)
			{
				if(invertS==FALSE)
					FTempo=0.0;
				else
					FTempo=1.0;
				for(k=0; k<Slices; k++, i+=2)
				{
					if(i+1>=UsedTexCoords[texUnit])
					{
						return -10;		//Boundary error!!!!! (DEBUG)
					}
					pTexCoords[texUnit][i  ]=FTempo;
					pTexCoords[texUnit][i+1]=FTempo2;
					FTempo+=texIncrementS;
				}
				if(i+1>=UsedTexCoords[texUnit])
				{
					return -10;		//Boundary error!!!!! (DEBUG)
				}
				//Back to initial position
				if(invertS==FALSE)
					pTexCoords[texUnit][i  ]=1.0;
				else
					pTexCoords[texUnit][i  ]=0.0;
				pTexCoords[texUnit][i+1]=FTempo2;
				i+=2;
				FTempo2+=texIncrementT;
			}
			
			//Bottom of the sphere
			if(i+1>=UsedTexCoords[texUnit])
			{
				return -10;		//Boundary error!!!!! (DEBUG)
			}
			pTexCoords[texUnit][i  ]=0.5;
			if(invertT==FALSE)
				pTexCoords[texUnit][i+1]=1.0;
			else
				pTexCoords[texUnit][i+1]=0.0;
			return 1;
		}
	}
	
	return -1;
}

//PURPOSE:	Be sure to call GenerateNormals() before calling this.
//			WE NEED TO GENERATE BOTH VECTORS AT THE SAME TIME.
//			Indices are required.
//			Vertices are required.
//			Normals are required.
//			Texcoord[0] is required.
//			
//
//RETURNS:
//			1 for success.
//			-1 if UsedIndex<=0
//			-2 if UsedVertex<=0
//			-3 if UsedNormals<=0
//			-4 if UsedTexCoords[0]<=0
//			-5 if bad value passed to ComputeTangentAndBinormalVectors_v1_FLOAT_1 or ComputeTangentAndBinormalVectors_v1_FLOAT_2
sint TSphere::GenerateTangentAndBinormalVectors()
{
	sint tempo, tempo2;

	if(UsedIndex<=0)
	{
		strcpy(ErrorMessage, "Index count is zero or less. @ GenerateTangentAndBinormalVectors 195");
		return -1;
	}

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. @ GenerateTangentAndBinormalVectors 201");
		return -2;
	}

	if(UsedNormals<=0)
	{
		strcpy(ErrorMessage, "Normal count is zero or less. @ GenerateTangentAndBinormalVectors 207");
		return -3;
	}

	if(UsedTexCoords[0]<=0)
	{
		strcpy(ErrorMessage, "Texcoord[0] count is zero or less. @ GenerateTangentAndBinormalVectors 213");
		return -4;
	}

	UsedTangents=0;
	UsedBinormals=0;
	//DeallocateMemoryForTangentVectors();
	//DeallocateMemoryForBinormalVectors();

	//One per vertex
	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	AllocateExact_1(pTangents, &UsedTangents, tempo);

	tempo2=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	AllocateExact_1(pBinormals, &UsedBinormals, tempo);

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		sint returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_2(UsedIndex, pIndex16Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 243");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 248");
			return 0;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		sint returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT(UsedIndex, pIndex32Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 258");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 263");
			return 0;
		}
	}

	UsedTangents=tempo;
	UsedBinormals=tempo2;

	return 1;
}

//PURPOSE:	As powerful as StartupByABCStacksAndSlices, but we added startingStack and endingStack.
//			startingStack is the first stack and endingStack is the last one to be included.
//			Example : If stacks = 10 and 
//			startingStack = 0
//			endingStack = 2
//
//			Stacks are numbered 0 to 9 in this case.
//			We will generate stack #0, 1 and 2
//
//PARAMETERS:
//			[in] radius : >0.0, else set to 1.0
//			[in] stacks : the number of layer about the z-axis. >=3
//			[in] slices : the number of slices about the z-axis. >=3
//			[in] startingStack : must be 0 to stacks-1, the first value (stacks must be valid)
//			[in] endingStack : must be 0 to stacks-1, the second value (stacks must be valid)
//
//RETURN:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByABCStacksAndSlices_StackBound(sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
		sint startingStack, sint endingStack)
{
	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	if(startingStack<0)
		StartingStack=0;
	else if(startingStack>=Stacks)
		StartingStack=Stacks-1;
	else
		StartingStack=startingStack;

	if(endingStack>=Stacks)
		EndingStack=Stacks-1;
	else if(endingStack<StartingStack)
		EndingStack=StartingStack;
	else
		EndingStack=endingStack;

	PolygonNumber=2*Slices+(Stacks-2)*Slices*2;		//Triangle count
	
	RadiusA=(aRadius<=0.0) ? 1.0 : aRadius;
	RadiusB=(bRadius<=0.0) ? 1.0 : bRadius;
	RadiusC=(cRadius<=0.0) ? 1.0 : cRadius;

	return SameStartupPatternForSphere();
}


//PURPOSE:	Calculate all the vertices.
//			There will be some duplicate vertices but not much. This is so that certain vertices can
//			have 2 texcoords.
//			Indices must be generated as well.
//
//RETURN:
//			1 for success.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::GenerateVertices2()
{
	sint i, j, k, m, returnVal;
	sreal FTempo3, FTempo4;
	sint modifiedStartingStack, modifiedEndingStack;


	//The top and bottom of the sphere is made of triangle_fan.
	//The rest is made with triangle strips.
	//Indices will of course be GL_TRIANGLES.

	DeallocateMemoryForIndex();
	DeallocateMemoryForVertex();
	DeallocateMemoryForNormals();
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		DeallocateMemoryForTexCoords(i);
	}
	DeallocateMemoryForColors();
	DeallocateMemoryForSecondaryColors();
	DeallocateMemoryForTangentVectors();
	DeallocateMemoryForBinormalVectors();

	DeallocateMemoryVertexInterlaced();

	if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
	{
		VertexCount=1+1+(Stacks-1)*(Slices+1);			//One for the top, one for the bottom, the rest is in the body
	}
	else //StartingStack and EndingStack are different from the defaults
	{
		VertexCount=0;									//Start with 0

		if(StartingStack==0)							//For the top vertex
			VertexCount++;

		if(EndingStack==Stacks-1)						//For the bottom vertex
			VertexCount++;

		//Now, for the middle of the sphere vertices
		if(StartingStack==0)
		{
			if(EndingStack==Stacks-1)
				VertexCount+=EndingStack*(Slices+1);
			else
				VertexCount+=(EndingStack+1)*(Slices+1);
		}
		else //if(StartingStack!=0)
		{
			if(EndingStack==Stacks-1)
				VertexCount+=(EndingStack-StartingStack+1)*(Slices+1);
			else
				VertexCount+=(EndingStack-StartingStack+2)*(Slices+1);
		}
	}

	Start_DrawRangeElements=0;
	End_DrawRangeElements=VertexCount-1;


	//For tangent and binormal generation, indices are needed so generate them first
	if(GenerateIndices()==-2)
		return -1;


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_V=new GLHVertex_V[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_V[0].x=0.0;
			pVertex_V[0].y=0.0;
			pVertex_V[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_V[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_V[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_V[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_V[i].x=pVertex_V[m].x;
				pVertex_V[i].y=pVertex_V[m].y;
				pVertex_V[i].z=pVertex_V[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			//Bottom vertex
			pVertex_V[i].x=0.0;
			pVertex_V[i].y=0.0;
			pVertex_V[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_V=new GLHVertex_V[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_V[0].x=0.0;
				pVertex_V[0].y=0.0;
				pVertex_V[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_V[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_V[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_V[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_V[i].x=pVertex_V[m].x;
					pVertex_V[i].y=pVertex_V[m].y;
					pVertex_V[i].z=pVertex_V[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_V[i].x=0.0;
				pVertex_V[i].y=0.0;
				pVertex_V[i].z=-RadiusC;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_VT=new GLHVertex_VT[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_VT[0].x=0.0;
			pVertex_VT[0].y=0.0;
			pVertex_VT[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VT[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_VT[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_VT[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VT[i].x=pVertex_VT[m].x;
				pVertex_VT[i].y=pVertex_VT[m].y;
				pVertex_VT[i].z=pVertex_VT[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			pVertex_VT[i].x=0.0;
			pVertex_VT[i].y=0.0;
			pVertex_VT[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_VT=new GLHVertex_VT[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_VT[0].x=0.0;
				pVertex_VT[0].y=0.0;
				pVertex_VT[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_VT[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_VT[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_VT[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VT[i].x=pVertex_VT[m].x;
					pVertex_VT[i].y=pVertex_VT[m].y;
					pVertex_VT[i].z=pVertex_VT[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VT[i].x=0.0;
				pVertex_VT[i].y=0.0;
				pVertex_VT[i].z=-RadiusC;
			}
		}

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_VNT=new GLHVertex_VNT[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_VNT[0].x=0.0;
			pVertex_VNT[0].y=0.0;
			pVertex_VNT[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNT[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_VNT[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_VNT[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNT[i].x=pVertex_VNT[m].x;
				pVertex_VNT[i].y=pVertex_VNT[m].y;
				pVertex_VNT[i].z=pVertex_VNT[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			pVertex_VNT[i].x=0.0;
			pVertex_VNT[i].y=0.0;
			pVertex_VNT[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_VNT=new GLHVertex_VNT[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_VNT[0].x=0.0;
				pVertex_VNT[0].y=0.0;
				pVertex_VNT[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_VNT[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_VNT[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_VNT[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNT[i].x=pVertex_VNT[m].x;
					pVertex_VNT[i].y=pVertex_VNT[m].y;
					pVertex_VNT[i].z=pVertex_VNT[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNT[i].x=0.0;
				pVertex_VNT[i].y=0.0;
				pVertex_VNT[i].z=-RadiusC;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_VNT3[0].x=0.0;
			pVertex_VNT3[0].y=0.0;
			pVertex_VNT3[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNT3[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_VNT3[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_VNT3[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNT3[i].x=pVertex_VNT3[m].x;
				pVertex_VNT3[i].y=pVertex_VNT3[m].y;
				pVertex_VNT3[i].z=pVertex_VNT3[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			pVertex_VNT3[i].x=0.0;
			pVertex_VNT3[i].y=0.0;
			pVertex_VNT3[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_VNT3[0].x=0.0;
				pVertex_VNT3[0].y=0.0;
				pVertex_VNT3[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_VNT3[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_VNT3[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_VNT3[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNT3[i].x=pVertex_VNT3[m].x;
					pVertex_VNT3[i].y=pVertex_VNT3[m].y;
					pVertex_VNT3[i].z=pVertex_VNT3[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNT3[i].x=0.0;
				pVertex_VNT3[i].y=0.0;
				pVertex_VNT3[i].z=-RadiusC;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_VNTT3T3[0].x=0.0;
			pVertex_VNTT3T3[0].y=0.0;
			pVertex_VNTT3T3[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNTT3T3[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_VNTT3T3[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_VNTT3T3[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[m].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[m].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			pVertex_VNTT3T3[i].x=0.0;
			pVertex_VNTT3T3[i].y=0.0;
			pVertex_VNTT3T3[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_VNTT3T3[0].x=0.0;
				pVertex_VNTT3T3[0].y=0.0;
				pVertex_VNTT3T3[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_VNTT3T3[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_VNTT3T3[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_VNTT3T3[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[m].x;
					pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[m].y;
					pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNTT3T3[i].x=0.0;
				pVertex_VNTT3T3[i].y=0.0;
				pVertex_VNTT3T3[i].z=-RadiusC;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 712");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 721");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
		{
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			pVertex_VNTT3[0].x=0.0;
			pVertex_VNTT3[0].y=0.0;
			pVertex_VNTT3[0].z=RadiusC;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			i=1;
			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index
				for(k=0; k<Slices; k++, i++)
				{
					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNTT3[i].x=RadiusA*cosf(Phi)*FTempo3;
					pVertex_VNTT3[i].y=RadiusB*sinf(Phi)*FTempo3;
					pVertex_VNTT3[i].z=FTempo4;
					Phi+=PhiIncrement;
				}

				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNTT3[i].x=pVertex_VNTT3[m].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[m].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[m].z;
				i++;
				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(i>=VertexCount)
			{
				return -1;		//Boundary error!!!!! (DEBUG)
			}

			pVertex_VNTT3[i].x=0.0;
			pVertex_VNTT3[i].y=0.0;
			pVertex_VNTT3[i].z=-RadiusC;
		}
		else //StartingStack and EndingStack are different from the defaults
		{
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];


			sreal PhiIncrement=(2.0*PI_DEF)/(sreal)Slices, Phi;
			sreal ThetaIncrement=PI_DEF/(sreal)Stacks, Theta;

			//Top vertex
			if(StartingStack==0)
			{
				pVertex_VNTT3[0].x=0.0;
				pVertex_VNTT3[0].y=0.0;
				pVertex_VNTT3[0].z=RadiusC;
				i=1;
			}
			else
				i=0;

			Theta=ThetaIncrement;
			Phi=0.0;
			FTempo3=sinf(Theta);
			FTempo4=RadiusC*cosf(Theta);

			if(StartingStack==0)
			{
				modifiedStartingStack=1;
				modifiedEndingStack=EndingStack+1;
			}
			else
			{
				modifiedStartingStack=StartingStack;

				if(EndingStack==Stacks-1)
					modifiedEndingStack=EndingStack;
				else
					modifiedEndingStack=EndingStack+1;
			}


			for(j=1; j<Stacks; j++)
			{
				Phi=0.0;
				m=i;									//Save start index

				if((StartingStack<=j)&&(j<=modifiedEndingStack))
				{
					for(k=0; k<Slices; k++, i++)
					{
						if(i>=VertexCount)
						{
							return -1;		//Boundary error!!!!! (DEBUG)
						}

						pVertex_VNTT3[i].x=RadiusA*cosf(Phi)*FTempo3;
						pVertex_VNTT3[i].y=RadiusB*sinf(Phi)*FTempo3;
						pVertex_VNTT3[i].z=FTempo4;
						Phi+=PhiIncrement;
					}

					if(i>=VertexCount)
					{
						return -1;		//Boundary error!!!!! (DEBUG)
					}

					pVertex_VNTT3[i].x=pVertex_VNTT3[m].x;
					pVertex_VNTT3[i].y=pVertex_VNTT3[m].y;
					pVertex_VNTT3[i].z=pVertex_VNTT3[m].z;
					i++;
				}

				Theta+=ThetaIncrement;
				FTempo3=sinf(Theta);
				FTempo4=RadiusC*cosf(Theta);
			}

			//Bottom vertex
			if(EndingStack==Stacks-1)
			{
				if(i>=VertexCount)
				{
					return -1;		//Boundary error!!!!! (DEBUG)
				}

				pVertex_VNTT3[i].x=0.0;
				pVertex_VNTT3[i].y=0.0;
				pVertex_VNTT3[i].z=-RadiusC;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);

		//We need to make the pVertex_VNTT3T3 format because ComputeTangentAndBinormalVectors_v1_FLOAT_4 and _3 needs it.
		pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3T3[i].x=pVertex_VNTT3[i].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3[i].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3[i].z;
			pVertex_VNTT3T3[i].nx=pVertex_VNTT3[i].nx;
			pVertex_VNTT3T3[i].ny=pVertex_VNTT3[i].ny;
			pVertex_VNTT3T3[i].nz=pVertex_VNTT3[i].nz;
			pVertex_VNTT3T3[i].s0=pVertex_VNTT3[i].s0;
			pVertex_VNTT3T3[i].t0=pVertex_VNTT3[i].t0;
		}

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 712");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 721");
				return -1;
			}
		}

		//Copy the tangents
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3[i].s1=pVertex_VNTT3T3[i].s1;
			pVertex_VNTT3[i].t1=pVertex_VNTT3T3[i].t1;
			pVertex_VNTT3[i].r1=pVertex_VNTT3T3[i].r1;
		}

		delete [] pVertex_VNTT3T3;
		pVertex_VNTT3T3=NULL;
	}

	return 1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TSphere::GenerateNormals2()
{
	sint i;
	sreal xscale, yscale, zscale;

	xscale=1.0/RadiusA;
	yscale=1.0/RadiusB;
	zscale=1.0/RadiusC;

	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNT[i].nx=pVertex_VNT[i].x;
			pVertex_VNT[i].ny=pVertex_VNT[i].y;
			pVertex_VNT[i].nz=pVertex_VNT[i].z;
			NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

			//Scale it, then renormalize
			pVertex_VNT[i].nx*=xscale;
			pVertex_VNT[i].ny*=yscale;
			pVertex_VNT[i].nz*=zscale;
			NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNT3[i].nx=pVertex_VNT3[i].x;
			pVertex_VNT3[i].ny=pVertex_VNT3[i].y;
			pVertex_VNT3[i].nz=pVertex_VNT3[i].z;
			NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

			//Scale it, then renormalize
			pVertex_VNT3[i].nx*=xscale;
			pVertex_VNT3[i].ny*=yscale;
			pVertex_VNT3[i].nz*=zscale;
			NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[i].x;
			pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[i].y;
			pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[i].z;
			NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

			//Scale it, then renormalize
			pVertex_VNTT3T3[i].nx*=xscale;
			pVertex_VNTT3T3[i].ny*=yscale;
			pVertex_VNTT3T3[i].nz*=zscale;
			NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3[i].nx=pVertex_VNTT3[i].x;
			pVertex_VNTT3[i].ny=pVertex_VNTT3[i].y;
			pVertex_VNTT3[i].nz=pVertex_VNTT3[i].z;
			NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

			//Scale it, then renormalize
			pVertex_VNTT3[i].nx*=xscale;
			pVertex_VNTT3[i].ny*=yscale;
			pVertex_VNTT3[i].nz*=zscale;
			NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);
		}

		return 1;
	}

	return -1;
}

//PURPOSE:	Calculate the texture coordinates for the sphere.
//
//PARAMETERS:
//			[in] invertS : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//			[in] invertT : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//
//RETURN:
//			1 if successful
//			-1 if TexCoordStyle is not 1 or if the VertexFormat is not supported
sint TSphere::GenerateTexCoords2(tbool invertS, tbool invertT)
{
	sint i, j, k;
	sreal FTempo, FTempo2;
	sreal texIncrementS, texIncrementT;		//Increment factors in texture coordinates
	sint modifiedStartingStack, modifiedEndingStack;

	if(TexCoordStyle==1)
	{
		//////////////////////////////////////////////////
		texIncrementS=(sreal)1.0/(sreal)Slices;
		texIncrementT=(sreal)1.0/(sreal)Stacks;

		if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
			{
				//Top of the sphere (triangle_fan)
				//The center-bottom of the texture applied to top of sphere
				i=0;
				pVertex_VT[i].s0=0.5;
				if(invertT==FALSE)
					pVertex_VT[i].t0=0.0;
				else
					pVertex_VT[i].t0=1.0;

				i++;
				FTempo=0.0;
				if(invertT==FALSE)
					FTempo2=texIncrementT;
				else
				{
					FTempo2=1.0-texIncrementT;
					texIncrementT=-texIncrementT;
				}

				if(invertS==TRUE)
					texIncrementS=-texIncrementS;

				for(j=1; j<Stacks; j++)
				{
					if(invertS==FALSE)
						FTempo=0.0;
					else
						FTempo=1.0;

					for(k=0; k<Slices; k++, i++)
					{
						pVertex_VT[i].s0=FTempo;
						pVertex_VT[i].t0=FTempo2;
						FTempo+=texIncrementS;
					}

					//Back to initial position
					if(invertS==FALSE)
						pVertex_VT[i].s0=1.0;
					else
						pVertex_VT[i].s0=0.0;

					pVertex_VT[i].t0=FTempo2;
					i++;
					FTempo2+=texIncrementT;
				}

				pVertex_VT[i].s0=0.5;
				if(invertT==FALSE)
					pVertex_VT[i].t0=1.0;
				else
					pVertex_VT[i].t0=0.0;
			}
			else //StartingStack and EndingStack are different from the defaults
			{
				//Top of the sphere (triangle_fan)
				//The center-bottom of the texture applied to top of sphere
				i=0;
				if(StartingStack==0)
				{
					pVertex_VT[i].s0=0.5;
					if(invertT==FALSE)
						pVertex_VT[i].t0=0.0;
					else
						pVertex_VT[i].t0=1.0;

					i++;
				}

				FTempo=0.0;
				if(invertT==FALSE)
					FTempo2=texIncrementT;
				else
				{
					FTempo2=1.0-texIncrementT;
					texIncrementT=-texIncrementT;
				}

				if(invertS==TRUE)
					texIncrementS=-texIncrementS;

				if(StartingStack==0)
				{
					modifiedStartingStack=1;
					modifiedEndingStack=EndingStack+1;
				}
				else
				{
					modifiedStartingStack=StartingStack;

					if(EndingStack==Stacks-1)
						modifiedEndingStack=EndingStack;
					else
						modifiedEndingStack=EndingStack+1;
				}

				for(j=1; j<Stacks; j++)
				{
					if((StartingStack<=j)&&(j<=modifiedEndingStack))
					{
						if(invertS==FALSE)
							FTempo=0.0;
						else
							FTempo=1.0;

						for(k=0; k<Slices; k++, i++)
						{
							pVertex_VT[i].s0=FTempo;
							pVertex_VT[i].t0=FTempo2;
							FTempo+=texIncrementS;
						}

						//Back to initial position
						if(invertS==FALSE)
							pVertex_VT[i].s0=1.0;
						else
							pVertex_VT[i].s0=0.0;

						pVertex_VT[i].t0=FTempo2;
						i++;
					}

					FTempo2+=texIncrementT;
				}

				if(EndingStack==Stacks-1)
				{
					pVertex_VT[i].s0=0.5;
					if(invertT==FALSE)
						pVertex_VT[i].t0=1.0;
					else
						pVertex_VT[i].t0=0.0;
				}
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			if((StartingStack==0)&&(EndingStack==Stacks-1))		//Generate the full sphere
			{
				//Top of the sphere (triangle_fan)
				//The center-bottom of the texture applied to top of sphere
				i=0;
				pVertex_VNT[i].s0=0.5;
				if(invertT==FALSE)
					pVertex_VNT[i].t0=0.0;
				else
					pVertex_VNT[i].t0=1.0;

				i++;
				FTempo=0.0;
				if(invertT==FALSE)
					FTempo2=texIncrementT;
				else
				{
					FTempo2=1.0-texIncrementT;
					texIncrementT=-texIncrementT;
				}

				if(invertS==TRUE)
					texIncrementS=-texIncrementS;

				for(j=1; j<Stacks; j++)
				{
					if(invertS==FALSE)
						FTempo=0.0;
					else
						FTempo=1.0;

					for(k=0; k<Slices; k++, i++)
					{
						pVertex_VNT[i].s0=FTempo;
						pVertex_VNT[i].t0=FTempo2;
						FTempo+=texIncrementS;
					}

					//Back to initial position
					if(invertS==FALSE)
						pVertex_VNT[i].s0=1.0;
					else
						pVertex_VNT[i].s0=0.0;

					pVertex_VNT[i].t0=FTempo2;
					i++;
					FTempo2+=texIncrementT;
				}

				pVertex_VNT[i].s0=0.5;
				if(invertT==FALSE)
					pVertex_VNT[i].t0=1.0;
				else
					pVertex_VNT[i].t0=0.0;
			}
			else //StartingStack and EndingStack are different from the defaults
			{
				//Top of the sphere (triangle_fan)
				//The center-bottom of the texture applied to top of sphere
				i=0;
				if(StartingStack==0)
				{
					pVertex_VNT[i].s0=0.5;
					if(invertT==FALSE)
						pVertex_VNT[i].t0=0.0;
					else
						pVertex_VNT[i].t0=1.0;

					i++;
				}

				FTempo=0.0;
				if(invertT==FALSE)
					FTempo2=texIncrementT;
				else
				{
					FTempo2=1.0-texIncrementT;
					texIncrementT=-texIncrementT;
				}

				if(invertS==TRUE)
					texIncrementS=-texIncrementS;

				if(StartingStack==0)
				{
					modifiedStartingStack=1;
					modifiedEndingStack=EndingStack+1;
				}
				else
				{
					modifiedStartingStack=StartingStack;

					if(EndingStack==Stacks-1)
						modifiedEndingStack=EndingStack;
					else
						modifiedEndingStack=EndingStack+1;
				}

				for(j=1; j<Stacks; j++)
				{
					if((StartingStack<=j)&&(j<=modifiedEndingStack))
					{
						if(invertS==FALSE)
							FTempo=0.0;
						else
							FTempo=1.0;

						for(k=0; k<Slices; k++, i++)
						{
							pVertex_VNT[i].s0=FTempo;
							pVertex_VNT[i].t0=FTempo2;
							FTempo+=texIncrementS;
						}

						//Back to initial position
						if(invertS==FALSE)
							pVertex_VNT[i].s0=1.0;
						else
							pVertex_VNT[i].s0=0.0;

						pVertex_VNT[i].t0=FTempo2;
						i++;
					}

					FTempo2+=texIncrementT;
				}

				if(EndingStack==Stacks-1)
				{
					pVertex_VNT[i].s0=0.5;
					if(invertT==FALSE)
						pVertex_VNT[i].t0=1.0;
					else
						pVertex_VNT[i].t0=0.0;
				}
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			//Top of the sphere (triangle_fan)
			//The center-bottom of the texture applied to top of sphere
			i=0;
			pVertex_VNT3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNT3[i].t0=0.0;
			else
				pVertex_VNT3[i].t0=1.0;

			pVertex_VNT3[i].r0=0.0;			//Just set to 0.0

			i++;
			FTempo=0.0;
			if(invertT==FALSE)
				FTempo2=texIncrementT;
			else
			{
				FTempo2=1.0-texIncrementT;
				texIncrementT=-texIncrementT;
			}

			if(invertS==TRUE)
				texIncrementS=-texIncrementS;

			for(j=1; j<Stacks; j++)
			{
				if(invertS==FALSE)
					FTempo=0.0;
				else
					FTempo=1.0;

				for(k=0; k<Slices; k++, i++)
				{
					pVertex_VNT3[i].s0=FTempo;
					pVertex_VNT3[i].t0=FTempo2;
					pVertex_VNT3[i].r0=0.0;			//Just set to 0.0
					FTempo+=texIncrementS;
				}

				//Back to initial position
				if(invertS==FALSE)
					pVertex_VNT3[i].s0=1.0;
				else
					pVertex_VNT3[i].s0=0.0;

				pVertex_VNT3[i].t0=FTempo2;
				pVertex_VNT3[i].r0=0.0;			//Just set to 0.0
				i++;
				FTempo2+=texIncrementT;
			}

			pVertex_VNT3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNT3[i].t0=1.0;
			else
				pVertex_VNT3[i].t0=0.0;

			pVertex_VNT3[i].r0=0.0;			//Just set to 0.0

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			//Top of the sphere (triangle_fan)
			//The center-bottom of the texture applied to top of sphere
			i=0;
			pVertex_VNTT3T3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNTT3T3[i].t0=0.0;
			else
				pVertex_VNTT3T3[i].t0=1.0;

			i++;
			FTempo=0.0;
			if(invertT==FALSE)
				FTempo2=texIncrementT;
			else
			{
				FTempo2=1.0-texIncrementT;
				texIncrementT=-texIncrementT;
			}

			if(invertS==TRUE)
				texIncrementS=-texIncrementS;

			for(j=1; j<Stacks; j++)
			{
				if(invertS==FALSE)
					FTempo=0.0;
				else
					FTempo=1.0;

				for(k=0; k<Slices; k++, i++)
				{
					pVertex_VNTT3T3[i].s0=FTempo;
					pVertex_VNTT3T3[i].t0=FTempo2;
					FTempo+=texIncrementS;
				}

				//Back to initial position
				if(invertS==FALSE)
					pVertex_VNTT3T3[i].s0=1.0;
				else
					pVertex_VNTT3T3[i].s0=0.0;

				pVertex_VNTT3T3[i].t0=FTempo2;
				i++;
				FTempo2+=texIncrementT;
			}

			pVertex_VNTT3T3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNTT3T3[i].t0=1.0;
			else
				pVertex_VNTT3T3[i].t0=0.0;

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			//Top of the sphere (triangle_fan)
			//The center-bottom of the texture applied to top of sphere
			i=0;
			pVertex_VNTT3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNTT3[i].t0=0.0;
			else
				pVertex_VNTT3[i].t0=1.0;

			i++;
			FTempo=0.0;
			if(invertT==FALSE)
				FTempo2=texIncrementT;
			else
			{
				FTempo2=1.0-texIncrementT;
				texIncrementT=-texIncrementT;
			}

			if(invertS==TRUE)
				texIncrementS=-texIncrementS;

			for(j=1; j<Stacks; j++)
			{
				if(invertS==FALSE)
					FTempo=0.0;
				else
					FTempo=1.0;

				for(k=0; k<Slices; k++, i++)
				{
					pVertex_VNTT3[i].s0=FTempo;
					pVertex_VNTT3[i].t0=FTempo2;
					FTempo+=texIncrementS;
				}

				//Back to initial position
				if(invertS==FALSE)
					pVertex_VNTT3[i].s0=1.0;
				else
					pVertex_VNTT3[i].s0=0.0;

				pVertex_VNTT3[i].t0=FTempo2;
				i++;
				FTempo2+=texIncrementT;
			}

			pVertex_VNTT3[i].s0=0.5;
			if(invertT==FALSE)
				pVertex_VNTT3[i].t0=1.0;
			else
				pVertex_VNTT3[i].t0=0.0;

			return 1;
		}
	}

	return -1;
}

//PURPOSE:	As powerful as StartupByABCStacksAndSlices, but we added startingStack and endingStack.
//			startingStack is the first stack and endingStack is the last one to be included.
//			Example : If stacks = 10 and 
//			startingStack = 0
//			endingStack = 2
//
//			Stacks are numbered 0 to 9 in this case.
//			We will generate stack #0, 1 and 2
//
//PARAMETERS:
//			[in] indexFormat : one of the defines in glhlib.h
//			[in] vertexFormat : one of the defines in glhlib.h
//			                    GLH_VERTEXFORMAT_VNTT3T3, T3 receives tangent and the other T3 receives binormal
//			[in] radius : >0.0, else set to 1.0
//			[in] stacks : the number of layer about the z-axis. >=3
//			[in] slices : the number of slices about the z-axis. >=3
//			[in] texCoordStyle : 0 for nothing to be written
//			                     1 for standard texture mapping
//			[in] scaleFactorS : to scale the texcoords
//			[in] scaleFactorT : to scale the texcoords
//			[in] startingStack : must be 0 to stacks-1, the first value (stacks must be valid)
//			[in] endingStack : must be 0 to stacks-1, the second value (stacks must be valid)
//
//RETURN:
//			1 for success
//			0 for memory error.
//			-1 for array related array (DEBUG) remove it in the future.
sint TSphere::StartupByABCStacksAndSlices_StackBound2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT,
		sint startingStack, sint endingStack)
{
	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	if(startingStack<0)
		StartingStack=0;
	else if(startingStack>=Stacks)
		StartingStack=Stacks-1;
	else
		StartingStack=startingStack;

	if(endingStack>=Stacks)
		EndingStack=Stacks-1;
	else if(endingStack<StartingStack)
		EndingStack=StartingStack;
	else
		EndingStack=endingStack;

	PolygonNumber=0;								//Start value
													//Triangle count
	if(StartingStack==0)
		PolygonNumber+=Slices;

	if(EndingStack==Stacks-1)
		PolygonNumber+=Slices;

	if(StartingStack==0)
	{
		if(EndingStack==Stacks-1)
			PolygonNumber+=(EndingStack-1)*Slices*2;
		else
			PolygonNumber+=EndingStack*Slices*2;
	}
	else //if(StartingStack!=0)
	{
		if(EndingStack==Stacks-1)
			PolygonNumber+=(EndingStack-StartingStack)*Slices*2;		//Triangle count
		else
			PolygonNumber+=(EndingStack-StartingStack+1)*Slices*2;		//Triangle count
	}

	RadiusA=(aRadius<=0.0) ? 1.0 : aRadius;
	RadiusB=(bRadius<=0.0) ? 1.0 : bRadius;
	RadiusC=(cRadius<=0.0) ? 1.0 : cRadius;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 1990");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 1996");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 2001");
		return -1;
	}

	return SameStartupPatternForSphere2();
}


//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//


