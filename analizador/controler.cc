#ifndef _controler_
#define _controler_
#include <iostream>
#include <string>
/* Incluimos los archivos que traen los comandos y los struct*/
#include "./comandos/comandoMKDISK.cc"
#include "./comandos/comandoRMDISK.cc"
#include "./comandos/comandoFDISK.cc"
#include "./comandos/comandoMOUNT.cc"
#include "./comandos/comandoUMOUNT.cc"
#include "./comandos/comandoREP.cc"
#include "./comandos/comandoMKFS.cc"
/*Prototipos de funciones*/
void comandMKDISK(std::string argumentos);
void comandRMDISK(std::string argumentos);
void comandFDISK(std::string argumentos);
void comandREP(std::string argumentos);
void comandMOUNT(std::string argumentos);
void comandUMOUNT(std::string argumentos);
void comandMKFS(std::string argumentos);
//

using namespace std;
void crearComandos(std::string comando, std::string argumentos){
    if(comando.compare("mkdisk")==0){
        comandMKDISK(argumentos);
    }else if(comando.compare("rmdisk")==0){
        comandRMDISK(argumentos);
    }else if(comando.compare("fdisk")==0){
        comandFDISK(argumentos);
    }else if(comando.compare("pause")==0){
        cout<<"Presione enter para continuar..."<<endl;
        cin.get();
    }else if(comando.compare("rep")==0){
        comandREP(argumentos);
    }else if(comando.compare("mount")==0){
        comandMOUNT(argumentos);
    }else if(comando.compare("umount")==0){
        comandUMOUNT(argumentos);
    }else if(comando.compare("mkfs")==0){
        comandMKFS(argumentos);
    }else{
        cout<<"Comando incorrecto"<<endl;
    }
}
#endif