#include "dialogsettooloffsetdistance.h"
#include "ui_dialogsettooloffsetdistance.h"

DialogSetToolOffsetDistance::DialogSetToolOffsetDistance(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetToolOffsetDistance)
{
    ui->setupUi(this);

	//Object
	connect(ui->pushButton_select_object, SIGNAL(clicked()), this,	SLOT(OnButtonSelectObject())) ;
	connect(ui->pushButton_reset_object, SIGNAL(clicked()), this,  SLOT(OnButtonResetObject())) ;
	
	//button
	connect(ui->pushButton_get_base_image, SIGNAL(clicked()), this,  SLOT(OnButtonGetImage())) ;
    connect(ui->pushButton_name_change, SIGNAL(clicked()), this,  SLOT(OnButtonNameChange())) ;

	//region get/set
	connect(ui->pushButton_region_set, SIGNAL(clicked()), this,  SLOT(OnButtonRegionSet())) ;
	connect(ui->pushButton_region_get, SIGNAL(clicked()), this,  SLOT(OnButtonRegionGet())) ;
	
	//radio button
	connect(ui->radioButton_1, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_2, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_3, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
	connect(ui->radioButton_4, SIGNAL(clicked()), this,  SLOT(OnButtonChangeDirection())) ;
		

    //background color
    ui->label_image_bg->setStyleSheet("QLabel { background-color : black; }");
}

DialogSetToolOffsetDistance::~DialogSetToolOffsetDistance()
{
    delete ui;
}

void DialogSetToolOffsetDistance::SetToolId(const std::string id)
{
	SetId(id) ;
    ui->label_id->setText(QString::fromUtf8(GetId().c_str()));
}

void DialogSetToolOffsetDistance::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev) ;

    //Get Name
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;
    ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

    qDebug("Tool Name = %s", tool_name.c_str()) ;

	int direction = Ensemble_Tool_Offset_Distance_Get_Direction(GetId()) ;

	if( direction == 0 ) 		ui->radioButton_1->setChecked(true) ;
	else if( direction == 1 ) 	ui->radioButton_2->setChecked(true) ;
	else if( direction == 2 ) 	ui->radioButton_3->setChecked(true) ;
	else if( direction == 3 ) 	ui->radioButton_4->setChecked(true) ;

	OnButtonRegionGet() ;
	
	//Image
	OnButtonGetImage() ;
}

void DialogSetToolOffsetDistance::OnButtonGetImage(void)
{
    //Get Base Job Image
    unsigned char* get_job_image_data = NULL ;
    int job_image_width = 0 ;
    int job_image_height = 0 ;

	const int image_type = IMAGE_RGB888 ;
	
    Ensemble_Tool_Get_Image(GetId(), image_type, &get_job_image_data, &job_image_width, &job_image_height)  ;

    if( job_image_width > 0 && job_image_height > 0 )
    {
    	if( image_type == IMAGE_YUV420)
    	{
	        //YUV420
	        cv::Mat get_image(job_image_height + job_image_height / 2, job_image_width, CV_8UC1, get_job_image_data) ;

	        CImgDec cls_image_decoder ;
	        m_image = cls_image_decoder.Decoding(get_image) ;
    	}
		else if( image_type == IMAGE_RGB888 )
		{
			cv::Mat get_image(job_image_height, job_image_width, CV_8UC3, get_job_image_data) ;
			cv::cvtColor(get_image, m_image, cv::COLOR_BGR2RGB) ;
		}

        updatePicture(m_image) ;
    }

    if( get_job_image_data != NULL )
    {
        delete [] get_job_image_data ;
        get_job_image_data = NULL ;
    }
}

