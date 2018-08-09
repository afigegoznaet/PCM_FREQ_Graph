#ifndef FREQUENCYANALIZERIODEVICE_HPP
#define FREQUENCYANALIZERIODEVICE_HPP

#include "xyseriesiodevice.hpp"
class FrequencyAnalizerIODevice : public XYSeriesIODevice
{
	Q_OBJECT
public:
	explicit FrequencyAnalizerIODevice(
			QXYSeries *series, FT2StreamReader* streamReader,
			int size = 4000, QObject *parent = nullptr);
public slots:
	void showData(quint16) override;
};

#endif // FREQUENCYANALIZERIODEVICE_HPP
