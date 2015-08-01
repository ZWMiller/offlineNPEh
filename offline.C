// Offline Plots - Z. Miller July 24, 2015
//
// .L offline.C
// offline("FILENAME") # Without .root Extension

void offline(const char* FileName="test")
{
  gROOT->SetBatch(kTRUE); // sets batch mode, so don't draw canvas
  
  // Set Style parameters for this macro
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
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
 // Reconstruction efficiency
  TH1D * LSIM[numPtBins][numTrigs];
  TH1D * USIM[numPtBins][numTrigs];
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
  TH3F* mh3DelPhiInclWt[numTrigs];
  TH3F* mh3DelPhiPhotLSWt[numTrigs];
  TH3F* mh3DelPhiPhotUSWt[numTrigs];
  TH2F* mh2InvMassPtLS[numTrigs];
  TH2F* mh2InvMassPtUS[numTrigs];
  TH3F* mh3nTracksZdcx[numTrigs];
  TH1D* projHPhi[numPtBins][numTrigs];
  TH1D* projnSigmaE[numPtBins][numTrigs];
  TH1D* projnSigmaE_eID[numPtBins][numTrigs];
  TH1D* projDelPhiIncl[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLS[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUS[numPtBins][numTrigs];
  TH1D* projDelPhiInclWt[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLSWt[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUSWt[numPtBins][numTrigs];
  TH1D* projInvMassLS[numPtBins][numTrigs];
  TH1D* projInvMassUS[numPtBins][numTrigs];
  TCanvas * c[numTrigs];
  TCanvas * c2[numTrigs];
  TCanvas * IN[numTrigs];
  TCanvas * IN2[numTrigs];
  TCanvas * pile[numTrigs];
  TCanvas * result[numTrigs];
  TCanvas * result2[numTrigs];
  TCanvas * inMass[numTrigs];
  
  
  for(Int_t trig = 0; trig < numTrigs; trig++){

    if(!fPaintAll && (trig == 1 || trig == 3)) continue; 
    // Create and Segment Canvas
    c[trig] = new TCanvas(Form("c%i",trig),"Photonic Hists (Unweighted)",100,0,1000,900);
    c2[trig] = new TCanvas(Form("cWt%i",trig),"Photonic Hists (Weighted)",100,0,1000,900);
    IN[trig]= new TCanvas(Form("IN%i",trig),"Inclusive Hists (Unweighted)",150,0,1050,900);
    IN2[trig]= new TCanvas(Form("INwt%i",trig),"Inclusive Hists (Weighted)",150,0,1050,900);
    pile[trig] = new TCanvas(Form("pile%i",trig),"Pileup Monitor",200,0,1100,900);
    inMass[trig] = new TCanvas(Form("inMass%i",trig),"Invariant Mass",200,0,1100,900);
    result[trig] = new TCanvas(Form("result%i",trig),"Inclusive - Photonic (Unweighted)",250,0,1150,900);
    result2[trig] = new TCanvas(Form("resultWt%i",trig),"Inclusive - Photonic (Weighted)",250,0,1150,900);
    c[trig] -> Divide(4,3);
    c2[trig]-> Divide(4,3);
    inMass[trig]->Divide(4,3);
    IN[trig]-> Divide(4,3);
    IN2[trig]->Divide(4,3);
    pile[trig]->Divide(2,2);
    result[trig]->Divide(4,3);
    result2[trig]->Divide(4,3);    

    // Make Projections (first get 2d/3d hists, then project)
    Float_t lowpt[14] ={2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.0};
    Float_t highpt[14]={3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.,200.};
    Float_t hptCut=0.2;
    mh2PhiQPt[trig]    = (TH2F*)f->Get(Form("mh2PhiQPt_%i",trig));
    mh2nSigmaEPt[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_%i",trig));
    mh2nSigmaEPt_eID[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_eID_%i",trig));
    mh3DelPhiIncl[trig] = (TH3F*)f->Get(Form("mh3DelPhiIncl_%i",trig));
    mh3DelPhiPhotLS[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotLS_%i",trig));
    mh3DelPhiPhotUS[trig] = (TH3F*)f->Get(Form("mh3DelPhiPhotUS_%i",trig));
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
	projDelPhiPhotLS[ptbin][trig] = mh3DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLS[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiInclWt[ptbin][trig] = mh3DelPhiInclWt[trig]->ProjectionX(Form("projDelPhiInclWt_%i_%i",ptbin,trig),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiInclWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiInclWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotUSWt[ptbin][trig] = mh3DelPhiPhotUSWt[trig]->ProjectionX(Form("projDelPhiPhotUSWt_%i_%i",ptbin,trig),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotUSWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotUSWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projDelPhiPhotLSWt[ptbin][trig] = mh3DelPhiPhotLSWt[trig]->ProjectionX(Form("projDelPhiPhotLSWt_%i_%i",ptbin,trig),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh3DelPhiPhotLSWt[trig]->GetYaxis()->FindBin(highpt[ptbin]),mh3DelPhiPhotLSWt[trig]->GetZaxis()->FindBin(hptCut),-1);
	projInvMassUS[ptbin][trig] = mh2InvMassPtUS[trig]->ProjectionX(Form("projInvMassUS_%i_%i",ptbin,trig),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projInvMassLS[ptbin][trig] = mh2InvMassPtLS[trig]->ProjectionX(Form("projInvMassLS_%i_%i",ptbin,trig),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	  
      }
    
    for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){

      Int_t counter = numPtBins*trig+ptbin;
      // DEBUG cout << counter << endl;
      c[trig]->cd(ptbin+1);
      
      LSIM[ptbin][trig]  = projDelPhiPhotLS[ptbin][trig];
      USIM[ptbin][trig]  = projDelPhiPhotUS[ptbin][trig];
      INCL[ptbin][trig]  = projDelPhiIncl[ptbin][trig];
      INCL2[ptbin][trig] = projDelPhiInclWt[ptbin][trig];
      LSIM2[ptbin][trig] = projDelPhiPhotLSWt[ptbin][trig];
      USIM2[ptbin][trig] = projDelPhiPhotUSWt[ptbin][trig];
      LSMM[ptbin][trig]  = projInvMassLS[ptbin][trig];
      USMM[ptbin][trig]  = projInvMassUS[ptbin][trig];
      
      // Actually manipulate histos and plot (photnic del Phi)
      
      USIM[ptbin][trig]->SetLineColor(kRed); //without hadron weighting
      USIM[ptbin][trig]->SetLineWidth(1);
      USIM[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USIM[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
      if(ptbin == 0)
	USIM[ptbin][trig]->SetTitle("Photonic Electron Reconstruction (Unweighted)");
      else if (ptbin == 1 && trig !=3)
	USIM[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USIM[ptbin][trig]->SetTitle("MB");
      else
	USIM[ptbin][trig]->SetTitle("");
      USIM[ptbin][trig]->Draw("hist");
      
      LSIM[ptbin][trig]->SetLineColor(kBlack);
      LSIM[ptbin][trig]->SetLineWidth(1);
      LSIM[ptbin][trig]->Draw("hist same");

      // Subtraction of (US-LS)
      TH1F *SUB = (TH1F*)USIM[ptbin][trig]->Clone(); //
      SUB->SetName("Subtraction");      // Create SUB as a clone of USIM
      SUB->Add(LSIM[ptbin][trig],-1);
      SUB->SetLineColor(kBlue);
      SUB->SetLineWidth(1);
      SUB->SetFillStyle(3001);
      SUB->SetFillColor(kBlue);
      SUB->Draw("hist same");
      TLegend* leg = new TLegend(0.7,0.7,0.85,0.85);
      leg->AddEntry(USIM[ptbin][trig],"Unlike Sign","l");
      leg->AddEntry(LSIM[ptbin][trig],"Like Sign", "l");
      leg->AddEntry(SUB,"Unlike - Like", "f");
      leg->Draw();

      c2[trig]->cd(ptbin+1); // With hadron weighting
      USIM2[ptbin][trig]->SetLineColor(kRed);
      USIM2[ptbin][trig]->SetLineWidth(1);
      USIM2[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USIM2[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
      if(ptbin == 0)
	USIM2[ptbin][trig]->SetTitle("Photonic Electron Reconstruction (Weighted)");
      else if (ptbin == 1 && trig !=3)
	USIM[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USIM2[ptbin][trig]->SetTitle("MB");
      else
	USIM2[ptbin][trig]->SetTitle("");
      USIM2[ptbin][trig]->Draw("hist");
      
      LSIM2[ptbin][trig]->SetLineColor(kBlack);
      LSIM2[ptbin][trig]->SetLineWidth(1);
      LSIM2[ptbin][trig]->Draw("hist same");

      // Subtraction of (US-LS)
      TH1F *SUB5 = (TH1F*)USIM2[ptbin][trig]->Clone(); //
      SUB5->SetName("Subtraction");      // Create SUB as a clone of USIM
      SUB5->Add(LSIM[ptbin][trig],-1);
      SUB5->SetLineColor(kBlue);
      SUB5->SetLineWidth(1);
      SUB5->SetFillStyle(3001);
      SUB5->SetFillColor(kBlue);
      SUB5->Draw("hist same");
      TLegend* leg3 = new TLegend(0.7,0.7,0.85,0.85);
      leg3->AddEntry(USIM2[ptbin][trig],"Unlike Sign","l");
      leg3->AddEntry(LSIM2[ptbin][trig],"Like Sign", "l");
      leg3->AddEntry(SUB5,"Unlike - Like", "f");
      leg3->Draw();

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
      USMM[ptbin][trig]->Draw("hist");
      
      LSMM[ptbin][trig]->SetLineColor(kBlack);
      LSMM[ptbin][trig]->SetLineWidth(1);
      LSMM[ptbin][trig]->Draw("hist same");

      // Subtraction of (US-LS)
      TH1F *SUB4 = (TH1F*)USMM[ptbin][trig]->Clone(); //
      SUB4->SetName("Subtraction");      // Create SUB as a clone of USIM
      SUB4->Add(LSMM[ptbin][trig],-1);
      SUB4->SetLineColor(kBlue);
      SUB4->SetLineWidth(1);
      SUB4->SetFillStyle(3001);
      SUB4->SetFillColor(kBlue);
      SUB4->Draw("hist same");
      TLegend* leg2 = new TLegend(0.7,0.7,0.85,0.85);
      leg2->AddEntry(USMM[ptbin][trig],"Unlike Sign","l");
      leg2->AddEntry(LSMM[ptbin][trig],"Like Sign", "l");
      leg2->AddEntry(SUB4,"Unlike - Like", "f");
      leg2->Draw();

       // Handle Inclusive Hists (unweigthed hads projection)
      IN[trig]->cd(ptbin+1);
      INCL[ptbin][trig]->SetLineColor(kBlue);
      INCL[ptbin][trig]->SetLineWidth(1);
      INCL[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      INCL[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
      if(ptbin == 0)
	INCL[ptbin][trig]->SetTitle("Inclusive Electrons (Unweighted Hadron)");
      else if (ptbin == 1 && trig !=3)
	INCL[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	INCL[ptbin][trig]->SetTitle("MB");
      else
	INCL[ptbin][trig]->SetTitle("");
      INCL[ptbin][trig]->Draw("hist");

      IN[trig]->Update();

        // Handle Inclusive Hists (weighted hads projection)
      IN2[trig]->cd(ptbin+1);
      INCL2[ptbin][trig]->SetLineColor(kBlue);
      INCL2[ptbin][trig]->SetLineWidth(1);
      INCL2[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      INCL2[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
      if(ptbin == 0)
	INCL2[ptbin][trig]->SetTitle("Inclusive Electrons (Weighted Hadron)");
      else if (ptbin == 1 && trig !=3)
	INCL2[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	INCL2[ptbin][trig]->SetTitle("MB");
      else
	INCL2[ptbin][trig]->SetTitle("");
      INCL2[ptbin][trig]->Draw("hist");

      IN2[trig]->Update();
      
      // Subtraction of Inclusive - (US-LS) (unweighted)
      result[trig]->cd(ptbin+1);
      TH1F *SUB2 = (TH1F*)INCL[ptbin][trig]->Clone(); //
      TH1F *SUB3 = (TH1F*)SUB->Clone();
      SUB2->SetName("");      // Create SUB as a clone of USIM
      SUB3->Sumw2(kFALSE); SUB3->Sumw2(kTRUE); // Lock errors before scaling
      SUB3->Scale(1./epsilon[ptbin]); // Scale by reconstruction efficiency
      SUB2->Add(SUB3,-1);
      SUB2->SetLineColor(kBlack);
      SUB2->SetLineWidth(1);
      SUB2->SetFillStyle(3001);
      SUB2->SetFillColor(kYellow);
      SUB2->GetXaxis()->SetRangeUser(-2,5);
      SUB2->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      if(ptbin == 0)
	SUB2->SetTitle("Inclusive - Photonic/#epsilon (unweighted)");
      else if (ptbin == 1 && trig !=3)
	SUB2->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	SUB2->SetTitle("MB");
      else
	SUB2->SetTitle("");
      SUB2->Draw("hist");

        // Subtraction of Inclusive - (US-LS) (weighted)
      result2[trig]->cd(ptbin+1);
      TH1F *SUB6 = (TH1F*)INCL2[ptbin][trig]->Clone(); //
      TH1F *SUB7 = (TH1F*)SUB5->Clone();
      SUB6->SetName("");      // Create SUB as a clone of USIM
      SUB7->Sumw2(kFALSE); SUB7->Sumw2(kTRUE); // Lock errors before scaling
      SUB7->Scale(1./epsilon[ptbin]); // Scale by reconstruction efficiency
      SUB6->Add(SUB7,-1);
      SUB6->SetLineColor(kBlack);
      SUB6->SetLineWidth(1);
      SUB6->SetFillStyle(3001);
      SUB6->SetFillColor(kYellow);
      SUB6->GetXaxis()->SetRangeUser(-2,5);
      SUB6->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      if(ptbin == 0)
	SUB6->SetTitle("Inclusive - Photonic/#epsilon (Weighted Hadron)");
      else if (ptbin == 1 && trig !=3)
	SUB6->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	SUB6->SetTitle("MB");
      else
	SUB6->SetTitle("");
      SUB6->Draw("hist");
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
	projZDCx[ptbin][trig] = profileZDCx[trig]->ProjectionX(Form("projHPhi_%i_%i",ptbin,trig),profileZDCx[trig]->GetYaxis()->FindBin(lowhpt[ptbin]),profileZDCx[trig]->GetYaxis()->FindBin(highhpt[ptbin]));

	// plot projections
	pile[trig]->cd(ptbin+1);
	projZDCx[ptbin][trig]->SetLineColor(kBlack);
	projZDCx[ptbin][trig]->GetXaxis()->SetTitle("ZDCx");
	if(ptbin == 0)
	  projZDCx[ptbin][trig]->SetTitle("Pileup by hadPT bins");
	projZDCx[ptbin][trig]->Draw("");
      }
  }

  // Make PDF with output canvases
  //Set front page
  TCanvas* fp = new TCanvas("fp","Front Page",100,0,1000,900);
  fp->cd();
  TBox *bLabel = new TBox(0.01, 0.88, 0.99, 0.99);
  bLabel->SetFillColor(kBlack);
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
  bFoot->SetFillColor(kBlack);
  bFoot->Draw();
  tl.SetTextColor(kWhite);
  tl.SetTextSize(0.05);
  tl.DrawLatex(0.05, 0.05, (new TDatime())->AsString());
  tl.SetTextColor(kBlack);
  tl.SetTextSize(0.03);
  tl.DrawLatex(0.1, 0.14, titlename);
  sprintf(tlName,"eID: -1 < n  #sigma_{e TPC} < 3;  #left|gDCA #right| < 1 cm; 0.3 < p/E < 1.5;");
  tl.DrawLatex(0.1, 0.8,tlName);
  sprintf(tlName,"       nHitsFit > 20; nHits   #frac{dE}{dx} > 15; nHitFit/Max > 0.52;  #left|#eta#right| < 0.7");
  tl.DrawLatex(0.1, 0.75,tlName);
  sprintf(tlName,"       n #phi > 1; n #eta > 1;  #left|dZ#right| < 3 cm;  #left|d #phi#right| < 0.015;");
  tl.DrawLatex(0.1, 0.7,tlName);
  sprintf(tlName,"hID: pT > 0.2;  #left|#eta#right| < 1; nHitsFit > 15; nHits   #frac{dE}{dx} > 10; DCA < 0.1 cm;");
  tl.DrawLatex(0.1, 0.6,tlName);
  sprintf(tlName,"       !( -1 < n  #sigma_{e TPC} < 3);");
  tl.DrawLatex(0.1, 0.55,tlName);
  sprintf(tlName,"Event:  #left|V_{z}#right| < 35 cm;  #left|V_{z}-V_{z-VPD}#right| < 6 cm;");
  tl.DrawLatex(0.1, 0.45,tlName);
        
  
  // Place canvases in order
  TCanvas* temp = new TCanvas();
  sprintf(name, "%s.pdf[", FileName);
  temp->Print(name);
  sprintf(name, "%s.pdf", FileName);
  temp = fp; // print front page
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
      temp = IN2[ii];
      temp->Print(name);
      temp = c2[ii];
      temp->Print(name);
      temp = result2[ii];
      temp->Print(name);
      temp = pile[ii];
      temp->Print(name);
      temp = inMass[ii];
      temp->Print(name);
      
    }
  sprintf(name, "%s.pdf]", FileName);
  temp->Print(name);

}

