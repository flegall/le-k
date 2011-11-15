#ifndef base_effects_hh
#define base_effects_hh

#include <effect.hh>
#include <color.hh>
#include <actuator.hh>
#include <base_actuators.hh>

// Clears the screen
class FX_Clear : public VisualEffect
{
   public:
      // Builds a default clear effect so you only have to specify one actuator : colorParameter, all the others are set to true.
      static shared_ptr<FX_Clear> BuildDefaultClear(Demo* demo);

      // Constructor
      FX_Clear(Demo * demo);
      // process method.
      virtual void process(const double ms);
      // Color parameter
      virtual void setColorParam(shared_ptr< Actuator<Color> > colorParameter);
      // Should the fx clear the color buffer ?
      virtual void setColorClearParam(shared_ptr< Actuator<bool> > colorClearParam);
      // Should the fx clear the depth buffer ?
      virtual void setDepthClearParam(shared_ptr< Actuator<bool> > depthClearParam);
      // Should the fx clears the accumulation buffer ?
      virtual void setAccumClearParam(shared_ptr< Actuator<bool> > accumClearParam);
      // Should the fx clear the stencil buffer ?
      virtual void setStencilClearParam(shared_ptr< Actuator<bool> > stencilClearParam);
   private:
      shared_ptr< Source<Color> > colorSource;
      shared_ptr< Source<bool> > colorClearSource;
      shared_ptr< Source<bool> > depthClearSource;
      shared_ptr< Source<bool> > accumClearSource;
      shared_ptr< Source<bool> > stencilClearSource;
};

class FX_CL0R : public VisualEffect
{
   public:
      FX_CL0R(Demo* demo);
      void process(const double ms);
};

class SFX_NULL : public AudioEffect
{
   public:
      SFX_NULL(Demo* demo);
      void process(const double ms, AudioSample& sample, const int frequency);
};

#endif
