//**************************************************************************************************************
//	Filename:		ReadLHE.h
//	Reviser:		Victoria Trenton
//	Last update:	Aug. 1, 2012
// 
// This is a header file for ReadLHE.C. Before running ReadLHE.C, the user should change
// kMaxParticle to the appropriate value.
//
// This class has been automatically generated on
// Wed Jul  6 15:37:46 2011 by ROOT version 5.26/00
// from TTree LHEF/Analysis tree
// found on file: MGDYMonopole_M1000.root
//**************************************************************************************************************

#ifndef ReadLHE_h
#define ReadLHE_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Stop printing of error messages. Used to suppress the warning "No dictionary for class"
gErrorIgnoreLevel = kError;

// Note: kMaxParticle is the maximum value of Particle_size (total number of particles produced)
// over all events
const Int_t kMaxEvent = 1;
const Int_t kMaxParticle = 22;		// For QBalls: 4, Leptosusy: 22

class ReadLHE
{
	public :
	TTree          *fChain;   //!pointer to the analyzed TTree or TChain
	Int_t           fCurrent; //!current Tree number in a TChain

	// Declaration of leaf types
	Int_t           Event_;
	UInt_t          Event_fUniqueID[kMaxEvent];   //[Event_]
	UInt_t          Event_fBits[kMaxEvent];   //[Event_]
	Long64_t        Event_Number[kMaxEvent];   //[Event_]
	Int_t           Event_Nparticles[kMaxEvent];   //[Event_]
	Int_t           Event_ProcessID[kMaxEvent];   //[Event_]
	Double_t        Event_Weight[kMaxEvent];   //[Event_]
	Double_t        Event_ScalePDF[kMaxEvent];   //[Event_]
	Double_t        Event_CouplingQED[kMaxEvent];   //[Event_]
	Double_t        Event_CouplingQCD[kMaxEvent];   //[Event_]
	Int_t           Event_size;
	Int_t           Particle_;
	UInt_t          Particle_fUniqueID[kMaxParticle];   //[Particle_]
	UInt_t          Particle_fBits[kMaxParticle];   //[Particle_]
	Int_t           Particle_PID[kMaxParticle];   //[Particle_]
	Int_t           Particle_Status[kMaxParticle];   //[Particle_]
	Int_t           Particle_Mother1[kMaxParticle];   //[Particle_]
	Int_t           Particle_Mother2[kMaxParticle];   //[Particle_]
	Int_t           Particle_ColorLine1[kMaxParticle];   //[Particle_]
	Int_t           Particle_ColorLine2[kMaxParticle];   //[Particle_]
	Double_t        Particle_Px[kMaxParticle];   //[Particle_]
	Double_t        Particle_Py[kMaxParticle];   //[Particle_]
	Double_t        Particle_Pz[kMaxParticle];   //[Particle_]
	Double_t        Particle_E[kMaxParticle];   //[Particle_]
	Double_t        Particle_M[kMaxParticle];   //[Particle_]
	Double_t        Particle_PT[kMaxParticle];   //[Particle_]
	Double_t        Particle_Eta[kMaxParticle];   //[Particle_]
	Double_t        Particle_Phi[kMaxParticle];   //[Particle_]
	Double_t        Particle_Rapidity[kMaxParticle];   //[Particle_]
	Double_t        Particle_LifeTime[kMaxParticle];   //[Particle_]
	Double_t        Particle_Spin[kMaxParticle];   //[Particle_]
	Int_t           Particle_size;
	
	// List of branches
	TBranch        *b_Event_;   //!
	TBranch        *b_Event_fUniqueID;   //!
	TBranch        *b_Event_fBits;   //!
	TBranch        *b_Event_Number;   //!
	TBranch        *b_Event_Nparticles;   //!
	TBranch        *b_Event_ProcessID;   //!
	TBranch        *b_Event_Weight;   //!
	TBranch        *b_Event_ScalePDF;   //!
	TBranch        *b_Event_CouplingQED;   //!
	TBranch        *b_Event_CouplingQCD;   //!
	TBranch        *b_Event_size;   //!
	TBranch        *b_Particle_;   //!
	TBranch        *b_Particle_fUniqueID;   //!
	TBranch        *b_Particle_fBits;   //!
	TBranch        *b_Particle_PID;   //!
	TBranch        *b_Particle_Status;   //!
	TBranch        *b_Particle_Mother1;   //!
	TBranch        *b_Particle_Mother2;   //!
	TBranch        *b_Particle_ColorLine1;   //!
	TBranch        *b_Particle_ColorLine2;   //!
	TBranch        *b_Particle_Px;   //!
	TBranch        *b_Particle_Py;   //!
	TBranch        *b_Particle_Pz;   //!
	TBranch        *b_Particle_E;   //!
	TBranch        *b_Particle_M;   //!
	TBranch        *b_Particle_PT;   //!
	TBranch        *b_Particle_Eta;   //!
	TBranch        *b_Particle_Phi;   //!
	TBranch        *b_Particle_Rapidity;   //!
	TBranch        *b_Particle_LifeTime;   //!
	TBranch        *b_Particle_Spin;   //!
	TBranch        *b_Particle_size;   //!
	
	ReadLHE(TTree *tree=0);
	
