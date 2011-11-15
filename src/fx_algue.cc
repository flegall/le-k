#include <boost.hh>
#include <cokus.hh>
#include <fx_algue.hh>
#include <log.hh>

#include <stdlib.h>

#include <set>
using namespace std;

const double MIN_SIZE = 5.0;

fx_algue::fx_algue(Demo* const demo,
		   const unsigned long seed,
		   const vector<context_rule> rules,
		   const string startRule,
		   const map<string, shared_ptr<_2D_Renderer> > renderers,
		   shared_ptr<Actuator<tuple<double, double> > > startPosition,
		   shared_ptr<Actuator<double > > startRotation,
		   shared_ptr<Actuator<double > > startSize,
		   shared_ptr<Actuator<double > > cutSize,
		   shared_ptr<MailBox<double> > mailbox
   ) :
   VisualEffect(demo),
   seed(seed),
   startRule(startRule),
   rules(rules),
   renderers(renderers),
   startPosition(startPosition->registerActuator()),
   startRotation(startRotation->registerActuator()),
   startSize(startSize->registerActuator()),
   cutSize(cutSize->registerActuator()),
   mailbox(mailbox)
{
   info("");

   set<string> defined_rules;
   
   for(vector<context_rule>::const_iterator i = rules.begin(); i != rules.end(); i++)
   {
      const string ruleName = (*i).name;
      if(defined_rules.find(ruleName) == defined_rules.end())
      {
	 defined_rules.insert(ruleName);
	 indexed_rules[ruleName] = vector<context_rule>();
      }
   }
   
   for(map<string, shared_ptr<_2D_Renderer> >::const_iterator i = renderers.begin(); i != renderers.end(); i++)
   {
      const string ruleName = (*i).first;
      if(defined_rules.find(ruleName) == defined_rules.end())
      {
	 defined_rules.insert(ruleName);
      }
      else
      {
	 string err0r = fPrint("Rule \'%s\' already defined", ruleName.c_str());
	 error(err0r);
	 throw err0r;
      }
   }

   if(defined_rules.find("") != defined_rules.end())
   {
      	 string err0r("Can't define a rule with no name");
	 error(err0r);
	 throw err0r;
   }
   
   for(vector<context_rule>::const_iterator i = rules.begin(); i != rules.end(); i++)
   {
      const context_rule rule = *i;
      
      indexed_rules[rule.name].push_back(rule);
      
      for(vector<called_rule>::const_iterator j = rule.called_rules.begin(); j != rule.called_rules.end(); j++)
      {
	 const string name = (*j).name;
	 if(defined_rules.find(name) == defined_rules.end())
	 {
	    string err0r = fPrint("Rule \'%s\' unknown", name.c_str());
	    error(err0r);
	    throw err0r;
	 }
      }
   }

   if(defined_rules.find(startRule) == defined_rules.end())
   {
      string err0r = fPrint("Start rule \'%s\' unknown", startRule.c_str());
      error(err0r);
      throw err0r;
   }
};

void fx_algue::process(const double ms)
{
   //processPrepare(ms + 1000.0);
   processRender(ms);
};

bool fx_algue::processPrepare(const double ms)
{
   //srand(seed);
   srand_cokus(seed);

   flSet2DMode640();

   ModelViewStack stack;

   flMatrixMode(FL_MODELVIEW);

   tuple<double, double> pos = startPosition->getValue(ms);
   flTranslate(pos.get<0>(),pos.get<1>());
   const double newSize = startSize->getValue(ms);
   flScale(newSize);
   const double rotation = startRotation->getValue(ms);
   flRotate(rotation);

   RenderedItem item;
   
   const bool do_render = newSize > cutSize->getValue(ms);
   applyRule(startRule,
	     stack,
	     newSize,
	     ms,
	     0,
	     do_render,
	     true,
	     item);
   
   const bool result = compareRendering(item, lastItem, ms);

   lastItem = item;

   return result;
}

