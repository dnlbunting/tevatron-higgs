#define Analysis_cxx
#include "Analysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TMath.h> 
#include <string>
#include <sstream> 
using namespace std;

void Analysis::ProcessJet(int njet)
{
	/* 
	
	Draws histograms of the Cartesian projections and transverse component of the njet-th jet
	
		Parameters:
			
			int njet  The jet number to process- 1,2, or 3
	
	*/
	
	cout<<"hello"<<endl;
	std::ostringstream ss;
	ss << njet;
	std::string sjet=ss.str();
	
	
	
	/* Initalise histograms */
	 
	TH1D* hx = new TH1D(("Px" + sjet).c_str(), ("Px Jet " + sjet).c_str(), 30,0,300);		//WHY DOES THIS USE C STRINGS!?
	TH1D* hy = new TH1D(("Py" + sjet).c_str(), ("Py Jet " + sjet).c_str(), 30,0,300);
	TH1D* hz = new TH1D(("Pz" + sjet).c_str(), ("Pz Jet " + sjet).c_str(), 30,0,300);
	
	TH1D* ht = new TH1D(("Pt" + sjet).c_str(), ("Transverse P Jet " + sjet).c_str(), 30,0,300);
	
	TH2F* hyz = new TH2F("Pyz", ("Py vs Pz Jet " + sjet).c_str(), 30,0,300,30,0,300);
	
	/* Loop over the chain	*/
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{    
		Long64_t ientry = LoadTree(jentry);     
		if (ientry < 0) break;     
		nb = fChain->GetEntry(jentry);   nbytes += nb;

	/*  Start of custom loop code */
		
		/* Populate a 3 vector with the spherical coords of the chosen jet */
		TVector3 P_vector;	  
	  	switch(njet){
	  		case 1:
 	  			P_vector.SetMagThetaPhi(P1, Theta1, Phi1); break;		
			case 2:
				P_vector.SetMagThetaPhi(P2, Theta2, Phi2); break;
			case 3:
				P_vector.SetMagThetaPhi(P3, Theta3, Phi3); break;
			default:
				cout << "Error invalid jet index" << endl; return;

			}
	/* Populate the histograms */
 	  hx->Fill(P_vector.X());
   	  hy->Fill(P_vector.Y());
 	  hz->Fill(P_vector.Z());
 	  ht->Fill(P_vector.Perp());
	  
	  hyz-> Fill(P_vector.Y(), P_vector.Z());
	  
	}
	
	/* Render the histograms */
	TCanvas *c1 = new TCanvas("c1",("Jet " + sjet+ " Momentum Plot").c_str());
	hyz->Draw();
	TCanvas *c2 = new TCanvas("c2",("Jet " + sjet+ " Momentum Plot").c_str());
	c2->Divide(2,2);
	c2->cd(1); hx->Draw();
	c2->cd(2); hy->Draw();
	c2->cd(3); hz->Draw();
	c2->cd(4); ht->Draw();

}

void Analysis::TransversePlot()
{
	
	/* 
	
	Draws overlayed histograms of the transverse components of the 3 jets
	
	*/
	
	/* Initalise histograms */
	
	TH1D* h1 = new TH1D("Pt1", "Pt Jet 1 ", 30,0,300);
	TH1D* h2 = new TH1D("Pt2", "Pt Jet 2 ", 30,0,300);
	TH1D* h3 = new TH1D("Pt3", "Pt Jet 3 ", 30,0,300);
	
	
	/* Loop over the chain */
	
	if (fChain == 0) return;
	Long64_t nentries = fChain->GetEntriesFast();
	Long64_t nbytes = 0, nb = 0;
	
	for (Long64_t jentry=0; jentry<nentries;jentry++) 
	{    
		Long64_t ientry = LoadTree(jentry);     
		if (ientry < 0) break;     
		nb = fChain->GetEntry(jentry);   nbytes += nb;

		/*  Start of custom loop code  */
		TVector3 P1_vector, P2_vector,P3_vector;	  
		P1_vector.SetMagThetaPhi(P1, Theta1, Phi1); 		
		P2_vector.SetMagThetaPhi(P2, Theta2, Phi2);
		P3_vector.SetMagThetaPhi(P3, Theta3, Phi3);
	
	 /* Populate the histograms */	  
 	  h1->Fill(P1_vector.Perp());
	  h2->Fill(P2_vector.Perp());
	  h3->Fill(P3_vector.Perp());
	
	}
	
	/* Render the histograms */
	
	TCanvas *c1 = new TCanvas("c1","Transverse Momenta");
	
	h1->Draw();
	h2->SetLineColor(kRed);   h2->Draw("sames");
	h3->SetLineColor(kGreen); h3->Draw("sames");
	
}

void Analysis::Loop()
{

  
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
	  Long64_t ientry = LoadTree(jentry);
  }
     
}
