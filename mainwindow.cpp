#include "mainwindow.h"
#include <QCompleter>
#include <QDebug>
#include <QFile>
#include <QSlider>
#include <QStringList>
#include <QDebug>
#include "predictor.cpp"
#include "./ui_mainwindow.h"
#include <map>
#include "dialog.h"

/*!
 * Returns and connects to the lineEdit a case-insensitive QCompleter it created
 * with the wordList it's been given.
*/
QCompleter *setCaseInsensitiveCompleter(MainWindow *window,
                                        QLineEdit *lineEdit,
                                        QStringList wordList)
{
    QCompleter *completer = new QCompleter(wordList, window);
    completer->setCaseSensitivity(Qt::CaseInsensitive); // We don't need case sensitivity
    lineEdit->setCompleter(completer);
    return completer;
}

/*!
 * Sets up two completers connected by a relation with wordlists.
*/
void setCaseInsensitiveCompletersWithRelation(MainWindow *window, QMap<QString, QList<QString>> relation, QLineEdit *lineEditA, QLineEdit *lineEditB, QStringList wordListA, QStringList wordListB) {
    window->connect(lineEditA, &QLineEdit::textChanged, window, [=]{
        QString enteredTextInLineEditB = lineEditB->text();
        if (enteredTextInLineEditB.isEmpty()) { // if there's no text in second LineEdit
            setCaseInsensitiveCompleter(window, lineEditA, wordListA); // then it means we give out the entire wordlist of suggestions (setting up the completer)
        } else { // if there's some text in second lineEdit
            auto foundBIter = relation.find(enteredTextInLineEditB); // we try to find a relation with the entered text
            if (foundBIter != relation.end()) { // if we find this relation
                QList<QString> listOfFoundAs = foundBIter.value(); // we recieve a list of suggestions for our lineEdit
                setCaseInsensitiveCompleter(window, lineEditA, listOfFoundAs); // which we set up it with
            }
        }
    });

    // we complete all the steps above in regards to the other lineEdit
    window->connect(lineEditB, &QLineEdit::textChanged, window, [=]{
        QString enteredTextInLineEditA = lineEditA->text();
        if (enteredTextInLineEditA.isEmpty()) {
            setCaseInsensitiveCompleter(window, lineEditB, wordListB);
        } else {
            auto foundAIter = relation.find(enteredTextInLineEditA);
            if (foundAIter != relation.end()) {
                QList<QString> listOfFoundBs = foundAIter.value();
                setCaseInsensitiveCompleter(window, lineEditB, listOfFoundBs);
            }
        }
    });
}

/*!
 * Returns a modified relation map between two Qstrings. Relation being set up in this function is one-to-many.
 * If one wants to set up a many-to-many relationship, they can use the function twice. Example of such many-to-many
 * relationship setup:
 *  createOrAppendToOneToManyRelation(relation, a, b);
 *  createOrAppendToOneToManyRelation(relation, b, a);
*/
QMap<QString, QList<QString>> &createOrAppendToOneToManyRelation(QMap<QString, QList<QString>> &relation, QString a, QString b) {
    auto it = relation.find(a); // we are trying to find out if at least a part of this relation already exists
    if (it != relation.end()){ // if it does
        if (!relation[a].contains(b)) { // and if we don't have the entire relation already
            relation[a].append(b); // we append the second part of this relation (a.k.a. "create" the relation between the QStrings)
        }
    } else { // if we don't have the "one" part of this relation
        relation[a] = *new QList<QString>; // we just create the entire relation
        relation[a].append(b); // "connecting" two strings
    }
    return relation;
}

/*!
 * Connects slider to the lineEdit so that when the slider value is changed,
 * the lineEdit value changes as well and vice versa.
*/
void setupSliderLineEdit(
    MainWindow *window,
    QSlider *slider,
    QLineEdit *lineEdit /*, void (*changeSliderValue)(int*), void (*changeLineEditValue)(int*) */)
{
    // connecting Slider to LineEdit
    window->connect(slider, &QSlider::valueChanged, window, [=] {
        int val = slider->value();
        // We'll change val here if needed using changeSliderValue(int*)
        lineEdit->setText(QString::number(val));
    });
    // connecting LineEdit to Slider
    window->connect(lineEdit, &QLineEdit::textChanged, window, [=] {
        int val = lineEdit->text().toInt();
        // We'll change val here if needed using changeLineEditValue(int*)
        slider->setValue(val);
    });
}

/*!
 * Returns the Qstring in the following form: "Word",
 * meaning it capitilizes the first letter while also lowering the rest of the word.
 */
QString capitalizeFirst(const QString word) {
    if (word.size() == 0) {
        return word;
    }
    QString capitilizedWord;
    capitilizedWord.append(word[0].toUpper());
    for (int i = 1; i < (int)word.size(); i++) {
        capitilizedWord.append(word[i].toLower());
    }
    return capitilizedWord;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setupSliderLineEdit(this, ui->yearHorizontalSlider, ui->yearLineEdit);
    setupSliderLineEdit(this, ui->mileageHorizontalSlider, ui->mileageLineEdit);

    ui->yearHorizontalSlider->setRange(1990, 2024);
    ui->yearHorizontalSlider->setSingleStep(1);
    ui->yearHorizontalSlider->setValue(2020);

    ui->mileageHorizontalSlider->setRange(0, 100000);
    ui->mileageHorizontalSlider->setSingleStep(1);
    ui->mileageHorizontalSlider->setValue(10000);

    // year brand model trim body_type transmission vin state condition odometer color interior seller mmr selling_price sale_date
    QFile file(":/car_prices.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }

    Predictor pred;

    QStringList brandNamesWordList;
    QStringList modelsWordList;
    QMap<QString, QList<QString>> brandToModel;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList paramList = line.split(',');

        QString brandName = capitalizeFirst(paramList[1]);
        QString modelName = capitalizeFirst(paramList[2]);

        createOrAppendToOneToManyRelation(brandToModel, brandName, modelName);
        createOrAppendToOneToManyRelation(brandToModel, modelName, brandName);

        pred.feedRow(brandName, modelName, paramList[0].toInt(), paramList[8].toInt(), paramList[9].toInt(), paramList[14].toInt());

        brandNamesWordList.append(brandName);
        brandNamesWordList.removeDuplicates();
        modelsWordList.append(modelName);
        modelsWordList.removeDuplicates();
    }

    setCaseInsensitiveCompletersWithRelation(this, brandToModel, ui->brandLineEdit, ui->modelLineEdit, brandNamesWordList, modelsWordList);

    this->connect(ui->pushButtonPredict, &QPushButton::released, [=] {
            QString textBrandName = ui->brandLineEdit->text();
            QString textModelName = ui->modelLineEdit->text();
            int year = ui->yearLineEdit->text().toInt();
            int condition = ui->conditionLineEdit->text().toInt();
            int odometer = ui->mileageLineEdit->text().toInt();
            double price;
            price = pred.fit(textBrandName, textModelName, year, condition, odometer);
            ui->labelPrice->setText(QString::number(price));

    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Dialog window;
    window.setModal(true);
    window.exec();

}

