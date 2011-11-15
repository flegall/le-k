#include <base_effects.hh>
#include <opengl.hh>

shared_ptr<FX_Clear> FX_Clear::BuildDefaultClear(Demo* demo)
{
   shared_ptr<FX_Clear> clear(new FX_Clear(demo));
   shared_ptr< ConstantActuator<bool> > cstActuatorTrue(new ConstantActuator<bool>(true));
   clear->setColorClearParam(cstActuatorTrue);
   clear->setDepthClearParam(cstActuatorTrue);
   clear->setAccumClearParam(cstActuatorTrue);
   clear->setStencilClearParam(cstActuatorTrue);
   
   return clear;
}

FX_Clear::FX_Clear(Demo * demo) : VisualEffect(demo)
{
}

void FX_Clear::process(const double ms)
{
   FLbitfield field;
   if(colorClearSource->getValue(ms))
   {
      const Color color = colorSource->getValue(ms);
      flClearColor(color.red, color.green, color.blue, color.alpha);
      field = FL_COLOR_BUFFER_BIT;
   }
   else
   {
      field = 0;
   }

   field |= depthClearSource->getValue(ms) ? FL_DEPTH_BUFFER_BIT : 0;
   field |= accumClearSource->getValue(ms) ? FL_ACCUM_BUFFER_BIT : 0;
   field |= stencilClearSource->getValue(ms) ? FL_STENCIL_BUFFER_BIT : 0;
   
   flClear(field);
}

void FX_Clear::setColorParam(shared_ptr< Actuator<Color> > colorParameter)
{
   colorSource = colorParameter->registerActuator();
}

void FX_Clear::setColorClearParam(shared_ptr< Actuator<bool> > colorClearParam)
{
   colorClearSource = colorClearParam->registerActuator();
}

void FX_Clear::setDepthClearParam(shared_ptr< Actuator<bool> > depthClearParam)
{
   depthClearSource = depthClearParam->registerActuator();
}

void FX_Clear::setAccumClearParam(shared_ptr< Actuator<bool> > accumClearParam)
{
   accumClearSource = accumClearParam->registerActuator();
}

void FX_Clear::setStencilClearParam(shared_ptr< Actuator<bool> > stencilClearParam)
{
   stencilClearSource = stencilClearParam->registerActuator();
}


FX_CL0R::FX_CL0R(Demo* demo) : VisualEffect(demo)
{
}

void FX_CL0R::process(const double ms)
{
   flClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   flClear(FL_COLOR_BUFFER_BIT);
}

SFX_NULL::SFX_NULL(Demo* demo) : AudioEffect(demo)
{
}

void SFX_NULL::process(const double ms, AudioSample& sample, const int frequency)
{
   sample.left = 0.0;
   sample.right = 0.0;
}



