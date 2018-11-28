#ifndef FUNCIONES_COMUN_H
#define FUNCIONES_COMUN_H
#include <QString>
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

}

#endif // FUNCIONES_COMUN_H
