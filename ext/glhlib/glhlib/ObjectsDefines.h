#if !defined(OBJECTSDEFINES_H)
#define OBJECTSDEFINES_H

//****** NOTE : THESE DECLARATION'S ORDER MUST BE PRESERVED IN SWITCH STATEMENT FOR HIGH PERFORMANCE
//              THEY ARE ORDERED FROM MOST IMPORTANT TO LEAST IMPORTANT




#define SILENT_OD





#ifndef NUMBEROFTEXTUREUNITS
#define NUMBEROFTEXTUREUNITS		8		//MUST BE CAREFUL WHEN CHANGING THIS VALUE (LOOK AT CPP FILES)
#ifndef SILENT_OD
#pragma message("NUMBEROFTEXTUREUNITS defined as 8. (ObjectsDefines.h)")
#endif
#else
#ifndef SILENT_OD
#pragma message("NUMBEROFTEXTUREUNITS already defined. (ObjectsDefines.h)")
#endif
#endif

//I will use this one in my render engine where I will need more tex units for my shader path
#ifndef NUMBEROFTEXTUREUNITS2
#define NUMBEROFTEXTUREUNITS2		16		//MUST BE CAREFUL WHEN CHANGING THIS VALUE (LOOK AT CPP FILES)
#ifndef SILENT_OD
#pragma message("NUMBEROFTEXTUREUNITS2 defined as 16. (ObjectsDefines.h)")
#endif
#else
#ifndef SILENT_OD
#pragma message("NUMBEROFTEXTUREUNITS2 already defined. (ObjectsDefines.h)")
#endif
#endif




#ifndef enumCOLORPRECISION
	#define enumCOLORPRECISION
	#ifndef SILENT_OD
	#pragma message("enumCOLORPRECISION defined. (ObjectsDefines.h)")
	#endif
	enum COLORPRECISION
	{
		COLORPRECISION_UBYTE,		//8 bit integer per component
		COLORPRECISION_16FLOAT,		//16 bit float per component
		COLORPRECISION_32FLOAT,		//32 bit float per component
	};
#else
	#ifndef SILENT_OD
	#pragma message("enumCOLORPRECISION already defined. (ObjectsDefines.h)")
	#endif
#endif

//MACRO for building 32 bit from 4 * 8 bits
#ifndef COLOR_BGRA
	#define COLOR_BGRA(b, g, r, a) ((uint)((a<<24) | (r<<16) | (g<<8) | b))
	#ifndef SILENT_OD
	#pragma message("COLOR_BGRA is beeing defined in ObjectsDefines.h")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("COLOR_BGRA is already defined")
	#endif
#endif

#ifndef COLOR_RGBA
	#define COLOR_RGBA(r, g, b, a) ((uint)((a<<24) | (b<<16) | (g<<8) | r))
	#ifndef SILENT_OD
	#pragma message("COLOR_RGBA is beeing defined in ObjectsDefines.h")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("COLOR_RGBA is already defined")
	#endif
#endif





#ifndef DEGREES_TO_RADIANS
	#define DEGREES_TO_RADIANS		0.017453292519943295769236907684886
	#ifndef SILENT_OD
	#pragma message("DEGREES_TO_RADIANS defined 0.017453292519943295769236907684886 (ObjectsDefines.h)")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("DEGREES_TO_RADIANS already defined. (ObjectsDefines.h)")
	#endif
#endif

#ifndef RADIANS_TO_DEGREES
	#define RADIANS_TO_DEGREES		57.295779513082320876798154814105
	#ifndef SILENT_OD
	#pragma message("RADIANS_TO_DEGREES defined 57.295779513082320876798154814105 (ObjectsDefines.h)")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("RADIANS_TO_DEGREES already defined. (ObjectsDefines.h)")
	#endif
#endif


//MACRO for building 32 bit from 4 * 8 bits
#ifndef COLOR_BGRA
	#define COLOR_BGRA(b, g, r, a) ((uint)((a<<24) | (r<<16) | (g<<8) | b))
	#ifndef SILENT_OD
	#pragma message("COLOR_BGRA defined. (ObjectsDefines.h)")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("COLOR_BGRA already defined. (ObjectsDefines.h)")
	#endif
#endif

#ifndef COLOR_RGBA
	#define COLOR_RGBA(r, g, b, a) ((uint)((a<<24) | (b<<16) | (g<<8) | r))
	#ifndef SILENT_OD
	#pragma message("COLOR_RGBA defined. (ObjectsDefines.h)")
	#endif
#else
	#ifndef SILENT_OD
	#pragma message("COLOR_RGBA already defined. (ObjectsDefines.h)")
	#endif
#endif



//Component count for vertices, normals, ....
#define X_COUNT			1
#define XY_COUNT		2
#define XYZ_COUNT		3
#define XYZW_COUNT		4



//Pi value related
#define PI_FULL_DEF			6.283185307179586476925286766559
#define PI_DEF				3.1415926535897932384626433832795
#define PI_2_DEF			1.5707963267948966192313216916398
#define PI_4_DEF			0.78539816339744830961566084581988

