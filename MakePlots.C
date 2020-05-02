//**************************************************************************************************************
//	Filename:	MakePlots.C
//	Author:		Victoria Trenton
// 
// This script generates picture files of histograms for the specified variables. The files allow
// easy comparison between all masses of a particular charge or all charges of a particular mass.
//
// To compile this script using ROOT (for debugging), first comment out the line
//		gROOT->ProcessLine (".q");
// Then type this on the command line:
//		root -l
//		.L MakePlots.C+
// Or in one line, type
//		root -l -L MakePlots.C+
//
// Before running this script, the user should make sure there is a directory called "Plots" and
// that all ROOT files to be accessed are in the "Results" directory.
// To run, type this on the command line:
// 		root -l MakePlots.C
//**************************************************************************************************************

// Load C++ libraries
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
using namespace std;

// Load ROOT libraries
#include <TH1.h>
#include <TFile.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLatex.h>

//------------------------------------------------------------------------------------------------------------------------------------------
// User can initialize global values for charge, mass, and histogram type
//------------------------------------------------------------------------------------------------------------------------------------------
// For QBalls and Leptosusy
//const int numTypes = 1;
//string type[numTypes] = {"eta"};
const int numTypes = 4;
string type[numTypes] = {"eta", "Ek", "gamma", "beta"};

// For QBalls
//const int numCharges = 1;
//float charge[numCharges] = {6};
//const int numCharges = 5;
//float charge[numCharges] = {2, 3, 4, 5, 6};
//const int numCharges = 3;
//float charge[numCharges] = {0.3, 0.7, 1.0};

// For Leptosusy
const int numCharges = 1;
float charge[numCharges] = {1};

// For QBalls
//const int numMasses = 1;
//int mass[numMasses] = {50};
//const int numMasses = 7;
//int mass[numMasses] = {50, 100, 200, 300, 400, 500, 600};
//const int numMasses = 3;
//int mass[numMasses] = {50, 100, 250};

// For Leptosusy (Option: Data structure)
const int numMasses = 9;
struct set
{
	int ms, mn, mg;
} massSet[numMasses];

/*
// For Leptosusy (Option: Arrays)
const int numMasses = 1;
int ms[numMasses] = {1500};
int mn[numMasses] = {600};
int mg[numMasses]= {1200};

const int numMasses = 9;
int ms[numMasses] = {1500,  1000, 10000,    500,     500,     500,  1000,   1000, 1000};
int mn[numMasses] = {  600,   600,     600,    200,     600,   1000,    200,     600, 1000};
int mg[numMasses] = {1200,  2000,   1000, 10000, 10000, 10000, 10000, 10000, 10000};
*/
//------------------------------------------------------------------------------------------------------------------------------------------

// Declare global variables
stringstream sStream;
string chargeString[numCharges];
string massString[numMasses];
TH1F* histogram[numCharges][numMasses][numTypes]; // Matrix to reference histograms

// Declare functions
int getTenthsDigit (float chargeVal);
void createChargeString();
void createMassString();
void getHistograms();
void makeMassComparisonPlots (int numRows, int numCols, int xScale, int yScale);
void makeChargeComparisonPlots (int numRows, int numCols, int xScale, int yScale);


