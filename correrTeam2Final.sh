echo Recoratorio!!! El path de config de este Team es:  ~/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Team2Final.config
sleep 2
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/tp-2020-1c-Grupo-Nachiten/shared/Debug
echo [Moviendose a carpeta de Team...]
sleep 1
cd Team/Debug
echo [Compilando...]
sleep 1
make clean
make all
echo [Ejecutando Team...]
sleep 1
./Team "/home/utnso/workspace/tp-2020-1c-Grupo-Nachiten/Configs/Team2Final.config"