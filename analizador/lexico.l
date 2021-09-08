%{
  #include <stdio.h>
  #include <string>
  #include "driver.h"
  #include "parser.tab.hh"
%}

%option case-insensitive
%option noyywrap
%option outfile="scanner.cc" 

ENTERO ("-")?[0-9]+
CADENA ("\""[^"\""]*"\"")
IDENTIFICADOR ([a-zA-Z_])[a-zA-Z0-9_]*
IDPART [0-9]+[a-zA-Z]
RUTA1 ("/"{IDENTIFICADOR})+("/"{IDENTIFICADOR}"."{IDENTIFICADOR})
RUTA2 ("/"{IDENTIFICADOR})+("/"{IDENTIFICADOR})
FSVAL ((2|3)fs)
%%
"="		              {return yy::Parser::make_IGUAL();}
"-"		              {return yy::Parser::make_MENOS();}
"mkdisk"            {return yy::Parser::make_MKDISK();}
"fs"		            {return yy::Parser::make_FS();}
"mkfs"              {return yy::Parser::make_MKFS();}
"pause"             {return yy::Parser::make_PAUSE();}
"rmdisk"            {return yy::Parser::make_RMDISK();}
"fdisk"             {return yy::Parser::make_FDISK();}
"type"              {return yy::Parser::make_TYPE();}
"delete"            {return yy::Parser::make_DELETE();}
"name"              {return yy::Parser::make_NAME();}
"add"               {return yy::Parser::make_ADD();}
"umount"            {return yy::Parser::make_UMOUNT();}
"mount"             {return yy::Parser::make_MOUNT();}
"rep"               {return yy::Parser::make_REP();}
"size"		          {return yy::Parser::make_SIZE();}
"u"		              {return yy::Parser::make_U();}
"path"		          {return yy::Parser::make_PATH();}
"id"                {return yy::Parser::make_ID();}
"f"		              {return yy::Parser::make_F();}

{ENTERO}            {return yy::Parser::make_ENTERO(strtol (yytext, NULL, 10));}
{FSVAL}             {return yy::Parser::make_FSVAL(std::string(yytext));}
{IDPART}            {return yy::Parser::make_IDPART(std::string(yytext));}
{IDENTIFICADOR}     {return yy::Parser::make_IDENTIFICADOR(std::string(yytext));}
{CADENA}            {return yy::Parser::make_CADENA(std::string(yytext));}
{RUTA1}             {return yy::Parser::make_RUTA1(std::string(yytext));}
{RUTA2}             {return yy::Parser::make_RUTA2(std::string(yytext));}
(\s|\r|\t|\n)+      {}

[[:blank:]]         {}
.                   {printf("Caracter no reconocido: %s\n",yytext);}
<<EOF>>		          {return yy::Parser::make_FIN();}
%%

void Driver::runScanner(){
  std::string arch = "entradaActual.txt";
  FILE *fichero;
  fichero = fopen(arch.c_str(),"w");
  fwrite(file.c_str(),1,file.size(),fichero);
  fclose(fichero);
  yy_flex_debug = false;
  yyin = fopen (arch.c_str(), "r");
  remove(arch.c_str());
}

void Driver::closeFile(){
    fclose(yyin);
}