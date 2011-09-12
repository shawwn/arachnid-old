// glhlib5.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
//#include <GL/gl.h>
//#include <GL/glext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include <stdlib.h>





#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'


////////////// GLOBALS VARIABLES////////////////////////
schar g_cbuffer256[256], g_cbuffer256_2[256];
schar g_cbuffer512[512];



inline sint IsWhiteChar(schar character)
{
	if((character==' ')||(character=='\t')||(character=='\n'))
		return 1;
	return 0;
}

//buffer should be large enough.
//Will copy until NULL char or white char is found
inline void CopyWordFromBuffer_v1(schar *ptext, sint &i, schar *buffer)
{
	sint j=0;
	for(; ptext[i]!=0; i++)
	{
		if(IsWhiteChar(ptext[i])==FALSE)
			buffer[j++]=ptext[i];
		else
			break;
	}
	buffer[j]=0;		//NULL terminate
	i--;				//i++ will happen later
}

//Copy until end of line or if "/*" or "//" or ';' is found
inline void CopyLineFromBuffer_v1(schar *ptext, sint &i, schar *buffer)
{
	sint j=0;
	for(; ptext[i]!=0; i++)
	{
		if(ptext[i]=='\n')
			break;
		if((ptext[i]=='/')&&(ptext[i+1]=='/'))
			break;
		if((ptext[i]=='/')&&(ptext[i+1]=='*'))
			break;
		if(ptext[i]==';')
			break;
		
		buffer[j++]=ptext[i];
	}
	buffer[j]=0;		//NULL terminate
	i--;				//i++ will happen later
}

//Called from TranslateInstructionFromBuffer_v1_ps to copy
//instruction, then operand 1, then operand 2 and then it puts a ';', then
//copies the rest.
inline void CopyPlainInstruction_v1(schar *cbuffer256, sint length, schar *cbufferOut256)
{
	sint i, j;
	for(i=j=0; i<length; i++)
	{	//In ARBfp, instructions are capitalized, in ps2.0, they are not
		if(IsWhiteChar(cbuffer256[i])==TRUE)
			break;
		cbufferOut256[j++]=(schar)toupper((sint)cbuffer256[i]);
	}
	for(; i<length; i++)
	{
		if(cbuffer256[i]==',')
		{
			cbufferOut256[j++]=cbuffer256[i++];
			break;
		}
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}

	for(; i<length; i++)
	{	//Last operand
		if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))	// '/' is the first comment char and ';'  is also used as comment in Direct3D asm shaders
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}

	cbufferOut256[j]=';';
	strcpy(&cbufferOut256[j+1], &cbuffer256[i]);
}

//Called from TranslateInstructionFromBuffer_v1_ps to copy
//instruction, then operand 1, then operand 2, then operand 3 and then it puts a ';', then
//copies the rest.
inline void CopyPlainInstruction_v2(schar *cbuffer256, sint length, schar *cbufferOut256)
{
	sint i, j;
	for(i=j=0; i<length; i++)
	{	//In ARBfp, instructions are capitalized, in ps2.0, they are not
		if(IsWhiteChar(cbuffer256[i])==TRUE)
			break;
		cbufferOut256[j++]=(schar)toupper((sint)cbuffer256[i]);
	}
	//Copy first operand
	for(; i<length; i++)
	{
		if(cbuffer256[i]==',')
		{
			cbufferOut256[j++]=cbuffer256[i++];
			break;
		}
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}
	//Copy second operand
	for(; i<length; i++)
	{
		if(cbuffer256[i]==',')
		{
			cbufferOut256[j++]=cbuffer256[i++];
			break;
		}
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}
	//Third operand
	for(; i<length; i++)
	{
		if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))	// '/' is the first comment char and ';'  is also used as comment in Direct3D asm shaders
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}

	cbufferOut256[j]=';';
	strcpy(&cbufferOut256[j+1], &cbuffer256[i]);
}

//Called from TranslateInstructionFromBuffer_v1_ps to copy
//instruction, then operand 1, then operand 2, then operand 3, then operand 4 and then it puts a ';', then
//copies the rest.
inline void CopyPlainInstruction_v3(schar *cbuffer256, sint length, schar *cbufferOut256)
{
	sint i, j;
	for(i=j=0; i<length; i++)
	{	//In ARBfp, instructions are capitalized, in ps2.0, they are not
		if(IsWhiteChar(cbuffer256[i])==TRUE)
			break;
		cbufferOut256[j++]=(schar)toupper((sint)cbuffer256[i]);
	}
	//Copy first operand
	for(; i<length; i++)
	{
		if(cbuffer256[i]==',')
		{
			cbufferOut256[j++]=cbuffer256[i++];
			break;
		}
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}
	//Copy second operand
	for(; i<length; i++)
	{
		if(cbuffer256[i]==',')
		{
			cbufferOut256[j++]=cbuffer256[i++];
			break;
		}
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}
	//Third operand
	for(; i<length; i++)
	{
		if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))	// '/' is the first comment char and ';'  is also used as comment in Direct3D asm shaders
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	for(; i<length; i++)
	{
		if(IsWhiteChar(cbuffer256[i])==FALSE)
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}
	//It's possible a '-' is present here. We also accept '+' but should not be present for PS2.0 syntax
	for(; i<length; i++)
	{
		if((cbuffer256[i]!='-')&&(cbuffer256[i]!='+'))
			break;
		cbufferOut256[j++]=cbuffer256[i];
		//Skip possible white spaces
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		break;
	}
	//Fourth operand
	for(; i<length; i++)
	{
		if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))	// '/' is the first comment char and ';'  is also used as comment in Direct3D asm shaders
			break;
		cbufferOut256[j++]=cbuffer256[i];
	}

	cbufferOut256[j]=';';
	strcpy(&cbufferOut256[j+1], &cbuffer256[i]);
}



//The line starts with possibly an instruction. Try to identify and convert from D3D to GL (ps2.0 to ARBfp1.0).
//Take care of "middle" part and end with a ';'
//Write any white chars if any are at the end of the line.
inline void TranslateInstructionFromBuffer_v1_ps(schar *cbuffer256, schar *cbufferOut256)
{
	sint i, j, k, count;
	sint length=strlen(cbuffer256);
	if((CompareTwoStrings(cbuffer256, "ps.2.0")==1)||
		(CompareTwoStrings(cbuffer256, "ps_2_0")==1))
	{
		strcpy(cbufferOut256, "!!ARBfp1.0");
		strcat(cbufferOut256, &cbuffer256[6]);
	}
	else if(CompareTwoStrings(cbuffer256, "def")==1)
	{	//Should be in the form : def c0, 0, 0, 0, 0
		//Convert to PARAM c0={0, 0, 0, 0};
		strcpy(cbufferOut256, "PARAM");
		j=5;
		for(i=3; cbuffer256[i]!=','; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='=';
		cbufferOut256[j++]='{';
		i++;
		for(count=0; i<length; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
			{
				count++;
				if(count==3)
				{
					i++;
					break;
				}
			}
		}
		for(; i<length; i++)
		{
			if((cbuffer256[i]!=' ')&&(cbuffer256[i]!='\t'))
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='}';
		cbufferOut256[j++]=';';
		cbufferOut256[j++]=0;
		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "abs")==1)||
		(CompareTwoStrings(cbuffer256, "abs_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "add")==1)||
		(CompareTwoStrings(cbuffer256, "add_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "crs")==1)||
		(CompareTwoStrings(cbuffer256, "crs_sat")==1))
	{
		//Hack! Write xpd in places of crs
		cbuffer256[0]='x';
		cbuffer256[1]='p';
		cbuffer256[2]='d';
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "exp")==1)||
		(CompareTwoStrings(cbuffer256, "exp_sat")==1))
	{
		//Hack!
		cbuffer256[2]='2';
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "frc")==1)||
		(CompareTwoStrings(cbuffer256, "frc_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "log")==1)||
		(CompareTwoStrings(cbuffer256, "log_sat")==1))
	{
		//Hack
		cbuffer256[1]='g';
		cbuffer256[2]='2';
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "max")==1)||
		(CompareTwoStrings(cbuffer256, "max_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "min")==1)||
		(CompareTwoStrings(cbuffer256, "min_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov")==1)||
		(CompareTwoStrings(cbuffer256, "mov_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul")==1)||
		(CompareTwoStrings(cbuffer256, "mul_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "nrm")==1)||
		(CompareTwoStrings(cbuffer256, "nrm_sat")==1))
	{
		//This instruction is not available
		//Must do a DP3/RSQ/MUL
		//DP3	r1.w, r0, r0;
		//RSQ	r1.w, r0.w;
		//MUL	r1, r0, r1.w;
		//Form is nrm dst, src
		//WARNING : w of src will be overwritten!!!
		if(cbuffer256[3]=='_')
			strcpy(cbufferOut256, "#TRANSLATOR NOTICE : nrm_sat expanded to DP3/RSQ/MUL_SAT!!!\n");
		else
			strcpy(cbufferOut256, "#TRANSLATOR NOTICE : nrm expanded to DP3/RSQ/MUL!!!\n");

		//Copy dest and source registers
		if(cbuffer256[3]=='_')
			i=7;
		else
			i=3;
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		//Copy dest
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j]=0;
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
		}
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		//Copy source
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256_2[j++]=cbuffer256[i];
		}
		g_cbuffer256_2[j]=0;

		strcat(cbufferOut256, "DP3\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ";\nRSQ\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256);
		if(cbuffer256[3]=='_')
			strcat(cbufferOut256, ".w;\nMUL_SAT\t");
		else
			strcat(cbufferOut256, ".w;\nMUL\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ";");
		//Copy the rest of the line
		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "pow")==1)||
		(CompareTwoStrings(cbuffer256, "pow_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "rcp")==1)||
		(CompareTwoStrings(cbuffer256, "rcp_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "rsq")==1)||
		(CompareTwoStrings(cbuffer256, "rsq_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub")==1)||
		(CompareTwoStrings(cbuffer256, "sub_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "texkill")==1)
	{
		//texkill and KIL are not the same
		//texkill kills when any of the RGB components is less than 0
		//KIL kills when any of the RGBA components is less than 0
		strcpy(cbufferOut256, "#TRANSLATOR WARNING :\n");
		strcat(cbufferOut256, "#texkill != KIL\nKIL");
		j=strlen(cbufferOut256);
		for(i=7; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]=';';
		strcpy(&cbufferOut256[j], &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "cmp")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_sat")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		for(i=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[i]=cbuffer256[i];
		}
		g_cbuffer256[i]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[i]=cbuffer256[i];
		}
		g_cbuffer256[i]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		k=i;
		for(j=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[j++]=cbuffer256[i];
		}
		g_cbuffer256_2[j]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[k++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[k++]=cbuffer256[i];
		}
		g_cbuffer256[k++]=',';
		for(j=0; g_cbuffer256_2[j]!=0; j++)
		{
			g_cbuffer256[k++]=g_cbuffer256_2[j];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[k++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_sat")==1))
	{
		CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad")==1)||
		(CompareTwoStrings(cbuffer256, "mad_sat")==1))
	{
		CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "m3x2")==1)
	{
		//Macro instruction :
		//m3x2   r0.xy, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x2 expanded improperly\n");
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[4]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
	}
	else if(CompareTwoStrings(cbuffer256, "m3x3")==1)
	{
		//Macro instruction :
		//m3x3   r0.xyz, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1
		//dp3   r0.z, r1, c2
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x3 expanded improperly\n");
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[4]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
	}
	else if(CompareTwoStrings(cbuffer256, "m3x4")==1)
	{
		//Macro instruction :
		//m3x4   r0.xyzw, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1
		//dp3   r0.z, r1, c2
		//dp3   r0.w, r1, c3
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x4 expanded improperly\n");
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[4]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
	}
	else if(CompareTwoStrings(cbuffer256, "m4x3")==1)
	{
		//Macro instruction :
		//m4x3   r0.xyz, r1, c0 is expanded to
		//dp4   r0.x, r1, c0
		//dp4   r0.y, r1, c1
		//dp4   r0.z, r1, c2
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m4x3 expanded improperly\n");
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[4]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
	}
	else if(CompareTwoStrings(cbuffer256, "m4x4")==1)
	{
		//Macro instruction :
		//m4x4   r0.xyzw, r1, c0 is expanded to
		//dp4   r0.x, r1, c0
		//dp4   r0.y, r1, c1
		//dp4   r0.z, r1, c2
		//dp4   r0.w, r1, c3
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m4x4 expanded improperly\n");
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[4]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
		strcat(cbufferOut256, "\n");				//Add new line
		strcat(cbufferOut256, g_cbuffer256_2);		//Copy again
	}
	else if((CompareTwoStrings(cbuffer256, "sincos")==1)||
		(CompareTwoStrings(cbuffer256, "sincos_sat")==1))
	{
		if(CompareTwoStrings(cbuffer256, "sincos_sat")==1)
		{
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : sincos has been split\n");
			strcpy(g_cbuffer256, "COS_SAT");			//This is correct
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);		//COS_SAT
			strcat(cbufferOut256, "\n");				//Add new line
			strcpy(g_cbuffer256, "SIN_SAT");			//This is correct
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);		//SIN_SAT
		}
		else
		{
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : sincos has been split\n");
			strcpy(g_cbuffer256, "COS");
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);	//COS
			strcat(cbufferOut256, "\n");				//Add new line
			strcpy(g_cbuffer256, "SIN");
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);	//SIN
		}
	}
	else if((CompareTwoStrings(cbuffer256, "dcl_2d")==1)||
		(CompareTwoStrings(cbuffer256, "dcl_cube")==1)||
		(CompareTwoStrings(cbuffer256, "dcl_volume")==1))
	{
		cbufferOut256[0]='#';
		//Just copy
		strcpy(&cbufferOut256[1], cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "dcl")==1)
	{
		//dcl t0 - tX is converted to texcoord
		//dcl v0 - v1 is converted to color.primary and color.secondary
		//NOTE: The entire text will need to be scanned and t0 and v0 and the other
		//ps2.0 registers need to be redone.
		//Uncomment these
		cbufferOut256[0]='#';
		//Just copy
		strcpy(&cbufferOut256[1], cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texldb")==1)
	{
		strcpy(g_cbuffer256, "TXB");
		strcpy(&g_cbuffer256[3], &cbuffer256[6]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "texldp")==1)
	{
		strcpy(g_cbuffer256, "TXP");
		strcpy(&g_cbuffer256[3], &cbuffer256[6]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "texld")==1)
	{
		strcpy(g_cbuffer256, "TEX");
		strcpy(&g_cbuffer256[3], &cbuffer256[5]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "dp2add")==1)
	{
		if(CompareTwoStrings(cbuffer256, "dp2add_sat")==1)
		{	//This is x1*x2 + y1*y2 + selected_component
			//example : dp2add r0, t0, t1, c1.w

			//The equivalent doesn't exist for GL so we do it with multiple instructions, and we must use
			//a temp buffer which I will call "r555"
			//MUL  r555, t0.x, t1.x;
			//MAD  r555, t0.y, t1.y, r555;
			//ADD_SAT  r0, r555, c1.w;
			strcpy(g_cbuffer512, "#TRANSLATOR WARNING : dp2add translated into MUL, MAD, ADD sequence\n");
			schar first[64];		//Copy r0 here
			schar second[64];	//Copy t0 here
			schar third[64];		//Copy t1 here
			schar fourth[64];	//Copy c1.w here
			schar spaceBetweenInstructionAndFirst[64];
			for(i=0; i<length; i++)
			{
				if(IsWhiteChar(cbuffer256[i])==TRUE)
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if(IsWhiteChar(cbuffer256[i])==FALSE)
					break;
				spaceBetweenInstructionAndFirst[j]=cbuffer256[i];
			}
			spaceBetweenInstructionAndFirst[j]=0;
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				first[j]=cbuffer256[i];
			}
			first[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				second[j]=cbuffer256[i];
			}
			second[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				third[j]=cbuffer256[i];
			}
			third[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
					break;
				fourth[j]=cbuffer256[i];
			}
			fourth[j]=0;
			//MUL  r555, t0.x, t1.x;
			strcat(g_cbuffer512, "MUL");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, "r555, ");
			strcat(g_cbuffer512, second);
			strcat(g_cbuffer512, ".x, ");
			strcat(g_cbuffer512, third);
			strcat(g_cbuffer512, ".x;\n");
			//MAD  r555, t0.y, t1.y, r555;
			strcat(g_cbuffer512, "MAD");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, "r555, ");
			strcat(g_cbuffer512, second);
			strcat(g_cbuffer512, ".y, ");
			strcat(g_cbuffer512, third);
			strcat(g_cbuffer512, ".y, r555;\n");
			//ADD_SAT  r0, r555, c1.w;
			strcat(g_cbuffer512, "ADD_SAT");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, first);
			strcat(g_cbuffer512, ", r555, ");
			strcat(g_cbuffer512, fourth);
			strcat(g_cbuffer512, ";");
			strcat(g_cbuffer512, &cbuffer256[i]);
			//Copy to output
			strcpy(cbufferOut256, g_cbuffer512);
		}
		else //if(CompareTwoStrings(cbuffer256, "dp2add")==1)
		{	//This is x1*x2 + y1*y2 + selected_component
			//example : dp2add r0, t0, t1, c1.w

			//The equivalent doesn't exist for GL so we do it with multiple instructions, and we must use
			//a temp buffer which I will call "r555"
			//MUL  r555, t0.x, t1.x;
			//MAD  r555, t0.y, t1.y, r555;
			//ADD  r0, r555, c1.w;
			strcpy(g_cbuffer512, "#TRANSLATOR WARNING : dp2add translated into MUL, MAD, ADD sequence\n");
			schar first[64];		//Copy r0 here
			schar second[64];	//Copy t0 here
			schar third[64];		//Copy t1 here
			schar fourth[64];	//Copy c1.w here
			schar spaceBetweenInstructionAndFirst[64];
			for(i=0; i<length; i++)
			{
				if(IsWhiteChar(cbuffer256[i])==TRUE)
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if(IsWhiteChar(cbuffer256[i])==FALSE)
					break;
				spaceBetweenInstructionAndFirst[j]=cbuffer256[i];
			}
			spaceBetweenInstructionAndFirst[j]=0;
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				first[j]=cbuffer256[i];
			}
			first[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				second[j]=cbuffer256[i];
			}
			second[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
					break;
				third[j]=cbuffer256[i];
			}
			third[j]=0;
			for(i++; i<length; i++)
			{
				if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
					break;
			}
			for(j=0; i<length; i++, j++)
			{
				if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
					break;
				fourth[j]=cbuffer256[i];
			}
			fourth[j]=0;
			//MUL  r555, t0.x, t1.x;
			strcat(g_cbuffer512, "MUL");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, "r555, ");
			strcat(g_cbuffer512, second);
			strcat(g_cbuffer512, ".x, ");
			strcat(g_cbuffer512, third);
			strcat(g_cbuffer512, ".x;\n");
			//MAD  r555, t0.y, t1.y, r555;
			strcat(g_cbuffer512, "MAD");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, "r555, ");
			strcat(g_cbuffer512, second);
			strcat(g_cbuffer512, ".y, ");
			strcat(g_cbuffer512, third);
			strcat(g_cbuffer512, ".y, r555;\n");
			//ADD  r0, r555, c1.w;
			strcat(g_cbuffer512, "ADD");
			strcat(g_cbuffer512, spaceBetweenInstructionAndFirst);
			strcat(g_cbuffer512, first);
			strcat(g_cbuffer512, ", r555, ");
			strcat(g_cbuffer512, fourth);
			strcat(g_cbuffer512, ";");
			strcat(g_cbuffer512, &cbuffer256[i]);
			//Copy to output
			strcpy(cbufferOut256, g_cbuffer512);
		}
	}
	else
	{	//Just copy since I couldn't understand it :
		//example : there is no nop instruction in ARBfp1.0 (no operation)
		//example : there is no dp2add and the instruction is too complex to translate
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : could not translate\n");
		strcat(cbufferOut256, cbuffer256);
	}
}

