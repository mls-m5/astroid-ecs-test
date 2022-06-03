#pragma once

#include "components.h"
#include "sdlpp/render.hpp"

void drawAstroid(sdl::RendererView renderer, const Position &, const Visible &);
void drawShip(sdl::RendererView renderer, const Position &, const Visible &);
void drawProjectile(sdl::RendererView renderer,
                    const Position &,
                    const Visible &);

void draw(entt::registry &reg, sdl::RendererView renderer);
