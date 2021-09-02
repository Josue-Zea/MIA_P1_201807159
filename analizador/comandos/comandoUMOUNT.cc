#include <iostream>
void correrComandoUMOUNT(std::string id);

void comandUMOUNT(std::string argumentos){
    std::string actual = "", actL="", id="";
    bool exito = true;
    //Se colocan los parametros separados y en el orden deseado
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if(actual.find("ID")==0){
                id=actual.replace(0,3,"");
            }else{
                exito = false;
            }
            actual="";
        }else{
            actual+=argumentos[i];
        }
    }
    if(exito==true){
        correrComandoUMOUNT(id);
        mostrarPartMontadas();
    }else{
        cout<<"Error en metodo umount, hay parametros invalidos";
    }
}

void correrComandoUMOUNT(std::string id){
    bool resultado = false;
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].id.compare(id)==0){
            ParMont.erase(ParMont.begin()+i);
            resultado = true;
            break;
        }
    }
    if(!resultado){
        cout<<"Error en comando umount, no se encontro la particion con id: "<<id<<endl;
    }
}