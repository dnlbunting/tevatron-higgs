/* NOT WORKING */
void TrainNetworkTMVA(MCData_t data, TString outfile_str, TString job_name)
{
	
	TFile *outfile = new TFile(outfile_str, "RECREATE");
			
	TMVA::Factory* factory = new TMVA::Factory( job_name, outfile, "Silent=False" );
	
	factory->AddSignalTree    ( data.train_signal,  1.0, TMVA::Types::kTraining );
	factory->AddSignalTree    ( data.test_signal,  1.0, TMVA::Types::kTesting );
	factory->AddBackgroundTree( data.train_background, 1.0, TMVA::Types::kTraining);
	factory->AddBackgroundTree( data.test_background, 1.0, TMVA::Types::kTesting);
	
	//factory->AddVariable( "MH",         'F' );
	factory->AddVariable( "dEta",       'F' );
	factory->AddVariable( "dPhi",   	'F' );
	//factory->AddVariable( "Angle",      'F' );
	factory->AddVariable( "pBalance",   'F' );
	//factory->AddVariable( "EtaH",       'F' );
	//factory->AddVariable( "Sphericity", 'F' );
	
	

	
	factory->BookMethod( TMVA::Types::kMLP, "MLP_ANN","ConvergenceTests=5:VerbosityLevel=Debug" );

	
	factory->TrainAllMethods();
		delete factory;/*
	factory->TestAllMethods();
	factory->EvaluateAllMethods();*/
	
	
}


TMultiLayerPerceptron* TrainNetworkROOT(TTree* merged, TString outfile_str, TString struct_string, stringstream* captured_cout, Int_t n_epoch = 100)
{
	/*
		Trains the network, returning the trained mlp object. 
		
		Takes a stringstream pointer and redirects cout into it in order to catch the error vs epoch information
		(Because whoever wrote MLP is a dick and didn't make it publicly available)
	
	
	*/
	
	
	streambuf* oldCout = cout.rdbuf();
	cout.rdbuf(captured_cout->rdbuf() );
				
	TMultiLayerPerceptron *mlp = new TMultiLayerPerceptron(struct_string, merged , "isTrain==1","isTrain==0");
	mlp->SetLearningMethod(TMultiLayerPerceptron::kBFGS);
	mlp->Train(n_epoch, "text,update=1");
	
	cout.rdbuf( oldCout );
	
	return(mlp);
	
}
