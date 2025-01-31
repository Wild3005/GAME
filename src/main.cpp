#include <iostream>
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <chrono>

#include "../include/state_game.hpp"
#include "../include/asset_game.hpp"

using namespace std;

const float panjang_layar = 1366;
const float lebar_layar = 768;


// const float

//variabel global intro
int state = 0;                  // Tracking animation states (State Machine)
float alpha = 1.0f;             // Useful for fading
int framesCounter = 0;
int lettersCount = 0;
int topSideRecWidth = 16;
int leftSideRecHeight = 16;
int bottomSideRecWidth = 16;
int rightSideRecHeight = 16;
int logoPositionX = panjang_layar/2 - 128;
int logoPositionY = lebar_layar/2 - 128;

//variabel global texture
Texture2D Character[3];
Texture2D Character_L[3];
Texture2D Cutscene[6];
Image background;

state_texture state_img = STAND;
int facing = 0; // 0 = kekanan , 1 = kekiri
const float frame_switchrate = 0.1f;
float frame_switchtimer = 0.0f;
int current_frame_character = 0;
const float rate_frame_character = 0.1f;
float timer_frame_character = 0.0f; 

//soubd game
Sound SFX_Shinra_Tensei[3];

// variabel state mulai
int state_temp = 0;
Rectangle button_mulai = {panjang_layar/2-60,lebar_layar/2,150,50};
Rectangle button_keluar = {panjang_layar/2-60,lebar_layar/2+80,150,50};

// state_game mode_game = INTRO;
state_game mode_game = SCREEN_MULAI;

//cooldown skill
float cooldown_skill_teleport = 0.0f;
float cooldown_rate_skill_teleport = 5.0f;

float cooldown_skill_shield = 0.0f;
float cooldown_rate_skill_shield = 60.0f;
float cooldown_skill_shield_active = 0.0f;
float cooldown_rate_skill_shield_active = 3.0f;
bool shield_active = false;

float cooldown_skill_shinra_tensei = 0.0f;
float cooldown_rate_skill_shinra_tensei = 60.0f;


//variabel play
Player player1;
Rectangle bola;
float deltatime = 0.0f;

//variabel draw
float draw_deltatime = 0.0f;

//variabel loncat
float velY = 0.0f;
float g = 1000.0f;
float jumpforce = -650.0f;
bool isjump = false;

//variabel respawn
float respawnRate_meteor = 1.0f; // Interval waktu respawn dalam detik
const float respawnRate_laser = 10.0f;
const float respawnRate_trigger_laser = 1.0f;
const float respawnRate_trigger_pralaser = 2.0f;
float respawnTimer_meteor = 0.0f;
float respawnTimer_laser = 0.0f;
float respawnTimer_laser_trigger = 0.0f;
float respawnTimer_trigger_pralaser = 0.0f;
float Rate_Obstacle = 20.0f;
float Timer_Obstacle = 0.0f;
float Time_Survive = 0.0f;

//variabel teleport
// bool condition_teleport = false;

//variabel obtacle
std::vector<Meteor> meteors;
std::vector<Laser> lasers;

Vector2 mousePoint = GetMousePosition();

//variabel shinra tensei
Rectangle shinra_jutsu;
bool shinra_tensei_active = false;
float shinra_expand_speed = 1000.0f; // Kecepatan ekspansi Shinra Tensei
float shinra_max_width = panjang_layar; // Batas maksimum lebar
float shinra_max_height = lebar_layar; // Batas maksimum tinggi
bool shinra_tensei_growing = false; // Apakah kotak sedang tumbuh

//variabel cutscene
float rate_cutscene_frame = 0.05f;
float timer_cutscene_frame = 0.0f;
int current_frame_cutscene = 0;

const float rate_pra_cutscene = 2.0f;
float timer_pra_cutscene = 0.0f;


//fungsi-fungsi
void inisiasi_player();
// void inisiasi_bola(int& kondisi);
void koresi_layar(Player& player);
void gravitasi(Player& player);
void spawn_meteors_logic(std::vector<Meteor> &meteors, float screen_width, float screen_height);
void update_meteor(Player &player ,Meteor &meteors, float deltaTime);
void spawn_lasers_logic(std::vector<Laser> &lasers, float screen_height);
void update_laser(Player &player,Laser &lasers, float deltaTime);
void update_pralaser(Laser &laser);

const void set_reset_jutsu_shinratensei();

