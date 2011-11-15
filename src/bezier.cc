#include <bezier.hh>
#include <log.hh>

#include <algorithm>

#include <math.h>

Bezier1DConfig::Bezier1DConfig(const double P0,
			       const double P1,
			       const double P2,
			       const double P3,
			       const double tBegin,
			       const double tEnd) :
   P0(P0),
   P1(P1),
   P2(P2),
   P3(P3),
   tBegin(tBegin),
   tEnd(tEnd)
{
   // Stolen from http://en.wikipedia.org/wiki/B%C3%A9zier_curve
    const double c = 3.0 * (P1 - P0);
    const double b = 3.0 * (P2 - P1) - c;
    const double a = P3 - P0 - c - b;
    
    coefs = tuple<double, double, double>(a, b, c);
}

bool Bezier1DConfig::operator < (const Bezier1DConfig& right) const
{
   return tBegin < right.tBegin;
}

Bezier1DEasyConfig::Bezier1DEasyConfig(const double t, const double value, const double variation) :
   t(t),
   value(value),
   variation(variation * fabs(value))
{
}

bool Bezier1DEasyConfig::operator < (const Bezier1DEasyConfig& right) const
{
   return t < right.t;
}

Bezier1D::Bezier1D(const vector<Bezier1DEasyConfig> easyConfigs)
{
   configs = processEasyConfigs(easyConfigs);
   for(vector<Bezier1DConfig>::const_iterator i = configs.begin(); i != configs.end(); i++)
   {
      const Bezier1DConfig& config = *i;
      if(config.tEnd <= config.tBegin)
      {
	 const string err0r = fPrint("Incorrect Bezier setting tBegin : %lf tEnd : %lf", config.tBegin, config.tEnd);
	 error(err0r);
	 throw err0r;
      }
   }

   sort(configs.begin(), configs.end());
   
   for(unsigned int i = 1; i < configs.size(); i++)
   {
      const Bezier1DConfig& first = configs[i - 1];
      const Bezier1DConfig& second = configs[i];
      if(first.tEnd != second.tBegin)
      {
	 const string err0r = fPrint("Incorrect Bezier setting : i-1 : %d i : %d", i-1, i);
	 error(err0r);
	 throw err0r;
      }
   }

   tBegin = (*(configs.begin())).tBegin;
   tEnd = configs[configs.size()-1].tEnd;
   info("tBegin:%lf tEnd:%lf", tBegin, tEnd);
}

// Stolen from http://en.wikipedia.org/wiki/B%C3%A9zier_curve
double Bezier1D::getValue(const double t)
{
   if(t < tBegin || t > tEnd)
   {
      const string err0r = fPrint("Incorrect bezier range : %lf", t);
      error(err0r);
      throw err0r;
   }

   Bezier1DConfig& config = getConfig(t);

   const double begin = config.tBegin;
   const double end = config.tEnd;

   const double length = end - begin;
   const double localT = (t - begin) / length;

   const double tSquared = localT * localT;
   const double tCubed = tSquared * localT;

   const double a = config.coefs.get<0>();
   const double b = config.coefs.get<1>();
   const double c = config.coefs.get<2>();

   const double result = (a * tCubed) + (b * tSquared) + (c * localT) + config.P0;

   return result;
}

Bezier1DConfig& Bezier1D::getConfig(const double t)
{
   unsigned int i = 0;
   for(i = 0; i < configs.size(); i++)
   {
      Bezier1DConfig& config = configs[i];
      if(config.tBegin <= t && t <= config.tEnd)
      {
	 return config;
      }
   }
   throw string("This should never happen");
}

vector<Bezier1DConfig> Bezier1D::processEasyConfigs(vector<Bezier1DEasyConfig> easyConfigs)
{
   assert(easyConfigs.size() >= 2);
   
   vector<Bezier1DConfig> result;
   
   sort(easyConfigs.begin(), easyConfigs.end());

   for(unsigned int i = 1; i < easyConfigs.size(); i++)
   {
      Bezier1DEasyConfig& first = easyConfigs[i - 1];
      Bezier1DEasyConfig& second = easyConfigs[i];

      Bezier1DConfig config(first.value, first.value + first.variation, second.value - second.variation, second.value, first.t, second.t);
      
      result.push_back(config);
   }
   
   return result;
}
   
