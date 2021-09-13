#include <iomanip>
#include <cmath>
#include "MassiveBody.h"
#include <iostream>

int MassiveBody::objCount = 0;
const double MassiveBody::SI_TIME_STEP = 0.1;
const double MassiveBody::SI_NEWTON_G = 6.674308e-11;

MassiveBody::MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos)
    : id(MassiveBody::objCount++), type(SolarSysBody::NONE), name(name), siMass(siMass), siRadius(siRadius)
{
    this->siPos = initSIPos;
    this->siVel = Vec2(0, 0);
}

MassiveBody::MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos, Vec2 initSIVel)
    : id(MassiveBody::objCount++), type(SolarSysBody::NONE), name(name), siMass(siMass), siRadius(siRadius)
{
    this->siPos = initSIPos;
    this->siVel = initSIVel;
}

MassiveBody::MassiveBody(SolarSysBody type, Vec2 initSIPos)
    : id(MassiveBody::objCount++), type(type), name(SOLAR_SYS_SI_NAMES.at(type)), siMass(SOLAR_SYS_SI_MASSES.at(type)), siRadius(SOLAR_SYS_SI_RADII.at(type))
{
    this->siPos = initSIPos;
}

MassiveBody::MassiveBody(SolarSysBody type, Vec2 initSIPos, Vec2 initSIVel)
    : id(MassiveBody::objCount++), type(type), name(SOLAR_SYS_SI_NAMES.at(type)), siMass(SOLAR_SYS_SI_MASSES.at(type)), siRadius(SOLAR_SYS_SI_RADII.at(type))
{
    this->siPos = initSIPos;
    this->siVel = initSIVel;
}

std::uint64_t MassiveBody::getID() const
{
    return this->id;
}

std::string MassiveBody::getName() const
{
    return this->name;
}

double MassiveBody::getSIMass() const
{
    return this->siMass;
}

double MassiveBody::getSIRadius() const
{
    return this->siRadius;
}

double MassiveBody::getRadius(double multiplier) const
{
    return this->siRadius * multiplier;
}

Vec2 MassiveBody::getSIPos() const
{
    return this->siPos;
}

Vec2 MassiveBody::getPos(double multiplier) const
{
    return this->siPos * multiplier;
}

Vec2 MassiveBody::getSIVel() const
{
    return this->siVel;
}

Vec2 MassiveBody::getSIVelOrbital(MassiveBody& mb, bool clockwise)
{
    Vec2 r = mb.getSIPos() - this->getSIPos();
    Vec2 dir = Vec2(r.y, -r.x).unit();

    if (!clockwise)
    {
        dir *= -1;
    }

    double speed = std::sqrt(this->getSIGravForce(mb).magn() * r.magn() / this->getSIMass());

    return speed * dir;
}

void MassiveBody::attemptOrbit(MassiveBody& mb, bool clockwise)
{
    this->siVel = this->getSIVelOrbital(mb, clockwise);
}

void MassiveBody::setSIVel(Vec2 siVel)
{
    this->siVel = siVel;
}

Vec2 MassiveBody::getSIGravForce(MassiveBody& mb)
{
    if (mb == *this)
    {
        return Vec2();
    }

    Vec2 r = this->getSIPos() - mb.getSIPos();
    Vec2 dir = r.unit();

    double scalarForce = (-MassiveBody::SI_NEWTON_G * this->getSIMass() * mb.getSIMass() / (r.magn() * r.magn()));

    return scalarForce * dir;
}

Vec2 MassiveBody::getSIGravForceNet(std::vector<std::reference_wrapper<MassiveBody>> mbs)
{
    Vec2 sum = Vec2();

    for (MassiveBody& mb : mbs)
    {
        sum += this->getSIGravForce(mb);
    }

    return sum;
}

Vec2 MassiveBody::getSIAccel(Vec2 siGravForceNet)
{
    return siGravForceNet / this->getSIMass();
}

void MassiveBody::budge(Vec2 siGravForceNet, double dt)
{
    this->siVel += this->getSIAccel(siGravForceNet) * dt;
    this->siPos += this->siVel * dt;
}

void MassiveBody::budgeInOrbit(MassiveBody& mb, bool clockwise, double dt)
{
    this->siVel = this->getSIVelOrbital(mb, clockwise);
    this->siPos += this->siVel * dt;
}

bool operator==(const MassiveBody& mb1, const MassiveBody& mb2)
{
    return mb1.getID() == mb2.getID();
}

bool operator!=(const MassiveBody& mb1, const MassiveBody& mb2)
{
    return mb1.getID() != mb2.getID();
}

std::ostream& operator<<(std::ostream& out, const MassiveBody& mb)
{
    out << "'" << mb.getName() << "'";
    out << "[ID: " << mb.getID() << ", ";
    out << "M: " << mb.getSIMass() << "kg, ";
    out << "R: " << mb.getSIRadius() << "m, " << std::endl;

    return out;
}