//=====================================================================
// Create mass and charge comparison plots for each type of histogram
//=====================================================================
void MakePlots()
{
	//----------------------------------------------------------------------------------------------------------------------------------
	// For Leptosusy, user can change values for each mass set {Ms, Mn, Mg}.
	// User can define the histogram grid of each canvas and set plot resolution.
	// Make sure there is room on the histogram grid for the canvas title:
	// (numMasses + 1) <= (numRowsMass * numColsMass)
	// (numCharges + 1) <= (numRowsCharge * numColsCharge).
	//----------------------------------------------------------------------------------------------------------------------------------
	// Assign mass set values for Leptosusy
	massSet[0].ms = 1500;	massSet[0].mn = 600;	massSet[0].mg = 1200;
	massSet[1].ms = 1000;	massSet[1].mn = 600;	massSet[1].mg = 2000;
	massSet[2].ms = 10000;	massSet[2].mn = 600;	massSet[2].mg = 1000;
	massSet[3].ms = 500;	massSet[3].mn = 200;	massSet[3].mg = 10000;
	massSet[4].ms = 500;	massSet[4].mn = 600;	massSet[4].mg = 10000;
	massSet[5].ms = 500;	massSet[5].mn = 1000;	massSet[5].mg = 10000;
	massSet[6].ms = 1000;	massSet[6].mn = 200;	massSet[6].mg = 10000;
	massSet[7].ms = 1000;	massSet[7].mn = 600;	massSet[7].mg = 10000;
	massSet[8].ms = 1000;	massSet[8].mn = 1000;	massSet[8].mg = 10000;

	// For QBalls
	//int numRowsMass = 4, numColsMass = 2;			// Mass comparison grid
	//int numRowsCharge = 3, numColsCharge = 2;			// Charge comparison grid
	//int numRowsMass = 2, numColsMass = 2;			// Mass comparison grid
	//int numRowsCharge = 2, numColsCharge = 2;			// Charge comparison grid
	
	// For Leptosusy
	//int numRowsMass = 1, numColsMass = 2;			// Mass set comparison grid
	//int numRowsCharge = 1, numColsCharge = 2;			// Charge comparison grid
	int numRowsMass = 4, numColsMass = 3;				// Mass set comparison grid
	int numRowsCharge = 1, numColsCharge = 2;			// Charge comparison grid
	
	int xScalePlot = 400, yScalePlot = 300;				// Plot scaling factors
	//----------------------------------------------------------------------------------------------------------------------------------
	
	float histoTitleXPos = 0.1;							// Pad bottom left: (0,0)
	float histoTitleYPos = 0.1;							// Pad top right: (1,1)
	
	// Set entries for histogram's statistics box. Reference TStyle::SetOptStat from
	// http://www-hades.gsi.de/docs/hydra/classDocumentation/doxy_dev/root52800b/html/
	// TStyle_8cxx-source.html
	gStyle->SetOptStat (1110);				// RMS, Mean, # Entries, No histogram name
	
	// Set default (g: global) style options
	gStyle->SetOptTitle (1);				// Show histogram title
	gStyle->SetTitleBorderSize (0);
	gStyle->SetTitleFillColor (10);
	
	// Set position of histogram titles
	gStyle->SetTitleX (histoTitleXPos);
	gStyle->SetTitleY (histoTitleYPos);

	createChargeString();					// Compensate for fractional charges
	createMassString();					// Create label for each mass set
	getHistograms();					// Store histograms in matrix

	// Generate picture files
	makeMassComparisonPlots (numRowsMass, numColsMass, xScalePlot, yScalePlot);
	makeChargeComparisonPlots (numRowsCharge, numColsCharge, xScalePlot, yScalePlot);
	
	gROOT->ProcessLine (".q");			// Quit ROOT
}

