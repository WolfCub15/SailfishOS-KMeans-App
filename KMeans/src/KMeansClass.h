#ifndef KMEANSCLASS_H
#define KMEANSCLASS_H

#include <iostream>
#include <vector>
#include <qimage.h>
#include <QObject>
#include <QMetaObject>
#include <QDebug>
#include <QUrl>
#include <QDir>
#include <time.h>
#include <random>
#include <QFile>
#include <QBuffer>


using namespace std;
const int max_iterations = 500;
//const int color_r[15]{0,255,74,0,0,0,125,0,252,231,103,200,248,57,0};
//const int color_g[15]{73,41,0,255,0,0,128,255,91,108,0,131,255,10,53};
//const int color_b[15]{0,20,62,0,238,99,0,249,255,0,16,255,142,0,64};
typedef struct {
    double r;
    double g;
    double b;
} rgb;


class KMeansClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ getUrl WRITE set NOTIFY calcKMeans)
    Q_PROPERTY(int n_klaster READ getNKlaster WRITE setNKlaster NOTIFY calcKMeans)

private:
    vector<pair<rgb, pair<int, int>>> pixcel;
    int k_klaster;
    int k_pixcel;
    int n_klaster;
    QImage photo;
    QImage resultImage;
    QString url = "null";

    vector<rgb> centr;
    void centers() {
        mt19937 gen;
        gen.seed(static_cast<unsigned int>(time(0)));
        vector<rgb> tmp_centr(k_klaster);
        for (int i = 0; i < k_klaster; i++) {
            tmp_centr[i] = pixcel[gen() % k_pixcel].first;
        }
        for (int i = 0; i < k_klaster; i++) {
            centr.push_back(tmp_centr[i]);
        }
        tmp_centr.resize(0);
    }

    double dist(rgb x, rgb y){
        return sqrt((x.r - y.r)* (x.r - y.r) + (x.g - y.g)*(x.g - y.g) + (x.b - y.b)*(x.b - y.b));
    }

    double average(double a, double b){
        return (a + b) / 2;
    }

    int clamp(int v, int max, int min) {//текущее значение, максимальное и минимальное значение
        if (v > max) return max;
        else if (v < min) return min;
        else return v;
    }

public:

    Q_INVOKABLE KMeansClass(): QObject() {
        k_klaster = 0;
        k_pixcel = 0;
        n_klaster = 10;
    }

    int getNKlaster() {
        return n_klaster;
    }

    void setNKlaster(int val){
        n_klaster = val;
    }

    QString getUrl() {
        return url;
    }


    Q_INVOKABLE void set(QString _url){
        url = _url;
        url.remove(0,3);

        QImage ph;
        ph.load(url);

        photo = ph;
        resultImage = calc();
        emit calcKMeans();
    }

    Q_INVOKABLE QString reciveImage() const {
         QByteArray bArray;
         QBuffer buffer(&bArray);
         buffer.open(QIODevice::WriteOnly);
         resultImage.save(&buffer, "JPEG");
         QString newStr("data:image/jpg;base64,");
         newStr.append(QString::fromLatin1(bArray.toBase64().data()));

         return newStr;

    }
    Q_INVOKABLE void clear(){
        pixcel.resize(0);
        centr.resize(0);
    }

    QImage calc(){
        rgb tmp;
        k_klaster = n_klaster;
        QImage result_image(photo);
        for (int x = 0; x < photo.width(); x++) {
            for (int y = 0; y < photo.height(); y++) {
                QColor color = photo.pixelColor(x, y);
                tmp.r = color.red();
                tmp.g = color.green();
                tmp.b = color.blue();
                pixcel.push_back(make_pair(tmp, make_pair(x, y)));
            }
        }
        k_pixcel = pixcel.size();
        centers();

        vector<int> vec1(k_pixcel, -1) , vec2(k_pixcel, -2);
        int iteration = 0;

        while (1) {
            if (vec1 == vec2 || iteration >= max_iterations) break;

            for (int i = 0; i < k_pixcel; i++) {
                vector<double>d(k_klaster);
                for (int j = 0; j < k_klaster; j++) {
                    d[j] = dist(pixcel[i].first, centr[j]);
                }
                double min_dist = d[0];
                int mi = 0;
                for (int j = 0; j < k_klaster; j++) {
                    if (min_dist > d[j]) {
                        min_dist = d[j];
                        mi = j;//Минимальное расстояние к центру
                    }
                }
                //пересчитываем центр
                centr[mi].r = average(pixcel[i].first.r, centr[mi].r);
                centr[mi].g = average(pixcel[i].first.g, centr[mi].g);
                centr[mi].b = average(pixcel[i].first.b, centr[mi].b);
                d.resize(0);
            }

            vector<int> vec_pix(k_pixcel);
            for (int k = 0; k < k_pixcel; k++) {
                vector<double>d(k_klaster);
                for (int i = 0; i < k_klaster; i++) {
                    d[i] = dist(pixcel[k].first, centr[i]);//расстояние до каждого центра
                }
                double min_dist = d[0];
                int mi = 0;
                for (int i = 0; i < k_klaster; i++) {
                    if (min_dist > d[i]) {
                        min_dist = d[i];
                        mi = i;
                    }
                }
                vec_pix[k] = mi;
            }

            for (int i = 0; i < k_pixcel; i++) {
                vec1[i] = vec_pix[i];//Массив соответствия пикселей и центров
            }

            for (int i = 0; i < k_klaster; i++) {
                int r = rand() % 300;
                int g = rand() % 400;
                int b = rand() % 255;
                for (int j = 0; j < k_pixcel; j++) {
                    if (vec_pix[j] == i) {
                        QColor res = QColor(clamp(r, 255, 0), clamp(g, 255, 0), clamp(b, 255, 0));
                        result_image.setPixelColor(pixcel[j].second.first, pixcel[j].second.second, res);
                    }
                }
            }
            vec_pix.resize(0);
            vec2 = vec1;
            iteration++;
        }
        resultImage = result_image;

        resultImage.save("kmeans.jpg");

        return result_image;
    }

signals:
    void calcKMeans();

private slots:
    void print() {
        qDebug() << "url: " << qPrintable(url);
    }
};



#endif // KMEANSCLASS_H
