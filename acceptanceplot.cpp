//file to plot CLAS acceptance 
#include <string>
#include <TH3.h>
#include <TH1D.h>
#include <TMath.h>
#include <TFile.h>
#include <TH2D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TString.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TPaletteAxis.h>
#include <TMath.h>
#include <TLine.h>
#include <TPad.h>
#include <TGaxis.h>
#include <iostream>
#include <vector>

using namespace std;
void acceptanceplot()
{
  gStyle->SetOptStat(false); // Disable  stats box                                                            

// Read file as a string                                                                                                                    
  string acceptance_file  = "C:/Users/Phron/Desktop/ROOT files/Pion_Production/Final_Scripts/Acc_12C_2_261.root";  

 // Create new file input from string
  TFile *f = new TFile(acceptance_file.c_str());


  f->ls(); // Print list of histograms in file     
  

  //Choose what to graph for acceptance  

  //TString Option = "xy";
    TString Option = "yz";
  //TString Option = "xz";

   //x = momentum , y = cos theta,  z = phi
  
  TCanvas *c1 = new TCanvas("e4nu","e4nu",1920,1080); // Create canvas for plotting acceptance
  
  TH3D *acc = (TH3D*) f->Get("Accepted Particles"); //Create 3D histogram of Accepted particles from ROOT file
  TH3D *gen = (TH3D*) f->Get("Generated Particles");//Create 3D histogram of Generated particles from ROOT file
  
  //Set range of axis (optional)
  //  acc->GetYaxis()->SetRangeUser(0.6,1);
  //  gen->GetYaxis()->SetRangeUser(0.6,1);
  
  TH2D *acc2D = (TH2D*) acc->Project3D(Option);
  TH2D *gen2D = (TH2D*) gen->Project3D(Option);
    
  TH2D *accrat2D = (TH2D*) (acc2D->Clone());//Creates clone of histogram (incase of overwriting)
  accrat2D->Divide(gen2D);//Divide acceptance by generated histsogram (yeilds acceptance)

  accrat2D->SetTitle("CLAS acceptance for final state proton in electron scattering on C12 at 2.261GeV");

  //Label Axis
  //accrat2D->GetZaxis()->SetTitle("proton momentum[GeV]");
  //accrat2D->GetYaxis()->SetTitle("proton momentum[GeV]");
  accrat2D->GetXaxis()->SetTitle("#phi(Degrees)");    
  accrat2D->GetYaxis()->SetTitle("Cos #theta");
  
  gen2D->SetTitle("CLAS acceptance for electron scattering on C12 at 2.261GeV");
  gen2D->GetYaxis()->SetTitle("electron momentum[GeV]");
  gen2D->GetXaxis()->SetTitle("cos #theta");

  //Set color palette
  gStyle->SetPalette(75);
  TColor::InvertPalette();

  //Draw acceptance onto canvas
  accrat2D->Draw("colz");
  //gen2D->Draw("colz");
  //  acc2D->Draw("colz");

  //Save canvas as a png
  c1->SaveAs("CLASacceptance_2GeV.png");

}