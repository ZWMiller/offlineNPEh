{
  // Set Style parameters for this macro
  gStyle->SetOptTitle(1); // Show Title (off by default for cleanliness)

  
  // Open ROOT File
  TFile *f = new TFile("/Users/zach/Research/rootFiles/july2015/npe_tree_july20_3.root","READ");
  if (!f)
    { std::cout << "!!! File Not Found !!!" << std::endl; exit(1); }
  // f->ls(); // - DEBUG by printing all objects in ROOT file
  
  // Create and Segment Canvas
  c1 = new TCanvas("cHERE","Test Canvas",0,0,900,500);
  c1->Divide(1,1);
  
  // Grab Histograms for manipulation (can draw without Get, must Get for manip)
  TH1F * LSIM = (TH1F*)f->Get("projDelPhiPhotLS_0_2"); // Like Sign Delta Phi (Photonic Electrons)
  TH1F * USIM = (TH1F*)f->Get("projDelPhiPhotUS_0_2"); // Unlike Sign Delta Phi (Photonic Electrons)

  // Actually manipulate histos and plot
  c1->cd(1);
  cHERE_1->SetLogy(0);
  USIM->SetLineColor(kRed);
  USIM->SetLineWidth(2);
  USIM->GetXaxis()->SetTitle("#Delta#phi_{eh}");
  USIM->GetXaxis()->SetRangeUser(0,3.5);
  USIM->SetTitle("Photonic Electron Reconstruction");
  USIM->Draw("hist");

  LSIM->SetLineColor(kBlack);
  LSIM->SetLineWidth(2);
  LSIM->Draw("hist same");

  TH1F *SUB = (TH1F*)USIM->Clone(); //
  SUB->SetName("Subtraction");      // Create SUB as a clone of USIM
  SUB->Add(LSIM,-1);
  SUB->SetLineColor(kBlue);
  SUB->SetFillStyle(3001);
  SUB->SetFillColor(kBlue);
  SUB->Draw("hist same");

  TLegend* leg = new TLegend(0.7,0.7,0.85,0.85);
  leg->AddEntry(USIM,"Unlike Sign","l");
  leg->AddEntry(LSIM,"Like Sign", "l");
  leg->AddEntry(SUB,"Unlike - Like", "f");
  leg->Draw();

  // Close ROOT File
  //f->Close();
  //delete f;

  // Make PDF if Desired
  //c1->Print("c1.pdf","pdf");
}

