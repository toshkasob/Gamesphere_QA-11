//  Copyright Anton Sobenin 2019.

//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_STATS_GENERALIZED_PARETO_HPP
#define BOOST_STATS_GENERALIZED_PARETO_HPP

// http://en.wikipedia.org/wiki/Generalized_Pareto_distribution
// change other references next \/
// http://www.itl.nist.gov/div898/handbook/eda/section3/eda3661.htm
// Also:
// Weisstein, Eric W. "Pareto Distribution."
// From MathWorld--A Wolfram Web Resource.
// http://mathworld.wolfram.com/ParetoDistribution.html
// Handbook of Statistical Distributions with Applications, K Krishnamoorthy, ISBN 1-58488-635-8, Chapter 23, pp 257 - 267.
// Caution KK's a and b are the reverse of Mathworld!
// change other references below /\

#include <boost/math/distributions/fwd.hpp>
#include <boost/math/distributions/complement.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>
#include <boost/math/special_functions/powm1.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/special_functions/log1p.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/tools/precision.hpp>

#include <utility> // for BOOST_CURRENT_VALUE?

namespace boost
{
  namespace math
  {
    namespace detail
    { // Parameter checking.

      template <class RealType, class Policy>
      inline bool check_generalized_pareto_location(
        const char* function,
        RealType location,
        RealType* result, const Policy& pol)
      {
        if((boost::math::isfinite)(location))
        { // any finite value is OK.
			return true;
        }
        else
        { // Not finite.
          *result = policies::raise_domain_error<RealType>(
            function,
			"Location parameter is %1%, but must be finite!", location, pol);
          return false;
        }
      } // bool check_generalized_pareto_location

      template <class RealType, class Policy>
      inline bool check_generalized_pareto_scale(
        const char* function,
        RealType scale,
        RealType* result, const Policy& pol)
      {
        if((boost::math::isfinite)(scale))
        { // any > 0 finite value is OK.
          if (scale > 0)
          {
            return true;
          }
          else
          {
            *result = policies::raise_domain_error<RealType>(
              function,
              "Scale parameter is %1%, but must be > 0!", scale, pol);
            return false;
          }
        }
        else
        { // Not finite.
          *result = policies::raise_domain_error<RealType>(
            function,
            "Scale parameter is %1%, but must be finite!", scale, pol);
          return false;
        }
      } // bool check_generalized_pareto_scale

      template <class RealType, class Policy>
      inline bool check_generalized_pareto_shape(
        const char* function,
        RealType shape,
        RealType* result, const Policy& pol)
      {
        if((boost::math::isfinite)(shape))
        { // Any finite value is OK.
          return true;
        }
        else
        { // Not finite.
          *result = policies::raise_domain_error<RealType>(
            function,
            "Shape parameter is %1%, but must be finite!", shape, pol);
          return false;
        }
      } // bool check_generalized_pareto_shape(

      template <class RealType, class Policy>
      inline bool check_generalized_pareto_x(
        const char* function,
        RealType const& x,
        RealType* result, const Policy& pol)
      {
        if((boost::math::isfinite)(x))
        { // Any finite value is OK.
          return true;
        }
        else
        { // Not finite..
          *result = policies::raise_domain_error<RealType>(
            function,
            "x parameter is %1%, but must be finite!", x, pol);
          return false;
        }
      } // bool check_generalized_pareto_x

      template <class RealType, class Policy>
      inline bool check_generalized_pareto( // check distribution parameters.
        const char* function,
        RealType scale,
        RealType shape,
		RealType location,
        RealType* result, const Policy& pol)
      {
        return check_generalized_pareto_scale(function, scale, result, pol)
			&& check_generalized_pareto_shape(function, shape, result, pol)
			&& check_generalized_pareto_location(function, location, result, pol);
      } // bool check_generalized_pareto(

    } // namespace detail

    template <class RealType = double, class Policy = policies::policy<> >
    class generalized_pareto_distribution
    {
    public:
      typedef RealType value_type;
      typedef Policy policy_type;

