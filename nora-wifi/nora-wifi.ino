#include <WiFi.h>
#include <DNSServer.h>

static const char *ssid = "nora-wifi";
static const char *pwd = "";

WiFiServer server(80);
DNSServer dnsServer;
IPAddress ip( 192, 168, 1, 1); 
IPAddress subnet( 255, 255, 255, 0 ); 

const byte DNS_PORT = 53;

void setup() { 
  Serial.begin(9600);
  WiFi.mode(WIFI_AP); 
  WiFi.softAPConfig(ip, ip, subnet); 
  WiFi.softAP(ssid, pwd);

  dnsServer.start(DNS_PORT, "*", ip);
  
  server.begin();
  Serial.println("ap setup..."); 

  randomSeed(analogRead(0));
}

void loop() {

  dnsServer.processNextRequest();

  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          Serial.println(currentLine);
          if (currentLine.length() == 0) {
            
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>nora-wifi</title></head><body>");
            client.println("<style>*{margin:0;}p{text-align:center;font-size:20px;line-height:300px;animation:kurukuru 6s infinite ease-in-out;}@keyframes kurukuru{0%{transform:rotate3d(0,1,0,0deg);}100%{transform:rotate3d(0,1,0,360deg);}}</style>");
            client.println("<p>🛜</p>");
            client.println("</body></html>");
            client.println();
            client.stop();
            break;

          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
  }
}