//**************************************************************************************************************
//	Filename:		RunMonoPlots.C
//	Reviser:		Victoria Trenton
// 
// This script calls the Loop function of ReadLHE.C for a particular charge.
//**************************************************************************************************************

// Load class
#include "ReadLHE.C"

int RunMonoPlots (float charge)
{
	//Make histograms
	ReadLHE *t = new ReadLHE (NULL);
	t->Loop (charge);
	
	gROOT->ProcessLine (".q");
	return 0;
}
