#include "ui_eliminarregistro.h"
#include "ui_ui_eliminarregistro.h"
#include <QSqlTableModel>
#include "funciones_comun.h"
#include <QMessageBox>

using namespace funciones_comun;

UI_EliminarRegistro::UI_EliminarRegistro(QString nombreTabla,QStringList listTipos,QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_EliminarRegistro)
{
    ui->setupUi(this);
    this->model = model;
    this->listTipos = listTipos;
    this->nombreTabla = nombreTabla;
    for(int i = 0; i < model->columnCount(); i++){
        ui->ddl_campos->addItem(model->headerData(i,Qt::Orientation::Horizontal).toString());
    }

    if(ui->ddl_campos->count() > 0){
        ui->txtPrevisualizacion->setText(consulta + this->nombreTabla + " WHERE " + ui->ddl_campos->itemText(0) + " = ");
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

void UI_EliminarRegistro::on_ddl_campos_currentIndexChanged(int index)
{
    ui->txtPrevisualizacion->setText(consulta + this->nombreTabla + " WHERE " + ui->ddl_campos->itemText(index) + " = ");
}

void UI_EliminarRegistro::on_txtIgual_textChanged(const QString &arg1)
{
    const QString txtActual = consulta + this->nombreTabla + " WHERE " + ui->ddl_campos->currentText() + " = ";
    if(coincide_tipoDato(this->listTipos.at(ui->ddl_campos->currentIndex()),arg1)){

        ui->txtPrevisualizacion->setText(txtActual + arg1);
    }
    else{
        ui->txtPrevisualizacion->setText(txtActual);
    }
}
