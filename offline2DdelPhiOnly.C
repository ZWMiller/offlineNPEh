// Offline Plots - Z. Miller July 24, 2015
//
// .L offline.C
// offline("FILENAME") # Without .root Extension

void offline(const char* FileName="test")
{
  gROOT->SetBatch(kFALSE); // sets batch mode, so don't draw canvas
  
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
  TH1D * LSMM[numPtBins][numTrigs];
  TH1D * USMM[numPtBins][numTrigs];
  TH2F * MON[numTrigs];
  TH1F * AVGTRKS[numTrigs];
  TH2F * MON2[numTrigs];
  TH1F * AVGTRKS2[numTrigs];
  TH2F* mh2PhiQPt[numTrigs];
  TH2F* mh2nSigmaEPt[numTrigs];
  TH2F* mh2nSigmaEPt_eID[numTrigs];
  TH2F* mh2DelPhiIncl[numTrigs];
  TH2F* mh2DelPhiPhotLS[numTrigs];
  TH2F* mh2DelPhiPhotUS[numTrigs];
  TH2F* mh2InvMassPtLS[numTrigs];
  TH2F* mh2InvMassPtUS[numTrigs];
  TH3F* mh3nTracksZdcx[numTrigs];
  TH1D* projHPhi[numPtBins][numTrigs];
  TH1D* projnSigmaE[numPtBins][numTrigs];
  TH1D* projnSigmaE_eID[numPtBins][numTrigs];
  TH1D* projDelPhiIncl[numPtBins][numTrigs];
  TH1D* projDelPhiPhotLS[numPtBins][numTrigs];
  TH1D* projDelPhiPhotUS[numPtBins][numTrigs];
  TH1D* projInvMassLS[numPtBins][numTrigs];
  TH1D* projInvMassUS[numPtBins][numTrigs];
  TCanvas * c[numTrigs];
  TCanvas * IN[numTrigs];
  TCanvas * pile[numTrigs];
  TCanvas * result[numTrigs];
  TCanvas * inMass[numTrigs];
  
  
  for(Int_t trig = 0; trig < numTrigs; trig++){

    if(!fPaintAll && (trig == 1 || trig == 3)) continue; 
    // Create and Segment Canvas
    c[trig] = new TCanvas(Form("c%i",trig),"Photonic Hists",100,0,1000,900);
    IN[trig]= new TCanvas(Form("IN%i",trig),"Inclusive Hists",150,0,1050,900);
    pile[trig] = new TCanvas(Form("pile%i",trig),"Pileup Monitor",200,0,1100,900);
    inMass[trig] = new TCanvas(Form("inMass%i",trig),"Invariant Mass",200,0,1100,900);
    result[trig] = new TCanvas(Form("result%i",trig),"Inclusive - Photonic",250,0,1150,900);
    c[trig] -> Divide(4,3);
    inMass[trig]->Divide(4,3);
    IN[trig]-> Divide(4,3);
    pile[trig]->Divide(2,2);
    result[trig]->Divide(4,3);
    /*  c[trig] -> Update();
    IN[trig]-> Update();
    pile[trig]->Update();
    result[trig]->Update();*/

    /* // Grab TRIG only histos
    MON[trig] = (TH2F*)f->Get(Form("mh2nTracksZdcx_%i",trig)); //Pileup Monitor
    MON2[trig] = (TH2F*)f->Get(Form("mh2nETracksZdcx_%i",trig)); //Pileup Monitor w eID

    // Make extra histos
    AVGTRKS[trig] = new TH1F(Form("AVGTRKS_%i",trig),"",MON[trig]->GetXaxis()->GetNbins()-2,MON[trig]->GetXaxis()->GetXmin(),MON[trig]->GetXaxis()->GetXmax());
    AVGTRKS2[trig] = new TH1F(Form("AVGTRKS2_%i",trig),"",MON[trig]->GetXaxis()->GetNbins()-2,MON[trig]->GetXaxis()->GetXmin(),MON[trig]->GetXaxis()->GetXmax());

    // Plot TRIG only histos
    pile[trig]->cd(1);
    if(trig<3)
      MON2[trig]->SetTitle(Form("HT%i",trig));
    else
      MON2[trig]->SetTitle("MB");
    MON2[trig]->Draw("colz");
    //pile[trig]->cd(3);
    //MON2[trig]->SetTitle("w/eID");
    //MON2[trig]->Draw("colz");

    // Calculate Average Tracks vs ZDCx (loop over all x and y bins, doing weighted average of track numbers)
    for(Int_t ii=0;ii<MON[trig]->GetXaxis()->GetLast();ii++)
      {
	Float_t temp = 0, temp3 = 0;
	Float_t temp2 = 0, temp4 = 0;

	for(Int_t jj=0;jj<MON[trig]->GetYaxis()->GetLast();jj++)
	  {
	    temp  += MON[trig]->GetBinContent(ii,jj) * MON[trig]->GetYaxis()->GetBinUpEdge(jj);
	    temp2 += MON[trig]->GetBinContent(ii,jj);
	    temp3 += MON2[trig]->GetBinContent(ii,jj) * MON2[trig]->GetYaxis()->GetBinUpEdge(jj);
	    temp4 += MON2[trig]->GetBinContent(ii,jj);
	  }
	if(temp2 < 1) temp2 = 1; // Stop divide by 0
	if(temp4 < 1) temp4 = 1; // Stop divide by 0	
	AVGTRKS[trig]->SetBinContent(ii,temp/temp2);
	AVGTRKS2[trig]->SetBinContent(ii,temp3/temp4);
      }
    pile[trig]->cd(2);
    AVGTRKS2[trig]->SetLineColor(kBlack);
    AVGTRKS2[trig]->Draw("");
    //pile[trig]->cd(4);
    //AVGTRKS2[trig]->SetLineColor(kBlack);
    //AVGTRKS2[trig]->Draw("hist");*/

    // Make Projections (first get 2d hists, then project)
    Float_t lowpt[14] ={2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.0};
    Float_t highpt[14]={3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.,200.};
    mh2PhiQPt[trig]    = (TH2F*)f->Get(Form("mh2PhiQPt_%i",trig));
    mh2nSigmaEPt[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_%i",trig));
    mh2nSigmaEPt_eID[trig] = (TH2F*)f->Get(Form("mh2nSigmaEPt_eID_%i",trig));
    mh2DelPhiIncl[trig] = (TH2F*)f->Get(Form("mh2DelPhiIncl_%i",trig));
    mh2DelPhiPhotLS[trig] = (TH2F*)f->Get(Form("mh2DelPhiPhotLS_%i",trig));
    mh2DelPhiPhotUS[trig] = (TH2F*)f->Get(Form("mh2DelPhiPhotUS_%i",trig));
    mh2InvMassPtLS[trig] = (TH2F*)f->Get(Form("mh2InvMassPtLS_%i",trig));
    mh2InvMassPtUS[trig] = (TH2F*)f->Get(Form("mh2InvMassPtUS_%i",trig));
   
    for(Int_t ptbin=0; ptbin<numPtBins; ptbin++)
      {
	// - Make projections into electron ptbins
	projHPhi[ptbin][trig]       = mh2PhiQPt[trig]->ProjectionX(Form("projHPhi_%i_%i",ptbin,trig),mh2PhiQPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2PhiQPt[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projnSigmaE[ptbin][trig]    = mh2nSigmaEPt[trig]->ProjectionX(Form("projnSigmaE_%i_%i",ptbin,trig),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projnSigmaE_eID[ptbin][trig]    = mh2nSigmaEPt_eID[trig]->ProjectionX(Form("projnSigmaE_eID_%i_%i",ptbin,trig),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2nSigmaEPt_eID[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projDelPhiIncl[ptbin][trig] = mh2DelPhiIncl[trig]->ProjectionX(Form("projDelPhiIncl_%i_%i",ptbin,trig),mh2DelPhiIncl[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2DelPhiIncl[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projDelPhiPhotLS[ptbin][trig] = mh2DelPhiPhotLS[trig]->ProjectionX(Form("projDelPhiPhotLS_%i_%i",ptbin,trig),mh2DelPhiPhotLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2DelPhiPhotLS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projDelPhiPhotUS[ptbin][trig] = mh2DelPhiPhotUS[trig]->ProjectionX(Form("projDelPhiPhotUS_%i_%i",ptbin,trig),mh2DelPhiPhotUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2DelPhiPhotUS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projInvMassUS[ptbin][trig] = mh2InvMassPtUS[trig]->ProjectionX(Form("projInvMassUS_%i_%i",ptbin,trig),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtUS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	projInvMassLS[ptbin][trig] = mh2InvMassPtLS[trig]->ProjectionX(Form("projInvMassLS_%i_%i",ptbin,trig),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(lowpt[ptbin]),mh2InvMassPtLS[trig]->GetYaxis()->FindBin(highpt[ptbin]));
	  
      }
    
    for(Int_t ptbin = 0; ptbin < numPtBins; ptbin++){

      Int_t counter = numPtBins*trig+ptbin;
      // DEBUG cout << counter << endl;
Float_t etapCount,pipCount,gammapCount;      c[trig]->cd(ptbin+1);
      
      LSIM[ptbin][trig] = projDelPhiPhotLS[ptbin][trig];
      USIM[ptbin][trig] = projDelPhiPhotUS[ptbin][trig];
      INCL[ptbin][trig] = projDelPhiIncl[ptbin][trig];
      LSMM[ptbin][trig] = projInvMassLS[ptbin][trig];
      USMM[ptbin][trig] = projInvMassUS[ptbin][trig];
      
      // Actually manipulate histos and plot (photnic del Phi)
      
      USIM[ptbin][trig]->SetLineColor(kRed);
      USIM[ptbin][trig]->SetLineWidth(1);
      USIM[ptbin][trig]->Rebin(4);
      USIM[ptbin][trig]->GetXaxis()->SetTitle("#Delta#phi_{eh}");
      USIM[ptbin][trig]->GetXaxis()->SetRangeUser(-2,5);
      if(ptbin == 0)
	USIM[ptbin][trig]->SetTitle("Photonic Electron Reconstruction");
      else if (ptbin == 1 && trig !=3)
	USIM[ptbin][trig]->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	USIM[ptbin][trig]->SetTitle("MB");
      else
	USIM[ptbin][trig]->SetTitle("");
      USIM[ptbin][trig]->Draw("hist");
      
      LSIM[ptbin][trig]->SetLineColor(kBlack);
      LSIM[ptbin][trig]->SetLineWidth(1);
      LSIM[ptbin][trig]->Rebin(4);
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

       // Handle Inclusive Hists
      IN[trig]->cd(ptbin+1);
      INCL[ptbin][trig]->SetLineColor(kBlue);
      INCL[ptbin][trig]->SetLineWidth(1);
      INCL[ptbin][trig]->Rebin(4);
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
      INCL[ptbin][trig]->Draw("hist");

      IN[trig]->Update();
      
      // Subtraction of Inclusive - (US-LS)
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
	SUB2->SetTitle("Inclusive - Photonic/#epsilon");
      else if (ptbin == 1 && trig !=3)
	SUB2->SetTitle(Form("HT%i",trig));
      else if (trig == 3 && ptbin == 1)
	SUB2->SetTitle("MB");
      else
	SUB2->SetTitle("");
      SUB2->Draw("hist");
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
  TCanvas* temp = new TCanvas();
  sprintf(name, "%s.pdf[", FileName);
  temp->Print(name);
    sprintf(name, "%s.pdf", FileName);
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
      temp = pile[ii];
      temp->Print(name);
      temp = inMass[ii];
      temp->Print(name);
      
    }
  sprintf(name, "%s.pdf]", FileName);
  temp->Print(name);

}

