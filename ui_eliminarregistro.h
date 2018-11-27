#ifndef UI_ELIMINARREGISTRO_H
#define UI_ELIMINARREGISTRO_H

#include <QDialog>
#include <QtSql/QSqlTableModel>
#include <QStringList>
#include <QList>

namespace Ui {
class UI_EliminarRegistro;
}

class UI_EliminarRegistro : public QDialog
{
    Q_OBJECT

public:
    explicit UI_EliminarRegistro(QList<QStringList> listTipos,QSqlTableModel *model,QWidget *parent = nullptr);
    ~UI_EliminarRegistro();

private:
    Ui::UI_EliminarRegistro *ui;
    QSqlTableModel *model;
    QStringList listTipos;

    QString get_tipoCampo(QString tCampo);
};

#endif // UI_ELIMINARREGISTRO_H