//The line starts with possibly an instruction.
//Try to identify and convert from D3D to GL (ps1.X to ARBfp1.0).
//The ps1.X have enough in common, one adds more capability (sometimes removing)
//so we can have a single function handle the translating to ARBfp1.0
//Take care of "middle" part and end with a ';'
//Write any white chars if any are at the end of the line.
inline void TranslateInstructionFromBuffer_v2(schar *cbuffer256, schar *cbufferOut256)
{
	//In ps 1.4, some instruction can have modifiers and we must
	//translate them in ARbfp1.0, like :
	//_x2 : mult by 2 before writing to destination
	//_x4 : mult by 4 before writing to destination
	//_x8 : mult by 8 before writing to destination
	//_d2 : div by 2 before writing to destination (mult by 0.5 in ARBfp1.0)
	//_d4 : div by 4 before writing to destination (mult by 0.25 in ARBfp1.0)
	//_d8 : div by 8 before writing to destination (mult by 0.125 in ARBfp1.0)
	//_sat : No big deal, ARBfp1.0 has this
	//
	//It is possible to combine 2 instruction modifiers
	//add_x2_sat	dst, src0, src1
	//(but I think add_sat_x2 is illegal)
	//The above example would become in ARBfp1.0 :
	//ADD		dst, src0, src1;
	//MUL_SAT	dst, dst, 2.0;			<--- mult by 2 then clamp result and write to register
	sint i, j, k, count;
	sint length=strlen(cbuffer256);
	if((CompareTwoStrings(cbuffer256, "ps.1.4")==1)||
		(CompareTwoStrings(cbuffer256, "ps_1_4")==1)||
		(CompareTwoStrings(cbuffer256, "ps.1.3")==1)||
		(CompareTwoStrings(cbuffer256, "ps_1_3")==1)||
		(CompareTwoStrings(cbuffer256, "ps.1.2")==1)||
		(CompareTwoStrings(cbuffer256, "ps_1_2")==1)||
		(CompareTwoStrings(cbuffer256, "ps.1.1")==1)||
		(CompareTwoStrings(cbuffer256, "ps_1_1")==1)||
		(CompareTwoStrings(cbuffer256, "ps.1.0")==1)||
		(CompareTwoStrings(cbuffer256, "ps_1_0")==1))
	{
		strcpy(cbufferOut256, "!!ARBfp1.0");
		strcat(cbufferOut256, &cbuffer256[6]);
	}
	else if(CompareTwoStrings(cbuffer256, "def")==1)
	{	//Should be in the form : def c0, 0, 0, 0, 0
		//Convert to PARAM c0={0, 0, 0, 0};
		strcpy(cbufferOut256, "PARAM");
		j=5;
		for(i=3; cbuffer256[i]!=','; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='=';
		cbufferOut256[j++]='{';
		i++;
		for(count=0; i<length; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
			{
				count++;
				if(count==3)
				{
					i++;
					break;
				}
			}
		}
		for(; i<length; i++)
		{
			if((cbuffer256[i]!=' ')&&(cbuffer256[i]!='\t'))
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='}';
		cbufferOut256[j++]=';';
		cbufferOut256[j++]=0;
		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "phase")==1)
	{	//Comment this out
		cbufferOut256[0]='#';
		strcpy(&cbufferOut256[1], cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "add_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "add_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "add_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}

		strcpy(g_cbuffer256, "ADD");
		strcat(g_cbuffer256, &cbuffer256[10]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		//Put a new line and prepare to do mult by 2 and sat instruction
		strcat(cbufferOut256, "\n");
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "add_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x2")==1)||
		(CompareTwoStrings(cbuffer256, "add_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x4")==1)||
		(CompareTwoStrings(cbuffer256, "add_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+add_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}

		strcpy(g_cbuffer256, "ADD");
		strcat(g_cbuffer256, &cbuffer256[6]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		//Put a new line and prepare to do mult by 2 and sat instruction
		strcat(cbufferOut256, "\n");
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "add_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "add_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "add_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}

		//There is no DIV instruction in ARBfp1.0, so I will use MUL instead
		strcpy(g_cbuffer256, "ADD");
		strcat(g_cbuffer256, &cbuffer256[10]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		//Put a new line and prepare to do mult by 2 and sat instruction
		strcat(cbufferOut256, "\n");
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "add_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d2")==1)||
		(CompareTwoStrings(cbuffer256, "add_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d4")==1)||
		(CompareTwoStrings(cbuffer256, "add_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+add_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}

		//There is no DIV instruction in ARBfp1.0, so I will use MUL instead
		strcpy(g_cbuffer256, "ADD");
		strcat(g_cbuffer256, &cbuffer256[6]);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		//Put a new line and prepare to do mult by 2 and sat instruction
		strcat(cbufferOut256, "\n");
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "add")==1)||
		(CompareTwoStrings(cbuffer256, "add_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+add")==1)||
		(CompareTwoStrings(cbuffer256, "+add_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	/*else if(CompareTwoStrings(cbuffer256, "bem")==1)
	{	//Cannot be co-issued
		//Apply a bump environment map transform
		//Example : bem r0.xy, r1, r2
		//
		//FORGET IT, it involves D3DTSS_BUMPENVMAT00 and such which I don't know about
		//for now.
	}*/
	else if((CompareTwoStrings(cbuffer256, "cmp_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x8_sat")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "CMP");
		for(i=10, j=3; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		for(k=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[k++]=cbuffer256[i];
		}
		g_cbuffer256_2[k]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=',';
		for(k=0; g_cbuffer256_2[k]!=0; k++)
		{
			g_cbuffer256[j++]=g_cbuffer256_2[k];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "cmp_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x2")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x4")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_x8")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "CMP");
		for(i=6, j=3; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		for(k=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[k++]=cbuffer256[i];
		}
		g_cbuffer256_2[k]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=',';
		for(k=0; g_cbuffer256_2[k]!=0; k++)
		{
			g_cbuffer256[j++]=g_cbuffer256_2[k];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "cmp_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d8_sat")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "CMP");
		for(i=10, j=3; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		for(k=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[k++]=cbuffer256[i];
		}
		g_cbuffer256_2[k]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=',';
		for(k=0; g_cbuffer256_2[k]!=0; k++)
		{
			g_cbuffer256[j++]=g_cbuffer256_2[k];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "cmp_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d2")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d4")==1)||
		(CompareTwoStrings(cbuffer256, "cmp_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp_d8")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "CMP");
		for(i=6, j=3; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		for(k=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[k++]=cbuffer256[i];
		}
		g_cbuffer256_2[k]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j++]=',';
		for(k=0; g_cbuffer256_2[k]!=0; k++)
		{
			g_cbuffer256[j++]=g_cbuffer256_2[k];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "cmp")==1)||
		(CompareTwoStrings(cbuffer256, "+cmp")==1))
	{	//The order of operands must be changed.
		//Example: cmp  r0, r1, r2, r3
		// becomes CMP r0, r1, r3, r2;
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		for(i=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[i]=cbuffer256[i];
		}
		g_cbuffer256[i]=cbuffer256[i++];
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256[i]=cbuffer256[i];
		}
		g_cbuffer256[i]=cbuffer256[i++];
		//Copy src1 into secondary buffer (along with space char in the beginning)
		k=i;
		for(j=0; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
			g_cbuffer256_2[j++]=cbuffer256[i];
		}
		g_cbuffer256_2[j]=0;
		
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			g_cbuffer256[k++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[k++]=cbuffer256[i];
		}
		g_cbuffer256[k++]=',';
		for(j=0; g_cbuffer256_2[j]!=0; j++)
		{
			g_cbuffer256[k++]=g_cbuffer256_2[j];
		}

		for(; i<length; i++)		//Copy the white chars in the end (if any)
		{
			g_cbuffer256[k++]=cbuffer256[i];
		}

		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "cnd")==1)
	//Let's the compiler give a warning and don't translate
	else if((CompareTwoStrings(cbuffer256, "dp3_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_x8_sat")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3_x2")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_x4")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_x8")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it		
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_d8_sat")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3_d2")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_d4")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_d8")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP3");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_sat")==1))
	{	//Cannot be co-issued
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_x8_sat")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4_x2")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_x4")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_x8")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_d8_sat")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4_d2")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_d4")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_d8")==1))
	{	//Cannot be co-issued
		strcpy(g_cbuffer256, "DP4");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_sat")==1))
	{	//Cannot be co-issued
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "LRP");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x2")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x4")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "LRP");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "LRP");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d2")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d4")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "LRP");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "lrp")==1)||
		(CompareTwoStrings(cbuffer256, "lrp_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp")==1)||
		(CompareTwoStrings(cbuffer256, "+lrp_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mad_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mad_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MAD");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x2")==1)||
		(CompareTwoStrings(cbuffer256, "mad_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x4")==1)||
		(CompareTwoStrings(cbuffer256, "mad_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MAD");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mad_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mad_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MAD");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d2")==1)||
		(CompareTwoStrings(cbuffer256, "mad_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d4")==1)||
		(CompareTwoStrings(cbuffer256, "mad_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MAD");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mad")==1)||
		(CompareTwoStrings(cbuffer256, "mad_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mad")==1)||
		(CompareTwoStrings(cbuffer256, "+mad_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mov_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mov_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MOV");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x2")==1)||
		(CompareTwoStrings(cbuffer256, "mov_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x4")==1)||
		(CompareTwoStrings(cbuffer256, "mov_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MOV");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mov_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mov_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MOV");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d2")==1)||
		(CompareTwoStrings(cbuffer256, "mov_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d4")==1)||
		(CompareTwoStrings(cbuffer256, "mov_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MOV");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov")==1)||
		(CompareTwoStrings(cbuffer256, "+mov")==1)||
		(CompareTwoStrings(cbuffer256, "mov_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mov_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mul_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mul_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MUL");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x2")==1)||
		(CompareTwoStrings(cbuffer256, "mul_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x4")==1)||
		(CompareTwoStrings(cbuffer256, "mul_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MUL");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mul_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "mul_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MUL");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d2")==1)||
		(CompareTwoStrings(cbuffer256, "mul_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d4")==1)||
		(CompareTwoStrings(cbuffer256, "mul_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "MUL");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul")==1)||
		(CompareTwoStrings(cbuffer256, "+mul")==1)||
		(CompareTwoStrings(cbuffer256, "mul_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+mul_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "sub_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "sub_x8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "SUB");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub_x2")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x2")==1)||
		(CompareTwoStrings(cbuffer256, "sub_x4")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x4")==1)||
		(CompareTwoStrings(cbuffer256, "sub_x8")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_x8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "SUB");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 2;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 4;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 8;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d2_sat")==1)||
		(CompareTwoStrings(cbuffer256, "sub_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d4_sat")==1)||
		(CompareTwoStrings(cbuffer256, "sub_d8_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d8_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "SUB");
		strcat(g_cbuffer256, &cbuffer256[10]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL_SAT");
		j=strlen(g_cbuffer256);
		for(i=10; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=10; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub_d2")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d2")==1)||
		(CompareTwoStrings(cbuffer256, "sub_d4")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d4")==1)||
		(CompareTwoStrings(cbuffer256, "sub_d8")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_d8")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		strcpy(g_cbuffer256, "SUB");
		strcat(g_cbuffer256, &cbuffer256[6]);	//Copy all of it
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
		strcat(cbufferOut256, "\n");
		//On the next line, do the mult or div operation and saturate if needed
		strcpy(g_cbuffer256, "MUL");
		j=strlen(g_cbuffer256);
		for(i=6; i<length; i++)		//Copy up to destination
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Write destination as first source
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		if(cbuffer256[5]=='2')
			strcpy(&g_cbuffer256[j], " 0.5;");
		else if(cbuffer256[5]=='4')
			strcpy(&g_cbuffer256[j], " 0.25;");
		else if(cbuffer256[5]=='8')
			strcpy(&g_cbuffer256[j], " 0.125;");
		//Concat to output buffer
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub")==1)||
		(CompareTwoStrings(cbuffer256, "+sub")==1)||
		(CompareTwoStrings(cbuffer256, "sub_sat")==1)||
		(CompareTwoStrings(cbuffer256, "+sub_sat")==1))
	{
		//Remove the '+' if there is one
		if(cbuffer256[0]=='+')
		{
			for(i=1; i<=length; i++)
			{
				cbuffer256[i-1]=cbuffer256[i];
			}
			length--;
		}
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	//else if((CompareTwoStrings(cbuffer256, "cnd")==1)
	//{	//Requires user help
	//}
	else if(CompareTwoStrings(cbuffer256, "texcoord")==1)
	{	//This is a weird instruction
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texcoord is meaningless\n");
		cbufferOut256[0]='#';
		strcpy(&cbufferOut256[1], cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texcrd")==1)
	{	//This is just a MOV operation
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texcrd turned into MOV\n");
		strcpy(g_cbuffer256, "MOV");
		strcat(g_cbuffer256, &cbuffer256[6]);
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
	}
	else if(CompareTwoStrings(cbuffer256, "texdepth")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texdepth not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texdp3tex")==1)
	{	//This is a weird instruction. It does a kind of DP3 instruction
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texdp3tex is meaningless\n#Please review\n");
		strcat(cbufferOut256, "DP3");
		j=strlen(cbufferOut256);
		for(i=6; i<length; i++)		//Duplicate first register
		{
			cbufferOut256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Copy the rest
		{
			cbufferOut256[j++]=cbuffer256[i];
		}

		strcpy(g_cbuffer256, "#TRANSLATOR WARNING : Add a tex instruction\nTEX\t....");
		strcat(cbufferOut256, g_cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texdp3")==1)
	{	//This is a weird instruction. It does a kind of DP3 instruction
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texdp3 is meaningless\n#Please review\n");
		strcat(cbufferOut256, "DP3");
		j=strlen(cbufferOut256);
		for(i=6; i<length; i++)		//Duplicate first register
		{
			cbufferOut256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i=6; i<length; i++)		//Copy the rest
		{
			cbufferOut256[j++]=cbuffer256[i];
		}
	}
	else if(CompareTwoStrings(cbuffer256, "texkill")==1)
	{
		//texkill and KIL are not the same
		//texkill kills when any of the RGB components is less than 0
		//KIL kills when any of the RGBA components is less than 0
		strcpy(cbufferOut256, "#TRANSLATOR WARNING :\n");
		strcat(cbufferOut256, "#texkill != KIL\nKIL");
		j=strlen(cbufferOut256);
		for(i=7; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]=';';
		strcpy(&cbufferOut256[j], &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x2depth")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x2depth not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x2pad")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x2pad not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x2tex")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x2tex not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x3pad")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x3pad not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x3spec")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x3spec not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x3tex")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x3tex not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x3vspec")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x3vspec not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texm3x3")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texm3x3 not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texreg2ar")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texreg2ar not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texreg2gb")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texreg2gb not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texreg2rgb")==1)
	{	//This requires users help
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : texreg2rgb not translated\n");
		strcat(cbufferOut256, cbuffer256);
	}
	else if(CompareTwoStrings(cbuffer256, "texld")==1)
	{	//In ps1.x, texld works by copying rgba from the texture sample
		//texld		r0, t0			//t0 is the sample
		//To convert to ARBfp1.0
		//TEX		r0, t0, s0;		//<--- will be written to buffer
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : Actual sample type unknown (2D?)\n");
		strcpy(g_cbuffer256, "TEX");
		j=strlen(g_cbuffer256);
		for(i=5; i<length; i++)
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
				break;
		}
		for(i++; i<length; i++)		//Find the 't', I want the number following it
		{
			g_cbuffer256[j++]=cbuffer256[i];
			if(cbuffer256[i]=='t')
				break;
		}
		k=0;
		for(i++; i<length; i++)		//Copy the number for the sampler, and store it in a buffer
		{
			if((cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256[j++]=cbuffer256[i];
			g_cbuffer256_2[k++]=cbuffer256[i];
		}
		g_cbuffer256[j]=g_cbuffer256_2[k]=0;
		strcat(g_cbuffer256, ", s");				//Add sample
		strcat(g_cbuffer256, g_cbuffer256_2);
		CopyPlainInstruction_v2(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
		strcat(cbufferOut256, g_cbuffer256_2);
	}
	else if(CompareTwoStrings(cbuffer256, "tex")==1)
	{
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : This is a weird ps1.X instruction\nReformating as texld\nPlease review\n");
		strcpy(g_cbuffer256, "TEX\tr0,");
		strcat(g_cbuffer256, &cbuffer256[3]);
	}
	else
	{	//Just copy since I couldn't understand it :
		//example : there is no nop instruction in ARBfp1.0 (no operation)
		//example : there is no dp2add and the instruction is too complex to translate
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : could not translate\n");
		strcat(cbufferOut256, cbuffer256);
	}
}

//The line starts with possibly an instruction. Try to identify and convert from D3D to GL (vs2.0 to ARBvp1.0).
//Take care of "middle" part and end with a ';'
//Write any white chars if any are at the end of the line.
inline void TranslateInstructionFromBuffer_v1_vs(schar *cbuffer256, schar *cbufferOut256)
{
	sint i, j, count;
	sint length=strlen(cbuffer256);
	if((CompareTwoStrings(cbuffer256, "vs.1.1")==1)||
		(CompareTwoStrings(cbuffer256, "vs_1_1")==1)||
		(CompareTwoStrings(cbuffer256, "vs.2.0")==1)||
		(CompareTwoStrings(cbuffer256, "vs_2_0")==1))
	{	//Other vs versions don't exist and vs1.1 is a subset of vs2.0
		strcpy(cbufferOut256, "!!ARBvp1.0");
		strcat(cbufferOut256, &cbuffer256[6]);
	}
	else if(CompareTwoStrings(cbuffer256, "def")==1)
	{	//Should be in the form : def c0, 0, 0, 0, 0
		//Convert to PARAM c0={0, 0, 0, 0};

		//Could be defb (bool) or defi (integer) 
		if(CompareTwoStrings(cbuffer256, "defb")==1)
		{
			i=4;
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : defb upgraded to float version\n");
		}
		else if(CompareTwoStrings(cbuffer256, "defi")==1)
		{
			i=4;
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : defi upgraded to float version\n");
		}
		else
		{
			i=3;
			cbufferOut256[0]=0;
		}

		strcat(cbufferOut256, "PARAM");
		j=strlen(cbufferOut256);
		for(; cbuffer256[i]!=','; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='=';
		cbufferOut256[j++]='{';
		i++;
		for(count=0; i<length; i++)
		{
			cbufferOut256[j++]=cbuffer256[i];
			if(cbuffer256[i]==',')
			{
				count++;
				if(count==3)
				{
					i++;
					break;
				}
			}
		}
		for(; i<length; i++)
		{
			if((cbuffer256[i]!=' ')&&(cbuffer256[i]!='\t'))
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			cbufferOut256[j++]=cbuffer256[i];
		}
		cbufferOut256[j++]='}';
		cbufferOut256[j++]=';';
		cbufferOut256[j++]=0;
		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "abs")==1)||
		(CompareTwoStrings(cbuffer256, "abs_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "add")==1)||
		(CompareTwoStrings(cbuffer256, "add_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "callnz")==1)			//Not supported
	//else if(CompareTwoStrings(cbuffer256, "call")==1)			//Not supported
	else if((CompareTwoStrings(cbuffer256, "crs")==1)||
		(CompareTwoStrings(cbuffer256, "crs_sat")==1))
	{
		//Hack! Write xpd in places of crs
		cbuffer256[0]='x';
		cbuffer256[1]='p';
		cbuffer256[2]='d';
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp3")==1)||
		(CompareTwoStrings(cbuffer256, "dp3_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dp4")==1)||
		(CompareTwoStrings(cbuffer256, "dp4_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "dst")==1)||
		(CompareTwoStrings(cbuffer256, "dst_sat")==1))
	{
		//GL and D3D are identical
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "else")==1)			//Not supported
	//else if(CompareTwoStrings(cbuffer256, "endif")==1)			//Not supported
	//else if(CompareTwoStrings(cbuffer256, "endloop")==1)			//Not supported
	//else if(CompareTwoStrings(cbuffer256, "endrep")==1)			//Not supported
	else if((CompareTwoStrings(cbuffer256, "expp")==1)||
		(CompareTwoStrings(cbuffer256, "expp_sat")==1))
	{
		//Hack! expp is for partial precision (who cares)
		//Remove the extra 'p'
		g_cbuffer256[0]=cbuffer256[0];
		g_cbuffer256[1]=cbuffer256[1];
		g_cbuffer256[2]='2';
		for(i=4, j=3; i<=length; i++)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}
		CopyPlainInstruction_v1(cbuffer256, strlen(g_cbuffer256), g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "exp")==1)||
		(CompareTwoStrings(cbuffer256, "exp_sat")==1))
	{
		//Hack!
		cbuffer256[2]='2';
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "frc")==1)||
		(CompareTwoStrings(cbuffer256, "frc_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "if")==1)			//Not supported
	//else if(CompareTwoStrings(cbuffer256, "label")==1)		//Not supported
	else if(CompareTwoStrings(cbuffer256, "lit")==1)
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "logp")==1)||
		(CompareTwoStrings(cbuffer256, "logp_sat")==1))
	{
		//logp is for partial precision
		g_cbuffer256[0]=cbuffer256[0];
		g_cbuffer256[1]=cbuffer256[1];
		g_cbuffer256[2]=cbuffer256[2];
		for(i=4, j=3; i<=length; i++)
		{
			g_cbuffer256[j++]=cbuffer256[i];
		}
		CopyPlainInstruction_v1(cbuffer256, strlen(g_cbuffer256), g_cbuffer256);
	}
	else if((CompareTwoStrings(cbuffer256, "log")==1)||
		(CompareTwoStrings(cbuffer256, "log_sat")==1))
	{
		//Hack!
		cbuffer256[1]='g';
		cbuffer256[2]='2';
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "loop")==1)			//Not supported
	//else if((CompareTwoStrings(cbuffer256, "lrp")==1)||		//Not supported
	//	(CompareTwoStrings(cbuffer256, "lrp_sat")==1))
	//{
	//	CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	//}
	else if(CompareTwoStrings(cbuffer256, "m3x2")==1)
	{
		//Macro instruction :
		//m3x2   r0.xy, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1

		schar whiteSpaces[64];
		schar first[64];
		schar second[64];
		schar third[64];

		//Skip instruction
		for(i=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
		}
		//Copy white spaces
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			whiteSpaces[j++]=cbuffer256[i];
		}
		whiteSpaces[j]=0;
		//Copy destination register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			first[j++]=cbuffer256[i];
		}
		first[j]=0;
		for(j=0; first[j]; j++)
		{
			if(first[j]=='.')
			{
				first[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source0 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			second[j++]=cbuffer256[i];
		}
		second[j]=0;
		for(j=0; second[j]; j++)
		{
			if(second[j]=='.')
			{
				second[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source1 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			third[j++]=cbuffer256[i];
		}
		third[j]=0;
		for(j=0; third[j]; j++)
		{
			if(third[j]=='.')
			{
				third[j]=0;
				break;
			}
		}

		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x2 expanded improperly\n");
		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".x, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".y, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";");

		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "m3x3")==1)
	{
		//Macro instruction :
		//m3x3   r0.xyz, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1
		//dp3   r0.z, r1, c2

		schar whiteSpaces[64];
		schar first[64];
		schar second[64];
		schar third[64];

		//Skip instruction
		for(i=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
		}
		//Copy white spaces
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			whiteSpaces[j++]=cbuffer256[i];
		}
		whiteSpaces[j]=0;
		//Copy destination register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			first[j++]=cbuffer256[i];
		}
		first[j]=0;
		for(j=0; first[j]; j++)
		{
			if(first[j]=='.')
			{
				first[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source0 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			second[j++]=cbuffer256[i];
		}
		second[j]=0;
		for(j=0; second[j]; j++)
		{
			if(second[j]=='.')
			{
				second[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source1 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			third[j++]=cbuffer256[i];
		}
		third[j]=0;
		for(j=0; third[j]; j++)
		{
			if(third[j]=='.')
			{
				third[j]=0;
				break;
			}
		}

		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x3 expanded improperly\n");
		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".x, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".y, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".z, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";");

		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "m3x4")==1)
	{
		//Macro instruction :
		//m3x4   r0.xyzw, r1, c0 is expanded to
		//dp3   r0.x, r1, c0
		//dp3   r0.y, r1, c1
		//dp3   r0.z, r1, c2
		//dp3   r0.w, r1, c3

		schar whiteSpaces[64];
		schar first[64];
		schar second[64];
		schar third[64];

		//Skip instruction
		for(i=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
		}
		//Copy white spaces
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			whiteSpaces[j++]=cbuffer256[i];
		}
		whiteSpaces[j]=0;
		//Copy destination register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			first[j++]=cbuffer256[i];
		}
		first[j]=0;
		for(j=0; first[j]; j++)
		{
			if(first[j]=='.')
			{
				first[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source0 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			second[j++]=cbuffer256[i];
		}
		second[j]=0;
		for(j=0; second[j]; j++)
		{
			if(second[j]=='.')
			{
				second[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source1 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			third[j++]=cbuffer256[i];
		}
		third[j]=0;
		for(j=0; third[j]; j++)
		{
			if(third[j]=='.')
			{
				third[j]=0;
				break;
			}
		}

		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m3x4 expanded improperly\n");
		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".x, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".y, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".z, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP3");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";");

		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "m4x3")==1)
	{
		//Macro instruction :
		//m4x3   r0.xyz, r1, c0 is expanded to
		//dp4   r0.x, r1, c0
		//dp4   r0.y, r1, c1
		//dp4   r0.z, r1, c2

		schar whiteSpaces[64];
		schar first[64];
		schar second[64];
		schar third[64];

		//Skip instruction
		for(i=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
		}
		//Copy white spaces
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			whiteSpaces[j++]=cbuffer256[i];
		}
		whiteSpaces[j]=0;
		//Copy destination register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			first[j++]=cbuffer256[i];
		}
		first[j]=0;
		for(j=0; first[j]; j++)
		{
			if(first[j]=='.')
			{
				first[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source0 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			second[j++]=cbuffer256[i];
		}
		second[j]=0;
		for(j=0; second[j]; j++)
		{
			if(second[j]=='.')
			{
				second[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source1 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			third[j++]=cbuffer256[i];
		}
		third[j]=0;
		for(j=0; third[j]; j++)
		{
			if(third[j]=='.')
			{
				third[j]=0;
				break;
			}
		}

		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m4x3 expanded improperly\n");
		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".x, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".y, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".z, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";");

		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if(CompareTwoStrings(cbuffer256, "m4x4")==1)
	{
		//Macro instruction :
		//m4x4   r0.xyzw, r1, c0 is expanded to
		//dp4   r0.x, r1, c0
		//dp4   r0.y, r1, c1
		//dp4   r0.z, r1, c2
		//dp4   r0.w, r1, c3
		
		schar whiteSpaces[64];
		schar first[64];
		schar second[64];
		schar third[64];

		//Skip instruction
		for(i=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
		}
		//Copy white spaces
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
			whiteSpaces[j++]=cbuffer256[i];
		}
		whiteSpaces[j]=0;
		//Copy destination register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			first[j++]=cbuffer256[i];
		}
		first[j]=0;
		for(j=0; first[j]; j++)
		{
			if(first[j]=='.')
			{
				first[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source0 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			second[j++]=cbuffer256[i];
		}
		second[j]=0;
		for(j=0; second[j]; j++)
		{
			if(second[j]=='.')
			{
				second[j]=0;
				break;
			}
		}
		//Skip whitespaces and comma
		for(; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==FALSE)&&(cbuffer256[i]!=','))
				break;
		}
		//Copy source1 register, then remove everything after the '.' if there is one
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==',')||(cbuffer256[i]=='/')||(cbuffer256[i]==';'))
				break;
			third[j++]=cbuffer256[i];
		}
		third[j]=0;
		for(j=0; third[j]; j++)
		{
			if(third[j]=='.')
			{
				third[j]=0;
				break;
			}
		}

		strcpy(cbufferOut256, "#TRANSLATOR WARNING : m4x4 expanded improperly\n");
		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".x, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".y, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".z, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";\n");

		strcat(cbufferOut256, "DP4");
		strcat(cbufferOut256, whiteSpaces);
		strcat(cbufferOut256, first);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, second);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, third);
		strcat(cbufferOut256, ";");

		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "mad")==1)||
		(CompareTwoStrings(cbuffer256, "mad_sat")==1))
	{
		CopyPlainInstruction_v3(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "max")==1)||
		(CompareTwoStrings(cbuffer256, "max_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "min")==1)||
		(CompareTwoStrings(cbuffer256, "min_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mova")==1)||
		(CompareTwoStrings(cbuffer256, "mova_sat")==1))
	{
		//This is converted to the special instruction : ARL
		g_cbuffer256[0]='A';
		g_cbuffer256[1]='R';
		g_cbuffer256[2]='L';
		strcpy(&g_cbuffer256[3], &cbuffer256[4]);
		CopyPlainInstruction_v1(g_cbuffer256, strlen(g_cbuffer256), cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mov")==1)||
		(CompareTwoStrings(cbuffer256, "mov_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "mul")==1)||
		(CompareTwoStrings(cbuffer256, "mul_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "nrm")==1)||
		(CompareTwoStrings(cbuffer256, "nrm_sat")==1))
	{
		//This instruction is not available
		//Must do a DP3/RSQ/MUL
		//DP3	r1.w, r0, r0;
		//RSQ	r1.w, r0.w;
		//MUL	r1, r0, r1.w;
		//Form is nrm dst, src
		//WARNING : w of src will be overwritten!!!
		if(cbuffer256[3]=='_')
			strcpy(cbufferOut256, "#TRANSLATOR NOTICE : nrm_sat expanded to DP3/RSQ/MUL_SAT!!!\n");
		else
			strcpy(cbufferOut256, "#TRANSLATOR NOTICE : nrm expanded to DP3/RSQ/MUL!!!\n");

		//Copy dest and source registers
		if(cbuffer256[3]=='_')
			i=7;
		else
			i=3;
		for(; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		//Copy dest
		for(j=0; i<length; i++)
		{
			if((IsWhiteChar(cbuffer256[i])==TRUE)||(cbuffer256[i]==','))
				break;
			g_cbuffer256[j++]=cbuffer256[i];
		}
		g_cbuffer256[j]=0;
		for(; i<length; i++)
		{
			if(cbuffer256[i]==',')
				break;
		}
		for(i++; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==FALSE)
				break;
		}
		//Copy source
		for(j=0; i<length; i++)
		{
			if(IsWhiteChar(cbuffer256[i])==TRUE)
				break;
			g_cbuffer256_2[j++]=cbuffer256[i];
		}
		g_cbuffer256_2[j]=0;

		strcat(cbufferOut256, "DP3\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ";\nRSQ\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256);
		if(cbuffer256[3]=='_')
			strcat(cbufferOut256, ".w;\nMUL_SAT\t");
		else
			strcat(cbufferOut256, ".w;\nMUL\t");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ", ");
		strcat(cbufferOut256, g_cbuffer256);
		strcat(cbufferOut256, ".w, ");
		strcat(cbufferOut256, g_cbuffer256_2);
		strcat(cbufferOut256, ";");
		//Copy the rest of the line
		strcat(cbufferOut256, &cbuffer256[i]);
	}
	else if((CompareTwoStrings(cbuffer256, "pow")==1)||
		(CompareTwoStrings(cbuffer256, "pow_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "rep")==1)		//Not supported
	//else if(CompareTwoStrings(cbuffer256, "ret")==1)		//Not supported
	else if((CompareTwoStrings(cbuffer256, "rcp")==1)||
		(CompareTwoStrings(cbuffer256, "rcp_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "rsq")==1)||
		(CompareTwoStrings(cbuffer256, "rsq_sat")==1))
	{
		CopyPlainInstruction_v1(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "sge")==1)||
		(CompareTwoStrings(cbuffer256, "sge_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	//else if(CompareTwoStrings(cbuffer256, "sgn")==1)		//Not supported
	else if((CompareTwoStrings(cbuffer256, "sincos")==1)||
		(CompareTwoStrings(cbuffer256, "sincos_sat")==1))
	{
		if(CompareTwoStrings(cbuffer256, "sincos_sat")==1)
		{
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : sincos has been split\n");
			strcpy(g_cbuffer256, "COS_SAT");			//This is correct
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);		//COS_SAT
			strcat(cbufferOut256, "\n");				//Add new line
			strcpy(g_cbuffer256, "SIN_SAT");			//This is correct
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);		//SIN_SAT
		}
		else
		{
			strcpy(cbufferOut256, "#TRANSLATOR WARNING : sincos has been split\n");
			strcpy(g_cbuffer256, "COS");
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);	//COS
			strcat(cbufferOut256, "\n");				//Add new line
			strcpy(g_cbuffer256, "SIN");
			strcat(g_cbuffer256, &cbuffer256[6]);
			CopyPlainInstruction_v3(g_cbuffer256, strlen(g_cbuffer256), g_cbuffer256_2);
			strcat(cbufferOut256, g_cbuffer256_2);	//SIN
		}
	}
	else if((CompareTwoStrings(cbuffer256, "slt")==1)||
		(CompareTwoStrings(cbuffer256, "slt_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if((CompareTwoStrings(cbuffer256, "sub")==1)||
		(CompareTwoStrings(cbuffer256, "sub_sat")==1))
	{
		CopyPlainInstruction_v2(cbuffer256, length, cbufferOut256);
	}
	else if(CompareTwoStrings(cbuffer256, "dcl")==1)
	{
		//vs2.0 registers need to be redone.
		//Uncomment these
		cbufferOut256[0]='#';
		//Just copy
		strcpy(&cbufferOut256[1], cbuffer256);
	}
	else
	{	//Just copy since I couldn't understand it :
		//example : there is no nop instruction in ARBfp1.0 (no operation)
		//example : there is no dp2add and the instruction is too complex to translate
		strcpy(cbufferOut256, "#TRANSLATOR WARNING : could not translate\n");
		strcat(cbufferOut256, cbuffer256);
	}
}

//Called from glhTranslateShader to repair the instruction in cbufferIn.
//cbufferIn may contain commented lines, multiple instructions. The lines should not start with white chars.
//cbufferOut should possibly be 512 bytes sint.
void FixRegisterModifiers_PS_1_4(schar *cbufferIn, schar *cbufferOut)
{
	cbufferOut[0]=0;		//Init

	//Scan cbufferIn for each line
	//Pick up instruction, spaces, and arguments
	//If arguments are using register modifiers like
	//bias = register_bias  (register-0.5)
	//invert = 1-register
	//negate = -register (directly supported by ARBfp1.0)
	//scale by 2 = register_x2
	//signed scaling = register_bx2 (2*(register-0.5))
	//there are a few others for PS1.4 but I think they are rarely used

	sint i, j, k, m, length, argCount;
	schar instruction[64], whiteSpaces[64];
	schar arg1[64], arg2[64], arg3[64], arg4[64], restOfLine[256];
	schar arg2b[64], arg3b[64], arg4b[64];
	schar ctemp[64];
	bool found, found2, found3;

	length=strlen(cbufferIn);

	for(i=j=0; i<length; i++)
	{
		if(cbufferIn[i]=='#')	//This is a ARBfp1.0 comment
		{
			cbufferOut[j++]=cbufferIn[i++];
			//Copy until end of line
			for(; i<=length; i++, j++)
			{
				cbufferOut[j]=cbufferIn[i];
				if(cbufferIn[i]=='\n')
					break;
			}
			cbufferOut[j+1]=0;
		}
		else	//The line contains an instruction, process the arguments
		{
			for(k=0; i<length; i++, k++)
			{
				if(IsWhiteChar(cbufferIn[i])==TRUE)
					break;
				instruction[k]=cbufferIn[i];
			}
			instruction[k]=0;

			if(strlen(instruction)==0)
			{
				continue;
			}
			//Compare string in instruction to all known strings in the ARBfp1.0 language
			//because at this point, the source should be partially translated already
			else if((CompareTwoStrings(instruction, "!!ARBfp1.0")==1)||		//These are not really instructions!!!
				(CompareTwoStrings(instruction, "ALIAS")==1)||
				(CompareTwoStrings(instruction, "PARAM")==1)||
				(CompareTwoStrings(instruction, "ATTRIBUTE")==1)||
				(CompareTwoStrings(instruction, "TEMP")==1)||
				(CompareTwoStrings(instruction, "OPTION")==1)||
				(CompareTwoStrings(instruction, "END")==1))
			{
				strcat(cbufferOut, instruction);
				k=strlen(cbufferOut);
				for(; i<length; i++)	//Copy until end of line
				{
					if(cbufferIn[i]=='\n')
					{
						cbufferOut[k++]=cbufferIn[i];
						break;
					}
					cbufferOut[k++]=cbufferIn[i];
				}
				cbufferOut[k]=0;
				continue;
			}


			for(k=0; i<length; i++, k++)
			{
				if(IsWhiteChar(cbufferIn[i])==FALSE)
					break;
				whiteSpaces[k]=cbufferIn[i];
			}
			whiteSpaces[k]=0;
			for(k=0; i<length; i++, k++)
			{
				if((IsWhiteChar(cbufferIn[i])==TRUE)||(cbufferIn[i]==','))
					break;
				arg1[k]=cbufferIn[i];
			}
			arg1[k]=0;
			for(k=0; i<length; i++, k++)
			{
				if((IsWhiteChar(cbufferIn[i])==FALSE)&&(cbufferIn[i]!=','))
					break;
			}
			//Copy everything from i to next ',' or '/' or ';'because user may have written "- reg" or "1 - reg"
			//so stopping at white chars is not enough
			for(k=0; i<length; i++, k++)
			{
				if((cbufferIn[i]==',')||(cbufferIn[i]=='/')||(cbufferIn[i]==';'))
					break;
				ctemp[k]=cbufferIn[i];
			}
			ctemp[k]=0;
			//Remove white chars from ctemp
			for(k=m=0; ctemp[k]; k++)
			{
				if(IsWhiteChar(ctemp[k])==FALSE)
					arg2[m++]=ctemp[k];
			}
			arg2[m]=0;
			
			//Identify the modifier part and remove it, write result to arg2b
			if(arg2[0]=='1')	//This is 1-reg
			{
				strcpy(arg2b, &arg2[2]);
			}
			else
			{
				for(k=0; arg2[k]; k++)
				{
					if(arg2[k]=='_')
					{
						for(m=0; m<k; m++)
						{
							arg2b[m]=arg2[m];
						}
						arg2b[m]=0;
					}
				}
			}
			
			//Is there a third argument? Is there a fourth argument? Search for comma
			for(found=false, m=i; m<length; m++)
			{
				if(cbufferIn[m]==',')
				{
					found=true;
					break;
				}
				else if(cbufferIn[m]=='\n')
					break;
			}
			if(found==false)
			{
				argCount=2;
				//Copy the rest of the line
				for(m=0; i<length; i++)
				{
					restOfLine[m++]=cbufferIn[i];
					if(cbufferIn[i]=='\n')
						break;
				}
				restOfLine[m]=0;
			}
			else
			{
				for(k=0; i<length; i++, k++)
				{
					if((IsWhiteChar(cbufferIn[i])==FALSE)&&(cbufferIn[i]!=','))
						break;
				}
				//Copy everything from i to next ',' or '/' or ';' because user may have written "- reg" or "1 - reg"
				//so stopping at white chars is not enough
				for(k=0; i<length; i++, k++)
				{
					if((cbufferIn[i]==',')||(cbufferIn[i]=='/')||(cbufferIn[i]==';'))
						break;
					ctemp[k]=cbufferIn[i];
				}
				ctemp[k]=0;
				//Remove white chars from ctemp
				for(k=m=0; ctemp[k]; k++)
				{
					if(IsWhiteChar(ctemp[k])==FALSE)
						arg3[m++]=ctemp[k];
				}
				arg3[m]=0;

				//Identify the modifier part and remove it, write result to arg2b
				if(arg3[0]=='1')	//This is 1-reg
				{
					strcpy(arg3b, &arg3[2]);
				}
				else
				{
					for(k=0; arg3[k]; k++)
					{
						if(arg3[k]=='_')
						{
							for(m=0; m<k; m++)
							{
								arg3b[m]=arg3[m];
							}
							arg3b[m]=0;
						}
					}
				}

				//Is there a fourth argument? Search for comma
				for(found=false, m=i; m<length; m++)
				{
					if(cbufferIn[m]==',')
					{
						found=true;
						break;
					}
					else if(cbufferIn[m]=='\n')
						break;
				}

				if(found==false)
				{
					argCount=3;
					//Copy the rest of the line
					for(m=0; i<length; i++)
					{
						restOfLine[m++]=cbufferIn[i];
						if(cbufferIn[i]=='\n')
							break;
					}
					restOfLine[m]=0;
				}
				else
				{
					for(k=0; i<length; i++, k++)
					{
						if((IsWhiteChar(cbufferIn[i])==FALSE)&&(cbufferIn[i]!=','))
							break;
					}
					//Copy everything from i to next ',' or '/' or ';' because user may have written "- reg" or "1 - reg"
					//so stopping at white chars is not enough
					for(k=0; i<length; i++, k++)
					{
						if((cbufferIn[i]==',')||(cbufferIn[i]=='/')||(cbufferIn[i]==';'))
							break;
						ctemp[k]=cbufferIn[i];
					}
					ctemp[k]=0;
					//Remove white chars from ctemp
					for(k=m=0; ctemp[k]; k++)
					{
						if(IsWhiteChar(ctemp[k])==FALSE)
							arg4[m++]=ctemp[k];
					}
					arg4[m]=0;

					//Identify the modifier part and remove it, write result to arg2b
					if(arg4[0]=='1')	//This is 1-reg
					{
						strcpy(arg4b, &arg4[2]);
					}
					else
					{
						for(k=0; arg4[k]; k++)
						{
							if(arg4[k]=='_')
							{
								for(m=0; m<k; m++)
								{
									arg4b[m]=arg4[m];
								}
								arg4b[m]=0;
							}
						}
					}

					argCount=4;
					//Copy the rest of the line
					for(m=0; i<length; i++)
					{
						restOfLine[m++]=cbufferIn[i];
						if(cbufferIn[i]=='\n')
							break;
					}
					restOfLine[m]=0;
				}
			}
			
			//Now check to see if any of the arguments have a register modifier, except for the first
			//which is the destination register and should not have any modifiers on it.
			if(argCount==2)
			{
				found=false;
				for(k=0; arg2[k]; k++)
				{
					if(arg2[k]=='_')
					{
						if((arg2[k+1]=='x')&&(arg2[k+2]=='2')&&(arg2[k+3]==0))
						{	//_x2, 2*reg
							found=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", 2.0;\n");
							strcat(cbufferOut, instruction);
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, arg1);
							strcat(cbufferOut, ", r555");
							strcat(cbufferOut, restOfLine);
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='x')&&(arg2[k+3]=='2')&&(arg2[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, 2.0, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
							strcat(cbufferOut, instruction);
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, arg1);
							strcat(cbufferOut, ", r555");
							strcat(cbufferOut, restOfLine);
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='i')&&(arg2[k+3]=='a')&&(arg2[k+4]=='s')&&(arg2[k+5]==0))
						{	//_bias, reg - 0.5
							found=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
							strcat(cbufferOut, instruction);
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, arg1);
							strcat(cbufferOut, ", r555");
							strcat(cbufferOut, restOfLine);
						}
						break;
					}
					else if((arg2[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r555, 1.0, ");
						strcat(cbufferOut, arg2b);
						strcat(cbufferOut, ";\n");
						strcat(cbufferOut, instruction);
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, arg1);
						strcat(cbufferOut, ", r555");
						strcat(cbufferOut, restOfLine);
						break;
					}
				}

				if(found==false)	//Copy single line as is
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, restOfLine);
				}
			}
			else if(argCount==3)
			{
				//There are 2 source operands that can have an instruction modifier, so
				//we treat each case in sequence
				found=false;
				for(k=0; arg2[k]; k++)
				{
					if(arg2[k]=='_')
					{
						if((arg2[k+1]=='x')&&(arg2[k+2]=='2')&&(arg2[k+3]==0))
						{	//_x2, 2*reg
							found=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", 2.0;\n");
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='x')&&(arg2[k+3]=='2')&&(arg2[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, 2.0, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='i')&&(arg2[k+3]=='a')&&(arg2[k+4]=='s')&&(arg2[k+5]==0))
						{	//_bias, reg - 0.5
							found=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						break;
					}
					else if((arg2[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r555, 1.0, ");
						strcat(cbufferOut, arg2b);
						strcat(cbufferOut, ";\n");
						break;
					}
				}
				
				found2=false;
				for(k=0; arg3[k]; k++)
				{
					if(arg3[k]=='_')
					{
						if((arg3[k+1]=='x')&&(arg3[k+2]=='2')&&(arg3[k+3]==0))
						{	//_x2, 2*reg
							found2=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", 2.0;\n");
						}
						else if((arg3[k+1]=='b')&&(arg3[k+2]=='x')&&(arg3[k+3]=='2')&&(arg3[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found2=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, 2.0, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						else if((arg3[k+1]=='b')&&(arg3[k+2]=='i')&&(arg3[k+3]=='a')&&(arg3[k+4]=='s')&&(arg3[k+5]==0))
						{	//_bias, reg - 0.5
							found2=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						break;
					}
					else if((arg3[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found2=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r556, 1.0, ");
						strcat(cbufferOut, arg3b);
						strcat(cbufferOut, ";\n");
						break;
					}
				}

				if((found==false)&&(found2==false))	//Copy single line as is
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, restOfLine);
				}
				else if((found==false)&&(found2))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, ", r556");
					strcat(cbufferOut, restOfLine);
				}
				else if((found)&&(found2==false))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, restOfLine);
				}
				else
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, r556");
					strcat(cbufferOut, restOfLine);
				}
			}
			else if(argCount==4)
			{
				//There are 3 source operands that can have an instruction modifier, so
				//we treat each case in sequence
				found=false;
				for(k=0; arg2[k]; k++)
				{
					if(arg2[k]=='_')
					{
						if((arg2[k+1]=='x')&&(arg2[k+2]=='2')&&(arg2[k+3]==0))
						{	//_x2, 2*reg
							found=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", 2.0;\n");
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='x')&&(arg2[k+3]=='2')&&(arg2[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, 2.0, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						else if((arg2[k+1]=='b')&&(arg2[k+2]=='i')&&(arg2[k+3]=='a')&&(arg2[k+4]=='s')&&(arg2[k+5]==0))
						{	//_bias, reg - 0.5
							found=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r555, ");
							strcat(cbufferOut, arg2b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						break;
					}
					else if((arg2[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r555, 1.0, ");
						strcat(cbufferOut, arg2b);
						strcat(cbufferOut, ";\n");
						break;
					}
				}
				
				found2=false;
				for(k=0; arg3[k]; k++)
				{
					if(arg3[k]=='_')
					{
						if((arg3[k+1]=='x')&&(arg3[k+2]=='2')&&(arg3[k+3]==0))
						{	//_x2, 2*reg
							found2=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", 2.0;\n");
						}
						else if((arg3[k+1]=='b')&&(arg3[k+2]=='x')&&(arg3[k+3]=='2')&&(arg3[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found2=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, 2.0, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						else if((arg3[k+1]=='b')&&(arg3[k+2]=='i')&&(arg3[k+3]=='a')&&(arg3[k+4]=='s')&&(arg3[k+5]==0))
						{	//_bias, reg - 0.5
							found2=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r556, ");
							strcat(cbufferOut, arg3b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						break;
					}
					else if((arg3[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found2=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r556, 1.0, ");
						strcat(cbufferOut, arg3b);
						strcat(cbufferOut, ";\n");
						break;
					}
				}

				found3=false;
				for(k=0; arg4[k]; k++)
				{
					if(arg4[k]=='_')
					{
						if((arg4[k+1]=='x')&&(arg4[k+2]=='2')&&(arg4[k+3]==0))
						{	//_x2, 2*reg
							found3=true;
							strcat(cbufferOut, "MUL");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r557, ");
							strcat(cbufferOut, arg4b);
							strcat(cbufferOut, ", 2.0;\n");
						}
						else if((arg4[k+1]=='b')&&(arg4[k+2]=='x')&&(arg4[k+3]=='2')&&(arg4[k+4]==0))
						{	//_bx2, 2*(reg - 0.5) or 2*reg - 0.5
							found3=true;
							strcat(cbufferOut, "MAD");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r557, 2.0, ");
							strcat(cbufferOut, arg4b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						else if((arg4[k+1]=='b')&&(arg4[k+2]=='i')&&(arg4[k+3]=='a')&&(arg4[k+4]=='s')&&(arg4[k+5]==0))
						{	//_bias, reg - 0.5
							found3=true;
							strcat(cbufferOut, "SUB");
							strcat(cbufferOut, whiteSpaces);
							strcat(cbufferOut, "r557, ");
							strcat(cbufferOut, arg4b);
							strcat(cbufferOut, ", -0.5;\n");
						}
						break;
					}
					else if((arg4[k]=='-')&&(k!=0))
					{	//invert, 1 - reg found
						found3=true;
						strcat(cbufferOut, "SUB");
						strcat(cbufferOut, whiteSpaces);
						strcat(cbufferOut, "r557, 1.0, ");
						strcat(cbufferOut, arg4b);
						strcat(cbufferOut, ";\n");
						break;
					}
				}

				if((found==false)&&(found2==false)&&(found3==false))	//Copy single line as is
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg4);
					strcat(cbufferOut, restOfLine);
				}
				else if((found==false)&&(found2)&&(found3==false))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, ", r556, ");
					strcat(cbufferOut, arg4);
					strcat(cbufferOut, restOfLine);
				}
				else if((found)&&(found2==false)&&(found3==false))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg4);
					strcat(cbufferOut, restOfLine);
				}
				else if((found)&&(found2)&&(found3==false))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, r556, ");
					strcat(cbufferOut, arg4);
					strcat(cbufferOut, restOfLine);
				}
				else if((found==false)&&(found2==false)&&(found3))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg2);
					strcat(cbufferOut, ", ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, ", r557");
					strcat(cbufferOut, restOfLine);
				}
				else if((found)&&(found2==false)&&(found3))
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, ");
					strcat(cbufferOut, arg3);
					strcat(cbufferOut, ", r557");
					strcat(cbufferOut, restOfLine);
				}
				else
				{
					strcat(cbufferOut, instruction);
					strcat(cbufferOut, whiteSpaces);
					strcat(cbufferOut, arg1);
					strcat(cbufferOut, ", r555, r556, r557");
					strcat(cbufferOut, restOfLine);
				}
			}
		}
	}
}

//Called from SuperFix_PS_2_0_To_ARBFP_1_0
//t# converted to texcoords
//v0 and v1 converted to fragment.color.primary and fragment.color.secondary
//c# constant registers are converted to program.local[#]
//oC0 converted to result.color
//If a line with PARAM is found, example PARAM c0={ 2.0f, -1.0f, 0.5f, 0.5f};
//all the 'f' characters are removed because this is not allowed in ARBfp1.0
void Convert_PS_2_0_RegistersTo_ARBFP_1_0(schar *tmpbuffer, schar *cbuffer)
{
	sint length=strlen(cbuffer);

	//Parts that follow '#' are ignored
	//Parts inside /* */ pairs are ignored
	sint firstSlashFound=-2, isARBFPCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
	//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
	//It will hold index of found char.
	sint secondAsteriskFound=-2;

	sint i, j, k, m;
	for(i=j=0; i<length; i++)
	{
		if((isARBFPCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
		{
			if(cbuffer[i]=='#')
			{
				isARBFPCommentFound=TRUE;
				tmpbuffer[j++]='#';
			}
			else if(cbuffer[i]=='/')
			{
				firstSlashFound=i;
			}
			else if(cbuffer[i]=='*')
			{
				if(firstSlashFound!=-2)
				{
					firstSlashFound=-2;
					isCPlusPlusCommentFound=TRUE;
					tmpbuffer[j++]='/';
					tmpbuffer[j++]='*';
				}
				else
					tmpbuffer[j++]=cbuffer[i];
			}
			else
			{
				if(cbuffer[i]=='t')
				{	//If tX, then write as fragment.texcoord[X]
					//tX must be surrounded by certain characters or nothing at all
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_kqw;
						}
					}

					//Copy fragment.texcoord[X]
					strcpy(&tmpbuffer[j], "fragment.texcoord[");
					strcat(&tmpbuffer[j], g_cbuffer256);
					strcat(&tmpbuffer[j], "]");
					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				else if(cbuffer[i]=='v')
				{	//If v0, then write as fragment.color.primary
					//If v1, then write as fragment.color.secondary
					//vX must be surround by certain characters or nothing at all
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_kqw;
						}
					}

					//If v0 or v1...
					if(strlen(g_cbuffer256)==1)
					{
						if(g_cbuffer256[0]='0')
						{
							strcpy(&tmpbuffer[j], "fragment.color.primary");
							j=strlen(tmpbuffer);
							i+=k-1;
							continue;
						}
						else if(g_cbuffer256[0]='1')
						{
							strcpy(&tmpbuffer[j], "fragment.color.secondary");
							j=strlen(tmpbuffer);
							i+=k-1;
							continue;
						}
						else
							goto jmp_kqw;
					}
				}
				else if(cbuffer[i]=='o')
				{	//If oD0, then write as result.color
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					if(i+1<length)
					{
						//After the o, there must be a C to make oC0
						if(cbuffer[i+1]!='C')
							goto jmp_kqw;
					}
					else
						goto jmp_kqw;
					i++;
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i+1<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i+1]==' ')||(cbuffer[k+i+1]=='\t')||
							(cbuffer[k+i+1]=='\n')||(cbuffer[k+i+1]==',')||
							(cbuffer[k+i+1]==';')||(cbuffer[k+i+1]=='.')||
							(cbuffer[k+i+1]=='+')||(cbuffer[k+i+1]=='-')||
							(cbuffer[k+i+1]=='*')||(cbuffer[k+i+1]=='/')||
							(cbuffer[k+i+1]=='(')||(cbuffer[k+i+1]==')')))
						{
							goto jmp_kqw;
						}
					}

					i--;
					if(strlen(g_cbuffer256)==1)
					{
						if(g_cbuffer256[0]='0')
						{
							i++;
							strcpy(&tmpbuffer[j], "result.color");
							j=strlen(tmpbuffer);
							i+=k-1;
							continue;
						}
						else
							goto jmp_kqw;
					}
				}
				else if(cbuffer[i]=='c')
				{	//If c#, then write as program.local[#]
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_kqw;
						}
					}

					strcpy(&tmpbuffer[j], "program.local[");
					strcat(&tmpbuffer[j], g_cbuffer256);
					strcat(&tmpbuffer[j], "]");
					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				else if(cbuffer[i]=='P')
				{
					//Copy until newline
					for(k=i; k<length; k++)
					{
						if(cbuffer[k]=='\n')
							break;
						g_cbuffer256[k-i]=cbuffer[k];
					}
					g_cbuffer256[k-i]=0;
					//PARAM?
					if(k-i>6)
					{
						if((g_cbuffer256[0]=='P')&&(g_cbuffer256[1]=='A')&&(g_cbuffer256[2]=='R')&&(g_cbuffer256[3]=='A')||(g_cbuffer256[4]=='M'))
						{
							i=k-1;

							//Move to first '{'
							for(k=m=0; g_cbuffer256[k]; k++, m++)
							{
								g_cbuffer256_2[m]=g_cbuffer256[k];
								if(g_cbuffer256[k]=='{')
								{
									//Skip ' ' and '\t', collect float, ignore 'f' if present, example 0.0f becomes 0.0
									for(k++, m++; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==TRUE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									//
									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==FALSE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									if(g_cbuffer256[k]=='f')
										k++;		//Skip

									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==TRUE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									//
									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==FALSE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									if(g_cbuffer256[k]=='f')
										k++;		//Skip

									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==TRUE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									//
									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==FALSE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									if(g_cbuffer256[k]=='f')
										k++;		//Skip

									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==TRUE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									//
									for(; g_cbuffer256[k]; k++, m++)
									{
										if(IsCharacterANumberOrAPointOrSignValue(g_cbuffer256[k])==FALSE)
											break;
										g_cbuffer256_2[m]=g_cbuffer256[k];
									}
									if(g_cbuffer256[k]=='f')
										k++;		//Skip

									k--;	//Compensate for k++
									m--;	//Compensate for m++
								}
							}
							strcpy(&tmpbuffer[j], g_cbuffer256_2);
							j=strlen(tmpbuffer);
							continue;
						}
					}
				}
				jmp_kqw:
				tmpbuffer[j++]=cbuffer[i];
			}
		}
		else if(isARBFPCommentFound)
		{	//Copy until end of line
			tmpbuffer[j++]=cbuffer[i];
			if(cbuffer[i]=='\n')
				isARBFPCommentFound=FALSE;
		}
		else if(isCPlusPlusCommentFound==TRUE)
		{
			//Copy until */
			if(secondAsteriskFound!=-2)
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='/')
				{
					secondAsteriskFound=-2;
					isCPlusPlusCommentFound=FALSE;	// */ found
				}
				else
					secondAsteriskFound=-2;
			}
			else
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='*')
					secondAsteriskFound=i;
			}
		}
	}

	tmpbuffer[j]=0;		//NULL terminate
	strcpy(cbuffer, tmpbuffer);
}

//Called from SuperFix_VS_2_0_To_ARBVP_1_0
//Convert v# to relevant vertex attributes (depends on declaration)
//Example : dcl_position0 v0, dcl_position1 v1, dcl_position2 v2, dcl_position v0 (could be any v#)
//			dcl_normal0 v0, dcl_normal1 v1, dcl_normal2 v2, dcl_normal3 v3, (could be any v#)
//			and so on. The others are blendweight, blendindices, psize, texcoord, tangent, binormal, tessfactor
//			positiont, color, fog, depth, sample.
//			I will only convert ---> position or position0 to position
//			blendweight# to weight[n]
//			normal or normal0 to normal
//			color or color0 to color (same as color.primary)
//			color1 to color.secondary
//			fog to fogcoord (not sure if this is correct)
//			texcoord# to texcoord[n]
//			blendindices# to matrixindex[n]
//			the rest including multiples like normal1 normal2, .... won't be converted.
//
//oPos converted to result.position
//oFog converted to result.fogcoord
//oPts converted to result.pointsize
//oD0 converted to result.color (same as result.color.primary)
//oD1 converted to result.color.secondary
//oT# converted to result.texcoord[#]
void Convert_VS_2_0_RegistersTo_ARBVP_1_0(schar *tmpbuffer, schar *cbuffer)
{
	sint i, j, k, m, done;
	sint length=strlen(cbuffer);

	//Parts that follow '#' are ignored
	//Parts inside /* */ pairs are ignored
	sint firstSlashFound=-2, isARBFPCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
	//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
	//It will hold index of found char.
	sint secondAsteriskFound=-2;

	//The input register declaration must be mapped to ARBvp1.0
	//Store string for each case : example : v0 or v1 or ....
	//If nothing, it will be NULL
	schar vertexposition[4];						//Only 1!
	schar vertexblendweight[16][4];				//Up to 16 should be more than enough
	schar vertexnormal[4];						//Only 1!
	schar vertexcolorprimary[4];					//Only 1!
	schar vertexcolorsecondary[4];				//Only 1!
	schar vertexfogcoord[4];						//Only 1!
	schar vertextexcoord[32][4];					//Up to 32 should be more than enough
	schar vertexmatrixindices[16][4];			//Up to 16 should be more than enough
	schar vertextangent[4];						//Only 1!
	schar vertexbinormal[4];						//Only 1!

	memset(vertexposition, 0, 4);
	memset(&vertexblendweight[0][0], 0, 16*4);
	memset(vertexnormal, 0, 4);
	memset(vertexcolorprimary, 0, 4);
	memset(vertexcolorsecondary, 0, 4);
	memset(vertexfogcoord, 0, 4);
	memset(&vertextexcoord[0][0], 0, 32*4);
	memset(&vertexmatrixindices[0][0], 0, 16*4);
	memset(vertextangent, 0, 4);
	memset(vertexbinormal, 0, 4);

	schar *c;
	for(i=0; i<length; )
	{
		c=strstr(&cbuffer[i], "dcl_");
		if(c==NULL)
			break;		//No more dcl_ !

		i=(sint)(c-cbuffer);
		if(CompareTwoStrings(&cbuffer[i], "dcl_position")==1)
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if(cbuffer[i]=='v')
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexposition[j++]=cbuffer[i];
				}
				vertexposition[j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_blendweight")==1)
		{
			i+=strlen("dcl_blendweight");
			//Collect the number, convert to int, use to index into vertexblendweight
			for(j=0; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
				g_cbuffer256[j++]=cbuffer[i];
			}
			g_cbuffer256[j]=0;
			if(g_cbuffer256[0]==0)
				k=0;
			else
				k=ConvertStringIntoSINT(g_cbuffer256);


			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if((cbuffer[i]=='v')&&(k<16))		//k must not exceed array limit
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexblendweight[k][j++]=cbuffer[i];
				}
				vertexblendweight[k][j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_normal")==1)
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if(cbuffer[i]=='v')
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexnormal[j++]=cbuffer[i];
				}
				vertexnormal[j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_color")==1)
		{
			if(CompareTwoStrings(&cbuffer[i], "dcl_color0")==1)				//primary
			{
				//Skip the chars until white char is met
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==TRUE)
						break;
				}
				//Skip white chars
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}

				if(cbuffer[i]=='v')
				{
					for(j=0; i<length; i++)
					{
						if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
							break;
						vertexcolorprimary[j++]=cbuffer[i];
					}
					vertexcolorprimary[j]=0;
				}
			}
			else if(CompareTwoStrings(&cbuffer[i], "dcl_color1")==1)		//secondary
			{
				//Skip the chars until white char is met
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==TRUE)
						break;
				}
				//Skip white chars
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}

				if(cbuffer[i]=='v')
				{
					for(j=0; i<length; i++)
					{
						if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
							break;
						vertexcolorsecondary[j++]=cbuffer[i];
					}
					vertexcolorsecondary[j]=0;
				}
			}
			else															//primary
			{
				//Skip the chars until white char is met
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==TRUE)
						break;
				}
				//Skip white chars
				for( ; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}

				if(cbuffer[i]=='v')
				{
					for(j=0; i<length; i++)
					{
						if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
							break;
						vertexcolorprimary[j++]=cbuffer[i];
					}
					vertexcolorprimary[j]=0;
				}
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_fog")==1)
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if(cbuffer[i]=='v')
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexfogcoord[j++]=cbuffer[i];
				}
				vertexfogcoord[j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_texcoord")==1)
		{
			i+=strlen("dcl_texcoord");
			//Collect the number, convert to int, use to index into vertextexcoord
			for(j=0; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
				g_cbuffer256[j++]=cbuffer[i];
			}
			g_cbuffer256[j]=0;
			if(g_cbuffer256[0]==0)
				k=0;
			else
				k=ConvertStringIntoSINT(g_cbuffer256);


			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if((cbuffer[i]=='v')&&(k<32))		//k must not exceed array limit
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertextexcoord[k][j++]=cbuffer[i];
				}
				vertextexcoord[k][j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_blendindices")==1)
		{
			i+=strlen("dcl_blendindices");
			//Collect the number, convert to int, use to index into vertexblendweight
			for(j=0; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
				g_cbuffer256[j++]=cbuffer[i];
			}
			g_cbuffer256[j]=0;
			if(g_cbuffer256[0]==0)
				k=0;
			else
				k=ConvertStringIntoSINT(g_cbuffer256);


			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if((cbuffer[i]=='v')&&(k<16))			//k must not exceed array limits
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexmatrixindices[k][j++]=cbuffer[i];
				}
				vertexmatrixindices[k][j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_tangent")==1)
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if(cbuffer[i]=='v')
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertextangent[j++]=cbuffer[i];
				}
				vertextangent[j]=0;
			}
		}
		else if(CompareTwoStrings(&cbuffer[i], "dcl_binormal")==1)
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==FALSE)
					break;
			}

			if(cbuffer[i]=='v')
			{
				for(j=0; i<length; i++)
				{
					if((IsWhiteChar(cbuffer[i])==TRUE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
						break;
					vertexbinormal[j++]=cbuffer[i];
				}
				vertexbinormal[j]=0;
			}
		}
		else
		{
			//Skip the chars until white char is met
			for( ; i<length; i++)
			{
				if(IsWhiteChar(cbuffer[i])==TRUE)
					break;
			}
			//Skip white chars
			for( ; i<length; i++)
			{
				if((IsWhiteChar(cbuffer[i])==FALSE)||(cbuffer[i]=='/')||(cbuffer[i]==';'))
					break;
			}
			//The above code will cause a skip of this unknown declaration
		}
	}

	for(i=j=0; i<length; i++)
	{
		if((isARBFPCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
		{
			if(cbuffer[i]=='#')
			{
				isARBFPCommentFound=TRUE;
				tmpbuffer[j++]='#';
			}
			else if(cbuffer[i]=='/')
			{
				firstSlashFound=i;
			}
			else if(cbuffer[i]=='*')
			{
				if(firstSlashFound!=-2)
				{
					firstSlashFound=-2;
					isCPlusPlusCommentFound=TRUE;
					tmpbuffer[j++]='/';
					tmpbuffer[j++]='*';
				}
				else
					tmpbuffer[j++]=cbuffer[i];
			}
			else
			{
				if(cbuffer[i]=='v')
				{	//If v#, then write relative ARBvp1.0 equivalent
					//v# must be surrounded by certain characters or nothing at all
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_kqw;
						}
					}

					done=0;
					if((CompareTwoStrings(&vertexposition[1], g_cbuffer256)==1)&&(vertexposition[0]!=0))
					{
						done=1;
						strcpy(&tmpbuffer[j], "vertex.position");
					}
					if(done==0)
					{
						for(m=0; m<16; m++)
						{
							if((CompareTwoStrings(&vertexblendweight[m][1], g_cbuffer256)==1)&&(vertexblendweight[m][0]!=0))
							{
								done=1;
								strcpy(&tmpbuffer[j], "vertex.weight[");
								//strcat(&tmpbuffer[j], g_cbuffer256);
								//Instead of the above, convert m to string and write it
								ConvertSINTIntoString(m, g_cbuffer256);
								strcat(&tmpbuffer[j], g_cbuffer256);
								strcat(&tmpbuffer[j], "]");
								break;
							}
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertexnormal[1], g_cbuffer256)==1)&&(vertexnormal[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.normal");
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertexcolorprimary[1], g_cbuffer256)==1)&&(vertexcolorprimary[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.color");
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertexcolorsecondary[1], g_cbuffer256)==1)&&(vertexcolorsecondary[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.color.secondary");
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertexfogcoord[1], g_cbuffer256)==1)&&(vertexfogcoord[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.fogcoord");
						}
					}
					if(done==0)
					{
						for(m=0; m<32; m++)
						{
							if((CompareTwoStrings(&vertextexcoord[m][1], g_cbuffer256)==1)&&(vertextexcoord[m][0]!=0))
							{
								done=1;
								strcpy(&tmpbuffer[j], "vertex.texcoord[");
								//strcat(&tmpbuffer[j], g_cbuffer256);
								//Instead of the above, convert m to string and write it
								ConvertSINTIntoString(m, g_cbuffer256);
								strcat(&tmpbuffer[j], g_cbuffer256);
								strcat(&tmpbuffer[j], "]");
								break;
							}
						}
					}
					if(done==0)
					{
						for(m=0; m<16; m++)
						{
							if((CompareTwoStrings(&vertexmatrixindices[m][1], g_cbuffer256)==1)&&(vertexmatrixindices[m][0]!=0))
							{
								done=1;
								strcpy(&tmpbuffer[j], "vertex.matrixindex[");
								//strcat(&tmpbuffer[j], g_cbuffer256);
								//Instead of the above, convert m to string and write it
								ConvertSINTIntoString(m, g_cbuffer256);
								strcat(&tmpbuffer[j], g_cbuffer256);
								strcat(&tmpbuffer[j], "]");
								break;
							}
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertextangent[1], g_cbuffer256)==1)&&(vertextangent[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.tangent");	//NOTE : this does not exist for ARBvp1.0
						}
					}
					if(done==0)
					{
						if((CompareTwoStrings(&vertexbinormal[1], g_cbuffer256)==1)&&(vertexbinormal[0]!=0))
						{
							done=1;
							strcpy(&tmpbuffer[j], "vertex.binormal");
						}
					}

					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				else if(cbuffer[i]=='o')
				{	//If oPos, write result.position
					//If oFog, write result.fogcoord
					//If oPts, write result.pointsize
					//If oD0, write result.color
					//If oD1, write result.color.secondary
					//If oT#, write result.texcoord[#]
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					
					//Copy until white char or other invalid char is found
					for(k=0; k+i<length; k++)
					{
						if((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')'))
							break;
						else
							g_cbuffer256[k]=cbuffer[k+i];
					}
					g_cbuffer256[k]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					
					{
						if(CompareTwoStrings(g_cbuffer256, "oPos")==1)
						{
							strcpy(&tmpbuffer[j], "result.position");
						}
						else if(CompareTwoStrings(g_cbuffer256, "oFog")==1)
						{
							strcpy(&tmpbuffer[j], "result.fogcoord");
						}
						else if(CompareTwoStrings(g_cbuffer256, "oPts")==1)
						{
							strcpy(&tmpbuffer[j], "result.pointsize");
						}
						else if(CompareTwoStrings(g_cbuffer256, "oD0")==1)
						{
							strcpy(&tmpbuffer[j], "result.color");
						}
						else if(CompareTwoStrings(g_cbuffer256, "oD1")==1)
						{
							strcpy(&tmpbuffer[j], "result.color.secondary");
						}
						else if(CompareTwoStrings(g_cbuffer256, "oT")==1)
						{
							strcpy(&tmpbuffer[j], "result.texcoord[");
							strcat(&tmpbuffer[j], &g_cbuffer256[2]);
							strcat(&tmpbuffer[j], "]");
						}
						
						j=strlen(tmpbuffer);
						i+=k-1;
						continue;
					}
				}
				else if(cbuffer[i]=='c')
				{	//If c#, then write as program.local[#]
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_kqw;
						}
					}
					
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_kqw;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_kqw;
						}
					}

					strcpy(&tmpbuffer[j], "program.local[");
					strcat(&tmpbuffer[j], g_cbuffer256);
					strcat(&tmpbuffer[j], "]");
					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				jmp_kqw:
				tmpbuffer[j++]=cbuffer[i];
			}
		}
		else if(isARBFPCommentFound)
		{	//Copy until end of line
			tmpbuffer[j++]=cbuffer[i];
			if(cbuffer[i]=='\n')
				isARBFPCommentFound=FALSE;
		}
		else if(isCPlusPlusCommentFound==TRUE)
		{
			//Copy until */
			if(secondAsteriskFound!=-2)
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='/')
				{
					secondAsteriskFound=-2;
					isCPlusPlusCommentFound=FALSE;	// */ found
				}
				else
					secondAsteriskFound=-2;
			}
			else
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='*')
					secondAsteriskFound=i;
			}
		}
	}

	tmpbuffer[j]=0;		//NULL terminate
	strcpy(cbuffer, tmpbuffer);
}


