// This code generates a PPM (Portable Pixmap) image file that is 256 x 256
// The color values for each pixel are determined based on the pixel's position

// To compile:
// make

// To clean
// make clean

#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

color ray_color(const ray& r){
    // Calculate the unit direction of the ray
    vec3 unit_direction = unit_vector(r.direction());

    // Map the y-component of the unit direction to the range [0,1]
    auto a = 0.5 * (unit_direction.y() + 1.0);

    // Linearly interpolate between white and blue based on the mapped y-component
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main(){

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height and ensure that it's at least 1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    auto camera_center = point3(0,0,0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    // Trace ray through each pixel of the image, compute the color along each ray, and output
    // resulting colors to the output stream
    for(int j = 0; j < image_height; ++j){
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; ++i){
            // Calculate the world-space position of the center of the current pixel
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);

            // Calculate the direction of the ray from the camera to the pixel
            auto ray_direction = pixel_center - camera_center;

            // Create a ray with the calculated direction, starting from the camera position
            ray r(camera_center, ray_direction);

            // Compute the color along the ray using the ray_color function
            color pixel_color = ray_color(r);

            // Write the color information to the output stream 
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone. \n";
}