#include <stdio.h>
#include <string>
#include <string.h>
#include <fstream>
#include "iostream"
#include "./analizador/driver.h"
std::string verExec(std::string cadena);
std::string obtenerTexto(std::string path);
std::string eliminarEspacios(std::string cadena);

static int iterador = 1;

using namespace std;

int main() {
	std::string aux = "";
	Driver driver;
	bool activo = true;
	std::string cadena;
	do{
		cout<<"Ingrese un comando: ";
		getline(cin,cadena);
		if(cadena.compare("salir")==0){
			activo = false;
		}else{
			std::string aux = "";
    		for (int i = 0; i < cadena.length(); i++) {
        		aux += toupper(cadena[i]);
    		}
			if(cadena.find("exec")<100){
				cadena = verExec(cadena);
			}
			driver.parse(cadena);
		}
	}while(activo);
	return 0;
}

std::string verExec(std::string cadena){
	std::string aux = "";
    for (int i = 0; i < cadena.length(); i++) {
       	aux += toupper(cadena[i]);
    }
	cadena = cadena.replace(aux.find("EXEC"),4,""); aux="";
    for (int i = 0; i < cadena.length(); i++) {
       	aux += toupper(cadena[i]);
    }
	cadena = cadena.replace(aux.find("-"),aux.find("-"),""); aux="";
	for (int i = 0; i < cadena.length(); i++) {
       	aux += toupper(cadena[i]);
    }
	cadena = cadena.replace(aux.find("PATH"),aux.find("PATH")+3,""); aux="";
	for (int i = 0; i < cadena.length(); i++) {
       	aux += toupper(cadena[i]);
    }
	cadena = cadena.replace(aux.find("="),aux.find("="),""); aux="";
	for (int i = 0; i < cadena.length(); i++) {
       	aux += toupper(cadena[i]);
    }
	if(cadena.find("\"")<100){
        cadena=cadena.substr(cadena.find("\"")+1,cadena.rfind("\"")-2);
    }else{
		cadena = eliminarEspacios(cadena);
	}
	return obtenerTexto(cadena);
}

std::string eliminarEspacios(std::string cadena){
	std::string aux = cadena;
	for(int i = 0; i<aux.length();i++){
		if(cadena.find(" ")<100){
			cadena=cadena.replace(cadena.find(" "),cadena.find(" ")+1,"");
		}
	}
	return cadena;
}

std::string obtenerTexto(std::string path){
	std::string textoFinal = "", textoActual;
    ifstream archivo;
    archivo.open(path,ios::in);
    if(!archivo.fail()){
        while(!archivo.eof()){
            getline(archivo,textoActual);
            textoFinal+=" "; textoFinal += textoActual;
        }
        return textoFinal;
    }else{
        cout<<"Error en metodo correrComandoEXEC, no se pudo abrir el archivo"<<endl;
		return "";
	}
}