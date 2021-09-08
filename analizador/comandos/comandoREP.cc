#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>
std::string datosMBR(std::string path);
std::string datosEBR(std::string path, int start);
void correrComandoREP(std::string path,std::string name,std::string id,std::string ruta,std::string root);
std::string buscarPathDisco(std::string id);
void reporteMBR(std::string path, std::string id);
void reporteDISK(std::string path, std::string id);
std::string codeDiskLogicas(std::string path, int start, double size);
void crearReporte(std::string codigoDot, std::string path);
void reporteSB(std::string path, std::string id);

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
    }else if(name.compare("DISK")==0){
        reporteDISK(path, id);
    }else if(name.compare("SB")==0){
        reporteSB(path, id);
    }else{
        cout<<"No se reconoce el tipo de reporte a realizar: "<<name<<endl;
    }
}

void reporteMBR(std::string path, std::string id){
    std::string dirDisc = buscarPathDisco(id);
    if(dirDisc.compare("")==0){
        cout<<"Error, no se encontro un id: "<<id<<" no se puede ubicar el disco"<<endl;
        return;
    }
    std::string var = datosMBR(dirDisc);
    crearReporte(var, path);
    cout<<"Reporte mbr generado con exito"<<endl;
}

void crearReporte(std::string codigoDot, std::string path){
    int val = path.rfind("/");
    //Ruta solo con carpetas
    std::string rutaFinal = path.substr(0,val);
    crearCarpetas(rutaFinal);
    ofstream file;
    file.open("/home/report.dot");
    file << codigoDot;
    file.close();
    std::string instruccion = "dot -Tpng /home/report.dot -o "+path;
    system(instruccion.c_str());
    remove("/home/report.dot");
}

