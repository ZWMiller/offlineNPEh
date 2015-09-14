// Read Xiaozhi Trigger Eff and Make Fit
//
// 9/9/15 - Z Miller

{

  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)
  gStyle->SetOptFit(111);
  // Open ROOT Files
  char name[1000];
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/xiaozhiPhotonicEff/TrigEfficiency.root");
  TFile *f1 = new TFile(name,"READ");
  if (f1->IsOpen()==kFALSE)
    { std::cout << "!!! f1 File Not Found !!!" << std::endl;
      exit(1); }
  
  TH1D* HT0 = (TH1D*)f1->Get("hh_11_0");
  TH1D* HT2 = (TH1D*)f1->Get("hh_18_0");
  TH1D* HT0p = (TH1D*)f1->Get("hh_11_0");
  TH1D* HT2p = (TH1D*)f1->Get("hh_18_0");
  TCanvas * c1 = new TCanvas("c1","",100,0,1000,900);
  TCanvas * c2 = new TCanvas("c2","",100,0,1000,900);
  TCanvas * c3 = new TCanvas("c3","",100,0,1000,900);
  
  c3->cd();
  HT0p->SetMarkerStyle(23);
  HT0p->SetMarkerSize(1.2);
  HT0p->SetMarkerColor(kRed);
  HT2p->SetLineColor(kBlack);
  HT0p->SetTitle("Trigger Efficiency");
  HT0p->Draw();
  HT2p->Draw("same");
  TLegend* leg2 = new TLegend(0.15,0.68,0.4,0.85);
  leg2->AddEntry(HT0p,"High Tower 0","pe");
  leg2->AddEntry(HT2p,"High Tower 2","pe");
  leg2->Draw("same");
  
  c1->cd();
  TF1 *fit=new TF1("fit","TMath::Erf(x-[0])",0,20);
  HT0->Fit("fit");
  HT0->GetFunction("fit")->SetLineColor(kRed);
  TPaveStats *st = ((TPaveStats*)(HT0->GetListOfFunctions()->FindObject("stats")));
  if (st) {
    st->SetTextColor(HT0->GetFunction("fit")->GetLineColor());
    st->SetX1NDC(0.64); st->SetX2NDC(0.99);
    st->SetY1NDC(0.4); st->SetY2NDC(0.6);
  }
  c1->Update();

  c2->cd();
  TF1 *fit2=new TF1("fit2","TMath::Erf([1]*x-[0])",0,20);
  HT2->Fit("fit2","R+","sames",4.5,20);
  HT2->GetFunction("fit2")->SetLineColor(kRed);
  HT2->Fit("expo","R","sames",2.5,4.5);
  HT2->GetFunction("expo")->SetLineColor(kBlue);
  
  TPaveStats *st2 = ((TPaveStats*)(HT2->GetListOfFunctions()->FindObject("stats")));
  if (st) {
    st2->SetTextColor(HT2->GetFunction("fit2")->GetLineColor());
    //st2->SetTextColor(HT2->GetFunction("expo")->GetLineColor());
    st2->SetX1NDC(0.64); st->SetX2NDC(0.99);
    st2->SetY1NDC(0.4); st->SetY2NDC(0.6);
    HT2->Draw("");
  }
  c2->Update();
  cout << endl << endl;
  cout << "HT0: TMath::Erf(x-2.41589)" << endl;
  cout << "HT2 pt < 4.5 GeV: exp(-16.0543+3.36662*x)" << endl;
  cout << "HT2 pt > 4.5 GeV: TMath::Erf(.651713*x-2.6212)" << endl;

}
