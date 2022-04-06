// Plots number of expected events as a function of the invariant hadronic mass, W
#include "TCanvas.h"
#include "TH1.h"
#include "TLegend.h" 
#include <iostream>
#include <stdio.h>

void Plot_W_Signal()

{
    // Disable status box
    gStyle->SetOptStat(false);

    // Open the input ROOT file
    TFile* inputFile_1 = new TFile("C12_1_1_GeV_G2019.root");
    inputFile_1->ls();

    // Load the ntuple from the file. 
    TTree* tree_1 = (TTree*)inputFile_1->Get("gst");  

// Cuts for only 1 pion in the final state (for neutrino files consider adding "cc" in these cuts as well)
    string qel_cut = "qel  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";
    string dis_cut = "dis  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";
    string mec_cut = "mec  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";
    string res_cut = "res  && resid != 0 && (nfpip + nfpim  == 1) && nfpi0 == 0 "; //note we are not including nfpi0 

        string resid_0_cut = "res && resid == 0  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";// Delta 1232 RES events

        //Higher RES events
        //string resid_1_cut = "res && resid == 1  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";
        //string resid_2_cut = "res && resid == 2  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";
        //string resid_3_cut = "res && resid == 3  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";

    string background_cut = "resid != 0 && (nfpip + nfpim  == 1) && nfpi0 == 0 ";

// Take in full signal
    TH1D* h_W = new TH1D("h_W", "Invariant Hadronic Mass For Argon 40 #nu^{#mu}", 100, 0, 3);
        tree_1->Draw("W>>h_W");
        int n_total = tree_1->GetEntries();
//Plot RES Events

        TH1D* h1_RES = new TH1D("h1_RES", "Invariant Hadronic Mass For Argon 40 #nu", 100, 0, 3); 
        tree_1->Draw("W>>h1_RES", res_mn_cut.c_str()); 

//Plot DIS events
        TH1D* h1_DIS = new TH1D("h1_DIS", "Invariant Hadronic Mass For Argon 40 #nu", 100, 0, 3); 
        tree_1->Draw("W>>h1_DIS", dis_mn_cut.c_str());

//Plot MEC events
        TH1D* h1_MEC = new TH1D("h1_MEC", "Invariant Hadronic Mass For Argon 40 #nu", 100, 0, 3);
        tree_1->Draw("W>>h1_MEC", mec_mn_cut.c_str());

//Plot qel events
        TH1D* h1_qel = new TH1D("h1_qel", "Invariant Hadronic Mass For Argon 40 #nu", 100, 0, 3);
        tree_1->Draw("W>>h1_qel", qel_mn_cut.c_str());

//Add all background
        TH1D* h1_Background = new TH1D("h1_Background", "Background Signal", 100, 0, 3);
        tree_1->Draw("W>>h1_Background", background_cut.c_str());

//Plot W for res events (2D)

        TH1D* h1_Wres_id_0 = new TH1D("h_Wres_id_0", "{}^{12}C 1.1GeV Electron #Delta Resonances; W(GeV); Events / 50 MeV", 100, 0, 3);
        tree_1->Draw("W>>h_Wres_id_0", resid_0_mn_cut.c_str());
        h1_Wres_id_0->SetLineColor(kRed);
        h1_Wres_id_0->SetLineWidth(4);
//Create stacked histogram for background signals

    THStack hs_background("hs_background", "{}^{12}C 2.2GeV Electron #Delta Resonances; W(GeV); Events / 50 MeV");

    hs_background.Add(h1_MEC);
    h1_MEC->SetFillColor(kOrange);
    hs_background.Add(h1_qel);
    h1_qel->SetFillColor(kPink);
    hs_background.Add(h1_RES);
    h1_RES->SetFillColor(kGreen);
    hs_background.Add(h1_DIS);
    h1_DIS->SetFillColor(kBlue);

  // Create canvas
    TCanvas c1("c1", "stacked hists", 10, 10, 1920, 1080);
    h1_Wres_id_0->Draw("HIST");
    hs_background.Draw("HIST SAME");
    h1_Wres_id_0->Draw("HIST SAME");
    

    // Add legend
    auto legend = new TLegend(0.68, 0.68, 0.9, 0.9);
    legend->SetHeader("Legend", "C");
    legend->AddEntry(h1_Wres_id_0, Form("#Delta 1232 Resonances (%d)", tree_1->GetEntries(resid_0_cut.c_str())), "l");
    legend->AddEntry(h1_qel, Form("QE (%d)", tree_1->GetEntries(qel_mn_cut.c_str())), "f");
    legend->AddEntry(h1_RES, Form("All other RES (%d)", tree_1->GetEntries(res_mn_cut.c_str())), "f");
    legend->AddEntry(h1_MEC, Form("MEC (%d)", tree_1->GetEntries(mec_mn_cut.c_str())), "f");
    legend->AddEntry(h1_DIS, Form("DIS (%d)", tree_1->GetEntries(dis_mn_cut.c_str())), "f");
    legend->Draw();

    // Save your output as an image file, so you can use it in presentations, papers etc
    c1.SaveAs("W_Signal_C12_1GeV_part2.png");
}
