#include "ui_import_csv.h"
#include "ui_ui_import_csv.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <qrandom.h>
#include <QDate>

ui_import_csv::ui_import_csv(QSqlDatabase db,QStringList listCampos,QString nombre_tabla,QStringList tipos,QList<bool> llaves,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_import_csv)
{
    ui->setupUi(this);
    this->tabla = nombre_tabla;
    this->tipos = tipos;
    this->llaves = llaves;
    this->listCampos = listCampos;
    this->db = db;
}

ui_import_csv::~ui_import_csv()
{
    delete ui;
}

void ui_import_csv::on_btnArchivo_clicked()
{
    const QString nombreArchi = QFileDialog::getOpenFileName(this,tr("Abrir CSV"),QDir::rootPath(),tr("CSV *.csv"));
    if(nombreArchi == "")
    {
        return;
    }

    ui->txtRuta->setText(nombreArchi);

    QFile archi(nombreArchi);
    if(!archi.exists()){
        QMessageBox::warning(this,"Error","El archivo no existe...");
        return;
    }
    if(!archi.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this,"Error fatal","Error al abrir archivo...");
        return;
    }


    QString linea = archi.readLine();
    linea.replace("\n","",Qt::CaseInsensitive);
    const QStringList listCols = linea.split(",");
    this->listChk.clear();
    this->listCols.clear();
    this->list_cbCampos.clear();

    for(int i = 0; i < listCols.size(); i++){
        //------------ Aquí añado los controles dinamicos
        QHBoxLayout *capaHor = new QHBoxLayout();
        QLabel *columna = new QLabel(listCols.at(i));
        QCheckBox *chkCol = new QCheckBox();
        QComboBox *cb_campos = new QComboBox();

        cb_campos->addItems(this->listCampos);
        chkCol->setObjectName(listCols.at(i));
        capaHor->addWidget(columna);
        capaHor->addWidget(chkCol);
        capaHor->addWidget(cb_campos);

        this->listChk.append(chkCol);
        this->listCols.append(columna);
        this->list_cbCampos.append(cb_campos);

        //-------- Añadir ya visualmente
        ui->vertLayout->addLayout(capaHor);
    }

    archi.close();
}

void ui_import_csv::on_btnImportar_clicked()
{
    if(ui->txtRuta->text().isEmpty()){
        QMessageBox::warning(this,"Error","Debe abrir un archivo antes de importar...");
    }
    else{

        int marca = -1;

        for(int i = 0; i < listChk.size(); i++){
            if(listChk.at(i)->isChecked()){
                marca++;
            }
        }

        if(marca > (listCampos.size() - 1)){
            QMessageBox::warning(this,"Error","No existen campos suficientes para las columnas seleccionadas");
        }
        else{

            QFile archi(ui->txtRuta->text());
            if(!archi.exists()){
                QMessageBox::warning(this,"Error","El archivo no existe...");
                return;
            }
            if(!archi.open(QIODevice::ReadOnly | QIODevice::Text)){
                QMessageBox::critical(this,"Error fatal","Error al leer archivo...");
                return;
            }

            //----------- Buscar columnas seleccionadas
            QList<int> list_indicesCols;  //------ Columnas del archiv csv
            QStringList list_ColsInsertar; //------ Columnas de la base de datos
            for(int i = 0; i < listChk.count(); i++){
                if(listChk.at(i)->isChecked()){
                    list_indicesCols.append(i);
                    //------------ Insertar columna seleccionada
                    list_ColsInsertar.append(list_cbCampos.at(i)->currentText());
                }
            }


            //----------- Leer archivo e insertar columnas seleccionadas
            bool primeraVuelta = false;
            QString linea;

            while(!archi.atEnd()){
                linea = archi.readLine();
                linea.replace("\n","",Qt::CaseInsensitive);
                QStringList listCols = linea.split(",");

                if(primeraVuelta){
                    if(!this->insertar_fila(list_ColsInsertar,listCols,list_indicesCols)){
                        QMessageBox::critical(this,"Error","Error al insertar registros");
                        return;
                    }
                }
                primeraVuelta = true;
            }
            archi.close();
            QMessageBox::information(this,"Exito","Información importada exitosamente...");
            this->close();
        }
    }
}

