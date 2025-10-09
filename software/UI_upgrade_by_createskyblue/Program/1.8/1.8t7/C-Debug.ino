/*
//This snippet is for DEBUG
void HelpMeSerialer() {
  //Serial port assistant
  if (Serial.available()) {
    char TmpChar = Serial.read();
    switch (TmpChar) {
      case 'x': x = Serial.parseInt();
      case 'y': y = Serial.parseInt();
      case 'a': a = Serial.parseInt();
      case 'b': b = Serial.parseInt();
    }
  }
  
    Serial.println(String("X ->") + x);
    Serial.println(String("Y ->") + y);
    Serial.println(String("A ->") + a);
    Serial.println(String("B ->") + b);
  
}
*/
