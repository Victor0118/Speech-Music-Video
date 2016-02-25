////
////  shiyinpin2.cpp
////  opencvCFSS
////
////  Created by Victor Young on 12/26/15.
////  Copyright (c) 2015 Victor Young. All rights reserved.
////
//
//// Farneback dense optical flow calculate and show in Munsell system of colors
//// Author : Zouxy
//// Date   : 2013-3-15
//// HomePage : http://blog.csdn.net/zouxy09
//// Email  : zouxy09@qq.com
//
//// API calcOpticalFlowFarneback() comes from OpenCV, and this
//// 2D dense optical flow algorithm from the following paper:
//// Gunnar Farneback. "Two-Frame Motion Estimation Based on Polynomial Expansion".
//// And the OpenCV source code locate in ..\opencv2.4.3\modules\video\src\optflowgf.cpp
//
//#include <iostream>
//#include "opencv2/opencv.hpp"
//
//using namespace cv;
//using namespace std;
//
//#define UNKNOWN_FLOW_THRESH 1e9
//double total = 0;
//
//// Color encoding of flow vectors from:
//// http://members.shaw.ca/quadibloc/other/colint.htm
//// This code is modified from:
//// http://vision.middlebury.edu/flow/data/
//void makecolorwheel(vector<Scalar> &colorwheel)
//{
//    int RY = 15;
//    int YG = 6;
//    int GC = 4;
//    int CB = 11;
//    int BM = 13;
//    int MR = 6;
//    
//    int i;
//    
//    for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255, 255 * i / RY, 0));
//    for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255 - 255 * i / YG, 255, 0));
//    for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0, 255, 255 * i / GC));
//    for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0, 255 - 255 * i / CB, 255));
//    for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255 * i / BM, 0, 255));
//    for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255, 0, 255 - 255 * i / MR));
//}
//
//void motionToColor(Mat flow, Mat &color)
//{
//    if (color.empty())
//        color.create(flow.rows, flow.cols, CV_8UC3);
//    
//    static vector<Scalar> colorwheel; //Scalar r,g,b
//    if (colorwheel.empty())
//        makecolorwheel(colorwheel);
//    
//    // determine motion range:
//    float maxrad = -1;
//    float radmean = 0;
//    float dot = 0;
//    
//    // Find max flow to normalize fx and fy
//    for (int i = 0; i < flow.rows; ++i)
//    {
//        for (int j = 0; j < flow.cols; ++j)
//        {
//            Vec2f flow_at_point = flow.at<Vec2f>(i, j);
//            float fx = flow_at_point[0];
//            float fy = flow_at_point[1];
//            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
//                continue;
//            float rad = sqrt(fx * fx + fy * fy);
//            radmean += rad / (flow.rows*flow.cols);
//            if (rad < 0.1){
//                dot += 1.0 / (flow.rows*flow.cols);
//            }
//            maxrad = maxrad > rad ? maxrad : rad;
//        }
//    }
//    cout << radmean<<endl;
//    cout << dot<<endl;
//    total = dot;
//    
//    /*for (int i = 0; i < flow.rows; ++i)
//     {
//     for (int j = 0; j < flow.cols; ++j)
//     {
//     uchar *data = color.data + color.step[0] * i + color.step[1] * j;
//     Vec2f flow_at_point = flow.at<Vec2f>(i, j);
//     
//     float fx = flow_at_point[0] / maxrad;
//     float fy = flow_at_point[1] / maxrad;
//     if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
//     {
//     data[0] = data[1] = data[2] = 0;
//     continue;
//     }
//     float rad = sqrt(fx * fx + fy * fy);
//     float angle = atan2(-fy, -fx) / CV_PI;
//     float fk = (angle + 1.0) / 2.0 * (colorwheel.size() - 1);
//     int k0 = (int)fk;
//     int k1 = (k0 + 1) % colorwheel.size();
//     float f = fk - k0;
//     //f = 0; // uncomment to see original color wheel
//     
//     for (int b = 0; b < 3; b++)
//     {
//     float col0 = colorwheel[k0][b] / 255.0;
//     float col1 = colorwheel[k1][b] / 255.0;
//     float col = (1 - f) * col0 + f * col1;
//     if (rad <= 1)
//					col = 1 - rad * (1 - col); // increase saturation with radius
//     else
//					col *= .75; // out of range
//     data[2 - b] = (int)(255.0 * col);
//     }
//     }
//     }*/
//}
//
//int main(int, char**)
//{
//    VideoCapture cap;
//    cap.open("test.mkv");
//    
//    if (!cap.isOpened())
//        return -1;
//    
//    Mat prevgray, gray, flow, cflow, frame;
//    //namedWindow("flow", 1);
//    
//    Mat motion2color;
//    Mat prehist;
//    Mat preframe;
//    Mat Totalhist;
//    
//    vector<Mat> framearray;
//    vector<Mat> histarray;
//    
//    float range[] = { 0, 256 };
//    const float* histRange = { range };
//    bool uniform = true;
//    bool accumulate = false;
//    int histSize = 256;
//    double dist;
//    Mat hist;
//    
//    int flag = 0;
//    
//    cap >> frame;
//    cvtColor(frame, gray, CV_BGR2GRAY);
//    calcHist(&gray, 1, 0, Mat(), prehist, 1, &histSize, &histRange, uniform, accumulate);
//    normalize(prehist, prehist, 1.0, 0.0, NORM_MINMAX);
//    Totalhist = prehist.clone();
//    framearray.push_back(frame.clone());
//    histarray.push_back(prehist.clone());
//    
//    
//    for (;;)
//    {
//        double t = (double)cvGetTickCount();
//        cap >> frame;
//        cvtColor(frame, gray, CV_BGR2GRAY);
//        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
//        normalize(hist, hist, 1.0, 0.0, NORM_MINMAX);
//        dist = norm(prehist, hist, NORM_L2);
//        cout << dist;
//        //imshow("original", gray);
//        if (dist > 1.5){
//            cout << dist << endl;
//            imshow("mirro", frame);
//            normalize(Totalhist, Totalhist, 1.0, 0.0, NORM_MINMAX);
//            double min = 10;
//            int index = -1;
//            for (int i = 0; i < framearray.size(); i++){
//                double tmp = norm(histarray[i], Totalhist, NORM_L2);
//                if (tmp < min){
//                    min = tmp;
//                    index = i;
//                }
//            }
//            imshow("Key", framearray[index]);
//            Totalhist = hist.clone();
//            framearray.clear();
//            histarray.clear();
//            framearray.push_back(frame.clone());
//            histarray.push_back(hist.clone());
//            waitKey();
//        }
//        else{
//            Totalhist = Totalhist + hist;
//            framearray.push_back(frame.clone());
//            histarray.push_back(hist.clone());
//        }
//        prehist = hist.clone();
//        imshow("now", frame);
//        /*if (prevgray.data)
//         {
//         calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
//         total = 0;
//         motionToColor(flow, motion2color);
//         //imshow("flow", motion2color);
//         imshow("now", gray);
//         if (total > 0.9 && !flag){
//         imshow("pre", prevgray);
//         imshow("now", gray);
//         flag = 1;
//         waitKey();
//         }
//         else if(total<0.9) flag = 0;
//         }*/
//        if (waitKey(10) >= 0)
//            break;
//        std::swap(prevgray, gray);
//        
//        t = (double)cvGetTickCount() - t;
//        cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
//    }
//    return 0;
//}