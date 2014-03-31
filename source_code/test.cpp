/**
 * @author 
 * @version 2014/03/31
 */

#include "face.h"

int main(){
    int img_num = 715;
    int pixel_pair_num = 400;
    int pixel_pair_in_fern = 5;
    int first_level_num = 10;
    int second_level_num = 500; 
    int landmark_num = 35;
    int initial_number = 1;


    vector<Mat_<uchar> > images;
    int average_height = 0;
    int average_width = 0;

    cout<<"Read images..."<<endl;

    for(int i = 0;i < img_num;i++){
        string image_name = "./../data/LFPW/lfpwFaces/";
        image_name = image_name + to_string(i+1) + ".jpg";
        Mat_<uchar> temp = imread(image_name,0);
        images.push_back(temp);
        average_height = average_height + temp.rows;
        average_width = average_width + temp.cols;
    }

    average_height = average_height / img_num;
    average_width  =average_width / img_num;

	for(int i = 0;i < img_num;i++){
		resize(images[i],images[i],Size(average_width,average_height));
	}

    ifstream fin;
    fin.open("./../data/LFPW/keypointsInfor.txt"); 
    double start_x;
    double start_y;
    double curr_width;
    double curr_height;
    Mat_<double> mean_shape(landmark_num,2);
    vector<Mat_<double> > target_shapes;
    for(int i = 0;i < landmark_num;i++){
        mean_shape(i,0) = 0;
        mean_shape(i,1) = 0;
    }
    while(fin>>start_x>>start_y>>curr_width>>curr_height){
        Mat_<double> temp(landmark_num,2);
        double keypoint_x;
        double keypoint_y;
        for(int i = 0;i < 35;i++){
            fin>>keypoint_x>>keypoint_y;
            keypoint_x = keypoint_x * average_width / curr_width;
            keypoint_y = keypoint_y * average_height / curr_height;
            temp(i,0) = keypoint_x;
            temp(i,1) = keypoint_y;
            mean_shape(i,0) += keypoint_x;
            mean_shape(i,1) += keypoint_y;
        }
        target_shapes.push_back(temp);
    }  

    mean_shape = 1.0/(img_num) * mean_shape;
	
    
    Mat_<uchar> test_image = imread("./../data/LFPW/lfpwFaces/10.jpg",0);
	resize(test_image,test_image,Size(average_width,average_height));
	Mat_<double> current_shape = test(test_image,target_shapes,mean_shape,1);
	for(int i = 0;i < landmark_num;i++){
		circle(test_image,Point2d(current_shape(i,0), current_shape(i,1)),3,Scalar(255,0,0),-1,8,0);
	}
	imshow("result",test_image);
	waitKey(0);

    return 0;
    
}