bool fx_algue::compareRendering(RenderedItem& current,
				RenderedItem& previous,
				const double ms)
{
   if((current == previous))
   {
      for(unsigned int i = 0; i < current.children.size(); i++)
      {
	 const bool result = compareRendering(current.children[i], previous.children[i], ms);
	 if(result)
	 {
	    return true;
	 }
	 //info("current.ruleName:%s", current.ruleName.c_str());
//	 info("current.children.size():%d", current.children.size());
//	 info("previous.children.size():%d", previous.children.size());	 
	 //compareRendering(current.children[i], previous.children[i], ms);
      }
   }
   else
   {
      if(previous.ruleName != "")
      {
	 //notifyChange(ms);
	 return true;
      }
   }
   return false;
}

void fx_algue::notifyChange(const double ms)
{
   //mailbox->post(ms);
}

void fx_algue::processRender(const double ms)
{
//   srand(seed);
   srand_cokus(seed);

   flSet2DMode640();

   ModelViewStack stack;

   flMatrixMode(FL_MODELVIEW);

   tuple<double, double> pos = startPosition->getValue(ms);
   flTranslate(pos.get<0>(), pos.get<1>());
   const double newSize = startSize->getValue(ms);
   flScale(newSize);
   const double rotation = startRotation->getValue(ms);
   flRotate(rotation);

   RenderedItem item;

   const bool do_render = newSize > cutSize->getValue(ms);
   applyRule(startRule,
	     stack,
	     newSize,
	     ms,
	     0,
	     do_render,
	     false,
	     item);
}

void fx_algue::applyRule(const string& rule,
			 ModelViewStack& stack,
			 const double size,
			 const double ms,
			 const int recurse_count,
			 const bool render,
			 const bool prepare,
			 RenderedItem& item)
{
   if(size > MIN_SIZE)
   {
      if(recurse_count > MAX_RECURSE)
      {
	 const string err0R = fPrint("Recurse count exceeded");
	 error(err0R);
	 throw err0R;
      }

      
      const map<string, shared_ptr<_2D_Renderer> >::const_iterator i = renderers.find(rule);
      
      if(i != renderers.end())
      {
	 if(render && !prepare)
	 {
	    shared_ptr<_2D_Renderer> renderer = (*i).second;
	    
	    renderer->begin();
	    renderer->render(1.0);
	    renderer->end();
	 }
      }
      else
      {
	 const context_rule& selectedRule = pickupRule(rule, ms);
	 
	 if(prepare && render)
	 {
	    RenderedItem child(rule);
	    execRule(selectedRule,
		     stack,
		     ms,
		     size,
		     recurse_count,
		     true,
		     true,
		     child);
	    item.children.push_back(child);
	 }
	 else
	 {  
	    execRule(selectedRule,
		     stack,
		     ms,
		     size,
		     recurse_count,
		     true,
		     false,
		     item);
	 }
      }
   }
}

void fx_algue::execRule(const context_rule& selectedRule,
			ModelViewStack& stack,
			const double ms,
			const double size,
			const int recurse_count,
			const bool render,
			const bool prepare,
			RenderedItem& item)
{
   for(vector<called_rule>::const_iterator i = selectedRule.called_rules.begin(); i != selectedRule.called_rules.end(); i++)
   {
      double newSize = size;
      const called_rule& called = *i;
      map<RuleParam, shared_ptr<Source<double> > >::const_iterator finder;

      if(render && !prepare)
      {
	 // case when we do need matrix stack.
	 if(selectedRule.called_rules.size() > 1)
	 {
	    stack.push();
	 }
	 
	 finder = called.params.find(Rotate);
	 if(finder != called.params.end())
	 {
	    const double angle = (*finder).second->getValue(ms);
	    flRotate(angle);
	 }
	 
	 finder = called.params.find(Flip);
	 if(finder != called.params.end())
	 {
	    const double angle = (*finder).second->getValue(ms);
	    flRotate(angle);
	    flScale(1.0, -1.0, 1.0);
	    flRotate(-angle);
	 }

	 finder = called.params.find(XTranslate);
	 if(finder != called.params.end())
	 {
	    const double x = (*finder).second->getValue(ms);
	    flTranslate(x, 0.0);
	 }
	 
	 finder = called.params.find(YTranslate);
	 if(finder != called.params.end())
	 {
	    const double y = (*finder).second->getValue(ms);
	    flTranslate(0.0, y);
	 }
      }

      finder = called.params.find(Scale);
      if(finder != called.params.end())
      {
	 const double scaleFactor = (*finder).second->getValue(ms);
	 if(render && !prepare)
	 {
	    flScale(scaleFactor);
	 }
	 newSize = size * scaleFactor;
      }

  

      const bool do_render = newSize > cutSize->getValue(ms);
      applyRule(called.name,
		stack,
		newSize,
		ms,
		recurse_count + 1,
		do_render,
		prepare,
		item);

      if(render && !prepare)
      {
	 // case when we do need matrix stack
	 if(selectedRule.called_rules.size() >1 )
	 {
	    stack.pop();
	 }
      }
   }
}



