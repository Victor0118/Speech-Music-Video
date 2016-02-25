//
//  shiziyinpin4.cpp
//  opencvCFSS
//
//  Created by Victor Young on 1/17/16.
//  Copyright (c) 2016 Victor Young. All rights reserved.
//

// Farneback dense optical flow calculate and show in Munsell system of colors
// Author : Zouxy
// Date   : 2013-3-15
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com

// API calcOpticalFlowFarneback() comes from OpenCV, and this
// 2D dense optical flow algorithm from the following paper:
// Gunnar Farneback. "Two-Frame Motion Estimation Based on Polynomial Expansion".
// And the OpenCV source code locate in ..\opencv2.4.3\modules\video\src\optflowgf.cpp

#include <iostream>
#include "opencv2/opencv.hpp"
#include <vector>
#include <math.h>
#include "GraphUtils.h"

using namespace cv;
using namespace std;

#define UNKNOWN_FLOW_THRESH 1e9
#define N 10
double total = 0;

void motionToColor(Mat flow)
{
    float maxrad = -1;
    float radmean = 0;
    float dot = 0;
    
    for (int i = 0; i < flow.rows; ++i)
    {
        for (int j = 0; j < flow.cols; ++j)
        {
            Vec2f flow_at_point = flow.at<Vec2f>(i, j);
            float fx = flow_at_point[0];
            float fy = flow_at_point[1];
            if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
                continue;
            float rad = sqrt(fx * fx + fy * fy);
            radmean += rad / (flow.rows*flow.cols);
            if (rad < 0.1){
                dot += 1.0 / (flow.rows*flow.cols);
            }
            maxrad = maxrad > rad ? maxrad : rad;
        }
    }
    cout << radmean<<endl;
    cout << dot<<endl;
    total = dot;
}

double caldist(Mat h1, Mat h2){
    double ret = 0;
    for (int i = 0; i < h1.rows; i++){
        double m = max(h1.at<float>(i, 0), h2.at<float>(i, 0));
        if (m>0)
            ret = ret + pow((h1.at<float>(i, 0) - h2.at<float>(i, 0)), 2) / m;
    }
    return ret;
}

int histtans(string str){
    VideoCapture cap;
    cap.open(str);
    if (!cap.isOpened())
        return -1;
    Mat prevgray, gray, frame;
    Mat prehist[N*N];
    Mat hist[N*N];
    
    vector<float> parray;
    
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    int histSize = 256;
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    double dist;
    
    cap >> frame;
    cvtColor(frame, gray, CV_BGR2GRAY);
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            Mat tmp = Mat(frame.rows / N, frame.cols / N, CV_8UC1);
            for (int ii = 0; ii < (frame.rows / N); ii++){
                uchar *t = tmp.ptr<uchar>(ii);
                uchar *g = gray.ptr<uchar>(i*(frame.rows / N) + ii);
                for (int jj = 0; jj < (frame.cols / N); jj++){
                    //tmp.at<uchar>(ii, jj) = gray.at<uchar>(i*(frame.rows / N) + ii, j*(frame.cols / N) + jj);
                    t[jj] = g[j*(frame.cols / N) + jj];
                }
            }
            Mat tmphist;
            calcHist(&tmp, 1, 0, Mat(), tmphist, 1, &histSize, &histRange, uniform, accumulate);
            normalize(tmphist, tmphist, 1.0, 0.0, NORM_MINMAX);
            prehist[i*N + j] = tmphist.clone();
        }
    }
    for (int index=1;index<totalFrameNumber;index++){
        double t = (double)cvGetTickCount();
        cap >> frame;
        cvtColor(frame, gray, CV_BGR2GRAY);
        for (int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                Mat tmp = Mat(frame.rows / N, frame.cols / N, CV_8UC1);
                for (int ii = 0; ii < (frame.rows / N); ii++){
                    uchar *t = tmp.ptr<uchar>(ii);
                    uchar *g = gray.ptr<uchar>(i*(frame.rows / N) + ii);
                    for (int jj = 0; jj < (frame.cols / N); jj++){
                        //tmp.at<uchar>(ii, jj) = gray.at<uchar>(i*(frame.rows / N) + ii, j*(frame.cols / N) + jj);
                        t[jj] = g[j*(frame.cols / N) + jj];
                    }
                }
                Mat tmphist;
                calcHist(&tmp, 1, 0, Mat(), tmphist, 1, &histSize, &histRange, uniform, accumulate);
                normalize(tmphist, tmphist, 1.0, 0.0, NORM_MINMAX);
                hist[i*N + j] = tmphist.clone();
            }
        }
        float probability = 0;
        for (int i = 0; i < N*N; i++){
            dist = norm(prehist[i], hist[i], NORM_L2);
            //dist = caldist(prehist[i], hist[i]);
            //cout << dist<<" ";
            if (dist > 1.5){
                probability += 1.0 / (N*N);
            }
        }
        if (probability>0.9){
            imshow("mirro", frame);
            waitKey();
        }
        parray.push_back(probability);
        cout <<index<<" "<< probability;
        imshow("now", frame);
        waitKey(1);
        t = (double)cvGetTickCount() - t;
        cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
        for (int i = 0; i < N*N; i++){
            prehist[i] = hist[i];
        }
    }
    IplImage *img = drawFloatGraph(&parray[0], 1000, 0, 0, 0, 1000);
    showImage(img, 0, "Direct");
    waitKey();
    return 0;
}

