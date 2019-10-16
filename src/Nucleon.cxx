#include "Nucleon.h"
#include "TMath.h"

ClassImp(Nucleon)

Nucleon::Nucleon() {
    r = 1.0;
    azimuthalAngle = 1.0;
    polarAngle = 1.0;
    bIsParticipant = false;
}

Nucleon::Nucleon(double radius, double azimuthal, double polar) {
    r = radius;
    azimuthalAngle = azimuthal;
    polarAngle = polar;
    bIsParticipant = false;
}

double Nucleon::GetX() {
    return r*TMath::Sin(polarAngle)*TMath::Cos(azimuthalAngle);
}

double Nucleon::GetY() {
    return r*TMath::Sin(polarAngle)*TMath::Sin(azimuthalAngle);
}

double Nucleon::GetZ() {
    return r*TMath::Cos(polarAngle);
}

void Nucleon::SetCoordinatesFromCartesian(double x, double y, double z) {
    r = TMath::Sqrt(x*x + y*y + z*z);
    if (x>0) azimuthalAngle = TMath::ATan(y/x);
    if (x<0 && y>=0) azimuthalAngle = TMath::ATan(y/x) + TMath::Pi();
    if (x<0 && y<0) azimuthalAngle = TMath::ATan(y/x) - TMath::Pi();
    if (x==0 && y>0) azimuthalAngle = TMath::Pi()/2.0;
    if (x==0 && y<0) azimuthalAngle = -TMath::Pi()/2.0;
    polarAngle = TMath::ACos(z/r);
}
