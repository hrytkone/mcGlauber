#ifndef _JNUCLEON_H_
#define _JNUCLEON_H_

#include "TObject.h"

class JNucleon : public TObject {

private:
    double r;
    double azimuthalAngle;
    double polarAngle;
    bool bIsParticipant;

public:
    JNucleon();
    JNucleon(double, double, double);

    virtual ~JNucleon() {;}

    double GetRadius() {return r;}
    double GetAzimuthalAngle() {return azimuthalAngle;}
    double GetPolarAngle() {return polarAngle;}
    bool IsParticipant() {return bIsParticipant;}

    double GetX();
    double GetY();
    double GetZ();

    void SetRadius(double setR) {r = setR;}
    void SetAzimuthalAngle(double setAzim) {azimuthalAngle = setAzim;}
    void SetPolarAngle(double setPolar) {polarAngle = setPolar;}
    void SetAsParticipant() {bIsParticipant = true;}

    void SetCoordinatesFromCartesian(double x, double y, double z);

protected:
    ClassDef(JNucleon, 1)

};
#endif
