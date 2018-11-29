#ifndef FUNCIONES_COMUN_H
#define FUNCIONES_COMUN_H
#include <QString>
#include <QStringList>
#include <QChar>

namespace funciones_comun{

bool validar_numeros(QString texto){
    QChar carac;
    for(int i = 0; i < texto.length(); i++){
        carac = texto.at(i);
        if(!carac.isNumber()){
            return false;
        }
    }
    return true;
}

bool validar_booleano(QString texto){
    texto = texto.toUpper();
    if(texto.contains("TRUE",Qt::CaseInsensitive) || texto.contains("FALSE",Qt::CaseInsensitive)){
        return true;
    }
    return false;
}


QString get_tipoCampo(QString texto){
    if(texto.contains("VARCHAR", Qt::CaseInsensitive)){
        return "VARCHAR";
    }
    else{
        return texto;
    }
}

bool coincide_tipoDato(QString tipoCampo,QString texto){
    if(get_tipoCampo(tipoCampo) == "INTEGER"){
        if(!validar_numeros(texto)){
            return false;
        }
        return true;
    }
    else if(get_tipoCampo(tipoCampo) == "BOOLEAN"){
        if(!validar_booleano(texto)){
            return false;
        }
        return true;
    }
}

bool coincide_tabla(const QString tabla, QString query){
    QString aux = "";
    return false;
}

QStringList separar_palabras(QString txt){
    QString aux = "";
    QStringList listWords;
    for(int i = 0; i < txt.length(); i++){
        if(txt.at(i) != "\n" && txt.at(i) != " "){
            aux += txt.at(i);
        }
        else if(txt.at(i) == " " || txt.at(i) == "\n"){
            if(txt != ""){
                listWords.append(aux);
                aux = "";
            }
        }
    }
    return listWords;
}

template <typename T>
T devolver_id_siguiente(QString tipoCampo, QString ant){
    if(tipoCampo == "VARCHAR"){

        if(ant.isEmpty()){
            return "1";
        }
        else if(!validar_numeros(ant)){
            return "00000000000X";
        }

        QChar cAux;

        for(int i = ant.length() - 1; i == 0; i++){
            cAux = ant.at(i);
            if(cAux == "9"){
                ant.replace(i,i,"0");

                if(ant.at(i - 1) != "\n" || ant.at(i - 1) != NULL || ant.at(i - 1) != nullptr){
                }

            }
            else if(cAux != "9"){
                int num = cAux.digitValue() + 1;
                ant.replace(i,i,QString::number(num));
            }
        }

    }
    else if(tipoCampo == "INTEGER"){

        if(ant.isEmpty()){
            return 1;
        }

        return ant.toInt() + 1;
    }
}

}

#endif // FUNCIONES_COMUN_H
