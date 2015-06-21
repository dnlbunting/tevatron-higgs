#include <TStyle.h>

void SeparationPlot(TH1D* sig_hist, TH1D* bkg_hist , TH1D* usig_hist, TH1D* ubkg_hist, TString fname )
{
	TStyle* sty = new TStyle();
	sty->SetOptStat(0);
	
	TCanvas *sep_cnavas = new TCanvas("sep_cnavas");		
	
	sig_hist->SetFillStyle(3005);
	sig_hist->SetFillColor(kRed);
	sig_hist->SetLineColor(kRed); 
	sig_hist->Draw();
	sep_cnavas->Update();
	sep_cnavas->SaveAs(fname + "_signal.pdf");
	
	sep_cnavas->Clear();
	
	bkg_hist->SetFillStyle(3005);
	bkg_hist->SetFillColor(kBlue);
	bkg_hist->SetLineColor(kBlue);
	bkg_hist->Draw();
	
	sep_cnavas->Update();
	sep_cnavas->SaveAs(fname + "_background.pdf"); 

	usig_hist->SetFillStyle(3005);
	usig_hist->SetFillColor(kRed);
	usig_hist->SetLineColor(kRed); 
	usig_hist->Draw();
	ubkg_hist->SetFillStyle(3005);
	ubkg_hist->SetFillColor(kBlue);
	ubkg_hist->SetLineColor(kBlue);
	ubkg_hist->Draw("same");
	usig_hist->GetXaxis()->SetTitle("NN output");
	sep_cnavas->Update();
	sep_cnavas->SaveAs(fname + "_seperation_plot.pdf"); 



}


TTree *Evaluate(TMultiLayerPerceptron * mlp, MCData_t data, TString fname, Double_t nPoints = 100)
{
	Int_t nbins = 60;
	Double_t s, b, S, tpr, fpr, cutoff = 0.;
	
	TH1D* sig_hist = new TH1D("sig_hist", "Signal", nbins, 0., 1.);
	TH1D* bkg_hist = new TH1D("bkg_hist", "Background", nbins, 0., 1.);
	TH1D* ubkg_hist = new TH1D("ubkg_hist", "Un-weighted Background", nbins, 0., 1.);
	TH1D* usig_hist = new TH1D("usig_hist", "Un-weighted Signal", nbins, 0., 1.);	
	
	TTree* evaluate = new TTree("evaluate", "evaluate");
	evaluate->Branch("cutoff",   &cutoff,   "cutoff/D");	
	evaluate->Branch("tpr",   &tpr,   "tpr/D");	
	evaluate->Branch("fpr",   &fpr,   "fpr/D");	
	evaluate->Branch("s",   &s,   "s/D");	
	evaluate->Branch("b",   &b,   "b/D");	
	evaluate->Branch("S",   &S,   "S/D");

	
	Double_t parameters[7], weight;
	Bool_t isSignal, isTrain;	
	
	data.test_signal->SetBranchAddress("MH",   &parameters[6]);	 	        data.test_background->SetBranchAddress("MH",   &parameters[6]);	 	       
	data.test_signal->SetBranchAddress("dEta", &parameters[0]);	            data.test_background->SetBranchAddress("dEta", &parameters[0]);	           
	data.test_signal->SetBranchAddress("dPhi",  &parameters[1]);	        	data.test_background->SetBranchAddress("dPhi",  &parameters[1]);	              
	data.test_signal->SetBranchAddress("Angle",    &parameters[2]);         	data.test_background->SetBranchAddress("Angle",    &parameters[2]);        
	data.test_signal->SetBranchAddress("pBalance",  &parameters[3]);        	data.test_background->SetBranchAddress("pBalance",  &parameters[3]);    
	data.test_signal->SetBranchAddress("EtaH", &parameters[4]);	            data.test_background->SetBranchAddress("EtaH", &parameters[4]);	           
	data.test_signal->SetBranchAddress("Sphericity",  &parameters[5]);	    	data.test_background->SetBranchAddress("Sphericity",  &parameters[5]);	
	data.test_signal->SetBranchAddress("isSignal",   &isSignal);            	data.test_background->SetBranchAddress("isSignal",   &isSignal);   
	data.test_signal->SetBranchAddress("isTrain",   &isTrain);              	data.test_background->SetBranchAddress("isTrain",   &isTrain);  
	data.test_signal->SetBranchAddress("weight",   &weight);              		data.test_background->SetBranchAddress("weight",   &weight); 
	

	for(Int_t j = 0; j < data.test_signal->GetEntries(); ++j)
	{
		data.test_signal->GetEntry(j);
		Double_t p = mlp->Evaluate(0,parameters);
		sig_hist->Fill(p, weight);
		usig_hist->Fill(p);
	}
	
	for(Int_t j = 0; j < data.test_background->GetEntries(); ++j)
	{
		data.test_background->GetEntry(j);
		Double_t p = mlp->Evaluate(0,parameters);
		bkg_hist->Fill(p, weight);
		ubkg_hist->Fill(p);
	}
	
	SeparationPlot(sig_hist, bkg_hist ,usig_hist, ubkg_hist, fname );
	
	TH1F *sigacc = new TH1F("sigacc", "sigacc", 60,  0, 1);
	TH1F *backacc = new TH1F("backacc", "backacc", 60,  0, 1);
	
	Int_t tp = 0;
	Int_t fp = 0;
	s = 0; b = 0; S = 0;
	for(Int_t i = 0; i < nbins; ++i)
	{
		tp += usig_hist->GetBinContent(nbins-i);
		fp += ubkg_hist->GetBinContent(nbins-i);
		
	    s += 100*sig_hist->GetBinContent(nbins-i);
	    b += bkg_hist->GetBinContent(nbins-i);

	 	S = (b > 0) ? s / sqrt(b+(0.02*0.02*b*b)) : 0.;		
		cutoff = 1 - i/(float)nbins;
		
	 //   cout << S << "\t" << s<< "\t" <<b<<"\t"<< cutoff << endl; 	
		sigacc->SetBinContent(nbins-i, s);
		backacc->SetBinContent(nbins-i, b);
		
		tpr = tp /(Double_t) data.test_signal->GetEntries();
		fpr = fp /(Double_t) data.test_background->GetEntries();
		//cout << tp << "\t" <<fp <<endl;		
		evaluate->Fill();
	}
	
	TCanvas *acc_canvas=new TCanvas("acc_canvas");
	sigacc->SetLineColor(kRed);
	backacc->Draw();
	acc_canvas->SaveAs(fname + "_backacc.pdf"); 	
	sigacc->Draw();
	acc_canvas->SaveAs(fname + "_sigacc.pdf"); 
					
			
	return(evaluate);

}