int his2key(string str){
    VideoCapture cap;
    cap.open(str);
    
    if (!cap.isOpened())
        return -1;
    Mat gray, frame;
    Mat prehist;
    Mat Totalhist;
    
    vector<Mat> framearray;
    vector<Mat> histarray;
    vector<float> parray;
    
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    int histSize = 256;
    double dist;
    Mat hist;
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    
    cap >> frame;
    cvtColor(frame, gray, CV_BGR2GRAY);
    calcHist(&gray, 1, 0, Mat(), prehist, 1, &histSize, &histRange, uniform, accumulate);
    normalize(prehist, prehist, 1.0, 0.0, NORM_MINMAX);
    Totalhist = prehist.clone();
    framearray.push_back(frame.clone());
    histarray.push_back(prehist.clone());
    
    for (int index = 1; index < totalFrameNumber; index++){
        double t = (double)cvGetTickCount();
        cap >> frame;
        cvtColor(frame, gray, CV_BGR2GRAY);
        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);
        normalize(hist, hist, 1.0, 0.0, NORM_MINMAX);
        dist = norm(prehist, hist, NORM_L2);
        parray.push_back(dist);
        if (dist > 1.5){
            cout << dist << endl;
            imshow("mirro", frame);
            normalize(Totalhist, Totalhist, 1.0, 0.0, NORM_MINMAX);
            double min = 10;
            int index = -1;
            for (int i = 0; i < framearray.size(); i++){
                double tmp = norm(histarray[i], Totalhist, NORM_L2);
                if (tmp < min){
                    min = tmp;
                    index = i;
                }
            }
            imshow("Key", framearray[index]);
            Totalhist = hist.clone();
            framearray.clear();
            histarray.clear();
            framearray.push_back(frame.clone());
            histarray.push_back(hist.clone());
            waitKey();
        }
        else{
            Totalhist = Totalhist + hist;
            framearray.push_back(frame.clone());
            histarray.push_back(hist.clone());
        }
        prehist = hist.clone();
        imshow("now", frame);
        waitKey(1);
    }
    IplImage *img = drawFloatGraph(&parray[0], 1000, 0, 0, 0, 1000);
    showImage(img, 0, "Direct");
    waitKey();
    return 0;
}

int flow2key(string str){
    VideoCapture cap;
    cap.open("test.mkv");
    
    if (!cap.isOpened())
        return -1;
    Mat prevgray, gray, flow, cflow, frame;
    Mat preframe;
    
    vector<float> parray;
    
    float range[] = { 0, 256 };
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    int histSize = 256;
    double dist;
    Mat hist;
    long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
    int flag = 0;
    
    for (int index = 1; index < totalFrameNumber; index++){
        double t = (double)cvGetTickCount();
        cap >> frame;
        cvtColor(frame, gray, CV_BGR2GRAY);
        if (prevgray.data)
        {
            calcOpticalFlowFarneback(prevgray, gray, flow, 0.5, 3, 15, 3, 5, 1.2, 0);
            total = 0;
            motionToColor(flow);
            imshow("now", frame);
            if (total > 0.9 && !flag){
                imshow("pre", preframe);
                imshow("now", frame);
                flag = 1;
                waitKey();
            }
            else if (total<0.9) flag = 0;
            
        }
        waitKey(1);
        std::swap(prevgray, gray);
        std::swap(preframe, frame);
        parray.push_back(total);
        t = (double)cvGetTickCount() - t;
        cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;
    }
    IplImage *img = drawFloatGraph(&parray[0], 1000, 0, 0, 0, 1000);
    showImage(img, 0, "Direct");
    waitKey();
    return 0;
}

int main(int, char**)
{
    //flow2key("test.mkv");
    his2key("/Users/Victor/Desktop/test.MP4");
    //histtans("test.mkv");
    return 0;
}