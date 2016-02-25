////
////  shiyinpin.cpp
////  opencvCFSS
////
////  Created by Victor Young on 12/18/15.
////  Copyright (c) 2015 Victor Young. All rights reserved.
////
//
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <iostream>
//#include <math.h>
//    
//using namespace std;
//using namespace cv;
//#define max(a,b) ((a)>(b)?(a):(b))
//
//int main()
//{
//    //打开视频文件：其实就是建立一个VideoCapture结构
//    VideoCapture capture("/Users/Victor/Desktop/test.MP4");
//    //检测是否正常打开:成功打开时，isOpened返回ture
//    if (!capture.isOpened())
//        cout << "fail toopen!"<< endl;
//    
//    //获取整个帧数
//    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);
//    cout << "整个视频共" << totalFrameNumber << "帧" << endl;
//    
//    
//    //设置开始帧()
//    long frameToStart = 1;
//    capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);
//    cout << "从第" <<frameToStart << "帧开始读" << endl;
//    
//    
//    //设置结束帧
//    int frameToStop = 500;
//    
//    if (frameToStop < frameToStart)
//    {
//        cout << "结束帧小于开始帧，程序错误，即将退出！" << endl;
//        return -1;
//    }
//    else
//    {
//        cout << "结束帧为：第" << frameToStop << "帧" << endl;
//    }
//    
//    //获取帧率
//    double rate = capture.get(CV_CAP_PROP_FPS);
//    cout << "帧率为:" << rate<< endl;
//    
//    
//    
//    //定义一个用来控制读取视频循环结束的变量
//    bool stop = false;
//    
//    //承载每一帧的图像
//    Mat frame;
//    
//    //显示每一帧的窗口
//    namedWindow("Extractedframe");
//    
//    //两帧间的间隔时间:
//    //int delay = 1000/rate;
//    double delay = 1000 / rate;
//    
//    
//    //利用while循环读取帧
//    //currentFrame是在循环体中控制读取到指定的帧后循环结束的变量
//    long currentFrame = frameToStart;
//    
//    
//    //滤波器的核
//    int kernel_size = 3;
//    Mat kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size*kernel_size);
//    float range[] = { 0, 256 } ;
//    const float* histRange = { range };
//    bool uniform = true;
//    bool accumulate = false;
//    int histSize = 256;
//    if(!capture.read(frame))
//    {
//        cout << "读取视频失败" << endl;
//        return -1;
//    }
//    Mat gray;
//    Mat prehist;
//    Mat preframe;
//    cvtColor(frame, gray, CV_BGR2GRAY);
//    calcHist(&gray,1,0,Mat(),prehist,1,&histSize,&histRange,uniform, accumulate );
//    normalize(prehist,prehist,1.0,0.0,NORM_MINMAX);
//    double dist,x2dist;
//    Mat hist;
//    int n=0;
//    while (!stop)
//    {
//        
//        
//        //读取下一帧
//        if(!capture.read(preframe))
//        {
//            cout << "读取视频失败" << endl;
//            return -1;
//        }
//        
//        
//        cvtColor(preframe, gray, CV_BGR2GRAY);
//        calcHist(&gray,1,0,Mat(),hist,1,&histSize,&histRange,uniform, accumulate );
//        normalize(hist,hist,1.0,0.0,NORM_MINMAX);
//        dist =  0;
//        for (int i=0; i<255; i++) {
//            int max =max(hist.at<float>(i,0),prehist.at<float>(i,0));
//            if (max == 0) {
//                max = 1;
//            }
//            dist += pow(hist.at<float>(i,0)-prehist.at<float>(i,0),2)/max;
//        }
////         dist = norm(prehist,hist,NORM_L2);
////        x2dist = pow(dist,2)/max(prehist,hist);
//        
//        cout << "第" << n++ << "帧:" << dist << endl;
//
//
//        if (dist>5) {
//            cout << "OK " << dist<< " ";
//            imshow("Extractedframe",frame);
//            imshow("Extractedframe2",preframe);
//            waitKey(0);
//            
//        }
//        
//        if(!capture.read(frame))
//        {
//            cout << "读取视频失败" << endl;
//            return -1;
//        }
//        
//        
//        cvtColor(frame, gray, CV_BGR2GRAY);
//        calcHist(&gray,1,0,Mat(),prehist,1,&histSize,&histRange,uniform, accumulate );
//        normalize(prehist,prehist,1.0,0.0,NORM_MINMAX);
////        dist = norm(prehist,hist,NORM_L2);
//        for (int i=0; i<255; i++) {
//            int max =max(hist.at<float>(i,0),prehist.at<float>(i,0));
//            if (max == 0) {
//                max = 1;
//            }
//            dist += pow(hist.at<float>(i,0)-prehist.at<float>(i,0),2)/max;
//        }
//        
//        if (dist>5) {
//            cout << "OK " << dist << " ";
//            imshow("Extractedframe",frame);
//            imshow("Extractedframe2",preframe);
//            waitKey(0);
//
//        }
//
//        
////        cout << frame; 
////        cout << "正在读取第" << currentFrame << "帧" << endl;
////        imshow("Extractedframe",frame);
////        waitKey(10);
////
////        cout << "正在写第" << currentFrame << "帧" << endl;
////        stringstream str;
////        str<<"sample" <<currentFrame << ".png";
////        cout << str.str() << endl;
////        imwrite(str.str() , frame);
//        
//        //waitKey(intdelay=0)当delay≤ 0时会永远等待；当delay>0时会等待delay毫秒
//        //当时间结束前没有按键按下时，返回值为-1；否则返回按键
//        //int c = waitKey(delay);
//        int c = waitKey(1);
//
//        //按下ESC或者到达指定的结束帧后退出读取视频
//        if ((char)c == 27 ||currentFrame > frameToStop)
//        {
//            stop = true;
//        }
//        //按下按键后会停留在当前帧，等待下一次按键
////        if (c >= 0)
////        {
////            waitKey(0);
////        }
//        currentFrame++;
//        
//    }
//    //关闭视频文件
//    capture.release();
//    waitKey(0);
//    return 0;
//}