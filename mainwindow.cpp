#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QtSql/QSqlQuery>
#include <QStringListModel>
#include <QtSql/QSqlError>
#include <QTableWidget>
#include <QtSql/QSqlRecord>
#include "ui_eliminarregistro.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabTablas->clear();
    ui->listTablas->clear();
    ui->listTablas->scrollToBottom();
    ui->listTablas->scrollToTop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAbrir_clicked()
{
    abrir_db();
}

bool MainWindow::abrir_db(){
    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        QMessageBox::critical(this,"Error","Driver QSQLITE no disponible...");
        return  false;
    }

    QString archi = QFileDialog::getOpenFileName(this,tr("DB SQLITE"),QDir::rootPath(),tr("Todos los archivos (*.*)"));
    if(archi != ""){
        ui->txtRutaDb->setText(archi);
    }
    else{
        QMessageBox::critical(this,"Error fatal","Error al abrir base de datos...");
        return false;
    }

    ui->listTablas->clear();
    ui->tabTablas->clear();
    this->tablasClickadas.clear();
    this->listModel.clear();
    this->listTables.clear();
    this->db.close();
    this->listaTiposCampos.clear();

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(ui->txtRutaDb->text());

    if(!db.open()){
        QMessageBox::warning(this,"Error fatal","Error al abrir base de datos | " + db.lastError().text());
        ui->txtRutaDb->setText("");
        db.close();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM sqlite_master WHERE type = 'table' ORDER BY name ASC");
    if(!query.exec()){
        QMessageBox::warning(this,"Error","Error al listar tablas de la base de datos...");
        db.close();
        return false;
    }
    while(query.next()){
        ui->listTablas->addItem(query.value("name").toString());
    }
    //db.close();
    return true;
}

void MainWindow::on_btnCerrar_clicked()
{
    this->cerrarConexiones();
}

void MainWindow::on_listTablas_itemDoubleClicked(QListWidgetItem *item)
{
    if(item->isSelected()){
        //db = QSqlDatabase::addDatabase("QSQLITE");
        //db.setDatabaseName(ui->txtRutaDb->text());

        for(int i = 0; i < tablasClickadas.count(); i++){
            if(tablasClickadas.at(i) == item->text()){
                return;
            }
        }

        if(!db.open()){
            QMessageBox::warning(this,"Error fatal","Error al abrir base de datos | " + db.lastError().text());
            ui->txtRutaDb->setText("");
            db.close();
        }
        else{
            QSqlTableModel *model = new QSqlTableModel(this,this->db);
            model->setTable(item->text());
            if(model->select()){
                listModel.append(model);
                QWidget *widget = new QWidget(ui->tabTablas);
                QHBoxLayout *box = new QHBoxLayout(widget);
                QTableView *tableWid = new QTableView(widget);
                box->addWidget(tableWid);
                tableWid->setObjectName(QStringLiteral("tableWid"));
                tableWid->setModel(model);
                tableWid->setContextMenuPolicy(Qt::ActionsContextMenu);
                tableWid->addAction(ui->actionNuevo_registro);
                tableWid->addAction(ui->actionInsertar_desde_archivo_CSV);
                tableWid->addAction(ui->actionEliminar_registro);
                this->listTables.append(tableWid);
                this->tablasClickadas.append(item->text());
                ui->tabTablas->addTab(widget,item->text());
                ui->tabTablas->setCurrentIndex(ui->tabTablas->count() - 1);
                this->enlistarTipos(ui->tabTablas->currentIndex());
                //db.close();
            }
            else{
                QMessageBox::warning(this,"Error", "Error al leer tabla de la base datos...");
                db.close();
            }
        }
    }
}

void MainWindow::on_actionSalir_triggered()
{
    this->cerrarConexiones();
}

void MainWindow::on_actionCerrar_db_triggered()
{
    this->cerrarConexiones();
}