//=====================================================================
// Create historgrams for each combination of charge, mass, and type
//=====================================================================
void getHistograms()
{
	// Cycle through charges
	for (int i = 0; i < numCharges; i++)
	{
		// Cycle through masses
        	for (int j = 0; j < numMasses; j++)
		{
			string sampleIdentifier, rootPath;
			TFile* rootFile;
			
			// Create identifier for sample's particular charge and mass set
			sStream.str ("");								// Clear sStream
//			sStream << "qball" << chargeString[i] << "_m" << massString[j];
//			sStream << "qball" << chargeString[i] << "_m" << massString[j] << "_b4000";
			sStream << "Leptosusy_"<< massString[j];
			sampleIdentifier = sStream.str();	     // e.g. Leptosusy_Ms1500_Mn600_Mg1200
			
			// Load ROOT file generated from the GenPlots_MGDY.sh script
			// e.g. Results/Leptosusy_Ms1500_Mn600_Mg1200_histos.root
			sStream.str ("");
			sStream << "Results/" << sampleIdentifier << "_histos.root";
			rootPath = sStream.str();
			rootFile = new TFile (rootPath.c_str());	 // Convert string to array of characters				
			//rootFile = new TFile (rootPath.c_str(), "READONLY");     // Alternative
			//rootFile = (TFile*) TFile::Open (rootPath.c_str());		     // If over secure shell		// TEST THIS!!!
			
			// Cycle through histogram types
			for (int k = 0; k < numTypes; k++)
			{
				string histoNameOld, histoName, histoTitle;
				TH1F* histo;

				// Get histogram from ROOT file
				sStream.str ("");
				sStream << "h_" << type[k];
				histoNameOld = sStream.str();			// e.g. h_eta
				histo = (TH1F*) rootFile->Get (histoNameOld.c_str());
				
				// Set name of histogram (Unneeded unless histogram is referenced)
				// e.g. h_eta_Leptosusy_Ms1500_Mn600_Mg1200
				sStream << "_" << sampleIdentifier;
				histoName = sStream.str();	
				histo->SetName (histoName.c_str());
				
				// Store histogram in matrix
				histogram[i][j][k] = histo;
			}
		}
	}
}

//=====================================================================
// Generate picture files of each type containing all masses of a particular charge
//=====================================================================
void makeMassComparisonPlots (int numRows, int numCols, int xScale, int yScale)
{
	// Create canvas for drawing histograms onto
	int width = numCols*xScale;
	int height = numRows*yScale;
	TCanvas* c1 = new TCanvas ("c1", "", width, height);
	
	// Cycle through histogram types
	for (int k = 0; k < numTypes; k++)
	{
		// Cycle through charges
		for (int i = 0; i < numCharges; i++)
		{
			string picTitle, filePathNoExt, pngPath, epsPath;
			TLatex* picTitleBox;
			float picTitleXPos = 0.4;					// Pad bottom left: (0,0)
			float picTitleYPos = 0.5;					// Pad top right: (1,1)
			float picTitleSize = 0.1;					// Picture title text size
			
			// Divide canvas into pads
			c1->Clear();
//			c1->SetWindowSize (width, height);
			c1->Divide (numCols, numRows);

			// Cycle through masses
        		for (int j = 0; j < numMasses; j++)
			{
				string histoTitle;
				
				// Set title of histogram
				sStream.str ("");
				//sStream << "Mass " << mass[j] << " GeV";	// Optional: " GeV/c^{2}";
				sStream << massString[j] << " GeV";
				histoTitle = sStream.str();	// e.g. Mass Set Ms1500_Mn600_Mg1200 GeV
				histogram[i][j][k]->SetTitle (histoTitle.c_str());
//				histogram[i][j][k]->SetLineColor(0);
				
				c1->cd (j+1);						// Set current pad
				histogram[i][j][k]->Draw();				// Draw histogram on pad
				
				// If on the last histogram, set current pad to the last pad
				if (j == numMasses-1)
					c1->cd (numMasses+2); // For QBalls: 1
			}
			
			// Draw picture title
			sStream.str ("");
			sStream << "Charge " << chargeString[i] << "#font[72]{e}";	// "e" in italics
			picTitle = sStream.str();						//e.g. Charge 6/e/
			picTitleBox = new TLatex (picTitleXPos, picTitleYPos, picTitle.c_str());
			picTitleBox->SetTextSize (picTitleSize);
			picTitleBox->Draw();					// Draw picture title on last pad
			
			// Store picture path without file extension
			sStream.str ("");
			//sStream << "Plots/" << type[k] << "_qball" << chargeString[i];
			//sStream << "Plots/" << type[k] << "_qball" << chargeString[i] << "_b4000";
			sStream << "Plots/" << type[k] << "_Leptosusy" << chargeString[i];
			filePathNoExt = sStream.str();
			
			// Create .png picture file
			sStream.str ("");
			sStream << filePathNoExt << ".png";
			pngPath = sStream.str();				// e.g. Plots/eta_Leptosusy1.png
			c1->Print (pngPath.c_str());
			
			// Create .eps picture file
			sStream.str ("");
			sStream << filePathNoExt << ".eps";
			epsPath = sStream.str();				// e.g. Plots/eta_Leptosusy1.eps
			c1->Print (epsPath.c_str());
			
			delete picTitleBox;
		}
	}
	
	delete c1;
}

