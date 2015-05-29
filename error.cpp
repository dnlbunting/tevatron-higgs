
TTree* ErrorPlot(stringstream *ss)
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
	error_canvas->SaveAs("error.pdf");
		
	return(error);
}
