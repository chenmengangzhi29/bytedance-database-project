## 一、介绍
- 项目的主要需求是实现数据在内存中的多种**关系存储**，并且编写一个能够支持四种不同类型查询的数据库，以衡量**数据布局**和**索引**对于**查询性能**的影响；
- 支持四种方式的数据存储，以及其对应的Table，即：RowTable、ColumnTable、IndexedRowTable、CustomTable；
- IndexedRowTable即在行存储的基础之上增加索引加速查询，代码中利用**红黑树**的数据结构来实现索引；
- CustomTable通过衡量前三种Table的性能，合理选择数据布局和数据索引以获得更良好的数据查询平均性能；
- 针对上述四种Table，实现了**select**和**update**相关的查询；
- 使用**Cmake**进行编译代码，通过**Googletest**框架完成联调测试；
- 这个项目是基于斯坦福课程cs245的。

## 二、环境准备
- cmake version(recommended): >=3.9.2
- C++ version: >=C++11
- gcc/g++ version: >=6.3.0

## 三、编译和测试
### 1.第一步
在你的环境中配置Googletest测试框架

`sh configure.sh`

配置完成后，Googletest的二进制文件将被安装在你的环境中。

### 2.第二步
使用C++语言实现你的数据库，然后生成二进制可执行文件（用于单元测试的执行）

如果你在进行lab1实验，可以单独建立lab1的单元测试，

`sh build.sh --clean --lab1`

或者单独建立lab2的测试,

`sh build.sh --clean --lab2`

或者同时建立两者的测试

`sh build.sh --clean --lab1 --lab2`

### 3.第三步
进入测试文件夹，运行所有单元测试。

`cd build && ctest -VV -R "database_*" -j`
