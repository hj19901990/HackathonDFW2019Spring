#include "facedetection.h"

 
void face_detection()
{
	cv::Mat cv_img = cv::imread("..\\man.bmp");
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
	deserialize("..\\shape_predictor_68_face_landmarks.dat") >> sp;
	anet_type net;
	deserialize("..\\dlib_face_recognition_resnet_model_v1.dat") >> net;


	cv::Mat cv_img = cv::imread("D:\\jinfeng.bmp");
	//cv::imshow("test", cv_img);
	//cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
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

	//cout << "face descriptor for one face: " << trans(face_descriptors[0]) << endl;
	fstream outfile("D:\\jinfeng.txt");
	//outfile.open("D:\\01.txt", std::ios::app);
	cv::Mat features_matrix = dlib::toMat(face_descriptors[0]);
	for (int i = 0; i < features_matrix.rows; i++)
	{
		outfile << *((float*)features_matrix.data+i) << endl;
	}
	outfile.close();

	cout << "face descriptor for one face: " << face_descriptors[0] << endl;
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


int face_detection(matrix<dlib::rgb_pixel> &_dlib_img, frontal_face_detector _detector, std::vector<rectangle> &_faces)
{
	//cv::Mat cv_img = cv::imread("..\\man.bmp");
	//cv::imshow("test", cv_img);
	//cv::cvtColor(cv_img, cv_img, CV_BGR2RGB);
	//matrix<dlib::rgb_pixel> img;
	//assign_image(img, cv_image<rgb_pixel>(cv_img));


	////frontal_face_detector detector = get_frontal_face_detector();
	////image_window win;


	////load_image(img, "D:\\HackDFW\\test.bmp");
	//pyramid_up(img);
	
	_faces.clear();
	_faces = _detector(_dlib_img);
	if (_faces.size() == 0)
	{
		return XFAIL;
	}
	else 
	{
		return XSUCESS;
	}

	//cv::waitKey(0);
}

int face_recognition(matrix<dlib::rgb_pixel>& _dlib_img,shape_predictor &sp, anet_type &net, std::vector<rectangle> &_faces,Individual_Info &_detect_res)
{
	std::vector<matrix<rgb_pixel>> face_chips;
	for (auto face : _faces)
	{
		auto shape = sp(_dlib_img, face);
		matrix<rgb_pixel> face_chip;
		extract_image_chip(_dlib_img, get_face_chip_details(shape, 150, 0.25), face_chip);
		face_chips.push_back(move(face_chip));
	}

	std::vector<matrix<float, 0, 1>> face_descriptors = net(face_chips);
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
	//cout << "number of people found in the image: " << num_clusters << endl;

	std::vector<image_window> win_clusters(num_clusters);
	for (size_t cluster_id = 0; cluster_id < num_clusters; ++cluster_id)
	{
		std::vector<matrix<rgb_pixel>> temp;
		for (size_t j = 0; j < labels.size(); ++j)
		{
			if (cluster_id == labels[j])
				temp.push_back(face_chips[j]);
		}
		win_clusters[cluster_id].set_title("face cluster " + cast_to_string(cluster_id));
		win_clusters[cluster_id].set_image(tile_images(temp));
	}
	
	//find the best result from database
	
		//convert matrix to Mat
		cv::Mat features_matrix = dlib::toMat(face_descriptors[0]);
		std::vector<std::string> data_path;
		search_database(data_path);
		getAllFiles("..\\database", data_path,"txt");
		std::vector<std::vector<string>> data_base;
		for (auto dir : data_path)
		{
			//read database
			ifstream infile(dir);
			int row_count = 0;
			//Individual temp_indiv;
			std::vector<string> each_people(135); 
			//data_in_database[86];
			while (!infile.eof())            
			{
				infile >> each_people[row_count];
				row_count++;
			}
			data_base.push_back(each_people);
		}
		std::vector<float> match;
		for (size_t i = 0; i < data_base.size(); i++)
		{
			cv::Mat features_database(128, 1, CV_32FC1);
			for (size_t j = 6; j < 134; j++)
			{
				*((float *)features_database.data + j - 6) = std::stof(data_base[i][j]);
			}
			match.push_back(euclidean_calculation(features_matrix, features_database));
		}
		int number = 0;
		float mini = 1000000000;
		for (size_t i = 0; i < match.size(); i++)
		{
			if (mini > match[i])
			{
				mini = match[i];
				number = i;
			}
		}
		if (mini < 10)
		{
			strcpy(&_detect_res.name[0], data_base[number][2].c_str());
			_detect_res.name_len = data_base[number][2].length();
			_detect_res.age = atoi(data_base[number][3].c_str());
			//_detect_res.id = atoi(data_base[number][0].c_str());
			//data_in_database[2].copy(*detect_res.name, 5, 0);
			/*detect_res.name = memcpy(data_in_database[2].c_str());*/
			return atoi(data_base[number][0].c_str());
		}
		else
		{
			return -2;
		}

			//for (int i = 6; i < 86; i++)
			//{
			//	*((float *)features_database.data + i - 6) = std::stof(data_in_database[i]);
			//}
			//if (euclidean_calculation(features_matrix, features_database) < 10)     //threshold of recognition
			//{
			//	strcpy(&_detect_res.name[0], data_in_database[2].c_str());
			//	_detect_res.name_len = data_in_database[2].length();
			//	_detect_res.age = atoi(data_in_database[3].c_str());
			//	//data_in_database[2].copy(*detect_res.name, 5, 0);
			//	/*detect_res.name = memcpy(data_in_database[2].c_str());*/
			//	return atoi(data_in_database[0].c_str());
			//}
			
		


		//if (euclidean_calculation(features_matrix, database[0]) < 10)
		//{
		//	ofstream output(".\\test.txt", ios::out | ios::binary);
		//	if (!output)
		//	{
		//		cerr << "Open output file error!" << endl;
		//		exit(-1);
		//	}

		//	output.write((char *)A, sizeof(A));
		//	for (i = 0; i < 3; i++)
		//	{
		//		for (j = 0; j < 10; j++)
		//		{
		//			//output.write ( ( char * ) & A [ i ][ j ], sizeof( A [ i ][ j ] ) );
		//		}
		//	}
		//}
	
	
	//cout << "face descriptor for one face: " << trans(face_descriptors[0]) << endl;
	//return XSUCESS;
}

int face_emotion(matrix<dlib::rgb_pixel>& _dlib_img, cv::Mat & _cv_img, shape_predictor &_sp, cv::Ptr<SVM>& _svm, std::vector<rectangle> &_faces)
{
	std::vector<full_object_detection> shapes;
	for (unsigned long i = 0; i < _faces.size(); ++i)
		shapes.push_back(_sp(_dlib_img, _faces[i]));

	if (!shapes.empty()) 
	{
		float testData[1][136];
		float coefficient = -(_faces[0].top() - _faces[0].bottom()) / 300.0;
		for (int i = 0; i < 68; i++) {
			circle(_cv_img, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 2, cv::Scalar(255, 0, 0), -1);
			testData[0][i * 2] = (shapes[0].part(i).x() - _faces[0].left()) / coefficient;
			testData[0][i * 2 + 1] = (shapes[0].part(i).y() - _faces[0].top()) / coefficient;
			//  shapes[0].part(i).x();//68  
		}
		cv::Mat result;

		cv::Mat query(1, 136, CV_32FC1, testData);

		if (_svm->predict(query) == 250) {
			return 1;   //happy
			//cv::putText(_cv_img, "Happy", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			//cout << "happy" << endl;
		}

		if (_svm->predict(query) == 170) {
			return 2;    //normal
			//cv::putText(_cv_img, "Calm", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			//cout << "calm" << endl;
		}
		if (_svm->predict(query) == 300) {

			return 3; // upset
			//cv::putText(_cv_img, "Upset", cv::Point(20, 60), 3, 2, cvScalar(0, 0, 255));
			//cout << "upset" << endl;
		}

	}
	return XFAIL;
}


float euclidean_calculation(cv::Mat matrix_1, cv::Mat matrix_2)
{
	return cv::norm(matrix_1, matrix_2, CV_L2);
}

void search_database(std::vector<std::string> &data_dir)
{
	//getAllFiles("..\\database\\", data_dir);
	/*WIN32_FIND_DATA fileInfo;
	HANDLE hFind;
	string root = "..\\database\\";
	hFind = FindFirstFile(TEXT("..\\database\\"), &fileInfo);
	while (FindNextFile(hFind, &fileInfo) != 0)
	{
		string file_name;
		Wchar_tToString(file_name, fileInfo.cFileName);	
		data_dir.push_back(root + file_name);
	}*/

	
}


void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);
	char *psText; 
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);
	szDst = psText;
	delete[]psText;
}


void getAllFiles(string path, std::vector<string>& files, string format)
{
	intptr_t  hFile = 0;//文件句柄  64位下long 改为 intptr_t
	struct _finddata_t fileinfo;//文件信息 
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1) //文件存在
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))//判断是否为文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)//文件夹名中不含"."和".."
				{
					files.push_back(p.assign(path).append("\\").append(fileinfo.name)); //保存文件夹名
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files, format); //递归遍历文件夹
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));//如果不是文件夹，储存文件名
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

