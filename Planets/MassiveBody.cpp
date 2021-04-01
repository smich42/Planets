#include <iomanip>
#include <cmath>
#include "MassiveBody.h"

int MassiveBody::objCount = 0;

MassiveBody::MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos)
    : id(MassiveBody::objCount++), name(name), siMass(siMass), siRadius(siRadius)
{
    this->siPos = initSIPos;
    this->siVel = Vec2(0, 0);
}

MassiveBody::MassiveBody(std::string name, double siMass, double siRadius, Vec2 initSIPos, Vec2 initSIVel)
    : id(MassiveBody::objCount++), name(name), siMass(siMass), siRadius(siRadius)
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

Vec2 MassiveBody::getSIPos() const
{
    return this->siPos;
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

Vec2 MassiveBody::getSIGravForce(MassiveBody& mb)
{
    if (mb == *this)
    {
        return Vec2();
    }

    Vec2 r = this->getSIPos() - mb.getSIPos();
    Vec2 dir = r.unit();

    double scalarForce = (-SI_NEWTON_G * this->getSIMass() * mb.getSIMass() / (r.magn() * r.magn()));

    return scalarForce * dir;
}

Vec2 MassiveBody::getSIGravForceNet(std::vector<std::reference_wrapper<MassiveBody>> mbs)
{
    Vec2 sum = Vec2();

    for (MassiveBody& mb : mbs)
    {
        sum += mb.getSIGravForce(*this);
    }

    return sum;
}

Vec2 MassiveBody::getSIAccel(Vec2 siGravForceNet)
{
    return siGravForceNet / this->getSIMass();
}

void MassiveBody::move(Vec2 siGravForceNet, double dt)
{
    this->siVel += this->getSIAccel(siGravForceNet) * dt;
    this->siPos += this->siVel * dt;
}

void MassiveBody::orbit(MassiveBody& mb, bool clockwise, double dt)
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
    out << "'" << mb.getName() << "'" << " [Object ID: " << mb.getID() << "]" << std::endl;

    out << "  " << std::setw(10) << std::left << "Mass: " << mb.getSIMass() << " kg" << std::endl;
    out << "  " << std::setw(10) << std::left << "Radius: " << mb.getSIRadius() << " m" << std::endl;
    out << "  " << std::setw(10) << std::left << "Position: " << mb.getSIPos() << std::endl;

    return out;
}
