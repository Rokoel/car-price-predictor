#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCompleter>
#include <QSlider>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <map>

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

std::map <QString, QStringList> brand_names_selections;
std::map <QString, QCompleter*> brand_names_completers;
std::map <QString, QStringList> model_names_selections;
std::map <QString, QCompleter*> model_names_completers;

QString capitalize_first(const QString word) {
    if (word.size() == 0) {
        return word;
    }
    QString to_return;
    to_return.append(word[0].toUpper());
    for (int i = 1; i < (int)word.size(); i++) {
        to_return.append(word[i].toLower());
    }
    return to_return;
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

        QString brand_name = capitalize_first(paramList[1]);
        QString model_name = capitalize_first(paramList[2]);

        if (brand_names_selections.find(brand_name) == brand_names_selections.end()) {
            QStringList a;
            brand_names_selections[brand_name] = a;
        }
        brand_names_selections[brand_name].append(model_name);

        if (model_names_selections.find(model_name) == model_names_selections.end()) {
            QStringList a;
            model_names_selections[model_name] = a;
        }
        model_names_selections[model_name].append(brand_name);

        brandNamesWordList.append(brand_name); // brand
        modelsWordList.append(model_name); // model
    }
    brandNamesWordList.removeDuplicates();
    for (auto p : brand_names_selections) {
        p.second.removeDuplicates();
        brand_names_completers[p.first] = new QCompleter(p.second, this);
        brand_names_completers[p.first]->setCaseSensitivity(Qt::CaseInsensitive);
    }
    modelsWordList.removeDuplicates();
    for (auto p : model_names_selections) {
        p.second.removeDuplicates();
        model_names_completers[p.first] = new QCompleter(p.second, this);
        model_names_completers[p.first]->setCaseSensitivity(Qt::CaseInsensitive);
    }
    model_names_selections.erase(QString(""));

    QCompleter *brandNamesCompleter = setCaseInsensitiveCompleter(this, ui->brandLineEdit, brandNamesWordList);
    QCompleter *modelsCompleter = setCaseInsensitiveCompleter(this, ui->modelLineEdit, modelsWordList);

    
    this->connect(ui->brandLineEdit, &QLineEdit::textChanged, this, [=]{
        if (brand_names_selections.find(ui->brandLineEdit->text()) == brand_names_selections.end()) {
            ui->modelLineEdit->setCompleter(modelsCompleter);
            return;
        }
        ui->modelLineEdit->setCompleter(brand_names_completers[ui->brandLineEdit->text()]);
    });

    this->connect(ui->modelLineEdit, &QLineEdit::textChanged, this, [=]{
        if (model_names_selections.find(ui->modelLineEdit->text()) == model_names_selections.end()) {
            ui->brandLineEdit->setCompleter(brandNamesCompleter);
            return;
        }
        ui->brandLineEdit->setCompleter(model_names_completers[ui->modelLineEdit->text()]);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

