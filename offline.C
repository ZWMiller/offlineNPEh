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
  TH1F::SetDefaultSumw2();
  // Set Style parameters for this macro
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
  gErrorIgnoreLevel = kError; // Set Verbosity Level (kPrint shows all)

  // Set Output options
  Int_t number;
  //Bool_t fPaintAll = checkPaintAllTrigs();
  Bool_t fPaintAll = kFALSE;
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
  Float_t hptMax=anaConst::hptMax; // Set max above range to allow overflow
  Float_t lowPhi=anaConst::lowPhi, highPhi=anaConst::highPhi;
  Double_t pu[2]; // To store fit parameters for later use
  Double_t hhNorm, HHScale, hadPur;

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
  TH2F* mh2PtEZdcx[numTrigs];
  TH2F* mh2PtEZdcxUS[numTrigs];
  TH2F* mh2PtEZdcxLS[numTrigs];
  TH2F* mh2PtEZdcxHad[numTrigs];
  TH3F* mh3nTracksZdcx[numTrigs];
  TH3F* mh3nTracksZdcxUS[numTrigs];
  TH3F* mh3nTracksZdcxLS[numTrigs];
  TH3F* mh3nTracksZdcxHad[numTrigs];
  TH3F* mh3MixedDelPhi;
  TH3F* mh3MixedDelEta;
  TH3F* mh3MixedEtaPhi;
  TH3F* mh3DelPhiHadHad[numTrigs];
  TH2F* mh2nSigmaPion[numTrigs];
  TH1F* mh1PtHadTracks[numTrigs];
  TH1F* mh1PtETracks[numTrigs];
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
  TH1D* projZDCxHad[numPtBins][numTrigs];
  TH1D* projZDCxTrig[numPtBins][numTrigs];
  TH1D* projZDCxHadUS[numPtBins][numTrigs];
  TH1D* projZDCxTrigUS[numPtBins][numTrigs];
  TH1D* projZDCxHadLS[numPtBins][numTrigs];
  TH1D* projZDCxTrigLS[numPtBins][numTrigs];
  TH1D* projZDCxHadHad[numPtBins][numTrigs];
  TH1D* projZDCxTrigHad[numPtBins][numTrigs];
  TH1D* pileTrigs[numPtBins][numTrigs];
  // 1D 2.5-3.5 GeV Check
  TH1F* mh1delPhiIncl;
  TH1F* mh1delPhiUS;
  TH1F* mh1delPhiLS;
  TH1F* mh1delPhiHad;
  TH1F* mh1TrigCount;
  TH1F* inclusivePU;
  TH1F* unlikePU;
  TH1F* likePU;
  TH1F* hadronPU;
  // For Comparison of Dists in different trigs
  TH1F* inclTrig[numPtBins][numTrigs];
  TH1F* npeTrig[numPtBins][numTrigs];
  TH1F* usTrig[numPtBins][numTrigs];
  TH1F* lsTrig[numPtBins][numTrigs];
  TH1F* hadTrig[numPtBins][numTrigs];
  TProfile2D* profileZDCx[numTrigs];
  TCanvas * c[numTrigs];
  TCanvas * c2[numTrigs];
  TCanvas * IN[numTrigs];
  TCanvas * IN2[numTrigs];
  TCanvas * pile[numTrigs];
  TCanvas * pileHad[numTrigs];
  TCanvas * pileTrig[numTrigs];
  TCanvas * result[numTrigs];
  TCanvas * result2[numTrigs];
  TCanvas * inMass[numTrigs];
  TCanvas * USComp[numTrigs];
  TCanvas * LSComp[numTrigs];
  TCanvas * InclComp[numTrigs];
  TCanvas * cHH[numTrigs];
  TCanvas * nSigPi[numTrigs];
  TCanvas * allDist[numTrigs];
  TCanvas * mixedC;
  TCanvas * mixedCbinEta;
  TCanvas * mixedCbinPhi;
  TCanvas * mixedCbin;
  TCanvas * c2535;
  TCanvas * singlePlot;
  TCanvas * inclComp;
  TCanvas * usComp;
  TCanvas * lsComp;
  TCanvas * hadComp;
  TCanvas * npeComp;

  TPaveText* lbl[numPtBins];
  TPaveText* pulbl[numPtBins];
  TPaveText* stat[numPtBins];
  char textLabel[100];
  singlePlot =  new TCanvas("singlePlot","Single Plot",150,0,1150,1000);
  inclComp   =  new TCanvas("inclComp","Trigger Comparison Incl",150,0,1150,1000);
  usComp     =  new TCanvas("usComp","Trigger Comparison US",150,0,1150,1000);
  lsComp     =  new TCanvas("lsComp","Trigger Comparison LS",150,0,1150,1000);
  hadComp    =  new TCanvas("hadComp","Trigger Comparison HAD",150,0,1150,1000);
  npeComp    =  new TCanvas("npeComp","Trigger Comparison NPE",150,0,1150,1000);
  inclComp->Divide(3,4);
  usComp->Divide(3,4);
  lsComp->Divide(3,4);
  hadComp->Divide(3,4);
  npeComp->Divide(3,4);
  // Trigger Independent Hists
  Float_t parCom[5] = {.26214, 4.75137, .526075, .0276979, .00054599};
  Float_t xCom = 3.;
  Float_t epsCom = parCom[0]*TMath::Log(parCom[1]*xCom - parCom[2]*xCom*xCom + parCom[3]*xCom*xCom*xCom - parCom[4]*xCom*xCom*xCom*xCom);
  Float_t pCom[3] = {0.9743, 0.02128, -0.00438};
  Float_t pur = pCom[0] + (pCom[1]*xCom)+(pCom[2]*xCom*xCom);
  Float_t hadPurCom = 1-pur;

  c2535 =  new TCanvas("c2535","2.5-3.5 GeV, 0.3>hpt",150,0,1150,1000);
  c2535->cd();
  mh1delPhiIncl = (TH1F*)f->Get("mh1delPhiIncl");
  mh1delPhiUS   = (TH1F*)f->Get("mh1delPhiUS");
  mh1delPhiLS   = (TH1F*)f->Get("mh1delPhiLS");
  mh1delPhiHad  = (TH1F*)f->Get("mh1delPhiHad");
  mh1TrigCount  = (TH1F*)f->Get("mh1TrigCount");
  mh1delPhiIncl -> Rebin(2);
  mh1delPhiUS   -> Rebin(2);
  mh1delPhiLS   -> Rebin(2);
  mh1delPhiHad  -> Rebin(2);
  TH1F* npe2535 = (TH1F*)mh1delPhiIncl->Clone();
  npe2535->SetName("npe2535");
  TH1F* US2535  = (TH1F*)mh1delPhiUS->Clone();
  TH1F* LS2535  = (TH1F*)mh1delPhiLS->Clone();
  TH1F* Had2535 = (TH1F*)mh1delPhiHad->Clone();
  Float_t norm2535 = (Float_t)mh1TrigCount->GetBinContent(1) - (1/epsCom - 1.)*(Float_t)mh1TrigCount->GetBinContent(2) + (1/epsCom)*(Float_t)mh1TrigCount->GetBinContent(3) - (mh1TrigCount->GetBinContent(1)/mh1TrigCount->GetBinContent(4))*hadPurCom*mh1TrigCount->GetBinContent(4);
  US2535->Scale(1./epsCom - 1.); // Scale each distribution by associated factors
  LS2535->Scale(1./epsCom);
  Had2535->Scale(HHScale*hadPur);
  npe2535->Add(US2535,-1);
  npe2535->Add(LS2535,1);
  npe2535->Add(Had2535,-1);
  npe2535->Scale(1./norm2535);
  npe2535->GetXaxis()->SetRangeUser(lowPhi,highPhi);

  // Plot All
  mh1delPhiIncl -> Scale(1./mh1TrigCount->GetBinContent(1));
  mh1delPhiUS -> Scale(1./mh1TrigCount->GetBinContent(2));
  mh1delPhiLS -> Scale(1./mh1TrigCount->GetBinContent(3));
  mh1delPhiHad -> Scale(1./mh1TrigCount->GetBinContent(4));
  mh1delPhiIncl->SetLineColor(7);
  mh1delPhiLS->SetLineColor(kBlue);
  mh1delPhiIncl->GetYaxis()->SetRangeUser(0.01,2);
  gPad->SetLogy(1);
  mh1delPhiIncl->GetXaxis()->SetRangeUser(lowPhi,highPhi);
  mh1delPhiIncl->SetMarkerStyle(20);
  mh1delPhiUS->SetMarkerStyle(21);
  mh1delPhiLS->SetMarkerStyle(22);
  mh1delPhiHad->SetMarkerStyle(23);
  mh1delPhiIncl->SetMarkerColor(7);
  mh1delPhiIncl->SetMarkerSize(0.3);
  mh1delPhiUS->SetMarkerSize(0.3);
  mh1delPhiLS->SetMarkerSize(0.3);
  mh1delPhiHad->SetMarkerSize(0.3);
  mh1delPhiIncl->SetMarkerColor(7);
  mh1delPhiUS->SetMarkerColor(kRed);
  mh1delPhiLS->SetMarkerColor(kBlue);
  mh1delPhiHad->SetMarkerColor(kGreen+3);
  npe2535->SetMarkerStyle(20);
  npe2535->SetMarkerSize(0.5);
  npe2535->SetMarkerColor(kBlack);
  mh1delPhiIncl->Draw("");
  mh1delPhiUS->Draw("same");
  mh1delPhiLS->Draw("same");
  mh1delPhiHad->Draw("same");
  npe2535->Draw("same");

  /// Mixed Event
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
    pileHad[trig]  = new TCanvas(Form("pileHad%i",trig),"Pileup Hadron per Event",150,0,1150,1000);
    pileTrig[trig] = new TCanvas(Form("pileTrig%i",trig),"Pileup Trigger per Event",150,0,1150,1000);
    inMass[trig]   = new TCanvas(Form("inMass%i",trig),"Invariant Mass",150,0,1150,1000);
    result[trig]   = new TCanvas(Form("result%i",trig),"Inclusive - Photonic",150,0,1150,1000);
    USComp[trig]   = new TCanvas(Form("USComp%i",trig),"Unlike Sign Distributions",150,0,1150,1000);
    LSComp[trig]   = new TCanvas(Form("LSComp%i",trig),"Like Sign Distributions",150,0,1150,1000);
    InclComp[trig] = new TCanvas(Form("InclComp%i",trig),"Inclusive Distributions",150,0,1150,1000);
    cHH[trig]      = new TCanvas(Form("cHH%i",trig),"Hadron-Hadron Distributions",150,0,1150,1000);
    nSigPi[trig]   = new TCanvas(Form("nSigPi_%i",trig),"n#sigma#pi QA",150,0,1150,1000);
    allDist[trig]  = new TCanvas(Form("allDist_%i",trig),"Distribution Comparison",150,0,1150,1000);
    c[trig]        -> Divide(4,3);
    inMass[trig]   -> Divide(4,3);
    IN[trig]       -> Divide(4,3);
    //pile[trig]     -> Divide(2,2);
    //pileHad[trig]  -> Divide(2,2);
    //pileTrig[trig] -> Divide(2,2);
    result[trig]   -> Divide(4,3);
    USComp[trig]   -> Divide(4,3);
    LSComp[trig]   -> Divide(4,3);
    InclComp[trig] -> Divide(4,3);
    cHH[trig]      -> Divide(4,3);
    nSigPi[trig]   -> Divide(4,3);
    allDist[trig]  -> Divide(4,3);


    // Pileup Calculation (using just the hPtCut in the anaConst.h), Done here since need fit results in deltaPhi
    mh3nTracksZdcx[trig]    = (TH3F*)f->Get(Form("mh3nTracksZdcx_%i_%i",trig,0));   // originally filled for various hpT cuts, use 0 which starts at hpt > 0.3
    mh3nTracksZdcxUS[trig]  = (TH3F*)f->Get(Form("mh3nTracksZdcxUS_%i_%i",trig,0)); // These histos are (epT,hpT,ZDCx), get nHadrons vs ZDCx
    mh3nTracksZdcxLS[trig]  = (TH3F*)f->Get(Form("mh3nTracksZdcxLS_%i_%i",trig,0));
    mh3nTracksZdcxHad[trig] = (TH3F*)f->Get(Form("mh3nTracksZdcxHad_%i_%i",trig,0));
    mh3nTracksZdcx[trig]->Sumw2(); mh3nTracksZdcxUS[trig]->Sumw2(); mh3nTracksZdcxLS[trig]->Sumw2();  mh3nTracksZdcxHad[trig]->Sumw2();

    mh2PtEZdcx[trig]       = (TH2F*)f->Get(Form("mh2PtEZdcx_%i",trig));            // Filled (epT,ZDCx). Get nTrigs vs ZDCx
    mh2PtEZdcxUS[trig]     = (TH2F*)f->Get(Form("mh2PtEZdcxUS_%i",trig));
    mh2PtEZdcxLS[trig]     = (TH2F*)f->Get(Form("mh2PtEZdcxLS_%i",trig));
    mh2PtEZdcxHad[trig]     = (TH2F*)f->Get(Form("mh2PtEZdcxHad_%i",trig));
    mh2PtEZdcx[trig]->Sumw2(); mh2PtEZdcxUS[trig]->Sumw2(); mh2PtEZdcxLS[trig]->Sumw2(); mh2PtEZdcxHad[trig]->Sumw2();

    Double_t pulowpt[4]  = {2.5,4,6,8};
    Double_t puhighpt[4] = {20,6,8,20};
    if(trig == 0){pulowpt[0]=2.5; puhighpt[0]=10.5;}
    if(trig == 2){pulowpt[0]=4.5; puhighpt[0]=10.5;}
    for(Int_t ptbin=0; ptbin<1; ptbin++)// cut in to trigger pt slices
    {
      projZDCxHad[ptbin][trig] = mh3nTracksZdcx[trig]->ProjectionZ(Form("projZDCxHad_%i_%i",ptbin,trig),mh3nTracksZdcx[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh3nTracksZdcx[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1,mh3nTracksZdcx[trig]->GetYaxis()->FindBin(hptCut),mh3nTracksZdcx[trig]->GetYaxis()->FindBin(hptMax));
      projZDCxHadUS[ptbin][trig] = mh3nTracksZdcxUS[trig]->ProjectionZ(Form("projZDCxHadUS_%i_%i",ptbin,trig),mh3nTracksZdcxUS[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh3nTracksZdcxUS[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1,mh3nTracksZdcxUS[trig]->GetYaxis()->FindBin(hptCut),mh3nTracksZdcxUS[trig]->GetYaxis()->FindBin(hptMax));
      projZDCxHadLS[ptbin][trig] = mh3nTracksZdcxLS[trig]->ProjectionZ(Form("projZDCxHadLS_%i_%i",ptbin,trig),mh3nTracksZdcxLS[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh3nTracksZdcxLS[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1,mh3nTracksZdcxLS[trig]->GetYaxis()->FindBin(hptCut),mh3nTracksZdcxLS[trig]->GetYaxis()->FindBin(hptMax));
      projZDCxHadHad[ptbin][trig] = mh3nTracksZdcxHad[trig]->ProjectionZ(Form("projZDCxHadHad_%i_%i",ptbin,trig),mh3nTracksZdcxHad[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh3nTracksZdcxHad[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1,mh3nTracksZdcxHad[trig]->GetYaxis()->FindBin(hptCut),mh3nTracksZdcxHad[trig]->GetYaxis()->FindBin(hptMax));

      projZDCxTrig[ptbin][trig] = mh2PtEZdcx[trig]->ProjectionY(Form("projZDCxTrig_%i_%i",ptbin,trig),mh2PtEZdcx[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh2PtEZdcx[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1);
      projZDCxTrigUS[ptbin][trig] = mh2PtEZdcxUS[trig]->ProjectionY(Form("projZDCxTrigUS_%i_%i",ptbin,trig),mh2PtEZdcxUS[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh2PtEZdcxUS[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1);
      projZDCxTrigLS[ptbin][trig] = mh2PtEZdcxLS[trig]->ProjectionY(Form("projZDCxTrigLS_%i_%i",ptbin,trig),mh2PtEZdcxLS[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh2PtEZdcxLS[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1);
      projZDCxTrigHad[ptbin][trig] = mh2PtEZdcxHad[trig]->ProjectionY(Form("projZDCxTrigHad_%i_%i",ptbin,trig),mh2PtEZdcxHad[trig]->GetXaxis()->FindBin(pulowpt[ptbin]),mh2PtEZdcxHad[trig]->GetXaxis()->FindBin(puhighpt[ptbin])-1);

      // Rebin to make the statistics better
      Int_t RBpu = 5;
      projZDCxHad[ptbin][trig]     -> Rebin(RBpu);
      projZDCxHadUS[ptbin][trig]   -> Rebin(RBpu);
      projZDCxHadLS[ptbin][trig]   -> Rebin(RBpu);
      projZDCxHadHad[ptbin][trig]  -> Rebin(RBpu);
      projZDCxTrig[ptbin][trig]    -> Rebin(RBpu);
      projZDCxTrigUS[ptbin][trig]  -> Rebin(RBpu);
      projZDCxTrigLS[ptbin][trig]  -> Rebin(RBpu);
      projZDCxTrigHad[ptbin][trig] -> Rebin(RBpu);

      pulbl[ptbin] = new TPaveText(.15,.15,.35,.23,Form("NB NDC%i",ptbin));
      sprintf(textLabel,"%.1f < P_{T,e} < %.1f",pulowpt[ptbin],puhighpt[ptbin]);
      pulbl[ptbin]->AddText(textLabel);
      pulbl[ptbin]->SetFillColor(kWhite);

      // Make plots for each subset
      inclusivePU = (TH1F*)projZDCxHad[ptbin][trig]->Clone(); inclusivePU->Divide(projZDCxTrig[ptbin][trig]);
      inclusivePU->SetName(Form("inclusivePU_%i",trig));
      unlikePU = (TH1F*)projZDCxHadUS[ptbin][trig]->Clone(); unlikePU->Divide(projZDCxTrigUS[ptbin][trig]);
      unlikePU->SetName(Form("unlikePU_%i",trig));
      likePU = (TH1F*)projZDCxHadLS[ptbin][trig]->Clone(); likePU->Divide(projZDCxTrigLS[ptbin][trig]);
      likePU->SetName(Form("likePU_%i",trig));
      hadronPU = (TH1F*)projZDCxHadHad[ptbin][trig]->Clone(); hadronPU->Divide(projZDCxTrigHad[ptbin][trig]);
      hadronPU->SetName(Form("hadronPU_%i",trig));
      inclusivePU->SetLineColor(kMagenta);
      unlikePU->SetLineColor(kRed);
      likePU->SetLineColor(kBlue);
      hadronPU->SetLineColor(kGreen+3);
      inclusivePU->SetMarkerColor(kMagenta);
      unlikePU->SetMarkerColor(kRed);
      likePU->SetMarkerColor(kBlue);
      hadronPU->SetMarkerColor(kGreen+3);
      inclusivePU->SetMarkerStyle(20);
      unlikePU->SetMarkerStyle(21);
      likePU->SetMarkerStyle(22);
      hadronPU->SetMarkerStyle(23);	

      // Draw
      pileHad[trig]->cd();
      projZDCxHad[ptbin][trig]->SetLineColor(kBlack);
      projZDCxHadUS[ptbin][trig]->SetLineColor(kRed);
      projZDCxHadLS[ptbin][trig]->SetLineColor(kBlue);
      projZDCxHadHad[ptbin][trig]->SetLineColor(kGreen+3);
      projZDCxTrig[ptbin][trig]->SetLineColor(kBlack);
      projZDCxTrigUS[ptbin][trig]->SetLineColor(kRed);
      projZDCxTrigLS[ptbin][trig]->SetLineColor(kBlue);
      projZDCxTrigHad[ptbin][trig]->SetLineColor(kGreen+3);
      TLegend* legPU = new TLegend(0.45,0.6,0.85,0.79);
      legPU->AddEntry(projZDCxHad[ptbin][trig],"Semi-Inclusive Trigs","lpe");
      legPU->AddEntry(projZDCxHadUS[ptbin][trig],"Unlike Sign Trigs","lpe");
      legPU->AddEntry(projZDCxHadLS[ptbin][trig],"Like Sign Trigs","lpe");
      legPU->AddEntry(projZDCxHadHad[ptbin][trig],"Hadron-Hadron Trigs","lpe");
      projZDCxHad[ptbin][trig]->GetXaxis()->SetTitle("nHadrons");
      projZDCxHad[ptbin][trig] -> DrawCopy();
      projZDCxHadUS[ptbin][trig] -> DrawCopy("same");
      projZDCxHadLS[ptbin][trig] -> DrawCopy("same");
      projZDCxHadHad[ptbin][trig] -> DrawCopy("same");
      legPU->Draw("same");
      pileTrig[trig]->cd();
      projZDCxTrig[ptbin][trig]->GetXaxis()->SetTitle("nTriggers");
      projZDCxTrig[ptbin][trig] -> DrawCopy();
      projZDCxTrigUS[ptbin][trig] -> DrawCopy("same");
      projZDCxTrigLS[ptbin][trig] -> DrawCopy("same");
      projZDCxTrigHad[ptbin][trig] -> DrawCopy("same");
      legPU->Draw("same");
      pulbl[ptbin]->Draw("same");

      // Get Total number of hadrons in pileup (first scale each distribution by efficiency, just like data)
      projZDCxHadUS[ptbin][trig] -> Scale(1./epsilon[ptbin] - 1.);
      projZDCxHadLS[ptbin][trig] -> Scale(1./epsilon[ptbin]);
      projZDCxHadHad[ptbin][trig]-> Scale(HHScale*hadPur);
      projZDCxHad[ptbin][trig] -> Add(projZDCxHadUS[ptbin][trig], -1.);
      projZDCxHad[ptbin][trig] -> Add(projZDCxHadLS[ptbin][trig], 1.);
      projZDCxHad[ptbin][trig] -> Add(projZDCxHadHad[ptbin][trig], -1.);

      // Get Total number of trigs in pileup (first scale each distribution by efficiency, just like data)
      projZDCxTrigUS[ptbin][trig] -> Scale(1./epsilon[ptbin] - 1.);
      projZDCxTrigLS[ptbin][trig] -> Scale(1./epsilon[ptbin]);
      projZDCxTrigHad[ptbin][trig]-> Scale(HHScale*hadPur);
      projZDCxTrig[ptbin][trig] -> Add(projZDCxTrigUS[ptbin][trig], -1.);
      projZDCxTrig[ptbin][trig] -> Add(projZDCxTrigLS[ptbin][trig], 1.);
      projZDCxTrig[ptbin][trig] -> Add(projZDCxTrigHad[ptbin][trig], -1.);

      // Actually do the division of total hadrons/total trigs
      projZDCxHad[ptbin][trig]->Divide(projZDCxTrig[ptbin][trig]);

      // plot projections
      pile[trig]->cd();
      projZDCxHad[ptbin][trig]->SetLineColor(kBlack);
      projZDCxHad[ptbin][trig]->SetMarkerStyle(25);
      projZDCxHad[ptbin][trig]->GetXaxis()->SetTitle("ZDCx");
      projZDCxHad[ptbin][trig]->GetYaxis()->SetTitle("<nHadrons>/<nTrigs>");
      projZDCxHad[ptbin][trig]->GetYaxis()->SetRangeUser(0,15);
      // 
      gStyle->SetOptFit(1111);
      projZDCxHad[ptbin][trig]->Fit("pol1","Q");
      projZDCxHad[ptbin][trig]->GetFunction("pol1")->SetLineColor(kBlack);
      TPaveStats *st = ((TPaveStats*)(projZDCxHad[ptbin][trig]->GetListOfFunctions()->FindObject("stats")));
      if (st) {
        st->SetTextColor(projZDCxHad[ptbin][trig]->GetFunction("pol1")->GetLineColor());
        st->SetX1NDC(0.64); st->SetX2NDC(0.99);
        st->SetY1NDC(0.4); st->SetY2NDC(0.6);
      }
      pile[trig]->Modified();pile[trig]->Update();
      projZDCxHad[ptbin][trig]->Draw("");
      pulbl[ptbin]->Draw("same");

      // Draw for each Dist
      inclusivePU ->Draw("same");
      unlikePU    ->Draw("same");
      likePU      ->Draw("same");
      hadronPU    ->Draw("same");

      TLegend* pileupLeg = new TLegend(0.2,0.73,0.55,0.85);
      pileupLeg->AddEntry(projZDCxHad[ptbin][trig],"NPE","lpe");
      pileupLeg->AddEntry(inclusivePU,"Semi-Inclusive","lpe");
      pileupLeg->AddEntry(unlikePU,"Unlike Sign","lpe");
      pileupLeg->AddEntry(likePU,"Like Sign","lpe");
      pileupLeg->AddEntry(hadronPU,"Hadron","lpe");
      pileupLeg->Draw("same");

      // Get Fit information and store to use in corrections
      TF1 *fitResult = projZDCxHad[ptbin][trig]->GetFunction("pol1");
      pu[0] = fitResult->GetParameter(0);
      pu[1] = fitResult->GetParameter(1);
      inclusivePU->Fit("pol1","Q");
      TF1* inclusiveFIT = inclusivePU->GetFunction("pol1");
      unlikePU->Fit("pol1","Q");
      TF1* unlikeFIT = unlikePU->GetFunction("pol1");
      likePU->Fit("pol1","Q");
      TF1* likeFIT = likePU->GetFunction("pol1");
      hadronPU->Fit("pol1","Q");
      TF1* hadronFIT = hadronPU->GetFunction("pol1");
      inclusiveFIT->SetLineColor(kMagenta);
      unlikeFIT->SetLineColor(kRed);
      likeFIT->SetLineColor(kBlue);
      hadronFIT->SetLineColor(kGreen+3);

      // For making table of fit errors
     /* cout <<"NPEtrig " << trig << " " << pu[0] <<" " << fitResult->GetParError(0) << " " << pu[1] <<" " << fitResult->GetParError(1) << endl;
      cout << "InclusiveTrig: " << trig <<" "<< inclusiveFIT->GetParameter(0) <<" "<< inclusiveFIT->GetParError(0)<< " " << inclusiveFIT->GetParameter(1) <<" "<<inclusiveFIT->GetParError(1)<< endl;
      cout << "unlikeTrig: " << trig <<" "<< unlikeFIT->GetParameter(0)  <<" "<< unlikeFIT->GetParError(0)<< " " << unlikeFIT->GetParameter(1) <<" "<<unlikeFIT->GetParError(1)<< endl;
      cout << "likeTrig: " << trig <<" "<< likeFIT->GetParameter(0) << " " << likeFIT->GetParError(0)<<" "<< likeFIT->GetParameter(1) <<" "<<likeFIT->GetParError(1)<< endl;
      cout << "hadronTrig: " << trig <<" "<< hadronFIT->GetParameter(0) << " " << hadronFIT->GetParError(0)<<" "<< hadronFIT->GetParameter(1) <<" "<<hadronFIT->GetParError(1)<< endl;
*/

    }

    // Make Projections (first get 2d/3d hists, then project)
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
      projDelPhiIncl[ptbin][trig] = mh3DelPhiIncl[trig]->ProjectionX(Form("projDelPhiIncl_%i_%i",ptbin,trig),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(highpt[ptbin])-1,mh3DelPhiIncl[trig]->GetZaxis()->FindBin(hptCut),mh3DelPhiIncl[trig]->GetZaxis()->FindBin(hptMax));

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

      //for pileup correction (need nTrigs per ZDC per ptbin)
      pileTrigs[ptbin][trig] = mh2PtEZdcx[trig]->ProjectionY(Form("projZDCxTrig_%i_%i",ptbin,trig),mh2PtEZdcx[trig]->GetXaxis()->FindBin(lowpt[ptbin]),mh2PtEZdcx[trig]->GetXaxis()->FindBin(highpt[ptbin])-1);
      pileTrigs[ptbin][trig]->SetName(Form("pileTrigs_%i_%i",ptbin,trig));
    }

    for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){

      // Init necessary plotting tools
      lbl[ptbin] = new TPaveText(.15,.15,.35,.23,Form("NB NDC%i",ptbin));
      sprintf(textLabel,"%.1f < P_{T,e} < %.1f",lowpt[ptbin],highpt[ptbin]);
      lbl[ptbin]->AddText(textLabel);
      lbl[ptbin]->SetFillColor(kWhite);

      // Calculate electron purity from pol3 fit of xiaozhi data
      Float_t ptAv = (lowpt[ptbin]+highpt[ptbin])/2.;
      Float_t p[3] = {0.9743, 0.02128, -0.00438};
      Float_t purity = p[0] + (p[1]*ptAv)+(p[2]*ptAv*ptAv);
      hadPur = 1-purity;

      // Calculate PHe Reconstruction Eff from Xiaozhi embedding (fit)
      Float_t par[5] = {.26214, 4.75137, .526075, .0276979, .00054599};
      Float_t x = ptAv;
      Float_t eps = par[0]*TMath::Log(par[1]*x - par[2]*x*x + par[3]*x*x*x - par[4]*x*x*x*x);
      epsilon[ptbin] = eps;

      // Make stats label with purity and effeciency
      char statLabel[100];
      stat[ptbin] = new TPaveText(.5,.7,.85,.87,Form("NB NDC%i",ptbin));
      sprintf(statLabel,"Eff: %.2f; ePure:%.2f",epsilon[ptbin],purity);
      stat[ptbin]->InsertText(statLabel);
      stat[ptbin]->SetFillColor(kWhite);

      // Calculate Normalization for NPE delPhi
      Float_t inclNorm = mh1PtETracks[trig]->Integral(mh1PtETracks[trig]->GetXaxis()->FindBin(lowpt[ptbin]),mh1PtETracks[trig]->GetXaxis()->FindBin(highpt[ptbin])-1);
      Float_t LSNorm   = projInvMassLS[ptbin][trig]->Integral();
      Float_t USNorm   = projInvMassUS[ptbin][trig]->Integral();
      hhNorm   = mh1PtHadTracks[trig]->Integral(mh1PtHadTracks[trig]->GetXaxis()->FindBin(lowpt[ptbin]),mh1PtHadTracks[trig]->GetXaxis()->FindBin(highpt[ptbin])-1);
      HHScale = (Float_t)inclNorm/(Float_t)hhNorm; // so the purity comparison is 1:1
      Float_t Norm = (Float_t)inclNorm - (1/epsilon[ptbin] - 1.)*(Float_t)USNorm + (1/epsilon[ptbin])*(Float_t)LSNorm - HHScale*hadPur*hhNorm; // Use the number of "signal" counts
      histoNorms->SetBinContent(histoNorms->GetBin(trig+1,ptbin+1), Norm); // Find the bin and fill with the Normalization
      // DEBUG - if(trig==0 && ptbin==0) cout << trig << "; " << ptbin << ": " << Norm << " norm2535: " << norm2535 << endl;

      // Calculate pileup for this ptbin
      Double_t pileCorrect,pileNumTrigs;
      for(Int_t zdcBin=0; zdcBin < pileTrigs[ptbin][trig]->GetXaxis()->GetLast();zdcBin++)
      {
        Double_t c=0,n=0;
        n = (Double_t)pileTrigs[ptbin][trig]->GetBinContent(zdcBin);        // Number of trigs in zdc
        c = pu[1]*pileTrigs[ptbin][trig]->GetBinCenter(zdcBin);   // From fit: (p1*zdc+p0)-p0
     //   cout << pileTrigs[ptbin][trig]->GetBinCenter(zdcBin) << " " << c << " " << n << endl;
        pileCorrect += c*n;
        pileNumTrigs += n;
      }
      Double_t dphiBins = (Double_t)projDelPhiIncl[ptbin][trig]->FindBin(3.1415) - (Double_t)projDelPhiIncl[ptbin][trig]->FindBin(-3.1414);
      pileCorrect = pileCorrect/pileNumTrigs/dphiBins;  // divide weighted average by number of bins. 
      cout << "pilecorrection: "<< dphiBins << " " << pileCorrect<< endl; // Subtract this value from every bin before scaling.

      
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
      TH1F *ULDP  = (TH1F*)USIMNP[ptbin][trig]->Clone();
      TH1F *LSDP  = (TH1F*)LSIMNP[ptbin][trig]->Clone();
      TH1F *HADDP = (TH1F*)HHDP[ptbin][trig]->Clone();
      INCDP->SetName(Form("scaleNPEhDelPhi_%i_%i",trig,ptbin));
      ULDP->Scale(1./epsilon[ptbin] - 1.); // Scale each distribution by associated factors
      LSDP->Scale(1./epsilon[ptbin]);
      HADDP->Scale(HHScale*hadPur);
      INCDP->Add(ULDP,-1);
      INCDP->Add(LSDP,1);
      INCDP->Add(HADDP,-1);
      TH1F *unscaleINCDP = (TH1F*)INCDP->Clone();   //
      unscaleINCDP->SetName(Form("NPEhDelPhi_%i_%i",trig,ptbin)); // Make a clone before normalization, to pass to fit program
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

      // All Distributions
      allDist[trig]->cd(ptbin+1);
      TH1F *INCLUSIVE = (TH1F*)INCLNP[ptbin][trig]->Clone();
      TH1F *UNLIKE  = (TH1F*)USIMNP[ptbin][trig]->Clone();
      TH1F *LIKE  = (TH1F*)LSIMNP[ptbin][trig]->Clone();
      TH1F *HADRON = (TH1F*)HHDP[ptbin][trig]->Clone();
      TH1F *USmLS  = (TH1F*)USIMNP[ptbin][trig]->Clone();
      USmLS->Add(LIKE,-1);
      USmLS->Scale(1./(USNorm-LSNorm));
      INCLUSIVE->Scale(1./inclNorm);
      UNLIKE->Scale(1./USNorm);
      LIKE->Scale(1./LSNorm);
      HADRON->Scale(1./hhNorm);
      INCLUSIVE->SetLineColor(7);
      LIKE->SetLineColor(kBlue);
      INCLUSIVE->GetYaxis()->SetRangeUser(0.001,10);
      gPad->SetLogy(1);
      INCLUSIVE->GetXaxis()->SetRangeUser(lowPhi,highPhi);
      INCLUSIVE->SetMarkerStyle(20);
      UNLIKE->SetMarkerStyle(21);
      LIKE->SetMarkerStyle(22);
      HADRON->SetMarkerStyle(23);
      USmLS->SetMarkerStyle(34);
      INCLUSIVE->SetMarkerColor(7);
      INCLUSIVE->SetMarkerSize(0.3);
      UNLIKE->SetMarkerSize(0.3);
      LIKE->SetMarkerSize(0.3);
      HADRON->SetMarkerSize(0.3);
      USmLS->SetMarkerSize(0.3);
      INCLUSIVE->SetMarkerColor(7);
      UNLIKE->SetMarkerColor(kRed);
      LIKE->SetMarkerColor(kBlue);
      HADRON->SetMarkerColor(kGreen+3);
      INCLUSIVE->Draw("");
      UNLIKE->Draw("same");
      LIKE->Draw("same");
      HADRON->Draw("same");
      USmLS->Draw("same");
      INCDP->Draw("same");
      lbl[ptbin]->Draw("same");
      stat[ptbin]->Draw("same");
      TLegend* legAll = new TLegend(0.13,0.7,0.5,0.89);
      legAll->AddEntry(INCLUSIVE,"Inclusive","lpe");
      legAll->AddEntry(UNLIKE,"Unlike Sign","lpe");
      legAll->AddEntry(LIKE,"Like Sign", "lpe");
      legAll->AddEntry(HADRON,"Hadron-Hadron", "lpe");
      legAll->AddEntry(USmLS,"Unlike-Like","lpe");
      legAll->AddEntry(INCDP,"NPE-h","lpe");
      legAll->Draw("same");

      // Clone the distributions for comparisons across trigs
      inclTrig[ptbin][trig]  = (TH1F*)INCLUSIVE->Clone();
      inclTrig[ptbin][trig]  ->SetName(Form("inclTrig_%i",trig));
      usTrig[ptbin][trig]    = (TH1F*)UNLIKE->Clone();
      usTrig[ptbin][trig]    ->SetName(Form("usTrig_%i",trig));
      lsTrig[ptbin][trig]    = (TH1F*)LIKE->Clone();
      lsTrig[ptbin][trig]    ->SetName(Form("lsTrig_%i",trig));
      hadTrig[ptbin][trig]   = (TH1F*)HADRON->Clone();
      hadTrig[ptbin][trig]   ->SetName(Form("hadTrig_%i",trig));
      npeTrig[ptbin][trig]   = (TH1F*)INCDP->Clone();
      npeTrig[ptbin][trig]   ->SetName(Form("npeTrig_%i",trig));
    }
  }


  // Plot all of the distributions for each trigger
  for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++)
  {
    inclTrig[ptbin][0]  -> SetLineColor(kBlack);
    inclTrig[ptbin][0]  -> SetMarkerColor(kBlack);
    usTrig[ptbin][0]    -> SetLineColor(kBlack);
    usTrig[ptbin][0]    -> SetMarkerColor(kBlack);
    lsTrig[ptbin][0]    -> SetLineColor(kBlack);
    lsTrig[ptbin][0]    -> SetMarkerColor(kBlack);
    hadTrig[ptbin][0]   -> SetLineColor(kBlack);
    hadTrig[ptbin][0]   -> SetMarkerColor(kBlack);
    npeTrig[ptbin][0]   -> SetLineColor(kBlack);
    npeTrig[ptbin][0]   -> SetMarkerColor(kBlack);
    inclTrig[ptbin][2]  -> SetLineColor(kRed);
    inclTrig[ptbin][2]  -> SetMarkerColor(kRed);
    usTrig[ptbin][2]    -> SetLineColor(kRed);
    usTrig[ptbin][2]    -> SetMarkerColor(kRed);
    lsTrig[ptbin][2]    -> SetLineColor(kRed);
    lsTrig[ptbin][2]    -> SetMarkerColor(kRed);
    hadTrig[ptbin][2]   -> SetLineColor(kRed);
    hadTrig[ptbin][2]   -> SetMarkerColor(kRed);
    npeTrig[ptbin][2]   -> SetLineColor(kRed);
    npeTrig[ptbin][2]   -> SetMarkerColor(kRed);

  if(ptbin == 0)
  {
    inclTrig[ptbin][0]->SetTitle("Inclusive Trigger DeltaPhi Comparison");
    usTrig[ptbin][0]->SetTitle("Unlike Sign Trigger DeltaPhi Comparison");
    lsTrig[ptbin][0]->SetTitle("Like Sign Trigger DeltaPhi Comparison");
    hadTrig[ptbin][0]->SetTitle("Hadron-Hadron Trigger DeltaPhi Comparison");
    npeTrig[ptbin][0]->SetTitle("NPE Trigger DeltaPhi Comparison");
  }
  else
  {
    inclTrig[ptbin][0]->SetTitle("");
    usTrig[ptbin][0]->SetTitle("");
    lsTrig[ptbin][0]->SetTitle("");
    hadTrig[ptbin][0]->SetTitle("");
    npeTrig[ptbin][0]->SetTitle("");
  }


    //reset ranges
    inclTrig[ptbin][0]->GetYaxis()->SetRangeUser(.001,5);
    usTrig[ptbin][0]->GetYaxis()->SetRangeUser(.001,5);
    lsTrig[ptbin][0]->GetYaxis()->SetRangeUser(.001,5);
    hadTrig[ptbin][0]->GetYaxis()->SetRangeUser(.001,5);
    npeTrig[ptbin][0]->GetYaxis()->SetRangeUser(.001,5);
    lsTrig[ptbin][0]->GetXaxis()->SetRangeUser(lowPhi,highPhi);

    TLegend* compLeg = new TLegend(0.2,0.73,0.55,0.85);
    compLeg -> AddEntry(inclTrig[ptbin][0],"HT0","le");
    compLeg -> AddEntry(inclTrig[ptbin][2],"HT2","le");
    lbl[ptbin] = new TPaveText(.5,.8,.7,.85,Form("NB NDC%i",ptbin));
    sprintf(textLabel,"%.1f < P_{T,e} < %.1f",lowpt[ptbin],highpt[ptbin]);
    lbl[ptbin]->AddText(textLabel);
    lbl[ptbin]->SetFillColor(kWhite);

    inclComp->cd(ptbin+1);
    gPad->SetLogy(1);
    inclTrig[ptbin][0]  -> Draw("p");
    inclTrig[ptbin][2]  -> Draw("same p");
    compLeg->Draw("same");
    lbl[ptbin]->Draw("same");
    usComp->cd(ptbin+1);
    gPad->SetLogy(1);
    usTrig[ptbin][0]    -> Draw("p");
    usTrig[ptbin][2]    -> Draw("same p");
    compLeg->Draw("same");
    lbl[ptbin]->Draw("same");
    lsComp->cd(ptbin+1);
    gPad->SetLogy(1);
    lsTrig[ptbin][0]    -> Draw("p");
    lsTrig[ptbin][2]    -> Draw("same p");
    compLeg->Draw("same");
    lbl[ptbin]->Draw("same");
    hadComp->cd(ptbin+1);
    gPad->SetLogy(1);
    hadTrig[ptbin][0]   -> Draw("p");
    hadTrig[ptbin][2]   -> Draw("same p");
    compLeg->Draw("same");
    lbl[ptbin]->Draw("same");
    npeComp->cd(ptbin+1);
    gPad->SetLogy(1);
    npeTrig[ptbin][0]   -> Draw("p");
    npeTrig[ptbin][2]   -> Draw("same p");
    compLeg->Draw("same");
    lbl[ptbin]->Draw("same");
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
    /*temp = mixedC;
    temp->Print(name);
    temp = mixedCbinEta;
    temp->Print(name);
    temp = mixedCbinPhi;
    temp->Print(name);
    temp = mixedCbin;
    temp->Print(name);*/
    temp = inclComp;
    temp->Print(name);
    temp = usComp;
    temp->Print(name);
    temp = lsComp;
    temp->Print(name);
    temp = hadComp;
    temp->Print(name);
    temp = npeComp;
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
      temp = pile[ii];
      temp->Print(name);
      temp = pileTrig[ii];
      temp->Print(name);
      temp = pileHad[ii];
      temp->Print(name);
      /*temp = inMass[ii];
        temp->Print(name);
        temp = USComp[ii];
        temp->Print(name);
        temp = LSComp[ii];
        temp->Print(name);
        temp = InclComp[ii];
        temp->Print(name);
        temp = nSigPi[ii];
        temp->Print(name);*/
      temp = allDist[ii];
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
