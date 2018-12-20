#include"palm.h"
#include <string>
#include<vector>
#include<stdint.h>

//#include"define.h"
#define MAXLINE 253600
#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
char buf[MAXLINE];
char ptr_buf2[MAXLINE];
char sendline[] = "getDistanceSorted";
uint16_t raw_dep;
int scale = 1;
int delta = 0;
int ddepth = -1;
int realindex, realrow, realcol;
//vector<vector<uint16_t> >fameDepthArray;
uint16_t fameDepthArray2[MAXLINE];
uint16_t depth[240][320];
//ushort raw_array[322][239];
//ushort real_array[321][239];
unsigned char* ptr_buf_unsigned = (unsigned char*)ptr_buf2;

vector<vector<Point>> g_TemplateContours;
vector<vector<Point>> g_TemplateContours_2;
vector<vector<Point>> g_TemplateContours_3;
vector<vector<Point>> g_TemplateContours_4;
vector<vector<Point>> g_TemplateContours_5;
vector<vector<Point>> g_TemplateContours_6;

double area_len[10];
double area_sample;
double length_sample;
double areabileng_sample;
//模板个数
int g_handTNum =41;
int g_handTNum_2 = 36;
int g_handTNum_3 = 31;
int n;
void socket_com(char sendline[], int length)
{

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	char* ptr_buf = buf;
	int count = 0;
	SOCKET  sockfd;
	int rec_len, Ret;
	struct sockaddr_in servaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET)
	{
		cout << "Create Socket Failed::" << GetLastError() << endl;
		exit(0);
	}


	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(50660);
	servaddr.sin_addr.s_addr = inet_addr("192.168.7.62");
	//if (inet_pton(AF_INET, "192.168.7.3", &servaddr.sin_addr) <= 0) {
	//	printf("inet_pton error for %s\n", "ip");
	//	exit(0);
	//}
	Ret = connect(sockfd, (SOCKADDR*)&servaddr, sizeof(servaddr));
	if (Ret == SOCKET_ERROR)
	{
		cout << "Connect Error::" << GetLastError() << endl;
		exit(0);
	}

	else
	{
		cout << "连接成功!" << endl;
	}
	/*
	if (connect(sockfd, (SOCKADDR*)&servaddr, sizeof(SOCKADDR))
	< 0) {
	printf("connect error: %s(errno: %d)\n", strerror(errno), errno);
	exit(0);
	}
	*/
	Ret = send(sockfd, sendline, strlen(sendline), 0);
	if (Ret == SOCKET_ERROR)
	{
		cout << "Send Info Error::" << GetLastError() << endl;
		exit(0);
	}
	else
	{
		cout << "send成功!" << endl;
	}
	/*
	if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
	printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);
	exit(0);
	}
	*/

	int i2 = 0;
	while (count < length) //153600
	{
		rec_len = recv(sockfd, buf, MAXLINE, 0);
		for (int i = 0; i < rec_len; i++)
		{

			ptr_buf2[i2] = buf[i];
			i2++;

		}
		if (rec_len == SOCKET_ERROR)
		{
			cout << "接受Error::" << GetLastError() << endl;
			exit(0);
		}

		count = count + rec_len;

	}
	//关闭套接字
	closesocket(sockfd);
	//终止使用 DLL
	WSACleanup();

	//system("pause");

}