void cek_dmg_meteor(Player& player, Object& Object);
void cek_dmg_laser(Player& player, Object& Object);

void mode_skill(Player& player);

const void update_intro();
const void draw_intro();

const void update_screen_awal();
const void draw_screen_awal();

const void update_play(float &deltaT);
const void draw_play(float &deltaT);

const void respawn_meteor();
const void respawn_laser();

const void update_screen_final();
const void draw_screen_final();

void shinra_active();
void draw_shinra_active();

const void update_pause();
const void draw_pause();

void draw_texture_character();
void update_texture();


void update_cutscene_shinra_tensei(float &time);
void draw_cutscene_shinra_tensei();

void update_pra_cutscene(float &time);
void draw_pra_cutscene();


int main(){
    InitWindow(panjang_layar, lebar_layar, "MYGAME");
    ToggleFullscreen();
    SetTargetFPS(60);

    InitAudioDevice();
    SFX_Shinra_Tensei[0] = LoadSound("/home/ichbinwil/game_project/Asset_Game_Sound/shinraa-tensei.mp3");
    SFX_Shinra_Tensei[1] = LoadSound("/home/ichbinwil/game_project/Asset_Game_Sound/ytmp3free.mp3");
    SFX_Shinra_Tensei[2] = LoadSound("/home/ichbinwil/game_project/Asset_Game_Sound/teleport-anime.mp3");


    //initiation local
    inisiasi_player();
    srand(time(NULL));
    // int kondisi_bola_random = rand() % 20;
    // inisiasi_bola(kondisi_bola_random);

    background = LoadImage("/home/ichbinwil/game_project/Asset_Game_IMG/30314886.jpg");

    //variabel for texture
    Character[0] = LoadTexture("../Asset_Game_IMG/Stand_Player.png");
    Character[1] = LoadTexture("../Asset_Game_IMG/run_player1.png");
    Character[2] = LoadTexture("../Asset_Game_IMG/run_Player2.png");
    if(Character[0].id == 0){
        printf("failed koad img\n");
        return 1;
    }
    // printf("texture load: id = %d, w = %d, h = %d",Character.id,Character.width, Character.height);

    Character_L[0] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/Stand_L_player (20241213032636).png");
    Character_L[1] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/Run_L_player1 (20241213031620).png");
    Character_L[2] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/Run_L_player1 (20241213031620).png");

    Cutscene[0] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes1 (20241213053332).png");
    Cutscene[1] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes2 (20241213061254).png");
    Cutscene[2] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes3 (20241213062927).png");
    Cutscene[3] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes4 (20241213080443).png");
    Cutscene[4] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes5 (20241213090353).png");
    Cutscene[5] = LoadTexture("/home/ichbinwil/game_project/Asset_Game_IMG/eyes6 (20241213090252).png");
    while (!WindowShouldClose()) {
        deltatime = GetFrameTime();

        switch (mode_game){
            case INTRO:
            update_intro();
            break;
            case SCREEN_MULAI:
            update_screen_awal();
            break;
            case PLAY:
            update_play(deltatime);
            break;
            case SCREEN_PEMENANG:
            update_screen_final();
            break;
            case PAUSE:
            update_pause();
            break;
            case CUTSCENE:
            update_cutscene_shinra_tensei(deltatime);
            break;
            case PRA_CUTSCENE:
            update_pra_cutscene(deltatime);
            break;
        
        
        }
        BeginDrawing();
        switch(mode_game){
            case INTRO:
            draw_intro();
            break;
            case SCREEN_MULAI:
            draw_screen_awal();
            break;
            case PLAY:
            draw_play(deltatime);
            break;
            case SCREEN_PEMENANG:
            draw_screen_final();
            break;
            case PAUSE:
            draw_pause();
            break;
            case CUTSCENE:
            draw_cutscene_shinra_tensei();
            break;
            case PRA_CUTSCENE:
            draw_pra_cutscene();
            break;
            
        }
        EndDrawing();
    }

    for(int i = 0;i < 3;i++){
        UnloadTexture(Character[i]);
    }
    for(int i = 0;i < 6;i++){
        UnloadTexture(Cutscene[i]);
    }
    for(int i = 0;i < 3;i++){
        UnloadTexture(Character_L[i]);
    }
    for(int i = 0;i < 3;i++){
        UnloadSound(SFX_Shinra_Tensei[i]);
    }
    UnloadImage(background);
    CloseWindow();

    return 0;
}

