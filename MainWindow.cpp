#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "xyseriesiodevice.h"
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>
#include <QAudioFormat>
#include <QAudioInput>

QAudioInput* m_audioInput;
MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_amplitudes(new QLineSeries),
	frequencies(new QBarSeries),
	frequencyStats(new QBarSeries)
{
	ui->setupUi(this);




	QAudioFormat formatAudio;
	formatAudio.setSampleRate(8000);
	formatAudio.setChannelCount(1);
	formatAudio.setSampleSize(8);
	formatAudio.setCodec("audio/pcm");
	formatAudio.setByteOrder(QAudioFormat::LittleEndian);
	formatAudio.setSampleType(QAudioFormat::UnSignedInt);

	m_audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), formatAudio, this);

	setupControls();
	setupAmplitudeChart();
	setupFrequencyChart();
	setupFrequencyStats();
}

MainWindow::~MainWindow(){
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

	m_amplitudes->setUseOpenGL(true);
	m_chart->addSeries(m_amplitudes);
	QValueAxis *axisX = new QValueAxis;
	int maxSamples =  ui->sampleRate->currentData().toInt();
	axisX->setRange(0,maxSamples);
	axisX->setLabelFormat("%g");
	axisX->setTitleText("Samples");
	QValueAxis *axisY = new QValueAxis;
	axisY->setRange(-1, 1);
	axisY->setTitleText("Audio level");
	m_chart->setAxisX(axisX, m_amplitudes);
	m_chart->setAxisY(axisY, m_amplitudes);
	m_chart->legend()->hide();
	m_chart->setTitle("Input data" );

	m_device = new XYSeriesIODevice(m_amplitudes, maxSamples, this);
	m_device->open(QIODevice::WriteOnly);

	m_audioInput->start(m_device);

}


void MainWindow::setupFrequencyChart(){
	auto chart = ui->frequencyChart->chart();
	chart->addSeries(frequencies);
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
	chart->addSeries(frequencyStats);
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
	rect.moveLeft(8);
	rect.moveTop(8);
	rect.setWidth(rect.width()+16);
	rect.setHeight(rect.height()+16);
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
