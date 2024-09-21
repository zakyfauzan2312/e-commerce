#include <iostream>
using namespace std;

// hash table
struct hashTable{
    /* data */
    string username,password;
} dataAkun[26];

// function hash
int hashFunction(string password){
return tolower(password[0]) - 97;

}

// cek daftar
bool cekDaftarAkun(string username, string password){

    int index = hashFunction(password);
    // cek index kosong atau sudah ditempati
    if(dataAkun[index].username != "" &&dataAkun[index].password != ""){
        // kalau sel index hash table tidak kosong
        cout << "user sudah ada!!" << endl;
        return false;
}else {
    // kalau sel index hash table kosong
    dataAkun[index].username = username;
    dataAkun[index].password = password;
    return true;
}
}

// cek login
bool cekLoginAkun(string username, string password){
    // hashin dulu
    int index = hashFunction(password);
    // cek apakah username & password sama atau tidak
    if (dataAkun[index].username == username && dataAkun[index].password == password){
        // kalau username & password sama
        return true;
    }else{
        // kalau username & password tidak sama
        return false;
    }
}

//cek hapus akun
bool cekHapusAkun(int index){
    // cak apakah sel index dihash table kosong atau tidak
if(dataAkun[index].username =="" && dataAkun[index].password == ""){
    // kalau selnya ksoong
    cout << "\nData akun tidak ada!!" << endl;
    return false;
    }else{
        // kalo ada datanya
        dataAkun[index].username = "";
        dataAkun[index].password = "";
        return true;
    }
}

// print hashtable
void printDataAkun(){
    cout << "data akun :" << endl;
    cout << "| index\t -username - password |" << endl;
    for (int  i = 0; i < 26; i++)
    {
        cout << "| " << i << "\t -";
        if(dataAkun[i].username == ""){
            cout << "(kosong) - ";
        }else {
            cout << dataAkun[i].username << " - "; 
        }
        if(dataAkun[i].password == ""){
            cout << "(kosong) |" << endl;
        }else{
            cout << dataAkun[i].password << " |" << endl;  
        }

    }
    
}

// view daftar akun
void viewDaftarAkun(){
    string username, password; 
    cout << "\nMENU DAFTAR AKUN\n" << endl;
    cout << "masukkan username & password" << endl;
    cout << "isi username anda : ";
    cin >> username;
    cout << "isi password anda : ";
    cin >> password;

    
    if (cekDaftarAkun(username, password)){
        cout << "Pendaftaran berhasil" << endl;
    }else{
        cout << "Pendaftaran gagal" << endl;
    }
}

// view tambah akun
void viewTambahAkun(){
    string username, password; 
    cout << "\nMENU TAMBAH AKUN\n" << endl;
    cout << "masukkan username & password" << endl;
    cout << "isi username akun : ";
    cin >> username;
    cout << "isi password akun : ";
    cin >> password;

    
    if (cekDaftarAkun(username, password)){
        cout << "Pendaftaran berhasil" << endl;
    }else{
        cout << "Pendaftaran gagal" << endl;
    }
}

// view hapus akun
void viewHapusAkun(){
    int index; 
    cout << "\nMENU HAPUS AKUN\n" << endl;
    cout << "isi index anda : ";
    cin >> index;

    if (cekHapusAkun(index)){
        cout << "Akun berhasil dihapus" << endl;
    }else{
        cout << "Akun gagal dihapus" << endl;
    }
    
}

// view menu login
void viewmenulogin(){
    while(true){
        string pilihan;
    cout << "\n** MENU ADMIN **" << endl;
    cout << "pilihan :" << endl;
    cout << "1. Tambah Akun" << endl;
    cout << "2. Hapus Akun" << endl;
    cout << "3. Lihat Data Akun" << endl;
    cout << "4. LogOut" << endl;
    cout << "Masukkan pilihan Anda: ";
    cin >> pilihan;
    if (pilihan == "1") {
    viewTambahAkun();
}else if(pilihan == "2"){
    viewHapusAkun();
}else if (pilihan == "3"){
    printDataAkun();
}else if (pilihan == "4"){
    cout << "Anda Berhasil Logout" << endl;
    break;
}else {
    cout << "Pilihan tidak valid" << endl;
}
    }
}

// view login
void viewlogin(){
    string username, password;
    cout << "\n== VIEW LOGIN ==" << endl;
    cout << "Masukkan username & password" << endl;
    cout << "isi username anda : ";
    cin >> username;
    cout << "isi password anda : ";
    cin >> password;

    // cek username dan password
    if(cekLoginAkun(username, password)){
        cout << "\n Login Berhasil" << endl;
        viewmenulogin();
    }else{
        cout << "\n Login Gagal" << endl;
    }
}    


// tampilan utama
void Welcome(){
    while (true)
    {
    string pilihan;
    cout << "\n** PROGRAM LOGIN SEDERHANA MENGGUNAKAN HASHING **" << endl;
    cout << "pilihan :" << endl;
    cout << "1. Daftar Akun" << endl;
    cout << "2. Login" << endl;
    cout << "3. Keluar" << endl;
    cout << "Masukkan pilihan Anda: ";
    cin >> pilihan;

if (pilihan == "1") {
    viewDaftarAkun();
}else if(pilihan == "2"){
    viewlogin();
}else if (pilihan == "3"){
    cout << "Terima kasih sudah menggunakan program kami" << endl;
    break;
}else {
    cout << "Pilihan tidak valid" << endl;
}
    }
    
}
int main() {

    Welcome();

}