fx_algue::~fx_algue()
{
   info("");
};

context_rule::context_rule(const string name, const shared_ptr<Actuator<double> > probability, const vector<called_rule> called_rules) :
   name(name),
   probability(probability->registerActuator()),
   called_rules(called_rules)
{
}

called_rule::called_rule(const string name, const map<RuleParam, shared_ptr<Actuator<double> > > params) :
   name(name),
   params(buildSources(params))
{
}


const map<RuleParam, shared_ptr<Source<double> > > called_rule::buildSources(const map<RuleParam, shared_ptr<Actuator<double> > > params)
{
   map<RuleParam, shared_ptr<Source<double> > > result;

   for(map<RuleParam, shared_ptr<Actuator<double> > >::const_iterator i = params.begin(); i != params.end(); i++)
   {
      const RuleParam paramName = (*i).first;
      shared_ptr<Actuator<double > > actuator = (*i).second;
      result[paramName] = actuator->registerActuator();
   }
   
   return result;
}

vector<context_rule*> fx_algue::getRules(const string& name, const double ms)
{
   vector<context_rule*> selected;
   for(vector<context_rule>::iterator i = rules.begin(); i != rules.end(); i++)
   {
      context_rule& rule = *i;
      if(name == rule.name)
      {
	 selected.push_back(&rule);
      }
   }
   return selected;
}

const context_rule& fx_algue::pickupRule(const string& name, const double ms)
{
   vector<context_rule*> rules = getRules(name, ms);
   vector<tuple<double, double, context_rule&> > target;
   
   double position = 0.0;
   for(vector<context_rule*>::const_iterator i = rules.begin(); i != rules.end(); i++)
   {
      context_rule& rule = **i;
      const double probability = rule.probability->getValue(ms);
      tuple<double, double, context_rule&> targetElement(position, position + probability, rule);
      target.push_back(targetElement);
      position += probability;
   }

//   const int r = rand();
//   const double rd = static_cast<double>(r);
   const double rd = rand_cokus();      

   for(vector<tuple<double, double, context_rule&> >::iterator i = target.begin(); i != target.end() ; i++)
   {   
      (*i).get<0>() /= position;
      (*i).get<1>() /= position;

      (*i).get<0>() *= static_cast<double>(UINT_MAX);
      (*i).get<1>() *= static_cast<double>(UINT_MAX);      
      
//      (*i).get<0>() *= static_cast<double>(RAND_MAX);
//      (*i).get<1>() *= static_cast<double>(RAND_MAX);

      if((*i).get<0>() <= rd &&  rd <=  (*i).get<1>())
      {
	 return  (*i).get<2>();
      }
   }

   throw string("unreachable code");
}


RenderedItem::RenderedItem()
   : ruleName("")
{
}

RenderedItem::RenderedItem(const string ruleName)
   : ruleName(ruleName)
{
}

bool RenderedItem::operator == (const RenderedItem& other)
{
   //info("ruleName: this:%s other:%s", ruleName.c_str(), other.ruleName.c_str());
   return (ruleName == other.ruleName) && (children.size() == other.children.size());
}
