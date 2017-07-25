#include <iostream>

extern "C" {
# include "lua.h"
# include "lauxlib.h"
}

#include "Matrix.h"

#define N_FUNCTIONS 6

static int l_meta(lua_State * state);
static int l_beginClip(lua_State * state);
static int l_endClip(lua_State * state);
static int l_frame(lua_State * state);
static int l_embed(lua_State * state);
static int l_rootClip(lua_State * state);

struct function_registry {
    const char * name;
    int (*function) (lua_State *);
};

static function_registry regs[N_FUNCTIONS] = {
        {"meta", &l_meta},
        {"beginclip", &l_beginClip},
        {"endclip", &l_endClip},
        {"frame", &l_frame},
        {"embed", &l_embed},
        {"rootclip", &l_rootClip}
};

Matrix matrix;

void parseQp4(const char * filename);
void renderFrames();

int main(int argc, char *argv[]) {

    if( argc < 2 ) {
        std::cout << "Too few arguments!" << std::endl;
        exit(1);
    }

    parseQp4(argv[1]);
    renderFrames();

    return 0;
}

void parseQp4(const char * filename) {
    lua_State * state = luaL_newstate();

    if( luaL_loadfile(state, filename) ) {
        std::cout << "Failed to open or parse file: " << filename << std::endl;
        exit(1);
    }

    for(auto const & reg : regs) {
        std::cout << "Registering function: " << reg.name << std::endl;

        lua_pushcfunction(state, reg.function);
        lua_setglobal(state, reg.name);
    }

    if( lua_pcall(state, 0, 0, 0) ) {
        std::cout << "Failed to run script!" << std::endl;
        exit(1);
    }

    lua_close(state);
}

void renderFrames() {
    matrix.calculateMaxClipDuration();

    std::cout << "Generating empty frames..." << std::endl;
    matrix.generateEmptyFrames();
    std::cout << "Empty frames generated." << std::endl;

    std::cout << "Generating frames..." << std::endl;
    matrix.generateFrames();
    std::cout << "Frames generated." << std::endl;

    std::cout << "Rescaling and saving..." << std::endl;
    matrix.reScaleAndSave(10, "frames/f_");
    std::cout << "Rescaled and saved." << std::endl;

//    ffmpeg -start_number 0 -i frames/f_%d.png -r 50 -s 320x260 -vcodec mpeg4 test.avi
}

static int l_meta(lua_State * state) {
    luaL_checktype(state, 1, LUA_TTABLE);

    std::cout << "Meta" << std::endl;

    lua_getfield(state, 1, "audio");
    auto audio = lua_tostring(state, -1);

    lua_getfield(state, 1, "year");
    auto year = lua_tointeger(state, -1);

    lua_getfield(state, 1, "team");
    auto team = lua_tostring(state, -1);

    lua_getfield(state, 1, "title");
    auto title = lua_tostring(state, -1);

    std::cout << "\taudio: " << audio << std::endl;
    std::cout << "\tyear: " << year << std::endl;
    std::cout << "\tteam: " << team << std::endl;
    std::cout << "\ttitle: " << title << std::endl;

    return 0;
}

static int l_beginClip(lua_State * state) {
    auto width = lua_tointeger(state, 1);
    auto height = lua_tointeger(state, 2);
    auto name = lua_tostring(state, 3);

    std::cout << "beginclip(" << width << ", " << height << ", " << name << ")" << std::endl;

    matrix.clips.emplace_back(name, (unsigned int)width, (unsigned int)height);

    return 0;
}

static int l_endClip(lua_State *) {
    return 0;
}

static int l_frame(lua_State * state) {
    luaL_checktype(state, 1, LUA_TTABLE);
    lua_pushnil(state);

    auto duration = lua_tointeger(state, 2);
    assert(duration > 0);

    Frame frame = Frame((unsigned int)duration);

    while( lua_next(state, 1) != 0 ) {

        lua_typename(state, lua_type(state, -2));
        lua_typename(state, lua_type(state, -1));

        auto pixel = static_cast<unsigned int>(lua_tointeger(state, -1));

        frame.pixels.push_back(pixel);

        lua_pop(state, 1);
    }

    assert(matrix.clips.size() != 0);
    matrix.clips[matrix.clips.size() - 1].frames.push_back(frame);

    return 0;
}

static int l_embed(lua_State * state) {
    luaL_checktype(state, 2, LUA_TTABLE);
    luaL_checktype(state, 3, LUA_TTABLE);

    auto name = lua_tostring(state, 1);

    lua_getfield(state, 2, "0");
    auto x = lua_tointeger(state, -1);

    lua_getfield(state, 3, "0");
    auto y = lua_tointeger(state, -1);

    auto z = lua_tointeger(state, 4);
    auto t = lua_tointeger(state, 5);

    assert(matrix.clips.size() != 0);
    matrix.clips[matrix.clips.size() - 1].embeds.emplace_back(name, (unsigned int)x, (unsigned int)y, (unsigned int)z, (unsigned int)t);

    return 0;
}

static int l_rootClip(lua_State * state) {
    auto name = lua_tostring(state, 1);

    std::cout << "rootclip: " << name << std::endl;

    assert(strlen(name) != 0);
    matrix.setRootClip(name);

    return 1;
}