//=====================================================================
// Generate picture files of each type containing all charges of a particular mass
//=====================================================================
void makeChargeComparisonPlots (int numRows, int numCols, int xScale, int yScale)
{
	// Create canvas for drawing histograms onto
	int width = numCols*xScale;
	int height = numRows*yScale;
	TCanvas* c1 = new TCanvas ("c1", "", width, height);
	
	// Cycle through histogram types
	for (int k = 0; k < numTypes; k++)
	{		
		// Cycle through masses
        	for (int j = 0; j < numMasses; j++)
		{
			string picTitle, filePathNoExt, pngPath, epsPath;
			TLatex* picTitleBox;
			float picTitleXPos = 0.1; // For QBalls: 0.3
			float picTitleYPos = 0.5;
			float picTitleSize = 0.09; // For QBalls: 0.1
			
			// Divide canvas into pads
			c1->Clear();
			c1->Divide (numCols, numRows);
		
  		     	// Cycle through charges
			for (int i = 0; i < numCharges; i++)
			{
				string histoTitle;
								
				// Set title of histogram
				sStream.str ("");
				sStream << "Charge " << chargeString[i] << "#font[72]{e}";	//e.g. Charge 6/e/
				histoTitle = sStream.str();
				histogram[i][j][k]->SetTitle (histoTitle.c_str());
				
				c1->cd (i+1);
				histogram[i][j][k]->Draw();
				
				// If on the last histogram, set current pad to the last pad
				if (i == numCharges-1)
					c1->cd (numCharges+1);
			}
			
			// Draw picture title
			sStream.str ("");
			//sStream << "Mass " << mass[j] << " GeV";		// Optional: " GeV/c^{2}";
			sStream << massString[j] << " GeV";
			picTitle = sStream.str();						// e.g. Mass 50 GeV
			picTitleBox = new TLatex (picTitleXPos, picTitleYPos, picTitle.c_str());
			picTitleBox->SetTextSize (picTitleSize);
			picTitleBox->Draw();
			
			// Store picture path without file extension
			sStream.str ("");
			//sStream << "Plots/" << type[k] << "_mass" << massString[j];
			//sStream << "Plots/" << type[k] << "_mass" << massString[j] << "_b4000";
			sStream << "Plots/" << type[k] << "_Leptosusy_" << massString[j];
			filePathNoExt = sStream.str();
			
			// Create .png picture file
			sStream.str ("");
			sStream << filePathNoExt << ".png";
			pngPath = sStream.str(); // e.g. Plots/eta_Leptosusy_Ms1500_Mn600_Mg1200.png
			c1->Print (pngPath.c_str());
			
			// Create .eps picture file
			sStream.str ("");
			sStream << filePathNoExt << ".eps";
			epsPath = sStream.str(); // e.g. Plots/eta_Leptosusy_Ms1500_Mn600_Mg1200.eps
			c1->Print (epsPath.c_str());
			
			delete picTitleBox;
		}
	}
	
	delete c1;
}

