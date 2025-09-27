#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define BUF_SIZE 256  // 缓冲区大小

int main() {
    // 打开串口设备
    int fd = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY);
    if (fd == -1) {
        perror("无法打开串口");
        return -1;
    }

    // 配置串口参数
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);  // 输入波特率115200
    cfsetospeed(&options, B115200);  // 输出波特率115200
    options.c_cflag |= (CLOCAL | CREAD);  // 启用接收和本地模式
    options.c_cflag &= ~PARENB;        // 无奇偶校验
    options.c_cflag &= ~CSTOPB;        // 1位停止位
    options.c_cflag &= ~CSIZE;         // 清除数据位掩码
    options.c_cflag |= CS8;            // 8位数据位
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  // 非规范模式
    options.c_iflag &= ~(IXON | IXOFF | IXANY);           // 关闭流控制
    options.c_oflag &= ~OPOST;         // 原始输出模式

    // 应用配置
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        perror("串口配置失败");
        close(fd);
        return -1;
    }

    // 清空缓冲区
    tcflush(fd, TCIFLUSH);

    char buffer[BUF_SIZE] = {0};  // 接收缓冲区
    int buffer_len = 0;           // 当前缓冲区数据长度

    printf("开始监听串口数据...\n");

    while (1) {
        // 读取串口数据
        char temp_buf[BUF_SIZE];
        int n = read(fd, temp_buf, sizeof(temp_buf));
        if (n > 0) {
            // 检查缓冲区溢出
            if (buffer_len + n >= BUF_SIZE) {
                fprintf(stderr, "缓冲区溢出，数据丢失！\n");
                buffer_len = 0;  // 清空缓冲区
            }
            // 将新数据追加到缓冲区
            memcpy(buffer + buffer_len, temp_buf, n);
            buffer_len += n;
        } else if (n < 0) {
            perror("读取错误");
            break;
        }

        // 处理缓冲区中的命令
        int processed = 0;  // 已处理的数据长度
        while (processed < buffer_len) {
            // 查找命令起始符'X'
            char *x_pos = memchr(buffer + processed, 'X', buffer_len - processed);
            if (!x_pos) {
                // 无更多命令，跳出循环
                break;
            }

            // 计算命令起始位置在缓冲区中的索引
            int cmd_start = x_pos - buffer;

            // 查找下一个'X'作为命令结束符
            char *next_x = memchr(x_pos + 1, 'X', buffer_len - cmd_start - 1);
            int cmd_end = next_x ? (next_x - buffer) : buffer_len;

            // 提取命令字符串
            int cmd_length = cmd_end - cmd_start;
            char cmd[cmd_length + 1];
            memcpy(cmd, buffer + cmd_start, cmd_length);
            cmd[cmd_length] = '\0';  // 添加字符串终止符

            // 解析命令参数
            int x, y, w, p, t, g;
            if (sscanf(cmd, "X%dY%dW%dP%dT%dG%d", &x, &y, &w, &p, &t, &g) == 6) {
                printf("收到指令: X=%d Y=%d W=%d P=%d T=%d G=%d\n", x, y, w, p, t, g);
            } else {
                printf("解析失败: %s\n", cmd);
            }

            // 更新已处理位置
            processed = cmd_end;
        }

        // 移除已处理的数据
        if (processed > 0) {
            memmove(buffer, buffer + processed, buffer_len - processed);
            buffer_len -= processed;
        }

        usleep(10000);  // 等待10ms避免CPU过载
    }

    close(fd);
    return 0;
}
