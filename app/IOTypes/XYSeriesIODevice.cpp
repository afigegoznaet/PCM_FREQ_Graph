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

#include "XYSeriesIODevice.hpp"

#include <QtCharts/QXYSeries>

XYSeriesIODevice::XYSeriesIODevice(QXYSeries *series, FT2StreamReader * streamReader,
								   int size, bool base, QObject *parent) :
	FT2StreamConsumer(streamReader, true, parent),
	m_series(series),
	maxSamples(size)
{
	if(!base)
		streamReader->addListener(this);
}

void XYSeriesIODevice::showData(quint16 transferredBytes){
	if (m_buffer.isEmpty()) {
		m_buffer.reserve(maxSamples);
		for (int i = 0; i < maxSamples; ++i)
			m_buffer.append(QPointF(i, 0));
	}

	int resolution = internalBuffer.size()/ maxSamples;
	resolution/=sampleBitSize;

	//std::vector<short> data[transferredBytes/2];
	auto data = reinterpret_cast< const short*>( internalBuffer.data().data() );
	for(int i=marker;i<transferredBytes/sampleBitSize;i+=resolution){
		//qDebug()<<transferredBytes/sampleBitSize<<"_"<<i;
		m_buffer[i/resolution].setY(((float)data[i])/SHRT_MAX);
		marker++;
	}
	if(marker >= maxSamples)
		marker = 0;
	m_series->replace(m_buffer);
}
