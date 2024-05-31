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
    ui->yearHorizontalSlider->setRange(1800, 2024);
    ui->yearHorizontalSlider->setSingleStep(1);

    // connecting Slider to LineEdit
    connect(ui->yearHorizontalSlider, &QSlider::sliderMoved, this, [=]{
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
