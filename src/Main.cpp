
#include "EasyBMP.h"

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <future>
#include <mutex>

#include "Fractal.h"
#include "FractalMessage.h"

#include "TextMessage.h"
#include "ThreadSafeQueue.h"
using namespace std;

typedef ThreadSafeQueue <std::shared_ptr<FractalMessage>> FractalMessageQueue;
typedef std::vector<std::shared_ptr<std::thread>> ThreadVector;

std::mutex cout_guard;

FractalMessageQueue Fqueue;

void render_fractal(std::shared_ptr<FractalMessage> msg) {

    // actually render a tile of the fractal by feeding Fractal.h::fractal with the data
    // that outputs to the output image 

    fractal(msg->get_output_image(), msg->get_left(), msg->get_top(), msg->get_x_size(), msg->get_y_size(),
        msg->get_min_x(), msg->get_min_y(), msg->get_max_x(), msg->get_max_y(),
        msg->get_image_x(), msg->get_image_y());
};

void do_tile(EasyBMP::Image* img, float left, float top, float xsize, float ysize, float img_min_x, float img_min_y, float img_max_x, float img_max_y, float img_x_size, float img_y_size) {

    // make a message, and render it directly!
    std::shared_ptr<FractalMessage> msg = std::make_shared<FractalMessage>(img, left, top, xsize, ysize, img_min_x, img_min_y, img_max_x, img_max_y, img_x_size, img_y_size);
    Fqueue.push(msg);

};

void consumer_func(FractalMessageQueue * Fqueue) {

    // many consumers will consume the messages - each consumer
    // thread runs this function

    while (true) {

        std::shared_ptr<FractalMessage> msg = Fqueue->pop();
        if (msg == nullptr) {
            return;
        }

        // otherwise we output the message
        render_fractal(msg);
    }

}

void line_output(const std::string& out) {
    // We don't want to see overlapping lines of output
    // so, we synchronize this to output only whole lines at a time
    // this is a "shared resource"
    std::scoped_lock(cout_guard);
    std::cout << out << std::endl;
}


int main()
{
    
    // Define an RGB color for black
    EasyBMP::RGBColor black(0, 0, 0);

    // make an image initialized with black .  Image size is 512x512
    // Image size should always be POT (power of 2)
    size_t img_size_x = 512;
    size_t img_size_y = 512;

    EasyBMP::Image img(img_size_x, img_size_y, "output.bmp", black);

    // the number of tiles in X & Y 
    size_t num_tiles_x = 4;
    size_t num_tiles_y = 4;

    // this is the size of the tiles in the output image
    size_t img_tile_size_x = img_size_x / num_tiles_x;
    size_t img_tile_size_y = img_size_y / num_tiles_y;

    // the overall area we are rendering in "fractal space"    
    float left = -1.75; float top = -0.25;
    float xsize = 0.25; float ysize = 0.45;

    // how big each tile is in x,y in "fractal space"
    float fractal_tile_size_x = xsize / num_tiles_x;
    float fractal_tile_size_y = ysize / num_tiles_y;


    for (size_t i = 0; i < num_tiles_x; i++) {
        for (size_t j = 0; j < num_tiles_y; j++) {

            // calculate the coords for the output tile, in bitmap(img) space 
            size_t img_tile_min_x = 0 + (img_tile_size_x * i);
            size_t img_tile_min_y = 0 + (img_tile_size_y * j);

            size_t img_tile_max_x = img_tile_min_x + img_tile_size_x;
            size_t img_tile_max_y = img_tile_min_y + img_tile_size_y;

            // calculate the coords for the output tile in fractal space
            float fractal_tile_min_x = left + (i * fractal_tile_size_x);
            float fractal_tile_min_y = top + (j * fractal_tile_size_y);

            float fractal_tile_max_x = fractal_tile_min_x + fractal_tile_size_x;
            float fractal_tile_max_y = fractal_tile_min_y + fractal_tile_size_y;

            cout << "TILE: " << i << "," << j << " img_tile_min_x : " << img_tile_min_x << " img_tile_min_y : " << img_tile_min_y  \
                << " img_tile_max_x : " << img_tile_max_x << " img_tile_max_y : " << img_tile_max_y << endl;

            cout << "FRACTAL_TILE: " << i << "," << j << " fractal_tile_min_x: " << fractal_tile_min_x << ", fractal_tile_min_y: " << fractal_tile_min_y  \
                << ", fractal_tile_max_x: " << fractal_tile_max_x << ", fractal_tile_max_y: " << fractal_tile_max_y << endl;

            do_tile(&img, fractal_tile_min_x, fractal_tile_min_y, fractal_tile_max_x, fractal_tile_max_y, img_tile_min_x, img_tile_min_y, img_tile_max_x, img_tile_max_y, img_size_x, img_size_y);

        }
    }

    ThreadVector threads;
    for (size_t i = 0; i < 8; i++) {
        std::shared_ptr<std::thread> thread = std::make_shared<std::thread>(consumer_func, &Fqueue);
        threads.push_back(thread);
    }
    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        line_output("Queue size is " + std::to_string(Fqueue.size()));
    } while (Fqueue.size() != 0);

    Fqueue.abort();
  
    for (auto thread : threads) {
        thread->join();
    }

    img.Write();
    return 0;
}

