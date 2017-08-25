#include "SelectedCameraWidget.h"

#include <QList>
#include <QCheckBox>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>

SelectedCameraWidget::SelectedCameraWidget(int numberOfCamera, QWidget *parent)
	:QWidget(parent), _numberOfCamera(numberOfCamera)
{
	_selectedAllButton = new QPushButton(tr(u8"全选"));
	_selectedAllButton->setDefault(true);
	connect(_selectedAllButton, SIGNAL(clicked()), this, SLOT(selectedAll()));

	_unselectedAllButton = new QPushButton(tr(u8"清除"));
	connect(_unselectedAllButton, SIGNAL(clicked()), this, SLOT(unselectedAll()));

	_topLayout = new QGridLayout;

	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(_selectedAllButton);
	bottomLayout->addWidget(_unselectedAllButton);
	bottomLayout->addStretch();

	QVBoxLayout *groupLayout = new QVBoxLayout;

	_groupBox = new QGroupBox;


	groupLayout->addLayout(_topLayout);
	groupLayout->addLayout(bottomLayout);
	_groupBox->setLayout(groupLayout);
	_groupBox->setTitle(tr(u8"选择相机--相机数量%1").arg(_numberOfCamera));

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(_groupBox);
	setLayout(mainLayout);

	reset();
}

void SelectedCameraWidget::reset()
{
	qDeleteAll(_checkBoxList);
	_checkBoxList.clear();
	QCheckBox *temp;
	int row = 0, column = 0;
	for (int i = 0; i < _numberOfCamera; ++i)
	{
		temp = new QCheckBox(tr(u8"%1").arg(i + 1));
		_checkBoxList.append(temp);
		_topLayout->addWidget(temp, row, column);
		if (++column == MaxCountOfColumn)
		{
			++row;
			column = 0;
		}
	}
	_groupBox->setTitle(tr(u8"选择相机--相机数量%1").arg(_numberOfCamera));

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