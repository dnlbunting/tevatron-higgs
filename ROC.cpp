
TTree* CalcROC(TMultiLayerPerceptron* mlp, MCData_t data, Int_t nPoints = 100 )
{
	
	/*
		Calculates the true and false positive rates (tpr/ fpr) at nPoints by varying the probability cutoff between signal and background from 0 to 1
	
		Returns a TTree with branches epoch, tpr and fpr
	*/
	
	Double_t tpr, fpr = 0.;
	
	TTree* accuracy_rates = new TTree("accuracy_rates", "accuracy rates");
	accuracy_rates->Branch("tpr",   &tpr,   "tpr/D");	
	accuracy_rates->Branch("fpr",   &fpr,   "fpr/D");	
	
	Double_t parameters[7];
	Bool_t isSignal, isTrain;	
	
	data.test_signal->SetBranchAddress("MH",   &parameters[0]);	 	        	data.test_background->SetBranchAddress("MH",   &parameters[0]);	 	       
	data.test_signal->SetBranchAddress("dEta", &parameters[1]);	            	data.test_background->SetBranchAddress("dEta", &parameters[1]);	           
	data.test_signal->SetBranchAddress("dPhi",  &parameters[2]);	        	data.test_background->SetBranchAddress("dPhi",  &parameters[2]);	              
	data.test_signal->SetBranchAddress("Angle",    &parameters[3]);         	data.test_background->SetBranchAddress("Angle",    &parameters[3]);        
	data.test_signal->SetBranchAddress("pBalance",  &parameters[4]);        	data.test_background->SetBranchAddress("pBalance",  &parameters[4]);    
	data.test_signal->SetBranchAddress("EtaH", &parameters[5]);	            	data.test_background->SetBranchAddress("EtaH", &parameters[5]);	           
	data.test_signal->SetBranchAddress("Sphericity",  &parameters[6]);	    	data.test_background->SetBranchAddress("Sphericity",  &parameters[6]);	
	data.test_signal->SetBranchAddress("isSignal",   &isSignal);            	data.test_background->SetBranchAddress("isSignal",   &isSignal);   
	data.test_signal->SetBranchAddress("isTrain",   &isTrain);              	data.test_background->SetBranchAddress("isTrain",   &isTrain);  
	
	Int_t tp, fp = 0.;
	
	for(Int_t i = 0; i < nPoints; ++i)
	{
		Double_t cutoff =  i*(1.0/nPoints);
		fp = 0.; tp = 0.;
		
		for(Int_t j = 0; j < data.test_signal->GetEntries(); ++j)
		{
			data.test_signal->GetEntry(j);
			Double_t p = mlp->Evaluate(0,parameters);
			if(p > cutoff) {tp++;}
		}
		
		for(Int_t j = 0; j < data.test_background->GetEntries(); ++j)
		{
			data.test_background->GetEntry(j);
			Double_t p = mlp->Evaluate(0,parameters);
			if(p >= cutoff) {fp++;}
		}
		
		tpr = tp /(Double_t) data.test_signal->GetEntries();
		fpr = fp /(Double_t) data.test_background->GetEntries();
		accuracy_rates->Fill();
	
	
	}
		
	return(accuracy_rates);
}

void ROCPlot(TTree* roc)
{
	/*
		Plots the ROC curve described by the points in roc.
		Saves output to roc.pdf
	
		TODO: Support TChain to have ROCs for multiple runs overlayed
		TODO: Calculate AUC
	*/
	
	TCanvas *roc_cnavas=new TCanvas("roc_cnavas");		
	roc->Draw("tpr:fpr");	
	TGraph *roc_gr = new TGraph( *((TGraph*)gPad->GetPrimitive("Graph")) ); 
	roc_cnavas->Clear();

	TF1 *fa1 = new TF1("fa1","x",0,1); fa1->Draw();
	roc_gr->Draw("CP");
	
	roc_cnavas->Update();
	roc_cnavas->SaveAs("roc.pdf"); 
	
}
