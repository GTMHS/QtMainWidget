#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEFAULT_SHOW_RATE (30)
#define TIMESTAMPFREQUENCY 125000000	//大华相机的时间戳频率固定为125,000,000Hz
bool revFlag = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
	, m_thdDisplayThread(CThreadLite::ThreadProc(&MainWindow::DisplayThreadProc, this), "Display")
	, m_nDisplayInterval(0)
	, m_nFirstFrameTime(0)
	, m_nLastFrameTime(0)
	, m_handler(NULL)
{
    ui->setupUi(this);
	
	m_hWnd = (VR_HWND)this->winId();
	//pui = new CammerWidget();
	//connect(pui, SIGNAL(SendUpdateLCDMsg(int)), this, SLOT(UpdateLCD()));

	// 默认显示30帧
	setDisplayFPS(30);

	m_elapsedTimer.start();

	// 启动显示线程
	if (!m_thdDisplayThread.isThreadOver())
	{
		m_thdDisplayThread.destroyThread();
	}

	if (!m_thdDisplayThread.createThread())
	{
		//MessageBoxA(NULL, "Create Display Thread Failed.", "", 0);
		//return FALSE;
	}
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);

	ui->splitter->setStretchFactor(0, 8);
	ui->splitter->setStretchFactor(1, 1);	
	ui->splitter->setStyleSheet("QSplitter::handle { background-color: gray }");
	this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
	int label_width = this->width() * 8 / 9;
	int label_heiht = this->height() / 5;
	ui->label->setMaximumWidth(label_width);
	ui->label->setMaximumHeight(label_heiht * 3);
	ui->label_2->setMaximumWidth(label_width);
	ui->label_2->setMaximumHeight(label_heiht);
	init_parameters();

}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::ShowImage(uint8_t* pRgbFrameBuf, int pRgbFrameBufSize, int nWidth, int nHeight, uint64_t pixelFormat)
{
	QImage image;
	if (NULL == pRgbFrameBuf ||
		nWidth == 0 ||
		nHeight == 0)
	{
		printf("%s image is invalid.\n", __FUNCTION__);
		return false;
	}
	if (Dahua::GenICam::gvspPixelMono8 == pixelFormat)
	{
		image = QImage(pRgbFrameBuf, nWidth, nHeight, QImage::Format_Indexed8);
	}
	else
	{
		image = QImage(pRgbFrameBuf, nWidth, nHeight, QImage::Format_RGB888);
	}
	
	//显示整幅图片
	QPixmap pixmap = QPixmap::fromImage(image);
	int w = ui->label->width();
	int h = ui->label->height();	
	ui->label->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
	//label2显示裁剪之后的照片
	Mat img = QImage2cvMat(image);
	img = img(rect_of_image); //裁剪
	cv::Mat out;
	cv::Mat in[] = { img, img, img };
	cv::merge(in, 3, out);
	ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
	//显示裁剪后并且识别后的图片
	if (true == Mode_of_trig) {
		try
		{
			//QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
			if (!bookdetection(out))//识别判断
			{
				Beep(1000, 1000);
				cout << "不合格" << endl << endl;
				//弹窗报警,2秒自动关闭
				alertWindow = new AlertWindow;
				alertWindow->show();
				waitKey(2000);
				alertWindow->close();
				//QMessageBox::information(NULL, "错误", "");

				//output file
				//imwrite(wrong_filename, src_mat);
				//run_database(current_time, "不合格");
				unsigned char uc[] = { 0x7e,0x01,0x55,0x55,0x0d,0x0d };
				int count = 0;
				while (revFlag != true) {
					revFlag = mycserialport.WriteData(uc, 6);
					Sleep(50);
					count++;
					if (count >= 3) {
						//cout << "未收到下位机确认信息!" << endl;
						//连续发三次，三次握手,返回动作执行成功
						count = 0;
						break;
					}
				}
				revFlag = false;
			}
			else {
				//ui->textEdit->setText("Correct!");
			}
		}
		catch (const std::exception& e)
		{
			QMessageBox::information(NULL, "识别部分出错", e.what());
			return true;
		}
	}
	else
	{
		if (true == Mode_of_trig_soft) {
			try
			{				
				Mat img = QImage2cvMat(image);
				Mode_of_trig_soft = false;
				cv::Mat out;
				cv::Mat in[] = { img, img, img };
				cv::merge(in, 3, out);
				//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
				//QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
				imwrite("Train/image/Pic.bmp", img);
				QMessageBox::information(NULL, "保存图片", "保存图片成功！");
			}
			catch (const std::exception& e)
			{
				QMessageBox::information(NULL, "识别部分出错", e.what());
				return true;
			}
			Mode_of_trig_soft = false;
		}
	}
	return true;
}

bool sortFun(Rect p1, Rect p2);