      generalized_pareto_distribution(RealType l_scale = 1, RealType l_shape = 0, RealType l_location = 0)
        : m_scale(l_scale), m_shape(l_shape), m_location(l_location)
      { // Constructor.
        RealType result = 0;
		detail::check_generalized_pareto("boost::math::generalized_pareto_distribution<%1%>::generalized_pareto_distribution", l_scale, l_shape, l_location, &result, Policy());
      }

      RealType scale()const
      { // AKA Xm and Wolfram b and beta
        return m_scale;
      }

      RealType shape()const
      { // AKA k and Wolfram a and alpha
        return m_shape;
      }

	  RealType location()const
      { // AKA threshold and Wolfram u and theta
        return m_location;
      }
    private:
      // Data members:
      RealType m_scale;		 // distribution scale (xm) or beta
      RealType m_shape;		// distribution shape (k) or alpha
	  RealType m_location;  // distribution location (u) or theta
    };

    typedef generalized_pareto_distribution<double> generalized_pareto; // Convenience to allow generalized_pareto(2., 3.)	???	;

	// ! NOTE: may be change it ! //
    template <class RealType, class Policy>
    inline const std::pair<RealType, RealType> range(const generalized_pareto_distribution<RealType, Policy>& /*dist*/)
    { // Range of permissible values for random variable x.
      using boost::math::tools::max_value<RealType>();
      return std::pair<RealType, RealType>(static_cast<RealType>(0), max_value<RealType>()); // scale zero to + infinity.
    } // range
	// ! NOTE: may be change it ! //

    template <class RealType, class Policy>
    inline const std::pair<RealType, RealType> support(const generalized_pareto_distribution<RealType, Policy>& dist)
    { // Range of supported values for random variable x.
      // This is range where cdf rises from 0 to 1, and outside it, the pdf is zero.
      using boost::math::tools::max_value<RealType>();
	  if (dist.shape() > -boost::math::tools::epsilon<RealType>())
		  return std::pair<RealType, RealType>(dist.location(), max_value<RealType>() ); // location to + infinity.
	  else
		  return std::pair<RealType, RealType>(dist.location(), dist.location() - dist.scale() / dist.shape() ); // location to (location - scale/shape).
    } // support

    template <class RealType, class Policy>
    inline RealType pdf(const generalized_pareto_distribution<RealType, Policy>& dist, const RealType& x)
    {
      BOOST_MATH_STD_USING  // for ADL of std function pow.
      static const char* function = "boost::math::pdf(const generalized_pareto_distribution<%1%>&, %1%)";
      RealType scale = dist.scale();
      RealType shape = dist.shape();
	  RealType location = dist.location();
      RealType result = 0;
      if(false == (detail::check_generalized_pareto_x(function, x, &result, Policy())
         && detail::check_generalized_pareto(function, scale, shape, location, &result, Policy())))
         return result;
      if (x < location)
      { // regardless of shape and location, pdf is zero (or should be disallow x < scale and throw an exception?).
        return 0;
      }

	  RealType z = (x - location) / scale;
	  if (z < -boost::math::tools::epsilon<RealType>())
		  z = boost::math::tools::max_value<RealType>();
	  if (abs(shape) < boost::math::tools::epsilon<RealType>())
	  {	//shape == 0
		  result = exp(-z) / scale;
		  return result;
	  } 
	  else if (		shape		< -boost::math::tools::epsilon<RealType>() 
				&&	(z*shape+1)	< boost::math::tools::epsilon<RealType>())
	  {	//shape < 0 && x > location - scale / shape
		  result = 0;
		  return result;
	  }

	    //shape > 0
	  RealType base = RealType(1) + shape * z;
	  RealType power = -(1 / shape + RealType(1));
      result = pow(base, power) / scale;

      return result;
    } // pdf

