void trainNetwork(TTree* simu, TString _struct_string, Int_t n_epoch)
{
	//Part 2 training the NN
	
	   /* Before you go on and uncomment this part of the code make sure you have 
	      saved the plots of the signal and background input variables.*/
	
	//Now make a neural network. Example structure string given below 
	//   TString _struct_string = " @MH, @dEta, @dPhi, @Eta1mEtaH, @EtaH, @pBalance, @Njets, @FW_H2:6:1:type";
	   TMultiLayerPerceptron *mlp = new TMultiLayerPerceptron(_struct_string, simu, "Entry$%2","(Entry$+1)%2");
	   cout << "hepp" << endl;
	//_struct_string takes the form "@var1,@var2,@var3,...,@varn:A:B:type"
	//First part is list of input variables separated by commas 
	//    (put @ at the beginning of each variable name
	//A is the number of nodes in first hidden layer 
	//    (at least 2 x number of variables 
	//B is the number of nodes in the second hidden layer (usually 1)
	//type is the signal/background discriminant
	  
	//Train the NN   
	   mlp->Train(n_epoch, "text,graph,update=10");
	
	//Save as C++ file
	   mlp->Export("test","C++");
	
	//Use TMLPAnalyzer to see what it looks for  
	   TCanvas* mlpa_canvas = new TCanvas("mlpa_canvas","Network analysis");
	   TMLPAnalyzer ana(mlp);
	   // Initialisation
	   ana.GatherInformations();
	   // output to the console
	   ana.CheckNetwork();
	    // shows how each variable influences the network
	   ana.DrawDInputs();
	   TCanvas* mlpa_canvas3 = new TCanvas("mlpa_canvas3","Network analysis");
	    // shows the network structure
	   mlp->Draw();
	   TCanvas* mlpa_canvas4 = new TCanvas("mlpa_canvas4","Network analysis");
	    // draws the resulting network
	   ana.DrawNetwork(0,"type==1","type==0");
	   mlpa_canvas->cd(4);
	
}