void reporteDISK(std::string path, std::string id){
    std::string dirDisc = buscarPathDisco(id);
    if(dirDisc.compare("")==0){
        cout<<"Error, no se encontro un id: "<<id<<" no se puede ubicar el disco"<<endl;
        return;
    }
    FILE*arch;
    arch=fopen(dirDisc.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    std::string var = "digraph Disk {\nnode [fontname=\"Arial\"];\nnod [shape=record label=\"MBR|\n";
    //Verificamos las particiones basicas
    double sizeDisk = mbr.mbr_tamano*1.0;
    int nActual=0;
    Partition aux;
    for(int i = 0; i<4; i++){        
        aux = mbr.mbr_partitions[i];
        if(aux.part_status == 1){//Si es una particion activa
            if(aux.part_type=='E'){
                var += codeDiskLogicas(dirDisc, aux.part_start, sizeDisk);
                if(i!=3){
                    var+="|";
                }
            }else{
                double sizePart = aux.part_size*1.0;
                int porcentaje = (sizePart/sizeDisk)*100;
                var+="Primaria\n-";
                var+=to_string(porcentaje); var+="\% del disco";
                if(i!=3){
                    var+="|";
                }
            }
        }else{
            if(aux.part_start==-1){
                double sizePart = aux.part_size*1.0;
                int porcentaje = (sizePart/sizeDisk)*100;
                var+="Libre\n-";
                var+=to_string(porcentaje); var+="\% del disco";
                if(i!=3){
                    var+="|";
                }
            }else{
                if(nActual == 0){
                    nActual = mbr.mbr_partitions[i-1].part_start+mbr.mbr_partitions[i-1].part_size;
                }
                if(i==3){
                    int sd = mbr.mbr_tamano;
                    int sp = sd-nActual;
                    double res = (sp*1.0)/(sd*1.0);
                    int porcentaje = res*100 + 1;
                    var+="Libre\n-";
                    var+=to_string(porcentaje); var+="\% del disco";
                }
            }
        }
    }
    var+="\"];\n}";
    crearReporte(var,path);
    cout<<"Se creo el reporte con exito"<<endl;
}

void reporteSB(std::string path, std::string id){
    std::string dirDisc = buscarPathDisco(id);
    if(dirDisc.compare("")==0){
        cout<<"Error, no se encontro un id: "<<id<<" no se puede ubicar el disco"<<endl;
        return;
    }
    int par = -1;
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].id.compare(id)==0){
            par = i;
            break;
        }
    }
    if(par==-1){cout<<"No hay una particiion montada con el id: "<<id<<endl; return;}
    int start = ParMont[par].part_start;
    SuperBloque sbl;
    FILE *used;
    used=fopen(dirDisc.c_str(),"rb+");
    fseek(used, start, SEEK_SET);
    fread(&sbl,sizeof(SuperBloque),1,used);
    fclose(used);
    std::string var = "digraph G {abc [shape=none, margin=0, label=<<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n";
    var+="<TR><TD>Nombre</TD><TD>Valor</TD></TR>\n";
    var+="<TR><TD>s_inodes_count</TD><TD>"+to_string(sbl.s_inodes_count)+"</TD></TR>\n";
    var+="<TR><TD>s_blocks_count</TD><TD>"+to_string(sbl.s_blocks_count)+"</TD></TR>\n";
    var+="<TR><TD>s_free_blocks_count</TD><TD>"+to_string(sbl.s_free_blocks_count)+"</TD></TR>\n";
    var+="<TR><TD>s_free_inodes_count</TD><TD>"+to_string(sbl.s_free_inodes_count)+"</TD></TR>\n";
    var+="<TR><TD>s_mtime</TD><TD>"+string(sbl.s_mtime)+"</TD></TR>\n";
    var+="<TR><TD>s_umtime</TD><TD>"+string(sbl.s_umtime)+"</TD></TR>\n";
    var+="<TR><TD>s_mnt_count</TD><TD>"+to_string(sbl.s_mnt_count)+"</TD></TR>\n";
    var+="<TR><TD>s_magic</TD><TD>"+to_string(sbl.s_magic)+"</TD></TR>\n";
    var+="<TR><TD>s_inode_size</TD><TD>"+to_string(sbl.s_inode_size)+"</TD></TR>\n";
    var+="<TR><TD>s_first_ino</TD><TD>"+to_string(sbl.s_first_ino)+"</TD></TR>\n";
    var+="<TR><TD>s_bm_inode_start</TD><TD>"+to_string(sbl.s_bm_inode_start)+"</TD></TR>\n";
    var+="<TR><TD>s_bm_block_start</TD><TD>"+to_string(sbl.s_bm_block_start)+"</TD></TR>\n";
    var+="<TR><TD>s_inode_start</TD><TD>"+to_string(sbl.s_inode_start)+"</TD></TR>\n";
    var+="<TR><TD>s_block_size</TD><TD>"+to_string(sbl.s_block_size)+"</TD></TR>\n";
    var+="</TABLE>>];}";
    crearReporte(var, path);
    cout<<"Reporte generado con exito"<<endl;
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

std::string codeDiskLogicas(std::string path, int start, double size){
    std::string var = "{Extendida|{\n";
    bool activo = true; EBR ebr, aux;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status==1){
            if(ebr.part_start!=-1){
                var+="EBR|Logica\n-";
                double sizePart = ebr.part_size*1.0;
                int porcentaje = (sizePart/size)*100;
                var+=to_string(porcentaje); var+="\% del disco";
                if(ebr.part_next!=-1){
                    var+="|\n";
                }
                aux = ebr;
            }else{
                var+="Libre\n-";
                double sizePart = (aux.part_start+aux.part_size)*1.0;
                int porcentaje = (sizePart/size)*100;
                var+=to_string(porcentaje); var+="\% del disco";
                if(ebr.part_next!=-1){
                    var+="|\n";
                }
                aux = ebr;
            }
            start = ebr.part_next;
        }else{
            if(aux.part_start+aux.part_size<(int)size){
                var+="|Libre\n-";
                double sizePart = size-((aux.part_start+aux.part_size)*1.0);
                int porcentaje = (sizePart/size)*100;
                var+=to_string(porcentaje); var+="\% del disco";
            }
            activo=false;
        }
    }
    var+="}}";
    return var;
}