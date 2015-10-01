{
  Int_t l=0;
  char line[1000];
  Float_t p0[100],ep0[100],p1[100],ep1[100],x[100];
  Float_t dx[100]={0.};

  ifstream fp("pileupFits.txt",ios::in);
  while (!fp.eof()){
    fp.getline(line,1000);
    sscanf(line,"%f %f %f %f",&p0[l],&ep0[l],&p1[l],&ep1[l]);
    printf("L: %f %f\n",p0[l],p1[l]);
    x[l]=l;
    l++;
  }
  fp.close();

  TGraphErrors *grp0 = new TGraphErrors(l-1,x,p0,dx,ep0);
  TGraphErrors *grp1 = new TGraphErrors(l-1,x,p1,dx,ep1);

  TCanvas* cP0 = new TCanvas("cP0","Parameter 0 (x=0)",100,0,1100,1000);
  TCanvas* cP1 = new TCanvas("cP1","Parameter 1 (Slope)",100,0,1100,1000);

  axis0 = grp0->GetXaxis();
  axis1 = grp1->GetXaxis();
  axis0->SetBinLabel(axis0->FindBin(0.1),"NPE 0");
  axis0->SetBinLabel(axis0->FindBin(1.),"INC 0");
  axis0->SetBinLabel(axis0->FindBin(2.),"US 0");
  axis0->SetBinLabel(axis0->FindBin(3.),"LS 0");
  axis0->SetBinLabel(axis0->FindBin(4.),"HAD 0");
  axis0->SetBinLabel(axis0->FindBin(5.),"NPE 2");
  axis0->SetBinLabel(axis0->FindBin(6.),"INC 2");
  axis0->SetBinLabel(axis0->FindBin(7.),"US 2");
  axis0->SetBinLabel(axis0->FindBin(8.),"LS 2");
  axis0->SetBinLabel(axis0->FindBin(9.),"HAD 2");
  axis1->SetBinLabel(axis1->FindBin(0.1),"NPE 0");
  axis1->SetBinLabel(axis1->FindBin(1.),"INC 0");
  axis1->SetBinLabel(axis1->FindBin(2.),"US 0");
  axis1->SetBinLabel(axis1->FindBin(3.),"LS 0");
  axis1->SetBinLabel(axis1->FindBin(4.),"HAD 0");
  axis1->SetBinLabel(axis1->FindBin(5.),"NPE 2");
  axis1->SetBinLabel(axis1->FindBin(6.),"INC 2");
  axis1->SetBinLabel(axis1->FindBin(7.),"US 2");
  axis1->SetBinLabel(axis1->FindBin(8.),"LS 2");
  axis1->SetBinLabel(axis1->FindBin(9.),"HAD 2");

  cP0->cd();
  grp0->SetMarkerStyle(20);
  grp0->GetXaxis()->SetRangeUser(-1,11);
  grp0->SetName("Parameter 0 from Different Triggers");
  grp0->Draw("ap");

  cP1->cd();
  grp1->GetXaxis()->SetRangeUser(-1,11);
  grp1->SetMarkerStyle(20);
  grp1->SetName("Parameter 1 from Different Triggers");
  grp1->Draw("ap");
}
