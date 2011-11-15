#include <2d_renderer.hh>
#include <activator.hh>
#include <base_effects.hh>
#include <base_actuators.hh>
#include <boost.hh>
#include <color.hh>
#include <demo_empty.hh>
#include <effect.hh>
#include <fx_algue.hh>
#include <fx_cadre.hh>
#include <fx_florent.hh>
#include <fx_glitch.hh>
#include <log.hh>
#include <mailbox.hh>
#include <rd_point.hh>


EmptyDemo::EmptyDemo(Demo* const demo) : Script(demo)
{
};

void EmptyDemo::initDemo()
{
   demo->setMusicLocation("data/EmptyDemo/music.ogg");
   demo->setMusicVolume(0.5);
   
   demo->setDemoDuration(100000.0);

   const double duration2 = 96000.0 + 5000.0;

   // mailbox fx_algue > fx_glitch
   shared_ptr<MailBox<double> > mailBox(new MailBox<double>());

   shared_ptr<TimeSegmentsActivator> videoActivator(new TimeSegmentsActivator());

   // hue
   Bezier1DEasyConfig hueGradArray[] =
   {
      Bezier1DEasyConfig(0.0, 230.0, 0.0),
      Bezier1DEasyConfig(10000.0, 230.0, 0.0),
      Bezier1DEasyConfig(25800.0, 230.0, 0.0),
      Bezier1DEasyConfig(26100.0, 193.0, 0.0),
      Bezier1DEasyConfig(33800.0, 193.0, 0.0),
      Bezier1DEasyConfig(34100.0, 35.0, 0.0),
      Bezier1DEasyConfig(51200.0, 35.0, 0.0),
      Bezier1DEasyConfig(51500.0, 28.0, 0.0),
      Bezier1DEasyConfig(60200.0, 0.0, 0.0),
      Bezier1DEasyConfig(70000.0, 0.0, 0.0),      
      Bezier1DEasyConfig(70200.0, 248.0, 0.0),      
      Bezier1DEasyConfig(duration2, 248.0, 0.0),
   };
   vector<Bezier1DEasyConfig> hueGrad = buildVectorFromArray(hueGradArray);
   Bezier1D hue = Bezier1D(hueGrad);

   // saturation
   Bezier1DEasyConfig satGradArray[] =
   {
      Bezier1DEasyConfig(0.0, 0.0, 0.0),
      Bezier1DEasyConfig(10000.0, 0.0, 0.0),
      Bezier1DEasyConfig(11000.0, 0.08, 0.0),
      Bezier1DEasyConfig(43100.0, 0.08, 0.0),
      Bezier1DEasyConfig(43300.0, 0.64, 0.0),
      Bezier1DEasyConfig(51200.0, 0.64, 0.0),
      Bezier1DEasyConfig(51500.0, 0.81, 0.0),
      Bezier1DEasyConfig(60200.0, 0.70, 0.0),
      Bezier1DEasyConfig(64700.0, 0.70, 0.0),
      Bezier1DEasyConfig(65000.0, 0.81, 0.0),
      Bezier1DEasyConfig(70000.0, 0.0, 0.0),      
      Bezier1DEasyConfig(76000.0, 0.0, 0.0),
      Bezier1DEasyConfig(82400.0, 0.14, 0.0),                  
      Bezier1DEasyConfig(duration2, 0.14, 0.0),
   };
   vector<Bezier1DEasyConfig> satGrad = buildVectorFromArray(satGradArray);
   Bezier1D sat = Bezier1D(satGrad);

   // value
   Bezier1DEasyConfig valGradArray[] =
   {
      Bezier1DEasyConfig(0.0, 0.0, 0.0),
      Bezier1DEasyConfig(7000.0, 0.0, 0.0),
      Bezier1DEasyConfig(16000.0, 1.0, 0.0),
      Bezier1DEasyConfig(21300.0, 1.0, 0.0),
      Bezier1DEasyConfig(21600.0, 0.6, 0.2),
      Bezier1DEasyConfig(25800.0, 0.6, 0.0),
      Bezier1DEasyConfig(26100.0, 0.7, 0.0),
      Bezier1DEasyConfig(36000.0, 0.7, 0.0),
      Bezier1DEasyConfig(36100.0, 0.9, 0.0),
      Bezier1DEasyConfig(40400.0, 0.9, 0.0),
      Bezier1DEasyConfig(40700.0, 0.6, 0.0),
      Bezier1DEasyConfig(43100.0, 0.6, 0.0),
      Bezier1DEasyConfig(43300.0, 0.5, 0.0),
      Bezier1DEasyConfig(46400.0, 0.5, 0.0),
      Bezier1DEasyConfig(46600.0, 0.6, 0.0),
      Bezier1DEasyConfig(51200.0, 0.86, 0.0),
      Bezier1DEasyConfig(51500.0, 0.86, -0.5),
      Bezier1DEasyConfig(59900.0, 0.70, 0.0),
      Bezier1DEasyConfig(64900.0, 0.70, 0.0),
      Bezier1DEasyConfig(65200.0, 0.50, 0.5),
      Bezier1DEasyConfig(76000.0, 0.80, 0.0),
      Bezier1DEasyConfig(82400.0, 0.60, 0.0),
      Bezier1DEasyConfig(90000, 0.0, 0.0),      
      Bezier1DEasyConfig(duration2, 0.0, 0.0),
   };
   vector<Bezier1DEasyConfig> valGrad = buildVectorFromArray(valGradArray);
   Bezier1D val = Bezier1D(valGrad);
   
   shared_ptr< Actuator<Color> > colorParam(new ColorActuator(hue, sat, val));
   shared_ptr<FX_Clear> clear = FX_Clear::BuildDefaultClear(demo);
   clear->setColorParam(colorParam);
   
   videoActivator->registerEffect(clear, 0.0, 0.0, 100000.0);
   
   // ALGUE EFFECT ////
   vector<context_rule> rules;

   
   // RENDERER
   map<string, shared_ptr<_2D_Renderer> >renderers;
   renderers["circle"] = shared_ptr<_2D_Renderer>(new PointRenderer());

   const map<RuleParam, shared_ptr<Actuator<double> > > emptyMAP;

   /*
   // rule 1
   vector<called_rule> rule1_rules;
   
   map<RuleParam, shared_ptr<Actuator<double> > > rule1_r1_params;
   rule1_r1_params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   //rule1_r1_params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   called_rule rule1_r1("circle", rule1_r1_params);
   rule1_rules.push_back(rule1_r1);

   map<RuleParam, shared_ptr<Actuator<double> > > rule1_r2_params;
   rule1_r2_params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.0));
   rule1_r2_params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.0));
   rule1_r2_params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   called_rule rule1_r2("circle", rule1_r2_params);
   rule1_rules.push_back(rule1_r2);
   
   shared_ptr<Actuator<double> > prob_rule1(new ConstantActuator<double>(1.0));
   context_rule rule1("rule1", prob_rule1, rule1_rules);
   rules.push_back(rule1);

   
   // rule 2
   vector<called_rule> rule2_rules;
   
   map<RuleParam, shared_ptr<Actuator<double> > > rule2_r1_params;
//   rule2_r1_params[Flip] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(90.0));   
//   rule2_r1_params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(30.0));
   called_rule rule2_r1("rule1", rule2_r1_params);
   rule2_rules.push_back(rule2_r1);

   shared_ptr<Actuator<double> > prob_rule2(new ConstantActuator<double>(1.0));
   context_rule rule2("rule2", prob_rule2, rule2_rules);
   rules.push_back(rule2);

      
   // startup
   //const string startRule("rule2");
 

   // start 1
   vector<called_rule> start1_rules;
   map<RuleParam, shared_ptr<Actuator<double> > > start1_1params;
   start1_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   called_rule start1_1("circle", start1_1params);
   start1_rules.push_back(start1_1);

    shared_ptr<Actuator<double> > prob_start1(new ConstantActuator<double>(1.0));
   context_rule start_1("start", prob_start1, start1_rules);
   rules.push_back(start_1);

   // start 2
   vector<called_rule> start2_rules;
   map<RuleParam, shared_ptr<Actuator<double> > > start2_1params;
   start2_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   called_rule start2_1("circle", start2_1params);
   start2_rules.push_back(start2_1);

   shared_ptr<Actuator<double> > prob_start2(new ConstantActuator<double>(1.0));
   context_rule start_2("start", prob_start2, start2_rules);
   rules.push_back(start_2);

   // start many
   vector<called_rule> startmany_rules;
   
   map<RuleParam, shared_ptr<Actuator<double> > > startmany_1params;
   startmany_1params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(45.0));
   startmany_1params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(2.0));
   called_rule startmany_1("start", startmany_1params);
   startmany_rules.push_back(startmany_1);

   map<RuleParam, shared_ptr<Actuator<double> > > startmany_2params;
   startmany_2params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(90.0));
   startmany_2params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(2.0));   
   called_rule startmany_2("start", startmany_2params);
   startmany_rules.push_back(startmany_2);
   
   map<RuleParam, shared_ptr<Actuator<double> > > startmany_3params;
   startmany_3params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(135.0));
   startmany_3params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(2.0));
   called_rule startmany_3("start", startmany_3params);
   startmany_rules.push_back(startmany_3);

   map<RuleParam, shared_ptr<Actuator<double> > > startmany_4params;
   startmany_4params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(180.0));
   startmany_4params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(2.0));
   called_rule startmany_4("start", startmany_4params);
   startmany_rules.push_back(startmany_4);

   
   shared_ptr<Actuator<double> > prob_startmany(new ConstantActuator<double>(1.0));
   context_rule startmany("start-many", prob_startmany , startmany_rules);
   rules.push_back(startmany);



   // inside
   vector<called_rule> inside_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > inside_1params;
   inside_1params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(20.0));
   inside_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.8));
   called_rule inside_1("inside", inside_1params);
   inside_rules.push_back(inside_1);

   map<RuleParam, shared_ptr<Actuator<double> > > inside_2params;
   //inside_1params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(20.0));
   //inside_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.7));
   called_rule inside_2("start-many", inside_2params);
   inside_rules.push_back(inside_2);
   

   shared_ptr<Actuator<double> > prob_inside(new ConstantActuator<double>(1.0));
   context_rule inside("inside", prob_inside , inside_rules);
   rules.push_back(inside);
   

//   const string startRule("inside");
//
*/

   // globalRotate
   Bezier1DEasyConfig globalRotateGradArray[] =
      {
	 Bezier1DEasyConfig(0.0, 0.0, 0.0),
	 Bezier1DEasyConfig(20000.0, 40.0, 0.0),
	 Bezier1DEasyConfig(40000.0, -40.0, 0.0),
	 Bezier1DEasyConfig(60000.0, 50.0, 0.0),	 
	 Bezier1DEasyConfig(duration2, 0.0, 0.0),
      };
   vector<Bezier1DEasyConfig> globalRotateGrad = buildVectorFromArray(globalRotateGradArray);
   Bezier1D globalRotateBezier = Bezier1D(globalRotateGrad);
   
   // top1
   vector<called_rule> top1_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top1_1params;
   top1_1params[Rotate] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, 3.0));
   top1_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top1_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.8));
   called_rule top1_1("top", top1_1params);
   top1_rules.push_back(top1_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top1_2params;
   called_rule top1_2("circle", top1_2params);
   top1_rules.push_back(top1_2);
   

   shared_ptr<Actuator<double> > prob_top1(new ConstantActuator<double>(1.0));
   context_rule top1("top", prob_top1 , top1_rules);
   rules.push_back(top1);

   // top2
   vector<called_rule> top2_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top2_1params;
   top2_1params[Rotate] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, -4.0));
   top2_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top2_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.8));
   top2_1params[Flip] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(90));      
   called_rule top2_1("top", top2_1params);
   top2_rules.push_back(top2_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top2_2params;
   called_rule top2_2("circle", top2_2params);
   top2_rules.push_back(top2_2);
   

   shared_ptr<Actuator<double> > prob_top2(new ConstantActuator<double>(1.0));
   context_rule top2("top", prob_top2 , top2_rules);
   rules.push_back(top2);

   // top3
   vector<called_rule> top3_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top3_1params;
   //top3_1params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(7.0));
   top3_1params[Rotate] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, 9.0));
   top3_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top3_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.95));
   top3_1params[Flip] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(90));   
   called_rule top3_1("top", top3_1params);
   top3_rules.push_back(top3_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top3_2params;
   called_rule top3_2("circle", top3_2params);
   top3_rules.push_back(top3_2);
   

   shared_ptr<Actuator<double> > prob_top3(new ConstantActuator<double>(1.0));
   context_rule top3("top", prob_top3 , top3_rules);
   rules.push_back(top3);

   // top4
   vector<called_rule> top4_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top4_1params;
//   top4_1params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(-5.0));
   top4_1params[Rotate] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, -7.0));
   top4_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top4_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.95));
   called_rule top4_1("top", top4_1params);
   top4_rules.push_back(top4_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top4_2params;
   called_rule top4_2("circle", top4_2params);
   top4_rules.push_back(top4_2);
   

   shared_ptr<Actuator<double> > prob_top4(new ConstantActuator<double>(1.0));
   context_rule top4("top", prob_top4 , top4_rules);
   rules.push_back(top4);

   // top5
   vector<called_rule> top5_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top5_1params;
   top5_1params[Flip] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, 90.0));
   top5_1params[Rotate] = shared_ptr<Actuator<double> >(new BezierActuator(globalRotateBezier, -14.0));
   top5_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top5_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.95));
   called_rule top5_1("top", top5_1params);
   top5_rules.push_back(top5_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top5_2params;
   top5_2params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(60));
   called_rule top5_2("top", top5_2params);
   top5_rules.push_back(top5_2);
   

   shared_ptr<Actuator<double> > prob_top5(new ConstantActuator<double>(0.5));
   context_rule top5("top", prob_top5 , top5_rules);
   rules.push_back(top5);

   // top6
   vector<called_rule> top6_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > top6_1params;
   top6_1params[Flip] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(90));
   top6_1params[YTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   top6_1params[Scale] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(0.95));
   called_rule top6_1("top", top6_1params);
   top6_rules.push_back(top6_1);

   map<RuleParam, shared_ptr<Actuator<double> > > top6_2params;
   top6_2params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(-70.0));
   called_rule top6_2("etoile", top6_2params);
   top6_rules.push_back(top6_2);
   

   shared_ptr<Actuator<double> > prob_top6(new ConstantActuator<double>(0.2));
   context_rule top6("top", prob_top6 , top6_rules);
   rules.push_back(top6);
   
   // etoile
   vector<called_rule> etoile_rules;

   map<RuleParam, shared_ptr<Actuator<double> > > etoile_1params;
   called_rule etoile_1("top", etoile_1params);
   etoile_rules.push_back(etoile_1);

   // rotationEtoile
   Bezier1DEasyConfig rotationEtoileGradArray[] =
      {
	 Bezier1DEasyConfig(0.0, -180.0, 0.0),
	 Bezier1DEasyConfig(10000.0, -180, 0.0),
	 Bezier1DEasyConfig(duration2, 180.0, 0.0),
      };
   vector<Bezier1DEasyConfig> rotationEtoileGrad = buildVectorFromArray(rotationEtoileGradArray);
   Bezier1D rotationEtoileBezier = Bezier1D(rotationEtoileGrad);
   shared_ptr<Actuator<double> > rotationEtoileParam(new BezierActuator(rotationEtoileBezier));

   map<RuleParam, shared_ptr<Actuator<double> > > etoile_2params;
   etoile_2params[Rotate] = rotationEtoileParam;
   called_rule etoile_2("top", etoile_2params);
   etoile_rules.push_back(etoile_2);

   map<RuleParam, shared_ptr<Actuator<double> > > etoile_3params;
   etoile_3params[Rotate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(-60.0));
   etoile_3params[XTranslate] = shared_ptr<Actuator<double> >(new ConstantActuator<double>(1.0));
   called_rule etoile_3("top", etoile_3params);
   etoile_rules.push_back(etoile_3);
   

   shared_ptr<Actuator<double> > prob_etoile(new ConstantActuator<double>(0.5));
   context_rule etoile("etoile", prob_etoile , etoile_rules);
   rules.push_back(etoile);
   

   const string startRule("etoile");
   
   tuple<double, double> startPosition(320.0, 240.0);
   
   shared_ptr<Actuator<tuple<double, double> > > startPositionParam(new ConstantActuator<tuple<double, double> >(startPosition));

   shared_ptr<Actuator<double> > startSizeParam(new ConstantActuator<double>(50.0));

   // startRotation
   Bezier1DEasyConfig startRotationGradArray[] =
      {
	 Bezier1DEasyConfig(0.0, 0.0, 0.0),
	 Bezier1DEasyConfig(10000.0, 0.0, 0.0),
	 Bezier1DEasyConfig(45000.0, -180.0, 0.0),	 
	 Bezier1DEasyConfig(duration2, -360.0, 0.0),
      };
   vector<Bezier1DEasyConfig> startRotationGrad = buildVectorFromArray(startRotationGradArray);
   Bezier1D startRotationBezier = Bezier1D(startRotationGrad);
   shared_ptr<Actuator<double> > startRotationParam(new BezierActuator(startRotationBezier));


   // cutsize
   Bezier1DEasyConfig cutSizeGradArray[] =
      {
	 Bezier1DEasyConfig(0.0, 49.0, 0.0),
	 Bezier1DEasyConfig(10000.0, 49.0, 0.0),
	 Bezier1DEasyConfig(30000.0, 7.0, 0.0),
	 Bezier1DEasyConfig(60000.0, 5.0, 0.0),	 
	 Bezier1DEasyConfig(duration2, 5.0, 0.0),
      };
   vector<Bezier1DEasyConfig> cutSizeGrad = buildVectorFromArray(cutSizeGradArray);
   Bezier1D cutSizeBezier = Bezier1D(cutSizeGrad);
   shared_ptr<Actuator<double> > cutSizeParam(new BezierActuator(cutSizeBezier));

   /*
   shared_ptr<fx_algue> effect_algue(new fx_algue(demo,
						  245635,
						  rules,
						  startRule,
						  renderers,
						  startPositionParam,
						  startRotationParam,
						  startSizeParam,
						  cutSizeParam,
						  mailBox
					));
   */

   shared_ptr<fx_algue> effect_algue(new fx_algue(demo,
						  98765432,
						  rules,
						  startRule,
						  renderers,
						  startPositionParam,
						  startRotationParam,
						  startSizeParam,
						  cutSizeParam,
						  mailBox
					));
   
   videoActivator->registerEffect(effect_algue, 1.0, 0.0, 96000.0);
   // ALGUE EFFECT DONE ////

   shared_ptr<fx_cadre> effect_cadre(new fx_cadre(demo));

   videoActivator->registerEffect(effect_cadre, 100.0, 0.0, 96000.0);

   demo->addVideoActivator(videoActivator);

   // florentValue
   Bezier1DEasyConfig florentValueGradArray[] =
      {
	 Bezier1DEasyConfig(90000.0, 0.0, 0.0),
	 Bezier1DEasyConfig(94000.0, 1.0, 0.0),
	 Bezier1DEasyConfig(96000.0, 1.0, 0.0),	 	 
	 Bezier1DEasyConfig(100000.0, 0.0, 0.0),
	 Bezier1DEasyConfig(120000.0, 0.0, 0.0)	 
      };
   vector<Bezier1DEasyConfig> florentValueGrad = buildVectorFromArray(florentValueGradArray);
   Bezier1D florentValueBezier = Bezier1D(florentValueGrad);
   shared_ptr<Actuator<double> > florentValueParam(new BezierActuator(florentValueBezier));

   shared_ptr<fx_florent> florent(new fx_florent(demo, florentValueParam));
   videoActivator->registerEffect(florent, 200.0, 90000.0, 100000.0);

