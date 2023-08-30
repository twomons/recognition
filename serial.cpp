#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

int *serial(char *file_serisl,char *data) {
    // 打开串口设备文件
    int serialPort = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    // 检查串口是否成功打开
    if (serialPort == -1) {
        std::cout << "无法打开串口设备" << std::endl;
        return -1;
    }
    // 配置串口参数
    struct termios serialParams;
    tcgetattr(serialPort, &serialParams);
    cfsetispeed(&serialParams, B9600);  // 设置波特率为9600
    cfsetospeed(&serialParams, B9600);
    tcsetattr(serialPort, TCSANOW, &serialParams);
    
    // 发送数据
    char data[] = "Hello, Serial!";
    write(serialPort, data, sizeof(data));
    
    // 读取数据
    char buffer[100];
    int bytesRead = read(serialPort, buffer, sizeof(buffer));
    
    // 显示接收到的数据
    if (bytesRead > 0) {
        std::cout << "接收到的数据：";
        for (int i = 0; i < bytesRead; i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;
    }
    
    // 关闭串口
    close(serialPort);
    
    return *buff;
}
