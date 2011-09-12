#if !defined(_3DGRAPHICSLIBRARYSMALL_H_)
#define _3DGRAPHICSLIBRARYSMALL_H_


//#include <GL/gl.h>
#include "glhlib.h"
#include <stdio.h>		//For definition of FILE
#include "global.h"
#include <math.h>
#include "MathLibrary.h"

#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'



//PURPOSE:	Returns 1 if ray does cross triangle, 0 if not
//			The pointers are all pointing to 3 memory locations.
//			normal is the normal vector for triangle, need not be normalized and
//			direction doesn't matter.
//			A ray has a starting point but goes to infinity. The t in the line equation must
//			be 0<=t<infinity
//			If this function returns 1, then PointOfIntersection is the actual point
//			of intersection.

//This is a new version.
//rayDirectionLengthSquared is rayDirection DOT rayDirection
//triangleVector1 is trianglePoint2-trianglePoint1
//triangleVector2 is triangleVector3-triangleVector1
//Returns pointOfIntersection
//rayDirection need not be normalized.
//Triangle edge vectors must be computed externally(triangleVector1 and triangleVector2)
sint DoesRayCrossTriangle_6_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal rayDirectionLengthSquared,
												  sreal *trianglePoint1, sreal *trianglePoint2,
												  sreal *triangleVector1, sreal *triangleVector2,
												  sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
												  sreal triangleVector1_DOT_triangleVector2,
												  sreal *pointOfIntersection);

//These Sqr functions are more accurate and less likely to give errors!!!
//Used by DoesRayCrossTriangle_6_FLOAT
sreal SqrDistanceRayTriangle_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal rayDirectionLengthSquared,
					sreal *trianglePoint1, sreal *trianglePoint2,
					sreal *triangleVector1, sreal *triangleVector2,
					sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
					sreal triangleVector1_DOT_triangleVector2,
					sreal *pfRayP);

//Used by SqrDistanceRayTriangle_FLOAT
sreal SqrDistancePointTriangle_FLOAT(sreal *rkPoint, sreal *trianglepoint1, sreal *triangleVector1,
									  sreal *triangleVector2,
									  sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
									  sreal triangleVector1_DOT_triangleVector2,
									  sreal *pfSParam, sreal *pfTParam);

//Used by SqrDistanceRayTriangle_FLOAT
sreal SqrDistanceRaySegment_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal *segmentPoint1, sreal *segmentDirection, sreal *pfRayP, sreal *pfSegP);

//PURPOSE:	reflect = 2 * dot(N, I) * N - I
//       <  ^  >
//        \ | /
//         \|/  the left one is I (incident), the center one is N (normal), the right is reflected
//=====================
//N and I should be normalized.
inline void ComputeReflectedVector_1(sreal *reflected, sreal *pincidentVector, sreal *pnormal)
{
	//reflect = 2 * dot(N, I) * N - I
	sreal temp=2.0*pnormal[0]*pincidentVector[0]+pnormal[1]*pincidentVector[1]+pnormal[2]*pincidentVector[2];
	reflected[0]=temp*pnormal[0]-pincidentVector[0];
	reflected[1]=temp*pnormal[1]-pincidentVector[1];
	reflected[2]=temp*pnormal[2]-pincidentVector[2];
}

//PURPOSE:
//       <  ^   
//        \ |  
//         \|   the left one is I (incident), the center one is N (normal), the right is refracted
//			\
//			 \
//			  >
//=====================
//N and I should be normalized.
//eta is the ratio of the indices (from material / to be entered material)
//NOTE:	It migth be a good idea to take total internal reflection into account
//		using external code.
inline void ComputeRefractedVector_1(sreal *refracted, sreal *pincidentVector, sreal *pnormal, sreal eta)
{
	//k = 1.0 - eta * eta * (1.0 - dot(N, I) * dot(N, I))
	//if (k < 0.0)
	//	result = genType(0.0)
	//else
	//	result = eta * I - (eta * dot(N, I) + sqrt(k)) * N

	//Snell's law says
	//n_1 * sin(theta_1) = n_2 * sin(theta_2) and that
	// constant = sin(theta_1)/sin(theta_2) = eta

	//pincidentVector[0]=-pincidentVector[0];
	//pincidentVector[1]=-pincidentVector[1];
	//pincidentVector[2]=-pincidentVector[2];
	sreal nDOTi=pnormal[0]*pincidentVector[0]+pnormal[1]*pincidentVector[1]+pnormal[2]*pincidentVector[2];
	sreal k=1.0-eta*eta*(1.0-nDOTi*nDOTi);
	if(k<0.0)	//Negative means total internal reflection?
	{
		refracted[0]=refracted[1]=refracted[2]=0.0;
	}
	else
	{
		sreal t=eta*nDOTi+sqrtf(k);
		refracted[0]=-eta*pincidentVector[0]-t*pnormal[0];
		refracted[1]=-eta*pincidentVector[1]-t*pnormal[1];
		refracted[2]=-eta*pincidentVector[2]-t*pnormal[2];

		//Test to see if above code is working OK
		NormalizeVectorFLOAT_2(refracted);
		float n[3];
		n[0]=0.0;
		n[1]=-1.0;
		n[2]=0.0;
		float a1=ComputeAngleBetweenTwoVectorFLOAT_2x(refracted, n);
		n[1]=1.0;
		float a2=ComputeAngleBetweenTwoVectorFLOAT_2x(pincidentVector, n);
		float val=sinf(a2);
		float val2=sinf(a1);
		float val3=eta*val;		//val should = val2
	}
	

	//GLSL code from Tom Nuydens (slightly different because of his "eta" I guess)
	/*vec4 MyRefraction(vec4 NDotV, vec4 N_eye, vec4 V_eye)
	{
		vec4 t;
		//sreal k=eta*(1.0-nDOTi*nDOTi)-1.0;
		t.x = (-NDotV.x) * NDotV.x + 1.0;
		t.x = t.x * ior.y - 1.0;
		//sreal t=eta*nDOTi+sqrtf(k);
		t.x = sqrt(t.x);
		t.x = ior.x * NDotV.x + t.x;
		t = N_eye * t.x;
		//refracted = eta*(-pincidentVector) + eta*pnormal
		t = ior.x * (-V_eye) + t;
		
		return vec4(ModelviewTranspose * vec3(t), 0.0);
	}*/
}


