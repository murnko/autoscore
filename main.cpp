#include "main.h"
#include "funkcje.h"

int main(int argc, char *argv[])
{
    ifstream baza_odpowiedzi;
    baza_odpowiedzi.open("/home/murnko/Documents/test_creator/odpowiedzi.txt");
    size_t rozm_bazy = sprawdz_baze(baza_odpowiedzi);
    cout <<"Rozmiar bazy: " << rozm_bazy << endl;
    int ** dobre_odpowiedzi = new int*[(int)rozm_bazy/2-1];
    int ** student_odpowiedzi = new int*[(int)rozm_bazy/2-1];

    int ** punkty_odpowiedzi = new int*[(int)rozm_bazy/2-1];
    int * liczba_pytan = new int[(int)rozm_bazy/2];//pozycja 0 przechowuje informację o liczbie wariantów odpowiedzi wybranych dla danego tesu
    parsuj_odpowiedzi(baza_odpowiedzi,int(rozm_bazy),dobre_odpowiedzi,punkty_odpowiedzi,liczba_pytan);
    cout << liczba_pytan[(int)rozm_bazy/2];
//    for (int a=0;a<int(rozm_bazy)/2;a++){
//        cout << endl;
//        for (int b=0;b<liczba_pytan[a];b++){
//            cout << punkty_odpowiedzi[a][b];
//        }
//        cout << endl;
//    }
    int ** student_kratki = new int* [(int)rozm_bazy/2];
    cv::Mat inputImage = cv::imread("/home/murnko/Documents/test.jpg");
    if (inputImage.empty()) return -1;
    cv::resize(inputImage, inputImage,cvSize(0,0),0.5, 0.5);
    cv::Mat grayscale;
    cv::cvtColor(inputImage,grayscale,CV_BGR2GRAY);
    cv::Mat binary;
    cv::Canny(grayscale,binary,0,50,5);
//    cv::imshow("Display Image", binary);
//    cvWaitKey(0);
    vector<vector<cv::Point> >contours;
    cv::findContours(binary.clone(),contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    std::vector<cv::Point> approx;
    //cv::Mat dst = inputImage.clone();
    cv::Mat smallImage;
    for (vector<cv::Point> contour : contours)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        cv::approxPolyDP(
            cv::Mat(contour),
            approx,
            cv::arcLength(cv::Mat(contour), true) * 0.02,
            true
        );

        // Skip small or too big contours
        if (std::fabs(cv::contourArea(contour)) < 12000 || !cv::isContourConvex(approx))
            continue;
        cout << cv::contourArea(contour) <<endl;
          cv::Rect r = cv::boundingRect(contour);
          smallImage = cv::Mat(inputImage, r).clone();

          //cv::Mat crop(binary.rows, binary.cols, CV_8UC3);
          //cv::drawContours(mask, vector<vector<cv::Point>>(1,contour), -1, cv::Scalar(255), CV_FILLED);
          //crop.setTo(cv::Scalar(0,255,0));
          //inputImage.copyTo(crop,mask); // kopiuje
//        cv:: normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
          cv::imshow("pierwsza kolejka", smallImage);
        cvWaitKey(0);
    }


    cv::Size size(5,5);
    //cv::GaussianBlur(smallImage,smallImage,size,0);


    cv::cvtColor(smallImage,grayscale,CV_BGR2GRAY);
    cv::adaptiveThreshold(grayscale,binary,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
    //cv::Canny(grayscale,binary,0,50,5);
    cvWaitKey(0);
    cv::findContours(binary.clone(),contours,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
    cv::imshow("Druga kolejka", binary);
    //cv::bitwise_not(img, img);

    cv::Mat smallest;
    int       z = 0;
    cv::Mat crop(smallImage.rows, smallImage.cols, CV_8UC3);
    crop.setTo(cv::Scalar(0,255,0));
    cv::Mat mask = cv::Mat::zeros(smallImage.rows, smallImage.cols, CV_8UC1);
    cv:: normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
    for (vector<cv::Point> contour : contours)
    {
        cv::approxPolyDP(
            cv::Mat(contour),
            approx,
            cv::arcLength(cv::Mat(contour), true) * 0.08,
            true
        );
        if (std::fabs(cv::contourArea(contour)) < 500 || cv::contourArea(contour) > 800)
            continue;
        z++;
        cout << cv::contourArea(contour) <<endl;
        drawContours(mask, vector<vector<cv::Point> >(1,contour), -1, cv::Scalar(255), CV_FILLED);
          cv::Rect r = cv::boundingRect(contour);
          smallest = cv::Mat(binary, r).clone();

        cv::imshow("malutki", smallest);
        int wartosc = smallest.rows * smallest.cols - cv::countNonZero(smallest);
        cout<<"wartosc: "<< wartosc << endl;
        cv::imshow("aktualny", mask);
        cvWaitKey(0);

    }
    cv::imshow("small", smallImage);
    cv::imshow("maska", mask);
  cvWaitKey(0);
    smallImage.copyTo(crop, mask);
    cv::imshow("crop", crop);
  cvWaitKey(0);
cout<<"Znalezione kontury: "<< z <<endl;
}
