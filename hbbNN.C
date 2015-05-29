#include "trainNetwork.C"

void hbbNN(Int_t ntrain=15) {
// Example of a Multi Layer Perceptron
// For a Tevatron search for SUSY Higgs in bh->bbb, a neural network 
// was used to separate the signal from the background passing 
// some selection cuts. Here is a simplified version of this network, 
// taking into account only bbb events.
//Author: P. Jonsson

//Part 1, preparing the inputs

//Booking some histograms for signal and background, 
//Add histograms yourself for the dijet mass (MH[0]) of the leading pT 
//(=transverse momenta) jets for background and signal  

   TH1F *detas = new TH1F("detas", "detas", 50, .0, 4.);
   TH1F *detab = new TH1F("detab", "detab", 50, .0, 4.);   
   TH1F *dphis = new TH1F("dphis", "dphis", 50, .0, 3.15);
   TH1F *dphib = new TH1F("dphib", "dphib", 50, .0, 3.15);  
   TH1F *angles = new TH1F("angles", "angles", 50, .0, 2.);
   TH1F *angleb = new TH1F("angleb", "angleb", 50, .0, 2.);  
   TH1F *pbalances = new TH1F("pbalances", "pbalances", 50, .0, 1.);
   TH1F *pbalanceb = new TH1F("pbalanceb", "pbalanceb", 50, .0, 1.); 
   TH1F *etahs = new TH1F("etahs", "etahs", 50, -2.5, 2.5);
   TH1F *etahb = new TH1F("etahb", "etahb", 50, -2.5, 2.5);  
   TH1F *sphericitys = new TH1F("sphericitys", "sphericitys", 50, .0, 1.);
   TH1F *sphericityb = new TH1F("sphericityb", "sphericityb", 50, .0, 1.); 

   if (!gROOT->GetClass("TMultiLayerPerceptron")) {
      gSystem->Load("libMLP");
   }

   // Prepare inputs
   // The 2 trees are merged into one, and a "type" branch, 
   // equal to 1 for the signal and 0 for the background is added.

   TChain * signal = new TChain("higgsSearch","");

   //Use a Higgs signal at a mass of  110 GeV   
    signal->Add("LHinput-cut_2b25_1j15-Tight-MC_higgs110GeV.root/higgsSearch");

    //Use a background of multijet bbb events
   TChain *background = new TChain("higgsSearch","");
   background->Add("LHinput-cut_2b25_1j15-Tight-MC_bbb.root/higgsSearch");

   //book combined tree of selected events top use for training
   TFile *f = new TFile("training.root","recreate");
   TTree *simu = new TTree("MonteCarlo", "Filtered Monte Carlo Events");

   Double_t nnoutxlm[4], MH[6], dEta[6], dPhi[6], Angle[6], EtaH[6], pBalance[6], Sphericity;
   Double_t dphi, deta, angle, etah, pbalance, sphericity; 
   Int_t Njets, typeH[6], typeO, nsignal, nbkg;

   //counters for signal and background events
   nsignal=0;
   nbkg=0;

   // 
   //Set branch addresses to use from signal and background trees
   //Description of variables inline below. 
   //Draw histograms of these  variables for signal and background on top 
   //of eachother in different colors. 
   //Save these plots (you can look up how to save plots to files at root.cern.ch)
 
   //Invariant di-jet mass
   signal->SetBranchAddress("MH",  &MH);

   //Delta eta, difference in pseudorapidity (spatial coodinate used to 
   //describe angle with the beam axis = -ln(tan(theta/2))
   signal->SetBranchAddress("dEta",    &dEta);

   //Delta phi, azimuthal angle (going from 0 to 2pi around the beampipe)
   signal->SetBranchAddress("dPhi",  &dPhi);

   //Angle between the leading jet and the combined jet-pair
   signal->SetBranchAddress("Eta1mEtaH", &Angle);

   //Pseudorapidity of the combined jet-pair
   signal->SetBranchAddress("EtaH",  &EtaH);

   //Which jet-pair comes from the true Higgs decay
   signal->SetBranchAddress("type", &typeH);

   //Momentum balance of the jet-pair: p1-p2/p1+p2 
   signal->SetBranchAddress("pBalance",  &pBalance);

   //Number of jets in the event 
   signal->SetBranchAddress("Njets", &Njets);

   //Event sphericity (measure of how spherical, round, the event is)
   signal->SetBranchAddress("FW_H2", &Sphericity);

   //same for background
   background->SetBranchAddress("MH",  &MH);
   background->SetBranchAddress("dEta",    &dEta);
   background->SetBranchAddress("dPhi",  &dPhi);
   background->SetBranchAddress("Eta1mEtaH", &Angle);
   background->SetBranchAddress("EtaH",  &EtaH);
   background->SetBranchAddress("pBalance",  &pBalance);
   background->SetBranchAddress("Njets", &Njets);
   background->SetBranchAddress("FW_H2", &Sphericity);

   //Make input branches of variables to use for the training
   simu->Branch("dEta", &deta, "dEta/D");
   simu->Branch("dPhi",  &dphi,  "dPhi/D");
   simu->Branch("Angle",    &angle,    "Angle/D");
   simu->Branch("pBalance",  &pbalance,  "pBalance/D");
   simu->Branch("EtaH", &etah, "EtaH/D");
   simu->Branch("Sphericity",  &sphericity,  "Sphericity/D");
   simu->Branch("type",   &typeO,   "type/I");
  
   //loop over signal and select events to use for training 
   Int_t i;
   for (i = 0; i < signal->GetEntries(); i++) {
     signal->GetEntry(i);
     //only consider events with 3 jets
      if (Njets==3){
          typeO=0; 
          //select only events where the two leading pT jets come from the 
	  //Higgs decay (typeH[0]==1), and these two jets are separated by 
	  //sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1, and the invariant 
          //di-jet mass is greater than 50 GeV
          if (typeH[0]==1 && sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1. && MH[0]>50.){          
	    deta = dEta[0];
	    dphi =2*acos(0)- dPhi[0]; 
            angle = Angle[0];
            pbalance = pBalance[0];
            etah = EtaH[0];
            sphericity = Sphericity;
            typeO=1; 
	   }
	  //fill selected signal events (type0==1)
          if (typeO==1){
	    //fill the histograms like this:
	      detas->Fill(deta); 	      
	      dphis->Fill(dphi);
	      angles->Fill(angle);
	      pbalances->Fill(pbalance);
	      etahs->Fill(etah);
	      sphericitys->Fill(sphericity);


	      //fill the training tree   
              simu->Fill();
              nsignal++; 
	  }
      }
   }

   cout << nsignal <<endl; 
   //now loop over background events
   for (i = 0; i < background->GetEntries(); i++) {
      background->GetEntry(i);
      if (Njets==3){
          typeO=1;
	  //similar for background events
          if (sqrt(dEta[0]*dEta[0]+dPhi[0]*dPhi[0]) > 1. && MH[0]>50.){
	    deta = dEta[0];
	    dphi =2*acos(0)- dPhi[0]; 
            angle = Angle[0];
            pbalance = pBalance[0];
            etah = EtaH[0];
            sphericity = Sphericity;
            typeO=0; 
          }
	  //fill the selected background events until you have the same 
          //number as signal
          if (typeO==0 && nbkg<nsignal){
	    //fill the background histograms here:
    	      detab->Fill(deta); 	      
    	      dphib->Fill(dphi);
    	      angleb->Fill(angle);
    	      pbalanceb->Fill(pbalance);
    	      etahb->Fill(etah);
    	      sphericityb->Fill(sphericity);
            
	    
	    
	    nbkg++;
            simu->Fill();
	  } 
      }

   }
   cout << nbkg << endl;   


   	/*Plot the simulated and background variable distributions */
	TCanvas *c1 = new TCanvas("c2", "Signal and Background Distributions");
	c1->Divide(2,3);
	c1->cd(1);  dphis->Draw();       dphib->	SetLineColor(kRed);   dphib->Draw("same");	
	c1->cd(2);  detas->Draw();       detab->	SetLineColor(kRed);   detab->Draw("same");	
	c1->cd(3);  angles->Draw();      angleb->	SetLineColor(kRed);   angleb->Draw("same");	
	c1->cd(4);  pbalances ->Draw();  pbalanceb->	SetLineColor(kRed);   pbalanceb ->Draw("same");	
	c1->cd(5);  etahs->Draw();       etahb->	SetLineColor(kRed);   etahb->Draw("same");	
	c1->cd(6);  sphericitys->Draw(); sphericityb->	SetLineColor(kRed);   sphericityb->Draw("same");	
	c1->SaveAs("sig_background_distr.pdf");	
	


	for(Int_t i = 1; i++; i < 5)
	{ 
		stringstream ss;
		ss << i*5;
		TString structure = "@dEta, @dPhi, @EtaH, @pBalance, @Sphericity, @Angle:" + ss.str() + ":1:type";;
		trainNetwork(simu, structure, 100);

	}
}
