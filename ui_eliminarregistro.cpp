#include "ui_eliminarregistro.h"
#include "ui_ui_eliminarregistro.h"
#include <QSqlTableModel>
#include <QtSql/QSqlDatabase>
#include "funciones_comun.h"
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

using namespace funciones_comun;

UI_EliminarRegistro::UI_EliminarRegistro(QSqlDatabase db,QString nombreTabla,QStringList listTipos,QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UI_EliminarRegistro)
{
    ui->setupUi(this);
    this->model = model;
    this->listTipos = listTipos;
    this->nombreTabla = nombreTabla;
    this->db = db;
    ui->gb_tabla->setTitle(nombreTabla);
    ui->gb_avanzado->setEnabled(false);
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

void UI_EliminarRegistro::on_rb_practico_clicked(bool checked)
{
    ui->gb_avanzado->setEnabled(!checked);
}

void UI_EliminarRegistro::on_btnAceptar_clicked()
{
    //----------------- Borra un registro con una consulta simple ya pre establecida --------------------------------
    if(!ui->gb_avanzado->isEnabled()){
        if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
            QMessageBox::warning(this,"Advertencia","Driver SQLite no disponible...");
            return;
        }

        QSqlQuery query(this->db);
        const QString que = ui->txtPrevisualizacion->text();
        query.prepare(que);
        if(!query.exec()){
            QMessageBox::warning(this,"Error", "Error al borrar registro | Error: " + query.lastError().text());
            return;
        }
        QMessageBox::information(this,"Exito","Registro eliminado exitosamente...");
        //------- Está sentencia cierra la ventana
        this->close();
    }
    //---------------------------------------------------------------------------------------------------------------
}

void UI_EliminarRegistro::on_btnEjecutar_clicked()
{
    //-------------------- Ejecutar consulta de la opcion de avanzado
    QSqlQuery query(this->db);
    query.prepare(ui->txtQuery->toPlainText());
    if(!query.exec()){
        QMessageBox::warning(this,"Error", "Error en consulta: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this,"Exito","Consulta ejecutada exitosamente...");
}

void UI_EliminarRegistro::on_rb_avanzado_clicked(bool checked)
{
    ui->gb_avanzado->setEnabled(checked);
}
