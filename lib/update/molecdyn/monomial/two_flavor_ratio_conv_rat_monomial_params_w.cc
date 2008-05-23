// $Id: two_flavor_ratio_conv_rat_monomial_params_w.cc,v 3.1 2008-05-23 21:31:34 edwards Exp $
/*! @file
 * @brief Two-flavor ratio of conventional fermion action monomial params
 */

#include "update/molecdyn/monomial/two_flavor_ratio_conv_rat_monomial_params_w.h"

namespace Chroma 
{ 
 
  // Read the parameters
  TwoFlavorRatioConvRatWilsonTypeFermMonomialParams::TwoFlavorRatioConvRatWilsonTypeFermMonomialParams(XMLReader& xml_in, const string& path)
  {
    // Get the top of the parameter XML tree
    XMLReader paramtop(xml_in, path);
    
    try 
    {
      // Read the inverter Parameters
      read(paramtop, "Numerator", numer);
      read(paramtop, "Denominator", denom);

      if( paramtop.count("./ChronologicalPredictor") == 0 ) 
      {
	predictor.xml = "";
      }
      else {
	predictor = readXMLGroup(paramtop, "ChronologicalPredictor", "Name");
      }
      
    }
    catch(const string& s) {
      QDPIO::cerr << "Caught Exception while reading parameters: " << s <<endl;
      QDP_abort(1);
    }
  }

  //! Read Parameters
  void read(XMLReader& xml, const std::string& path,
	    TwoFlavorRatioConvRatWilsonTypeFermMonomialParams& params) 
  {
    TwoFlavorRatioConvRatWilsonTypeFermMonomialParams tmp(xml, path);
    params = tmp;
  }

  //! Write Parameters
  void write(XMLWriter& xml, const std::string& path,
	     const TwoFlavorRatioConvRatWilsonTypeFermMonomialParams& params) 
  {
    write(xml, "Numerator", params.numer);
    write(xml, "Denominator", params.denom); 
    xml << params.predictor.xml;
  }
 
} //end namespace Chroma