//Called from FixTextureInstructions_ARBFP_1_0
//We will search for dcl_2d, dcl_cube, dcl_volume to
//get the sampler types.
void GetSamplerTypesFrom_PS_2_0(schar *cbuffer, sint *samplerType)
{
	sint i, j, temp;
	sint length=strlen(cbuffer);
	for(i=0; i<length; i++)
	{
		if(cbuffer[i]=='d')
		{
			if(CompareTwoStrings(&cbuffer[i], "dcl_2d")==1)
			{
				i+=6;
				//Skip white spaces, get to sX
				for(; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}
				if(cbuffer[i]=='s')
				{
					i++;
					for(j=0; j+i<length; j++)
					{
						if(IsWhiteChar(cbuffer[j+i])==TRUE)
							break;
						g_cbuffer256_2[j]=cbuffer[j+i];
					}
					g_cbuffer256_2[j]=0;
					temp=atoi(g_cbuffer256_2);
					temp=(temp>=32) ? 31 : temp;
					samplerType[temp]=0;
				}
			}
			else if(CompareTwoStrings(&cbuffer[i], "dcl_cube")==1)
			{
				i+=8;
				//Skip white spaces, get to sX
				for(; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}
				if(cbuffer[i]=='s')
				{
					i++;
					for(j=0; j+i<length; j++)
					{
						if(IsWhiteChar(cbuffer[j+i])==TRUE)
							break;
						g_cbuffer256_2[j]=cbuffer[j+i];
					}
					g_cbuffer256_2[j]=0;
					temp=atoi(g_cbuffer256_2);
					temp=(temp>=32) ? 31 : temp;
					samplerType[temp]=1;
				}
			}
			else if(CompareTwoStrings(&cbuffer[i], "dcl_volume")==1)
			{
				i+=10;
				//Skip white spaces, get to sX
				for(; i<length; i++)
				{
					if(IsWhiteChar(cbuffer[i])==FALSE)
						break;
				}
				if(cbuffer[i]=='s')
				{
					i++;
					for(j=0; j+i<length; j++)
					{
						if(IsWhiteChar(cbuffer[j+i])==TRUE)
							break;
						g_cbuffer256_2[j]=cbuffer[j+i];
					}
					g_cbuffer256_2[j]=0;
					temp=atoi(g_cbuffer256_2);
					temp=(temp>=32) ? 31 : temp;
					samplerType[temp]=2;
				}
			}
		}
	}
}

