// Offline Plots - Z. Miller July 24, 2015
//
// .L offline.C
// offline("FILENAME") # Without .root Extension

#include "anaConst.h"

// Declare functions
Bool_t checkPaintAllTrigs();
void checkBatchMode();
Bool_t checkMakePDF();
Bool_t checkMakeRoot();

void offline(const char* FileName="test")
{
   
  // Set Style parameters for this macro
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
  gErrorIgnoreLevel = kError; // Set Verbosity Level (kPrint shows all)

  // Set Output options
  Int_t number;
  Bool_t fPaintAll = checkPaintAllTrigs();
  checkBatchMode();
  Bool_t makePDF = checkMakePDF();
  Bool_t makeROOT= checkMakeRoot();
  
  // Open ROOT File
  char name[1000];
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/%s.root",FileName);
  TFile *f = new TFile(name,"READ");
  if (f->IsOpen()==kFALSE)
    { std::cout << "!!! File Not Found !!!" << std::endl;
      exit(1); }
  // f->ls(); // - DEBUG by printing all objects in ROOT file

  char fname[100];
  TFile* file;
  if(makeROOT){
    sprintf(fname,"/Users/zach/Research/rootFiles/run12NPEhPhi/%s_processed.root",FileName);
    file = new TFile(fname,"RECREATE");
    if (file->IsOpen()==kFALSE)
      {
	std::cout << "!!! Outfile Not Opened !!!" << std::endl;
	makeROOT = kFALSE;
      }
  }
  
  const Int_t numPtBins = anaConst::nPtBins;
  Float_t lowpt[numPtBins],highpt[numPtBins];
  for(Int_t c=0; c< numPtBins; c++){
    lowpt[c] = anaConst::lpt[c];
    highpt[c] = anaConst::hpt[c];
  }
  Float_t hptCut=anaConst::hptCut;
  const Int_t numTrigs = 4;
  Double_t epsilon[numPtBins] = {0.593164, 0.626663, 0.655916, 0.674654, 0.685596, 0.700600, 0.716682, 0.724638, 0.713977, 0.730550, 0.735204, 0.744336, 0.761323, 0.758423};
  Float_t hptMax=25; // Set max above range to allow overflow
  Float_t lowPhi=anaConst::lowPhi, highPhi=anaConst::highPhi;
  // Reconstruction efficiency
  TH1D * LSIM[numPtBins][numTrigs];
  TH1D * USIM[numPtBins][numTrigs];
  TH1D * USIMNP[numPtBins][numTrigs];
  TH1D * LSIMNP[numPtBins][numTrigs];
  TH1D * INCLNP[numPtBins][numTrigs];
  TH1D * INCL[numPtBins][numTrigs];
  TH1D * LSIM2[numPtBins][numTrigs];
  TH1D * USIM2[numPtBins][numTrigs];
  TH1D * INCL2[numPtBins][numTrigs];
  TH1D * LSMM[numPtBins][numTrigs];
  TH1D * USMM[numPtBins][numTrigs];
  TH1D * HHDP[numPtBins][numTrigs];
  TH1D * NSPI[numPtBins][numTrigs];
  TH2F * MON[numTrigs];
  TH1F * AVGTRKS[numTrigs];
  TH2F * MON2[numTrigs];
  TH1F * AVGTRKS2[numTrigs];
  TH2F* mh2PhiQPt[numTrigs];
  TH2F* mh2nSigmaEPt[numTrigs];
  TH2F* mh2nSigmaEPt_eID[numTrigs];
  TH3F* mh3DelPhiIncl[numTrigs];
  TH3F* mh3DelPhiPhotLS[numTrigs];
  TH3F* mh3DelPhiPhotUS[numTrigs];
  TH3F* mh3DelPhiPhotUSNP[numTrigs];
  TH3F* mh3DelPhiPhotLSNP[numTrigs];
  TH3F* mh3DelPhiInclNP[numTrigs];
  TH3F* mh3DelPhiInclWt[numTrigs];
  TH3F* mh3DelPhiPhotLSWt[numTrigs];
  TH3F* mh3DelPhiPhotUSWt[numTrigs];
  TH2F* mh2InvMassPtLS[numTrigs];
  TH2F* mh2InvMassPtUS[numTrigs];
  TH3F* mh3nTracksZdcx[numTrigs];
  TH3F* mh3MixedDelPhi;
  TH3F* mh3MixedDelEta;
  TH3F* mh3MixedEtaPhi;
  TH3F* mh3DelPhiHadHad[numTrigs];
  TH2F* mh2nSigmaPion[numTrigs];
  TH1F* mh1PtHadTracks[numTrigs];
  TH1D* projHPhi[numPtBins][numTrigs];
  TH1D* projnSigmaE[numPtBins][numTrigs];
  TH1D* projnSigmaE_eID[numPtBins][numTrigs];
  TH1D* projDelPhiIncl[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLS[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUS[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUSNP[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLSNP[numPtBins][numTrigs];
  TH1D* projDelPhiInclNP[numPtBins][numTrigs];
  TH1D* projDelPhiInclWt[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLSWt[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUSWt[numPtBins][numTrigs];
  TH1D* projDelPhiHadHad[numPtBins][numTrigs];
  TH1D* projInvMassLS[numPtBins][numTrigs];
  TH1D* projInvMassUS[numPtBins][numTrigs];
  TH1D* projnSigmaPion[numPtBins][numTrigs];
  TH1D* projEMixedEtaPhi;
  TH1D* projPMixedEtaPhi;
  TH2D* proj2DMixedEtaPhi;
  TH2D* proj2DMixedEvent[numPtBins];
  TH1D* projMixedDelPhi[numPtBins];
  TH1D* projMixedDelEta[numPtBins];
  TH2F* histoNorms;
  TCanvas * c[numTrigs];
  TCanvas * c2[numTrigs];
  TCanvas * IN[numTrigs];
  TCanvas * IN2[numTrigs];
  TCanvas * pile[numTrigs];
  TCanvas * result[numTrigs];
  TCanvas * result2[numTrigs];
  TCanvas * inMass[numTrigs];
  TCanvas * USComp[numTrigs];
  TCanvas * LSComp[numTrigs];
  TCanvas * InclComp[numTrigs];
  TCanvas * cHH[numTrigs];
  TCanvas * nSigPi[numTrigs];
  TCanvas * mixedC;
  TCanvas * mixedCbinEta;
  TCanvas * mixedCbinPhi;
  TCanvas * mixedCbin2D;
  TCanvas * singlePlot;

  TPaveText* lbl[numPtBins];
  char textLabel[100];
  singlePlot =  new TCanvas("singlePlot","Single Plot",150,0,1150,1000);

  // Trigger Independent Hists
  mixedC     = new TCanvas("mixedC","Mixed Events",150,0,1150,1000);
  mixedCbinEta = new TCanvas("mixedCbinEta","Mixed Events Eta",150,0,1150,1000);
  mixedCbinPhi = new TCanvas("mixedCbinPhi","Mixed Events Phi",150,0,1150,1000);
  mixedCbin = new TCanvas("mixedCbin","Mixed Events 2D",150,0,1150,1000);
  
  mixedC       -> Divide(2,2);
  mixedCbinEta -> Divide(4,3);
  mixedCbinPhi -> Divide(4,3);
  
  mh3MixedDelPhi = (TH3F*)f->Get("mh3MixedDelPhi");
  mh3MixedDelEta = (TH3F*)f->Get("mh3MixedDelEta");
  mh3MixedEtaPhi = (TH3F*)f->Get("mh3MixedEtaPhi");

  ///////////////////////////
  projPMixedEtaPhi  = mh3MixedEtaPhi -> ProjectionX("projPMixedEtaPhi");
  projEMixedEtaPhi  = mh3MixedEtaPhi -> ProjectionY("projEMixedEtaPhi");
  proj2DMixedEtaPhi = (TH2D*)mh3MixedEtaPhi -> Project3D("yx");
  Int_t RB2 = 2;
  projEMixedEtaPhi->Rebin(RB2);
  projPMixedEtaPhi->Rebin(RB2);
  
  mixedC->cd(1);
  mh3MixedEtaPhi->GetXaxis()->SetTitle("#Delta#phi");
  mh3MixedEtaPhi->GetXaxis()->SetRangeUser(lowPhi,highPhi);
  mh3MixedEtaPhi->GetYaxis()->SetTitle("#Delta#eta");
  mh3MixedEtaPhi->GetYaxis()->SetRangeUser(-1.5,1.5);
  mh3MixedEtaPhi->GetZaxis()->SetTitle("P_{t,e}");
  mh3MixedEtaPhi->Draw();
  mixedC->cd(2);
  projPMixedEtaPhi->GetXaxis()->SetRangeUser(lowPhi,highPhi);
  projPMixedEtaPhi->GetXaxis()->SetTitle("#Delta#phi");
  projPMixedEtaPhi->GetYaxis()->SetRangeUser(0,20000);
  projPMixedEtaPhi->SetTitle("Mixed Event #Delta#phi");
  projPMixedEtaPhi->Draw();
  mixedC->cd(3);
  projEMixedEtaPhi->GetXaxis()->SetRangeUser(-2.5,2.5);
  projEMixedEtaPhi->GetXaxis()->SetTitle("#Delta#eta");
  projEMixedEtaPhi->SetTitle("Mixed Event #Delta#eta");
  projEMixedEtaPhi->Draw();
  mixedC->cd(4);
  mixedC->SetLogz(1);
  proj2DMixedEtaPhi->GetXaxis()->SetTitle("#Delta#phi");
  proj2DMixedEtaPhi->GetXaxis()->SetRangeUser(lowPhi,highPhi);
  proj2DMixedEtaPhi->GetYaxis()->SetTitle("#Delta#eta");
  proj2DMixedEtaPhi->GetYaxis()->SetRangeUser(-1.5,1.5);
  proj2DMixedEtaPhi->Draw("colz");

  TH3F* temp3D[numPtBins];
  // PtBins for Mixed Event
  for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){
    
    // Init necessary plotting tools
    lbl[ptbin] = new TPaveText(.2,.8,.5,.85,Form("NB NDC%i",ptbin));
    sprintf(textLabel,"%.1f < P_{T,e} < %.1f",lowpt[ptbin],highpt[ptbin]);
    lbl[ptbin]->AddText(textLabel);
    lbl[ptbin]->SetFillColor(kWhite);

    projMixedDelPhi[ptbin] = mh3MixedEtaPhi->ProjectionX(Form("projMixedDelPhi_%i",ptbin),0,-1,mh3MixedEtaPhi->GetZaxis()->FindBin(lowpt[ptbin]),mh3MixedEtaPhi->GetZaxis()->FindBin(highpt[ptbin])-1);
    projMixedDelEta[ptbin] = mh3MixedEtaPhi->ProjectionY(Form("projMixedDelEta_%i",ptbin),0,-1,mh3MixedEtaPhi->GetZaxis()->FindBin(lowpt[ptbin]),mh3MixedEtaPhi->GetZaxis()->FindBin(highpt[ptbin])-1);
    
    mixedCbinEta->cd(ptbin+1);
    projMixedDelEta[ptbin]->GetXaxis()->SetRangeUser(-2.5,2.5);
    projMixedDelEta[ptbin]->GetXaxis()->SetTitle("#Delta#eta");
    projMixedDelEta[ptbin]->Draw();

    mixedCbinPhi->cd(ptbin+1);
    projMixedDelPhi[ptbin]->GetXaxis()->SetRangeUser(lowPhi-.5,highPhi+.5);
    // projMixedDelPhi[ptbin]->GetYaxis()->SetRangeUser(0,2000);
    projMixedDelPhi[ptbin]->GetXaxis()->SetTitle("#Delta#phi");
    projMixedDelPhi[ptbin]->Draw();

    /*   temp3D[ptbin] = (TH3F*)mh3MixedEtaPhi->Clone(); // make a clone to set axis range on for 3D to 2D projection
    temp3D[ptbin]->GetZaxis()->SetRangeUser(lowpt[ptbin],highpt[ptbin]); // project3d only projects active range
    proj2DMixedEvent[ptbin] = (TH2D*)temp3D[ptbin] -> Project3D("yx");
    proj2DMixedEvent[ptbin]->SetName(Form("proj2DMixedEvent_%i",ptbin));

    mixedCbin->cd(ptbin+1);
    proj2DMixedEvent[ptbin]->GetXaxis()->SetTitle("#Delta#phi");
    proj2DMixedEvent[ptbin]->GetXaxis()->SetRangeUser(lowPhi,highPhi);
    proj2DMixedEvent[ptbin]->GetYaxis()->SetTitle("#Delta#eta");
    proj2DMixedEvent[ptbin]->GetYaxis()->SetRangeUser(-1.5,1.5);
    proj2DMixedEvent[ptbin]->Draw("colz");*/
    
  }

  /// TRIGGER LOOP
  histoNorms = new TH2F("histoNorms","",4,0,12,20,0,20); // Fill normalization in a single histogram for accessing later

  for(Int_t trig = 0; trig < numTrigs; trig++){

    if(!fPaintAll && (trig == 1 || trig == 3)) continue; 
    // Create and Segment Canvas
    c[trig]        = new TCanvas(Form("c%i",trig),"Photonic Hists",150,0,1150,1000);
    IN[trig]       = new TCanvas(Form("IN%i",trig),"Inclusive Hists",150,0,1150,1000);
    pile[trig]     = new TCanvas(Form("pile%i",trig),"Pileup Monitor",150,0,1150,1000);
    inMass[trig]   = new TCanvas(Form("inMass%i",trig),"Invariant Mass",150,0,1150,1000);
    result[trig]   = new TCanvas(Form("result%i",trig),"Inclusive - Photonic",150,0,1150,1000);
    USComp[trig]   = new TCanvas(Form("USComp%i",trig),"Unlike Sign Distributions",150,0,1150,1000);
    LSComp[trig]   = new TCanvas(Form("LSComp%i",trig),"Like Sign Distributions",150,0,1150,1000);
    InclComp[trig] = new TCanvas(Form("InclComp%i",trig),"Inclusive Distributions",150,0,1150,1000);
    cHH[trig]      = new TCanvas(Form("cHH%i",trig),"Hadron-Hadron Distributions",150,0,1150,1000);
    nSigPi[trig]   = new TCanvas(Form("nSigPi_%i",trig),"n#sigma#pi QA",150,0,1150,1000);
    c[trig]        -> Divide(4,3);
    inMass[trig]   -> Divide(4,3);
    IN[trig]       -> Divide(4,3);
    pile[trig]     -> Divide(2,2);
    result[trig]   -> Divide(4,3);
    USComp[trig]   -> Divide(4,3);
    LSComp[trig]   -> Divide(4,3);
    InclComp[trig] -> Divide(4,3);
    cHH[trig]      -> Divide(4,3);
    nSigPi[trig]   -> Divide(4,3);

    // Make Projections (first get 2d/3d hists, then project)
    mh2PhiQPt[trig]         = (TH2F*)f->Get(Form("mh2PhiQPt_%i",trig));
    mh2nSigmaEPt[trig]      = (TH2F*)f->Get(Form("mh2nSigmaEPt_%i",trig));
    mh2nSigmaEPt_eID[trig]  = (TH2F*)f->Get(Form("mh2nSigmaEPt_eID_%i",trig));
    mh3DelPhiIncl[trig]     = (TH3F*)f->Get(Form("mh3DelPhiIncl_%i",trig));
    mh3DelPhiPhotLS[trig]   = (TH3F*)f->Get(Form("mh3DelPhiPhotLS_%i",trig));
    mh3DelPhiPhotUS[trig]   = (TH3F*)f->Get(Form("mh3DelPhiPhotUS_%i",trig));
    mh3DelPhiPhotUSNP[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUSNP_%i",trig));
    mh3DelPhiPhotLSNP[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLSNP_%i",trig));
    mh3DelPhiInclNP[trig]   = (TH3F*)f->Get(Form("mh3DelPhiPhotInclNP_%i",trig));
    mh3DelPhiInclWt[trig]   = (TH3F*)f->Get(Form("mh3DelPhiInclWt_%i",trig));
    mh3DelPhiPhotLSWt[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLSWt_%i",trig));
    mh3DelPhiPhotUSWt[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUSWt_%i",trig));
    mh2InvMassPtLS[trig]    = (TH2F*)f->Get(Form("mh2InvMassPtLS_%i",trig));
    mh2InvMassPtUS[trig]    = (TH2F*)f->Get(Form("mh2InvMassPtUS_%i",trig));
    mh3DelPhiHadHad[trig]   = (TH3F*)f->Get(Form("mh3DelPhiHadHad_%i",trig));
    mh1PtHadTracks[trig]    = (TH1F*)f->Get(Form("mh1PtHadTracks_%i",trig));
    mh1PtETracks[trig]      = (TH1F*)f->Get(Form("mh1PtETracks_%i",trig));
    mh2nSigmaPion[trig]     = (TH2F*)f->Get(Form("mh2nSigmaPionPt_%i",trig));

    for(Int_t ptbin=0; ptbin<numPtBins; ptbin++)
      {
	// - Make projections into electron ptbins
	projHPhi[ptbin][trig]       = mh2PhiQPt[trig]->ProjectionX(Form("projHPhi_%i_%i",ptbin,trig),mh2PhiQPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2PhiQPt[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
	projnSigmaE[ptbin][trig]    = mh2nSigmaEPt[trig]->ProjectionX(Form("projnSigmaE_%i_%i",ptbin,trig),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
	projnSigmaE_eID[ptbin][trig] = mh2nSigmaEPt_eID[trig]->ProjectionX(Form("projnSigmaE_eID_%i_%i",ptbin,trig),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
	projDelPhiIncl[ptbin][trig] = mh3DelPhiIncl[trig]->ProjectionX(Form("projDelPhiIncl_%i_%i",ptbin,trig),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiIncl[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotUS[ptbin][trig] = mh3DelPhiPhotUS[trig]->ProjectionX(Form("projDelPhiPhotUS_%i_%i",ptbin,trig),mh3DelPhiPhotUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotUS[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotUSNP[ptbin][trig] = mh3DelPhiPhotUSNP[trig]->ProjectionX(Form("projDelPhiPhotUSNP_%i_%i",ptbin,trig),mh3DelPhiPhotUSNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUSNP[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotUSNP[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotLSNP[ptbin][trig] = mh3DelPhiPhotLSNP[trig]->ProjectionX(Form("projDelPhiPhotLSNP_%i_%i",ptbin,trig),mh3DelPhiPhotLSNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLSNP[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotLSNP[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiInclNP[ptbin][trig] = mh3DelPhiInclNP[trig]->ProjectionX(Form("projDelPhiInclNP_%i_%i",ptbin,trig),mh3DelPhiInclNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiInclNP[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiInclWt[ptbin][trig] = mh3DelPhiInclWt[trig]->ProjectionX(Form("projDelPhiInclWt_%i_%i",ptbin,trig),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiInclWt[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotUSWt[ptbin][trig] = mh3DelPhiPhotUSWt[trig]->ProjectionX(Form("projDelPhiPhotUSWt_%i_%i",ptbin,trig),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotUSWt[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projDelPhiPhotLSWt[ptbin][trig] = mh3DelPhiPhotLSWt[trig]->ProjectionX(Form("projDelPhiPhotLSWt_%i_%i",ptbin,trig),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiPhotLSWt[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projInvMassUS[ptbin][trig] = mh2InvMassPtUS[trig]->ProjectionX(Form("projInvMassUS_%i_%i",ptbin,trig),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
	projInvMassLS[ptbin][trig] = mh2InvMassPtLS[trig]->ProjectionX(Form("projInvMassLS_%i_%i",ptbin,trig),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
	projDelPhiHadHad[ptbin][trig] = mh3DelPhiHadHad[trig]->ProjectionX(Form("projDelPhiHadHad_%i_%i",ptbin,trig),mh3DelPhiHadHad[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiHadHad[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiHadHad[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptMax));
	projnSigmaPion[ptbin][trig] = mh2nSigmaPion[trig]->ProjectionX(Form("projnSigmaPion_%i_%i",ptbin,trig),mh2nSigmaPion[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaPion[trig]->GetYaxis()->FindBin(highpt[ptbin])-1);
      }
    
    for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){

      // Init necessary plotting tools
      lbl[ptbin] = new TPaveText(.2,.8,.5,.85,Form("NB NDC%i",ptbin));
      sprintf(textLabel,"%.1f < P_{T,e} < %.1f",lowpt[ptbin],highpt[ptbin]);
      lbl[ptbin]->AddText(textLabel);
      lbl[ptbin]->SetFillColor(kWhite);

      // Calculate electron purity from pol3 fit of xiaozhi data
      Float_t ptAv = (lowpt[ptbin]+highpt[ptbin])/2.;
      Float_t p[3] = {0.9743, 0.02128, -0.00438};
      Float_t purity = p[0] + (p[1]*ptAv)+(p[2]*ptAv*ptAv);
      Float_t hadPur = 1-purity;
      
      // Calculate Normalization for NPE delPhi
      Int_t inclNorm = mh1PtETracks[trig]->Integral(mh1PtETracks[trig]->GetXaxis()->FindBin(lowpt[ptbin]),mh1PtETracks[trig]->GetXaxis()->FindBin(highpt[ptbin])-1);
      Int_t LSNorm   = projInvMassLS[ptbin][trig]->Integral();
      Int_t USNorm   = projInvMassUS[ptbin][trig]->Integral();
      Int_t hhNorm   = mh1PtHadTracks[trig]->Integral(mh1PtHadTracks[trig]->GetXaxis()->FindBin(lowpt[ptbin]),mh1PtHadTracks[trig]->GetXaxis()->FindBin(highpt[ptbin])-1);
      Double_t HHScale = (Float_t)inclNorm/(Float_t)hhNorm; // so the purity comparison is 1:1
      Float_t Norm = (Float_t)inclNorm - (1/epsilon[ptbin] - 1.)*(Float_t)USNorm + (1/epsilon[ptbin])*(Float_t)LSNorm - HHScale*hadPur*hhNorm; // Use the number of "signal" counts
      histoNorms->SetBinContent(histoNorms->GetBin(trig+1,ptbin+1), Norm); // Find the bin and fill with the Normalization
      
      Int_t counter = numPtBins*trig+ptbin;
      // DEBUG cout << counter << endl;
      c[trig]->cd(ptbin+1);
      // Assign to a single, simpler name for manip
      LSIM[ptbin][trig]  = projDelPhiPhotLS[ptbin][trig];
      USIM[ptbin][trig]  = projDelPhiPhotUS[ptbin][trig];
      USIMNP[ptbin][trig]= projDelPhiPhotUSNP[ptbin][trig];
      LSIMNP[ptbin][trig]= projDelPhiPhotLSNP[ptbin][trig];
      INCLNP[ptbin][trig]= projDelPhiInclNP[ptbin][trig];
      INCL[ptbin][trig]  = projDelPhiIncl[ptbin][trig];
      INCL2[ptbin][trig] = projDelPhiInclWt[ptbin][trig];
      LSIM2[ptbin][trig] = projDelPhiPhotLSWt[ptbin][trig];
      USIM2[ptbin][trig] = projDelPhiPhotUSWt[ptbin][trig];
      LSMM[ptbin][trig]  = projInvMassLS[ptbin][trig];
      USMM[ptbin][trig]  = projInvMassUS[ptbin][trig];
      HHDP[ptbin][trig]  = projDelPhiHadHad[ptbin][trig];
      NSPI[ptbin][trig]  = projnSigmaPion[ptbin][trig];
      // Rebin all as necessary
      Int_t RB = 1;
      LSIM[ptbin][trig]  -> Rebin(RB);
      USIM[ptbin][trig]  -> Rebin(RB);
      USIMNP[ptbin][trig]-> Rebin(RB);
      LSIMNP[ptbin][trig]-> Rebin(RB);
      INCLNP[ptbin][trig]-> Rebin(RB);
      INCL[ptbin][trig]  -> Rebin(RB);
      INCL2[ptbin][trig] -> Rebin(RB);
      LSIM2[ptbin][trig] -> Rebin(RB);
      USIM2[ptbin][trig] -> Rebin(RB);
      HHDP[ptbin][trig]  -> Rebin(RB);
      NSPI[ptbin][trig]  -> Rebin(10);
      
      // Actually manipulate histos and plot (photnic del Phi)
      
      USIMNP[ptbin][trig]->SetLineColor(kRed);
      USIMNP[ptbin][trig]->SetLineWidth(1);
      USIMNP[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USIMNP[ptbin][trig]->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	USIMNP[ptbin][trig]->SetTitle("Photonic Electron Reconstruction (No Partner Track)");
      else if (ptbin == 1 && trig !=3)
	USIMNP[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USIMNP[ptbin][trig]->SetTitle("MB");
      else
	USIMNP[ptbin][trig]->SetTitle("");
      USIMNP[ptbin][trig]->Draw("");
      
      LSIMNP[ptbin][trig]->SetLineColor(kBlack);
      LSIMNP[ptbin][trig]->SetLineWidth(1);
      LSIMNP[ptbin][trig]->Draw(" same");
      lbl[ptbin]->Draw("same");
      
      // Subtraction of (USNP-LS)
      TH1F *SUB = (TH1F*)USIMNP[ptbin][trig]->Clone(); //
      SUB->SetName("Subtraction");      // Create SUB as a clone of USIMNP
      SUB->Add(LSIMNP[ptbin][trig],-1);
      SUB->SetLineColor(kBlue);
      SUB->SetLineWidth(1);
      SUB->SetFillStyle(3001);
      SUB->SetFillColor(kBlue);
      SUB->Draw("same");
      lbl[ptbin]->Draw("same");
      TLegend* leg = new TLegend(0.2,0.73,0.55,0.85);
      leg->AddEntry(USIMNP[ptbin][trig],"Unlike Sign","lpe");
      leg->AddEntry(LSIM[ptbin][trig],"Like Sign", "lpe");
      leg->AddEntry(SUB,"Unlike - Like", "lpe");
      leg->Draw();

      // Plot different US (w/wo partner)
      USComp[trig]->cd(ptbin+1);
      TH1F *USwP = (TH1F*)USIM[ptbin][trig]->Clone();
      USwP->SetLineColor(kRed); //with partner tracks
      USwP->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USwP->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	USwP->SetTitle("Photonic Unlike Sign Distributions");
      else if (ptbin == 1 && trig !=3)
	USwP->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USwP->SetTitle("MB");
      else
	USwP->SetTitle("");
      USwP->Draw("");
      TH1F *USnP = (TH1F*)USIMNP[ptbin][trig]->Clone();
      USnP->SetLineColor(kBlack);
      USnP->Draw("same");
      lbl[ptbin]->Draw("same");
      TLegend* legUS = new TLegend(0.35,0.8,0.77,0.87);
      legUS->AddEntry(USwP,"With Partner Track","lpe");
      legUS->AddEntry(USnP,"Partner Track Removed", "lpe");
      legUS->Draw();

      // Plot different LS (w/wo partner)
      LSComp[trig]->cd(ptbin+1);
      TH1F *LSwP = (TH1F*)LSIM[ptbin][trig]->Clone();
      LSwP->SetLineColor(kRed); //with partner tracks
      LSwP->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      LSwP->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	LSwP->SetTitle("Photonic Like Sign Distributions");
      else if (ptbin == 1 && trig !=3)
	LSwP->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	LSwP->SetTitle("MB");
      else
	LSwP->SetTitle("");
      LSwP->Draw("");
      TH1F *LSnP = (TH1F*)LSIMNP[ptbin][trig]->Clone();
      LSnP->SetLineColor(kBlack);
      LSnP->Draw("same");
      lbl[ptbin]->Draw("same");
      TLegend* legLS = new TLegend(0.35,0.8,0.77,0.87);
      legLS->AddEntry(LSwP,"With Partner Track","lpe");
      legLS->AddEntry(LSnP,"Partner Track Removed", "lpe");
      legLS->Draw();

      // Plot different Incl (w/wo partner)
      InclComp[trig]->cd(ptbin+1);
      TH1F *InclwP = (TH1F*)INCL[ptbin][trig]->Clone();
      InclwP->SetLineColor(kRed); //with partner tracks
      InclwP->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      InclwP->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	InclwP->SetTitle("Inclusive Distributions");
      else if (ptbin == 1 && trig !=3)
	InclwP->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	InclwP->SetTitle("MB");
      else
	InclwP->SetTitle("");
      InclwP->Draw("");
      TH1F *InclnP = (TH1F*)INCLNP[ptbin][trig]->Clone();
      InclnP->SetLineColor(kBlack);
      InclnP->Draw("same");
      lbl[ptbin]->Draw("same");
      TLegend* legIncl = new TLegend(0.35,0.8,0.77,0.87);
      legIncl->AddEntry(InclwP,"Inclusive","lpe");
      legIncl->AddEntry(InclnP,"Semi-Inclusive", "lpe");
      legIncl->Draw();

      // Actually manipulate histos and plot (photonic InvMass)
      inMass[trig]->cd(ptbin+1);
      USMM[ptbin][trig]->SetLineColor(kRed);
      USMM[ptbin][trig]->SetLineWidth(1);
      USMM[ptbin][trig]->GetXaxis()->SetTitle("InvMass (GeV/c^{2})");
      USMM[ptbin][trig]->GetXaxis()->SetRangeUser(0,0.4);
      if(ptbin == 0)
	USMM[ptbin][trig]->SetTitle("Photonic Electron Reconstruction");
      else if (ptbin == 1 && trig !=3)
	USMM[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USMM[ptbin][trig]->SetTitle("MB");
      else
	USMM[ptbin][trig]->SetTitle("");
      USMM[ptbin][trig]->Draw("");
      
      LSMM[ptbin][trig]->SetLineColor(kBlack);
      LSMM[ptbin][trig]->SetLineWidth(1);
      LSMM[ptbin][trig]->Draw("same");
      lbl[ptbin]->Draw("same");

      // Subtraction of (US-LS)
      TH1F *SUB4 = (TH1F*)USMM[ptbin][trig]->Clone(); //
      SUB4->SetName("Subtraction");      // Create SUB as a clone of USIM
      SUB4->Add(LSMM[ptbin][trig],-1);
      SUB4->SetLineColor(kBlue);
      SUB4->SetLineWidth(1);
      SUB4->SetFillStyle(3001);
      SUB4->SetFillColor(kBlue);
      SUB4->Draw("same");
      TLegend* leg2 = new TLegend(0.45,0.6,0.85,0.75);
      leg2->AddEntry(USMM[ptbin][trig],"Unlike Sign","lpe");
      leg2->AddEntry(LSMM[ptbin][trig],"Like Sign", "lpe");
      leg2->AddEntry(SUB4,"Unlike - Like", "lpe");
      leg2->Draw();

       // Handle Inclusive Hists
      IN[trig]->cd(ptbin+1);
      INCL[ptbin][trig]->SetLineColor(kBlue);
      INCL[ptbin][trig]->SetLineWidth(1);
      INCL[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      INCL[ptbin][trig]->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	INCL[ptbin][trig]->SetTitle("Inclusive Electrons");
      else if (ptbin == 1 && trig !=3)
	INCL[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	INCL[ptbin][trig]->SetTitle("MB");
      else
	INCL[ptbin][trig]->SetTitle("");
      INCL[ptbin][trig]->Draw("");
      lbl[ptbin]->Draw("same");

      IN[trig]->Update();

      // Plot Had-Had correlations
      cHH[trig]->cd(ptbin+1);
      HHDP[ptbin][trig]->SetLineColor(kGreen+3);
      HHDP[ptbin][trig]->SetLineWidth(1);
      HHDP[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      HHDP[ptbin][trig]->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      if(ptbin == 0)
	HHDP[ptbin][trig]->SetTitle("Hadron-Hadron Correlations");
      else if (ptbin == 1 && trig !=3)
	HHDP[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	HHDP[ptbin][trig]->SetTitle("MB");
      else
	HHDP[ptbin][trig]->SetTitle("");
      HHDP[ptbin][trig]->Draw("");
      lbl[ptbin]->Draw("same");
      
      
      // Subtraction of Inclusive - (1/e - 1)US + (1/e)LS - (1-purity)HadHad
      result[trig]->cd(ptbin+1);
      TH1F *INCDP = (TH1F*)INCLNP[ptbin][trig]->Clone();
      TH1F *unscaleINCDP = (TH1F*)INCLNP[ptbin][trig]->Clone();
      TH1F *ULDP  = (TH1F*)USIMNP[ptbin][trig]->Clone();
      TH1F *LSDP  = (TH1F*)LSIMNP[ptbin][trig]->Clone();
      TH1F *HADDP = (TH1F*)HHDP[ptbin][trig]->Clone();
      INCDP->SetName(Form("scaleNPEhDelPhi_%i_%i",trig,ptbin));
      unscaleINCDP->SetName(Form("NPEhDelPhi_%i_%i",trig,ptbin));
      ULDP->Scale(1./epsilon[ptbin] - 1.); // Scale each distribution by associated factors
      LSDP->Scale(1./epsilon[ptbin]);
      HADDP->Scale(HHScale*hadPur);
      INCDP->Add(ULDP,-1);
      INCDP->Add(LSDP,1);
      INCDP->Add(HADDP,-1);
      INCDP->Scale(1./((Double_t)Norm));//*INCDP->GetBinWidth(1))); 
      INCDP->SetLineColor(kBlack);
      INCDP->SetLineWidth(1);
      INCDP->SetFillStyle(3001);
      INCDP->SetFillColor(kYellow);
      INCDP->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      INCDP->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      INCDP->GetYaxis()->SetTitle("1/N_{NPE} #upoint dN/d(#Delta)#phi");
      INCDP->GetYaxis()->SetTitleOffset(1.55);
      if(ptbin == 0)
	INCDP->SetTitle("#Delta#phi Non-Photonic Electrons and Hadrons");
      else if (ptbin == 1 && trig !=3)
	INCDP->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	INCDP->SetTitle("MB");
      else
	INCDP->SetTitle("");
      INCDP->Draw("");
      lbl[ptbin]->Draw("same");

      // Setup unscaled for fractionFit (don't normalize here!)
      unscaleINCDP->SetLineColor(kBlack);
      unscaleINCDP->SetLineWidth(1);
      unscaleINCDP->SetFillStyle(3001);
      unscaleINCDP->SetFillColor(kYellow);
      unscaleINCDP->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      unscaleINCDP->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      unscaleINCDP->GetYaxis()->SetTitle("1/N_{NPE} #upoint dN/d(#Delta)#phi");
      unscaleINCDP->GetYaxis()->SetTitleOffset(1.55);
      if(ptbin == 0)
	unscaleINCDP->SetTitle("#Delta#phi Non-Photonic Electrons and Hadrons");
      else if (ptbin == 1 && trig !=3)
	unscaleINCDP->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	unscaleINCDP->SetTitle("MB");
      else
	unscaleINCDP->SetTitle("");

      // nSigmaPion QA
      nSigPi[trig]->cd(ptbin+1);
      NSPI[ptbin][trig]->SetLineColor(kGreen+3);
      NSPI[ptbin][trig]->SetLineWidth(1);
      NSPI[ptbin][trig]->GetXaxis()->SetTitle("n#sigma_{#pi}");
      NSPI[ptbin][trig]->GetXaxis()->SetRangeUser(-2.,2.);
      if(ptbin == 0)
	NSPI[ptbin][trig]->SetTitle("n Sigma Pion (n#sigma_{#pi})");
      else if (ptbin == 1 && trig !=3)
	NSPI[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	NSPI[ptbin][trig]->SetTitle("MB");
      else
	NSPI[ptbin][trig]->SetTitle("");
      NSPI[ptbin][trig]->Draw("");
      lbl[ptbin]->Draw("same");
      
    }

    // Make projections of hadron pt bins
    const Int_t numHPtBins = 4;
    Float_t lowhpt[numHPtBins] ={0.2,0.5,1.0,1.5};
    Float_t highhpt[numHPtBins]={0.2,0.5,1.0,1.5};
    mh3nTracksZdcx[trig] = (TH3F*)f->Get(Form("mh3nTracksZdcx_%i",trig));
    TH1D* projZDCx[numHPtBins][numTrigs];
    TProfile2D* profileZDCx[numTrigs];
   
    profileZDCx[trig] = mh3nTracksZdcx[trig]->Project3DProfile("zx");

    for(Int_t ptbin=0; ptbin<numHPtBins; ptbin++)
      {
	// - Make projections into electron ptbins
	projZDCx[ptbin][trig] = profileZDCx[trig]->ProjectionX(Form("projZDCx_%i_%i",ptbin,trig),profileZDCx[trig]->GetYaxis()->FindBin(lowhpt[ptbin]),profileZDCx[trig]->GetYaxis()->FindBin(highhpt[ptbin]));

	// plot projections
	pile[trig]->cd(ptbin+1);
	projZDCx[ptbin][trig]->SetLineColor(kBlack);
	projZDCx[ptbin][trig]->GetXaxis()->SetTitle("ZDCx");
	projZDCx[ptbin][trig]->GetYaxis()->SetTitle("<nTracks>");

	gStyle->SetOptFit(1111);
	if(ptbin == 0)
	  projZDCx[ptbin][trig]->SetTitle("p_{T,h} > 0.2 GeV/c");
	if(ptbin == 1)
	  projZDCx[ptbin][trig]->SetTitle("p_{T,h} > 0.5 GeV/c");
	if(ptbin == 2)
	  projZDCx[ptbin][trig]->SetTitle("p_{T,h} > 1.0 GeV/c");
	if(ptbin == 3)
	  projZDCx[ptbin][trig]->SetTitle("p_{T,h} > 1.5 GeV/c");
	projZDCx[ptbin][trig]->Fit("pol1");
	projZDCx[ptbin][trig]->GetFunction("pol1")->SetLineColor(kRed);
	TPaveStats *st = ((TPaveStats*)(projZDCx[ptbin][trig]->GetListOfFunctions()->FindObject("stats")));
	if (st) {
	  st->SetTextColor(projZDCx[ptbin][trig]->GetFunction("pol1")->GetLineColor());
	  st->SetX1NDC(0.64); st->SetX2NDC(0.99);
	  st->SetY1NDC(0.4); st->SetY2NDC(0.6);
	}
	pile[trig]->Modified();pile[trig]->Update();
	projZDCx[ptbin][trig]->Draw("");
      }
  }
  
  // Draw on "SinglePlot" canvas for saving single plots from grid
  TPad* pNew = (TPad*)result[2]->GetPad(4)->Clone();
  singlePlot->cd();
  pNew->ResizePad();
  pNew->Draw();  

  // Make PDF with output canvases
  if(makePDF)
    {
      //Set front page
      TCanvas* fp = new TCanvas("fp","Front Page",100,0,1000,900);
      fp->cd();
      TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
      bLabel->SetFillColor(38);
      bLabel->Draw();
      TLatex tl;
      tl.SetNDC();
      tl.SetTextColor(kWhite);
      tl.SetTextSize(0.033);
      char tlName[100];
      char tlName2[100];
      
      TString titlename = FileName;
      int found = titlename.Last('/');
      if(found >= 0){
	titlename.Replace(0, found+1, "");
      } 
      sprintf(tlName, "RUN 12 pp 200 GeV NPE-h    #Delta#phi Analysis");
      tl.SetTextSize(0.05);
      tl.SetTextColor(kWhite);
      tl.DrawLatex(0.05, 0.92,tlName);
      
      TBox *bFoot = new TBox(0.01, 0.01, 0.99, 0.12);
      bFoot->SetFillColor(38);
      bFoot->Draw();
      tl.SetTextColor(kWhite);
      tl.SetTextSize(0.05);
      tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
      tl.SetTextColor(kBlack);
      tl.SetTextSize(0.03);
      tl.DrawLatex(0.1, 0.14, titlename);
      sprintf(tlName,"eID: -1 < n  #sigma_{e TPC} < 3;  #left|gDCA #right| < 1 cm; 0.3 < p/E < 1.5;");
      tl.DrawLatex(0.1, 0.8,tlName);
      sprintf(tlName,"       nHitsFit > 20; nHits   #frac{dE}{dx} > 15; nHitFit/Max > 0.52;    #left|#eta#right| < 0.7;");
      tl.DrawLatex(0.1, 0.75,tlName);
      sprintf(tlName,"       n #phi > 1; n #eta > 1;  #left|dZ#right| < 3 cm;  #left|d#phi#right| < 0.015;");
      tl.DrawLatex(0.1, 0.7,tlName);
      sprintf(tlName,"hID: p_{T} > 0.5;  #left|#eta#right| < 1; nHitsFit > 15; nHits   #frac{dE}{dx} > 10; DCA < 1 cm;");
      tl.DrawLatex(0.1, 0.6,tlName);
      sprintf(tlName,"Event:  #left|V_{z}#right| < 35 cm;");
      tl.DrawLatex(0.1, 0.5,tlName);
       sprintf(tlName,"Triggers:  BHT0; BHT2;");
      tl.DrawLatex(0.1, 0.4,tlName);
      
      
      // Place canvases in order
      TCanvas* temp = new TCanvas();
      sprintf(name, "%s.pdf[", FileName);
      temp->Print(name);
      sprintf(name, "%s.pdf", FileName);
      temp = fp; // print front page
      temp->Print(name);
      temp = mixedC;
      temp->Print(name);
      temp = mixedCbinEta;
      temp->Print(name);
      temp = mixedCbinPhi;
      temp->Print(name);
      temp = mixedCbin;
      temp->Print(name);
      for(Int_t ii=0; ii<numTrigs; ii++)
	{
	  if(!fPaintAll && (ii==1 || ii==3))
	    continue;
	  temp = IN[ii];
	  temp->Print(name);
	  temp = c[ii];
	  temp->Print(name);
	  temp = cHH[ii];
	  temp->Print(name);
	  temp = result[ii];
	  temp->Print(name);
	  /*temp = pile[ii];
	  temp->Print(name);
	  temp = inMass[ii];
	  temp->Print(name);
	  temp = USComp[ii];
	  temp->Print(name);
	  temp = LSComp[ii];
	  temp->Print(name);
	  temp = InclComp[ii];
	  temp->Print(name);*/
	  temp = nSigPi[ii];
	  temp->Print(name);
	  
	}
      sprintf(name, "%s.pdf]", FileName);
      temp->Print(name);
    }

   if(makeROOT)
    {
      file->Write();
      file->Close();
    }
}

Bool_t checkPaintAllTrigs(){
  
  Bool_t cPaintAll = kFALSE;
  Int_t number = 2;
  while(number > 1 || number < 0){
    std::cout << "Show all trigs? [default: 0]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ) {
      std::istringstream stream( input );
      stream >> number;
      if(number == 1)
	cPaintAll=kTRUE;
    }
    else
      number = 0;
  }

  return cPaintAll;

}

void checkBatchMode(){

 // sets batch mode, so don't draw canvas
  Int_t number = 2;
  while(number > 1 || number < 0){
    std::cout << "Batch Mode? [default: 1]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ) {
      std::istringstream stream( input );
      stream >> number;
      if(number == 0)
	gROOT->SetBatch(kFALSE);
      if(number == 1)
	gROOT->SetBatch(kTRUE);
    }
    else
      {
	number = 1;
	gROOT->SetBatch(kTRUE);
      }
  }
}

Bool_t checkMakePDF(){

  // Set option for pdf creation
  Int_t number = 2; Bool_t fmakePDF = kTRUE;
  while(number > 1 || number < 0){
    std::cout << "Make PDF? [default: 1]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ){
      std::istringstream stream( input );
      stream >> number;
      if(number == 0)
	fmakePDF = kFALSE;
      if(number == 1)
	fmakePDF = kTRUE;
    }
    else
      number = 1; 
  }
  return fmakePDF;
}

Bool_t checkMakeRoot(){

  // Set option for .root creation
  Int_t number = 2; Bool_t fmakeROOT = kTRUE;
  while(number > 1 || number < 0){
    std::cout << "Make .root? [default: 1]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ){
      std::istringstream stream( input );
      stream >> number;
      if(number == 0)
	fmakeROOT = kFALSE;
      if(number == 1){
	fmakeROOT = kTRUE;
      }
    }
    else
      number = 1; 
  }
  return fmakeROOT;
}