bool ui_import_csv::insertar_fila(QStringList lista_camposInser, QStringList valores, QList<int> indicesInsertar)
{
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        QMessageBox::critical(this,"Error","Driver QSQLITE no disponible...");
        return false;
    }
    else{
        //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        //db.setDatabaseName(ui->txtRuta->text());

        if(!this->db.open()){
            QMessageBox::critical(this,"Error fatal","Se genero un error al insertar en fila en base de datos (Base de datos no abierta)...");
            db.close();
            return false;
        }
        else{
            QSqlQuery query(this->db);
            QString prepareQuery = "INSERT INTO " + this->tabla + "(";

            for(int i = 0; i < this->listCampos.size(); i++){
                if(i == (listCampos.size() - 1)){
                    prepareQuery += listCampos.at(i);
                }
                else{
                    prepareQuery += listCampos.at(i) + ",";
                }
            }

            prepareQuery += ") VALUES(";
            bool entro = false;

            for(int i = 0; i < listCampos.size(); i++){
                entro = false;
                for(int j = 0; j < lista_camposInser.size(); j++){
                    //----------- hace esto si encuentra el campo
                    if(listCampos.at(i) == lista_camposInser.at(j)){

                        if(tipos.at(i).toUpper().contains("INTEGER",Qt::CaseInsensitive)){
                            prepareQuery += valores.at(indicesInsertar.at(j));
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("VARCHAR",Qt::CaseInsensitive)){
                            prepareQuery += "'" + valores.at(indicesInsertar.at(j)) + "'";
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("BOOLEAN",Qt::CaseInsensitive)){
                            prepareQuery += valores.at(indicesInsertar.at(j));
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("FLOAT",Qt::CaseInsensitive)){
                            prepareQuery += valores.at(indicesInsertar.at(j));
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("DATE",Qt::CaseInsensitive)){
                            prepareQuery += valores.at(indicesInsertar.at(j));
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("REAL",Qt::CaseInsensitive)){
                            prepareQuery += valores.at(indicesInsertar.at(j));
                            entro = true;
                        }
                        else
                        if(tipos.at(i).toUpper().contains("TEXT",Qt::CaseInsensitive)){
                            prepareQuery += "'" + valores.at(indicesInsertar.at(j)) + "'";
                            entro = true;
                        }

                        qDebug() << prepareQuery;
                        prepareQuery += ",";

                        break;
                    }
                }
                //------------ Hace esto si no lo encuentra

                if(tipos.at(i).toUpper().contains("INTEGER",Qt::CaseInsensitive) && !entro){
                    prepareQuery += QString::number(qrand() % 9999999 + 1);
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("VARCHAR",Qt::CaseInsensitive) && !entro){
                    prepareQuery += "'T'";
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("BOOLEAN",Qt::CaseInsensitive) && !entro){
                    prepareQuery += "FALSE";
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("FLOAT",Qt::CaseInsensitive) && !entro){
                    prepareQuery += QString::number(qrand() % 9999999 + 1);
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("DATE",Qt::CaseInsensitive) && !entro){
                    prepareQuery += "date('now')";
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("REAL",Qt::CaseInsensitive) && !entro){
                    prepareQuery += QString::number(qrand() % 9999999 + 1);
                    prepareQuery += ",";
                }
                else
                if(tipos.at(i).toUpper().contains("TEXT",Qt::CaseInsensitive) && !entro){
                    prepareQuery += "'T'";
                    prepareQuery += ",";
                }
            }

            //----------------- En esta parte quitamos la , que no debería ir
            prepareQuery.remove(prepareQuery.length() - 1, prepareQuery.length() - 1);
            prepareQuery += ");";

            if(!query.exec(prepareQuery)){
                QMessageBox::critical(this,"Error", query.lastError().text());
                return false;
            }
            return true;
        }
    }
}
