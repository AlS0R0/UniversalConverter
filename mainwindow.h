#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void pushButton_clicked();
    void openFile_clicked();      // для кнопки "Открыть файл"
    void saveToFile_clicked();    // для кнопки "Записать в файл"
    void actionOpen_triggered();  // для пункта меню File → Open
    void actionSave_triggered();  // для пункта меню File → Save

private:
    Ui::MainWindow *ui;
    void showError(const QString& msg);
    void showResult(const QString& result, const QString& warning = QString());
};

#endif // MAINWINDOW_H