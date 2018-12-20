#include"palm.h"
pair<Point, double> DetectInCircles(vector<Point> contour, Mat src)
{

	Mat dist_image;
	distanceTransform(src, dist_image, CV_DIST_L2, 3);
	int temp = 0, R = 0, cx = 0, cy = 0;
	int d;
	for (int i = 0; i < src.rows; i++)
	{

		for (int j = 0; j<src.cols; j++)
		{
			/* checks if the point is inside the contour. 
			Optionally computes the signed distance from the point to the contour boundary
			*/
			d = pointPolygonTest(contour, Point2f(j, i), 0);
			if (d>0)
			{
				temp = (int)dist_image.ptr<float>(i)[j];
				if (temp>R)
				{
					R = temp;
					cy = i;
					cx = j;
				}

			}
		}
	}

	return make_pair(Point(cx, cy), R);

}



double matchShape(Mat src, vector<Point> contour)
{
	Mat img;
	Mat img_threshold;
	Mat img_gray;
	Mat img_roi;
	Mat dst;
	cvtColor(src, img_gray, CV_RGB2GRAY);
	medianBlur(img_gray, img_gray, 3);
	threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY);
	vector< vector<Point> > contours;	// ����
	vector< vector<Point> > filterContours;	// ɸѡ�������
	vector< Vec4i > hierarchy;	// �����Ľṹ��Ϣ
	vector< Point > hull;	// ͹����ĵ㼯
	vector< Point > hull2;	// ͹����ĵ㼯
	findContours(img_threshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
	// ȥ��α����
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (fabs(contourArea(Mat(contours[i]))) >1000)	//�ж��ֽ����������ֵ
		{
			filterContours.push_back(contours[i]);
		}
	}
	double comres;
	comres = matchShapes(filterContours[0], contour, CV_CONTOURS_MATCH_I1, 0.0);
	return comres;
}

