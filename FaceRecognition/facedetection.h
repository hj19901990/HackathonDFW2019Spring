#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>
#include <dlib/dnn.h>
#include <dlib/clustering.h>
#include <dlib/string.h>
#include <dlib/image_io.h>
#include <algorithm>
#include "CloudServer.h"

#include<io.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>



#define XSUCESS 1
#define XFAIL 0


using namespace std;
using namespace dlib;
using namespace cv::ml;
namespace fs = std::experimental::filesystem;


template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual = add_prev1<block<N, BN, 1, tag1<SUBNET>>>;

template <template <int, template<typename>class, int, typename> class block, int N, template<typename>class BN, typename SUBNET>
using residual_down = add_prev2<avg_pool<2, 2, 2, 2, skip1<tag2<block<N, BN, 2, tag1<SUBNET>>>>>>;

template <int N, template <typename> class BN, int stride, typename SUBNET>
using block = BN<con<N, 3, 3, 1, 1, relu<BN<con<N, 3, 3, stride, stride, SUBNET>>>>>;

template <int N, typename SUBNET> using ares = relu<residual<block, N, affine, SUBNET>>;
template <int N, typename SUBNET> using ares_down = relu<residual_down<block, N, affine, SUBNET>>;

template <typename SUBNET> using alevel0 = ares_down<256, SUBNET>;
template <typename SUBNET> using alevel1 = ares<256, ares<256, ares_down<256, SUBNET>>>;
template <typename SUBNET> using alevel2 = ares<128, ares<128, ares_down<128, SUBNET>>>;
template <typename SUBNET> using alevel3 = ares<64, ares<64, ares<64, ares_down<64, SUBNET>>>>;
template <typename SUBNET> using alevel4 = ares<32, ares<32, ares<32, SUBNET>>>;

using anet_type = loss_metric<fc_no_bias<128, avg_pool_everything<
	alevel0<
	alevel1<
	alevel2<
	alevel3<
	alevel4<
	max_pool<3, 3, 2, 2, relu<affine<con<32, 7, 7, 2, 2,
	input_rgb_image_sized<150>
	>>>>>>>>>>>>;

std::vector<matrix<rgb_pixel>> jitter_image(
	const matrix<rgb_pixel>& img
);


void face_detection();
void face_recognition();

int face_detection(matrix<dlib::rgb_pixel>& _dlib_img, frontal_face_detector _detector, std::vector<rectangle> &_faces);
int face_recognition(matrix<dlib::rgb_pixel>& _dlib_img, shape_predictor &sp, anet_type &net, std::vector<rectangle> &_faces, Individual_Info &_detect_res);
int face_emotion(matrix<dlib::rgb_pixel>& _dlib_img, cv::Mat & _cv_img, shape_predictor &_sp, cv::Ptr<SVM>& _svm, std::vector<rectangle> &_faces);
void search_database(std::vector<std::string> &data_dir);
void Wchar_tToString(std::string& szDst, wchar_t *wchar);
float euclidean_calculation(cv::Mat matrix_1,cv::Mat matrix_2);
void getAllFiles(string path, std::vector<string>& files, string format);