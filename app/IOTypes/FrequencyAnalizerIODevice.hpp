#ifndef FREQUENCYANALIZERIODEVICE_HPP
#define FREQUENCYANALIZERIODEVICE_HPP

#include "XYSeriesIODevice.hpp"
#include "FrequencySpectrum.hpp"
#include "../3rdparty/fftreal/FFTRealFixLenParam.h"

enum WindowFunction {
	NoWindow,
	HannWindow
};

const WindowFunction DefaultWindowFunction = HannWindow;


template<int N> class PowerOfTwo
{ public: static const int Result = PowerOfTwo<N-1>::Result * 2; };


template<> class PowerOfTwo<0>
{ public: static const int Result = 1; };

class FFTRealWrapper;

/**
 * Implementation of the spectrum analysis which can be run in a
 * separate thread.
 */
class SpectrumAnalyserThread : public QObject
{
	Q_OBJECT

public:
	SpectrumAnalyserThread(QObject *parent);
	~SpectrumAnalyserThread();

public slots:
	void setWindowFunction(WindowFunction type);
	void calculateSpectrum(const QByteArray &buffer,
						   int inputFrequency,
						   int bytesPerSample);

signals:
	void calculationComplete(const FrequencySpectrum &spectrum);

private:
	void calculateWindow();

private:
#ifndef DISABLE_FFT
	FFTRealWrapper*                             m_fft;
#endif

	const int                                   m_numSamples;

	WindowFunction                              m_windowFunction;

	typedef FFTRealFixLenParam::DataType        DataType;
	QVector<DataType>                           m_window;
	QVector<DataType>                           m_input;
	QVector<DataType>                           m_output;
	FrequencySpectrum                           m_spectrum;

	QThread*                                    m_thread;
};

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
