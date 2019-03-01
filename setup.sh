# install necessary packages
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install -y build-essential
sudo apt-get install -y libcrypto++6 libcrypto++6-dbg libcrypto++-dev
sudo apt-get install -y python
sudo apt-get install -y libgmp3-dev
sudo apt-get install -y golang-go
sudo apt-get install -y yasm # needed by mpir
sudo apt-get install -y m4 # needed by mpir

# download SimpleOT
git clone https://github.com/pascholl/SimpleOT/tree/ad5929c154fbf65dc5b7aeb9344e811ca7782ae6
cd SimpleOT
make 

# config stuff
mylocal="$HOME/local"
mkdir -p ${mylocal}

# install MPIR 3.0.0
cd ${mylocal}
curl -O "http://mpir.org/mpir-3.0.0.tar.bz2"
tar xf mpir-3.0.0.tar.bz2
cd mpir-3.0.0
./configure --enable-cxx --prefix="${mylocal}/mpir"
make && make check
sudo make install

# install libsodium
cd $mylocal
curl -O https://download.libsodium.org/libsodium/releases/libsodium-1.0.16.tar.gz
tar xf libsodium-1.0.16.tar.gz
cd libsodium-1.0.16/
./configure
make && make check
sudo make install

# install OpenSSL 1.1.0
cd $mylocal
curl -O "https://www.openssl.org/source/openssl-1.1.0j.tar.gz"
tar -xf openssl-1.1.0j.tar.gz
cd openssl-1.1.0j
./config --prefix="${mylocal}/openssl"
make && sudo make install

# update bashrc
echo "export mylocal=$HOME/local" >> $HOME/.bashrc
echo "export mylocal=$HOME/local" >> $HOME/.bashrc
echo "export mylocal=$HOME/local" >> $HOME/.bashrc
echo "export mylocal=$HOME/local" >> $HOME/.bashrc

# export OpenSSL paths
echo "export PATH=\"${mylocal}/openssl/bin/:${PATH}\"" >> $HOME/.bashrc
echo "export C_INCLUDE_PATH=${mylocal}/openssl/include/:${C_INCLUDE_PATH}\"" >> $HOME/.bashrc
echo "export CPLUS_INCLUDE_PATH=${mylocal}/openssl/include/:${CPLUS_INCLUDE_PATH}\"" >> $HOME/.bashrc
echo "export LIBRARY_PATH=${mylocal}/openssl/lib/:${LIBRARY_PATH}\"" >> $HOME/.bashrc
echo "export LD_LIBRARY_PATH=${mylocal}/openssl/lib/:${LD_LIBRARY_PATH}\"" >> $HOME/.bashrc

# export libsodium paths
echo "export LD_LIBRARY_PATH=\"${mylocal}:/usr/local/lib/:${LD_LIBRARY_PATH}\"" >> $HOME/.bashrc
echo "export LIBRARY_PATH=\"${mylocal}:/usr/local/lib/:${LIBRARY_PATH}\"" >> $HOME/.bashrc

# export MPIR paths
echo "export PATH=\"${mylocal}/mpir/bin/:${PATH}\"" >> $HOME/.bashrc
echo "export C_INCLUDE_PATH=\"${mylocal}/mpir/include/:${C_INCLUDE_PATH}\"" >> $HOME/.bashrc
echo "export CPLUS_INCLUDE_PATH=\"${mylocal}/mpir/include/:${CPLUS_INCLUDE_PATH}\"" >> $HOME/.bashrc
echo "export LIBRARY_PATH=\"${mylocal}/mpir/lib/:${LIBRARY_PATH}\"" >> $HOME/.bashrc
echo "export LD_LIBRARY_PATH=\"${mylocal}/mpir/lib/:${LD_LIBRARY_PATH}\"" >> $HOME/.bashrc

# update bash
source $HOME/.bashrc
