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
	//ui->pushButton->setEnabled(true);
	//ui->pushButton_2->setEnabled(false);
	//ui->pushButton_3->setEnabled(false);
	//ui->pushButton_4->setEnabled(false);

	ui->splitter->setStretchFactor(0, 4);
	ui->splitter->setStretchFactor(1, 1);	
	//ui->splitter->setStyleSheet("QSplitter::handle { background-color: gray }");
	//this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
	int label_width = this->width() * 8 / 9;
	int label_heiht = this->height() / 5;
	ui->label->setMaximumWidth(label_width);
	ui->label->setMaximumHeight(label_heiht * 3);
	ui->label_2->setMaximumWidth(label_width);
	ui->label_2->setMaximumHeight(label_heiht);
	init_parameters();

}

void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
//������ʼ��
void MainWindow::init_parameters() {

	s = Config().Get("Line_Fitting", "s").toDouble();
	b = Config().Get("Line_Fitting", "b").toDouble();
	k = Config().Get("Line_Fitting", "k").toDouble();
	Num_of_blocks = Config().Get("Line_Fitting", "n").toInt();

	total_number = Config().Get("Count", "Count").toInt();
	sum_of_correct = Config().Get("Count", "sum_of_correct").toInt();
	sum_of_wrong = Config().Get("Count", "sum_of_wrong").toInt();

	rect_of_image.x = Config().Get("Image_Rect", "x").toInt();
	rect_of_image.y = Config().Get("Image_Rect", "y").toInt();
	rect_of_image.width = Config().Get("Image_Rect", "width").toInt();
	rect_of_image.height = Config().Get("Image_Rect", "height").toInt();
	
	
	
	for (int i = 1; i <= Num_of_blocks; i++) {
		string xyk = Config().Get("relative_location", QString::number(i)).toString().toStdString();
		if (xyk == "")
			break;
		vector<string> v;
		SplitString(xyk, v, " "); //�ɰ�����ַ����ָ�;
		relative_location tempxyk;
		tempxyk.x = stoi(v[0]);
		tempxyk.y = stoi(v[1]);
		tempxyk.k = stod(v[2]);
		re_locations.push_back(tempxyk);
	}

	//subthread = new QThread(this);
	//m_MyThread = new MyThread();
	//m_MyThread->moveToThread(subthread);
	//connect(subthread, SIGNAL(finished()), m_MyThread, SLOT(deleteLater()));//https://blog.csdn.net/naibozhuan3744/article/details/81201502?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-3.nonecase
	//connect(subthread, SIGNAL(started()), m_MyThread, SLOT(startThreadSlot()));
	//connect(this, &MainWindow::StartThread, m_MyThread, &MyThread::MyWork);
	//connect(m_MyThread, SIGNAL(&MyThread::signal_back), this, SLOT(&MainWindow::on_pushButton_2_clicked));
	//connect(m_MyThread, SIGNAL(&MyThread::signal_back), this, SLOT(&MainWindow::on_pushButton_2_clicked));
	//qDebug() << "the main thread number:" << QThread::currentThread();
	//connect(this, &MainWindow::destoryed, this, &MainWindow::CloseWidget);   //���ϽǴ��ڹر�
	
	//mycserialport.InitPort();
	//mycserialport.OpenListenThread();

	detector =new Detector("D://yolov3.cfg", "D://yolov3_final.weights", 0);
	//״̬����ʾ��Ϣhttps://blog.csdn.net/theRookie1/article/details/84751548
	//QLabel *locationLabel;
	//locationLabel = new QLabel("July");
	//locationLabel->setAlignment(Qt::AlignCenter);
	//locationLabel->setMinimumSize(locationLabel->sizeHint());
	//QLabel *aixLabel;
	//aixLabel = new QLabel("��CTRL + H�� for help");
	//Optional
	//statusBar()->setStyleSheet(QString("QStatusBar::item{ border: 0px }")); // ���ò���ʾlabel�ı߿�
	//statusBar()->setSizeGripEnabled(false); //�����Ƿ���ʾ�ұߵĴ�С���Ƶ�
	//statusBar()->addWidget(locationLabel);
	//statusBar()->addWidget(aixLabel, 1);

	QWidget* searchWidget = new QWidget(this);
	ui->toolBar->addWidget(searchWidget);
	QHBoxLayout* searchLayout = new QHBoxLayout(searchWidget);
	QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	searchLayout->addItem(spacer);
	QLabel *per1 = new QLabel();
	QLabel *per2 = new QLabel("�Ϻ��Ÿ�������޹�˾", this);
	QPixmap image; //����һ��ͼ
	image.load("jiuge-new.jpg");//����
	per1->clear();//���
	per1->setPixmap(image);//���ص�Label��ǩ
	per1->setScaledContents(true);
	per1->setFixedSize(35, 35);
	per1->show();//��ʾ
	//statusBar()->addPermanentWidget(per1); //��ʵ������Ϣ
	//statusBar()->addPermanentWidget(per2);
	//mSearchButtonWidget = new QPushButton(tr("����"), searchWidget);
	//mSearchEditWidget = new QLineEdit(searchWidget);
	//mSearchEditWidget->setMaximumWidth(120);
	searchLayout->addWidget(per1);
	searchLayout->addWidget(per2);

	//QLabel *per1 = new QLabel();
	//QLabel *per2 = new QLabel("�Ϻ��Ÿ�������޹�˾", this);
	//QPixmap image; //����һ��ͼ
	//image.load("jiuge-new.jpg");//����
	//per1->clear();//���
	//per1->setPixmap(image);//���ص�Label��ǩ
	//per1->setScaledContents(true);
	//per1->setFixedSize(35, 35);
	//per1->show();//��ʾ
	//statusBar()->addPermanentWidget(per1); //��ʵ������Ϣ
	//statusBar()->addPermanentWidget(per2);

	//����������
	//image.load("Pic (1).bmp");
	//ui->label->setPixmap(image);
	//image.load("Pic (1)-cut.png");
	//ui->label_2->setPixmap(image);
	//ui->label_3->setText("20200705 09:36:47 �ź��ﳣ�����ֵ�");
	//ui->label_7->setText("��־���˳����ȷ��������ȷ��װ����ȷ��");
	//ui->lcdNumber->display(2598);
	//ui->lcdNumber_2->display(2602);
	//ui->lcdNumber_3->display(4);

	//QLabel *per3 = new QLabel("Ready3", this);
	//statusBar()->insertPermanentWidget(2, per3);
	//statusBar()->showMessage("Status is here��", 3000); // ��ʾ��ʱ��Ϣ��ʱ��3����.
}

