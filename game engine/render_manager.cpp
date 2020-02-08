#include "render_manager.h"

render_manager::render_manager () :
	objects_ (),
	lighting_ (),
	opaques_ (),
	lighting_shader_ (nullptr),
	loaded_ (false)
{
}

render_manager::~render_manager ()
{
	if (loaded_ && lighting_shader_)
		delete lighting_shader_;
}

render_manager::objects_array& render_manager::get_objects_array ()
{
	return objects_;
}

render_manager::lights_array& render_manager::get_lights_array ()
{
	return lighting_;
}

render_manager:: opaques_array& render_manager::get_opaques_array ()
{
	return opaques_;
}

void render_manager::set_shader (sf::Shader * shader)
{
	lighting_shader_ = shader;
}

void render_manager::load_shader (std::string shader_path)
{
	loaded_ = true;

	lighting_shader_ = new sf::Shader ();
	lighting_shader_->loadFromFile (shader_path, sf::Shader::Type::Fragment);
}

void render_manager::render (sf::RenderTarget& target)
{
	int bounds_size = 64;
	int lights_size = 64;

	sf::Glsl::Vec2* bounds = new sf::Glsl::Vec2[bounds_size];
	sf::Glsl::Vec2* lights = new sf::Glsl::Vec2[lights_size];

	for (int i = 0; i < opaques_.size (); i++)
	{
		sf::FloatRect rect = opaques_[i]->get_bounds ();

		bounds[i * 4    ] = sf::Glsl::Vec2(rect.left             , rect.top              );
		bounds[i * 4 + 1] = sf::Glsl::Vec2(rect.left + rect.width, rect.top              );
		bounds[i * 4 + 2] = sf::Glsl::Vec2(rect.left + rect.width, rect.top + rect.height);
		bounds[i * 4 + 3] = sf::Glsl::Vec2(rect.left             , rect.top + rect.height);
	}

	for (int i = 0; i < lighting_.size (); i++)
		lights[i] = lighting_[i]->get_position ();

	lighting_shader_->setUniform ("aa", 0.0f);
	lighting_shader_->setUniform ("bb", 1.1f);

	lighting_shader_->setUniform ("current", sf::Shader::CurrentTexture);

	for (auto&& el : objects_)
		target.draw (el->get_drawable (), lighting_shader_);


	delete[] bounds;
	delete[] lights;
}