#pragma once
#include <unordered_map>

// Bodies in the solar system with radii >400km
const enum class SolarSysBody
{
    NONE,
    SUN,
    JUPITER,
    SATURN,
    URANUS,
    NEPTUNE,
    EARTH,
    VENUS,
    MARS,
    GANYMEDE,
    TITAN,
    MERCURY,
    CALLISTO,
    IO,
    LUNA,
    EUROPA,
    TRITON,
    PLUTO,
    ERIS,
    TITANIA,
    HAUMEA,
    RHEA,
    OBERON,
    IAPETUS,
    MAKEMAKE,
    GONGGONG,
    CHARON,
    UMBRIEL,
    ARIEL,
    DIONE,
    QUAOAR,
    TETHYS,
    SEDNA,
    CERES,
    ORCUS,
    SALACIA,
};

static const std::unordered_map<SolarSysBody, double> SOLAR_SYS_SI_MASSES = {
    { SolarSysBody::SUN,      1989100000e21 },
    { SolarSysBody::JUPITER,  1898187e21 },
    { SolarSysBody::SATURN,   568317e21 },
    { SolarSysBody::URANUS,   86813e21 },
    { SolarSysBody::NEPTUNE,  102413e21 },
    { SolarSysBody::EARTH,    5972e21 },
    { SolarSysBody::VENUS,    4868e21 },
    { SolarSysBody::MARS,     642e21 },
    { SolarSysBody::GANYMEDE, 148e21 },
    { SolarSysBody::TITAN,    135e21 },
    { SolarSysBody::MERCURY,  330e21 },
    { SolarSysBody::CALLISTO, 108e21 },
    { SolarSysBody::IO,       89e21 },
    { SolarSysBody::LUNA,     73e21 },
    { SolarSysBody::EUROPA,   48e21 },
    { SolarSysBody::TRITON,   21e21 },
    { SolarSysBody::PLUTO,    13e21 },
    { SolarSysBody::ERIS,     17e21 },
    { SolarSysBody::TITANIA,  3.4e21 },
    { SolarSysBody::HAUMEA,   4.0e21 },
    { SolarSysBody::RHEA,     2.3e21 },
    { SolarSysBody::OBERON,   3.1e21 },
    { SolarSysBody::IAPETUS,  1.8e21 },
    { SolarSysBody::MAKEMAKE, 3.1e21 },
    { SolarSysBody::GONGGONG, 1.8e21 },
    { SolarSysBody::CHARON,   1.6e21 },
    { SolarSysBody::UMBRIEL,  1.3e21 },
    { SolarSysBody::ARIEL,    1.3e21 },
    { SolarSysBody::DIONE,    1.1e21 },
    { SolarSysBody::QUAOAR,   1.4e21 },
    { SolarSysBody::TETHYS,   0.62e21 },
    { SolarSysBody::SEDNA,    3.9e21 }, // From averaging out highest and lowest estimates
    { SolarSysBody::CERES,    0.94e21 },
    { SolarSysBody::ORCUS,    0.61e21 },
    { SolarSysBody::SALACIA,  0.49e21 },
};

static const std::unordered_map<SolarSysBody, double> SOLAR_SYS_SI_RADII = {
    { SolarSysBody::SUN,      695508e3 },
    { SolarSysBody::JUPITER,  69911e3 },
    { SolarSysBody::SATURN,   58232e3 },
    { SolarSysBody::URANUS,   25362e3 },
    { SolarSysBody::NEPTUNE,  24622e3 },
    { SolarSysBody::EARTH,    6371e3 },
    { SolarSysBody::VENUS,    6052e3 },
    { SolarSysBody::MARS,     3390e3 },
    { SolarSysBody::GANYMEDE, 2634e3 },
    { SolarSysBody::TITAN,    2575e3 },
    { SolarSysBody::MERCURY,  2439e3 },
    { SolarSysBody::CALLISTO, 2410e3 },
    { SolarSysBody::IO,       1822e3 },
    { SolarSysBody::LUNA,     1738e3 },
    { SolarSysBody::EUROPA,   1561e3 },
    { SolarSysBody::TRITON,   1353e3 },
    { SolarSysBody::PLUTO,    1188e3 },
    { SolarSysBody::ERIS,     1163e3 },
    { SolarSysBody::TITANIA,   788e3 },
    { SolarSysBody::HAUMEA,    780e3 },
    { SolarSysBody::RHEA,      764e3 },
    { SolarSysBody::OBERON,    761e3 },
    { SolarSysBody::IAPETUS,   735e3 },
    { SolarSysBody::MAKEMAKE,  715e3 },
    { SolarSysBody::GONGGONG,  615e3 },
    { SolarSysBody::CHARON,    606e3 },
    { SolarSysBody::UMBRIEL,   585e3 },
    { SolarSysBody::ARIEL,     579e3 },
    { SolarSysBody::DIONE,     561e3 },
    { SolarSysBody::QUAOAR,    561e3 },
    { SolarSysBody::TETHYS,    531e3 },
    { SolarSysBody::SEDNA,     498e3 },
    { SolarSysBody::CERES,     470e3 },
    { SolarSysBody::ORCUS,     458e3 },
    { SolarSysBody::SALACIA,   423e3 },
};