MainWindow::~MainWindow()
{

	CameraClose();
    delete ui;
}
//#define OPENCV 
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

	QPixmap pixmap = QPixmap::fromImage(image);
	int w = ui->label->width();
	int h = ui->label->height();
	ui->label->setPixmap(pixmap.scaled(w, h, Qt::KeepAspectRatio));
	//free(pRgbFrameBuf);
	
	CEnumNode nodeEnum(m_pCamera, "TriggerSource");
	CString strTriggerSource;
	nodeEnum.getValueSymbol(strTriggerSource);
	if (strTriggerSource == "Software")
	{
		ui->label_7->setText("Software");
	}
	//CEnumNode nodeEnum2(m_pCamera, "TriggerMode");
	//CString strTriggerMode;
	//nodeEnum2.getValueSymbol(strTriggerMode);
	if (strTriggerSource == "Line1") {

		ui->lcdNumber_2->display(++total_number);
		Config().Set("Count", "Count", total_number);
		//�������� ÿ����һ�ٸ���д��һ��
		if (0 == total_number % 100) {
			//�ο�����https://www.cnblogs.com/findumars/p/7252854.html
			ofstream outFile;
			outFile.open("count.csv", ios::app); // ��ģʽ��ʡ�ԣ��˴��õ������ģʽ
			outFile << get_datetime() << ',' << "����" << ',' << total_number << ',' << "��ȷ����" << ',' << sum_of_correct << ',' << "��������" << ',' << sum_of_wrong << endl;
			outFile.close();
		}

		Mat img = QImage2cvMat(image);
		Pic_to_Save = img;
		if (true == Mode_of_trig_soft) {
			imwrite("Train/image/Pic.bmp", Pic_to_Save);
			ui->label_3->setText("����ͼ��ɹ�");
			//QMessageBox::information(this, "����ͼƬ�ɹ�", "����ͼ��ɹ�");
			Mode_of_trig_soft = false;
		}
		//�ü�����ʾ
		img = img(rect_of_image); 
		cv::Mat out;
		cv::Mat in[] = { img, img, img };
		cv::merge(in, 3, out);
		//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
		//ʶ��ü����ͼƬ
		try
		{
			startTime = clock();
			vector<Point> points;
#ifdef OPENCV
			std::vector<bbox_t> result_vec = detector->detect(out);
			for (auto &i : result_vec) {
				cv::rectangle(out, cv::Rect(i.x, i.y, i.w, i.h), cv::Scalar(50, 200, 50), 3);
				points.push_back(Point(i.x + i.w*0.5, i.y + i.h*0.5));
			}
			ui->lcdNumber_3->display(++sum_of_wrong);
			ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
			if (result_vec.size() == Num_of_blocks && LinearFitting(points, k, b, s))
			{
				ui->label_3->setText("Correct");
				ui->lcdNumber->display(++sum_of_correct);
				Config().Set("Count", "sum_of_correct", sum_of_correct);
				return true;
			}
			else {
				ui->label_3->setText("Wrong");
				Config().Set("Count", "sum_of_wrong", sum_of_wrong);
				
				return false;
			}
#endif // OPENCV
			//Beep(1000, 1000);
			//cout << "���ϸ�" << endl << endl;
			//emit StartThread();
			//��������,2���Զ��ر�
			//alertWindow = new AlertWindow;
			//alertWindow->startTimer();
			//alertWindow->exec();
			//output file
			//imwrite(wrong_filename, src_mat);
			//run_database(current_time, "���ϸ�");
			//unsigned char uc[] = { 0x7e,0x01,0x55,0x55,0x55,0x55 };
			//int count = 0;
			//while (revFlag != true) {
			//	revFlag = mycserialport.WriteData(uc, 6);
			//	Sleep(50);
			//	count++;
			//	if (count >= 3) {
			//		//cout << "δ�յ���λ��ȷ����Ϣ!" << endl;
			//		//���������Σ���������,���ض���ִ�гɹ�
			//		count = 0;
			//		break;
			//	}
			//}
			//revFlag = false;
			image_t img_t;
			img_t.w = nWidth;
			img_t.h = nHeight;
			img_t.data = (float *)pRgbFrameBuf;
			
			//if (!bookdetection(out))//ʶ���ж�
			//{
			//	ui->lcdNumber_3->display(++sum_of_wrong);
			//	Config().Set("Count", "sum_of_wrong", sum_of_wrong);
			//	//Beep(1000, 1000);
			//	//cout << "���ϸ�" << endl << endl;
			//	//emit StartThread();
			//	//��������,2���Զ��ر�
			//	//alertWindow = new AlertWindow;
			//	//alertWindow->startTimer();
			//	//alertWindow->exec();
			//	//output file
			//	//imwrite(wrong_filename, src_mat);
			//	//run_database(current_time, "���ϸ�");
			//	//unsigned char uc[] = { 0x7e,0x01,0x55,0x55,0x55,0x55 };
			//	//int count = 0;
			//	//while (revFlag != true) {
			//	//	revFlag = mycserialport.WriteData(uc, 6);
			//	//	Sleep(50);
			//	//	count++;
			//	//	if (count >= 3) {
			//	//		//cout << "δ�յ���λ��ȷ����Ϣ!" << endl;
			//	//		//���������Σ���������,���ض���ִ�гɹ�
			//	//		count = 0;
			//	//		break;
			//	//	}
			//	//}
			//	//revFlag = false;
			//}
			//else {
			//	ui->lcdNumber->display(++sum_of_correct);
			//	Config().Set("Count", "sum_of_correct", sum_of_correct);
			//}
			endTime = clock();
			string s = get_datetime() + "����ʱ��: " + to_string((double)(endTime - startTime) / CLOCKS_PER_SEC) + "s";
			QString st = QString::fromStdString(s);
			ui->label_7->setText(st);
		}
		catch (const std::exception& e)
		{
			QMessageBox::information(NULL, "ʶ�𲿷ֳ���", e.what());
			return true;
		}
	}
	

	////else if (true == Mode_of_trig_soft) {
	////	try
	////	{
	////		Pic_to_Save = QImage2cvMat(image);
	////		ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(Pic_to_Save)));
	////		////imshow("test", img);
	////		//bool ret = imwrite("Train/image/Pic.bmp", img);
	////		////cv::Mat out;
	////		////cv::Mat in[] = { img, img, img };
	////		////cv::merge(in, 3, out);
	////		////ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
	////		//////QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
	////		////if (ret) {
	////		////	//QMessageBox::information(NULL, "����ͼƬ", "����ͼƬ�ɹ���");
	////		////	ui->label_3->setText("����ͼƬ�ɹ�");
	////		////	Mode_of_trig_soft = false;
	////		////}
	////		////else
	////		////{
	////		////	QMessageBox::warning(NULL, "����ͼƬʧ�ܣ�", "����ͼƬʧ�ܣ�");
	////		////}
	////		//Mat i = imread("Train/image/Pic.bmp");
	////		//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(i)));
	////	}
	////	catch (const std::exception& e)
	////	{
	////		QMessageBox::information(NULL, "ʶ�𲿷ֳ���", e.what());
	////		return true;
	////	}
	////	Mode_of_trig_soft = false;
	////}
	////else
	////{
	////	//������ť����ͼƬ
	////	if (true == Mode_of_trig_soft) {
	////		try
	////		{
	////			Mat img = QImage2cvMat(image);
	////			Mode_of_trig_soft = false;
	////			cv::Mat out;
	////			cv::Mat in[] = { img, img, img };
	////			cv::merge(in, 3, out);
	////			//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(out)));
	////			//QMessageBox::information(NULL, "img channels", QString::number(out.channels()));
	////			imwrite("Train/image/Pic.bmp", img);
	////			QMessageBox::information(NULL, "����ͼƬ", "����ͼƬ�ɹ���");
	////		}
	////		catch (const std::exception& e)
	////		{
	////			QMessageBox::information(NULL, "ʶ�𲿷ֳ���", e.what());
	////			return true;
	////		}
	////		Mode_of_trig_soft = false;
	////	}
	////}
	return true;
}

