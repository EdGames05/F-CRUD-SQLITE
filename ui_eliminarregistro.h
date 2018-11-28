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
    explicit UI_EliminarRegistro(QString nombreTabla, QStringList listTipos, QSqlTableModel *model, QWidget *parent = nullptr);
    ~UI_EliminarRegistro();

private slots:
    void on_ddl_campos_currentIndexChanged(int index);

    void on_txtIgual_textChanged(const QString &arg1);

private:
    Ui::UI_EliminarRegistro *ui;
    QSqlTableModel *model;
    QStringList listTipos;
    const QString consulta = "DELETE FROM ";
    QString nombreTabla;
    QString get_tipoCampo(QString tCampo);
};

#endif // UI_ELIMINARREGISTRO_H
