
#include <TChain.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <iostream>

using namespace std;

typedef struct
{
	TTree* test_signal;
    TTree* train_signal;
    TTree* test_background;
    TTree* train_background; 
	TTree* merged; 

} MCData_t;


void CreateTraining(TString working_directory   = "~/Documents/Imperial/project/tevatron-higgs/")
{
		/*
			Reads in the outputs from the seperate signal and background Monte Carlo simulations, 
			creating a new file with train and test trees for each.
		*/
		
			/* File name strings */			
            TString background_file_str = working_directory +  "LHinput-cut_2b25_1j15-Tight-MC_bbb.root";
            TString signal_file_str     = working_directory + "LHinput-cut_2b25_1j15-Tight-MC_higgs110GeV.root";
			TString output_file_str     = working_directory + "combined_output.root";
			
			cout << "Creating signal/background train/test trees from: " << signal_file_str << " and " << background_file_str << endl;
			
			/* Load the background tree */
            TFile* background_file = new TFile(background_file_str, "READ");
            TTree* background_tree = (TTree*)background_file->Get("higgsSearch");
             
			/* Load the signal tree */                
            TFile* signal_file = new TFile(signal_file_str, "READ");
            TTree* signal_tree = (TTree*)signal_file->Get("higgsSearch");
			
			/* Prepare the output trees */
			TFile *  output = new TFile(output_file_str, "RECREATE");
			TTree* test_signal = new TTree("test_signal", "test signal");
            TTree* train_signal = new TTree("train_signal", "train signal");
			TTree* test_background = new TTree("test_background", "test background");
            TTree* train_background = new TTree("train_background", "train background"); 
			 
			/* Input branch addresses */            
	        Double_t MH[6], dEta[6], dPhi[6], Angle[6], EtaH[6], pBalance[6], Sphericity, lumiCor, w3tag;;
	        Int_t Njets, typeH[6], evtNum;
			
			cout << "Loading trees" << endl;
			
			/* Set the input branches */
	        background_tree->SetBranchAddress("MH",  &MH);                		  signal_tree->SetBranchAddress("MH",  &MH);
	        background_tree->SetBranchAddress("dEta",    &dEta);          		  signal_tree->SetBranchAddress("dEta",    &dEta);
	        background_tree->SetBranchAddress("dPhi",  &dPhi);            		  signal_tree->SetBranchAddress("dPhi",  &dPhi);
	        background_tree->SetBranchAddress("Eta1mEtaH", &Angle);       		  signal_tree->SetBranchAddress("Eta1mEtaH", &Angle);
	        background_tree->SetBranchAddress("EtaH",  &EtaH);            		  signal_tree->SetBranchAddress("EtaH",  &EtaH);
	        background_tree->SetBranchAddress("pBalance",  &pBalance);    		  signal_tree->SetBranchAddress("pBalance",  &pBalance);
	        background_tree->SetBranchAddress("Njets", &Njets);           		  signal_tree->SetBranchAddress("Njets", &Njets);
	        background_tree->SetBranchAddress("FW_H2", &Sphericity);      		  signal_tree->SetBranchAddress("FW_H2", &Sphericity);
	        background_tree->SetBranchAddress("evtNum", &evtNum);           	  signal_tree->SetBranchAddress("evtNum", &evtNum);
			background_tree->SetBranchAddress("Njets", &Njets);           	  	  signal_tree->SetBranchAddress("Njets", &Njets);
			background_tree->SetBranchAddress("type", &typeH);           	  	  signal_tree->SetBranchAddress("type", &typeH);
			background_tree->SetBranchAddress("w3tag", &w3tag); 				  signal_tree->SetBranchAddress("w3tag", &w3tag); 
			background_tree->SetBranchAddress("lumiCor", &lumiCor);               signal_tree->SetBranchAddress("lumiCor", &lumiCor); 
			
			/* Output branch addresses */
		    Double_t mh, deta, dphi, angle, etah, pbalance, sphericity, weight;
			Bool_t isSignal, isTrain;
			
			cout << "Creating output trees" << endl;
			                                                                                                                                              	
			/* Create the output branches */
			                                                                                    	
			test_signal->Branch("MH",   &MH,   "MH/D");			             		    train_signal->Branch("MH",   &MH,   "MH/D");				              	
			test_signal->Branch("dEta", &deta, "dEta/D");                    		    train_signal->Branch("dEta", &deta, "dEta/D");	                      	
			test_signal->Branch("dPhi",  &dphi,  "dPhi/D");                  		    train_signal->Branch("dPhi",  &dphi,  "dPhi/D");	                      	
			test_signal->Branch("Angle",    &angle,    "Angle/D");           		    train_signal->Branch("Angle",    &angle,    "Angle/D");	              	
			test_signal->Branch("pBalance",  &pbalance,  "pBalance/D");      		    train_signal->Branch("pBalance",  &pbalance,  "pBalance/D");	          	
			test_signal->Branch("EtaH", &etah, "EtaH/D");                    		    train_signal->Branch("EtaH", &etah, "EtaH/D"); 	                      	
			test_signal->Branch("Sphericity",  &sphericity,  "Sphericity/D");		    train_signal->Branch("Sphericity",  &sphericity,  "Sphericity/D");	  	
			test_signal->Branch("isSignal",   &isSignal,   "type/O");        		    train_signal->Branch("isSignal",   &isSignal,   "type/O");
			test_signal->Branch("isTrain",   &isTrain,   "type/O");        		    	train_signal->Branch("isTrain",   &isTrain,   "type/O");	
			test_signal->Branch("w3tag", &w3tag, "w3tag/D"); 				  			train_signal->Branch("w3tag", &w3tag, "w3tag/D"); 
			test_signal->Branch("lumiCor", &lumiCor, "lumiCor/D");               		train_signal->Branch("lumiCor", &lumiCor, "lumiCor/D");              
		  	test_signal->Branch("weight", &weight, "weight/D");               			train_signal->Branch("weight", &weight ,"weight/D"); 
							          				
			test_background->Branch("MH",   &MH,   "MH/D");			             		train_background->Branch("MH",   &MH,   "MH/D");				              	
			test_background->Branch("dEta", &deta, "dEta/D");                    		train_background->Branch("dEta", &deta, "dEta/D");	                      	
			test_background->Branch("dPhi",  &dphi,  "dPhi/D");                  		train_background->Branch("dPhi",  &dphi,  "dPhi/D");	                      	
			test_background->Branch("Angle",    &angle,    "Angle/D");           		train_background->Branch("Angle",    &angle,    "Angle/D");	              	
			test_background->Branch("pBalance",  &pbalance,  "pBalance/D");      		train_background->Branch("pBalance",  &pbalance,  "pBalance/D");	          	
			test_background->Branch("EtaH", &etah, "EtaH/D");                    		train_background->Branch("EtaH", &etah, "EtaH/D"); 	                      	
			test_background->Branch("Sphericity",  &sphericity,  "Sphericity/D");		train_background->Branch("Sphericity",  &sphericity,  "Sphericity/D");	  	
			test_background->Branch("isSignal",   &isSignal,   "type/O");        		train_background->Branch("isSignal",   &isSignal,   "type/O");
			test_background->Branch("isTrain",   &isTrain,   "type/O");        			train_background->Branch("isTrain",   &isTrain,   "type/O");	  	     
			test_background->Branch("w3tag", &w3tag, "w3tag/D"); 				  		train_background->Branch("w3tag", &w3tag, "w3tag/D"); 
			test_background->Branch("lumiCor", &lumiCor,"lumiCor/D");               	train_background->Branch("lumiCor", &lumiCor,"lumiCor/D");              
		  	test_background->Branch("weight", &weight,"weight/D");               		train_background->Branch("weight", &weight, "weight/D");    
		  
		  	cout << "Filtering signal tree" << endl;
		    /* Iterate over signal tree */  
			Int_t nS_train, nS_test = 0;  
			for(Int_t i = 0; i < signal_tree->GetEntries(); ++i)
			{
				signal_tree->GetEntry(i);
				
				/* Select only events where:
						- there are 3 jets,
				   	 	- the two leading pT jets come from the Higgs decay (typeH[0]==1), 
						- these two jets are separated by sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1,
		            	- the invariant di-jet mass is greater than 50 GeV
				*/
		      	
			    if ( Njets==3 && typeH[0]==1 && MH[0]>50  && sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1. )
				{
					/* Load up output branches */
 			        deta = dEta[0];
			        dphi = 2*acos(0)- dPhi[0]; 
		            angle = Angle[0];
		            pbalance = pBalance[0];
		            etah = EtaH[0];
		            sphericity = Sphericity;
		            isSignal = true; 
					weight = lumiCor*w3tag;

					/* 50/50 split between train and test */
					if(nS_train > nS_test){isTrain =false; test_signal->Fill(); nS_test++;}
					else{isTrain =true; train_signal->Fill(); nS_train++;}
										
				}
								
			}
			
		  	cout << "Filtering background tree" << endl;		
			/* Iterate over background tree */ 
			Int_t nB_train, nB_test = 0;    
			for(Int_t i = 0; i < background_tree->GetEntries(); ++i)
			{
				background_tree->GetEntry(i);
				
				/* Select only events where:
						- there are 3 jets,
				   	 	- the two leading pT jets come from the Higgs decay (typeH[0]==1), 
						- these two jets are separated by sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1,
		            	- the invariant di-jet mass is greater than 50 GeV
						- Only keep as many background events there are signal events
				*/
		      	
			    if ( Njets==3 && typeH[0]==1 && MH[0]>50  && sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1. && nB_train+nB_test < nS_train+nS_test )
				{

					/* Load up output branches */
 			        deta = dEta[0];
			        dphi = 2*acos(0)- dPhi[0]; 
		            angle = Angle[0];
		            pbalance = pBalance[0];
		            etah = EtaH[0];
		            sphericity = Sphericity;
		            isSignal = false;
					weight = lumiCor*w3tag; 

					/* 50/50 split between train and test */
					if(nB_train > nB_test){isTrain =false; test_background->Fill(); nB_test++;}
					else{isTrain =true; train_background->Fill(); nB_train++;}
										
				}
								
			}


		cout << "Writing output trees to " << output_file_str << endl;
		cout << "           Test\tTrain" << endl;
		
		cout << "Signal     " << nS_test << "\t" << nS_train << endl;
		cout << "Background " << nB_test << "\t" << nB_train << endl;
		
		TList *tlist = new TList;
		tlist->Add(train_signal);
		tlist->Add(test_signal);
		tlist->Add(train_background);
		tlist->Add(test_background);
		TTree* merged = TTree::MergeTrees(tlist);
	    merged->SetName("merged");
		
		
		output->Write();
}


MCData_t LoadTraining(TString file_str = "~/Documents/Imperial/project/tevatron-higgs/combined_output.root")
{
	/* 	
		Loads the output of CreateTraining
	*/
	
    	TFile* file = new TFile(file_str, "READ");
		
		MCData_t data;
		data.test_signal = (TTree*)file->Get("test_signal");
    	data.train_signal = (TTree*)file->Get("train_signal");
		data.test_background = (TTree*)file->Get("test_background");
    	data.train_background = (TTree*)file->Get("train_background"); 
    	data.merged = (TTree*)file->Get("merged"); 
		
		cout << "Loaded data from " << file_str << endl;
		return(data);	

}




