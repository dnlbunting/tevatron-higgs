#include <TChain.h>
#include <TFile.h>
#include <TMVA/Factory.h>
#include <TMVA/Types.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TApplication.h>
void fitNN()
{

	/* Load up the signal and background data files */

    //Use a Higgs signal at a mass of  110 GeV   
	TChain * signal = new TChain("higgsSearch","");
    signal->Add("LHinput-cut_2b25_1j15-Tight-MC_higgs110GeV.root/higgsSearch");

    //Use a background of multijet bbb events
    TChain *background = new TChain("higgsSearch","");
    background->Add("LHinput-cut_2b25_1j15-Tight-MC_bbb.root/higgsSearch");
	
	
    Double_t nnoutxlm[4], MH[6], dEta[6], dPhi[6], Angle[6], EtaH[6], pBalance[6], Sphericity;
    Double_t dphi, deta, angle, etah, pbalance, sphericity; 
    Int_t Njets, typeH[6], typeO, nsignal, nbkg;

    signal->SetBranchAddress("MH", 		 	&MH);
    signal->SetBranchAddress("dEta",  		&dEta);
    signal->SetBranchAddress("dPhi",  		&dPhi);
    signal->SetBranchAddress("Eta1mEtaH", 	&Angle);
    signal->SetBranchAddress("EtaH",  		&EtaH);
    signal->SetBranchAddress("type",		&typeH);
    signal->SetBranchAddress("pBalance", 	&pBalance);
    signal->SetBranchAddress("Njets", 		&Njets);
    signal->SetBranchAddress("FW_H2", 		&Sphericity);

    //same for background
    background->SetBranchAddress("MH",  		&MH);
    background->SetBranchAddress("dEta",    	&dEta);
    background->SetBranchAddress("dPhi",  		&dPhi);
    background->SetBranchAddress("Eta1mEtaH", 	&Angle);
    background->SetBranchAddress("EtaH", 		&EtaH);
    background->SetBranchAddress("pBalance", 	&pBalance);
    background->SetBranchAddress("Njets", 		&Njets);
    background->SetBranchAddress("FW_H2", 		&Sphericity);\
		
	/*	Initialise an output file */
	TFile *outputFile = new TFile("output.root", "RECREATE");
	
		
	TMVA::Factory* factory = new TMVA::Factory( "JobName", outputFile, "" );
	
	factory->AddSignalTree    ( signal,  1.0  );
	factory->AddBackgroundTree( background, 1.0 );
	
	factory->AddVariable( "dEta",            'F' );
	factory->AddVariable( "dPhi",            'F' );
	factory->AddVariable( "pBalance",            'F' );
	
	factory->BookMethod( TMVA::Types::kMLP, "MLP_ANN", "" );
	
	factory->TrainAllMethods();
	factory->TestAllMethods();
	factory->EvaluateAllMethods();
}




int main (int argc, char  *argv[])
{
	TApplication *theApp = new TApplication("app", &argc, argv); 
	fitNN();
	 theApp->Run();
	return 0;
}

