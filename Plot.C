#include <iostream>

#include <TMath.h>
#include <TFile.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TEllipse.h>

#include "Nucleon.h"

// Cross sections: http://pdg.lbl.gov/2018/reviews/rpp2018-rev-cross-section-plots.pdf

using namespace std;

void Plot(double b = 7.0, TString sFileName = "mcGlauber.root") {

    TFile *fIn = TFile::Open(sFileName);

    if (!fIn) return;
    fIn->ls();

    TTree *t = (TTree*)fIn->Get("tNucleus");

    TClonesArray *nucleusA = new TClonesArray("Nucleon", 100);
    TClonesArray *nucleusB = new TClonesArray("Nucleon", 100);
    t->SetBranchAddress("nucleusA", &nucleusA);
    t->SetBranchAddress("nucleusB", &nucleusB);

    Nucleon *nucleon;

    TCanvas *c1 = new TCanvas("c1");
    c1->Range(-11, -11, 21, 21);
    gPad->DrawFrame(-15, -10, 15, 10);

    double x, y, z, nucleonR = 0.86;
    double nucleusR = 6.68;
    int nNucleons;
    //int nEvents = t->GetEntriesFast();
    int nEvents = 1; // Draw only one event
    for (int i=0; i<nEvents; i++) {
        //t->Show(i);
        t->GetEntry(i);
        nNucleons = nucleusA->GetEntriesFast();
        for (int j=0; j<nNucleons; j++) {
            nucleon = (Nucleon*)nucleusA->At(j);
            x = nucleon->GetX();
            y = nucleon->GetY();
            TEllipse *circle = new TEllipse(x, y, nucleonR, nucleonR);

            if (nucleon->IsParticipant()) {
                circle->SetFillColorAlpha(9, 0.5);
            } else {
                circle->SetFillColorAlpha(9, 0.1);
                circle->SetLineStyle(2);
            }

            circle->Draw();
        }

        nNucleons = nucleusB->GetEntriesFast();
        for (int j=0; j<nNucleons; j++) {
            nucleon = (Nucleon*)nucleusB->At(j);
            x = nucleon->GetX();
            y = nucleon->GetY();
            TEllipse *circle = new TEllipse(x, y, nucleonR, nucleonR);

            if (nucleon->IsParticipant()) {
                circle->SetFillColorAlpha(46, 0.5);
            } else {
                circle->SetFillColorAlpha(46, 0.1);
                circle->SetLineStyle(2);
            }

            circle->Draw();
        }

        TEllipse *pbA = new TEllipse(-b/2, 0, nucleusR, nucleusR);
        pbA->SetFillColorAlpha(46, 0.0);
        pbA->SetLineStyle(2);
        pbA->Draw();

        TEllipse *pbB = new TEllipse(b/2, 0, nucleusR, nucleusR);
        pbB->SetFillColorAlpha(46, 0.0);
        pbB->SetLineStyle(2);
        pbB->Draw();
    }

    TCanvas *c2 = new TCanvas("c2");
    c2->Range(-25, -10, 25, 10);
    gPad->DrawFrame(-30, -15, 30, 15);

    for (int i=0; i<nEvents; i++) {
        //t->Show(i);
        t->GetEntry(i);
        nNucleons = nucleusA->GetEntriesFast();
        for (int j=0; j<nNucleons; j++) {
            nucleon = (Nucleon*)nucleusA->At(j);
            x = nucleon->GetX();
            z = nucleon->GetZ();
            TEllipse *circle = new TEllipse(z-15.0, x, nucleonR, nucleonR);

            if (nucleon->IsParticipant()) {
                circle->SetFillColorAlpha(9, 0.5);
            } else {
                circle->SetFillColorAlpha(9, 0.1);
                circle->SetLineStyle(2);
            }

            circle->Draw();
        }

        nNucleons = nucleusB->GetEntriesFast();
        for (int j=0; j<nNucleons; j++) {
            nucleon = (Nucleon*)nucleusB->At(j);
            x = nucleon->GetX();
            z = nucleon->GetZ();
            TEllipse *circle = new TEllipse(z+15.0, x, nucleonR, nucleonR);

            if (nucleon->IsParticipant()) {
                circle->SetFillColorAlpha(46, 0.5);
            } else {
                circle->SetFillColorAlpha(46, 0.1);
                circle->SetLineStyle(2);
            }

            circle->Draw();
        }
    }
}