void init_hand_template() 
{
	//int handTNum = 10;
	string temp = "HandTemplate1_R//";

	for (int i = 1; i<g_handTNum; i++)
	{
		stringstream ss;
		ss <<"1 ("<<i<<")" << ".bmp";

		string fileName = temp + ss.str();

		//读入灰度图像
		Mat src = imread(fileName);
		//Mat src = imread("i1.bmp");
		Mat img_gray, img_threshold;
	
		if (!src.data)
		{
			printf("未找到文件: %s/n", fileName);
			continue;
		}
		vector< vector<Point>> filterContours;	// 筛选后的手势轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		cvtColor(src, img_gray, CV_RGB2GRAY);
		medianBlur(img_gray, img_gray, 3);
		//threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY);
		findContours(img_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int idx = 0;
		double maxArea = 0.0;
		int maxId = -1;
		for (size_t i = 0; i < contours.size(); i++)
		{
			Mat temp(contours.at(i));
			double area = fabs(contourArea(temp));
			
			
			if (area >maxArea)	//判断手进入区域的阈值
			{
				maxId = i;
				maxArea = area;

			}
		}
		filterContours.push_back(contours[maxId]);
	   // area_sample = fabs(contourArea(Mat(filterContours[0])));
		//length_sample = fabs(arcLength(Mat(filterContours[0]), true));
		//areabileng_sample = area_sample / length_sample;  //面积周长比
		//findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		g_TemplateContours.push_back(filterContours[0]);
		//area_len[i-1] = areabileng_sample;  //i 0-8
	}
	
}
////////////////////////////第二组////////////////////////////
void init_hand_template_2()
{
	//int handTNum = 10;
	//string temp = "HandTemplate4/";
	string temp = "HandTemplate2_R/";
	for (int i = 1; i<g_handTNum_2; i++)
	{
		stringstream ss;
		ss << "2 (" << i << ")" << ".bmp";

		string fileName = temp + ss.str();

		//读入灰度图像
		Mat src = imread(fileName);
		Mat img_gray, img_threshold;

		if (!src.data)
		{
			printf("未找到文件: %s/n", fileName);
			continue;
		}
		vector< vector<Point>> filterContours;	// 筛选后的手势轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		cvtColor(src, img_gray, CV_RGB2GRAY);
		medianBlur(img_gray, img_gray, 3);
		//threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY);
		findContours(img_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int idx = 0;
		double maxArea = 0.0;
		int maxId = -1;
		for (size_t i = 0; i < contours.size(); i++)
		{
			Mat temp(contours.at(i));
			double area = fabs(contourArea(temp));


			if (area >maxArea)	//判断手进入区域的阈值
			{
				maxId = i;
				maxArea = area;

			}
		}
		filterContours.push_back(contours[maxId]);
		// area_sample = fabs(contourArea(Mat(filterContours[0])));
		//length_sample = fabs(arcLength(Mat(filterContours[0]), true));
		//areabileng_sample = area_sample / length_sample;  //面积周长比
		//findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		g_TemplateContours_2.push_back(filterContours[0]);
		//area_len[i-1] = areabileng_sample;  //i 0-8
	}

}
//////////////////////////////////////////////////////////////////////

void init_hand_template_3()
{
	//int handTNum = 10;
	//string temp = "HandTemplate5/";
	string temp = "HandTemplate3_R/";

	for (int i = 1; i<g_handTNum_3; i++)
	{
		stringstream ss;
		ss << "3 (" << i << ")" << ".bmp";

		string fileName = temp + ss.str();

		//读入灰度图像
		Mat src = imread(fileName);
		Mat img_gray, img_threshold;

		if (!src.data)
		{
			printf("未找到文件: %s/n", fileName);
			continue;
		}
		vector< vector<Point>> filterContours;	// 筛选后的手势轮廓
		vector<vector<Point>> contours;
		vector<Vec4i> hierarchy;
		cvtColor(src, img_gray, CV_RGB2GRAY);
		medianBlur(img_gray, img_gray, 3);
		//threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY);
		findContours(img_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
		int idx = 0;
		double maxArea = 0.0;
		int maxId = -1;
		for (size_t i = 0; i < contours.size(); i++)
		{
			Mat temp(contours.at(i));
			double area = fabs(contourArea(temp));


			if (area >maxArea)	//判断手进入区域的阈值
			{
				maxId = i;
				maxArea = area;

			}
		}
		filterContours.push_back(contours[maxId]);
		// area_sample = fabs(contourArea(Mat(filterContours[0])));
		//length_sample = fabs(arcLength(Mat(filterContours[0]), true));
		//areabileng_sample = area_sample / length_sample;  //面积周长比
		//findContours(src, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		g_TemplateContours_3.push_back(filterContours[0]);
		//area_len[i-1] = areabileng_sample;  //i 0-8
	}

}

int hand_template_match(Mat& hand, vector<vector<Point>> g_TemplateContours, int g_handTNum)
{
	int minId = -1;
	double minHu = 1;
	double hu;
	int method = CV_CONTOURS_MATCH_I1;
	for (int i = 0; i<g_handTNum-1; i++)
	{

		Mat temp(g_TemplateContours.at(i));
		hu = matchShapes(temp, hand, method, 0.0);
		//cout << "相似度(越小越相似)--> " << hu<<endl;
		//cout << "面积周长比--> " << area_len[i] << endl;
		//找到hu矩最小的模板
		if (hu < minHu){
			minHu = hu;
			minId = i;
		}

		
	}

	//显示匹配结果
	//int Hmatch_value = 15;//模板匹配系数 ，不考虑

	//if (minHu<((double)Hmatch_value) / 100)
		return minId;
	//else
		//return -1;
}


int main()
{
		
		init_hand_template();
		init_hand_template_2();
	
		Mat img;
		Mat img_threshold;
		Mat img_gray;
		Mat img_roi;

		int count = 0;
		double hu[7];
		char filename[100];
		while (1)
		{

			socket_com(sendline, 153600);
			double time0 = static_cast<double>(getTickCount());
			for (int j = 0; j < 76800; j++)
			{
				raw_dep = ptr_buf_unsigned[j * 2 + 1] * 256 + ptr_buf_unsigned[2 * j];//按数据传送序列，找到每一个像素点相应的深度值
				realindex = 76800 - (j / 320 + 1) * 320 + j % 320;   //镜像
				realrow = 239 - j / 320;                             //行
				realcol = j % 320;                                   //列
				fameDepthArray2[realindex] = raw_dep;
		
			}
			for (int i = 0; i < 240; i++)
			{
				for (int j = 0; j < 320; j++)
				{
					depth[i][j] = fameDepthArray2[i * 320 + j];
				}
			}

			Mat src_1(240, 320, CV_16UC1, Scalar(0));
			Mat src_2(240, 320, CV_8UC1, Scalar(0));
			for (int i = 0; i < 240; i++)
			{
				for (int j = 0; j < 320; j++)
				{
					src_1.at<ushort>(i, j) = depth[i][j];

				}

			}
			//imshow("原图src", src);
			for (int i = 0; i < 240; i++)
			{
				for (int j = 0; j < 320; j++)
				{
					if (src_1.at<ushort>(i, j)>1000)
					{
						src_1.at<ushort>(i, j) = 0;
					}
					else
						src_1.at<ushort>(i, j) = 255;
				}
			}
			//

			for (int i = 0; i < 240; i++)
			{
				for (int j = 0; j < 320; j++)
				{
					src_2.at<uchar>(i, j) = src_1.at<ushort>(i, j);
					//cout << src2.at<uchar>(i, j) << endl;
				}

			}

			
			sprintf_s(filename, "HandTest3_R\\%d.bmp", n++);
		
			//imwrite(filename, src_2);
		
			imshow("原图src", src_2);
			//cvWaitKey(0);
			vector< vector<Point> > contours;	// 轮廓
			vector< vector<Point> > filterContours;	// 筛选后的轮廓
			vector< Vec4i > hierarchy;	// 轮廓的结构信息
			vector< Point > hull;	// 凸包络的点集
			vector< Point > hull2;	// 凸包络的点集


			cvtColor(src_2, img_gray, CV_GRAY2BGR);
			cvtColor(img_gray, img_gray, CV_RGB2GRAY);			
			//进行形态学操作  
			medianBlur(img_gray, img_gray, 3);
			
			findContours(img_gray, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point());
			// 去除伪轮廓
			int idx = 0;
			double maxArea = 0.0;
			int maxId = -1;
			for (size_t i = 0; i < contours.size(); i++)
			{
				Mat temp(contours.at(i));
				double area = fabs(contourArea(temp));
				if (area >maxArea)	//判断手进入区域的阈值
				{
					maxId = i;
					maxArea = area;

				}
			}
			if (maxArea<2000)
			{
				count = 0;

			}

			else
			{

				filterContours.push_back(contours[maxId]);
				cout << "轮廓数: " << filterContours.size() << endl;

				// 画轮廓
				vector< vector<int> > hull_int(filterContours.size());
				vector< vector<int> > hull_int2(filterContours.size());
				vector<vector<Point> >hull_point(filterContours.size());
				vector<vector<Point> >hull_point2(filterContours.size());
				vector<vector<Vec4i>> defects(filterContours.size());

				Mat drawing = Mat::zeros(src_2.size(), CV_8UC3);
				Mat drawing2 = Mat::zeros(src_2.size(), CV_8UC3);
				Mat dst = Mat::zeros(src_2.size(), CV_8UC3);
				Mat dst2 = Mat::zeros(src_2.size(), CV_8UC3);
				Mat src, src2, src_gray, src_gray2;
				drawContours(drawing, filterContours, -1, Scalar(255, 255, 255), -1); //轮廓图填充白色
				drawContours(drawing2, filterContours, -1, Scalar(255, 255, 255), 1); //轮廓图填充白色
				drawing.copyTo(src);
				drawing2.copyTo(src2);
				cvtColor(src, src_gray, CV_RGB2GRAY);
				cvtColor(src2, src_gray2, CV_RGB2GRAY);
				
				Moments mts = moments(src_gray); //灰度输入
				HuMoments(mts, hu);
			

				for (int j = 0; j < filterContours.size(); j++)
				{
					
					
					double area = fabs(contourArea(Mat(filterContours[j])));
				

					pair<Point, double> m = DetectInCircles(filterContours[j], src_gray);
				
					convexHull(Mat(filterContours[j]), hull, true);//顺时
					convexHull(Mat(filterContours[j]), hull_int[j], true);//顺时

					//绘制凸包
		        	int t = 0;
					int hullcount = (int)hull_int[j].size();

					for (int i = 0; i < hullcount; i++)
					{
						int ind = hull_int[j][i];
						hull_point[j].push_back(filterContours[j][ind]);
					}

					for (int i = 0; i < hullcount - 1; i++)
					{
						
						float tmp = sqrt((float)(hull_point[j][i].x - hull_point[j][i + 1].x)*(hull_point[j][i].x - hull_point[j][i + 1].x) +
							(hull_point[j][i].y - hull_point[j][i + 1].y)* (hull_point[j][i].y - hull_point[j][i + 1].y));

						if (tmp>5)
						{
							hull_point2[j].push_back(hull_point[j][i]);
							hull_int2[j].push_back(hull_int[j][i]);
						}

					}
					float tmp0 = sqrt((float)(hull_point[j][hullcount - 1].x - hull_point[j][0].x)*(hull_point[j][hullcount - 1].x - hull_point[j][0].x) +
						(hull_point[j][hullcount - 1].y - hull_point[j][0].y)* (hull_point[j][hullcount - 1].y - hull_point[j][0].y));
					if (tmp0 > 5)
					{
						hull_point2[j].push_back(hull_point[j][hullcount - 1]);
						hull_int2[j].push_back(hull_int[j][hullcount - 1]);
					}
					hullcount = (int)hull_int2[j].size();			
					float tmpr, r = 0, R = 0;
					for (int i = 0; i < hullcount - 1; i++)
					{

						//计算中心到边缘的最大距离
						tmpr = sqrt((float)(m.first.x - hull_point2[j][i].x)*(m.first.x - hull_point2[j][i].x) + (m.first.y - hull_point2[j][i].y)*(m.first.y - hull_point2[j][i].y));//计算与中心点的大小
						if (tmpr>r)
							r = tmpr;  //得到最大半径						
						line(drawing, hull_point2[j][i + 1], hull_point2[j][i], Scalar(0, 0, 255), 1, CV_AA);
					}
					line(drawing, hull_point2[j][hullcount - 1], hull_point2[j][0], Scalar(0, 0, 255), 1, CV_AA);
					circle(drawing, m.first, r, Scalar(0, 255, 0), 1);					
					convexityDefects(Mat(filterContours[j]), hull_int2[j], defects[j]);  //缺陷检测
					//绘制缺陷
					vector<Vec4i>::iterator d = defects[j].begin();

					while (d != defects[j].end())
					{

						Vec4i& v = (*d);
						int startidx = v[0];

						Point ptStart(filterContours[j][startidx]); // 缺陷开始轮廓凸点
						int endidx = v[1];
						Point ptEnd(filterContours[j][endidx]); // point of the contour where the defect ends
						int faridx = v[2];
						Point ptFar(filterContours[j][faridx]);//  farthest
						int depth = v[3] / 256; // 距离
						if (depth > 27.5)
						{
							count++;							
							line(drawing, ptStart, ptFar, Scalar(255, 0, 0), 1);
							line(drawing, ptEnd, ptFar, Scalar(255, 0, 0), 1);
							circle(drawing, ptStart, 3, Scalar(255, 0, 100), 2);
							circle(drawing, ptEnd, 3, Scalar(255, 0, 100), 2);
							circle(drawing, ptFar, 3, Scalar(100, 0, 255), 2);//凹点
						}
						d++;
					}
					imshow("result", drawing);
				}

				count = count + 1;
				time0 = ((double)getTickCount() - time0) / getTickFrequency();
				cout << "time:   --------------------------->    " << time0 << "s"<< endl;
				cout << "手指数: " << count << endl;
				cout<< endl<<endl;
				Mat hand(filterContours.at(0));
			
				Scalar templateColor(255, 0, 255);
				
				stringstream ss;
				if (count == 1)  //1,6,7,8,9
				{
					int value = hand_template_match(hand, g_TemplateContours,41);
					drawContours(dst, g_TemplateContours, value, templateColor, 1);
					if (value + 1>=1 && value + 1<=6)
						
					{
						cout << "Match result--------------------------->    " << 1<<endl;
						ss << "Match result " << 1;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

					}
					else if (value + 1>=7 && value + 1<=16)
					{
						cout << "Match result--------------------------->    " << 6 << endl;
						ss << "Match result " << 6;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1 >=17&& value + 1 <=21)
					{
						cout << "Match result--------------------------->    " << 7 << endl;
						ss << "Match result " << 7;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1 >=22&& value + 1 <= 31)
					{
						cout << "Match result--------------------------->    " << 8 << endl;
						ss << "Match result " << 8;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1 >=32&& value + 1 <=36)
					{
						cout << "Match result--------------------------->    " << 9 << endl;
						ss << "Match result " << 9;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1 >=37 && value + 1 <= 42)
					{
						cout << "Match result--------------------------->    " << 10 << endl;
						ss << "Match result " << 10;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}

				}

				else if (count == 2)  //2,8,9,6
				{
					int value = hand_template_match(hand, g_TemplateContours_2, 36);
					drawContours(dst, g_TemplateContours_2, value, templateColor, 1);
					if (value + 1 >=1&&value + 1 <=10)

					{
						cout << "Match result--------------------------->    " << 2 << endl;
						ss << "Match result " << 2;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);

					}
					else if (value + 1>=11 && value + 1<=20)
					{
						cout << "Match result--------------------------->    " << 6 << endl;
						ss << "Match result " << 6;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1>=21&& value + 1<=30)
					{
						cout << "Match result--------------------------->    " << 8 << endl;
						ss << "Match result " << 8;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}
					else if (value + 1 >= 31 && value + 1 <= 35)
					{
						cout << "Match result--------------------------->    " << 9 << endl;
						ss << "Match result " << 9;
						string text = ss.str();
						putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
					}

				}

				
				else if (count == 3)
				{
					cout << "Match result--------------------------->    " << 3 << endl;
					ss << "Match result " << 3;
					string text = ss.str();
					putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
				}
				else if (count == 4)
				{
					cout << "Match result--------------------------->    " << 4 << endl;
					ss << "Match result " << 4;
					string text = ss.str();
					putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
				}
				else if (count == 5)
				{
					cout << "Match result--------------------------->    " << 5 << endl;
					ss << "Match result " << 5;
					string text = ss.str();
					putText(dst, text, Point(0, 100), FONT_HERSHEY_SIMPLEX, 1.0, templateColor);
				}

			}
			
			count = 0;
			cvWaitKey(100);
		}
		
	return 0;
	//cvWaitKey(0);
}

