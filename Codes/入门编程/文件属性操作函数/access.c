/*
    #include <unistd.h>
    参数
        - pathname 判断的文件路径
        - mode 
            R_OK: 判断是否有读权限
            W_OK: 判断是否有写权限
            X_OK: 判断是否有执行权限
            F_OK: 判断文件是否有存在
    返回值:
        成功 0
        失败 -1
    int access(const char *pathname, int mode);
    作用:判断某个文件是否有某个权限，或者判断文件是否存在
*/

#include <unistd.h>
#include <stdio.h>

int main(){
    int ret = access("a.txt" , F_OK);
    if(ret == -1){
        perror("access");
        return -1;
    }
    printf("文件存在\n");

}