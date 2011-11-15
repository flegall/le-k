#include <font.hh>
#include <fx_florent.hh>
#include <opengl.hh>


fx_florent::fx_florent(Demo* const demo, shared_ptr<Actuator<double> > param) :
   VisualEffect(demo),
   fontRenderer(constants::console_font_file, 24, constants::console_font_style),
   source(param->registerActuator())
{
   info("");
}

void fx_florent::process(const double ms)
{
   flSet2DMode();
   FontRenderSquare square(0.0, static_cast<double>(constants::demo_width), 0.0, static_cast<double>(constants::demo_height));
   FontRenderConstraints constraints(FontRenderConstraints::XCenter, FontRenderConstraints::YCenter, square);

   const string text = "le k. \nflorent . lobstre";

   FontRenderPlan plan = fontRenderer.getRenderDimensions(text, constraints);

   fontRenderer.renderText(text, constraints);

}

fx_florent::~fx_florent()
{
   
}


