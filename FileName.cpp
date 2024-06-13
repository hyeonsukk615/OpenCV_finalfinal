

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
void onMouse(int event, int x, int y, int flags, void* userdata);
void run(Mat canvas);
Mat canvas(500, 900, CV_8UC3, Scalar(255, 255, 255));// windows img
Mat img(500, 900, CV_8UC3, Scalar(255, 255, 255));// windows img
void Set(Mat canvas) { // interface function
    imshow("windows", canvas); // img print
    vector<vector<string>> text = { {"Save", "Load", "Clear", "Run", "Exit"},
    {"CuTX", "find", "Cent", "pixels", "4"} };
    Mat img_size(100, 200, CV_8UC3, Scalar(255, 255, 255));
    for (int i = 0; i < text.size(); i++) {
        for (int j = 0; j < text[i].size(); j++) {
            putText(canvas, text[i][j], Point(200 * i + 520, 100 * j + 70), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 3);
            rectangle(canvas, Rect(500 + (200 * i), 100 * j, 200, 100), Scalar(0, 0, 0), 3);
        }
    }
};

Point average(Mat canvas) {// �ܰ��� ��� ��ǥ�� ���ϱ�
    Mat binaryImage = canvas.clone();
    Mat final1;
    double X = 0, Y = 0, Z = 0;
    vector<vector<Point>> contours;
    cvtColor(binaryImage, final1, COLOR_BGR2GRAY);
    threshold(final1, final1, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    findContours(final1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    for (int i = 0; i < contours.size(); ++i) {
        for (int j = 0; j < contours[i].size(); ++j) {
            X += contours[i][j].x;
            Y += contours[i][j].y;
            Z++;
        }
    }
    return Point(X / Z, Y / Z);
};
int findContours1(Mat canvas) {// �ܰ��� �� ����
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone();
    resize(binaryImage, binaryImage, Size(500, 500));
    Point avg = average(canvas);
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    Mat final1, final2;
    cvtColor(binaryImage, final1, COLOR_BGR2GRAY);
    morphologyEx(final1, final1, MORPH_OPEN, element);
    threshold(final1, final1, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    vector<vector<Point>> contours;
    findContours(final1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    cout << "�ܰ��� ��: " << contours.size() << endl;
    return (int)contours.size();
};

int findContours2(Mat canvas) {// �ܰ��� �� ����
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone();
    resize(binaryImage, binaryImage, Size(500, 500));
    Point avg = average(canvas);
    Mat element = getStructuringElement(MORPH_RECT, Size(10, 10));
    Mat final1, final2;
    cvtColor(binaryImage, final1, COLOR_BGR2GRAY);
    morphologyEx(final1, final1, MORPH_OPEN, element);
    threshold(final1, final1, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    vector<vector<Point>> contours;
    findContours(final1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    return (int)contours.size();
};
int findContours3(Mat canvas) {// �ܰ��� �� ����
    Mat binaryImage = canvas.clone();
    Point avg = average(canvas);
    Mat final1, final2;
    cvtColor(binaryImage, final1, COLOR_BGR2GRAY);
    threshold(final1, final1, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    vector<vector<Point>> contours;
    findContours(final1, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
    return (int)contours.size();
};



int pixels(Mat canvas) {// �ȼ��� ���ϱ�
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone();
    resize(binaryImage, binaryImage, Size(500, 500));
    Mat final1, binary;
    cvtColor(binaryImage, final1, COLOR_BGR2GRAY);
    threshold(final1, binary, 100, 255, THRESH_BINARY);
    int whitePixelCount = countNonZero(binary);
    int totalPixelCount = binary.rows * binary.cols;
    int blackPixelCount = totalPixelCount - whitePixelCount;
    //cout << "White pixels: " << whitePixelCount << endl;
    cout << "Black pixels: " << blackPixelCount << endl;
    return blackPixelCount;
};

Mat BOX(Mat canvas) {// �ٿ�� �ڽ��ϰ� ��ǥ�� ���ϱ�
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone(), save;
    resize(binaryImage, binaryImage, Size(500, 500));
    Mat gray, bin, final1;
    int minX = INT_MAX, minY = INT_MAX, maxX = 0, maxY = 0;
    cvtColor(binaryImage, gray, COLOR_BGR2GRAY);// �׷��̽����� ��ȯ
    threshold(gray, bin, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);// ����ȭ
    vector<vector<Point>> contours;
    findContours(bin, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);// �ܰ��� ����
    cvtColor(bin, final1, COLOR_GRAY2BGR);
    for (const auto& contour : contours) {// �ٿ�� �ڽ�, �ּ� ���� �簢��, �ּ� ���� �� �׸���
        for (const auto& point : contour) {// ������ �ٿ�� �ڽ� �׸���
            minX = min(minX, point.x);
            minY = min(minY, point.y);
            maxX = max(maxX, point.x);
            maxY = max(maxY, point.y);
        }
        rectangle(final1, Point(minX, minY), Point(maxX, maxY), Scalar(0, 0, 255), 2);
    }
    save = binaryImage(Rect(minX, minY, maxX - minX, maxY - minY)).clone();
    imshow("Contours", save);
    return save;
};

void CuTX(Mat canvas) {
    int rows = canvas.rows;
    int cols = canvas.cols - 600;
    // ������ ������ �ڸ���
    Rect rightHalfRect(cols / 2, 0, cols / 2, rows);
    Mat rightHalf = canvas(rightHalfRect);// ������ ���ݿ��� �ʺ� 1�� �� ���簢�� ����
    Rect thinRightRect(0, 0, 1, rows);
    Mat thinRight = rightHalf(thinRightRect);
    // �Ʒ��� ������ �ڸ���
    Rect bottomHalfRect(0, rows / 2, cols, rows / 2);
    Mat bottomHalf = canvas(bottomHalfRect);// �ϴ� ���ݿ��� ���̰� 1�� �� ���簢�� ����
    Rect thinBottomRect(0, 0, cols, 1);
    Mat thinBottom = bottomHalf(thinBottomRect);
    imshow("ContourR", thinRight);
    imshow("ContourB", thinBottom);
};

vector<Point2f> cent(Mat canvas) {
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone();
    resize(binaryImage, binaryImage, Size(500, 500));
    vector<vector<Point>> trs;
    cvtColor(binaryImage, binaryImage, COLOR_BGR2GRAY);
    threshold(binaryImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    morphologyEx(binaryImage, binaryImage, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
    Mat labels, stats, centroids;
    int cnt = connectedComponentsWithStats(binaryImage, labels, stats, centroids);
    findContours(binaryImage, trs, RETR_LIST, CHAIN_APPROX_NONE);
    cvtColor(binaryImage, binaryImage, COLOR_GRAY2BGR);
    vector<Point2f> tr;
    Point2f ct(0, 0);
    for (int i = 0; i < trs.size(); i++) {
        for (int j = 0; j < trs[i].size(); j++) {
            ct.x = ct.x + trs[i][j].x;
            ct.y = ct.y + trs[i][j].y;
        }
        ct.x = ct.x / trs[i].size();
        ct.y = ct.y / trs[i].size();
        tr.push_back(ct);
        cout << "cen" << i + 1 << "tr: " << tr[i].x << ", " << tr[i].y << endl;
        circle(binaryImage, tr[i], 5, Scalar(0, 0, 255), -1);
    }
    imshow("center", binaryImage);
    return tr;
};
vector<Point2f> cent1(Mat canvas) {
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone();
    resize(binaryImage, binaryImage, Size(500, 500));
    vector<vector<Point>> trs;
    cvtColor(binaryImage, binaryImage, COLOR_BGR2GRAY);
    threshold(binaryImage, binaryImage, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);
    morphologyEx(binaryImage, binaryImage, MORPH_CLOSE, Mat(20, 20, CV_8UC1));
    Mat labels, stats, centroids;
    int cnt = connectedComponentsWithStats(binaryImage, labels, stats, centroids);
    findContours(binaryImage, trs, RETR_LIST, CHAIN_APPROX_NONE);
    cvtColor(binaryImage, binaryImage, COLOR_GRAY2BGR);
    vector<Point2f> tr;
    Point2f ct(0, 0);
    for (int i = 0; i < trs.size(); i++) {
        for (int j = 0; j < trs[i].size(); j++) {
            ct.x = ct.x + trs[i][j].x;
            ct.y = ct.y + trs[i][j].y;
        }
        ct.x = ct.x / trs[i].size();
        ct.y = ct.y / trs[i].size();
        tr.push_back(ct);
        circle(binaryImage, tr[i], 5, Scalar(0, 0, 255), -1);
    }
    return tr;
};

void run(Mat canvas) {
    Mat binaryImage = canvas(Rect(1, 1, 498, 498)).clone(), result;
    resize(binaryImage, binaryImage, Size(500, 500));
    int find = findContours2(canvas);
    int blackpi = pixels(canvas);
    vector<Point2f> cent = cent1(canvas);
    if (find <= 0) { cout << "�Է��Ͻ� ���ڰ� �����ϴ�." << endl; }
    else if (find == 3) { cout << "�Ǻ��� ���ڴ� 8 �Դϴ�. " << endl; }
    else if (find == 2) {
        if ((cent[0].y < cent[1].y) ) { cout << "�Ǻ��� ���ڴ� 9 �Դϴ�. " << endl; }
        else if ((cent[0].y - cent[1].y) < 20 && cent[0].y - cent[1].y > -20) { cout << "�Ǻ��� ���ڴ� 0 �Դϴ�. " << endl; }
        else if (cent[0].y > cent[1].y) { cout << "�Ǻ��� ���ڴ� 6 �Դϴ�. " << endl; }
    }
    else if (find == 1) {
        cout << "�Ǻ��� ���ڴ� 1 �Դϴ�. " << endl;
    }
    else {
        cout << "���ڰ� �ƴմϴ�. " << endl;
    }
}
void onMouse(int event, int x, int y, int flags, void* userdata) {// Mouse event function
    static bool drawing = false;
    static Point prevPoint(-1, -1);
    Mat canvas = *(Mat*)userdata;
    Rect area(0, 0, 500, 500);// img area
    int count = 0;
    int minX = 0, minY = 0, maxX = 0, maxY = 0;
    switch (event) {
    case EVENT_LBUTTONDOWN:// mouse L down
        prevPoint = Point(x, y);
        if (area.contains(Point(x, y))) { drawing = true; }// 500,500 area inspect
        if (500 < x && x < 600) {// draw area
            if (y > 400) { exit(1); }// exit area
            else if (y > 300) { run(canvas); }// run area
            else if (y > 200) {
                Rect clearButton(500, 200, 200, 100);
                if (clearButton.contains(Point(x, y))) {
                    canvas = (500, 700, CV_8UC3, Scalar(255, 255, 255));
                    cout << "windows Clear" << endl;
                    Set(canvas);
                }
            }// clear area
            else if (y > 100) {
                string Name = "";
                cout << "load file name: ";
                cin >> Name;
                Mat number = imread(Name);
                number.copyTo(canvas(Rect(0, 0, 500, 500)));
            }// load area
            else if (y > 0) {
                string Name = "";
                Mat save = canvas(Rect(1, 1, 498, 498)).clone();
                resize(save, save, Size(500, 500));
                cout << "save file name : ";
                cin >> Name;
                Name = Name + ".jpg";
                imwrite(Name, save);
                cout << Name << " file saved" << endl;
            }// save area
        }
        else if (700 < x) {
            if (y > 400) { cout << "5"; }// �ٿ�� �ڽ��ϰ� ��ǥ�� ���ϱ�
            else if (y > 300) { pixels(canvas); }// �ȼ��� ���ϱ�
            else if (y > 200) { cent(canvas); }// �ܰ��� �����ǥ ���ϱ�
            else if (y > 100) { findContours1(canvas); }// �ܰ��� �� ���ϱ�
            else if (y > 0) { CuTX(canvas); }// �����Լ�
        }

    case EVENT_MOUSEMOVE:// mouse move
        if (drawing) {
            if (500 > x) {
                line(canvas, prevPoint, Point(x, y), Scalar(0, 0, 0), 10);
                prevPoint = Point(x, y);
            }
        }
        break;
    case EVENT_LBUTTONUP:// mouse L up
        drawing = false;
        break;
    }
};
int main() {
    namedWindow("windows", WINDOW_AUTOSIZE);// window named set
    setMouseCallback("windows", onMouse, &canvas);// mouse call back
    while (true) { Set(canvas); char key = waitKey(10); }
    return 0;
}
