//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#pragma once


#include <vector>
#include <dlib/geometry.h>
#include <dlib/image_processing/full_object_detection.h>
#include "ofImage.h"
#include "ofPixels.h"
#include "ofRectangle.h"
#include "ofTypes.h"


namespace ofx {
namespace Dlib {


/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec4<T>& toOf(const dlib::vector<T, 4>& v)
{
    return *reinterpret_cast<const glm::tvec4<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec3<T>& toOf(const dlib::vector<T, 3>& v)
{
    return *reinterpret_cast<const glm::tvec3<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec2<T>& toOf(const dlib::vector<T, 2>& v)
{
    return *reinterpret_cast<const glm::tvec2<T>*>(&v);
}

/// \brief Wrap a dlib::vector to a glm vector with no copies.
/// \param v The input dlib vector.
/// \tparam T vector data type.
/// \returns a const reference to the glm vector.
template<typename T>
inline const glm::tvec1<T>& toOf(const dlib::vector<T, 1>& v)
{
    return *reinterpret_cast<const glm::tvec1<T>*>(&v);
}

/// \brief Copy a dlib::rectangle to ofRectangle.
/// \param r the dlib::rectangle.
/// \returns an ofRectangle.
inline ofRectangle toOf(const dlib::rectangle& r)
{
    return ofRectangle(r.left(), r.top(), r.width(), r.height());
}

/// \brief Copy a dlib::rgb_pixel to an ofColor.
/// \param v The dlib::rgb_pixel to convert.
/// \returns an ofColor.
inline ofColor toOf(const dlib::rgb_pixel& v)
{
    return ofColor(v.red, v.green, v.blue);
}

/// \brief Convert a dlib::rgb_alpha_pixel to an ofColor.
/// \param v The dlib::rgb_alpha_pixel to convert.
/// \returns an ofColor.
inline ofColor toOf(const dlib::rgb_alpha_pixel& v)
{
    return ofColor(v.red, v.green, v.blue, v.alpha);
}


///// \todo Re-work.
//inline void toDlib(const ofPixels& input,
//                   dlib::array2d<dlib::rgb_pixel>& output)
//{
//    auto width = input.getWidth();
//    auto height = input.getHeight();
//    auto chans = input.getNumChannels();
//
//    const unsigned char* data = input.getData();
//
//    output.set_size(static_cast<long>(height), static_cast<long>(width));
//
//    for (std::size_t n = 0; n < height; ++n)
//    {
//        const unsigned char* v =  &data[n * width *  chans];
//
//        for (std::size_t m = 0; m < width; ++m)
//        {
//            if (chans == 1)
//            {
//                auto p = v[m];
//                dlib::assign_pixel(output[n][m], p);
//            }
//            else
//            {
//                dlib::rgb_pixel p;
//                p.red = v[m * 3];
//                p.green = v[m * 3 + 1];
//                p.blue = v[m * 3 + 2];
//                dlib::assign_pixel(output[n][m], p);
//            }
//        }
//    }
//}


///// \todo Re-work.
//inline ofPixels toOf(const dlib::matrix<unsigned char>& input)
//{
//    auto w = std::size_t(input.nc());
//    auto h = std::size_t(input.nr());

//    ofPixels output;

//    output.allocate(w, h, 1);

//    for (std::size_t y = 0; y < h; ++y)
//    {
//        for (std::size_t x = 0; x < w; ++x)
//        {
//            output.setColor(x, y, ofColor(input(y, x)));
//        }
//    }

//    return output;
//}


//template<typename T>
//inline ofPixels copy(const dlib::matrix<T>& in)
//{
//    ofPixels pixels;
//
//    auto channels = dlib::pixel_traits<T>::num;
//    auto height = dlib::num_rows(in);
//    auto width = dlib::num_columns(in);
//
//    pixels.allocate(width, height, channels);
//
//    for (auto y = 0; y < height; ++y)
//    {
//        for (auto x = 0; x < width; ++x)
//        {
//            pixels.setColor(x, y, toOf(in(y, x)));
//        }
//    }
//
//    return pixels;
//}            img.load("Puppy.jpg");


/// \brief Deterimne the best openFrameworks ofPixelFormat.
/// \returns the best pixel format given the templated parameter.
/// \tparam The pixel_type called like getPixelFormat<MY_FORMAT>();
template<typename pixel_type>
inline ofPixelFormat getPixelFormat()
{
    if (std::is_same<pixel_type, dlib::rgb_pixel>::value)
    {
        return OF_PIXELS_RGB;
    }
    else if (std::is_same<pixel_type, dlib::rgb_alpha_pixel>::value)
    {
        return OF_PIXELS_RGBA;
    }
    else if (std::is_same<pixel_type, dlib::bgr_pixel>::value)
    {
        return OF_PIXELS_BGR;
    }
    else if (std::is_same<pixel_type, dlib::hsi_pixel>::value)
    {
        ofLogWarning("getPixelFormat") << "HSI pixel format not supported, using RGB.";
        return OF_PIXELS_RGB;
    }
    else if (std::is_same<pixel_type, dlib::lab_pixel>::value)
    {
        ofLogWarning("getPixelFormat") << "LAB pixel format not supported, using RGB.";
        return OF_PIXELS_BGR;
    }
    else if (dlib::pixel_traits<pixel_type>::num == 1)
    {
        return OF_PIXELS_GRAY;
    }
    else
    {
        ofLogWarning("getPixelFormat") << "Unknown pixel format.";
        return OF_PIXELS_UNKNOWN;
    }
}


// TODO:
//
// - hsi_pixel, lab_pixel do not have a corresponding OF_PIXELS_* type.
// - When converting from a non-char pixel type, the dlib values stay in 0-255
//   range.
// - reduce ...
// - reduce return type lengths


/// \brief Convert const dlib::matrix_exp to an ofPixels object.
/// \tparam E the underlying expression type.
template<typename E>
inline ofPixels_<typename dlib::pixel_traits<typename dlib::matrix_traits<E>::type>::basic_pixel_type> toOf(const dlib::matrix_exp<E>& in)
{
    // Extract the underlying type from stored in the dlib::matrix_exp.
    typedef typename dlib::matrix_traits<E>::type pixel_type;

    // For dlib::matrix_exp, we must assign the expression to a concrete type
    // in order to resolve the expression. We cannot use an `auto` here.
    //
    // We can now instantiate our dlib::matrix.
    dlib::matrix<pixel_type> m = in;

    // Extract the underlying basic_pixel_type from the pixel_type.
    // In some cases, this may be the same, but in cases with custom-defined
    // dlib::pixel_traits, this will be different. For example, a data_type
    // of dlib::rgb_pixel has a basic_pixel_type of unsigned char, which we
    // need to know to correctly template our ofPixels_<unsigned char> object.
    typedef typename dlib::pixel_traits<pixel_type>::basic_pixel_type basic_pixel_type;

    // We can now instantiate our ofPixels_ type.
    ofPixels_<basic_pixel_type> pixels;

    // Since a dlib::matrix also implements the dlib::generic_image template
    // interface, we can use the dlib::generic_image helpers to determine our
    // pixel allocation.
    pixels.setFromPixels(reinterpret_cast<const basic_pixel_type*>(dlib::image_data(m)),
                         dlib::num_columns(m),
                         dlib::num_rows(m),
                         getPixelFormat<pixel_type>());

    return pixels;
}



template<typename pixel_type>
inline ofPixels_<typename dlib::pixel_traits<pixel_type>::basic_pixel_type> toOf(dlib::array2d<pixel_type>& in)
{
    typedef typename dlib::pixel_traits<pixel_type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> out;

    out.setFromExternalPixels(reinterpret_cast<basic_pixel_type*>(dlib::image_data(in)),
                              dlib::num_columns(in),
                              dlib::num_rows(in),
                              getPixelFormat<pixel_type>());
    return out;
}


template<typename pixel_type>
inline ofPixels_<typename dlib::pixel_traits<pixel_type>::basic_pixel_type> toOf(const dlib::array2d<pixel_type>& in)
{
    typedef typename dlib::pixel_traits<pixel_type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> out;

    out.setFromPixels(reinterpret_cast<const basic_pixel_type*>(dlib::image_data(in)),
                      dlib::num_columns(in),
                      dlib::num_rows(in),
                      getPixelFormat<pixel_type>());

    return out;
}


template<typename pixel_type>
inline ofPixels_<typename dlib::pixel_traits<pixel_type>::basic_pixel_type> toOf(dlib::matrix<pixel_type>& in)
{
    typedef typename dlib::pixel_traits<pixel_type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> out;

    out.setFromExternalPixels(reinterpret_cast<basic_pixel_type*>(dlib::image_data(in)),
                              dlib::num_columns(in),
                              dlib::num_rows(in),
                              getPixelFormat<pixel_type>());

    return out;
}


template<typename pixel_type>
inline ofPixels_<typename dlib::pixel_traits<pixel_type>::basic_pixel_type> toOf(const dlib::matrix<pixel_type>& in)
{
    typedef typename dlib::pixel_traits<pixel_type>::basic_pixel_type basic_pixel_type;

    ofPixels_<basic_pixel_type> out;

    out.setFromPixels(reinterpret_cast<const basic_pixel_type*>(dlib::image_data(in)),
                      dlib::num_columns(in),
                      dlib::num_rows(in),
                      getPixelFormat<pixel_type>());

    return out;
}


/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::rectangle& in, double scaler)
{
    in.left() *= scaler;
    in.top() *= scaler;
    in.right() *= scaler;
    in.bottom() *= scaler;
}

/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::full_object_detection& in, double scaler)
{
    for (unsigned int i = 0; i < in.num_parts(); ++i)
    {
        // Must be done individually.
        in.part(i).x() *= scaler;
        in.part(i).y() *= scaler;
    }

    scale(in.get_rect(), scaler);

}

/// \brief Scale the given object detection by the given amount.
/// \param v The object to be scaled.
/// \param scale The amount the scale the object by.
inline void scale(dlib::mmod_rect& in, double scaler)
{
    scale(in.rect, scaler);
}

/// \brief A helper function to convert an ofPixels object to grayscale.
/// \param pixels The pixels to convert.
/// \returns A grayscale version of the image.
inline ofPixels toGrayscale(const ofPixels& pixels)
{
    // Quick and dirty hack to use existing FreeImage algorithm.
    ofImage img;
    img.setUseTexture(false);
    img.setFromPixels(pixels);
    img.setImageType(OF_IMAGE_GRAYSCALE);
    return img.getPixels();;
}


} } // namespace ofx::Dlib
