struct Position {
    float x;
    float y;
};

struct Velocity {
    float dx;
    float dy;
};

struct Health {
    int currentHP;
    int maxHP;
};

struct Input {
    bool up;
    bool down;
    bool left;
    bool right;
    bool attack;
};

struct Damage {
    int damage;
};
