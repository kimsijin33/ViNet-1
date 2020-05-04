#include "formjobtool.h"
#include "ui_formjobtool.h"

FormJobTool::FormJobTool(QWidget *parent) :
    QWidget(parent),
    m_type(-1),
    m_i_option_count(0) ,
    ui(new Ui::FormJobTool)
{
	setAttribute(Qt::WA_Hover);

    ui->setupUi(this);

    ui->pushButton_change->setVisible(false);	
	ui->pushButton_del->setVisible(false);
	ui->pushButton_option->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
	
	//button
    connect(ui->pushButton_del, SIGNAL(clicked()), this,  SLOT(OnButtonDel())) ;
    connect(ui->pushButton_change, SIGNAL(clicked()), this,  SLOT(OnButtonSetBase())) ;
	connect(ui->pushButton_option, SIGNAL(clicked()), this,  SLOT(OnButtonSetOption())) ;

    connect(&m_dlg_set_tool_object, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_object, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

    //connect(&m_dlg_set_tool_crack, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//line
	connect(&m_dlg_set_tool_line, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_line, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//circle
	connect(&m_dlg_set_tool_circle, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_circle, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//code
	connect(&m_dlg_set_code, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;	
	connect(&m_dlg_set_code, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	
	//distance
	connect(&m_dlg_set_tool_distance, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//offset distance
	connect(&m_dlg_set_tool_offset_distance, SIGNAL(UpdateToolObjectImage()), this, SLOT(OnUpdateImage())) ;
	connect(&m_dlg_set_tool_offset_distance, SIGNAL(UpdateToolName(QString)), this, SLOT(OnUpdateName(QString))) ;

	//update list
	connect(&m_dlg_select_option, SIGNAL(UpdateList()), this, SLOT(OnUpdateList())) ;

	
	//check box
	connect(ui->checkBox_run, SIGNAL(clicked(bool)), this, SLOT(OnRunCheckBoxToggled(bool)));
	connect(ui->checkBox_view, SIGNAL(clicked(bool)), this, SLOT(OnViewCheckBoxToggled(bool)));

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), 
	        this, SLOT(ShowContextMenu(const QPoint &)));
}

FormJobTool::~FormJobTool()
{
    delete ui;
}


void FormJobTool::ShowContextMenu(const QPoint &pos) 
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
		QMenu contextMenu(tr("Context menu"), this);

		//Get Sub Job List
		const std::string str_addable_job_list_xml = p_device->Ensemble_Info_Get_Addable_Subjob_List_Xml(GetIdInfo()) ;

		//parsing
		CParsingAddableJobList cls_parsing_addable_job_list ;
		std::vector<AddableJobInfo> vec_list = cls_parsing_addable_job_list.GetAddableJobList(str_addable_job_list_xml) ;

        const int size_list = vec_list.size() ;
		for( int i=0 ; i<size_list ; i++ )
		{
            std::string str_menu ;
            str_menu = "New " + vec_list[i].name ;
			if( !vec_list[i].description.empty() )	str_menu += "(" + vec_list[i].description + ")" ;
			
            //QAction action1(str_name.c_str(), this);
            //connect(&action1, SIGNAL(triggered()), this, SLOT(removeDataPoint()));

            //contextMenu.addAction(&action1);
            contextMenu.addAction(str_menu.c_str());
            //contextMenu.exec(mapToGlobal(pos));
        }

        QAction* selectedItem = contextMenu.exec(mapToGlobal(pos));
        if( selectedItem )
        {

        }
	}
}


void FormJobTool::showEvent(QShowEvent *ev)
{
    QWidget::showEvent(ev) ;

	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
	    //Set Initialize
	    //Get Option Count
	    m_i_option_count = p_device->Ensemble_Tool_Get_OptionList_Count(GetType()) ;

		OnUpdateImage() ;

		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);
	}
}

void FormJobTool::OnButtonSetOption(void) 
{
	if( m_i_option_count > 0 )
	{
		m_dlg_select_option.SetToolType(GetType()) ;
		m_dlg_select_option.SetToolId(GetIdInfo()) ;

		int dialogCode = m_dlg_select_option.exec();

		//Update
	    if(dialogCode == QDialog::Accepted)
	    {
	    	OnUpdateList();
	    }
	}
}

