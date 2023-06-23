// Lab 13-5
#include "mbed.h"

#define SERVER_IP "172.20.10.4"
#define SERVER_PORT 11100


UnbufferedSerial pc(CONSOLE_TX, CONSOLE_RX, 230400); 
WiFiInterface *wifi;

TCPSocket socket;
Thread sock_thread;

char rxBuf_pc[80];
char txBuf_pc[80];
char buffer[80];
char juno[80];


int index = 0;
volatile int flag;


void rx_cb(void){
    char ch;
    pc.read(&ch,1);
    if(ch != '\n') pc.write(&ch,1);
    juno[index++] = ch;
    if(ch == '\r') {
        int idx=0;
        for(int i=0 ; i<index ; i++){
            if(juno[i]!='\r' && juno[i] != '\n') rxBuf_pc[idx++] = juno[i];
        }
        
        rxBuf_pc[idx] = '\0';
        index = 0;
        flag = 1;
    }
}

void rx_thread(){
    char* buf = (char*)malloc(1024);
    
    while(true){
        nsapi_size_or_error_t size = socket.recv(buf, 1024);
        if(size <= 0){
            if(size == NSAPI_ERROR_WOULD_BLOCK) continue;
            
            sprintf(buffer, "Close or Error while receiving data (%d)\r\n", size);
            pc.write(buffer,strlen(buffer));
            return ;
        }
        buf[size] = '\0';
        sprintf(buffer, "\r\nRX data: (%d) %s \r\n", size, buf);
        pc.write(buffer,strlen(buffer));
    }
    
}

// main() runs in its own thread in the OS 
int main() {
    SocketAddress sockAddr;
    SocketAddress serverAddr(SERVER_IP, SERVER_PORT);
    
    sprintf(buffer, "\r\nWiFi TCP Server example\r\n");
    pc.write(buffer, strlen(buffer));
    sprintf(buffer, "\r\nSet SoftAP+Station mode\r\n");
    pc.write(buffer, strlen(buffer));
    pc.attach(rx_cb);
    
    wifi = WiFiInterface::get_default_instance();
    if(!wifi){
        sprintf(buffer, "ERROR: No WiFiInterface found.\n");
        pc.write(buffer, strlen(buffer));
        while(1){};
    }
    
    sprintf(buffer, "Connecting to %s...\r\n", MBED_CONF_APP_WIFI_SSID);
    pc.write(buffer, strlen(buffer));
    
    int ret = wifi->connect( MBED_CONF_APP_WIFI_SSID,  MBED_CONF_APP_WIFI_PASSWORD, NSAPI_SECURITY_WPA_WPA2);
    if(ret != 0){
        sprintf(buffer, "Connection error!!\r\n");
        pc.write(buffer, strlen(buffer));
        return -1;
    }
    
    sprintf(buffer, "Success!!\r\n");
    pc.write(buffer, strlen(buffer));
    
    sprintf(buffer, "RSSI : %d\r\n",wifi->get_rssi());
    pc.write(buffer, strlen(buffer));
    
    sprintf(buffer, "MAC : %s\r\n",wifi->get_mac_address());
    pc.write(buffer, strlen(buffer));
    
    wifi->get_ip_address(&sockAddr);
    sprintf(buffer, "IP : %s, ",sockAddr.get_ip_address());
    pc.write(buffer, strlen(buffer));
    
    wifi->get_netmask(&sockAddr);
    //pc.printf("Netmask : %s, ", sockAddr.get_ip_address());
    sprintf(buffer, "Netmask : %s, ",sockAddr.get_ip_address());
    pc.write(buffer, strlen(buffer));
    
    wifi->get_gateway(&sockAddr);
    sprintf(buffer, "Gateway : %s\r\n",sockAddr.get_ip_address());
    pc.write(buffer, strlen(buffer));
    
    //open a TCP socket on the network interface
    socket.open(wifi);
    
    // create a TCP connection to a Server
    int response = socket.connect(serverAddr);
    if(response != 0){
        sprintf(buffer, "Error connecting : %d\r\n",response);
        pc.write(buffer, strlen(buffer));
        socket.close();
        return -1;
    }
    
    sock_thread.start(&rx_thread);
    
    while(true){
        flag = 0;
        sprintf(buffer, "Enter characters to send to a server : ");
        pc.write(buffer, strlen(buffer));
        while(flag != 1){}
            
        if(!strcmp(rxBuf_pc,"q") || !strcmp(rxBuf_pc,"Q")){
            pc.write(buffer, strlen(buffer));
            break;
        }   
        else{
            int len = strlen(rxBuf_pc);
            sprintf(buffer, "Sent: %s   (%d)\r\n", rxBuf_pc, len);
            pc.write(buffer, strlen(buffer));
              
            rxBuf_pc[len++] = '\r';
            rxBuf_pc[len++] = '\n';

            socket.send((const char*)rxBuf_pc, len);
            ThisThread::sleep_for(500ms); // 0.5sec  
        }
        
    }
    
    socket.close();
    wifi->disconnect();
    
    sock_thread.join();
    sprintf(txBuf_pc, "RX sock_thread joined!!\r\n");
    pc.write(txBuf_pc, strlen(txBuf_pc));
    sprintf(txBuf_pc, "\nDone\r\n");
    pc.write(txBuf_pc, strlen(txBuf_pc));
    
    while(1){};
}
 