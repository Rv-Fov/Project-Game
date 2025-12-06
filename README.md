# Project Strukdat

Nama  : Muhammad Arifin Umasangadji
NRP   : 5024241083

Pada project matkul ini kita diminta untuk membuat simulasi sederhana yang menampilkan bola bergerak terus menerus dan akan memantul jika menyentuh jendela atau bola lain dengan minimal menggunakan BruteForce dan Quadtree. Disini menggunakan raylib sebagai library dan ada tambahan fitur.

## Fitur Utama

-   **Dua Metode Deteksi Tabrakan:**
    -   Quadtree: Implementasi struktur data Quadtree untuk membagi ruang simulasi. Metode ini secara signifikan mengurangi jumlah pasangan bola yang harus diperiksa, menawarkan kinerja yang lebih efisien terutama dengan jumlah bola yang tinggi (Diaktifkan secara default: `USE_QUADTREE = true`.
    -   Brute Force: Metode sederhana di mana setiap bola diperiksa tabrakannya dengan setiap bola lainnya. Memiliki kompleksitas waktu $O(N^2)$, yang menjadi lambat saat jumlah bola (N) bertambah.
-   **Simulasi Fisika:**
    -   Pergerakan bola yang terus-menerus.
    -   Deteksi dan resolusi tabrakan dengan jendela dan bola lain.
   
-   **Interaksi Pengguna:** Memungkinkan pengguna untuk mengubah jumlah bola secara *real-time* melalui *input* keyboard.
-   **Visualisasi:** Bola dengan ukuran, kecepatan, dan warna yang diacak, serta tampilan *frame rate* (FPS) dan metode yang sedang digunakan.


# Kode Penjelasan
![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%201.png?raw=true)

Kode ini digunakan untuk konfigurasi berjalannya simulasi ini, dengan menggunakan metode quadtree dan bruteforce juga library raylib. Terdapat juga beberapa parameter untuk ukuran layar, batas jumlah karakter input yang ditampilkan, serta batas maksimal jumlah bola.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%202.png?raw=true)

Mendefinisikan dua struktur yaitu `Ball` dan `RectangleBounds`. `Ball` berfungsi untuk mencetak setiap bola dengan properti posisi, kecepatan, warna, dan id. `rectangleBounds` struktur yang digunakan untuk definisi area jendela dengan batas-batas nya untuk deteksi tabrakan.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%203.png?raw=true)

Kode ini merupakan implementasi struktur Quadtree yang menjadi dasar untuk menyimpan area, kedalaman, daftar objek, dan 4 pointer. `Getindex()` berfungsi untuk menentukan kuadran mana (0, 1, 2, atau 3) tempat objek (`Ball`) harus disimpan, berdasarkan posisi bola relatif terhadap titik tengah batas node. 
`Subdivide()` berfungsi untuk membagi node saat ini menjadi empat node anak yang lebih kecil setelah batas kapasitas objek tercapai.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%204.png?raw=true)

fungsi `SpawnBalls` berfungsi untuk menginisialisasi dan menempatkan sejumlah bola ke dalam simulasi. Pertama memastikan random number generator telah di-set (`srand`) agar bola selalu memiliki atribut acak setiap kali dipanggil, kemudian membersihkan daftar bola yang ada (`balls.clear()`) dan memastikan jumlah bola yang akan dibuat tidak melebihi batas global (`BALL_LIMIT`). Dalam loop, membuat objek Ball baru, memberikan radius acak (10-25), posisi awal, kecepatan awal, dan warna yang masing-masing teracak. Setiap bola diberi ID, sehingga siap untuk disimulasikan di loop utama.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%205.png?raw=true)

Kode yang ditampilkan merupakan fungsi implementasi dasar dari algoritma deteksi tabrakan antar objek berbentuk lingkaran. Fungsi ini mengambil dua objek Ball (`b1` dan `b2`) sebagai input dan bekerja berdasarkan prinsip geometri Euclidean. Dengan menghitung selisih jarak horizontal (dx) dan vertikal (dy) antara pusat kedua bola dengan rumus Jarak Euclidean (teorema Pythagoras: $d = \sqrt{dx^2 + dy^2}$), didapat nilai untuk menentukan jarak sebenarnya antara dua pusat bola. Fungsi ini mengembalikan nilai true jika jarak yang dihitung tersebut lebih kecil atau sama dengan jumlah dari kedua radius bola (`b1.radius + b2.radius`), yang dimana menandakan bahwa bola tersebut bertabrakan atau bersentuhan.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%206.png?raw=true)

Fungsi `ResolveCollision` bertanggung jawab untuk menangani dampak tabrakan antara dua bola. Fungsi ini melakukan dua hal utama: pertama, ia mereaksi kecepatan kedua bola dengan menukar nilai `speed_x` dan `speed_y` masing-masing bola yang menyebabkan kedua bola memantul dari satu sama lain. Kedua, kode ini menangani penyesuaian posisi untuk mencegah overlap (saling tumpang tindih) yang menyebabkan glitch. Dengan menghitung overlap dan distance, bola-bola tersebut didorong menjauh satu sama lain sepanjang vektor normal (`normalX`, `normalY`) sejauh setengah dari nilai overlap tersebut. Selain itu ada juga tambahan fitur setiap bola bertabrakan warna akan berganti secara acak.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%207.png?raw=true)

