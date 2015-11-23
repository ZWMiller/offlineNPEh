  
/************************************************

  This macro is for the photonic electron reconstruction efficiency
  xiaozhi bai
  xiaozhi@uic.edu
Thu Jul  9 11:47:12 CDT 2015
 ************************************************/
#include <iostream>
#include "TLatex.h"
#include "TStyle.h"
#include "TH3F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "Bin.h"
#include "Binning.h"
//const int NPtBin= 28;


using namespace std;
//Double_t pt_arr[]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.4,1.7,2,2.3,2.6,3,3.5,4,4.5,5,5.5,6,6.5,7,7.5,8.5,10,14};
//Double_t pt_arr[]={0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1,1.1,1.2,1.4,1.7,2,2.3,2.6,3,3.5,4};//,4.5,5,5.5,6,6.5,7,7.5,8.5,10,14};

TH1F * mh1Sinle_Track_HT[3];
TH1F * mh1Pair_HT[3];

TH1F * mh1Sinle_Track_MB[3];
TH1F * mh1Pair_MB[3];


TH1F * PHE_RE_MB[3];
TH1F * PHE_RE_HT[3];

//TGraphAsymmErrors  * Photonic_re_eff[3];

TString histname[3]={"Gamma_conversion","Pi0Dalitz","EtaDalitzDecay"};
void Draw_effciency(TH1F *a[],TH1F *b[]);
void Draw_effciency_Total( TH1F * const  PHE_RE_HT[],TH1F * const PHE_RE_MB[] );
TLatex* drawLatex(Double_t, Double_t, char* , Int_t , Double_t , Int_t);
void setpad(TPad *,float , float , float , float );


