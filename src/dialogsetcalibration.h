#ifndef DIALOGSETCALIBRATION_H
#define DIALOGSETCALIBRATION_H

#include <QDialog>
#include <QMouseEvent>

#include "cmat2qimage.h"

//API
#include "EnsembleAPI.h"


namespace Ui {
class DialogSetCalibration;
}

class DialogSetCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetCalibration(QWidget *parent = 0);
    ~DialogSetCalibration();

	void SetId(const std::string id) ;
    std::string GetId(void) ;
	
private:
    Ui::DialogSetCalibration *ui;

	std::string m_str_id ;
	void UpdateDataCalibrationRun(float f_pixel_x, float f_pixel_y) ;
		
protected :
	void showEvent(QShowEvent *ev) override;
	void mousePressEvent(QMouseEvent *event) override;

public slots:
	void OnButtonGetChessInfo(void) ;
	void OnButtonGetCalibrationImage(void) ;
	void OnButtonAddCalibrationInfo(void) ;
	void OnButtonDelCalibrationInfo(void) ;
	void OnButtonClearCalibrationInfo(void) ;
	void OnButtonUpdateCalibrationInfo(void) ;
	void OnButtonCalibrationRun(void) ;

	void OnButtonExposureGet(void) ;
	void OnButtonGainGet(void) ;
	void OnButtonFocusGet(void) ;
	
	void OnButtonExposureSet(void) ;
	void OnButtonGainSet(void) ;
	void OnButtonFocusSet(void) ;

	//slider
	void OnSliderSetExposure(void) ;
    //void OnSliderMoveExposure(int value) ;
	void OnSliderSetGain(void) ;
    //void OnSliderMoveGain(int value) ;
	void OnSliderSetFocus(void) ;
    //void OnSliderMoveFocus(int value) ;


};

#endif // DIALOGSETCALIBRATION_H