    template <class RealType, class Policy>
    inline RealType cdf(const generalized_pareto_distribution<RealType, Policy>& dist, const RealType& x)
    {
      BOOST_MATH_STD_USING  // for ADL of std function pow.
      static const char* function = "boost::math::cdf(const generalized_pareto_distribution<%1%>&, %1%)";
      RealType scale = dist.scale();
      RealType shape = dist.shape();
	  RealType location = dist.location();
      RealType result = 0;

      if(false == (detail::check_generalized_pareto_x(function, x, &result, Policy())
         && detail::check_generalized_pareto(function, scale, shape, location, &result, Policy())))
         return result;

      if (x <= location)
      { // regardless of shape, cdf is zero.
        return 0;
      }
	  
	  RealType z = (x - location) / scale;
	  if (z < -boost::math::tools::epsilon<RealType>())
		  z = boost::math::tools::max_value<RealType>();
	  if (abs(shape) < boost::math::tools::epsilon<RealType>())
	  {	//shape == 0
		  //result = 1 - exp(-z);
		  result = - boost::math::expm1(-z, Policy()); // should be more accurate.
		  return result;
	  } 
	  else if (		shape		< -boost::math::tools::epsilon<RealType>() 
				&&	(z*shape+1)	< boost::math::tools::epsilon<RealType>())
	  {	//shape < 0 && x > location - scale / shape
		  result = 1;
		  return result;
	  }

	    //shape > 0
	  RealType base = RealType(1) + shape * z;
	  RealType power = - 1 / shape ;
      //result = RealType(1) - pow(base, power) ;
	  result = -boost::math::powm1(base, power, Policy()); // should be more accurate.
      
	  return result;
    } // cdf

    template <class RealType, class Policy>
    inline RealType quantile(const generalized_pareto_distribution<RealType, Policy>& dist, const RealType& p)
    {
      BOOST_MATH_STD_USING  // for ADL of std functions.
      static const char* function = "boost::math::quantile(const generalized_pareto_distribution<%1%>&, %1%)";
      RealType result = 0;
      RealType scale = dist.scale();
      RealType shape = dist.shape();
	  RealType location = dist.location();
      if(false == (detail::check_probability(function, p, &result, Policy())
           && detail::check_generalized_pareto(function, scale, shape, location, &result, Policy())))
      {
        return result;
      }
      if (p == 0)
      {
        return location; // x must be location (or less).
      }
      if (p == 1)
      {
        return policies::raise_overflow_error<RealType>(function, 0, Policy()); // x = + infinity.
      }

	  if (abs(shape) < boost::math::tools::epsilon<RealType>())	//shape == 0
		  result = location - scale * boost::math::log1p(-p);
	  else
		  result = location + scale / shape * (boost::math::powm1(1 - p, -shape, Policy())) ;

      return result;
    } // quantile

    template <class RealType, class Policy>
    inline RealType cdf(const complemented2_type<generalized_pareto_distribution<RealType, Policy>, RealType>& c)
    {
       BOOST_MATH_STD_USING  // for ADL of std function pow.
       static const char* function = "boost::math::cdf(const generalized_pareto_distribution<%1%>&, %1%)";
       RealType result = 0;
       RealType x = c.param;
       RealType scale = c.dist.scale();
       RealType shape = c.dist.shape();
	   RealType location = c.dist.location();
       if(false == (detail::check_generalized_pareto_x(function, x, &result, Policy())
           && detail::check_generalized_pareto(function, scale, shape, location, &result, Policy())))
         return result;

       if (x <= location)
       { // regardless of shape, cdf is zero, and complement is unity.
         return 1;
       }
	   	  
       RealType z = (x - location) / scale;
       if (z < -boost::math::tools::epsilon<RealType>())
		   z = boost::math::tools::max_value<RealType>();
       if (abs(shape) < boost::math::tools::epsilon<RealType>())
       {	//shape == 0
		   //result = 1 - exp(-z);
		   result = exp(-z); // should be more accurate.
		   return result;
	   } 
	   else if (		shape		< -boost::math::tools::epsilon<RealType>() 
				&&	(z*shape+1)	< boost::math::tools::epsilon<RealType>())
	   {	//shape < 0 && x > location - scale / shape
		   result = 0;
		   return result;
	   }

	     //shape > 0
	   RealType base = RealType(1) + shape * z;
	   RealType power = - 1 / shape ;
       //result = RealType(1) - pow(base, power) ;	//it's cdf, but we need a complement
	   result = pow(base, power); // should be more accurate.

       return result;
    } // cdf complement

