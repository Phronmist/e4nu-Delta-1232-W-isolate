// Plots optimal high cut on the invariant hadronic mass, W
#include "TCanvas.h"
#include "TH1.h" 
#include "TLegend.h" 
#include <iostream>
#include <stdio.h>

void Plot_optimal_cut()

{
// Setup
    gStyle->SetOptStat(false);  // Disable status box

    TFile* inputFile = new TFile("C12_2GeV.root");  // Open ROOT file as input
    inputFile->ls();

    TTree* tree = (TTree*)inputFile->Get("gst");    // Load the ntuple from the file.

// Create cuts (default is 1 pion (plus or minus)) NOTE: we are not including pion 0 (nfpi0) in this case unless you want to

    string qel_cut = "qel  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";    // QE cuts
    string dis_cut = "dis  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";    // DIS cuts
    string mec_cut = "mec  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";    // MEC cuts
    string res_cut = "res  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";    // RES cuts
    string resid_0_cut = "res && resid == 0  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";  // Delta 1232 Cuts
    string resid_1_cut = "res && resid == 1  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";  // kS11_1535 RES cuts
    string resid_2_cut = "res && resid == 2  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";  // kD13_1520 RES cuts
    string resid_3_cut = "res && resid == 3  && (nfpip + nfpim  == 1) && nfpi0 == 0 ";  // kS11_1650 RES cuts

//Initilize W cut parameters


    float Max_W_cut = 1.8;  // Value of W to stop calculating
    float Min_W_cut = 1;    // Value of W to begin calculating
    float step_size = 0.01; // Step size of W for calculation
    int steps = (Max_W_cut - Min_W_cut) * (1 / step_size) + 1;  // Number of steps between max W and min W (notice this is an integer and is subject to rounding)
//Initilize arrays for plotting

    float pur_x_eff[steps]; //Purity times Efficiency  (y value of plot)
    float Eff_new[steps];   //Efficiency (y value of plot)
    float Pur_new[steps];   //Purity (y value of plot)
    float W[steps];         //Value of W (x value of plot)

    //Fill in W array with appropriate values (1.1, 1.2 etc.)
    for (int i = 0; i < steps; ++i)
    {
        W[i] = Min_W_cut + (i * step_size);
    }

    float resid_total = tree->GetEntries(resid_0_cut.c_str()); // # total Delta 1232 events from root file

//Calculate purity and efficiency for loop
    for (int i = 0; i < steps; ++i)
    {
        string W_0_cut = Form("resid ==0  && (nfpip + nfpim == 1) && nfpi0 == 0 && W<=%f", W[i]); //define cut on W for every step for signal
        float resid_cut_new = tree->GetEntries(W_0_cut.c_str()); // calculate Delta 1232 events within new cut
        Eff_new[i] = resid_cut_new / resid_total; // calculate Efficiency
        string signal_background = Form("(nfpip + nfpim == 1) && nfpi0 == 0 &&  W<=%f", W[i]);// calculate signal background
        float n_total_cut_new = tree->GetEntries(signal_background.c_str());    //Save new signal background as a float

        if (n_total_cut_new == 0) //avoid dividing by zero
            Pur_new[i] = 0;
        else
            Pur_new[i] = resid_cut_new / n_total_cut_new; // calculate purity
        pur_x_eff[i] = Eff_new[i] * Pur_new[i]; // calculate new element
    }
// create graph of purity and efficiency as a function of W

    TGraph* gr = new TGraph(steps, W, pur_x_eff);// Purity x Efficiency graph
    TGraph* gr2 = new TGraph(steps, W, Eff_new); //Efficecny Graph
    TGraph* gr3 = new TGraph(steps, W, Pur_new); //Purity Graph

    gr->GetXaxis()->SetTitle("High Cut on W"); //Label x Axis

//set color of graphs

    gr->SetLineColor(kRed);
    gr2->SetLineColor(kBlue);
    gr3->SetLineColor(kGreen);

//Create canvas

    TCanvas* c2 = new TCanvas("c2", "Graph Draw Options", 200, 10, 1920, 1080);

    TMultiGraph* mg = new TMultiGraph();
    mg->Add(gr);
    mg->Add(gr2);
    mg->Add(gr3);
    mg->SetTitle("Optimal Cut for 2.2GeV Electron Beam (1 #pi^{#pm} only) ;High Cut on W; Value");
    mg->GetXaxis()->CenterTitle(true);
    mg->GetYaxis()->CenterTitle(true);


    //find where purity x efficiency is max
    const int N = sizeof(pur_x_eff) / sizeof(int);
    int max_index = distance(pur_x_eff, max_element(pur_x_eff, pur_x_eff + N)); 
// Create dashed line at max value of pur x eff

    TLine* l = new TLine(W[max_index], 0, W[max_index], 1);
    l->SetLineColor(kBlack);
    l->SetLineStyle(10);
    l->SetLineWidth(2);
    mg->Draw("AC*");
    l->Draw("SAME");

//Print optimal values in legend as strings

    std::string textW = "Optimal Cut: W =  ";//print value of optimal W 
    textW += std::to_string(W[max_index]); 

    std::string textPur = "Purity (Optimal Purity: "; //print value of optimal purity
        textPur += std::to_string(Pur_new[max_index]);
        textPur += ")";

        std::string textPxE = "Purity x Efficiency (Optimal Value: ";//print value of optimal purity times efficiency
        textPxE += std::to_string(pur_x_eff[max_index]);
        textPxE += ")";

        std::string textEff = "Efficiency (Optimal Efficiency: ";//print value of optimal efficiency
        textEff += std::to_string(Eff_new[max_index]);
        textEff += ")";

// Create legend

    auto legend = new TLegend(0.65, 0.2, 0.9, 0.4); 
    legend->SetHeader("Legend", "C");
    legend->AddEntry(gr, Form(textPxE.c_str()), "l");
    legend->AddEntry(gr2, Form(textEff.c_str()), "l");
    legend->AddEntry(gr3, Form(textPur.c_str()), "l");
    legend->AddEntry(l, Form(textW.c_str()), "l");
    legend->Draw();

    c2->SaveAs("PurxEff_C12_2GeV_part2.png"); //Save canvas as png
}
