#include "WireCellUtil/Pimpos.h"

#include <iostream>             // debugging

using namespace WireCell;

Pimpos::Pimpos(int nwires, double minwirepitch, double maxwirepitch,
               const Vector& wire,
               const Vector& pitch,
               const Point& origin,
	       int nbins)
    : m_nimpbins_per_wire(nbins)
    , m_origin(origin)
    , m_axis{Vector(0,0,0), wire.norm(), pitch.norm()}
{
    // drift = wire X pitch = Y cross Z
    m_axis[0] = m_axis[1].cross(m_axis[2]);

    // binnings:
    const int nimpstot = nwires*nbins;
    const double regionsize = (maxwirepitch-minwirepitch)/(nwires-1);
    const double impsize = regionsize/nbins;
    const double pmin = minwirepitch - 0.5*nbins*impsize;
    const double pmax = maxwirepitch + 0.5*nbins*impsize;
    m_regionbins.set(nwires, pmin, pmax);
    m_impactbins.set(nimpstot, pmin, pmax);
}

std::pair<int, int> Pimpos::closest(double pitch) const
{
    const int iwire = m_regionbins.bin(pitch);
    const double wire_pitch = m_regionbins.center(iwire);
    const double remainder = pitch - wire_pitch;
    const int irelimp = int(round(remainder/m_impactbins.binsize()));
    return std::make_pair(iwire, irelimp);
}

int Pimpos::wire_impact(int wireind) const
{
    return (0.5 + wireind)*m_nimpbins_per_wire;
}

std::pair<int,int> Pimpos::wire_impacts(int wireind) const
{
    const int wi = wire_impact(wireind);
    return std::make_pair(wi - m_nimpbins_per_wire/2,
                          wi + m_nimpbins_per_wire/2);
}


int Pimpos::reflect(int wireind, int impind) const
{
    const int wireimpind = wire_impact(wireind);
    const int delta = impind - wireimpind;
    return wireimpind - delta;
}




Vector Pimpos::relative(const Point& pt) const
{
    return pt - m_origin;
}

double Pimpos::distance(const Point& pt, int axis) const
{
    return m_axis[axis].dot(relative(pt));
}

Point Pimpos::transform(const Point& pt) const
{
    const Vector v = relative(pt);
    Point ret;
    for (int axis=0; axis<3; ++axis) {
	ret[axis] = m_axis[axis].dot(v);
    }
    return ret;
}


// Local Variables:
// mode: c++
// c-basic-offset: 4
// End: