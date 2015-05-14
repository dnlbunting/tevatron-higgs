//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 11 15:32:55 2015 by ROOT version 6.03/04
// from TChain hbb/
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Analysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Double_t        Spher;
   Double_t        weight;
   Double_t        w_2tag;
   Double_t        w_3tag;
   Int_t           fileID;
   Double_t        P1;
   Double_t        P2;
   Double_t        P3;
   Double_t        Theta1;
   Double_t        Theta2;
   Double_t        Theta3;
   Double_t        Phi1;
   Double_t        Phi2;
   Double_t        Phi3;
   Double_t        BTag1;
   Double_t        BTag2;
   Double_t        BTag3;
   Double_t        HT;

   // List of branches
   TBranch        *b_Spher;   //!
   TBranch        *b_weight;   //!
   TBranch        *b_w_2tag;   //!
   TBranch        *b_w_3tag;   //!
   TBranch        *b_fileID;   //!
   TBranch        *b_P1;   //!
   TBranch        *b_P2;   //!
   TBranch        *b_P3;   //!
   TBranch        *b_Theta1;   //!
   TBranch        *b_Theta2;   //!
   TBranch        *b_Theta3;   //!
   TBranch        *b_Phi1;   //!
   TBranch        *b_Phi2;   //!
   TBranch        *b_Phi3;   //!
   TBranch        *b_BTag1;   //!
   TBranch        *b_BTag2;   //!
   TBranch        *b_BTag3;   //!
   TBranch        *b_HT;   //!

   Analysis(TTree *tree=0);
   virtual ~Analysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void ProcessJet(int njet);
   void TransversePlot();
};

#endif

#ifdef Analysis_cxx
Analysis::Analysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {

#ifdef SINGLE_TREE
      // The following code should be used if you want this class to access
      // a single tree instead of a chain
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("Memory Directory");
      if (!f || !f->IsOpen()) {
         f = new TFile("Memory Directory");
      }
      f->GetObject("hbb",tree);

#else // SINGLE_TREE

      // The following code should be used if you want this class to access a chain
      // of trees.
      TChain * chain = new TChain("hbb","");
      chain->Add("bbb.root/hbb");
      tree = chain;
#endif // SINGLE_TREE

   }
   Init(tree);
}

Analysis::~Analysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t Analysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t Analysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
  
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void Analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Spher", &Spher, &b_Spher);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   fChain->SetBranchAddress("w_2tag", &w_2tag, &b_w_2tag);
   fChain->SetBranchAddress("w_3tag", &w_3tag, &b_w_3tag);
   fChain->SetBranchAddress("fileID", &fileID, &b_fileID);
   fChain->SetBranchAddress("P1", &P1, &b_P1);
   fChain->SetBranchAddress("P2", &P2, &b_P2);
   fChain->SetBranchAddress("P3", &P3, &b_P3);
   fChain->SetBranchAddress("Theta1", &Theta1, &b_Theta1);
   fChain->SetBranchAddress("Theta2", &Theta2, &b_Theta2);
   fChain->SetBranchAddress("Theta3", &Theta3, &b_Theta3);
   fChain->SetBranchAddress("Phi1", &Phi1, &b_Phi1);
   fChain->SetBranchAddress("Phi2", &Phi2, &b_Phi2);
   fChain->SetBranchAddress("Phi3", &Phi3, &b_Phi3);
   fChain->SetBranchAddress("BTag1", &BTag1, &b_BTag1);
   fChain->SetBranchAddress("BTag2", &BTag2, &b_BTag2);
   fChain->SetBranchAddress("BTag3", &BTag3, &b_BTag3);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   Notify();
}

Bool_t Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void Analysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t Analysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef Analysis_cxx