void MainWindow::testRun() {
	clock_t startTime, startTime1, endTime;
	startTime = clock();
	stringstream ss;
	//string imagefile = "D:\\Pic\\";
	string imagefile = "E:\\pic\\";
	try
	{
		string outfile;
		Mat image_for_write;
		for (int i = 1; i <150; i++) {
			startTime1 = clock();
			ss << imagefile <<"Pic (" << i << ").bmp";
			string infile = ss.str();
			QString infile2 = QString::fromStdString(infile);
			ui->label_3->setText(infile2);
			QString filename(infile2);
			QImage* img = new QImage;
			if (!(img->load(infile2))) //加载图像
			{
				QMessageBox::information(this,tr("OPEN FAILED"),tr("OPEN FAILED!"));
				delete img;
				return;
			}
			ui->label->setPixmap(QPixmap::fromImage(*img));
			cout << infile << endl;
			image_for_write = imread(infile);			
			image_for_write = image_for_write(rect_of_image);
			QImage Img;
			cv::Mat Rgb;
			if (image_for_write.channels() == 3)//RGB Img
			{
				cv::cvtColor(image_for_write, Rgb, CV_BGR2RGB);//颜色空间转换
				Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
			}
			else//Gray Img
			{
				Img = QImage((const uchar*)(image_for_write.data), image_for_write.cols, image_for_write.rows, image_for_write.cols*image_for_write.channels(), QImage::Format_Indexed8);
			}
			ui->label_2->setPixmap(QPixmap::fromImage(Img));
			waitKey(100);
			ss.str("");
			ui->lcdNumber_2->display(i);
			if (!bookdetection(image_for_write))//识别判断
			{
				Beep(1000, 1000);
				cout << "不合格" << endl << endl;
				ss << imagefile << "wrong-" << i << ".bmp";
				imwrite(ss.str(), image_for_write);
				//弹窗报警
				alertWindow = new AlertWindow;
				alertWindow->show();
				waitKey(1000);
				alertWindow->close();
			}
			endTime = clock();
			//ui->label_3->setText("");
			string s = "The run time is: " + to_string((double)(endTime - startTime1) / CLOCKS_PER_SEC) + "s";
			QString st = QString::fromStdString(s);
			//ui->label_3->setText(st);
			cout << "The run time is: " << (double)(endTime - startTime1) / CLOCKS_PER_SEC << "s" << endl << endl;
			ss.str("");
			waitKey(1000);
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		throw;
	}
	endTime = clock();
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl << endl;
}

bool MainWindow::bookdetection(Mat imagefile) {
	string outfile = "E:\\pic\\label\\" + get_datetime() + ".bmp";
	String modelConfiguration = "D:/yolov3.cfg";
	String model_label_Weights = "D:/yolov3-voc_9000.weights";
	//识别黑色标志，返回黑色方格的矩形信息保存在boxes中
	vector<Rect> boxes = detect_image(imagefile, model_label_Weights, modelConfiguration);
	//排序，根据得到的方框的中点的纵坐标进行排序，按照y从小到大的顺序排
	sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;
	int max_point_x = 0, max_point_y = 0;
	double average_piexl_value = 0;

	//for循环获取每个黑块的中点，并存储到points中
	for (int i = 0; i < boxes.size(); i++) {
		points.push_back(Point(boxes[i].x + 0.5*boxes[i].width, boxes[i].y + 0.5*boxes[i].height));
		//average_piexl_value = average_piexl_value + imagefile.at<uchar>(points[i]);
		//if (boxes[i].x > max_point_x) {
		//	max_point_x = boxes[i].x;
		//	max_point_y = boxes[i].y;
		//}
		//if (i > 0)
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << " 两点间距：" << points[i - 1].x - points[i].x << ", " << points[i].y - points[i - 1].y << endl;
		//else
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << endl;
		//cout << "Mid-points " << i << "," << points[i].x << "," << points[i].y << endl;
		//cout << i << ": boxes[i].x " << boxes[i].x << " boxes[i].y " << boxes[i].y << " boxes[i].width " << boxes[i].width << " boxes[i].height " << boxes[i].height << endl;
	}
	//if (boxes.size() < block_nums) {
	//	int this_boxes_size = boxes.size();
	//	//计算各中点的相对位置
	//	for (int i = 0; i <= points.size(); i++) {
	//		//如果当前框的中点与下一个框的中点的相对位置的y相差＜5，则认为此框的位置正确
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//否则，有误，是否是缺失？
	//		else
	//		{
	//			//;如果是缺失，则补一个框。
	//			//找到下一个框的左上角位置,x是从大到小，y是从小到大
	//			int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width;
	//			int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height;
	//			Rect r = Rect(x_1, y_1, locations[i + 1].width, locations[i + 1].height);
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			Mat imagefile1 = imagefile(r);
	//			double value = 0;
	//			for (int i = 0; i < imagefile1.rows; ++i)
	//				for (int j = 0; j < imagefile1.cols; ++j)
	//					value += imagefile1.at<uchar>(i, j);
	//			value = value / (imagefile1.cols*imagefile1.rows);
	//			if (value < average_piexl_value)
	//			{
	//				this_boxes_size++;
	//				rectangle(imagefile, r, Scalar(255, 178, 50), 2);
	//				cout << "缺失框位置像素正确" << endl;
	//				imwrite(outfile, imagefile);
	//				cout << "文件写入：" + outfile << endl << endl;
	//				if (this_boxes_size < points.size()) 
	//					continue;
	//				else
	//					return 1;
	//			}
	//			else
	//				//cout << "缺失框位置像素有误!" << endl;
	//				continue;
	//				//return false;				
	//		}
	//	}
	//	return 1;
	//}
	//前十个用来自适应调整截距斜率跟相关系数参数
	//if (total_number < 11) {
	//	//v_block_locations.push_back((boxes));
	//}
	//if (total_number == 10) {
	//	double sum_k = 0, sum_b = 0, sum_r_q = 0;
	//	for (int i = 0; i < 11; i++) {
	//		sum_b += v_b[i];
	//		sum_k += v_k[i];
	//		sum_r_q += v_r_q[i];
	//	}
	//	cout << "sum_b: " << sum_b << " sum_k：" << sum_k << " sum_r_q: " << sum_r_q << endl;
	//	k = sum_k / 10;
	//	b = sum_b / 10;
	//	r_q = sum_r_q / 10;
	//	cout << "k: " << k << " b: " << b << " r_q: " << r_q;
	//}
	//前十个用来自适应调整截距斜率跟相关系数参数
	//if (boxes.size() == block_nums && total_number < 11) {
	//	LinearFitting(points, 0, 0, 0);
	//}

	//cout << "本书黑色标记数量： " << boxes.size() << endl;
	int this_block_nums = boxes.size();
	int val = 0;
	//if (this_block_nums > block_nums) {
	//	for (int i = 0; i < boxes.size(); i++) {
	//		//if (boxes[i].width > 100) {
	//		//	boxes.erase(boxes.begin() + i);
	//		//}
	//	}
	//	//if (boxes.size() == block_nums) {
	//	//	return 1;
	//	//}
	//}	
	//if (boxes.size() < block_nums) {
	//	//计算各中点的相对位置
	//	for (int i = 0; i <= points.size(); i++) {
	//		//如果当前框的中点与下一个框的中点的相对位置的y相差＜5，则认为此框的位置正确
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//否则，有误，是否是缺失？
	//		else
	//		{
	//			//;如果是缺失，则补一个框。
	//			////找到下一个框的左上角位置,x是从大到小，y是从小到大
	//			//int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width;
	//			//int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height;
	//			//按照y从小到大的顺序排序
	//			int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width + 0.5*locations[i].width;
	//			int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height + 0.5*locations[i].height;
	//			Rect r = Rect(x_1, y_1, locations[i + 1].width, locations[i + 1].height);
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			Mat imagefile1 = imagefile(r);
	//			double value = 0;
	//			//循环遍历补得黑框中的所有像素计算平局灰度值
	//			for (int i = 0; i < imagefile1.rows; ++i) for (int j = 0; j < imagefile1.cols; ++j) value += imagefile1.at<uchar>(i, j);
	//			value = value / (imagefile1.cols*imagefile1.rows);
	//			if (value < average_piexl_value)
	//			{  
	//				rectangle(imagefile, r, Scalar(255, 108, 50), 2);
	//				cout << "缺失框位置像素正确" << endl;
	//				boxes.push_back(r);
	//				continue;
	//			}
	//			else
	//			{
	//				cout << "缺失框位置像素有误!" << endl;
	//				continue;
	//			}
	//		}
	//	}			
	//}
	//ui->textEdit->append("this_block_nums is " + boxes.size());

	if (Num_of_blocks == this_block_nums) {
		ui->label_3->setText("Correct");
		LinearFitting(points, k, b, s);
		return true;
	}
	else {
		ui->label_3->setText("Wrong");
		return false;
	}

	//switch (this_block_nums)
	//{
	//case 11:
	//	cout << "黑色标志点数量正确" << endl;
	//	ui->label_3->setText("Correct");
	//	//imwrite(outfile, imagefile);
	//	//cout << "文件写入：" + outfile << endl;
	//	run_database(get_datetime(), "正常");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points,k, b, s);
	//case 12:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "黑色标志点数为12" << endl;
	//	outfile = "E:\\pic\\label\\12-" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "文件写入：" + outfile << endl;
	//	ui->label_3->setText("12");
	//	run_database(get_datetime(), "正常");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points, k, b, s);
	//case 10:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "黑色标志点数为10" << endl;
	//	ui->label_3->setText("10");
	//	outfile = "E:\\pic\\label\\少" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "文件写入：" + outfile << endl;
	//	run_database(get_datetime(), "异常");
	//	//emit SendUpdateLCDMsg(2);
	//	return LinearFitting(points, k,b,s) && 0;
	//default:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	cout << "黑色标志数量错误：  " << this_block_nums << endl;
	//	ui->label_3->setText("Wrong");
	//	outfile = "E:\\pic\\label\\有误" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "文件写入：" + outfile << endl;
	//	run_database(get_datetime(), "异常");
	//	//emit SendUpdateLCDMsg(2);
	//	return 0;
	//}
	
}

//OpenCV Mat与QImage相互转换函数
Mat MainWindow::QImage2cvMat(QImage &image, bool clone, bool rb_swap)
{
	cv::Mat mat;
	//qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void *)image.constBits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void *)image.constBits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		if (rb_swap) cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
	case QImage::Format_Grayscale8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void *)image.bits(), image.bytesPerLine());
		if (clone)  mat = mat.clone();
		break;
	}
	return mat;
}