    template <class RealType, class Policy>
    inline RealType quantile(const complemented2_type<generalized_pareto_distribution<RealType, Policy>, RealType>& c)
    {
      BOOST_MATH_STD_USING  // for ADL of std functions.
      static const char* function = "boost::math::quantile(const generalized_pareto_distribution<%1%>&, %1%)";
      RealType result = 0;
      RealType q = c.param;
      RealType scale = c.dist.scale();
      RealType shape = c.dist.shape();
	  RealType location = c.dist.location();
      if(false == (detail::check_probability(function, q, &result, Policy())
           && detail::check_generalized_pareto(function, scale, shape, location, &result, Policy())))
      {
        return result;
      }
      if (q == 1)
      {
        return location; // x must be scale (or less).
      }
      if (q == 0)
      {
         return policies::raise_overflow_error<RealType>(function, 0, Policy()); // x = + infinity.
      }
	  if (abs(shape) < boost::math::tools::epsilon<RealType>())	//shape == 0
		  result = location - scale * log(q);
	  else
		  result = location + scale / shape * (boost::math::powm1(q, -shape, Policy())) ;

      return result;
    } // quantile complement

    template <class RealType, class Policy>
    inline RealType mean(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      RealType result = 0;
      static const char* function = "boost::math::mean(const generalized_pareto_distribution<%1%>&, %1%)";
	  if(false == detail::check_generalized_pareto(function, dist.scale(), dist.shape(), dist.location(), &result, Policy()))
      {
        return result;
      }
      if (dist.shape() < RealType(1))
      {
        return dist.location() + dist.scale() / (1 - dist.shape());
      }
      else
      {
		// There is no mean:
		   typedef typename Policy::assert_undefined_type assert_type;
		   BOOST_STATIC_ASSERT(assert_type::value == 0);	    
		return policies::raise_domain_error<RealType>(
		"boost::math::mean(generalized_pareto<%1%>&)",
		"The Generalized Pareto distribution does not have a mean with shape > 1: "
		"the possible return value is %1%.",
		std::numeric_limits<RealType>::quiet_NaN(), Policy());
      }
    } // mean

    template <class RealType, class Policy>
    inline RealType mode(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
		if (abs(dist.shape()) < boost::math::tools::epsilon<RealType>() 
			&& abs(dist.location()) < boost::math::tools::epsilon<RealType>())
				return 0;	//exponential
		else if (shape > boost::math::tools::epsilon<RealType>() 
				&& abs(dist.location() - dist.scale() / dist.shape()) < boost::math::tools::epsilon<RealType>())
					return dist.scale() / dist.shape();	//Pareto classical

		// There is no mode, if GPD is not equal to expotential or Pareto classical:
		typedef typename Policy::assert_undefined_type assert_type;
		BOOST_STATIC_ASSERT(assert_type::value == 0);	    
		return policies::raise_domain_error<RealType>(
		"boost::math::mode(generalized_pareto<%1%>&)",
		"The Generalized Pareto distribution does not have a mode: "
		"the possible return value is %1%.",
		std::numeric_limits<RealType>::quiet_NaN(), Policy());
    } // mode

    template <class RealType, class Policy>
    inline RealType median(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      BOOST_MATH_STD_USING  // for ADL of std functions.
      RealType result = 0;
      static const char* function = "boost::math::median(const generalized_pareto_distribution<%1%>&, %1%)";
	  if(false == detail::check_generalized_pareto(function, dist.scale(), dist.shape(), dist.location(), &result, Policy()))
      {
        return result;
      }
	  return dist.location() + dist.scale() / dist.shape() * boost::math::powm1(RealType(2), dist.shape());
    } // median

