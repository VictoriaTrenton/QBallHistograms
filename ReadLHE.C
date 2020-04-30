//**************************************************************************************************************
//	Filename:		ReadLHE.C
//	Reviser:		Victoria Trenton
//	Last update:	Aug. 24, 2012
// 
// This script creates histograms for a particular charge and writes them to an output ROOT file.
//**************************************************************************************************************

#define ReadLHE_cxx

// Load header file
#include "ReadLHE.h"

// Load libraries
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <sstream>
using namespace std;

//=====================================================================
// Create histograms for a particular charge and write them to an output ROOT file
//=====================================================================
void ReadLHE::Loop (float charge)
{
//   In a ROOT session, you can do:
//      Root > .L ReadLHE.C
//      Root > ReadLHE t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

	// If reached end of chain, exit loop
	if (fChain == 0)
		return;
	
	// Create histograms
	TH1F* h_eta = new TH1F("h_eta",";Pseudorapidty #eta;Events/(0.1 units)",60,-3.0,3.0);
	TH1F* h_eta_cut = new TH1F("h_eta_cut",";Pseudorapidity #eta;Events/(0.1 units)",60,-3.0,3.0);
	TH1F* h_E = new TH1F("h_E",";Energy E [GeV];Events/(10 GeV)",100,0.0,1000.);
	TH1F* h_Ek = new TH1F("h_Ek",";Kinetic Energy E_{K} [GeV];Events/(10 GeV)",150,0.0,1500.);
	TH1F* h_ET = new TH1F("h_ET",";Transverse Energy E_{T} [GeV];Events/(10 GeV)",150,0.0,1500.);
	TH1F* h_pt = new TH1F("h_pt",";Transverse Momentum p_{T} [GeV/c];Events/(10 GeV/c)",100,0.0,1000.);
	TH1F* h_gamma = new TH1F("h_gamma",";Relativistic #gamma factor;Events/(0.1 units)",80,0.0,8.);
	TH1F* h_beta = new TH1F("h_beta",";Relativistic velocity #beta;Events/(0.01 units)",100,0.0,1.);
	TH2F* h_ek_eta = new TH2F("h_ek_eta",";Pseudorapidity #eta; Kinetic Energy E_{K} [GeV]",44,-2.2,2.2,150, 0.0, 1500.0);
	
	//----------------------------------------------------------------------------------------------------------------------------------
	// Store PDG ID (PID) based on the particle's charge
	// Reference: http://pdg.lbl.gov/2012/reviews/rpp2012-rev-monte-carlo-numbering.pdf
	// Note:	General PID format is 7 digits:		+/- n nr nL q1 q2 q3 nJ
	// 		For muons:								 13
	//----------------------------------------------------------------------------------------------------------------------------------
	int pid;
	
	// If charge is 1, particle is a muon with PID#13
	if (charge == 1)
		pid = 13;									// Store PID
		
/*
	//----------------------------------------------------------------------------------------------------------------------------------
	// Store QBall PDG ID based on its charge. (Charge <= 999.9 with only one decimal place)
	// Note: PDG ID (PID) format is		100XXXY0 	where charge is XXX.Y
	//----------------------------------------------------------------------------------------------------------------------------------
	stringstream sStream;
	int pid, tenthsDigit;
	float tenthsNum, roundoffError;

	// Store first 3 digits of the PID
	sStream.str ("");									// Clear sStream
	sStream << "100";								// e.g. PID: 100_____

	// If charge has 2 significant figures before decimal place
	if (charge < 100 && charge >= 10)					// e.g. charge 23.7
		sStream << "0";								// PID: 1000____	
	// Else if charge has 1 or 0 significant figures before decimal place
	else if (charge < 10)								// e.g. charge 2.7 or 0.7
		sStream << "00";							// PID: 10000___
	
	// Find tenths digit
	tenthsNum = (charge - (int)charge) * 10;				// (2.7-2)*10 = 6.9999
	roundoffError = tenthsNum - (int)tenthsNum;			// 6.9999-6 = 0.9999
	
	// If tenths digit is stored as a number just below an integer, round up
	if (roundoffError > 0.5)
		tenthsDigit = (int)(ceil (tenthsNum));					// 6.9999->7
	// Else tenths digit is stored as a number just above an integer, so round down
	else
		tenthsDigit = (int)(floor (tenthsNum));					// 7.0001->7

	// Store all other digits of the PID					// e.g. charge 2.7
	sStream << (int)charge << tenthsDigit << "0";			// PID: 10000270
	sStream >> pid;									// Store PID
*/	
	//----------------------------------------------------------------------------------------------------------------------------------
	
	// Get the total number of entries (events) in the chain
	int ncount = 0;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	
	// Cycle through entries in the chain (total number of events)
	for (Long64_t jentry = 0; jentry < nentries; jentry++)
	{
		// Load tree from the chain
		Long64_t ientry = LoadTree (jentry);
		if (ientry < 0)
			break;
			
		// Add to total bytes read
		nb = fChain->GetEntry (jentry);			// Returns total number of bytes read
		nbytes += nb;
		// if (Cut(ientry) < 0) continue;
		ncount++;

		// Print statements to help find value of kMaxParticle
//		cout << "Particle_size: " << Particle_size << endl;
//		cout << "kMaxParticle: " << kMaxParticle << endl;
		
		// Cycle through total number of particles produced
		for (int ip = 0; ip < Particle_size; ip++)
		{
			// Correct certain masses to experimentally determiend values (in GeV/c^{2})
			// (Note: This is a fix for particles inaccurately assigned a mass of zero in the
			// MadGraph model file "particles.dat" used to generate the input ROOT file.)
			if (abs (Particle_PID[ip]) == 1)				// Down quark
				Particle_M[ip] = 0.0049;
			else if (abs (Particle_PID[ip]) == 2)			// Up quark
				Particle_M[ip] = 0.0024;
			else if (abs (Particle_PID[ip]) == 11)			// Electron
				Particle_M[ip] = 0.0005110;
			else if (abs (Particle_PID[ip]) == 13)			// Muon
				Particle_M[ip] = 0.1057;				
				
			// If nondecayed particle or antiparticle has the given PID, fill histograms with its
			// leaf data. (Note: Particles have pos. PID; antiparticles have neg. PID.)
			if (abs (Particle_PID[ip]) == pid && Particle_Status[ip] == 1)
			{			
				h_eta->Fill ( (float) Particle_Eta[ip] );
				h_pt->Fill ( (float) Particle_PT[ip] );
				h_E->Fill ( (float) Particle_E[ip] );
				Double_t EK = Particle_E[ip] - Particle_M[ip];
				h_Ek->Fill ( (float) EK);
				Double_t theta = 2.0 * atan (exp (- Particle_Eta[ip] ) );
				Double_t ET = EK * sin(theta);
				h_ET->Fill ( (float) ET );				
				Double_t gamma = Particle_E[ip] / Particle_M[ip];
				h_gamma->Fill ( (float) gamma );			
				Double_t beta = sqrt(1 - 1 / (gamma * gamma));		//(gamma**2));
				h_beta->Fill ( (float) beta );		
				h_ek_eta->Fill ( (float) Particle_Eta[ip], (float) (Particle_E[ip] - Particle_M[ip]) );
				
				if ( fabs (Particle_Eta[ip]) < 2.2 )
					h_eta_cut->Fill ( (float) Particle_Eta[ip] );
			} 

		}
	}
	
	// Create ROOT file
	TFile* file = new TFile ("OUT_histos.root", "recreate");
	
	// Set entries for each histogram's statistics box.
	// Reference TStyle::SetOptStat from http://www-hades.gsi.de/docs/hydra/
	// classDocumentation/doxy_dev/root52800b/html/TStyle_8cxx-source.html
	gStyle->SetOptStat (111110); // Overflows,underflows,RMS,Mean,# Entries,No histo name
	
	// Write histograms to ROOT file
	h_eta->Write();
	h_eta_cut->Write();
	h_E->Write();
	h_Ek->Write();
	h_ET->Write();
	h_pt->Write();
	h_ek_eta->Write();
	h_gamma->Write();
	h_beta->Write();
	 
}
