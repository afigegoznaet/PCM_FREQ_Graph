/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef XYSERIESIODEVICE_H
#define XYSERIESIODEVICE_H

#include <QtCore/QIODevice>
#include <QtCore/QPointF>
#include <QtCore/QVector>
#include <QtCharts/QChartGlobal>
#include "FT2StreamReader.hpp"

QT_CHARTS_BEGIN_NAMESPACE
class QXYSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE


class XYSeriesIODevice : public FT2StreamConsumer
{
	Q_OBJECT
public:
	//explicit XYSeriesIODevice(QXYSeries *series, int size = 2000, QObject *parent = nullptr);
	explicit XYSeriesIODevice(QXYSeries *series, FT2StreamReader* streamReader, int size = 2000, QObject *parent = nullptr);
	bool open(OpenMode mode) override {
		return FT2StreamConsumer::open(mode);
	}
public:
	void setBitness(int bitsPerSample){
		this->sampleBitSize= bitsPerSample;
	}

public slots:
	void showData(quint16);

protected:
	//qint64 readData(char *data, qint64 maxSize) override;
	//qint64 writeData(const char *data, qint64 maxSize) override;

private:
	int sampleBitSize=2;
	QXYSeries *m_series;
	QVector<QPointF> m_buffer;
	//int sampleCount = 2000;
	int maxSamples = 2000;
	int marker = 0;
};

#endif // XYSERIESIODEVICE_H
