"# Arduino" 
FOR THE FINAL LAB EXAM**


FOR LAB 8
เปิด Terminal 1 มาแล้วพิมพ์
sudo apt update
sudo apt upgrade
sudo apt-get install -y mosquitto mosquitto-clients
sudo systemctl enable mosquitto.service
จากนั้นพิมพ์
sudo nano /etc/mosquitto/mosquitto.conf
จะเข้าหน้าไฟล์ ให้พิมพ์ listener 1883 allow_anonymous true

Ctrl O Enter แล้ว Ctrl X ออกมา

เปิด Topic
mosquitto -d
ps | grep mosquitto
mosquitto -d -t topic

เปิด terminal 2
mosquitto_pub -d -t topic -m "test"