void FormJobTool::OnButtonSetBase(void)
{
    int type = GetType() ;

	qDebug("%s : type=%d", __func__, type) ;
	
    if( type == ToolTypeList::TOOL_TYPE_DETECT_OBJECT )		//obejct
	{
		qDebug("Object Set") ;
		
	    //DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_object.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_object.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_object.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    if( type == ToolTypeList::TOOL_TYPE_DETECT_LINE )		//line
	{	
		qDebug("Line Set") ;
		
	    m_dlg_set_tool_line.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_line.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_line.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_DETECT_CIRCLE )		//circle
   	{
   		qDebug("Circle Set") ;
		
	    m_dlg_set_tool_circle.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_circle.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_circle.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_DETECT_CODE )		//circle
   	{
   		qDebug("Code Set") ;
		
	    m_dlg_set_code.SetParentId(GetParentIdInfo());
	    m_dlg_set_code.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_code.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_CALC_DISTANCE )
	{
		qDebug("Distance Set") ;
		
	    m_dlg_set_tool_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_CALC_ANGLE )
	{
		qDebug("Angle Set") ;
		
	    m_dlg_set_tool_angle.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_angle.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_angle.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
	else if( type == ToolTypeList::TOOL_TYPE_OFFSET_DISTANCE )
	{
		qDebug("Offset Distance Set") ;
		
	    m_dlg_set_tool_offset_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_offset_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_offset_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_CRACK )		//crack
   	{
   		DialogChangeName dlg_change_name ;

	    dlg_change_name.SetId(GetIdInfo());
	    dlg_change_name.SetName(GetNameInfo());

	    int dialogCode = dlg_change_name.exec();

	    if(dialogCode == QDialog::Accepted)
	    { // YesButton clicked
	        std::string change_name = dlg_change_name.GetName() ;

	        qDebug("Project Change Name = %s", change_name.c_str()) ;

			CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

			if( p_device )
			{
		        if( !change_name.empty() )
		        {
		            p_device->Ensemble_Tool_Set_Name(GetIdInfo(), change_name) ;
		        }

		        std::string project_name = p_device->Ensemble_Tool_Get_Name(GetIdInfo()) ;
		        ui->label_name->setText(QString::fromUtf8(project_name.c_str()));

		        qDebug("Project Name = %s", project_name.c_str()) ;
			}
	    }
	}
	
#if 0
    else if( type == ToolTypeList::TOOL_TYPE_INSPECT_DISTANCE )
	{
		qDebug("Inspect Distance Set") ;

		m_dlg_set_tool_inspect_distance.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_inspect_distance.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_inspect_distance.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
    else if( type == ToolTypeList::TOOL_TYPE_INSPECT_CRACK )		//crack
   	{
   		qDebug("Crack Set") ;
		
   		//DialogSetBase dlg_setbase ;
	    m_dlg_set_tool_crack.SetParentId(GetParentIdInfo());
	    m_dlg_set_tool_crack.SetToolId(GetIdInfo());

	    int dialogCode = m_dlg_set_tool_crack.exec();

	    if(dialogCode == QDialog::Accepted)
	    {
	    }
	}
#endif
}

void FormJobTool::OnUpdateList(void)
{
	emit UpdateList();
}

void FormJobTool::OnButtonDel(void)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
	    p_device->Ensemble_Tool_Del(GetIdInfo()) ;
	}
	
	OnUpdateList();
}


void FormJobTool::SetNameInfo(const std::string name)
{
    ui->label_name->setText(QString::fromUtf8(name.c_str()));
}

std::string FormJobTool::GetNameInfo(void)
{
    QString name = ui->label_name->text() ;

    std::string str_name = name.toUtf8().constData();

    return str_name ;
}

void FormJobTool::SetIdInfo(const std::string id)
{
    ui->label_id->setText(QString::fromUtf8(id.c_str()));
}

std::string FormJobTool::GetIdInfo(void)
{
    QString id = ui->label_id->text() ;

    std::string str_id = id.toUtf8().constData();

    return str_id ;
}

void FormJobTool::SetParentIdInfo(const std::string id)
{
	m_str_parent_id = id ;
}

std::string FormJobTool::GetParentIdInfo(void)
{
	return m_str_parent_id ;
}

void FormJobTool::SetTypeInfo(const std::string type)
{
    ui->label_type->setText(QString::fromUtf8(type.c_str()));
}

std::string FormJobTool::GetTypeInfo(void)
{
    QString type = ui->label_type->text() ;

    std::string str_type = type.toUtf8().constData();

    return str_type ;
}

void FormJobTool::SetType(const int type)
{
	m_type = type ;
}

int FormJobTool::GetType(void)
{
	return m_type ;
}

bool FormJobTool::event(QEvent * e)
{
    switch(e->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(e));
        return true;
        break;
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(e));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(e);
}