QImage MainWindow::cvMat2QImage(const cv::Mat& mat)
{
	// 8-bits unsigned, NO. OF CHANNELS = 1
	if (mat.type() == CV_8UC1)
	{
		QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat
		uchar *pSrc = mat.data;
		for (int row = 0; row < mat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, mat.cols);
			pSrc += mat.step;
		}
		return image;
	}
	// 8-bits unsigned, NO. OF CHANNELS = 3
	else if (mat.type() == CV_8UC3)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	else if (mat.type() == CV_8UC4)
	{
		// Copy input Mat
		const uchar *pSrc = (const uchar*)mat.data;
		// Create QImage with same dimensions as input Mat
		QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
		return image.copy();
	}
	else
	{
		return QImage();
	}
}

bool MainWindow::LinearFitting(const vector<Point> points, double slope, double intercept, double r_square)
{
	int length = points.size();
	double xmean = 0.0;
	double ymean = 0.0;
	for (int i = 0; i < length; i++)
	{
		xmean += points[i].x;
		ymean += points[i].y;
	}
	xmean /= length;
	ymean /= length;

	double sumx2 = 0.0;
	double sumy2 = 0.0;
	double sumxy = 0.0;
	for (int i = 0; i < length; i++)
	{
		sumx2 += (points[i].x - xmean) * (points[i].x - xmean);
		sumy2 += (points[i].y - ymean) * (points[i].y - ymean);
		sumxy += (points[i].y - ymean) * (points[i].x - xmean);
	}
	double slope1 = sumxy / sumx2;  //斜率
	double intercept1 = ymean - slope * xmean; //截距
	double r_square1 = sumxy * sumxy / (sumx2 * sumy2); //相关系数
	cout << "y = " << slope1 << "x+ " << intercept1 << "  r_square1 is " << r_square1 << endl;

	//判断条件只添加了相关系数和斜率
	if (abs(r_square - r_square1) <= 0.1 && abs(slope - slope1) <= 0.1) {
		cout << "相关系数正确" << endl;

		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + "Correct";
		ui->label_3->setText(str);
		return true;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_RED);
		cout << "相关系数错误！" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + "Wrong";
		ui->label_3->setText(str);
		return false;
	}

}

string MainWindow::get_datetime()
{
	SYSTEMTIME st = { 0 };
	GetLocalTime(&st);

	char temp[100];
	sprintf_s(temp, "%d-%02d-%02d-%02d-%02d-%02d",
		st.wYear,
		st.wMonth,
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond
	);
	std::string params = string(temp);
	return params;
}

