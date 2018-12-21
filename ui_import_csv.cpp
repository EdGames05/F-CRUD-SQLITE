#include "ui_import_csv.h"
#include "ui_ui_import_csv.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QHBoxLayout>

ui_import_csv::ui_import_csv(QString nombre_tabla,QStringList tipos,QList<bool> llaves,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ui_import_csv)
{
    ui->setupUi(this);
    this->tabla = nombre_tabla;
    this->tipos = tipos;
    this->llaves = llaves;
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

    for(int i = 0; i < listCols.size(); i++){
        //------------ Aquí añado los controles dinamicos
        QHBoxLayout *capaHor = new QHBoxLayout();
        QLabel *columna = new QLabel(listCols.at(i));
        QCheckBox *chkCol = new QCheckBox();
        chkCol->setObjectName(listCols.at(i));
        capaHor->addWidget(columna);
        capaHor->addWidget(chkCol);

        this->listChk.append(chkCol);
        this->listCols.append(columna);

        //-------- Añadir ya visualmente
        ui->vertLayout->addLayout(capaHor);
    }

    /*while(!archi.atEnd()){
        linea = archi.readLine();
        if(primeraVuelta){

        }
        primeraVuelta = true;
    }*/
}
