# Whitebox Implementation of AES and SM4

This repository implements two popular approaches of symmetric whitebox cryptography: boolean circuit and look up table.

We implement both approaches on two widely used block cipher: SMS4 and AES.

`masked_aes.cpp` `masked_sm4.cpp` contain the boolean circuit implementation of AES and SM4, `look_up_table_aes.cpp` `look_up_table_sm4.cpp` contain the look up table implementation of AES and SM4

# Build from source code

##### m4->GMP 6.2.1->NTL 11.5.1->whitebox_aes_sm4

This project depends on NTL(Number Theory Library) and NTL depends largely on GMP, so you need to install GMP first and NTL second to build our project. The following instructions will guide you to build from bare metal.

###  1. Preparing docker

In order to show the complete steps to build from scratch, we **pull a new ubuntu docker first**: `docker pull ubuntu`

```
Using default tag: latest
latest: Pulling from library/ubuntu
37aaf24cf781: Pull complete 
Digest: sha256:9b8dec3bf938bc80fbe758d856e96fdfab5f56c39d44b0cff351e847bb1b01ea
Status: Downloaded newer image for ubuntu:latest
docker.io/library/ubuntu:latest
[10/06/23]seed@VM:~/.../whitebox$ docker images
REPOSITORY   TAG       IMAGE ID       CREATED       SIZE
ubuntu       latest    3565a89d9e81   10 days ago   77.8MB
```

**run the docker**: `docker run -itd --name whitebox_test ubuntu`

```
[10/06/23]seed@VM:~/.../whitebox$ docker ps
CONTAINER ID   IMAGE     COMMAND       CREATED          STATUS          PORTS     NAMES
f055b40ec53a   ubuntu    "/bin/bash"   22 seconds ago   Up 21 seconds             whitebox_test
```

go into the docker: `docker exec -it whitebox_test bash`

### 2. Install GMP

Notice that NTL version and GMP version must be compliant. For example, the latest version of NTL is 11.5.1 and it requires GMP 6.2.1, if you use higher or lower version of GMP the compilation will fail.

Install some dependencies for installing GMP

```
apt-get update
apt-get upgrade
apt-get install gcc g++ m4 lzip gzip wget automake autoconf libtool make vim
```

**GCC version**

```
root@f055b40ec53a:/# gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 11.4.0-1ubuntu1~22.04' --with-bugurl=file:///usr/share/doc/gcc-11/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-11 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-gcn/usr --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=2
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
```

**g++ version**

```
root@f055b40ec53a:/# g++ -v
Using built-in specs.
COLLECT_GCC=g++
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/11/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 11.4.0-1ubuntu1~22.04' --with-bugurl=file:///usr/share/doc/gcc-11/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-11 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-gcn/usr --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=2
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
```

**Kernal version**

```
root@f055b40ec53a:/# uname -srm
Linux 5.4.0-54-generic x86_64
```

**Download GMP 6.2.0**

```
mkdir ../gmp && cd gmp
wget https://gmplib.org/download/gmp/gmp-6.2.1.tar.lz
```

**Unzip GMP**

```
lzip -d gmp-6.2.1.tar.lz
tar -xvf gmp-6.2.1.tar
cd gmp-6.2.1
```

**Install GMP 6.2.1**

```
./configure --enable-cxx
make
make check
make install
```

### 3. Install NTL

**Download NTL 11.5.1**

```
mkdir ntl && cd ntl
wget https://libntl.org/ntl-11.5.1.tar.gz
```

**Unzip NTL**

```
gzip -d ntl-11.5.1.tar.gz 
tar -xvf ntl-11.5.1.tar
```

**Install NTL**

Be aware that if the `make` step outputs something like: "in consistency between gmp.h and libgmp", it means that the version of GMP you installed was wrong.

```
cd ./ntl/ntl-11.5.1/src
./configure
make
make check
```

The check step should output somethings like:

```
checking results...
0 GOOD
1 GOOD
2 GOOD
3 GOOD
4 GOOD
5 GOOD
```

```
make install
```

**Test if NTL is installed properly**

```
touch test.cpp && vim test.cpp
```

**test.cpp**

```c++
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

int main()
{
   ZZ a, b, c;

   cin >> a;
   cin >> b;
   c = (a+1) * (b+1);
   cout << c << "\n";
}

```

**Run the test code**

```
g++ test.cpp -o test -lntl -pthread -lgmp
./test
```

The output of the test code should be something like:

```
root@f055b40ec53a:/home# ./test
1247612784612784678126312678312
1237861237128637812367812
1544371505018286386974788343593388546562717616304339469
```

This indicates that NTL is successfully installed

### 4. Build the project

**Prepare all the source code and compile them**

```
g++ main.cpp look_up_table_aes.cpp look_up_table_sm4.cpp masked_sm4.cpp masked_aes.cpp utils.cpp gcm.cpp -o whitebox_aes_sm4 -fpermissive -lntl -pthread -lgmp
```

The output should be something like:

```
root@f055b40ec53a:/home/whitebox_aes_sm4# ./whitebox_aes_sm4 
CBC Successful
```

This indicates that project is successfully built

# Build from docker
We will provide docker images later

