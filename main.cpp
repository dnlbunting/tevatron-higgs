
#include <TChain.h>
#include <TFile.h>
#include <TMVA/Factory.h>
#include <TMVA/Types.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TList.h>
#include <TGraph.h>
#include <TF1.h>
#include<iostream>
#include <TObjArray.h>
#include <TMultiLayerPerceptron.h>


#include "createTraining.cpp"
#include "TrainNetwork.cpp"
#include "evaluate.cpp"


int main (int argc, char  *argv[])
{
	
	/* Uncomment when run the first time to generate the test/train data sets */
	
	//	CreateTraining();
	
	MCData_t data = LoadTraining("combined_output.root");
	
	TTree* evaluate =  new TTree();
	Double_t auc = 0.;
	TString ss = "";
	TString* ssp = &ss;
	
	/* Open or create a root file to store all the results */
	
	TFile* f = new TFile("mlp_train.root", "UPDATE");
	TTree* mlp_training;	
	if(f->GetListOfKeys()->Contains("mlp_training")) 
	{
		mlp_training = (TTree*)f->Get("mlp_training");
		
		mlp_training->SetBranchAddress("eval", &evaluate);
		mlp_training->SetBranchAddress("auc", &auc);
		mlp_training->SetBranchAddress("struct_string", &ssp);
					
	}
	else 
	{
		mlp_training = new TTree("mlp_training", "mlp_training");
		
		mlp_training->Branch("eval", &evaluate);
		mlp_training->Branch("auc", &auc, "auc/D");
		mlp_training->Branch("struct_string", &ssp);
	}
	
	
	TString n_epoch;	
	while(1)
	{	
		cout << "Please enter a MLP structure string and a number of epochs to train for or q to save and quit:" << endl;
		cin >> ss >> n_epoch;
		if (ss == "q") { break;}

		stringstream *captured_cout = new stringstream() ;	
		TMultiLayerPerceptron* mlp = TrainNetworkROOT(data.merged, "@MH, @dEta, @dPhi, @Angle, @pBalance, @EtaH, @Sphericity:" + ss + ":1:isSignal", captured_cout, n_epoch.Atoi());
		evaluate = Evaluate(mlp, data, 100);
		
		TString fname = "plots/" + ss.ReplaceAll(TString(':'), 1, TString('_'), 1) + "," + n_epoch;

		SignificancePlot(evaluate, fname);		
		ErrorPlot(captured_cout, fname);
		auc = ROCPlot(evaluate, fname);
		SeparationPlot(data, mlp, fname);
		
		mlp_training->Fill();
		f->Write();
	
	}
 	
	f->Write();
	cout << "Done" <<endl;
	
	return 0;
}
