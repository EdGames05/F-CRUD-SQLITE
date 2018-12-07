#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QListWidgetItem>
#include <QTableView>
#include "ui_eliminarregistro.h"

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
    QSqlDatabase db;
    QStringList tablasClickadas;
    QList<QStringList> listaTiposCampos;
    UI_EliminarRegistro *eliminar;
    bool abrir_db();
    QString get_tipoCampo(QString);

    void cerrarConexiones();
    void enlistarTipos(int indexTabla);
    bool validar_numeros(QString texto);
};

#endif // MAINWINDOW_H