void DialogSetToolOffsetDistance::updatePicture(cv::Mat image, cv::Rect rect_user)
{
    const int draw_width = ui->label_image_bg->width();
    const int draw_height = ui->label_image_bg->height();

    float rescale_w = (float)draw_width / (float)image.cols ;
    float rescale_h = (float)draw_height / (float)image.rows ;

    float min_rescale = std::fmin(rescale_w, rescale_h) ;
    if( min_rescale < 1.0 )
    {
        cv::resize(image, image, cv::Size(), min_rescale, min_rescale) ;
    }

    //fit image label by image isze
    int pos_x = (int)((float)ui->label_image_bg->x() + (float)(draw_width - image.cols)/2.0) ;
    int pos_y = (int)((float)ui->label_image_bg->y() + (float)(draw_height - image.rows)/2.0) ;

    ui->label_image->setGeometry(pos_x, pos_y, image.cols, image.rows);

    //QImage qt_display_image = QImage((const unsigned char*)image.data, image.cols, image.rows, QImage::Format_RGB888);
    CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;
	
	//draw set rect
	if( !rect_user.empty() )
	{
	    qDebug("%s : rect(%d,%d,%d,%d", __func__, rect_user.x, rect_user.y, rect_user.width, rect_user.height) ;

		if( rect_user.width > 0 && rect_user.height > 0 )
		{
			QPainter qPainter(&qt_display_image);
			qPainter.setBrush(Qt::NoBrush);
			qPainter.setPen(Qt::red);
			qPainter.drawRect(rect_user.x,rect_user.y,rect_user.width,rect_user.height);
			bool bEnd = qPainter.end();
		}
	}

    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}


void DialogSetToolOffsetDistance::OnButtonNameChange(void)
{
    std::string tool_name = Ensemble_Tool_Get_Name(GetId()) ;

    DialogChangeName dlg_change_name ;

    dlg_change_name.SetId(GetId());
    dlg_change_name.SetName(tool_name);

    int dialogCode = dlg_change_name.exec();

    if(dialogCode == QDialog::Accepted)
    { // YesButton clicked
        std::string change_name = dlg_change_name.GetName() ;

        qDebug("Tool Change Name = %s", change_name.c_str()) ;
		
        if( !change_name.empty() )
        {
            Ensemble_Tool_Set_Name(GetId(), change_name) ;
        }

        tool_name = Ensemble_Tool_Get_Name(GetId()) ;
        ui->label_name->setText(QString::fromUtf8(tool_name.c_str()));

        qDebug("Tool Name = %s", tool_name.c_str()) ;
		
		emit UpdateToolName(QString::fromUtf8(tool_name.c_str())) ;
    }
}

void DialogSetToolOffsetDistance::OnButtonSelectObject(void)
{
	m_cls_set_user_region.SetStatus(SetBaseStatus::SET_OBJECT) ;
}

void DialogSetToolOffsetDistance::OnButtonResetObject(void)
{
	//Ensemble_Job_Del_SelectObject(GetId()) ;
	
    OnButtonGetImage() ;
}

void DialogSetToolOffsetDistance::mousePressEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

    if (event->button() == Qt::LeftButton && m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL ) 
	{
        QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

		cv::Rect rect_user = m_cls_set_user_region.StartSetRegion(point.x(), point.y()) ;

		updatePicture(m_image, rect_user) ;
    }
}

void DialogSetToolOffsetDistance::mouseMoveEvent(QMouseEvent *event)
{
    qDebug("%s - %d", __func__, __LINE__) ;

   if ( m_cls_set_user_region.GetStatus() > SetBaseStatus::NORMAL)
	{
		QPoint point = event->pos() ;
        point.setX(point.x() - ui->label_image->x());
        point.setY(point.y() - ui->label_image->y());

        if( (event->buttons() & Qt::LeftButton) )
        {
            cv::Rect rect_user = m_cls_set_user_region.MoveSetRegion(point.x(), point.y()) ;

            updatePicture(m_image, rect_user) ;
        }
	}
}

