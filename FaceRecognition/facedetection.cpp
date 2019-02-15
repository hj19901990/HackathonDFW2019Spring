#include "facedetection.h"


void face_detection()
{
	cv::Mat cv_img = cv::imread("D:\\HackDFW\\man.bmp");
	cv::imshow("test", cv_img);
	cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	matrix<dlib::rgb_pixel> img;
	assign_image(img, cv_image<rgb_pixel>(cv_img));


	frontal_face_detector detector = get_frontal_face_detector();
	image_window win;
	
	
	//load_image(img, "D:\\HackDFW\\test.bmp");
	pyramid_up(img);

	std::vector<rectangle> dets = detector(img);
	win.clear_overlay();
	win.set_image(img);
	win.add_overlay(dets, rgb_pixel(255, 0, 0));

	cv::waitKey(0);
}

void face_recognition()
{
	frontal_face_detector detector = get_frontal_face_detector();
	
	shape_predictor sp;
	deserialize("D:\\HackDFW\\FaceRecognition\\shape_predictor_68_face_landmarks.dat") >> sp;
	anet_type net;
	deserialize("D:\\HackDFW\\FaceRecognition\\dlib_face_recognition_resnet_model_v1.dat") >> net;


	cv::Mat cv_img = cv::imread("D:\\HackDFW\\two.bmp");
	//cv::imshow("test", cv_img);
	cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	matrix<dlib::rgb_pixel> img;
	assign_image(img, cv_image<rgb_pixel>(cv_img));
	/*pyramid_up(img);
	std::vector<rectangle> dets = detector(img);*/

	image_window win(img);
	
	std::vector<matrix<rgb_pixel>> faces;
	
	pyramid_up(img);
	for (auto face : detector(img))
	{
		auto shape = sp(img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(img, get_face_chip_details(shape, 150, 0.25), face_chip);
		faces.push_back(move(face_chip));
		// Also put some boxes on the faces so we can see that the detector is finding
		// them.
		win.add_overlay(face, rgb_pixel(255, 0, 0));
	}


	if (faces.size() == 0)
	{
		cout << "No faces found in image!" << endl;
	}
	
	std::vector<matrix<float, 0, 1>> face_descriptors = net(faces);
	std::vector<sample_pair> edges;
	for (size_t i = 0; i < face_descriptors.size(); ++i)
	{
		for (size_t j = i; j < face_descriptors.size(); ++j)
		{
			// Faces are connected in the graph if they are close enough.  Here we check if
			// the distance between two face descriptors is less than 0.6, which is the
			// decision threshold the network was trained to use.  Although you can
			// certainly use any other threshold you find useful.
			if (length(face_descriptors[i] - face_descriptors[j]) < 0.6)
				edges.push_back(sample_pair(i, j));
		}
	}
	std::vector<unsigned long> labels;
	const auto num_clusters = chinese_whispers(edges, labels);

	cout << "number of people found in the image: " << num_clusters << endl;

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

	cout << "face descriptor for one face: " << trans(face_descriptors[0]) << endl;
	//matrix<float, 0, 1> face_descriptor = mean(mat(net(jitter_image(faces[0]))));
	//cout << "jittered face descriptor for one face: " << trans(face_descriptor) << endl;

	cin.get();
}

std::vector<matrix<rgb_pixel>> jitter_image(
	const matrix<rgb_pixel>& img
)
{
	// All this function does is make 100 copies of img, all slightly jittered by being
	// zoomed, rotated, and translated a little bit differently. They are also randomly
	// mirrored left to right.
	thread_local dlib::rand rnd;

	std::vector<matrix<rgb_pixel>> crops;
	for (int i = 0; i < 100; ++i)
		crops.push_back(jitter_image(img, rnd));

	return crops;
}


