#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTasks(); // Загружаем задачи при запуске приложения
    connect(ui->addTaskButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(ui->deleteTaskButton, &QPushButton::clicked, this, &MainWindow::deleteTask);
    connect(ui->detailsTaskButton, &QPushButton::clicked, this, &MainWindow::showTaskDetails);
}

MainWindow::~MainWindow()
{
    saveTasks();
    delete ui;
}




void MainWindow::addTask()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Добавить задачу");
    QVBoxLayout layout(&dialog);

    QLineEdit titleEdit;
    titleEdit.setPlaceholderText("Название задачи");
    layout.addWidget(&titleEdit);

    QTextEdit descriptionEdit;
    descriptionEdit.setPlaceholderText("Описание задачи");
    layout.addWidget(&descriptionEdit);

    QDateEdit dateEdit;
    dateEdit.setCalendarPopup(true);
    layout.addWidget(&dateEdit);

    QPushButton addButton("Добавить");
    layout.addWidget(&addButton);

    connect(&addButton, &QPushButton::clicked, [&]() {
        QString title = titleEdit.text().trimmed();
        QString description = descriptionEdit.toPlainText().trimmed();
        QDate date = dateEdit.date();

        if (title.isEmpty() || title.simplified().isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Название задачи не может быть пустым или состоять только из пробелов.");
            return;
        }

        if (description.isEmpty() || description.simplified().isEmpty()) {
            QMessageBox::warning(&dialog, "Ошибка", "Описание задачи не может быть пустым или состоять только из пробелов.");
            return;
        }

        // Добавляем задачу в список и сохраняем дополнительные данные
        QListWidgetItem *newItem = new QListWidgetItem(title);
        newItem->setData(Qt::UserRole, description);  // Сохраняем описание
        newItem->setData(Qt::UserRole + 1, date);  // Сохраняем дату

        ui->tasksListWidget->addItem(newItem);

        dialog.accept();
    });

    dialog.exec();
}

void MainWindow::deleteTask()
{
    QListWidgetItem *item = ui->tasksListWidget->currentItem();
    if (item) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Удаление задачи");
        msgBox.setText("Вы уверены, что хотите удалить задачу?");

        // Добавляем стандартные кнопки с их ролями
        QPushButton* yesButton = msgBox.addButton(QMessageBox::Yes);
        QPushButton* noButton = msgBox.addButton(QMessageBox::No);

        // Меняем текст кнопок
        yesButton->setText("Да");
        noButton->setText("Нет");

        // Показываем окно подтверждения
        msgBox.exec();

        // Проверяем, была ли нажата кнопка "Да"
        if (msgBox.clickedButton() == yesButton) {
            delete item;
        }
    } else {
        QMessageBox::warning(this, "Удаление задачи", "Выберите задачу для удаления.");
    }
}

void MainWindow::showTaskDetails()
{
    QListWidgetItem *item = ui->tasksListWidget->currentItem();

    if (item) {
        // Предположим, что в тексте элемента списка хранится только название задачи и дата выполнения.
        QString taskDetails = item->text();

        // Для полноты картины, мы можем добавить описание и дату выполнения задачи.
        // Допустим, мы добавим описание и дату выполнения как пользовательские данные в item.
        QString description = item->data(Qt::UserRole).toString();  // описание задачи
        QDate taskDate = item->data(Qt::UserRole + 1).toDate();  // дата выполнения задачи

        // Формируем строку с подробной информацией о задаче
        QString details = QString("<b>Задача:</b> %1\n<b>Описание:</b> %2\n<b>Дата выполнения:</b> %3")
                              .arg(taskDetails)
                              .arg(description)
                              .arg(taskDate.toString("dd.MM.yyyy"));

        // Показываем информацию в диалоговом окне
        QMessageBox::information(this, "Детали задачи", details);
    } else {
        QMessageBox::warning(this, "Просмотр задачи", "Выберите задачу для просмотра.");
    }
}
void MainWindow::saveTasks()
{
    QSettings settings("MyCompany", "TaskManager");  // Уникальные идентификаторы для компании и приложения

    // Получаем количество задач в списке
    int taskCount = ui->tasksListWidget->count();

    // Сохраняем количество задач
    settings.setValue("taskCount", taskCount);

    // Сохраняем каждую задачу
    for (int i = 0; i < taskCount; ++i) {
        QListWidgetItem *item = ui->tasksListWidget->item(i);

        QString title = item->text();
        QString description = item->data(Qt::UserRole).toString();
        QDate date = item->data(Qt::UserRole + 1).toDate();

        settings.setValue("task_" + QString::number(i) + "/title", title);
        settings.setValue("task_" + QString::number(i) + "/description", description);
        settings.setValue("task_" + QString::number(i) + "/date", date.toString("yyyy-MM-dd"));
    }
}
void MainWindow::loadTasks()
{
    QSettings settings("MyCompany", "TaskManager");  // Используем те же уникальные идентификаторы

    // Получаем количество сохраненных задач
    int taskCount = settings.value("taskCount", 0).toInt();

    // Загружаем каждую задачу
    for (int i = 0; i < taskCount; ++i) {
        QString title = settings.value("task_" + QString::number(i) + "/title").toString();
        QString description = settings.value("task_" + QString::number(i) + "/description").toString();
        QDate date = QDate::fromString(settings.value("task_" + QString::number(i) + "/date").toString(), "yyyy-MM-dd");

        // Добавляем задачу в список
        QListWidgetItem *newItem = new QListWidgetItem(title);
        newItem->setData(Qt::UserRole, description);  // Сохраняем описание
        newItem->setData(Qt::UserRole + 1, date);  // Сохраняем дату

        ui->tasksListWidget->addItem(newItem);
    }
}
