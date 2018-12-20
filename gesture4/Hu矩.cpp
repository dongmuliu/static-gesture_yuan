#include"palm.h"
void  Calculate_Moment(Mat src_gray)  //输入灰度图像
{
	int i = 0, j = 0;
	int Img_H = src_gray.rows;
	int Img_W = src_gray.cols;
	uchar temp = 0;
	double m_00 = 0, m_01 = 0, m_10 = 0;
	double X_c = 0, Y_c = 0, Center_x = 0, Center_y = 0;
	double H_00 = 0, H_02 = 0, H_03 = 0, H_11 = 0, H_12 = 0, H_20 = 0, H_21 = 0, H_30 = 0;
	double eta_02 = 0, eta_03 = 0, eta_11 = 0, eta_12 = 0, eta_20 = 0, eta_21 = 0, eta_30 = 0;
	double hu[7];
	//求几何阶矩
	for (i = 0; i<Img_H; i++)
	{
		for (j = 0; j<Img_W; j++)
		{
			temp = src_gray.at<uchar>(i, j);
			//cout << temp << endl;
			m_00 = m_00 + (powf(i, 0)*powf(j, 0)*temp);
			m_01 = m_01 + (powf(i, 0)*powf(j, 1)*temp);
			m_10 = m_10 + (powf(i, 1)*powf(j, 0)*temp);
		}
	}
	X_c = m_10 / m_00;
	Y_c = m_01 / m_00;
	//中心矩
	for (i = 0; i<Img_H; i++)
	{
		for (j = 0; j<Img_W; j++)
		{
			temp = src_gray.at<uchar>(i, j);
			Center_x = i - X_c;
			Center_y = j - Y_c;
			H_00 = H_00 + pow(Center_x, 0)*pow(Center_y, 0)*temp;
			H_02 = H_02 + pow(Center_x, 0)*pow(Center_y, 2)*temp;
			H_03 = H_03 + pow(Center_x, 0)*pow(Center_y, 3)*temp;
			H_11 = H_11 + pow(Center_x, 1)*pow(Center_y, 1)*temp;
			H_12 = H_12 + pow(Center_x, 1)*pow(Center_y, 2)*temp;
			H_20 = H_20 + pow(Center_x, 2)*pow(Center_y, 0)*temp;
			H_21 = H_21 + pow(Center_x, 2)*pow(Center_y, 1)*temp;
			H_30 = H_30 + pow(Center_x, 3)*pow(Center_y, 0)*temp;
		}
	}
	//矩的归一化
	eta_02 = H_02 / pow(H_00, 2);
	eta_03 = H_03 / pow(H_00, 2.5);
	eta_11 = H_11 / pow(H_00, 2);
	eta_12 = H_12 / pow(H_00, 2.5);
	eta_20 = H_20 / pow(H_00, 2);
	eta_21 = H_21 / pow(H_00, 2.5);
	eta_30 = H_30 / pow(H_00, 2.5);
	//矩不变特征
	hu[0] = eta_20 + eta_02;
	hu[1] = pow(eta_20 - eta_02, 2) + 4 * pow(eta_11, 2);
	hu[2] = pow(eta_30 - 3 * eta_12, 2) + pow(3 * eta_21 - eta_03, 2);
	hu[3] = pow(eta_30 + eta_12, 2) + pow(eta_21 + eta_03, 2);
	hu[4] = (eta_30 - 3 * eta_12)*(eta_03 + eta_21)*(pow(eta_30 + eta_12, 2) - 3 * pow(eta_21 + eta_03, 2)) +
		(3 * eta_12 - eta_03)*(eta_21 + eta_30)*(3 * pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2));
	hu[5] = (eta_20 - eta_02)*(pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2)) +
		4 * eta_11*(eta_30 + eta_12)*(eta_03 + eta_21);
	hu[6] = (3 * eta_21 - eta_03)*(eta_30 + eta_12)*(pow(eta_30 + eta_12, 2) - 3 * pow(eta_21 + eta_03, 2)) +
		(eta_30 - 3 * eta_12)*(eta_21 + eta_30)*(3 * pow(eta_30 + eta_12, 2) - pow(eta_21 + eta_03, 2));
	for (int i = 0; i<7; i++)
	{
		cout << "Hu矩" << abs(hu[i]) << endl;
	}


}

double Similar(double *hu1, double *hu2)

{	
	double sim = 0; //相似度
	double dSigmaST = 0;
	double dSigmaS = 0;
	double dSigmaT = 0;
	double temp = 0;
	{for (int i = 0; i<7; i++)
	{
		temp = fabs(hu1[i] *hu2[i]);
		dSigmaST += temp;
		dSigmaS += pow(hu1[i], 2);
		dSigmaT += pow(hu2[i], 2);
	}}
	sim = dSigmaST / (sqrt(dSigmaS)*sqrt(dSigmaT));
	return sim;

}