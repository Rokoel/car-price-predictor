#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QCompleter"
#include "QSlider"
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QDebug>


void printFirstRows(const QSqlDatabase &db, int rowCount) {
    if (!db.isOpen()) {
        qDebug() << "Database not open!";
        return;
    }

    QSqlQuery query("SELECT * FROM your_table_name"); // Replace 'your_table_name' with your actual table name

    int columnCount = query.record().count();

    int rowCounter = 0;
    while (query.next() && rowCounter < rowCount) {
        for (int i = 0; i < columnCount; i++) {
            qDebug() << query.value(i).toString();
        }
        rowCounter++;
        qDebug() << "----------"; // Just for separation of rows
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    // db.setDatabaseName("car_prices.sql");
    // db.open();
    // // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // if (!db.open()) {
    //     qDebug() << "Error: Could not connect to database.";
    // }

    // printFirstRows(db, 5); // Print the first 5 rows

    // db.close();


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
