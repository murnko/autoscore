#include "main.h"
#include "funkcje.h"

struct kratka{
    int wartosc;
    int x;
    int y;
    cv::Rect r;
};
bool sortx(kratka const&lewa, kratka const&prawa) {return lewa.x < prawa.x; }
bool sorty(kratka const&lewa, kratka const&prawa) {return lewa.y > prawa.y; }
bool sortv(kratka const&lewa, kratka const&prawa) {return lewa.wartosc > prawa.wartosc; }
int main(int argc, char *argv[])
{
    ifstream baza_odpowiedzi;
    baza_odpowiedzi.open("/home/murnko/Documents/test_creator/odpowiedzi.txt");

    size_t rozm_bazy = sprawdz_baze(baza_odpowiedzi);
    //cout <<"Rozmiar bazy: " << rozm_bazy << endl;

    int ** dobre_odpowiedzi = new int*[(int)rozm_bazy/2-1]; //klucz
    int ** punkty_odpowiedzi = new int*[(int)rozm_bazy/2-1]; //klucz
    int * liczba_pytan = new int[(int)rozm_bazy/2];//klucz //pozycja ostatnia przechowuje informację o liczbie wariantów odpowiedzi wybranych dla danego testu

    //wypełnia powyższe
    parsuj_odpowiedzi(baza_odpowiedzi,int(rozm_bazy),dobre_odpowiedzi,punkty_odpowiedzi,liczba_pytan);
    int liczba_odp = liczba_pytan[(int)rozm_bazy/2];
    cout <<"ilosc odpowiedzi " << liczba_odp<<endl;

    int ** student_odpowiedzi = new int* [(int)rozm_bazy/2];//macierz testyXpytania dla odpowiedzi studenta
//    for (int a=0;a<int(rozm_bazy)/2;a++){
//        cout << endl;
//        for (int b=0;b<liczba_pytan[a];b++){
//            cout << punkty_odpowiedzi[a][b];
//        }
//        cout << endl;
//    }
    int suma_punkty;
    int student_punkty;



//JEDEN TEST
int t = 0;
    student_odpowiedzi[t] = new int[liczba_pytan[t]];
    //macierz do przechowywania wartości kratek dla jednego testu
    kratka ** test_kratki = new kratka*[liczba_pytan[t]+1];//jedno więcej na literki
    for (int p=0; p<liczba_pytan[t]+1;p++){
        test_kratki[p] = new kratka[liczba_odp+1];//jedno więcej na numerki
    }
    //cout <<"liczba pytan "<< liczba_pytan[t]<<endl;
    int liczba_kratek = (liczba_pytan[t]+1)*(liczba_pytan[((int)rozm_bazy/2)]+1);
    //cout << liczba_kratek<< endl;
    vector<kratka> wszystkie_kratki(liczba_kratek); //= new kratka[liczba_kratek];



//ZNALEZIENIE POLA ODPOWIEDZI
    cv::Mat inputImage = cv::imread("/home/murnko/Documents/testy6.jpg");
    if (inputImage.empty()) return -1;
    cv::resize(inputImage, inputImage,cvSize(0,0),0.5, 0.5);
    cv::Mat grayscale;
    cv::cvtColor(inputImage,grayscale,CV_BGR2GRAY);
    cv::Mat binary;
    cv::Canny(grayscale,binary,0,50,5);
    vector<vector<cv::Point> >contours;
    cv::findContours(binary.clone(),contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);
    std::vector<cv::Point> approx;
    //cv::Mat dst = inputImage.clone();
    cv::Mat smallImage;
    for (vector<cv::Point> contour : contours)
    {
        cv::approxPolyDP(
            cv::Mat(contour),
            approx,
            cv::arcLength(cv::Mat(contour), true) * 0.02,
            true
        );

        if (std::fabs(cv::contourArea(contour)) < 12000 || !cv::isContourConvex(approx))
            continue;
        //cout << cv::contourArea(contour) <<endl;
          cv::Rect r = cv::boundingRect(contour);
          smallImage = cv::Mat(inputImage, r).clone();
         // cv::imshow("pierwsza kolejka", smallImage);
    }

//ROZPOZNANIE POLA ODPOWIEDZI

    //cv::Size size(5,5);
    //cv::GaussianBlur(smallImage,smallImage,size,0);



//WERSJA Z LINIAMI Hougha
    cv::Mat mask_hough = cv::Mat::zeros(smallImage.rows, smallImage.cols, CV_8UC1);
    cv::Mat dst, cdst;
    cv::cvtColor(smallImage,grayscale,CV_BGR2GRAY);
    cv::Canny(grayscale, dst, 50, 200, 3);
    cv::adaptiveThreshold(grayscale,dst,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,75,10);
    cv::cvtColor(mask_hough, mask_hough, CV_GRAY2BGR);

    vector<cv::Vec2f> lines;
        // detect lines
        cv::HoughLines(dst, lines, 1, CV_PI/90, 100, 0, 0 );

        // draw lines
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            if( theta>CV_PI/180*175 || theta<CV_PI/180*5){
            cv::Point pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cv::line( mask_hough, pt1, pt2, cv::Scalar(255,255,255), 1, CV_AA);
            imshow("detected lines",  mask_hough);
            }
        }
        cv::HoughLines(dst, lines, 1, CV_PI/90, 300, 0, 0 );
        for( size_t i = 0; i < lines.size(); i++ )
        {
            float rho = lines[i][0], theta = lines[i][1];
            if( theta>CV_PI/180*88 && theta<CV_PI/180*92){
                cv::Point pt1, pt2;
                double a = cos(theta), b = sin(theta);
                double x0 = a*rho, y0 = b*rho;
                pt1.x = cvRound(x0 + 1000*(-b));
                pt1.y = cvRound(y0 + 1000*(a));
                pt2.x = cvRound(x0 - 1000*(-b));
                pt2.y = cvRound(y0 - 1000*(a));
                cv::line( mask_hough, pt1, pt2, cv::Scalar(255,255,255), 1, CV_AA);
                imshow("detected lines",  mask_hough);
            }
        }
        imshow("source", smallImage);
     cvWaitKey(0);