const void update_screen_awal(){
    mousePoint = GetMousePosition();
    if(state_temp == 0){
        if(CheckCollisionPointRec(mousePoint, button_mulai)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) state_temp = 1;
        }
        else if(CheckCollisionPointRec(mousePoint, button_keluar)){
            if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) CloseWindow();
        }
    }else if(state_temp == 1){
        mode_game = PLAY;
    }
}

const void draw_screen_awal(){
    ClearBackground(RAYWHITE);
    // ImageClearBackground(&background,BLACK);
    // cout << "posisi x:" << mousePoint.x << "\nposisi y:" << mousePoint.y << endl;
    DrawRectangleRec(button_mulai,CheckCollisionPointRec(mousePoint, button_mulai)? GRAY : WHITE );
    DrawRectangleRec(button_keluar,CheckCollisionPointRec(mousePoint, button_keluar)? GRAY : WHITE);
    DrawText("myGAME", 30, 30, 50, BLACK);
    DrawText("PLAY", panjang_layar/2-50, lebar_layar/2, 50, CheckCollisionPointRec(mousePoint, button_mulai)? WHITE : BLACK);
    DrawText("QUIT", panjang_layar/2-50, lebar_layar/2+80, 50, CheckCollisionPointRec(mousePoint, button_keluar)? WHITE : BLACK);
}

void inisiasi_player(){
    player1.rec.x = 256;
    player1.rec.y = lebar_layar/2;
    player1.rec.height = 200;
    player1.rec.width = 200;
    player1.inithp(100);
    player1.skill = NONE;
    player1.hitbox = {player1.rec.x,player1.rec.y,player1.rec.width-100,player1.rec.height};
}

// void inisiasi_bola(int& kondisi){
//     if(kondisi >= 0 && kondisi <=9){
//         bola.x = 276;
//         bola.y = lebar_layar/2;
//         bola.height = 5;
//         bola.width = 5;
//     }
//     else if (kondisi >= 10 && kondisi <= 19){
//         bola.x = 1090;
//         bola.y = lebar_layar/2;
//         bola.height = 5;
//         bola.width = 5;
//     }
// }

// void inisiasi_meteor(){
//     meteors.rec.height = 20;
//     meteors.rec.width = 20;
// }

const void update_intro(){
    if (state == 0)                 // State 0: Small box blinkin
        {
            framesCounter++;

            if (framesCounter == 120)
            {
                state = 1;
                framesCounter = 0;      // Reset counter... will be used later...
            }
        }
        else if (state == 1)            // State 1: Top and left bars growing
        {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;

            if (topSideRecWidth == 256) state = 2;
        }
        else if (state == 2)            // State 2: Bottom and right bars growing
        {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;

            if (bottomSideRecWidth == 256) state = 3;
        }
        else if (state == 3)            // State 3: Letters appearing (one by one)
        {
            framesCounter++;

            if (framesCounter/12)       // Every 12 frames, one more letter!
            {
                lettersCount++;
                framesCounter = 0;
            }

            if (lettersCount >= 10)     // When all letters have appeared, just fade out everything
            {
                alpha -= 0.02f;

                if (alpha <= 0.0f)
                {
                    alpha = 0.0f;
                    mode_game = SCREEN_MULAI;
                }
            }
        }
}

const void draw_intro(){
    ClearBackground(RAYWHITE);
            if (state == 0)
            {
                if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
            }
            else if (state == 1)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
            }
            else if (state == 2)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

                DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
            }
            else if (state == 3)
            {
                DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

                DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                DrawRectangle(GetScreenWidth()/2 - 112, GetScreenHeight()/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth()/2 - 44, GetScreenHeight()/2 + 48, 50, Fade(BLACK, alpha));
            }
}

