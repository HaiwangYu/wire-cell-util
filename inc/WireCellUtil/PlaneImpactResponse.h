#ifndef WIRECELLGEN_PLANEIMPACTRESPONSE
#define WIRECELLGEN_PLANEIMPACTRESPONSE

#include "WireCellUtil/Response.h"
#include "WireCellUtil/Waveform.h"
#include "WireCellUtil/Units.h"

namespace WireCell {

    /** The information about detector response at a particular impact
     * position (discrete position along the pitch direction of a
     * plane on which a response function is defined).  Note,
     * different physical positions may share the same ImpactResponse.
     */
    class ImpactResponse {
        int m_impact;
	Waveform::compseq_t m_spectrum;

    public:
	ImpactResponse(int impact, Waveform::compseq_t spec)
            : m_impact(impact), m_spectrum(spec) {}

	/// Frequency-domain spectrum of response
	const Waveform::compseq_t& spectrum() const { return m_spectrum; }

        /// Corresponding impact number
        int impact() const { return m_impact; }

    };

    /** Collection of all impact responses for a plane */
    class PlaneImpactResponse {
    public:

	PlaneImpactResponse(const Response::Schema::FieldResponse& fr, int plane_number,
                            Binning tbins,
			    double gain=0.0, double shaping=0.0*units::us);
	~PlaneImpactResponse();

	std::pair<int,int> closest_wire_impact(double relpitch) const;

	/// Return the response at the impact position closest to
	/// the given relative pitch location (measured relative
	/// to the wire of interest).
	const ImpactResponse* closest(double relpitch) const;

        /// Return the ImpactResponse associated with the absolute
        /// impact index.  See bywire_map() for a convenient way to
        /// get these indices.
	const ImpactResponse* impact_response(int ind) const { return m_ir[ind]; }

	/// Return the two responses which are associated with the
	/// impact positions on either side of the given pitch
	/// location (measured relative to the wire of interest).
	typedef std::pair<const ImpactResponse*,const ImpactResponse*> TwoImpactResponses;
	TwoImpactResponses bounded(double relpitch) const;

	typedef std::vector<int> region_indices_t;
	typedef std::vector<region_indices_t> wire_region_indicies_t;

	const wire_region_indicies_t& bywire_map() const { return m_bywire; }

	const Response::Schema::FieldResponse& field_response() const { return m_fr; }
	const Response::Schema::PlaneResponse& plane_response() const { return m_fr.planes[m_plane_number]; }

	double pitch_range() const { return 2.0*m_half_extent; }
	double pitch() const { return m_pitch; }
	double impact() const { return m_impact; }

	int nwires() const { return m_bywire.size(); }
	int nimp_per_wire() const { return m_bywire[0].size(); }

        Binning tbins() const { return m_tbins; }

    private:
	const Response::Schema::FieldResponse& m_fr;
	int m_plane_number;
        Binning m_tbins;

	wire_region_indicies_t m_bywire;

	std::vector<ImpactResponse*> m_ir;
	double m_half_extent, m_pitch, m_impact;
    };

}

#endif