//KONIEC WERSJA Z LINIAMI Hougha
     int erosion_size =1;
     cv::Mat element = cv::getStructuringElement(0,
                                                 cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                                 cv::Point( erosion_size, erosion_size ) );




    cv::cvtColor(mask_hough,grayscale,CV_BGR2GRAY);
    cv::adaptiveThreshold(grayscale,binary,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV,75,10);
    //cv::Canny(grayscale,binary,0,50,5);
    //binary = grayscale;
    cv::findContours(binary.clone(),contours,CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);

    cv::cvtColor(smallImage,grayscale,CV_BGR2GRAY);
    cv::adaptiveThreshold(grayscale,binary,255,CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,75,10);
    cv::imshow("Druga kolejka", binary);
    cvWaitKey(0);
    //cv::bitwise_not(img, img);

    cv::Mat smallest;
    int z = 0;
    cv::Mat crop(smallImage.rows, smallImage.cols, CV_8UC3);
    crop.setTo(cv::Scalar(0,255,0));
    cv::Mat mask = cv::Mat::zeros(smallImage.rows, smallImage.cols, CV_8UC1);
    cv:: normalize(mask.clone(), mask, 0.0, 255.0, CV_MINMAX, CV_8UC1);
    float wartosc;
    for (vector<cv::Point> contour : contours)
    {
        cv::approxPolyDP(
            cv::Mat(contour),
            approx,
            cv::arcLength(cv::Mat(contour), true) * 0.08,
            true
        );
        if (std::fabs(cv::contourArea(contour)) < 450 || cv::contourArea(contour) > 650)
            continue;

        //cout <<"wielkosc: " << cv::contourArea(contour) <<endl;
        drawContours(mask, vector<vector<cv::Point> >(1,contour), -1, cv::Scalar(255), CV_FILLED);
          cv::Rect r = cv::boundingRect(contour);
          cout<< "wspolrzedne: "<<r.x<<"\t"<<r.y<<
                 "\trozmiar"<< r.area()<<endl;
          smallest = cv::Mat(binary, r).clone();

        cv::imshow("malutki", smallest);


         wartosc = 0.1*(smallest.rows * smallest.cols - cv::countNonZero(smallest));
         dilate( smallest, smallest, element );
         wartosc += smallest.rows * smallest.cols - cv::countNonZero(smallest);
         cv::imshow("erozja", smallest);
        wszystkie_kratki[z].wartosc = wartosc;
        wszystkie_kratki[z].x = r.x;
        wszystkie_kratki[z].y = r.y;
        wszystkie_kratki[z].r = r;

        //cout<<"wartosc: "<< wszystkie_kratki[z].wartosc << endl;
        //cout<<z<<endl;
        //cv::imshow("maska pierwsza", mask);cvWaitKey(0);

        z++;
    }
    if (z != liczba_kratek){
        cout <<"\nTest: "<<t<<"\tBłąd wykrywania!";
        exit(-2);
        //continue; //dla dużej pętli
    }


    //cvWaitKey(0);

//    cout << endl;
    sort(wszystkie_kratki.begin(),wszystkie_kratki.end(),sortx);
//    for (kratka g :wszystkie_kratki ) cout<<g.y<<"\t";
    int pos = 0;
    for (int p=0; p<liczba_pytan[t]+1; p++){
       cout << endl;
        for(int o=0; o<liczba_odp+1; o++){
            test_kratki[p][o] = wszystkie_kratki[pos];
            cout<<"y: "<<test_kratki[p][o].y <<"\tx: " <<test_kratki[p][o].x<< endl;
            pos++;
        }
    }
    cout <<"iteracje"<< pos<<endl;
    for(int p=0; p<liczba_pytan[t]+1; p++){
        sort(test_kratki[p],test_kratki[p]+liczba_odp+1,sorty);
    }


