#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;

int main()
{
    RenderWindow app(VideoMode(400, 400), "15-Puzzle!");
    app.setFramerateLimit(60); 

    Font font;
    // Memuat font dari file ARCADE_I.TTF untuk digunakan dalam teks
    if (!font.loadFromFile("ARCADE_I.TTF"))
    {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    int w = 100; // Ukuran tiap blok dalam grid adalah 100x100 piksel
    int grid[6][6] = { 0 }; // Matriks 6x6 digunakan untuk menyimpan nilai grid (termasuk padding untuk batas)
    Text text[20]; // Array teks untuk menampilkan angka pada grid

    // Inisialisasi grid dan objek teks
    int n = 0;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            n++; // Menghitung angka (1-15)
            grid[i + 1][j + 1] = n; // Memasukkan angka ke dalam grid (dengan offset +1 untuk padding)
            text[n].setFont(font);
            text[n].setString(std::to_string(n)); // Mengatur angka sebagai string
            text[n].setCharacterSize(48); // Ukuran teks
            text[n].setFillColor(Color::White); // Warna teks putih
            FloatRect textBounds = text[n].getLocalBounds();
            text[n].setOrigin(textBounds.width / 2, textBounds.height / 2); // Pusatkan teks pada kotak
        }

    grid[4][4] = 16; // Menandai kotak kosong (angka 16 dianggap kosong)

    // Mengacak grid dengan menggeser kotak kosong secara acak
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 1000; i++) // Loop untuk mengacak 1000 langkah
    {
        int x = 4; // Posisi awal kotak kosong
        int y = 4;

        int dx = 0, dy = 0; // Delta untuk menggerakkan kotak kosong
        int dir = rand() % 4; // Pilih arah acak (0 = kiri, 1 = kanan, 2 = atas, 3 = bawah)
        if (dir == 0) dx = -1;
        if (dir == 1) dx = 1;
        if (dir == 2) dy = -1;
        if (dir == 3) dy = 1;

        // Periksa apakah pergerakan valid (tidak keluar dari grid)
        if (grid[x + dx][y + dy] != 0 && x + dx >= 1 && x + dx <= 4 && y + dy >= 1 && y + dy <= 4)
        {
            std::swap(grid[x][y], grid[x + dx][y + dy]); // Tukar nilai dengan kotak kosong
        }
    }

    bool isWin = false; // Menandai apakah puzzle sudah selesai atau belum

    // Loop utama aplikasi
    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e)) // Memeriksa semua event yang terjadi
        {
            if (e.type == Event::Closed) // Jika jendela ditutup
                app.close();

            if (e.type == Event::KeyPressed && !isWin) // Jika ada tombol ditekan dan belum menang
            {
                int x = 0, y = 0;

                // Cari posisi kotak kosong (16)
                for (int i = 1; i <= 4; i++)
                    for (int j = 1; j <= 4; j++)
                        if (grid[i][j] == 16)
                        {
                            x = i;
                            y = j;
                        }

                int dx = 0, dy = 0; // Delta untuk pergerakan
                if (e.key.code == Keyboard::Left)  dx = 0, dy = 1;  // Geser ke kiri
                if (e.key.code == Keyboard::Right) dx = 0, dy = -1; // Geser ke kanan
                if (e.key.code == Keyboard::Up)    dx = 1, dy = 0;  // Geser ke atas
                if (e.key.code == Keyboard::Down)  dx = -1, dy = 0; // Geser ke bawah

                // Periksa apakah pergerakan valid
                if (grid[x + dx][y + dy] != 0 && x + dx >= 1 && x + dx <= 4 && y + dy >= 1 && y + dy <= 4)
                {
                    std::swap(grid[x][y], grid[x + dx][y + dy]); // Tukar nilai dengan kotak kosong
                }
            }
        }

        // Periksa kondisi kemenangan
        isWin = true;
        int n = 0;
        for (int i = 1; i <= 4; i++)
            for (int j = 1; j <= 4; j++)
            {
                n++;
                if (n == 16) break; // Kotak kosong diabaikan
                if (grid[i][j] != n) isWin = false; // Jika urutan salah, belum menang
            }

        app.clear(Color::White); // Bersihkan layar dengan warna putih

        // Gambar grid dan angka
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                int n = grid[i + 1][j + 1];
                if (n == 16) continue; // Kotak kosong tidak digambar

                // Gambar blok hitam
                RectangleShape block(Vector2f(w - 2, w - 2));
                block.setFillColor(Color::Black);
                block.setPosition(j * w + 1, i * w + 1);
                app.draw(block);

                // Gambar angka pada blok
                text[n].setPosition(j * w + w / 2, i * w + w / 2);
                app.draw(text[n]);
            }

        if (isWin) // Jika menang, tampilkan pesan "YOU WIN!"
        {
            Text winText;
            winText.setFont(font);
            winText.setString("YOU WIN!");
            winText.setCharacterSize(48);
            winText.setFillColor(Color::Red);
            FloatRect textBounds = winText.getLocalBounds();
            winText.setOrigin(textBounds.width / 2, textBounds.height / 2);
            winText.setPosition(app.getSize().x / 2, app.getSize().y / 2);
            app.draw(winText);
        }

        app.display(); // Tampilkan semua elemen di layar
    }

    return 0; // Keluar dari program
}
