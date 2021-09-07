#include <iostream>
using namespace std;
void correrComandoFDISK(std::string size,std::string Uval,std::string path,std::string type,std::string Fval,std::string deleteVal,std::string name,std::string add);
void fdiskDelete(std::string path,std::string name, std::string deleteVal);
void fdiskAdd(std::string path,std::string name, std::string add, std::string Uval);
void fdiskCreatePartition(std::string path,std::string name, std::string size, std::string type, std::string Uval, std::string Fval);
void createPrimaryPartition(std::string path, std::string name, int tamanio,std::string type,std::string Uval,std::string Fval);
void createExtendPartition(std::string path,std::string name,int tamanio,std::string type,std::string Uval,std::string Fval);
void createLogicPartition(std::string path,std::string name,int tamanio,std::string type,std::string Uval,std::string Fval);
void imprimirdatosdisco(std::string path);
void imprimirLogicas(std::string path, int start);

struct EBR{
    char part_status = 0;
    char part_fit;
    int part_start = 0;
    int part_size = 0;
    int part_next = 0;
    char part_name[16];
};

void comandFDISK(std::string argumentos){
    std::string actual = "", path = "", Uval = "K", Fval = "WF", size = "", actL="", type = "P", name="", add="", deleteVal = "";
    bool exito = true;
    //Se colocan los parametros separados y en el orden deseado
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if (actual.find("SIZE")==0){
                size=actual.replace(0,5,"");
            }else if(actual.find("PATH")==0){
                path=actual.replace(0,5,"");
                //Si la ruta contiene comillas significa que tiene espacios, se eliminan comillas
                if(path.find("\"")==0){
                    path = path.substr(1,path.size()-2);
                }
            }else if(actual.find("U")==0){
                Uval=actual.replace(0,2,"");
            }else if(actual.find("F")==0){
                Fval=actual.replace(0,2,"");
            }else if(actual.find("TYPE")==0){
                type=actual.replace(0,5,"");
            }else if(actual.find("DELETE")==0){
                deleteVal=actual.replace(0,7,"");
            }else if(actual.find("NAME")==0){
                name=actual.replace(0,5,"");
                //Si el nombre comillas significa que tiene espacios, se eliminan comillas
                if(name.find("\"")==0){
                    name = name.substr(1,name.size()-2);
                }
            }else if(actual.find("ADD")==0){
                add=actual.replace(0,4,"");
            }else{
                exito = false;
            }
            actual="";
        }else{
            actual+=argumentos[i];
        }
    }
    if(exito==true){
        correrComandoFDISK(size,Uval,path,type,Fval,deleteVal,name,add);
        //imprimirdatosdisco(path);
    }else{
        cout<<"Error en metodo comandFDISK, hay parametros invalidos";
    }
}

void correrComandoFDISK(std::string size,std::string Uval,std::string path,std::string type,std::string Fval,std::string deleteVal,std::string name,std::string add){
    if(!verificarArchivo(path)){
        cout<<"Error en correrComandoFDISK, no se encontro el disco"<<endl;
        return;
    }
    if(!add.compare("")==0 && !deleteVal.compare("")==0){
        cout<<"Error no se puede realizar add y delete al mismo tiempo en fdisk"<<endl;
        return;
    }else if(!add.compare("")==0){
        Uval = toupper(Uval[0]);
        fdiskAdd(path,name,add,Uval);
        return;
    }else if(!deleteVal.compare("")==0){
        for(int i =0; i<deleteVal.length();i++){
            deleteVal[i]=toupper(deleteVal[i]);
        }
        if(deleteVal.compare("FAST")==0 || deleteVal.compare("FULL")==0){
            fdiskDelete(path,name,deleteVal);
        }else{
            cout<<"Error en delete de fdisk, parametro invalido en -delete"<<endl;
            return;
        }
    }else{
        type = toupper(type[0]);
        Uval = toupper(Uval[0]);
        Fval = toupper(Fval[0]);
        fdiskCreatePartition(path,name,size,type,Uval,Fval);
        return;
    }
}

