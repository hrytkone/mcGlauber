#include "TH1D.h"

class JHistos {

public:
    JHistos();
    virtual ~JHistos(){;}

    TH1D *hImpactParameter;
    TH1D *hNcoll;
    TH1D *hNpart;

};
