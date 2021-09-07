#include <iostream>
void correrComandoMKFS(std::string id, std::string type, std::string fs);

struct SuperBloque{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    char s_mtime[24];
    char s_umtime[24];
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
};

struct Inodo{
    int i_uid;
    int i_gid;
    int i_size;
    char i_atime[24];
    char i_ctime[24];
    char i_mtime[24];
    int i_block[15];
    char i_type;
    int i_perm;
};

struct content{
    char b_name[12];
    int b_inodo;
};

struct Bloque_Carpetas{
    content b_content[4];
};

struct Bloque_Archivos{
    char b_content[64];
};

struct Bloque_Apuntadores{
    int b_pointers[16];
};

struct Journal{
    char Journal_Tipo_Operacion[10] ; //mkfs,mkfile,...etc
    char Journal_tipo; //Carpeta(0), Archivo(1).
    char Journal_nombre [200]; //..../home....../home/chepe........../home/chepe/datos.txt
    char Journal_contenido [200]; //....este es el contenido de mi archivo de texto.....-
    char Journal_fecha[16]; // 18/06/2019 10:30
    char Journal_propietario [10]; //Quien creó el archivo o carpeta
    int Journal_permisos; //777...664
};

void comandMKFS(std::string argumentos){
    std::string actual = "", id = "", type = "FULL", fs = "EXT2", actL="";
    bool exito = true;
    //Se colocan los parametros separados y en el orden deseado
    for(int i = 0; i<argumentos.size();i++){
        actL = argumentos[i];
        if(actL.compare("&")==0){
            if (actual.find("ID")==0){
                id=actual.replace(0,3,"");
            }else if(actual.find("TYPE")==0){
                type=actual.replace(0,5,"");
            }else if(actual.find("FS")==0){
                fs=actual.replace(0,3,"");
                //Pasamos todo a mayusculas
                for(int i = 0; i<fs.size();i++){
                    fs[i] = toupper(fs[i]);
                }
                //Verificamos que se encuentre fast o full
                if(!(fs.compare("FAST")==0 || fs.compare("FULL")==0)){
                    cout<<"Error en mkfs, debe venir fast o full"<<endl;
                    return;
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
        correrComandoMKFS(id, type, fs);
    }else{
        cout<<"Error en metodo comandMKDISK, hay parametros invalidos";
    }
}

void correrComandoMKFS(std::string id, std::string type, std::string fs){
    //Buscamos entre las particiones montadas
    int par = -1, found = -1;
    for(int i = 0; i<ParMont.size();i++){
        if(ParMont[i].id.compare(id)==0){
            par = i;
        }
    }
    if(par==-1){
        cout<<"Error, no se encuentra una particion con id: "<<id<<" montada en el sistema"<<endl;
        return;
    }
    //Leemos el disco de la particion montada
    FILE*arch;
    arch=fopen(ParMont[par].path.c_str(),"rb+");
    MBR mbr;
    fseek(arch, 0, SEEK_SET);
    fread(&mbr,sizeof(MBR),1,arch);
    fclose(arch);
    //Particion auxiliar donde manejaremos los datos
    Partition aux; bool encontrado = false;
    //Buscamos la particion y la guardamos en un auxiliar para trabajar con ella
    for(int i = 0; i<4; i++){
        if(strcmp(mbr.mbr_partitions[i].part_name, ParMont[par].name.c_str())==0){
            aux = mbr.mbr_partitions[i];
            encontrado = true;
        }
    }
    //Si no se encuentra la particion que está montada dentro del disco
    if(!encontrado){ cout<<"Error en mkfs, no se encontro la particion en el disco"<<endl; return;}
    //Calculo de numero de inodos y bloques
    int n=((aux.part_size - sizeof(SuperBloque))/(4+sizeof(Journal)+sizeof(Inodo)+3*sizeof(Bloque_Archivos)));
    //Formateo
    if(fs.compare("FULL")==0){//SI ES FULL TENGO QUE BORRAR TODO Y LLENAR DE CEROS
        FILE * archFormat;
        archFormat=fopen(ParMont[par].path.c_str(),"rb+");
        fseek(archFormat,aux.part_start,SEEK_SET);//ME POSICIONO AL INICIO DE LA PARTICION
        char buff;
        buff='\0';
        for(int i=0;i<aux.part_size;i++) { //lleno de ceros el archivo
            fwrite(&buff,sizeof(buff),1,archFormat);
            //--PART_STAR ES 100 - 101 -102 -103
            fseek(archFormat, aux.part_start + i, SEEK_SET);// AGREGUE ESTA LINEA
        }
        fclose(archFormat);
    }
    //Creamos el superbloque
    SuperBloque sbloque;
    if(type.compare("EXT2")==0){
        sbloque.s_filesystem_type=2;
    }else if(type.compare("EXT3")==0){
        sbloque.s_filesystem_type=3;
    }else{
        cout<<"Tipo de sistema no reconocido: "<<type<<endl;
        return;
    }
    sbloque.s_inodes_count=n;
    sbloque.s_blocks_count=3*n;
    sbloque.s_free_blocks_count=3*n;//RESTO DESPUES
    sbloque.s_free_inodes_count=n; //RESTO DESPUES
    strcpy(sbloque.s_mtime,getFecha().c_str());
    strcpy(sbloque.s_umtime,"");
    sbloque.s_mnt_count=1;//
    sbloque.s_magic=0xEF53;
    sbloque.s_inode_size=sizeof(Inodo);
    sbloque.s_block_size=sizeof(Bloque_Archivos);
    sbloque.s_first_ino=2;//el primer inodo libre va a ser el 2
    sbloque.s_first_blo=2;     //100+64+(10*64)=804 byte
    sbloque.s_bm_inode_start=aux.part_start + sizeof(SuperBloque) + (100*sizeof(Journal));
    sbloque.s_bm_block_start=sbloque.s_bm_inode_start + n ;
    sbloque.s_inode_start=sbloque.s_bm_block_start+3*n;
    sbloque.s_block_start=sbloque.s_inode_start+n*sizeof(Inodo);
    fseek(arch,aux.part_start, SEEK_SET); //INICIO DE LA PARTICION
    fwrite(&sbloque,sizeof(SuperBloque),1, arch); //escribimos el superbloque en la particion
    //Creamos bitmap de inodos y bloques
    char bitinodos[n];
    char bibloques[3*n];
    //Escribimos los inodos
    int iniciobitnodos=aux.part_start + sizeof(SuperBloque) + (100*sizeof(Journal));
    for(int i=2;i<n;i++){
        bitinodos[i]='0';
    }
    bitinodos[0]='1';//Ocupado por la carpeta root
    bitinodos[1]='1';
    fseek(arch,iniciobitnodos, SEEK_SET);
    fwrite(&bitinodos,sizeof(bitinodos),1,arch);
    //Escribimos los bloques
    int iniciobloques=iniciobitnodos+n;
    for(int i=2;i<3*n;i++){
        bibloques[i]='0';
    }
    bibloques[0]='1';//---1 para direnciar carpeta
    bibloques[1]='2';//--2 para diferenciar archivo
    fseek(arch,iniciobloques, SEEK_SET);
    fwrite(&bibloques,sizeof(bibloques),1,arch);
    //Escribimos los Journal
    int iniciojournal=(aux.part_start + sizeof(SuperBloque));
    Journal journalicial;
    //Journal quemado inicial
    journalicial.Journal_Tipo_Operacion[0]='-';
    journalicial.Journal_tipo='-';
    journalicial.Journal_nombre[0]='\0';
    journalicial.Journal_contenido[0]='\0';
    journalicial.Journal_fecha[0]='\0';
    journalicial.Journal_propietario[0] = '\0';
    journalicial.Journal_permisos=-1;
    //Escribimos todos los journal
    for (int i = 0; i < n; i++) {
        fseek(arch, (iniciojournal + (i * sizeof(Journal))), SEEK_SET);
        fwrite(&journalicial, sizeof(Journal), 1, arch);
    }
    //Escribimos el journal de la carpeta root
    Journal journalicial2;
    strcpy(journalicial2.Journal_Tipo_Operacion,"mkfs");
    journalicial2.Journal_tipo='0';//carpeta
    strcpy(journalicial2.Journal_nombre,"/");
    strcpy(journalicial2.Journal_contenido,"-");
    strcpy(journalicial2.Journal_fecha,getFecha().c_str());
    strcpy(journalicial2.Journal_propietario,"1");
    journalicial2.Journal_permisos=664;
    //---ESCRIBO EL JOURNAL DE LA CARPETA EN EL ARCHIVO
    int posicioninical=aux.part_start+sizeof(SuperBloque);
    fseek(arch,posicioninical+0*sizeof(Journal),SEEK_SET); //apunta al journal
    fwrite(&journalicial2,sizeof(Journal),1,arch);
    //------------ESCRIBO JOURNAL DEL ARCHIVO
    Journal journalicial3;
    strcpy(journalicial3.Journal_Tipo_Operacion,"mkfs");
    journalicial3.Journal_tipo='1';//archivo
    strcpy(journalicial3.Journal_nombre,"/users.txt");
    strcpy(journalicial3.Journal_contenido,"1,G,root\n1,U,root,root,123\n");
    strcpy(journalicial3.Journal_fecha,getFecha().c_str());
    strcpy(journalicial3.Journal_propietario,"1");
    journalicial3.Journal_permisos=664;
    //---ESCRIBO EL JOURNAL DE LA CARPETA EN EL ARCHIVO
    fseek(arch,posicioninical+1*sizeof(Journal),SEEK_SET); //apunta al journal
    fwrite(&journalicial3,sizeof(Journal),1,arch);
    //AHORA LEO EL SUPERBLOQUE PARA ESCRIBIR EN SUS BLOQUES E INODOS
    SuperBloque auxsuperbloque;
    fseek(arch,aux.part_start,SEEK_SET);
    fread(&auxsuperbloque,sizeof(SuperBloque),1,arch);
    //UN INODO PARA LA CARPETA RAIz
    Inodo raiz;
    raiz.i_uid=1;
    raiz.i_gid=1;
    raiz.i_size=0;//REVISAR
    string fechacarpeta=getFecha();
    string fechacarpeta2=getFecha();
    string fechacarpeta3=getFecha();
    memset(raiz.i_atime,0,sizeof(raiz.i_atime));
    memset(raiz.i_mtime,0,sizeof(raiz.i_atime));
    memset(raiz.i_ctime,0,sizeof(raiz.i_atime));
    strcpy(raiz.i_atime,fechacarpeta.c_str());
    strcpy(raiz.i_mtime,fechacarpeta2.c_str());
    strcpy(raiz.i_ctime,fechacarpeta3.c_str());
    for(int i=0;i<15;i++){
        raiz.i_block[i]=-1;
    }
    raiz.i_type='0'; //es carpeta
    raiz.i_perm=664;
    raiz.i_block[0]=0;
    //---creo el bloque carpetas
    Bloque_Carpetas carpetaroot;
    content contenraiz;
    strcpy(contenraiz.b_name, ".");//definimos la carpeta actual
    contenraiz.b_inodo = 0; //apuntador
    carpetaroot.b_content[0] = contenraiz;
    strcpy(contenraiz.b_name, "..");//definimos al padre de la carpeta
    carpetaroot.b_content[1] = contenraiz;
    //content para archivo users.txt
    strcpy(contenraiz.b_name, "users.txt");//agregamos nombre
    contenraiz.b_inodo = 1;//apuntadora hacia inodo asociado
    carpetaroot.b_content[2] = contenraiz;//guardamos el inodo archivo a los apuntadores del inodo de la carpeta raiz
    memset(&contenraiz.b_name, 0, sizeof(contenraiz.b_name));//definimos la carpeta actual
    contenraiz.b_inodo = -1;
    carpetaroot.b_content[3] = contenraiz;
    //------GUARDAMOS EL INODO Y EL BLOQUE DE LA CARPETA EN EL DISCO
    fseek(arch, auxsuperbloque.s_inode_start, SEEK_SET);
    fwrite(&raiz, sizeof(Inodo), 1, arch);
    auxsuperbloque.s_free_inodes_count--;//disminuyo en 1 los inodos ocupados
    fseek(arch, auxsuperbloque.s_block_start, SEEK_SET);
    fwrite(&carpetaroot, 64, 1, arch);
    auxsuperbloque.s_free_blocks_count--;//disminuyo en 1 los bloques de archivos ocupados
    //------------------------creo user.txt
    //-------------------AHORA CREO INODO PARA EL ARCHIVO USER----
    char datosarchivo[]="1,G,root\n1,U,root,root,123\n";//cadena a quemar en el archivo
    Inodo archivousuarios;
    archivousuarios.i_gid=1;
    archivousuarios.i_size=sizeof(datosarchivo);//el tamaño de la cadena que voy a quemar
    archivousuarios.i_uid=1;
    string fech=getFecha();
    string fech2=getFecha();
    string fech3=getFecha();
    memset(archivousuarios.i_atime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_mtime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_ctime,0,sizeof(archivousuarios.i_atime));
    strcpy(archivousuarios.i_atime,fech.c_str());
    strcpy(archivousuarios.i_mtime,fech2.c_str());
    strcpy(archivousuarios.i_ctime,fech3.c_str());
    for(int i=0;i<15;i++){
        archivousuarios.i_block[i]=-1;
    }
    archivousuarios.i_perm=664;
    archivousuarios.i_block[0]=1; //su bloque directo va al bloque 1 que es donde va a estar el archivo USERS
    archivousuarios.i_type='1'; //es archivo
    //termino de crear el inodo de archivos usuario
    //------------------------AHORA CREO BLOQUE PARA EL ARCHIVO USER
    Bloque_Archivos bloquearchivos;
    strcpy(bloquearchivos.b_content,datosarchivo);
    //-------------------------------TERMINAMOS DE CREAR EL BLOQUE DE ARCHIVOS
    //------GUARDAMOS EL INODO Y EL BLOQUE DEL ARCHIVO USR EN EL DISCO
    fseek(arch, auxsuperbloque.s_inode_start+sizeof(Inodo), SEEK_SET);
    fwrite(&archivousuarios, sizeof(Inodo), 1, arch);
    auxsuperbloque.s_free_inodes_count--;//disminuyo en 1 los inodos ocupados
    fseek(arch, auxsuperbloque.s_block_start+64, SEEK_SET);
    fwrite(&bloquearchivos, 64, 1, arch);
    auxsuperbloque.s_free_blocks_count--;//disminuyo en 1 los bloques ocupados
    //REESCRIBO EL SUPERBLOQUE
    fseek(arch, aux.part_start, SEEK_SET);
    fwrite(&auxsuperbloque, sizeof(SuperBloque), 1, arch);
    fclose(arch); //cierro el archivo
    std::cout << "\nPARTICION FORMATEADA CORRECTAMENTE!!! \n"; //si da null es porque no se encontro el archivo
}