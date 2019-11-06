#ifndef DIALOGSETCALIBRATION_H
#define DIALOGSETCALIBRATION_H

#include <QDialog>
#include <QMouseEvent>
#include <QStringList>
#include <QStringListModel>


#include "cmat2qimage.h"

//API
#include "EnsembleAPI.h"

#include "pugixml.hpp"


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

	QStringListModel *m_calibration_copy_list_model ;
		
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

	void OnButtonBrightnessGet(void) ;
	void OnButtonSharpnessGet(void) ;
	void OnButtonContrastGet(void) ;
	void OnButtonISOGet(void) ;
	void OnButtonShutterSpeedGet(void) ;
	
	void OnButtonExposureSet(void) ;
	void OnButtonGainSet(void) ;
	void OnButtonFocusSet(void) ;

	void OnButtonBrightnessSet(void) ;
	void OnButtonSharpnessSet(void) ;
	void OnButtonContrastSet(void) ;
	void OnButtonISOSet(void) ;
	void OnButtonShutterSpeedSet(void) ;

	//slider
	void OnSliderSetExposure(void) ;
	void OnSliderSetGain(void) ;
	void OnSliderSetFocus(void) ;
	void OnSliderSetBrightness(void) ;
	void OnSliderSetSharpness(void) ;
	void OnSliderSetContrast(void) ;
	void OnSliderSetISO(void) ;
	void OnSliderSetShutterSpeed(void) ;

	void OnButtonCalibrationCopyListUpdate(void) ;


};

#endif // DIALOGSETCALIBRATION_H
