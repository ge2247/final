// fms_gamma.h - Gamma distribution
#pragma once
#include <cmath>
#include <tuple>
#include "../xll12/xll/ensure.h"

#ifndef M_SQRT2PI
#define M_SQRT2PI  2.50662827463100050240
#endif

//
// Cephes library declarations
//

// Gamma function
// int_0^infty t^{a - 1} exp(-a t) dt
extern "C" double gamma(double a);

// Incomplete Gamma function 
// int_0^x t^{a - 1} exp(-a t) dt
extern "C" double igam(double a, double x);

namespace fms::gamma {

	// Gamma distribution: g(x) = x^{a-1} exp(-b x) b^a/Gamma(a), x > 0
	inline double pdf(double x, double a, double b)
	{
		return pow(x, a - 1) * exp(-b * x) * pow(b, a) / ::gamma(a);
	}

	inline double cdf(double x, double a, double b)
	{
		return ::igam(a, b * x);
	}

	// The Gamma distribution has density function g(x) = x ^ (a - 1) exp(-b x) b ^ a / Gamma(a), x > 0,
	// It has mean a/b and variance a/b^2.
	//
	// The Black distribution is F = f exp(s Z - s^2/2), where Z is standard normal and s = sigma sqrt(t).  
	// It has mean f and variance f^2 (exp(s^2) - 1).
	//
	// We Gamma distribution has F = f G, where G has mean 1 and variance exp(s^2) - 1
	// Solving 1 = a/b and (exp(s^2) - 1) = a/b^2 gives
	// a = b and b = 1/(exp(s^2) - 1).
	inline std::pair<double, double> convert(double s)
	{
		//!!! return (a, b) above
		double a = 1 / (exp(s * s) - 1);

		return std::pair(a, a);
	}

	// Put value is E[(k - F)^+] = k P(F <= k) - E[F 1(F <= k)]
	// E[F 1(G <= k/f)] = f int_0^k x g(x) dx = f gamma::cdf(k/f, a + 1, b)
	inline double put(double f, double sigma, double k, double t)
	{
		ensure(f > 0);
		ensure(sigma > 0);
		ensure(k > 0);
		ensure(t > 0);
		
		double s = sigma * sqrt(t);
		std::pair<double, double> parameters = convert(s);

		double a = parameters.first;
		double b = parameters.second;

		double first = k * gamma::cdf(k / f, a, b);
		double expectation = f * gamma::cdf(k / f, a + 1, b);
		
		//!!! calculate put value
		return first - expectation;
	}
}