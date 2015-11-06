{
  double pa[3];
  TFile* f = new TFile("Purity.root");
  TH1F* p = (TH1F*) f->Get("purity");
  TH1F* pC = (TH1F*) p->Clone();

  // move points in purity to highest error bar point
  TCanvas * test = new TCanvas("test");
  TH1F* changeHist = p;
  test->cd();
  changeHist->DrawClone("AP");
  TAxis* ax = changeHist->GetXaxis();
  for(int chn = ax->GetFirst(); chn <= ax->GetLast(); chn++)
  {
    double current = changeHist->GetBinContent(chn);
    double error = changeHist->GetBinError(chn);
    //changeHist->SetBinContent(chn, current+error);
  }
  changeHist->SetLineColor(kGreen);
  changeHist->SetMarkerColor(kGreen);
  changeHist->DrawClone("same");

  p->Fit("pol2","Q");
  TF1* fit = p->GetFunction("pol2");
  pa[0] = fit->GetParameter(0);
  pa[1] = fit->GetParameter(1);
  pa[2] = fit->GetParameter(2);
  pC->Draw("same");
  FILE* pFile;
  pFile = fopen("purParFits.txt","w");
  fprintf(pFile,"%lf %lf %lf",pa[0],pa[1],pa[2]);
  fclose(pFile);


  
}
