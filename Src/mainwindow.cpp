#pragma execution_character_set("utf-8")
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEFAULT_SHOW_RATE (30)
#define TIMESTAMPFREQUENCY 125000000	//�������ʱ���Ƶ�ʹ̶�Ϊ125,000,000Hz
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

	// Ĭ����ʾ30֡
	setDisplayFPS(30);

	m_elapsedTimer.start();

	// ������ʾ�߳�
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
	
	//��ʾ����ͼƬ
	QPixmap pixmap = QPixmap::fromImage(image);
	int w = ui->label->width();
	int h = ui->label->height();	
	ui->label->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
	//label2��ʾ�ü�֮�����Ƭ
	Mat img = QImage2cvMat(image);
	img = img(rect_of_image); //�ü�
	cv::Mat out;
	cv::Mat in[] = { img, img, img };
	cv::merge(in, 3, out);
	ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
	//��ʾ�ü�����ʶ����ͼƬ
	if (true == Mode_of_trig) {
		try
		{
			//QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
			if (!bookdetection(out))//ʶ���ж�
			{
				Beep(1000, 1000);
				cout << "���ϸ�" << endl << endl;
				//��������,2���Զ��ر�
				alertWindow = new AlertWindow;
				alertWindow->show();
				waitKey(2000);
				alertWindow->close();
				//QMessageBox::information(NULL, "����", "");

				//output file
				//imwrite(wrong_filename, src_mat);
				//run_database(current_time, "���ϸ�");
				unsigned char uc[] = { 0x7e,0x01,0x55,0x55,0x0d,0x0d };
				int count = 0;
				while (revFlag != true) {
					revFlag = mycserialport.WriteData(uc, 6);
					Sleep(50);
					count++;
					if (count >= 3) {
						//cout << "δ�յ���λ��ȷ����Ϣ!" << endl;
						//���������Σ���������,���ض���ִ�гɹ�
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
			QMessageBox::information(NULL, "ʶ�𲿷ֳ���", e.what());
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
				QMessageBox::information(NULL, "����ͼƬ", "����ͼƬ�ɹ���");
			}
			catch (const std::exception& e)
			{
				QMessageBox::information(NULL, "ʶ�𲿷ֳ���", e.what());
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
			if (!(img->load(infile2))) //����ͼ��
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
				cv::cvtColor(image_for_write, Rgb, CV_BGR2RGB);//��ɫ�ռ�ת��
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
			if (!bookdetection(image_for_write))//ʶ���ж�
			{
				Beep(1000, 1000);
				cout << "���ϸ�" << endl << endl;
				ss << imagefile << "wrong-" << i << ".bmp";
				imwrite(ss.str(), image_for_write);
				//��������
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
	//ʶ���ɫ��־�����غ�ɫ����ľ�����Ϣ������boxes��
	vector<Rect> boxes = detect_image(imagefile, model_label_Weights, modelConfiguration);
	//���򣬸��ݵõ��ķ�����е��������������򣬰���y��С�����˳����
	sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;
	int max_point_x = 0, max_point_y = 0;
	double average_piexl_value = 0;

	//forѭ����ȡÿ���ڿ���е㣬���洢��points��
	for (int i = 0; i < boxes.size(); i++) {
		points.push_back(Point(boxes[i].x + 0.5*boxes[i].width, boxes[i].y + 0.5*boxes[i].height));
		//average_piexl_value = average_piexl_value + imagefile.at<uchar>(points[i]);
		//if (boxes[i].x > max_point_x) {
		//	max_point_x = boxes[i].x;
		//	max_point_y = boxes[i].y;
		//}
		//if (i > 0)
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << " �����ࣺ" << points[i - 1].x - points[i].x << ", " << points[i].y - points[i - 1].y << endl;
		//else
		//	cout << "Rect(" << boxes[i].x << ", " << boxes[i].y << ", " << boxes[i].width << ", " << boxes[i].height << ")" << "Mid-points " << ":" << points[i].x << "," << points[i].y << endl;
		//cout << "Mid-points " << i << "," << points[i].x << "," << points[i].y << endl;
		//cout << i << ": boxes[i].x " << boxes[i].x << " boxes[i].y " << boxes[i].y << " boxes[i].width " << boxes[i].width << " boxes[i].height " << boxes[i].height << endl;
	}
	//if (boxes.size() < block_nums) {
	//	int this_boxes_size = boxes.size();
	//	//������е�����λ��
	//	for (int i = 0; i <= points.size(); i++) {
	//		//�����ǰ����е�����һ������е�����λ�õ�y��5������Ϊ�˿��λ����ȷ
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//���������Ƿ���ȱʧ��
	//		else
	//		{
	//			//;�����ȱʧ����һ����
	//			//�ҵ���һ��������Ͻ�λ��,x�ǴӴ�С��y�Ǵ�С����
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
	//				cout << "ȱʧ��λ��������ȷ" << endl;
	//				imwrite(outfile, imagefile);
	//				cout << "�ļ�д�룺" + outfile << endl << endl;
	//				if (this_boxes_size < points.size()) 
	//					continue;
	//				else
	//					return 1;
	//			}
	//			else
	//				//cout << "ȱʧ��λ����������!" << endl;
	//				continue;
	//				//return false;				
	//		}
	//	}
	//	return 1;
	//}
	//ǰʮ����������Ӧ�����ؾ�б�ʸ����ϵ������
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
	//	cout << "sum_b: " << sum_b << " sum_k��" << sum_k << " sum_r_q: " << sum_r_q << endl;
	//	k = sum_k / 10;
	//	b = sum_b / 10;
	//	r_q = sum_r_q / 10;
	//	cout << "k: " << k << " b: " << b << " r_q: " << r_q;
	//}
	//ǰʮ����������Ӧ�����ؾ�б�ʸ����ϵ������
	//if (boxes.size() == block_nums && total_number < 11) {
	//	LinearFitting(points, 0, 0, 0);
	//}

	//cout << "�����ɫ��������� " << boxes.size() << endl;
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
	//	//������е�����λ��
	//	for (int i = 0; i <= points.size(); i++) {
	//		//�����ǰ����е�����һ������е�����λ�õ�y��5������Ϊ�˿��λ����ȷ
	//		if (abs(abs(points[i].y - points[i + 1].y) - relative_locations[i].y) <= 10) {
	//			cout << "Correct point" << i << endl;
	//			continue;
	//		}
	//		//���������Ƿ���ȱʧ��
	//		else
	//		{
	//			//;�����ȱʧ����һ����
	//			////�ҵ���һ��������Ͻ�λ��,x�ǴӴ�С��y�Ǵ�С����
	//			//int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width;
	//			//int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height;
	//			//����y��С�����˳������
	//			int x_1 = points[i].x - relative_locations[i].x - 0.5*locations[i + 1].width + 0.5*locations[i].width;
	//			int y_1 = points[i].y + relative_locations[i].y - 0.5*locations[i + 1].height + 0.5*locations[i].height;
	//			Rect r = Rect(x_1, y_1, locations[i + 1].width, locations[i + 1].height);
	//			average_piexl_value = average_piexl_value / boxes.size() + 10;
	//			Mat imagefile1 = imagefile(r);
	//			double value = 0;
	//			//ѭ���������úڿ��е��������ؼ���ƽ�ֻҶ�ֵ
	//			for (int i = 0; i < imagefile1.rows; ++i) for (int j = 0; j < imagefile1.cols; ++j) value += imagefile1.at<uchar>(i, j);
	//			value = value / (imagefile1.cols*imagefile1.rows);
	//			if (value < average_piexl_value)
	//			{  
	//				rectangle(imagefile, r, Scalar(255, 108, 50), 2);
	//				cout << "ȱʧ��λ��������ȷ" << endl;
	//				boxes.push_back(r);
	//				continue;
	//			}
	//			else
	//			{
	//				cout << "ȱʧ��λ����������!" << endl;
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
	//	cout << "��ɫ��־��������ȷ" << endl;
	//	ui->label_3->setText("Correct");
	//	//imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "����");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points,k, b, s);
	//case 12:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "��ɫ��־����Ϊ12" << endl;
	//	outfile = "E:\\pic\\label\\12-" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	ui->label_3->setText("12");
	//	run_database(get_datetime(), "����");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points, k, b, s);
	//case 10:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "��ɫ��־����Ϊ10" << endl;
	//	ui->label_3->setText("10");
	//	outfile = "E:\\pic\\label\\��" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "�쳣");
	//	//emit SendUpdateLCDMsg(2);
	//	return LinearFitting(points, k,b,s) && 0;
	//default:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	cout << "��ɫ��־��������  " << this_block_nums << endl;
	//	ui->label_3->setText("Wrong");
	//	outfile = "E:\\pic\\label\\����" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	//imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "�쳣");
	//	//emit SendUpdateLCDMsg(2);
	//	return 0;
	//}
	
}

//OpenCV Mat��QImage�໥ת������
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
	double slope1 = sumxy / sumx2;  //б��
	double intercept1 = ymean - slope * xmean; //�ؾ�
	double r_square1 = sumxy * sumxy / (sumx2 * sumy2); //���ϵ��
	cout << "y = " << slope1 << "x+ " << intercept1 << "  r_square1 is " << r_square1 << endl;

	//�ж�����ֻ��������ϵ����б��
	if (abs(r_square - r_square1) <= 0.1 && abs(slope - slope1) <= 0.1) {
		cout << "���ϵ����ȷ" << endl;

		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + "Correct";
		ui->label_3->setText(str);
		return true;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_RED);
		cout << "���ϵ������" << endl;
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
	//cout << "indices.size(ƥ��õ���Ŀ����):" << indices.size() << endl;
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
		//����
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
	// database.exe 2016-01-22 08:45:50 �쳣
	string filename = "database.exe";
	//string params = "2016-01-22 08:45:50 �쳣"
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
	ShExecInfo.lpFile = _T("labelImg\\labelImg.exe");        // ִ�еĳ����� 
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;                // ȫ����ʾ������� 
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}

void MainWindow::run_cut()
{
	// database.exe 2016-01-22 08:45:50 �쳣
	//string filename = "labelImg\\labelImg.exe";
	//string params = "2016-01-22 08:45:50 �쳣"
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

	//����1-��ʾͼ���Label������2-Ҫ��ʾ��Mat
	Mat Rgb;
	QImage Img;
	if (img.channels() == 3)//RGB Img
	{
		cv::cvtColor(img, Rgb, CV_BGR2RGB);//��ɫ�ռ�ת��
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

	// ��ӡ���������Ϣ��key, ��������Ϣ, �ͺ�, ���кţ�
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
		//Ĭ�������б��еĵ�һ�����Ϊ��ǰ�����������������򿪡��رա��޸��عⶼ�������������
		m_pCamera = m_vCameraPtrList[0];
	}
}

//�л��ɼ���ʽ��������ʽ �������ɼ����ⲿ���������������
void MainWindow::CameraChangeTrig(ETrigType trigType)
{
	if (NULL == m_pCamera)
	{
		printf("Change Trig fail. No camera or camera is not connected.\n");
		return;
	}

	if (trigContinous == trigType)
	{
		//���ô���ģʽ
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
		//���ô���ԴΪ����
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

		//���ô�����
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

		//���ô���ģʽ
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
		//���ô���ԴΪLine1����
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

		//���ô�����
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

		//���ô���ģʽ
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

		// �����ⴥ��Ϊ�����أ��½���ΪFallingEdge��
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

//�����
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

//�ر����
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

//��ʼ�ɼ�
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

//ֹͣ�ɼ�
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

	/* �����ʾ���� */
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

	/* �ڴ�����ʧ�ܣ�ֱ�ӷ��� */
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

		// �ж��Ƿ�Ҫ��ʾ��������ʾ���ޣ�30֡�����Ͳ���ת�롢��ʾ����
		if (!isTimeToDisplay(frameInfo.m_nTimeStamp))
		{
			/* �ͷ��ڴ� */
			free(frameInfo.m_pImageBuf);
			continue;
		}

		/* mono8��ʽ�ɲ���ת�룬ֱ����ʾ��������ʽ��Ҫ����ת�������ʾ */
		if (Dahua::GenICam::gvspPixelMono8 == frameInfo.m_PixelType)
		{
			/* ��ʾ */
			if (false == ShowImage(frameInfo.m_pImageBuf, frameInfo.m_nBufferSize, frameInfo.m_nWidth, frameInfo.m_nHeight, frameInfo.m_PixelType))
			{
				printf("_render.display failed.\n");
			}
			/* �ͷ��ڴ� */
			free(frameInfo.m_pImageBuf);
		}
		else
		{
			/* ת�� */
			uint8_t *pRGBbuffer = NULL;
			int nRgbBufferSize = 0;
			nRgbBufferSize = frameInfo.m_nWidth * frameInfo.m_nHeight * 3;
			pRGBbuffer = (uint8_t *)malloc(nRgbBufferSize);
			if (pRGBbuffer == NULL)
			{
				/* �ͷ��ڴ� */
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
				/* �ͷ��ڴ� */
				printf("IMGCNV_ConvertToRGB24 failed.\n");
				free(frameInfo.m_pImageBuf);
				free(pRGBbuffer);
				return;
			}

			/* �ͷ��ڴ� */
			free(frameInfo.m_pImageBuf);

			/* ��ʾ */
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

	// ����ʾ
	if (m_nDisplayInterval <= 0)
	{
		return false;
	}

	// ��һ֡������ʾ
	if (m_nFirstFrameTime == 0 || m_nLastFrameTime == 0)
	{
		m_nFirstFrameTime = m_elapsedTimer.nsecsElapsed();
		m_nLastFrameTime = m_nFirstFrameTime;

		return true;
	}

	// ��ǰ֡����һ֡�ļ�����������ʾ�������ʾ
	uint64_t nCurTimeTmp = m_elapsedTimer.nsecsElapsed();
	double dAcquisitionInterval = nCurTimeTmp * 1000 / TIMESTAMPFREQUENCY;
	uint64_t nAcquisitionInterval = nCurTimeTmp - m_nLastFrameTime;
	if (nAcquisitionInterval > m_nDisplayInterval)
	{
		m_nLastFrameTime = nCurTimeTmp;
		return true;
	}


	// ��ǰ֡����ڵ�һ֡��ʱ����
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
//�����
void MainWindow::on_pushButton_clicked()
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);

	//����Ҫ����ͷ�������ļ����Ժ�����
	testRun();

	//----------------------------���»���-------------------------------------------

	////��������ͷʵʱ�����
	////�����
	//ICameraPtr cameraSptr;
	////�����豸
	//CSystem &systemObj = CSystem::getInstance();
	//TVector<ICameraPtr> vCameraPtrList;
	//bool bRet = systemObj.discovery(vCameraPtrList);

	//if (!bRet)
	//{
	//	QMessageBox::warning(NULL, "warning", "�����豸ʧ��\n", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//	ui->pushButton->setEnabled(true);
	//	ui->pushButton_2->setEnabled(false);
	//	ui->pushButton_3->setEnabled(false);
	//	ui->pushButton_4->setEnabled(false);
	//	return;
	//}

	//if (0 == vCameraPtrList.size())
	//{
	//	QMessageBox::warning(NULL, "warning", "��������ͷʧ��\n");
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
//�ر����
void MainWindow::on_pushButton_2_clicked()
{
	//�ر����
	CameraStop();
	CameraClose();
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(false);
}
//��ʼ ʶ��
void MainWindow::on_pushButton_3_clicked()
{
	//��ʼ ʶ��
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(true);
	//�������ΪӲ��������ÿ����һ��ִ��һ���ж�
	CameraChangeTrig(trigLine);
	
}
//ֹͣʶ��
void MainWindow::on_pushButton_4_clicked()
{
	//ֹͣʶ��
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);
	ui->pushButton_4->setEnabled(false);
	//�ָ����Ϊ��������
	CameraChangeTrig(trigContinous);
}

//��������Ƭ
void MainWindow::on_actionSavePic_triggered()
{
	//�����
	/*
	����Ϊ���������ʽ������������
	����һ����ť��Ȼ�������գ�
	*/
	CSystem &systemObj = CSystem::getInstance();

	bool bRet = systemObj.discovery(m_vCameraPtrList);
	if (false == bRet || m_vCameraPtrList.size() < 1)
	{
		printf("discovery fail.\n");
		QMessageBox::critical(NULL, "����", "δ���ӵ������");
		return;
	}
	else
	{
		takephoto = new TakePhoto;
		connect(takephoto, SIGNAL(sendTakePhoteToMainWidget()), this, SLOT(set_Mode_of_trig_soft()));
		takephoto->show();
	}
}
//ͼ����
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

//����ѵ��
void MainWindow::on_actionTrain_triggered()
{
	//ͼ��ѵ��

}

//��ʾ�ü����ڡ��ο�https://www.cnblogs.com/blogpro/p/11343975.html
void MainWindow::on_actionOpenCutWindow_triggered()
{
	/* QtУ��ģ���ļ�hashֵ�ж��Ƿ���� https://blog.csdn.net/emdfans/article/details/23871741 
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
	//�жϲü����ڹر�ǰ��ģ���ļ��Ƿ����˱仯�������仯˵�������˲ü���δ�����仯��˵��û�вü���ִ���κβ���ֻ������ʾ
	if (ba != bb) {
		ui->label_3->setText("�ü������");
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
		Config().Set("Image Rect", "Book Name", "����");
		Config().Set("Image Rect", "x", rect_mask.x);
		Config().Set("Image Rect", "y", rect_mask.y);
		Config().Set("Image Rect", "width", rect_mask.width);
		Config().Set("Image Rect", "height", rect_mask.height);

		img = img(rect_mask);
		imshow("�ü����ͼ��", img);
		imwrite("Train/image/Pic.bmp", img);
	}
	else
	{
		QMessageBox::warning(NULL, "ͼ��ü�����", "ͼ��δִ�вü����������������");
		ui->label_3->setText("�ü�����ɴ���δִ�вü����������������");
		return;
	}
	//SHELLEXECUTEINFO  ShExecInfo;
	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = NULL;
	//ShExecInfo.hwnd = NULL;
	//ShExecInfo.lpVerb = NULL;
	//ShExecInfo.lpFile = _T("TemplateEditor\\TemplateEditor.exe");        // ִ�еĳ����� 
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_MAXIMIZE;                // ȫ����ʾ������� 
	//ShExecInfo.hInstApp = NULL;
	//BOOL ret = ShellExecuteEx(&ShExecInfo);
	//ui->label->setText("�ü��Ѵ�");
	//HANDLE hThread = CreateThread(NULL, 0, MyThreadProc1, NULL, 0, NULL);
	//DWORD dwReturn = WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	////DWORD dwReturn = WaitForSingleObject(hThread, INFINITE);
	//switch (dwReturn)
	//{
	//case WAIT_OBJECT_0:
	//	// hProcess������Ľ�����5���ڽ���
	//	cout << "MyThreadProc1 >>> WaitForSingleObject signaled\n" << endl;
	//	ui->label->setText("�ü������");
	//	Config().Set("Image Rect", "Image Rect", "1236");
	//	break;
	//case WAIT_TIMEOUT:
	//	// �ȴ�ʱ�䳬��5��
	//	break;
	//case WAIT_FAILED:
	//	// ��������ʧ�ܣ����紫����һ����Ч�ľ��
	//	break;
	//}	
}
//ͼ��ü�
void MainWindow::on_actionCut_2_triggered()
{
	on_actionOpenCutWindow_triggered();
}

//��������
void MainWindow::on_actionPara_triggered()
{
	para = new ParametersSetting;
	//connect(this, SIGNAL(sendData(double, double, double, int)), para, SLOT(recevieData(double, double, double)));	
	//emit sendData(k, b, s);
	connect(para, SIGNAL(sendDataToMainWidget(double, double, double)), this, SLOT(recevieDataFromSubWin(double, double, double, int)));
	para->show();
}

//���ղ������ô������õĲ���
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

//������ʼ��
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
//ʶ�������д��config�ļ���
void MainWindow::on_actionGetParemeter_triggered()
{
	Mat img = imread("Train/image/Pic.bmp");
	//img = img(rect_of_image);
	String modelConfiguration = "D:/yolov3.cfg";
	String model_label_Weights = "D:/yolov3-voc_9000.weights";

	vector<Rect> boxes = detect_image(img, model_label_Weights, modelConfiguration);
	//���򣬸��ݵõ��ķ�����е��������������򣬰���y��С�����˳����
	sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;

	//forѭ����ȡÿ���ڿ���е㣬���洢��points��
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
	double slope1 = sumxy / sumx2;  //б��
	double intercept1 = ymean - slope1 * xmean; //�ؾ�
	double r_square1 = sumxy * sumxy / (sumx2 * sumy2); //���ϵ��

	//д�������ļ�
	Config().Set("Line Fitting", "k", slope1);
	Config().Set("Line Fitting", "b", intercept1);
	Config().Set("Line Fitting", "s", r_square1);
	Config().Set("Line Fitting", "n", length);

	//�Ѳ������µ������Ա�����
	Num_of_blocks = length;
	k = slope1;
	b = intercept1;
	s = r_square1;
	ui->label_3->setText("����ʶ����ϣ�");

	QMessageBox::information(NULL, "����ʶ��", "����ʶ����ɣ�");
}

void MainWindow::on_actiontakephoto_triggered()
{
     on_actionSavePic_triggered();
}