const void update_play(float &deltaT){
    PlaySound(SFX_Shinra_Tensei[0]);
    Time_Survive += deltaT;
    player1.hitbox.x = player1.rec.x+25;
    player1.hitbox.y = player1.rec.y;
    if(!shield_active){
        player1.move_player(KEY_A,KEY_D);
        gravitasi(player1);   
    }
    koresi_layar(player1);

    respawn_meteor();
    respawn_laser();

    //cek skill
    if(cooldown_skill_teleport <= 0.0f && IsKeyDown(KEY_RIGHT_SHIFT)){
        // condition_teleport = true;
        player1.skill = HIRAISHIN;
        mode_skill(player1);
        player1.skill = NONE;
    }
    if(cooldown_skill_teleport > 0.0f) cooldown_skill_teleport -= deltatime;

    if(cooldown_skill_shield <= 0.0f && IsKeyDown(KEY_ENTER)){
        player1.skill = SHIELD;
        mode_skill(player1);
        player1.skill = NONE;
    }
    if(cooldown_skill_shield > 0.0f) cooldown_skill_shield -= deltatime;
    if(cooldown_skill_shield_active > 0.0f) cooldown_skill_shield_active -= deltatime;
    if(cooldown_skill_shield_active <= 0) shield_active = false;

    // if(cooldown_skill_shinra_tensei <= 0 && IsKeyDown(KEY_SPACE)){
    //     player1.skill = SHINRA_TENSEI;
    //     shinra_jutsu = player1.rec;
    //     mode_skill(player1);
    //     player1.skill = NONE;
    // }

    if (cooldown_skill_shinra_tensei <= 0 && IsKeyDown(KEY_SPACE)) {
        mode_game = PRA_CUTSCENE;
        if (!shinra_tensei_active) {
            player1.skill = SHINRA_TENSEI;
            shinra_jutsu = {player1.rec.x, player1.rec.y, player1.rec.width, player1.rec.height};
            shinra_tensei_active = true;
            shinra_tensei_growing = true; // Memulai proses pertumbuhan
            cooldown_skill_shinra_tensei = cooldown_rate_skill_shinra_tensei;
        }
    }
    if(cooldown_skill_shinra_tensei > 0.0f) cooldown_skill_shinra_tensei -= deltatime;

    if (shinra_tensei_active) {
        shinra_active();
    }
    
    //cek unutuk pause
    if (IsKeyPressed(KEY_P)) {
        mode_game = PAUSE;
    }

    //timer rate obstacle respawn
    Timer_Obstacle += deltaT;
    if(Timer_Obstacle >= Rate_Obstacle){
        respawnRate_meteor = respawnRate_meteor * 0.5f;
        Rate_Obstacle += 20.0f;
        Timer_Obstacle = 0.0f;
    }

    update_texture();
}

const void draw_play(float &deltaT){
    ClearBackground(RAYWHITE);

    // Draw players
    // DrawRectangle(player1.rec.x, player1.rec.y, player1.rec.width, player1.rec.height, RED);
    // DrawRectangle(bola.x, bola.y, bola.width, bola.height, BROWN);

    draw_texture_character();

    //respawn meteor
    for (auto &meteor : meteors) {
        if (meteor.active) {
            DrawRectangleRec(meteor.rec, BLUE);
        }
    }
    //respawn laser
    for (auto &laser : lasers) {
        if (laser.active) {
            laser.rec.width = panjang_layar;
            DrawRectangleRec(laser.rec, YELLOW);
        }else if(laser.pra_active){
            DrawRectangleRec(laser.rec, RED);
        }
    }

    //cek hp player
    if(player1.gethp() <= 0){
        mode_game = SCREEN_PEMENANG;
    }

    //shinra draw
    if (shinra_tensei_active) {
        DrawRectangle(shinra_jutsu.x, shinra_jutsu.y, shinra_jutsu.width, shinra_jutsu.height, GRAY);
    }

    //gambar hp player
    DrawText("HP", 10, 40, 20, BLACK);
    DrawLineEx({10,65},{player1.gethp(),65},10,BLACK);
    DrawText(TextFormat("Cooldown Teleport: %.2f", cooldown_skill_teleport), 15, lebar_layar-70, 20, BLACK);
    DrawText(TextFormat("Cooldown Shieald: %.2f", cooldown_skill_shield), 10, lebar_layar-40, 20, BLACK);
    DrawText(TextFormat("Cooldown shinra tensei: %.2f", cooldown_skill_shinra_tensei), 280, lebar_layar-40, 20, BLACK);
    DrawText(TextFormat("Time Survive: %.2f", Time_Survive), panjang_layar-270, 10, 20, BLACK);
    DrawText(TextFormat("Ok: %d", facing), 10, 80, 20, BLACK); //temp debug
    printf("Ok: %d\n", facing);
    // DrawLineEx({0,648},{panjang_layar,648},10,MAGENTA);
    // DrawText(TextFormat("Cooldown Shieald_dur: %.2f", cooldown_skill_shield_active), 10, 500, 20, BLACK);
    // DrawText(TextFormat("Cooldown Shieald_dur: %d", shield_active), 10, 520, 20, BLACK);
}

