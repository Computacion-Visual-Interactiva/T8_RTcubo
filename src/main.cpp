#include "camara.h"
#include "rtcubo.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "cube.h"

int main()
{
    hittable_list world;

    world.add(make_shared<cube>(point3(0, 0, -1), 0.5));      // cubo
    world.add(make_shared<cube>(point3(0, -100.5, -1), 200)); // plano 100

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.render(world);
}