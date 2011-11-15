#ifndef bezier_hh
#define bezier_hh

#include <boost.hh>

#include <vector>
using namespace std;

class Bezier1DConfig
{
   public:
      Bezier1DConfig(const double P0,
		     const double P1,
		     const double P2,
		     const double P3,
		     const double tBegin,
		     const double tEnd);
      // First point coord
      double P0;
      // First control point coord
      double P1;
      // Second control point coord
      double P2;
      // Second point coord
      double P3;
      // beginning of the segment
      double tBegin;
      // end of the segment;
      double tEnd;
      // the polynomial coefficients <a, b, c>
      tuple<double, double, double> coefs;

      bool operator < (const Bezier1DConfig& right) const;
};

class Bezier1DEasyConfig
{
   public:
      Bezier1DEasyConfig(const double t, const double value, const double variation);
      // position
      double t;
      // value
      double value;
      // variation
      double variation;

      bool operator < (const Bezier1DEasyConfig& right) const;
      
};

class Bezier1D
{
   public:
      Bezier1D(vector<Bezier1DEasyConfig> easyConfigs);
      double getValue(const double t);
      
   private:
      vector<Bezier1DConfig> configs;
      double tBegin;
      double tEnd;
      Bezier1DConfig& getConfig(const double t);

      vector<Bezier1DConfig> processEasyConfigs(vector<Bezier1DEasyConfig> easyConfigs);
};

#endif