    template <class RealType, class Policy>
    inline RealType variance(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      RealType result = 0;
      RealType scale = dist.scale();
      RealType shape = dist.shape();
      static const char* function = "boost::math::variance(const generalized_pareto_distribution<%1%>&, %1%)";
      if(false == detail::check_generalized_pareto(function, scale, shape, dist.location(), &result, Policy()))
      {
        return result;
      }
	  if (shape < boost::math::constants::half)
      {
        result = (scale * scale) /
         ((1 - shape) *  (1 - shape) * (1 - 2*shape));
      }
      else
      {
        result = policies::raise_domain_error<RealType>(
          function,
          "variance is undefined for shape >= 0.5, but got %1%.", dist.shape(), Policy());
      }
      return result;
    } // variance

    template <class RealType, class Policy>
    inline RealType skewness(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      BOOST_MATH_STD_USING
      RealType result = 0;
      RealType shape = dist.shape();
      static const char* function = "boost::math::skewness(const generalized_pareto_distribution<%1%>&, %1%)";
	  if(false == detail::check_generalized_pareto(function, dist.scale(), shape, dist.location(), &result, Policy()))
      {
        return result;
      }
	  if (shape < boost::math::constants::third )
      {
        result = sqrt(1 - 2 * shape) *
          2 * (shape + 1) /
          (1 - 3 * shape);
      }
      else
      {
        result = policies::raise_domain_error<RealType>(
          function,
          "skewness is undefined for shape >= 1/3, but got %1%.", dist.shape(), Policy());
      }
      return result;
    } // skewness

    template <class RealType, class Policy>
    inline RealType kurtosis(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      RealType result = 0;
      RealType shape = dist.shape();
      static const char* function = "boost::math::kurtosis(const generalized_pareto_distribution<%1%>&, %1%)";
	  if(false == detail::check_generalized_pareto(function, dist.scale(), shape, dist.location(), &result, Policy()))
      {
        return result;
      }
	  if (shape < boost::math::constants::half / 2)
      {
        result = 3 * ((1 - 2 * shape) * (2 * shape * shape + shape + 3)) /
					((1 - 3 * shape) * (1 - 4 * shape));
		// kurtosis_excess = kurtosis - 3 , isn't it?
      }
      else
      {
        result = policies::raise_domain_error<RealType>(
          function,
          "kurtosis_excess is undefined for shape >= 1/4, but got %1%.", shape, Policy());
      }
      return result;
    } // kurtosis

    template <class RealType, class Policy>
    inline RealType kurtosis_excess(const generalized_pareto_distribution<RealType, Policy>& dist)
    {
      RealType result = 0;
      RealType shape = dist.shape();
      static const char* function = "boost::math::kurtosis_excess(const generalized_pareto_distribution<%1%>&, %1%)";
	  if(false == detail::check_generalized_pareto(function, dist.scale(), shape, dist.location(), &result, Policy()))
      {
        return result;
      }
	  if (shape < boost::math::constants::half / 2)
      {
        result = -3 + 
			3 * ((1 - 2 * shape) * (2 * shape * shape + shape + 3)) /
			((1 - 3 * shape) * (1 - 4 * shape));
      }
      else
      {
        result = policies::raise_domain_error<RealType>(
          function,
          "kurtosis_excess is undefined for shape >= 1/4, but got %1%.", shape, Policy());
      }
      return result;
    } // kurtosis_excess

    } // namespace math
  } // namespace boost

  // This include must be at the end, *after* the accessors
  // for this distribution have been defined, in order to
  // keep compilers that support two-phase lookup happy.
#include <boost/math/distributions/detail/derived_accessors.hpp>

#endif // BOOST_STATS_GENERALIZED_PARETO_HPP


