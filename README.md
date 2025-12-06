# Project Strukdat

Nama  : Muhammad Arifin Umasangadji
NRP   : 5024241083

Pada project matkul ini kita diminta untuk membuat simulasi sederhana yang menampilkan bola bergerak terus menerus dan akan memantul jika menyentuh jendela atau bola lain dengan minimal menggunakan BruteForce dan Quadtree. Disini menggunakan raylib sebagai library dan ada tambahan fitur.

## Fitur Utama

-   **Dua Metode Deteksi Tabrakan:**
    -   Quadtree: Implementasi struktur data Quadtree untuk membagi ruang simulasi. Metode ini secara signifikan mengurangi jumlah pasangan bola yang harus diperiksa, menawarkan kinerja yang lebih efisien terutama dengan jumlah bola yang tinggi (Diaktifkan secara default: `USE_QUADTREE = true`).
    -   Brute Force: Metode sederhana di mana setiap bola diperiksa tabrakannya dengan setiap bola lainnya. Memiliki kompleksitas waktu $O(N^2)$, yang menjadi lambat saat jumlah bola (N) bertambah.
-   **Simulasi Fisika:**
    -   Pergerakan bola yang terus-menerus.
    -   Deteksi dan resolusi tabrakan dengan jendela dan bola lain.
   
-   **Interaksi Pengguna:** Memungkinkan pengguna untuk mengubah jumlah bola secara *real-time* melalui *input* keyboard.
-   **Visualisasi:** Bola dengan ukuran, kecepatan, dan warna yang diacak, serta tampilan *frame rate* (FPS) dan metode yang sedang digunakan.


# Kode Penjelasan

<p align="center">
  <img src="Library.jpg" alt="" width="800">
</p>

// Sintaks: ![Teks Alternatif](path/ke/gambar)

![Kode_Program](Raylib-CPP-Starter-Template-for-VSCODE-main/kode_1.png)

<br>
<br>


