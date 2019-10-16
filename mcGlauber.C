#include <iostream>
#include <vector>
#include <stdlib.h>

#include "src/Histos.h"
#include "src/Nucleon.h"

#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TF1.h"
#include "TClonesArray.h"
#include "TRandom3.h"

#include "TStopwatch.h"

using namespace std;

const double pi = TMath::Pi();
const double minDist = 1.0; //fm

double WoodsSaxonDist(double *x, double *p);
double ImpactParameterDist(double *x, double *p);
double GetDistance(vector<double> a, vector<double> b);
void GetNucleonCoordinates(TClonesArray *nucleus, double nNucleons, TRandom3 *rand, Histos *histos, TF1 *fRadiusDist, double shift);
void GetCollisions(TClonesArray *nucleusA, TClonesArray *nucleusB, double inelasticCrossSection, int &Ncoll, int &Npart);

int main(int argc, char **pargv) {

    TString outFileName = "mcGlauber.root";
    TFile *fOut = TFile::Open(outFileName, "RECREATE");

    TStopwatch timer;
    timer.Start();

    TRandom3 *rand = new TRandom3();
    rand->SetSeed(0);

    TTree *t = new TTree("tNucleus", "Tree of nuclei");
    Histos *histos = new Histos();

    int nEvents = 1;
    if (argc>1)
        nEvents = atof(pargv[1]);

    // For Pb
    double w = 0;
    double R = 6.68;
    double a = 0.546;
    double Z = 82;
    double nNeutrons = 125;

    // Impact parameter
    double b = 0.0;

    TF1 *fRadiusDist = new TF1("fRadiusDist", WoodsSaxonDist, 0.0, 10.0, 3);
    fRadiusDist->SetParameters(w, R, a);

    TF1 *fImpactParamDist = new TF1("fImpactParamDist", ImpactParameterDist, 0.0, 15.0, 1);
    fImpactParamDist->SetParameter(0, 10.0);

    double nNucleons = Z + nNeutrons;
    TClonesArray *nucleusA = new TClonesArray("Nucleon", 100);
    TClonesArray *nucleusB = new TClonesArray("Nucleon", 100);

    t->Branch("nucleusA", "TClonesArray", &nucleusA);
    t->Branch("nucleusB", "TClonesArray", &nucleusB);

    // energy = 10 GeV
    double inelasticCrossSection = 3.0;
    int Npart, Ncoll;

    int nOutput = nEvents/20;
    for (int i=0; i<nEvents; i++){

        if (i % nOutput == 0)
            cout << 100*i/nEvents << " % finished" << endl;

        Npart = 0;
        Ncoll = 0;

        b = fImpactParamDist->GetRandom();
        histos->hImpactParameter->Fill(b);

        nucleusA->Clear();
        nucleusB->Clear();

        GetNucleonCoordinates(nucleusA, nNucleons, rand, histos, fRadiusDist, b/2);
        GetNucleonCoordinates(nucleusB, nNucleons, rand, histos, fRadiusDist, -b/2);

        GetCollisions(nucleusA, nucleusB, inelasticCrossSection, Ncoll, Npart);

        histos->hNcoll->Fill(Ncoll);
        histos->hNpart->Fill(Npart);

        t->Fill();
    }

    fOut->Write();
    timer.Print();

    return 0;
}

void GetNucleonCoordinates(TClonesArray *nucleus, double nNucleons, TRandom3 *rand, Histos *histos, TF1 *fRadiusDist, double shift) {
    double r, azimuthal, polar;
    double x, y, z;
    Nucleon nucleon;

    int vecSize = 0;
    vector<vector<double>> XYZ;
    vector<double> vecTemp = {0, 0, 0};

    int i=0;
    bool bAcceptNucleon = true;
    while (i < nNucleons) {

        r = fRadiusDist->GetRandom();
        azimuthal = rand->Uniform(0, 2*pi);
        polar = rand->Uniform(0, pi);

        nucleon.SetRadius(r);
        nucleon.SetAzimuthalAngle(azimuthal);
        nucleon.SetPolarAngle(polar);

        if (i==0) {
            x = nucleon.GetX()+shift;
            y = nucleon.GetY();
            z = nucleon.GetZ();

            vecTemp = {x, y, z};
            XYZ.push_back(vecTemp);

            nucleon.SetCoordinatesFromCartesian(x, y, z);
            new((*nucleus)[i]) Nucleon(nucleon);
            i++;
        } else {
            x = nucleon.GetX() + shift;
            y = nucleon.GetY();
            z = nucleon.GetZ();

            vecTemp = {x, y, z};

            bAcceptNucleon = true;
            vecSize = XYZ.size();
            for (int j=0; j<vecSize; j++) {
                if (GetDistance(vecTemp, XYZ[j]) < minDist) {
                    bAcceptNucleon = false;
                    break;
                }
            }

            if (bAcceptNucleon) {
                XYZ.push_back(vecTemp);

                nucleon.SetCoordinatesFromCartesian(x, y, z);
                new((*nucleus)[i]) Nucleon(nucleon);
                i++;
            }

        }
    }
}

void GetCollisions(TClonesArray *nucleusA, TClonesArray *nucleusB, double inelasticCrossSection, int &Ncoll, int &Npart) {

    double interactionRadius = TMath::Sqrt(inelasticCrossSection/pi);

    Nucleon *nucleonA;
    Nucleon *nucleonB;

    vector<double> A = {0, 0};
    vector<double> B = {0, 0};

    int i, j;
    int nucleonsAmax = nucleusA->GetEntriesFast();
    int nucleonsBmax = nucleusB->GetEntriesFast();
    for (i=0; i<nucleonsAmax; i++) {
        nucleonA = (Nucleon*)nucleusA->At(i);
        A = {nucleonA->GetX(), nucleonA->GetY()};

        for (j=0; j<nucleonsBmax; j++) {
            nucleonB = (Nucleon*)nucleusB->At(j);
            B = {nucleonB->GetX(), nucleonB->GetY()};

            if (GetDistance(A, B) < interactionRadius) {
                Ncoll++;

                if (!nucleonA->IsParticipant()) {
                    nucleonA->SetAsParticipant();
                    Npart++;
                }

                if (!nucleonB->IsParticipant()) {
                    nucleonB->SetAsParticipant();
                    Npart++;
                }

            }
        }
    }
}

double WoodsSaxonDist(double *x, double *p) {
    double r = x[0];
    double w = p[0];
    double R = p[1];
    double a = p[2];
    return (1+(w*r*r)/(R*R))/(1+TMath::Exp((r-R)/a));
}

double ImpactParameterDist(double *x, double *p) {
    double b = x[0];
    double radius = p[0];
    return b < radius ? 2*pi*b : 0;
}

double GetDistance(vector<double> a, vector<double> b) {
    int length = a.size();
    double sum = 0;
    for (int i=0; i<length; i++) {
        sum += (a[i]-b[i])*(a[i]-b[i]);
    }
    return TMath::Sqrt(sum);
}