//Called from SuperFix_PS_2_0_To_ARBFP_1_0
//Tex instruction in ps2.0 are of the form
//texld  dest, texcoord, samplerX
//which has been temporarily converted to
//TEX    dest, texcoord, samplerX;
//We need to convert samplerX to texture[X], 2D;  (or whatever)
void FixTextureInstructions_ARBFP_1_0(schar *tmpbuffer, schar *cbuffer)
{
	sint length=strlen(cbuffer);

	//Parts that follow '#' are ignored
	//Parts inside /* */ pairs are ignored
	sint firstSlashFound=-2, isARBFPCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
	//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
	//It will hold index of found char.
	sint secondAsteriskFound=-2;

	//The samplers are declared in the biginning of the file.
	//Search for dcl_2d dcl_cube and dcl_volume.
	//Write the types in the samplerType array.
	sint samplerType[32];		//Can't be more than 32 for sure
								//0 for 2D, 1 for cube, 2 for 3D
	memset(samplerType, 0, 32*sizeof(sint));
	GetSamplerTypesFrom_PS_2_0(cbuffer, samplerType);

	sint i, j, k, temp;
	for(i=j=0; i<length; i++)
	{
		if((isARBFPCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
		{
			if(cbuffer[i]=='#')
			{
				isARBFPCommentFound=TRUE;
				tmpbuffer[j++]='#';
			}
			else if(cbuffer[i]=='/')
			{
				firstSlashFound=i;
			}
			else if(cbuffer[i]=='*')
			{
				if(firstSlashFound!=-2)
				{
					firstSlashFound=-2;
					isCPlusPlusCommentFound=TRUE;
					tmpbuffer[j++]='/';
					tmpbuffer[j++]='*';
				}
				else
					tmpbuffer[j++]=cbuffer[i];
			}
			else
			{
				if(cbuffer[i]=='s')
				{	//This could be a sampler in a tex instruction
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_ydg;
						}
					}
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_ydg;
					if(k+i+1<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i+1]==' ')||(cbuffer[k+i+1]=='\t')||
							(cbuffer[k+i+1]=='\n')||(cbuffer[k+i+1]==',')||
							(cbuffer[k+i+1]==';')||(cbuffer[k+i+1]=='.')||
							(cbuffer[k+i+1]=='+')||(cbuffer[k+i+1]=='-')||
							(cbuffer[k+i+1]=='*')||(cbuffer[k+i+1]=='/')||
							(cbuffer[k+i+1]=='(')||(cbuffer[k+i+1]==')')))
						{
							goto jmp_ydg;
						}
					}

					temp=atoi(g_cbuffer256);
					temp=(temp>=32) ? 31 : temp;
					strcpy(&tmpbuffer[j], "texture[");
					strcat(&tmpbuffer[j], g_cbuffer256);
					strcat(&tmpbuffer[j], "], ");
					if(samplerType[temp]==0)		//2D
						strcat(&tmpbuffer[j], "2D");
					else if(samplerType[temp]==1)	//Cube
						strcat(&tmpbuffer[j], "CUBE");
					else if(samplerType[temp]==2)	//3D
						strcat(&tmpbuffer[j], "3D");

					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				jmp_ydg:
				tmpbuffer[j++]=cbuffer[i];
			}
		}
		else if(isARBFPCommentFound)
		{	//Copy until end of line
			tmpbuffer[j++]=cbuffer[i];
			if(cbuffer[i]=='\n')
				isARBFPCommentFound=FALSE;
		}
		else if(isCPlusPlusCommentFound==TRUE)
		{
			//Copy until */
			if(secondAsteriskFound!=-2)
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='/')
				{
					secondAsteriskFound=-2;
					isCPlusPlusCommentFound=FALSE;	// */ found
				}
				else
					secondAsteriskFound=-2;
			}
			else
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='*')
					secondAsteriskFound=i;
			}
		}
	}

	tmpbuffer[j]=0;		//NULL terminate
	strcpy(cbuffer, tmpbuffer);
}