void DialogSetToolOffsetDistance::mouseReleaseEvent(QMouseEvent *event)
{
    //if (event->button() == Qt::LeftButton && scribbling) {
    //    drawLineTo(event->pos());
    //    scribbling = false;
    //}

	//Set
	int set_status = m_cls_set_user_region.GetStatus() ;

	qDebug("%s - %d : m_set_status(%d), event->buttons()=%d", __func__, __LINE__, set_status, event->buttons()) ;
	
    if (set_status > SetBaseStatus::NORMAL)
	{
		qDebug("%s - %d", __func__, __LINE__) ;
		
		float f_x = 0.0 ;
		float f_y = 0.0 ;
		float f_w = 0.0 ;
		float f_h = 0.0 ;

        int label_w = ui->label_image->width() ;
        int label_h = ui->label_image->height() ;

		cv::Rect rect_user = m_cls_set_user_region.EndSetRegion() ;
	
        f_x = (float)rect_user.x / (float)label_w ;
        f_y = (float)rect_user.y / (float)label_h ;
        f_w = (float)rect_user.width / (float)label_w ;
        f_h = (float)rect_user.height / (float)label_h ;

		qDebug("%s - %d : m_set_status(%d)", __func__, __LINE__, set_status) ;
		
        if( set_status == SetBaseStatus::SET_AREA )
        {
            //Ensemble_Job_Set_DetectArea(GetId(), f_x, f_y, f_w, f_h) ;

            //emit UpdateToolObjectImage();
        }
        else if( set_status == SetBaseStatus::SET_ZOOM)
        {
            //Ensemble_Job_Set_Zoom(GetId(), f_x, f_y, f_w, f_h) ;
        }
		else if( set_status == SetBaseStatus::SET_MASK)
        {
        	#if 0
            bool b_enable_inside = false ;
            if (ui->checkBox_mask_enable_inside->isChecked())	b_enable_inside = false ;
            else												b_enable_inside = true ;
				
            Ensemble_Tool_Set_MaskArea(GetId(), f_x, f_y, f_w, f_h, b_enable_inside) ;

			emit UpdateToolObjectImage();
			#endif
        }
		else if( set_status == SetBaseStatus::SET_OBJECT)
		{
			//SelectObject
            Ensemble_Tool_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;

			OnButtonRegionGet() ;
			
			emit UpdateToolObjectImage();
		}
		else if( set_status == SetBaseStatus::SET_REF_POINT)
		{
			#if 0
			QPoint point = event->pos() ;
	        point.setX(point.x() - ui->label_image->x());
	        point.setY(point.y() - ui->label_image->y());

			f_x = (float)point.x() / (float)label_w ;
        	f_y = (float)point.y() / (float)label_h ;
		
			//SelectObject
			//Ensemble_Job_Set_SelectObject(GetId(), f_x, f_y, f_w, f_h) ;
            Ensemble_Tool_Set_Ref_Point(GetId(), f_x, f_y) ;

            emit UpdateToolObjectImage();
			#endif
		}
		
        OnButtonGetImage() ;
	}

	updatePicture(m_image) ;
}

void DialogSetToolOffsetDistance::OnButtonChangeDirection(void)
{
	QButtonGroup group;
    QList<QRadioButton *> allButtons = ui->groupBox->findChildren<QRadioButton *>();

    for(int i = 0; i < allButtons.size(); ++i)
    {
        group.addButton(allButtons[i],i);
    }

    int index = group.checkedId() ;

	Ensemble_Tool_Offset_Distance_Set_Direction(GetId(), index) ;

	int direction = Ensemble_Tool_Offset_Distance_Get_Direction(GetId()) ;

	if( direction == 0 ) 		ui->radioButton_1->setChecked(true) ;
	else if( direction == 1 ) 	ui->radioButton_2->setChecked(true) ;
	else if( direction == 2 ) 	ui->radioButton_3->setChecked(true) ;
	else if( direction == 3 ) 	ui->radioButton_4->setChecked(true) ;
	
#if 0
    group.checkedButton()->setChecked(true);

    foreach(QAbstractButton* button, allButtons)
    {
        if (id(button) == _id)
        {
            button->setChecked(true);
            break;
        }
    }
#endif	

	OnButtonGetImage() ;
}

void DialogSetToolOffsetDistance::OnButtonRegionGet(void)
{
	float x=0,y=0,width=0,height=0 ; 
	Ensemble_Tool_Offset_Distance_Get_Region(GetId(), &x, &y, &width, &height) ;

	ui->lineEdit_region_x->setText(QString::number(x));
	ui->lineEdit_region_y->setText(QString::number(y));
	ui->lineEdit_region_width->setText(QString::number(width));
	ui->lineEdit_region_height->setText(QString::number(height));
}

void DialogSetToolOffsetDistance::OnButtonRegionSet(void)
{
	int region_x = ui->lineEdit_region_x->text().toInt() ;
	int region_y = ui->lineEdit_region_y->text().toInt() ;
	int region_width = ui->lineEdit_region_width->text().toInt() ;
	int region_height = ui->lineEdit_region_height->text().toInt() ;

	Ensemble_Tool_Offset_Distance_Set_Region(GetId(), region_x, region_y, region_width, region_height) ;

	OnButtonGetImage() ;
}

