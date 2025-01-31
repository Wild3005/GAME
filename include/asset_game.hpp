#ifndef ASSET_GAME
#define ASSET_GAME

#include <raylib.h>
#include <vector>
#include <cmath>

enum Skill{
        NONE,
        HIRAISHIN,
        SHIELD,
        SHINRA_TENSEI
};

class Object{
    public:
    Rectangle rec;
};

class Meteor: public Object{
    public:
    bool active;
    Vector2 speed;
};

class Laser: public Object{
    public:
    bool active;
        bool pra_active;
    Rectangle tringger_laser;
};

class Player: public Object{
    public:
    Rectangle hitbox;
    Skill skill;
    void move_player(int keyLeft, int keyRight) {
        if (IsKeyDown(keyLeft)) {
            rec.x -= 10;
        }
        if (IsKeyDown(keyRight)) {
            rec.x += 10;
        }
    }
    void inithp(float value){
        hp = value;
    }
    float gethp(){
        return hp;
    }
    void dmghp(float value){
        hp -= value;
    }
    void healplayer(float value){
        hp += value;
    }
    private:
    float hp;
    


};


#endif