void Photonic_RE_efficiency(TString Gamma_conversion_HT="Root_File_PHE/Gamma_HTRC.root",TString Pi0Dalitz_HT="Root_File_PHE/Pi0_HTRC.root",TString EtaDalitz_HT="Root_File_PHE/Eta_HTRC.root",TString Gamma_conversion_MB="Root_File_PHE/Gamma_MBRC.root",TString Pi0Dalitz_MB="Root_File_PHE/Pi0_MBRC.root",TString EtaDalitz_MB="Root_File_PHE/Eta_MBRC.root")
//void Photonic_RE_efficiency(TString Gamma_conversion_HT="Gamma_HTMC.root",TString Pi0Dalitz_HT="Pi0_HTMC.root",TString EtaDalitz_HT="Eta_HTMC.root",TString Gamma_conversion_MB="Gamma_MBMC.root",TString Pi0Dalitz_MB="Pi0_MBMC.root",TString EtaDalitz_MB="Eta_MBMC.root")
{
  char buf[1024];
  gStyle->SetOptFit(kTRUE);                                                    
  gStyle->SetTitleSize(0.05,"XY");
  gStyle->SetTitleFontSize(0.08);
  gStyle->SetTitleOffset(1,"X");
  gStyle->SetTitleOffset(1,"Y");
  //  gStyle->SetLabelSize(0.08,"xy");
  gStyle->SetPadTopMargin(0.13);
  gStyle->SetPadRightMargin(0.02);
  gStyle->SetPadBottomMargin(0.13);
  gStyle->SetPadLeftMargin(0.13); 
  
  TH1F::SetDefaultSumw2();  
  TFile * inFile_Gamma_MB= new TFile(Gamma_conversion_MB); //gammma  
  TFile * inFile_Pi0Dalitz_MB= new TFile(Pi0Dalitz_MB); //  Pi0
  TFile * inFile_EtaDalitz_MB= new TFile(EtaDalitz_MB);  //eta

  TFile * inFile_Gamma_HT= new TFile(Gamma_conversion_HT); //gammma  
  TFile * inFile_Pi0Dalitz_HT= new TFile(Pi0Dalitz_HT); //  Pi0
  TFile * inFile_EtaDalitz_HT= new TFile(EtaDalitz_HT);  //eta
  
  // Get single and pair for gamma, Pi0, Eta 
  mh1Sinle_Track_HT[0]=(TH1F *) inFile_Gamma_HT->Get("mSingle_Track");
  mh1Pair_HT[0]=(TH1F *)inFile_Gamma_HT->Get("mPair");

  mh1Sinle_Track_HT[1]=(TH1F *) inFile_Pi0Dalitz_HT->Get("mSingle_Track");
  mh1Pair_HT[1]=(TH1F *)inFile_Pi0Dalitz_HT->Get("mPair");
  
  mh1Sinle_Track_HT[2]=(TH1F *) inFile_EtaDalitz_HT->Get("mSingle_Track");
  mh1Pair_HT[2]=(TH1F *)inFile_EtaDalitz_HT->Get("mPair");

  // Get single and pair for gamma, Pi0, Eta 
  mh1Sinle_Track_MB[0]=(TH1F *) inFile_Gamma_MB->Get("mSingle_Track");
  mh1Pair_MB[0]=(TH1F *)inFile_Gamma_MB->Get("mPair");

  mh1Sinle_Track_MB[1]=(TH1F *) inFile_Pi0Dalitz_MB->Get("mSingle_Track");
  mh1Pair_MB[1]=(TH1F *)inFile_Pi0Dalitz_MB->Get("mPair");
  
  mh1Sinle_Track_MB[2]=(TH1F *) inFile_EtaDalitz_MB->Get("mSingle_Track");
  mh1Pair_MB[2]=(TH1F *)inFile_EtaDalitz_MB->Get("mPair");
 
  // std::cout<< sizeof(pt_arr)/sizeof(pt_arr[0])<<std::endl;

  //  mh1Pair_HT[0]->Draw();
  // return ;
  TString histname_single_HT[3]={"Gamma_conversion_single_HT","Pi0Dalitz_single_HT","EtaDalitzDecay_single_HT"};
  TString histname_pair_HT[3]={"Gamma_conversion_pair_HT","Pi0Dalitz_pair_HT","EtaDalitzDecay_pair_HT"};

  TString histname_single_MB[3]={"Gamma_conversion_single_MB","Pi0Dalitz_single_MB","EtaDalitzDecay_single_MB"};
  TString histname_pair_MB[3]={"Gamma_conversion_pair_MB","Pi0Dalitz_pair_MB","EtaDalitzDecay_pair_MB"};
  
  TH1F *  mh1Sinle_Track_rebin_HT[3];
  TH1F * mh1Pair_rebin_HT[3];
  TH1F *  mh1Sinle_Track_rebin_MB[3];
  TH1F * mh1Pair_rebin_MB[3];
     
  

  TGraphAsymmErrors  * Photonic_re_eff_HT[3];   
  TGraphAsymmErrors  * Photonic_re_eff_MB[3];   


  
  for(Int_t i=0;i<3;i++)
    {
      mh1Sinle_Track_rebin_HT[i]=(TH1F *) mh1Sinle_Track_HT[i]->Rebin(Nbins_AA,histname_single_HT[i],pt_arr_AuAu);


      mh1Pair_rebin_HT[i]=(TH1F * ) mh1Pair_HT[i]->Rebin(Nbins_AA,histname_pair_HT[i],pt_arr_AuAu);
      mh1Sinle_Track_rebin_MB[i]=(TH1F *) mh1Sinle_Track_MB[i]->Rebin(Nbins_AA,histname_single_MB[i],pt_arr_AuAu);
      mh1Pair_rebin_MB[i]=(TH1F * ) mh1Pair_MB[i]->Rebin(Nbins_AA,histname_pair_MB[i],pt_arr_AuAu);
      
      cout<<       mh1Sinle_Track_rebin_HT[i]->GetNbinsX()<<endl;
      cout<<       mh1Pair_rebin_HT[i]->GetNbinsX()<<endl;

      cout<<       mh1Sinle_Track_rebin_MB[i]->GetNbinsX()<<endl;
      cout<<       mh1Pair_rebin_MB[i]->GetNbinsX()<<endl;
      


      // mh1Sinle_Track_rebin_HT[i]->SetMarkerStyle(20);
      // mh1Pair_rebin_HT[i]->SetMarkerStyle(24);
      // mh1Sinle_Track_rebin_MB[i]->SetMarkerStyle(20);
      // mh1Pair_rebin_MB[i]->SetMarkerStyle(24);
      
      // mh1Sinle_Track_rebin_MB[i]->SetMarkerColor(2);
      // mh1Pair_rebin_MB[i]->SetMarkerColor(2);

      cout<< " "<< " 11"<<i<<endl;      
      Photonic_re_eff_HT[i]=new TGraphAsymmErrors(mh1Pair_rebin_HT[i],mh1Sinle_Track_rebin_HT[i],"N");
      cout<< " "<< " 22"<<endl;      
      Photonic_re_eff_MB[i]=new TGraphAsymmErrors(mh1Pair_rebin_MB[i],mh1Sinle_Track_rebin_MB[i],"N");
      cout<< " "<< " 33"<<i<<endl;      
      
      PHE_RE_MB[i]=new TH1F(histname[i]+TString("MB"),"Photonic electron reconstruction efficiency",Nbins_AA,pt_arr_AuAu);
      PHE_RE_HT[i]=new TH1F(histname[i]+TString("HT"),"Photonic electron reconstruction efficiency",Nbins_AA,pt_arr_AuAu);
      
      for(Int_t ipt=0;ipt<Nbins_AA;ipt++)
	{
	  Double_t x=0,y=0,x_err=0,y_err=0;
	  Photonic_re_eff_HT[i]  ->GetPoint(ipt,x,y);
	  y_err=Photonic_re_eff_HT[i]->GetErrorY(ipt);
	   cout<<"i  "<<ipt<<" x  "<<x<<"yy "<<y<<  "   "<<y_err<<endl;
	  
	  PHE_RE_HT[i]->SetBinContent(ipt+1,y);
	  PHE_RE_HT[i]->SetBinError(ipt+1,y_err);
	  
	  Photonic_re_eff_MB[i]  ->GetPoint(ipt,x,y);
	  y_err=Photonic_re_eff_MB[i]->GetErrorY(i);
	  
	  PHE_RE_MB[i]->SetBinContent(ipt+1,y);
	  PHE_RE_MB[i]->SetBinError(ipt+1,y_err);
	  
	}
    }
 
 // mh1Sinle_Track_rebin_MB[0]->Draw();
 //  return ;


  // PHE_RE_MB[0]->Draw();
  //    return ;

  Draw_effciency(PHE_RE_HT,PHE_RE_MB);
  Draw_effciency_Total(PHE_RE_HT,PHE_RE_MB);
}
void Draw_effciency_Total( TH1F * const  PHE_RE_HT[],TH1F * const PHE_RE_MB[] )
{

  TFile * file_PHE=TFile::Open("PHE_Contribution_MB.root");
  TH1F*	mh1_Gamma=(TH1F *)file_PHE->Get("mh1_Gamma");	
  TH1F*	mh1_pi0=(TH1F *)file_PHE->Get("mh1_pi0");	
  TH1F*	mh1_Eta=(TH1F *)file_PHE->Get("mh1_Eta");


  TH1F *mh1_HT=new TH1F("mh1_HT","",Nbins_AA,pt_arr_AuAu);
  TH1F *mh1_MB=new TH1F("mh1_MB","",Nbins_AA,pt_arr_AuAu);

  TH1F *Gamma_MB=(TH1F *) PHE_RE_MB[0]->Clone("Gamma_MB");
  TH1F *pi0_MB=(TH1F *) PHE_RE_MB[1]->Clone("pi0_MB");
  TH1F *Eta_MB=(TH1F *) PHE_RE_MB[2]->Clone("Eta_MB");

  // cout<<"Gamma "<<endl;
  // Gamma_MB->Print("all");
  // cout<<"pi0 "<<endl;
  // pi0_MB->Print("all");
  // cout<<"Eta "<<endl;
  // Eta_MB->Print("all");


  // cout<<"Gamma 1 "<<endl;
  // mh1_Gamma->Print("all");
  // cout<<"pi0 1 "<<endl;
  // mh1_pi0->Print("all");
  // cout<<"Eta 1 "<<endl;
  // mh1_Eta->Print("all");
  
  // Code to shift the contribution by its error before fits

 /*TCanvas * test = new TCanvas("test");
  TH1F* changeHist = mh1_Gamma;
  TH1F* beforeShift = (TH1F*) changeHist->Clone();
  double shiftFactor = 0.14;
  test->cd();
  beforeShift->SetLineColor(kBlack);
  beforeShift->SetMarkerColor(kBlack);
  beforeShift->SetTitle("#gamma #rightarrow e^{+}e^{-}");
  beforeShift->GetYaxis()->SetTitle("Contribution to PHE");
  beforeShift->GetYaxis()->SetRangeUser(0,1);
  beforeShift->Draw("P");
  TAxis* ax = changeHist->GetXaxis();
  for(int chn = ax->GetFirst(); chn <= ax->GetLast(); chn++)
  {
    double current = changeHist->GetBinContent(chn);
    double error = changeHist->GetBinError(chn);
    changeHist->SetBinContent(chn, current+error);
    //changeHist->SetBinContent(chn, current+current*shiftFactor);
  }
  changeHist->SetLineColor(kRed);
  changeHist->SetMarkerColor(kRed);
  changeHist->DrawClone("same");
  TLegend* legZ = new TLegend(0.57,0.7,0.88,0.8);
  legZ->SetTextSize(.03);
  legZ->AddEntry(beforeShift,"Contribution","pl");
  legZ->AddEntry(changeHist,"Contribution #pm 1#sigma","pl");
  //legZ->AddEntry(changeHist,Form("Contribution #pm %.1f%%",shiftFactor*100),"pl");
  legZ->Draw("same"); 
*/
  /*changeHist = mh1_Gamma;
  beforeShift = (TH1F*) changeHist->Clone();
  ax = changeHist->GetXaxis();
  for(int chn = ax->GetFirst(); chn <= ax->GetLast(); chn++)
  {
    double current = changeHist->GetBinContent(chn);
    double error = changeHist->GetBinError(chn);
    changeHist->SetBinContent(chn, current+current*shiftFactor);
  }*/


  Gamma_MB->Multiply(mh1_Gamma);
  pi0_MB->Multiply(mh1_pi0);
  Eta_MB->Multiply(mh1_Eta);


  // Gamma_MB->Multiply(mh1_Gamma);
  // pi0_MB->Multiply(mh1_Eta);
  // Eta_MB->Multiply(mh1_pi0);

  mh1_MB->Add(pi0_MB);
  mh1_MB->Add(Gamma_MB);
  mh1_MB->Add(Eta_MB);

  cout<<"Total "<<endl;

  mh1_MB->Print("all");

  /*TCanvas * test = new TCanvas("test");
  TH1F* changeHist = mh1_MB;
  changeHist->SetMarkerStyle(20);
  TH1F* beforeShift = (TH1F*) changeHist->Clone();
  test->cd();
  beforeShift->SetLineColor(kBlack);
  beforeShift->SetMarkerColor(kBlack);
  beforeShift->SetTitle("Photonic Electron Reco. Efficiency");
  beforeShift->GetYaxis()->SetTitle("Efficiency");
  beforeShift->GetYaxis()->SetRangeUser(0,1);
  beforeShift->Draw("P");
  TAxis* ax = changeHist->GetXaxis();
  for(int chn = ax->GetFirst(); chn <= ax->GetLast(); chn++)
  {
    double current = changeHist->GetBinContent(chn);
    double error = changeHist->GetBinError(chn);
    changeHist->SetBinContent(chn, current-error);
  }
  changeHist->SetLineColor(kRed);
  changeHist->SetMarkerColor(kRed);
  changeHist->DrawClone("same");
  TLegend* legZ = new TLegend(0.57,0.7,0.88,0.8);
  legZ->SetTextSize(.03);
  legZ->AddEntry(beforeShift,"Reco. Efficiency","pl");
  legZ->AddEntry(changeHist,"Reco. Efficiency #pm 1#sigma","pl");
  //legZ->AddEntry(changeHist,Form("Contribution #pm %.1f%%",shiftFactor*100),"pl");
  legZ->Draw("same"); */


  //  return;

  TCanvas *c2 = new TCanvas("c2"," Rhotonic_Re_eff",0,0,800,600);
  c2->cd(); 

  TH2F * hh=new TH2F("hh","Photonic Electron Rec. Efficiency",100,0,20,100,0,1);
  
  hh->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hh->GetYaxis()->SetTitle(" #epsilon Photonic");
  hh->GetYaxis()->CenterTitle(true);
  hh->GetXaxis()->CenterTitle(true);

  hh->Draw();

  mh1_MB->SetMarkerStyle(20);
  //  mh1_MB->SetMarkerStyle(20);

  TF1 *f_1 = new TF1("f_1","[0]*(1.0 -TMath::Exp(-(x+[1])/[2]))",0,20);
  f_1->SetParameters(0.8,9.37659e-02,4.40904e-01);
  f_1->SetLineColor(2);
  double p[3];
  mh1_MB->Fit("f_1","R+","same",0.2,14);
  
  // Write fit parameters to text file to be grabbed by offline.C - ZWM
  TF1* fitRes = mh1_MB->GetFunction("f_1");
  p[0] = fitRes->GetParameter(0);
  p[1] = fitRes->GetParameter(1);
  p[2] = fitRes->GetParameter(2);
  FILE* pFile;
  pFile = fopen("../effParFits.txt","w");
  fprintf(pFile,"%lf %lf %lf",p[0],p[1],p[2]);
  fclose(pFile);


  // TFile *file=new TFile("Photonic_re_Efficiency.root","READ");
  // TH1F *mh1_MB_test=(TH1F *) file->Get("mh1_MB");
  // mh1_MB_test->SetMarkerColor(2);
  
  //  mh1_MB_test->Draw("sameP");
   
  c2->SaveAs("PHE_Efficiency.pdf");
  TFile *file=new TFile("Photonic_re_Efficiency.root","RECREATE");
        mh1_MB->Write();
  	file->Close();

  // TF1 *f_2 = new TF1("f_2","[0]/(TMath::Exp(-(x-[1])/[2]) + 1)",0,20);
  // f_2->FixParameter(0,0.8);
  // f_2->SetParameter(1,5.0);
  // f_2->SetParameter(2,1.5);

  // f_2->SetLineColor(4);

  //  mh1_MB->Fit("f_2","R+","same",0.2,14);


  //  mh1_MB->Draw("sameCL");


}

