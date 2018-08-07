#include "ui_MainWindow.h"
#include "MainWindow.hpp"
#include "IOTypes/xyseriesiodevice.hpp"
#include "IOTypes/FT2StreamReader.hpp"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QAudioFormat>
#include <QAudioOutput>
#include <QSettings>

#define X_SAMPLES 1024

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_amplitudes(new QLineSeries),
	frequencies(new QBarSeries),
	frequencyStats(new QBarSeries)
{

	/***
	 * Restore UI
	 * */
	ui->setupUi(this);
	QSettings settings;
	settings.beginGroup("MainWindow");

	resize(settings.value("size", QSize(800, 600)).toSize());
	move(settings.value("pos", QPoint(200, 200)).toPoint());
	QByteArray verticalSplitterSizes = settings.value("verticalSplitterSizes").toByteArray();
	QByteArray horizontalSplitterSizes = settings.value("horizontalSplitterSizes").toByteArray();
	bool maximized = settings.value("maximized", false).toBool();
	if(maximized)
		setWindowState(Qt::WindowMaximized);
	settings.endGroup();

	/***
	 * End restore UI
	 * */


	QAudioFormat formatAudio;
	formatAudio.setSampleRate(48000);
	formatAudio.setChannelCount(1);
	formatAudio.setSampleSize(16);
	formatAudio.setCodec("audio/pcm");
	formatAudio.setByteOrder(QAudioFormat::LittleEndian);
	formatAudio.setSampleType(QAudioFormat::SignedInt);

	setupAudio();

	setupControls();

	auto chart1 = ui->frequencyChart->chart();
	chart1->addSeries(frequencies);
	auto chart2 = ui->frequencyStatistics->chart();
	chart2->addSeries(frequencyStats);
	auto chart3 = ui->timeAplitudeChart->chart();

	m_amplitudes->setUseOpenGL(true);
	chart3->addSeries(m_amplitudes);

	setupAmplitudeChart();
	setupFrequencyChart();
	setupFrequencyStats();

	if(!ui->verticalSplitter->restoreState(verticalSplitterSizes))
		ui->verticalSplitter->setSizes({400,400});
	if(!ui->horizontalSplitter->restoreState(horizontalSplitterSizes))
		ui->horizontalSplitter->setSizes({300,300});

	setupStreams();
}

MainWindow::~MainWindow(){
	saveSettings();
	delete ui;
}

void MainWindow::setupControls(){
	int rates[] = {8000, 16000, 32000, 41000, 48000, 384000};
	int depths[] = {8, 16, 24, 32};
	for(const int& item: rates )
		ui->sampleRate->addItem(QString::number(item), item);

	for(const int& item: depths )
		ui->bitDepth->addItem(QString::number(item), item);

}

void MainWindow::setupAmplitudeChart(){
	auto m_chart = ui->timeAplitudeChart->chart();

	//m_amplitudes->setUseOpenGL(true);
	//m_chart->addSeries(m_amplitudes);
	QValueAxis *axisX = new QValueAxis;

	axisX->setRange(0,X_SAMPLES);
	axisX->setLabelFormat("%g");
	axisX->setTitleText("Data Points");
	QValueAxis *axisY = new QValueAxis;
	axisY->setRange(-1, 1);
	axisY->setTitleText("Audio level");
	m_chart->setAxisX(axisX, m_amplitudes);
	m_chart->setAxisY(axisY, m_amplitudes);
	m_chart->legend()->hide();
	m_chart->setTitle("Input data" );

}


void MainWindow::setupFrequencyChart(){
	auto chart = ui->frequencyChart->chart();
	//chart->addSeries(frequencies);
	QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0,  ui->sampleRate->currentData().toInt() / 2);
	axisX->setLabelFormat("%g");
	axisX->setTitleText("Frequency");
	QValueAxis *axisY = new QValueAxis;
	axisY->setRange(0, 1);
	axisY->setTitleText("Decibels");
	chart->setAxisX(axisX, frequencies);
	chart->setAxisY(axisY, frequencies);
	chart->legend()->hide();
	ui->frequencyChart->chart()->setTitle("FFT deduced data" );
	//m_chart->addSeries(m_series);
	//ui->frequencyChart->fitInView(ui->frequencyChart->viewport()->rect());
}

