#pragma once
#include <vector>
#include "MassiveBody.h"

class MassSysIsolated
{
public:
    std::vector<std::reference_wrapper<MassiveBody>> mbs;

    MassSysIsolated();
    MassSysIsolated(std::vector<std::reference_wrapper<MassiveBody>> mbs);

    void addBody(MassiveBody& mbToAdd);
    void addBodies(std::vector<std::reference_wrapper<MassiveBody>> mbsToAdd);

    void budgeAll();
};