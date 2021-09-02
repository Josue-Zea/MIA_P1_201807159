#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
std::string datosMBR(std::string path);
std::string datosEBR(std::string path, int start);
void correrComandoREP(std::string path,std::string name,std::string id,std::string ruta,std::string root);
std::string buscarPathDisco(std::string id);
void reporteMBR(std::string path, std::string id);

void comandREP(std::string argumentos){
    std::string actual = "", path = "", actL="", name="", id="", ruta = "", root="";
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
            }else if(actual.find("ID")==0){
                id=actual.replace(0,3,"");
            }else if(actual.find("RUTA")==0){
                ruta=actual.replace(0,5,"");
            }else if(actual.find("ROOT")==0){
                root=actual.replace(0,5,"");
            }else{
                exito = false;
            }
            actual="";
        }else{
            actual+=argumentos[i];
        }
    }
    if(exito==true){
        correrComandoREP(path,name,id,ruta,root);
    }else{
        cout<<"Error en metodo rep, hay parametros invalidos";
    }
}

void correrComandoREP(std::string path,std::string name,std::string id,std::string ruta,std::string root){
    for(int i = 0; i<name.length();i++){
        name[i] = toupper(name[i]);
    }
    if(name.compare("MBR")==0){
        reporteMBR(path, id);
    }
}

void reporteMBR(std::string path, std::string id){
    std::string dirDisc = buscarPathDisco(id);
    if(dirDisc.compare("")==0){
        cout<<"Error, no se encontro un id: "<<id<<" no se puede ubicar el disco"<<endl;
        return;
    }
    std::string var = datosMBR(dirDisc);
    ofstream file;
    file.open("/home/report.dot");
    file << var;
    file.close();
    int val = path.rfind("/");
    //Ruta solo con carpetas
    std::string rutaFinal = path.substr(0,val);
    crearCarpetas(rutaFinal);
    std::string instruccion = "dot -Tpng /home/report.dot -o "+path;
    system(instruccion.c_str());
    remove("/home/report.dot");
    cout<<"Reporte mbr generado con exito"<<endl;
}

std::string buscarPathDisco(std::string id){
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].id.compare(id)==0){
            return ParMont[i].path;
        }
    }
    return "";
}

std::string datosMBR(std::string path){
    std::string var = "digraph G {abc [shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    var+="<TR><TD>Nombre</TD><TD>Disco3.dk</TD></TR>\n";
    var+="<TR><TD>MBR_TAMANIO</TD><TD>"+std::to_string(mbr.mbr_tamano)+"</TD></TR>\n";
    var+="<TR><TD>MBR_FECHA_CREACION</TD><TD>"+std::string(mbr.mbr_fecha_creacion)+"</TD></TR>\n";
    std::string aux = "";
    var+="<TR><TD>MBR_DISK_SIGNATURE</TD><TD>"+std::to_string(mbr.mbr_disk_signature)+"</TD></TR>\n";
    aux = mbr.disk_fit;
    var+="<TR><TD>MBR_DISK_FIT</TD><TD>"+aux+"</TD></TR>\n";
    /*De las particiones*/
    for(int i=0;i<4;i++){
        if(mbr.mbr_partitions[i].part_status==1){
            std::string iter = std::to_string(i);
            var+="<TR><TD>PART_STATUS_"+iter+"</TD><TD>"+std::to_string(mbr.mbr_partitions[i].part_status)+"</TD></TR>\n";
            aux = mbr.mbr_partitions[i].part_type;
            var+="<TR><TD>PART_TYPE_"+iter+"</TD><TD>"+aux+"</TD></TR>\n";
            aux = mbr.mbr_partitions[i].part_fit;
            var+="<TR><TD>PART_FIT_"+iter+"</TD><TD>"+aux+"</TD></TR>\n";
            var+="<TR><TD>PART_START_"+iter+"</TD><TD>"+std::to_string(mbr.mbr_partitions[i].part_fit)+"</TD></TR>\n";
            var+="<TR><TD>PART_SIZE_"+iter+"</TD><TD>"+std::to_string(mbr.mbr_partitions[i].part_size)+"</TD></TR>\n";
            var+="<TR><TD>PART_NAME_"+iter+"</TD><TD>"+std::string(mbr.mbr_partitions[i].part_name)+"</TD></TR>\n";
            if(mbr.mbr_partitions[i].part_type=='E'){
                var+= datosEBR(path,mbr.mbr_partitions[i].part_start);
            }
        }
    }
    var+="</TABLE>>];}";
    return var;
}

std::string datosEBR(std::string path, int start){
    std::string var = "", aux="";
    bool activo = true; EBR ebr; int n = 0;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status == 1){
            if(ebr.part_start == -1){
                start = ebr.part_next;
                cout<<"Saltamos esta particion"<<endl;
                continue;
            }
            var+="<TR><TD>EBR_"+std::to_string(n)+"</TD><TD>-------</TD></TR>\n";
            var+="<TR><TD>PART_STATUS</TD><TD>"+std::to_string(ebr.part_status)+"</TD></TR>\n";
            aux = ebr.part_fit;
            var+="<TR><TD>PART_FIT</TD><TD>"+aux+"</TD></TR>\n";
            var+="<TR><TD>PART_START</TD><TD>"+std::to_string(ebr.part_start)+"</TD></TR>\n";
            var+="<TR><TD>PART_SIZE</TD><TD>"+std::to_string(ebr.part_size)+"</TD></TR>\n";
            var+="<TR><TD>PART_NEXT</TD><TD>"+std::to_string(ebr.part_next)+"</TD></TR>\n";
            var+="<TR><TD>PART_NAME</TD><TD>"+std::string(ebr.part_name)+"</TD></TR>\n";
            start = ebr.part_next;
            n = n+1;
        }else{
            activo=false;
        }
    }
    return var;
}