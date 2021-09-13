#pragma once
#include <string>
#include <vector>
#include "Vec2.h"
#include "SolarSysData.h"

class MassiveBody
{
private:
    static const double SI_TIME_STEP;
    static const double SI_NEWTON_G;

    static int objCount;
    const uint64_t id;

    const std::string name;

    const double siMass;
    const double siRadius; // Radius ONLY for displaying; bodies behave like point masses

    Vec2 siPos;
    Vec2 siVel;

public:
    const SolarSysBody type;

    MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos);
    MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos, Vec2 initSIVel);
    MassiveBody(SolarSysBody type, Vec2 initSIPos);
    MassiveBody(SolarSysBody type, Vec2 initSIPos, Vec2 initSIVel);

    std::uint64_t getID() const;
    std::string getName() const;

    double getSIMass() const;

    double getSIRadius() const;
    double getRadius(double multiplier) const;

    Vec2 getSIPos() const;
    Vec2 getPos(double multiplier) const;

    void setSIVel(Vec2 siVel);
    Vec2 getSIVel() const;
    Vec2 getSIVelOrbital(MassiveBody& mb, bool clockwise);

    void attemptOrbit(MassiveBody& mb, bool clockwise);

    Vec2 getSIGravForce(MassiveBody& mb);
    Vec2 getSIGravForceNet(std::vector<std::reference_wrapper<MassiveBody>> mbs);
    Vec2 getSIAccel(Vec2 siGravForceNet);

    void budge(Vec2 siGravForceNet, double dt);
    void budgeInOrbit(MassiveBody& mb, bool clockwise, double dt);
};

bool operator==(const MassiveBody&, const MassiveBody&);
bool operator!=(const MassiveBody&, const MassiveBody&);

std::ostream& operator<<(std::ostream&, const MassiveBody&);