%skeleton "lalr1.cc" /* -*- C++ -*- */

%defines
%define api.parser.class {Parser}
%define api.token.constructor
%define api.value.type variant

%define parse.trace
%define parse.error verbose
%param { Driver& driver }


%code requires
{
   class Driver;
}

%{
    #include <string>
    #include <stdio.h>
    #include "driver.h"
    #include <iostream>
    void crearComandos(std::string comando , std::string argumentos);
%}


/******* TERMINALES ********/
%token MKDISK RMDISK MENOS SIZE U PATH F IGUAL FDISK TYPE DELETE NAME ADD PAUSE REP ID MOUNT UMOUNT MKFS FS
%token<float> ENTERO
%token<std::string> IDENTIFICADOR
%token<std::string> IDPART
%token<std::string> RUTA1
%token<std::string> RUTA2
%token<std::string> CADENA
%token<std::string> FSVAL
/*%token<std::string> K
%token<std::string> M*/
%token FIN 0 "eof"

/******* NO TERMINALES ********/
%type INSTRUCCION
%type<std::string> PARAMETROS
%type<std::string> PARAMETRO
%type<std::string> RUTAS
%type<std::string> VALORES
%start INSTRUCCIONES;

%%
INSTRUCCIONES: INSTRUCCIONES INSTRUCCION
    | INSTRUCCION
;

INSTRUCCION: MKDISK PARAMETROS      { crearComandos("mkdisk", $2); }
    | RMDISK PARAMETROS             { crearComandos("rmdisk", $2); }
    | FDISK PARAMETROS              { crearComandos("fdisk", $2); }
    | PAUSE                         { crearComandos("pause", ""); }
    | REP PARAMETROS                { crearComandos("rep",$2); }
    | MOUNT PARAMETROS              { crearComandos("mount",$2); }
    | UMOUNT PARAMETROS             { crearComandos("umount",$2); }
    | MKFS PARAMETROS               { crearComandos("mkfs",$2); }
;

PARAMETROS: PARAMETROS PARAMETRO    { $$=$1+$2; }
    | PARAMETRO                     { $$=$1; }
;

PARAMETRO: MENOS SIZE IGUAL ENTERO          { $$="SIZE:"+std::to_string($4)+"&"; }
    | MENOS F IGUAL IDENTIFICADOR           { $$="F:"+$4+"&"; }
    | MENOS U IGUAL IDENTIFICADOR           { $$="U:"+$4+"&"; }
    | MENOS PATH IGUAL RUTAS                { $$="PATH:"+$4+"&"; }
    | MENOS TYPE IGUAL IDENTIFICADOR        { $$="TYPE:"+$4+"&"; }
    | MENOS DELETE IGUAL IDENTIFICADOR      { $$="DELETE:"+$4+"&"; }
    | MENOS NAME IGUAL VALORES              { $$="NAME:"+$4+"&"; }
    | MENOS ADD IGUAL ENTERO                { $$="ADD:"+std::to_string($4)+"&"; }
    | MENOS ID IGUAL IDPART                 { $$="ID:"+$4+"&"; }
    | MENOS FS IGUAL FSVAL                  { $$="FS:"+$4+"&"; }
;

VALORES: CADENA                             { $$=$1; }
    | IDENTIFICADOR                         { $$=$1; }
;

RUTAS: RUTA1                                { $$=$1; }
    | CADENA                                { $$=$1; }
    | RUTA2                                 { $$=$1; }
    | IDENTIFICADOR                         { $$=$1; }
;
      
%%

void yy::Parser::error( const std::string& error){
  std::cout << error << std::endl;
}