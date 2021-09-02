#include <iostream>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <chrono>
#include <ctime>
#include <cstring>
using namespace std;
void correrComandoMKDISK(std::string size, std::string path, std::string Uval, std::string Fval);
bool crearCarpetasMKDISK(std::string path);
bool crearCarpetas(std::string ruta);
bool VerificarCarpeta(std::string ruta);
bool crearRuta(std::string ruta);
bool crearDisco(std::string path, std::string size,std::string Uval);
bool agregarMbrDisco(std::string path, std::string size,std::string Uval, std::string Fval);
std::string getFecha();

struct Partition{
    int part_status = 0;
    char part_type;
    char part_fit;
    int part_start = 0;
    int part_size = 0;
    char part_name[16];
};

struct MBR{
    int mbr_tamano;
    char mbr_fecha_creacion[24];
    int mbr_disk_signature;
    char disk_fit;
    Partition mbr_partitions[4];
};

void comandMKDISK(std::string argumentos){
    std::string actual = "", path = "", Uval = "M", Fval = "FF", size = "", actL="";
    bool exito = true;
    //Se colocan los parametros separados y en el orden deseado
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if (actual.find("SIZE")==0){
                size=actual.replace(0,5,"");
            }else if(actual.find("PATH")==0){
                path=actual.replace(0,5,"");
                //Si la ruta contiene comillas significa que tiene espacios, se eliminan
                if(path.find("\"")==0){
                    path =  path.substr(1,path.size()-2);
                }
            }else if(actual.find("U")==0){
                Uval=actual.replace(0,2,"");
            }else if(actual.find("F")==0){
                Fval=actual.replace(0,2,"");
            }else{
                exito = false;
            }
            actual="";
        }else{
            actual+=argumentos[i];
        }
    }
    if(exito==true){
        correrComandoMKDISK(size,path,Uval,Fval);
    }else{
        cout<<"Error en metodo comandMKDISK, hay parametros invalidos";
    }
}

void correrComandoMKDISK(std::string size, std::string path, std::string Uval, std::string Fval){
    std::string disco="", rutaFinal="";
    int val = 0;
    val = path.rfind("/");
    //Ruta solo con carpetas
    rutaFinal = path.substr(0,val);
    //Nombre del disco
    disco = path.substr(val+1,path.size());
    crearCarpetas(rutaFinal);
    //Pasamos a mayusculas todo lo que venga dentro de F
    for (int i = 0; i < Fval.length(); i++) {
        Fval[i] = toupper(Fval[i]);
    }
    if(crearDisco(path,size,Uval)){
        if(agregarMbrDisco(path,size,Uval,Fval)){
            cout<<"Disco creado con exito"<<endl;
        }
    }else{
        cout<<"Error en correrComandoMKDISK, no se pudo crear el disco"<<endl;
    }
}

bool crearCarpetas(std::string ruta){
    //Verificamos si las carpetas existen y si no estan se crean
    std::string Letra="", rutaActual="";
    for(int i = 0; i<ruta.size();i++){
        Letra = ruta[i];
        if(Letra.compare("/") == 0 && i != 0){
            if(!VerificarCarpeta(rutaActual)){
                if(!crearRuta(rutaActual)){return false;}
            }
            rutaActual+=Letra;
        }else{
            rutaActual+=Letra;
            if(i==ruta.size()-1){
                if(!VerificarCarpeta(rutaActual)){
                    if(!crearRuta(rutaActual)){return false;}
                }
            }
        }
    }
    return true;
}

bool VerificarCarpeta(std::string ruta){
    //Metodo para verificar si la carpeta existe
    DIR * directorio;
    if(directorio = opendir(ruta.c_str())){
        closedir(directorio);
        return true;
    }else{
        return false;
    }
}

bool crearRuta(std::string ruta){
    int status;
    status = mkdir(ruta.c_str(),0777);
    if (status == 0){
        return true;
    }else{
        return false;
    }
}

bool crearDisco(std::string path, std::string size,std::string Uval){
    char buffer[1024]; for(int i = 0 ; i < 1024 ; i++){buffer[i]='\0';}
    int tamanio = atoi(size.c_str());
    if(Uval.compare("k")==0||Uval.compare("K")==0){tamanio=tamanio; }
    else if(Uval.compare("m")==0||Uval.compare("M")==0){tamanio=tamanio*1024;}
    FILE *fichero;
    fichero = fopen(path.c_str(),"wb");
    for(int i = 0 ; i < tamanio; i++){
        fwrite(&buffer,1024,1,fichero);
    }
    fclose(fichero);
    return true;
}

bool agregarMbrDisco(std::string path, std::string size,std::string Uval, std::string Fval){
    //Creamos el struct de mbr para agregarselo al disco creado
    MBR mbr;
    //Agregamos los atributos al mbr
    int tamano = Uval.compare("m")==0||Uval.compare("M")==0 ? atoi(size.c_str())*1024*1024: atoi(size.c_str())*1024;
    mbr.mbr_tamano = tamano; std::string fecha = getFecha();
    strcpy(mbr.mbr_fecha_creacion,fecha.c_str());
    if(Fval.compare("FF")==0){
        mbr.disk_fit = 'F';
    }else if(Fval.compare("WF")==0){
        mbr.disk_fit = 'W';
    }else if(Fval.compare("BF")==0){
        mbr.disk_fit = 'B';
    }
    //Agregamos un valor aleatorio entre 0 y 100 para identificador del disco
    mbr.mbr_disk_signature = rand()%(101)+0;
    //Creamos un struct de tipo particion que incluiremos al mbr
    Partition partition;
    /*partition.part_status='0';//status inactivo*/
    partition.part_status=0;
    partition.part_type='-';
    partition.part_fit='-';
    partition.part_start=0;
    partition.part_size=0;
    partition.part_name[0] = '\0';
    for(int i = 0 ; i < 4 ; i ++){
        mbr.mbr_partitions[i] = partition;
    }

    FILE *archivo;
    archivo = fopen(path.c_str(), "rb+");
    if(archivo != NULL){
        fseek(archivo,0,SEEK_SET);
        fwrite(&mbr, sizeof(mbr), 1, archivo);
        fclose(archivo);
        return true;
    }else{
        cout<<"Error en metodo agregarMbrDisco, no se pudo acceder al disco"<<endl;
        return false;
    }
}

std::string getFecha(){
    auto start = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(start);
    return std::ctime(&end_time);
}