//Called from SuperFix_PS_2_0_To_ARBFP_1_0. Also called from SuperFix_VS_2_0_To_ARBVP_1_0
void FixTempDeclarations_ARBFP_1_0(schar *tmpbuffer, schar *cbuffer)
{
	sint length=strlen(cbuffer);

	//cbuffer contains rX. We need to pick up the names
	//and declare them below !!ARBfp1.0 as
	//TEMP  rX, rX, rX, ....;\n\n
	//Find the rX and write them into g_cbuffer256_2
	g_cbuffer256_2[0]=0;

	//Parts that follow '#' are ignored
	//Parts inside /* */ pairs are ignored
	sint firstSlashFound=-2, isARBFPCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
	//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
	//It will hold index of found char.
	sint secondAsteriskFound=-2;

	sint *temp_account, tempcount=0;
	temp_account=new sint[4096];		//Could be many. Would be better to have dynamic array
	if(temp_account==NULL)
		return;
	memset(temp_account, -1, 4096*sizeof(sint));


	sint i, j, k, m, temp;
	for(i=j=0; i<length; i++)
	{
		if((isARBFPCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
		{
			if(cbuffer[i]=='#')
			{
				isARBFPCommentFound=TRUE;
				tmpbuffer[j++]='#';
			}
			else if(cbuffer[i]=='/')
			{
				firstSlashFound=i;
			}
			else if(cbuffer[i]=='*')
			{
				if(firstSlashFound!=-2)
				{
					firstSlashFound=-2;
					isCPlusPlusCommentFound=TRUE;
					tmpbuffer[j++]='/';
					tmpbuffer[j++]='*';
				}
				else
					tmpbuffer[j++]=cbuffer[i];
			}
			else
			{
				if(cbuffer[i]=='r')
				{	//This could be a temporary register
					if(i!=0)
					{
						if(!((cbuffer[i-1]==' ')||(cbuffer[i-1]=='\t')||
							(cbuffer[i-1]=='\n')||(cbuffer[i-1]==',')||
							(cbuffer[i-1]==';')||(cbuffer[i-1]=='.')||
							(cbuffer[i-1]=='+')||(cbuffer[i-1]=='-')||
							(cbuffer[i-1]=='*')||(cbuffer[i-1]=='/')||
							(cbuffer[i-1]=='(')||(cbuffer[i-1]==')')))
						{
							goto jmp_ydg;
						}
					}
					//Copy until non-number found
					for(k=1; k+i<length; k++)
					{
						if(IsCharacterANumber(cbuffer[k+i])==1)
							g_cbuffer256[k-1]=cbuffer[k+i];
						else
							break;
					}
					g_cbuffer256[k-1]=0;
					if(strlen(g_cbuffer256)==0)		//Too short! Must be at least 1
						goto jmp_ydg;
					if(k+i<length)				//Must be a OK character following it
					{
						if(!((cbuffer[k+i]==' ')||(cbuffer[k+i]=='\t')||
							(cbuffer[k+i]=='\n')||(cbuffer[k+i]==',')||
							(cbuffer[k+i]==';')||(cbuffer[k+i]=='.')||
							(cbuffer[k+i]=='+')||(cbuffer[k+i]=='-')||
							(cbuffer[k+i]=='*')||(cbuffer[k+i]=='/')||
							(cbuffer[k+i]=='(')||(cbuffer[k+i]==')')))
						{
							goto jmp_ydg;
						}
					}

					//Has it been added already?
					temp=atoi(g_cbuffer256);
					for(m=0; m<tempcount; m++)
					{
						if(temp_account[m]==temp)
							break;
					}
					if(m>=tempcount)
					{
						strcat(g_cbuffer256_2, "r");
						strcat(g_cbuffer256_2, g_cbuffer256);
						strcat(g_cbuffer256_2, ", ");

						temp_account[tempcount++]=temp;
					}
					
					strcpy(&tmpbuffer[j], "r");
					strcat(&tmpbuffer[j], g_cbuffer256);
					j=strlen(tmpbuffer);
					i+=k-1;
					continue;
				}
				jmp_ydg:
				tmpbuffer[j++]=cbuffer[i];
			}
		}
		else if(isARBFPCommentFound)
		{	//Copy until end of line
			tmpbuffer[j++]=cbuffer[i];
			if(cbuffer[i]=='\n')
				isARBFPCommentFound=FALSE;
		}
		else if(isCPlusPlusCommentFound==TRUE)
		{
			//Copy until */
			if(secondAsteriskFound!=-2)
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='/')
				{
					secondAsteriskFound=-2;
					isCPlusPlusCommentFound=FALSE;	// */ found
				}
				else
					secondAsteriskFound=-2;
			}
			else
			{
				tmpbuffer[j++]=cbuffer[i];
				if(cbuffer[i]=='*')
					secondAsteriskFound=i;
			}
		}
	}

	tmpbuffer[j]=0;		//NULL terminate
	
	//If temp registers found, we must declare them for ARBfp1.0
	if(strlen(g_cbuffer256_2)>0)
	{
		//Copy the !!ARBfp1.0
		length=strlen(tmpbuffer);
		for(i=0; i<length; i++)
		{
			if(CompareTwoStrings(&tmpbuffer[i], "!!ARBfp1.0")==0)
			{
				memcpy(&cbuffer[i], &tmpbuffer[i], 10);
				i+=10;
				//Copy until end of line
				for(; i<length; i++)
				{
					cbuffer[i]=tmpbuffer[i];
					if(cbuffer[i]=='\n')
					{
						i++;
						break;
					}
				}

				strcpy(&cbuffer[i], "#TEMPORARY DECLARATIONS :\nTEMP\t");
				temp=strlen(g_cbuffer256_2);
				g_cbuffer256_2[temp-2]=';';		//Remove the last ", " 
				g_cbuffer256_2[temp-1]='\n';
				strcat(&cbuffer[i], g_cbuffer256_2);
				strcat(&cbuffer[i], "\n");
				break;
			}
			else
				cbuffer[i]=tmpbuffer[i];
		}

		strcat(cbuffer, &tmpbuffer[i]);
	}
	else
		strcpy(cbuffer, tmpbuffer);

	delete [] temp_account;
}