	virtual ~ReadLHE();
	virtual Int_t    Cut(Long64_t entry);
	virtual Int_t    GetEntry(Long64_t entry);
	virtual Long64_t LoadTree(Long64_t entry);
	virtual void     Init(TTree *tree);
	virtual void     Loop (float charge);
	virtual Bool_t   Notify();
	virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ReadLHE_cxx
ReadLHE::ReadLHE(TTree *tree)
{
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	if (tree == 0)
	{	
		#ifdef SINGLE_TREE
		// The following code should be used if you want this class to access
		// a single tree instead of a chain
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
		
		if (!f)
		{
			f = new TFile("Memory Directory");
			f->cd("Rint:/");
		}

		//tree = (TTree*)gDirectory->Get("LHEF");
		tree = (TTree*)gDirectory->Get("LHEF;2");
		
		#else // SINGLE_TREE
		// The following code should be used if you want this class to access a chain
		// of trees.
		TChain * chain = new TChain("LHEF","");
		chain->Add("ntuplesdir/*.root");
		tree = chain;

		#endif // SINGLE_TREE
	}

	Init(tree);
}

ReadLHE::~ReadLHE()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t ReadLHE::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

Long64_t ReadLHE::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
	if (!fChain) return -5;

	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (!fChain->InheritsFrom(TChain::Class()))  return centry;

	TChain *chain = (TChain*)fChain;
	if (chain->GetTreeNumber() != fCurrent)
	{
		fCurrent = chain->GetTreeNumber();
		Notify();
	}
	return centry;
}

void ReadLHE::Init(TTree *tree)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).
	
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);
		
	fChain->SetBranchAddress("Event", &Event_, &b_Event_);
	fChain->SetBranchAddress("Event.fUniqueID", Event_fUniqueID, &b_Event_fUniqueID);
	fChain->SetBranchAddress("Event.fBits", Event_fBits, &b_Event_fBits);
	fChain->SetBranchAddress("Event.Number", Event_Number, &b_Event_Number);
	fChain->SetBranchAddress("Event.Nparticles", Event_Nparticles, &b_Event_Nparticles);
	fChain->SetBranchAddress("Event.ProcessID", Event_ProcessID, &b_Event_ProcessID);
	fChain->SetBranchAddress("Event.Weight", Event_Weight, &b_Event_Weight);
	fChain->SetBranchAddress("Event.ScalePDF", Event_ScalePDF, &b_Event_ScalePDF);
	fChain->SetBranchAddress("Event.CouplingQED", Event_CouplingQED, &b_Event_CouplingQED);
	fChain->SetBranchAddress("Event.CouplingQCD", Event_CouplingQCD, &b_Event_CouplingQCD);
	fChain->SetBranchAddress("Event_size", &Event_size, &b_Event_size);
	fChain->SetBranchAddress("Particle", &Particle_, &b_Particle_);
	fChain->SetBranchAddress("Particle.fUniqueID", Particle_fUniqueID, &b_Particle_fUniqueID);
	fChain->SetBranchAddress("Particle.fBits", Particle_fBits, &b_Particle_fBits);
	fChain->SetBranchAddress("Particle.PID", Particle_PID, &b_Particle_PID);
	fChain->SetBranchAddress("Particle.Status", Particle_Status, &b_Particle_Status);
	fChain->SetBranchAddress("Particle.Mother1", Particle_Mother1, &b_Particle_Mother1);
	fChain->SetBranchAddress("Particle.Mother2", Particle_Mother2, &b_Particle_Mother2);
	fChain->SetBranchAddress("Particle.ColorLine1", Particle_ColorLine1, &b_Particle_ColorLine1);
	fChain->SetBranchAddress("Particle.ColorLine2", Particle_ColorLine2, &b_Particle_ColorLine2);
	fChain->SetBranchAddress("Particle.Px", Particle_Px, &b_Particle_Px);
	fChain->SetBranchAddress("Particle.Py", Particle_Py, &b_Particle_Py);
	fChain->SetBranchAddress("Particle.Pz", Particle_Pz, &b_Particle_Pz);
	fChain->SetBranchAddress("Particle.E", Particle_E, &b_Particle_E);
	fChain->SetBranchAddress("Particle.M", Particle_M, &b_Particle_M);
	fChain->SetBranchAddress("Particle.PT", Particle_PT, &b_Particle_PT);
	fChain->SetBranchAddress("Particle.Eta", Particle_Eta, &b_Particle_Eta);
	fChain->SetBranchAddress("Particle.Phi", Particle_Phi, &b_Particle_Phi);
	fChain->SetBranchAddress("Particle.Rapidity", Particle_Rapidity, &b_Particle_Rapidity);
	fChain->SetBranchAddress("Particle.LifeTime", Particle_LifeTime, &b_Particle_LifeTime);
	fChain->SetBranchAddress("Particle.Spin", Particle_Spin, &b_Particle_Spin);
	fChain->SetBranchAddress("Particle_size", &Particle_size, &b_Particle_size);
	
	Notify();
}

Bool_t ReadLHE::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. It is normally not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed. The return value is currently not used.
	
	return kTRUE;
}

void ReadLHE::Show(Long64_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}

Int_t ReadLHE::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}

#endif // #ifdef ReadLHE_cxx

