#include "MassSysIsolated.h"
#include <iostream>

MassSysIsolated::MassSysIsolated()
{
    this->mbs = {};
}

MassSysIsolated::MassSysIsolated(std::vector<std::reference_wrapper<MassiveBody>> mbs)
{
    this->mbs = mbs;
}

void MassSysIsolated::addBody(MassiveBody& mbToAdd)
{
    this->mbs.emplace_back(mbToAdd);
}

void MassSysIsolated::addBodies(std::vector<std::reference_wrapper<MassiveBody>> mbsToAdd)
{
    this->mbs.insert(std::end(this->mbs), std::begin(mbsToAdd), std::end(mbsToAdd));
}

void MassSysIsolated::budgeAll()
{
    for (MassiveBody& mb : this->mbs)
    {
        mb.budge(mb.getSIGravForceNet(this->mbs));
    }
}
