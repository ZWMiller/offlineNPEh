// Read in the files from Xiaozhi's data embedding and analysis. Calculate the reconstruction eff for each decay channel, then combine by the contribution ratio

{
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
  // Open ROOT Files
  char name[1000];
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/xiaozhiPhotonicEff/Eta_HTRC.root");
  TFile *f1 = new TFile(name,"READ");
  if (f1->IsOpen()==kFALSE)
    { std::cout << "!!! f1 File Not Found !!!" << std::endl;
      exit(1); }

   sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/xiaozhiPhotonicEff/Pi0_HTRC.root");
  TFile *f2 = new TFile(name,"READ");
  if (f2->IsOpen()==kFALSE)
    { std::cout << "!!! f2 File Not Found !!!" << std::endl;
      exit(1); }

   sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/xiaozhiPhotonicEff/Gamma_HTRC.root");
  TFile *f3 = new TFile(name,"READ");
  if (f3->IsOpen()==kFALSE)
    { std::cout << "!!! f3 File Not Found !!!" << std::endl;
      exit(1); }

  // Grab histograms to integrate
  TH1F* etaSingles   = (TH1F*)f1->Get("mSingle_Track");
  TH1F* etaPairs     = (TH1F*)f1->Get("mPair");
  TH1F* piSingles    = (TH1F*)f2->Get("mSingle_Track");
  TH1F* piPairs      = (TH1F*)f2->Get("mPair");
  TH1F* gammaSingles = (TH1F*)f3->Get("mSingle_Track");
  TH1F* gammaPairs   = (TH1F*)f3->Get("mPair");

  // Assign pT Bin Limits
  Float_t lowpt[14] ={2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.0};
  Float_t highpt[14]={3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.5,10.,14.,200.};
  Float_t etaEff[14],piEff[14],gammaEff[14];
  Float_t x[14];
  Float_t y[14];
  Float_t dx[14];
  Float_t dy[14],dyg[14],dye[14],dyp[14];
  Float_t wt[3]={.1,.36,.54}; //contributions of each decay
  Int_t plotRange=0;
  Float_t etapCount,pipCount,gammapCount;
  Float_t etaCount,piCount,gammaCount;
  for(Int_t i=0;i<14;i++)
    {
      etapCount = etaPairs->Integral(etaPairs->GetXaxis()->FindBin(lowpt[i]),etaPairs->GetXaxis()->FindBin(highpt[i]));
      etaCount = etaSingles->Integral(etaPairs->GetXaxis()->FindBin(lowpt[i]),etaPairs->GetXaxis()->FindBin(highpt[i]));
      etaEff[i] = etapCount/etaCount;

      pipCount = piPairs->Integral(piPairs->GetXaxis()->FindBin(lowpt[i]),piPairs->GetXaxis()->FindBin(highpt[i]));
      piCount = piSingles->Integral(piPairs->GetXaxis()->FindBin(lowpt[i]),piPairs->GetXaxis()->FindBin(highpt[i]));
      piEff[i] = pipCount/piCount;

      gammapCount = gammaPairs->Integral(gammaPairs->GetXaxis()->FindBin(lowpt[i]),gammaPairs->GetXaxis()->FindBin(highpt[i]));
      gammaCount = gammaSingles->Integral(gammaPairs->GetXaxis()->FindBin(lowpt[i]),gammaPairs->GetXaxis()->FindBin(highpt[i]));
      gammaEff[i] = gammapCount/gammaCount;
																	       
      x[i]  = (highpt[i]+lowpt[i])/2.;
      dx[i] = (highpt[i]-lowpt[i])/2.;
      dy[i] = 0.; 
      y[i]  = wt[0]*etaEff[i]+wt[1]*piEff[i]+wt[2]*gammaEff[i];
      cout << x[i] << " " << y[i] << endl;
      plotRange++;
    }

  TGraphErrors* etaPlot = new TGraphErrors(plotRange-1,x,etaEff,dx,dy);
  TGraphErrors* piPlot = new TGraphErrors(plotRange-1,x,piEff,dx,dy);
  TGraphErrors* gammaPlot = new TGraphErrors(plotRange-1,x,gammaEff,dx,dy);
  TGraphErrors* effPlot = new TGraphErrors(plotRange-1,x,y,dx,dy);

 

  TCanvas * c1 = new TCanvas("c1","",100,0,1000,900);
  c1->Divide(2,2);
  c1->cd(1);
  etaPlot->SetTitle("#eta Dalitz");
  etaPlot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  etaPlot->GetYaxis()->SetTitle("Recon. Effic.");
  etaPlot->GetYaxis()->SetRangeUser(0.,1.);
  etaPlot->GetXaxis()->SetRangeUser(2.5,14.);
  etaPlot->SetLineColor(kBlue);
  etaPlot->SetMarkerStyle(7);
  etaPlot->Draw("ap");

  c1->cd(2);
  piPlot->SetTitle("#pi^{0} Dalitz");
  piPlot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  piPlot->GetYaxis()->SetTitle("Recon. Effic.");
  piPlot->GetYaxis()->SetRangeUser(0.,1.);
  piPlot->GetXaxis()->SetRangeUser(2.5,14.);
  piPlot->SetLineColor(kBlue);
  piPlot->SetMarkerStyle(7);
  piPlot->Draw("ap");

  c1->cd(3);
  gammaPlot->SetTitle("Photon (#gamma) Conversion");
  gammaPlot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  gammaPlot->GetYaxis()->SetTitle("Recon. Effic.");
  gammaPlot->GetYaxis()->SetRangeUser(0.,1.);
  gammaPlot->GetXaxis()->SetRangeUser(2.5,14.);
  gammaPlot->SetLineColor(kBlue);
  gammaPlot->SetMarkerStyle(7);
  gammaPlot->Draw("ap");

  c1->cd(4);
  effPlot->SetTitle("Photonic Electron Reco. Efficiency");
  effPlot->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  effPlot->GetYaxis()->SetTitle("Recon. Effic.");
  effPlot->GetYaxis()->SetRangeUser(0.,1.);
  effPlot->GetXaxis()->SetRangeUser(2.5,14.);
  effPlot->SetLineColor(kBlue);
  effPlot->SetMarkerStyle(7);
  effPlot->Draw("ap");

   // Do fits
  char fitName[100];
  sprintf(fitName,"myF");
  TF1 * myF = new TF1("myF","[0]*TMath::Log(x*[1])");
  myF->SetParName(0,"c0");
  myF->SetParName(1,"c1");
  myF->SetParameter(0,1);
  myF->SetParameter(1,1);
  gStyle->SetOptFit(1111);
  effPlot->Fit(fitName,"","",3,13);
  effPlot->GetFunction(fitName)->SetLineColor(kRed);
  TPaveStats *st = ((TPaveStats*)(effPlot->GetListOfFunctions()->FindObject("stats")));
  if (st) {
    st->SetTextColor(effPlot->GetFunction(fitName)->GetLineColor());
    st->SetX1NDC(0.64); st->SetX2NDC(0.99);
    st->SetY1NDC(0.4); st->SetY2NDC(0.6);
  }
  c1->Update();
  
}
