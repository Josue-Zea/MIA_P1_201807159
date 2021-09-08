#include <iostream>
#include <dirent.h>
using namespace std;
bool verificarArchivo(std::string path);
void correrComandoRMDISK(std::string path);
bool eliminarDisco(std::string path);

void comandRMDISK(std::string argumentos){
    std::string actual = "", path = "", actL="";
    bool exito = true;
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if(actual.find("PATH")==0){
                path=actual.replace(0,5,"");
                if(path.find("\"")==0){
                    path =  path.substr(1,path.size()-2);
                }
            }else{
                exito = false;
            }
            actual="";
        }else{
            actual+=argumentos[i];
        }
    }
    if(exito==true){
        correrComandoRMDISK(path);
    }else{
        cout<<"Parametros invalidos en comando mkdisk, no se creo el disco";
    }
}

void correrComandoRMDISK(std::string path){
    if(verificarArchivo(path)){
       if(eliminarDisco(path)){
           cout<<"Se elimino el disco con exito"<<endl;
       }else{
           cout<<"No se elimino el disco"<<endl;
       }
    }else{
        cout<<"Error en rmdisk, no existe el disco"<<endl;
    }
}

bool verificarArchivo(std::string path){
    FILE *archivo;
    if(archivo = fopen(path.c_str(),"r")){
        fclose(archivo);
        return true;
    }else{
        return false;
    }
}

bool eliminarDisco(std::string path){
    std::string cadena = " ";
    cout<<"¿Esta seguro que desea eliminar el disco (S/N)?: ";
    getline(cin,cadena);
    cadena[0]=toupper(cadena[0]);
    if(cadena.compare("S")==0){
        remove(path.c_str());
    }
    if(!verificarArchivo(path)){
        return true;
    }else{
        return false;
    }
}