//Called from glhTranslateShader for ps2.0 to ARBfp1.0
//Also works for ps1.x
sint SuperFix_PS_2_0_To_ARBFP_1_0(schar *cbuffer)
{
	sint length=strlen(cbuffer);

	schar *tmpbuffer=new schar[length*2+512];		//*2 to make sure we have enough space
	if(tmpbuffer==NULL)
		return 0;

	Convert_PS_2_0_RegistersTo_ARBFP_1_0(tmpbuffer, cbuffer);
	FixTextureInstructions_ARBFP_1_0(tmpbuffer, cbuffer);
	FixTempDeclarations_ARBFP_1_0(tmpbuffer, cbuffer);
	//cbuffer now contains fixed shader
	delete [] tmpbuffer;
	return 1;
}

//Called from glhTranslateShader for vs2.0 to ARBvp1.0
//Also works for vs1.x
sint SuperFix_VS_2_0_To_ARBVP_1_0(schar *cbuffer)
{
	sint length=strlen(cbuffer);

	schar *tmpbuffer=new schar[length*2+512];		//*2 to make sure we have enough space
	if(tmpbuffer==NULL)
		return 0;

	Convert_VS_2_0_RegistersTo_ARBVP_1_0(tmpbuffer, cbuffer);
	FixTempDeclarations_ARBFP_1_0(tmpbuffer, cbuffer);		//Vertex and pixel shader are the same
	//cbuffer now contains fixed shader
	delete [] tmpbuffer;
	return 1;
}


