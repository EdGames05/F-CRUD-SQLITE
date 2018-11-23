#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QListWidgetItem>
#include <QTableView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAbrir_clicked();

    void on_btnCerrar_clicked();

    void on_listTablas_itemDoubleClicked(QListWidgetItem *item);

    void on_actionSalir_triggered();

    void on_actionCerrar_db_triggered();

    void on_actionNuevo_registro_triggered();

    void on_actionAbrir_db_triggered();

    void on_actionEliminar_registro_triggered();

private:
    Ui::MainWindow *ui;
    QList<QSqlTableModel*> listModel;
    QList<QTableView*> listTables;
    QStringList tablasClickadas;
    bool abrir_db();
    QString get_tipoCampo(QString);
};

#endif // MAINWINDOW_H
