#include "common_afx.h"
#include "m_gaussiankernel.h"

//***************************************************************************
// Private Functions
//***************************************************************************

//---------------------------------------------------------------------------
static double
LostLight(int taps, double standardDeviation)
{
	int halfTaps = (taps - 1) / 2 + 1;

	double sum = Gaussian(0, standardDeviation);
	for (int i = 1; i < halfTaps; i++)
		sum += Gaussian(i, standardDeviation) * 2;

	return -1.0 + (1.0 / sum);
}

//---------------------------------------------------------------------------
int GetWeightsInternal( int taps, double standardDeviation,
					   double* outWeights, int uiWeightsMax )
{
	int halfTaps = (taps - 1) / 2 + 1;

	if ( outWeights != NULL )
	{
		for (int i = 0; (i < halfTaps) && (i < uiWeightsMax); i++)
			outWeights[i] = Gaussian(i, standardDeviation);

		double augmentationFactor = 1 + LostLight(taps, standardDeviation);
		for (int i = 0; (i < halfTaps) && (i < uiWeightsMax); i++)
			outWeights[i] *= augmentationFactor;
	}

	return halfTaps;
}

//===========================================================================
// Public Interface
//===========================================================================


//---------------------------------------------------------------------------
extern int
GaussianKernel_GetFastWeights( int taps, double standardDeviation,
							  float* outWeights, int uiWeightsMax,
							  float* outOffsets, int uiOffsetsMax )
{
	if (taps < 1 || taps % 2 == 0)
	{
		E_ASSERT( !"Invalid tap count" );
		return 0;
	}

	int uiTempWeightsCount = GetWeightsInternal(taps, standardDeviation, NULL, 0);

	bool hasEndTap  = ((uiTempWeightsCount % 2) == 0);
	int  joinedTaps = ((uiTempWeightsCount - 1) / 2);
	int  totalTaps  = ((joinedTaps    + 1) + (hasEndTap ? 1 : 0));

	if ( outWeights == NULL || outOffsets == NULL )
	{
		E_ASSERT( (outWeights == NULL) && (outOffsets == NULL) );
		return totalTaps;
	}

	// Hmm.. I don't like how the algorithm requires dynamic allocation;
	// however I don't have any time to think of something better.
	// Auto-delete the temp weight array.
	double* pTempWeights = E_NEW(double)[ uiTempWeightsCount ];
	DeleteArrayOnLeavingThisScope<double> cDelTempWeights( pTempWeights );

	GetWeightsInternal(taps, standardDeviation,
		pTempWeights, uiTempWeightsCount );

	if (uiWeightsMax > 0)
		outWeights[0] = (float)pTempWeights[0];

	if (uiOffsetsMax > 0)
		outOffsets[0] = 0.0F;

	int i = 0;
	for (; (i < joinedTaps); i++)
	{
		double sum = pTempWeights[i * 2 + 1] + pTempWeights[i * 2 + 2];

		if ( (i + 1) < uiWeightsMax )
			outWeights[i + 1] = (float)sum;

		if ( (i + 1) < uiOffsetsMax )
			outOffsets[i + 1] = (float)(0.5 - pTempWeights[i * 2 + 1] / sum);
	}

	if (hasEndTap && (i < uiWeightsMax))
	{
		E_ASSERT( totalTaps > 0 );
		if ( totalTaps > 0 )
			outWeights[i] = (float)pTempWeights[totalTaps - 1];
	}

	return totalTaps;
}

//---------------------------------------------------------------------------
double
GaussianKernel_IdealStandardDeviation( int taps )
{
	switch ( taps )
	{
	case 5:		return 7.013915463849E-01;
	case 7:		return 8.09171316279E-01;
	case 9:		return 9.0372907227E-01;
	case 11:	return 9.890249035E-01;
	case 13:	return 1.067359295;
	case 15:	return 1.1402108;
	case 17:	return 1.2086;
	}

	E_ASSERT( !"Unknown ideal standard deviation for specified taps." );
	return 0.0F;
}
