// Offline Plots - Z. Miller July 24, 2015
//
// .L offline.C
// offline("FILENAME") # Without .root Extension

void offline(const char* FileName="test")
{
   
  // Set Style parameters for this macro
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
  gErrorIgnoreLevel = kError; // Set Verbosity Level (kPrint shows all)
  Bool_t fPaintAll = kFALSE;
  Int_t number = 2;
  while(number > 1 || number < 0){
    std::cout << "Show all trigs? [default: 0]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ) {
      std::istringstream stream( input );
      stream >> number;
      if(number == 1)
	fPaintAll=kTRUE;
    }
    else
      number = 0;
  }
  // sets batch mode, so don't draw canvas
  number = 2;
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

    // Set option for pdf creation
  number = 2; Bool_t makePDF = kTRUE;
  while(number > 1 || number < 0){
    std::cout << "Make PDF? [default: 1]: ";
    std::string input;
    std::getline( std::cin, input );
    if ( !input.empty() ){
      std::istringstream stream( input );
      stream >> number;
      if(number == 0)
	makePDF = kFALSE;
      if(number == 1)
	makePDF = kTRUE;
    }
    else
      number = 1; 
  }
  
  // Open ROOT File
  char name[1000];
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/%s.root",FileName);
  TFile *f = new TFile(name,"READ");
  if (f->IsOpen()==kFALSE)
    { std::cout << "!!! File Not Found !!!" << std::endl;
      exit(1); }
  // f->ls(); // - DEBUG by printing all objects in ROOT file
  

  const Int_t numPtBins = 14;
  const Int_t numTrigs = 4;
  Double_t epsilon[numPtBins] = {0.593164, 0.626663, 0.655916, 0.674654, 0.685596, 0.700600, 0.716682, 0.724638, 0.713977, 0.730550, 0.735204, 0.744336, 0.761323, 0.758423};
  Float_t lowpt[14] ={2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.0};
  Float_t highpt[14]={3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.,200.};
  Float_t hptCut=0.5;
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
  TH1D* projInvMassLS[numPtBins][numTrigs];
  TH1D* projInvMassUS[numPtBins][numTrigs];
  TH1D* projMixedDelPhi;
  TH1D* projMixedDelEta;
  TH1D* projEMixedEtaPhi;
  TH1D* projPMixedEtaPhi;
  TH2D* proj2DMixedEtaPhi;
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
  TCanvas * mixedC;
  TCanvas * singlePlot;
  TPaveText* lbl[numPtBins];
  char textLabel[100];
  singlePlot =  new TCanvas("singlePlot","Single Plot",150,0,1150,1000);

  // Trigger Independent Hists
  mixedC = new TCanvas("mixedC","Mixed Events",150,0,1150,1000);
  mixedC->Divide(2,2);
  mh3MixedDelPhi = (TH3F*)f->Get("mh3MixedDelPhi");
  mh3MixedDelEta = (TH3F*)f->Get("mh3MixedDelEta");
  mh3MixedEtaPhi = (TH3F*)f->Get("mh3MixedEtaPhi");
  // ONLY FOR AUG12_1 due to ERROR REMOVE FOR ALL OTHER RUNS
  //mh3MixedEtaPhi = mh3MixedDelEta;
  ///////////////////////////
  projMixedDelPhi   = mh3MixedDelPhi -> ProjectionX("projMixedDelPhi");
  projMixedDelEta   = mh3MixedDelEta -> ProjectionX("projMixedDelEta");
  projPMixedEtaPhi  = mh3MixedEtaPhi -> ProjectionX("projPMixedEtaPhi");
  projEMixedEtaPhi  = mh3MixedEtaPhi -> ProjectionY("projEMixedEtaPhi");
  proj2DMixedEtaPhi = (TH2D*)mh3MixedEtaPhi -> Project3D("yx");
  Int_t RB2 = 4;
  projMixedDelPhi->Rebin(RB2);
  projMixedDelEta->Rebin(RB2);
  projEMixedEtaPhi->Rebin(RB2);
  projPMixedEtaPhi->Rebin(RB2);
  
  mixedC->cd(1);
  mh3MixedEtaPhi->GetXaxis()->SetTitle("#Delta#phi");
  mh3MixedEtaPhi->GetXaxis()->SetRangeUser(-2,5);
  mh3MixedEtaPhi->GetYaxis()->SetTitle("#Delta#eta");
  mh3MixedEtaPhi->GetYaxis()->SetRangeUser(-1.5,1.5);
  mh3MixedEtaPhi->GetZaxis()->SetTitle("P_{t,e}");
  mh3MixedEtaPhi->Draw();
  mixedC->cd(2);
  projPMixedEtaPhi->GetXaxis()->SetRangeUser(-2,5);
  projPMixedEtaPhi->GetXaxis()->SetTitle("#Delta#phi");
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
  proj2DMixedEtaPhi->GetXaxis()->SetRangeUser(-2,5);
  proj2DMixedEtaPhi->GetYaxis()->SetTitle("#Delta#eta");
  proj2DMixedEtaPhi->GetYaxis()->SetRangeUser(-1.5,1.5);
  proj2DMixedEtaPhi->Draw("colz");
 
  for(Int_t trig = 0; trig < numTrigs; trig++){

    if(!fPaintAll && (trig == 1 || trig == 3)) continue; 
    // Create and Segment Canvas
    c[trig] = new TCanvas(Form("c%i",trig),"Photonic Hists",150,0,1150,1000);
    IN[trig]= new TCanvas(Form("IN%i",trig),"Inclusive Hists",150,0,1150,1000);
    pile[trig] = new TCanvas(Form("pile%i",trig),"Pileup Monitor",150,0,1150,1000);
    inMass[trig] = new TCanvas(Form("inMass%i",trig),"Invariant Mass",150,0,1150,1000);
    result[trig] = new TCanvas(Form("result%i",trig),"Inclusive - Photonic",150,0,1150,1000);
    USComp[trig] = new TCanvas(Form("USComp%i",trig),"Unlike Sign Distributions",150,0,1150,1000);
    LSComp[trig] = new TCanvas(Form("LSComp%i",trig),"Like Sign Distributions",150,0,1150,1000);
    InclComp[trig] = new TCanvas(Form("InclComp%i",trig),"Inclusive Distributions",150,0,1150,1000);
    c[trig] -> Divide(4,3);
    inMass[trig]->Divide(4,3);
    IN[trig]-> Divide(4,3);
    pile[trig]->Divide(2,2);
    result[trig]->Divide(4,3);
    USComp[trig]->Divide(4,3);
    LSComp[trig]->Divide(4,3);
    InclComp[trig]->Divide(4,3);

    // Make Projections (first get 2d/3d hists, then project)
    mh2PhiQPt[trig]    = (TH2F*)f->Get(Form("mh2PhiQPt_%i",trig));
    mh2nSigmaEPt[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_%i",trig));
    mh2nSigmaEPt_eID[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_eID_%i",trig));
    mh3DelPhiIncl[trig] = (TH3F*)f->Get(Form("mh3DelPhiIncl_%i",trig));
    mh3DelPhiPhotLS[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLS_%i",trig));
    mh3DelPhiPhotUS[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUS_%i",trig));
    mh3DelPhiPhotUSNP[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUSNP_%i",trig));
    mh3DelPhiPhotLSNP[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLSNP_%i",trig));
    mh3DelPhiInclNP[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotInclNP_%i",trig));
    mh3DelPhiInclWt[trig] = (TH3F*)f->Get(Form("mh3DelPhiInclWt_%i",trig));
    mh3DelPhiPhotLSWt[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLSWt_%i",trig));
    mh3DelPhiPhotUSWt[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUSWt_%i",trig));
    mh2InvMassPtLS[trig] = (TH2F*)f->Get(Form("mh2InvMassPtLS_%i",trig));
    mh2InvMassPtUS[trig] = (TH2F*)f->Get(Form("mh2InvMassPtUS_%i",trig));
   
    for(Int_t ptbin=0; ptbin<numPtBins; ptbin++)
      {
	// - Make projections into electron ptbins
	projHPhi[ptbin][trig]       = mh2PhiQPt[trig]->ProjectionX(Form("projHPhi_%i_%i",ptbin,trig),mh2PhiQPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2PhiQPt[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projnSigmaE[ptbin][trig]    = mh2nSigmaEPt[trig]->ProjectionX(Form("projnSigmaE_%i_%i",ptbin,trig),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projnSigmaE_eID[ptbin][trig]    = mh2nSigmaEPt_eID[trig]->ProjectionX(Form("projnSigmaE_eID_%i_%i",ptbin,trig),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projDelPhiIncl[ptbin][trig] = mh3DelPhiIncl[trig]->ProjectionX(Form("projDelPhiIncl_%i_%i",ptbin,trig),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiIncl[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiIncl[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotUS[ptbin][trig] = mh3DelPhiPhotUS[trig]->ProjectionX(Form("projDelPhiPhotUS_%i_%i",ptbin,trig),mh3DelPhiPhotUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUS[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotUS[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotUSNP[ptbin][trig] = mh3DelPhiPhotUSNP[trig]->ProjectionX(Form("projDelPhiPhotUSNP_%i_%i",ptbin,trig),mh3DelPhiPhotUSNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUSNP[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotUSNP[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLSNP[ptbin][trig] = mh3DelPhiPhotLSNP[trig]->ProjectionX(Form("projDelPhiPhotLSNP_%i_%i",ptbin,trig),mh3DelPhiPhotLSNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLSNP[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLSNP[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiInclNP[ptbin][trig] = mh3DelPhiInclNP[trig]->ProjectionX(Form("projDelPhiInclNP_%i_%i",ptbin,trig),mh3DelPhiInclNP[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiInclNP[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiInclNP[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiInclWt[ptbin][trig] = mh3DelPhiInclWt[trig]->ProjectionX(Form("projDelPhiInclWt_%i_%i",ptbin,trig),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiInclWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotUSWt[ptbin][trig] = mh3DelPhiPhotUSWt[trig]->ProjectionX(Form("projDelPhiPhotUSWt_%i_%i",ptbin,trig),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotUSWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLSWt[ptbin][trig] = mh3DelPhiPhotLSWt[trig]->ProjectionX(Form("projDelPhiPhotLSWt_%i_%i",ptbin,trig),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLSWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projInvMassUS[ptbin][trig] = mh2InvMassPtUS[trig]->ProjectionX(Form("projInvMassUS_%i_%i",ptbin,trig),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projInvMassLS[ptbin][trig] = mh2InvMassPtLS[trig]->ProjectionX(Form("projInvMassLS_%i_%i",ptbin,trig),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	  
      }
    
    for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){

      // Init necessary plotting tools
      lbl[ptbin] = new TPaveText(.2,.8,.5,.85,Form("NB NDC%i",ptbin));
      sprintf(textLabel,"%.1f < P_{T,e} < %.1f",lowpt[ptbin],highpt[ptbin]);
      lbl[ptbin]->AddText(textLabel);
      lbl[ptbin]->SetFillColor(kWhite);
      
      Int_t inclNorm = projnSigmaE_eID[ptbin][trig]->GetEntries();
      Int_t LSNorm   = projInvMassLS[ptbin][trig]->GetEntries();
      Int_t USNorm   = projInvMassUS[ptbin][trig]->GetEntries();

      Float_t Norm = (Float_t)inclNorm - (1/epsilon[ptbin])*((Float_t)USNorm - (Float_t)LSNorm); // Use the number of "signal" counts

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
      // Rebin all as necessary
      Int_t RB = 4;
      LSIM[ptbin][trig]  -> Rebin(RB);
      USIM[ptbin][trig]  -> Rebin(RB);
      USIMNP[ptbin][trig]-> Rebin(RB);
      LSIMNP[ptbin][trig]-> Rebin(RB);
      INCLNP[ptbin][trig]-> Rebin(RB);
      INCL[ptbin][trig]  -> Rebin(RB);
      INCL2[ptbin][trig] -> Rebin(RB);
      LSIM2[ptbin][trig] -> Rebin(RB);
      USIM2[ptbin][trig] -> Rebin(RB);
      
      // Actually manipulate histos and plot (photnic del Phi)
      
      USIMNP[ptbin][trig]->SetLineColor(kRed);
      USIMNP[ptbin][trig]->SetLineWidth(1);
      USIMNP[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USIMNP[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
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
      USwP->GetXaxis()->SetRangeUser(-2,5);
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
      LSwP->GetXaxis()->SetRangeUser(-2,5);
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
      InclwP->GetXaxis()->SetRangeUser(-2,5);
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

       // Handle Inclusive Hists (unweigthed hads projection)
      IN[trig]->cd(ptbin+1);
      INCL[ptbin][trig]->SetLineColor(kBlue);
      INCL[ptbin][trig]->SetLineWidth(1);
      INCL[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      INCL[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
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
      
      // Subtraction of Inclusive - (US-LS)
      result[trig]->cd(ptbin+1);
      TH1F *SUB2 = (TH1F*)INCLNP[ptbin][trig]->Clone(); // Inclusive
      SUB2->SetName("");
      TH1F *SUB3 = (TH1F*)SUB->Clone(); //(USNP - LSNP)
      //SUB2->Add(USIMNP[ptbin][trig],-1); // Inclusive - US w/Partner 
      SUB3->Sumw2(kFALSE); SUB3->Sumw2(kTRUE); // Lock errors before scaling
      SUB3->Scale((1./epsilon[ptbin])-1.); // Scale by (1/eps - 1)
      SUB2->Add(SUB3,-1); // Subtract the scaled (USNP - LSNP)
      SUB2->Add(LSIMNP[ptbin][trig]); // Add back LS w/Partner
      SUB2->Scale(1./((Double_t)Norm*SUB2->GetBinWidth(1))); // Normalize to triggers.
      SUB2->SetLineColor(kBlack);
      SUB2->SetLineWidth(1);
      SUB2->SetFillStyle(3001);
      SUB2->SetFillColor(kYellow);
      SUB2->GetXaxis()->SetRangeUser(-2,5);
      SUB2->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      SUB2->GetYaxis()->SetTitle("1/N_{NPE} #upoint dN/d(#Delta)#phi");
      SUB2->GetYaxis()->SetTitleOffset(1.55);
      if(ptbin == 0)
	SUB2->SetTitle("#Delta#phi Non-Photonic Electrons and Hadrons");
      else if (ptbin == 1 && trig !=3)
	SUB2->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	SUB2->SetTitle("MB");
      else
	SUB2->SetTitle("");
      SUB2->Draw("");
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
  TPad* pNew = (TPad*)InclComp[0]->GetPad(4)->Clone();
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
      for(Int_t ii=0; ii<numTrigs; ii++)
	{
	  if(!fPaintAll && (ii==1 || ii==3))
	    continue;
	  temp = IN[ii];
	  temp->Print(name);
	  temp = c[ii];
	  temp->Print(name);
	  temp = result[ii];
	  temp->Print(name);
	  /* temp = IN2[ii];
	  temp->Print(name);
	  temp = c2[ii];
	  temp->Print(name);
	  temp = result2[ii];
	  temp->Print(name);*/
	  temp = pile[ii];
	  temp->Print(name);
	  temp = inMass[ii];
	  temp->Print(name);
	  temp = USComp[ii];
	  temp->Print(name);
	  temp = LSComp[ii];
	  temp->Print(name);
	  temp = InclComp[ii];
	  temp->Print(name);
	  
	}
      sprintf(name, "%s.pdf]", FileName);
      temp->Print(name);
    }
}

