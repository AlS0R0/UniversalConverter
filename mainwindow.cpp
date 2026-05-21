#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "baseconverter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создаём меню "File"
    QMenu* fileMenu = menuBar()->addMenu(tr("&Universal Converter"));
    QAction* openAction = fileMenu->addAction(tr("&Open..."));
    QAction* saveAction = fileMenu->addAction(tr("&Save..."));

    // Явные connect для пунктов меню
    connect(openAction, &QAction::triggered, this, &MainWindow::actionOpen_triggered);
    connect(saveAction, &QAction::triggered, this, &MainWindow::actionSave_triggered);

    // Явные connect для кнопок на форме
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::pushButton_clicked);
    connect(ui->openFile, &QPushButton::clicked, this, &MainWindow::openFile_clicked);
    connect(ui->saveToFile, &QPushButton::clicked, this, &MainWindow::saveToFile_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Конвертация по кнопке "convert"
void MainWindow::pushButton_clicked()
{
    QString input = ui->inInt->text();
    QString pStr = ui->pLine->text();
    QString qStr = ui->qLine->text();

    bool pOk, qOk;
    int p = pStr.toInt(&pOk);
    int q = qStr.toInt(&qOk);

    if (!pOk || !qOk) {
        showError("Основания систем счисления должны быть целыми числами.");
        return;
    }

    std::string output, error;
    bool success = BaseConverter::convert(input.toStdString(), p, q, output, error);

    if (!success) {
        showError(QString::fromStdString(error));
        ui->outInt->clear();
    } else {
        if (!error.empty()) {
            showResult(QString::fromStdString(output), QString::fromStdString(error));
        } else {
            showResult(QString::fromStdString(output));
        }
    }
}

// Обработчик кнопки "Открыть файл"
void MainWindow::openFile_clicked()
{
    actionOpen_triggered(); // вызываем логику из меню
}

// Обработчик кнопки "Записать в файл"
void MainWindow::saveToFile_clicked()
{
    actionSave_triggered(); // вызываем логику из меню
}

// Загрузка из файла (меню)
void MainWindow::actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Input File"), "",
                                                    tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot open file."));
        return;
    }

    QTextStream in(&file);
    QString pStr = in.readLine();
    QString qStr = in.readLine();
    QString input = in.readLine();

    if (pStr.isNull() || qStr.isNull() || input.isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("File must contain exactly three lines: p, q, input."));
        return;
    }

    ui->pLine->setText(pStr.trimmed());
    ui->qLine->setText(qStr.trimmed());
    ui->inInt->setText(input.trimmed());
}

// Сохранение результата в файл (меню)
void MainWindow::actionSave_triggered()
{
    QString result = ui->outInt->toPlainText();

    if (result.isEmpty()) {
        QMessageBox::information(this, tr("Save"), tr("Nothing to save."));
        return;
    }

    // Проверяем, было ли сообщение об обрезке результата
    QString warning = statusBar()->currentMessage();
    bool truncated = warning.contains("обрезана") || warning.contains("длинная");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Result"), "",
                                                    tr("Text Files (*.txt);;All Files (*)"));
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Error"), tr("Cannot write to file."));
        return;
    }

    QTextStream out(&file);
    out << result;
    if (truncated && !warning.isEmpty()) {
        out << "\n\n" << warning;
    }

    QMessageBox::information(this, tr("Save"), tr("Result saved."));
}

// Отображение ошибки
void MainWindow::showError(const QString& msg)
{
    ui->outInt->clear();
    statusBar()->showMessage(msg, 5000);
    QMessageBox::warning(this, tr("Conversion Error"), msg);
}

// Отображение результата (с возможным предупреждением)
void MainWindow::showResult(const QString& result, const QString& warning)
{
    ui->outInt->setPlainText(result);

    if (!warning.isEmpty()) {
        statusBar()->showMessage(warning, 10000);
    } else {
        statusBar()->clearMessage();
    }
}