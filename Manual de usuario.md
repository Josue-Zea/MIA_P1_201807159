# Manual de usuario
En este primer proyecto utilizamos una aplicación de comandos que, con base a las instrucciones que se le indique logra simular un sistema de archivos para que pueda ser visualizada su estructura, esto se logra mediante la creación de reportes utilizando la herramienta graphviz para poder ser vistos de mejor manera.
## Comandos soportados
| Comando | Funcion |
| ------ | ------ |
| mkdisk | Crea un archivo binario que simulará un disco |
| rmdisk | Eimina el disco simulado con un archivo binario |
| fdisk | Crea, modifica o elimina las particiones en un disco |
| mount | Monta en memoria una partición de un disco específico |
| umount | Elimina de memora una particion montada en el disco |
| mkfs | Crea un sistema de archivos en una particion |
| exec | Ejecuta una lista de comandos que estén dentro de un archivo .sh |
| rep | Genera reportes |

# Detalles por comando
* mkdisk

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| size | Obligatorio | Será el tamaño del disco |
| f | Opcional | Indica el ajuste que se usará |
| u | Opcional | Unidad de tamaño a usar |
| path | Obligatorio | Ruta en la que se creará el disco |
Ejemplos de uso
```sh
mkdisk -size=3000 -u=k -path=/home/user/Disco1.dk
mkdisk -path=/home/user/Disco2.dk -U=M -size=100
```

* rmdidsk

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| path | Obligatorio | Ruta en la que se encuentra el disco |
Ejemplos de uso
```sh
rmkdisk -path="/home/mis discos/user/Disco1.dk"
```

* fdisk

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| size | Obligatorio (al crear) | Tamaño de la particion a crear |
| u | opcional | Unidad que se usará en el tamaño |
| path | Obligatorio | Ruta en la que se encuentra el disco |
| type | Opcional | Tipo de partición a crear |
| f | Opcional | Indica el tipo de ajuste a usar |
| delete | Opcional | Indica que se eliminará la particion y en que manera |
| name | Obligatorio | Indica el nombre de la partición |
| add | Opcional | Se utiliza para agregar o quitar espacio a la partición |
Ejemplos de uso
```sh
fdisk -size=300 -path=/home/Disco1.dk -name=Particion1
fdisk -name=Particion1 -delete=full -path=/home/Disco1.dk
fdisk -add=-500 -size=10 -u=k -path="/home/mis discos/D1.dk"
```
* Mount

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| path | Obligatorio | Ruta en la que se encuentra el disco |
| name | Obligatorio | Indica el nombre de la partición |
Ejemplos de uso
```sh
mount -path=/home/Disco1.dk -name=Part1
```
* Umount

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| id | Obligatorio | Indica el identificador generado al montar |
Ejemplos de uso
```sh
umount -id=591a
```

* Mkfs

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| id | Obligatorio | Indica el identificador generado al montar |
| type | Opcional | Indica el tipo de formateo que se le realizará a la particion |
| fs | Opcional | Indica el tipo de sistema que se creará |
Ejemplos de uso
```sh
mkfs -type=fast -id=591A
mkfs -id=582A
```
* Exec

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| path | Obligatorio|Indica la ruta del archivo |
Ejemplos de uso
```sh
exec -path=/home/usuario/Escritorio/entrada.sh
```
* Rep

| Parametro | Categoría |Descripcion |
| ------ | ------ | ------ |
| name | Obligatorio | Indica el tipo de reporte a crear |
| path | Obligatorio | Indica la ruta del reporte a generar y su nombre |
| id | Obligatorio | Indica la ruta del reporte a generar y su nombre |
| ruta | Opcional | Nombre del archivo o carpeta |
| root | Opcional | Nombre del inodo que usará como raiz |
Ejemplos de uso
```sh
rep -id=591a -path=/home/user/reports/reporte1.jpg -name=mbr
```