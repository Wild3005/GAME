# GAME (MyGame) — C++ / raylib

Game 2D sederhana berbasis **C++** dan **raylib**. Targetnya: pemain bertahan hidup selama mungkin sambil menghindari obstacle (meteor & laser). Cocok untuk demo project/gameplay loop + implementasi state machine.

## Ringkasan
- **Genre**: survival / avoid obstacles
- **Tujuan**: bertahan hidup selama mungkin (timer “Time Survive”)
- **Tantangan**: meteor jatuh + laser muncul periodik
- **Fitur**: HP, pause, 3 skill (teleport, shield, shinra tensei)

## Cara Main
### Menu Awal
- Klik **PLAY** untuk mulai
- Klik **QUIT** untuk keluar

### Kontrol
- Gerak kiri/kanan: `A` / `D`
- Lompat: `W`
- Pause/lanjut: `P`

### Skill
- **Teleport (Hiraishin)**: tahan `Right Shift`, lalu tekan salah satu `A/D/W/S` untuk teleport (cooldown tampil di HUD)
- **Shield**: tahan `Enter` untuk aktifkan shield (durasi singkat)
- **Shinra Tensei**: tahan `Space` untuk memicu cutscene + area efek (cooldown panjang)

### Selesai Game
- Saat HP habis, muncul layar hasil.
- Tekan `Space` untuk keluar.

## Build & Run (Developer)
Project ini memakai **CMake** dan **raylib**.

### 1) Prasyarat (Linux)
- Compiler C/C++: `g++`/`clang++`
- CMake >= 3.25
- Dependency grafis/audio untuk raylib (umumnya X11/GL/ALSA)

Di Debian/Ubuntu biasanya:
```bash
sudo apt update
sudo apt install -y build-essential cmake \
	libgl1-mesa-dev libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev \
	libasound2-dev
```

### 2) Install raylib (sesuai CMake)
Project ini **melinking raylib lewat CMake `add_subdirectory()`**, sesuai konfigurasi di
[CMakeLists.txt](CMakeLists.txt).

Artinya:
- Kamu perlu punya **source code raylib** di path yang sama persis dengan yang ada di CMake:
	`/home/ichbinwil/INSTALL/raylib`
- Raylib akan ikut **di-build bareng project ini** (jadi tidak wajib `sudo install` ke sistem).

Secara teknis, [CMakeLists.txt](CMakeLists.txt) melakukan:
- `add_subdirectory(/home/ichbinwil/INSTALL/raylib raylib)` untuk menambahkan project raylib
- `target_link_libraries(main raylib)` untuk melink executable `main` ke target `raylib`

Kalau di komputermu lokasi raylib berbeda, opsi paling sederhana adalah **ubah path** di
[CMakeLists.txt](CMakeLists.txt) (1 baris `add_subdirectory(...)`) agar sesuai lokasi raylib kamu.

Di bawah ini cara menyiapkan raylib agar path-nya cocok tanpa mengubah CMake.

#### Opsi A — Punya source raylib di folder yang diminta CMake (disarankan)
1) Pastikan dependency raylib terpasang (contoh Debian/Ubuntu):
```bash
sudo apt update
sudo apt install -y git build-essential cmake \
	libgl1-mesa-dev libx11-dev libxcursor-dev libxrandr-dev libxinerama-dev libxi-dev \
	libasound2-dev
```

2) Clone raylib ke lokasi yang dipakai oleh [CMakeLists.txt](CMakeLists.txt):
```bash
mkdir -p /home/ichbinwil/INSTALL
git clone https://github.com/raysan5/raylib.git /home/ichbinwil/INSTALL/raylib
```

Selesai—raylib akan di-compile saat kamu build project ini.

#### Opsi B — Install raylib ke sistem (opsional)
Kalau kamu lebih suka raylib “ter-install” (misalnya untuk project lain), kamu bisa build & install raylib.
Namun untuk project ini, Opsi A sudah cukup.

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
cmake -S . -B build -DBUILD_EXAMPLES=OFF
cmake --build build -j
sudo cmake --install build
```

### 3) Build
Dari root repo:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

### 4) Jalankan
Asset di-load via path relatif dari lokasi executable.

Contoh:
```bash
cd build
./main
```

Atau dari root repo:
```bash
./build/main
```

## Struktur Folder
- [src/main.cpp](src/main.cpp): game loop + state (menu, play, pause, cutscene)
- [include/asset_game.hpp](include/asset_game.hpp): class `Player`, `Meteor`, `Laser` + enum skill
- [include/state_game.hpp](include/state_game.hpp): enum state game
- [Asset_Game_IMG/](Asset_Game_IMG/): asset gambar
- [Asset_Game_Sound/](Asset_Game_Sound/): asset suara

## Troubleshooting
- Jika build gagal karena library X11/GL/ALSA: pastikan paket prasyarat sudah terpasang.
- Jika asset tidak muncul / suara tidak bunyi: jalankan dari folder `build/` (atau pastikan struktur folder asset ada).

## Catatan
- Nama window: "MYGAME" (lihat [src/main.cpp](src/main.cpp)).
- Repo bisa tetap bernama `GAME`, sementara nama project di CMake adalah `MyGame`.