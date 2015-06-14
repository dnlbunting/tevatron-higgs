
#include <TChain.h>
#include <TFile.h>
#include <TMVA/Factory.h>
#include <TMVA/Types.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TList.h>
#include <TGraph.h>
#include <TMath.h>
#include <TF1.h>
#include<iostream>
#include <TObjArray.h>
#include <TMultiLayerPerceptron.h>


#include "createTraining.cpp"
#include "TrainNetwork.cpp"
#include "evaluate.cpp"



void run(MCData_t data, TTree* mlp_training, TString* ssp, Int_t * n, TTree* evaluate, Double_t* auc, Double_t* max_sig)
{
	
	TString fname = "plots/" + ssp->ReplaceAll(TString(':'), 1, TString('_'), 1) + "," + TString::Itoa(*n, 10);
	ssp->ReplaceAll(TString('_'), 1, TString(':'), 1);
	
	stringstream *captured_cout = new stringstream() ;	
	TMultiLayerPerceptron* mlp = TrainNetworkROOT(data.merged, "@MH, @dEta, @dPhi, @Angle, @pBalance, @EtaH, @Sphericity:" + *ssp + ":1:isSignal", captured_cout, fname, *n);
	evaluate = Evaluate(mlp, data, 100);
	
	*max_sig = SignificancePlot(evaluate, fname);		
	ErrorPlot(captured_cout, fname);
	*auc = ROCPlot(evaluate, fname);
	SeparationPlot(data, mlp, fname);
	
	mlp_training->Fill();
}


void interactive(MCData_t data, TTree* mlp_training, TString* ssp, Int_t * n, TTree* evaluate, Double_t* auc, Double_t* max_sig )
{
	
	TString n_epoch;	
	while(1)
	{	
		cout << "Please enter a MLP structure string or q to save and quit:" << endl;
		cin >> *ssp; 
		if (*ssp == "q") { break;}
		
		cout << "Please enter a number of epochs to train for or q to save and quit:" << endl;
		cin >> n_epoch;
		if (n_epoch == "q") { break;}
		
		*n = n_epoch.Atoi();		
		
		run(data, mlp_training , ssp, n, evaluate, auc, max_sig);
	
	}
}




void batch(MCData_t data, TTree* mlp_training, TString* ssp, Int_t * n, TTree* evaluate, Double_t* auc, Double_t* max_sig )
{
	
	TString n_epoch, batch_ss;
	Int_t min, max, step;	
	
	cout << "Please enter a MLP structure string, substituting x for the layer to iterate over or q to save and quit:" << endl;
	cin >> batch_ss; 
	if (batch_ss == "q") { return;}
	
	cout << "Please enter a start value for x:" << endl;
	cin >> min; 
	cout << "Please enter a stop value for x:" << endl;
	cin >> max;  
	cout << "Please enter a step size for x:" << endl;
	cin >> step; 
	
	cout << "Please enter a number of epochs to train for or q to save and quit:" << endl;
	cin >> n_epoch;
	if (n_epoch == "q") { return;}
	
	*n = n_epoch.Atoi();		
	
	
	cout << "Running:" <<endl;
	for(Int_t i = min; i <= max; i+=step)
	{
		*ssp = batch_ss;
		ssp->ReplaceAll(TString('x'), TString::Itoa(i, 10));
		
		cout << "\tSSP = " << *ssp <<endl;
	
		TString fname = "plots/" + ssp->ReplaceAll(TString(':'), 1, TString('_'), 1) + "," + TString::Itoa(*n, 10);
		ssp->ReplaceAll(TString('_'), 1, TString(':'), 1);
	
		run(data, mlp_training , ssp, n, evaluate, auc, max_sig);
	}
	


}



int main (int argc, char  *argv[])
{
	
	/* Uncomment when run the first time to generate the test/train data sets */
	
	//	CreateTraining();
	
	MCData_t data = LoadTraining("combined_output.root");	
	TTree* evaluate =  new TTree();
	Double_t auc, max_sig = 0.;
	Int_t n=0;

	TString* ssp = new TString("");
	
	/* Open or create a root file to store all the results */
	
	TFile* f = new TFile("mlp_train.root", "UPDATE");
	TTree* mlp_training;	
	if(f->GetListOfKeys()->Contains("mlp_training")) 
	{
		mlp_training = (TTree*)f->Get("mlp_training");
		
		mlp_training->SetBranchAddress("eval", &evaluate);
		mlp_training->SetBranchAddress("auc", &auc);
		mlp_training->SetBranchAddress("max_sig", &max_sig);
		mlp_training->SetBranchAddress("n", &n);
		mlp_training->SetBranchAddress("struct_string", &ssp);
					
	}
	else 
	{
		mlp_training = new TTree("mlp_training", "mlp_training");
		
		mlp_training->Branch("eval", &evaluate);
		mlp_training->Branch("auc", &auc, "auc/D");
		mlp_training->Branch("max_sig", &max_sig, "auc/D");
		mlp_training->Branch("n", &n, "n/I");
		mlp_training->Branch("struct_string", &ssp);
	}
	
	TString mode;
	cout << "Please enter i for interactive or b for batch mode" <<endl;
	cin >> mode;

		if (mode == 'i')
			interactive(data, mlp_training , ssp, &n, evaluate, &auc, &max_sig);
		
		else if (mode == 'b')
			batch(data, mlp_training , ssp, &n, evaluate, &auc, &max_sig);
		
		else
			cout << "Please enter i for interactive or b for batch mode" <<endl;
 
	
	f->Write();
	f->Close();
	cout << "Done" <<endl;
	
	return 0;
}