//PURPOSE:	Convert a shader from one format into another.
//			D3D pixel shader to GL fragment program
//			D3D vertex shader to GL vertex program
//			For example, from Direct3D ps2.0 to OpenGL ARBfp1.0
//			**** This function can be considered obsolete because for GL, it's own
//			**** high level shader is prefered, called GLSL
//			**** In the future, I may decide to add support for converting D3D low
//			**** level shader to GLSL and convert GL low level shaders to GLSL
//
//PARAMETERS:
//			shaderTypeIn  : should be one of the defines starting with GLH_SHADERTYPE_
//			                GLH_SHADERTYPE_PS_2_0
//			                GLH_SHADERTYPE_PS_1_4
//			                GLH_SHADERTYPE_VS_1_1
//			                GLH_SHADERTYPE_VS_2_0
//			pshaderTextIn : contains a NULL terminated ASCII text of the shader.
//			shaderTypeOut  : should be one of the defines starting with GLH_SHADERTYPE_
//			                 GLH_SHADERTYPE_ARBFP_1_0
//			                 GLH_SHADERTYPE_ARBVP_1_0
//			pshaderTextOut : You should allocate some space for this function to write the output string
//			                 Allocate twice the amount as pshaderTextIn
//			maxLengthOfShaderTextOut : The actual size of pshaderTextOut in bytes.
//
//RETURN:
//			1 for success
//			-1 if not yet implemented
sint glhTranslateShader(sint shaderTypeIn, const schar *pshaderTextIn,
						 sint shaderTypeOut, schar *pshaderTextOut, sint maxLengthOfShaderTextOut)
{
	if(shaderTypeIn==GLH_SHADERTYPE_PS_2_0)
	{
		//v# are input registers (max 2)
		//r# are temporary registers (12 min. 32 max)
		//c# are constant float registers (max 32)
		//i# are constant int registers (max 16)
		//b# are constant bool registers (max 16)
		//p0 is the predicate register (only 1)
		//s# are the samplers (max 16)
		//t# are the texture coordinate registers (max 8)
		//oC# are output color registers (I think 2)
		//oDepth is the depth (only 1)
		if(shaderTypeOut==GLH_SHADERTYPE_ARBFP_1_0)
		{
			//Run through pshaderTextIn, find typical tokens and translate.
			//If not recognized, don't change it.
			//Stuff inside /* */ are left as is.
			// // is converted to #
			// ;  is converted to #
			sint i, j;
			sint firstSlashFound=-2, isCCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
			//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
			//It will hold index of found char.
			sint secondAsteriskFound=-2;
			schar cbuffer256[256], cbufferOut256[256];

			for(i=j=0; pshaderTextIn[i]!=0; i++)
			{
				if((isCCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
				{
					if(pshaderTextIn[i]==';')
					{
						isCPlusPlusCommentFound=TRUE;	//This is like a C++ comment
						pshaderTextOut[j++]='#';		//In ARBfp1.0, // is #
					}
					else if(pshaderTextIn[i]=='/')
					{
						if(firstSlashFound+1==i)
						{
							isCPlusPlusCommentFound=TRUE;
							firstSlashFound=-2;
							pshaderTextOut[j++]='#';		//In ARBfp1.0, // is #
						}
						else
							firstSlashFound=i;		//Keep index
					}
					else if((pshaderTextIn[i]=='*')&&(firstSlashFound+1==i))
					{
						isCCommentFound=TRUE;
						firstSlashFound=-2;
						pshaderTextOut[j++]='/';			//In ARBfp1.0, /*  */ is not supported
						pshaderTextOut[j++]='*';			//so we leave it as is.
					}
					else if(IsWhiteChar(pshaderTextIn[i])==TRUE)
					{	//White chars are simply copied as is
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						pshaderTextOut[j++]=pshaderTextIn[i];
					}
					else
					{
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						//We need to process instructions (or otherwise)
						//one line at a time.
						CopyLineFromBuffer_v1((schar *)pshaderTextIn, i, cbuffer256);

						//Convert from D3D to GL
						TranslateInstructionFromBuffer_v1_ps(cbuffer256, cbufferOut256);

						if(strlen(cbufferOut256)+j>=maxLengthOfShaderTextOut)
							break;
						pshaderTextOut[j]=0;
						strcat(pshaderTextOut, cbufferOut256);
						j+=strlen(cbufferOut256);
					}
				}
				else if(isCPlusPlusCommentFound==TRUE)
				{	//Copy until end of line
					pshaderTextOut[j++]=pshaderTextIn[i];
					if(pshaderTextIn[i]=='\n')
						isCPlusPlusCommentFound=FALSE;
				}
				else if(isCCommentFound==TRUE)
				{
					//Copy until */
					if(secondAsteriskFound!=-2)
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='/')
						{
							secondAsteriskFound=-2;
							isCCommentFound=FALSE;	// */ found
						}
						else
							secondAsteriskFound=-2;
					}
					else
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='*')
							secondAsteriskFound=i;
					}
				}

				if(j==maxLengthOfShaderTextOut)
				{
					j--;
					break;
				}
			}

			if(j<maxLengthOfShaderTextOut)	//maxLengthOfShaderTextOut could be 0
			{
				//ARBfp1.0 needs to end with END so put 2 newlines, then write END
				if(j+6<maxLengthOfShaderTextOut)
				{
					pshaderTextOut[j]=0;
					strcat(pshaderTextOut, "\n\nEND");
				}
			}

			//Scan the text again : convert ps2.0 registers to ARBfp1.0 registers
			//and fix TEX, TXP, TXB instructions and make sure temp
			//registers are declared.
			SuperFix_PS_2_0_To_ARBFP_1_0(pshaderTextOut);

			//Find the !!ARBfp1.0 and on the next next line, put a #Uncomment one of these if you wish
			//#OPTION ARB_precision_hint_fastest
			//#OPTION ARB_precision_hint_nicest
			sint length=strlen(pshaderTextOut);
			schar *tempShader=new schar[length*2+512];
			strcpy(tempShader, pshaderTextOut);
			schar *c=strstr(tempShader, "!!ARBfp1.0");		//Get the address
			if(c)		//Must not be NULL
			{
				i=(sint)(c-tempShader);
				i+=strlen("!!ARBfp1.0\n");
				pshaderTextOut[i]=0;		//Null terminate
				strcat(pshaderTextOut, "\n#Uncomment one of these if you wish\n");
				strcat(pshaderTextOut, "#OPTION ARB_precision_hint_fastest;\n#OPTION ARB_precision_hint_nicest;\n\n");
				strcat(pshaderTextOut, &tempShader[i]);
			}

			delete [] tempShader;
			return 1;
		}
	}
	else if(shaderTypeIn==GLH_SHADERTYPE_PS_1_4)
	{
		//c# are constant registers (8)
		//r# are temporary registers (6)
		//t# are texture registers (6)
		//v# are color registers (2 in phase 2)
		if(shaderTypeOut==GLH_SHADERTYPE_ARBFP_1_0)
		{
			//Run through pshaderTextIn, find typical tokens and translate.
			//If not recognized, don't change it.
			//Stuff inside /* */ are left as is.
			// // is converted to #
			// ;  is converted to #
			sint i, j;
			sint firstSlashFound=-2, isCCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
			//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
			//It will hold index of found char.
			sint secondAsteriskFound=-2;
			schar cbuffer256[256], cbufferOut256[256], cbufferOut512[512];

			for(i=j=0; pshaderTextIn[i]!=0; i++)
			{
				if((isCCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
				{
					if(pshaderTextIn[i]==';')
					{
						isCPlusPlusCommentFound=TRUE;	//This is like a C++ comment
						pshaderTextOut[j++]='#';		//In ARBfp1.0, // is #
					}
					else if(pshaderTextIn[i]=='/')
					{
						if(firstSlashFound+1==i)
						{
							isCPlusPlusCommentFound=TRUE;
							firstSlashFound=-2;
							pshaderTextOut[j++]='#';		//In ARBfp1.0, // is #
						}
						else
							firstSlashFound=i;		//Keep index
					}
					else if((pshaderTextIn[i]=='*')&&(firstSlashFound+1==i))
					{
						isCCommentFound=TRUE;
						firstSlashFound=-2;
						pshaderTextOut[j++]='/';			//In ARBfp1.0, /*  */ is not supported
						pshaderTextOut[j++]='*';			//so we leave it as is.
					}
					else if(IsWhiteChar(pshaderTextIn[i])==TRUE)
					{	//White chars are simply copied as is
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						pshaderTextOut[j++]=pshaderTextIn[i];
					}
					else
					{
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						//We need to process instructions (or otherwise)
						//one line at a time.
						CopyLineFromBuffer_v1((schar *)pshaderTextIn, i, cbuffer256);

						//Convert from D3D to GL
						TranslateInstructionFromBuffer_v2(cbuffer256, cbufferOut256);
						//If a register modifier is used in the above cbufferOut256, this functions will correct it
						FixRegisterModifiers_PS_1_4(cbufferOut256, cbufferOut512);

						if(strlen(cbufferOut512)+j>=maxLengthOfShaderTextOut)
							break;
						pshaderTextOut[j]=0;
						strcat(pshaderTextOut, cbufferOut512);
						j+=strlen(cbufferOut512);
					}
				}
				else if(isCPlusPlusCommentFound==TRUE)
				{	//Copy until end of line
					pshaderTextOut[j++]=pshaderTextIn[i];
					if(pshaderTextIn[i]=='\n')
						isCPlusPlusCommentFound=FALSE;
				}
				else if(isCCommentFound==TRUE)
				{
					//Copy until */
					if(secondAsteriskFound!=-2)
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='/')
						{
							secondAsteriskFound=-2;
							isCCommentFound=FALSE;	// */ found
						}
						else
							secondAsteriskFound=-2;
					}
					else
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='*')
							secondAsteriskFound=i;
					}
				}

				if(j==maxLengthOfShaderTextOut)
				{
					j--;
					break;
				}
			}

			if(j<maxLengthOfShaderTextOut)	//maxLengthOfShaderTextOut could be 0
			{
				//ARBfp1.0 needs to end with END so put 2 newlines, then write END
				if(j+6<maxLengthOfShaderTextOut)
				{
					pshaderTextOut[j]=0;
					strcat(pshaderTextOut, "\n\nEND");
				}
			}

			//Scan the text again : convert ps1.4 registers to ARBfp1.0 registers
			//and fix TEX, TXP, TXB instructions and make sure temp
			//registers are declared.
			SuperFix_PS_2_0_To_ARBFP_1_0(pshaderTextOut);		//This function also works for ps1.4

			//Find the !!ARBfp1.0 and on the next next line, put a #Uncomment one of these if you wish
			//#OPTION ARB_precision_hint_fastest
			//#OPTION ARB_precision_hint_nicest
			sint length=strlen(pshaderTextOut);
			schar *tempShader=new schar[length*2+512];
			strcpy(tempShader, pshaderTextOut);
			schar *c=strstr(tempShader, "!!ARBfp1.0");		//Get the address
			if(c)		//Must not be NULL
			{
				i=(sint)(c-tempShader);
				i+=strlen("!!ARBfp1.0\n");
				pshaderTextOut[i]=0;		//Null terminate
				strcat(pshaderTextOut, "\n#Uncomment one of these if you wish\n");
				strcat(pshaderTextOut, "#OPTION ARB_precision_hint_fastest;\n#OPTION ARB_precision_hint_nicest;\n\n");
				strcat(pshaderTextOut, &tempShader[i]);
			}

			//Just before "END", put a line that says MOV  result.color, r0;
			//because in ps1.4, r0 is the output color register but during translation to ARBfp1.0,
			//r0 is just another register.
			length=strlen(pshaderTextOut);
			strcpy(tempShader, pshaderTextOut);		//I'm sure there is enough spaces in tempShader
			c=strstr(&tempShader[length-5], "END");		//Get the address
			if(c)		//Must not be NULL
			{
				i=(sint)(c-tempShader);
				strcpy(&tempShader[i], "MOV\tresult.color, r0;\n\n\nEND");
				strcpy(pshaderTextOut, tempShader);		//Copy back
			}

			delete [] tempShader;
			return 1;
		}
	}
	else if((shaderTypeIn==GLH_SHADERTYPE_VS_1_1)||
		(shaderTypeIn==GLH_SHADERTYPE_VS_2_0))
	{
		//v# are input registers (max 16)
		//r# are temporary registers (max 12)
		//c# are constant float registers (max 2)
		//i# are constant int registers (max 16)
		//b# are constant bool registers (max 16)
		//a0 is the address register (only 1)
		//aL is the loop count register (onyl 1)  (can be used in loop...endloop, it get incremented)
		//oPos is the output position register
		//oFog is the output fog register (per vertex fog color)
		//oPts is the output point size register (only the x is used)
		//oD0 is the output diffuse color (rgba)
		//oD1 is the output secondary color (probably rgb, not sure)
		//oT# are tex coord registers (max 8)
		if(shaderTypeOut==GLH_SHADERTYPE_ARBVP_1_0)
		{
			//Run through pshaderTextIn, find typical tokens and translate.
			//If not recognized, don't change it.
			//Stuff inside /* */ are left as is.
			// // is converted to #
			// ;  is converted to #
			sint i, j;
			sint firstSlashFound=-2, isCCommentFound=FALSE, isCPlusPlusCommentFound=FALSE;
			//-2 because we do (firstSlashFound+1==i) test and it would fail if it was -1
			//It will hold index of found char.
			sint secondAsteriskFound=-2;
			schar cbuffer256[256], cbufferOut256[256];

			for(i=j=0; pshaderTextIn[i]!=0; i++)
			{
				if((isCCommentFound==FALSE)&&(isCPlusPlusCommentFound==FALSE))
				{
					if(pshaderTextIn[i]==';')
					{
						isCPlusPlusCommentFound=TRUE;	//This is like a C++ comment
						pshaderTextOut[j++]='#';		//In ARBfp1.0, // is #
					}
					else if(pshaderTextIn[i]=='/')
					{
						if(firstSlashFound+1==i)
						{
							isCPlusPlusCommentFound=TRUE;
							firstSlashFound=-2;
							pshaderTextOut[j++]='#';		//In ARBvp1.0, // is #
						}
						else
							firstSlashFound=i;		//Keep index
					}
					else if((pshaderTextIn[i]=='*')&&(firstSlashFound+1==i))
					{
						isCCommentFound=TRUE;
						firstSlashFound=-2;
						pshaderTextOut[j++]='/';			//In ARBvp1.0, /*  */ is not supported
						pshaderTextOut[j++]='*';			//so we leave it as is.
					}
					else if(IsWhiteChar(pshaderTextIn[i])==TRUE)
					{	//White chars are simply copied as is
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						pshaderTextOut[j++]=pshaderTextIn[i];
					}
					else
					{
						if(firstSlashFound!=-2)
						{
							firstSlashFound=-2;
							pshaderTextOut[j++]='/';
						}
						//We need to process instructions (or otherwise)
						//one line at a time.
						CopyLineFromBuffer_v1((schar *)pshaderTextIn, i, cbuffer256);

						//Convert from D3D to GL
						TranslateInstructionFromBuffer_v1_vs(cbuffer256, cbufferOut256);

						if(strlen(cbufferOut256)+j>=maxLengthOfShaderTextOut)
							break;
						pshaderTextOut[j]=0;
						strcat(pshaderTextOut, cbufferOut256);
						j+=strlen(cbufferOut256);
					}
				}
				else if(isCPlusPlusCommentFound==TRUE)
				{	//Copy until end of line
					pshaderTextOut[j++]=pshaderTextIn[i];
					if(pshaderTextIn[i]=='\n')
						isCPlusPlusCommentFound=FALSE;
				}
				else if(isCCommentFound==TRUE)
				{
					//Copy until */
					if(secondAsteriskFound!=-2)
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='/')
						{
							secondAsteriskFound=-2;
							isCCommentFound=FALSE;	// */ found
						}
						else
							secondAsteriskFound=-2;
					}
					else
					{
						pshaderTextOut[j++]=pshaderTextIn[i];
						if(pshaderTextIn[i]=='*')
							secondAsteriskFound=i;
					}
				}

				if(j==maxLengthOfShaderTextOut)
				{
					j--;
					break;
				}
			}

			if(j<maxLengthOfShaderTextOut)	//maxLengthOfShaderTextOut could be 0
			{
				//ARBvp1.0 needs to end with END so put 2 newlines, then write END
				if(j+6<maxLengthOfShaderTextOut)
				{
					pshaderTextOut[j]=0;
					strcat(pshaderTextOut, "\n\nEND");
				}
			}

			//Scan the text again : convert vs2.0 registers to ARBvp1.0 registers
			//Make sure temp registers are declared.
			SuperFix_VS_2_0_To_ARBVP_1_0(pshaderTextOut);

			//Find the !!ARBvp1.0 and on the next next line, put a #Uncomment this if you wish
			//#OPTION ARB_position_invariant
			sint length=strlen(pshaderTextOut);
			schar *tempShader=new schar[length*2+512];
			strcpy(tempShader, pshaderTextOut);
			schar *c=strstr(tempShader, "!!ARBvp1.0");		//Get the address
			if(c)		//Must not be NULL
			{
				i=(sint)(c-tempShader);
				i+=strlen("!!ARBvp1.0\n");
				pshaderTextOut[i]=0;		//Null terminate
				strcat(pshaderTextOut, "\n#Uncomment this if you wish\n");
				strcat(pshaderTextOut, "#OPTION ARB_position_invariant;\n\n");
				strcat(pshaderTextOut, &tempShader[i]);
			}

			delete [] tempShader;
			return 1;
		}
	}


	return -1;
}




