#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QtCharts/QChartGlobal>

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
class QLineSeries;
class QBarSeries;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class XYSeriesIODevice;
class QAudioOutput;
class FT2StreamReader;
class FT2StreamConsumer;
class FFTRealWrapper;
class FrequencyAnalizerIODevice;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_startStopButton_clicked();

	void on_sampleRate_currentIndexChanged(int index);

	void on_bitDepth_currentIndexChanged(int index);

	void on_playSoundCheckBox_toggled(bool checked);

	void on_startStopButton_toggled(bool checked);

private:
	void setupControls();
	void setupAmplitudeChart();
	void setupFrequencyStats();
	void setupFrequencyChart();
	void setupAudio();
	void setupStreams();
	void saveSettings();

	Ui::MainWindow *ui;
	QChart *m_chart;
	QLineSeries *m_amplitudes ;
	QLineSeries* frequencies;
	QLineSeries* frequencyStats;
	XYSeriesIODevice* amplitudeSeries;
	FrequencyAnalizerIODevice* frequencySeries;
	QAudioOutput* m_audioOutput;
	FT2StreamReader* streamReader;
	FT2StreamConsumer* m_file;
	FT2StreamConsumer* m_analysisFile;
	FFTRealWrapper* m_fft;
};

#endif // MAINWINDOW_HPP