void fdiskDelete(std::string path,std::string name, std::string deleteVal){
    MBR mbr;
    FILE *arch;
    arch=fopen(path.c_str(),"rb+");
    fseek(arch,0,SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    int par = -1, start = 0, size = 0;
    for(int i =0; i<4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            par = i;
            break;
        }
    }
    if(par!=-1){//Si la particion a eliminar es primaria o extendida
        start = mbr.mbr_partitions[par].part_start;
        size = mbr.mbr_partitions[par].part_size;
        mbr.mbr_partitions[par].part_status=0;
        mbr.mbr_partitions[par].part_type='-';
        mbr.mbr_partitions[par].part_fit='-';
        mbr.mbr_partitions[par].part_start=-1;
        //Eliminamos esta linea para guardar el tamaño de la particion en caso de que se haya eliminado
        //mbr.mbr_partitions[par].part_size=0;
        mbr.mbr_partitions[par].part_name[0] = '\0';
        FILE *archivo;
        archivo = fopen(path.c_str(), "rb+");
        fseek(archivo,0,SEEK_SET);
        fwrite(&mbr, sizeof(mbr), 1, archivo);
        if(deleteVal.compare("FULL")==0){
            fseek(arch,start,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION
            char buff = '\0';
            for(int i=0;i<size;i++) { //lleno de ceros el archivo
                fwrite(&buff,sizeof(buff),1,arch);
                fseek(arch, start + i, SEEK_SET);//Le agregamos +i para que se vaya corriendo 1 bit
            }
        }
        fclose(archivo);
        cout<<"Particion "<<name<<" eliminada con exito"<<endl;
        return;
    }
    //Si no esta en las primarias puede estar en una extendida, ser una logica
    par = -1;
    for(int i = 0 ; i < 4 ; i ++){//Verificamos que exista una particion extendida;
        if(mbr.mbr_partitions[i].part_type=='E'){
            par = i;
        }
    }
    if(par==-1){//Si no existe una extendida par sigue siendo -1, devolvemos error
        cout<<"Error al eliminar particion, no se encontro la particion: "<<name<<endl; 
        return;
    }
    start = mbr.mbr_partitions[par].part_start;
    bool activo = true; EBR ebr;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status==1){// Si la particion esta activa la analizamos
            if(strcmp(ebr.part_name,name.c_str())==0){///Si encontramos el nombre
                start = ebr.part_start;
                ebr.part_status=1;
                ebr.part_fit='-';
                ebr.part_start=-1;
                ebr.part_size=0;
                std::string vacio = "";
                strcpy(ebr.part_name,vacio.c_str());
                FILE *archivo;
                archivo=fopen(path.c_str(),"rb+");
                fseek(archivo,start,SEEK_SET);
                fwrite(&ebr,sizeof(EBR),1,archivo);
                if(deleteVal.compare("FULL")==0){
                    fseek(arch,start,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION
                    char buff = '\0';
                    for(int i=0;i<size;i++) { //lleno de ceros el archivo
                        fwrite(&buff,sizeof(buff),1,arch);
                        fseek(arch, start + i, SEEK_SET);//Le agregamos +i para que se vaya corriendo 1 bit
                    }
                }
                fclose(archivo);
                cout<<"Particion "<<name<<" eliminada con exito"<<endl;
            }else{
                start = ebr.part_next;
            }
        }else{
            activo =  false;
        }
    }
}