vector<Rect> MainWindow::detect_image(Mat frame, string modelWeights, string modelConfiguration) {
	// Load the network
	Net net = readNetFromDarknet(modelConfiguration, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	//net.setPreferableTarget(DNN_TARGET_OPENCL);
	net.setPreferableTarget(DNN_TARGET_CPU);

	Mat blob;
	blobFromImage(frame, blob, 1 / 255.0, cvSize(inpWidth, inpHeight), Scalar(0, 0, 0), true, false);

	//Sets the input to the network
	net.setInput(blob);

	// Runs the forward pass to get output of the output layers
	vector<Mat> outs;
	net.forward(outs, getOutputsNames(net));

	// Remove the bounding boxes with low confidence
	vector<Rect> boxes = postprocess_return(frame, outs);
	// Put efficiency information. The function getPerfProfile returns the overall time for inference(t) and the timings for each of the layers(in layersTimes)
	vector<double> layersTimes;
	double freq = getTickFrequency() / 1000;
	double t = net.getPerfProfile(layersTimes) / freq;
	string label = format("Inference time for a frame : %.2f ms", t);
	//cout << label<<endl;
	//putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255));
	// Write the frame with the detection boxes
	//imshow(kWinName, frame);
	//imwrite(classesFile, frame);
	return boxes;
	//cv::waitKey(30);
}

bool sortFun(Rect p1, Rect p2) {
	return p1.y + 0.5*p1.height < p2.y + 0.5* p2.height;
}

vector<Rect> MainWindow::postprocess_return(Mat& frame, const vector<Mat>& outs)
{
	vector<int> classIds;
	vector<float> confidences;
	vector<Rect> boxes;
#pragma omp parallel for
	for (int i = 0; i < outs.size(); ++i)
	{
		// Scan through all the bounding boxes output from the network and keep only the
		// ones with high confidence scores. Assign the box's class label as the class
		// with the highest score for the box.
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
		{
			Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			Point classIdPoint;
			double confidence;
			// Get the value and location of the maximum score
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold)
			{
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;

				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}

	// Perform non maximum suppression to eliminate redundant overlapping boxes with
	// lower confidences
	vector<int> indices;
	NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	//cout << "indices.size(匹配得到的目标数):" << indices.size() << endl;
	vector<Rect> boxes_for_return;

	//Rect box = boxes[0];

	for (size_t i = 0; i < indices.size(); ++i)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		if (box.x < 0)
		{
			box.width = box.width + box.x;
			box.x = 0;
		}
		if (box.width > frame.cols) {
			box.width = frame.cols - box.x;
		}
		if (box.x + box.width > frame.cols) {
			box.width = frame.cols - box.x;
		}
		boxes_for_return.push_back(box);
		//drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);
		//画框
		rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 2);
	}
	ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(frame)));
	return boxes_for_return;
}

vector<String> MainWindow::getOutputsNames(const Net& net)
{
	static vector<String> names;
	if (names.empty())
	{
		//Get the indices of the output layers, i.e. the layers with unconnected outputs
		vector<int> outLayers = net.getUnconnectedOutLayers();

		//get the names of all the layers in the network
		vector<String> layersNames = net.getLayerNames();

		// Get the names of the output layers in names
		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); ++i)
			names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}

wstring MainWindow::string2tchar(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void MainWindow::run_exe(string path, string params)
{
	std::wstring t_path = string2tchar(path);
	std::wstring t_params = string2tchar(params);
	SHELLEXECUTEINFO ShellInfo;
	memset(&ShellInfo, 0, sizeof(ShellInfo));
	ShellInfo.cbSize = sizeof(ShellInfo);
	ShellInfo.hwnd = NULL;
	ShellInfo.lpVerb = _T("open");
	ShellInfo.lpFile = t_path.c_str();
	ShellInfo.lpParameters = t_params.c_str();
	ShellInfo.nShow = SW_SHOWNORMAL;
	ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	BOOL bResult = ShellExecuteEx(&ShellInfo);
}

void MainWindow::run_database(string time, string result)
{
	// database.exe 2016-01-22 08:45:50 异常
	string filename = "database.exe";
	//string params = "2016-01-22 08:45:50 异常"
	std::stringstream ss;
	ss << " \"" << time << "\" \"" << result << "\"";
	std::string params = ss.str();
	run_exe(filename, params);
}

void MainWindow::run_labelImg()
{
	SHELLEXECUTEINFO  ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("labelImg\\labelImg.exe");        // 执行的程序名 
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;                // 全屏显示这个程序 
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}

void MainWindow::run_cut()
{
	// database.exe 2016-01-22 08:45:50 异常
	//string filename = "labelImg\\labelImg.exe";
	//string params = "2016-01-22 08:45:50 异常"
	//std::stringstream ss;
	//ss << " \"" << time << "\" \"" << result << "\"";
	//std::string params = ss.str();
	//run_exe(filename,"");
	Mat mask = read_mask();
	Mat img = imread("Pic.bmp");
	Rect reck_mask = mask_boundingRect(mask);	
	img = img(reck_mask);
	imwrite("Pic.bmp",img);
	QImage qimg = cvMat2QImage(img);

	//参数1-显示图像的Label，参数2-要显示的Mat
	Mat Rgb;
	QImage Img;
	if (img.channels() == 3)//RGB Img
	{
		cv::cvtColor(img, Rgb, CV_BGR2RGB);//颜色空间转换
		Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
	}
	else//Gray Img
	{
		Img = QImage((const uchar*)(img.data), img.cols, img.rows, img.cols*img.channels(), QImage::Format_Indexed8);
	}
	ui->label->setPixmap(QPixmap::fromImage(Img));
	
	//ui->label->setText(QString::number(reck_mask.x));
	//ui->label_2->setText(QString::number(reck_mask.y));
	//ui->label_3->setText(QString::number(reck_mask.height));
}

Mat MainWindow::read_mask()
{
	String maskfile = "moban.png";	
	Mat mask = imread(maskfile);
	int channels = mask.channels();
	if (mask.empty())
	{
		throw;
	}
	Mat mask2;

	const int height = mask.rows;
	const int width = mask.cols;

	if (channels == 3)
	{
		mask2 = Mat::zeros(mask.rows, mask.cols, CV_8UC3);
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if (mask.ptr(j)[i * 3 + 0] > 0) {
					mask2.at<Vec3b>(j, i) = WHITE_COLOR;
				}
			}
		}
	}
	else if (channels == 1)
	{
		mask2 = Mat::zeros(mask.rows, mask.cols, CV_8U);

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if (mask.ptr(j)[i * 3 + 0] > 0) {
					mask2.ptr(j)[i] = 255;
				}
			}
		}
	}
	return mask2;
}

void MainWindow::run_train() {
	string params = "darknet.exe detector train cfg/voc.data cfg/yolov3-voc.cfg darknet53.conv.7";
	string filename = "darknet.exe";
	run_exe(filename, params);
}

