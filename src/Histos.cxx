#include "Histos.h"
#include "TMath.h"

Histos::Histos() {

    hImpactParameter = new TH1D("hImpactParameter", "hImpactParameter", 100, 0.0, 15.0);
    hNcoll = new TH1D("hNcoll", "hNcoll", 150, 0.0, 1500.0);
    hNpart = new TH1D("hNpart", "hNpart", 150, 0.0, 500.0);

}