void fdiskAdd(std::string path,std::string name, std::string add, std::string Uval){
    MBR mbr;
    FILE *arch;
    arch=fopen(path.c_str(),"rb+");
    fseek(arch,sizeof(MBR),SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    //Por si se busca una particion primaria o extendida
    int par = -1, addVal = atoi(add.c_str());
    if(Uval.compare("M")==0){
        addVal=addVal*1024*1024;
    }else if(Uval.compare("K")==0){
        addVal=addVal*1024;
    }else if(Uval.compare("B")==0){
        addVal=addVal;
    }else{ cout<<"Error en fdisk, valores incorrectos en Unidad"<<endl; return;}
    for(int i = 0 ; i < 4 ; i ++){//Buscamos la particion por su nombre
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            par = i;
            break;
        }
    }
    if(par!=-1){//Si se encuentra el nombre de la particion entonces es una primaria o extendida
        if(mbr.mbr_partitions[par].part_size+addVal<=0){//Se verifica que al agregar la particion el valor sea mayor que cero
            cout<<"Error, no se puede eliminar mas espacio del disponible en particion "<<name<<endl;
            return;
        }
        if(addVal<0){//Si se resta espacio a la particion entonces solo se quita el espacio
            mbr.mbr_partitions[par].part_size = mbr.mbr_partitions[par].part_size + addVal;
        }else if(par==3){//Positivo: verificamos si es la ultima particion para ver si el espacio final no sobrepasa el disco
            if(mbr.mbr_partitions[par].part_start+mbr.mbr_partitions[par].part_size+addVal<mbr.mbr_tamano){
                mbr.mbr_partitions[par].part_size = mbr.mbr_partitions[par].part_size + addVal;
            }else{
                cout<<"Error en add, no se puede sobrepasar el tamaño del disco "<<name<<endl;
                return;
            }
        }else{//Positivo y no es la ultima particion
            if(mbr.mbr_partitions[par+1].part_status==0){//Si la siguiente particion no se ha creado
                mbr.mbr_partitions[par].part_size = mbr.mbr_partitions[par].part_size + addVal;
            }else{
                if(mbr.mbr_partitions[par].part_start + mbr.mbr_partitions[par].part_size + addVal < mbr.mbr_partitions[par+1].part_start){
                    mbr.mbr_partitions[par].part_size = mbr.mbr_partitions[par].part_size + addVal;
                }else{
                    cout<<"Error en add, alcanza a tocar otra particion "<<name<<endl;
                }
            }
        }
        FILE *archivo;
        archivo = fopen(path.c_str(), "rb+");
        if(archivo != NULL){
            fseek(archivo,0,SEEK_SET);
            fwrite(&mbr, sizeof(mbr), 1, archivo);
            fclose(archivo);
            cout<<"Add efectuado con exito en particion: "<<name<<endl;
            return;
        }else{
            cout<<"Error en metodo agregarMbrDisco, no se pudo acceder al disco"<<endl;
            return;
        }
    }
    for(int i = 0 ; i < 4 ; i ++){//Verificamos si existe una particion extendida
        if(mbr.mbr_partitions[i].part_type=='E'){
            par = i;
            break;
        }
    }
    int start = mbr.mbr_partitions[par].part_start;
    bool activo = true; EBR ebr;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status==1){//Si la particion se encuentra activa
            if(strcmp(ebr.part_name,name.c_str())==0){//Verificamos su nombre
                if(addVal<0){
                    if(ebr.part_size+addVal<=0){
                        cout<<"Error, no se puede quitar todo el espacio a la particon: "<<name<<endl;
                        return;
                    }else{
                        ebr.part_size = ebr.part_size + addVal;
                    }
                }else{//Si se agrega valor en lugar de quitar
                    if(ebr.part_next == -1 && ebr.part_start + ebr.part_size + addVal < mbr.mbr_partitions[par].part_start + mbr.mbr_partitions[par].part_size){//Verificamos que no haya particion siguiente y que no sobrepase el tamaño de la extendida
                        ebr.part_start = ebr.part_start+ addVal;
                    }
                }
                FILE *archivo;
                archivo=fopen(path.c_str(),"rb+");
                fseek(archivo,ebr.part_start,SEEK_SET);
                fwrite(&ebr,sizeof(EBR),1,archivo);
                fclose(archivo);
                cout<<"Comando add efectuado con exito en particon: "<<name<<endl;
                return;
            }else{
                start = ebr.part_next;
            }
        }else{
            activo=false;
        }
    }
}

void fdiskCreatePartition(std::string path,std::string name, std::string size, std::string type, std::string Uval, std::string Fval){
    int tamanio = atoi(size.c_str()); if(tamanio<=0){cout<<"Error en fdisk, el tamanio debe ser positivo y mayor que cero"<<endl; return;}
    if(Uval.compare("M")==0){
        tamanio=tamanio*1024*1024;
    }else if(Uval.compare("K")==0){
        tamanio=tamanio*1024;
    }else if(Uval.compare("B")==0){
        tamanio=tamanio;
    }else{ cout<<"Error en fdisk, valores incorrectos en Unidad"<<endl; return;}
    //Verificamos que tipo de particion crearemos
    if(type.compare("P")==0){ //Primaria
        createPrimaryPartition(path, name, tamanio, type, Uval, Fval);
    }else if(type.compare("E")==0){//Extendida
        createExtendPartition(path, name, tamanio, type, Uval, Fval);
    }else if(type.compare("L")==0){//Logica
        createLogicPartition(path, name, tamanio, type, Uval, Fval);
    }else{
        cout<<"Error al crear particion "<<name<<", valor incorrecto en type: "<<type<<endl;
        return;
    }
}

