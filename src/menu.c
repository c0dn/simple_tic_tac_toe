#include <menu.h>

GameResources load_game_resources(int screen_width, int screen_height) {
    GameResources resources = { 0 };

    // Load audio resources
    resources.background_music = LoadMusicStream("assets/bg_music.mp3");
    resources.fx_click = LoadSound("assets/btn_click.mp3");
    resources.fx_symbol = LoadSound("assets/click_symbol.mp3");
    resources.fx_win = LoadSound("assets/game_win.mp3");
    resources.fx_draw = LoadSound("assets/game_draw.mp3");

    // Load and resize background
    Image background = LoadImage("assets/main1.png");
    ImageResize(&background, screen_width, screen_height);
    resources.main_menu_img = LoadTextureFromImage(background);
    UnloadImage(background);

    // Load and resize instruction images
    Image instructions1 = LoadImage("assets/instructions_1.png");
    ImageResize(&instructions1, 700, 190);
    resources.instructions_1 = LoadTextureFromImage(instructions1);
    UnloadImage(instructions1);

    Image instructions2 = LoadImage("assets/instructions_2.png");
    ImageResize(&instructions2, 700, 190);
    resources.instructions_2 = LoadTextureFromImage(instructions2);
    UnloadImage(instructions2);


    Image music_on = LoadImage("assets/music_on.png");
    resources.music_on = LoadTextureFromImage(music_on);
    UnloadImage(music_on);

    Image music_off = LoadImage("assets/music_off.png");
    resources.music_off = LoadTextureFromImage(music_off);
    UnloadImage(music_off);

    return resources;
}

void unload_game_resources(const GameResources* resources) {
    UnloadMusicStream(resources->background_music);
    UnloadSound(resources->fx_click);
    UnloadSound(resources->fx_symbol);
    UnloadSound(resources->fx_win);
    UnloadSound(resources->fx_draw);
    UnloadTexture(resources->main_menu_img);
    UnloadTexture(resources->instructions_1);
    UnloadTexture(resources->instructions_2);
    UnloadTexture(resources->music_off);
    UnloadTexture(resources->music_on);

}