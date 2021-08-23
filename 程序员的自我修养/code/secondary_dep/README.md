# 目的
研究动态链接库间接依赖的行为，例如存在如下依赖关系：app -> bar.so -> foo.so，那么bar.so生成时，是否指定foo.so的影响。
参见：http://www.kaizou.org/2015/01/linux-libraries.html

# 动态链接库查找顺序
The linker uses the following search paths to locate required shared libraries:
* directories specified by -rpath-link options (more on that later)
* directories specified by -rpath options (more on that later)
* directories specified by the environment variable LD_RUN_PATH
* directories specified by the environment variable LD_LIBRARY_PATH
* directories specified in DT_RUNPATH or DT_RPATH of a shared library are searched for shared libraries needed by it
* default directories, normally /lib and /usr/lib
* directories listed in the /etc/ld.so.conf file
