void inicjuj_LittleFS(){

  if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
    Serial.println("LITTLEFS Mount Failed");
    return;
  }
}

//void zrob_backup()

void zapisz_we_flashu(){

  writeFile(LITTLEFS, "/wyniki.csv", (String(wyniki[0].czas)+ ",0\n").c_str());

  for(int i = 1; i <= wyniki[0].czas; i++)
  {

    appendFile(LITTLEFS, "/wyniki.csv", (String(wyniki[i].czas)+ ',').c_str());
    appendFile(LITTLEFS, "/wyniki.csv", (String(wyniki[i].nazwa)+ '\n').c_str());

  }

  readFile(LITTLEFS, "/wyniki.csv");
  listDir(LITTLEFS, "/", 3);

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