void createLogicPartition(std::string path,std::string name,int tamanio,std::string type,std::string Uval,std::string Fval){
    //Numero que se usará para ver la posición del arreglo de particiones del mbr
    int par = -1;
    FILE *arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    for(int i = 0 ; i < 4 ; i ++){//Verificamos que exista una particion extendida;
        if(mbr.mbr_partitions[i].part_type=='E'){
            par = i;
        }
    }
    if(par==-1){//Si no existe una extendida par sigue siendo -1, devolvemos error
        cout<<"Error al crear particion logica, no existe particion extendida"<<endl; 
        return;
    }
    for(int i = 0 ; i < 4 ; i ++){//Verificamos que no exista otra particion con el nombre
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            cout<<"Error al crear particion logica, ya existe una particion llamada: "<<name<<endl;
            return;
        }
    }
    bool activo = true, ant = false; EBR ebr, ebrAnt;
    int start = mbr.mbr_partitions[par].part_start;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status==0 && ebr.part_start==0){//Significa que no hay particion logica aqui
            activo = false;
            //Validamos que la particion logica quepa dentro de la particion extendida
            if(start + tamanio>mbr.mbr_partitions[par].part_start+mbr.mbr_partitions[par].part_size){
                cout<<"Error al creal particion logica: "<<name<<" no hay mas esspacio en la particion extendida"<<endl;
                return;
            }
            ebr.part_status = 1;
            char pfit = 'W';
            if(Fval.compare("W")==0){pfit='W';}else if(Fval.compare("F")==0){pfit='F';}else if(Fval.compare("B")==0){pfit='B';}
            ebr.part_fit = pfit;
            ebr.part_start = start;
            ebr.part_size = tamanio;
            ebr.part_next = -1;
            strcpy(ebr.part_name,name.c_str());
            FILE *archivo;
            archivo=fopen(path.c_str(),"rb+");
            fseek(archivo,start,SEEK_SET);
            fwrite(&ebr,sizeof(EBR),1,archivo);
            fclose(archivo);
            if(ant){
                FILE *archivo;
                archivo=fopen(path.c_str(),"rb+");
                fseek(archivo,ebrAnt.part_start,SEEK_SET);
                fwrite(&ebrAnt,sizeof(EBR),1,archivo);
                fclose(archivo);
            }
        }else{
            if(strcmp(ebr.part_name,name.c_str())==0){
                cout<<"Error al crear particion logica, ya existe una particion llamada: "<<name<<endl;
                return;
            }
            ebrAnt = ebr; ant = true;
            if(ebr.part_next!=-1){
                start = ebr.part_next;
            }else{
                start = ebr.part_start + ebr.part_size;
                ebrAnt.part_next = ebr.part_start + ebr.part_size;
            }
        }
    }
    cout<<"Se creo la particion: "<<name<<" con exito"<<endl;
}

