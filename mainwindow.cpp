#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCompleter>
#include <QSlider>
#include <QFile>
#include <QStringList>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // connecting Slider to LineEdit
    connect(ui->yearHorizontalSlider, &QSlider::valueChanged, this, [=]{
        int val = ui->yearHorizontalSlider->value();
        // We'll change val here if needed
        ui->yearLineEdit->setText(QString::number(val));
    });

    // connecting LineEdit to Slider
    connect(ui->yearLineEdit, &QLineEdit::textChanged, this, [=]{
        int val = ui->yearLineEdit->text().toInt();
        // We'll change val here if needed
        ui->yearHorizontalSlider->setValue(val);
    });



    connect(ui->conditionHorizontalSlider, &QSlider::valueChanged, this, [=]{
        int val = ui->conditionHorizontalSlider->value();
        // We'll change val here if needed
        ui->conditionLineEdit->setText(QString::number(val));
    });

    // connecting LineEdit to Slider
    connect(ui->conditionLineEdit, &QLineEdit::textChanged, this, [=]{
        int val = ui->conditionLineEdit->text().toInt();
        // We'll change val here if needed
        ui->conditionHorizontalSlider->setValue(val);
    });



    connect(ui->mileageHorizontalSlider, &QSlider::valueChanged, this, [=]{
        int val = ui->mileageHorizontalSlider->value();
        // We'll change val here if needed
        ui->mileageLineEdit->setText(QString::number(val));
    });

    // connecting LineEdit to Slider
    connect(ui->mileageLineEdit, &QLineEdit::textChanged, this, [=]{
        int val = ui->mileageLineEdit->text().toInt();
        // We'll change val here if needed
        ui->mileageHorizontalSlider->setValue(val);
    });

    ui->yearHorizontalSlider->setRange(1800, 2024);
    ui->yearHorizontalSlider->setSingleStep(1);
    ui->yearHorizontalSlider->setValue(2020);

    ui->conditionHorizontalSlider->setRange(1, 100);
    ui->conditionHorizontalSlider->setSingleStep(1);
    ui->conditionHorizontalSlider->setValue(70);

    ui->mileageHorizontalSlider->setRange(0, 100000);
    ui->mileageHorizontalSlider->setSingleStep(1);
    ui->mileageHorizontalSlider->setValue(10000);




    // TODO: using QCompleter and QLineEdit, add fields for inputing text with search (for typing brand names, model names etc.)
    // For each Qline edit it should look something like this:
    // QStringList wordList;
    // fill the wordlist with words from database

    // QLineEdit *lineEdit = new QLineEdit(this);

    // QCompleter *completer = new QCompleter(wordList, this);
    // completer->setCaseSensitivity(Qt::CaseInsensitive); // We don't need case sensitivity
    // lineEdit->setCompleter(completer);
}

MainWindow::~MainWindow()
{
    delete ui;
}