struct Colour
{
    uint8_t r, g, b;

    Colour()
    {
        this->r = this->g = this->b = 255;
    }

    Colour(uint8_t r, uint8_t g, uint8_t b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }
};

static const std::unordered_map<SolarSysBody, Colour> SOLAR_SYS_SI_COLOURS = {
    { SolarSysBody::SUN,      Colour(255, 120, 0) },
    { SolarSysBody::JUPITER,  Colour() },
    { SolarSysBody::SATURN,   Colour() },
    { SolarSysBody::URANUS,   Colour() },
    { SolarSysBody::NEPTUNE,  Colour() },
    { SolarSysBody::EARTH,    Colour(0, 150, 255) },
    { SolarSysBody::VENUS,    Colour() },
    { SolarSysBody::MARS,     Colour(255, 40, 0) },
    { SolarSysBody::GANYMEDE, Colour() },
    { SolarSysBody::TITAN,    Colour() },
    { SolarSysBody::MERCURY,  Colour() },
    { SolarSysBody::CALLISTO, Colour() },
    { SolarSysBody::IO,       Colour() },
    { SolarSysBody::LUNA,     Colour(110, 110, 110) },
    { SolarSysBody::EUROPA,   Colour() },
    { SolarSysBody::TRITON,   Colour() },
    { SolarSysBody::PLUTO,    Colour() },
    { SolarSysBody::ERIS,     Colour() },
    { SolarSysBody::TITANIA,  Colour() },
    { SolarSysBody::HAUMEA,   Colour() },
    { SolarSysBody::RHEA,     Colour() },
    { SolarSysBody::OBERON,   Colour() },
    { SolarSysBody::IAPETUS,  Colour() },
    { SolarSysBody::MAKEMAKE, Colour() },
    { SolarSysBody::GONGGONG, Colour() },
    { SolarSysBody::CHARON,   Colour() },
    { SolarSysBody::UMBRIEL,  Colour() },
    { SolarSysBody::ARIEL,    Colour() },
    { SolarSysBody::DIONE,    Colour() },
    { SolarSysBody::QUAOAR,   Colour() },
    { SolarSysBody::TETHYS,   Colour() },
    { SolarSysBody::SEDNA,    Colour() },
    { SolarSysBody::CERES,    Colour() },
    { SolarSysBody::ORCUS,    Colour() },
    { SolarSysBody::SALACIA,  Colour() },
};


static const std::unordered_map<SolarSysBody, std::string> SOLAR_SYS_SI_NAMES = {
    { SolarSysBody::SUN,      "Sun" },
    { SolarSysBody::JUPITER,  "Jupiter" },
    { SolarSysBody::SATURN,   "Saturn" },
    { SolarSysBody::URANUS,   "Uranus" },
    { SolarSysBody::NEPTUNE,  "Neptune" },
    { SolarSysBody::EARTH,    "Earth" },
    { SolarSysBody::VENUS,    "Venus" },
    { SolarSysBody::MARS,     "Mars" },
    { SolarSysBody::GANYMEDE, "Ganymede" },
    { SolarSysBody::TITAN,    "Titan" },
    { SolarSysBody::MERCURY,  "Mercury" },
    { SolarSysBody::CALLISTO, "Callisto" },
    { SolarSysBody::IO,       "Io" },
    { SolarSysBody::LUNA,     "Moon (Luna)" },
    { SolarSysBody::EUROPA,   "Europa" },
    { SolarSysBody::TRITON,   "Triton" },
    { SolarSysBody::PLUTO,    "Pluto" },
    { SolarSysBody::ERIS,     "Eris" },
    { SolarSysBody::TITANIA,  "Titania" },
    { SolarSysBody::HAUMEA,   "Haumea" },
    { SolarSysBody::RHEA,     "Rhea" },
    { SolarSysBody::OBERON,   "Oberon" },
    { SolarSysBody::IAPETUS,  "Iapetus" },
    { SolarSysBody::MAKEMAKE, "Makemake" },
    { SolarSysBody::GONGGONG, "Gonggong" },
    { SolarSysBody::CHARON,   "Charon" },
    { SolarSysBody::UMBRIEL,  "Umbriel" },
    { SolarSysBody::ARIEL,    "Ariel" },
    { SolarSysBody::DIONE,    "Dione" },
    { SolarSysBody::QUAOAR,   "Quaoar" },
    { SolarSysBody::TETHYS,   "Tethys" },
    { SolarSysBody::SEDNA,    "Sedna" },
    { SolarSysBody::CERES,    "Ceres" },
    { SolarSysBody::ORCUS,    "Orcus" },
    { SolarSysBody::SALACIA,  "Salacia" },
};