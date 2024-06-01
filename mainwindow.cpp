#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCompleter>
#include <QSlider>
#include <QFile>
#include <QStringList>
#include <QDebug>

/*!
  Returns and connects to the lineEdit a case-insensitive QCompleter it created
  with the wordList it's been given.
*/
QCompleter *setCaseInsensitiveCompleter(MainWindow *window, QLineEdit *lineEdit, QStringList wordList) {
    QCompleter *completer = new QCompleter(wordList, window);
    completer->setCaseSensitivity(Qt::CaseInsensitive); // We don't need case sensitivity
    lineEdit->setCompleter(completer);
    return completer;
}

/*!
  Connects slider to the lineEdit so that when the slider value is changed,
  the lineEdit value changes as well and vice versa.
*/
void setupSliderLineEdit(MainWindow *window, QSlider *slider, QLineEdit *lineEdit /*, void (*changeSliderValue)(int*), void (*changeLineEditValue)(int*) */) {
    // connecting Slider to LineEdit
    window->connect(slider, &QSlider::valueChanged, window, [=]{
        int val = slider->value();
        // We'll change val here if needed using changeSliderValue(int*)
        lineEdit->setText(QString::number(val));
    });
    // connecting LineEdit to Slider
    window->connect(lineEdit, &QLineEdit::textChanged, window, [=]{
        int val = lineEdit->text().toInt();
        // We'll change val here if needed using changeLineEditValue(int*)
        slider->setValue(val);
    });
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupSliderLineEdit(this, ui->yearHorizontalSlider, ui->yearLineEdit);
    setupSliderLineEdit(this, ui->conditionHorizontalSlider, ui->conditionLineEdit);
    setupSliderLineEdit(this, ui->mileageHorizontalSlider, ui->mileageLineEdit);

    ui->yearHorizontalSlider->setRange(1800, 2024);
    ui->yearHorizontalSlider->setSingleStep(1);
    ui->yearHorizontalSlider->setValue(2020);

    ui->conditionHorizontalSlider->setRange(1, 100);
    ui->conditionHorizontalSlider->setSingleStep(1);
    ui->conditionHorizontalSlider->setValue(70);

    ui->mileageHorizontalSlider->setRange(0, 100000);
    ui->mileageHorizontalSlider->setSingleStep(1);
    ui->mileageHorizontalSlider->setValue(10000);

    // year brand model trim body_type transmission vin state condition odometer color interior seller mmr selling_price sale_date
    QFile file(":/car_prices.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    QStringList brandNamesWordList;
    QStringList modelsWordList;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList paramList = line.split(',');
        brandNamesWordList.append(paramList[1]); // brand
        modelsWordList.append(paramList[2]); // model
    }
    brandNamesWordList.removeDuplicates();
    modelsWordList.removeDuplicates();

    QCompleter *brandNamesCompleter = setCaseInsensitiveCompleter(this, ui->brandLineEdit, brandNamesWordList);
    QCompleter *modelsCompleter = setCaseInsensitiveCompleter(this, ui->modelLineEdit, modelsWordList);
}

MainWindow::~MainWindow()
{
    delete ui;
}