//    for (int p=0; p<liczba_pytan[t]+1; p++){
//        for(int o=0; o<liczba_odp+1; o++){
//            cout<<"y: "<<test_kratki[p][o].y <<"\tx: " <<test_kratki[p][o].x<< endl;
//            cout<< "wartosc: " << test_kratki[p][o].wartosc <<endl;
//            smallest = cv::Mat(binary, test_kratki[p][o].r).clone();
//            cv::imshow("malutki", smallest);
//            cvWaitKey(0);
//        }
//    }

    //cv::imshow("small", smallImage);
    cv::imshow("maska", mask);
  //cvWaitKey(0);
    smallImage.copyTo(crop, mask);
    cv::imshow("crop", crop);
  //cvWaitKey(0);
  int max_hist,odp,mediana,suma,srednia,dryf,wariancja;
  kratka *mediana_tmp = new kratka[liczba_odp];
  for(int p=1; p<liczba_pytan[t]+1; p++){
      max_hist = wariancja = suma =0;
      //mediana
      copy(test_kratki[p],test_kratki[p]+liczba_odp,mediana_tmp);
      sort(mediana_tmp,mediana_tmp+liczba_odp,sortv);
      if (liczba_odp%2){
          mediana = mediana_tmp[liczba_odp/2+1].wartosc;
      }
      else {
          mediana = (mediana_tmp[liczba_odp/2].wartosc+mediana_tmp[liczba_odp/2+1].wartosc)/2;
          dryf = abs((mediana_tmp[liczba_odp/2+1].wartosc - mediana_tmp[liczba_odp/2].wartosc)/2);
      }
       //srednia
      for (int o=0; o<liczba_odp   ; o++){
          //cout << "wartosc_end: "<<test_kratki[p][o].wartosc<<endl;
          suma += test_kratki[p][o].wartosc;
      }
       srednia = suma/liczba_odp;

       //wariancja
       for (int o=0; o<liczba_odp   ; o++){
       wariancja +=(test_kratki[p][o].wartosc-srednia)*(test_kratki[p][o].wartosc-srednia);
//       smallest = cv::Mat(binary, test_kratki[p][o].r).clone();
//                   cv::imshow("malutki", smallest);
//                   cvWaitKey(0);
       }
 //      cout << p << ": "<< wariancja <<endl;



        if(srednia<mediana){
            for (int o=0; o<liczba_odp   ; o++){
                if (test_kratki[p][o].wartosc < mediana+(liczba_odp%2)*dryf && test_kratki[p][o].wartosc > max_hist){
                    odp = o; max_hist = test_kratki[p][o].wartosc;
                }
                cout << "wartosc_end: " <<p<<": "<<test_kratki[p][o].wartosc<<endl;
            }
        }
        else{
            int min_hist = 10000;
            for (int o=0; o<liczba_odp   ; o++){
                if (test_kratki[p][o].wartosc > srednia+(!(liczba_odp%2))*dryf && test_kratki[p][o].wartosc < min_hist){
                    odp = o; min_hist = test_kratki[p][o].wartosc;
                }
                cout << "wartosc_end: " <<p<<": "<<test_kratki[p][o].wartosc<<endl;

               smallest = cv::Mat(binary, test_kratki[p][o].r).clone();
             cv::imshow("malutki", smallest);
              cvWaitKey(0);

            }
            cout<< "MED: "<<mediana<<endl;
            cout<<"AVG: "<<srednia<<" +dryf= "<<srednia+(!(liczba_odp%2))*dryf<<endl;
            cout<<"wybrany: "<< min_hist<<endl;
            cvWaitKey(0);
        }

      student_odpowiedzi[t][p-1] =  wariancja >1000 ? (liczba_odp-1-odp) : 9;

  }
suma_punkty = student_punkty =0;
for (int p=0; p<liczba_pytan[t]; p++)
{
      cout<<"odp: "<<student_odpowiedzi[t][p]<<endl;
      if (student_odpowiedzi[t][p] == dobre_odpowiedzi[t][p])
      {
          student_punkty += punkty_odpowiedzi[t][p];
      }
      suma_punkty += punkty_odpowiedzi[t][p];
}
cout<<"\nTEST: "<<t+1;
cout<<"\nSTUDENT: \t";

for (int p=0; p<liczba_pytan[t]; p++){
        if (student_odpowiedzi[t][p] < 9){
        cout <<(char)(student_odpowiedzi[t][p]+65)<< "  ";
        }
        else{cout<<"x  ";}
}
cout<<"\nKLUCZ:  \t";
for (int p=0; p<liczba_pytan[t]; p++){
        cout <<(char)(dobre_odpowiedzi[t][p]+65)<< "  ";
}
cout <<"\nWYNIK: " << student_punkty << "/"<<suma_punkty << "\t" <<(float)student_punkty/suma_punkty*100<<"%";
cout << endl;
//KONIEC JEDNEGO TESTU

//cout<<"Znalezione kontury: "<< z <<endl;
}
