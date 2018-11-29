#ifndef UI_ELIMINARREGISTRO_H
#define UI_ELIMINARREGISTRO_H

#include <QDialog>
#include <QtSql/QSqlTableModel>
#include <QStringList>
#include <QList>
#include <QtSql/QSqlDatabase>

namespace Ui {
class UI_EliminarRegistro;
}

class UI_EliminarRegistro : public QDialog
{
    Q_OBJECT

public:
    explicit UI_EliminarRegistro(QSqlDatabase db,QString nombreTabla, QStringList listTipos, QSqlTableModel *model, QWidget *parent = nullptr);
    ~UI_EliminarRegistro();

private slots:
    void on_ddl_campos_currentIndexChanged(int index);

    void on_txtIgual_textChanged(const QString &arg1);

    void on_rb_practico_clicked(bool checked);

    void on_btnAceptar_clicked();

    void on_btnEjecutar_clicked();

    void on_rb_avanzado_clicked(bool checked);

private:
    Ui::UI_EliminarRegistro *ui;
    QSqlTableModel *model;
    QStringList listTipos;
    const QString consulta = "DELETE FROM ";
    QString nombreTabla;
    QString get_tipoCampo(QString tCampo);
    QSqlDatabase db;
    QStringList palabras;
};

#endif // UI_ELIMINARREGISTRO_H