void koresi_layar(Player& player) {
    if (player.rec.x > panjang_layar-100)
        player.rec.x = panjang_layar-100;
    if (player.rec.x < -50)
        player.rec.x = -50;
    if (player.rec.y >= 468){
        player.rec.y = 468;
        velY = 0;
        isjump = false;
    }
}

void gravitasi(Player& player){
    if(IsKeyPressed(KEY_W)&&!isjump){
        velY = jumpforce;
        isjump = true;
    }
    velY += g*deltatime;
    player.rec.y += velY * deltatime;
}

void update_meteor(Player &player,Meteor &meteors, float deltaTime) {
    if (meteors.active) {
        meteors.rec.x += meteors.speed.x * deltaTime; // Gerakan horizontal
        meteors.rec.y += meteors.speed.y * deltaTime; // Gerakan vertikal

        // Cek jika meteors keluar dari layar dan nonaktifkan
        if (meteors.rec.y > lebar_layar || meteors.rec.x < 0 || meteors.rec.x > panjang_layar) {
            meteors.active = false; // Nonaktifkan meteor jika keluar layar
        }
        if(CheckCollisionRecs(player.hitbox,meteors.rec)){
            player.dmghp(5);
            meteors.active = false;
            //skill shield
            if(shield_active){
                player.healplayer(20);
            }
        }
        if(CheckCollisionRecs(meteors.rec,shinra_jutsu)){
            meteors.active = false;
        }
    }
}

void spawn_meteors_logic(std::vector<Meteor> &meteors, float screen_width, float screen_height) {
    Meteor meteor;
    meteor.rec.height = 20;
    meteor.rec.width = 20;
    meteor.rec.x = rand() % static_cast<int>(screen_width);
    meteor.rec.y = -meteor.rec.height;
    meteor.speed.y = 200 + rand() % 200;
    meteor.speed.x = rand() % 100 - 50;
    meteor.active = true;
    meteors.push_back(meteor);
}
void update_laser(Player &player, Laser &lasers, float deltaTime) {
    if(lasers.pra_active){
        respawnTimer_trigger_pralaser += deltaTime;
        if(respawnTimer_trigger_pralaser >= respawnRate_trigger_pralaser){
            lasers.active = true;
            lasers.pra_active = false;
            respawnTimer_trigger_pralaser = 0.0;
        }
    }
    if (lasers.active) {
        respawnTimer_laser_trigger += deltaTime;
        if(respawnTimer_laser_trigger >= respawnRate_trigger_laser){
            lasers.active = false;
            respawnTimer_laser_trigger = 0.0;
        }
        //logika buat hit player
        if(!shinra_tensei_active){
            if(CheckCollisionRecs(lasers.rec,player.rec)){
                player.dmghp(0.5f);
            }
        }
    }
}



void spawn_lasers_logic(std::vector<Laser> &lasers, float screen_height){
    Laser laser;
    laser.rec.height = 10;
    laser.rec.width = 10;
    laser.rec.x = 10;
    laser.rec.y = 200 + rand() % 500;
    laser.active = false;
    laser.pra_active = true;
    lasers.push_back(laser);
}

const void respawn_meteor(){
    //respawn meteor
    respawnTimer_meteor += deltatime;
    if (respawnTimer_meteor >= respawnRate_meteor) {
        spawn_meteors_logic(meteors,panjang_layar,lebar_layar);
        respawnTimer_meteor = 0.0f; // Reset timer respawn
    }
    for (auto &meteor : meteors) {
        if (meteor.active) {
            update_meteor(player1,meteor,deltatime);
        }
    }
}
const void respawn_laser(){
    //respawn laser
    respawnTimer_laser += deltatime;
    if(respawnTimer_laser >= respawnRate_laser){
        spawn_lasers_logic(lasers, lebar_layar);
        respawnTimer_laser = 0.0f;
    }
    for(auto &laser : lasers){
        if(laser.pra_active || laser.active){
            update_laser(player1,laser,deltatime);
        }
    }
}

// void cek_dmg_meteor(Player& player, Object& Object){
//     if(CheckCollisionRecs(player.rec,Object.rec) && !player1.skill == SHIELD){
//         player.dmghp(5);
//     }
// }

