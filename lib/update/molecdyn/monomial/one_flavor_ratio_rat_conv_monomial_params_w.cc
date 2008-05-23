// $Id: one_flavor_ratio_rat_conv_monomial_params_w.cc,v 3.1 2008-05-23 21:31:33 edwards Exp $
/*! @file
 * @brief One-flavor monomial params
 */

#include "update/molecdyn/monomial/one_flavor_ratio_rat_conv_monomial_params_w.h"

namespace Chroma 
{ 

  // Read the parameters
  OneFlavorWilsonTypeFermRatioRatConvMonomialParams::OneFlavorWilsonTypeFermRatioRatConvMonomialParams(XMLReader& xml_in, const string& path)
  {
    // Get the top of the parameter XML tree
    XMLReader paramtop(xml_in, path);
    
    try 
    {
      read(paramtop, "num_pf", num_pf);
      read(paramtop, "Numerator", numer);
      read(paramtop, "Denominator", denom);
    }
    catch(const string& s) 
    {
      QDPIO::cerr << "Caught Exception while reading parameters: " << s <<endl;
      QDP_abort(1);
    }
  }

  //! Read Parameters
  void read(XMLReader& xml, const std::string& path,
	    OneFlavorWilsonTypeFermRatioRatConvMonomialParams& params) 
  {
    OneFlavorWilsonTypeFermRatioRatConvMonomialParams tmp(xml, path);
    params = tmp;
  }


  //! Write Parameters
  void write(XMLWriter& xml, const std::string& path,
	     const OneFlavorWilsonTypeFermRatioRatConvMonomialParams& params) 
  {
    push(xml, path);

    write(xml, "num_pf", params.num_pf);
    write(xml, "Numerator", params.numer);
    write(xml, "Denominator", params.denom); 

    pop(xml);
  }
  
} //end namespace Chroma