void createExtendPartition(std::string path,std::string name,int tamanio,std::string type,std::string Uval,std::string Fval){
    //Numero que se usará para ver la posición del arreglo de particiones del mbr
    int par = -1;
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    for(int i = 0 ; i < 4 ; i ++){//Verificamos que no exista otra particion extendida;
        if(mbr.mbr_partitions[i].part_type=='E'){
            cout<<"Error, ya existe una particion extendida en el disco, no se creo particion :"<<name<<endl;
            return;
        }
    }
    //Variable usada para ver el espacio disponible en el disco
    int spaceFree = mbr.mbr_tamano;
    for(int i = 0 ; i < 4 ; i ++){
        if(mbr.mbr_partitions[i].part_status==0 &&mbr.mbr_partitions[i].part_start==0){ //Obtenemos la primera particion inactiva que tengamos
            par = i;
            break;
        }else{
            spaceFree = spaceFree - mbr.mbr_partitions[i].part_size;
        }
    }
    if(par==-1){//Si no se encuentra un espacio disponible para la particion par sigue siendo -1, devolvemos error
        cout<<"Error al crear particion extendida, las 4 particiones estan ocupadas"<<endl; 
        return;
    }
    if(spaceFree - tamanio<1){ //Vemos si en el espacio disponible cabe la particion que necesitamos
        cout<<"Error al crear particion extendida, no hay espacio disponible"<<endl; 
        return;
    }
    for(int i = 0 ; i < 4 ; i ++){//Verificamos si el nombre de la particion ya existe
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            cout<<"Error, el nombre de la particion ya existe en este disco"<<endl;
            return;
        }
    }
    //Agregamos la informacion importante a la particion
    mbr.mbr_partitions[par].part_status = 1;
    mbr.mbr_partitions[par].part_type = 'E';
    //No se por que marca error pero agregamos if para arreglar
    char pfit = 'W';
    if(Fval.compare("W")==0){pfit='W';}else if(Fval.compare("F")==0){pfit='F';}else if(Fval.compare("B")==0){pfit='B';}
    mbr.mbr_partitions[par].part_fit = pfit;
    mbr.mbr_partitions[par].part_size = tamanio;
    strcpy(mbr.mbr_partitions[par].part_name, name.c_str());
    if(par==0){ //Si es la primera particion la colocamos luego del mbr
        mbr.mbr_partitions[par].part_start = sizeof(MBR);
    }else{
        //Variable usada para saber donde comienza la nueva particion
        int start = sizeof(MBR);
        for(int i = 0; i<par; i++){
            start = start+ mbr.mbr_partitions[i].part_size;
        }
        //Si piden colocar el espacio del struct de la particion agregale +sizeof(Partition)
        mbr.mbr_partitions[par].part_start = start;
    }
    //Creamos un ebr y lo llenamos inactivo
    EBR ebr;
    ebr.part_status = 0;    //char
    ebr.part_fit='0';       //char
    ebr.part_start = 0;    //int
    ebr.part_size = 0;      //int
    ebr.part_next = -1;     //int
    std::string val = "";   //Var auxiliar para que no falle el  strcpy
    strcpy(ebr.part_name,val.c_str());
    //Agregamos un ebr quemado e inactivo al inicio de la particion extendida
    FILE *archivoEBR;
    archivoEBR=fopen(path.c_str(),"rb+");
    fseek(archivoEBR,mbr.mbr_partitions[par].part_start,SEEK_SET);
    fwrite(&ebr,sizeof(EBR),1,archivoEBR);
    fclose(archivoEBR);
    //Guardamos el mbr nuevamente
    FILE *archivo;
    archivo =fopen(path.c_str(),"rb+");
    fseek(archivo, 0, SEEK_SET);//QUEREMOS modificar en donde estaba el MBR original
    fwrite(&mbr, sizeof(MBR), 1, archivo);
    fclose(archivo);
    cout<<"Se creo la particion: "<<name<<" con exito"<<endl;
}