`int main()`, fungsi utama yang mengontrol titik awal dan seluruh loop yang ada. Inisialisasi raylib untuk memanggil `InitWindow` untuk membuat jendela game berukuran $1400 \times 800$ piksel dan mengatur frame rate agar stabil pada 60 FPS (`SetTargetFPS(60)`). Selanjutnya, kode ini mendeklarasikan variabel penting seperti `balls` (vektor untuk menyimpan semua objek Ball) dan `input_text` (untuk input pengguna). Game dimulai dengan memanggil `SpawnBalls(balls, 10)` untuk membuat 10 bola pertama. Terakhir, ia mendefinisikan batas jendela `worldBounds` dan menginisialisasi akar Quadtree (`quadtreeRoot`) ke nullptr, menandakan persiapan untuk game loop utama yang akan menangani semua pembaruan fisika, deteksi tabrakan, dan penggambaran.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%208.png?raw=true)

Kode yang ditampilkan berfungsi sebagai sistem pemrosesan input pengguna di dalam game loop untuk memasukkan angka dan membuat bola baru. `While loop` dirancang untuk menangani pengetikan angka, di mana setiap ditekan akan ditambahkan ke string `input_text` dengan ketentuan harus angka. Kode di Baris 13-17 untuk menghapus karakter terakhir dari `input_text` jika string tidak kosong. Pada Baris 19-27, ketika Enter ditekan akan mengkonversi `input_text` menjadi angka (`num_to_spawn`) dan memanggil fungsi SpawnBalls untuk membuat bola sebanyak angka tersebut.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%209.png?raw=true)

Kode ini adalah bagian penting dari game loop yang bertanggung jawab untuk mensimulasikan pergerakan setiap bola dan menangani tabrakan bola dengan batas-batas jendela. Loop utama mengiterasi melalui setiap bola (`ball`) dalam vektor balls. Pertama, posisi bola (`ball.x` dan `ball.y`) diperbarui dengan menambahkan nilai kecepatannya (`ball.speed_x` dan `ball.speed_y`), sehingga bola bergerak di layar. Kemudian, dua kode `if` dan `else if` terpisah menangani tabrakan dengan dinding: Jika bola menyentuh batas horizontal (`SCREEN_WIDTH` atau 0), kecepatan sumbu X-nya dibalik (`*= -1`), dan posisinya disesuaikan agar tidak menembus dinding. Logika yang sama diterapkan untuk tabrakan dengan batas vertikal (`SCREEN_HEIGHT` atau 0), membalik kecepatan sumbu Y.

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%2010.png?raw=true)

Kode ini bertugas mengelola sistem deteksi dan resolusi tabrakan dalam simulasi juga logika Quadtree diimplementasikan sebagai optimasi. Jika konstanta global `USE_QUADTREE` bernilai true, kode akan membersihkan dan membangun ulang Quadtree (`quadtreeRoot`) di setiap frame, dengan memasukkan semua bola (`balls`) ke dalamnya. Setelah itu, untuk setiap bola, ia akan meminta (`query`) Quadtree untuk mendapatkan daftar kandidat tabrakan terdekat (`candidates`). Jika tabrakan terdeteksi antara bola dan salah satu kandidatnya (`CheckCollisionCircles`), maka fungsi `ResolveCollision` dipanggil. Jika `USE_QUADTREE` bernilai false, kode akan menjalankan deteksi tabrakan bruteforce dengan membandingkan setiap bola dengan setiap bola lainnya dalam loop ganda (i dan j).

![alt text](https://github.com/Rv-Fov/Project-Game/blob/master/kode%2011.png?raw=true)

Kode ini berfungsi untuk menggambar semua elemen game ke layar dan melakukan pembersihan memori sebelum menutup aplikasi. Bagian Drawing dimulai dengan `BeginDrawing()` dan diakhiri dengan `EndDrawing()`. Di antara itu, layar di-clear menjadi hitam (`ClearBackground(BLACK)`), Frame Per Second (FPS) ditampilkan, dan mode tabrakan yang sedang aktif (Quadtree atau Brute Force) ditampilkan. Kode ini kemudian menggambar kotak input dan teksnya, diikuti dengan menggambar setiap bola (`DrawCircle`) di layar, dan menampilkan jumlah bola yang sedang aktif. Saat jendela ditutup, kode di Baris 24-25 akan menghapus objek Quadtree dari memori (`delete quadtreeRoot`) untuk mencegah kebocoran memori. Terakhir, `CloseWindow()` menutup jendela Raylib dan aplikasi berakhir.

>
<br>


