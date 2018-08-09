#include "FrequencyAnalizerIODevice.hpp"
#include <QtCharts/QXYSeries>
FrequencyAnalizerIODevice::FrequencyAnalizerIODevice(
		QXYSeries *series, FT2StreamReader* streamReader, int size, QObject *parent) :
	XYSeriesIODevice(series, streamReader, size, true, parent)
{
streamReader->addListener(this);
}

void FrequencyAnalizerIODevice::showData(quint16 transferredBytes){
	if (m_buffer.isEmpty()) {
		m_buffer.reserve(maxSamples);
		for (int i = 0; i < maxSamples; ++i)
			m_buffer.append(QPointF(i, 0));
	}

	//int resolution = internalBuffer.size()/ maxSamples;
	//resolution/=sampleBitSize;
	int resolution=1;

	//std::vector<short> data[transferredBytes/2];
	auto data = reinterpret_cast< const short*>( internalBuffer.data().data() );
	for(int i=marker;i<transferredBytes/sampleBitSize;i+=resolution){
		//qDebug()<<transferredBytes/sampleBitSize<<"_"<<i;
		m_buffer[i/resolution].setY(((float)data[i])/SHRT_MAX);
		marker++;
	}
	if(marker >= maxSamples)
		marker = 0;
	//m_series->replace(m_buffer);
}