void createPrimaryPartition(std::string path, std::string name, int tamanio,std::string type,std::string Uval,std::string Fval){
    //Numero que se usará para ver la posición del arreglo de particiones del mbr
    int par = -1;
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    //Variable usada para ver cuanto espacio libre hay en el disco
    int spaceFree = mbr.mbr_tamano;
    for(int i = 0 ; i < 4 ; i ++){
        if(mbr.mbr_partitions[i].part_status==0 && mbr.mbr_partitions[i].part_start==0){ //Obtenemos la primera particion inactiva que tengamos (le agregue la validacion start = 0 para ver que este espacio nunca se haya eliminado)
            par = i;
            break;
        }else{
            //Si la particion esta ocupada le restamos este espacio a la variable que controla el espacio libre
            spaceFree = spaceFree - mbr.mbr_partitions[i].part_size;
        }
    }
    if(par==-1){//Si no se encuentra un espacio disponible para la primaria par sigue siendo -1, devolvemos error
        cout<<"Error al crear particion primaria, las 4 particiones estan ocupadas"<<endl; 
        return;
    }
    if(spaceFree - tamanio<0){ //Vemos si en el espacio disponible cabe la particion que necesitamos
        cout<<"Error al crear particion primaria, no hay espacio disponible"<<endl; 
        return;
    }
    for(int i = 0 ; i < 4 ; i ++){//Verificamos si el nombre de la particion ya existe
        if(strcmp(mbr.mbr_partitions[i].part_name,name.c_str())==0){
            cout<<"Error, el nombre de la particion ya existe en este disco"<<endl;
            return;
        }
    }
    //Agregamos la informacion importante a la particion
    mbr.mbr_partitions[par].part_status = 1;
    mbr.mbr_partitions[par].part_type = 'P';
    //No se por que marca error pero agregamos if para arreglar
    char pfit = 'W';
    if(Fval.compare("W")==0){pfit='W';}else if(Fval.compare("F")==0){pfit='F';}else if(Fval.compare("B")==0){pfit='B';}
    mbr.mbr_partitions[par].part_fit = pfit;
    mbr.mbr_partitions[par].part_size = tamanio;
    strcpy(mbr.mbr_partitions[par].part_name, name.c_str());
    if(par==0){ //Si es la primera particion la colocamos luego del mbr
        mbr.mbr_partitions[par].part_start = sizeof(MBR);
    }else{
        //Variable usada para saber donde comienza la nueva particion
        int start = sizeof(MBR);
        for(int i = par-1; i>=0;i--){
            start = start + mbr.mbr_partitions[i].part_size;
        }
        //Si se necesita agregar el tamaño de la particion ponele +sizeof(Partition)
        mbr.mbr_partitions[par].part_start = start;
    }
    //Guardamos el mbr nuevamente
    FILE *archivo;
    archivo =fopen(path.c_str(),"rb+");
    fseek(archivo, 0, SEEK_SET);//QUEREMOS modificar en donde estaba el MBR original
    fwrite(&mbr, sizeof(MBR), 1, archivo);
    fclose(archivo);
    cout<<"Se creo la particion: "<<name<<" con exito"<<endl;
}

void imprimirdatosdisco(std::string path){
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    cout << "\n----------DATOS DEL DISCO-----\n";
    cout << "MBR NAME: "<< mbr.mbr_disk_signature<<endl;
    cout << "MBR SIZE: "<< mbr.mbr_tamano<<endl;
    cout << "MBR FECHA: "<< mbr.mbr_fecha_creacion<<endl;
    cout << "MBR DISK_FIT: "<< mbr.disk_fit<<endl;
    for(int i=0;i<4;i++){
        if(mbr.mbr_partitions[i].part_status==1){
            cout<<"*************************"<<endl;
            cout << "PARTICION : "<<i<<endl;
            cout << "PARTICION STATUS : "<<mbr.mbr_partitions[i].part_status<<endl;
            cout << "PARTICION TYPE : "<<mbr.mbr_partitions[i].part_type<<endl;
            cout << "PARTICION FIT : "<<mbr.mbr_partitions[i].part_fit<<endl;
            cout << "PARTICION START : "<<mbr.mbr_partitions[i].part_start<<endl;
            cout << "PARTICION SIZE : "<<mbr.mbr_partitions[i].part_size<<endl;
            cout << "PARTICION NAME : "<<mbr.mbr_partitions[i].part_name<<endl;
            if(mbr.mbr_partitions[i].part_type=='E'){
                imprimirLogicas(path,mbr.mbr_partitions[i].part_start);
            }
            cout<<"*************************"<<endl;
        }
    }
}

void imprimirLogicas(std::string path, int start){
    bool activo = true; EBR ebr;
    while(activo){
        FILE *archebr;
        archebr=fopen(path.c_str(),"rb+");
        fseek(archebr,start, SEEK_SET);
        fread(&ebr,sizeof(EBR),1,archebr);
        fclose(archebr);
        if(ebr.part_status==1){
            cout<<"-----------------------------------"<<endl;
            cout<<"Status: "<<ebr.part_status<<endl;
            cout<<"Fit: "<<ebr.part_fit<<endl;
            cout<<"Start: "<<ebr.part_start<<endl;
            cout<<"Size: "<<ebr.part_size<<endl;
            cout<<"Next: "<<ebr.part_next<<endl;
            cout<<"Name: "<<ebr.part_name<<endl;
            cout<<"-----------------------------------"<<endl;
            start = ebr.part_next;
        }else{
            activo=false;
        }
    }
}