bool sortFun(bbox_t p1, bbox_t p2) {
	return p1.y + 0.5*p1.h < p2.y + 0.5* p2.h;
}


void MainWindow::draw_boxes(cv::Mat mat_img, std::vector<bbox_t> result_vec) {
	for (auto &i : result_vec) {
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), cv::Scalar(50, 200, 50), 3);
	}
	//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(mat_img)));
	//cv::imshow("window name", mat_img);
	//cv::waitKey(0);
}

//���ڲ��Ա���ͼƬ�ļ�
void MainWindow::testRun() {
	clock_t startTime, startTime1, endTime;
	startTime = clock();
	stringstream ss;
	//string imagefile = "C:\\Users\\30923\\MVviewer\\pictures\\A3600MG18_3L05FEDPAK00028\\";
	string imagefile = "F:\\pic\\";

	//string imagefile = "D:\\Pic\\";
	try
	{
		string outfile;
		Mat image_for_write;
		for (int i = 4465; i <4500; i++) {
			//ui->label_7->setText("");
			startTime1 = clock();
			//ss << imagefile <<"Pic_2020_06_26 (" << i << ").bmp";Pic_blockId#4268.bmp
			ss << imagefile << "Pic_blockId#" << i <<".bmp";
			
			string infile = ss.str();
			QString infile2 = QString::fromStdString(infile);
			ui->label_3->setText(infile2);
			QImage* img = new QImage;
			if (!(img->load(infile2))) //����ͼ��
			{
				QMessageBox::information(this,tr("����ͼ�� FAILED"),tr("����ͼ�� FAILED!"));
				delete img;
				return;
			}
			ui->label->setPixmap(QPixmap::fromImage(*img));
			delete img;
			
			//image_for_write = imread(infile);			
			//image_for_write = image_for_write(rect_of_image);
			//ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(image_for_write)));
			//QImage Img;
			//Mat Rgb;
			//if (image_for_write.channels() == 3)//RGB Img
			//{
			//	cv::cvtColor(image_for_write, Rgb, CV_BGR2RGB);//��ɫ�ռ�ת��
			//	Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
			//}
			//else//Gray Img
			//{
			//	Img = QImage((const uchar*)(image_for_write.data), image_for_write.cols, image_for_write.rows, image_for_write.cols*image_for_write.channels(), QImage::Format_Indexed8);
			//}
			//ui->label_2->setPixmap(QPixmap::fromImage(Img));
			
			ui->lcdNumber_2->display(++total_number);			
			Config().Set("Count", "Count", total_number);
			//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			//if (!bookdetection(image_for_write))//ʶ���ж�
			//{
			//	ui->label_7->setText("װ������");
			//	Beep(1000, 1000);
			//	cout << "���ϸ�" << endl << endl;
			//	ss << imagefile << "wrong-" << i << ".bmp";
			//	imwrite(ss.str(), image_for_write);
			//	ui->lcdNumber_3->display(++sum_of_wrong);
			//	Config().Set("Count", "sum_of_wrong", sum_of_wrong);
			//}
			//else
			//{
			//	ui->label_7->setText("װ����ȷ��");
			//	ui->lcdNumber->display(++sum_of_correct);
			//	Config().Set("Count", "sum_of_correct", sum_of_correct);
			//}
			//==============================================
			//detector->detect(infile, 0.9);
#ifdef OPENCV
			cv::Mat mat_img = cv::imread(infile);
			mat_img = mat_img(rect_of_image);
			std::vector<bbox_t> result_vec = detector->detect(mat_img,0.9);
			draw_boxes(mat_img, result_vec);
#else
			std::vector<bbox_t> result_vec = detector->detect(ss.str());
#endif
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
			int this_block_size = result_vec.size();
			sort(result_vec.begin(), result_vec.end(), sortFun);
			vector<Point> points;
			int max_point_x = 0, max_point_y = 0;
			double average_piexl_value = 100;
			//forѭ����ȡÿ���ڿ���е㣬���洢��points��
			ui->label_3->setText(QString::number(this_block_size));

			//for (int i = 0; i < this_block_size; i++) {
			//	//rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(255, 178, 50), 2);
			//	points.push_back(Point(result_vec[i].x + 0.5*result_vec[i].w, result_vec[i].y + 0.5*result_vec[i].h));
			//}
			//
			//if (this_block_size > Num_of_blocks)
			//{
			//	ui->label_3->setText("װ������," + QString::number(this_block_size));
			//	
			//}
			//else {
			//	if (this_block_size == Num_of_blocks && LinearFitting(points, k, b, s)) {
			//		ui->lcdNumber->display(++sum_of_correct);
			//		ui->label_3->setText("װ����ȷ");
			//	}
			//	else {
			//		for (size_t i = 0; i < this_block_size - 1; i++) {
			//			if (abs(points[i + 1].x - points[i].x - re_locations[i].x) < 5 &&
			//				abs(points[i].y - points[i + 1].y - re_locations[i].y) < 5 &&
			//				abs((points[i + 1].x - points[i].x) / points[i + 1].y - points[i].y - re_locations[i].k) < 0.01)
			//				continue;
			//			else
			//			{
			//				int x = points[i].x + re_locations[i].x;
			//				int y = points[i].y + re_locations[i].y;
			//				//ѭ���������úڿ��е��������ؼ���ƽ�ֻҶ�ֵ
			//				int value = 0;
			//				Mat imagefile1 = imread(ss.str());
			//				for (int i = x; i < ab_locations[i].width; ++i) 
			//					for (int j = y; j < ab_locations[i].height; ++j) 
			//						value += imagefile1.at<uchar>(i, j);
			//				value = value / (ab_locations[i].width*ab_locations[i].height);
			//				if (value < average_piexl_value)
			//				{
			//					continue;
			//				}
			//				else
			//				{
			//					cout << "ȱʧ��λ����������!" << endl;
			//					break;
			//				}
			//			}
			//		}
			//	}
			//}

			ui->lcdNumber_3->display(++sum_of_wrong);
			endTime = clock();
			string s = "The run time is: " + to_string((double)(endTime - startTime1) / CLOCKS_PER_SEC) + "s";
			QString st = QString::fromStdString(s);
			ui->label_7->setText(st);
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
//QImage��OpenCV Mat�໥ת������
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
//������Ϻ���
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
	ofstream outFile;
	outFile.open("LinearFitting.csv", ios::app); // ��ģʽ��ʡ��,ʹ�õ�app׷��ģʽ
	outFile << get_datetime() << ',' << length << ',' << slope1 << ',' << intercept1 << ',' << r_square1 << endl;
	outFile.close();
	//�ж�����ֻ��������ϵ����б��
	if (abs(r_square - r_square1) <= 0.1 && abs(slope - slope1) <= 0.1) {
		cout << "���ϵ����ȷ" << endl;
		QString str = QString::number(length) + " y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + " Correct";
		ui->label_3->setText(str);
		return true;
	}
	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | BACKGROUND_RED);
		cout << "���ϵ������" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		QString str = "y=" + QString::number(slope) + "x+ " + QString::number(intercept1) + "  r_square1 is " + QString::number(r_square1) + " Wrong";
		ui->label_3->setText(str);
		return false;
	}

}
//��������-ʱ���ַ���
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