// void cek_dmg_laser(Player& player, Object& Object){
//     if(CheckCollisionRecs(player.rec,Object.rec) && !player1.skill == SHIELD){
//         player.dmghp(15);
//     }
// }

const void update_screen_final(){
    if(IsKeyPressed(KEY_SPACE)){
        CloseWindow();
     }
    //  else if(IsKeyPressed(KEY_ENTER)){
    //     mode_game = SCREEN_MULAI;
    // }
}

const void draw_screen_final(){
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("Waktu Bertahan Anda: %.2f Detik",Time_Survive),100,lebar_layar/2,50,BLACK);
    DrawText("tekan [SPACE] untuk keluar dari game",100,lebar_layar/2+200,25,BLACK);
    // DrawText("tekan [ENTER] untuk keluar dari game",400,lebar_layar/2+200,25,BLACK);
    
}

void mode_skill(Player& player){
    //skill hiraishin
    if(player1.skill == HIRAISHIN && (IsKeyPressed(KEY_A)||IsKeyPressed(KEY_D)||IsKeyPressed(KEY_W)||IsKeyPressed(KEY_S))){
        PlaySound(SFX_Shinra_Tensei[2]);
        if (IsKeyPressed(KEY_A)) {
            player1.rec.x -= 400;
        }
        if (IsKeyPressed(KEY_D)) {
            player1.rec.x += 400;
        }
        if (IsKeyPressed(KEY_W)) {
            player1.rec.y -= 400;
        }
        if (IsKeyPressed(KEY_S)) {
            player1.rec.y += 400;
        }
        cooldown_skill_teleport = cooldown_rate_skill_teleport;
    }
    if(player1.skill == SHIELD){
        shield_active = true;
        cooldown_skill_shield = cooldown_rate_skill_shield;
        cooldown_skill_shield_active = cooldown_rate_skill_shield_active;
    }
    if(player1.skill == SHINRA_TENSEI){
        shinra_tensei_active = true;
        cooldown_skill_shinra_tensei  = cooldown_rate_skill_shinra_tensei;
    }
}

// void shinra_active(){
//     if(shinra_jutsu.height < lebar_layar){
//         shinra_jutsu.width += 20;
//         shinra_jutsu.height += 20;
//     }else shinra_tensei_active = false;
// }

void shinra_active() {
    if (shinra_tensei_growing) {
        shinra_jutsu.width += shinra_expand_speed * deltatime;
        shinra_jutsu.height += shinra_expand_speed * deltatime;

        // Posisi kotak diperluas ke kiri dan atas
        shinra_jutsu.x -= (shinra_expand_speed * deltatime) / 2;
        shinra_jutsu.y -= (shinra_expand_speed * deltatime) / 2;

        // Jika ukuran sudah mencapai layar penuh, hentikan
        if (shinra_jutsu.width >= shinra_max_width*2 || shinra_jutsu.height >= shinra_max_height*2) {
            shinra_tensei_growing = false;
            shinra_tensei_active = false;
            set_reset_jutsu_shinratensei();
        }
    }
}

const void update_pause() {
    if (IsKeyPressed(KEY_P)) {
        mode_game = PLAY;
    }
}

const void draw_pause() {
    ClearBackground(RAYWHITE);
    DrawText("PAUSE - Tekan P untuk melanjutkan", 50, 50, 30, BLACK);
}

const void set_reset_jutsu_shinratensei(){
    shinra_jutsu.height = 0;
    shinra_jutsu.width = 0;
    shinra_jutsu.x = player1.rec.x;
    shinra_jutsu.y = player1.rec.y;
}

