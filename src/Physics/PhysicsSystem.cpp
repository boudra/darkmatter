#include "PhysicsSystem.hpp"
#include "Core/EntityManager.hpp"
#include "Core/Engine.hpp"

#include "Components/RenderComponent.hpp"

namespace dm {
   
PhysicsSystem::PhysicsSystem() : SystemBase("Physics")
{

}

PhysicsSystem::~PhysicsSystem()
{

}

bool PhysicsSystem::initialize()
{
 return true;
}

void PhysicsSystem::render(float interpolation)
{

}

bool check_aabb(const AABB& a, const AABB& b) {

   return (a.max.x > b.min.x &&
         a.min.x < b.max.x &&
         a.max.y > b.min.y &&
         a.min.y < b.max.y &&
         a.max.z > b.min.z &&
         a.min.z < b.max.z);
}

void PhysicsSystem::update()
{

   float speed = 0.0f;
   auto& components = COMPONENTS(PhysicsComponent);
   Matrix4f rotation;

   int cnt = 0;

   for(PhysicsComponent& c : components)
   {

      c.velocity += (c.acceleration / c.mass);
      c.velocity *= c.friction;

      speed = c.velocity.length();

      if(speed > c.max_velocity)
      {
         c.velocity = c.velocity.normalize() * c.max_velocity;
      }

   //   c.m_acceleration *= c.m_friction;

      //c.position += c.velocity;

      c.aabb.min = c.position;
      c.aabb.max = c.position;

      c.aabb.max += c.size;

      if(c.parent->has_component<Render2d>())
      {

         /*
         LOG(DEBUG, "SCALE " << scale);
         LOG(DEBUG, "POSIT" << c.position);
         LOG(DEBUG, "PIVOT" << c.scale_origin);
         */

         c.rotation.get_matrix(rotation);

         c.transform.identity();

         Matrix4f translate_matrix(1),
                  scale_matrix(1), 
                  rotation_matrix(1),
                  scale_relative(1);

         translate_matrix.translate(c.position);

         scale_matrix.translate((c.scale_origin));
         scale_matrix.scale(c.scale);
         scale_matrix.translate(-(c.scale_origin));

         scale_relative.scale(c.size);

         scale_matrix = scale_matrix * scale_relative;

         rotation_matrix.translate(c.rotation_origin);
         rotation_matrix = rotation_matrix * rotation; 
         rotation_matrix.translate(-(c.rotation_origin));

         c.transform = translate_matrix * rotation_matrix * scale_matrix * c.transform;

         c.parent->component<Render2d>().set_transform(c.transform);

      }

      cnt++;

   }

   // LOG(DEBUG, cnt);

}


} /* namespace dm */
