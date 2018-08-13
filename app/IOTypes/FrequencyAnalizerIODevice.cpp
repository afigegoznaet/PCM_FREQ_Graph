#include "FrequencyAnalizerIODevice.hpp"
#include <QtCharts/QXYSeries>
#include "../3rdparty/fftreal/fftreal_wrapper.h"
#include <QtMath>

const qint16  PCMS16MaxValue     =  32767;
const quint16 PCMS16MaxAmplitude =  32768; // because minimum is -32768

qreal pcmToReal(qint16 pcm)
{
	return qreal(pcm) / PCMS16MaxAmplitude;
}

qint16 realToPcm(qreal real)
{
	return real * PCMS16MaxValue;
}

const int    SpectrumLengthSamples  = PowerOfTwo<FFTLengthPowerOfTwo>::Result;

SpectrumAnalyserThread::SpectrumAnalyserThread(QObject *parent)
	:   QObject(parent)
#ifndef DISABLE_FFT
	,   m_fft(new FFTRealWrapper)
#endif
	,   m_numSamples(SpectrumLengthSamples)
	,   m_windowFunction(DefaultWindowFunction)
	,   m_window(SpectrumLengthSamples, 0.0)
	,   m_input(SpectrumLengthSamples, 0.0)
	,   m_output(SpectrumLengthSamples, 0.0)
	,   m_spectrum(SpectrumLengthSamples)
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
	,   m_thread(new QThread(this))
#endif
{
#ifdef SPECTRUM_ANALYSER_SEPARATE_THREAD
	// moveToThread() cannot be called on a QObject with a parent
	setParent(0);
	moveToThread(m_thread);
	m_thread->start();
#endif
	calculateWindow();
}

SpectrumAnalyserThread::~SpectrumAnalyserThread()
{
#ifndef DISABLE_FFT
	delete m_fft;
#endif
}

void SpectrumAnalyserThread::setWindowFunction(WindowFunction type)
{
	m_windowFunction = type;
	calculateWindow();
}

void SpectrumAnalyserThread::calculateWindow()
{
	for (int i=0; i<m_numSamples; ++i) {
		DataType x = 0.0;

		switch (m_windowFunction) {
		case NoWindow:
			x = 1.0;
			break;
		case HannWindow:
			x = 0.5 * (1 - qCos((2 * M_PI * i) / (m_numSamples - 1)));
			break;
		default:
			Q_ASSERT(false);
		}

		m_window[i] = x;
	}
}

void SpectrumAnalyserThread::calculateSpectrum(QVector<DataType> *buffer,
												int inputFrequency,
												int bytesPerSample)
{
	//Q_ASSERT(buffer->size() == m_numSamples * bytesPerSample);

	// Initialize data array
	const float *ptr = buffer->constData();
	for (int i=0; i<m_numSamples; ++i) {
		//const qint16 pcmSample = *reinterpret_cast<const qint16*>(ptr);
		// Scale down to range [-1.0, 1.0]
		//const DataType realSample = pcmToReal(pcmSample);
		const DataType windowedSample = ptr[i] * m_window[i];
		m_input[i] = windowedSample;
		//ptr += bytesPerSample;
	}

	// Calculate the FFT
	m_fft->calculateFFT(m_output.data(), m_input.data());

	//m_fft->calculateFFT(m_output.data(), buffer->data());
	// Analyze output to obtain amplitude and phase for each frequency
	for (int i=2; i<=m_numSamples/2; ++i) {
		// Calculate frequency of this complex sample
		m_spectrum[i].frequency = qreal(i * inputFrequency) / (m_numSamples);

		const qreal real = m_output[i];
		qreal imag = 0.0;
		if (i>0 && i<m_numSamples/2)
			imag = m_output[m_numSamples/2 + i];

		const qreal magnitude = qSqrt(real*real + imag*imag);
		//qreal amplitude = SpectrumAnalyserMultiplier * qLn(magnitude);
		qreal amplitude = qLn(magnitude);

		// Bound amplitude to [0.0, 1.0]
		m_spectrum[i].clipped = (amplitude > 1.0);
		//amplitude = qMax(qreal(0.0), amplitude);
		//amplitude = qMin(qreal(1.0), amplitude);
		m_spectrum[i].amplitude = amplitude;
	}
	emit calculationComplete(m_spectrum);
}



FrequencyAnalizerIODevice::FrequencyAnalizerIODevice(
		QXYSeries *series, FT2StreamReader* streamReader, int size, QObject *parent) :
	XYSeriesIODevice(series, streamReader, size, true, parent),
	buf1(SpectrumLengthSamples),
	buf2(SpectrumLengthSamples)
{
	streamReader->addListener(this);
	analizerThread = new SpectrumAnalyserThread(this);
	connect(this, SIGNAL(calculateSpectrum(QVector<DataType> *, int, int)),
			analizerThread, SLOT(calculateSpectrum(QVector<DataType> *, int, int)));
	connect(analizerThread, SIGNAL(calculationComplete(const FrequencySpectrum &)),
			this, SLOT(calculationComplete(const FrequencySpectrum &)));
	curBuf = &buf1;
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
		(*curBuf)[marker]=((float)data[i])/SHRT_MAX;
		marker++;
		if(marker >= SpectrumLengthSamples){
			marker = 0;
			emit calculateSpectrum(curBuf, 48000, 2);
			if(curBuf == &buf1)
				curBuf = &buf2;
			else
				curBuf = &buf1;
		}

	}

}

void FrequencyAnalizerIODevice::calculationComplete(const FrequencySpectrum &spectrum){
	for(int i=0;i<spectrum.count();i++)
		m_buffer[i].setY(spectrum[i].amplitude);
	m_series->replace(m_buffer);
}
