int pesar(){
  int peso;
        if (scale.is_ready()) {
         long reading = scale.read();
                if(reading <= 220000){
                  peso = 0xA1;
                }else if(reading <= 300000){
                  peso = 0xA2;
                }else{
                 peso = 0xA3;
                }
          Serial.write(peso);
          //Serial.println(reading);
        } else {
         Serial.println(0, BIN);
        }
        return peso;
}