//-------�����ĸ�������opencv�и�����YOLOģ��ʶ��Ĵ��룬���иĶ�--------
//����������
bool MainWindow::bookdetection(Mat imagefile) {
	
	//Config().Set("Log", "Function BookDetection", "BookDetectionִ��");
	//string outfile = "E:\\pic\\label\\" + get_datetime() + ".bmp";
	String modelConfiguration = "D:/yolov3.cfg";
	String model_label_Weights = "D:/yolov3_final.weights";
	//ʶ���ɫ��־�����غ�ɫ����ľ�����Ϣ������boxes��
	vector<Rect> boxes = detect_image(imagefile, model_label_Weights, modelConfiguration);
	Config().Set("Log", "Function detect_image return", "detect_image ���سɹ�");
	//���򣬸��ݵõ��ķ�����е��������������򣬰���y��С�����˳����
	//sort(boxes.begin(), boxes.end(), sortFun);
	vector<Point> points;
	int max_point_x = 0, max_point_y = 0;
	double average_piexl_value = 0;
	//forѭ����ȡÿ���ڿ���е㣬���洢��points��
	for (int i = 0; i < boxes.size(); i++) {
		points.push_back(Point(boxes[i].x + 0.5*boxes[i].width, boxes[i].y + 0.5*boxes[i].height));
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

	//string outfile;
	//switch (this_block_nums)
	//{
	//case 11:
	//	cout << "��ɫ��־��������ȷ" << endl;
	//	//imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "����");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points,0.403, -359, 0.985);
	//case 12:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "��ɫ��־����Ϊ12" << endl;
	//	outfile = "E:\\20200511\\12-" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "����");
	//	//emit SendUpdateLCDMsg(1);
	//	return LinearFitting(points, k, b, s);
	//case 10:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_RED);
	//	cout << "��ɫ��־����Ϊ10" << endl;
	//	outfile = "E:\\20200511\\��" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "�쳣");
	//	//emit SendUpdateLCDMsg(2);
	//	return LinearFitting(points, k,b,s) && 0;
	//default:
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	cout << "��ɫ��־��������  " << this_block_nums << endl;
	//	outfile = "E:\\20200511\\����" + get_datetime() + ".bmp";
	//	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	//	imwrite(outfile, imagefile);
	//	//cout << "�ļ�д�룺" + outfile << endl;
	//	run_database(get_datetime(), "�쳣");
	//	ui->label_3->setText("Wrong");
	//	//emit SendUpdateLCDMsg(2);
	//	return 0;
	//}
	//return false;
}

vector<Rect> MainWindow::detect_image(Mat frame, string modelWeights, string modelConfiguration) {
	Config().Set("Log", "Function detect_image", "detect_imageִ��");
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
	Config().Set("Log", "Function detect_image", "detect_imageִ�гɹ�");
	//cv::waitKey(30);
}

vector<Rect> MainWindow::postprocess_return(Mat& frame, const vector<Mat>& outs)
{
	Config().Set("Log", "Function postprocess_return", "postprocess_returnִ��");
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

	//label2 ��ʾ��ǵ�ͼ��
	ui->label_2->setPixmap(QPixmap::fromImage(cvMat2QImage(frame)));
	Config().Set("Log", "Function postprocess_return", "postprocess_returnִ�гɹ�");
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
	Config().Set("Log", "Function getOutputsNames", "getOutputsNamesִ�гɹ�");
	return names;
}
//-----------------------------------

//����ʶ�𵽵ĺڿ��λ������

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
//����ִ��exe�ļ�
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
//�������ݵ����ݿ⡣�ƺ���������
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
//��labelImg�����������
void MainWindow::run_labelImg()
{
	QProcess *pro = new QProcess;
	pro->startDetached("labelImg.exe", QStringList());
	
	//SHELLEXECUTEINFO  ShExecInfo;
	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = NULL;
	//ShExecInfo.hwnd = NULL;
	//ShExecInfo.lpVerb = NULL;
	//ShExecInfo.lpFile = _T("labelImg.exe");        // ִ�еĳ����� 
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_MAXIMIZE;                // ȫ����ʾ������� 
	//ShExecInfo.hInstApp = NULL;
	//ShellExecuteEx(&ShExecInfo);

	//SHELLEXECUTEINFO ShExecInfo = { 0 };
	//ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	//ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	//ShExecInfo.hwnd = NULL;
	//ShExecInfo.lpVerb = NULL;
	//ShExecInfo.lpFile = _T("labelImg.exe");
	//ShExecInfo.lpParameters = NULL;
	//ShExecInfo.lpDirectory = NULL;
	//ShExecInfo.nShow = SW_SHOW;
	//ShExecInfo.hInstApp = NULL;
	//ShellExecuteEx(&ShExecInfo);
	
}
//ģ��ѵ�������еĲü�ͼƬ
void MainWindow::run_cut()
{
	// database.exe 2016-01-22 08:45:50 �쳣
	//string filename = "labelImg.exe";
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
	
}
//��ȡģ�壬ʶ�𻭵Ŀ���mask_boundingRectһ����
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
//��ģ���еľ�����ȡ��������ȡ����
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
//ѵ��ģ��
void MainWindow::run_train() {
	string params = "darknet.exe detector train cfg/voc.data cfg/yolov3-voc.cfg yolov3-voc_last.weights";
	string filename = "darknet.exe";
	run_exe(filename, params);
}
//�������Ƿ�����
void MainWindow::CameraCheck(void)
{
	CSystem &systemObj = CSystem::getInstance();

	bool bRet = systemObj.discovery(m_vCameraPtrList);
	if (false == bRet)
	{
		printf("discovery fail.\n");
		return;
	}

	//// ��ӡ���������Ϣ��key, ��������Ϣ, �ͺ�, ���кţ�
	//for (int i = 0; i < m_vCameraPtrList.size(); i++)
	//{
	//	ICameraPtr cameraSptr = m_vCameraPtrList[i];
	//	printf("Camera[%d] Info :\n", i);
	//	printf("    key           = [%s]\n", cameraSptr->getKey());
	//	printf("    vendor name   = [%s]\n", cameraSptr->getVendorName());
	//	printf("    model         = [%s]\n", cameraSptr->getModelName());
	//	printf("    serial number = [%s]\n", cameraSptr->getSerialNumber());
	//}

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

// ִ��һ������
bool MainWindow::ExecuteSoftTrig(void)
{
	if (NULL == m_pCamera)
	{
		printf("Set GainRaw fail. No camera or camera is not connected.\n");
		return false;
	}

	CCmdNode nodeTriggerSoftware(m_pCamera, "TriggerSoftware");
	if (false == nodeTriggerSoftware.isValid())
	{
		printf("get TriggerSoftware node fail.\n");
		return false;
	}
	if (false == nodeTriggerSoftware.execute())
	{
		printf("set TriggerSoftware fail.\n");
		return false;
	}

	printf("ExecuteSoftTrig success.\n");
	return true;
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
//����ͼƬ�еĲۺ���
void MainWindow::set_Mode_of_trig_soft() {
	Mode_of_trig_soft = true;
}
//�����
void MainWindow::on_pushButton_clicked()
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);
	ui->pushButton_3->setEnabled(true);

	//���߳�����---------------------------------
	//if (subthread->isRunning() == true) 
	//	return;
	//m_MyThread->setFlag(false);
	////start the child thread    
	//subthread->start();
	////�������̣߳����ǲ�û�н����߳�
	////����ͨ���ź�/�۵ķ�ʽ�������߳�
	////ֱ��ͨ��m_MyThread->Mywork()�ǲ��еģ�����MyWork()�е��߳̾������߳�
	//emit StartThread();
	//---------------------------------

	//-------------------------------------------------------------------------------
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

	//if (!bRet || 0 == vCameraPtrList.size())
	//{
	//	QMessageBox::warning(NULL, "warning", "�����豸ʧ��\n", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//	ui->pushButton->setEnabled(true);
	//	ui->pushButton_2->setEnabled(false);
	//	ui->pushButton_3->setEnabled(false);
	//	ui->pushButton_4->setEnabled(false);
	//	return;
	//}
	//else {
	//	try {
	//		CameraCheck();
	//		bool camera_open = CameraOpen();
	//		CameraStart();
	//		ui->label_3->setText("������ӳɹ���");
	//		//SetExposeTime(10000);
	//		//SetAdjustPlus(5);
	//		//CameraChangeTrig(trigLine);
	//		CameraChangeTrig(trigContinous);
	//		CEnumNode nodeEnum(m_pCamera, "TriggerSource");
	//		//CString strTriggerSource;
	//		//nodeEnum.getValueSymbol(strTriggerSource);
	//		nodeEnum.setValueBySymbol("Software");
	//		//ui->label_2->setEnabled(false);
	//		//ui->label_2->setVisible(false);
	//		ui->pushButton_3->setEnabled(true);
	//		ui->pushButton_4->setEnabled(false);
	//	}
	//	catch (Exception e) {
	//		QMessageBox::warning(NULL, "warning in open camera", e.what(), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	//	}
	//}

	//-------------------------------------------------------------------------------
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
//��ʼʶ��
void MainWindow::on_pushButton_3_clicked()
{
	//��ʼ ʶ��
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(false);
	ui->pushButton_3->setEnabled(false);
	ui->pushButton_4->setEnabled(true);
	//�������ΪӲ��������ÿ����һ��ִ��һ���ж�
	CameraChangeTrig(trigLine);	
	ui->label_2->setEnabled(true);
	ui->label_2->setVisible(true);
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
	CEnumNode nodeEnum(m_pCamera, "TriggerSource");
	//CString strTriggerSource;
	//nodeEnum.getValueSymbol(strTriggerSource);
	nodeEnum.setValueBySymbol("Software");
}
//��������Ƭ
void MainWindow::on_actionSavePic_triggered()
{
	//�����	/*	����Ϊ���������ʽ������������	����һ����ť��Ȼ�������գ�	*/
	//�����豸
	CSystem &systemObj = CSystem::getInstance();
	TVector<ICameraPtr> vCameraPtrList;
	bool bRet = systemObj.discovery(vCameraPtrList);
	if (!bRet && 0 == vCameraPtrList.size() && !m_pCamera->connect())
	{
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
    QProcess *pro = new QProcess;
    pro->startDetached("labelImg.exe", QStringList());
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
	/* 
	QtУ��ģ���ļ�hashֵ�ж��Ƿ���� https://blog.csdn.net/emdfans/article/details/23871741 
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
			rect_mask.height = abs(img.rows - rect_mask.y);
		}
		Config().Set("Image_Rect", "x", rect_mask.x);
		Config().Set("Image_Rect", "y", rect_mask.y);
		Config().Set("Image_Rect", "width", rect_mask.width);
		Config().Set("Image_Rect", "height", rect_mask.height);

		img = img(rect_mask);
		cv::namedWindow("�ü����ͼ��", CV_WINDOW_NORMAL);
		cv::imshow("�ü����ͼ��", img);
		imwrite("Train/image/Pic.bmp", img);
	}
	else
	{
		QMessageBox::warning(NULL, "ͼ��ü�����", "ͼ��δִ�вü����������������");
		ui->label_3->setText("����δִ�вü����������������");
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
	//	Config().Set("Image_Rect", "Image_Rect", "1236");
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

void MainWindow::on_actionCut_triggered()
{
	on_actionOpenCutWindow_triggered();
}
//ʶ�������д��config�ļ���
void MainWindow::on_actionGetParemeter_triggered()
{
	try
	{
		//Mat img = imread("Train/image/Pic.bmp");
		////img = img(rect_of_image);
		//String modelConfiguration = "D:/yolov3.cfg";
		//String model_label_Weights = "D:/yolov3_final.weights";

		//vector<Rect> boxes = detect_image(img, model_label_Weights, modelConfiguration);

		std::vector<bbox_t> boxes = detector->detect("Train/image/Pic.bmp");
		//���򣬸��ݵõ��ķ�����е��������������򣬰���y��С�����˳����
		sort(boxes.begin(), boxes.end(), sortFun);
		vector<Point> points;

		//forѭ����ȡÿ���ڿ���е㣬���洢��points��
		for (int i = 0; i < boxes.size(); i++) {
			points.push_back(Point(boxes[i].x + 0.5*boxes[i].w, boxes[i].y + 0.5*boxes[i].h));
			//ab_locations.push_back(Rect(boxes[i].x, boxes[i].y, boxes[i].w, boxes[i].h));
		}
		relative_location r;
		for (int i = 1; i < points.size(); i++) {
			int x = points[i].x - points[i-1].x;
			int y = points[i].y - points[i - 1].y;
			double k = y / x;
			r.x = x;
			r.y = y;
			r.k = k;
			QString s = QString::number(x) + " " + QString::number(y) + " " + QString::number(k);
			re_locations.push_back(r);
			Config().Set("relative_location", QString::number(i), s);
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
		Config().Set("Line_Fitting", "k", slope1);
		Config().Set("Line_Fitting", "b", intercept1);
		Config().Set("Line_Fitting", "s", r_square1);
		Config().Set("Line_Fitting", "n", length);

		//�Ѳ������µ������Ա�����
		Num_of_blocks = length;
		k = slope1;
		b = intercept1;
		s = r_square1;
		ui->label_3->setText("����ʶ����ϣ�");

		QMessageBox::information(NULL, "����ʶ��", "����ʶ����ɣ�");
	}
	catch (const std::exception& e)
	{
		QMessageBox::warning(NULL, "����ʶ�����", e.what());
		return;
	}	
}

void MainWindow::on_actiontakephoto_triggered()
{
     on_actionSavePic_triggered();
}

//��С���������غ�������----------------------
//����ο�����https://www.jianshu.com/p/2e7a5347b56c
//�����������ｫ������ϳɶ�������
int MainWindow::polyfit(vector<Point> points)
{
	double coefficient[5];
	memset(coefficient, 0, sizeof(double) * Num_of_blocks);
	vector<double> vx, vy;
	for (int i = 0; i<points.size(); i++)
	{
		vx.push_back(points[i].x);
		vy.push_back(points[i].y);
	}
	EMatrix(vx, vy, Num_of_blocks, 3, coefficient);
	printf("��Ϸ���Ϊ��y = %lf + %lfx + %lfx^2 \n", coefficient[1], coefficient[2], coefficient[3]);
	return 0;
}
//�ۼ�
double MainWindow::sum(vector<double> Vnum, int n)
{
	double dsum = 0;
	for (int i = 0; i<n; i++)
	{
		dsum += Vnum[i];
	}
	return dsum;
}
//�˻���
double MainWindow::MutilSum(vector<double> Vx, vector<double> Vy, int n)
{
	double dMultiSum = 0;
	for (int i = 0; i<n; i++)
	{
		dMultiSum += Vx[i] * Vy[i];
	}
	return dMultiSum;
}
//ex�η���
double MainWindow::RelatePow(vector<double> Vx, int n, int ex)
{
	double ReSum = 0;
	for (int i = 0; i<n; i++)
	{
		ReSum += pow(Vx[i], ex);
	}
	return ReSum;
}
//x��ex�η���y�ĳ˻����ۼ�
double MainWindow::RelateMutiXY(vector<double> Vx, vector<double> Vy, int n, int ex)
{
	double dReMultiSum = 0;
	for (int i = 0; i<n; i++)
	{
		dReMultiSum += pow(Vx[i], ex)*Vy[i];
	}
	return dReMultiSum;
}
//���㷽������������
void MainWindow::EMatrix(vector<double> Vx, vector<double> Vy, int n, int ex, double coefficient[])
{
	for (int i = 1; i <= ex; i++)
	{
		for (int j = 1; j <= ex; j++)
		{
			Em[i][j] = RelatePow(Vx, n, i + j - 2);
		}
		Em[i][ex + 1] = RelateMutiXY(Vx, Vy, n, i - 1);
	}
	Em[1][1] = n;
	CalEquation(ex, coefficient);
}
//��ⷽ��
void MainWindow::CalEquation(int exp, double coefficient[])
{
	for (int k = 1; k<exp; k++) //��Ԫ����
	{
		for (int i = k + 1; i<exp + 1; i++)
		{
			double p1 = 0;

			if (Em[k][k] != 0)
				p1 = Em[i][k] / Em[k][k];

			for (int j = k; j<exp + 2; j++)
				Em[i][j] = Em[i][j] - Em[k][j] * p1;
		}
	}
	coefficient[exp] = Em[exp][exp + 1] / Em[exp][exp];
	for (int l = exp - 1; l >= 1; l--)   //�ش����
		coefficient[l] = (Em[l][exp + 1] - F(coefficient, l + 1, exp)) / Em[l][l];
}
//��CalEquation��������
double MainWindow::F(double c[], int l, int m)
{
	double sum = 0;
	for (int i = l; i <= m; i++)
		sum += Em[l - 1][i] * c[i];
	return sum;
}
//ԭ�ģ�https://blog.csdn.net/lsh_2013/article/details/46697625 
//��С���������غ�������----------------------

