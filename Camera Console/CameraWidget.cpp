#include "CameraWidget.h"

#include <QLabel>
#include <QPalette>
#include <QLayout>
#include <QPixmap>

CameraWidget::CameraWidget(int id, QWidget *parent)
	: QWidget(parent), _id(id)
{
	this->setWindowTitle(tr(u8"Па»ъ%1").arg(_id));
	

	QPalette generalLabelPalette;


	_zoomTitleLabel = new QLabel(tr(u8"hhhh"));
	_zoomTitleLabel->setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground, false);

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(_zoomTitleLabel);


	setAutoFillBackground(true);
	setLayout(mainLayout);
	
}

void CameraWidget::setImage(const QImage & image)
{
	QPalette backgroundPalette = this->palette();
	QPixmap pixmap = QPixmap(":/CameraConsole/Ex.jpg").scaled(this->size(), Qt::AspectRatioMode::KeepAspectRatioByExpanding);

	backgroundPalette.setBrush(QPalette::Background, pixmap);
	setPalette(backgroundPalette);
	
}