Rect MainWindow::mask_boundingRect(Mat mask)
{
	int j, k;
	int start_w, end_w;
	int start_h, end_h;

	for (j = 0; j < mask.rows; j++) {
		// top, start_h
		bool flag = false;
		for (k = 0; k < mask.cols; k++) {
			if (mask.ptr(j)[k] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	start_h = j;

	for (j = mask.rows - 1; j > start_h; j--) {
		// bottom, end_h
		bool flag = false;
		for (k = 0; k < mask.cols; k++) {
			if (mask.ptr(j)[k] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	end_h = j;

	for (j = 0; j < mask.cols; j++) {
		// left, start_w
		bool flag = false;
		for (k = 0; k < mask.rows; k++) {
			if (mask.ptr(k)[j] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) break;
	}
	start_w = j;

	for (j = mask.cols - 1; j > start_w; j--) {
		// left, start_w
		bool flag = false;
		for (k = 0; k < mask.rows; k++) {
			if (mask.ptr(k)[j] > 0) {
				flag = true;
				break;
			}
		}
		if (flag) break;
	}

	end_w = j;
	Rect rect;
	rect.x = start_w;
	rect.y = start_h;
	rect.width = end_w - start_w + 1;
	rect.height = end_h - start_h + 1;

	return rect;
}

void MainWindow::CameraCheck(void)
{
	CSystem &systemObj = CSystem::getInstance();

	bool bRet = systemObj.discovery(m_vCameraPtrList);
	if (false == bRet)
	{
		printf("discovery fail.\n");
		return;
	}

	// 打印相机基本信息（key, 制造商信息, 型号, 序列号）
	for (int i = 0; i < m_vCameraPtrList.size(); i++)
	{
		ICameraPtr cameraSptr = m_vCameraPtrList[i];

		printf("Camera[%d] Info :\n", i);
		printf("    key           = [%s]\n", cameraSptr->getKey());
		printf("    vendor name   = [%s]\n", cameraSptr->getVendorName());
		printf("    model         = [%s]\n", cameraSptr->getModelName());
		printf("    serial number = [%s]\n", cameraSptr->getSerialNumber());
	}

	if (m_vCameraPtrList.size() < 1)
	{
		printf("no camera.\n");
	}
	else
	{
		//默认设置列表中的第一个相机为当前相机，其他操作比如打开、关闭、修改曝光都是针对这个相机。
		m_pCamera = m_vCameraPtrList[0];
	}
}

//切换采集方式、触发方式 （连续采集、外部触发、软件触发）
void MainWindow::CameraChangeTrig(ETrigType trigType)
{
	if (NULL == m_pCamera)
	{
		printf("Change Trig fail. No camera or camera is not connected.\n");
		return;
	}

	if (trigContinous == trigType)
	{
		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("Off"))
		{
			printf("set TriggerMode value = Off fail.\n");
			return;
		}
		Mode_of_trig = false;
	}
	else if (trigSoftware == trigType)
	{
		//设置触发源为软触发
		CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
		if (false == nodeTriggerSource.isValid())
		{
			printf("get TriggerSource node fail.\n");
			return;
		}
		if (false == nodeTriggerSource.setValueBySymbol("Software"))
		{
			printf("set TriggerSource value = Software fail.\n");
			return;
		}

		//设置触发器
		CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
		if (false == nodeTriggerSelector.isValid())
		{
			printf("get TriggerSelector node fail.\n");
			return;
		}
		if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
		{
			printf("set TriggerSelector value = FrameStart fail.\n");
			return;
		}

		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("On"))
		{
			printf("set TriggerMode value = On fail.\n");
			return;
		}
		Mode_of_trig = false;
	}
	else if (trigLine == trigType)
	{
		//设置触发源为Line1触发
		CEnumNode nodeTriggerSource(m_pCamera, "TriggerSource");
		if (false == nodeTriggerSource.isValid())
		{
			printf("get TriggerSource node fail.\n");
			return;
		}
		if (false == nodeTriggerSource.setValueBySymbol("Line1"))
		{
			printf("set TriggerSource value = Line1 fail.\n");
			return;
		}

		//设置触发器
		CEnumNode nodeTriggerSelector(m_pCamera, "TriggerSelector");
		if (false == nodeTriggerSelector.isValid())
		{
			printf("get TriggerSelector node fail.\n");
			return;
		}
		if (false == nodeTriggerSelector.setValueBySymbol("FrameStart"))
		{
			printf("set TriggerSelector value = FrameStart fail.\n");
			return;
		}

		//设置触发模式
		CEnumNode nodeTriggerMode(m_pCamera, "TriggerMode");
		if (false == nodeTriggerMode.isValid())
		{
			printf("get TriggerMode node fail.\n");
			return;
		}
		if (false == nodeTriggerMode.setValueBySymbol("On"))
		{
			printf("set TriggerMode value = On fail.\n");
			return;
		}

		// 设置外触发为上升沿（下降沿为FallingEdge）
		CEnumNode nodeTriggerActivation(m_pCamera, "TriggerActivation");
		if (false == nodeTriggerActivation.isValid())
		{
			printf("get TriggerActivation node fail.\n");
			return;
		}
		if (false == nodeTriggerActivation.setValueBySymbol("RisingEdge"))
		{
			printf("set TriggerActivation value = RisingEdge fail.\n");
			return;
		}
		Mode_of_trig = true;
	}
}

//打开相机
bool MainWindow::CameraOpen(void)
{
	if (NULL == m_pCamera)
	{
		printf("connect camera fail. No camera.\n");
		return false;
	}

	if (true == m_pCamera->isConnected())
	{
		printf("camera is already connected.\n");
		return false;
	}

	if (false == m_pCamera->connect())
	{
		printf("connect camera fail.\n");
		return false;
	}

	return true;
}

//关闭相机
bool MainWindow::CameraClose(void)
{
	if (NULL == m_pCamera)
	{
		printf("disconnect camera fail. No camera.\n");
		return false;
	}

	if (false == m_pCamera->isConnected())
	{
		printf("camera is already disconnected.\n");
		return false;
	}

	if (false == m_pCamera->disConnect())
	{
		printf("disconnect camera fail.\n");
		return false;
	}

	return true;
}

//开始采集
bool MainWindow::CameraStart()
{
	if (m_pStreamSource != NULL)
		return true;

	m_pStreamSource = CSystem::getInstance().createStreamSource(m_pCamera);
	if (NULL == m_pStreamSource)
	{
		MessageBoxA(NULL, "Get Stream Failed.", "", 0);
		return false;
	}

	bool isSuccess = m_pStreamSource->attachGrabbing(IStreamSource::Proc(&MainWindow::DahuaCallback, this));
	if (!isSuccess)
	{
		return false;
	}

	if (!m_pStreamSource->startGrabbing())
	{
		m_pStreamSource.reset();
		MessageBoxA(NULL, "Start Stream Grabbing Failed.", "", 0);
		return false;
	}

	return true;
}

//停止采集
bool MainWindow::CameraStop()
{
	if (m_pStreamSource == NULL)
		return true;

	bool isSuccess = m_pStreamSource->detachGrabbing(IStreamSource::Proc(&MainWindow::DahuaCallback, this));
	if (!isSuccess)
	{
		return false;
	}

	m_pStreamSource->stopGrabbing();
	m_pStreamSource.reset();

	/* 清空显示队列 */
	//CGuard guard(m_mutexQue);
	m_qDisplayFrameQueue.clear();

	return true;
}

void MainWindow::DahuaCallback(const CFrame& frame)
{
	CFrameInfo frameInfo;
	frameInfo.m_nWidth = frame.getImageWidth();
	frameInfo.m_nHeight = frame.getImageHeight();
	frameInfo.m_nBufferSize = frame.getImageSize();
	frameInfo.m_nPaddingX = frame.getImagePadddingX();
	frameInfo.m_nPaddingY = frame.getImagePadddingY();
	frameInfo.m_PixelType = frame.getImagePixelFormat();
	frameInfo.m_pImageBuf = (BYTE *)malloc(sizeof(BYTE)* frameInfo.m_nBufferSize);
	frameInfo.m_nTimeStamp = frame.getImageTimeStamp();

	/* 内存申请失败，直接返回 */
	if (frameInfo.m_pImageBuf != NULL)
	{
		memcpy(frameInfo.m_pImageBuf, frame.getImage(), frame.getImageSize());

		if (m_qDisplayFrameQueue.size() > 16)
		{
			CFrameInfo frameOld;
			m_qDisplayFrameQueue.get(frameOld);
			free(frameOld.m_pImageBuf);
		}
		m_qDisplayFrameQueue.push_back(frameInfo);
	}
}

void MainWindow::DisplayThreadProc(Dahua::Infra::CThreadLite& lite)
{
	while (lite.looping())
	{
		CFrameInfo frameInfo;

		if (false == m_qDisplayFrameQueue.get(frameInfo, 500))
		{
			continue;
		}

		// 判断是否要显示。超过显示上限（30帧），就不做转码、显示处理
		if (!isTimeToDisplay(frameInfo.m_nTimeStamp))
		{
			/* 释放内存 */
			free(frameInfo.m_pImageBuf);
			continue;
		}

		/* mono8格式可不做转码，直接显示，其他格式需要经过转码才能显示 */
		if (Dahua::GenICam::gvspPixelMono8 == frameInfo.m_PixelType)
		{
			/* 显示 */
			if (false == ShowImage(frameInfo.m_pImageBuf, frameInfo.m_nBufferSize, frameInfo.m_nWidth, frameInfo.m_nHeight, frameInfo.m_PixelType))
			{
				printf("_render.display failed.\n");
			}
			/* 释放内存 */
			free(frameInfo.m_pImageBuf);
		}
		else
		{
			/* 转码 */
			uint8_t *pRGBbuffer = NULL;
			int nRgbBufferSize = 0;
			nRgbBufferSize = frameInfo.m_nWidth * frameInfo.m_nHeight * 3;
			pRGBbuffer = (uint8_t *)malloc(nRgbBufferSize);
			if (pRGBbuffer == NULL)
			{
				/* 释放内存 */
				free(frameInfo.m_pImageBuf);
				printf("RGBbuffer malloc failed.\n");
				continue;
			}

			IMGCNV_SOpenParam openParam;
			openParam.width = frameInfo.m_nWidth;
			openParam.height = frameInfo.m_nHeight;
			openParam.paddingX = frameInfo.m_nPaddingX;
			openParam.paddingY = frameInfo.m_nPaddingY;
			openParam.dataSize = frameInfo.m_nBufferSize;
			openParam.pixelForamt = frameInfo.m_PixelType;

			IMGCNV_EErr status = IMGCNV_ConvertToRGB24(frameInfo.m_pImageBuf, &openParam, pRGBbuffer, &nRgbBufferSize);
			if (IMGCNV_SUCCESS != status)
			{
				/* 释放内存 */
				printf("IMGCNV_ConvertToRGB24 failed.\n");
				free(frameInfo.m_pImageBuf);
				free(pRGBbuffer);
				return;
			}

			/* 释放内存 */
			free(frameInfo.m_pImageBuf);

			/* 显示 */
			if (false == ShowImage(pRGBbuffer, nRgbBufferSize, openParam.width, openParam.height, openParam.pixelForamt))
			{
				printf("_render.display failed.");
			}
			free(pRGBbuffer);
		}
	}
}

bool MainWindow::isTimeToDisplay(uintmax_t nCurTime)
{
	CGuard guard(m_mxTime);

	// 不显示
	if (m_nDisplayInterval <= 0)
	{
		return false;
	}

	// 第一帧必须显示
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = m_elapsedTimer.nsecsElapsed();
		m_nLastFrameTime = m_nFirstFrameTime;

		return true;
	}

	// 当前帧和上一帧的间隔如果大于显示间隔就显示
	uint64_t nCurTimeTmp = m_elapsedTimer.nsecsElapsed();
	double dAcquisitionInterval = nCurTimeTmp * 1000 / TIMESTAMPFREQUENCY;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;
	if (nAcquisitionInterval > m_nDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		return true;
	}


	// 当前帧相对于第一帧的时间间隔
	uint64_t nPre = (m_nLastFrameTime - m_nFirstFrameTime) % m_nDisplayInterval;
	if (nPre + nAcquisitionInterval > m_nDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		return true;
	}

	return false;
}

void MainWindow::setDisplayFPS(int nFPS)
{
	if (nFPS > 0)
	{
		CGuard guard(m_mxTime);

		m_nDisplayInterval = 1000 * 1000 * 1000.0 / nFPS;
	}
	else
	{
		CGuard guard(m_mxTime);
		m_nDisplayInterval = 0;
	}
}

void MainWindow::set_Mode_of_trig_soft() {
	Mode_of_trig_soft = true;
}
//打开相机
void MainWindow::on_pushButton_clicked()
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);

	//不需要摄像头，本地文件测试函数用
	testRun();

	//----------------------------上下互斥-------------------------------------------

	////连接摄像头实时监测用
	////打开相机
	//ICameraPtr cameraSptr;
	////发现设备
	//CSystem &systemObj = CSystem::getInstance();
	//TVector<ICameraPtr> vCameraPtrList;
	//bool bRet = systemObj.discovery(vCameraPtrList);

	//if (!bRet)
	//{
	//	QMessageBox::warning(NULL, "warning", "发现设备失败\n", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//	ui->pushButton->setEnabled(true);
	//	ui->pushButton_2->setEnabled(false);
	//	ui->pushButton_3->setEnabled(false);
	//	ui->pushButton_4->setEnabled(false);
	//	return;
	//}

	//if (0 == vCameraPtrList.size())
	//{
	//	QMessageBox::warning(NULL, "warning", "发现摄像头失败\n");
	//	ui->pushButton->setEnabled(true);
	//	ui->pushButton_2->setEnabled(false);
	//	ui->pushButton_3->setEnabled(false);
	//	ui->pushButton_4->setEnabled(false);
	//	return;
	//}
	//try {
	//	CameraCheck();
	//	bool camera_open = CameraOpen();
	//	CameraStart();
	//	//SetExposeTime(10000);
	//	//SetAdjustPlus(5);
	//	CameraChangeTrig(trigContinous);
	//}
	//catch (Exception e) {
	//	QMessageBox::warning(NULL, "warning in open camera", e.what(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//}
}
//关闭相机
void MainWindow::on_pushButton_2_clicked()
{
	//关闭相机
	CameraStop();
	CameraClose();
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
}
//开始 识别
void MainWindow::on_pushButton_3_clicked()
{
	//开始 识别
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(true);
	//将相机改为硬件触发，每触发一次执行一次判断
	CameraChangeTrig(trigLine);
	
}
//停止识别
void MainWindow::on_pushButton_4_clicked()
{
	//停止识别
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);
	ui->pushButton_4->setEnabled(false);
	//恢复相机为持续拉流
	CameraChangeTrig(trigContinous);
}

//保存新照片
void MainWindow::on_actionSavePic_triggered()
{
	//打开相机
	/*
	设置为软件触发方式，并触发拍照
	弹窗一个按钮？然后点击拍照？
	*/
	CSystem &systemObj = CSystem::getInstance();

	bool bRet = systemObj.discovery(m_vCameraPtrList);
	if (false == bRet || m_vCameraPtrList.size() < 1)
	{
		printf("discovery fail.\n");
		QMessageBox::critical(NULL, "错误", "未连接到相机！");
		return;
	}
	else
	{
		takephoto = new TakePhoto;
		connect(takephoto, SIGNAL(sendTakePhoteToMainWidget()), this, SLOT(set_Mode_of_trig_soft()));
		takephoto->show();
	}
}
//图书标记
void MainWindow::on_actionLabel_triggered()
{
	//SHELLEXECUTEINFO ShExecInfo = { 0 };
	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	//ShExecInfo.hwnd = NULL;
	//ShExecInfo.lpVerb = NULL;
	//ShExecInfo.lpFile = _T("labelImg\\labelImg.exe");
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_SHOW;
	//ShExecInfo.hInstApp = NULL;
	//ShellExecuteEx(&ShExecInfo);
	//WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
}

//新书训练
void MainWindow::on_actionTrain_triggered()
{
	//图书训练

}

//显示裁剪窗口。参考https://www.cnblogs.com/blogpro/p/11343975.html
void MainWindow::on_actionOpenCutWindow_triggered()
{
	/* Qt校验模板文件hash值判断是否更改 https://blog.csdn.net/emdfans/article/details/23871741 
	*/
	QFile theFile("moban.png");
	theFile.open(QIODevice::ReadOnly);
	QByteArray ba = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
	theFile.close();
	Config().Set("Template","Hash", ba.toHex().constData());
	
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("TemplateEditor\\TemplateEditor.exe");
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

	theFile.open(QIODevice::ReadOnly);
	QByteArray bb = QCryptographicHash::hash(theFile.readAll(), QCryptographicHash::Md5);
	theFile.close();
	//判断裁剪窗口关闭前后模板文件是否发生了变化，发生变化说明进行了裁剪，未发生变化则说明没有裁剪不执行任何操作只弹窗提示
	if (ba != bb) {
		ui->label_3->setText("裁剪已完成");
		Mat mask = read_mask();
		Rect rect_mask = mask_boundingRect(mask);
		Mat img = imread("Train/image/Pic.bmp");
		if (rect_mask.x < 0) {
			rect_mask.x = 0;
		}
		if (rect_mask.x + rect_mask.width > img.cols)
		{
			rect_mask.width = img.cols - rect_mask.x;
		}
		if (rect_mask.y + rect_mask.height > img.rows) {
			rect_mask.height = img.rows - rect_mask.y;
		}
		QRectF qrectf = QRectF(rect_mask.x, rect_mask.y, rect_mask.width, rect_mask.height);
		Config().Set("Image Rect", "Book Name", "书名");
		Config().Set("Image Rect", "x", rect_mask.x);
		Config().Set("Image Rect", "y", rect_mask.y);
		Config().Set("Image Rect", "width", rect_mask.width);
		Config().Set("Image Rect", "height", rect_mask.height);

		img = img(rect_mask);
		imshow("裁剪后的图书", img);
		imwrite("Train/image/Pic.bmp", img);
	}
	else
	{
		QMessageBox::warning(NULL, "图书裁剪警告", "图书未执行裁剪操作或操作不当！");
		ui->label_3->setText("裁剪已完成窗口未执行裁剪操作或操作不当！");
		return;
	}
	//SHELLEXECUTEINFO  ShExecInfo;
	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = NULL;
	//ShExecInfo.hwnd = NULL;
	//ShExecInfo.lpVerb = NULL;
	//ShExecInfo.lpFile = _T("TemplateEditor\\TemplateEditor.exe");        // 执行的程序名 
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_MAXIMIZE;                // 全屏显示这个程序 
	//ShExecInfo.hInstApp = NULL;
	//BOOL ret = ShellExecuteEx(&ShExecInfo);
	//ui->label->setText("裁剪已打开");
	//HANDLE hThread = CreateThread(NULL, 0, MyThreadProc1, NULL, 0, NULL);
	//DWORD dwReturn = WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	////DWORD dwReturn = WaitForSingleObject(hThread, INFINITE);
	//switch (dwReturn)
	//{
	//case WAIT_OBJECT_0:
	//	// hProcess所代表的进程在5秒内结束
	//	cout << "MyThreadProc1 >>> WaitForSingleObject signaled\n" << endl;
	//	ui->label->setText("裁剪已完成");
	//	Config().Set("Image Rect", "Image Rect", "1236");
	//	break;
	//case WAIT_TIMEOUT:
	//	// 等待时间超过5秒
	//	break;
	//case WAIT_FAILED:
	//	// 函数调用失败，比如传递了一个无效的句柄
	//	break;
	//}	
}
//图书裁剪
void MainWindow::on_actionCut_2_triggered()
{
	on_actionOpenCutWindow_triggered();
}

//参数设置
void MainWindow::on_actionPara_triggered()
{
	para = new ParametersSetting;
	//connect(this, SIGNAL(sendData(double, double, double, int)), para, SLOT(recevieData(double, double, double)));	
	//emit sendData(k, b, s);
	connect(para, SIGNAL(sendDataToMainWidget(double, double, double)), this, SLOT(recevieDataFromSubWin(double, double, double, int)));
	para->show();
}

//接收参设设置窗口设置的参数
void MainWindow::recevieDataFromSubWin(double rk, double rb, double rs, int rn)
{
	k = rk;
	b = rb;
	s = rs;
	Num_of_blocks = rn;
	stringstream ss;
	ss << 'k' << k << "  b" << b << "  s" << s << " n" << Num_of_blocks;
	string str;
	ss >> str;
	ui->label->setText(QString::fromStdString(str));	
}

//参数初始化
void MainWindow::init_parameters() {

	//QString fileName;
	////fileName = QCoreApplication::applicationDirPath();
	////fileName = "../Config/conf.ini";
	//fileName = "conf.ini";

	//QSettings settings(fileName, QSettings::IniFormat);
	//settings.setIniCodec("UTF-8");
	//settings.setValue("systemCfg/mode", mode);
	//settings.setValue("server/ip", "10.10.64.115");
	//Config().Set("user", "name", "test");
	//QString qstrname = Config().Get("user", "name").toString();
	//Config().Set("systemCfg", "mode", 1);
	//Config().Set("server", "ip", "10.10.64.115");
	//Config().Set("server", "netmask", "255.255.255.0");

	s = Config().Get("Line Fitting", "s").toDouble();
	b = Config().Get("Line Fitting", "b").toDouble();
	k = Config().Get("Line Fitting", "k").toDouble();
	Num_of_blocks = Config().Get("Line Fitting", "n").toInt();

	Mat mask = read_mask();
	Rect reck_mask = mask_boundingRect(mask);
	rect_of_image = reck_mask;	
	//QString x = QString::number(reck_mask.x);
	//QString y = QString::number(reck_mask.y);
	//QString width = QString::number(reck_mask.width);
	//QString height = QString::number(reck_mask.height);
	//QString str = x + "\t y:" + y + "\t width:" + width + "\t  height:" + height;
	//ui->label_3->setText(str);
}
void MainWindow::on_actionCut_triggered()
{
	on_actionOpenCutWindow_triggered();
}
//识别参数，写入config文件中
void MainWindow::on_actionGetParemeter_triggered()
{
	Mat img = imread("Train/image/Pic.bmp");
	//img = img(rect_of_image);
	String modelConfiguration = "D:/yolov3.cfg";
	String model_label_Weights = "D:/yolov3-voc_9000.weights";

	vector<Rect> boxes = detect_image(img, model_label_Weights, modelConfiguration);
	//排序，根据得到的方框的中点的纵坐标进行排序，按照y从小到大的顺序排
	sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;

	//for循环获取每个黑块的中点，并存储到points中
	for (int i = 0; i < boxes.size(); i++) {
		points.push_back(Point(boxes[i].x + 0.5*boxes[i].width, boxes[i].y + 0.5*boxes[i].height));
	}

	int length = points.size();
	double xmean = 0.0;
	double ymean = 0.0;
	for (int i = 0; i < length; i++)
	{
		xmean += points[i].x;
		ymean += points[i].y;
	}
	xmean /= length;
	ymean /= length;

	double sumx2 = 0.0;
	double sumy2 = 0.0;
	double sumxy = 0.0;
	for (int i = 0; i < length; i++)
	{
		sumx2 += (points[i].x - xmean) * (points[i].x - xmean);
		sumy2 += (points[i].y - ymean) * (points[i].y - ymean);
		sumxy += (points[i].y - ymean) * (points[i].x - xmean);
	}
	double slope1 = sumxy / sumx2;  //斜率
	double intercept1 = ymean - slope1 * xmean; //截距
	double r_square1 = sumxy * sumxy / (sumx2 * sumy2); //相关系数

	//写入配置文件
	Config().Set("Line Fitting", "k", slope1);
	Config().Set("Line Fitting", "b", intercept1);
	Config().Set("Line Fitting", "s", r_square1);
	Config().Set("Line Fitting", "n", length);

	//把参数更新到类属性变量中
	Num_of_blocks = length;
	k = slope1;
	b = intercept1;
	s = r_square1;
	ui->label_3->setText("参数识别完毕！");

	QMessageBox::information(NULL, "参数识别", "参数识别完成！");
}

void MainWindow::on_actiontakephoto_triggered()
{
     on_actionSavePic_triggered();
}
