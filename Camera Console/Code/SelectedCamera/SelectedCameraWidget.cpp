#include "SelectedCameraWidget.h"

#include <QList>
#include <QCheckBox>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>

SelectedCameraWidget::SelectedCameraWidget(int numberOfCamera, QWidget *parent)
	:_numberOfCamera(numberOfCamera)
{
	_selectedAllButton = new QPushButton(tr(u8"全选"));
	_selectedAllButton->setDefault(true);
	connect(_selectedAllButton, SIGNAL(clicked()), this, SLOT(selectedAll()));

	_unselectedAllButton = new QPushButton(tr(u8"清除"));
	connect(_unselectedAllButton, SIGNAL(clicked()), this, SLOT(unselectedAll()));

	topLayout = new QGridLayout;
	reset();
}

void SelectedCameraWidget::reset()
{
	qDeleteAll(_checkBoxList);
	QCheckBox *temp;
	int row = 0, column = 0;
	for (int i = 0; i < _numberOfCamera; ++i)
	{
		temp = new QCheckBox(tr(u8"%1").arg(i + 1));
		_checkBoxList.append(temp);
		topLayout->addWidget(temp, row, column);
		if (++column == MaxCountOfColumn)
		{
			++row;
			column = 0;
		}
	}
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(_selectedAllButton);
	bottomLayout->addWidget(_unselectedAllButton);
	bottomLayout->addStretch();

	QVBoxLayout *groupLayout = new QVBoxLayout;

	QGroupBox *groupBox = new QGroupBox;


	groupLayout->addLayout(topLayout);
	groupLayout->addLayout(bottomLayout);
	groupBox->setLayout(groupLayout);
	groupBox->setTitle(tr(u8"选择相机"));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(groupBox);
	setLayout(mainLayout);
}

quint32 SelectedCameraWidget::flagOfTheSelectedCamera()
{
	quint32 temp = 0x00000000;
	for (int i = 0; i < _checkBoxList.count(); ++i)
	{
		if (_checkBoxList[i]->isChecked())
		{
			temp |= (0x01 << i);
		}
	}
	return temp;
}

void SelectedCameraWidget::selectedAll()
{
	for (auto i : _checkBoxList)
	{
		i->setChecked(true);
	}
}

void SelectedCameraWidget::unselectedAll()
{
	for (auto i : _checkBoxList)
	{
		i->setChecked(false);
	}
}