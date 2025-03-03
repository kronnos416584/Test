#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void deleteTask();
    void showTaskDetails();

private:
    Ui::MainWindow *ui;
    void loadTasks();
    void saveTasks();

};
#endif // MAINWINDOW_H