void FormJobTool::enterEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}

void FormJobTool::leaveEvent(QEvent * e)
{
    //qDebug() << Q_FUNC_INFO << e->type();
}


void FormJobTool::hoverEnter(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(true);
	ui->pushButton_del->setVisible(true);
	if( m_i_option_count > 0 )	ui->pushButton_option->setVisible(true);

	//ui->checkBox_run->setVisible(true);
	//ui->checkBox_view->setVisible(true);

    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobTool::hoverLeave(QHoverEvent * event)
{
    ui->pushButton_change->setVisible(false);
	ui->pushButton_del->setVisible(false);
	if( m_i_option_count > 0 )	ui->pushButton_option->setVisible(false);

	//ui->checkBox_run->setVisible(false);
	//ui->checkBox_view->setVisible(false);
	
    //qDebug() << Q_FUNC_INFO << event->type();
}

void FormJobTool::hoverMove(QHoverEvent * event)
{
    //qDebug() << Q_FUNC_INFO << event->type() ;
}

void FormJobTool::OnUpdateImage(void)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
		//Get Base Object Image
	    //unsigned char* get_object_image_data = NULL ;
	    //int object_image_width = 0 ;
	    //int object_image_height = 0 ;
	    ImageBuf image_buf ;
	    image_buf.image_width = 0 ;
	    image_buf.image_height = 0 ;

		const int image_type = IMAGE_RGB888 ;
	    //int get_image_type = 0 ;
	    p_device->Ensemble_Tool_Get_ObjectImage(GetIdInfo(), image_type+IMAGE_ICON, &image_buf)  ;

		CImageBuf2Mat cls_imagebuf2mat ;
		cv::Mat object_image = cls_imagebuf2mat.Cvt(image_buf) ;

	    if( image_buf.p_buf != NULL )
	    {
	        delete [] image_buf.p_buf ;
	        image_buf.p_buf = NULL ;
	    }
		
		SetObjectImage(object_image) ;
	}
	
}

void FormJobTool::SetObjectImage(cv::Mat image)
{
	CMat2QImage cls_mat_2_qimage ;
	QImage qt_display_image = cls_mat_2_qimage.cvtMat2QImage(image, ui->label_image->width(), ui->label_image->height()) ;
	
    ui->label_image->setPixmap(QPixmap::fromImage(qt_display_image));
}

void FormJobTool::OnUpdateName(QString name)
{
	std::string str_name = name.toUtf8().constData();
	
	SetNameInfo(str_name) ;
}

void FormJobTool::OnRunCheckBoxToggled(bool checked)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
		p_device->Ensemble_Task_Set_Run_Option(GetIdInfo(), checked) ;
			
		//run checkbox
		int run_option = p_device->Ensemble_Task_Get_Run_Option(GetIdInfo()) ;
		ui->checkBox_run->setChecked(run_option);

		QString qstr_id = QString::fromStdString(GetParentIdInfo());
		emit UpdateResultImage(qstr_id) ;
	}
}

void FormJobTool::OnViewCheckBoxToggled(bool checked)
{
	CEnsembleAPI *p_device = CEnsemble::getInstance()->GetDevice(GetNetworkInfo_Ip_Address(), GetNetworkInfo_Port()) ;

	if( p_device )
	{
		p_device->Ensemble_Task_Set_View_Option(GetIdInfo(), checked) ;
			
		//view checkbox
		int view_option = p_device->Ensemble_Task_Get_View_Option(GetIdInfo()) ;
		ui->checkBox_view->setChecked(view_option);

		QString qstr_id = QString::fromStdString(GetParentIdInfo());
		emit UpdateResultImage(qstr_id) ;
	}
}


void FormJobTool::SetAlarm(const bool b_on_off)
{
}

