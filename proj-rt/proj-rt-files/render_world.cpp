#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<double>::max();
    Hit actualHit;
    for (unsigned int i = 0; i < objects.size(); i++){
        Hit possibleHit = objects.at(i)->Intersection(ray, 0);
        min_t = possibleHit.dist;
        if (possibleHit.dist > small_t && possibleHit.dist <= min_t){
            actualHit = possibleHit;
            min_t = possibleHit.dist;
        }
    }
    return actualHit;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
    // set up the initial view ray here
    
    vec3 endpoint = camera.position;
    vec3 direction = (camera.World_Position(pixel_index) - endpoint).normalized();
    Ray ray = Ray(endpoint, direction);
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    if (Closest_Intersection(ray).dist >= small_t) { // if there is an intersection, set color to intersection surface point
        color = Closest_Intersection(ray).object->material_shader->Shade_Surface(ray, ray.Point(Closest_Intersection(ray).dist), Closest_Intersection(ray).object->Normal(ray.Point(Closest_Intersection(ray).dist), Closest_Intersection(ray).part), recursion_depth);
        /* shade_surface parameters:
 *             1. ray = ray
 *                         2. vec3 intersection_point = ray.point of closest intersection.distance
 *                                     3. vec3 normal = closest intersection object's normal (closest intersection.distance, closest intersection.part)
 *                                                 4. recursion_depth = recursion_depth
 *                                                          */
    } else if (Closest_Intersection(ray).dist < small_t) {
        color = {0,0,0};
    }
    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
