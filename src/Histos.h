#include "TH1D.h"

class Histos {

public:
    Histos();
    virtual ~Histos(){;}

    TH1D *hImpactParameter;
    TH1D *hNcoll;
    TH1D *hNpart;

};