//=====================================================================
// When dealing with fractional charges, ensure a zero is in the tenths place for whole charges
// when storing them as strings.
// e.g. Store charges {2, 2.7} as {2.0, 2.7} for strings
// Note: PDG ID format is		100XXXY0		where charge is XXX.Y
//=====================================================================
void createChargeString()
{
	bool foundNonzeroTenths = false;
	
	// Cycle through charges
	for (int i = 0; i < numCharges; i++)				// e.g. charge = 2.7
	{
		// Note if at least one charge is fractional
		if (getTenthsDigit (charge[i]) != 0)
			foundNonzeroTenths = true;
	}
	
	// If a fractional charge was found, change format to store all charges with tenths digit
	if (foundNonzeroTenths)
	{
		sStream.setf (ios::fixed);
		sStream.precision (1);
	}
			
	// Cycle through charges
	for (int i = 0; i < numCharges; i++)
	{				
		// Store charge as a string
		sStream.str ("");
		sStream << charge[i];
		chargeString[i] = sStream.str();
	}
	
	// If format was changed, return it to default
	if (foundNonzeroTenths)
	{
		sStream.unsetf (ios::fixed);
		sStream.precision (6);
	}
	
/*
	// User can manually specify strings for charges
	// Cycle through charges
	for (int i = 0; i < numCharges; i++)
	{
		sStream.str ("");
		
		if (charge[i] == 1)
			sStream << "1.0";
		else
			sStream << charge[i];
			
		chargeString[i] = sStream.str();
	}
*/
}

//=====================================================================
// Find tenths digit of a charge
// Note: PDG ID format is		100XXXY0		where charge is XXX.Y
//=====================================================================
int getTenthsDigit (float chargeVal)
{
	float tenthsNum, roundoffError;
	int tenthsDigit;
	
	// Find tenths digit as a float						// e.g. charge = 2.7
	tenthsNum = (chargeVal - (int)chargeVal) * 10;			// (2.7-2)*10 = 6.9999
	roundoffError = tenthsNum - (int)tenthsNum;			// 6.9999-6 = 0.9999
	
	// If tenths digit is stored as a number just below an integer, round up
	if (roundoffError > 0.5)
		tenthsDigit = (int)(ceil (tenthsNum));				// 6.9999->7
	// Else tenths digit is stored as a number just above an integer, so round down
	else
		tenthsDigit = (int)(floor (tenthsNum));				// 7.0001->7
		
	return tenthsDigit;
}

//=====================================================================
// Define a string to represent the set of masses
// OR user can use code commented out:
// As necessary, add 1 or 2 zeroes to the beginning of each integer mass to give it 3 digits
//=====================================================================
void createMassString()
{
	// For Leptosusy (Option: Data structure)
	// Cycle through masses
	for (int j = 0; j < numMasses; j++)
	{
		sStream.str ("");
		sStream << "Ms" << massSet[j].ms << "_Mn" << massSet[j].mn << "_Mg"
				<< massSet[j].mg;
		massString[j] = sStream.str();				// e.g. Ms1500_Mn600_Mg1200
	}
	
/*
	// For Leptosusy (Option: Arrays)
	// Cycle through masses
	for (int j = 0; j < numMasses; j++)
	{
		sStream.str ("");
		sStream << "Ms" << ms[j] << "_Mn" << mn[j] << "_Mg" << mg[j];
		massString[j] = sStream.str();				// e.g. Ms1500_Mn600_Mg1200
	}
*/

		
/*
	// If filenames have prefixed zeroes (e.g. "m050") modify and implement the following code:
	
	// As necessary, add 1 or 2 zeroes to the beginning of each integer mass to give it 3 digits
	// Cycle through masses
	for (int j = 0; j < numMasses; j++)
	{
		sStream.str ("");
		
		// If 0 <= mass <= 9, prefix two zeroes to mass.
		if (mass[j] >= 0 && mass[j] <= 9)
			sStream << "00" << mass[j];					// e.g. 005
		// If 10 <= mass <= 99, prefix one zero to mass.
		else if (mass[j] >= 10 && mass[j] <= 99)
			sStream << "0" << mass[j];					// e.g. 050
		else
			sStream << mass[j];							// e.g. 500
		
		massString[j] = sStream.str();
	}
*/
}

/*