Double_t ROCPlot(TTree* evaluate, TString fname)
{
	/*
		Plots the ROC curve described by the points in roc.
		Saves output to roc.pdf
	
		TODO: Support TChain to have ROCs for multiple runs overlayed
		TODO: Calculate AUC
	*/
	
	TCanvas *roc_cnavas=new TCanvas("roc_cnavas");		
	evaluate->Draw("tpr:fpr");	
	TGraph *roc_gr = new TGraph( *((TGraph*)gPad->GetPrimitive("Graph")) ); 
	roc_cnavas->Clear();

	TF1 *fa1 = new TF1("fa1","x",0,1); fa1->Draw();
	roc_gr->Draw("CP");
	
	roc_cnavas->Update();
	roc_cnavas->SaveAs(fname + "_roc.pdf"); 
	
	
	Double_t auc = roc_gr->Integral() + 0.5; // Integrate closes the polygon by linking the first and last points ie (0,0) and (1,1), so actually calulates area above  the line x=y
	return(auc);
	
}


Double_t SignificancePlot(TTree* evaluate, TString fname)
{

	TCanvas *sig_cnavas=new TCanvas("sig_cnavas");		
	evaluate->Draw("S:cutoff");	
	TGraph *sig_gr = new TGraph( *((TGraph*)gPad->GetPrimitive("Graph")) ); 
	sig_cnavas->Clear();
	
	TF1 *fa1 = new TF1("fa1","x",0,1); fa1->Draw();

	sig_gr->Draw("ACP");
	
	sig_cnavas->Update();
	sig_cnavas->SaveAs(fname + "_sig.pdf"); 
		
    int n = sig_gr->GetN();
    Double_t *x = sig_gr->GetY();
	
	cout << n << x <<endl;

    int locmin = TMath::LocMax(n,x);
	return(x[locmin]);

}




void ErrorPlot(stringstream *ss, TString fname)
{
	
	/*
		Processes the cout stream caught from TrainNetwork to extract train and test set error
		against epoch.
	
		Plots error vs epoch to file error.pdf
	*/
	

	string line;
	
	Double_t train_err,test_err;
	Int_t epoch;
	
	TTree *error = new TTree("error", "error");	
	error->Branch("train_err", &train_err, "train_err/D");
	error->Branch("test_err", &test_err, "test_err/D");
	error->Branch("epoch", &epoch, "epoch/I");	
	
	while(getline(*ss, line))
	{
		
		TObjArray tx = *(TString(line).Tokenize(" "));

		if(((TObjString*)tx[0])->GetString()== "Epoch:")
		{
				TString train_s = ((TObjString*)tx[2])->GetString();
				train_err = TString(train_s(6,8)).Atof();
				
				TString test_s = ((TObjString*)tx[3])->GetString();
				test_err = TString(test_s(5 ,8)).Atof();
				epoch = ((TObjString*)tx[1])->GetString().Atoi();
				
				error->Fill();
		}
	}

	TCanvas *error_canvas=new TCanvas("error_canvas");
	
	error->Draw("test_err:epoch");
    TGraph *test_gr = new TGraph( *((TGraph*)gPad->GetPrimitive("Graph")) ); 
	error_canvas->Clear();
	
	error->Draw("train_err:epoch");
	TGraph *train_gr = new TGraph(*((TGraph*)gPad->GetPrimitive("Graph"))); 
	error_canvas->Clear();
	
	train_gr->SetLineColor(kRed);
	train_gr->Draw("ACP");
	test_gr->Draw("CP");	
	error_canvas->Update();
	error_canvas->SaveAs(fname + "_error.pdf");
		
}