void Draw_effciency( TH1F *  PHE_RE_HT[],TH1F *PHE_RE_MB[] )
{
  
  
  // TH1F::SetDefaultSumw2();
  char buf[1024];
  gStyle->SetOptStat(00000);    
  TCanvas *c2 = new TCanvas("c2"," Rhotonic_Re_eff",0,0,800,600);
  c2->cd(); 

  TH2F * hh=new TH2F("hh","Photonic Electron Rec. Efficiency",100,0,20,100,0,1);
  
  hh->GetXaxis()->SetTitle("p_{T} (GeV/c)");
  hh->GetYaxis()->SetTitle(" #epsilon Photonic");
  hh->GetYaxis()->CenterTitle(true);
  hh->GetXaxis()->CenterTitle(true);
  //Gamma
  PHE_RE_MB[0]->SetMarkerStyle(20);
  PHE_RE_MB[0]->SetMarkerColor(1);
  PHE_RE_MB[0]->SetLineColor(1);
  PHE_RE_HT[0]->SetMarkerStyle(24);
  PHE_RE_HT[0]->SetMarkerColor(1);
  PHE_RE_HT[0]->SetLineColor(1);
  //Pi0
  PHE_RE_MB[1]->SetMarkerStyle(20);
  PHE_RE_MB[1]->SetMarkerColor(2);
  PHE_RE_MB[1]->SetLineColor(2);
  PHE_RE_HT[1]->SetMarkerStyle(24);
  PHE_RE_HT[1]->SetMarkerColor(2);
  PHE_RE_HT[1]->SetLineColor(2);
  //Eta
  
  PHE_RE_MB[2]->SetMarkerStyle(20);
  PHE_RE_MB[2]->SetMarkerColor(4);
  PHE_RE_MB[2]->SetLineColor(4);
  PHE_RE_HT[2]->SetMarkerStyle(24);
  PHE_RE_HT[2]->SetMarkerColor(4);
  PHE_RE_HT[2]->SetLineColor(4);
  
  hh->Draw();
  PHE_RE_MB[0]->Draw("samePE1");
  PHE_RE_MB[1]->Draw("samePE1");
  PHE_RE_MB[2]->Draw("samePE1");

 
  PHE_RE_HT[0]->Draw("samePE1");
  PHE_RE_HT[1]->Draw("samePE1");
  PHE_RE_HT[2]->Draw("samePE1");
  
  


  TLegend *legend_HT = new TLegend(0.65,0.35,0.85,0.5);   
  legend_HT->AddEntry(PHE_RE_HT[0], "HT  #gamma conversion","lp"); 
  legend_HT->AddEntry(PHE_RE_HT[1], "HT  #pi0 #rightarrow->#gamma e^{+} e^{-}","lp"); 
  legend_HT->AddEntry(PHE_RE_HT[2], "HT  #eta #rightarrow->#gamma e^{+} e^{-}","lp"); 

  TLegend *legend_MB = new TLegend(0.5,0.35,0.65,0.5);

  legend_MB->AddEntry(PHE_RE_MB[0], " MB  #gamma conversion","lp"); 
  legend_MB->AddEntry(PHE_RE_MB[1], " MB  #pi #rightarrow->#gamma e^{+} e^{-}","lp"); 
  legend_MB->AddEntry(PHE_RE_MB[2], " MB  #eta #rightarrow->#gamma e^{+} e^{-}","lp"); 
  
  legend_HT->SetTextSize(0.03); 
  legend_HT->SetBorderSize(0);
  legend_HT->SetFillStyle(0);   
  legend_HT->Draw();

  legend_MB->SetTextSize(0.03); 
  legend_MB->SetBorderSize(0);
  legend_MB->SetFillStyle(0);   
  legend_MB->Draw();


  c2->SaveAs("PHE_Plots/Photonic_re_efficiency_La0080.pdf");    
  
  // fit 

  TCanvas *c3=new TCanvas("c3","",800,600);
  c3->cd();

  hh->Draw();
  TF1 *f0 = new TF1("f0","[0]*(1.0 -TMath::Exp(-(x+[1])/[2]))",0,20);
  f0->SetParameters(0.8,9.37659e-02,4.40904e-01);

  f0->SetLineColor(1);
  TF1 *f1 = new TF1("f1","[0]*(1.0 -TMath::Exp(-(x+[1])/[2]))",0,20);
  f1->SetParameters(0.8,9.37659e-02,4.40904e-01);
  f1->SetLineColor(2);

  TF1 *f2 = new TF1("f2","[0]*(1.0 -TMath::Exp(-(x+[1])/[2]))",0,20);
  f2->SetParameters(0.8,9.37659e-02,4.40904e-01);

  f2->SetLineColor(4);

  PHE_RE_MB[0]->Fit("f0","R","same",0.2,14);
  PHE_RE_MB[1]->Fit("f1","R","same",0.2,14);
  PHE_RE_MB[2]->Fit("f2","R","same",0.2,14);
  legend_MB->Draw();


  c3->SaveAs("PHE_Plots/Photonic_MB.pdf");


  TCanvas *c4=new TCanvas("c4","",800,600);
  c4->cd();

  hh->Draw();
  PHE_RE_HT[0]->Fit("f0","R","same",0.2,14);
  PHE_RE_HT[1]->Fit("f1","R","same",0.2,14);
  PHE_RE_HT[2]->Fit("f2","R","same",0.2,14);
  legend_HT->Draw();
  c4->SaveAs("PHE_Plots/Photonic_HT.pdf");



  
  // TFile *file_2=new TFile("Photonic_ReconStructionEfficiency_MB.root","RECREATE");
  
  // PHE_RE_MB[0]->Write();
  // PHE_RE_MB[1]->Write();
  // PHE_RE_MB[2]->Write();
  
  // file_2->Close();

}

void setpad(TPad *pad,float left, float right, float top, float bottom)
{       
  pad->SetFillColor(10);                                                      
  pad->SetBorderMode(0);                                                       
  pad->SetBorderSize(10);                                                      
  pad->SetFrameFillColor(10);                                                  
  pad->SetFrameBorderMode(0);                                                  
  pad->SetFrameBorderSize(0);                                                  
  pad->SetLeftMargin(left);                                                    
  pad->SetRightMargin(right);
  pad->SetTopMargin(top);
  pad->SetRightMargin(right);                                                      
  pad->SetBottomMargin(bottom);                                                
}
TLatex* drawLatex(Double_t x, Double_t y, char* text, Int_t textFont, Double_t textSize, Int_t colorIndex)
{
  TLatex *latex = new TLatex(x,y,text);                                        
  latex->SetNDC();                                                             
  latex->SetTextFont(textFont);                                                
  latex->SetTextSize(textSize);                                                
  latex->SetTextColor(colorIndex);                                             
  latex->Draw("same");                                                      
  return latex;                                                                
}




