{
  char name[100];
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/npe_tree_Sep1_1.root");
  TFile *fZ = new TFile(name,"READ");
  sprintf(name,"/Users/zach/Research/rootFiles/run12NPEhPhi/XiaozhiJune4.root");
  TFile *fX = new TFile(name,"READ");
  if (fZ->IsOpen()==kFALSE || fX->IsOpen()==kFALSE)
    cout << "not correct" << endl;
  
  TH1F* zach;
  TH1F* xiao;
  TH1F* divi;
  TH1F* line = new TH1F("line","",1,0,100);
  line->SetBinContent(1,1);
  TCanvas* c = new TCanvas("c","",150,0,1150,1000);
  c->Divide(1,2);
  c->cd(1);
  zach = (TH1F*)fZ->Get("mh1PtETracks_2");
  xiao = (TH1F*)fX->Get("mh1electronPtTrg2");
  xiao->SetTitle("HT2 Inclusive");
  xiao->Draw();
  zach->Draw("same");
  zach->Rebin(2);
  zach->SetMarkerColor(kRed);
  zach->SetLineColor(kRed);
  divi = (TH1F*)xiao->Clone();
  cout << "made clone." << endl;
  divi->Divide(divi,zach);
  cout << "division done." << endl;
  c->cd(2);
  divi->GetYaxis()->SetRangeUser(0.75,1.25);
  divi->GetYaxis()->SetTitle("Xiaozhi/Zach");
  divi->Draw();
  line->SetLineColor(kGreen+3);
  line->Draw("same");
}