//Used by TPoint for defining shape : ShapeStyle
#define SHAPESTYLE_SQUARE 1
#define SHAPESTYLE_CIRCLE 2

//Used by RenderMode by TSphere, TCylinder : for rendering in OpenGL
#define RENDERMODE_FLATSHADING 1
#define RENDERMODE_GOURAUDSHADING 2
#define RENDERMODE_WIREFRAMESURFACE 3
#define RENDERMODE_WIREFRAMEINTERNAL 4

//Used by ColorStyle by TSphere, TCylinder : for rendering in OpenGL
#define COLORSTYLE_SOLID  1 
#define COLORSTYLE_CHECKER  2 
#define COLORSTYLE_BUMBLEBEE  3 

//Used in TSphere, TCylinder : for position object
#define POSITION_BOTTOM  1 
#define POSITION_CENTER  2 
#define POSITION_TOP  3 

//Used for Render() function in TCylinder, TSphere
#define WITH_LIGHTING  1 
#define NO_LIGHTING  0 

//Used for Render() function to indicate whether texturing is on or off
#define WITH_TEXTURING  1 
#define NO_TEXTURING  0 

//Used for texturing
#define TEXTURINGMODE_WRAPAROUND  1 
#define TEXTURINGMODE_PROJECTONTO_LINEAR  2 
#define TEXTURINGMODE_PROJECTONTO_QUADRATICATTENUATION  3 
#define TEXTURINGMODE_PROJECTONTO_EXPONENTIALATTENUATION  4 
#define TEXTURINGMODE_PROJECTONTO_EXPONENTIALSQUAREATTENUATION  5 
#define TEXTURINGMODE_WRAP_LIKE_SPHERE  6 
#define TEXTURINGMODE_WRAP_LIKE_CYLINDER	7 

//Used in TArrow : for ArrowStyle
#define ARROWSTYLE_1D_WITHTIP  1 
#define ARROWSTYLE_3D_LINETOCONE  2 
#define ARROWSTYLE_3D_BALLTOLINETOCONE  3 
#define ARROWSTYLE_3D_BALLANDCONE  4 
#define ARROWSTYLE_3D_CONE  5 
#define ARROWSTYLE_1D_NOTIP  6 
#define ARROWSTYLE_1D  7 

//Used in TCoordinateSystem : for CoordStyle
#define COORDSTYLE_1D				1 
#define COORDSTYLE_3D				2 
#define COORDSTYLE_1D_AND_3D		3



//Used in TCoordinateSystem : for CoordMode
#define COORDMODE_POINTSINSIDE  1 				//Points inside the coordinate system but axis are still present
#define COORDMODE_LINESINSIDE  2 					//Lines inside the coordinate system with axis always present
#define COORDMODE_POINTSANDLINESINSIDE  3 		//Both are present

//Used in arrays or other places to specify primitive (simple polygons) types
#define PRIMITIVE_TRIANGLES  1 
#define PRIMITIVE_TRIANGLE_STRIP  2 
#define PRIMITIVE_TRIANGLE_FAN  3 
#define PRIMITIVE_QUADS  4 
#define PRIMITIVE_QUAD_STRIP  5 
#define PRIMITIVE_LINES  6 
#define PRIMITIVE_LINE_STRIP  7 
#define PRIMITIVE_LINE_LOOP  8 
#define PRIMITIVE_POINTS  9 
#define PRIMITIVE_POLYGON  10 

//Used in TFont::PolygonType to specify whether if a line is to be traced from outside
//to the center of the polygon, if the number of times it crosses the polygon outline
//is even or odd.
#define POLYGONTYPE_ODD  1 
#define POLYGONTYPE_EVEN  2 

//Used in TBoundingBox
#define BOXSTYLE_1D_RECTANGULAR_WIRE  1 		//The bounding box looks like a wireframe with _WIRE
#define BOXSTYLE_3D_RECTANGULAR_WIRE  2 
#define BOXSTYLE_1D_SPHERICAL_WIRE  3 
#define BOXSTYLE_3D_SPHERICAL_WIRE  4 
#define BOXSTYLE_2D_RECTANGULAR_FILL  5 		//The bounding box looks like a complete box with _FILL...
#define BOXSTYLE_2D_SPHERICAL_FILL  6 		//... and blending should be turned on to see the object inside.

//Used in TExtrudeXYFace for ExtrudeFace function
#define EXTRUDESTYLE_SIMPLE						1	//A flat extrusion
#define EXTRUDESTYLE_POLYLINE					2	//A polyline is used on extrusion
#define EXTRUDESTYLE_BEZIERSPLINE				3	//A bezier spline is used on extrusion

//Used in TExtrudeXYFace for ExtrudeFace function
#define EXTRUDEDCORNER_SIMPLE					1	//Corners angle is computed, and vector is traced out from corner, etc
#define EXTRUDEDCORNER_FLAT						2	//Corner is given a flat surface, will not work for corners with angle <90 degree
#define EXTRUDEDCORNER_ROUNDED					3	//Corner is given a round fillet, this is the best solution

#endif //OBJECTSDEFINES_H


