// -*- C++ -*-
// $Id: gauge_monomial.h,v 3.4 2007-03-22 17:39:23 bjoo Exp $
/*! \file
 *  \brief Generic gauge action monomial wrapper
 */

#ifndef __gaugeact_monomial_h__
#define __gaugeact_monomial_h__

#include "chromabase.h"

#include "update/molecdyn/field_state.h"
#include "update/molecdyn/monomial/abs_monomial.h"
#include "update/molecdyn/monomial/force_monitors.h"
#include "io/xmllog_io.h"
#include "util/gauge/taproj.h"


#include "tower_array.h"
#include "poisson.h"

namespace Chroma 
{
  /*! @ingroup monomial */
  namespace GaugeMonomialEnv 
  {
    extern const string name;
    bool registerAll();
  }


  // Parameter structure
  /*! @ingroup monomial */
  struct GaugeMonomialParams 
  {
    // Base Constructor
    GaugeMonomialParams();

    // Read monomial from some root path
    GaugeMonomialParams(XMLReader& in, const std::string& path);
    string gauge_act;
  };


  //! Wrapper class for  gauge monomials
  /*! @ingroup monomial
   *
   * Monomial is expected to be the same for these fermacts
   */
  class GaugeMonomial :
    public ExactMonomial< multi1d<LatticeColorMatrix>,
                          multi1d<LatticeColorMatrix> >    
  {
  public: 
    typedef multi1d<LatticeColorMatrix>  P;
    typedef multi1d<LatticeColorMatrix>  Q;
    typedef PQTraits<Q>::Base_t U;

    //! Construct out of a parameter struct. Check against the desired GaugeAct name
    GaugeMonomial(const GaugeMonomialParams& param_);

    //! Copy Constructor
    GaugeMonomial(const GaugeMonomial& m) : gaugeact((m.gaugeact)) {}

    //! Create a suitable state and compute F
    void dsdq(P& F, const AbsFieldState<P,Q>& s) 
    {
      START_CODE();

      XMLWriter& xml_out = TheXMLLogWriter::Instance();
      push(xml_out, "GaugeMonomial");

      // Make a gauge connect state
      Handle< GaugeState<P,Q> > g_state(getGaugeAct().createState(s.getQ()));

      TowerArray<LatticeColorMatrix> u_in(1);
      TowerArray<LatticeColorMatrix> u_out(1);

      for(int mu=0; mu < Nd; mu++) { 
	(u_in[mu])[0] = (g_state->getLinks())[mu];
      }

      getGaugeAct().deriv(u_out, u_in);

      for(int mu=0; mu < Nd; mu++){ 
	F[mu] = u_out[mu][0];
      }
      // Try to compute gauge action with the tower deriv as a hack.


      // monitorForces(xml_out, "Forces", F);
      pop(xml_out);

      END_CODE();
    }

    //! Create a suitable state and compute F
    //! On entry the height of u_out[0] is the desired tower height.
    void dsdq(TowerArray<U>& u_out, const AbsFieldState<P,Q>& s) 
    {
      START_CODE();

      XMLWriter& xml_out = TheXMLLogWriter::Instance();
      push(xml_out, "GaugeMonomial");

      // Make a gauge connect state
      Handle< GaugeState<P,Q> > g_state(getGaugeAct().createState(s.getQ()));

      // get height of tower:
      int N = u_out.getHeight();

      // Tower to start with.
      TowerArray<U> u_in(N);


      for(int mu=0; mu < Nd; mu++) { 
	// Always to this
	(u_in[mu])[0] = (g_state->getLinks())[mu];
      }

      if( N > 1) {
	// Lift U-s
	for(int i=1; i < N; i++) {  
	  for(int mu=0; mu < Nd; mu++) {
	    u_in[mu][i] = -s.getP()[mu]*u_in[mu][i-1];
	  }
	}
      }

      // Do the terivative with towers
      // FIXME: How to avoid extra work, and not redo level 0 here ?
      getGaugeAct().deriv(u_out, u_in);

      // Taproj it
      for(int i=0; i < N; i++) { 
	for(int mu=0; mu < Nd; mu++) { 
	  taproj(u_out[mu][i]);
	}
      }

      // and done

      pop(xml_out);

      END_CODE();
    }

    void computePBVectorField(TowerArray<U>& F,
			      TowerArray<U>& G, 
			      const AbsFieldState<P,Q>& s)
    {
      for(int mu=0; mu < Nd; mu++) { 
	F[mu] = zero;
	G[mu] = zero;
      }

      dsdq(F,s);

      Handle< AbsFieldState<P,Q> > s_new(s.clone());

      for(int mu=0; mu < Nd; mu++) { 
	s_new->getP()[mu] = F[mu][0];
      }

      dsdq(G, (*s_new));

     

    }


    //! Gauge action value
    Double S(const AbsFieldState<P,Q>& s)  
    {
      START_CODE();

      XMLWriter& xml_out = TheXMLLogWriter::Instance();
      push(xml_out, "GaugeMonomial");

      Handle< GaugeState<P,Q> > g_state(getGaugeAct().createState(s.getQ()));
      Double action = getGaugeAct().S(g_state);

      write(xml_out, "S", action);
      pop(xml_out);

      END_CODE();

      return action;
    }
	
	
    void refreshInternalFields(const AbsFieldState<P,Q>& s) 
    {
      //No internal fields to refresh => Nop
    }

    void setInternalFields(const Monomial<P,Q>& m) 
    {
      // No internal fields to refresh => Nop
    }

    protected:
      const GaugeAction<P,Q>& getGaugeAct(void) const { 
	return *gaugeact;
      }

    private:
      // Hide empty constructor and =
      GaugeMonomial();
      void operator=(const GaugeMonomial&);

    private:
      // A handle for the gaugeact
      Handle< GaugeAction<P,Q> > gaugeact;
    };


}; //end namespace chroma

#endif
