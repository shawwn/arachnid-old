#pragma once

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Gaussian
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
E_INLINE double Gaussian(double distance, double standardDeviation)
{
	double len =  (Sqrt(2 * Pi) * standardDeviation);
	double num = -Pow(distance, 2);
	double den =  (2 * Pow(standardDeviation,2));

	return (1.0 / len) * Exp(num / den);
}

//===========================================================================
// GaussianKernel
//
//	Based on the "FullScreenShaders" project: 
//	http://theinstructionlimit.com/post-processingfullscreen-texture-sampling
//
//===========================================================================
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	IdealStandardDeviation( numTaps )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern double				GaussianKernel_IdealStandardDeviation( int taps );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GetWeights( taps, standardDeviation )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern const float*			GaussianKernel_GetWeights( int taps, double standardDeviation );
E_INLINE const float*		GaussianKernel_GetWeights( int taps )
{
	// use the ideal deviation for the specified number of taps.
	return GaussianKernel_GetWeights( taps, GaussianKernel_IdealStandardDeviation(taps) );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	GetFastWeights( taps, standardDeviation )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern int					GaussianKernel_GetFastWeights( int taps, double standardDeviation,
														  float* outWeights, int uiWeightsMax,
														  float* outOffsets, int uiOffsetsMax );
