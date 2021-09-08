#include <iostream>
#include <vector>
void correrComandoMOUNT(std::string path,std::string name);
int verificarDiscos(std::string path, std::string nameDisc);
int verificarLetra(std::string nameDisc);
bool verificarExisteMount(std::string path, std::string name);
void mostrarPartMontadas();

//Struct usado para cada particion montada en el sistema
struct PartMontada{
    std::string path;
    std::string name;       //Nombre de la particion
    std::string disco;      //Nombre del disco
    int val;                //Numero de las particiones para este disco
    int valLetra;
    std::string letra;
    int status;
    char part_fit;
    int part_start;
    int part_size;
    std::string tipo;
    std::string id;
};

//Arreglo global que contiene las particiones montadas
std::vector<PartMontada> ParMont;

//Variable usada para definir que numero se le asigna al mount de cada disco
int iterador = 1;

void comandMOUNT(std::string argumentos){
    std::string actual = "", path = "", actL="", name="";
    bool exito = true;
    //Se colocan los parametros separados y en el orden deseado
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if(actual.find("PATH")==0){
                path=actual.replace(0,5,"");
                //Si la ruta contiene comillas significa que tiene espacios, se eliminan comillas
                if(path.find("\"")==0){
                    path = path.substr(1,path.size()-2);
                }
            }else if(actual.find("NAME")==0){
                name=actual.replace(0,5,"");
                //Eliminamos comillas si es que vienen en el nombre
                if(path.find("\"")==0){
                    name = name.substr(1,name.size()-2);
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
        correrComandoMOUNT(path,name);
    }else{
        cout<<"Error en metodo rep, hay parametros invalidos";
    }
}

void correrComandoMOUNT(std::string path,std::string name){
    //Carnet: 201807159
    std::string discName = "";
    int val = 0;
    val = path.rfind("/");
    discName=path.substr(val+1,path.size());
    if(verificarExisteMount(path,name)){
        cout<<"Error al montar particion: "<<name<<" del disco :"<<discName<<" ya se encuentra en memoria"<<endl;
        return;
    }
    int par = -1;
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    for(int i=0; i<4; i++){//Verificamos en las particiones basicas si existe una con el nombre buscado
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            par=i;
            break;
        }
    }
    if(par!=-1){
        PartMontada montar;
        montar.path=path;
        montar.name=name;
        montar.disco=discName;
        montar.val=verificarDiscos(path,discName);
        montar.valLetra=verificarLetra(discName);
        montar.letra=char(montar.valLetra);
        montar.status=mbr.mbr_partitions[par].part_status;
        montar.part_fit=mbr.mbr_partitions[par].part_fit;
        montar.tipo = "P";
        montar.part_start=mbr.mbr_partitions[par].part_start;
        montar.part_size=mbr.mbr_partitions[par].part_size;
        montar.id = "59"+to_string(montar.val)+montar.letra;
        ParMont.push_back(montar);
    }
    if( par == -1 ){//Se busca la existencia de particiones extendidas
        int parExt = -1;
        for(int i = 0; i<4;i++){
            if(mbr.mbr_partitions[i].part_type=='E'){
                parExt = i;
                break;
            }
        }
        if( parExt ==-1 ){//No existen particiones extendidas
            cout<<"Error, no existe una particion llamada: "<<name<<" en el disco : "<<discName<<endl;
            return;
        }
        bool activo = true; EBR ebr;
        int start = mbr.mbr_partitions[parExt].part_start;
        while(activo){
            FILE *archebr;
            archebr=fopen(path.c_str(),"rb+");
            fseek(archebr,start, SEEK_SET);
            fread(&ebr,sizeof(EBR),1,archebr);
            fclose(archebr);
            if(ebr.part_status==1){
                if(strcmp(ebr.part_name,name.c_str())==0){
                    PartMontada montar;
                    montar.path=path;
                    montar.name=name;
                    montar.disco=discName;
                    montar.val=verificarDiscos(path,discName);
                    montar.valLetra=verificarLetra(discName);
                    montar.letra=char(montar.valLetra);
                    montar.status=ebr.part_status;
                    montar.part_fit=ebr.part_fit;
                    montar.tipo = "L";
                    montar.part_start=ebr.part_start;
                    montar.part_size=ebr.part_size;
                    montar.id = "59"+to_string(montar.val)+montar.letra;
                    ParMont.push_back(montar);
                    activo =  false;
                }else{
                    start = ebr.part_next;
                }
            }else{
                activo=false;
            }
        }
    }
    mostrarPartMontadas();
}

bool verificarExisteMount(std::string path, std::string name){
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].path.compare(path)==0 && ParMont[i].name.compare(name)==0){
            return true;
        }
    }
    return false;
}

void mostrarPartMontadas(){
    cout<<"------------------------------------------------------"<<endl;
    for(int i = 0; i<ParMont.size();i++){
        cout<<ParMont[i].path<<"|"<<ParMont[i].name<<"|"<<ParMont[i].id<<endl;
    }
    cout<<"------------------------------------------------------"<<endl;
}

int verificarDiscos(std::string path, std::string nameDisc){
    int disc = 1;
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].disco.compare(nameDisc)==0){
            return ParMont[i].val;
        }
    }
    disc = iterador;
    iterador = iterador+1;
    return disc;
}

int verificarLetra(std::string nameDisc){
    int actual = 97;
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].disco.compare(nameDisc)==0){
            if(ParMont[i].valLetra == actual){
                actual = actual + 1;
            }
        }
    }
    return actual;
}