//PURPOSE:	Compute tangent and binormal vectors.
//			index, vertex, normal and tex coords must be supplied and
//			memory for tangent and binormal vectors should be allocated externally.
//			numCompVertex is the number of components per vertex. Ditto for the rest.
//
//PARAMETERS:
//			[in] totalIndices : The number of indices
//			[in] pindices : An array containing indices. Each 3 forms a triangle.
//			[in] totalVertex : This is actually number of vertices * numCompVertex.
//			[in] numCompVertex : Number of components per vertex. 3 or 4.
//			[in] pvertex : An array containing vertices.
//			[in] numCompNormals : Number of components per normal. 3 or 4.
//			[in] pnormals : An array containing normals
//			[in] numCompTexCoord : Number of components per texcoord. 2, 3, 4.
//			[in] ptexCoord : An array containing texcoords
//			[in] numCompTangent : Number of components per tangent. 3 or 4.
//			[out] ptangent : An array containing tangents
//			[in] numCompBinormal : Number of components per binormals. 3 or 4.
//			[out] pbinormal : An array containing binormals
//
//RETURN:
//			1 for sucess,
//			-1 if bad values passed (0 for array size, bad numCmp value).
//			0 for memory error.
sint ComputeTangentAndBinormalVectors_v1_FLOAT(sint totalIndices, uint *pindices, sint totalVertex, sint numCompVertex, sreal *pvertex, sint numCompNormals, sreal *pnormals, sint numCompTexCoord, sreal *ptexCoord, sint numCompTangent, sreal *ptangent, sint numCompBinormal, sreal *pbinormal);
sint ComputeTangentAndBinormalVectors_v1_FLOAT_2(sint totalIndices, ushort *pindices, sint totalVertex, sint numCompVertex, sreal *pvertex, sint numCompNormals, sreal *pnormals, sint numCompTexCoord, sreal *ptexCoord, sint numCompTangent, sreal *ptangent, sint numCompBinormal, sreal *pbinormal);
sint ComputeTangentAndBinormalVectors_v1_FLOAT_3(sint totalIndices, uint *pindices, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);
sint ComputeTangentAndBinormalVectors_v1_FLOAT_4(sint totalIndices, ushort *pindices, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);


//PURPOSE:	Computes the normals for every vertex.
//			Normals are computed with respect to the right hand rule (right handed coordinate system)
//			Normals are averaged when a vertex is shared between different triangles.
//
//PARAMETERS:
//			[in] totalIndex : the size of pindex
//			[in] pindex : index array
//			[in] vertexCount : number of vertices
//			[in, out] pvertex : the array of vertices
//
//RETURN:
//			pindex and pvertex must not be NULL.
//			By default, the normals are average over the neighboring faces.
//			Returns -1 if((pindex==NULL)||(pvertex==NULL), else 1 for success.
sint GenerateNormalsForIndexList_FLOAT_3(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT *pvertex);
sint GenerateNormalsForIndexList_FLOAT_4(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT *pvertex);
sint GenerateNormalsForIndexList_FLOAT_5(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex);
sint GenerateNormalsForIndexList_FLOAT_6(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex);
sint GenerateNormalsForIndexList_FLOAT_7(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);
sint GenerateNormalsForIndexList_FLOAT_8(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);
sint GenerateNormalsForIndexList_FLOAT_9(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex);
sint GenerateNormalsForIndexList_FLOAT_10(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex);

//PURPOSE:	Computes the normals for every vertex.
//			Normals are computed with respect to the right hand rule (right handed coordinate system)
//			Normals are averaged when a vertex is shared between different triangles. The code checks if
//			multiple vertices in pvertex array are identicle.
//
//PARAMETERS:
//			[in] totalIndex : the size of pindex
//			[in] pindex : index array
//			[in] vertexCount : number of vertices
//			[in, out] pvertex : the array of vertices
//
//RETURN:
//			pindex and pvertex must not be NULL.
//			By default, the normals are average over the neighboring faces.
//			Returns -1 if((pindex==NULL)||(pvertex==NULL), else 1 for success.
sint GenerateNormalsForIndexList_Shared_FLOAT_3(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_4(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_5(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_6(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_7(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_8(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_9(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex);
sint GenerateNormalsForIndexList_Shared_FLOAT_10(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex);



#endif		//_3DGRAPHICSLIBRARYSMALL_H_
