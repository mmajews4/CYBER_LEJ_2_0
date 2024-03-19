/*
Pamięć FLASH na ESP32 jest za mała. Jest tam 4096 bit czyli 512 B
Na takiej ilości miejsca, zakładające że przekowertuje czas z floata na uint16_t z trzema miejscami po przecinku zawsze żeby wiedzić gdzie go dopisać,
oraz z nazwą praktycznie NIEAKCEPTOWALNIE krótką 8 znaków, 10 B na wpis, mogę pomieścić max 51 wpisów, za mało.

Próbowałem uruchomić kartę microSD z modułem to tej karty za pomocą przykłądu SDtest ale wyświetla się komunikat Initialization failed...
Pomyślałem że może yo być wina konwertera logiki 5V na 3V3 ale jak zamontowałem moduł na arduino nie pomogło.
Zformatowałem kartę na windowsie na FAT32, nie daiła, zformatowałem kartę spocjalnym narzędzem poleconym na forum arduino do FAT16, wykrywa że karta jest w module
ale nie może odczytać jej pojemnochci ani wpisać czy odczytać czegokolwiek.

*/

/* You only need to format LITTLEFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin
   
   If you test two partitions, you need to use a custom
   partition.csv file, see in the sketch folder */

//#define TWOPART

//#define FORMAT_LITTLEFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    
    while(file.available()){                    // Wypisanie zawartości pliku od razu na ekran
      Serial.write(file.read());
    }

    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}

// SPIFFS-like write and delete file, better use #define CONFIG_LITTLEFS_SPIFFS_COMPAT 1

void writeFile2(fs::FS &fs, const char * path, const char * message){
    if(!fs.exists(path)){
		if (strchr(path, '/')) {
            Serial.printf("Create missing folders of: %s\r\n", path);
			char *pathStr = strdup(path);
			if (pathStr) {
				char *ptr = strchr(pathStr, '/');
				while (ptr) {
					*ptr = 0;
					fs.mkdir(pathStr);
					*ptr = '/';
					ptr = strchr(ptr+1, '/');
				}
			}
			free(pathStr);
		}
    }

    Serial.printf("Writing file to: %s\r\n", path);
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void deleteFile2(fs::FS &fs, const char * path){
    Serial.printf("Deleting file and empty folders on path: %s\r\n", path);

    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }

    char *pathStr = strdup(path);
    if (pathStr) {
        char *ptr = strrchr(pathStr, '/');
        if (ptr) {
            Serial.printf("Removing all empty folders on path: %s\r\n", path);
        }
        while (ptr) {
            *ptr = 0;
            fs.rmdir(pathStr);
            ptr = strrchr(pathStr, '/');
        }
        free(pathStr);
    }
}

void testFileIO(fs::FS &fs, const char * path){
    Serial.printf("Testing file I/O with %s\r\n", path);

    static uint8_t buf[512];
    size_t len = 0;
    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }

    size_t i;
    Serial.print("- writing" );
    uint32_t start = millis();
    for(i=0; i<2048; i++){
        if ((i & 0x001F) == 0x001F){
          Serial.print(".");
        }
        file.write(buf, 512);
    }
    Serial.println("");
    uint32_t end = millis() - start;
    Serial.printf(" - %u bytes written in %u ms\r\n", 2048 * 512, end);
    file.close();

    file = fs.open(path);
    start = millis();
    end = start;
    i = 0;
    if(file && !file.isDirectory()){
        len = file.size();
        size_t flen = len;
        start = millis();
        Serial.print("- reading" );
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            if ((i++ & 0x001F) == 0x001F){
              Serial.print(".");
            }
            len -= toRead;
        }
        Serial.println("");
        end = millis() - start;
        Serial.printf("- %u bytes read in %u ms\r\n", flen, end);
        file.close();
    } else {
        Serial.println("- failed to open file for reading");
    }
}

void test_LittleFS(){

#ifdef TWOPART
    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED, "/lfs2", 5, "part2")){
    Serial.println("part2 Mount Failed");
    return;
    }
    appendFile(LITTLEFS, "/hello0.txt", "World0!\r\n");
    readFile(LITTLEFS, "/hello0.txt");
    LITTLEFS.end();

    Serial.println( "Done with part2, work with the first lfs partition..." );
#endif

    if(!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
        Serial.println("LITTLEFS Mount Failed");
        return;
    }
    Serial.println( "SPIFFS-like write file to new path and delete it w/folders" );
    writeFile2(LITTLEFS, "/new1/new2/new3/hello3.txt", "Hello3");
    listDir(LITTLEFS, "/", 3);
    deleteFile2(LITTLEFS, "/new1/new2/new3/hello3.txt");
    
    listDir(LITTLEFS, "/", 3);
    createDir(LITTLEFS, "/mydir");
    writeFile(LITTLEFS, "/mydir/hello2.txt", "Hello2");
    listDir(LITTLEFS, "/", 1);
    deleteFile(LITTLEFS, "/mydir/hello2.txt");
    removeDir(LITTLEFS, "/mydir");
    listDir(LITTLEFS, "/", 1);
    writeFile(LITTLEFS, "/hello.txt", "Hello ");
    appendFile(LITTLEFS, "/hello.txt", "World!\r\n");
    readFile(LITTLEFS, "/hello.txt");
    renameFile(LITTLEFS, "/hello.txt", "/foo.txt");
    readFile(LITTLEFS, "/foo.txt");
    deleteFile(LITTLEFS, "/foo.txt");
    testFileIO(LITTLEFS, "/test.txt");
    deleteFile(LITTLEFS, "/test.txt");
	
    Serial.println( "Test complete" ); 
}