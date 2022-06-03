#include "drawcomponents.h"

namespace {

struct Model {
    struct Point {
        float x = 0;
        float y = 0;
    };

    Model(std::string_view src) {
        int numPoints = 0;
        for (int c : src) {
            if (isdigit(c)) {
                numPoints = std::max(numPoints, c - '0' + 1);
            }
        }

        points.resize(numPoints);

        int x = 0;
        int y = 0;

        float cx = 0;
        float cy = 0;
        for (auto c : src) {
            if (isdigit(c)) {
                points.at(c - '0') = {static_cast<float>(x),
                                      static_cast<float>(y)};
            }
            else if (c == 'c') {
                cx = x;
                cy = y;
            }

            if (c == '\n') {
                ++y;
                x = 0;
            }
            else {
                ++x;
            }
        }

        for (auto &p : points) {
            p.x = (p.x - cx) / 3.;
            p.y -= cy;
        }
    }

    void draw(sdl::RendererView &renderer, const Position &pos) const {
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);

        auto transform = [&](Point p) {
            return Point{pos.x + p.x * sy + p.y * sx,
                         pos.y + p.y * sy - p.x * sx};
        };

        auto drawLine = [&](Point p1, Point p2) {
            auto t1 = transform(p1);
            auto t2 = transform(p2);
            renderer.drawLine(t1.x, t1.y, t2.x, t2.y);
        };
        for (size_t i = 1; i < points.size(); ++i) {
            auto p1 = points.at(i - 1);
            auto p2 = points.at(i);
            drawLine(p1, p2);
        }
        drawLine(points.front(), points.back());
    }

    std::vector<Point> points;
};

auto astroidModel = Model{R"_(
                 1
      0                      2


                 c


     5                       3
                 4
)_"};

auto shipModel = Model{R"_(
      3                      1

                 2



                 c



                 0
)_"};

auto projectileModel = Model{R"_(
    0




  3 c 1
    2
)_"};

} // namespace

void drawAstroid(sdl::RendererView renderer,
                 const Position &pos,
                 const Visible &vis) {
    astroidModel.draw(renderer, pos);
}

void drawShip(sdl::RendererView renderer,
              const Position &pos,
              const Visible &) {
    shipModel.draw(renderer, pos);
}

void drawProjectile(sdl::RendererView renderer,
                    const Position &pos,
                    const Visible &) {
    projectileModel.draw(renderer, pos);
}

void draw(entt::registry &reg, sdl::RendererView renderer) {
    renderer.drawColor({255, 255, 255, 255});
    for (auto [e, pos, vis] : reg.view<Position, Visible>().each()) {
        vis.f(renderer, pos, vis);
    }
}
