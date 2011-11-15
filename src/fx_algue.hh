#ifndef fx_algue_hh
#define fx_algue_hh

#include <2d_renderer.hh>
#include <actuator.hh>
#include <boost.hh>
#include <effect.hh>
#include <opengl.hh>
#include <script.hh>
#include <source.hh>
#include <rd_point.hh>

#include <vector>
#include <map>
using namespace std;

enum RuleParam {Rotate, Scale, Flip, YTranslate, XTranslate};

class called_rule
{
   public:
      called_rule(const string name, const map<RuleParam, shared_ptr<Actuator<double> > > params);
      string name;
      map<RuleParam, shared_ptr<Source<double> > > params;
   private:
      static const map<RuleParam, shared_ptr<Source<double> > > buildSources(const map<RuleParam, shared_ptr<Actuator<double> > > params);
};

class context_rule
{
   public:
      context_rule(const string name, const shared_ptr<Actuator<double> > probability, const vector<called_rule> called_rules);
      string name;
      shared_ptr<Source<double> > probability;
      vector<called_rule> called_rules;
};

class RenderedItem
{
   public:
      RenderedItem();
      RenderedItem(const string ruleName);
      string ruleName;
      vector<RenderedItem> children;

      bool operator == (const RenderedItem& other);
};


class fx_algue : public VisualEffect
{
   public:
      fx_algue(Demo* const demo,
	       const unsigned long seed,
	       const vector<context_rule> rules,
	       const string startRule,
	       const map<string, shared_ptr<_2D_Renderer> > renderers,
	       shared_ptr<Actuator<tuple<double, double> > > startPosition,
	       shared_ptr<Actuator<double> > startRotation,
	       shared_ptr<Actuator<double> > startSize,
	       shared_ptr<Actuator<double> > cutSize,
	       shared_ptr<MailBox<double> > mailbox
	 );
      void process(const double ms);
      bool processPrepare(const double ms);      
      ~fx_algue();
      
   private:

      void processRender(const double ms);
      
      void applyRule(const string& rule,
		     ModelViewStack& stack,
		     const double size,
		     const double ms,
		     const int recurse_count,
		     const bool render,
		     const bool prepare,
		     RenderedItem& item);
      
      void execRule(const context_rule& selectedRule,
		    ModelViewStack& stack,
		    const double ms,
		    const double size,
		    const int recurse_count,
		    const bool render,
		    const bool prepare,
		    RenderedItem& item);

      bool compareRendering(RenderedItem& current,
			    RenderedItem& previous,
			    const double ms);

      void notifyChange(const double ms);
      
      const context_rule& pickupRule(const string& name, const double ms);
      vector<context_rule*> getRules(const string& name, const double ms);
				  
      const unsigned long seed;
      const string startRule;
      vector<context_rule> rules;
      const map<string, shared_ptr<_2D_Renderer> > renderers;
      map<string, vector<context_rule> > indexed_rules;
      shared_ptr<Source<tuple<double, double> > > startPosition;
      shared_ptr<Source<double> > startRotation;
      shared_ptr<Source<double> > startSize;
      shared_ptr<Source<double> > cutSize;
      shared_ptr<MailBox<double> > mailbox;
      RenderedItem lastItem;
      static const int MAX_RECURSE = 1000;
};

#endif