void MainWindow::setupFrequencyStats(){
	auto chart = ui->frequencyStatistics->chart();
	//chart->addSeries(frequencyStats);
	QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0,  ui->sampleRate->currentData().toInt() / 2);
	axisX->setLabelFormat("%g");
	axisX->setTitleText("Frequency");
	QValueAxis *axisY = new QValueAxis;
	axisY->setRange(0, 1);
	axisY->setTitleText("Frequency");
	chart->setAxisX(axisX, frequencyStats);
	chart->setAxisY(axisY, frequencyStats);
	chart->legend()->hide();
	chart->setTitle("Frequency statistics" );
	//qDebug()<<ui->frequencyStatistics->viewportMargins().left() <<" "<<ui->frequencyStatistics->viewportMargins().right();
	auto rect = ui->frequencyStatistics->viewport()->rect();
	rect.moveLeft(2);
	rect.moveTop(2);
	rect.setWidth(rect.width()-8);
	rect.setHeight(rect.height()-8);
	//ui->frequencyStatistics->fitInView(rect);

}

void MainWindow::on_startStopButton_clicked(){

}

void MainWindow::on_sampleRate_currentIndexChanged(int index){

}

void MainWindow::on_bitDepth_currentIndexChanged(int index){

}

void MainWindow::on_playSoundCheckBox_toggled(bool checked){

}

void MainWindow::on_startStopButton_toggled(bool checked){
	if(checked){
		ui->startStopButton->setText("Stop");
		setupAmplitudeChart();
		setupFrequencyChart();
		setupFrequencyStats();
		//m_audioInput->start(m_device);
		m_audioOutput->start(m_file);
	}else{
		ui->startStopButton->setText("Start");
		m_audioOutput->stop();
	}

}

void MainWindow::setupAudio(){

	const auto deviceInfo =  QAudioDeviceInfo::defaultOutputDevice();

	QAudioFormat format;
	format.setSampleRate(48000);
	format.setChannelCount(1);
	format.setSampleSize(16);
	format.setCodec("audio/pcm");
	format.setByteOrder(QAudioFormat::LittleEndian);
	format.setSampleType(QAudioFormat::SignedInt);

	if (!deviceInfo.isFormatSupported(format)) {
		qWarning() << "Default format not supported - trying to use nearest";
		format = deviceInfo.nearestFormat(format);
	}

	m_audioOutput = new QAudioOutput(deviceInfo, format, this);

}

void MainWindow::setupStreams(){

	streamReader = new FT2StreamReader(this);
	m_file = new FT2StreamConsumer(streamReader, this);
	m_file->open(QIODevice::ReadWrite);
	m_file->readHeader();
	m_analysisFile = new FT2StreamConsumer(streamReader, this);
	m_analysisFile->open(QIODevice::ReadWrite);
	m_analysisFile->readHeader();
	//int maxSamples =  ui->sampleRate->currentData().toInt();
	m_device = new XYSeriesIODevice(m_amplitudes, streamReader, X_SAMPLES, this);
	m_device->open(QIODevice::ReadWrite);

	//initialize();
}

void MainWindow::saveSettings(){
	QSettings settings;

	settings.beginGroup("MainWindow");
	settings.setValue("size", size());
	settings.setValue("pos", pos());
	settings.setValue("verticalSplitterSizes", ui->verticalSplitter->saveState());
	settings.setValue("horizontalSplitterSizes", ui->horizontalSplitter->saveState());
	settings.setValue("maximized", isMaximized());
	settings.endGroup();
}
