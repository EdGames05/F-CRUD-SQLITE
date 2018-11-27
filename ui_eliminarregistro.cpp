#include "ui_eliminarregistro.h"
#include "ui_ui_eliminarregistro.h"
#include <QSqlTableModel>

UI_EliminarRegistro::UI_EliminarRegistro(QList<QStringList> listTipos,QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_EliminarRegistro)
{
    ui->setupUi(this);
    this->model = model;

    for(int i = 0; i == model->rowCount(); i++){
        ui->ddl_campos->addItem(model->headerData(i,Qt::Orientation::Horizontal).toString());
    }

}

UI_EliminarRegistro::~UI_EliminarRegistro()
{
    delete ui;
}

QString UI_EliminarRegistro::get_tipoCampo(QString tCampo)
{
    if(tCampo.contains("VARCHAR", Qt::CaseInsensitive)){
        return "VARCHAR";
    }
    else{
        return tCampo;
    }
}