void MainWindow::on_actionNuevo_registro_triggered()
{
    QSqlTableModel *model = this->listModel.at(ui->tabTablas->currentIndex());

    if(!QSqlDatabase::isDriverAvailable("QSQLITE")){
        QMessageBox::critical(this,"Error","Driver QSQLITE no disponible...");
        return;
    }

    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(ui->txtRutaDb->text());

    if(!db.open()){
        QMessageBox::warning(this,"Error fatal","Error al abrir base de datos | " + db.lastError().text());
        ui->txtRutaDb->setText("");
        db.close();
        return;
    }

    QStringList listCampos;
    QStringList listTipos;
    QList<bool> listPk;
    QSqlQuery query(db);
    query.prepare("PRAGMA table_info(" + this->tablasClickadas.at(ui->tabTablas->currentIndex()) + ")");

    if(!query.exec()){
        QMessageBox::critical(this,"Error", "Error al insertar registro...");
        return;
    }

    while(query.next()){
        listCampos.append(query.value("name").toString());
        listTipos.append(query.value("type").toString());
        listPk.append(query.value("pk").toBool());
    }

    query.clear();
    QSqlRecord record = model->record();

    for(int i = 0; i < listCampos.count(); i++){
        if((get_tipoCampo(listTipos.at(i)) == "VARCHAR") && (listPk.at(i))){
            record.setValue(i,"1");
        }
        else if(get_tipoCampo(listTipos.at(i)) == "INTEGER"){
            record.setValue(i,1);
        }
        else if((get_tipoCampo(listTipos.at(i)) == "VARCHAR") && (!listPk.at(i))){
            record.setValue(i,"TEXTO");
        }
        else if(get_tipoCampo(listTipos.at(i)) == "TEXT"){
            record.setValue(i,"TEXTO");
        }
        else if(get_tipoCampo(listTipos.at(i)) == "BOOLEAN"){
            record.setValue(i, false);
        }
        else if(get_tipoCampo(listTipos.at(i)) == "FLOAT"){
            record.setValue(i,0.00);
        }
        else if(get_tipoCampo(listTipos.at(i)) == "NUMERIC"){
            record.setValue(i,0);
        }
    }

    if(model->insertRecord(-1,record)){
        QMessageBox::information(this,"Exito","Fila insertada exitosamente");
    }
    else{
        QMessageBox::warning(this,"Error", "Error al insertar nuevo registro...");
    }
}

void MainWindow::on_actionAbrir_db_triggered()
{
    abrir_db();
}

QString MainWindow::get_tipoCampo(QString tCampo)
{
    if(tCampo.contains("VARCHAR", Qt::CaseInsensitive)){
        return "VARCHAR";
    }
    else{
        return tCampo;
    }
}

void MainWindow::cerrarConexiones()
{
    ui->listTablas->clear();
    ui->tabTablas->clear();
    ui->txtRutaDb->setText("");
    this->tablasClickadas.clear();
    this->listModel.clear();
    this->listTables.clear();
    this->db.close();
    this->listaTiposCampos.clear();
}

void MainWindow::enlistarTipos(int indexTabla)
{
    QStringList listTipos;

    QSqlQuery query(this->db);

    query.prepare("PRAGMA table_info(" + this->tablasClickadas.at(indexTabla) + ")");

    if(!query.exec()){
        QMessageBox::critical(this,"Error", "Error al insertar registro...");
        return;
    }

    while(query.next()){
        //listCampos.append(query.value("name").toString());
        listTipos.append(query.value("type").toString());
        //listPk.append(query.value("pk").toBool());
    }

    query.clear();
    this->listaTiposCampos.append(listTipos);
}

void MainWindow::on_actionEliminar_registro_triggered()
{
    QSqlTableModel *model = this->listModel.at(ui->tabTablas->currentIndex());
    QMessageBox msgBox;
    msgBox.setText("<h3>Eliminar registro</h3>");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setInformativeText("¿Desea eliminar el registro seleccionado?");
    if(msgBox.exec() == QMessageBox::Cancel){
        return;
    }
    else{
        this->eliminar = new UI_EliminarRegistro(this->db,this->tablasClickadas.at(ui->tabTablas->currentIndex()),this->listaTiposCampos.at(ui->tabTablas->currentIndex()),model,this);
        this->eliminar->setHidden(true);
        this->eliminar->setFocus();
        this->eliminar->exec();
        model->select();
    }
}
