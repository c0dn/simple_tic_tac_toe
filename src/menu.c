#include <menu.h>
#include <neural.h>
#include <stdlib.h>

/**
 * Loads game resources including audio, textures, and AI models
 *
 * @return GameResources structure
 * @warning Requires proper asset file paths
 * @warning Caller is responsible for unloading resources
 */
GameResources load_game_resources() {
    GameResources resources = { 0 };

    // Load audio resources
    resources.background_music = LoadMusicStream("assets/bg_music.mp3");
    resources.fx_click = LoadSound("assets/btn_click.mp3");
    resources.fx_symbol = LoadSound("assets/click_symbol.mp3");
    resources.fx_win = LoadSound("assets/game_win.mp3");
    resources.fx_draw = LoadSound("assets/game_draw.mp3");

    // Load and resize background
    Image background = LoadImage("assets/main1.png");
    ImageResize(&background, GetScreenWidth(), GetScreenHeight());
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

    NeuralNetwork* neural_network = load_model();
    resources.models = malloc(sizeof(AiModels));
    resources.models->neural_network = neural_network;

    BayesModel* bayes_model = load_naive_bayes();
    resources.models->bayes_model = bayes_model;
    return resources;
}

/**
 * Calculates and updates grid dimensions
 * @param context Pointer to current GameContext to update grid properties
 */
void update_grid_dimensions(GameContext* context) {
    const int screen_width = GetScreenWidth();
    const int screen_height = GetScreenHeight();

    context->grid.grid_size = (float)screen_width < (float)screen_height
                             ? (float)screen_width * 0.6f
                             : (float)screen_height * 0.6f;
    context->grid.cell_size = (int)context->grid.grid_size / 3;
    context->grid.start_x = (int)((float)screen_width - context->grid.grid_size) / 2;
    context->grid.start_y = (int)((float)screen_height - context->grid.grid_size) / 2;
}


/**
 * Releases all game resources
 *
 * @param resources Pointer to GameResources structure to unload
 */
void unload_game_resources(GameResources* resources) {
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
    free(resources->models->neural_network);
    free(resources->models->bayes_model);
    free(resources->models);
    resources->models = NULL;
}
