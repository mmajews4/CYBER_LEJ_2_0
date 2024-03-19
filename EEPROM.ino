void inicjuj_LittleFS(){

  if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LITTLEFS Mount Failed");
    return;
  }
}





//void zrob_backup()

void zapisz_we_flashu(){

/*        ---------------     W A Ż N E    --------------------

  Mam wrażenie że to bardzo źle że tyle razy nadpisuje flash do którego można wpisywać tylko 100000 razy

  Teoretycznie mógłbym zrobić jedną zmienną w której bym zapisał cały plik i później go przesłać
  lub podzielić zawartość na kilka większych pakietów i tak przesyłąć żeby nie zużywac dużej ilości pamięci

  Czy warto robić backup czy to za dużo pamięci zabiera?

!!! Gdzie są te pliki? Wiem że we flashu ale czy da radę je jakoś przesłać na kompa

  Zapis 15 wyników trwa około sekundy, co gdyby zapisać trzeba było 150 wyników...
  trzeba to jakoś skrócić np patrząc co się zmienia od ostatniego zapisu lub może jakieś inne pomysły?

*/
  writeFile(LITTLEFS, "/wyniki.csv", (String(wyniki[0].czas)+ ",0\n").c_str()); 

  for(int i = 1; i <= wyniki[0].czas; i++)
  {
    appendFile(LITTLEFS, "/wyniki.csv", (String(wyniki[i].czas)+','+ String(wyniki[i].nazwa)+'\n').c_str());
    //appendFile(LITTLEFS, "/wyniki.csv", (String(wyniki[i].nazwa)+ '\n').c_str());
  }


  writeFile(LITTLEFS, "/ranking.txt", (String(ranking_index[0])+ ',').c_str());

  for(int j = 1; ranking_index[j] != 0; j++)
  {
    appendFile(LITTLEFS, "/ranking.txt", (String(ranking_index[j])+ ',').c_str());
  }
  appendFile(LITTLEFS, "/ranking.txt", "0,");

  //readFile(LITTLEFS, "/wyniki.csv");
  //listDir(LITTLEFS, "/", 3);

  //Serial.println(odczyt_wynikow_flash);

/*  listDir(LITTLEFS, "/", 3);
	writeFile(LITTLEFS, "/hello2.txt", "Hello2");
	listDir(LITTLEFS, "/", 1);
	deleteFile(LITTLEFS, "/mydir/hello2.txt");
	removeDir(LITTLEFS, "/mydir");*/
/*	listDir(LITTLEFS, "/", 1);
  writeFile(LITTLEFS, "/hello.txt", "Hello ");
  appendFile(LITTLEFS, "/hello.txt", "World!\r\n");
  readFile(LITTLEFS, "/hello.txt");*/
/*  renameFile(LITTLEFS, "/hello.txt", "/foo.txt");
  readFile(LITTLEFS, "/foo.txt");
  deleteFile(LITTLEFS, "/foo.txt");
  testFileIO(LITTLEFS, "/test.txt");
  deleteFile(LITTLEFS, "/test.txt");*/
}

void czytaj_z_flashu(){

  fs::FS &fs = LITTLEFS; 
  char* path = "/wyniki.csv";

  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");

  int lineCount = 0;                              // Wczytenie pliku od razu do tablicy wyników
  char bufor[8] = "";
  char* endptr;

  while (lineCount <= wyniki[0].czas && file.available()) {                    

    String line = file.readStringUntil(',');                                        // Czytam część przed przecinkiem, zamieniam ją na float i wpisuję do czasu
    line.toCharArray(bufor, sizeof(bufor));
    wyniki[lineCount].czas = strtof(bufor, &endptr);

    String line2 = file.readStringUntil('\n');
    line2.toCharArray(wyniki[lineCount].nazwa, sizeof(wyniki[lineCount].nazwa));

    lineCount++;
  }

  file.close();

  char* path2 = "/ranking.txt";

  Serial.printf("Reading file: %s\r\n", path2);

  File file2 = fs.open(path2);
  if(!file2 || file2.isDirectory()){
    Serial.println("- failed to open file for reading");
    return;
  }

  Serial.println("- read from file:");

  lineCount = 0;                              // Wczytenie pliku od razu do tablicy wyników
  char bufor2[5] = "";

  while (file2.available()) {                    

    String line = file2.readStringUntil(',');                                        // Czytam część przed przecinkiem, zamieniam ją na float i wpisuję do czasu
    line.toCharArray(bufor2, sizeof(bufor2));
    if(!strcmp(bufor2, "0,") || !strcmp(bufor2, "0,")){
      Serial.println("Doszedłem do ostatnego wpisu w rankingu");
      break;
    }
    ranking_index[lineCount] = atoi(bufor2);

    lineCount++;
  }
  Serial.println("Wyszedłem z while");

  file2.close();
}