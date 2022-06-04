#include "game.h"
#include "components.h"
#include <random>

namespace {

auto gen = std::mt19937{std::random_device{}()};

void handleCollisions(entt::registry &reg) {
    auto vdist = std::normal_distribution(0.f, 2.f);

    auto isCollision = [](Position pos1, Position pos2, float size) {
        auto dx = pos2.x - pos1.x;
        auto dy = pos2.y - pos1.y;

        auto dist2 = dx * dx + dy * dy;
        return dist2 < size * size;
    };

    for (auto [e1, pos1, col] : reg.view<Position, Collidable>().each()) {
        for (auto [e2, pos2, proj] : reg.view<Position, Projectile>().each()) {
            if (proj.damage == 0) {
                continue;
            }

            if (isCollision(pos1, pos2, col.size)) {
                setDead(reg, e1);
                setDead(reg, e2);
                proj.damage = 0;

                createExplosion(reg, pos1);

                if (col.size > 3.f) {
                    for (size_t i = 0; i < 3; ++i) {
                        auto nx = vdist(gen);
                        auto ny = vdist(gen);
                        createAstroid(reg,
                                      {pos1.x + nx * 2, pos1.y + ny * 2},
                                      {vdist(gen), vdist(gen)},
                                      Collidable{col.size / 2.f});
                    }
                }
            }
        }
    }

    for (auto [e1, pos1, col] : reg.view<Position, Collidable>().each()) {
        for (auto [e2, pos2, proj] :
             reg.view<Position, Controllable>().each()) {
            if (isCollision(pos1, pos2, 5)) {
                setDead(reg, e1);
                setDead(reg, e2);

                createExplosion(reg, pos1);
                createExplosion(reg, pos2);
            }
        }
    }
}

void triggerBomb(entt::registry &reg, Position pos) {
    for (auto [e, col, p] : reg.view<Collidable, Position>().each()) {
        if (col.size > 3) {
            continue;
        }
        auto dx = p.x - pos.x;
        auto dy = p.y - pos.y;
        auto dist2 = dx * dx + dy * dy;
        if (!reg.any_of<Lifetime>(e)) {
            reg.emplace<Lifetime>(e, Lifetime{std::sqrt(dist2) / 20.f, true});
        }
    }
}

void handlePlayer(entt::registry &reg, double t) {
    for (auto [e, con, pos, vel] :
         reg.view<Controllable, Position, Velocity>().each()) {
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);
        vel.rot = con.rot / 2.f;

        vel.x += sx * con.throttle * t;
        vel.y += sy * con.throttle * t;
    }

    for (auto [e, con, pos, vel, weap] :
         reg.view<Controllable, Position, Velocity, Weapon>().each()) {
        const float projectileSpeed = 10.;
        auto sx = std::sin(pos.a);
        auto sy = std::cos(pos.a);

        if (con.fire) {
            while (weap.currentCooldown > 0) {
                createProjectile(reg,
                                 pos,
                                 {
                                     static_cast<float>(vel.x + sx * 10.),
                                     static_cast<float>(vel.y + sy * 10.),
                                 });
                weap.currentCooldown -= 1;
            }
        }

        weap.currentCooldown += t;
        weap.currentCooldown = std::min(weap.currentCooldown, weap.cooldown);
    }

    for (auto [e, con, pos, sec] :
         reg.view<Controllable, Position, SecondaryWeapon>().each()) {
        if (con.secondary) {
            //            if (sec.num) {
            //                sec.num = 0;
            //            triggerBomb(reg, pos);
            createHomingMissiles(reg, pos, {std::sin(pos.a), std::cos(pos.a)});
            //            }
        }
    }
}

} // namespace

Game::Game(entt::registry &reg, float width, float height)
    : _width{width}
    , _height{height} {

    reg.on_construct<Collidable>().connect<&Game::astroidCreated>(this);
    reg.on_destroy<Collidable>().connect<&Game::astroidDestroyed>(this);

    startLevel(reg);

    createPlayer(reg, {width / 2.f, height / 2.f});

    reg.on_destroy<Controllable>().connect<&Game::onPlayerDeath>();
}

void Game::update(entt::registry &reg, double t) {
    for (auto [e, lifetime, pos] : reg.view<Lifetime, Position>().each()) {
        lifetime.t -= t;
        if (lifetime.t < 0) {
            if (lifetime.shouldExplode) {
                createExplosion(reg, pos);
            }
            reg.destroy(e);
        }
    }

    handlePlayer(reg, t);

    for (auto [e, pos, proj] : reg.view<Position, ParticleSmoke>().each()) {
        auto dist = std::normal_distribution<float>{0, 1};
        createParticle(reg, pos, {dist(gen), dist(gen)});
    }

    for (auto [e, pos, vel] : reg.view<Position, Velocity>().each()) {
        pos.x += vel.x * t;
        pos.y += vel.y * t;
        pos.a += vel.rot * t;

        if (pos.x < -10.f) {
            pos.x += _width + 10.f;
        }
        else if (pos.x > _width + 10.f) {
            pos.x -= _width + 10.f;
        }
        if (pos.y < -10.f) {
            pos.y += _height + 10.f;
        }
        else if (pos.y > _height + 10.f) {
            pos.y -= _height + 10.f;
        }
    }

    for (auto [e, pos, vel, homing] :
         reg.view<Position, Velocity, Homing>().each()) {
        if (!reg.valid(homing.target)) {
            reg.emplace<Dead>(e);
            continue;
        }

        homing.timeLeft -= homing.speed * t;
        auto targetPos = reg.get<Position>(homing.target);
        auto d = targetPos - homing.pos;

        auto inv = 1.f - homing.timeLeft;

        pos = homing.pos * homing.timeLeft + targetPos * inv +
              Position{homing.direction.x, homing.direction.y} *
                  homing.timeLeft * inv * 100.;
        pos.a = std::atan2(d.x, d.y);
    }

    handleCollisions(reg);

    {
        auto view = reg.view<Dead>();
        reg.destroy(view.begin(), view.end());
    }
}

void Game::onPlayerDeath(entt::registry &reg, entt::entity) {
    createPlayer(reg, {width / 2.f, height / 2.f});
}

void Game::astroidCreated(entt::registry &, entt::entity) {
    ++numAstroids;
}

void Game::astroidDestroyed(entt::registry &reg, entt::entity) {
    --numAstroids;
    if (numAstroids == 0) {
        startLevel(reg);
    }
}

void Game::startLevel(entt::registry &registry) {
    auto gen = std::mt19937{std::random_device{}()};
    auto xdist = std::uniform_real_distribution(0.f, static_cast<float>(width));
    auto ydist =
        std::uniform_real_distribution(0.f, static_cast<float>(height));

    auto dist = std::normal_distribution(0.f, 2.f);
    auto linear = std::uniform_real_distribution(0.f, 3.14f);
    for (size_t i = 0; i < 10; ++i) {
        createAstroid(registry,
                      Position{xdist(gen), ydist(gen), linear(gen)},
                      {dist(gen), dist(gen), dist(gen) / 10.f},
                      {10.f});
    }
}
