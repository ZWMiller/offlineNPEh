{

  // Open ROOT File
  TFile *f = new TFile("/Users/zach/Research/rootFiles/june2015/npe_tree_june25.root","READ");
  if (!f)
    { std::cout << "!!! File Not Found !!!" << std::endl; exit(1); }
  // f->ls(); // - DEBUG by printing all objects in ROOT file
  
  // Create and Segment Canvas
  c1 = new TCanvas("c1","Test Canvas",0,0,900,500);
  c1->Divide(1,1);
  
  // Grab Histograms for manipulation (can draw without Get, must Get for manip)
  TH1F * LSDP = (TH1F*)f->Get("mh1DeltaPhiPhotonicLS_lowPt"); // Like Sign Delta Phi (Photonic Electrons)
  TH1F * USDP = (TH1F*)f->Get("mh1DeltaPhiPhotonicUS_lowPt"); // Unlike Sign Delta Phi (Photonic Electrons)

  // Actually manipulate histos and plot
  c1->cd(1);
  c1_1->SetLogy();
  USDP->SetLineColor(kRed);
  USDP->SetLineWidth(2);
  USDP->Draw();

  LSDP->SetLineColor(kBlack);
  LSDP->SetLineWidth(2);
  LSDP->Draw("same");

  TH1F *SUB = (TH1F*)USDP->Clone(); //
  SUB->SetName("Subtraction");      // Create SUB as a clone of USDP
  SUB->Add(LSDP,-1);
  SUB->SetLineColor(kBlue);
  SUB->SetFillStyle(3001);
  SUB->SetFillColor(kBlue);
  SUB->Draw("sameL");

  // Close ROOT File
  //f->Close();

  // Make PDF if Desired
  //c1->Print("c1.pdf","pdf");
}