void draw_texture_character(){

    Rectangle srcrec;
    Rectangle destrec;
    Vector2 origin;

    // if(facing == 0){
    //     if(!IsKeyDown(KEY_A) || !IsKeyDown(KEY_D)){
    //         state_img = STAND;
    //     }else if(IsKeyDown(KEY_A)){
    //         facing = 1;  
    //     }else if(IsKeyDown(KEY_D)){
    //         state_img = RUN;
    //         facing = 0;
    //     }
    // }else if(facing == 1){
    //     if(!IsKeyDown(KEY_A) || !IsKeyDown(KEY_D)){
    //         state_img = STAND;
    //     }else if(IsKeyDown(KEY_A)){
    //         state_img = RUN_L;
    //         facing = 1;
    //     }else if(IsKeyDown(KEY_D)){
    //         facing = 0;
    //     }
    // }

    // if(IsKeyDown(KEY_A)){
    //     if(facing != 1){
    //         facing = 1;
    //     }
    // }

    // switch(state_img){
    //     case STAND:
    //     if(facing == 1){
    //         srcrec = {0,0,(float)Character_L[0].width,(float)Character_L[0].height};
    //         destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
    //         origin = {0,0};

    //         DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
    //         DrawTexturePro(Character_L[0],srcrec,destrec,origin,0.0f,WHITE);
    //     }else if(facing == 0){
    //         srcrec = {0,0,(float)Character[0].width,(float)Character[0].height};
    //         destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
    //         origin = {0,0};

    //         DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
    //         DrawTexturePro(Character[0],srcrec,destrec,origin,0.0f,WHITE);
    //     }
    // }
    if(facing == 0 && !IsKeyDown(KEY_D)){
        current_frame_character = 0;
        srcrec = {0,0,(float)Character[0].width,(float)Character[0].height};
        destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
        origin = {0,0};

        // DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
        DrawTexturePro(Character[0],srcrec,destrec,origin,0.0f,WHITE);
    }
    else if(facing == 1 && !IsKeyDown(KEY_A)){
        current_frame_character = 0;
        srcrec = {0,0,(float)Character_L[0].width,(float)Character_L[0].height};
        destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
        origin = {0,0};

        // DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
        DrawTexturePro(Character_L[0],srcrec,destrec,origin,0.0f,WHITE);
    }else if(facing == 0){
        timer_frame_character += deltatime;
        srcrec = {0,0,(float)Character[0].width,(float)Character[0].height};
        destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
        origin = {0,0};

        if(timer_frame_character >= rate_frame_character){
            current_frame_character += 1;
            if(current_frame_character >= 2){
                current_frame_character = 1;
            }
        }

        // DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
        DrawTexturePro(Character[current_frame_character],srcrec,destrec,origin,0.0f,WHITE);
    }else if(facing == 1){
        timer_frame_character += deltatime;
        srcrec = {0,0,(float)Character_L[0].width,(float)Character_L[0].height};
        destrec = {player1.rec.x-100,player1.rec.y-90,player1.rec.width+150,player1.rec.height+150};
        origin = {0,0};

        if(timer_frame_character >= rate_frame_character){
            current_frame_character += 1;
            if(current_frame_character >= 2){
                current_frame_character = 1;
            }
        }

        // DrawRectangle(player1.hitbox.x,player1.hitbox.y,player1.hitbox.width,player1.hitbox.height,RED);
        DrawTexturePro(Character_L[current_frame_character],srcrec,destrec,origin,0.0f,WHITE);
    }
}

void update_texture(){
    if(IsKeyDown(KEY_D)){
        facing = 0;
    }else if(IsKeyDown(KEY_A)){
        facing = 1;
    }
}

void update_cutscene_shinra_tensei(float &time){
    PlaySound(SFX_Shinra_Tensei[1]);
    timer_cutscene_frame += time;
    if(timer_cutscene_frame >= rate_cutscene_frame){
        current_frame_cutscene +=1;
        timer_cutscene_frame = 0;
    }
    if(current_frame_cutscene == 6){
        mode_game = PLAY;
        current_frame_cutscene = 0;
    }
}

void draw_cutscene_shinra_tensei(){
    ClearBackground(BLACK);
    Rectangle srcrec_cutscene;
    Rectangle destrec_cutscene;
    Vector2 origin_cutscene;

    // if(current_frame_cutscene == 0){
        
    // }
    srcrec_cutscene = {0,0,(float)Cutscene[0].width,(float)Cutscene[0].height};
    destrec_cutscene = {panjang_layar/2-(822/2),lebar_layar/2-(292/2),822,192};
    origin_cutscene = {0,0};

    DrawTexturePro(Cutscene[current_frame_cutscene],srcrec_cutscene,destrec_cutscene,origin_cutscene,0.0f,WHITE);

}

void update_pra_cutscene(float &time){
    timer_pra_cutscene += time;
    if(timer_pra_cutscene >= rate_pra_cutscene){
        mode_game = CUTSCENE;
        timer_pra_cutscene = 0;
    }
}

void draw_pra_cutscene(){
    ClearBackground(BLACK);
}