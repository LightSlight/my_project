#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>


static void usage(char *prog_name){
    fprintf(stderr, "Invalid input\n");
    fprintf(stderr, "Usage: %s  file  device  start\n", prog_name);
    fprintf(stderr, "Example: %s ./test.txt  /dev/sdb  0\n", prog_name);
}

int main(int argc, char *argv[]){
    char buf[4096];
    int fdd = 0;
    int fds = 0;
  //  int i = 0;
  //  int j = 0;
    int start = 0;
    int count = 0;
    char *device = NULL;
    char *src_device = NULL;

    if(argc != 4){
        usage(basename(argv[0]));
        exit(1);
    }
    
    src_device = argv[1];
    device = argv[2];
    start = atol(argv[3]) * 512; // 输入的start表示从指定的扇区开始写数据，转换之后变成字节计数
   // count = atol(argv[3]);       // count表示读取多少字节的数据

    fdd = open(device, O_RDWR);
    if(-1 == fdd){
        fprintf(stderr, "Cannot open %s\nNo such device file.\n", device);
        exit(1);
    
    }
    fds = open(src_device, O_RDONLY);
    if(-1 == fdd){
        fprintf(stderr, "Cannot open %s\nNo such file.\n", src_device);
        exit(1);
    }

    count = read(fds,buf,sizeof(buf));
    if(-1 == count){
        fprintf(stderr, "Read fail!");
        exit(1);
    }

    // 将要写入的设备文件指针指向输入的指定扇区
    if(lseek(fdd, start, SEEK_SET) != start){
        fprintf(stderr, "Cannot seek at %d", start);
        exit(1);
    }

    while(count){
        write(fdd, buf, count);// 写入数据
        count = read(fds, buf, sizeof(buf));
    }

//   while(count > 0){
//       int size = count > sizeof(buf) ? sizeof(buf) : count;
//
//       read(fdd, buf, size);
//
//       /* 每行显示16个字节，每两个字节间以空格隔开 */
//       for(i = 0; i <= size/16; i++){
//           fprintf(stdout, "%08x:", i*16+start);       
//           for(j = 0; j < 16; j++){
//               fprintf(stdout, "%02x", (int)buf[i*16+j] & 0xFF);
//               if((j+1) % 2 == 0){
//                   fprintf(stdout, " ");
//               }
//           }
//           fprintf(stdout, "\n");
//           fflush(stdout);
//       }
//       count -= size;
//       start += size;
//   
//   }
    close(fds);
    close(fdd);
    return 0;
}

