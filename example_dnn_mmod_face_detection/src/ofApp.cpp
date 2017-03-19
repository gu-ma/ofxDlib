//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    // Load the network data.
    dlib::deserialize(ofToDataPath("mmod_human_face_detector.dat", true)) >> net;

    if (ofLoadImage(pixels, "Crowd.jpg"))
    {
        texture.loadData(pixels);
    }

    dlib::matrix<rgb_pixel> img;

    dlib::load_image(img, ofToDataPath("Crowd.jpg", true));

    // Upsampling the image will allow us to detect smaller faces but will cause
    // the program to use more RAM and run longer.

    float initialSize = img.size();

    //    while (img.size() < 800 * 800)
    //    {
    //        dlib::pyramid_up(img);
    //    }

    // Note that you can process a bunch of images in a std::vector at once and
    // it runs much faster, since this will form mini-batches of images and
    // therefore get better parallelism out of your GPU hardware.  However, all
    // the images must be the same size.  To avoid this requirement on images
    // being the same size we process them individually in this example.
    auto dets = net(img);

    for (auto&& d: dets)
    {
        rectangles.push_back(d);
    }

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    std::cout << "finished computation at " << std::ctime(&end_time);
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s";
    std::cout << std::endl;

    // 16 seconds on MacBook Pro (15-inch, Mid 2012), no CUDA support.
}


void ofApp::draw()
{
    ofBackground(0);
    ofNoFill();
    ofSetColor(ofColor::white);

    texture.draw(0, 0);

    for (auto& r: rectangles)
    {
        std::stringstream ss;
        ss << "Confidence: " << r.detection_confidence << std::endl;
        ss << "Ignore: " << r.ignore;

        ofRectangle rect = toOf(r);

        ofDrawRectangle(rect);
        ofPopMatrix();
        ofDrawBitmapString(ss.str(), rect.getCenter());
    }

}