/*
   for(int i = 0; i < 100000; i++)
   {
      const double ms = static_cast<double>(i);
      if(effect_algue->processPrepare(ms))
      {
	 info("Glitch at : %lf", ms);
      }
   }
*/

   const double timings[] =
      {
	 12270.000000,
	 13750.000000,
	 15973.000000,
	 16707.000000,
	 17368.000000,
	 17974.000000,
	 18536.000000,
	 18724.000000,
	 19237.000000,
	 19721.000000,
	 20180.000000,
	 20616.000000,
	 20764.000000,
	 21032.000000,
	 21174.000000,
	 21568.000000,
	 21946.000000,
	 22311.000000,
	 22436.000000,
	 22664.000000,
	 22785.000000,
	 23006.000000,
	 23123.000000,
	 23338.000000,
	 23452.000000,
	 23661.000000,
	 23773.000000,
	 23883.000000,
	 24086.000000,
	 24194.000000,
	 24393.000000,
	 24499.000000,
	 24694.000000,
	 24798.000000,
	 24990.000000,
	 25093.000000,
	 25283.000000,
	 25385.000000,
	 25487.000000,
	 25574.000000,
	 25675.000000,
	 25776.000000,
	 25863.000000,
	 25964.000000,
	 26065.000000,
	 26152.000000,
	 26254.000000,
	 26356.000000,
	 26546.000000,
	 26649.000000,
	 26754.000000,
	 26843.000000,
	 26949.000000,
	 27148.000000,
	 27258.000000,
	 27369.000000,
	 27465.000000,
	 27581.000000,
	 27802.000000,
	 27927.000000,
	 28056.000000,
	 28171.000000,
	 28191.000000,
	 28312.000000,
	 28461.000000,
	 28598.000000,
	 28773.000000,
	 28971.000000,
	 29170.000000,
	 29209.000000,
	 29484.000000,
	 32634.000000,
	 35025.000000,
	 36940.000000,
	 38509.000000,
	 39700.000000,
	 39894.000000,
	 41163.000000,
	 42185.000000,
	 43166.000000,
	 43330.000000,
	 44434.000000,
	 45358.000000,
	 46424.000000,
	 47486.000000,
	 48398.000000,
	 49482.000000,
	 50600.000000,
	 51598.000000,
	 52847.000000,
	 54026.000000,
	 54242.000000,
	 55657.000000,
	 58121.000000,
      };

   vector<double> timingsVector = buildVectorFromArray(timings);

   
   
   shared_ptr<TimeSegmentsActivator> audioActivator(new TimeSegmentsActivator());
   shared_ptr<fx_glitch> sfx(new fx_glitch(demo, timingsVector, 0.3));
   audioActivator->registerEffect(sfx, 0.0, 0.0, 96000.0);

   demo->addAudioActivator(audioActivator);
   

   //shared_ptr<Notifier> notifier(new NotifierImpl<fx_glitch, double>(sfx, mailBox));
   //demo->addAudioNotifier(notifier);
}
