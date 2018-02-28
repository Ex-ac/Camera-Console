#ifndef SELECTEDCAMERAWIDGET_H_
#define SELECTEDCAMERAWIDGET_H_

#include <QtWidgets/QWidget>
class QCheckBox;
class QPushButton;
class QGridLayout;
class QGroupBox;

class SelectedCameraWidget : public QWidget
{
	Q_OBJECT
public:
	SelectedCameraWidget(int munberOfCamera, QWidget *parent = nullptr);
	~SelectedCameraWidget()
	{
		//指针已经在topLayout中
		/*qDeleteAll(_checkBoxList);*/
	}
	inline void setNumberOfCamera(int number) { _numberOfCamera = number; reset(); }
	inline int numberOfCamera() { return _numberOfCamera; }
	quint32 flagOfTheSelectedCamera();

	void reset();
public slots:
	void selectedAll();
	void unselectedAll();
private:
	enum
	{
		MaxCountOfColumn = 8
	};
	QList<QCheckBox *> _checkBoxList;
	QPushButton *_selectedAllButton;
	QPushButton *_unselectedAllButton;
	QGridLayout *_topLayout;
	QGroupBox *_groupBox;
	int _numberOfCamera;
};

#endif
