from flask import Flask, Response, request, send_from_directory
import cv2
import threading
import time
import os
from nanoSerial import NanoSerial  # 导入 nanoSerial.py 中的类

app = Flask(__name__, static_folder='static')

# 串口配置
SERIAL_PORT = '/dev/ttyTHS1'
BAUDRATE = 115200

# 初始化串口
nano_serial = NanoSerial(SERIAL_PORT, BAUDRATE)

# 串口发送线程
def serial_sender():
    last_sent = b''  # 在函数内部初始化 last_sent 变量为字节流
    while True:
        if nano_serial.is_open():
            try:
                # 优先处理即时指令
                current_X = app.config.get('current_X', 0)
                current_Y = app.config.get('current_Y', 0)
                current_W = app.config.get('current_W', 0)
                current_P = app.config.get('current_P', 0)
                current_T = app.config.get('current_T', 0)
                current_G = app.config.get('current_G', 0)

                # 转换数据类型
                move_X = current_X 
                move_Y = current_Y 
                move_Z = current_W 
                pose = current_P % 256     # 无符号 8 位整型
                T = bool(current_T)       # 布尔类型
                G_flg = current_G % 256    # 无符号 8 位整型

                # 组合指令
                command = f"X{int(move_X)}Y{int(move_Y)}W{int(move_Z)}P{int(pose)}T{int(T)}G{int(G_flg)}"
                #command = f"X0Y{int(move_Y)}W{int(move_Z)}P{int(pose)}T{int(T)}G{int(G_flg)}"
                command_bytes = command.encode()  # 将字符串编码为字节流

                # 发送指令
                if command_bytes != last_sent:
                    nano_serial.send(command)
                    last_sent = command_bytes

                time.sleep(0.01)  # 缩短等待时间
            except Exception as e:
                print(f"Serial error: {str(e)}")
                time.sleep(1)

# 静态文件路由
@app.route('/')
def index():
    return send_from_directory(app.static_folder, 'index.html')

# 指令接收接口
@app.route('/command', methods=['POST'])
def handle_command():
    data = request.get_json()
    new_Y = data.get('Y', 0)
    new_W = data.get('W', 0)
    new_P = data.get('P', 0)
    new_T = data.get('T', 0)
    new_G = data.get('G', 0)

    app.config['current_Y'] = new_Y
    app.config['current_W'] = new_W
    app.config['current_P'] = new_P
    app.config['current_T'] = new_T
    app.config['current_G'] = new_G

    return {'status': 'success'}

# 视频流路由
@app.route('/video_feed')
def video_feed():
    def generate():
        camera = cv2.VideoCapture(0)
        camera.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        camera.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        
        while True:
            success, frame = camera.read()
            if not success: 
                break
            
            frame = cv2.flip(frame, 0)
            frame = cv2.flip(frame, 1)

            _, buffer = cv2.imencode('.jpg', frame)
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + buffer.tobytes() + b'\r\n')
    
    return Response(generate(), mimetype='multipart/x-mixed-replace; boundary=frame')

if __name__ == '__main__':
    # 创建静态文件夹
    if not os.path.exists('static'):
        os.makedirs('static')

    # 初始化串口
    if nano_serial.open():
        threading.Thread(target=serial_sender, daemon=True).start()
        app.run(host='0.0.0.0', port=7500, threaded=True)
    else:
        print("Serial port not available. Starting server without serial support.")
        app.run(host='0.0.0.0', port=7500, threaded=True)
