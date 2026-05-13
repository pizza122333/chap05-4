#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

// [코드 5-7 기반] 히스토그램 계산 함수
Mat calcGrayHist(const Mat& img) {
    CV_Assert(img.type() == CV_8UC1);
    Mat hist;
    int channels[] = { 0 };
    int dims = 1;
    const int histSize[] = { 256 };
    float graylevel[] = { 0, 256 };
    const float* ranges[] = { graylevel };
    calcHist(&img, 1, channels, noArray(), hist, dims, histSize, ranges);
    return hist;
}

// [코드 5-8 기반] 히스토그램 그래프 그리기 함수
Mat getGrayHistImage(const Mat& hist) {
    double histMax;
    minMaxLoc(hist, 0, &histMax);
    Mat imgHist(100, 256, CV_8UC1, Scalar(255));
    for (int i = 0; i < 256; i++) {
        line(imgHist, Point(i, 100),
            Point(i, 100 - cvRound(hist.at<float>(i, 0) * 100 / histMax)),
            Scalar(0));
    }
    return imgHist;
}

int main() {
    // 1. 영상 불러오기
    Mat src = imread("crayfish.jpg", IMREAD_GRAYSCALE);
    if (src.empty()) {
        cerr << "Image load failed!" << endl;
        return -1;
    }

    // 2. 히스토그램 스트레칭을 위한 최소/최대값 찾기
    double gmin, gmax;
    minMaxLoc(src, &gmin, &gmax);

    // 3. 직접 참조 방식 스트레칭 수행 (과제 4 핵심)
    Mat dst = Mat::zeros(src.size(), src.type());
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            uchar p_src = src.at<uchar>(y, x);
            // 포화 연산(saturate_cast) 적용
            dst.at<uchar>(y, x) = saturate_cast<uchar>((p_src - gmin) * 255 / (gmax - gmin));
        }
    }

    // 4. 결과 출력 및 시각화
    imshow("src", src); // 입력 영상
    imshow("dst", dst); // 출력 영상 (스트레칭 후)

    imshow("srcHist", getGrayHistImage(calcGrayHist(src))); // 입력 히스토그램
    imshow("dstHist", getGrayHistImage(calcGrayHist(dst))); // 출력 히스토그램

    waitKey(0);
    destroyAllWindows();

    return 0;
}