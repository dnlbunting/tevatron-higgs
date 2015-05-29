
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
#include "error.cpp"
#include "ROC.cpp"

int main (int argc, char  *argv[])
{
	//CreateTraining();
	
	MCData_t data = LoadTraining("~/Documents/Imperial/project/tevatron-higgs/combined_output.root");
	
	TFile* f = new TFile("roc.root", "RECREATE");
	
	stringstream *captured_cout = new stringstream() ;
	
	TMultiLayerPerceptron* mlp = TrainNetworkROOT(data.merged,"blah", "@MH, @dEta, @dPhi, @Angle, @pBalance, @EtaH, @Sphericity:50:1:isSignal", captured_cout, 200);
	TTree *roc = CalcROC(mlp, data, 100);
	

	ErrorPlot(captured_cout);
	
	ROCPlot(roc);

 

	/*TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");	
	Double_t auc = graph->Integral();
	cout << "AUC: " << auc <<endl;*/

	 
	cout << "Done" <<endl;

	return 0;
}
