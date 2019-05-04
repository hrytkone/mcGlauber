#include "JNucleon.h"
#include "TMath.h"

ClassImp(JNucleon)

JNucleon::JNucleon() {
    r = 1.0;
    azimuthalAngle = 1.0;
    polarAngle = 1.0;
    bIsParticipant = false;
}

JNucleon::JNucleon(double radius, double azimuthal, double polar) {
    r = radius;
    azimuthalAngle = azimuthal;
    polarAngle = polar;
    bIsParticipant = false;
}

double JNucleon::GetX() {
    return r*TMath::Sin(polarAngle)*TMath::Cos(azimuthalAngle);
}

double JNucleon::GetY() {
    return r*TMath::Sin(polarAngle)*TMath::Sin(azimuthalAngle);
}

double JNucleon::GetZ() {
    return r*TMath::Cos(polarAngle);
}

void JNucleon::SetCoordinatesFromCartesian(double x, double y, double z) {
    r = TMath::Sqrt(x*x + y*y + z*z);
    if (x>0) azimuthalAngle = TMath::ATan(y/x);
    if (x<0 && y>=0) azimuthalAngle = TMath::ATan(y/x) + TMath::Pi();
    if (x<0 && y<0) azimuthalAngle = TMath::ATan(y/x) - TMath::Pi();
    if (x==0 && y>0) azimuthalAngle = TMath::Pi()/2.0;
    if (x==0 && y<0) azimuthalAngle = -TMath::Pi()/2.0;
    polarAngle = TMath::ACos(z/r);
}
