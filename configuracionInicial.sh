cd ~/
echo Clonando commons...
sleep 1
git clone https://github.com/sisoputnfrba/so-commons-library/
echo Moviendose a carpeta...
sleep 1
cd so-commons-library
echo Instalando commons..
sleep 1
sudo make install
echo Moviendose a carpeta del repo...
sleep 1
cd ~/workspace/tp-2020-1c-Grupo-Nachiten
echo Library path shared...
sleep 1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/workspace/tp-2020-1c-Grupo-Nachiten/shared/Debug
echo Moviendose a carpeta shared...
sleep 1
cd shared/Debug
echo Buildeando shared...
sleep 1
make clean
make all
echo Volviendo a carpeta root del repo...
sleep 1
cd ~/workspace/tp-2020-1c-Grupo-Nachiten