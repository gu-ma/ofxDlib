//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


std::vector<matrix<rgb_pixel>> jitter_image(const matrix<rgb_pixel>& img)
{
    // All this function does is make 100 copies of img, all slightly jittered by being
    // zoomed, rotated, and translated a little bit differently.

    // Would like to use thread_local here, but OSX doesn't support it.
    static dlib::thread_specific_data<random_cropper> cropper;
    cropper.data().set_chip_dims(150,150);
    cropper.data().set_randomly_flip(true);
    cropper.data().set_max_object_height(0.99999);
    cropper.data().set_background_crops_fraction(0);
    cropper.data().set_min_object_height(0.97);
    cropper.data().set_translate_amount(0.02);
    cropper.data().set_max_rotation_degrees(3);

    std::vector<mmod_rect> raw_boxes(1), ignored_crop_boxes;
    raw_boxes[0] = shrink_rect(get_rect(img),3);
    std::vector<matrix<rgb_pixel>> crops;

    matrix<rgb_pixel> temp;
    for (int i = 0; i < 100; ++i)
    {
        cropper.data()(img, raw_boxes, temp, ignored_crop_boxes);
        crops.push_back(move(temp));
    }
    return crops;
}



void ofApp::setup()
{
    // The first thing we are going to do is load all our models.  First, since
    // we need to find faces in the image we will need a face detector:
    detector = get_frontal_face_detector();

    // We will also use a face landmarking model to align faces to a standard
    // pose:  (see face_landmark_detection_ex.cpp for an introduction)
    deserialize(ofToDataPath("shape_predictor_68_face_landmarks.dat", true)) >> sp;

    // And finally we load the DNN responsible for face recognition.
    deserialize(ofToDataPath("dlib_face_recognition_resnet_model_v1.dat", true)) >> net;

}


void ofApp::update()
{
    if (loadNext)
    {
        /*
        matrix<rgb_pixel> img;
        load_image(img, "img.png");//argv[1]);
        // Display the raw image on the screen
        // image_window win(img);

        // Run the face detector on the image of our action heroes, and for each face extract a
        // copy that has been normalized to 150x150 pixels in size and appropriately rotated
        // and centered.
        std::vector<matrix<rgb_pixel>> faces;
        for (auto face : detector(img))
        {
            auto shape = sp(img, face);
            matrix<rgb_pixel> face_chip;
            extract_image_chip(img, get_face_chip_details(shape,150,0.25), face_chip);
            faces.push_back(move(face_chip));
            // Also put some boxes on the faces so we can see that the detector is finding
            // them.
            //win.add_overlay(face);
        }

        if (faces.size() == 0)
        {
            cout << "No faces found in image!" << endl;
            //return 1;
        }

        // This call asks the DNN to convert each face image in faces into a 128D vector.
        // In this 128D vector space, images from the same person will be close to each other
        // but vectors from different people will be far apart.  So we can use these vectors to
        // identify if a pair of images are from the same person or from different people.
        std::vector<matrix<float,0,1>> face_descriptors = net(faces);


        // In particular, one simple thing we can do is face clustering.  This next bit of code
        // creates a graph of connected faces and then uses the Chinese whispers graph clustering
        // algorithm to identify how many people there are and which faces belong to whom.
        std::vector<sample_pair> edges;
        for (size_t i = 0; i < face_descriptors.size(); ++i)
        {
            for (size_t j = i+1; j < face_descriptors.size(); ++j)
            {
                // Faces are connected in the graph if they are close enough.  Here we check if
                // the distance between two face descriptors is less than 0.6, which is the
                // decision threshold the network was trained to use.  Although you can
                // certainly use any other threshold you find useful.
                if (length(face_descriptors[i]-face_descriptors[j]) < 0.6)
                    edges.push_back(sample_pair(i,j));
            }
        }
        std::vector<unsigned long> labels;
        const auto num_clusters = chinese_whispers(edges, labels);
        // This will correctly indicate that there are 4 people in the image.
        cout << "number of people found in the image: "<< num_clusters << endl;


        // Now let's display the face clustering results on the screen.  You will see that it
        // correctly grouped all the faces.
*/
        /*
        std::vector<image_window> win_clusters(num_clusters);
        for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
        {
            std::vector<matrix<rgb_pixel>> temp;
            for (size_t j = 0; j < labels.size(); ++j)
            {
                if (cluster_id == labels[j])
                    temp.push_back(faces[j]);
            }
            win_clusters[cluster_id].set_title("face cluster " + cast_to_string(cluster_id));
            win_clusters[cluster_id].set_image(tile_images(temp));
        }
         */


/*

        // Finally, let's print one of the face descriptors to the screen.
        cout << "face descriptor for one face: " << trans(face_descriptors[0]) << endl;

        // It should also be noted that face recognition accuracy can be improved if jittering
        // is used when creating face descriptors.  In particular, to get 99.38% on the LFW
        // benchmark you need to use the jitter_image() routine to compute the descriptors,
        // like so:
        matrix<float,0,1> face_descriptor = mean(mat(net(jitter_image(faces[0]))));
        cout << "jittered face descriptor for one face: " << trans(face_descriptor) << endl;
        // If you use the model without jittering, as we did when clustering the bald guys, it
        // gets an accuracy of 99.13% on the LFW benchmark.  So jittering makes the whole
        // procedure a little more accurate but makes face descriptor calculation slower.
*/
        loadNext = false;
    }
}


void ofApp::draw()
{

}


void ofApp::keyPressed(int key)
{
    loadNext = true;
}
