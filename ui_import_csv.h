#ifndef UI_IMPORT_CSV_H
#define UI_IMPORT_CSV_H

#include <QDialog>
#include <QList>
#include <QStringList>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>

namespace Ui {
class ui_import_csv;
}

class ui_import_csv : public QDialog
{
    Q_OBJECT

public:
    explicit ui_import_csv(QStringList listCampos,QString nombre_tabla,QStringList tipos,QList<bool> llaves,QWidget *parent = nullptr);
    ~ui_import_csv();

private slots:
    void on_btnArchivo_clicked();

    void on_btnImportar_clicked();

private:
    Ui::ui_import_csv *ui;
    QString tabla;
    QStringList tipos;
    QStringList listCampos;
    QList<bool> llaves;
    QList<QCheckBox*> listChk;
    QList<QLabel*> listCols;
    QList<QComboBox*> list_cbCampos; // ---- cb = QComboBox
    bool insertar_fila(QStringList lista_camposInser, QStringList valores, QList<int> indicesInsertar);
};

#endif // UI_IMPORT_CSV_H
