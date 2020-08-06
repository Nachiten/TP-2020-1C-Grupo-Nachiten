echo Recoratorio!!! Revisar que la config esté bien en ~/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Broker.config
sleep 2
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/tp-2020-1c-Grupo-Nachiten/shared/Debug
echo [Moviendose a carpeta de broker...]
sleep 1
cd Broker/Debug
echo [Compilando...]
sleep 1
make clean
make all
echo [Ejecutando Broker...]
sleep 1
./Broker