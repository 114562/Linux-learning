/*
    #include <sys/stat.h>
    修改文件的权限
    参数
        - pathname： 需要修改的文件的路径
        - mode： 需要修改的权限值，八进制的数
    返回值
        成功返回0 
        失败返回-1
    int chmod(const char *pathname, mode_t mode);
*/

#include <sys/stat.h>
#include <stdio.h>

int main(){
    int ret = chmod("a.txt",0775);
    if(ret == -1){
        perror("chmod");
        return -1;
    }

    printf("修改成功\n");\
    return 0;
}